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

#include "linkmotiondeploystep.h"
#include "linkmotiondeploysettingswidget.h"
#include "linkmotiondeployplugin_constants.h"
#include "../lmbuildplugin/linkmotionbuildconfiguration.h"

#include <QThread>
#include <QApplication>
#include <qloggingcategory.h>
#include <projectexplorer/deployconfiguration.h>
#include <projectexplorer/target.h>
#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/project.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/toolchain.h>
#include <genericprojectmanager/genericmakestep.h>

#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/gnumakeparser.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/gcctoolchain.h>
#include <qmakeprojectmanager/qmakebuildconfiguration.h>
#include <qmakeprojectmanager/qmakeprojectmanagerconstants.h>
#include <qmakeprojectmanager/qmakeprojectmanager.h>
#include <qmakeprojectmanager/qmakeproject.h>
#include <qmakeprojectmanager/qmakenodes.h>
#include <qtsupport/qtkitinformation.h>
#include <qtsupport/qtparser.h>
#include <utils/stringutils.h>
#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>

#include "linkmotiondeploystepconfigwidget.h"

Q_LOGGING_CATEGORY(LinkMotionLog, "linkmotion.linkmotion.common")

/*using namespace Core;
using namespace ProjectExplorer;
using namespace QmakeProjectManager;
*/
using namespace LinkMotion;
using namespace LinkMotion::Internal;

const char LINKMOTION_DEPLOY_STEP_ID[] = "LinkMotion.LinkMotionDeployStep";
const char LINKMOTION_DEPLOY_STEP_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LinkMotion::Internal::LinkMotionDeployStep",
                                                             "vmsdk-install");

const char DEPLOY_USE_DEFAULT_ARGS_KEY[] = "LinkMotion.LinkMotionDeployStep.ArgumentsUseDefaults";
const char DEPLOY_ARGUMENTS_KEY[] = "LinkMotion.LinkMotionDeployStep.Arguments";
const char CLEAN_KEY[] = "LinkMotion.LinkMotionDeployStep.Clean";


const Core::Id LinkMotionDeployStep::Id("LinkMotion.LinkMotionDeployStep");

LinkMotionDeployStep::LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent) :
    AbstractProcessStep(parent, Core::Id(LINKMOTION_DEPLOY_STEP_ID)),
    m_useDefaultArguments(true),
    m_clean(false)
{
    qDebug() << Q_FUNC_INFO;
    ctor();
}

LinkMotionDeployStep::LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent, const Core::Id id) :
    AbstractProcessStep(parent, id),
    m_useDefaultArguments(true),
    m_clean(false)
{
    qDebug() << Q_FUNC_INFO;
    ctor();
}

LinkMotionDeployStep::LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent, LinkMotionDeployStep *bs) :
    AbstractProcessStep(parent, bs),
    m_baseBuildArguments(bs->m_baseBuildArguments),
    m_useDefaultArguments(bs->m_useDefaultArguments),
    m_clean(bs->m_clean)
{
    qDebug() << Q_FUNC_INFO;
    ctor();
}

void LinkMotionDeployStep::ctor()
{
    qDebug() << Q_FUNC_INFO;
    setDefaultDisplayName(QCoreApplication::translate("LinkMotion::Internal::LinkMotionDeployStep",
                                                      LINKMOTION_DEPLOY_STEP_DISPLAY_NAME));

}

LinkMotionDeployStep::~LinkMotionDeployStep()
{
    qDebug() << Q_FUNC_INFO;
}

