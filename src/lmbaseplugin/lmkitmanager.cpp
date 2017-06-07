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
#include "lmkitmanager.h"
#include <lmbaseplugin/lmtoolchain.h>
#include <lmbaseplugin/lmbaseplugin_constants.h>
#include <lmbaseplugin/lmqtversion.h>
#include <lmbaseplugin/lmtargettool.h>

//#include "ubuntuclickdialog.h"
#include "settings.h"

/*
#include <ubuntu/device/remote/ubuntudevice.h>
#include <ubuntu/device/container/containerdevice.h>
*/

#include <coreplugin/icore.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/toolchainmanager.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/devicesupport/devicemanager.h>
#include <debugger/debuggeritemmanager.h>
#include <debugger/debuggeritem.h>
#include <debugger/debuggerkitinformation.h>
#include <qtsupport/qtkitinformation.h>

#include <cmakeprojectmanager/cmaketoolmanager.h>
#include <cmakeprojectmanager/cmaketool.h>
#include <cmakeprojectmanager/cmakekitinformation.h>
#include <cmakeprojectmanager/cmakeconfigitem.h>
#include <qtsupport/qtversionmanager.h>

#include <QMessageBox>
#include <QRegularExpression>
#include <QTextStream>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>
#include <QPair>
#include <QInputDialog>

