/*####################################################################
#
# This file is part of the LinkMotion Deploy plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotiondeploystepfactory.h"

#include <utils/qtcassert.h>
#include <coreplugin/id.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/target.h>

#include "linkmotiondeploystep.h"
#include "linkmotiondeployplugin_constants.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDeployStepFactory::LinkMotionDeployStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{

}

QList<ProjectExplorer::BuildStepInfo> LinkMotionDeployStepFactory::availableSteps(ProjectExplorer::BuildStepList *parent) const
{
    QList<ProjectExplorer::BuildStepInfo> retval;
    if (parent->id() == ProjectExplorer::Constants::BUILDSTEPS_DEPLOY && !parent->contains(LinkMotionDeployStep::Id))
        retval << ProjectExplorer::BuildStepInfo(LinkMotionDeployStep::Id,this->displayNameForId(LinkMotionDeployStep::Id));
    return retval;
}

QString LinkMotionDeployStepFactory::displayNameForId(Core::Id id) const
{
    if (id == LinkMotionDeployStep::Id)
        return tr("Deploy to LinkMotion device or emulator");
    return QString();
}

bool LinkMotionDeployStepFactory::canCreate(ProjectExplorer::BuildStepList *parent, Core::Id id) const
{
    return true;
}

ProjectExplorer::BuildStep *LinkMotionDeployStepFactory::create(ProjectExplorer::BuildStepList *parent, Core::Id id)
{
    Q_ASSERT(canCreate(parent, id));
    Q_UNUSED(id);
    return new LinkMotionDeployStep(parent);
}

bool LinkMotionDeployStepFactory::canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep *LinkMotionDeployStepFactory::restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    Q_ASSERT(canRestore(parent, map));
    LinkMotionDeployStep * const step = new LinkMotionDeployStep(parent);
    if (!step->fromMap(map)) {
        delete step;
        return 0;
    }
    return step;
}

bool LinkMotionDeployStepFactory::canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *step) const
{
    return canCreate(parent, step->id());
}

ProjectExplorer::BuildStep *LinkMotionDeployStepFactory::clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *step)
{
    Q_ASSERT(canClone(parent, step));
    return new LinkMotionDeployStep(parent, static_cast<LinkMotionDeployStep *>(step));
}
