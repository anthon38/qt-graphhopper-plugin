TARGET = qtgeoservices_graphhopper

QT += location-private positioning-private network

HEADERS += \
    qgeocodereplygraphhopper.h \
    qgeocodingmanagerenginegraphhopper.h \
    qgeoroutereplygraphhopper.h \
    qgeoroutingmanagerenginegraphhopper.h \
    qgeoserviceproviderplugingraphhopper.h

SOURCES += \
    qgeocodereplygraphhopper.cpp \
    qgeocodingmanagerenginegraphhopper.cpp \
    qgeoroutereplygraphhopper.cpp \
    qgeoroutingmanagerenginegraphhopper.cpp \
    qgeoserviceproviderplugingraphhopper.cpp

OTHER_FILES += \
    graphhopper_plugin.json

PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryGraphhopper
load(qt_plugin)
