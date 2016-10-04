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

#include "linkmotionbuildsettingswidget.h"

#include <utils/qtcassert.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

#include <QFormLayout>

LinkMotionBuildSettingsWidget::LinkMotionBuildSettingsWidget(LinkMotionBuildConfiguration *conf, QWidget *parent)
    : ProjectExplorer::NamedWidget(parent) ,
      m_buildConfiguration(conf)
{
    qDebug() << Q_FUNC_INFO << conf;
/*
    QFormLayout *fl = new QFormLayout(this);
    fl->setContentsMargins(20, -1, 0, -1);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(fl);
*/
    //TODO: Add
    // Linkmotion username
    // linkmotion password
    // linkmotion device
    // and then those will be set to environment on build step.

   // m_pathChooser = new Utils::PathChooser(this);
   /* if (m_buildConfiguration) {
    m_pathChooser->setPath(m_buildConfiguration->rawBuildDirectory().toString());
    }*/
    //fl->addRow(tr("LM Build directory:"), m_pathChooser);

    //connect(m_pathChooser->lineEdit(),SIGNAL(editingFinished()),this,SLOT(onBuilddirChanged()));
    //connect(m_buildConfiguration,SIGNAL(buildDirectoryChanged()),this,SLOT(updateBuildDirectory()));
}

void LinkMotionBuildSettingsWidget::updateBuildDirectory() const
{
    qDebug() << Q_FUNC_INFO;
   /* m_pathChooser->blockSignals(true);
    m_pathChooser->setPath(m_buildConfiguration->rawBuildDirectory().toString());
    m_pathChooser->blockSignals(false);*/
}
