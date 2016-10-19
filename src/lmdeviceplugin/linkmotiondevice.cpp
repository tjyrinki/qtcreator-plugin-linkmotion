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
#include "linkmotiondevice.h"
#include "linkmotiondeviceplugin_constants.h"

#include <QCoreApplication>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDevice::LinkMotionDevice() : ProjectExplorer::IDevice::IDevice(Core::Id(Constants::LINKMOTION_DEVICE_TYPE),
                                                                                     IDevice::AutoDetected,
                                                                                     IDevice::Hardware,
                                                                                    Constants::LINKMOTION_DEVICE_ID)
{
    qDebug() << Q_FUNC_INFO;
}


LinkMotionDevice::LinkMotionDevice(const LinkMotionDevice &other)
    : IDevice(other)
{ }

LinkMotionDevice::LinkMotionDevice(const QString &uid)
    : IDevice(Core::Id(Constants::LINKMOTION_DEVICE_TYPE),
                             IDevice::AutoDetected,
                             IDevice::Hardware,
                             Core::Id(Constants::LINKMOTION_DEVICE_ID).withSuffix(uid))
{
    setDisplayName(LinkMotionDevice::name());
    setDeviceState(DeviceConnected);
}


QString LinkMotionDevice::displayType() const
{
    qDebug() << Q_FUNC_INFO;
    return QCoreApplication::translate("LinkMotion::Internal::LinkMotionDevice", "LinkMotion");
}

QString LinkMotionDevice::name()
{
    qDebug() << Q_FUNC_INFO;
    return QCoreApplication::translate("LinkMotion::Internal::LinkMotionDevice", "LinkMotion Device");
}

ProjectExplorer::IDeviceWidget *LinkMotionDevice::createWidget()
{
    qDebug() << Q_FUNC_INFO;
    return 0;
}

ProjectExplorer::DeviceProcessSignalOperation::Ptr LinkMotionDevice::signalOperation() const
{
    qDebug() << Q_FUNC_INFO;
    return ProjectExplorer::DeviceProcessSignalOperation::Ptr();
}

QList<Core::Id> LinkMotionDevice::actionIds() const
{
    qDebug() << Q_FUNC_INFO;
    return QList<Core::Id>();
}

QString LinkMotionDevice::displayNameForActionId(Core::Id actionId) const
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(actionId)
    return QString();
}

void LinkMotionDevice::executeAction(Core::Id actionId, QWidget *parent)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(actionId)
    Q_UNUSED(parent)
}

ProjectExplorer::IDevice::Ptr LinkMotionDevice::clone() const
{
    qDebug() << Q_FUNC_INFO;
    return IDevice::Ptr(new LinkMotionDevice(*this));
}
