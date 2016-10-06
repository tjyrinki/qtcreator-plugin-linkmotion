#include "linkmotionbuildstepconfigwidget.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionBuildStepConfigWidget::LinkMotionBuildStepConfigWidget(ProjectExplorer::BuildStep* step) : BuildStepConfigWidget()
{
    m_step = step;

}
