####################################################################
#
# This file is part of the LinkMotion Qt Creator plugins.
#
# License: Proprietary
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

QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/usr/src/qtcreator

## Where our plugin will be compiled to
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=../builddir

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

INCLUDEPATH += $$QTCREATOR_SOURCES/src/
LIBS += -L$$[QT_INSTALL_LIBS]/qtcreator
LIBS += -L$$[QT_INSTALL_LIBS]/qtcreator/plugins
