/*####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionproject.h"
#include "linkmotionprojectplugin_constants.h"
#include "linkmotionkitmatcher.h"

#include <coreplugin/icontext.h>
#include <qtsupport/qtsupportconstants.h>
#include <coreplugin/documentmanager.h>
#include <coreplugin/featureprovider.h>
#include <qtsupport/qtkitinformation.h>

#include <QSettings>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionProject::LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName) : QmakeProjectManager::QmakeProject(manager, fileName){
    qDebug() << Q_FUNC_INFO;
    setId(Constants::LINKMOTIONPROJECT_ID);
    setRequiredKitMatcher(LinkMotionKitMatcher());

    QSet<Core::Id> features;
    features << QtSupport::Constants::FEATURE_DESKTOP;
    setPreferredKitMatcher(QtSupport::QtKitInformation::qtVersionMatcher(features));
    setProjectContext(Core::Context(Constants::LINKMOTIONPROJECT_PROJECTCONTEXT));
}

LinkMotionProject::~LinkMotionProject() {
    qDebug() << Q_FUNC_INFO;
    /* Developer Note! This has been added here to investigate the double free corruption
     * which can happen when a project is closed when there are multiple projects open
     * more likely an internal qt creator issue which is in this alpha source tree.
     *
     * More likely it is something to do with the delete function, better to leak memory than to crash..
     *   this more likely will be fixed in the Qt Creator itself. As it seems to be a racecondition.
     *
     * Session.cpp:722
     *    delete pro;
     *
     * #5  0x00007fffd55898bc in ProjectExplorer::ProjectConfiguration::~ProjectConfiguration (this=0x2f784d0, __in_chrg=<optimized out>) at ../../../qtcreator/src/plugins/projectexplorer/projectconfiguration.h:47
#6  0x00007fffd55899d8 in ProjectExplorer::BuildConfiguration::~BuildConfiguration (this=0x2f784d0, __in_chrg=<optimized out>) at ../../../qtcreator/src/plugins/projectexplorer/buildconfiguration.h:44
#7  0x00007fffd55947b2 in LinkMotion::Internal::LinkMotionBuildConfiguration::~LinkMotionBuildConfiguration (this=0x2f784d0, __in_chrg=<optimized out>) at .moc/debug-shared/../../linkmotionbuildconfiguration.h:21
#8  0x00007fffd55947e2 in LinkMotion::Internal::LinkMotionBuildConfiguration::~LinkMotionBuildConfiguration (this=0x2f784d0, __in_chrg=<optimized out>) at .moc/debug-shared/../../linkmotionbuildconfiguration.h:21
#9  0x00007fffe0d4ec3e in ProjectExplorer::Target::~Target() () from /opt/linkmotion/qtcreator/lib/qtcreator/plugins/libProjectExplorer.so
#10 0x00007fffe0d4edc9 in ProjectExplorer::Target::~Target() () from /opt/linkmotion/qtcreator/lib/qtcreator/plugins/libProjectExplorer.so
#11 0x00007fffe0d8967e in ProjectExplorer::Project::~Project() () from /opt/linkmotion/qtcreator/lib/qtcreator/plugins/libProjectExplorer.so
#12 0x00007fffd4d24112 in LinkMotion::Internal::LinkMotionProject::~LinkMotionProject (this=0x2f75390, __in_chrg=<optimized out>) at linkmotionproject.h:28
#13 0x00007fffd4d24142 in LinkMotion::Internal::LinkMotionProject::~LinkMotionProject (this=0x2f75390, __in_chrg=<optimized out>) at linkmotionproject.h:28
#14 0x00007fffe0d7a973 in ProjectExplorer::SessionManager::removeProjects(QList<ProjectExplorer::Project*>) () from /opt/linkmotion/qtcreator/lib/qtcreator/plugins/libProjectExplorer.so
#15 0x00007fffe0d7abe3 in ProjectExplorer::SessionManager::removeProject(ProjectExplorer::Project*) () from /opt/linkmotion/qtcreator/lib/qtcreator/plugins/libProjectExplorer.so
#16 0x00007fffe0cdda49 in ProjectExplorer::ProjectExplorerPlugin::unloadProject(ProjectExplorer::Project*) () from /opt/linkmotion/qtcreator/lib/qtcreator/plugins/libProjectExplorer.so

     */
}
