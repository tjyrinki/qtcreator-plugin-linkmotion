uri = Clock
TARGET = clock
APPNAME = app

CONFIG += lm_lib

# Input
SOURCES += \
    clock_plugin.cpp \
    clocklabel.cpp

HEADERS += \
    clock_plugin.h \
    clocklabel.h

DISTFILES = qmldir
