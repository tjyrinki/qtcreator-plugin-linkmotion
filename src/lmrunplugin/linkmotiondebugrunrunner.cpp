#include "linkmotiondebugrunrunner.h"
#include <debugger/debuggerstartparameters.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <debugger/debuggercore.h>
#include <debugger/debuggeractions.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDebugRunRunner::LinkMotionDebugRunRunner(ProjectExplorer::RunConfiguration *runConfig,
                                                   Debugger::DebuggerRunControl *runControl) :
    QObject(),
    m_runControl(runControl),
    m_runConfig(runConfig)
{
    qDebug() << Q_FUNC_INFO;
    connect(m_runControl,SIGNAL(requestRemoteSetup()),this,SLOT(slotRunControl_RequestRemoteSetup()));
    connect(m_runControl,SIGNAL(finished()),this,SLOT(slotRunControl_Finished()));
    connect(m_runControl,SIGNAL(started()),this,SLOT(slotRunControl_Started()));
    connect(m_runControl,SIGNAL(stateChanged(Debugger::DebuggerState)),this,SLOT(slotRunControl_StateChanged(Debugger::DebuggerState)));


    connect(&m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotProcess_Finished(int, QProcess::ExitStatus)));
    connect(&m_process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(slotProcess_StateChanged(QProcess::ProcessState)));
    connect(&m_process, SIGNAL(readyRead()), this, SLOT(slotProcess_ReadyRead()));
    connect(&m_process, SIGNAL(readyReadStandardError()), this, SLOT(slotProcess_ReadyReadStandardError()));
    connect(&m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotProcess_ReadyReadStandardOutput()));
}


void LinkMotionDebugRunRunner::slotRunControl_RequestRemoteSetup() {
    qDebug() << Q_FUNC_INFO;
    QString appName = m_runConfig->target()->project()->displayName();

    QString executable = QStringLiteral("/altdata/apps/%0/bin/%0").arg(appName);

    // launch the debugger and our application
    m_process.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-shell"),QStringLiteral("gdbserver --once --multi :25555 %0 -platform eglfs -qmljsdebugger=port:3768,block,services:DebugMessages,QmlDebugger,V8Debugger,QmlInspector").arg(executable));
    m_process.start();

}

void LinkMotionDebugRunRunner::slotRunControl_Finished() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionDebugRunRunner::slotRunControl_Started() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionDebugRunRunner::slotRunControl_StateChanged(Debugger::DebuggerState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionDebugRunRunner::slotProcess_Finished(int retval, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << retval << status;
    m_runControl->notifyInferiorExited();

}

void LinkMotionDebugRunRunner::slotProcess_StateChanged(QProcess::ProcessState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionDebugRunRunner::slotProcess_ReadyRead() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionDebugRunRunner::slotProcess_ReadyReadStandardError() {
    QByteArray data = m_process.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;

    if (data.contains(QByteArray("Can't bind address"))) {
        m_runControl->appendMessage(QString::fromLatin1(data),Utils::ErrorMessageFormat);
        Debugger::RemoteSetupResult result;
        result.success = false;
        result.reason = QString::fromLatin1(data);
        m_runControl->notifyEngineRemoteSetupFinished(result);
    }
    if (data.contains(QByteArray("Listening on port "))) {
        Debugger::RemoteSetupResult result;
        result.gdbServerPort = Utils::Port(25555);
       // result.inferiorPid = 123;

        result.qmlServerPort = Utils::Port(3768);
        result.success = true;
        m_runControl->notifyEngineRemoteSetupFinished(result);
    }
    m_runControl->appendMessage(QString::fromLatin1(data),Utils::ErrorMessageFormat);

}

void LinkMotionDebugRunRunner::slotProcess_ReadyReadStandardOutput() {
    qDebug() << Q_FUNC_INFO << m_process.readAllStandardOutput();

}
