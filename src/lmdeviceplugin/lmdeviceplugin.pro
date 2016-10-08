####################################################################
#
# This file is part of the LinkMotion Device plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)


DISTFILES += \
    lmdeviceplugin_dependencies.pri \
    lmdeviceplugin.json.in

HEADERS += \
    linkmotiondeviceplugin.h \
    linkmotiondeviceplugin_global.h \
    linkmotiondevicefactory.h \
    linkmotiondeviceplugin_constants.h \
    linkmotiondevice.h \
    linkmotionconfiguration.h

SOURCES += \
    linkmotiondeviceplugin.cpp \
    linkmotiondevicefactory.cpp \
    linkmotiondevice.cpp \
    linkmotionconfiguration.cpp
