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

#include "linkmotionbuildstep.h"
#include "linkmotionbuildsettingswidget.h"
#include "buildplugin_constants.h"

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/toolchain.h>
#include <genericprojectmanager/genericmakestep.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;


LinkMotionBuildStep::LinkMotionBuildStep(ProjectExplorer::BuildStepList *bsl)
    : LinkMotionBuildStep(bsl,Constants::LINKMOTION_BUILDSTEP_ID)
{
    qDebug() << Q_FUNC_INFO;
    setDefaultDisplayName(tr("LinkMotion Build"));
}

LinkMotionBuildStep::LinkMotionBuildStep(ProjectExplorer::BuildStepList *bsl,Core::Id typeId)
    : AbstractProcessStep(bsl,typeId)
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionBuildStep::LinkMotionBuildStep(ProjectExplorer::BuildStepList *bsl, LinkMotionBuildStep *bs)
    : AbstractProcessStep(bsl,bs)
{
    qDebug() << Q_FUNC_INFO;
}

bool LinkMotionBuildStep::init(QList<const BuildStep *> &earlierSteps)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(earlierSteps);
    QString projectDir = target()->project()->projectDirectory().toString();

    ProjectExplorer::BuildConfiguration *bc = target()->activeBuildConfiguration();
    if(!bc)
        return false;

    return true;
}

void LinkMotionBuildStep::run(QFutureInterface<bool> &fi) {
    bool result = false;
    QProcess vmsdkBuild;
    //TODO: FIXME
    // using qprocess will hang qt creator. check the proper qtc function.

    emit addOutput(tr("Building inside vmsdk.. please wait."), ProjectExplorer::BuildStep::NormalOutput);

    vmsdkBuild.start(QString::fromLatin1("vmsdk-build lm-ui-plugins"));
    if (vmsdkBuild.waitForFinished(120000)) {
        emit addOutput(tr("Build command executed!"), ProjectExplorer::BuildStep::NormalOutput);
    } else {
        emit addOutput(tr("Build failed!"), ProjectExplorer::BuildStep::ErrorOutput);
    }
    emit addOutput(QString::fromLatin1(vmsdkBuild.readAllStandardOutput()), ProjectExplorer::BuildStep::NormalOutput);
    emit addOutput(QString::fromLatin1(vmsdkBuild.readAllStandardError()), ProjectExplorer::BuildStep::ErrorOutput);

    fi.reportFinished(&result);
}

ProjectExplorer::BuildStepConfigWidget *LinkMotionBuildStep::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionBuildStepWidget();
}
