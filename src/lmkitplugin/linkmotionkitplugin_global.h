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

#ifndef LINKMOTIONKITPLUGIN_GLOBAL_H
#define LINKMOTIONKITPLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(LINKMOTIONKIT_LIBRARY)
#  define LINKMOTIONKITSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONKITSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONKITPLUGIN_GLOBAL_H
