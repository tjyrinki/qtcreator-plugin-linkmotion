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
#ifndef LINKMOTIONDEPLOYSTEPFACTORY_H
#define LINKMOTIONDEPLOYSTEPFACTORY_H

#include <QObject>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeployStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT
public:
    explicit LinkMotionDeployStepFactory(QObject *parent = 0);

    QList<ProjectExplorer::BuildStepInfo> availableSteps(ProjectExplorer::BuildStepList *parent) const override;
    QString displayNameForId(Core::Id id) const;

    bool canCreate(ProjectExplorer::BuildStepList *parent,
                   Core::Id id) const;
    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;

    bool canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const;
    ProjectExplorer::BuildStep *restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) override;

    bool canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *step) const;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent,
                                      ProjectExplorer::BuildStep *step) override;
};

}
}

#endif // LINKMOTIONDEPLOYSTEPFACTORY_H
