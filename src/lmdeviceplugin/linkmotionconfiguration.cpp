/*####################################################################
#
# This file is part of the LinkMotion Device plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionconfiguration.h"

#include <projectexplorer/devicesupport/devicemanager.h>
#include "linkmotiondeviceplugin_constants.h"
#include "linkmotiondevice.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

static LinkMotionConfiguration *m_instance = 0;

LinkMotionConfiguration::LinkMotionConfiguration(QObject *parent) : QObject(parent)
{

}


QObject *LinkMotionConfiguration::instance()
{
    return m_instance;
}


void LinkMotionConfiguration::initialize()
{
    m_instance = new LinkMotionConfiguration(0);
    m_instance->updateDevices();
}

void LinkMotionConfiguration::updateDevices() {
    ProjectExplorer::DeviceManager *devManager = ProjectExplorer::DeviceManager::instance();
    Core::Id devId = Constants::LINKMOTION_DEVICE_ID;
    ProjectExplorer::IDevice::ConstPtr dev = devManager->find(devId);
    if (dev.isNull()) {
        dev = ProjectExplorer::IDevice::ConstPtr(new LinkMotionDevice(devId.toString()));
        devManager->addDevice(dev);
    }
}
