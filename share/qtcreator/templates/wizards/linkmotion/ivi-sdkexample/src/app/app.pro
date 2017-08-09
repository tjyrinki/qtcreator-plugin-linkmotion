QT += qml quick
APPNAME = %ProjectName%
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += libsystemd

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = ../modules

TARGET = $$APPNAME
target.path = /usr/apps/$$APPNAME/bin
INSTALLS += target

SOURCES += main.cpp \
    servicenotifier.cpp

HEADERS += servicenotifier.h

QML_IMPORT_PATH += ../modules

RESOURCES += qml.qrc \
    assets/images/images.qrc
