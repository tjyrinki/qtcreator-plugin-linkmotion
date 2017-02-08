#include "linkmotiondebugrunrunner.h"
#include <debugger/debuggerstartparameters.h>
#include <projectexplorer/kit.h>
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

    connect(&m_processStart, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotStart_Finished(int, QProcess::ExitStatus)));
    connect(&m_processStart, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(slotStart_StateChanged(QProcess::ProcessState)));
    connect(&m_processStart, SIGNAL(readyRead()), this, SLOT(slotStart_ReadyRead()));
    connect(&m_processStart, SIGNAL(readyReadStandardError()), this, SLOT(slotStart_ReadyReadStandardError()));
    connect(&m_processStart, SIGNAL(readyReadStandardOutput()), this, SLOT(slotStart_ReadyReadStandardOutput()));

    connect(&m_processStop, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotStop_Finished(int, QProcess::ExitStatus)));
    connect(&m_processStop, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(slotStop_StateChanged(QProcess::ProcessState)));
    connect(&m_processStop, SIGNAL(readyRead()), this, SLOT(slotStop_ReadyRead()));
    connect(&m_processStop, SIGNAL(readyReadStandardError()), this, SLOT(slotStop_ReadyReadStandardError()));
    connect(&m_processStop, SIGNAL(readyReadStandardOutput()), this, SLOT(slotStop_ReadyReadStandardOutput()));
}


void LinkMotionDebugRunRunner::slotRunControl_RequestRemoteSetup() {
    qDebug() << Q_FUNC_INFO;
    QString appName = m_runConfig->target()->project()->displayName();

    Utils::Environment env = Utils::Environment::systemEnvironment();
    m_runConfig->target()->kit()->addToEnvironment(env);

    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_processStart.setEnvironment(env);
    ProjectExplorer::TaskHub::clearTasks(LinkMotion::Constants::TASK_CATEGORY_DEBUG);

    // get debugging ports
    QProcess p;
    p.setEnvironment(env.toStringList());
    p.start("/bin/bash -c \". /opt/linkmotion/sdk/vm/.vmsdk-target-source; os_gdb_port; os_qml_port\"");

    if (p.waitForFinished(1000)) {
        QString out = QString::fromLocal8Bit(p.readAllStandardOutput());
        QString gdb = out.section('\n', 0, 0);
        QString qml = out.section('\n', 1, 1);

        bool isNumber;
        quint16 portNum;

        portNum = gdb.toLong(&isNumber);
        m_gdbPort = (isNumber) ? Utils::Port(portNum) : Utils::Port();
        portNum = qml.toLong(&isNumber);
        m_qmlPort = (isNumber) ? Utils::Port(portNum) : Utils::Port();
    } else {
        qWarning() << Q_FUNC_INFO << "Can't get debugging port numbers!";
        m_gdbPort = Utils::Port();
        m_qmlPort = Utils::Port();
    }

    qDebug() << Q_FUNC_INFO << "For GDB using port:" << m_gdbPort.number();
    qDebug() << Q_FUNC_INFO << "For QML using port:" << m_qmlPort.number();

    // launch the debugger and our application
    m_processStart.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-debug-start"),QStringLiteral("%0").arg(appName));
    m_processStart.start();

}

void LinkMotionDebugRunRunner::slotRunControl_Finished() {
    qDebug() << Q_FUNC_INFO;
    m_processStart.terminate();

    if (!m_runConfig) return;
    if (!m_runConfig->target()) return;
    if (!m_runConfig->target()->project()) return;

    QString projectName = m_runConfig->target()->project()->displayName();

    Utils::Environment env = Utils::Environment::systemEnvironment();
    m_runConfig->target()->kit()->addToEnvironment(env);

    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_processStop.setEnvironment(env);
    m_processStop.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-debug-stop"),projectName);
    m_processStop.start();
}

void LinkMotionDebugRunRunner::slotRunControl_Started() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionDebugRunRunner::slotRunControl_StateChanged(Debugger::DebuggerState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionDebugRunRunner::slotStart_Finished(int retval, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << retval << status;
 //   m_runControl->notifyInferiorExited();

}

void LinkMotionDebugRunRunner::slotStart_StateChanged(QProcess::ProcessState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionDebugRunRunner::slotStart_ReadyRead() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionDebugRunRunner::slotStart_ReadyReadStandardError() {
    QByteArray data = m_processStart.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;
    if (!m_runControl) return;


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
            result.gdbServerPort = m_gdbPort;
            result.qmlServerPort = m_qmlPort;
            result.success = true;
            m_runControl->notifyEngineRemoteSetupFinished(result);
        } else if (line.startsWith(QStringLiteral("Cannot exec "))) {
            outputFormat = Utils::ErrorMessageFormat;
            Debugger::RemoteSetupResult result;
            result.gdbServerPort = m_gdbPort;
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

void LinkMotionDebugRunRunner::slotStart_ReadyReadStandardOutput() {
    QByteArray data = m_processStart.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;
    if (!m_runControl) return;

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

void LinkMotionDebugRunRunner::slotStop_Finished(int retval, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << retval << status;
    if (!m_runControl) return;
    m_runControl->notifyInferiorExited();
}

void LinkMotionDebugRunRunner::slotStop_StateChanged(QProcess::ProcessState state) {
    qDebug() << Q_FUNC_INFO << state;
}

void LinkMotionDebugRunRunner::slotStop_ReadyRead() {
    qDebug() << Q_FUNC_INFO;
}


void LinkMotionDebugRunRunner::slotStop_ReadyReadStandardError() {
    QByteArray data = m_processStop.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;
}

void LinkMotionDebugRunRunner::slotStop_ReadyReadStandardOutput() {
    QByteArray data = m_processStop.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;
}
