TARGET = components
uri = Components
APPNAME = app

CONFIG += lm_lib

DISTFILES = qmldir

OTHER_FILES += $$system(find . |grep qml$) \
	qmldir

SOURCES += components_plugin.cpp
HEADERS += components_plugin.h
RESOURCES += qml.qrc
