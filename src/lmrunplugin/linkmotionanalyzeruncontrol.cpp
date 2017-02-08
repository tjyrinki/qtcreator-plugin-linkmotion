#include "linkmotionanalyzeruncontrol.h"
#include <projectexplorer/target.h>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QProcess>

#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/kit.h>

#include <utils/environment.h>

#include <debugger/analyzer/analyzerstartparameters.h>
#include <debugger/analyzer/analyzerruncontrol.h>
#include <debugger/analyzer/analyzermanager.h>
#include "linkmotionanalyzerunrunner.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;


ProjectExplorer::RunControl* LinkMotionAnalyzeRunControl::create(LinkMotionRunConfiguration *runConfig, Core::Id runMode) {
    qDebug() << Q_FUNC_INFO;
    if (!runConfig) return NULL;
    if (!runConfig->target()) return NULL;
    if (!runConfig->target()->project()) return NULL;
    if (!runConfig->target()->kit()) return NULL;

    QString appName = runConfig->target()->project()->displayName();

    Debugger::AnalyzerConnection params;

    if (runMode == ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
         params.analyzerHost = QStringLiteral("127.0.0.1");
    }

    // get debugging port
    Utils::Environment env = Utils::Environment::systemEnvironment();
    runConfig->target()->kit()->addToEnvironment(env);

    QProcess p;
    p.setEnvironment(env.toStringList());
    p.start("/bin/bash -c \". /opt/linkmotion/sdk/vm/.vmsdk-target-source; os_qml_port\"");

    if (p.waitForFinished(1000)) {
        QString out = QString::fromLocal8Bit(p.readAllStandardOutput());
        QString qml = out.section('\n', 0, 0);

        bool isNumber;
        quint16 portNum = qml.toLong(&isNumber);
        params.analyzerPort = (isNumber) ? Utils::Port(portNum) : Utils::Port();
    } else {
        qWarning() << Q_FUNC_INFO << "Can't get debugging port number!";
        params.analyzerPort = Utils::Port();
    }

    qDebug() << Q_FUNC_INFO << "For QML Analyzer using port:" << params.analyzerPort.number();

    Debugger::AnalyzerRunControl* runControl = Debugger::createAnalyzerRunControl(runConfig, runMode);
    runControl->setDisplayName(QStringLiteral("LinkMotion QML Analyzer (%0)").arg(appName));
    runControl->setConnection(params);

    new LinkMotionAnalyzeRunRunner(runConfig, runControl);
    return runControl;
}
