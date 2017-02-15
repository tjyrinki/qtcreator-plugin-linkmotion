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
#ifndef LINKMOTIONKITPLUGIN_CONSTANTS_H
#define LINKMOTIONKITPLUGIN_CONSTANTS_H

#include <QString>

namespace LinkMotion {
namespace Constants {

const char LINKMOTION_LINUX_QT[] = "LinkMotion.LinuxQt";
const char LINKMOTION_AUTODECTION_SOURCE_ID[] = "LinkMotion.Qt.AutoDetect.Id";
const QString LINKMOTION_QT_DIR = QStringLiteral("/opt/linkmotion/qt");
const char LINKMOTION_AUTOOS_KIT_ID[] = "LinkMotion.AutoOS.QtKit.5.8";
const char LINKMOTION_IVIOS_KIT_ID[] = "LinkMotion.IVIOS.QtKit.5.8";
const char LINKMOTION_GDB_COMMAND[] = "/opt/linkmotion/gdb/bin/gdb";

}
}

#endif // LINKMOTIONKITPLUGIN_CONSTANTS_H