bool LinkMotionDeployStep::init(QList<const BuildStep *> &earlierSteps)
{
    qDebug() << Q_FUNC_INFO << buildConfiguration() << target()->activeBuildConfiguration();
    LinkMotionDeployConfiguration *dc = qobject_cast<LinkMotionDeployConfiguration*>(deployConfiguration());
    LinkMotionBuildConfiguration *bc = qobject_cast<LinkMotionBuildConfiguration*>(buildConfiguration());
    if (!dc) {
        qDebug() << "missing deploy config 1";
        dc = (LinkMotionDeployConfiguration*)target()->activeDeployConfiguration();
    }
    if (!bc) {
        qDebug() << "missing build config 1";
        bc = (LinkMotionBuildConfiguration*)target()->activeBuildConfiguration();
        qDebug() << bc;
    }
    if (!dc) {
        qDebug() << "invalid deploy config";
        return false;
    }

    if (!dc) {
        qDebug() << "faulty configuration";
        return false;
    }

    if (!target()) {
        qDebug() << Q_FUNC_INFO << "no target";
    }
    if (!target()->project()) {
        qDebug() << Q_FUNC_INFO << "no project";
    }
    QString projectName = QDir(target()->project()->projectDirectory().toString()).dirName();

    ProjectExplorer::ProcessParameters *pp = processParameters();
    Utils::Environment env = bc->environment();

    env.set(QStringLiteral("LINKMOTION_DEVICE"),dc->m_device);
    env.set(QStringLiteral("LINKMOTION_USERNAME"),dc->m_username);
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),dc->m_password);
    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    pp->setEnvironment(env);
    pp->setMacroExpander(dc->macroExpander());
    QString projectPath = Utils::FileName::fromString(QStringLiteral("%0/..").arg(bc->buildDirectory().toString())).toFileInfo().absoluteFilePath();
    pp->setWorkingDirectory(projectPath);
    pp->setCommand(QStringLiteral("vmsdk-install"));
    // TODO: find all generated rpm files.
    //       those should be parsed from the output when buildplugin is creating the rpm packages.
    QString arch = env.value(QStringLiteral("LINKMOTION_DEVICE"));
    if (arch.isEmpty())
        arch = QStringLiteral("intel");

    pp->setArguments(QString(QStringLiteral("%0/build-%1-%2-latest/*.rpm")).arg(projectPath).arg(projectName).arg(arch));
    pp->resolveAll();

    // If we are cleaning, then build can fail with an error code, but that doesn't mean
    // we should stop the clean queue
    // That is mostly so that rebuild works on an already clean project
    setIgnoreReturnValue(m_clean);

    setOutputParser(new ProjectExplorer::GnuMakeParser());
    ProjectExplorer::IOutputParser *parser = target()->kit()->createOutputParser();
    if (parser)
        appendOutputParser(parser);
    outputParser()->setWorkingDirectory(pp->effectiveWorkingDirectory());

    //Developer Note! Check run(QFutureInterface<bool> &fi)
    //   this needs refactoring, probably we can remove most of the duplicates from this function.

    return AbstractProcessStep::init(earlierSteps);
}

void LinkMotionDeployStep::setClean(bool clean)
{
    qDebug() << Q_FUNC_INFO;
    m_clean = clean;
}

bool LinkMotionDeployStep::isClean() const
{
    qDebug() << Q_FUNC_INFO;
    return m_clean;
}

QVariantMap LinkMotionDeployStep::toMap() const
{
    qDebug() << Q_FUNC_INFO;
    QVariantMap map(AbstractProcessStep::toMap());

    map.insert(QLatin1String(DEPLOY_ARGUMENTS_KEY), m_baseBuildArguments);
    map.insert(QLatin1String(DEPLOY_USE_DEFAULT_ARGS_KEY), m_useDefaultArguments);
    map.insert(QLatin1String(CLEAN_KEY), m_clean);
    return map;
}

bool LinkMotionDeployStep::fromMap(const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    QVariant bArgs = map.value(QLatin1String(DEPLOY_ARGUMENTS_KEY));
    m_baseBuildArguments = bArgs.toStringList();
    m_useDefaultArguments = map.value(QLatin1String(DEPLOY_USE_DEFAULT_ARGS_KEY)).toBool();
    m_clean = map.value(QLatin1String(CLEAN_KEY)).toBool();

    return BuildStep::fromMap(map);
}

