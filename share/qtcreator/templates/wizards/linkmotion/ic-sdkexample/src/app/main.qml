import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import Components 1.0
import Clock 1.0

import "pages"

ApplicationWindow {
    visible: true
    footer: NotificationBar {
        objectName: "NotificationBar"
        areaMiddle: Row {
            anchors.centerIn: parent

            ClockLabel {
                objectName: "ClockLabel"
                blinkEnabled: true
                color: "#fff"
                font.pixelSize: 16
            }
        }

        areaRight: Row {
            anchors {
                top: parent.top
                bottom: parent.bottom
                margins: 5
            }
            spacing: 3

            Image {
                source: "/icons/icon_comfort_inactive.svg"
                height: parent.height
                fillMode: Image.PreserveAspectFit
            }

            Image {
                source: "/icons/icon_enjoy_inactive.svg"
                height: parent.height
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent

        PageCluster {
            objectName: "PageCluster"
        }
    }
}

