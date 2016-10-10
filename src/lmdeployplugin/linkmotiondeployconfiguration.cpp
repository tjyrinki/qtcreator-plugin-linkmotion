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
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDeployConfiguration::LinkMotionDeployConfiguration(ProjectExplorer::Target *target)
    : DeployConfiguration(target,Constants::LINKMOTION_DC_ID)
{
    qDebug() << Q_FUNC_INFO;
    ctor();
}

LinkMotionDeployConfiguration::LinkMotionDeployConfiguration(ProjectExplorer::Target *target, LinkMotionDeployConfiguration *source)
    : DeployConfiguration(target,source)
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
    return DeployConfiguration::fromMap(map);
}

QVariantMap LinkMotionDeployConfiguration::toMap() const
{
    qDebug() << Q_FUNC_INFO;
    return DeployConfiguration::toMap();
}

ProjectExplorer::NamedWidget *LinkMotionDeployConfiguration::createConfigWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionDeploySettingsWidget(this);
}

