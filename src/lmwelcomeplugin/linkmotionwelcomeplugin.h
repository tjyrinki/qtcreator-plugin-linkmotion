/*####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef WELCOMEPLUGIN_H
#define WELCOMEPLUGIN_H

#include <QtCore>
#include <extensionsystem/iplugin.h>
#include "linkmotionwelcomemode.h"
#include <coreplugin/modemanager.h>
#include "linkmotionwelcomeglobal.h"

namespace LinkMotion {
namespace Internal {

class LINKMOTIONWELCOMESHARED_EXPORT WelcomePlugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "lmwelcomeplugin.json")

public:
    WelcomePlugin();
    ~WelcomePlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) override;
    virtual void extensionsInitialized() override;

protected:
    LinkMotionWelcomeMode* m_linkMotionWelcomeMode;
};
}
}

#endif
