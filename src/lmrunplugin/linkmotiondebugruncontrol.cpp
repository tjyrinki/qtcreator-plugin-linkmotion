#include "linkmotiondebugruncontrol.h"

#include <debugger/debuggerstartparameters.h>
#include <debugger/debuggerengine.h>
#include <debugger/debuggerruncontrol.h>
#include <debugger/debuggerrunconfigurationaspect.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/buildtargetinfo.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <qmldebug/qmldebugcommandlinearguments.h>
#include <remotelinux/remotelinuxdebugsupport.h>

#include "linkmotiondebugrunrunner.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

ProjectExplorer::RunControl* LinkMotionDebugRunControl::create(ProjectExplorer::RunConfiguration *runConfig, QString *errorMessage) {
    qDebug() << Q_FUNC_INFO;

    if (!runConfig) return NULL;
    if (!runConfig->target()) return NULL;
    if (!runConfig->target()->project()) return NULL;

    QString appName = runConfig->target()->project()->displayName();

    Debugger::Internal::DebuggerRunParameters params;
    params.debugSourceLocation.append(QStringLiteral("/usr/src/debug/altdata/apps/%0").arg(appName));
    params.startMode = Debugger::AttachToRemoteServer;
    params.closeMode = Debugger::KillAndExitMonitorAtClose;
    params.remoteSetupNeeded = true;
    params.useContinueInsteadOfRun = true;

    QString sourcePath = runConfig->target()->project()->projectDirectory().toString();

    // add source path to map in order to resolve the disassembler data into source code
    // FIXME: there is fixed package version to 1.0.0.
    params.sourcePathMap.insert(QStringLiteral("/usr/src/debug/%0-1.0.0").arg(appName),sourcePath);

    ProjectExplorer::IDevice::ConstPtr dev = ProjectExplorer::DeviceKitInformation::device(runConfig->target()->kit());
    if (!dev) {
        *errorMessage = tr("Cannot debug: Kit has no device.");
        return 0;
    }

     auto aspect = runConfig->extraAspect<Debugger::DebuggerRunConfigurationAspect>();


    if (aspect->useQmlDebugger()) {
        params.qmlServer.host = dev->sshParameters().host;
        params.qmlServer.port = Utils::Port();
    }
    if (aspect->useCppDebugger()) {
        aspect->setUseMultiProcess(true);
        params.multiProcess = true;
        params.inferior.executable = QStringLiteral("/altdata/apps/%0/bin/%0").arg(appName);

        params.inferior.commandLineArguments = QStringLiteral(" -platform eglfs");
        params.remoteChannel = dev->sshParameters().host + QLatin1String(":-1");
    }

    Debugger::DebuggerRunControl* const debuggerRunControl = Debugger::createDebuggerRunControl(params, runConfig, errorMessage);

    new LinkMotionDebugRunRunner(runConfig, debuggerRunControl);
    return debuggerRunControl;
}

