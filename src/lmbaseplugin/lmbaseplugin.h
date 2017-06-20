/*
 * Copyright 2013 Canonical Ltd.
 * Copyright 2017 Link Motion Oy.
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
 * Authors:
 * Juhapekka Piiroinen <juhapekka.piiroinen@canonical.com>
 * Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef LMBASEPLUGIN_INCLUDED_H
#define LMBASEPLUGIN_INCLUDED_H

#include "lmbaseplugin_global.h"
#include "settings.h"
#if 0
#include "ubuntudevicemode.h"
#include "ubuntupackagingmode.h"
#include "ubuntusettingsdeviceconnectivitypage.h"
#include "ubuntusettingsclickpage.h"
#endif

#include <extensionsystem/iplugin.h>
#include <projectexplorer/project.h>

namespace LmBase {
namespace Internal {

class LMBASESHARED_EXPORT LinkMotionBasePlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmbaseplugin.json")

public:
    LinkMotionBasePlugin();
    ~LinkMotionBasePlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override;

    static QString lmTargetTool ();
    static QString lmTargetWrapper ();

private slots:
    void onKitsLoaded ();
    void showFirstStartWizard ();
    void updateContextMenu(ProjectExplorer::Project *project,ProjectExplorer::Node *node);

private:
    bool checkContainerSetup();

protected:
    //UbuntuDeviceMode       *m_ubuntuDeviceMode;
    //UbuntuPackagingMode    *m_ubuntuPackagingMode;
    //QAction                *m_migrateProjectAction;
    Settings                m_settings;

    ProjectExplorer::Project *m_currentContextMenuProject;
};


} // Internal
} // LmBase

#endif // LMBASEPLUGIN_INCLUDED_H

