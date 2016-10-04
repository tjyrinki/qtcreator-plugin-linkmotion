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

namespace LinkMotion {
namespace Internal {

class LinkMotionBuildStep : public ProjectExplorer::AbstractProcessStep
{
public:
    LinkMotionBuildStep(ProjectExplorer::BuildStepList *bsl);
    LinkMotionBuildStep(ProjectExplorer::BuildStepList *bsl, Core::Id typeId);
    LinkMotionBuildStep(ProjectExplorer::BuildStepList *bsl, LinkMotionBuildStep *bs);

    virtual bool init(QList<const BuildStep *> &earlierSteps);
    virtual ProjectExplorer::BuildStepConfigWidget *createConfigWidget();
    virtual void run(QFutureInterface<bool> &fi);

};

class LinkMotionBuildStepWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT
public:
    virtual QString summaryText() const { return QString::fromLatin1("Build inside VMSDK"); }
    virtual QString additionalSummaryText() const { return QString::fromLatin1("Builds and installs the package into vmsdk image."); }
    virtual QString displayName() const { return QString::fromLatin1("VMSDK build step"); }
};

}
}
#endif // LINKMOTIONBUILDSTEP_H
