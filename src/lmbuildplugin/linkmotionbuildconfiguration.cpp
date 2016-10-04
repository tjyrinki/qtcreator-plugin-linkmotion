/*####################################################################
#
# This file is part of the LinkMotion Build plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionbuildconfiguration.h"
#include "linkmotionbuildsettingswidget.h"
#include "buildplugin_constants.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionBuildConfiguration::LinkMotionBuildConfiguration(ProjectExplorer::Target *target)
    : BuildConfiguration(target,Constants::LINKMOTION_BC_ID)
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionBuildConfiguration::LinkMotionBuildConfiguration(ProjectExplorer::Target *target, LinkMotionBuildConfiguration *source)
    : BuildConfiguration(target,source)
{
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionBuildConfiguration::fromMap(const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    return BuildConfiguration::fromMap(map);
}

QVariantMap LinkMotionBuildConfiguration::toMap() const
{
    qDebug() << Q_FUNC_INFO;
    return BuildConfiguration::toMap();
}

ProjectExplorer::NamedWidget *LinkMotionBuildConfiguration::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionBuildSettingsWidget(this);
}

ProjectExplorer::BuildConfiguration::BuildType LinkMotionBuildConfiguration::buildType() const
{
    qDebug() << Q_FUNC_INFO;
    return Release;
}
