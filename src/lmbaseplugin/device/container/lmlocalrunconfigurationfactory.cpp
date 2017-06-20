/*
 * Copyright 2013 - 2016 Canonical Ltd.
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
 * Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#include "lmlocalrunconfigurationfactory.h"
#include <lmbaseplugin/lmtoolchain.h>

//#include <ubuntu/device/remote/ubunturemoterunconfiguration.h>
//#include <ubuntu/device/remote/ubuntudevice.h>

#include <projectexplorer/taskhub.h>
#include <projectexplorer/buildtargetinfo.h>
#include <cmakeprojectmanager/cmakeprojectconstants.h>
#include <qmakeprojectmanager/qmakeprojectmanagerconstants.h>

#include <remotelinux/remotelinuxrunconfiguration.h>
#include <remotelinux/remotelinuxrunconfigurationfactory.h>

#include <projectexplorer/toolchain.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>

using namespace LmBase;
using namespace LmBase::Internal;

enum {
    debug = 0
};


const char *LinkMotionLocalRunConfigurationFactory::IdPrefix = "RemoteLinuxRunConfiguration:LinkMotionLocal:";

QList<Core::Id> LinkMotionLocalRunConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode) const
{
    Q_UNUSED(mode);

    QList<Core::Id> types;

    Core::Id targetDevice = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(parent->kit());
    if(!targetDevice.toString().startsWith(QLatin1String(LmBase::Constants::LM_CONTAINER_DEVICE_TYPE_ID))
            && !targetDevice.toString().startsWith(QLatin1String(LmBase::Constants::LM_DEVICE_TYPE_ID))) {
        if(debug) qDebug()<<"Rejecting device type: "<<targetDevice.toString();
        return types;
    }

    bool isCMake  = parent->project()->id() == CMakeProjectManager::Constants::CMAKEPROJECT_ID;
    bool isQMake  = parent->project()->id() == QmakeProjectManager::Constants::QMAKEPROJECT_ID;

    if (!isCMake &&!isQMake)
        return types;

    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::
            toolChain(parent->kit(), ProjectExplorer::Constants::CXX_LANGUAGE_ID);
    if(tc && tc->typeId() != LmBase::Constants::LM_TARGET_TOOLCHAIN_ID)
        return types;

    if (isCMake || isQMake) {
        const Core::Id base = Core::Id(LinkMotionLocalRunConfigurationFactory::IdPrefix);
        foreach (const ProjectExplorer::BuildTargetInfo &bti, parent->applicationTargets().list)
            types << base.withSuffix(bti.targetName);
    }

    return types;
}

QString LinkMotionLocalRunConfigurationFactory::displayNameForId(const Core::Id id) const
{
    return id.suffixAfter(LinkMotionLocalRunConfigurationFactory::IdPrefix)
            + QLatin1Char(' ') + tr("(on Link Motion Host)");
}

bool LinkMotionLocalRunConfigurationFactory::canCreate(ProjectExplorer::Target *parent,
                                                   const Core::Id id) const
{
    if (!parent)
        return false;

    if (id.toString().startsWith(QLatin1String(LinkMotionLocalRunConfigurationFactory::IdPrefix)))
        return true;
    return false;
}

bool LinkMotionLocalRunConfigurationFactory::canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const {
    if (!parent)
        return false;

    Core::Id id = ProjectExplorer::idFromMap(map);
    return canCreate(parent, id);
}

ProjectExplorer::RunConfiguration *LinkMotionLocalRunConfigurationFactory::doCreate(ProjectExplorer::Target *parent, const Core::Id id) {
    if (!canCreate(parent, id))
        return nullptr;

    if (id.toString().startsWith(QLatin1String(LinkMotionLocalRunConfigurationFactory::IdPrefix))) {
        RemoteLinux::RemoteLinuxRunConfiguration *conf = new RemoteLinux::RemoteLinuxRunConfiguration(parent, id,
                                                            id.suffixAfter(LinkMotionLocalRunConfigurationFactory::IdPrefix));
        conf->setUseAlternateExecutable(true);
        conf->setAlternateRemoteExecutable(conf->localExecutableFilePath());
        return conf;
    }

    return 0;
}

ProjectExplorer::RunConfiguration *LinkMotionLocalRunConfigurationFactory::doRestore(ProjectExplorer::Target *parent, const QVariantMap &map) {
    if (!canRestore(parent, map))
        return nullptr;

    return create(parent,ProjectExplorer::idFromMap(map));
}

bool LinkMotionLocalRunConfigurationFactory::canClone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *product) const {
    return false;
    //return canCreate(parent,product->id());
}

ProjectExplorer::RunConfiguration *LinkMotionLocalRunConfigurationFactory::clone(ProjectExplorer::Target *parent,
                                                                             ProjectExplorer::RunConfiguration *source) {
    /*
    if (!canClone(parent, source))
        return nullptr;

    if(RemoteLinux::RemoteLinuxRunConfiguration *old = static_cast<RemoteLinux::RemoteLinuxRunConfiguration *>(source))
        return new RemoteLinux::RemoteLinuxRunConfiguration(parent, old);
    */
    return nullptr;
}
