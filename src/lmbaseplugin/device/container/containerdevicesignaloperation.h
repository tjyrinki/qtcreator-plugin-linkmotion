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

#ifndef LM_INTERNAL_CONTAINERDEVICESIGNALOPERATION_H
#define LM_INTERNAL_CONTAINERDEVICESIGNALOPERATION_H

#include "containerdevice.h"

#include <QProcess>

namespace LmBase {
namespace Internal {

class ContainerDeviceSignalOperation : public ProjectExplorer::DeviceProcessSignalOperation
{
    Q_OBJECT
public:
    ~ContainerDeviceSignalOperation();
    void killProcess(int pid);
    void killProcess(const QString &filePath);
    void interruptProcess(int pid);
    void interruptProcess(const QString &filePath);

private:
    void killProcessSilently(int pid);
    void interruptProcessSilently(int pid);

    void appendMsgCannotKill(int pid, const QString &why);
    void appendMsgCannotInterrupt(int pid, const QString &why);

protected:
    explicit ContainerDeviceSignalOperation(ContainerDevice::ConstPtr dev);

    friend class ContainerDevice;

    void sendSignal(int pid, int signal);
protected slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitState);
    void processError(QProcess::ProcessError procErr);

private:
    ContainerDevice::ConstPtr m_dev;
};

} // namespace Internal
} // namespace LmBase

#endif // LM_INTERNAL_CONTAINERDEVICESIGNALOPERATION_H
