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

#ifndef LINKMOTIONRUNGLOBAL_H
#define LINKMOTIONRUNGLOBAL_H
#include <QtGlobal>

#if defined(LINKMOTIONRUN_LIBRARY)
#  define LINKMOTIONRUNSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONRUNSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONRUNGLOBAL_H
