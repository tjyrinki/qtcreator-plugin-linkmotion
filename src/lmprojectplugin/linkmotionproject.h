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
#ifndef LINKMOTIONPROJECT_H
#define LINKMOTIONPROJECT_H

#include <qmakeprojectmanager/qmakeproject.h>
#include <projectexplorer/project.h>

#include "linkmotionprojectmanager.h"
#include "linkmotionprojectfile.h"
#include "linkmotionprojectnode.h"


namespace LinkMotion {
namespace Internal {

class LinkMotionProjectFile;
class LinkMotionProjectNode;

class LinkMotionProject : public QmakeProjectManager::QmakeProject
{
public:
    LinkMotionProject(LinkMotionProjectManager *manager, const QString &fileName);
    ~LinkMotionProject();

};

}
}

#endif // LINKMOTIONPROJECT_H
