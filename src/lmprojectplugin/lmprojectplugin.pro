####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)


DISTFILES += \
    lmprojectplugin_dependencies.pri \
    lmprojectplugin.json.in \
    lmprojectplugin.mimetypes.xml

HEADERS += \
    linkmotionprojectplugin.h \
    linkmotionprojectplugin_global.h \
    linkmotionprojectmanager.h \
    linkmotionprojectplugin_constants.h \
    linkmotionproject.h \
    linkmotionprojectfile.h \
    linkmotionprojectnode.h \
    linkmotionkitmatcher.h

SOURCES += \
    linkmotionprojectplugin.cpp \
    linkmotionprojectmanager.cpp \
    linkmotionproject.cpp \
    linkmotionprojectfile.cpp \
    linkmotionprojectnode.cpp \
    linkmotionkitmatcher.cpp

RESOURCES += \
    resources.qrc
