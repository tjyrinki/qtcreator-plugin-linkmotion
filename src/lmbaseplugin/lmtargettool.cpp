/*
 * Copyright 2014 Canonical Ltd.
 * Copyright 2017 Link Motion Oy.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#include "lmtargettool.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>
#include <lmbaseplugin/lmtoolchain.h>
#include <lmbaseplugin/lmshared.h>
#include <lmbaseplugin/settings.h>

#include <QRegularExpression>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QProcess>
#include <QTimer>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QCollator>
#include <QTextStream>

#include <coreplugin/icore.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/kit.h>
#include <utils/qtcprocess.h>
#include <utils/environment.h>
#include <utils/consoleprocess.h>

#include <QDebug>

namespace LmBase {

enum {
    debug = 0
};

const char CREATE_TARGET_ARGS[]  = "create -n %1 -p %2";
const char DESTROY_TARGET_ARGS[] = "destroy %1";
const char UPGRADE_TARGET_ARGS[] = "upgrade %0";
const char TARGET_OPEN_TERMINAL[]       = "%0 maint %1";

/**
 * @brief LmTargetTool::LmTargetTool
 * Implements functionality needed for executing the target
 * tool
 */
LmTargetTool::LmTargetTool()
{
}

/**
 * @brief LmTargetTool::runToolInTarget
 * Adjusts the \a paramsIn to run in the \a target
 */
ProjectExplorer::ProcessParameters LmTargetTool::prepareToRunInTarget(ProjectExplorer::Kit *target, const QString &cmd,
                                                                         const QStringList &args,
                                                                         const QString &wd,
                                                                         const QMap<QString, QString> &envMap)
{
    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(target, ProjectExplorer::Constants::CXX_LANGUAGE_ID);
    if (!tc || tc->typeId() != Constants::LM_TARGET_TOOLCHAIN_ID) {
        ProjectExplorer::ProcessParameters p;
        p.setArguments(Utils::QtcProcess::joinArgs(args));
        p.setCommand(cmd);
        p.setWorkingDirectory(wd);

        Utils::Environment env = Utils::Environment::systemEnvironment();
        for (const QString &key : envMap.keys()) {
            env.set(key, envMap[key]);
        }
        p.setEnvironment(env);
        return p;
    }

    Internal::LinkMotionToolChain *cTc = static_cast<Internal::LinkMotionToolChain *>(tc);
    const Target &clickTarget = cTc->lmTarget();

    ProjectExplorer::ProcessParameters paramsOut;
    paramsOut.setCommand(Constants::LM_TARGET_TOOL);

    QStringList arguments{
        QStringLiteral("exec"),
        clickTarget.containerName,
        QStringLiteral("--")
    };

    //@TODO map env vars into the container
    if (envMap.size()) {

    }

    arguments.append(cmd);
    arguments.append(args);

    paramsOut.setArguments(Utils::QtcProcess::joinArgs(arguments));
    paramsOut.setWorkingDirectory(wd);

    paramsOut.setEnvironment(Utils::Environment::systemEnvironment());
    return paramsOut;
}

/**
 * @brief LmTargetTool::parametersForCreateChroot
 * Initializes a ProjectExplorer::ProcessParameters object with command and arguments
 * to create a new chroot
 */
void LmTargetTool::parametersForCreateChroot(const Target &target, ProjectExplorer::ProcessParameters *params)
{
    Utils::Environment env = Utils::Environment::systemEnvironment();
    QString command = QString::fromLatin1(CREATE_TARGET_ARGS)
            .arg(target.containerName)
            .arg(target.imageName);

    params->setCommand(Constants::LM_TARGET_TOOL);
    params->setEnvironment(Utils::Environment::systemEnvironment());
    params->setArguments(command);
}

/**
 * @brief LmTargetTool::parametersForMaintainChroot
 * Initializes params with the arguments for maintaining the chroot
 * @note does not call ProjectExplorer::ProcessParameters::resolveAll()
 */
void LmTargetTool::parametersForMaintainChroot(const LmTargetTool::MaintainMode &mode, const Target &target, ProjectExplorer::ProcessParameters *params)
{
    QString arguments;
    switch (mode) {
        case Upgrade:
            params->setCommand(Constants::LM_TARGET_TOOL);
            arguments = QString::fromLatin1(UPGRADE_TARGET_ARGS)
                    .arg(target.containerName);
            break;
        case Delete:
            params->setCommand(Constants::LM_TARGET_TOOL);
            arguments = QString::fromLatin1(DESTROY_TARGET_ARGS)
                    .arg(target.containerName);
            break;
    }


    params->setEnvironment(Utils::Environment::systemEnvironment());
    params->setArguments(arguments);
}

