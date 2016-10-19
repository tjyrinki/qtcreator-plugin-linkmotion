####################################################################
#
# This file is part of the LinkMotion Run plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)

HEADERS += \
    linkmotionrunplugin.h \
    linkmotionrunplugin_global.h \
    linkmotionrunplugin_constants.h \
    linkmotionruncontrolfactory.h \
    linkmotionruncontrol.h \
    linkmotionrunconfiguration.h \
    linkmotionrunconfigurationfactory.h \
    linkmotiondebugruncontrol.h

SOURCES += \
    linkmotionrunplugin.cpp \
    linkmotionruncontrolfactory.cpp \
    linkmotionruncontrol.cpp \
    linkmotionrunconfiguration.cpp \
    linkmotionrunconfigurationfactory.cpp \
    linkmotiondebugruncontrol.cpp

DISTFILES += \
    lmrunplugin_dependencies.pri \
    lmrunplugin.json.in


