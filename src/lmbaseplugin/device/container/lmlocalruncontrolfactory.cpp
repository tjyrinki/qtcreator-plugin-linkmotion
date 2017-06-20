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

#include "lmlocalruncontrolfactory.h"
#include "lmlocalrunconfiguration.h"
#include <ubuntu/ubuntuconstants.h>
#include <ubuntu/clicktoolchain.h>

#include "containerdevice.h"

#include <debugger/analyzer/analyzermanager.h>
#include <debugger/analyzer/analyzerruncontrol.h>
#include <debugger/analyzer/analyzerstartparameters.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/target.h>
#include <projectexplorer/abi.h>
#include <debugger/debuggerruncontrol.h>
#include <debugger/debuggerstartparameters.h>
#include <debugger/debuggerrunconfigurationaspect.h>
#include <qmldebug/qmldebugcommandlinearguments.h>
#include <utils/fileutils.h>
#include <utils/portlist.h>
#include <qmlprofiler/localqmlprofilerrunner.h>
#include <valgrind/callgrindtool.h>
#include <valgrind/memchecktool.h>

#include <remotelinux/remotelinuxruncontrol.h>
#include <remotelinux/remotelinuxdebugsupport.h>
#include <remotelinux/remotelinuxanalyzesupport.h>

#include <QTcpServer>
#include <QSet>

namespace LmBase {
namespace Internal {

bool UbuntuLocalRunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const
{
    if(qobject_cast<UbuntuLocalRunConfiguration*>(runConfiguration)) {
        if (mode != ProjectExplorer::Constants::NORMAL_RUN_MODE
                && mode != ProjectExplorer::Constants::DEBUG_RUN_MODE
                && mode != ProjectExplorer::Constants::DEBUG_RUN_MODE_WITH_BREAK_ON_MAIN
                && mode != ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
            return false;
        }

        return runConfiguration->isEnabled();
    }
    return false;
}

ProjectExplorer::RunControl *UbuntuLocalRunControlFactory::create(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode, QString *errorMessage)
{
    UbuntuLocalRunConfiguration *ubuntuRC = qobject_cast<UbuntuLocalRunConfiguration*>(runConfiguration);
    if (!ubuntuRC)
        return 0;

    if (!ubuntuRC->aboutToStart(errorMessage))
        return 0;

    QTC_ASSERT(canRun(runConfiguration, mode), return 0);
    const auto rcRunnable = runConfiguration->runnable();
    QTC_ASSERT(rcRunnable.is<ProjectExplorer::StandardRunnable>(), return 0);
    const auto stdRunnable = rcRunnable.as<ProjectExplorer::StandardRunnable>();

    ProjectExplorer::IDevice::ConstPtr genericDev = ProjectExplorer::DeviceKitInformation::device(runConfiguration->target()->kit());
    if (!genericDev || !genericDev->type().toString().startsWith(QLatin1String(Constants::LM_CONTAINER_DEVICE_TYPE_ID))) {
        if(!errorMessage)
            *errorMessage = tr("Wrong or no device assigned to runconfiguration.");
        return 0;
    }

    ProjectExplorer::ToolChain *genericToolchain = ProjectExplorer::ToolChainKitInformation::toolChain(runConfiguration->target()->kit());
    if (!genericToolchain || genericToolchain->typeId() != Constants::LM_CLICK_TOOLCHAIN_ID) {
        if(!errorMessage)
            *errorMessage = tr("Wrong toolchain type in runconfiguration.");
        return 0;
    }

    ContainerDevice::ConstPtr dev = qSharedPointerCast<const ContainerDevice>(genericDev);
    ClickToolChain *tc = static_cast<ClickToolChain *>(genericToolchain);

    if (mode == ProjectExplorer::Constants::NORMAL_RUN_MODE) {
        RemoteLinux::RemoteLinuxRunControl *runControl = new RemoteLinux::RemoteLinuxRunControl(ubuntuRC);
        return runControl;
    }

    else if(mode == ProjectExplorer::Constants::DEBUG_RUN_MODE
            || mode == ProjectExplorer::Constants::DEBUG_RUN_MODE_WITH_BREAK_ON_MAIN) {

        auto aspect = ubuntuRC->extraAspect<Debugger::DebuggerRunConfigurationAspect>();
        if (aspect->portsUsedByDebugger() > dev->freePorts().count()) {
            *errorMessage = tr("Cannot debug: Not enough free ports available.");
            return 0;
        }

        /*
         * Taken from remotelinuxruncontrolfactory.cpp and adapted
         * to work here.
         */
        Debugger::DebuggerStartParameters params;
        params.startMode = Debugger::AttachToRemoteServer;
        params.closeMode = Debugger::KillAndExitMonitorAtClose;
        params.remoteSetupNeeded = true;

        if (aspect->useQmlDebugger()) {
            params.qmlServer.host = dev->sshParameters().host;
            params.qmlServer.port = Utils::Port(); // port is selected later on
        }
        if (aspect->useCppDebugger()) {
            aspect->setUseMultiProcess(true);
            params.inferior.executable = stdRunnable.executable;
            params.inferior.commandLineArguments = stdRunnable.commandLineArguments;
            if (aspect->useQmlDebugger()) {
                params.inferior.commandLineArguments.prepend(QLatin1Char(' '));
                params.inferior.commandLineArguments.prepend(QmlDebug::qmlDebugTcpArguments(QmlDebug::QmlDebuggerServices));
            }
            params.remoteChannel = dev->sshParameters().host + QLatin1String(":-1");
            params.symbolFile = ubuntuRC->localExecutableFilePath();
        }

        params.solibSearchPath.append(ubuntuRC->soLibSearchPaths());

        Debugger::DebuggerRunControl * const runControl = Debugger::createDebuggerRunControl(params, ubuntuRC, errorMessage, mode);
        if (!runControl)
            return 0;

        (void) new RemoteLinux::LinuxDeviceDebugSupport(ubuntuRC, runControl);
        return runControl;
    }
    else if(mode == ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
        /*
         * Taken from remotelinuxruncontrolfactory.cpp and adapted
         * to work here.
         */
        auto runControl = Debugger::createAnalyzerRunControl(ubuntuRC, mode);
        Debugger::AnalyzerConnection connection;
        connection.connParams =
            ProjectExplorer::DeviceKitInformation::device(ubuntuRC->target()->kit())->sshParameters();
        connection.analyzerHost = connection.connParams.host;
        runControl->setConnection(connection);
        (void) new RemoteLinux::RemoteLinuxAnalyzeSupport(ubuntuRC, runControl, mode);
        return runControl;
    }
    QTC_ASSERT(false, return 0);

}

QString UbuntuLocalRunControlFactory::displayName() const
{
    return tr("Run Ubuntu project locally");
}

} //namespace Internal
} //namespace LmBase
