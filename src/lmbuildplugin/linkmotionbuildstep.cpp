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
#include "linkmotionbuildplugin_constants.h"

#include <QThread>
#include <QApplication>
#include <qloggingcategory.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/target.h>
#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/project.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/toolchain.h>
#include <genericprojectmanager/genericmakestep.h>
#include <projectexplorer/gccparser.h>
#include <qmakeprojectmanager/qmakeparser.h>

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

#include "linkmotionbuildoutputparser.h"

Q_LOGGING_CATEGORY(LinkMotionLog, "linkmotion.linkmotion.common")

using namespace LinkMotion;
using namespace LinkMotion::Internal;

const char LINKMOTION_BUILD_STEP_ID[] = "LinkMotion.LinkMotionBuildStep";
const char LINKMOTION_BUILD_STEP_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("LinkMotion::Internal::LinkMotionBuildStep",
                                                             "vmsdk-build");

const char BUILD_USE_DEFAULT_ARGS_KEY[] = "LinkMotion.LinkMotionBuildStep.ArgumentsUseDefaults";
const char BUILD_ARGUMENTS_KEY[] = "LinkMotion.LinkMotionBuildStep.Arguments";
const char CLEAN_KEY[] = "LinkMotion.LinkMotionBuildStep.Clean";

LinkMotionBuildStep::LinkMotionBuildStep(ProjectExplorer::BuildStepList *parent) :
    ProjectExplorer::AbstractProcessStep(parent, Core::Id(LINKMOTION_BUILD_STEP_ID)),
    m_useDefaultArguments(true),
    m_clean(false)
{
    ctor();
}

LinkMotionBuildStep::LinkMotionBuildStep(ProjectExplorer::BuildStepList *parent, const Core::Id id) :
    ProjectExplorer::AbstractProcessStep(parent, id),
    m_useDefaultArguments(true),
    m_clean(false)
{
    ctor();
}

LinkMotionBuildStep::LinkMotionBuildStep(ProjectExplorer::BuildStepList *parent, LinkMotionBuildStep *bs) :
    ProjectExplorer::AbstractProcessStep(parent, bs),
    m_baseBuildArguments(bs->m_baseBuildArguments),
    m_useDefaultArguments(bs->m_useDefaultArguments),
    m_clean(bs->m_clean)
{
    ctor();
}

void LinkMotionBuildStep::ctor()
{
    qDebug() << Q_FUNC_INFO;
    setDefaultDisplayName(QCoreApplication::translate("LinkMotion::Internal::LinkMotionBuildStep",
                                                      LINKMOTION_BUILD_STEP_DISPLAY_NAME));
    connect(this,SIGNAL(finished()),this,SLOT(onFinished()));

}

void LinkMotionBuildStep::onFinished() {
    qDebug() << Q_FUNC_INFO;
    /*BuildConfiguration *bc = buildConfiguration();
    if (bc) {
        emit addOutput(QStringLiteral("Build directory is here @ %0").arg(QDir(bc->buildDirectory().toString()).dirName()),ProjectExplorer::BuildStep::NormalOutput);

    }*/
}

LinkMotionBuildStep::~LinkMotionBuildStep()
{
    qDebug() << Q_FUNC_INFO;
    disconnect(this,SIGNAL(finished()),this,SLOT(onFinished()));
}

bool LinkMotionBuildStep::init()
{
    qDebug() << Q_FUNC_INFO;
    LinkMotionBuildConfiguration *bc = qobject_cast<LinkMotionBuildConfiguration*>(buildConfiguration());
    if (!bc) {
        qDebug() << "missing build config 1";
        bc = qobject_cast<LinkMotionBuildConfiguration*>(target()->activeBuildConfiguration());
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
        return false;
    }


    if (!target()) {
        qDebug() << Q_FUNC_INFO << "no target";
    }
    if (!target()->project()) {
        qDebug() << Q_FUNC_INFO << "no project";
    }
    QString projectName = QDir(target()->project()->projectDirectory().toString()).dirName();
    QString arch = bc->m_device;

    ProjectExplorer::ProcessParameters *pp = processParameters();
    Utils::Environment env = bc->environment();
    // Force output to english for the parsers. Do this here and not in the toolchain's
    // addToEnvironment() to not screw up the users run environment.
    env.set(QLatin1String("LC_ALL"), QLatin1String("C"));
    pp->setEnvironment(env);
    pp->setMacroExpander(bc->macroExpander());
    pp->setWorkingDirectory(QDir(bc->buildDirectory().toString()).dirName());
    pp->setCommand(QStringLiteral("vmsdk-build"));
    pp->setArguments(QStringLiteral("%0 %1/..").arg(projectName).arg(bc->buildDirectory().toString()));
    pp->resolveAll();

    // If we are cleaning, then build can fail with an error code, but that doesn't mean
    // we should stop the clean queue
    // That is mostly so that rebuild works on an already clean project
    setIgnoreReturnValue(m_clean);

    setOutputParser(new LinkMotionBuildOutputParser(projectName,arch));
    appendOutputParser(new ProjectExplorer::GccParser());
    appendOutputParser(new QmakeProjectManager::QMakeParser());
    ProjectExplorer::IOutputParser *parser = target()->kit()->createOutputParser();
    if (parser)
        appendOutputParser(parser);
    outputParser()->setWorkingDirectory(pp->effectiveWorkingDirectory());

    qDebug() << "init almost done";
    return AbstractProcessStep::init();
}

