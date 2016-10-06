/*####################################################################
#
# This file is part of the LinkMotion Deploy plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#include "linkmotiondeploysettingswidget.h"

#include <utils/qtcassert.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

#include <QFormLayout>

LinkMotionDeploySettingsWidget::LinkMotionDeploySettingsWidget(LinkMotionDeployConfiguration *conf, QWidget *parent)
    : ProjectExplorer::NamedWidget(parent),
      m_deployConfiguration(conf)
{
    qDebug() << Q_FUNC_INFO << conf;

    QFormLayout *fl = new QFormLayout(this);
    fl->setContentsMargins(20, -1, 0, -1);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(fl);

    m_username = new QLineEdit(m_deployConfiguration->m_username,this);
    m_password = new QLineEdit(m_deployConfiguration->m_password,this);
    m_device = new QLineEdit(m_deployConfiguration->m_device,this);
    connect(m_username,SIGNAL(textChanged(QString)),SLOT(onUsernameChanged(QString)));
    connect(m_password,SIGNAL(textChanged(QString)),SLOT(onPasswordChanged(QString)));
    connect(m_device,SIGNAL(textChanged(QString)),SLOT(onDeviceChanged(QString)));

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
    fl->addRow(tr("LINKMOTION_USERNAME"), m_username);
    fl->addRow(tr("LINKMOTION_PASSWORD"), m_password);
    fl->addRow(tr("LINKMOTION_DEVICE"), m_device);

    //connect(m_pathChooser->lineEdit(),SIGNAL(editingFinished()),this,SLOT(onBuilddirChanged()));
    //connect(m_buildConfiguration,SIGNAL(buildDirectoryChanged()),this,SLOT(updateBuildDirectory()));
}

void LinkMotionDeploySettingsWidget::updateBuildDirectory() const
{
    qDebug() << Q_FUNC_INFO;
   /* m_pathChooser->blockSignals(true);
    m_pathChooser->setPath(m_buildConfiguration->rawBuildDirectory().toString());
    m_pathChooser->blockSignals(false);*/
}

void LinkMotionDeploySettingsWidget::onUsernameChanged(QString username) {
    qDebug() << Q_FUNC_INFO;
    m_deployConfiguration->m_username = username;
}

void LinkMotionDeploySettingsWidget::onPasswordChanged(QString password) {
    qDebug() << Q_FUNC_INFO;
    m_deployConfiguration->m_password = password;
}

void LinkMotionDeploySettingsWidget::onDeviceChanged(QString device) {
    qDebug() << Q_FUNC_INFO;
    m_deployConfiguration->m_device = device;
}
