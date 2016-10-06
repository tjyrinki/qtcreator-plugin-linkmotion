#include "linkmotiondeploystepconfigwidget.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDeployStepConfigWidget::LinkMotionDeployStepConfigWidget(ProjectExplorer::BuildStep* step) : BuildStepConfigWidget()
{
    m_step = step;

}
