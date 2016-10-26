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

#ifndef LINKMOTIONBUILDSTEP_H
#define LINKMOTIONBUILDSTEP_H

#include <QObject>
#include <QDebug>

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/toolchain.h>
#include <utils/qtcprocess.h>
#include "linkmotionbuildsettingswidget.h"
#include "linkmotionbuildstepconfigwidget.h"

namespace LinkMotion {
namespace Internal {

class LinkMotionBuildStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT
    friend class LinkMotionBuildStepConfigWidget;
    friend class LinkMotionBuildStepFactory;

public:
    LinkMotionBuildStep(ProjectExplorer::BuildStepList *parent);
    ~LinkMotionBuildStep();

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
    LinkMotionBuildStep(ProjectExplorer::BuildStepList *parent, LinkMotionBuildStep *bs);
    LinkMotionBuildStep(ProjectExplorer::BuildStepList *parent, Core::Id id);
    bool fromMap(const QVariantMap &map) override;

private:
    void ctor();

    QStringList m_baseBuildArguments;
    QStringList m_extraArguments;
    QString m_buildCommand;
    bool m_useDefaultArguments;
    bool m_clean;
};


}
}
#endif // LINKMOTIONBUILDSTEP_H
