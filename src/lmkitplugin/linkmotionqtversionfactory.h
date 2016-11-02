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
#ifndef LINKMOTIONQTVERSIONFACTORY_H
#define LINKMOTIONQTVERSIONFACTORY_H

#include <qtsupport/qtversionfactory.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionQtVersionFactory : public QtSupport::QtVersionFactory
{
public:
    explicit LinkMotionQtVersionFactory(QObject *parent = 0);
    ~LinkMotionQtVersionFactory();

    bool canRestore(const QString &type);
    QtSupport::BaseQtVersion *restore(const QString &type, const QVariantMap &data);

    int priority() const;
    QtSupport::BaseQtVersion *create(const Utils::FileName &qmakePath, ProFileEvaluator *evaluator,
                                     bool isAutoDetected = false,
                                     const QString &autoDetectionSource = QString());
};

}
}

#endif // LINKMOTIONQTVERSIONFACTORY_H
