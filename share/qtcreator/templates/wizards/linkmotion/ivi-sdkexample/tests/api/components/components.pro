TARGET = tst_components
IMPORTPATH = ../../../src/modules

CONFIG += lm_qmltests

SOURCES += tst_components.cpp

OTHER_FILES += $$system(find . |grep qml$)
