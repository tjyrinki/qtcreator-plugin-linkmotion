#include "linkmotionprojectnode.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProjectNode::LinkMotionProjectNode(LinkMotionProject *project, Core::IDocument *projectFile)
 : ProjectExplorer::ProjectNode(projectFile->filePath()),
   m_project(project),
   m_projectFile(projectFile) {

    setDisplayName(projectFile->filePath().toFileInfo().completeBaseName());

}

QList<ProjectExplorer::ProjectAction> LinkMotionProjectNode::supportedActions(Node *node) const {
    qDebug() << Q_FUNC_INFO;
    QList<ProjectExplorer::ProjectAction> actions;
    return actions;
}

bool LinkMotionProjectNode::canAddSubProject(const QString &proFilePath) const {
    qDebug() << Q_FUNC_INFO;
    return false;
}

bool LinkMotionProjectNode::addSubProjects(const QStringList &proFilePaths) {
    qDebug() << Q_FUNC_INFO;
    return false;
}
bool LinkMotionProjectNode::removeSubProjects(const QStringList &proFilePaths) {
    qDebug() << Q_FUNC_INFO;
    return false;
}

bool LinkMotionProjectNode::addFiles(const QStringList &, QStringList *) {
    qDebug() << Q_FUNC_INFO;
    return false;
}

bool LinkMotionProjectNode::removeFiles(const QStringList &, QStringList *) {
    qDebug() << Q_FUNC_INFO;
    return false;
}

bool LinkMotionProjectNode::deleteFiles(const QStringList &) {
    qDebug() << Q_FUNC_INFO;
    return false;
}

bool LinkMotionProjectNode::renameFile(const QString &, const QString &) {
    qDebug() << Q_FUNC_INFO;
    return false;
}
