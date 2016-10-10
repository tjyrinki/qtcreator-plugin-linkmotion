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
