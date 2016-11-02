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
    foreach(QtSupport::BaseQtVersion* qtVersion, QtSupport::QtVersionManager::unsortedVersions()) {
        if (qtVersion->autodetectionSource().compare(LinkMotion::Constants::LINKMOTION_AUTODECTION_SOURCE_ID) == 0) {
            QtSupport::QtVersionManager::removeVersion(qtVersion);
        }
    }

    // add the new one
    Utils::FileName qmake = Utils::FileName::fromLatin1(LinkMotion::Constants::LINKMOTION_QT_QMAKE);
    if (!qmake.exists()) {
        qWarning() << Q_FUNC_INFO << "LinkMotion Qt is not installed!";
        return;
    }
    QtSupport::QtVersionManager::addVersion(new LinkMotionQtVersion(qmake,true,LinkMotion::Constants::LINKMOTION_AUTODECTION_SOURCE_ID));
}

void LinkMotionKitConfiguration::initialize() {
    qDebug() << Q_FUNC_INFO;

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
        if (!k->isSdkProvided()) {
            qDebug() << " * was not sdk provided";
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
    ProjectExplorer::IDevice::ConstPtr dev = dm->find(Core::Id(LinkMotion::Constants::LINKMOTION_DEVICE_ID));
    if (dev.isNull()) {
        qDebug() << "Unable to detect LinkMotion device!";
        foreach(ProjectExplorer::Kit* k, existingKits) {
            qDebug() << "deregistering kits" << k->displayName();
            ProjectExplorer::KitManager::deregisterKit(k);
        }
        return;
    }

    // lets register our qt kit

}
