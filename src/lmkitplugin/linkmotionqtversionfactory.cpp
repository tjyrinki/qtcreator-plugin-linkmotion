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
#include "linkmotionqtversionfactory.h"

#include "linkmotionkitplugin_constants.h"
#include "linkmotionqtversion.h"

#include <QDebug>
#include <QFileInfo>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionQtVersionFactory::LinkMotionQtVersionFactory(QObject *parent) : QtSupport::QtVersionFactory(parent) {
    qDebug() << Q_FUNC_INFO;
}

LinkMotionQtVersionFactory::~LinkMotionQtVersionFactory() {
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionQtVersionFactory::canRestore(const QString &type)
{
    qDebug() << Q_FUNC_INFO;
    return type == QLatin1String(LinkMotion::Constants::LINKMOTION_LINUX_QT);
}

QtSupport::BaseQtVersion *LinkMotionQtVersionFactory::restore(const QString &type, const QVariantMap &data)
{
    qDebug() << Q_FUNC_INFO;
    if (!canRestore(type))
        return 0;
    LinkMotionQtVersion *v = new LinkMotionQtVersion;
    v->fromMap(data);
    return v;
}

int LinkMotionQtVersionFactory::priority() const
{
    qDebug() << Q_FUNC_INFO;
    return 10;
}


QtSupport::BaseQtVersion *LinkMotionQtVersionFactory::create(const Utils::FileName &qmakePath,
                                                                ProFileEvaluator *evaluator,
                                                                bool isAutoDetected,
                                                                const QString &autoDetectionSource)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(evaluator);

    QFileInfo fi = qmakePath.toFileInfo();
    if (!fi.exists() || !fi.isExecutable() || !fi.isFile())
        return 0;

    LinkMotionQtVersion *version = new LinkMotionQtVersion(qmakePath, isAutoDetected, autoDetectionSource);
    return version;
}
