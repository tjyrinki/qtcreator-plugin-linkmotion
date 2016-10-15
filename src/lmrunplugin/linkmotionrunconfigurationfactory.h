#ifndef LINKMOTIONRUNCONFIGURATIONFACTORY_H
#define LINKMOTIONRUNCONFIGURATIONFACTORY_H

#include <QObject>
#include <projectexplorer/runconfiguration.h>
#include <QDebug>

class LinkMotionRunConfigurationFactory : public ProjectExplorer::IRunConfigurationFactory
{
    Q_OBJECT
public:
    explicit LinkMotionRunConfigurationFactory() {
        qDebug() << Q_FUNC_INFO;
        setObjectName(QLatin1String("LinkMotionRunConfigurationFactory"));
    }

    QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode = UserCreate) const override;
    QString displayNameForId(const Core::Id id) const override;

    bool canCreate(ProjectExplorer::Target *parent, const Core::Id id) const override;
    bool canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const override;
    bool canClone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *product) const override;
    ProjectExplorer::RunConfiguration *clone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *source) override;

private:
    bool canHandle(ProjectExplorer::Target *parent) const;
    ProjectExplorer::RunConfiguration *doCreate(ProjectExplorer::Target *parent, const Core::Id id) override;
    ProjectExplorer::RunConfiguration *doRestore(ProjectExplorer::Target *parent, const QVariantMap &map) override;
};

#endif // LINKMOTIONRUNCONFIGURATIONFACTORY_H
