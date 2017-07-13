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

#include "lmlocaldeployconfiguration.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/buildsteplist.h>

#include <QTimer>

namespace LmBase {
namespace Internal {

/*!
 * \class LinkMotionLocalDeployConfigurationFactory
 * Creates the required deploy configuration for locally
 * run apps.
 */
LinkMotionLocalDeployConfigurationFactory::LinkMotionLocalDeployConfigurationFactory(QObject *parent)
    :DeployConfigurationFactory(parent)
{

}

QList<Core::Id> LinkMotionLocalDeployConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent) const
{
    QList<Core::Id> ids;
    if (!parent->project()->supportsKit(parent->kit()))
        return ids;

    Core::Id targetDevice = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(parent->kit());
    if(!targetDevice.toString().startsWith(QLatin1String(LmBase::Constants::LM_CONTAINER_DEVICE_TYPE_ID)))
        return ids;

    ids << Core::Id(Constants::LM_LOCAL_DEPLOYCONFIGURATION_ID);

    return ids;
}

QString LinkMotionLocalDeployConfigurationFactory::displayNameForId(Core::Id id) const
{
    if( Core::Id(Constants::LM_LOCAL_DEPLOYCONFIGURATION_ID) == id )
        return tr("Link Motion SDK Local Deployment");

    return QString();
}

bool LinkMotionLocalDeployConfigurationFactory::canCreate(ProjectExplorer::Target *parent, Core::Id id) const
{
    return availableCreationIds(parent).contains(id);
}

ProjectExplorer::DeployConfiguration *LinkMotionLocalDeployConfigurationFactory::create(ProjectExplorer::Target *parent, Core::Id id)
{
    if(!canCreate(parent,id))
        return 0;

    return new LinkMotionLocalDeployConfiguration(parent,id);
}

bool LinkMotionLocalDeployConfigurationFactory::canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::DeployConfiguration *LinkMotionLocalDeployConfigurationFactory::restore(ProjectExplorer::Target *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    Core::Id id = ProjectExplorer::idFromMap(map);

    LinkMotionLocalDeployConfiguration* const dc = new LinkMotionLocalDeployConfiguration(parent, id);
    if (!dc->fromMap(map)) {
        delete dc;
        return 0;
    }
    return dc;
}

bool LinkMotionLocalDeployConfigurationFactory::canClone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product) const
{
    return canCreate(parent, product->id());
}

ProjectExplorer::DeployConfiguration *LinkMotionLocalDeployConfigurationFactory::clone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product)
{
    if (!canClone(parent, product))
        return 0;
    return new LinkMotionLocalDeployConfiguration(parent,qobject_cast<LinkMotionLocalDeployConfiguration *>(product));
}


///
// LinkMotionLocalDeployConfiguration
///
LinkMotionLocalDeployConfiguration::LinkMotionLocalDeployConfiguration(ProjectExplorer::Target *target, const Core::Id id)
    : DeployConfiguration(target, id)
{
    setDefaultDisplayName(tr("Link Motion SDK deploy locally"));
}

LinkMotionLocalDeployConfiguration::LinkMotionLocalDeployConfiguration(ProjectExplorer::Target *target, LinkMotionLocalDeployConfiguration *source)
    : DeployConfiguration(target, source)
{
    cloneSteps(source);
}

} // namespace Internal
} // namespace LmBase
