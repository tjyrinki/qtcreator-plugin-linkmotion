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
#include <QDebug>

using namespace LinkMotion;
using namespace LinkMotion::Internal;
using namespace LinkMotion::Internal::Constants;


LinkMotionBuildStepFactory::LinkMotionBuildStepFactory(QObject *parent) :
    IBuildStepFactory(parent)
{
    qDebug() << Q_FUNC_INFO;
}

bool LinkMotionBuildStepFactory::canCreate(ProjectExplorer::BuildStepList *parent, const Core::Id id) const
{
    qDebug() << Q_FUNC_INFO;
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_CLEAN
            && parent->id() != ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return false;

    if (!parent->target()) return false;
    if (!parent->target()->kit()) return false;

    ProjectExplorer::Kit *kit = parent->target()->kit();
    Core::Id deviceType = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(kit);
    return ((deviceType == Constants::LINKMOTION_DEVICE_TYPE
            || deviceType == Constants::LINKMOTION_SIMULATOR_TYPE)
            && id == LINKMOTION_BUILD_STEP_ID);
}

ProjectExplorer::BuildStep *LinkMotionBuildStepFactory::create(ProjectExplorer::BuildStepList *parent, const Core::Id id)
{
    qDebug() << Q_FUNC_INFO;
    if (!canCreate(parent, id))
        return 0;
    LinkMotionBuildStep *step = new LinkMotionBuildStep(parent);
    if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_CLEAN) {
        step->setClean(true);
        step->setExtraArguments(QStringList(QLatin1String("clean")));
    } else if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_BUILD) {
        // normal setup
    }
    return step;
}

bool LinkMotionBuildStepFactory::canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *source) const
{
    qDebug() << Q_FUNC_INFO;
    return canCreate(parent, source->id());
}

ProjectExplorer::BuildStep *LinkMotionBuildStepFactory::clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *source)
{
    qDebug() << Q_FUNC_INFO;
    if (!canClone(parent, source))
        return 0;
    LinkMotionBuildStep *old(qobject_cast<LinkMotionBuildStep *>(source));
    Q_ASSERT(old);
    return new LinkMotionBuildStep(parent, old);
}

bool LinkMotionBuildStepFactory::canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const
{
    qDebug() << Q_FUNC_INFO;
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep *LinkMotionBuildStepFactory::restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    if (!canRestore(parent, map))
        return 0;
    LinkMotionBuildStep *bs(new LinkMotionBuildStep(parent));
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return 0;
}

QList<ProjectExplorer::BuildStepInfo> LinkMotionBuildStepFactory::availableSteps(ProjectExplorer::BuildStepList *parent) const
{
    qDebug() << Q_FUNC_INFO;
    QList<ProjectExplorer::BuildStepInfo> retval;
    if (!parent->target()) return retval;
    if (!parent->target()->kit()) return retval;

    ProjectExplorer::Kit *kit = parent->target()->kit();
    Core::Id deviceType = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(kit);
    if (deviceType == Constants::LINKMOTION_DEVICE_TYPE
            || deviceType == Constants::LINKMOTION_SIMULATOR_TYPE)
        retval << ProjectExplorer::BuildStepInfo(Core::Id(LINKMOTION_BUILD_STEP_ID),this->displayNameForId(Core::Id(LINKMOTION_BUILD_STEP_ID)));

    return retval;
}

QString LinkMotionBuildStepFactory::displayNameForId(const Core::Id id) const
{
    qDebug() << Q_FUNC_INFO;
    if (id == LINKMOTION_BUILD_STEP_ID)
        return QCoreApplication::translate("LinkMotion::Internal::LinkMotionBuildStep",
                                           LINKMOTION_BUILD_STEP_DISPLAY_NAME);
    return QString();
}
