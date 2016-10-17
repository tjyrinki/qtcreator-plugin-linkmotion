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