namespace LmBase {
namespace Internal {

enum {
    debug = 0
};

static bool equalKits(ProjectExplorer::Kit *a, ProjectExplorer::Kit *b)
{
    return ProjectExplorer::ToolChainKitInformation::toolChain(a, ProjectExplorer::Constants::CXX_LANGUAGE_ID)
            == ProjectExplorer::ToolChainKitInformation::ToolChainKitInformation::toolChain(b, ProjectExplorer::Constants::CXX_LANGUAGE_ID);
}

static void createOrFindDeviceAndType(ProjectExplorer::Kit *k, LinkMotionToolChain *tc)
{
#if 0
    if (LmTargetTool::compatibleWithHostArchitecture(tc->lmTarget().architecture)) {
        Core::Id devId = ContainerDevice::createIdForContainer(tc->lmTarget().containerName);
        ProjectExplorer::IDevice::ConstPtr ptr
                = ProjectExplorer::DeviceManager::instance()->find(devId);

        if (!ptr) {
            ContainerDevice::Ptr dev = ContainerDevice::create(devId, devId);
            ProjectExplorer::DeviceManager::instance()->addDevice(dev);
            ptr = dev;
        }

        ProjectExplorer::DeviceTypeKitInformation::setDeviceTypeId(k,devId);
        ProjectExplorer::DeviceKitInformation::setDevice(k, ptr);
    } else {
#endif
        //a Kit that cannot take a Container Device
        Core::Id devTypeId = Core::Id(Constants::LM_DEVICE_TYPE_ID).withSuffix(tc->lmTarget().architecture);
        ProjectExplorer::DeviceTypeKitInformation::setDeviceTypeId(k,devTypeId);
    //}
}
LinkMotionKitManager::LinkMotionKitManager()
{
}

QList<LinkMotionToolChain *> LinkMotionKitManager::linkMotionToolChains()
{
    QList<LinkMotionToolChain *> toolchains;
    // having a empty toolchains list will remove all autodetected kits for android
    // exactly what we want in that case
    foreach (ProjectExplorer::ToolChain *tc, ProjectExplorer::ToolChainManager::toolChains()) {
        if(tc) {
            if (!tc->isAutoDetected())
                continue;
            if (tc->typeId() != Constants::LM_TARGET_TOOLCHAIN_ID)
                continue;
            toolchains << static_cast<LinkMotionToolChain *>(tc);
        }
    }
    return toolchains;
}

QList<ProjectExplorer::Kit *> LinkMotionKitManager::findKitsUsingTarget (const LmTargetTool::Target &target)
{
    auto matcher = [&target](const ProjectExplorer::Kit *k) {
        ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k, ProjectExplorer::Constants::CXX_LANGUAGE_ID);
        if (!tc)
            return false;

        if (tc->typeId() != Constants::LM_TARGET_TOOLCHAIN_ID)
            return false;

        LinkMotionToolChain *cTc = static_cast<LinkMotionToolChain *>(tc);
        return (cTc->lmTarget().containerName == target.containerName);
    };

    return ProjectExplorer::KitManager::kits(matcher);
}

LinkMotionQtVersion *LinkMotionKitManager::createOrFindQtVersion(LinkMotionToolChain *tc)
{
    QString qmakePath = LmTargetTool::findOrCreateQMakeWrapper(tc->lmTarget());
    if(!QFile::exists(qmakePath)) {
        return 0;
    } else {
        //try to find a already existing Qt Version for this target
        foreach (QtSupport::BaseQtVersion *qtVersion, QtSupport::QtVersionManager::versions()) {
            if (qtVersion->type() != QLatin1String(Constants::LM_QTVERSION_TYPE))
                continue;

            if (qtVersion->qmakeCommand().toFileInfo().absoluteFilePath() == QFileInfo(qmakePath).absoluteFilePath())
                return static_cast<LinkMotionQtVersion*> (qtVersion);
        }
    }

    LinkMotionQtVersion *qtVersion = new LinkMotionQtVersion(tc->lmTarget().containerName, Utils::FileName::fromString(qmakePath),false);
    QtSupport::QtVersionManager::addVersion(qtVersion);
    return qtVersion;
}

CMakeProjectManager::CMakeTool *LinkMotionKitManager::createOrFindCMakeTool(LinkMotionToolChain *tc)
{
    QString cmakePathStr = LmTargetTool::findOrCreateToolWrapper(QStringLiteral("cmake"), tc->lmTarget());
    Utils::FileName cmakePath = Utils::FileName::fromString(cmakePathStr);

    CMakeProjectManager::CMakeTool *cmake = CMakeProjectManager::CMakeToolManager::findByCommand(cmakePath);
    if (cmake)
        return cmake;

    cmake = createCMakeTool(tc);
    if (!CMakeProjectManager::CMakeToolManager::registerCMakeTool(cmake)) {
        delete cmake;
        return 0;
    }

    return cmake;
}

CMakeProjectManager::CMakeTool *LinkMotionKitManager::createCMakeTool(LinkMotionToolChain *tc)
{
    return createCMakeTool(tc->lmTarget());
}

CMakeProjectManager::CMakeTool *LinkMotionKitManager::createCMakeTool(const LmTargetTool::Target &target)
{
    QString cmakePathStr = LmTargetTool::findOrCreateToolWrapper(QStringLiteral("cmake"), target);
    Utils::FileName cmakePath = Utils::FileName::fromString(cmakePathStr);
    CMakeProjectManager::CMakeTool *cmake = new CMakeProjectManager::CMakeTool(CMakeProjectManager::CMakeTool::AutoDetection,
                                                                               CMakeProjectManager::CMakeTool::createId());

    cmake->setCMakeExecutable(cmakePath);
    cmake->setDisplayName(tr("Link Motion SDK cmake (%1-%2)")
                          .arg(target.architecture)
                          .arg(target.containerName));
    return cmake;
}

#if 0

static bool lessThanToolchain (const LinkMotionToolChain* left, const LinkMotionToolChain* right)
{
    const LmTargetTool::Target &leftTarget = left->lmTarget();
    const LmTargetTool::Target &rightTarget = right->lmTarget();
    return UbuntuClickFrameworkProvider::caseInsensitiveFWLessThan(leftTarget.framework, rightTarget.framework);
}

void LinkMotionKitManager::autoCreateKit(UbuntuDevice::Ptr device)
{
    ProjectExplorer::Abi requiredAbi = LinkMotionToolChain::architectureNameToAbi(device->architecture());
    if(requiredAbi.isNull()) {
        QMessageBox::warning(Core::ICore::mainWindow(),
                             tr("Unknown device architecture"),
                             tr("Kit autocreation for %1 is not supported!")
                             .arg(device->architecture()));
        return;
    }

    if(device->framework().isEmpty()) {
        QMessageBox::warning(Core::ICore::mainWindow(),
                             tr("Device framework is unknown."),
                             tr("The supported framework of the device is not known, please make sure to redetect the device features."));
        return;
    }

    QList<LinkMotionToolChain*> toolchains = LinkMotionToolChains();

    auto findCompatibleTc = [&](){
        QList<LinkMotionToolChain *> perfectMatches;
        QList<LinkMotionToolChain *> fuzzyMatches;
        if(toolchains.size() > 0) {
            qSort(toolchains.begin(),toolchains.end(),lessThanToolchain);

            for( int i = toolchains.size() -1; i >= 0; i-- ) {
                LinkMotionToolChain* tc = toolchains[i];

                if (tc->lmTarget().framework != device->framework())
                    continue;

                if( tc->targetAbi() == requiredAbi ) {
                    perfectMatches.append(tc);
                    continue;
                }

                //the abi is compatible but not exactly the same
                if(tc->targetAbi().isCompatibleWith(requiredAbi))
                    fuzzyMatches.append(tc);
            }
        }
        return qMakePair(perfectMatches, fuzzyMatches);
    };

    LinkMotionToolChain* match = nullptr;
    while (!match) {

        //search a tk with a compatible arch
        QPair<QList<LinkMotionToolChain *>, QList<LinkMotionToolChain *> > matches = findCompatibleTc();
        QList<LinkMotionToolChain *> perfect = matches.first;
        QList<LinkMotionToolChain *> fuzzy = matches.second;

        auto getToolchain = [](const QList<LinkMotionToolChain *> &toolchains, bool *ok) {
            QStringList names;
            foreach (const LinkMotionToolChain *curr, toolchains) {
                names.append(curr->displayName());
            }

            if (names.empty())
                return static_cast<LinkMotionToolChain *>(nullptr);

            QString selection = QInputDialog::getItem(Core::ICore::mainWindow(), qApp->applicationName(),
                                                      tr("There are multiple compatible Toolchains available, please select one:"),
                                                      names, 0, false, ok);
            if (!ok) {
                return static_cast<LinkMotionToolChain *>(nullptr);
            }

            return toolchains.at(names.indexOf(selection));
        };

        if (perfect.size() > 0) {
            if (perfect.size() == 1) {
                match = perfect.first();
                break;
            } else {
                bool ok = true;
                match = getToolchain(perfect, &ok);
                if (!ok)
                    return;

                break;
            }
        } else if (fuzzy.size() > 0) {
            if (fuzzy.size() == 1) {
                match = fuzzy.first();
                break;
            } else {
                bool ok = true;
                match = getToolchain(fuzzy, &ok);
                if (!ok)
                    return;

                break;
            }
        }

        if (!match) {
            //create target
            int choice = QMessageBox::question(Core::ICore::mainWindow(),
                                  tr("No target available"),
                                  tr("There is no compatible target available on your system, do you want to create it now?"));

            if(choice == QMessageBox::Yes) {
                if(!UbuntuClickDialog::createClickChrootModal(false, device->architecture(), device->framework()))
                    return;
                toolchains = LinkMotionToolChains();
            } else
                return;
        }
    }

    ProjectExplorer::Kit* newKit = createKit(match);
    if(newKit) {
        fixKit(newKit);

        newKit->setUnexpandedDisplayName(tr("%1 (GCC %2-%3-%4)")
                                        .arg(device->displayName())
                                        .arg(match->lmTarget().architecture)
                                        .arg(match->lmTarget().framework)
                                        .arg(match->lmTarget().containerName));

        ProjectExplorer::DeviceKitInformation::setDevice(newKit,device);
        ProjectExplorer::KitManager::registerKit(newKit);
    }
}
#endif

void LinkMotionKitManager::autoDetectKits()
{
    // having a empty toolchains list will remove all autodetected kits for ubuntu
    // exactly what we want in that case
    QList<LinkMotionToolChain *> toolchains = linkMotionToolChains();

    QList<ProjectExplorer::Kit *> existingKits;
    foreach (ProjectExplorer::Kit *k, ProjectExplorer::KitManager::kits()) {
        if (k->isSdkProvided())
            continue;

        ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k, ProjectExplorer::Constants::CXX_LANGUAGE_ID);
        if (tc && tc->typeId() != Constants::LM_TARGET_TOOLCHAIN_ID)
            continue;

        //@TODO check for ubuntu device information
        if(debug) qDebug()<<"Found possible Ubuntu Kit: "<<k->displayName();
        existingKits << k;
    }

