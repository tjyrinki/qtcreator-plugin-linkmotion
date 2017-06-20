/*
 * Copyright 2016 Canonical Ltd.
 * Copyright 2017 Link Motion Oy
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

#ifndef LM_INTERNAL_CONTAINERPROCESSLIST_H
#define LM_INTERNAL_CONTAINERPROCESSLIST_H

#include "containerdevice.h"
#include <projectexplorer/devicesupport/deviceprocesslist.h>


namespace LmBase {
namespace Internal {

class ContainerProcessList : public ProjectExplorer::DeviceProcessList
{
    Q_OBJECT

public:
    ContainerProcessList(const ContainerDevice::ConstPtr &device, QObject *parent = 0);

    static QList<ProjectExplorer::DeviceProcessItem> getContainerProcesses(const QString &container);

private:
    void doUpdate();
    void doKillProcess(const ProjectExplorer::DeviceProcessItem &process);

private slots:
    void handleUpdate();
    void reportDelayedKillStatus(const QString &errorMessage);

private:
    ProjectExplorer::DeviceProcessSignalOperation::Ptr m_sig;

};

} // namespace Internal
} // namespace LmBase

#endif // LM_INTERNAL_CONTAINERPROCESSLIST_H
