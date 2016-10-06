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
    : IBuildConfigurationFactory(parent)
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionDeployConfigurationFactory::~LinkMotionDeployConfigurationFactory()
{
    qDebug() << Q_FUNC_INFO;
}

int LinkMotionDeployConfigurationFactory::priority(const ProjectExplorer::Target *parent) const
{
    qDebug() << Q_FUNC_INFO;
    if (canHandle(parent))
        return 100;
    return -1;
}

QList<ProjectExplorer::BuildInfo *> LinkMotionDeployConfigurationFactory::availableBuilds(const ProjectExplorer::Target *parent) const
{
    qDebug() << Q_FUNC_INFO;
    if(!canHandle(parent))
        return QList<ProjectExplorer::BuildInfo *>();
    return createBuildInfos(parent->kit(),parent->project()->projectDirectory().toString());
}

int LinkMotionDeployConfigurationFactory::priority(const ProjectExplorer::Kit *k, const QString &projectPath) const
{
    qDebug() << Q_FUNC_INFO;
    return (k && Utils::MimeDatabase().mimeTypeForFile(projectPath)
            .matchesName(QLatin1String(QmlProjectManager::Constants::QMLPROJECT_MIMETYPE))) ? 100 : 100;
}

QList<ProjectExplorer::BuildInfo *> LinkMotionDeployConfigurationFactory::availableSetups(const ProjectExplorer::Kit *k, const QString &projectPath) const
{
    qDebug() << Q_FUNC_INFO;
    return createBuildInfos(k,projectPath);
}

LinkMotionDeployConfiguration *LinkMotionDeployConfigurationFactory::create(ProjectExplorer::Target *parent, const ProjectExplorer::BuildInfo *info) const
{
    qDebug() << Q_FUNC_INFO;
    QTC_ASSERT(info->factory() == this, return 0);
    QTC_ASSERT(info->kitId == parent->kit()->id(), return 0);
    QTC_ASSERT(!info->displayName.isEmpty(), return 0);

    LinkMotionDeployConfiguration *conf = new LinkMotionDeployConfiguration(parent);
    conf->setBuildDirectory(info->buildDirectory);
    conf->setDefaultDisplayName(info->displayName);
    conf->setDisplayName(info->displayName);

    // TODO: Check that this is a linkmotion project
    // then only add the steps
    qDebug() << "INSERTING deploy step";
    ProjectExplorer::BuildStepList *bs = conf->stepList(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY);
    for (int i=0; i<bs->count(); i++) {
        bs->at(i)->deleteLater();

    }
    bs->insertStep(0, new LinkMotionDeployStep(bs));
    return conf;
}

bool LinkMotionDeployConfigurationFactory::canRestore(const ProjectExplorer::Target *parent, const QVariantMap &map) const
{
    qDebug() << Q_FUNC_INFO;
    if (!canHandle(parent))
        return false;
    qDebug() << Q_FUNC_INFO << ProjectExplorer::idFromMap(map);
    return ProjectExplorer::idFromMap(map) == Constants::LINKMOTION_BC_ID;
}

LinkMotionDeployConfiguration *LinkMotionDeployConfigurationFactory::restore(ProjectExplorer::Target *parent, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    if (!canRestore(parent,map) )
        return 0;

    LinkMotionDeployConfiguration *conf = new LinkMotionDeployConfiguration(parent);
    if (conf->fromMap(map)) {
        ProjectExplorer::BuildStepList *bs = conf->stepList(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY);
        bs->insertStep(0, new LinkMotionDeployStep(bs));
        qDebug() << "2";

        return conf;
    }

    qDebug() << "1";

    delete conf;
    return 0;
}

bool LinkMotionDeployConfigurationFactory::canClone(const ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *product) const
{
    qDebug() << Q_FUNC_INFO;

    if (!canHandle(parent))
        return false;
    if (product->id() != Constants::LINKMOTION_BC_ID )
        return false;

    return true;
}

LinkMotionDeployConfiguration *LinkMotionDeployConfigurationFactory::clone(ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *product)
{
    qDebug() << Q_FUNC_INFO;
    if (!canClone(parent,product))
        return 0;
    return new LinkMotionDeployConfiguration(parent,static_cast<LinkMotionDeployConfiguration*>(product));
}

bool LinkMotionDeployConfigurationFactory::canHandle(const ProjectExplorer::Target *t) const
{
    qDebug() << Q_FUNC_INFO;

    return true;
}

QList<ProjectExplorer::BuildInfo *> LinkMotionDeployConfigurationFactory::createBuildInfos(const ProjectExplorer::Kit *k, const QString &projectDir) const
{
    qDebug() << Q_FUNC_INFO;

    QList<ProjectExplorer::BuildInfo *> builds;

    ProjectExplorer::BuildInfo *info = new ProjectExplorer::BuildInfo(this);
    info->buildDirectory = Utils::FileName::fromString(projectDir);

    info->typeName = tr("Qml");
    info->kitId    = k->id();
    info->displayName = tr("Default");

    builds << info;
    qDebug() << Q_FUNC_INFO << info->buildDirectory << info->kitId;
    return builds;
}
