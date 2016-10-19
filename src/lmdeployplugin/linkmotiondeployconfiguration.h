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
#ifndef LINKMOTIONDEPLOYCONFIGURATION_H
#define LINKMOTIONDEPLOYCONFIGURATION_H

#include <QObject>
#include <QDebug>
#include <projectexplorer/deployconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeployConfiguration : public ProjectExplorer::DeployConfiguration
{
    Q_OBJECT

public:
    LinkMotionDeployConfiguration(ProjectExplorer::Target *target);
    LinkMotionDeployConfiguration(ProjectExplorer::Target *target, LinkMotionDeployConfiguration *source);

    virtual bool fromMap(const QVariantMap &map) override;
    virtual QVariantMap toMap() const override;

    virtual ProjectExplorer::NamedWidget *createConfigWidget() override;

    QString m_username;
    QString m_password;
    QString m_device;

protected:
    friend class LinkMotionDeployConfigurationFactory;

private:
    void ctor();

};

}
}

#endif // LINKMOTIONDEPLOYCONFIGURATION_H
