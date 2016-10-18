TARGET = tst_clock
IMPORTPATH = ../../../src/modules

CONFIG += lm_qmltests

SOURCES += tst_clock.cpp

OTHER_FILES += $$system(find . |grep qml$)
