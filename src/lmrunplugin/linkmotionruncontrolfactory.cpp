/*####################################################################
#
# This file is part of the LinkMotion Run plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionruncontrolfactory.h"
#include <QDebug>


#include "linkmotionruncontrol.h"
#include "linkmotionrunconfiguration.h"
#include "linkmotiondebugruncontrol.h"
#include "linkmotionanalyzeruncontrol.h"
#include <projectexplorer/taskhub.h>
#include "linkmotionrunplugin_constants.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunControlFactory::LinkMotionRunControlFactory(QObject *parent)
    : IRunControlFactory(parent)
{
    qDebug() << Q_FUNC_INFO;
    ProjectExplorer::TaskHub::addCategory(Core::Id(LinkMotion::Constants::TASK_CATEGORY_DEBUG),QStringLiteral("Debug"), true);
    ProjectExplorer::TaskHub::addCategory(Core::Id(LinkMotion::Constants::TASK_CATEGORY_RUN),QStringLiteral("Run"), true);
    ProjectExplorer::TaskHub::addCategory(Core::Id(LinkMotion::Constants::TASK_CATEGORY_ANALYZE),QStringLiteral("Analyze"), true);
}

bool LinkMotionRunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const
{
    qDebug() << Q_FUNC_INFO << mode;
    if (mode != ProjectExplorer::Constants::DEBUG_RUN_MODE &&
            mode != ProjectExplorer::Constants::DEBUG_RUN_MODE_WITH_BREAK_ON_MAIN &&
            mode != ProjectExplorer::Constants::NORMAL_RUN_MODE &&
            mode != ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
        return false;
    }
    return qobject_cast<LinkMotionRunConfiguration *>(runConfiguration);
}

ProjectExplorer::RunControl *LinkMotionRunControlFactory::create(ProjectExplorer::RunConfiguration *runConfig, Core::Id mode, QString *errorMessage)
{
    Q_UNUSED(errorMessage)

    qDebug() << Q_FUNC_INFO << mode;
    Q_ASSERT(canRun(runConfig, mode));
    LinkMotionRunConfiguration *rc = qobject_cast<LinkMotionRunConfiguration *>(runConfig);
    Q_ASSERT(rc);
    if (mode == ProjectExplorer::Constants::NORMAL_RUN_MODE) {
        return new LinkMotionRunControl(rc);
    } else if (mode == ProjectExplorer::Constants::DEBUG_RUN_MODE || mode == ProjectExplorer::Constants::DEBUG_RUN_MODE_WITH_BREAK_ON_MAIN) {
        QString errorMessage;
        ProjectExplorer::RunControl* retval = LinkMotionDebugRunControl::create(rc, &errorMessage);
        if (!errorMessage.isEmpty()) {
            qWarning() << Q_FUNC_INFO << errorMessage;
        }
        return retval;
    } else if (mode == ProjectExplorer::Constants::QML_PROFILER_RUN_MODE) {
        return LinkMotionAnalyzeRunControl::create(rc, mode);
    } else {
        qWarning() << Q_FUNC_INFO << "UNHANDLED mode detected";
    }

    QTC_CHECK(false);
    return 0;
}
