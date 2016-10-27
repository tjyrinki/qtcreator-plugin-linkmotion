#include "linkmotiondebugrunrunner.h"
#include <debugger/debuggerstartparameters.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <debugger/debuggercore.h>
#include <debugger/debuggeractions.h>
#include <projectexplorer/taskhub.h>
#include "linkmotionrunplugin_constants.h"

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

    Utils::Environment env = Utils::Environment::systemEnvironment();
    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_process.setEnvironment(env);
    ProjectExplorer::TaskHub::clearTasks(LinkMotion::Constants::TASK_CATEGORY_DEBUG);

    // launch the debugger and our application
    m_process.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-debug-start"),QStringLiteral("%0 %1 %2").arg(appName).arg(3768).arg(25555));
    m_process.start();

}

void LinkMotionDebugRunRunner::slotRunControl_Finished() {
    qDebug() << Q_FUNC_INFO;
    m_process.terminate();
    QString projectName = m_runConfig->target()->project()->displayName();
    m_process.startDetached(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-debug-stop %0").arg(projectName));
    m_runControl->notifyInferiorExited();
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

    QStringList lines = QString::fromLatin1(data).split("\n");

    foreach (QString line, lines) {
        Utils::OutputFormat outputFormat = Utils::StdOutFormat;
        if (line.startsWith(QStringLiteral("Can't bind address"))) {
            outputFormat = Utils::ErrorMessageFormat;
            Debugger::RemoteSetupResult result;
            result.success = false;
            result.reason = line;
            m_runControl->notifyEngineRemoteSetupFinished(result);
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_DEBUG);
        } else if (line.startsWith(QStringLiteral("Listening on port "))) {
            Debugger::RemoteSetupResult result;
            result.gdbServerPort = Utils::Port(25555);
            result.qmlServerPort = Utils::Port(3768);
            result.success = true;
            m_runControl->notifyEngineRemoteSetupFinished(result);
        } else if (line.startsWith(QStringLiteral("Cannot exec "))) {
            outputFormat = Utils::ErrorMessageFormat;
            Debugger::RemoteSetupResult result;
            result.gdbServerPort = Utils::Port(25555);
            result.success = false;
            result.reason = line;
            // we have already notified the EngineRemoteSetupFinished, so just abort
            m_runControl->abortDebugger();
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_DEBUG);
        } else if (line.startsWith(QStringLiteral("QQmlApplicationEngine failed to load component"))) {
            outputFormat = Utils::ErrorMessageFormat;
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_DEBUG);
        } else if (line.startsWith(QStringLiteral("qrc:/"))) {
            outputFormat = Utils::NormalMessageFormat;
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_DEBUG);
        }

        m_runControl->appendMessage(line,outputFormat);
        m_runControl->appendMessage(QString::fromLatin1("\n"),outputFormat);
    }

}

void LinkMotionDebugRunRunner::slotProcess_ReadyReadStandardOutput() {
    QByteArray data = m_process.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;

    QStringList lines = QString::fromLatin1(data).split("\n");
    foreach (QString line, lines) {
        Utils::OutputFormat outputFormat = Utils::StdOutFormat;
        if (line.startsWith(QStringLiteral("ERROR: "))) {
            outputFormat = Utils::ErrorMessageFormat;
        }
        m_runControl->appendMessage(line,outputFormat);
        m_runControl->appendMessage(QString::fromLatin1("\n"),outputFormat);
    }
}
