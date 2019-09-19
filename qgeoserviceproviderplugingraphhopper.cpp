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

#include "qgeoserviceproviderplugingraphhopper.h"
#include "qgeocodingmanagerenginegraphhopper.h"
#include "qgeoroutingmanagerenginegraphhopper.h"

QT_BEGIN_NAMESPACE

static inline QString msgAccessTokenParameter()
{
    return QGeoServiceProviderFactoryGraphhopper::tr("Graphhopper plugin requires a 'graphhopper.api_key' parameter.\n"
                                                "Please visit https://www.graphhopper.com");
}

QGeoCodingManagerEngine *QGeoServiceProviderFactoryGraphhopper::createGeocodingManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{
    const QString accessToken = parameters.value(QStringLiteral("graphhopper.api_key")).toString();

    if (!accessToken.isEmpty()) {
        return new QGeoCodingManagerEngineGraphhopper(parameters, error, errorString);
    } else {
        *error = QGeoServiceProvider::MissingRequiredParameterError;
        *errorString = msgAccessTokenParameter();
        return nullptr;
    }
}

QGeoMappingManagerEngine *QGeoServiceProviderFactoryGraphhopper::createMappingManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return nullptr;
}

QGeoRoutingManagerEngine *QGeoServiceProviderFactoryGraphhopper::createRoutingManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{
     const QString accessToken = parameters.value(QStringLiteral("graphhopper.api_key")).toString();

     if (!accessToken.isEmpty()) {
         return new QGeoRoutingManagerEngineGraphhopper(parameters, error, errorString);
     } else {
         *error = QGeoServiceProvider::MissingRequiredParameterError;
         *errorString = msgAccessTokenParameter();
         return nullptr;
     }
}

QPlaceManagerEngine *QGeoServiceProviderFactoryGraphhopper::createPlaceManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return nullptr;
}

QT_END_NAMESPACE
