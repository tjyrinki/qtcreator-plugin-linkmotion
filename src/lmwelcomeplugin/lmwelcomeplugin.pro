####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)
QT += qml quickwidgets quick

HEADERS += \
    linkmotionwelcomeglobal.h \
    linkmotionwelcomeplugin.h \
    linkmotionwelcomeplugin_constants.h \
    linkmotionwelcomemode.h

SOURCES += \
    linkmotionwelcomeplugin.cpp \
    linkmotionwelcomemode.cpp

DISTFILES += \
    lmwelcomeplugin_dependencies.pri \
    lmwelcomeplugin.json.in

RESOURCES += \
    resources.qrc
