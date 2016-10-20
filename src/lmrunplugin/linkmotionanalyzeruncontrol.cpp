#include "linkmotionanalyzeruncontrol.h"
#include <projectexplorer/target.h>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <projectexplorer/project.h>
#include <projectexplorer/target.h>

#include <analyzerbase/ianalyzertool.h>
#include <analyzerbase/analyzermanager.h>
#include <analyzerbase/analyzerruncontrol.h>
#include <analyzerbase/analyzerstartparameters.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;


LinkMotionAnalyzeRunControl::LinkMotionAnalyzeRunControl(LinkMotionRunConfiguration *runConfig, Analyzer::AnalyzerRunControl *runControl)
    : QObject(runControl)
{
    qDebug() << Q_FUNC_INFO;
    m_projectName = runConfig->target()->project()->displayName();
    m_appName = m_projectName;
    m_runControl = runControl;
    m_qmlPort = -1;
    qDebug() << Q_FUNC_INFO << m_projectName << m_appName;

    connect(&m_process,SIGNAL(readyReadStandardError()),this,SLOT(onStdErr()));
    connect(&m_process,SIGNAL(readyReadStandardOutput()),this,SLOT(onStdOut()));
    connect(&m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onFinished(int, QProcess::ExitStatus)));

    connect(m_runControl,SIGNAL(finished()),&m_process,SLOT(terminate()));
    connect(m_runControl,SIGNAL(starting(const Analyzer::AnalyzerRunControl *)),this,SLOT(start()));

}

ProjectExplorer::RunControl* LinkMotionAnalyzeRunControl::create(LinkMotionRunConfiguration *runConfig, Core::Id runMode) {
    qDebug() << Q_FUNC_INFO;
    ProjectExplorer::Target *target = runConfig->target();
    Analyzer::AnalyzerStartParameters params;
    params.runMode = runMode;
    params.displayName = QStringLiteral("LinkMotion QML Analyzer (%0)").arg(runConfig->target()->project()->displayName());
    params.workingDirectory = target->project()->projectDirectory().toString();
    if (runMode == ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
         params.analyzerHost = QStringLiteral("127.0.0.1");
    }

    Analyzer::AnalyzerRunControl* runControl = Analyzer::AnalyzerManager::createRunControl(params, runConfig);
    (void) new LinkMotionAnalyzeRunControl(runConfig, runControl);
    return runControl;
}


void LinkMotionAnalyzeRunControl::start() {
    qDebug() << Q_FUNC_INFO;
    m_stdout.clear();
    QString gdbServer = QStringLiteral("vmsdk-shell /altdata/apps/%0/bin/%1 -platform eglfs -qmljsdebugger=port:3768,block").arg(m_projectName).arg(m_appName);
    m_process.start(gdbServer);
    m_process.waitForStarted();
}

void LinkMotionAnalyzeRunControl::onFinished(int code, QProcess::ExitStatus status) {
    qDebug() << Q_FUNC_INFO << code << status;
    QMetaObject::invokeMethod(m_runControl, "notifyRemoteFinished", Qt::QueuedConnection);

}

void LinkMotionAnalyzeRunControl::detectGdbServer() {
    qDebug() << Q_FUNC_INFO;
//    m_pid = getProcessPid();
//    m_gdbPort = getPort();
    m_qmlPort = getQmlPort();

    if ( m_qmlPort == -1)  {
        return;
    }

    m_runControl->notifyRemoteSetupDone(m_qmlPort);
    qDebug() << Q_FUNC_INFO << "Notified that analyzer server is running";

}


void LinkMotionAnalyzeRunControl::onStdErr() {
    qDebug() << Q_FUNC_INFO;
    QByteArray lineData = m_process.readAllStandardError();
    m_stdout.append(lineData);
    qDebug() << lineData;
    if ( m_qmlPort == -1) {
        detectGdbServer();
    }
 //   ProjectExplorer::RunControl::appendMessage(QString::fromLatin1(m_process.readAllStandardError()), Utils::ErrorMessageFormat);
}

void LinkMotionAnalyzeRunControl::onStdOut() {
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
int LinkMotionAnalyzeRunControl::getProcessPid() {
    qDebug() << Q_FUNC_INFO;
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

int LinkMotionAnalyzeRunControl::getPort() {
    qDebug() << Q_FUNC_INFO;
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

int LinkMotionAnalyzeRunControl::getQmlPort() {
    qDebug() << Q_FUNC_INFO;
    // QML Debugger: Waiting for connection on port 3768...
    QRegularExpression portExp(QStringLiteral("^QML Debugger: Waiting for connection on port ([0-9]*?)...$"));

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
            qWarning() <<  Q_FUNC_INFO << "Could not detect qml port";
        }
    }
    return portNumber;
}