    // create new kits
    QList<ProjectExplorer::Kit *> newKits;
    foreach (LinkMotionToolChain *tc, toolchains) {
        ProjectExplorer::Kit* newKit = createKit(tc);
        newKit->makeSticky();
        newKits << newKit;
    }

    //remove already existing kits
    for (int i = existingKits.count() - 1; i >= 0; --i) {
        ProjectExplorer::Kit *existingKit = existingKits.at(i);
        for (int j = 0; j < newKits.count(); ++j) {
            ProjectExplorer::Kit *newKit = newKits.at(j);
            if (equalKits(existingKit, newKit)) {
                // Kit is already registered, nothing to do
                ProjectExplorer::Kit *oldKit = existingKits.takeAt(i);
                oldKit->blockNotification();
                oldKit->makeSticky();

                //make sure kit has all required informations
                fixKit(oldKit);
                oldKit->unblockNotification();

                newKits.removeAt(j);
                ProjectExplorer::KitManager::deleteKit(newKit);
                j = newKits.count();
            }
        }
    }

    //all kits remaining need to be removed if they don't have all informations
    foreach (ProjectExplorer::Kit *k, existingKits) {
        ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k, ProjectExplorer::Constants::CXX_LANGUAGE_ID);
        CMakeProjectManager::CMakeTool* cmake = CMakeProjectManager::CMakeKitInformation::cmakeTool(k);
        if (tc && tc->typeId() == Constants::LM_TARGET_TOOLCHAIN_ID
                && cmake
                && cmake->isValid()) {
            fixKit(k);

            //existing targets are not autodetected anymore
            k->makeUnSticky();
            k->setAutoDetected(false);
        } else {
            //has not all informations, go away
            ProjectExplorer::KitManager::deregisterKit(k);
        }
    }

