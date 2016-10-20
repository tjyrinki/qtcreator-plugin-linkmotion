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
    virtual QString summaryText() const { return QStringLiteral("Deploy to VM image."); }
    virtual QString displayName() const { return QStringLiteral("Deploy"); }

protected:
    ProjectExplorer::BuildStep* m_step;

};

}
}

#endif // LINKMOTIONDEPLOYSTEPCONFIGWIDGET_H
