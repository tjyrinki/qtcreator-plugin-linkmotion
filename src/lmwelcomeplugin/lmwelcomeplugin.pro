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
QT += qml quickwidgets

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
    qml/welcome.qml \
    qml/LM_logo_boxed_2.png \
    lmwelcomeplugin.json.in

RESOURCES += \
    resources.qrc
