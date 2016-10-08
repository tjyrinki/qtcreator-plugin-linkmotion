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

#ifndef LINKMOTIONDEVICEPLUGIN_GLOBAL_H
#define LINKMOTIONDEVICEPLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(LINKMOTIONDEVICE_LIBRARY)
#  define LINKMOTIONDEVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONDEVICESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONDEVICEPLUGIN_GLOBAL_H
