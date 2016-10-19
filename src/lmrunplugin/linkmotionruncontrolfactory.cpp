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

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunControlFactory::LinkMotionRunControlFactory(QObject *parent)
    : IRunControlFactory(parent)
{
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionRunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const
{
    qDebug() << Q_FUNC_INFO;
    if (mode != ProjectExplorer::Constants::NORMAL_RUN_MODE) {
        return false;
    }
    return qobject_cast<LinkMotionRunConfiguration *>(runConfiguration);
}

ProjectExplorer::RunControl *LinkMotionRunControlFactory::create(ProjectExplorer::RunConfiguration *runConfig,
                                        Core::Id mode, QString *errorMessage)
{
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT(canRun(runConfig, mode));
    LinkMotionRunConfiguration *rc = qobject_cast<LinkMotionRunConfiguration *>(runConfig);
    Q_ASSERT(rc);
    if (mode == ProjectExplorer::Constants::NORMAL_RUN_MODE)
        return new LinkMotionRunControl(rc);

    QTC_CHECK(false);
    return 0;
}
