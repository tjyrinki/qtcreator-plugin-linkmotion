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
    lmwizardplugin.json.in \
    lmwizardplugin_dependencies.pri

HEADERS += \
    linkmotionwizardplugin.h \
    linkmotionwizardplugin_global.h

SOURCES += \
    linkmotionwizardplugin.cpp
