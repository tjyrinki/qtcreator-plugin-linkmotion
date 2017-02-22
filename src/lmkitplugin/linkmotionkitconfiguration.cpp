/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionkitconfiguration.h"
#include <QDebug>
#include <QDirIterator>
#include <QVersionNumber>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kitinformation.h>
#include <qtsupport/baseqtversion.h>
#include <qtsupport/qtversionmanager.h>
#include <projectexplorer/devicesupport/devicemanager.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kitinformation.h>
#include <debugger/debuggerkitinformation.h>
#include <qtsupport/qtkitinformation.h>

#include "../lmdeviceplugin/linkmotiondeviceplugin_constants.h"
#include "linkmotionkitplugin_constants.h"
#include "linkmotionoskitinformation.h"
#include "linkmotionqtversion.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionKitConfiguration::LinkMotionKitConfiguration(QObject *parent) : QObject(parent) {
    qDebug() << Q_FUNC_INFO;
    connect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsLoaded()),this,SLOT(autoConfigure()));
}

void LinkMotionKitConfiguration::autoConfigure() {
    qDebug() << Q_FUNC_INFO;
    disconnect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsLoaded()));
    // remove old autodetections

    bool foundQtVersion = false;

    Utils::FileName qmake;
    {
        // Find qmake binary of the highest Qt version in LINKMOTION_QT_DIR
        QVersionNumber qtVersion;

        QDirIterator it(LinkMotion::Constants::LINKMOTION_QT_DIR,
                        QDir::Dirs | QDir::NoDotAndDotDot);
        while (it.hasNext()) {
            QDir dir = it.next();

            auto version = QVersionNumber::fromString(dir.dirName());
            auto file = Utils::FileName::fromString(
                    dir.absoluteFilePath(QStringLiteral("bin/qmake")));

            if (version > qtVersion && file.exists()) {
                qtVersion = version;
                qmake = file;
            }
        }
    }

    if (qmake.isNull()) {
        qWarning() << Q_FUNC_INFO << "LinkMotion Qt is not installed!";
        return;
    }

    // check if we have already configured the QtSupport
    foreach(QtSupport::BaseQtVersion* qtVersion, QtSupport::QtVersionManager::unsortedVersions()) {
        if (qtVersion->autodetectionSource().compare(LinkMotion::Constants::LINKMOTION_AUTODECTION_SOURCE_ID) == 0) {
            foundQtVersion = true;
            m_qtVersion = (LinkMotionQtVersion*)qtVersion;
        }
    }

    // add the new one
    if (!foundQtVersion) {
        qDebug() << "Registering LinkMotion Qt..";
        m_qtVersion = new LinkMotionQtVersion(qmake,true,LinkMotion::Constants::LINKMOTION_AUTODECTION_SOURCE_ID);
        QtSupport::QtVersionManager::addVersion(m_qtVersion);
    }

    // register debuggers
    QList<ProjectExplorer::Abi> linkMotionQtAbis = m_qtVersion->qtAbis();
    foreach(ProjectExplorer::Abi abi, linkMotionQtAbis) {
        LinkMotionDebuggerItem debugger(abi);
        Debugger::DebuggerItemManager::registerDebugger((Debugger::DebuggerItem)debugger);

    }
}

