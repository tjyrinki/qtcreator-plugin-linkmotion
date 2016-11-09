TARGET = mediaApp
uri = MediaApp
APPNAME = %ProjectName%

CONFIG += lm_lib

DISTFILES = qmldir plugins.qmltypes

OTHER_FILES += $$system(find . |grep qml$) \
	qmldir \
	plugins.qmltypes

SOURCES += mediaapp_plugin.cpp
HEADERS += mediaapp_plugin.h
RESOURCES += qml.qrc
