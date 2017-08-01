TARGET = components
uri = Components
APPNAME = %ProjectName%

TEMPLATE = lib
QT += qml quick quickcontrols2
#QT_PRIVATE += quick-private quickcontrols2-private quicktemplates2-private
CONFIG += qt plugin c++11

TARGET.path = /usr/apps/$$APPNAME/modules/$$LIBNAME/
TARGET = $$qtLibraryTarget($$TARGET)

OTHER_FILES += qmldir

!linux-g++-armv7l {
        OTHER_FILES += $$OUT_PWD/plugins.qmltypes
        DISTFILES = $$OUT_PWD/plugins.qmltypes

        ###########################
        # update plugins.qmltypes
        QMAKE_POST_LINK += cd $$OUT_PWD/.. && LD_LIBRARY_PATH=$$[QT_INSTALL_LIBS] $$[QT_INSTALL_BINS]/qmlplugindump $${uri} 1.0 . > $${uri}/plugins.qmltypes
} else {
    DISTFILES = plugins.qmltypes
    OTHER_FILES = plugins.qmltypes
}

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
installPath = /usr/apps/$$APPNAME/modules/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir

DISTFILES = qmldir

OTHER_FILES += $$system(find . |grep qml$) \
	qmldir \
	plugins.qmltypes

SOURCES += components_plugin.cpp
HEADERS += components_plugin.h
RESOURCES += qml.qrc
