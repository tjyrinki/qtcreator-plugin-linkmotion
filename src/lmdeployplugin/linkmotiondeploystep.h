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
#ifndef LINKMOTIONDEPLOYSTEP_H
#define LINKMOTIONDEPLOYSTEP_H

#include <QObject>
#include <projectexplorer/abstractprocessstep.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeployStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT
    friend class LinkMotionDeployStepConfigWidget;
    friend class LinkMotionDeployStepFactory;

public:
    LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent);
    ~LinkMotionDeployStep();

    bool init(QList<const BuildStep *> &earlierSteps) override;
    void run(QFutureInterface<bool> &fi) override;

    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() override;
    bool immutable() const override;
    void setBaseArguments(const QStringList &args);
    void setExtraArguments(const QStringList &extraArgs);
    QStringList baseArguments() const;
    QStringList allArguments() const;
    QStringList defaultArguments() const;
    QString buildCommand() const;

    void setClean(bool clean);
    bool isClean() const;

    QVariantMap toMap() const override;

protected slots:
    void onFinished();

protected:
    LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent, LinkMotionDeployStep *bs);
    LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent, Core::Id id);
    bool fromMap(const QVariantMap &map) override;

private:
    void ctor();

    static const Core::Id Id;

    QStringList m_baseBuildArguments;
    QStringList m_extraArguments;
    QString m_buildCommand;
    bool m_useDefaultArguments;
    bool m_clean;
};

}
}


#endif // LINKMOTIONDEPLOYSTEP_H
