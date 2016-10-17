#include "linkmotionprojectmanager.h"
#include "linkmotionprojectplugin_constants.h"

#include <QFileInfo>
#include <projectexplorer/session.h>

#include "linkmotionproject.h"

#include <QSettings>

using namespace LinkMotion;
using namespace LinkMotion::Internal;


QString LinkMotionProjectManager::mimeType() const {
    qDebug() << Q_FUNC_INFO;
    return QLatin1String(Constants::LINKMOTIONPROJECT_MIMETYPE);
}

ProjectExplorer::Project* LinkMotionProjectManager::openProject(const QString &filePath, QString *errorString) {
    qDebug() << Q_FUNC_INFO << filePath;
    QSettings* lmprojectReader = new QSettings(filePath,QSettings::IniFormat);
    lmprojectReader->beginGroup(QStringLiteral("project"));
    QString proFileName = lmprojectReader->value(QStringLiteral("MainProjectFile")).toString();
    lmprojectReader->deleteLater();

    qDebug() << "Read the projectFile" << proFileName;
    QFileInfo fileInfoProject(filePath);

    QString proFilePath = QStringLiteral("%0/%1").arg(fileInfoProject.dir().absolutePath()).arg(proFileName);
    qDebug() << "Full path is" << proFilePath;
    QFileInfo fileInfo(proFilePath);

    foreach (ProjectExplorer::Project *pi, ProjectExplorer::SessionManager::projects()) {
        if (filePath == pi->document()->filePath().toString()) {
            if (errorString)
                *errorString = tr("Failed opening project '%1': Project already open") .arg(QDir::toNativeSeparators(filePath));
            return 0;
        }
    }

    if (fileInfo.isFile())
        return new LinkMotionProject(this, proFilePath);

    *errorString = tr("Failed opening project '%1': Project file is not a file").arg(QDir::toNativeSeparators(filePath));
    return 0;
}

