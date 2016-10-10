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
#ifndef LINKMOTIONDEPLOYCONFIGURATIONFACTORY_H
#define LINKMOTIONDEPLOYCONFIGURATIONFACTORY_H

#include "linkmotiondeployconfiguration.h"
#include <QDebug>

#include <QObject>
#include <projectexplorer/deployconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDeployConfigurationFactory : public ProjectExplorer::DeployConfigurationFactory
{
public:
    LinkMotionDeployConfigurationFactory(QObject *parent = 0);
    ~LinkMotionDeployConfigurationFactory();

    // used to show the list of possible additons to a target, returns a list of types
    virtual QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent) const;
    // used to translate the types to names to display to the user
    virtual QString displayNameForId(Core::Id id) const;

    virtual bool canCreate(ProjectExplorer::Target *parent, Core::Id id) const;
    virtual ProjectExplorer::DeployConfiguration *create(ProjectExplorer::Target *parent, Core::Id id);
    // used to recreate the runConfigurations when restoring settings
    virtual bool canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const;
    virtual ProjectExplorer::DeployConfiguration *restore(ProjectExplorer::Target *parent, const QVariantMap &map);
    virtual bool canClone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product) const;
    virtual ProjectExplorer::DeployConfiguration *clone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product);

};

}
}

#endif // LINKMOTIONDEPLOYCONFIGURATIONFACTORY_H
