/*
 * Copyright 2016 Canonical Ltd.
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
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#include "lmtargetdialog.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>
#include <lmbaseplugin/lmtoolchain.h>
#include <lmbaseplugin/lmkitmanager.h>
#include <lmbaseplugin/wizards/createtargetwizard.h>

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/toolchainmanager.h>
#include <projectexplorer/kitmanager.h>
#include <coreplugin/helpmanager.h>
#include <coreplugin/icore.h>
#include <qtsupport/qtversionmanager.h>

#include <QMessageBox>

namespace LmBase {

namespace Constants {

}

namespace Internal {

bool LinkMotionTargetDialog::doCreateTarget (bool redetectKits, const LinkMotionTargetTool::Target &t, QWidget *parent)
{
    ProjectExplorer::ProcessParameters params;
    LinkMotionTargetTool::parametersForCreateTarget(t, &params);

    bool success = (runProcessModal(params, parent) == 0);

    if(success) {
        LinkMotionToolChain* tc = new LinkMotionToolChain(t, ProjectExplorer::Constants::C_LANGUAGE_ID, ProjectExplorer::ToolChain::AutoDetection);
        ProjectExplorer::ToolChainManager::registerToolChain(tc);

        tc = new LinkMotionToolChain(t, ProjectExplorer::Constants::CXX_LANGUAGE_ID, ProjectExplorer::ToolChain::AutoDetection);
                ProjectExplorer::ToolChainManager::registerToolChain(tc);

        if(redetectKits)
            LinkMotionKitManager::autoDetectKits();
    }

    return success;
}

bool LinkMotionTargetDialog::createTargetModal(bool redetectKits, QWidget *parent)
{
    LinkMotionTargetTool::Target t;
    if(!CreateTargetWizard::getNewTarget(&t, parent))
        return false;
    return doCreateTarget(redetectKits, t, parent);
}

bool LinkMotionTargetDialog::createTargetModal(bool redetectKits, const QString &arch, const QString &framework, QWidget *parent)
{
    LinkMotionTargetTool::Target t;
    if(!CreateTargetWizard::getNewTarget(&t,arch,framework,parent))
        return false;
    return doCreateTarget(redetectKits, t, parent);
}

int LinkMotionTargetDialog::maintainTargetModal(const LinkMotionTargetTool::Target &target, const LinkMotionTargetTool::MaintainMode &mode)
{
    return maintainTargetModal(QList<LinkMotionTargetTool::Target>()<<target,mode);
}

int LinkMotionTargetDialog::maintainTargetModal(const QList<LinkMotionTargetTool::Target> &targetList, const LinkMotionTargetTool::MaintainMode &mode)
{
    QList<ProjectExplorer::ProcessParameters> paramList;
    foreach(const LinkMotionTargetTool::Target &target, targetList) {
        if(mode == LinkMotionTargetTool::Delete) {

            QStringList docToRemove;
            QString rootfs = LinkMotionTargetTool::targetBasePath(target);
            if (!rootfs.isEmpty()) {
                foreach(const QString &ns, Core::HelpManager::registeredNamespaces()) {
                    QString fileName = Core::HelpManager::fileFromNamespace(ns);
                    if(fileName.startsWith(rootfs)) {
                        docToRemove.append(ns);
                    }
                }
            }

            QString title = tr("Delete Target");
            QString text  = tr("Are you sure you want to delete this target?");
            if( QMessageBox::question(Core::ICore::mainWindow(),title,text) != QMessageBox::Yes )
                return 0;

            //remove all kits using the target
            QList<ProjectExplorer::Kit *> kitsToDelete = LinkMotionKitManager::findKitsUsingTarget(target);
            foreach(ProjectExplorer::Kit *curr, kitsToDelete) {
                ProjectExplorer::KitManager::deregisterKit(curr);
            }

            //make sure no help files are still opened
            Core::HelpManager::unregisterDocumentation(docToRemove);
        }

        ProjectExplorer::ProcessParameters params;
        LinkMotionTargetTool::parametersForMaintainChroot(mode,target,&params);
        paramList<<params;
    }

    int code = runProcessModal(paramList);
    if(mode == LinkMotionTargetTool::Delete) {
        //redetect documentation
        QtSupport::QtVersionManager::triggerDocumentationUpdate();
    }
    return code;
}

}}
