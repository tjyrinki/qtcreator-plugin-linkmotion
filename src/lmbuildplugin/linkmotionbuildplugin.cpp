/*####################################################################
#
# This file is part of the LinkMotion Build plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#include "linkmotionbuildplugin.h"

#include <QtGui>
#include "linkmotionbuildstepfactory.h"
#include "linkmotionbuildconfigurationfactory.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

BuildPlugin::BuildPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

BuildPlugin::~BuildPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

bool BuildPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addAutoReleasedObject(new LinkMotionBuildConfigurationFactory);
    addAutoReleasedObject(new LinkMotionBuildStepFactory);

    return true;
}
