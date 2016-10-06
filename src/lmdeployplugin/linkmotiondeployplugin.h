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

#ifndef LINKMOTIONDEPLOYPLUGIN_H
#define LINKMOTIONDEPLOYPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotiondeployplugin_global.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONDEPLOYSHARED_EXPORT DeployPlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmdeployplugin.json")

public:
    DeployPlugin();
    ~DeployPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override {}
};
}
}

#endif
