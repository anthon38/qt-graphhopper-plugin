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

#ifndef QGEOROUTEREPLYGRAPHHOPPER_H
#define QGEOROUTEREPLYGRAPHHOPPER_H

#include <QNetworkReply>
#include <QGeoRouteReply>

QT_BEGIN_NAMESPACE

class QGeoRouteReplyGraphhopper : public QGeoRouteReply
{
    Q_OBJECT

public:
    explicit QGeoRouteReplyGraphhopper(QObject *parent = nullptr);
    QGeoRouteReplyGraphhopper(QNetworkReply *reply, const QGeoRouteRequest &request, QObject *parent = nullptr);
    ~QGeoRouteReplyGraphhopper() override;

    void abort() override;

private Q_SLOTS:
    void onNetworkReplyFinished();
    void onNetworkReplyError(QNetworkReply::NetworkError error);

private:
    QNetworkReply *m_reply;
};

QT_END_NAMESPACE

#endif // QGEOROUTEREPLYGRAPHHOPPER_H

