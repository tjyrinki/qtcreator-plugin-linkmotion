/*####################################################################
#
# This file is part of the LinkMotion Deploy plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#include "linkmotiondeployplugin.h"

#include <QtGui>
#include "linkmotiondeploystepfactory.h"
#include "linkmotiondeployconfigurationfactory.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

DeployPlugin::DeployPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

DeployPlugin::~DeployPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

bool DeployPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addAutoReleasedObject(new LinkMotionDeployConfigurationFactory);
    addAutoReleasedObject(new LinkMotionDeployStepFactory);

    return true;
}