/**
 * @brief LmTargetTool::openChrootTerminal
 * Opens a new terminal logged into the chroot specified by \a target
 * The terminal emulator used is specified in the Creator environment option page
 */
void LmTargetTool::openChrootTerminal(const LmTargetTool::Target &target)
{
    QStringList args = Utils::QtcProcess::splitArgs(Utils::ConsoleProcess::terminalEmulator(Core::ICore::settings()));
    QString     term = args.takeFirst();

    args << QString(QLatin1String(TARGET_OPEN_TERMINAL))
            .arg(Constants::LM_TARGET_TOOL)
            .arg(target.containerName);

    if(!QProcess::startDetached(term,args,QDir::homePath())) {
        printToOutputPane(QCoreApplication::translate("LmTargetTool", "Error when starting terminal"));
    }
}

#if 0
bool LmTargetTool::getTargetFromUser(Target *target, const QString &framework)
{
    QList<LmTargetTool::Target> targets = LmTargetTool::listAvailableTargets(framework);
    if (!targets.size()) {
        QString message = QCoreApplication::translate("LmTargetTool",Constants::UBUNTU_CLICK_NOTARGETS_MESSAGE);
        if(!framework.isEmpty()) {
            message = QCoreApplication::translate("LmTargetTool",Constants::UBUNTU_CLICK_NOTARGETS_FRAMEWORK_MESSAGE)
                    .arg(framework);
        }

        QMessageBox::warning(Core::ICore::mainWindow(),
                             QCoreApplication::translate("LmTargetTool",Constants::UBUNTU_CLICK_NOTARGETS_TITLE),
                             message);
        return false;
    }

    //if we have only 1 target there is nothing to choose
    if(targets.size() == 1){
        *target = targets[0];
        return true;
    }

    QStringList items;
    foreach(const LmTargetTool::Target& t, targets)
        items << QString::fromLatin1("%0-%1").arg(t.framework).arg(t.architecture);

    bool ok = false;
    QString item = QInputDialog::getItem(Core::ICore::mainWindow()
                                         ,QCoreApplication::translate("LmTargetTool",Constants::UBUNTU_CLICK_SELECT_TARGET_TITLE)
                                         ,QCoreApplication::translate("LmTargetTool",Constants::UBUNTU_CLICK_SELECT_TARGET_LABEL)
                                         ,items,0,false,&ok);
    //get index of item in the targets list
    int idx = items.indexOf(item);
    if(!ok || idx < 0 || idx >= targets.size())
        return false;

    *target = targets[idx];
    return true;
}
#endif

QString LmTargetTool::targetBasePath(const LmTargetTool::Target &target)
{
    static QMap<QString, QString> basePathCache;
    if (basePathCache.contains(target.containerName))
        return basePathCache.value(target.containerName);

    QProcess sdkTool;
    sdkTool.setReadChannel(QProcess::StandardOutput);
    sdkTool.setProgram(Constants::LM_TARGET_TOOL);
    sdkTool.setArguments(QStringList()<<QStringLiteral("rootfs")<<target.containerName);
    sdkTool.start(QIODevice::ReadOnly);
    if (!sdkTool.waitForFinished(3000)
            || sdkTool.exitCode() != 0
            || sdkTool.exitStatus() != QProcess::NormalExit)
        return QString();

    QTextStream in(&sdkTool);
    QString basePath = in.readAll().trimmed();
    basePathCache.insert(target.containerName, basePath);
    return basePath;
}

bool LmTargetTool::parseContainerName(const QString &name, LmTargetTool::Target *target, QStringList *allExt)
{
    QStringList ext;

#if 0
    target->framework = UbuntuClickFrameworkProvider::getBaseFramework(name, &ext);
    if (target->framework.isEmpty())
        return false;
#endif

    //ubuntu-sdk-15.04-i386-i386-dev
    //the architecture of the the container is always the second extension
    if (ext.isEmpty() || ext.size() != 3)
        return false;
    target->architecture = ext[1];

    if (allExt)
        *allExt = ext;

    return true;
}

/*!
 * \brief LmTargetTool::targetExists
 * checks if the target is still available
 */
bool LmTargetTool::targetExists(const LmTargetTool::Target &target)
{
    return targetExists(target.containerName);
}

bool LmTargetTool::targetExists(const QString &targetName)
{
    QProcess proc;
    proc.start(Constants::LM_TARGET_TOOL,
               QStringList()<<QStringLiteral("exists")<<targetName);
    if(!proc.waitForFinished(3000)) {
        qWarning()<<"usdk-target did not return in time.";
        return false;
    }

    return (proc.exitCode() == 0);
}

/**
 * @brief LmTargetTool::listAvailableTargets
 * @return all currently existing chroot targets in the system
 */
