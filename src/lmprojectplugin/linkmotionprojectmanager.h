#ifndef LINKMOTIONPROJECTMANAGER_H
#define LINKMOTIONPROJECTMANAGER_H

#include <QObject>
#include <projectexplorer/iprojectmanager.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    virtual QString mimeType() const override;
    ProjectExplorer::Project* openProject(const QString &filePath, QString *errorString) override;
};

}
}

#endif // LINKMOTIONPROJECTMANAGER_H
