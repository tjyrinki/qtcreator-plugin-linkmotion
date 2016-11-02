/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#include "linkmotionkitplugin.h"
#include "linkmotionqtversionfactory.h"
#include "linkmotionkitconfiguration.h"
#include "linkmotionqtversion.h"
#include <qtsupport/qtversionmanager.h>
#include <QDebug>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionKitPlugin::LinkMotionKitPlugin() {
    qDebug() << Q_FUNC_INFO;
}

LinkMotionKitPlugin::~LinkMotionKitPlugin() {
    qDebug() << Q_FUNC_INFO;
}

bool LinkMotionKitPlugin::initialize(const QStringList &arguments, QString *errorString) {
    qDebug() << Q_FUNC_INFO;
    addAutoReleasedObject(new LinkMotionQtVersionFactory);
    m_linkMotionKitConfiguration = new LinkMotionKitConfiguration(this);

    return true;
}

void LinkMotionKitPlugin::extensionsInitialized() {
    qDebug() << Q_FUNC_INFO;
    m_linkMotionKitConfiguration->initialize();
}

