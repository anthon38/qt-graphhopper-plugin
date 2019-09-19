/************************************************************************************
 *  Copyright (c) 2019 Anthony Vital <anthony.vital@gmail.com>                      *
 *                                                                                  *
 *  This file is part of qt-graphhopper-plugin.                                     *
 *                                                                                  *
 *  qt-graphhopper-plugin is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by            *
 *  the Free Software Foundation, either version 3 of the License, or               *
 *  (at your option) any later version.                                             *
 *                                                                                  *
 *  qt-graphhopper-plugin is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                  *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   *
 *  GNU General Public License for more details.                                    *
 *                                                                                  *
 *  You should have received a copy of the GNU General Public License               *
 *  along with qt-graphhopper-plugin. If not, see <http://www.gnu.org/licenses/>.   *
 ************************************************************************************/

#include "qgeoroutingmanagerenginegraphhopper.h"
#include "qgeoroutereplygraphhopper.h"

#include <QNetworkAccessManager>
#include <QUrlQuery>

QT_BEGIN_NAMESPACE

QGeoRoutingManagerEngineGraphhopper::QGeoRoutingManagerEngineGraphhopper(const QVariantMap &parameters,
                                                         QGeoServiceProvider::Error *error,
                                                         QString *errorString)
    : QGeoRoutingManagerEngine(parameters),
      m_networkManager(new QNetworkAccessManager(this))
{
    if (parameters.contains(QStringLiteral("graphhopper.api_key"))) {
        m_accessToken = parameters.value(QStringLiteral("graphhopper.api_key")).toString();
    }

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeoRoutingManagerEngineGraphhopper::~QGeoRoutingManagerEngineGraphhopper()
{
}

QGeoRouteReply* QGeoRoutingManagerEngineGraphhopper::calculateRoute(const QGeoRouteRequest &request)
{
    QUrlQuery query;
    for (const auto &wp : request.waypoints()) {
        query.addQueryItem(QStringLiteral("point"), QString("%1,%2").arg(wp.latitude()).arg(wp.longitude()));
    }
    QGeoRouteRequest::TravelModes travelModes = request.travelModes();
    if (travelModes.testFlag(QGeoRouteRequest::PedestrianTravel)) {
        query.addQueryItem(QStringLiteral("vehicle"), QStringLiteral("foot"));
    } else if (travelModes.testFlag(QGeoRouteRequest::BicycleTravel)) {
        query.addQueryItem(QStringLiteral("vehicle"), QStringLiteral("bike"));
    } else if (travelModes.testFlag(QGeoRouteRequest::CarTravel)) {
        query.addQueryItem(QStringLiteral("vehicle"), QStringLiteral("car"));
    }
    query.addQueryItem(QStringLiteral("instructions"), QStringLiteral("false"));
    query.addQueryItem(QStringLiteral("elevation"), QStringLiteral("true"));
    query.addQueryItem(QStringLiteral("key"), m_accessToken);

    QUrl url(QStringLiteral("https://graphhopper.com/api/1/route?"));
    url.setQuery(query);

    QNetworkRequest networkRequest(url);

    QNetworkReply *reply = m_networkManager->get(networkRequest);
    QGeoRouteReplyGraphhopper *routeReply = new QGeoRouteReplyGraphhopper(reply, request, this);

    connect(routeReply, &QGeoRouteReplyGraphhopper::finished, this, &QGeoRoutingManagerEngineGraphhopper::onReplyFinished);
    connect(routeReply, QOverload<QGeoRouteReply::Error, const QString &>::of(&QGeoRouteReply::error),
            this, &QGeoRoutingManagerEngineGraphhopper::onReplyError);

    return routeReply;
}

void QGeoRoutingManagerEngineGraphhopper::onReplyFinished()
{
    QGeoRouteReply *reply = qobject_cast<QGeoRouteReply *>(sender());
    if (reply)
        Q_EMIT(finished(reply));
}

void QGeoRoutingManagerEngineGraphhopper::onReplyError(QGeoRouteReply::Error errorCode,
                                             const QString &errorString)
{
    QGeoRouteReply *reply = qobject_cast<QGeoRouteReply *>(sender());
    if (reply)
        Q_EMIT(error(reply, errorCode, errorString));
}

QT_END_NAMESPACE
