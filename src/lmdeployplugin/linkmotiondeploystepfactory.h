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

    QList<Core::Id> availableCreationIds(ProjectExplorer::BuildStepList *parent) const override;
    QString displayNameForId(Core::Id id) const override;

    bool canCreate(ProjectExplorer::BuildStepList *parent,
                   Core::Id id) const override;
    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;

    bool canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const override;
    ProjectExplorer::BuildStep *restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) override;

    bool canClone(ProjectExplorer::BuildStepList *parent,
                  ProjectExplorer::BuildStep *step) const override;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent,
                                      ProjectExplorer::BuildStep *step) override;
};

}
}

#endif // LINKMOTIONDEPLOYSTEPFACTORY_H
