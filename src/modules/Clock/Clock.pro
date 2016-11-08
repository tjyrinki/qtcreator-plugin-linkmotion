uri = Clock
TARGET = clock
APPNAME = %ProjectName%
QT_PRIVATE += quick-private quickcontrols2-private quicktemplates2-private
CONFIG += lm_lib

# Input
SOURCES += \
    clock_plugin.cpp \
    clocklabel.cpp

HEADERS += \
    clock_plugin.h \
    clocklabel.h

RESOURCES = qml.qrc

DISTFILES = qmldir plugins.qmltypes
OTHER_FILES = qmldir plugins.qmltypes
