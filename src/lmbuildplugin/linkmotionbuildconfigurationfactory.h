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
#ifndef LINKMOTIONBUILDCONFIGURATIONFACTORY_H
#define LINKMOTIONBUILDCONFIGURATIONFACTORY_H

#include "linkmotionbuildconfiguration.h"
#include <QDebug>

#include <QObject>
#include <projectexplorer/buildconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionBuildConfigurationFactory : public ProjectExplorer::IBuildConfigurationFactory
{
public:
    LinkMotionBuildConfigurationFactory(QObject *parent = 0);
    ~LinkMotionBuildConfigurationFactory();

    virtual int priority(const ProjectExplorer::Target *parent) const override;
    virtual QList<ProjectExplorer::BuildInfo *> availableBuilds(const ProjectExplorer::Target *parent) const override;
    virtual int priority(const ProjectExplorer::Kit *k, const QString &projectPath) const override;
    virtual QList<ProjectExplorer::BuildInfo *> availableSetups(const ProjectExplorer::Kit *k, const QString &projectPath) const override;
    virtual LinkMotionBuildConfiguration *create(ProjectExplorer::Target *parent, const ProjectExplorer::BuildInfo *info) const override;
    virtual bool canRestore(const ProjectExplorer::Target *parent, const QVariantMap &map) const override;
    virtual LinkMotionBuildConfiguration *restore(ProjectExplorer::Target *parent, const QVariantMap &map) override;
    virtual bool canClone(const ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *product) const override;
    virtual LinkMotionBuildConfiguration* clone(ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *product) override;
private:
    bool canHandle(const ProjectExplorer::Target *t) const;
    QList<ProjectExplorer::BuildInfo *> createBuildInfos (const ProjectExplorer::Kit *k, const QString &projectDir) const;
};

}
}

#endif // LINKMOTIONBUILDCONFIGURATIONFACTORY_H
