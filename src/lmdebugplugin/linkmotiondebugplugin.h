/*####################################################################
#
# This file is part of the LinkMotion Debug plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONDEBUGPLUGIN_H
#define LINKMOTIONDEBUGPLUGIN_H

#include <QtCore>
#include <QDebug>
#include <extensionsystem/iplugin.h>
#include "linkmotiondebugplugin_global.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONDEBUGSHARED_EXPORT LinkMotionDebugPlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmdebugplugin.json")

public:
    LinkMotionDebugPlugin();
    ~LinkMotionDebugPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override;
};
}
}

#endif // LINKMOTIONDEBUGPLUGIN_H
