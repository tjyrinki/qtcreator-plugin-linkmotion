####################################################################
#
# This file is part of the LinkMotion Base plugin.
#
# License: GNU Lesser General Public License v 2.1
# Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
#
# All rights reserved.
# (C) 2017 Link Motion Oy
####################################################################

include(../linkmotion-qtc-plugin.pri)

CONFIG += c++11
QT += network qml quick
QT += quickwidgets

HEADERS += \
    lmbaseplugin_constants.h \
    lmbaseplugin_global.h \
    lmtargettool.h \
    lmtoolchain.h \
    lmqtversion.h \
    lmkitmanager.h \
    settings.h \
    lmbaseplugin.h \
    lmwelcomepage.h \
    lmshared.h \
    processoutputdialog.h \
    lmtargetdialog.h \
    lmsettingstargetwidget.h \
    lmsettingstargetpage.h \
    simplecrypt.h \
    cred_key.h

DEFINES += LMBASE_LIBRARY

SOURCES += \
    lmtargettool.cpp \
    lmtoolchain.cpp \
    lmqtversion.cpp \
    lmkitmanager.cpp \
    settings.cpp \
    lmbaseplugin.cpp \
    lmwelcomepage.cpp \
    processoutputdialog.cpp \
    lmshared.cpp \
    lmtargetdialog.cpp \
    lmsettingstargetwidget.cpp \
    lmsettingstargetpage.cpp \
    simplecrypt.cpp

DISTFILES += \
    lmbaseplugin_dependencies.pri \
    lmbaseplugin.json.in

RESOURCES += \
    resources.qrc

FORMS += \
    processoutputdialog.ui \
    lmsettingstargetwidget.ui

include(wizards/wizards.pri)
include(device/device.pri)


