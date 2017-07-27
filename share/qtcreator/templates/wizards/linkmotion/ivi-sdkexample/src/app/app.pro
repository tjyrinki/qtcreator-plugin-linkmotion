QT += qml quick

CONFIG += c++11
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = ../modules

target.path = /altdata/apps/$${TARGET}/bin
INSTALLS += target

TARGET = %ProjectName%

SOURCES += main.cpp

QML_IMPORT_PATH += ../modules

RESOURCES += qml.qrc \
    assets/images/images.qrc
