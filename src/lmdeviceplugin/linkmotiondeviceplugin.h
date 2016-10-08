/*####################################################################
#
# This file is part of the LinkMotion Device plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONDEVICEPLUGIN_H
#define LINKMOTIONDEVICEPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotiondeviceplugin_global.h"
#include "linkmotionconfiguration.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONDEVICESHARED_EXPORT LinkMotionDevicePlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmdeviceplugin.json")

public:
    LinkMotionDevicePlugin();
    ~LinkMotionDevicePlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override {}

};
}
}

#endif
