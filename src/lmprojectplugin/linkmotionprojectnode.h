/*####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONPROJECTNODE_H
#define LINKMOTIONPROJECTNODE_H

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include "linkmotionprojectmanager.h"
#include "linkmotionproject.h"

namespace LinkMotion {
namespace Internal {

class LinkMotionProject;

class LinkMotionProjectNode : public ProjectExplorer::ProjectNode
{
public:
    LinkMotionProjectNode(LinkMotionProject *project, Core::IDocument *projectFile);

    QList<ProjectExplorer::ProjectAction> supportedActions(Node *node) const override;

    virtual bool canAddSubProject(const QString &proFilePath) const override;
    virtual bool addSubProjects(const QStringList &proFilePaths) override;
    virtual bool removeSubProjects(const QStringList &proFilePaths) override;
    virtual bool addFiles(const QStringList &filePaths, QStringList *notAdded = 0) override;
    virtual bool removeFiles(const QStringList &filePaths, QStringList *notRemoved = 0) override;
    virtual bool deleteFiles(const QStringList &filePaths) override;
    virtual bool renameFile(const QString &filePath, const QString &newFilePath) override;

protected:
    LinkMotionProject *m_project;
    Core::IDocument *m_projectFile;

};

}
}

#endif // LINKMOTIONPROJECTNODE_H
