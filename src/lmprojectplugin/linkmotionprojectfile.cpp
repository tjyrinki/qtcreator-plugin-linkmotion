#include "linkmotionprojectfile.h"

#include "linkmotionprojectplugin_constants.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;


LinkMotionProjectFile::LinkMotionProjectFile(LinkMotionProject *project, QString fileName)
    : m_project(project),
      m_filename(fileName)
{
    qDebug() << Q_FUNC_INFO;
    setFilePath(Utils::FileName::fromString(fileName));
    setMimeType(QLatin1String(Constants::LINKMOTIONPROJECT_MIMETYPE));
}

bool LinkMotionProjectFile::save(QString *errorString, const QString &fileName, bool autoSave) {
    qDebug() << Q_FUNC_INFO;
    return false;

}

QString LinkMotionProjectFile::defaultPath() const {
    qDebug() << Q_FUNC_INFO;
    return QString();

}

QString LinkMotionProjectFile::suggestedFileName() const {
    qDebug() << Q_FUNC_INFO;
    return QString();
}

bool LinkMotionProjectFile::isModified() const {
    qDebug() << Q_FUNC_INFO;
    return false;

}

bool LinkMotionProjectFile::isSaveAsAllowed() const {
    qDebug() << Q_FUNC_INFO;
    return false;

}

Core::IDocument::ReloadBehavior LinkMotionProjectFile::reloadBehavior(ChangeTrigger state, ChangeType type) const {
    return Core::IDocument::BehaviorSilent;
}

bool LinkMotionProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType) {
    return true;
}
