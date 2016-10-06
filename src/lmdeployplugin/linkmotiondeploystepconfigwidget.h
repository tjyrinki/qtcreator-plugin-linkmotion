#ifndef LINKMOTIONDEPLOYSTEPCONFIGWIDGET_H
#define LINKMOTIONDEPLOYSTEPCONFIGWIDGET_H

#include <projectexplorer/buildstep.h>

#include <QObject>

namespace LinkMotion {
namespace Internal {


class LinkMotionDeployStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    LinkMotionDeployStepConfigWidget(ProjectExplorer::BuildStep* step);
    virtual QString summaryText() const { return QStringLiteral("deploysummaryTextHere"); }
    virtual QString displayName() const { return QStringLiteral("deploydisplayNamehere"); }

protected:
    ProjectExplorer::BuildStep* m_step;

};

}
}

#endif // LINKMOTIONDEPLOYSTEPCONFIGWIDGET_H
