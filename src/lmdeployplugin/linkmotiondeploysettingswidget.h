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
#ifndef LINKMOTIONDEPLOYSETTINGSWIDGET_H
#define LINKMOTIONDEPLOYSETTINGSWIDGET_H

#include <QObject>
#include <QDebug>

#include <utils/pathchooser.h>
#include <projectexplorer/namedwidget.h>
#include <projectexplorer/buildconfiguration.h>

#include "linkmotiondeployconfiguration.h"

namespace LinkMotion {
namespace Internal {

class LinkMotionDeploySettingsWidget : public ProjectExplorer::NamedWidget
{
    Q_OBJECT

public:
    explicit LinkMotionDeploySettingsWidget(LinkMotionDeployConfiguration *conf, QWidget *parent = 0);

    QString username() { return m_deployConfiguration->m_username; }
    QString password() { return m_deployConfiguration->m_password; }
    QString device() { return m_deployConfiguration->m_device; }

public slots:
    void onUsernameChanged(QString username);
    void onPasswordChanged(QString password);
    void onDeviceChanged(QString device);

private slots:
    void updateBuildDirectory() const;

private:
    LinkMotionDeployConfiguration *m_deployConfiguration;

    QLineEdit* m_username;
    QLineEdit* m_password;
    QLineEdit* m_device;
};
}
}

#endif // LINKMOTIONDEPLOYSETTINGSWIDGET_H