    foreach (ProjectExplorer::Kit *kit, newKits) {
        LinkMotionToolChain *tc = static_cast<LinkMotionToolChain *>(ProjectExplorer::ToolChainKitInformation::toolChain(kit, ProjectExplorer::Constants::CXX_LANGUAGE_ID));
        kit->setUnexpandedDisplayName(tr("Link Motion SDK for %1 (GCC %2)")
                                      .arg(tc->lmTarget().containerName)
                                      .arg(tc->lmTarget().architecture));
        ProjectExplorer::KitManager::registerKit(kit);
        fixKit(kit);
    }

    static bool cmakeUpdaterSet = false;
    if (!cmakeUpdaterSet) {

        cmakeUpdaterSet = true;

        auto cmakeUpdater = [](const Core::Id &id){
            CMakeProjectManager::CMakeTool *tool = CMakeProjectManager::CMakeToolManager::findById(id);
            if (!tool)
                return;

            QString basePath = Settings::settingsPath().toString();
            if (tool->cmakeExecutable().toString().startsWith(basePath)) {
                qDebug()<<"Setting mapper to "<<tool->displayName();
                tool->setPathMapperFactory(&LmTargetTool::mapIncludePathsForCMakeFactory);
            } else {
                qDebug()<<"Unsetting mapper from "<<tool->displayName();
                tool->setPathMapperFactory(CMakeProjectManager::CMakeTool::PathMapperFactory());
            }
        };

        connect(CMakeProjectManager::CMakeToolManager::instance(), &CMakeProjectManager::CMakeToolManager::cmakeAdded,
                cmakeUpdater);
        connect(CMakeProjectManager::CMakeToolManager::instance(), &CMakeProjectManager::CMakeToolManager::cmakeRemoved,
                cmakeUpdater);
    }
}

/*!
 * \brief LinkMotionKitManager::createKit
 * Creates a new Kit for the LM toolchain and sets default
 * values
 */
ProjectExplorer::Kit *LinkMotionKitManager::createKit(LinkMotionToolChain *tc)
{
    //@TODO find a qt version
    ProjectExplorer::Kit* newKit = new ProjectExplorer::Kit;
    newKit->setAutoDetected(false); //let the user delete that stuff
    //newKit->setIconPath(Utils::FileName::fromString(QLatin1String(Constants::UBUNTU_ICON)));
    ProjectExplorer::ToolChainKitInformation::setToolChain(newKit, tc);

    CMakeProjectManager::CMakeTool *cmake = createOrFindCMakeTool(tc);
    if (cmake) {
        cmake->setPathMapperFactory(&LmTargetTool::mapIncludePathsForCMakeFactory);
        CMakeProjectManager::CMakeKitInformation::setCMakeTool(newKit, cmake->id());
    }

    ProjectExplorer::SysRootKitInformation::setSysRoot(newKit,Utils::FileName::fromString(LmTargetTool::targetBasePath(tc->lmTarget())));

    createOrFindDeviceAndType(newKit, tc);

    //@TODO add gdbserver support
    QtSupport::QtKitInformation::setQtVersion(newKit, createOrFindQtVersion(tc));
    return newKit;
}



/*!
 * \brief LinkMotionKitManager::createOrFindDebugger
 * Tries to find a already existing ubuntu debugger, if it can not find one
 * it is registered and returned
 */
