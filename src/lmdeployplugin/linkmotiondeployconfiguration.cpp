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
#include "linkmotiondeployconfiguration.h"
#include "linkmotiondeploysettingswidget.h"
#include "linkmotiondeployplugin_constants.h"
#include <QProcessEnvironment>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDeployConfiguration::LinkMotionDeployConfiguration(ProjectExplorer::Target *target)
    : BuildConfiguration(target,Constants::LINKMOTION_BC_ID)
{
    qDebug() << Q_FUNC_INFO;
    ctor();
}

LinkMotionDeployConfiguration::LinkMotionDeployConfiguration(ProjectExplorer::Target *target, LinkMotionDeployConfiguration *source)
    : BuildConfiguration(target,source)
{
    qDebug() << Q_FUNC_INFO;
    ctor();

}

void LinkMotionDeployConfiguration::ctor() {
    qDebug() << Q_FUNC_INFO;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_username = env.value(QStringLiteral("LINKMOTION_USERNAME"),QStringLiteral("linkmotion"));
    m_password = env.value(QStringLiteral("LINKMOTION_PASSWORD"),QStringLiteral("[not set]"));
    m_device = env.value(QStringLiteral("LINKMOTION_DEVICE"),QStringLiteral("intel"));
}

bool LinkMotionDeployConfiguration::fromMap(const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    return BuildConfiguration::fromMap(map);
}

QVariantMap LinkMotionDeployConfiguration::toMap() const
{
    qDebug() << Q_FUNC_INFO;
    return BuildConfiguration::toMap();
}

ProjectExplorer::NamedWidget *LinkMotionDeployConfiguration::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionDeploySettingsWidget(this);
}

ProjectExplorer::BuildConfiguration::BuildType LinkMotionDeployConfiguration::buildType() const
{
    qDebug() << Q_FUNC_INFO;
    return Release;
}
