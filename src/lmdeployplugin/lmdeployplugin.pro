####################################################################
#
# This file is part of the LinkMotion Build plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)

HEADERS += \
    linkmotiondeployplugin.h \
    linkmotiondeployplugin_global.h \
    linkmotiondeployplugin_constants.h \
    linkmotiondeploystepfactory.h \
    linkmotiondeploystep.h \
    linkmotiondeployconfiguration.h \
    linkmotiondeployconfigurationfactory.h \
    linkmotiondeploysettingswidget.h \
    linkmotiondeploystepconfigwidget.h

SOURCES += \
    linkmotiondeployplugin.cpp \
    linkmotiondeploystepfactory.cpp \
    linkmotiondeploystep.cpp \
    linkmotiondeployconfiguration.cpp \
    linkmotiondeployconfigurationfactory.cpp \
    linkmotiondeploysettingswidget.cpp \
    linkmotiondeploystepconfigwidget.cpp

DISTFILES += \
    lmdeployplugin_dependencies.pri \
    lmdeployplugin.json.in


