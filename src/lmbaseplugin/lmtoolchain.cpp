/*
 * Copyright 2016 Canonical Ltd.
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

#include "lmtoolchain.h"
#include "lmbaseplugin_constants.h"

#include <utils/fileutils.h>
#include <utils/algorithm.h>
#include <projectexplorer/abi.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <QDebug>
#include <QVariant>
#include <QVariantMap>
#include <QFile>
#include <QFileInfo>

namespace LmBase {
namespace Internal {

enum {
    debug = 0
};

QLatin1String LM_TARGET_ARCH_KEY = QLatin1String("LinkMotion.ToolChain.Target.Arch");
QLatin1String LM_TARGET_CONTAINER_KEY = QLatin1String("LinkMotion.ToolChain.Target.ContainerName");

#if 1
static QMap <QString,ProjectExplorer::Abi> init_architectures()
{
    QMap<QString,ProjectExplorer::Abi> map;
    map.insert(QLatin1String("armhf") , ProjectExplorer::Abi(ProjectExplorer::Abi::ArmArchitecture,
                                                             ProjectExplorer::Abi::LinuxOS,
                                                             ProjectExplorer::Abi::GenericLinuxFlavor,
                                                             ProjectExplorer::Abi::ElfFormat,
                                                             32));

    map.insert(QLatin1String("i386") ,ProjectExplorer::Abi(ProjectExplorer::Abi::X86Architecture,
                                                           ProjectExplorer::Abi::LinuxOS,
                                                           ProjectExplorer::Abi::GenericLinuxFlavor,
                                                           ProjectExplorer::Abi::ElfFormat,
                                                           32));

    map.insert(QLatin1String("amd64"),ProjectExplorer::Abi(ProjectExplorer::Abi::X86Architecture,
                                                           ProjectExplorer::Abi::LinuxOS,
                                                           ProjectExplorer::Abi::GenericLinuxFlavor,
                                                           ProjectExplorer::Abi::ElfFormat,
                                                           64));
    map.insert(QLatin1String("arm64") , ProjectExplorer::Abi(ProjectExplorer::Abi::ArmArchitecture,
                                                             ProjectExplorer::Abi::LinuxOS,
                                                             ProjectExplorer::Abi::GenericLinuxFlavor,
                                                             ProjectExplorer::Abi::ElfFormat,
                                                             64));
    return map;
}

QMap <QString,ProjectExplorer::Abi> lmDeviceArchitectures = init_architectures();
#endif


QList<Utils::FileName> LinkMotionToolChain::suggestedMkspecList() const
{
    return ProjectExplorer::GccToolChain::suggestedMkspecList();
}

Utils::FileName LinkMotionToolChain::suggestedDebugger() const
{
    return Utils::FileName::fromString(QLatin1String("/usr/bin/gdb-multiarch"));
}

QString LinkMotionToolChain::typeDisplayName() const
{
    return LinkMotionToolChainFactory::tr("Link Motion GCC");
}

bool LinkMotionToolChain::isValid() const
{
    return GccToolChain::isValid() && targetAbi().isValid() && LmTargetTool::targetExists(m_lmTarget);
}

void LinkMotionToolChain::addToEnvironment(Utils::Environment &env) const
{
    GccToolChain::addToEnvironment(env);
}

QString LinkMotionToolChain::makeCommand(const Utils::Environment &) const
{
    return LmTargetTool::findOrCreateMakeWrapper(lmTarget());
}

bool LinkMotionToolChain::operator ==(const ProjectExplorer::ToolChain &tc) const
{
    if (!GccToolChain::operator ==(tc))
        return false;

    return (m_lmTarget.architecture == m_lmTarget.architecture
            && m_lmTarget.containerName == m_lmTarget.containerName);
}

ProjectExplorer::ToolChainConfigWidget *LinkMotionToolChain::configurationWidget()
{
    return GccToolChain::configurationWidget();
}

const LmTargetTool::Target &LinkMotionToolChain::lmTarget() const
{
    return m_lmTarget;
}

ProjectExplorer::Abi LinkMotionToolChain::architectureNameToAbi(const QString &arch)
{
    if( !lmDeviceArchitectures.contains(arch) )
        return ProjectExplorer::Abi();

    return lmDeviceArchitectures[arch];
}

QString LinkMotionToolChain::abiToArchitectureName(const ProjectExplorer::Abi &abi)
{
    for (auto i = lmDeviceArchitectures.constBegin(); i != lmDeviceArchitectures.constEnd(); i++){
        if (i.value() == abi)
            return i.key();
    }
    return QString("unknown");
}

QList<QString> LinkMotionToolChain::supportedArchitectures()
{
    return lmDeviceArchitectures.keys();
}

QString LinkMotionToolChain::remoteCompilerCommand() const
{
    return QString::fromLatin1("/usr/bin/%1").arg(compilerCommand().fileName());
}

QVariantMap LinkMotionToolChain::toMap() const
{
    QVariantMap map = GccToolChain::toMap();
    map.insert(LM_TARGET_ARCH_KEY,m_lmTarget.architecture);
    map.insert(LM_TARGET_CONTAINER_KEY,m_lmTarget.containerName);
    return map;
}

QString LinkMotionToolChain::gnutriplet() const
{
    return gnutriplet(targetAbi());
}

QString LinkMotionToolChain::gnutriplet(const ProjectExplorer::Abi &abi)
{
    switch(abi.architecture()) {
        case ProjectExplorer::Abi::ArmArchitecture:
            switch(abi.wordWidth())
            {
                case 32:
                    return QLatin1String("arm-linux-gnueabihf");
                case 64:
                    return QLatin1String("aarch64-linux-gnu");
            }
            break;
        case ProjectExplorer::Abi::X86Architecture:
            switch(abi.wordWidth())
            {
                case 32:
                    return QLatin1String("i386-linux-gnu");
                case 64:
                    return QLatin1String("x86_64-linux-gnu");
            }
            break;
        default:
            Q_ASSERT_X(false,Q_FUNC_INFO,"Unknown Target architecture");
    }
    return QString();
}

bool LinkMotionToolChain::fromMap(const QVariantMap &data)
{
    if(!GccToolChain::fromMap(data))
        return false;

    if(!data.contains(LM_TARGET_ARCH_KEY)
            || !data.contains(LM_TARGET_CONTAINER_KEY))
        return false;

    m_lmTarget.architecture  = data[LM_TARGET_ARCH_KEY].toString();
    m_lmTarget.containerName = data[LM_TARGET_CONTAINER_KEY].toString();
    return isValid();
}

Utils::FileName LinkMotionToolChain::compilerCommand() const
{
    return GccToolChain::compilerCommand();
}

LinkMotionToolChain::LinkMotionToolChain(const LmTargetTool::Target &target, Detection d)
    : GccToolChain(Constants::LM_TARGET_TOOLCHAIN_ID, d)
    , m_lmTarget(target)
{
    resetToolChain(Utils::FileName::fromString(
                           LmTargetTool::findOrCreateGccWrapper(target)
                           ));

    setDisplayName(QString::fromLatin1("Link Motion GCC (%1-%2)")
                   .arg(ProjectExplorer::Abi::toString(targetAbi().architecture()))
                   .arg(target.containerName));
}

LinkMotionToolChain::LinkMotionToolChain(const LinkMotionToolChain &other)
    : GccToolChain(other)
    , m_lmTarget(other.m_lmTarget)
{
}

LinkMotionToolChain::LinkMotionToolChain()
    : GccToolChain(Constants::LM_TARGET_TOOLCHAIN_ID,ManualDetection)
{
}

LinkMotionToolChainFactory::LinkMotionToolChainFactory()
{
    setDisplayName(tr("Link Motion GCC"));
}

QList<ProjectExplorer::ToolChain *> LinkMotionToolChainFactory::autoDetect(
        const QList<ProjectExplorer::ToolChain *> &alreadyKnown)
{
    return createToolChainsForLMTargets(alreadyKnown);
}

bool LinkMotionToolChainFactory::canRestore(const QVariantMap &data)
{
    return typeIdFromMap(data) == Constants::LM_TARGET_TOOLCHAIN_ID;
}

ProjectExplorer::ToolChain *LinkMotionToolChainFactory::restore(const QVariantMap &data)
{
    LinkMotionToolChain *tc = new LinkMotionToolChain();
    if (tc->fromMap(data)) {
        return tc;
    }
    delete tc;
    return 0;
}

QSet<Core::Id> LinkMotionToolChainFactory::supportedLanguages() const
{
    return {ProjectExplorer::Constants::CXX_LANGUAGE_ID};
}

QList<ProjectExplorer::ToolChain *> LinkMotionToolChainFactory::createToolChainsForLMTargets(const QList<ProjectExplorer::ToolChain *> &alreadyKnown)
{
    QList<ProjectExplorer::ToolChain*> toolChains;

    QList<LmTargetTool::Target> targets = LmTargetTool::listAvailableTargets();
    foreach(const LmTargetTool::Target &target, targets) {
        if(debug) qDebug()<<"Found Target"<<target;

        if(!lmDeviceArchitectures.contains(target.architecture))
            continue;

        QString comp = LmTargetTool::findOrCreateGccWrapper(target);
        if(comp.isEmpty())
            continue;

        auto predicate = [&](ProjectExplorer::ToolChain *tc) {
            if (tc->typeId() != Constants::LM_TARGET_TOOLCHAIN_ID)
                return false;
            auto lmTc = static_cast<LinkMotionToolChain *>(tc);
            return lmTc->lmTarget().containerName == target.containerName;
        };

        ProjectExplorer::ToolChain *tc = Utils::findOrDefault(alreadyKnown, predicate);
        if (!tc)
            tc = new LinkMotionToolChain(target, ProjectExplorer::ToolChain::AutoDetection);
        toolChains.append(tc);
    }

    return toolChains;
}

} // namespace Internal
} // namespace LmBase
