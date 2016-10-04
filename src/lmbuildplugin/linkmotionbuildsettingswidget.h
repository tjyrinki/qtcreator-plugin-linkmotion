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
#ifndef LINKMOTIONBUILDSETTINGSWIDGET_H
#define LINKMOTIONBUILDSETTINGSWIDGET_H

#include <QObject>
#include <QDebug>

#include <utils/pathchooser.h>
#include <projectexplorer/namedwidget.h>
#include <projectexplorer/buildconfiguration.h>

#include "linkmotionbuildconfiguration.h"

namespace LinkMotion {
namespace Internal {

class LinkMotionBuildSettingsWidget : public ProjectExplorer::NamedWidget
{
public:
    explicit LinkMotionBuildSettingsWidget(LinkMotionBuildConfiguration *conf, QWidget *parent = 0);

private slots:
    void updateBuildDirectory() const;

private:
    Utils::PathChooser *m_pathChooser;
    LinkMotionBuildConfiguration *m_buildConfiguration;

};
}
}

#endif // LINKMOTIONBUILDSETTINGSWIDGET_H