void LinkMotionKitConfiguration::initialize() {
    qDebug() << Q_FUNC_INFO;

    disconnect(ProjectExplorer::KitManager::instance(),SIGNAL(kitsLoaded()));
    disconnect(ProjectExplorer::KitManager::instance(),SIGNAL(kitsChanged()));
    if (!ProjectExplorer::KitManager::isLoaded()) {
        qDebug() << "KitManager is not ready";
        connect(ProjectExplorer::KitManager::instance(),SIGNAL(kitsLoaded()),this,SLOT(initialize()));
        return;
    }

    disconnect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsLoaded()));
    disconnect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsChanged(QList<int>,QList<int>,QList<int>)));
    if (!QtSupport::QtVersionManager::isLoaded()) {
        qDebug() << Q_FUNC_INFO << "QtVersionmanager is not ready";
        connect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsLoaded()),this,SLOT(initialize()));
        return;
    }
    connect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsChanged(QList<int>,QList<int>,QList<int>)),this,SLOT(initialize()));

    // lets get a list of automatically added Kits
    qDebug() << "Detecting kits..";
    QList<ProjectExplorer::Kit*> existingKits;
    foreach (ProjectExplorer::Kit *k, ProjectExplorer::KitManager::kits()) {
        qDebug() << "Detected" << k->displayName();
        if (ProjectExplorer::DeviceTypeKitInformation::deviceTypeId(k) != Core::Id(LinkMotion::Constants::LINKMOTION_DEVICE_TYPE)) {
            qDebug() << " * invalid device type";
            continue;
        }
        if (!k->isAutoDetected()) {
            qDebug() << " * was not autodetected";
            continue;
        }
        if (!k->displayName().contains("LinkMotion")) {
            qDebug() << " * was not a linkmotion kit";
            continue;
        }

        if (k->displayName() == QStringLiteral("LinkMotion Emulator Kit")) {
            qDebug() << " * removing legacy LinkMotion Emulator Kit";
            // Saved configuration contains old LinkMotion kit used before
            // separate kits for Auto and IVI OSes replaced it. Remove the
            // legacy kit.
            ProjectExplorer::KitManager::deregisterKit(k);
            continue;
        }

        qDebug() << " => accepted " << k->displayName();
        existingKits << k;
    }

    // lets get a list of QtVersions which are available
    QHash<ProjectExplorer::Abi, QList<QtSupport::BaseQtVersion *> > linkMotionQtVersionsAvailable;
    qDebug() << "Detecting available Qt versions..";

    foreach(QtSupport::BaseQtVersion* qtVersion, QtSupport::QtVersionManager::unsortedVersions()) {
        qDebug() << "Detected" << qtVersion->type() << qtVersion->displayName();
        if (qtVersion->type() != QLatin1String(LinkMotion::Constants::LINKMOTION_LINUX_QT)) {
            continue;
        }
        if (!qtVersion->displayName().contains("LinkMotion")) {
            continue;
        }
        QList<ProjectExplorer::Abi> linkMotionQtAbis = qtVersion->qtAbis();
        qDebug() << linkMotionQtAbis.count();
        foreach(ProjectExplorer::Abi abi, linkMotionQtAbis) {
            linkMotionQtVersionsAvailable[abi].append(qtVersion);
            qDebug() << "Added" << qtVersion->description();
        }
    }

    // lets detect LinkMotion devices
    qDebug() << "detecting LinkMotion devices..";
    ProjectExplorer::DeviceManager* dm = ProjectExplorer::DeviceManager::instance();
    disconnect(dm,SIGNAL(devicesLoaded()));
    if (!dm->isLoaded()) {
        connect(dm,SIGNAL(devicesLoaded()),this,SLOT(initialize()));
        return;
    }
    connect(dm,SIGNAL(updated()),this,SLOT(initialize()));

    qDebug() << dm->deviceCount() << dm->defaultDevice(Core::Id(LinkMotion::Constants::LINKMOTION_IVIOS_DEVICE_ID));
    for (int i=0; i<dm->deviceCount(); i++) {
        qDebug() << "Detected:" << dm->deviceAt(i)->displayName() << dm->deviceAt(i)->id();
    }

    ProjectExplorer::IDevice::ConstPtr autoDevice = dm->find(Constants::LINKMOTION_AUTOOS_DEVICE_ID);
    ProjectExplorer::IDevice::ConstPtr iviDevice = dm->find(Constants::LINKMOTION_IVIOS_DEVICE_ID);

    if (iviDevice.isNull() || autoDevice.isNull()) {
        qDebug() << "Unable to detect LinkMotion device!";
        foreach(ProjectExplorer::Kit* k, existingKits) {
            qDebug() << "deregistering kits" << k->displayName();
            ProjectExplorer::KitManager::deregisterKit(k);
        }
        return;
    }

    qDebug() << "Valid LinkMotion Auto OS device found" << autoDevice->displayName();
    qDebug() << "Valid LinkMotion IVI OS device found" << iviDevice->displayName();

    ProjectExplorer::KitManager::registerKitInformation(new LinkMotionOsKitInformation);

    // lets register our qt kits
    foreach(ProjectExplorer::Abi abi, linkMotionQtVersionsAvailable.keys()) {
        QList<QtSupport::BaseQtVersion*> availableQtVersionsForAbi = linkMotionQtVersionsAvailable[abi];

        foreach(QtSupport::BaseQtVersion* qt, availableQtVersionsForAbi) {
            registerKit(Constants::LINKMOTION_AUTOOS_KIT_ID,
                        QStringLiteral("LinkMotion Auto OS Emulator Kit"), qt,
                        autoDevice, existingKits);
            registerKit(Constants::LINKMOTION_IVIOS_KIT_ID,
                        QStringLiteral("LinkMotion IVI OS Emulator Kit"), qt,
                        iviDevice, existingKits);
        }
    }


}

void LinkMotionKitConfiguration::registerKit(Core::Id id,
                                             const QString &displayName,
                                             QtSupport::BaseQtVersion* qt,
                                             ProjectExplorer::IDevice::ConstPtr dev,
                                             QList<ProjectExplorer::Kit*> &existingKits)
{
    ProjectExplorer::Kit* kit = new ProjectExplorer::Kit(id);
    kit->setAutoDetected(true);
    kit->setUnexpandedDisplayName(displayName);
    kit->makeSticky();

    ProjectExplorer::DeviceTypeKitInformation::setDeviceTypeId(kit, Core::Id(LinkMotion::Constants::LINKMOTION_DEVICE_TYPE));
    QtSupport::QtKitInformation::setQtVersion(kit, qt);
    ProjectExplorer::DeviceKitInformation::setDevice(kit, dev);

    Utils::FileName gdb = Utils::FileName::fromLatin1(LinkMotion::Constants::LINKMOTION_GDB_COMMAND);
    Debugger::DebuggerItem *gdbDebuggerItem = Debugger::DebuggerItemManager::findByCommand(gdb);
    if (gdbDebuggerItem) {
        Debugger::DebuggerKitInformation::setDebugger(kit, gdbDebuggerItem->id());
    } else {
        qWarning() << "Could not find Link Motion gdb!";
    }

    qDebug() << "Registering kit" << kit->displayName();

    foreach(ProjectExplorer::Kit* k, existingKits) {
        if (k->displayName().compare(kit->displayName()) == 0) {
            // Left on purpose, this is used to remove the old kits as it might
            // be that there is an older configuration with malfunctioning stuff.
            ProjectExplorer::KitManager::deregisterKit(k);
            existingKits.removeOne(k);
        }
    }

    ProjectExplorer::KitManager::registerKit(kit);
}
