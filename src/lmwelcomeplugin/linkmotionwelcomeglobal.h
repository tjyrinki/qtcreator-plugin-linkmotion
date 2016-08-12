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

#ifndef LINKMOTIONWELCOMEGLOBAL_H
#define LINKMOTIONWELCOMEGLOBAL_H
#include <QtGlobal>

#if defined(LINKMOTIONWELCOME_LIBRARY)
#  define LINKMOTIONWELCOMESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONWELCOMESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONWELCOMEGLOBAL_H
