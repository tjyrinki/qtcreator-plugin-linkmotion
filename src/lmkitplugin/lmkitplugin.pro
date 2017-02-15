####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)

DISTFILES += \
    lmkitplugin_dependencies.pri \
    lmkitplugin.json.in

HEADERS += \
    linkmotionkitplugin.h \
    linkmotionkitplugin_global.h \
    linkmotionoskitinformation.h \
    linkmotionqtversion.h \
    ../lmdeviceplugin/linkmotiondeviceplugin_constants.h \
    linkmotionkitplugin_constants.h \
    linkmotionqtversionfactory.h \
    linkmotionkitconfiguration.h \
    linkmotiondebuggeritem.h

SOURCES += \
    linkmotionkitplugin.cpp \
    linkmotionoskitinformation.cpp \
    linkmotionqtversion.cpp \
    linkmotionqtversionfactory.cpp \
    linkmotionkitconfiguration.cpp \
    linkmotiondebuggeritem.cpp
