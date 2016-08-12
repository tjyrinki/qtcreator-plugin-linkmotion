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
QT += qml

HEADERS += \
    welcomeplugin.h \
    linkmotionwelcomepage.h \
    linkmotionwelcomeglobal.h \
    welcomeplugin_constants.h

SOURCES += \
    welcomeplugin.cpp \
    linkmotionwelcomepage.cpp

DISTFILES += \
    lmwelcomeplugin_dependencies.pri \
    LMWelcome.json.in \
    qml/welcome.qml \
    qml/LM_logo_boxed_2.png

RESOURCES += \
    resources.qrc
