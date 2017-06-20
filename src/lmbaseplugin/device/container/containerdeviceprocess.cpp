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

#include "containerdeviceprocess.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>

#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>
#include <projectexplorer/runnables.h>

#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QTemporaryDir>

#include <stdlib.h>

namespace LmBase {
namespace Internal {

ContainerDeviceProcess::ContainerDeviceProcess(const QSharedPointer<const ProjectExplorer::IDevice> &device,
                                           QObject *parent)
    : LinuxDeviceProcess(device, parent)
{
    m_pidFile = QString::fromLatin1("/tmp/qtc.%1.pid").arg(QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex()));
}

ContainerDeviceProcess::~ContainerDeviceProcess()
{
    SshDeviceProcess *cleaner = new SshDeviceProcess(device());

    QString pid = m_pidFile;
    auto callback = [pid, cleaner](){
        if (cleaner->exitCode() != 0) {
            qWarning()<<"Cleaning the pidfile "<<pid<<" has failed";
        }
        cleaner->deleteLater();
    };

    connect(cleaner, &SshDeviceProcess::finished, callback);

    ProjectExplorer::StandardRunnable r;
    r.executable = QStringLiteral("rm");
    r.commandLineArguments = m_pidFile;
    cleaner->start(r);

    cleanupWestonProcess();
}

void ContainerDeviceProcess::start(const ProjectExplorer::Runnable &runnable)
{
    ProjectExplorer::StandardRunnable rc = runnable.as<ProjectExplorer::StandardRunnable>();
    if(rc.runMode == ProjectExplorer::ApplicationLauncher::Gui) {

        cleanupWestonProcess();

        m_westonDir = new QTemporaryDir(QStringLiteral("/tmp/lmsdk-XXXXXX"));
        if (!m_westonDir->isValid()) {
            qDebug()<<"Unable to create weston dir";
        }

        m_westonProc = new QProcess(this);
        m_westonProc->setArguments(QStringList{
            QString::fromLatin1("-c%1/../weston.ini").arg(Constants::LM_SCRIPTPATH)
        });

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("XDG_RUNTIME_DIR", m_westonDir->path()); // Add an environment variable
        m_westonProc->setProcessEnvironment(env);
        m_westonProc->setProgram(QStringLiteral("weston"));

        connect(this, &ProjectExplorer::SshDeviceProcess::finished,
                this, &ContainerDeviceProcess::cleanupWestonProcess);

        m_westonProc->start();
        if (!m_westonProc->waitForStarted(5000)) {
            qDebug()<<"Failed to start Weston "<<m_westonProc->errorString();
        }

        rc.environment.appendOrSet(QStringLiteral("XDG_RUNTIME_DIR"),
                                   m_westonDir->path());
        rc.environment.appendOrSet(QStringLiteral("QT_QPA_PLATFORM"),
                                   QStringLiteral("wayland"));
        rc.environment.appendOrSet(QStringLiteral("QT_WAYLAND_DISABLE_WINDOWDECORATION"),
                                   QStringLiteral("1"));
    }
    LinuxDeviceProcess::start(rc);
}

void ContainerDeviceProcess::doSignal(const int sig)
{
    SshDeviceProcess *signaler = new SshDeviceProcess(device(), this);
    connect(signaler, &SshDeviceProcess::finished, [signaler](){
        if (signaler->exitCode() != 0) {
            qWarning()<<"Killing the process has failed";
            qWarning()<<signaler->readAllStandardOutput();
            qWarning()<<signaler->readAllStandardError();
        }
        signaler->deleteLater();
    });

    ProjectExplorer::StandardRunnable r;
    r.executable = QStringLiteral("kill");
    r.commandLineArguments = QString::fromLatin1("-%2 `cat %1`").arg(m_pidFile).arg(sig);
    signaler->start(r);
}

void ContainerDeviceProcess::cleanupWestonProcess()
{
    if(m_westonProc) {
        if (m_westonProc->state() != QProcess::NotRunning) {
            m_westonProc->terminate();
            if(m_westonProc->waitForFinished(5000)) {
                m_westonProc->kill();
            }
        }
        delete m_westonProc;
        m_westonProc = nullptr;
    }

    if (m_westonDir) {
        delete m_westonDir;
        m_westonDir = nullptr;
    }
}

QString ContainerDeviceProcess::fullCommandLine(const ProjectExplorer::StandardRunnable &runnable) const
{
    //reimpl start and check for runmode, open weston window if its GUI

    QString fullCommandLine;
    QStringList rcFiles {
        QLatin1String("/etc/profile")
        , QLatin1String("$HOME/.profile")
    };
    foreach (const QString &filePath, rcFiles)
        fullCommandLine += QString::fromLatin1("test -f %1 && . %1;").arg(filePath);
    if (runnable.workingDirectory.isEmpty()) {
        fullCommandLine.append(QLatin1String("cd ")).append(Utils::QtcProcess::quoteArgUnix(runnable.workingDirectory))
                .append(QLatin1String(" && "));
    }
    QString envString;
    for (auto it = runnable.environment.constBegin(); it != runnable.environment.constEnd(); ++it) {
        if (!envString.isEmpty())
            envString += QLatin1Char(' ');
        envString.append(it.key()).append(QLatin1String("='")).append(it.value())
                .append(QLatin1Char('\''));
    }
    if (!envString.isEmpty())
        fullCommandLine.append(QLatin1Char(' ')).append(envString);

    if (!fullCommandLine.isEmpty())
        fullCommandLine += QLatin1Char(' ');

    fullCommandLine.append(Utils::QtcProcess::quoteArgUnix(QStringLiteral("dbus-run-session")));
    fullCommandLine += QString::fromLatin1(" bash -c \"echo \\$\\$ > %1; exec ").arg(m_pidFile);
    fullCommandLine.append(Utils::QtcProcess::quoteArgUnix(runnable.executable));
    if (!runnable.commandLineArguments.isEmpty()) {
        fullCommandLine.append(QLatin1Char(' '));
        fullCommandLine.append(runnable.commandLineArguments);
    }
    fullCommandLine.append(QStringLiteral("\""));
    return fullCommandLine;
}

} // namespace Internal
} // namespace LmBase

