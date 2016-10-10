#include "linkmotionprojectmanager.h"
#include "linkmotionprojectplugin_constants.h"

#include <QFileInfo>
#include <projectexplorer/session.h>

#include "linkmotionproject.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;


QString LinkMotionProjectManager::mimeType() const {
    qDebug() << Q_FUNC_INFO;
    return QLatin1String(Constants::LINKMOTIONPROJECT_MIMETYPE);
}

ProjectExplorer::Project* LinkMotionProjectManager::openProject(const QString &filePath, QString *errorString) {
    qDebug() << Q_FUNC_INFO;
    QFileInfo fileInfo(filePath);

    foreach (ProjectExplorer::Project *pi, ProjectExplorer::SessionManager::projects()) {
        if (filePath == pi->document()->filePath().toString()) {
            if (errorString)
                *errorString = tr("Failed opening project '%1': Project already open") .arg(QDir::toNativeSeparators(filePath));
            return 0;
        }
    }

    if (fileInfo.isFile())
        return new LinkMotionProject(this, filePath);

    *errorString = tr("Failed opening project '%1': Project file is not a file").arg(QDir::toNativeSeparators(filePath));
    return 0;
}
