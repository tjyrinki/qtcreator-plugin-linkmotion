#include "linkmotiondebugruncontrol.h"

#include <debugger/debuggerstartparameters.h>
#include <debugger/debuggerruncontrol.h>
#include <debugger/debuggerrunconfigurationaspect.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDebugRunControl::LinkMotionDebugRunControl(ProjectExplorer::RunConfiguration *runConfig,
                                                     Debugger::DebuggerRunControl *runControl)
                                                     : QObject(runControl),
                                                       m_runControl(runControl)
{
    qDebug() << Q_FUNC_INFO;
    m_projectName = runConfig->target()->project()->displayName();
    m_appName = m_projectName;
    m_pid = -1;
    m_gdbPort = -1;
    qDebug() << Q_FUNC_INFO << m_projectName << m_appName;

    connect(&m_process,SIGNAL(readyReadStandardError()),this,SLOT(onStdErr()));
    connect(&m_process,SIGNAL(readyReadStandardOutput()),this,SLOT(onStdOut()));
    connect(&m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onFinished(int, QProcess::ExitStatus)));

    Debugger::DebuggerRunConfigurationAspect *aspect = runConfig->extraAspect<Debugger::DebuggerRunConfigurationAspect>();

    if (!aspect->useCppDebugger() && !aspect->useQmlDebugger()) {
        qWarning() << Q_FUNC_INFO << "Debugger not enabled for C++ or for QML";
        return;
    }

    connect(m_runControl,SIGNAL(finished()),&m_process,SLOT(terminate()));
    connect(m_runControl,SIGNAL(requestRemoteSetup()),this,SLOT(start()));


    /*


    // FIXME: Move signal to base class and generalize handling.
    connect(m_runControl, &DebuggerRunControl::aboutToNotifyInferiorSetupOk,
            m_runner, &AndroidRunner::handleRemoteDebuggerRunning);

    connect(m_runner, &AndroidRunner::remoteServerRunning,
        [this](const QByteArray &serverChannel, int pid) {
            QTC_ASSERT(m_runControl, return);
            m_runControl->notifyEngineRemoteServerRunning(serverChannel, pid);
        });

    connect(m_runner, &AndroidRunner::remoteProcessStarted,
            this, &AndroidDebugSupport::handleRemoteProcessStarted);

    connect(m_runner, &AndroidRunner::remoteProcessFinished,
        [this](const QString &errorMsg) {
            QTC_ASSERT(m_runControl, return);
            m_runControl->appendMessage(errorMsg, Utils::DebugFormat);
            QMetaObject::invokeMethod(m_runControl, "notifyInferiorExited", Qt::QueuedConnection);
        });

*/
}

ProjectExplorer::RunControl* LinkMotionDebugRunControl::create(ProjectExplorer::RunConfiguration *runConfig, QString *errorMessage) {
    qDebug() << Q_FUNC_INFO;
    Debugger::DebuggerStartParameters params;
    params.startMode = Debugger::AttachToRemoteServer;
    params.displayName = tr("LinkMotion Remote Debugger");
    params.remoteSetupNeeded = true;

    Debugger::DebuggerRunConfigurationAspect* aspect = runConfig->extraAspect<Debugger::DebuggerRunConfigurationAspect>();
    if (aspect->useQmlDebugger()) {

    } else if (aspect->useCppDebugger()) {

    }

    Debugger::DebuggerRunControl* const debuggerRunControl = Debugger::createDebuggerRunControl(params, runConfig, errorMessage);
    new LinkMotionDebugRunControl(runConfig, debuggerRunControl);
    return debuggerRunControl;
}

void LinkMotionDebugRunControl::start() {
    qDebug() << Q_FUNC_INFO;
    m_stdout.clear();
    QString gdbServer = QStringLiteral("vmsdk-shell /root/tila/gdb-7.12/gdb/gdbserver/gdbserver --once --multi :25555 /altdata/apps/%0/bin/%1 -platform eglfs -qml_debug=true -qmljsdebugger=port:3768").arg(m_projectName).arg(m_appName);
    m_process.start(gdbServer);
    m_process.waitForStarted();
}

void LinkMotionDebugRunControl::onFinished(int code, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << code << status;
    QMetaObject::invokeMethod(m_runControl, "notifyInferiorExited", Qt::QueuedConnection);

}

void LinkMotionDebugRunControl::detectGdbServer() {
    qDebug() << Q_FUNC_INFO;
    m_pid = getProcessPid();
    m_gdbPort = getPort();
    int qmlPort = 3768;

    if (m_pid == -1 || m_gdbPort == -1)  {
        return;
    }

    Debugger::RemoteSetupResult result;
    result.success = true;
    result.gdbServerPort = m_gdbPort;
    result.qmlServerPort = qmlPort;
    m_runControl->notifyEngineRemoteSetupFinished(result);
   // m_runControl->notifyEngineRemoteServerRunning(QStringLiteral(":%0").arg(m_gdbPort).toLatin1(), m_pid);
    qDebug() << Q_FUNC_INFO << "Notified that server is running";

}

void LinkMotionDebugRunControl::onStdErr() {
    qDebug() << Q_FUNC_INFO;
    QByteArray lineData = m_process.readAllStandardError();
    m_stdout.append(lineData);
    qDebug() << lineData;
    if (m_pid == -1 || m_gdbPort == -1) {
        detectGdbServer();
    }
 //   ProjectExplorer::RunControl::appendMessage(QString::fromLatin1(m_process.readAllStandardError()), Utils::ErrorMessageFormat);
}

void LinkMotionDebugRunControl::onStdOut() {
    qDebug() << Q_FUNC_INFO;
    QByteArray lineData = m_process.readAllStandardOutput();
    m_stdout.append(lineData);
    qDebug() << lineData;
   // ProjectExplorer::RunControl::appendMessage(QString::fromLatin1(lineData), Utils::NormalMessageFormat);
}

/*
 *
 * Process /altdata/apps/untitled186/bin/untitled186 created; pid = 7597
Listening on port 25555
*/
int LinkMotionDebugRunControl::getProcessPid() {
    QRegularExpression pidExp(QLatin1String("^Process /altdata/apps/([a-zA-Z_0-9.]*?)/bin/([a-zA-Z_0-9.]*?) created; pid = ([0-9]*?)$"));

    int pidNumber = -1;
    QStringList lines = QString::fromLatin1(m_stdout).split(QStringLiteral("\n"));
    foreach(QString line, lines) {
        qDebug() << line;
        QRegularExpressionMatch match = pidExp.match(line);

        if (match.hasMatch()) {
            QString projectName = match.captured(1);
            QString binaryName = match.captured(2);
            pidNumber = match.captured(3).toInt();
            qDebug() <<  Q_FUNC_INFO << "Found" << projectName << binaryName << pidNumber;
            break;
        } else {
            qWarning() << Q_FUNC_INFO << "Could not detect pid";
        }
    }
    return pidNumber;
}

int LinkMotionDebugRunControl::getPort() {
    QRegularExpression portExp(QStringLiteral("^Listening on port ([0-9]*?)$"));

    int portNumber = -1;
    QStringList lines = QString::fromLatin1(m_stdout).split(QStringLiteral("\n"));
    foreach(QString line, lines) {
        qDebug() << line;
        QRegularExpressionMatch match = portExp.match(line);

        if (match.hasMatch()) {
            portNumber = match.captured(1).toInt();
            qDebug() << Q_FUNC_INFO << "Found" << portNumber;
            break;
        } else {
            qWarning() <<  Q_FUNC_INFO << "Could not detect pid";
        }
    }
    return portNumber;
}
