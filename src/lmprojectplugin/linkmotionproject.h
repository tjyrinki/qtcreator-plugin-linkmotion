#ifndef LINKMOTIONPROJECT_H
#define LINKMOTIONPROJECT_H

#include <projectexplorer/project.h>

#include "linkmotionprojectmanager.h"
#include "linkmotionprojectfile.h"
#include "linkmotionprojectnode.h"


namespace LinkMotion {
namespace Internal {

class LinkMotionProjectFile;
class LinkMotionProjectNode;

class LinkMotionProject : public ProjectExplorer::Project
{
public:
    LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName);

    QString displayName() const override;
    Core::IDocument *document() const override;
    ProjectExplorer::IProjectManager *projectManager() const override;

    ProjectExplorer::ProjectNode *rootProjectNode() const override;
    QStringList files(FilesMode fileMode) const override;

protected:
    LinkMotionProjectManager *m_projectManager;

    QString m_projectName;
    QString m_fileName;
    QString m_mainFile;

    QSharedPointer<LinkMotionProjectFile> m_file;
    QSharedPointer<LinkMotionProjectNode> m_rootNode;
};

}
}

#endif // LINKMOTIONPROJECT_H
