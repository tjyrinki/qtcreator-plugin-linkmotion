####################################################################
#
# This file is part of the LinkMotion Qt Creator plugins.
#
# License: GNU Lesser General Public License v 2.1
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################
CONFIG += c++11
QT += core

PROVIDER = LinkMotion

contains(CONFIG, BUILDTOHOME) {
  USE_USER_DESTDIR = true
}

isEmpty(QTCREATOR_INSTALL) {
    QTCREATOR_INSTALL = $$(QTC_INSTALL)
    isEmpty(QTCREATOR_INSTALL):QTCREATOR_INSTALL=/
}

isEmpty(QTCREATOR_SOURCES) {
    QTCREATOR_SOURCES = $$(QTC_SOURCE)
    isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/
}

## Where our plugin will be compiled to
isEmpty(IDE_BUILD_TREE) {
    IDE_BUILD_TREE = $$(QTC_BUILD)
    isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=../builddir/$$QTCREATOR_INSTALL
}

QTC_PREFIX=$$(QTC_PREFIX)
isEmpty(QTC_PREFIX):QTC_PREFIX=$$QTCREATOR_INSTALL

IDE_LIBRARY_PATH = $$QTCREATOR_INSTALL/lib/qtcreator

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

INCLUDEPATH += $$PWD

LMSDK_DATA_DIRS = \
    $$PWD/../share/qtcreator/templates \
    $$PWD/../share/qtcreator/linkmotion \

for(data_dir, LMSDK_DATA_DIRS) {
    files = $$files($$data_dir/*, true)
    # Info.plist.in are handled below
    for(file, files):!exists($$file/*): {
        LMSDK_FILES += $$file
    }
}

lmsdk_copy2build.input = LMSDK_FILES
lmsdk_copy2build.output = $$IDE_DATA_PATH/${QMAKE_FUNC_FILE_IN_stripSrcDir}
lmsdk_copy2build.variable_out = PRE_TARGETDEPS
lmsdk_copy2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
lmsdk_copy2build.name = COPY ${QMAKE_FILE_IN}
lmsdk_copy2build.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += lmsdk_copy2build

for(data_dir, LMSDK_DATA_DIRS) {
    eval($${data_dir}.files = $$data_dir)
    eval($${data_dir}.path = $$QTC_PREFIX/share/qtcreator)
    eval($${data_dir}.CONFIG += no_check_exist)
    INSTALLS += $$data_dir
}

