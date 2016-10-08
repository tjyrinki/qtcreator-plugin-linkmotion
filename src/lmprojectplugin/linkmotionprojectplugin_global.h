/*####################################################################
#
# This file is part of the LinkMotion Project plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONPROJECTPLUGIN_GLOBAL_H
#define LINKMOTIONPROJECTPLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(LINKMOTIONPROJECT_LIBRARY)
#  define LINKMOTIONPROJECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONPROJECTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONPROJECTPLUGIN_GLOBAL_H
