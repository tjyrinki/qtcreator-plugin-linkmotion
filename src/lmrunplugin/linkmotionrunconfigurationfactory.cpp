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
#include "linkmotionrunconfigurationfactory.h"
#include "linkmotionrunconfiguration.h"
#include <qmakeprojectmanager/qmakenodes.h>
#include <qmakeprojectmanager/qmakeproject.h>
#include <qmakeprojectmanager/qmakeprojectmanagerconstants.h>
#include <projectexplorer/target.h>

#include <QDebug>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

const char QMAKE_RC_PREFIX[] = "Qt4ProjectManager.Qt4RunConfiguration:";


ProjectExplorer::RunConfiguration *LinkMotionRunConfigurationFactory::clone(ProjectExplorer::Target *parent,
                                                                             ProjectExplorer::RunConfiguration *source) {
    qDebug() << Q_FUNC_INFO;
    if (!canClone(parent, source))
        return NULL;

    return new LinkMotionRunConfiguration(parent,static_cast<LinkMotionRunConfiguration*>(source));
}

bool LinkMotionRunConfigurationFactory::canClone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *product) const {
    qDebug() << Q_FUNC_INFO;
    return canCreate(parent,product->id());
}

bool LinkMotionRunConfigurationFactory::canCreate(ProjectExplorer::Target *parent,
                                                   const Core::Id id) const
{
    qDebug() << Q_FUNC_INFO;
    if (!parent)
        return false;

    //TODO: do id based filtering

    return true;
}

bool LinkMotionRunConfigurationFactory::canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const {
    qDebug() << Q_FUNC_INFO;
    if (!parent)
        return false;

    Core::Id id = ProjectExplorer::idFromMap(map);
    return canCreate(parent, id);
}

QString LinkMotionRunConfigurationFactory::displayNameForId(const Core::Id id) const
{
    qDebug() << Q_FUNC_INFO;
    return QString(QStringLiteral("LinkMotionRunId_%0").arg(id.toString()));
}


QList<Core::Id> LinkMotionRunConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode) const
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(mode);

    QList<Core::Id> types;
    QmakeProjectManager::QmakeProject *project = static_cast<QmakeProjectManager::QmakeProject *>(parent->project());
    QList<QmakeProjectManager::QmakeProFileNode *> nodes = project->applicationProFiles();
    if (mode == AutoCreate)
        nodes = QmakeProjectManager::QmakeProject::nodesWithQtcRunnable(nodes);

    QList<QmakeProjectManager::QmakeProFileNode *> supportedNodes;
    for (int idx=0; idx<nodes.length(); idx++) {
        QmakeProjectManager::QmakeProFileNode* node = nodes.at(idx);
        qDebug() << node->sourceDir();
        if (node->sourceDir().endsWith(QLatin1String("src/app"))) {
            supportedNodes << nodes.at(idx);
        }
    }
    types = QmakeProjectManager::QmakeProject::idsForNodes(Core::Id(), supportedNodes);
    qDebug() << Q_FUNC_INFO << types;

    //TODO: fix this. We will have multiple run configurations from other projectmanagers
    //      and this will have also an empty display name.

    return types;
}

ProjectExplorer::RunConfiguration *LinkMotionRunConfigurationFactory::doCreate(ProjectExplorer::Target *parent, const Core::Id id) {
    if (!canCreate(parent, id))
        return NULL;

    return new LinkMotionRunConfiguration(parent, id);
}


ProjectExplorer::RunConfiguration *LinkMotionRunConfigurationFactory::doRestore(ProjectExplorer::Target *parent, const QVariantMap &map) {
    if (!canRestore(parent, map))
        return NULL;

    ProjectExplorer::RunConfiguration *conf = create(parent,ProjectExplorer::idFromMap(map));
    if(!conf)
        return NULL;
    if(!conf->fromMap(map)) {
        delete conf;
        return NULL;
    }
    return conf;
}
