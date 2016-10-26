/*####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONWELCOMEMODE_H
#define LINKMOTIONWELCOMEMODE_H

#include <coreplugin/imode.h>
#include <QObject>
#include <QtQuickWidgets/QQuickWidget>
#include <projectexplorer/session.h>
#include <projectexplorer/projectexplorer.h>
#include <coreplugin/icore.h>
#include "linkmotionwelcomemodels.h"
#include <coreplugin/iwizardfactory.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionWelcomeMode : public Core::IMode
{
    Q_OBJECT

public:
    LinkMotionWelcomeMode();
    ~LinkMotionWelcomeMode();

public slots:
    void loadSession(QString sessionName) {
        qDebug() << Q_FUNC_INFO;
        ProjectExplorer::SessionManager::loadSession(sessionName);
    }

    void loadProject(QString projectUrl) {
        qDebug() << Q_FUNC_INFO;
        ProjectExplorer::ProjectExplorerPlugin::openProject(projectUrl);
    }

    void newProject()
    {
        qDebug() << Q_FUNC_INFO;
      /*  Core::ICore::showNewItemDialog(tr("New Project"),
                                       Core::IWizardFactory::wizardFactoriesOfKind(Core::IWizardFactory::ProjectWizard));*/
    }

    void openProject()
    {
        qDebug() << Q_FUNC_INFO;
        ProjectExplorer::ProjectExplorerPlugin::openOpenProjectDialog();
    }

protected slots:
    void onSceneGraphError(QQuickWindow::SceneGraphError,QString);

protected:
    QWidget* m_rootWidget;
    QQuickWidget* m_quickWidget;

    LinkMotion::Internal::SessionModel* m_sessionModel;
    LinkMotion::Internal::ProjectModel* m_projectModel;
};

}
}

#endif // LINKMOTIONWELCOMEMODE_H
