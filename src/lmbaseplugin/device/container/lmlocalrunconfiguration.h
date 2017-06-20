/*
 * Copyright 2013 - 2016 Canonical Ltd.
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
 * Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef UBUNTURUNCONFIGURATION_H
#define UBUNTURUNCONFIGURATION_H

#include <QObject>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/deployconfiguration.h>
#include <projectexplorer/applicationlauncher.h>
#include <remotelinux/remotelinuxenvironmentaspect.h>
#include <utils/fileutils.h>


namespace ProjectExplorer {
class Target;
}


namespace LmBase {
namespace Internal {

class UbuntuClickManifest;

class LinkMotionLocalEnvironmentAspect : public RemoteLinux::RemoteLinuxEnvironmentAspect
{
    Q_OBJECT
public:
    LinkMotionLocalEnvironmentAspect(ProjectExplorer::RunConfiguration *parent);
    virtual Utils::Environment baseEnvironment() const override;

};

class LinkMotionLocalRunConfiguration : public ProjectExplorer::RunConfiguration
{
    Q_OBJECT
public:
    LinkMotionLocalRunConfiguration(ProjectExplorer::Target *parent, Core::Id id);
    LinkMotionLocalRunConfiguration(ProjectExplorer::Target *parent, LinkMotionLocalRunConfiguration* source);

    QWidget *createConfigurationWidget() override;
    bool isEnabled() const override;
    bool aboutToStart (QString *errorMessage);

    QString appId() const;
    void addToBaseEnvironment(Utils::Environment &env) const;

    QString localExecutableFilePath() const;
    QString remoteExecutableFilePath() const;
    QStringList arguments() const;
    Utils::Environment environment() const;
    QString workingDirectory() const;

    // RunConfiguration interface
    virtual bool isConfigured () const override;
    virtual ConfigurationState ensureConfigured(QString *) override;
    virtual ProjectExplorer::Runnable runnable() const override;

    //static helpers
    static QString getDesktopFile (RunConfiguration *config, QString appId, QString *errorMessage = 0);
    static bool readDesktopFile (const QString &desktopFile, QString *executable, QStringList *arguments, QString *errorMessage);

    QStringList soLibSearchPaths() const;
private:
    bool ensureClickAppConfigured (QString *errorMessage);
    bool ensureScopesAppConfigured (QString *errorMessage);
    bool ensureUbuntuProjectConfigured (QString *errorMessage);

private:
    QString m_executable;
    Utils::FileName m_workingDir;
    QStringList m_args;


};

}
}

#endif // UBUNTURUNCONFIGURATION_H
