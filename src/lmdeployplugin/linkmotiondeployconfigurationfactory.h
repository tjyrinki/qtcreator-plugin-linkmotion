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
#ifndef LINKMOTIONDEPLOYCONFIGURATIONFACTORY_H
#define LINKMOTIONDEPLOYCONFIGURATIONFACTORY_H

#include "linkmotiondeployconfiguration.h"
#include <QDebug>

#include <QObject>
#include <projectexplorer/buildconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeployConfigurationFactory : public ProjectExplorer::IBuildConfigurationFactory
{
public:
    LinkMotionDeployConfigurationFactory(QObject *parent = 0);
    ~LinkMotionDeployConfigurationFactory();

    virtual int priority(const ProjectExplorer::Target *parent) const override;
    virtual QList<ProjectExplorer::BuildInfo *> availableBuilds(const ProjectExplorer::Target *parent) const override;
    virtual int priority(const ProjectExplorer::Kit *k, const QString &projectPath) const override;
    virtual QList<ProjectExplorer::BuildInfo *> availableSetups(const ProjectExplorer::Kit *k, const QString &projectPath) const override;
    virtual LinkMotionDeployConfiguration *create(ProjectExplorer::Target *parent, const ProjectExplorer::BuildInfo *info) const override;
    virtual bool canRestore(const ProjectExplorer::Target *parent, const QVariantMap &map) const override;
    virtual LinkMotionDeployConfiguration *restore(ProjectExplorer::Target *parent, const QVariantMap &map) override;
    virtual bool canClone(const ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *product) const override;
    virtual LinkMotionDeployConfiguration* clone(ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *product) override;
private:
    bool canHandle(const ProjectExplorer::Target *t) const;
    QList<ProjectExplorer::BuildInfo *> createBuildInfos (const ProjectExplorer::Kit *k, const QString &projectDir) const;
};

}
}

#endif // LINKMOTIONDEPLOYCONFIGURATIONFACTORY_H
