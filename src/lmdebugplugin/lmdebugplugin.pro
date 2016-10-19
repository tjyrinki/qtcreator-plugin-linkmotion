####################################################################
#
# This file is part of the LinkMotion Debug plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)

HEADERS += \
    linkmotiondebugplugin.h \
    linkmotiondebugplugin_global.h \
    linkmotiondebugruncontrolfactory.h \
    ../lmrunplugin/linkmotionrunconfiguration.h \
    linkmotiondebugruncontrol.h

SOURCES += \
    linkmotiondebugplugin.cpp \
    linkmotiondebugruncontrolfactory.cpp \
    ../lmrunplugin/linkmotionrunconfiguration.cpp \
    linkmotiondebugruncontrol.cpp

DISTFILES += \
    lmdebugplugin.json.in \
    lmdebugplugin_dependencies.pri
