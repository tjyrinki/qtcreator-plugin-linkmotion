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
    ctor();
}

LinkMotionDeployStep::LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent, const Core::Id id) :
    AbstractProcessStep(parent, id),
    m_useDefaultArguments(true),
    m_clean(false)
{
    ctor();
}

LinkMotionDeployStep::LinkMotionDeployStep(ProjectExplorer::BuildStepList *parent, LinkMotionDeployStep *bs) :
    AbstractProcessStep(parent, bs),
    m_baseBuildArguments(bs->m_baseBuildArguments),
    m_useDefaultArguments(bs->m_useDefaultArguments),
    m_clean(bs->m_clean)
{
    ctor();
}

void LinkMotionDeployStep::ctor()
{
    qDebug() << Q_FUNC_INFO;
    setDefaultDisplayName(QCoreApplication::translate("LinkMotion::Internal::LinkMotionDeployStep",
                                                      LINKMOTION_DEPLOY_STEP_DISPLAY_NAME));
    connect(this,SIGNAL(finished()),this,SLOT(onFinished()));

}

void LinkMotionDeployStep::onFinished() {
    qDebug() << Q_FUNC_INFO;
    /*BuildConfiguration *bc = buildConfiguration();
    if (bc) {
        emit addOutput(QStringLiteral("Build directory is here @ %0").arg(QDir(bc->buildDirectory().toString()).dirName()),ProjectExplorer::BuildStep::NormalOutput);

    }*/
}

LinkMotionDeployStep::~LinkMotionDeployStep()
{
    qDebug() << Q_FUNC_INFO;
    disconnect(this,SIGNAL(finished()),this,SLOT(onFinished()));
}

bool LinkMotionDeployStep::init()
{
    qDebug() << Q_FUNC_INFO << buildConfiguration() << target()->activeBuildConfiguration();
    ProjectExplorer::DeployConfiguration *dc = deployConfiguration();
    LinkMotionBuildConfiguration *bc = qobject_cast<LinkMotionBuildConfiguration*>(buildConfiguration());
    if (!dc) {
        qDebug() << "missing deploy config 1";
        dc = target()->activeDeployConfiguration();
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

    /*ToolChain *tc = ToolChainKitInformation::toolChain(target()->kit());
    if (!tc) {
        qDebug() << "missing compiler";
        emit addTask(Task::compilerMissingTask());
    }*/

    if (!dc /*|| !tc*/) {
        qDebug() << "faulty configuration";
       // emitFaultyConfigurationMessage();
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

    env.set(QStringLiteral("LINKMOTION_DEVICE"),bc->m_device);
    env.set(QStringLiteral("LINKMOTION_USERNAME"),bc->m_username);
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),bc->m_password);

    pp->setEnvironment(env);
    pp->setMacroExpander(dc->macroExpander());
    pp->setWorkingDirectory(QDir(bc->buildDirectory().toString()).dirName());
    pp->setCommand(QStringLiteral("vmsdk-install"));
    // TODO: find all generated rpm files.
    //       those should be parsed from the output when buildplugin is creating the rpm packages.
    QString arch = env.value(QStringLiteral("LINKMOTION_DEVICE"));
    if (arch.isEmpty())
        arch = QStringLiteral("intel");
    pp->setArguments(QString(QStringLiteral("../build-%0-%1-latest/*.rpm")).arg(projectName).arg(arch));
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

    qDebug() << "init almost done";
    return AbstractProcessStep::init();
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
    ProjectExplorer::Kit *kit = target()->kit();
    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(kit);
    if (tc->type() == QLatin1String("gcc") || tc->type() == QLatin1String("clang")) {
      //  GccToolChain *gtc = static_cast<GccToolChain *>(tc);
        //res << gtc->platformCodeGenFlags();
    }
    //if (!SysRootKitInformation::sysRoot(kit).isEmpty())
    //    res << QLatin1String("-sdk") << SysRootKitInformation::sysRoot(kit).toString();
    //res << QLatin1String("SYMROOT=") + LINKMOTIONManager::resDirForTarget(target());
    //res << QStringLiteral("ui_center-plugin");
    return res;
}

QString LinkMotionDeployStep::buildCommand() const
{
    qDebug() << Q_FUNC_INFO;
    return QLatin1String("vmsdk-install"); // add path?
}

void LinkMotionDeployStep::run(QFutureInterface<bool> &fi)
{
    qDebug() << Q_FUNC_INFO;
    LinkMotionBuildConfiguration *bc = qobject_cast<LinkMotionBuildConfiguration*>(buildConfiguration());

    if (!bc) {
        qDebug() << "missing build config 1";
        bc = (LinkMotionBuildConfiguration*)target()->activeBuildConfiguration();
        qDebug() << bc;
    }
    if (!bc) {
        qDebug() << "missing build config 2";
        emit addTask(ProjectExplorer::Task::buildConfigurationMissingTask());
    }

    /*ToolChain *tc = ToolChainKitInformation::toolChain(target()->kit());
    if (!tc) {
        qDebug() << "missing compiler";
        emit addTask(Task::compilerMissingTask());
    }*/

    if (!bc /*|| !tc*/) {
        qDebug() << "faulty configuration";
       // emitFaultyConfigurationMessage();
        return;
    }

    ProjectExplorer::ProcessParameters *pp = processParameters();
    Utils::Environment env = bc->environment();

    env.set(QStringLiteral("LINKMOTION_DEVICE"),bc->m_device);
    env.set(QStringLiteral("LINKMOTION_USERNAME"),bc->m_username);
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),bc->m_password);

    pp->setEnvironment(env);
    pp->setWorkingDirectory(QDir(bc->buildDirectory().toString()).dirName());
    // TODO: find all generated rpm files.
    //       those should be parsed from the output when buildplugin is creating the rpm packages.
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

    pp->setArguments(QString(QStringLiteral("build-%0-%1-latest/*.rpm")).arg(projectName).arg(arch));
    pp->resolveAll();


    AbstractProcessStep::run(fi);
}

ProjectExplorer::BuildStepConfigWidget *LinkMotionDeployStep::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionDeployStepConfigWidget(this);
}

bool LinkMotionDeployStep::immutable() const
{
    qDebug() << Q_FUNC_INFO;
    return false;
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
