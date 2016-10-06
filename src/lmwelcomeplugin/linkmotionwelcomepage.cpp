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

#include "linkmotionwelcomepage.h"
#include "linkmotionwelcomeplugin_constants.h"

#include <QtQml>
#include <utils/fileutils.h>

using namespace LinkMotion;
using namespace LinkMotion::Internal;
using namespace LinkMotion::Welcome;

QUrl LinkMotionWelcomePage::pageLocation() const
{
    return QUrl(Constants::LINKMOTION_WELCOMESCREEN_QML);
}

QString LinkMotionWelcomePage::title() const
{
    return tr("LinkMotion-SDK");
}

int LinkMotionWelcomePage::priority() const
{
    return 0;
}

void LinkMotionWelcomePage::facilitateQml(QQmlEngine *engine)
{
    engine->setOutputWarningsToStandardError(true);
    QQmlContext *context = engine->rootContext();
    context->setContextProperty(QLatin1String("linkmotionWelcomePage"), this);
}

Core::Id LinkMotionWelcomePage::id() const
{
    return "LinkMotionWelcomePage";
}