QList<LmTargetTool::Target> LmTargetTool::listAvailableTargets(const QString &)
{
    QProcess sdkTool;
    sdkTool.setProgram(Constants::LM_TARGET_TOOL);
    sdkTool.setArguments(QStringList()<<QStringLiteral("list"));
    sdkTool.start(QIODevice::ReadOnly);
    if (!sdkTool.waitForFinished(3000)
            || sdkTool.exitCode() != 0
            || sdkTool.exitStatus() != QProcess::NormalExit)
        return QList<Target>();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(sdkTool.readAllStandardOutput(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray())
        return QList<Target>();

#if 0
    QString filterRegex;
    if (!framework.isEmpty()) {
        QString baseFw = UbuntuClickFrameworkProvider::getBaseFramework(framework);
        if (!baseFw.isEmpty()) {
            if(debug) qDebug()<<"Filtering for base framework: "<<baseFw;
            filterRegex = QString::fromLatin1(Constants::UBUNTU_CLICK_TARGETS_FRAMEWORK_REGEX)
                    .arg(clickChrootSuffix())
                    .arg(baseFw);
        }
    }
#endif

    QList <Target> targets;
    QVariantList data = doc.toVariant().toList();
    //QRegularExpression frameworkFilter(filterRegex);
    foreach (const QVariant &target, data) {
        QVariantMap map = target.toMap();

        if (!map.contains(QStringLiteral("name"))
                || !map.contains(QStringLiteral("architecture")))
            continue;

#if 0
        QString targetFw = map.value(QStringLiteral("framework")).toString();

        if (!filterRegex.isEmpty()) {
            QRegularExpressionMatch match = frameworkFilter.match(targetFw);
            if(!match.hasMatch()) {
                continue;
            }
        }
#endif

        Target t;
        t.architecture  = map.value(QStringLiteral("architecture")).toString();
        t.containerName = map.value(QStringLiteral("name")).toString();
        t.upgradesEnabled = map.value(QStringLiteral("updatesEnabled"),false).toBool();
        targets.append(t);
    }
    return targets;
}

QList<LmTargetTool::Target> LmTargetTool::listPossibleDeviceContainers()
{
    QString arch = hostArchitecture();
    if (arch.isEmpty())
        return QList<LmTargetTool::Target>();

    QList<Target> allTargets = listAvailableTargets();

    QList<Target> deviceTargets;
    foreach(const Target &t, allTargets) {
        if (t.architecture == arch
                || (QStringLiteral("amd64") == arch && t.architecture == QStringLiteral("i386")))
            deviceTargets.append(t);
    }
    return deviceTargets;
}

/*!
 * \brief LmTargetTool::clickTargetFromTarget
 * Tries to get the Click target from a projectconfiguration,
 * \returns 0 if nothing was found
 */
const LmTargetTool::Target *LmTargetTool::lmTargetFromTarget(ProjectExplorer::Target *t)
{
#ifndef IN_TEST_PROJECT
    if(!t)
        return nullptr;

    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(t->kit(), ProjectExplorer::Constants::CXX_LANGUAGE_ID);
    if(!tc || (tc->typeId() != Constants::LM_TARGET_TOOLCHAIN_ID))
        return nullptr;

    Internal::LinkMotionToolChain *clickTc = static_cast<Internal::LinkMotionToolChain*>(tc);
    if(!clickTc)
        return nullptr;

    return  &clickTc->lmTarget();
#else
    Q_UNUSED(t);
    return nullptr;
#endif
}

bool LmTargetTool::setTargetUpgradesEnabled(const Target &target, const bool set)
{
    QProcess sdkTool;
    sdkTool.setProgram(Constants::LM_TARGET_TOOL);
    sdkTool.setArguments(QStringList{
        QStringLiteral("set"),
        target.containerName,
        set ? QStringLiteral("upgrades-enabled") : QStringLiteral("upgrades-disabled")
    });
    sdkTool.start(QIODevice::ReadOnly);
    if (!sdkTool.waitForFinished(3000)
            || sdkTool.exitCode() != 0
            || sdkTool.exitStatus() != QProcess::NormalExit)
        return false;
    return true;
}

QString LmTargetTool::findOrCreateGccWrapper (const LmTargetTool::Target &target)
{
    QString compiler;

    if(target.architecture == QStringLiteral("armhf"))
        compiler = QStringLiteral("arm-linux-gnueabihf-gcc");
    else if(target.architecture == QStringLiteral("i386"))
        compiler = QStringLiteral("i686-linux-gnu-gcc");
    else if(target.architecture == QStringLiteral("amd64"))
        compiler = QStringLiteral("x86_64-linux-gnu-gcc");
    else {
        qWarning()<<"Invalid architecture, can not create gcc wrapper link";
        return QString();
    }

    return LmTargetTool::findOrCreateToolWrapper(compiler,target);
}

QString LmTargetTool::findOrCreateQMakeWrapper (const LmTargetTool::Target &target)
{
    QString qmake;

    if(target.architecture == QStringLiteral("armhf"))
        qmake = QStringLiteral("qt5-qmake-arm-linux-gnueabihf");
    else
        qmake = QStringLiteral("qmake");

    return LmTargetTool::findOrCreateToolWrapper(qmake,target);
}

QString LmTargetTool::findOrCreateMakeWrapper (const LmTargetTool::Target &target)
{
    return LmTargetTool::findOrCreateToolWrapper(QStringLiteral("make"),target);
}

CMakeProjectManager::CMakeTool::PathMapper LmTargetTool::mapIncludePathsForCMakeFactory(const ProjectExplorer::Target *t)
{
    return [t](const Utils::FileName &in){
        if (in.isEmpty())
            return in;

        bool canMap = ProjectExplorer::ToolChainKitInformation::toolChain(t->kit(), ProjectExplorer::Constants::CXX_LANGUAGE_ID)
                && ProjectExplorer::ToolChainKitInformation::toolChain(t->kit(), ProjectExplorer::Constants::CXX_LANGUAGE_ID)->typeId() == Constants::LM_TARGET_TOOLCHAIN_ID
                && !ProjectExplorer::SysRootKitInformation::sysRoot(t->kit()).isEmpty();

        if (!canMap)
            return in;


        QString tmp = in.toString();
        QString replace = QString::fromLatin1("\\1%1/\\2").arg(ProjectExplorer::SysRootKitInformation::sysRoot(t->kit()).toUserOutput());
        QStringList pathsToMap = {
            QLatin1String("var"),QLatin1String("bin"),QLatin1String("boot"),QLatin1String("dev"),
            QLatin1String("etc"),QLatin1String("lib"),QLatin1String("lib64"),QLatin1String("media"),
            QLatin1String("mnt"),QLatin1String("opt"),QLatin1String("proc"),QLatin1String("root"),
            QLatin1String("run"),QLatin1String("sbin"),QLatin1String("srv"),QLatin1String("sys"),
            QLatin1String("usr")
        };

        for (const QString &path : pathsToMap) {
            QRegularExpression exp(QString::fromLatin1("(^|[^\\w+]|\\s+|[-=]\\w)\\/(%1)").arg(path));
            tmp.replace(exp,replace);
        }

        return Utils::FileName::fromString(tmp);
    };
}

QString LmTargetTool::hostArchitecture()
{
    static QString hostArch;

    if(!hostArch.isEmpty())
        return hostArch;

    QProcess proc;
    proc.setProgram(QStringLiteral("dpkg"));
    proc.setArguments(QStringList()<<QStringLiteral("--print-architecture"));
    proc.start(QIODevice::ReadOnly);
    if (!proc.waitForFinished(3000) || proc.exitCode() != 0 || proc.exitStatus() != QProcess::NormalExit) {
        qWarning()<<"Could not determine the host architecture";
        return QString();
    }

    QTextStream in(&proc);
    hostArch = in.readAll().simplified();
    return hostArch;
}

bool LmTargetTool::compatibleWithHostArchitecture(const QString &targetArch)
{
    QString arch = hostArchitecture();
    return (targetArch == arch || (QStringLiteral("amd64") == arch && targetArch == QStringLiteral("i386")));
}

QString LmTargetTool::findOrCreateToolWrapper (const QString &tool, const LmTargetTool::Target &target)
{
    QString baseDir = Internal::Settings::settingsPath()
            .appendPath(target.containerName).toString();

    QDir d(baseDir);
    if(!d.exists()) {
        if(!d.mkpath(baseDir)){
            qWarning()<<"Could not create config directory.";
            return QString();
        }
    }

    QString toolWrapper = (Utils::FileName::fromString(baseDir).appendPath(tool).toString());
    QString toolTarget  = Constants::LM_TARGET_WRAPPER;

    QFileInfo symlinkInfo(toolWrapper);

    if(!symlinkInfo.exists() || toolTarget != symlinkInfo.symLinkTarget()) {
        //in case of a broken link QFile::exists also will return false
        //lets try to delete it and ignore the error in case the file
        //simply does not exist
        QFile::remove(toolWrapper);
        if(!QFile::link(toolTarget,toolWrapper)) {
            qWarning()<<"Unable to create link for the tool wrapper: "<<toolWrapper;
            return QString();
        }

    }
    return toolWrapper;
}

QDebug operator<<(QDebug dbg, const LmTargetTool::Target& t)
{
    dbg.nospace() << "("<<"container: "<<t.containerName<<" "
                        <<"arch: "<<t.architecture<<" "
                        <<")";

    return dbg.space();
}

} // namespace LinkMotion