void LinkMotionBuildStep::setClean(bool clean)
{
    qDebug() << Q_FUNC_INFO;
    m_clean = clean;
}

bool LinkMotionBuildStep::isClean() const
{
    qDebug() << Q_FUNC_INFO;
    return m_clean;
}

QVariantMap LinkMotionBuildStep::toMap() const
{
    qDebug() << Q_FUNC_INFO;
    QVariantMap map(AbstractProcessStep::toMap());

    map.insert(QLatin1String(BUILD_ARGUMENTS_KEY), m_baseBuildArguments);
    map.insert(QLatin1String(BUILD_USE_DEFAULT_ARGS_KEY), m_useDefaultArguments);
    map.insert(QLatin1String(CLEAN_KEY), m_clean);
    return map;
}

bool LinkMotionBuildStep::fromMap(const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    QVariant bArgs = map.value(QLatin1String(BUILD_ARGUMENTS_KEY));
    m_baseBuildArguments = bArgs.toStringList();
    m_useDefaultArguments = map.value(QLatin1String(BUILD_USE_DEFAULT_ARGS_KEY)).toBool();
    m_clean = map.value(QLatin1String(CLEAN_KEY)).toBool();

    return BuildStep::fromMap(map);
}

QStringList LinkMotionBuildStep::allArguments() const
{
    qDebug() << Q_FUNC_INFO;
    return baseArguments() + m_extraArguments;
}

QStringList LinkMotionBuildStep::defaultArguments() const
{
    qDebug() << Q_FUNC_INFO;
    QStringList res;
    ProjectExplorer::Kit *kit = target()->kit();
    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(kit);
    switch (target()->activeBuildConfiguration()->buildType()) {
    case ProjectExplorer::BuildConfiguration::Debug :
      //  res << QLatin1String("-configuration") << QLatin1String("debug");
        break;
    case ProjectExplorer::BuildConfiguration::Release :
       // res << QLatin1String("-configuration") << QLatin1String("release");
        break;
    case ProjectExplorer::BuildConfiguration::Unknown :
        break;
    default:
        qCWarning(LinkMotionLog) << "LinkMotionBuildStep had an unknown buildType "
                          << target()->activeBuildConfiguration()->buildType();
    }
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

QString LinkMotionBuildStep::buildCommand() const
{
    qDebug() << Q_FUNC_INFO;
    return QLatin1String("vmsdk-build"); // add path?
}

void LinkMotionBuildStep::run(QFutureInterface<bool> &fi)
{
    qDebug() << Q_FUNC_INFO;
    ProjectExplorer::BuildManager::instance()->toggleOutputWindow();
    LinkMotionBuildConfiguration *bc = qobject_cast<LinkMotionBuildConfiguration*>(buildConfiguration());
    ProjectExplorer::ProcessParameters *pp = processParameters();
    Utils::Environment env = bc->environment();

    env.set(QStringLiteral("LINKMOTION_DEVICE"),bc->m_device);
    env.set(QStringLiteral("LINKMOTION_USERNAME"),bc->m_username);
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),bc->m_password);

    // Force output to english for the parsers. Do this here and not in the toolchain's
    // addToEnvironment() to not screw up the users run environment.
    env.set(QLatin1String("LC_ALL"), QLatin1String("C"));
    pp->setEnvironment(env);
    AbstractProcessStep::run(fi);
}

ProjectExplorer::BuildStepConfigWidget *LinkMotionBuildStep::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionBuildStepConfigWidget(this);
}

bool LinkMotionBuildStep::immutable() const
{
    qDebug() << Q_FUNC_INFO;
    return false;
}

void LinkMotionBuildStep::setBaseArguments(const QStringList &args)
{
    qDebug() << Q_FUNC_INFO;
    m_baseBuildArguments = args;
    m_useDefaultArguments = (args == defaultArguments());
}

void LinkMotionBuildStep::setExtraArguments(const QStringList &extraArgs)
{
    qDebug() << Q_FUNC_INFO;
    m_extraArguments = extraArgs;
}

QStringList LinkMotionBuildStep::baseArguments() const
{
    qDebug() << Q_FUNC_INFO;
    if (m_useDefaultArguments)
        return defaultArguments();
    return m_baseBuildArguments;
}
