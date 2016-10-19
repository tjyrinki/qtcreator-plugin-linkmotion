/*####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONKITMATCHER_H
#define LINKMOTIONKITMATCHER_H

#include <QObject>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionKitMatcher : public ProjectExplorer::KitMatcher
{
public:
    LinkMotionKitMatcher() : KitMatcher(&LinkMotionKitMatcher::matches) {}
    static bool matches(const ProjectExplorer::Kit *k);
};

}
}

#endif // LINKMOTIONKITMATCHER_H
