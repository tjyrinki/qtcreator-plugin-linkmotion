/*####################################################################
#
# This file is part of the LinkMotion Deploy plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONDEPLOYGLOBAL_H
#define LINKMOTIONDEPLOYGLOBAL_H
#include <QtGlobal>

#if defined(LINKMOTIONDEPLOY_LIBRARY)
#  define LINKMOTIONDEPLOYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONDEPLOYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONDEPLOYGLOBAL_H
