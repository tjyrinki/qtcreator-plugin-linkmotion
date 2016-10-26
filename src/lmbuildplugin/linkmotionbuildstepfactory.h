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
#ifndef LINKMOTIONBUILDSTEPFACTORY_H
#define LINKMOTIONBUILDSTEPFACTORY_H

#include <projectexplorer/buildstep.h>
#include <QObject>

namespace LinkMotion {
namespace Internal {

class LinkMotionBuildStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT

public:
    explicit LinkMotionBuildStepFactory(QObject *parent = 0);

    bool canCreate(ProjectExplorer::BuildStepList *parent, Core::Id id) const;
    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;
    bool canClone(ProjectExplorer::BuildStepList *parent,
                  ProjectExplorer::BuildStep *source) const;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent,
                                      ProjectExplorer::BuildStep *source) override;
    bool canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const;
    ProjectExplorer::BuildStep *restore(ProjectExplorer::BuildStepList *parent,
                                        const QVariantMap &map) override;

    QList<ProjectExplorer::BuildStepInfo> availableSteps(ProjectExplorer::BuildStepList *bc) const override;
    QString displayNameForId(Core::Id id) const;
};

}
}

#endif // LINKMOTIONBUILDSTEPFACTORY_H

