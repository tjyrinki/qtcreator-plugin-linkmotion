uri = Clock
TARGET = clock
APPNAME = %ProjectName%

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
