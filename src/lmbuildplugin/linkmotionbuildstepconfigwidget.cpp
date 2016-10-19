/*####################################################################
#
# This file is part of the LinkMotion Build plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotionbuildstepconfigwidget.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionBuildStepConfigWidget::LinkMotionBuildStepConfigWidget(ProjectExplorer::BuildStep* step) : BuildStepConfigWidget()
{
    m_step = step;

}
