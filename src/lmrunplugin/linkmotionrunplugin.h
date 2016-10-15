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

#ifndef LINKMOTIONRUNPLUGIN_H
#define LINKMOTIONRUNPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotionrunplugin_global.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONRUNSHARED_EXPORT RunPlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmrunplugin.json")

public:
    RunPlugin();
    ~RunPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override {}
};
}
}

#endif
