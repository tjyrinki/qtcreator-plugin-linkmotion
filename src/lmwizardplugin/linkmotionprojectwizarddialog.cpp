#include "linkmotionprojectwizarddialog.h"

#include "../lmprojectplugin/linkmotionkitmatcher.h"
#include <qtsupport/qtkitinformation.h>
#include <qtsupport/qtsupportconstants.h>

#include <QFileInfo>
#include <projectexplorer/project.h>
#include <utils/mimetypes/mimedatabase.h>
#include <projectexplorer/iprojectmanager.h>
#include <extensionsystem/pluginmanager.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProjectWizardDialog::LinkMotionProjectWizardDialog(const Core::BaseFileWizardFactory *factory, QWidget *parent, const Core::WizardDialogParameters &parameters)
    : ProjectExplorer::BaseProjectWizardDialog(factory, parent, parameters)
{
    qDebug() << Q_FUNC_INFO;
    setWindowTitle(tr("New LinkMotion Project"));
    setIntroDescription(tr("This wizard will create LinkMotion project based on LinkMotion UI Toolkit."));


    m_targetSetupPage = new ProjectExplorer::TargetSetupPage;
    const QString platform = selectedPlatform();
    Core::FeatureSet features = Core::FeatureSet(QtSupport::Constants::FEATURE_DESKTOP);
    if (platform.isEmpty())
        m_targetSetupPage->setPreferredKitMatcher(QtSupport::QtKitInformation::qtVersionMatcher(features));
    else
        m_targetSetupPage->setPreferredKitMatcher(QtSupport::QtKitInformation::platformMatcher(platform));

    m_targetSetupPage->setRequiredKitMatcher(LinkMotionKitMatcher());
    setPage(10, m_targetSetupPage);
    wizardProgress()->item(10)->setTitle(tr("LinkMotion Kits"));
}

LinkMotionProjectWizardDialog::~LinkMotionProjectWizardDialog() {
    qDebug() << Q_FUNC_INFO;
    if (m_targetSetupPage)
        m_targetSetupPage->deleteLater();
}


QList<Core::Id> LinkMotionProjectWizardDialog::selectedKits() const {
    qDebug() << Q_FUNC_INFO;
    if(m_targetSetupPage)
        return m_targetSetupPage->selectedKits();

    return QList<Core::Id>();
}


bool LinkMotionProjectWizardDialog::writeUserFile(const QString &projectFileName) const {
    qDebug() << Q_FUNC_INFO;
    if (!m_targetSetupPage)
        return false;

    QFileInfo fi = QFileInfo(projectFileName);
    if (!fi.exists())
        return false;

    QString filePath = fi.canonicalFilePath();

    Utils::MimeDatabase mimeDb;
    const Utils::MimeType mt = mimeDb.mimeTypeForFile(fi);
    if (mt.isValid()) {
        QList<ProjectExplorer::IProjectManager*> allProjectManagers = ExtensionSystem::PluginManager::getObjects<ProjectExplorer::IProjectManager>();
        foreach (ProjectExplorer::IProjectManager *manager, allProjectManagers) {
            if (manager->mimeType() == mt.name()) {
                QString tmp;
                if (ProjectExplorer::Project *pro = manager->openProject(filePath, &tmp)) {

                    bool success = m_targetSetupPage->setupProject(pro);
                    if(success) {
                        pro->saveSettings();
                    }
                    pro->deleteLater();
                    return success;
                }
                break;
            }
        }
    }
    return false;
}
