/*####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionkitmatcher.h"

#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectexplorerconstants.h>
#include "../lmdeviceplugin/linkmotiondeviceplugin_constants.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

bool LinkMotionKitMatcher::matches(const ProjectExplorer::Kit *k)
{
    qDebug() << Q_FUNC_INFO;
    if (ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(k) == Constants::LINKMOTION_DEVICE_TYPE)
        return true;

    return false;
}
