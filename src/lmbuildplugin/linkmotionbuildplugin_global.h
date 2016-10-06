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

#ifndef LINKMOTIONBUILDGLOBAL_H
#define LINKMOTIONBUILDGLOBAL_H
#include <QtGlobal>

#if defined(LINKMOTIONBUILD_LIBRARY)
#  define LINKMOTIONBUILDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONBUILDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONBUILDGLOBAL_H
