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

#ifndef QGEOSERVICEPROVIDER_GRAPHHOPPER_H
#define QGEOSERVICEPROVIDER_GRAPHHOPPER_H

#include <QObject>
#include <QGeoServiceProviderFactory>

QT_BEGIN_NAMESPACE

class QGeoServiceProviderFactoryGraphhopper: public QObject, public QGeoServiceProviderFactory
{
    Q_OBJECT
    Q_INTERFACES(QGeoServiceProviderFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/5.0"
                      FILE "graphhopper_plugin.json")

public:
    QGeoCodingManagerEngine *createGeocodingManagerEngine(const QVariantMap &parameters,
                                                          QGeoServiceProvider::Error *error,
                                                          QString *errorString) const;
    QGeoMappingManagerEngine *createMappingManagerEngine(const QVariantMap &parameters,
                                                         QGeoServiceProvider::Error *error,
                                                         QString *errorString) const;
    QGeoRoutingManagerEngine *createRoutingManagerEngine(const QVariantMap &parameters,
                                                         QGeoServiceProvider::Error *error,
                                                         QString *errorString) const;
    QPlaceManagerEngine *createPlaceManagerEngine(const QVariantMap &parameters,
                                                  QGeoServiceProvider::Error *error,
                                                  QString *errorString) const;
};

QT_END_NAMESPACE

#endif 
