/*####################################################################
#
# This file is part of the LinkMotion Deploy plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotiondeploystepconfigwidget.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDeployStepConfigWidget::LinkMotionDeployStepConfigWidget(ProjectExplorer::BuildStep* step) : BuildStepConfigWidget()
{
    m_step = step;

}
