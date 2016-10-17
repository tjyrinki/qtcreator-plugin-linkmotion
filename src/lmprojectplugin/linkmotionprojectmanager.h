#ifndef LINKMOTIONPROJECTMANAGER_H
#define LINKMOTIONPROJECTMANAGER_H

#include <QObject>
#include <projectexplorer/iprojectmanager.h>
#include <qmakeprojectmanager/qmakeprojectmanager.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionProjectManager : public QmakeProjectManager::QmakeManager
{
    Q_OBJECT

public:
    LinkMotionProjectManager() : QmakeProjectManager::QmakeManager() {
        qDebug() << Q_FUNC_INFO;
    }
    ~LinkMotionProjectManager() {
        qDebug() << Q_FUNC_INFO;
    }
    virtual QString mimeType() const override;
    ProjectExplorer::Project* openProject(const QString &filePath, QString *errorString) override;
};

}
}

#endif // LINKMOTIONPROJECTMANAGER_H
