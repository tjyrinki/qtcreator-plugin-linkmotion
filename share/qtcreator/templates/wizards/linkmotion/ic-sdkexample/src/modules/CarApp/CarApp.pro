TARGET = carApp
uri = CarApp
APPNAME = %ProjectName%

CONFIG += lm_lib

DISTFILES = qmldir plugins.qmltypes

OTHER_FILES += $$system(find . |grep qml$) \
	qmldir \
	plugins.qmltypes

SOURCES += carapp_plugin.cpp
HEADERS += carapp_plugin.h
RESOURCES += qml.qrc
