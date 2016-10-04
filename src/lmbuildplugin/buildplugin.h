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

#ifndef LINKMOTIONBUILDPLUGIN_H
#define LINKMOTIONBUILDPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "buildplugin_global.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONBUILDSHARED_EXPORT BuildPlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LMBuild.json")

public:
    BuildPlugin();
    ~BuildPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override {}
};
}
}

#endif
