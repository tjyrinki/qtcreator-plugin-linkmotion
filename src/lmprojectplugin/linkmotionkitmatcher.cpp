#include "linkmotionkitmatcher.h"

#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectexplorerconstants.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

bool LinkMotionKitMatcher::matches(const ProjectExplorer::Kit *k)
{
    qDebug() << Q_FUNC_INFO;
    if (ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(k) == ProjectExplorer::Constants::DESKTOP_DEVICE_TYPE)
        return true;

    return false;
}
