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
    linkmotionbuildconfiguration.h \
    linkmotionbuildsettingswidget.h \
    linkmotionbuildconfigurationfactory.h \
    linkmotionbuildstep.h \
    linkmotionbuildstepconfigwidget.h \
    linkmotionbuildstepfactory.h \
    linkmotionbuildplugin.h \
    linkmotionbuildplugin_global.h \
    linkmotionbuildplugin_constants.h

SOURCES += \
    linkmotionbuildconfiguration.cpp \
    linkmotionbuildsettingswidget.cpp \
    linkmotionbuildconfigurationfactory.cpp \
    linkmotionbuildstep.cpp \
    linkmotionbuildstepconfigwidget.cpp \
    linkmotionbuildstepfactory.cpp \
    linkmotionbuildplugin.cpp

DISTFILES += \
    lmbuildplugin_dependencies.pri \
    lmbuildplugin.json.in


