/*####################################################################
#
# This file is part of the LinkMotion Debug plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONDEBUGPLUGIN_GLOBAL_H
#define LINKMOTIONDEBUGPLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(LINKMOTIONDEBUG_LIBRARY)
#  define LINKMOTIONDEBUGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONDEBUGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONDEBUGPLUGIN_GLOBAL_H
