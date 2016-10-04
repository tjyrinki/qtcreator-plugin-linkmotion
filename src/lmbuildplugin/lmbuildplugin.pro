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
    buildplugin.h \
    buildplugin_constants.h \
    linkmotionbuildconfiguration.h \
    linkmotionbuildsettingswidget.h \
    buildplugin_global.h \
    linkmotionbuildconfigurationfactory.h \
    linkmotionbuildstep.h

SOURCES += \
    buildplugin.cpp \
    linkmotionbuildconfiguration.cpp \
    linkmotionbuildsettingswidget.cpp \
    linkmotionbuildconfigurationfactory.cpp \
    linkmotionbuildstep.cpp

DISTFILES += \
    lmbuildplugin_dependencies.pri \
    LMBuild.json.in


