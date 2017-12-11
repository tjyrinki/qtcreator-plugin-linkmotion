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

#ifndef LMBASE_QTVERSION_H
#define LMBASE_QTVERSION_H

#include <qtsupport/baseqtversion.h>
#include <qtsupport/qtversionfactory.h>

namespace LmBase {
namespace Internal {

class LinkMotionQtVersion : public QtSupport::BaseQtVersion
{
public:
    LinkMotionQtVersion();
    LinkMotionQtVersion(const QString &containerName, const Utils::FileName &path, bool isAutodetected = false, const QString &autodetectionSource = QString());
    ~LinkMotionQtVersion() override;

    // BaseQtVersion interface
    virtual void fromMap(const QVariantMap &map) override;
    virtual QVariantMap toMap() const override;
    virtual void addToEnvironment(const ProjectExplorer::Kit *k, Utils::Environment &env) const override;
    virtual Utils::Environment qmakeRunEnvironment() const override;

    LinkMotionQtVersion *clone() const override;

    QString type() const override;

    QList<ProjectExplorer::Abi> detectQtAbis() const override;

    QString description() const override;

    virtual QSet<Core::Id> targetDeviceTypes() const override;

    QString remoteQMakeCommand () const;

    // BaseQtVersion interface
    virtual bool hasQmlDump() const override;
    virtual bool hasQmlDumpWithRelocatableFlag() const override;
    virtual bool needsQmlDump() const override;

private:
    void addPathToEnv (Utils::Environment &env) const;

private:
    QString m_containerName;
};

class LinkMotionQtVersionFactory : public QtSupport::QtVersionFactory
{
public:
    // QtVersionFactory interface
    virtual bool canRestore(const QString &type) override;
    virtual QtSupport::BaseQtVersion *restore(const QString &type, const QVariantMap &data) override;
    virtual int priority() const override;
    virtual QtSupport::BaseQtVersion *create(const Utils::FileName &qmakePath, ProFileEvaluator *evaluator, bool isAutoDetected, const QString &autoDetectionSource) override;
};

} // namespace Internal
} // namespace LmBase

#endif // LMBASE_QTVERSION_H
