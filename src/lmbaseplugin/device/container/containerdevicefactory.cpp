/*
 * Copyright 2016 Canonical Ltd.
 * Copyright 2017 Link Motion Oy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#include "containerdevicefactory.h"
#include "containerdevice.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>
#include <lmbaseplugin/lmtargettool.h>

#include <utils/hostosinfo.h>
#include <utils/qtcassert.h>

#include <QProcess>

namespace LmBase {
namespace Internal {

ContainerDeviceFactory::ContainerDeviceFactory(QObject *parent) : IDeviceFactory(parent)
{ }

QString ContainerDeviceFactory::displayNameForId(Core::Id type) const
{
    if (type.toString().startsWith(QLatin1String(Constants::LM_CONTAINER_DEVICE_TYPE_ID))) {
        return QStringLiteral("Link Motion Local Device (")
                + type.suffixAfter(Constants::LM_CONTAINER_DEVICE_TYPE_ID)
                + QStringLiteral(")");
    }
    return QString();
}

QList<Core::Id> ContainerDeviceFactory::availableCreationIds() const
{
    QList<Core::Id> deviceIds;
    QList<LinkMotionTargetTool::Target> targets = LinkMotionTargetTool::listPossibleDeviceContainers();

    foreach(const LinkMotionTargetTool::Target &t, targets) {
        deviceIds.append(ContainerDevice::createIdForContainer(t.containerName));
    }

    return deviceIds;
}

ProjectExplorer::IDevice::Ptr ContainerDeviceFactory::create(Core::Id id) const
{
    Q_UNUSED(id);
    return ProjectExplorer::IDevice::Ptr();
}

bool ContainerDeviceFactory::canRestore(const QVariantMap &map) const
{
    //we can only restore devices that have a existing container
    return availableCreationIds().contains(ProjectExplorer::IDevice::idFromMap(map));
}

ProjectExplorer::IDevice::Ptr ContainerDeviceFactory::restore(const QVariantMap &map) const
{
    QTC_ASSERT(canRestore(map), return ProjectExplorer::IDevice::Ptr());

    Core::Id typeId = ContainerDevice::typeFromMap(map);
    Core::Id devId  = ContainerDevice::idFromMap(map);

    if (!typeId.isValid() || !devId.isValid())
        return ProjectExplorer::IDevice::Ptr();

    const ProjectExplorer::IDevice::Ptr device
            = ContainerDevice::create(typeId, devId);
    device->fromMap(map);
    return device;
}

bool ContainerDeviceFactory::canCreate() const
{
    return false;
}

QIcon ContainerDeviceFactory::iconForId(Core::Id) const
{
    return QIcon(QLatin1String(Constants::LM_LOGO_ROUND));
}

} // namespace Internal
} // namespace LmBase

