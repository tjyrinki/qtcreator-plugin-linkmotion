/*####################################################################
#
# This file is part of the LinkMotion Wizard plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTIONWIZARDPLUGIN_GLOBAL_H
#define LINKMOTIONWIZARDPLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(LINKMOTIONWIZARD_LIBRARY)
#  define LINKMOTIONWIZARDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LINKMOTIONWIZARDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LINKMOTIONWIZARDPLUGIN_GLOBAL_H
