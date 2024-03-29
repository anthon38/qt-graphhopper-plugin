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

#ifndef QGEOCODEREPLYMAPBOXGRAPHHOPPER_H
#define QGEOCODEREPLYMAPBOXGRAPHHOPPER_H

#include <QNetworkReply>
#include <QGeoCodeReply>

QT_BEGIN_NAMESPACE

class QGeoCodeReplyGraphhopper : public QGeoCodeReply
{
    Q_OBJECT

public:
    explicit QGeoCodeReplyGraphhopper(QNetworkReply *reply, QObject *parent = nullptr);
    ~QGeoCodeReplyGraphhopper();

private Q_SLOTS:
    void onNetworkReplyFinished();
    void onNetworkReplyError(QNetworkReply::NetworkError error);
};

QT_END_NAMESPACE

#endif // QGEOCODEREPLYMAPBOXGRAPHHOPPER_H
