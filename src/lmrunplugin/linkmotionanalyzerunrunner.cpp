#include "linkmotionanalyzerunrunner.h"

#include <debugger/debuggerstartparameters.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <debugger/debuggercore.h>
#include <debugger/debuggeractions.h>
#include <debugger/analyzer/analyzerruncontrol.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionAnalyzeRunRunner::LinkMotionAnalyzeRunRunner(ProjectExplorer::RunConfiguration *runConfig,
                                                   Debugger::AnalyzerRunControl *runControl) :
    QObject(),
    m_runControl(runControl),
    m_runConfig(runConfig)
{
    qDebug() << Q_FUNC_INFO;
    connect(m_runControl,SIGNAL(finished()),this,SLOT(slotRunControl_Finished()));
    connect(m_runControl,SIGNAL(started()),this,SLOT(slotRunControl_Started()));
    connect(m_runControl,SIGNAL(starting()),this,SLOT(slotRunControl_Started()));

    connect(&m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotProcess_Finished(int, QProcess::ExitStatus)));
    connect(&m_process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(slotProcess_StateChanged(QProcess::ProcessState)));
    connect(&m_process, SIGNAL(readyRead()), this, SLOT(slotProcess_ReadyRead()));
    connect(&m_process, SIGNAL(readyReadStandardError()), this, SLOT(slotProcess_ReadyReadStandardError()));
    connect(&m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotProcess_ReadyReadStandardOutput()));
}

void LinkMotionAnalyzeRunRunner::slotRunControl_Finished() {
    qDebug() << Q_FUNC_INFO;
    m_process.terminate();

}

void LinkMotionAnalyzeRunRunner::slotRunControl_Started() {
    qDebug() << Q_FUNC_INFO;    qDebug() << Q_FUNC_INFO;
    QString appName = m_runConfig->target()->project()->displayName();

    QString executable = QStringLiteral("/altdata/apps/%0/bin/%0").arg(appName);

    // launch the debugger and our application
//    m_process.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-shell"),QStringLiteral("gdbserver --once --multi :25555 %0 -platform eglfs -qmljsdebugger=port:3768,services:CanvasFrameRate,EngineControl,DebugMessages").arg(executable));
    m_process.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-shell"),QStringLiteral("%0 -platform eglfs -qmljsdebugger=port:3768,block").arg(executable));
    m_process.start();
}


void LinkMotionAnalyzeRunRunner::slotProcess_Finished(int retval, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << retval << status;
    m_runControl->notifyRemoteFinished();

}

void LinkMotionAnalyzeRunRunner::slotProcess_StateChanged(QProcess::ProcessState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionAnalyzeRunRunner::slotProcess_ReadyRead() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionAnalyzeRunRunner::slotProcess_ReadyReadStandardError() {
    QByteArray data = m_process.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;

    QStringList lines = QString::fromLatin1(data).split("\n");

    foreach (QString line, lines) {
        if (line.startsWith(QStringLiteral("Can't bind address"))) {
            m_runControl->appendMessage(line,Utils::ErrorMessageFormat);
            m_runControl->appendMessage(QString::fromLatin1("\n"),Utils::ErrorMessageFormat);
            m_runControl->notifyRemoteSetupFailed(line);
        } else if (line.startsWith(QStringLiteral("Listening on port "))) {
            //m_runControl->notifyRemoteSetupDone(Utils::Port(3768));
        } else {
            m_runControl->appendMessage(line,Utils::ErrorMessageFormat);
            m_runControl->appendMessage(QString::fromLatin1("\n"),Utils::ErrorMessageFormat);
        }
    }
}

void LinkMotionAnalyzeRunRunner::slotProcess_ReadyReadStandardOutput() {
    qDebug() << Q_FUNC_INFO << m_process.readAllStandardOutput();

}
