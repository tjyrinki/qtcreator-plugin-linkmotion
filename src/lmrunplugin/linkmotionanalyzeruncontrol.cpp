#include "linkmotionanalyzeruncontrol.h"
#include <projectexplorer/target.h>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <projectexplorer/project.h>
#include <projectexplorer/target.h>

#include <utils/environment.h>

#include <debugger/analyzer/analyzerstartparameters.h>
#include <debugger/analyzer/analyzerruncontrol.h>
#include <debugger/analyzer/analyzermanager.h>
#include "linkmotionanalyzerunrunner.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;


ProjectExplorer::RunControl* LinkMotionAnalyzeRunControl::create(LinkMotionRunConfiguration *runConfig, Core::Id runMode) {
    qDebug() << Q_FUNC_INFO;
    QString appName = runConfig->target()->project()->displayName();

    Debugger::AnalyzerConnection params;

    if (runMode == ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
         params.analyzerHost = QStringLiteral("127.0.0.1");
    }
    params.analyzerPort = Utils::Port(3768);


    Debugger::AnalyzerRunControl* runControl = Debugger::createAnalyzerRunControl(runConfig, runMode);
    runControl->setDisplayName(QStringLiteral("LinkMotion QML Analyzer (%0)").arg(appName));
    runControl->setConnection(params);

    new LinkMotionAnalyzeRunRunner(runConfig, runControl);
    return runControl;
}

/*


void LinkMotionAnalyzeRunControl::detectGdbServer() {
    qDebug() << Q_FUNC_INFO;
//    m_pid = getProcessPid();
//    m_gdbPort = getPort();
    m_qmlPort = getQmlPort();

    if ( m_qmlPort == -1)  {
        return;
    }

    m_runControl->notifyRemoteSetupDone(Utils::Port(m_qmlPort));
    qDebug() << Q_FUNC_INFO << "Notified that analyzer server is running";

}

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
*/
