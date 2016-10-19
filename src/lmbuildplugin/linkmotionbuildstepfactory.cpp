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
#include "linkmotionbuildstepfactory.h"

#include <utils/qtcassert.h>
#include <coreplugin/id.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/target.h>
#include "linkmotionbuildstep.h"
#include "linkmotionbuildplugin_constants.h"

#include <projectexplorer/kitinformation.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;
using namespace LinkMotion::Internal::Constants;


LinkMotionBuildStepFactory::LinkMotionBuildStepFactory(QObject *parent) :
    IBuildStepFactory(parent)
{
}

bool LinkMotionBuildStepFactory::canCreate(ProjectExplorer::BuildStepList *parent, const Core::Id id) const
{
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_CLEAN
            && parent->id() != ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return false;
    ProjectExplorer::Kit *kit = parent->target()->kit();
    Core::Id deviceType = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(kit);
    return ((deviceType == Constants::LINKMOTION_DEVICE_TYPE
            || deviceType == Constants::LINKMOTION_SIMULATOR_TYPE)
            && id == LINKMOTION_BUILD_STEP_ID);
}

ProjectExplorer::BuildStep *LinkMotionBuildStepFactory::create(ProjectExplorer::BuildStepList *parent, const Core::Id id)
{
    if (!canCreate(parent, id))
        return 0;
    LinkMotionBuildStep *step = new LinkMotionBuildStep(parent);
    if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_CLEAN) {
        step->setClean(true);
        step->setExtraArguments(QStringList(QLatin1String("clean")));
    } else if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_BUILD) {
        // nomal setup
    }
    return step;
}

bool LinkMotionBuildStepFactory::canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *source) const
{
    return canCreate(parent, source->id());
}

ProjectExplorer::BuildStep *LinkMotionBuildStepFactory::clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *source)
{
    if (!canClone(parent, source))
        return 0;
    LinkMotionBuildStep *old(qobject_cast<LinkMotionBuildStep *>(source));
    Q_ASSERT(old);
    return new LinkMotionBuildStep(parent, old);
}

bool LinkMotionBuildStepFactory::canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep *LinkMotionBuildStepFactory::restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    LinkMotionBuildStep *bs(new LinkMotionBuildStep(parent));
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return 0;
}

QList<Core::Id> LinkMotionBuildStepFactory::availableCreationIds(ProjectExplorer::BuildStepList *parent) const
{
    ProjectExplorer::Kit *kit = parent->target()->kit();
    Core::Id deviceType = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(kit);
    if (deviceType == Constants::LINKMOTION_DEVICE_TYPE
            || deviceType == Constants::LINKMOTION_SIMULATOR_TYPE)
        return QList<Core::Id>() << Core::Id(LINKMOTION_BUILD_STEP_ID);
    return QList<Core::Id>();
}

QString LinkMotionBuildStepFactory::displayNameForId(const Core::Id id) const
{
    if (id == LINKMOTION_BUILD_STEP_ID)
        return QCoreApplication::translate("LinkMotion::Internal::LinkMotionBuildStep",
                                           LINKMOTION_BUILD_STEP_DISPLAY_NAME);
    return QString();
}
