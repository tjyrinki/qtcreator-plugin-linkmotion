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
#ifndef LINKMOTIONBUILDSTEPCONFIGWIDGET_H
#define LINKMOTIONBUILDSTEPCONFIGWIDGET_H

#include <projectexplorer/buildstep.h>

#include <QObject>

namespace LinkMotion {
namespace Internal {


class LinkMotionBuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    LinkMotionBuildStepConfigWidget(ProjectExplorer::BuildStep* step);
    virtual QString summaryText() const { return QStringLiteral("Build inside VM image."); }
    virtual QString displayName() const { return QStringLiteral("Build"); }

protected:
    ProjectExplorer::BuildStep* m_step;

};

}
}

#endif // LINKMOTIONBUILDSTEPCONFIGWIDGET_H
