#include "linkmotiondebugruncontrolfactory.h"

#include "linkmotiondebugruncontrol.h"
#include "../lmrunplugin/linkmotionrunconfiguration.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDebugRunControlFactory::LinkMotionDebugRunControlFactory(QObject *parent) {
    qDebug() << Q_FUNC_INFO;
}

bool LinkMotionDebugRunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const {
    qDebug() << Q_FUNC_INFO << mode;
    if (mode != ProjectExplorer::Constants::DEBUG_RUN_MODE && mode != ProjectExplorer::Constants::DEBUG_RUN_MODE_WITH_BREAK_ON_MAIN) {
        return false;
    }
    qDebug() << "Should be ok";
    return qobject_cast<LinkMotionRunConfiguration *>(runConfiguration);
}

ProjectExplorer::RunControl* LinkMotionDebugRunControlFactory::create(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode, QString *errorMessage) {
    qDebug() << Q_FUNC_INFO;
    LinkMotionRunConfiguration *rc = qobject_cast<LinkMotionRunConfiguration *>(runConfiguration);
    Q_ASSERT(rc);
    if (mode == ProjectExplorer::Constants::DEBUG_RUN_MODE || mode == ProjectExplorer::Constants::DEBUG_RUN_MODE_WITH_BREAK_ON_MAIN)
        return LinkMotionDebugRunControl::create(rc, errorMessage);

    return 0;
}



