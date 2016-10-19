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
#include "linkmotiondevicefactory.h"

#include "linkmotiondeviceplugin_constants.h"
#include "linkmotiondevice.h"

using namespace LinkMotion;
using namespace Internal;

LinkMotionDeviceFactory::LinkMotionDeviceFactory()
{
    setObjectName(QLatin1String("LinkMotionDeviceFactory"));
}

QString LinkMotionDeviceFactory::displayNameForId(Core::Id type) const
{
    return type == Constants::LINKMOTION_DEVICE_TYPE ? LinkMotionDevice::name() : QString();
}

QList<Core::Id> LinkMotionDeviceFactory::availableCreationIds() const
{
    return QList<Core::Id>() << Core::Id(Constants::LINKMOTION_DEVICE_TYPE);
}

bool LinkMotionDeviceFactory::canCreate() const
{
    return false;
}

ProjectExplorer::IDevice::Ptr LinkMotionDeviceFactory::create(Core::Id id) const
{
    Q_UNUSED(id)
    return ProjectExplorer::IDevice::Ptr();
}

bool LinkMotionDeviceFactory::canRestore(const QVariantMap &map) const
{
    if (ProjectExplorer::IDevice::typeFromMap(map) != Constants::LINKMOTION_DEVICE_TYPE)
        return false;
    QVariantMap vMap = map.value(QLatin1String(Constants::EXTRA_INFO_KEY)).toMap();
    if (vMap.isEmpty()
            || vMap.value(QLatin1String("deviceName")).toString() == QLatin1String("*unknown*"))
        return false; // transient device (probably generated during an activation)
    return true;
}

ProjectExplorer::IDevice::Ptr LinkMotionDeviceFactory::restore(const QVariantMap &map) const
{
    LinkMotionDevice *newDev = new LinkMotionDevice;
    newDev->fromMap(map);
    return ProjectExplorer::IDevice::Ptr(newDev);
}
