CONFIG += lm_app
TARGET = %ProjectName%

SOURCES += main.cpp

QML_IMPORT_PATH += ../modules

RESOURCES += qml.qrc \
    assets/images/images.qrc \
    assets/images/indicators.qrc \
    apps/media/media.qrc \
    apps/car/car.qrc \
    apps/settings/settings.qrc