QStringList LinkMotionDeployStep::allArguments() const
{
    qDebug() << Q_FUNC_INFO;
    return baseArguments() + m_extraArguments;
}

QStringList LinkMotionDeployStep::defaultArguments() const
{
    qDebug() << Q_FUNC_INFO;
    QStringList res;

    return res;
}

QString LinkMotionDeployStep::buildCommand() const
{
    qDebug() << Q_FUNC_INFO;
    return QLatin1String("vmsdk-install");
}

void LinkMotionDeployStep::run(QFutureInterface<bool> &fi)
{
    qDebug() << Q_FUNC_INFO;
    if (!target()) return;
    if (!target()->project()) return;

    LinkMotionBuildConfiguration *bc = qobject_cast<LinkMotionBuildConfiguration*>(buildConfiguration());
    LinkMotionDeployConfiguration *dc = qobject_cast<LinkMotionDeployConfiguration*>(deployConfiguration());
    if (!dc) {
        qDebug() << "missing deploy config 1";
        dc = (LinkMotionDeployConfiguration*)target()->activeDeployConfiguration();
    }

    if (!bc) {
        qDebug() << "missing build config 1";
        bc = (LinkMotionBuildConfiguration*)target()->activeBuildConfiguration();
        qDebug() << bc;
    }
    if (!bc) {
        qDebug() << "missing build config 2";
        emit addTask(ProjectExplorer::Task::buildConfigurationMissingTask());
    }

    if (!bc) {
        qDebug() << "faulty configuration";
        return;
    }

    ProjectExplorer::ProcessParameters *pp = processParameters();
    Utils::Environment env = bc->environment();

    env.set(QStringLiteral("LINKMOTION_DEVICE"),dc->m_device);
    env.set(QStringLiteral("LINKMOTION_USERNAME"),dc->m_username);
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),dc->m_password);
    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    pp->setEnvironment(env);
    QString arch = env.value(QStringLiteral("LINKMOTION_DEVICE"));
    if (arch.isEmpty())
        arch = QStringLiteral("intel");
    if (!target()) {
        qDebug() << Q_FUNC_INFO << "no target";
    }
    if (!target()->project()) {
        qDebug() << Q_FUNC_INFO << "no project";
    }
    QString projectName = QDir(target()->project()->projectDirectory().toString()).dirName();
    QString projectPath = Utils::FileName::fromString(QStringLiteral("%0/..").arg(bc->buildDirectory().toString())).toFileInfo().absoluteFilePath();
    pp->setWorkingDirectory(projectPath);
    pp->setArguments(QString(QStringLiteral("%0/build-%1-%2-latest/*.rpm")).arg(projectPath).arg(projectName).arg(arch));
    pp->resolveAll();

    AbstractProcessStep::run(fi);
}

ProjectExplorer::BuildStepConfigWidget *LinkMotionDeployStep::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO << this;
    ProjectExplorer::BuildStepConfigWidget* retval = new LinkMotionDeployStepConfigWidget(this);
    return retval;
}

bool LinkMotionDeployStep::immutable() const
{
    qDebug() << Q_FUNC_INFO;
    return true;
}

void LinkMotionDeployStep::setBaseArguments(const QStringList &args)
{
    qDebug() << Q_FUNC_INFO;
    m_baseBuildArguments = args;
    m_useDefaultArguments = (args == defaultArguments());
}

void LinkMotionDeployStep::setExtraArguments(const QStringList &extraArgs)
{
    qDebug() << Q_FUNC_INFO;
    m_extraArguments = extraArgs;
}

QStringList LinkMotionDeployStep::baseArguments() const
{
    qDebug() << Q_FUNC_INFO;
    if (m_useDefaultArguments)
        return defaultArguments();
    return m_baseBuildArguments;
}
