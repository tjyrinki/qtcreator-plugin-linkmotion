/*####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionwelcomemodels.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QFileInfo>
#include <QDir>

#include <coreplugin/icore.h>
#include <coreplugin/iwizardfactory.h>
#include <projectexplorer/session.h>
#include <projectexplorer/projectexplorer.h>

#include <utils/fileutils.h>
#include <utils/stringutils.h>
#include <utils/algorithm.h>

using namespace LinkMotion;
using namespace Internal;
using namespace ProjectExplorer;

SessionModel::SessionModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(SessionManager::instance(), SIGNAL(sessionLoaded(QString)), SLOT(resetSessions()));
}

int SessionModel::rowCount(const QModelIndex &) const
{
    return SessionManager::sessions().count();
}

QStringList pathsToBaseNames(const QStringList &paths)
{
    return Utils::transform(paths, [](const QString &path) {
        return QFileInfo(path).completeBaseName();
    });
}



QStringList pathsWithTildeHomePath(const QStringList &paths)
{
    return Utils::transform(paths, [](const QString &path) {
        return Utils::withTildeHomePath(QDir::toNativeSeparators(path));
    });
}

QVariant SessionModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == DefaultSessionRole ||
            role == LastSessionRole || role == ActiveSessionRole || role == ProjectsPathRole  || role == ProjectsDisplayRole) {
        QString sessionName = SessionManager::sessions().at(index.row());
        if (role == Qt::DisplayRole)
            return sessionName;
        else if (role == DefaultSessionRole)
            return SessionManager::isDefaultSession(sessionName);
        else if (role == LastSessionRole)
            return SessionManager::lastSession() == sessionName;
        else if (role == ActiveSessionRole)
            return SessionManager::activeSession() == sessionName;
        else if (role == ProjectsPathRole)
            return pathsWithTildeHomePath(SessionManager::projectsForSessionName(sessionName));
        else if (role == ProjectsDisplayRole)
            return pathsToBaseNames(SessionManager::projectsForSessionName(sessionName));
    }
    return QVariant();
}

QHash<int, QByteArray> SessionModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[Qt::DisplayRole] = "sessionName";
    roleNames[DefaultSessionRole] = "defaultSession";
    roleNames[ActiveSessionRole] = "activeSession";
    roleNames[LastSessionRole] = "lastSession";
    roleNames[ProjectsPathRole] = "projectsPath";
    roleNames[ProjectsDisplayRole] = "projectsName";
    return roleNames;
}

bool SessionModel::isDefaultVirgin() const
{
    return SessionManager::isDefaultVirgin();
}

void SessionModel::resetSessions()
{
    beginResetModel();
    endResetModel();
}

void SessionModel::deleteSession(const QString &session)
{
    if (!SessionManager::confirmSessionDelete(session))
        return;
    beginResetModel();
    SessionManager::deleteSession(session);
    endResetModel();
}

ProjectModel::ProjectModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(ProjectExplorerPlugin::instance(), &ProjectExplorerPlugin::recentProjectsChanged,
            this, &ProjectModel::resetProjects);
}

int ProjectModel::rowCount(const QModelIndex &) const
{
    return ProjectExplorerPlugin::recentProjects().count();
}


QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    QPair<QString,QString> data = ProjectExplorerPlugin::recentProjects().at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return data.second;
        break;
    case FilePathRole:
        return data.first;
    case PrettyFilePathRole:
        return Utils::withTildeHomePath(data.first);
    default:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> ProjectModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[Qt::DisplayRole] = "displayName";
    roleNames[FilePathRole] = "filePath";
    roleNames[PrettyFilePathRole] = "prettyFilePath";
    return roleNames;
}

void ProjectModel::resetProjects()
{
    beginResetModel();
    endResetModel();
}
