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

#include "linkmotiondeployconfigurationfactory.h"
#include "linkmotiondeployplugin_constants.h"
#include "linkmotiondeploystep.h"
#include "linkmotiondeployconfiguration.h"

#include <projectexplorer/buildconfiguration.h>

#include <qmlprojectmanager/qmlprojectconstants.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDeployConfigurationFactory::LinkMotionDeployConfigurationFactory(QObject *parent)
    : DeployConfigurationFactory(parent)
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionDeployConfigurationFactory::~LinkMotionDeployConfigurationFactory()
{
    qDebug() << Q_FUNC_INFO;
}

// used to show the list of possible additons to a target, returns a list of types
QList<Core::Id> LinkMotionDeployConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent) const {
    qDebug() << Q_FUNC_INFO;
    return QList<Core::Id>() << Constants::LINKMOTION_DC_ID;
}

// used to translate the types to names to display to the user
QString LinkMotionDeployConfigurationFactory::displayNameForId(Core::Id id) const {
    qDebug() << Q_FUNC_INFO;
    if (id == Constants::LINKMOTION_DC_ID) {
        return tr("LinkMotion Deploy Step");
    } else {
        return tr("");
    }
}

bool LinkMotionDeployConfigurationFactory::canCreate(ProjectExplorer::Target *parent, Core::Id id) const {
    qDebug() << Q_FUNC_INFO;
    if (id != Constants::LINKMOTION_DC_ID )
        return false;
    return true;
}

ProjectExplorer::DeployConfiguration* LinkMotionDeployConfigurationFactory::create(ProjectExplorer::Target *parent, Core::Id id) {
    qDebug() << Q_FUNC_INFO;
    LinkMotionDeployConfiguration *conf = new LinkMotionDeployConfiguration(parent);
    conf->setDefaultDisplayName(parent->activeBuildConfiguration()->displayName());
    conf->setDisplayName(parent->activeBuildConfiguration()->displayName());

    ProjectExplorer::BuildStepList *bs = conf->stepList();

    bs->insertStep(0, new LinkMotionDeployStep(bs));
    return conf;

}

// used to recreate the runConfigurations when restoring settings
bool LinkMotionDeployConfigurationFactory::canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const {
    qDebug() << Q_FUNC_INFO;
    return ProjectExplorer::idFromMap(map) == Constants::LINKMOTION_DC_ID;
}

ProjectExplorer::DeployConfiguration* LinkMotionDeployConfigurationFactory::restore(ProjectExplorer::Target *parent, const QVariantMap &map) {
    qDebug() << Q_FUNC_INFO;
    if (!canRestore(parent, map)) {
        return 0;
    }

    LinkMotionDeployConfiguration *conf = new LinkMotionDeployConfiguration(parent);
    if (conf->fromMap(map)) {
        ProjectExplorer::BuildStepList *bs = conf->stepList();

        LinkMotionDeployStep* deployStep = new LinkMotionDeployStep(bs);
        if (!bs->contains(deployStep->id())) {
            bs->insertStep(0, deployStep);
        }
        return conf;
    }

    delete conf;
    return 0;

}

bool LinkMotionDeployConfigurationFactory::canClone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product) const {
    qDebug() << Q_FUNC_INFO;
    if (product->id() != Constants::LINKMOTION_DC_ID )
        return false;
    return true;
}

ProjectExplorer::DeployConfiguration* LinkMotionDeployConfigurationFactory::clone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product) {
    qDebug() << Q_FUNC_INFO;
    if (!canClone(parent, product)) {
        return 0;
    }
    return new LinkMotionDeployConfiguration(parent,static_cast<LinkMotionDeployConfiguration*>(product));
}
