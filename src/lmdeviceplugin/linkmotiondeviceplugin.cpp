/*####################################################################
#
# This file is part of the LinkMotion Device plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#include "linkmotiondeviceplugin.h"
#include "linkmotiondevicefactory.h"
#include "linkmotionconfiguration.h"
#include <QtGui>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDevicePlugin::LinkMotionDevicePlugin()
{
    qDebug() << Q_FUNC_INFO;

}

LinkMotionDevicePlugin::~LinkMotionDevicePlugin()
{
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionDevicePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addAutoReleasedObject(new LinkMotionDeviceFactory);
    Internal::LinkMotionConfiguration::initialize();
    return true;
}
