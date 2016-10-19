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
#include "linkmotionbuildplugin_constants.h"
#include <QProcessEnvironment>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionBuildConfiguration::LinkMotionBuildConfiguration(ProjectExplorer::Target *target)
    : BuildConfiguration(target,Constants::LINKMOTION_BC_ID)
{
    qDebug() << Q_FUNC_INFO;
    ctor();
}

LinkMotionBuildConfiguration::LinkMotionBuildConfiguration(ProjectExplorer::Target *target, LinkMotionBuildConfiguration *source)
    : BuildConfiguration(target,source)
{
    qDebug() << Q_FUNC_INFO;
    ctor();

}

void LinkMotionBuildConfiguration::ctor() {
    qDebug() << Q_FUNC_INFO;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_username = env.value(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    m_password = env.value(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("[not set]"));
    m_device = env.value(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
    if (!this->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD)) {
        qDebug() << "WARNING" << "BUILD STEPS ARE NULL";
        //m_stepLists.append(new ProjectExplorer::BuildStepList(this,ProjectExplorer::Constants::BUILDSTEPS_BUILD));
    }
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
