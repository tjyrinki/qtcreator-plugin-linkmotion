TARGET = tst_clock
QML_IMPORT_PATH = ../../../src/modules

CONFIG += lm_qmltests

SOURCES += tst_clock.cpp

OTHER_FILES += $$system(find . |grep qml$)
