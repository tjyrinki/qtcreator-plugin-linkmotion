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

import QtQuick 2.4

Rectangle {
    anchors.fill: parent
    color: "#414c59"

    property int maximumWidth : 900

    Image {
        anchors.centerIn: parent
        source: "LM_logo_boxed_2.png"
    }
}
