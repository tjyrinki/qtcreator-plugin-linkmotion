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

#include "containerdevicesignaloperation.h"
#include "containerprocesslist.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>
#include <lmbaseplugin/lmbaseplugin.h>

#include <QDebug>

#include <signal.h>

namespace LmBase {
namespace Internal {

ContainerDeviceSignalOperation::ContainerDeviceSignalOperation(ContainerDevice::ConstPtr dev)
    : m_dev(dev)
{

}

ContainerDeviceSignalOperation::~ContainerDeviceSignalOperation()
{
}

void ContainerDeviceSignalOperation::killProcess(int pid)
{
    killProcessSilently(pid);
}

void ContainerDeviceSignalOperation::killProcess(const QString &filePath)
{
    m_errorMessage.clear();
    foreach (const ProjectExplorer::DeviceProcessItem &process, ContainerProcessList::getContainerProcesses(m_dev->containerName())) {
        if (process.cmdLine == filePath) {
            killProcessSilently(process.pid);
            return;
        }
    }
    emit finished(m_errorMessage);
}

void ContainerDeviceSignalOperation::interruptProcess(int pid)
{
    m_errorMessage.clear();
    interruptProcessSilently(pid);
}

void ContainerDeviceSignalOperation::interruptProcess(const QString &filePath)
{
    m_errorMessage.clear();
    foreach (const ProjectExplorer::DeviceProcessItem &process, ContainerProcessList::getContainerProcesses(m_dev->containerName())) {
        if (process.cmdLine == filePath) {
            interruptProcessSilently(process.pid);
            return;
        }
    }
    emit finished(m_errorMessage);
}

void ContainerDeviceSignalOperation::appendMsgCannotKill(int pid, const QString &why)
{
    if (!m_errorMessage.isEmpty())
        m_errorMessage += QChar::fromLatin1('\n');
    m_errorMessage += tr("Cannot kill process with pid %1: %2").arg(pid).arg(why);
    m_errorMessage += QLatin1Char(' ');
}

void ContainerDeviceSignalOperation::appendMsgCannotInterrupt(int pid, const QString &why)
{
    if (!m_errorMessage.isEmpty())
        m_errorMessage += QChar::fromLatin1('\n');
    m_errorMessage += tr("Cannot interrupt process with pid %1: %2").arg(pid).arg(why);
    m_errorMessage += QLatin1Char(' ');
}

void ContainerDeviceSignalOperation::killProcessSilently(int pid)
{
    if (pid <= 0) {
        appendMsgCannotKill(pid, tr("Invalid process id."));
        emit finished(m_errorMessage);
        return;
    }
    sendSignal(pid, SIGKILL);
}

void ContainerDeviceSignalOperation::interruptProcessSilently(int pid)
{
    if (pid <= 0) {
        appendMsgCannotInterrupt(pid, tr("Invalid process id."));
        emit finished(m_errorMessage);
        return;
    }
    sendSignal(pid, SIGINT);
}

void ContainerDeviceSignalOperation::sendSignal(int pid, int signal)
{
    if(!m_dev) {
        emit finished(tr("There was a internal error when trying to signal the process"));
        return;
    }

    QProcess *proc = new QProcess(this);
    connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),proc,SLOT(deleteLater()));
    connect(proc,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)));

    proc->setProgram(LinkMotionBasePlugin::lmTargetTool());
    proc->setArguments(QStringList()
                       << QStringLiteral("maint")
                       << m_dev->containerName()
                       << QStringLiteral("kill")
                       << QStringLiteral("-%1").arg(signal)
                       << QString::number(pid));

    proc->start();

}

void ContainerDeviceSignalOperation::processFinished(int exitCode ,QProcess::ExitStatus exitState)
{
    if(exitCode == 0 && exitState == QProcess::NormalExit) {
        emit finished(QString());
        return;
    }

    QString error = QStringLiteral("Can not kill process. Exit Code: %1").arg(exitCode);
    QProcess *proc = qobject_cast<QProcess*>(sender());
    if(proc) {
        QString err;

        if(exitState != QProcess::NormalExit)
            err = proc->errorString();
        else
            err = QString::fromLatin1(proc->readAllStandardError());

        if(!err.isEmpty())
            error.append(QStringLiteral("\n%1").arg(err));
    }
    emit finished(error);
}

void ContainerDeviceSignalOperation::processError(QProcess::ProcessError procErr)
{
    QProcess *proc = qobject_cast<QProcess*>(sender());
    if(proc) {
        proc->deleteLater();
        QString error = QStringLiteral("Can not kill the process. Error: %1 %2").arg(procErr).arg(proc->errorString());
        emit finished(error);
    }
}

} // namespace Internal
} // namespace LmBase

