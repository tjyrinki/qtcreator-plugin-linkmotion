/*####################################################################
#
# This file is part of the LinkMotion Run plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONRUNCONFIGURATION_H
#define LINKMOTIONRUNCONFIGURATION_H

#include <QObject>
#include <projectexplorer/runconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionRunConfiguration : public ProjectExplorer::RunConfiguration
{
    Q_OBJECT
public:
    LinkMotionRunConfiguration(ProjectExplorer::Target *parent, Core::Id id);
    LinkMotionRunConfiguration(ProjectExplorer::Target *parent, LinkMotionRunConfiguration *source);

    QWidget *createConfigurationWidget();
    Utils::OutputFormatter *createOutputFormatter() const;

public slots:
    void enableDebuggers();
};

}
}

#endif // LINKMOTIONRUNCONFIGURATION_H
