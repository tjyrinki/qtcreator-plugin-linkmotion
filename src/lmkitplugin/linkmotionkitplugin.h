/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONKITPLUGIN_H
#define LINKMOTIONKITPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotionkitplugin_global.h"

#include "linkmotionkitconfiguration.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONKITSHARED_EXPORT LinkMotionKitPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmkitplugin.json")

public:
    LinkMotionKitPlugin();
    ~LinkMotionKitPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override;

protected:
    LinkMotionKitConfiguration* m_linkMotionKitConfiguration;
};
}
}

#endif // LINKMOTIONKITPLUGIN_H
