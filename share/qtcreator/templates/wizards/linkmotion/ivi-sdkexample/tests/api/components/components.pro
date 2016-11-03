TARGET = tst_components
QML_IMPORT_PATH = ../../../src/modules

CONFIG += lm_qmltests

SOURCES += tst_components.cpp

OTHER_FILES += $$system(find . |grep qml$)
