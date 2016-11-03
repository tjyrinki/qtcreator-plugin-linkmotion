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
    Utils::FileName qmake = Utils::FileName::fromLatin1(LinkMotion::Constants::LINKMOTION_QT_QMAKE);
    if (!qmake.exists()) {
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
    //connect(ProjectExplorer::KitManager::instance(),SIGNAL(kitsChanged()),this,SLOT(initialize()));

    disconnect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsLoaded()));
    disconnect(QtSupport::QtVersionManager::instance(),SIGNAL(qtVersionsChanged(QList<int>,QList<int>,QList<int>)));
    if (!QtSupport::QtVersionManager::isLoaded()) {
        qDebug() << Q_FUNC_INFO << "Qtversionmanager is not ready";
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

    ProjectExplorer::IDevice::ConstPtr dev = dm->find(Core::Id(LinkMotion::Constants::LINKMOTION_DEVICE_ID));
    qDebug() << dm->deviceCount() << dm->defaultDevice(Core::Id(LinkMotion::Constants::LINKMOTION_DEVICE_ID));
    for (int i=0; i<dm->deviceCount(); i++) {
        qDebug() << "Detected:" << dm->deviceAt(i)->displayName() << dm->deviceAt(i)->id();
    }
    if (dev.isNull()) {
        qDebug() << "Unable to detect LinkMotion device!";
        foreach(ProjectExplorer::Kit* k, existingKits) {
            qDebug() << "deregistering kits" << k->displayName();
            ProjectExplorer::KitManager::deregisterKit(k);
        }
        return;
    }

    qDebug() << "Valid LinkMotion device found" << dev->displayName();

    // lets register our qt kits
    foreach(ProjectExplorer::Abi abi, linkMotionQtVersionsAvailable.keys()) {
        QList<QtSupport::BaseQtVersion*> availableQtVersionsForAbi = linkMotionQtVersionsAvailable[abi];

        foreach(QtSupport::BaseQtVersion* qt, availableQtVersionsForAbi) {
            Utils::FileName gdb = Utils::FileName::fromLatin1(LinkMotion::Constants::LINKMOTION_GDB_COMMAND);
            QVariant debuggerId = Debugger::DebuggerItemManager::findByCommand(gdb)->id();
            ProjectExplorer::Kit* lmkit = new ProjectExplorer::Kit(Core::Id(LinkMotion::Constants::LINKMOTION_KIT_ID));
            lmkit->setAutoDetected(true);
            ProjectExplorer::DeviceTypeKitInformation::setDeviceTypeId(lmkit, Core::Id(LinkMotion::Constants::LINKMOTION_DEVICE_TYPE));
            QtSupport::QtKitInformation::setQtVersion(lmkit, qt);
            ProjectExplorer::DeviceKitInformation::setDevice(lmkit, dev);
            Debugger::DebuggerKitInformation::setDebugger(lmkit, debuggerId);
            lmkit->setUnexpandedDisplayName(QStringLiteral("LinkMotion Emulator Kit"));
            lmkit->makeSticky();
            qDebug() << "Registering kit" << lmkit->displayName();

            //bool isAlreadyRegistered = false;
            foreach(ProjectExplorer::Kit* k, existingKits) {
                if (k->displayName().compare(lmkit->displayName()) == 0) {
                    // left on purpose, this can be used in the future to remove the old kits
                    // if we create changes above and we need to recreate the kit which uses the same name
                    ProjectExplorer::KitManager::deregisterKit(k);
                    //isAlreadyRegistered = true;
                }
            }

            //if (!isAlreadyRegistered) {
            ProjectExplorer::KitManager::registerKit(lmkit);
           // } else {
            //    qDebug() << "was already registered";
            //}
        }
    }


}
