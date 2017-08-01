QT += qml quick
APPNAME = %ProjectName%
CONFIG += c++11
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = ../modules

TARGET = $$APPNAME
target.path = /usr/apps/$$APPNAME/bin
INSTALLS += target

SOURCES += main.cpp

QML_IMPORT_PATH += ../modules

RESOURCES += qml.qrc \
    assets/images/images.qrc
