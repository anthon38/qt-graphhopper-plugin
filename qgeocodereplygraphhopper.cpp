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

#include "qgeocodereplygraphhopper.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>

QT_BEGIN_NAMESPACE

QGeoCodeReplyGraphhopper::QGeoCodeReplyGraphhopper(QNetworkReply *reply, QObject *parent)
:   QGeoCodeReply(parent)
{
    Q_ASSERT(parent);
    if (!reply) {
        setError(UnknownError, QStringLiteral("Null reply"));
        return;
    }

    connect(reply, &QNetworkReply::finished, this, &QGeoCodeReplyGraphhopper::onNetworkReplyFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &QGeoCodeReplyGraphhopper::onNetworkReplyError);

    connect(this, &QGeoCodeReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
}

QGeoCodeReplyGraphhopper::~QGeoCodeReplyGraphhopper()
{
}

void QGeoCodeReplyGraphhopper::onNetworkReplyFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (!document.isObject()) {
        setError(ParseError, tr("Response parse error"));
        return;
    }

    QGeoLocation location;
qDebug()<<document.object();
    const QJsonArray hits = document.object().value(QStringLiteral("hits")).toArray();
    QJsonObject poi = hits.at(0).toObject();
    QJsonObject coordinates = poi.value(QStringLiteral("point")).toObject();
    location.setCoordinate(QGeoCoordinate(coordinates.value(QStringLiteral("lat")).toDouble(),
                                          coordinates.value(QStringLiteral("lng")).toDouble()));
    QVariantMap attributes;
    attributes.insert(QStringLiteral("name"), poi.value(QStringLiteral("name")).toString());
    location.setExtendedAttributes(attributes);

    addLocation(location);

    setFinished(true);
}

void QGeoCodeReplyGraphhopper::onNetworkReplyError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    setError(QGeoCodeReply::CommunicationError, reply->errorString());
}

QT_END_NAMESPACE
