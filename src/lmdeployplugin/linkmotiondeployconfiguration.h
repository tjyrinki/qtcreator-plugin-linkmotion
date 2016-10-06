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
#ifndef LINKMOTIONDEPLOYCONFIGURATION_H
#define LINKMOTIONDEPLOYCONFIGURATION_H

#include <QObject>
#include <QDebug>
#include <projectexplorer/buildconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeployConfiguration : public ProjectExplorer::BuildConfiguration
{
    Q_OBJECT

public:
    LinkMotionDeployConfiguration(ProjectExplorer::Target *target);
    LinkMotionDeployConfiguration(ProjectExplorer::Target *target, LinkMotionDeployConfiguration *source);

    virtual bool fromMap(const QVariantMap &map) override;
    virtual QVariantMap toMap() const override;

    virtual ProjectExplorer::NamedWidget *createConfigWidget() override;
    virtual BuildType buildType() const override;

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
