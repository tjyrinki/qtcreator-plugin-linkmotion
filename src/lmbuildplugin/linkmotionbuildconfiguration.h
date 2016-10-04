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
#ifndef LINKMOTIONBUILDCONFIGURATION_H
#define LINKMOTIONBUILDCONFIGURATION_H

#include <QObject>
#include <QDebug>
#include <projectexplorer/buildconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionBuildConfiguration : public ProjectExplorer::BuildConfiguration
{
    Q_OBJECT

public:
    LinkMotionBuildConfiguration(ProjectExplorer::Target *target);
    LinkMotionBuildConfiguration(ProjectExplorer::Target *target, LinkMotionBuildConfiguration *source);

    virtual bool fromMap(const QVariantMap &map) override;
    virtual QVariantMap toMap() const override;

    virtual ProjectExplorer::NamedWidget *createConfigWidget() override;
    virtual BuildType buildType() const override;

protected:
    friend class LinkMotionBuildConfigurationFactory;
};

}
}

#endif // LINKMOTIONBUILDCONFIGURATION_H
