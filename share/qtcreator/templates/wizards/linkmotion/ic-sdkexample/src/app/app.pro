CONFIG += lm_app
TARGET = %ProjectName%

SOURCES += main.cpp

QML_IMPORT_PATH += ../modules

RESOURCES += qml.qrc \
    assets/images/images.qrc \
    assets/images/indicators.qrc
