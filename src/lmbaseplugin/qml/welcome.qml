/*####################################################################
#
# This file is part of the LinkMotion Welcome plugin.
#
# License: GNU Lesser General Public License v 2.1
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

import QtQuick 2.4
import QtQuick.Controls 1.4

Rectangle {
    id: welcomeArea
    visible: true
    color: "#414c59"

    Column {
        anchors.centerIn: parent
        Image {
            source: "LM_logo_boxed_2.png"
        }
   }
}
