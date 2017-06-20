/*
 * Copyright 2013 - 2016 Canonical Ltd.
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
 * Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef LMRUNCONFIGURATIONFACTORY_H
#define LMRUNCONFIGURATIONFACTORY_H

#include <QtGlobal>
#include <QObject>
#include <lmbaseplugin/lmbaseplugin_constants.h>
//#include <lmbaseplugin/lmlocalrunconfiguration.h>

#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/messagemanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <coreplugin/documentmanager.h>
#include <projectexplorer/projectmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/project.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/target.h>
#include <projectexplorer/session.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/applicationlauncher.h>

namespace LmBase {
namespace Internal {

class LinkMotionLocalRunConfigurationFactory : public ProjectExplorer::IRunConfigurationFactory
{
    Q_OBJECT
public:
    explicit LinkMotionLocalRunConfigurationFactory() {
        setObjectName(QLatin1String("LinkMotionLocalRunConfigurationFactory"));
    }

    QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode = UserCreate) const override;
    QString displayNameForId(const Core::Id id) const override;

    bool canCreate(ProjectExplorer::Target *parent, const Core::Id id) const override;
    bool canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const override;
    bool canClone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *product) const override;
    ProjectExplorer::RunConfiguration *clone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *source) override;

    static const char *IdPrefix;

private:
    bool canHandle(ProjectExplorer::Target *parent) const;
    ProjectExplorer::RunConfiguration *doCreate(ProjectExplorer::Target *parent, const Core::Id id) override;
    ProjectExplorer::RunConfiguration *doRestore(ProjectExplorer::Target *parent, const QVariantMap &map) override;
};

}
}
#endif // LMRUNCONFIGURATIONFACTORY_H
