/*
 * Copyright 2016 Canonical Ltd.
 * Copyright 2017 Link Motion Oy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef LM_INTERNAL_LOCALDEPLOYCONFIGURATIONFACTORY_H
#define LM_INTERNAL_LOCALDEPLOYCONFIGURATIONFACTORY_H

#include <projectexplorer/deployconfiguration.h>

namespace LmBase {
namespace Internal {

class LinkMotionLocalDeployConfigurationFactory : public ProjectExplorer::DeployConfigurationFactory
{
    Q_OBJECT

public:
    explicit LinkMotionLocalDeployConfigurationFactory(QObject *parent = 0);

    QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent) const override;
    QString displayNameForId(Core::Id id) const override;
    bool canCreate(ProjectExplorer::Target *parent, Core::Id id) const override;
    ProjectExplorer::DeployConfiguration *create(ProjectExplorer::Target *parent, Core::Id id) override;
    bool canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const override;
    ProjectExplorer::DeployConfiguration *restore(ProjectExplorer::Target *parent, const QVariantMap &map) override;

    bool canClone(ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *product) const;
    ProjectExplorer::DeployConfiguration *clone(ProjectExplorer::Target *parent,
                                                ProjectExplorer::DeployConfiguration *product) override;
};

class LinkMotionLocalDeployConfiguration : public ProjectExplorer::DeployConfiguration
{
    Q_OBJECT
    friend class LinkMotionLocalDeployConfigurationFactory; // for the ctors
protected:
    LinkMotionLocalDeployConfiguration(ProjectExplorer::Target *target, const Core::Id id);
    LinkMotionLocalDeployConfiguration(ProjectExplorer::Target *target, LinkMotionLocalDeployConfiguration *source);

protected slots:
    void selectAsDefaultHack();
};

} // namespace Internal
} // namespace LmBase

#endif // LM_INTERNAL_LOCALDEPLOYCONFIGURATIONFACTORY_H