QVariant LinkMotionKitManager::createOrFindDebugger(const Utils::FileName &path)
{
    if(path.isEmpty())
        return QVariant();

    QList<Debugger::DebuggerItem> debuggers = Debugger::DebuggerItemManager::debuggers();
    foreach(const Debugger::DebuggerItem& debugger,debuggers) {
        if(debugger.command() == path) {
            return debugger.id();
        }
    }


    Debugger::DebuggerItem debugger;
    debugger.setCommand(path);
    debugger.setEngineType(Debugger::GdbEngineType);
    debugger.setUnexpandedDisplayName(tr("Link Motion SDK Debugger"));
    debugger.setAutoDetected(true);
    //multiarch debugger
    ProjectExplorer::Abi abi(ProjectExplorer::Abi::UnknownArchitecture
                             ,ProjectExplorer::Abi::LinuxOS
                             ,ProjectExplorer::Abi::GenericLinuxFlavor
                             ,ProjectExplorer::Abi::UnknownFormat
                             ,0);
    debugger.setAbi(abi);
    return Debugger::DebuggerItemManager::registerDebugger(debugger);
}

/*!
 * \brief LinkMotionKitManager::fixKit
 * Tries to fix a Kit if there is missing information
 */
void LinkMotionKitManager::fixKit(ProjectExplorer::Kit *k)
{
    k->setAutoDetected(false);

    LinkMotionToolChain* tc = static_cast<LinkMotionToolChain *> (ProjectExplorer::ToolChainKitInformation::toolChain(k,  ProjectExplorer::Constants::CXX_LANGUAGE_ID));
    if(!tc) {
        return;
    }

    //make sure we have the multiarch debugger
    QVariant dId = createOrFindDebugger(tc->suggestedDebugger());
    const Debugger::DebuggerItem *debugger = Debugger::DebuggerKitInformation::debugger(k);
    if(!debugger) {
        if(dId.isValid())
            Debugger::DebuggerKitInformation::setDebugger(k,dId);
    } else if (debugger->id() != dId){
        if(dId.isValid())
            Debugger::DebuggerKitInformation::setDebugger(k,dId);
    }

    if(ProjectExplorer::SysRootKitInformation::sysRoot(k).isEmpty()) {
        ProjectExplorer::SysRootKitInformation::setSysRoot(k,Utils::FileName::fromString(LmTargetTool::targetBasePath(tc->lmTarget())));
    }

    //make sure we point to a ubuntu device
    Core::Id devId = ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(k);
    bool devValid        = devId.isValid(); //invalid type
    bool hasLMDevType = devId.toString().startsWith(QLatin1String(Constants::LM_DEVICE_TYPE_ID));/* ||
            devId.toString().startsWith(QLatin1String(Constants::UBUNTU_CONTAINER_DEVICE_TYPE_ID)); //kit has a wrong device type */

    if (!devValid || !hasLMDevType) {
        createOrFindDeviceAndType(k, tc);
    }

    //values the user can change
    k->setSticky(ProjectExplorer::DeviceKitInformation::id(),false);
    k->setSticky(Debugger::DebuggerKitInformation::id(),false);

    //values the user cannot change
    k->setSticky(ProjectExplorer::SysRootKitInformation::id(),true);
    k->setMutable(ProjectExplorer::SysRootKitInformation::id(),false);

    //make sure we use a ubuntu Qt version
    LinkMotionQtVersion *qtVer = createOrFindQtVersion(tc);
    QtSupport::QtKitInformation::setQtVersion(k, qtVer);

    //make sure we use a link motion cmake
    CMakeProjectManager::CMakeTool *cmake = createOrFindCMakeTool(tc);
    if(cmake) {
        CMakeProjectManager::CMakeConfig  conf{
            CMakeProjectManager::CMakeConfigItem("QT_QMAKE_EXECUTABLE",  qtVer->remoteQMakeCommand().toUtf8()),
            CMakeProjectManager::CMakeConfigItem("CMAKE_CXX_COMPILER",  tc->remoteCompilerCommand().toUtf8())
        };

        cmake->setPathMapperFactory(&LmTargetTool::mapIncludePathsForCMakeFactory);
        CMakeProjectManager::CMakeKitInformation::setCMakeTool(k, cmake->id());
        CMakeProjectManager::CMakeConfigurationKitInformation::setConfiguration(k , conf);
    }

}

} // namespace Internal
} // namespace Ubuntu
