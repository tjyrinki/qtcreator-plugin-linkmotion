#include "linkmotionanalyzerunrunner.h"

#include <debugger/debuggerstartparameters.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <debugger/debuggercore.h>
#include <debugger/debuggeractions.h>
#include <debugger/analyzer/analyzerruncontrol.h>
#include <projectexplorer/taskhub.h>
#include <projectexplorer/task.h>
#include "linkmotionrunplugin_constants.h"

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

void LinkMotionAnalyzeRunRunner::slotRunControl_Finished() {
    qDebug() << Q_FUNC_INFO;
    m_processStart.terminate();
}

void LinkMotionAnalyzeRunRunner::slotRunControl_Started() {
    qDebug() << Q_FUNC_INFO;    qDebug() << Q_FUNC_INFO;

    if (!m_runConfig) return;
    if (!m_runConfig->target()) return;
    if (!m_runConfig->target()->project()) return;

    QString appName = m_runConfig->target()->project()->displayName();

    Utils::Environment env = Utils::Environment::systemEnvironment();
    m_runConfig->target()->kit()->addToEnvironment(env);

    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    //FIXME
    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_processStart.setEnvironment(env);

    ProjectExplorer::TaskHub::clearTasks(LinkMotion::Constants::TASK_CATEGORY_ANALYZE);

    // launch the application for profiling
    m_processStart.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-profile-start"),QStringLiteral("%0 %1").arg(appName).arg(3768));
    m_processStart.start();
}


void LinkMotionAnalyzeRunRunner::slotStart_Finished(int retval, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << retval << status;
    m_processStart.terminate();

    if (!m_runConfig) return;
    if (!m_runConfig->target()) return;
    if (!m_runConfig->target()->project()) return;

    QString projectName =  m_runConfig->target()->project()->displayName();
    Utils::Environment env = Utils::Environment::systemEnvironment();
    m_runConfig->target()->kit()->addToEnvironment(env);

    env.prependOrSetPath("/opt/linkmotion/sdk/vm");
    env.prependOrSetPath("/opt/linkmotion/sdk/hw");

    //FIXME
    env.set(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    env.set(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    env.set(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("notset"));

    m_processStop.setEnvironment(env);

    m_processStop.setCommand(QStringLiteral("/opt/linkmotion/sdk/vm/vmsdk-app-profile-stop"),projectName);
    m_processStop.start();
}

void LinkMotionAnalyzeRunRunner::slotStart_StateChanged(QProcess::ProcessState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionAnalyzeRunRunner::slotStart_ReadyRead() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionAnalyzeRunRunner::slotStart_ReadyReadStandardError() {
    QByteArray data = m_processStart.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;
    if (!m_runControl) return;

    QStringList lines = QString::fromLatin1(data).split("\n");

    foreach (QString line, lines) {
        Utils::OutputFormat outputFormat = Utils::StdOutFormat;
        if (line.startsWith(QStringLiteral("Can't bind address"))) {
            outputFormat = Utils::ErrorMessageFormat;
            m_runControl->notifyRemoteSetupFailed(line);
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_ANALYZE);
        } else if (line.startsWith(QStringLiteral("Listening on port "))) {
            //m_runControl->notifyRemoteSetupDone(Utils::Port(3768));
        } else if (line.startsWith(QStringLiteral("Cannot exec "))) {
            outputFormat = Utils::ErrorMessageFormat;
            m_runControl->notifyRemoteSetupFailed(line);
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_ANALYZE);
        } else if (line.startsWith(QStringLiteral("QQmlApplicationEngine failed to load component"))) {
            outputFormat = Utils::ErrorMessageFormat;
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_ANALYZE);
        } else if (line.startsWith(QStringLiteral("qrc:/"))) {
            outputFormat = Utils::NormalMessageFormat;
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, line, LinkMotion::Constants::TASK_CATEGORY_ANALYZE);
        }

        m_runControl->appendMessage(line,outputFormat);
        m_runControl->appendMessage(QString::fromLatin1("\n"),outputFormat);
    }
}

void LinkMotionAnalyzeRunRunner::slotStart_ReadyReadStandardOutput() {
    QByteArray data = m_processStart.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;
    if (!m_runControl) return;

    QStringList lines = QString::fromLatin1(data).split("\n");

    foreach (QString line, lines) {
        Utils::OutputFormat outputFormat = Utils::StdOutFormat;
        m_runControl->appendMessage(line,outputFormat);
        m_runControl->appendMessage(QString::fromLatin1("\n"),outputFormat);
    }

}

void LinkMotionAnalyzeRunRunner::slotStop_Finished(int retval, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << retval << status;
    if (!m_runControl) return;
    m_runControl->notifyRemoteFinished();
}

void LinkMotionAnalyzeRunRunner::slotStop_StateChanged(QProcess::ProcessState state) {
    qDebug() << Q_FUNC_INFO << state;

}

void LinkMotionAnalyzeRunRunner::slotStop_ReadyRead() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionAnalyzeRunRunner::slotStop_ReadyReadStandardError() {
    QByteArray data = m_processStop.readAllStandardError();
    qDebug() << Q_FUNC_INFO << data;
}

void LinkMotionAnalyzeRunRunner::slotStop_ReadyReadStandardOutput() {
    QByteArray data = m_processStop.readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << data;

}
