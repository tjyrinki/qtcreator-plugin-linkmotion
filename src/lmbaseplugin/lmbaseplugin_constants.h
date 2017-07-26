/*
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

#ifndef LMBASEPLUGIN_CONSTANTS_H
#define LMBASEPLUGIN_CONSTANTS_H

#include <QString>
#include <QStandardPaths>

#include <coreplugin/icore.h>

/*the file cred_key.h contains the encryption key used to cypher passwords in the settings file
  it is intentionally not part of the repository, but it contains code similar to:

    #ifndef LM_CRED_KEY_H
    #define LM_CRED_KEY_H

    #include <qglobal.h>

    namespace LmBase {
    namespace Constants {

       const quint64 LM_CREDENTIALS = Q_UINT64_C(0xd52472ee500973d5);

    }}

    #endif

*/
#include "cred_key.h"

namespace LmBase {
namespace Constants {

const QString LM_RESOURCE_PATH = Core::ICore::resourcePath();
const QString LM_SCRIPTPATH = LM_RESOURCE_PATH + QLatin1String("/linkmotion/scripts");

const char LM_LOGO_ROUND[] = ":/linkmotionbase/icons/LM_logo.png";


const char LM_SUDO_BINARY[]   = "pkexec";

//Build support
const char LM_TARGET_TOOLCHAIN_ID[]   = "LinkMotion.ToolChain.ID";

//Qtversion support
const char LM_QTVERSION_TYPE[]   = "LinkMotion.QtVersion.ID";
const char LM_PLATFORM_NAME[]    = "Link Motion";
const char LM_PLATFORM_NAME_TR[] = QT_TRANSLATE_NOOP("LinkMotionBasePlugin", "Link Motion");

//Devicesupport
const char LM_DEVICE_TYPE_ID[] = "LinkMotion.DeviceTypeId";

//Container devices
const int  LM_DESKTOP_PORT_START = 40000;
const int  LM_DESKTOP_PORT_END = 41000;
const char LM_CONTAINER_DEVICE_TYPE_ID[] = "LinkMotion.LocalDeviceTypeId.";
const char LM_CONTAINER_DEPLOY_PUBKEY_SCRIPT[] = "%0/container_publickey_deploy";
const char LM_TASK_CATEGORY_DEVICE [] = "Task.Category.LinkMotion.ContainerDevice";
const char LM_DEVICE_SSHIDENTITY[] = "lmdevice_id_rsa";
const char LM_LOCAL_DEPLOYCONFIGURATION_ID[] = "LinkMotion.LocalDeployConfigurationId";





}}


#endif

