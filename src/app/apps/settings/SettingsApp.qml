import QtQuick 2.7
import QtQuick.Controls 2.1

import LinkMotion.Base.Core 1.0

VirtualClusterApp {
    id: settingsApp
    title: "Settings"
    backgroundColor: "green"
    content: [
        Pane {
            StackView {
                id: stack
                initialItem: SettingsAppMain {}
                anchors.fill: parent
            }
        }
    ]

}
