#ifndef LINKMOTIONPROJECT_H
#define LINKMOTIONPROJECT_H

#include <qmakeprojectmanager/qmakeproject.h>
#include <projectexplorer/project.h>

#include "linkmotionprojectmanager.h"
#include "linkmotionprojectfile.h"
#include "linkmotionprojectnode.h"


namespace LinkMotion {
namespace Internal {

class LinkMotionProjectFile;
class LinkMotionProjectNode;

class LinkMotionProject : public QmakeProjectManager::QmakeProject
{
public:
    LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName);

protected:
    LinkMotionProjectManager *m_projectManager;
    QString m_lmprojectFile;

};

}
}

#endif // LINKMOTIONPROJECT_H
