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

namespace LmBase {
namespace Constants {

const QString LM_TARGET_TOOL = QStandardPaths::findExecutable(QStringLiteral("lmsdk-target"));
const QString LM_TARGET_WRAPPER = QStandardPaths::findExecutable(QStringLiteral("lmsdk-wrapper"));

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
const char LM_CONTAINER_DEVICE_TYPE_ID[] = "UbuntuProjectManager.LocalDeviceTypeId.";
const char LM_CONTAINER_DEPLOY_PUBKEY_SCRIPT[] = "%0/container_publickey_deploy";
const char LM_TASK_CATEGORY_DEVICE [] = "Task.Category.Ubuntu.ContainerDevice";



}}


#endif

