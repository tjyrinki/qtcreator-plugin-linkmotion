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

#include "linkmotionrunplugin.h"

#include <QtGui>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

RunPlugin::RunPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

RunPlugin::~RunPlugin()
{
    qDebug() << Q_FUNC_INFO;

}

bool RunPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    return true;
}
