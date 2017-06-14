/*
 * Copyright 2014 Canonical Ltd.
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
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef LINKMOTION_INTERNAL_TARGETTOOL_H
#define LINKMOTION_INTERNAL_TARGETTOOL_H

#include <QList>
#include <QString>
#include <QDialog>
#include <QFutureInterface>
#include <QQueue>
#include <projectexplorer/processparameters.h>
#include <cmakeprojectmanager/cmaketool.h>
#include <utils/qtcprocess.h>
#include <QDebug>

#include "lmbaseplugin_global.h"


class QDialogButtonBox;
class QPlainTextEdit;
class QLabel;
class QAction;
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;

namespace ProjectExplorer {
    class Project;
    class Target;
    class Kit;
}

namespace LmBase {
namespace Internal {
class UbuntuClickBuildConfiguration;
} // namespace Internal


class LMBASESHARED_EXPORT LinkMotionTargetTool
{
public:

    enum MaintainMode {
        Upgrade,//runs click chroot upgrade
        Delete  //runs click chroot delete
    };

    struct Target {
        QString imageArchitecture; //only required for creating new targets
        QString architecture;
        QString containerName;
        QString distribution;
        QString version;
        bool upgradesEnabled = false;
    };

    LinkMotionTargetTool();

    static void parametersForCreateTarget   (const Target &target, ProjectExplorer::ProcessParameters* params);
    static void parametersForMaintainChroot (const MaintainMode &mode,const Target& target,ProjectExplorer::ProcessParameters* params);

    static void openTargetTerminal (const Target& target);

    static QString targetBasePath (const Target& target);
    static bool parseContainerName (const QString &name, Target *target, QStringList *allExt = 0);
    //static bool getTargetFromUser (Target* target, const QString &framework=QString());
    static QStringList getSupportedFrameworks (const Target *target);
    static QString getMostRecentFramework ( const QString &subFramework, const Target *target );
    static QString findOrCreateGccWrapper(const LinkMotionTargetTool::Target &target, const Core::Id &language);
    static QString findOrCreateToolWrapper(const QString &tool, const LinkMotionTargetTool::Target &target);
    static QString findOrCreateQMakeWrapper(const LinkMotionTargetTool::Target &target);
    static QString findOrCreateMakeWrapper(const LinkMotionTargetTool::Target &target);
    static CMakeProjectManager::CMakeTool::PathMapper mapIncludePathsForCMakeFactory(const ProjectExplorer::Target *t);
    static QString hostArchitecture ();
    static bool    compatibleWithHostArchitecture (const QString &targetArch);

    static bool          targetExists (const Target& target);
    static bool          targetExists (const QString& targetName);
    static QList<Target> listAvailableTargets (const QString &framework=QString());
    static QList<Target> listPossibleDeviceContainers ();
    static const Target *lmTargetFromTarget(ProjectExplorer::Target *t);
    static bool          setTargetUpgradesEnabled (const Target& target, const bool set = true);

    static ProjectExplorer::ProcessParameters prepareToRunInTarget (ProjectExplorer::Kit *target, const QString &cmd,
                                                                    const QStringList &args, const QString &wd,
                                                                    const QMap<QString, QString> &envMap = QMap<QString, QString>() );
};

QDebug operator<<(QDebug dbg, const LinkMotionTargetTool::Target& t);
} // namespace LmBase

#endif // LINKMOTION_INTERNAL_TARGETTOOL_H
