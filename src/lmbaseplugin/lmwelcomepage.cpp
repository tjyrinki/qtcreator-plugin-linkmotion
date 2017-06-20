/*
 * Copyright 2014 Canonical Ltd.
 * Copyright 2017 Link Motion Oy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Juhapekka Piiroinen <juhapekka.piiroinen@canonical.com>
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#include "lmwelcomepage.h"
#include "lmbaseplugin_constants.h"

#include <QQmlEngine>
#include <QQmlContext>

#include <utils/fileutils.h>
#include <utils/algorithm.h>
#include <coreplugin/iwizardfactory.h>
#include <coreplugin/icore.h>
#include <projectexplorer/projectexplorer.h>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QDir>
#include <QDebug>
#include <QList>
#include <QCoreApplication>
#include <QProcess>
#include <QQuickWidget>
#include <QObject>

using namespace LmBase;
using namespace LmBase::Internal;

QWidget *LinkMotionWelcomePage::createWidget() const
{
    QQuickWidget *widget = new QQuickWidget();
    widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    facilitateQml(widget->engine());
    widget->setSource(pageLocation());
    return widget;
}

QUrl LinkMotionWelcomePage::pageLocation() const
{
    // normalize paths so QML doesn't freak out if it's wrongly capitalized on Windows
    const QString resourcePath = Utils::FileUtils::normalizePathName(QStringLiteral(":/linkmotionbase/qml/welcome.qml"));
    return QUrl::fromLocalFile(resourcePath);
}

QString LinkMotionWelcomePage::title() const
{
    return tr("Link Motion SDK");
}

int LinkMotionWelcomePage::priority() const
{
    return 0;
}

void LinkMotionWelcomePage::facilitateQml(QQmlEngine *engine) const
{
    engine->setOutputWarningsToStandardError(true);
    QQmlContext *context = engine->rootContext();
    context->setContextProperty(QLatin1String("lmWelcomeMode"), this);
}

Core::Id LinkMotionWelcomePage::id() const
{
    return "LinkMotionWelcomePage";
}

void LinkMotionWelcomePage::newProject()
{
    Core::ICore::showNewItemDialog(tr("New Project"), Utils::filtered(Core::IWizardFactory::allWizardFactories(),
                                                                      [](Core::IWizardFactory *f) {
                                                                          return f->kind() == Core::IWizardFactory::ProjectWizard;
                                                                      }));
}

void LinkMotionWelcomePage::openProject()
{
    ProjectExplorer::ProjectExplorerPlugin::instance()->openOpenProjectDialog();
}

void LinkMotionWelcomePage::openGallery()
{
    //QProcess::startDetached(QString::fromLatin1("%1/qtc_launch_gallery").arg(Constants::LM_SCRIPTPATH));
}
