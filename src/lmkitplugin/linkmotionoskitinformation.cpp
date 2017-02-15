/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
#
# All rights reserved.
# (C) 2017 Link Motion Oy
####################################################################*/

#include "linkmotionoskitinformation.h"

#include "../lmdeviceplugin/linkmotiondeviceplugin_constants.h"

#include <projectexplorer/kitinformation.h>
#include <projectexplorer/task.h>
#include <utils/environment.h>

#include <QVariant>

namespace LinkMotion {
namespace Internal {

LinkMotionOsKitInformation::LinkMotionOsKitInformation()
{
    setId(LinkMotionOsKitInformation::id());
}

QVariant LinkMotionOsKitInformation::defaultValue(const ProjectExplorer::Kit *k) const
{
    Q_UNUSED(k)

    return QString();
}

QList<ProjectExplorer::Task> LinkMotionOsKitInformation::validate(const ProjectExplorer::Kit *k) const
{
    Q_UNUSED(k)

    return QList<ProjectExplorer::Task>();
}

ProjectExplorer::KitConfigWidget *LinkMotionOsKitInformation::createConfigWidget(ProjectExplorer::Kit *k) const
{
    Q_UNUSED(k)

    return nullptr;
}

ProjectExplorer::KitInformation::ItemList LinkMotionOsKitInformation::toUserOutput(const ProjectExplorer::Kit *k) const
{
    return ItemList({{ QStringLiteral("LINKMOTION_OS"), os(k) }});
}

void LinkMotionOsKitInformation::addToEnvironment(const ProjectExplorer::Kit *k, Utils::Environment &env) const
{
    Q_UNUSED(k)

    env.set(QStringLiteral("LINKMOTION_OS"), os(k));
}

Core::Id LinkMotionOsKitInformation::id()
{
    return "LinkMotion.KitInformation";
}

QString LinkMotionOsKitInformation::os(const ProjectExplorer::Kit *k)
{
    auto device = ProjectExplorer::DeviceKitInformation::device(k);

    if (device->id() == Constants::LINKMOTION_AUTOOS_DEVICE_ID) {
        return QStringLiteral("auto");
    }
    if (device->id() == Constants::LINKMOTION_IVIOS_DEVICE_ID) {
        return QStringLiteral("ivi");
    }

    return QString();
}

} // namespace Internal
} // namespace LinkMotion
