/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionqtversion.h"
#include "linkmotionkitplugin_constants.h"
#include "../lmdeviceplugin/linkmotiondeviceplugin_constants.h"

#include <QDebug>
#include <coreplugin/id.h>
#include <qtsupport/qtsupportconstants.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionQtVersion::LinkMotionQtVersion() : BaseQtVersion()
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionQtVersion::~LinkMotionQtVersion()
{
    qDebug() << Q_FUNC_INFO;
}

LinkMotionQtVersion::LinkMotionQtVersion(const Utils::FileName &path, bool isAutodetected, const QString &autodetectionSource)
    : BaseQtVersion(path, isAutodetected, autodetectionSource)
{
    qDebug() << Q_FUNC_INFO;
    setUnexpandedDisplayName(QStringLiteral("LinkMotion Qt"));
}

LinkMotionQtVersion *LinkMotionQtVersion::clone() const
{
    qDebug() << Q_FUNC_INFO;
    return new LinkMotionQtVersion(*this);
}

QString LinkMotionQtVersion::type() const
{
    qDebug() << Q_FUNC_INFO;
    return QLatin1String(LinkMotion::Constants::LINKMOTION_LINUX_QT);
}

QList<ProjectExplorer::Abi> LinkMotionQtVersion::detectQtAbis() const
{
    qDebug() << Q_FUNC_INFO;
    return qtAbisFromLibrary(qtCorePaths(versionInfo(), qtVersionString()));
}

QString LinkMotionQtVersion::description() const
{
    qDebug() << Q_FUNC_INFO;
    return QCoreApplication::translate("QtVersion", "LinkMotion Linux", "Qt Version is used for LinkMotion development");
}

QSet<Core::Id> LinkMotionQtVersion::targetDeviceTypes() const
{
    qDebug() << Q_FUNC_INFO;
    return { Constants::LINKMOTION_DEVICE_TYPE };
}

