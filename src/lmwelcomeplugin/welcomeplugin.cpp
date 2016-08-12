/*####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#include "welcomeplugin.h"

#include <QtGui>
#include "linkmotionwelcomepage.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

WelcomePlugin::WelcomePlugin()
{

}

WelcomePlugin::~WelcomePlugin()
{

}

bool WelcomePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QFont  defaultFont = QGuiApplication::font();
    defaultFont.setFamily(QStringLiteral("Arial"));
    defaultFont.setWeight(QFont::Light);
    QGuiApplication::setFont(defaultFont);

    addAutoReleasedObject(new LinkMotionWelcomePage);

    return true;
}
