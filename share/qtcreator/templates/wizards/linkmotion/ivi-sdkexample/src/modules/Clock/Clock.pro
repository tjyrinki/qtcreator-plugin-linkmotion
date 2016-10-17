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

DISTFILES = qmldir plugins.qmltypes
