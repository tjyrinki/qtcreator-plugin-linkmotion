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
#include "linkmotionrunconfiguration.h"

#include <QDebug>
#include <qtsupport/qtoutputformatter.h>
#include <projectexplorer/target.h>
#include <debugger/debuggerrunconfigurationaspect.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionRunConfiguration::LinkMotionRunConfiguration(ProjectExplorer::Target *parent, Core::Id id)
    : RunConfiguration(parent, id)
{
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(configurationFinished()),this,SLOT(enableDebuggers()));
}

LinkMotionRunConfiguration::LinkMotionRunConfiguration(ProjectExplorer::Target *parent, LinkMotionRunConfiguration *source)
    : RunConfiguration(parent, source)
{
    qDebug() << Q_FUNC_INFO;
}

void LinkMotionRunConfiguration::enableDebuggers() {
    qDebug() << Q_FUNC_INFO;
    auto aspect = extraAspect<Debugger::DebuggerRunConfigurationAspect>();
    // override the checkboxes in the Projects tab
    aspect->setUseCppDebugger(true);
    aspect->setUseQmlDebugger(true);
}

QWidget *LinkMotionRunConfiguration::createConfigurationWidget()
{
    qDebug() << Q_FUNC_INFO;
    return new QWidget();// no special running configurations
}

Utils::OutputFormatter *LinkMotionRunConfiguration::createOutputFormatter() const
{
    qDebug() << Q_FUNC_INFO;
    return new QtSupport::QtOutputFormatter(target()->project());
}
