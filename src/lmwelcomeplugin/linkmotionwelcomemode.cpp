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

#include "linkmotionwelcomemode.h"
#include "linkmotionwelcomeplugin_constants.h"

#include <QtQml>
#include <QVBoxLayout>
#include <QtQuickWidgets/QQuickWidget>
#include <projectexplorer/projectwelcomepage.h>
#include <utils/fileutils.h>
#include <utils/styledbar.h>
#include <coreplugin/icore.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;
using namespace LinkMotion::Welcome;

LinkMotionWelcomeMode::LinkMotionWelcomeMode() {
    qDebug() << Q_FUNC_INFO;
    setDisplayName(tr("LinkMotion"));
    setIcon(QIcon(QLatin1String(":/linkmotion/qml/LM_logo_boxed_2.png")));
    setPriority(Constants::P_MODE_LINKMOTION);
    setId(Constants::MODE_LINKMOTION);
    setContext(Core::Context(Constants::C_LINKMOTION_MODE));

    m_rootWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_rootWidget);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_quickWidget = new QQuickWidget(m_rootWidget);
/*
    QStringList importPaths = m_quickWidget->engine()->importPathList();
    const QString resourcePath = Utils::FileUtils::normalizePathName(Core::ICore::resourcePath());
    importPaths << resourcePath + QLatin1String("/welcomescreen/");

    m_quickWidget->engine()->setImportPathList(importPaths);
    QQmlContext *ctx = m_quickWidget->engine()->rootContext();
    ctx->setContextProperty(QLatin1String("linkMotionWelcome"),this);
*/
    connect(m_quickWidget, SIGNAL(sceneGraphError(QQuickWindow::SceneGraphError,QString)),
            this, SLOT(onSceneGraphError(QQuickWindow::SceneGraphError,QString)));
    layout->addWidget(m_quickWidget);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_quickWidget->setSource(QUrl(Constants::LINKMOTION_WELCOMESCREEN_QML));

    m_rootWidget->setLayout(layout);
    setWidget(m_rootWidget);
}

LinkMotionWelcomeMode::~LinkMotionWelcomeMode() {
    qDebug() << Q_FUNC_INFO;

}

void LinkMotionWelcomeMode::onSceneGraphError(QQuickWindow::SceneGraphError,QString) {
    qDebug() << Q_FUNC_INFO;
}
