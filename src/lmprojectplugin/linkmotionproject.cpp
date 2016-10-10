#include "linkmotionproject.h"
#include "linkmotionprojectplugin_constants.h"
#include "linkmotionkitmatcher.h"

#include <coreplugin/icontext.h>
#include <qtsupport/qtsupportconstants.h>
#include <coreplugin/documentmanager.h>
#include <qtsupport/qtkitinformation.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProject::LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName) :
    m_projectManager(manager),
    m_fileName(fileName)
{
    qDebug() << Q_FUNC_INFO;
    setId(Constants::LINKMOTIONPROJECT_ID);
    setRequiredKitMatcher(LinkMotionKitMatcher());
    setPreferredKitMatcher(QtSupport::QtKitInformation::qtVersionMatcher(Core::FeatureSet(QtSupport::Constants::FEATURE_DESKTOP)));
    setProjectContext(Core::Context(Constants::LINKMOTIONPROJECT_PROJECTCONTEXT));

    QFileInfo fileInfo(m_fileName);
    m_projectName = fileInfo.completeBaseName();

    m_file = QSharedPointer<LinkMotionProjectFile>(new LinkMotionProjectFile(this, m_fileName));

    Core::DocumentManager::addDocument(m_file.data(), true);
    m_rootNode = QSharedPointer<LinkMotionProjectNode>(new LinkMotionProjectNode(this, m_file.data()));

    //m_projectManager->registerProject(this);
}


QString LinkMotionProject::displayName() const {
    qDebug() << Q_FUNC_INFO;
    return m_projectName;
}

Core::IDocument *LinkMotionProject::document() const {
    qDebug() << Q_FUNC_INFO;
    return m_file.data();
}

ProjectExplorer::IProjectManager *LinkMotionProject::projectManager() const {
    qDebug() << Q_FUNC_INFO;
    return m_projectManager;
}

ProjectExplorer::ProjectNode *LinkMotionProject::rootProjectNode() const {
    qDebug() << Q_FUNC_INFO;
    return m_rootNode.data();
}

QStringList LinkMotionProject::files(FilesMode fileMode) const {
    QStringList files;
    //enumChild(projectDir(), files);
    return files;
}
