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

#ifndef LMBASE_LINKMOTION_TOOLCHAIN_H
#define LMBASE_LINKMOTION_TOOLCHAIN_H

#include <projectexplorer/gcctoolchain.h>
#include "lmtargettool.h"

namespace LmBase {
namespace Internal {

class LinkMotionToolChainFactory;

class LinkMotionToolChain : public ProjectExplorer::GccToolChain
{
    friend class LinkMotionToolChainFactory;

    // ToolChain interface
public:
    LinkMotionToolChain(const LmTargetTool::Target &target,Detection d);

    virtual QList<Utils::FileName> suggestedMkspecList() const override;
    virtual Utils::FileName suggestedDebugger() const override;
    virtual QString typeDisplayName() const override;
    virtual bool isValid() const override;
    virtual void addToEnvironment(Utils::Environment &env) const override;
    virtual QString makeCommand(const Utils::Environment &) const override;
    virtual bool operator ==(const ProjectExplorer::ToolChain &tc) const override;
    virtual ProjectExplorer::ToolChainConfigWidget *configurationWidget() override;
    virtual QVariantMap toMap() const override;

    QString gnutriplet () const;
    static QString gnutriplet (const ProjectExplorer::Abi &abi);
    const LmTargetTool::Target &lmTarget () const;

    static ProjectExplorer::Abi architectureNameToAbi ( const QString &arch );
    static QString abiToArchitectureName ( const ProjectExplorer::Abi &abi );
    static QList<QString> supportedArchitectures ();

    QString remoteCompilerCommand () const;

protected:
    virtual bool fromMap(const QVariantMap &data) override;

    LinkMotionToolChain(const LinkMotionToolChain& other);
    LinkMotionToolChain();

private:
    LmTargetTool::Target m_lmTarget;

    // ToolChain interface
public:
    virtual Utils::FileName compilerCommand() const override;
};

class LinkMotionToolChainFactory : public ProjectExplorer::ToolChainFactory
{
    Q_OBJECT
public:
    LinkMotionToolChainFactory();

    // ToolChainFactory interface
public:
    virtual QList<ProjectExplorer::ToolChain *> autoDetect(const QList<ProjectExplorer::ToolChain *> &alreadyKnown) override;
    virtual bool canRestore(const QVariantMap &data) override;
    virtual ProjectExplorer::ToolChain *restore(const QVariantMap &data) override;
    virtual QSet<Core::Id> supportedLanguages() const override;

    static QList<ProjectExplorer::ToolChain *> createToolChainsForLMTargets(const QList<ProjectExplorer::ToolChain *> &alreadyKnown);
};

} // namespace Internal
} // namespace LmBase

#endif // LMBASE_LINKMOTION_TOOLCHAIN_H
