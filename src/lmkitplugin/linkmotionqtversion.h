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
#ifndef LINKMOTIONQTVERSION_H
#define LINKMOTIONQTVERSION_H

#include <qtsupport/baseqtversion.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionQtVersion : public QtSupport::BaseQtVersion
{
public:
    LinkMotionQtVersion();
    ~LinkMotionQtVersion();
    LinkMotionQtVersion(const Utils::FileName &path, bool isAutodetected = false, const QString &autodetectionSource = QString());
    LinkMotionQtVersion *clone() const;

    QString type() const;

    QList<ProjectExplorer::Abi> detectQtAbis() const;

    QString description() const;

    QSet<Core::Id> targetDeviceTypes() const;
};

}
}


#endif // LINKMOTIONQTVERSION_H
