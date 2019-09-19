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

#ifndef QGEOROUTINGMANAGERENGINEGRAPHHOPPER_H
#define QGEOROUTINGMANAGERENGINEGRAPHHOPPER_H

#include <QGeoServiceProvider>
#include <QGeoRoutingManagerEngine>

QT_BEGIN_NAMESPACE

class QNetworkAccessManager;

class QGeoRoutingManagerEngineGraphhopper : public QGeoRoutingManagerEngine
{
    Q_OBJECT

public:
    QGeoRoutingManagerEngineGraphhopper(const QVariantMap &parameters,
                                QGeoServiceProvider::Error *error,
                                QString *errorString);
    ~QGeoRoutingManagerEngineGraphhopper();

    QGeoRouteReply *calculateRoute(const QGeoRouteRequest &request);

private Q_SLOTS:
    void onReplyFinished();
    void onReplyError(QGeoRouteReply::Error errorCode, const QString &errorString);

private:
    QNetworkAccessManager *m_networkManager;
    QString m_accessToken;
};

QT_END_NAMESPACE

#endif // QGEOROUTINGMANAGERENGINEGRAPHHOPPER_H

