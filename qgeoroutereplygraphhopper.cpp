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

#include "qgeoroutereplygraphhopper.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE

static QList<QGeoCoordinate> decodePolyline(const QString &line)
{
        QList<QGeoCoordinate> path;
        QByteArray data(line.toLocal8Bit());

        bool is3D = true;
        int index = 0;
        int len = data.length();
        int lat = 0, lng = 0, ele = 0;
        while (index < len) {
            // latitude
            int b, shift = 0, result = 0;
            do {
                b = data.at(index++) - 63;
                result |= (b & 0x1f) << shift;
                shift += 5;
            } while (b >= 0x20);
            int deltaLatitude = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
            lat += deltaLatitude;

            // longitute
            shift = 0;
            result = 0;
            do {
                b = data.at(index++) - 63;
                result |= (b & 0x1f) << shift;
                shift += 5;
            } while (b >= 0x20);
            int deltaLongitude = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
            lng += deltaLongitude;

            if (is3D) {
                // elevation
                shift = 0;
                result = 0;
                do {
                    b = data.at(index++) - 63;
                    result |= (b & 0x1f) << shift;
                    shift += 5;
                } while (b >= 0x20);
                int deltaElevation = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
                ele += deltaElevation;
                path.append(QGeoCoordinate(static_cast<double>(lat) / 1e5, static_cast<double>(lng) / 1e5, static_cast<double>(ele) / 100.0));
            } else
                path.append(QGeoCoordinate(static_cast<double>(lat) / 1e5, static_cast<double>(lng) / 1e5));
        }
        return path;
    }

static QList<QGeoCoordinate> parseGeometry(const QJsonValue &geometry)
{
    QList<QGeoCoordinate> path;
    if (geometry.isString()) path = decodePolyline(geometry.toString());
//    if (geometry.isObject()) {
//        QJsonArray coords = geometry.toObject().value(QStringLiteral("coordinates")).toArray();
//        for (int i = 0; i < coords.count(); i++) {
//            QJsonArray coord = coords.at(i).toArray();
//            if (coord.count() != 2) continue;
//            path.append(QGeoCoordinate(coord.at(1).toDouble(), coord.at(0).toDouble()));
//        }
//    }
    return path;
}

QGeoRouteReplyGraphhopper::QGeoRouteReplyGraphhopper(QNetworkReply *reply, const QGeoRouteRequest &request,
                                     QObject *parent)
:   QGeoRouteReply(request, parent), m_reply(reply)
{
    connect(m_reply, &QNetworkReply::finished, this, &QGeoRouteReplyGraphhopper::onNetworkReplyFinished);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &QGeoRouteReplyGraphhopper::onNetworkReplyError);
}

QGeoRouteReplyGraphhopper::~QGeoRouteReplyGraphhopper()
{
    if (m_reply)
        m_reply->deleteLater();
}

void QGeoRouteReplyGraphhopper::abort()
{
    if (!m_reply)
        return;

    m_reply->abort();

    m_reply->deleteLater();
    m_reply = nullptr;
}

static QGeoRoute constructRoute(const QJsonObject &obj)
{
    QGeoRoute route;
    route.setDistance(obj.value(QStringLiteral("distance")).toDouble());
    route.setTravelTime(obj.value(QStringLiteral("time")).toInt()/1000);

    QVariantMap attributes;
    attributes["ascent"] = obj.value(QStringLiteral("ascend"));
    attributes["descent"] = obj.value(QStringLiteral("descend"));
    route.setExtendedAttributes(attributes);

    QList<QGeoCoordinate> path = parseGeometry(obj.value(QStringLiteral("points")));
    route.setPath(path);

    return route;
}

void QGeoRouteReplyGraphhopper::onNetworkReplyFinished()
{
    if (!m_reply)
        return;

    if (m_reply->error() != QNetworkReply::NoError) {
        setError(QGeoRouteReply::CommunicationError, m_reply->errorString());
        m_reply->deleteLater();
        m_reply = nullptr;
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll());
    if (document.isObject()) {
        QList<QGeoRoute> list;
        QJsonArray routes = document.object().value(QStringLiteral("paths")).toArray();
        for (int i = 0; i < routes.count(); i++) {
            QGeoRoute route = constructRoute(routes.at(i).toObject());
            list.append(route);
        }
        setRoutes(list);
        setFinished(true);
    } else {
        setError(QGeoRouteReply::ParseError, "Couldn't parse json.");
    }

    m_reply->deleteLater();
    m_reply = nullptr;
}

void QGeoRouteReplyGraphhopper::onNetworkReplyError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)

    if (!m_reply)
        return;

    setError(QGeoRouteReply::CommunicationError, m_reply->errorString());

    m_reply->deleteLater();
    m_reply = nullptr;
}

QT_END_NAMESPACE
