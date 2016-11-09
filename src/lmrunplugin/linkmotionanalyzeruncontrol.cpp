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
    if (!runConfig) return NULL;
    if (!runConfig->target()) return NULL;
    if (!runConfig->target()->project()) return NULL;

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
