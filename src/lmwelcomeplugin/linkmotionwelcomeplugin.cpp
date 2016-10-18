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

#include "linkmotionwelcomeplugin.h"

#include <QtGui>
#include <QDebug>
#include "linkmotionwelcomemode.h"


using namespace LinkMotion;
using namespace LinkMotion::Internal;

WelcomePlugin::WelcomePlugin()
{
    qDebug() << Q_FUNC_INFO;

}

WelcomePlugin::~WelcomePlugin()
{
    qDebug() << Q_FUNC_INFO;
    if (m_linkMotionWelcomeMode) m_linkMotionWelcomeMode->deleteLater();
}

bool WelcomePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_linkMotionWelcomeMode = new LinkMotionWelcomeMode();

    addAutoReleasedObject(m_linkMotionWelcomeMode);

    return true;
}


void WelcomePlugin::extensionsInitialized() {
    qDebug() << Q_FUNC_INFO;
    Core::ModeManager::activateMode(m_linkMotionWelcomeMode->id());
}
