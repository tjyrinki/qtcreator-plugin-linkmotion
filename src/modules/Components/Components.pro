TARGET = components
uri = Components
APPNAME = %ProjectName%

CONFIG += lm_lib

DISTFILES = qmldir plugins.qmltypes

OTHER_FILES += $$system(find . |grep qml$) \
	qmldir \
	plugins.qmltypes

SOURCES += components_plugin.cpp
HEADERS += components_plugin.h
RESOURCES += qml.qrc
