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

#include "lmqtversion.h"
#include "lmbaseplugin_constants.h"
#include "settings.h"

#include <lmbaseplugin/lmtargettool.h>
#include <lmbaseplugin/lmbaseplugin.h>
//#include <ubuntu/device/container/containerdevice.h>
#include <qtsupport/qtsupportconstants.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

namespace LmBase {
namespace Internal {

const char CONTAINER_NAME[] = "LinkMotion.QtVersion.ContainerName";

LinkMotionQtVersion::LinkMotionQtVersion()
    : BaseQtVersion()
{ }

LinkMotionQtVersion::LinkMotionQtVersion(const QString &containerName, const Utils::FileName &path, bool isAutodetected, const QString &autodetectionSource)
    : BaseQtVersion(path, isAutodetected, autodetectionSource),
      m_containerName(containerName)
{
    setUnexpandedDisplayName(defaultUnexpandedDisplayName(path, false));
}

LinkMotionQtVersion::~LinkMotionQtVersion()
{}

void LinkMotionQtVersion::fromMap(const QVariantMap &map)
{
    BaseQtVersion::fromMap(map);
    m_containerName = map.value(QLatin1String(CONTAINER_NAME),QString()).toString();
}

QVariantMap LinkMotionQtVersion::toMap() const
{
    QVariantMap map = BaseQtVersion::toMap();
    map.insert(QLatin1String(CONTAINER_NAME), m_containerName);
    return map;
}

LinkMotionQtVersion *LinkMotionQtVersion::clone() const
{
    return new LinkMotionQtVersion(*this);
}

QString LinkMotionQtVersion::type() const
{
    return QLatin1String(Constants::LM_QTVERSION_TYPE);
}

QList<ProjectExplorer::Abi> LinkMotionQtVersion::detectQtAbis() const
{
    return qtAbisFromLibrary(qtCorePaths());
}

QString LinkMotionQtVersion::description() const
{
    return QCoreApplication::translate("LinkMotionQtVersion", "Link Motion", "Qt Version is used for Link Motion development");
}

QSet<Core::Id> LinkMotionQtVersion::targetDeviceTypes() const
{
#if 0
    QSet<Core::Id> set{
        Constants::LM_DEVICE_TYPE_ID
    };

    auto hostAbi = ProjectExplorer::Abi::hostAbi();
    for (const ProjectExplorer::Abi &abi : qtAbis()) {
        if (abi.architecture() == hostAbi.architecture() &&
                abi.os() == hostAbi.os()) {
            set << ContainerDevice::createIdForContainer(m_containerName);
        }
    }

    return set;
#endif
    return QSet<Core::Id>();
}

QString LinkMotionQtVersion::remoteQMakeCommand() const
{
    return QString::fromLatin1("/usr/bin/%2").arg(qmakeCommand().fileName());
}

bool LinkMotionQtVersion::hasQmlDump() const
{
    return false;
}

bool LinkMotionQtVersion::hasQmlDumpWithRelocatableFlag() const
{
    return false;
}

bool LinkMotionQtVersion::needsQmlDump() const
{
    return false;
}

void LinkMotionQtVersion::addPathToEnv(Utils::Environment &env) const
{
    QString path = env.value(QStringLiteral("PATH"));
    path.prepend(qmakeCommand().parentDir().toString()+":");
    env.set(QStringLiteral("PATH"), path);
}

void LinkMotionQtVersion::addToEnvironment(const ProjectExplorer::Kit *k, Utils::Environment &env) const
{
    QtSupport::BaseQtVersion::addToEnvironment(k, env);
    addPathToEnv(env);
}

Utils::Environment LinkMotionQtVersion::qmakeRunEnvironment() const
{
    auto env = Utils::Environment::systemEnvironment();
    addPathToEnv(env);
    return env;
}


bool LinkMotionQtVersionFactory::canRestore(const QString &type)
{
    return type == QLatin1String(Constants::LM_QTVERSION_TYPE);
}

QtSupport::BaseQtVersion *LinkMotionQtVersionFactory::restore(const QString &type, const QVariantMap &data)
{
    if(!canRestore(type))
        return 0;

    LinkMotionQtVersion *v = new LinkMotionQtVersion();
    v->fromMap(data);
    return v;
}

int LinkMotionQtVersionFactory::priority() const
{
    return 100;
}

QtSupport::BaseQtVersion *LinkMotionQtVersionFactory::create(const Utils::FileName &qmakePath, ProFileEvaluator *evaluator, bool isAutoDetected, const QString &autoDetectionSource)
{
    Q_UNUSED(evaluator);
    //we only care about our qmakes
    QFileInfo qmakeInfo = qmakePath.toFileInfo();
    if(!qmakeInfo.absolutePath().contains(Settings::settingsPath().toString()))
        return 0;

    if(!qmakeInfo.isSymLink() || qmakeInfo.symLinkTarget() != LinkMotionBasePlugin::lmTargetWrapper())
        return 0;

    QString containerName = qmakePath.toFileInfo().dir().dirName();
    if (!LinkMotionTargetTool::targetExists(containerName))
        return 0;

    return new LinkMotionQtVersion(containerName, qmakePath,isAutoDetected,autoDetectionSource);
}

} // namespace Internal
} // namespace LmBase
