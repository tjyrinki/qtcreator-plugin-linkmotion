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

#ifndef LM_INTERNAL_CONTAINERDEVICEPROCESS_H
#define LM_INTERNAL_CONTAINERDEVICEPROCESS_H

#include "containerdevice.h"

#include <remotelinux/linuxdeviceprocess.h>
#include <utils/environment.h>

#include <QTemporaryDir>

namespace LmBase {
namespace Internal {

class ContainerDevice;

class ContainerDeviceProcess: public RemoteLinux::LinuxDeviceProcess
{
    Q_OBJECT
public:
    ContainerDeviceProcess(const QSharedPointer<const ProjectExplorer::IDevice> &device, QObject *parent = 0);
    ~ContainerDeviceProcess();

    // DeviceProcess interface
    void start(const ProjectExplorer::Runnable &runnable);
    virtual void interrupt() override { doSignal(2); }
    virtual void terminate() override { doSignal(15); }
    virtual void kill() override { doSignal(9); }

    void doSignal (const int sig);
private:

    void cleanupWestonProcess ();

    // SshDeviceProcess interface
    virtual QString fullCommandLine(const ProjectExplorer::StandardRunnable &) const override;
    QString m_pidFile;
    QProcess *m_westonProc = nullptr;
    QTemporaryDir *m_westonDir = nullptr;
};

} // namespace Internal
} // namespace LmBase

#endif // LM_INTERNAL_CONTAINERDEVICEPROCESS_H
