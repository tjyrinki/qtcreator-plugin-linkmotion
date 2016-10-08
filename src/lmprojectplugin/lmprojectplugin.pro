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
    lmprojectplugin.json.in

HEADERS += \
    linkmotionprojectplugin.h \
    linkmotionprojectplugin_global.h

SOURCES += \
    linkmotionprojectplugin.cpp
