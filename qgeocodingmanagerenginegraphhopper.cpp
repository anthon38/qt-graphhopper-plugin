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

#include "qgeocodingmanagerenginegraphhopper.h"
#include "qgeocodereplygraphhopper.h"

#include <QGeoCoordinate>
#include <QUrlQuery>

QT_BEGIN_NAMESPACE

QGeoCodingManagerEngineGraphhopper::QGeoCodingManagerEngineGraphhopper(const QVariantMap &parameters,
                                                       QGeoServiceProvider::Error *error,
                                                       QString *errorString)
:   QGeoCodingManagerEngine(parameters), m_networkManager(new QNetworkAccessManager(this))
{
    if (parameters.contains(QStringLiteral("graphhopper.api_key"))) {
        m_accessToken = parameters.value(QStringLiteral("graphhopper.api_key")).toString();
    }

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeoCodingManagerEngineGraphhopper::~QGeoCodingManagerEngineGraphhopper()
{
}

//QGeoCodeReply *QGeoCodingManagerEngineGraphhopper::geocode(const QGeoAddress &address, const QGeoShape &bounds)
//{
//}

//QGeoCodeReply *QGeoCodingManagerEngineGraphhopper::geocode(const QString &address, int limit, int offset, const QGeoShape &bounds)
//{
//}

QGeoCodeReply *QGeoCodingManagerEngineGraphhopper::reverseGeocode(const QGeoCoordinate &coordinate, const QGeoShape &bounds)
{
    Q_UNUSED(bounds)

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("point"), QString::number(coordinate.latitude())+QLatin1Char(',')+QString::number(coordinate.longitude()));
    query.addQueryItem(QStringLiteral("reverse"), QStringLiteral("true"));
    query.addQueryItem(QStringLiteral("limit"), QString::number(1));
    const QString &locale = QLocale::system().name().section(QLatin1Char('_'), 0, 0);
    QStringList localesSupported;
    localesSupported << QStringLiteral("en");
    localesSupported << QStringLiteral("fr");
    localesSupported << QStringLiteral("de");
    localesSupported << QStringLiteral("it");
    if (localesSupported.contains(locale))
        query.addQueryItem(QStringLiteral("locale"), locale);
    query.addQueryItem(QStringLiteral("key"), m_accessToken);

    QUrl url(QStringLiteral("https://graphhopper.com/api/1/geocode"));
    url.setQuery(query);

    QNetworkRequest networkRequest(url);

    QNetworkReply *networkReply = m_networkManager->get(networkRequest);
    QGeoCodeReplyGraphhopper *reply = new QGeoCodeReplyGraphhopper(networkReply, this);

    connect(reply, &QGeoCodeReplyGraphhopper::finished, this, &QGeoCodingManagerEngineGraphhopper::onReplyFinished);
    connect(reply, QOverload<QGeoCodeReply::Error, const QString &>::of(&QGeoCodeReply::error),
            this, &QGeoCodingManagerEngineGraphhopper::onReplyError);

    return reply;
}

void QGeoCodingManagerEngineGraphhopper::onReplyFinished()
{
    QGeoCodeReply *reply = qobject_cast<QGeoCodeReply *>(sender());
    if (reply)
        Q_EMIT(finished(reply));
}

void QGeoCodingManagerEngineGraphhopper::onReplyError(QGeoCodeReply::Error errorCode, const QString &errorString)
{
    QGeoCodeReply *reply = qobject_cast<QGeoCodeReply *>(sender());
    if (reply)
        Q_EMIT(error(reply, errorCode, errorString));
}

QT_END_NAMESPACE
