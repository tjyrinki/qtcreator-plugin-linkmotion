import QtQuick 2.7

Rectangle {
    anchors {
        left: parent.left
        right: parent.right
    }
    height: 30
    color: "#353637"

    property alias areaLeft: leftArea.children
    property alias areaMiddle: middleArea.children
    property alias areaRight: rightArea.children

    Row {
        id: leftArea
        anchors {
            left: left.right
            leftMargin: 6
            top: parent.top
            bottom: parent.bottom
        }
    }

    Item {
        id: middleArea
        anchors {
            left: leftArea.right
            right: rightArea.left
            top: parent.top
            bottom: parent.bottom
        }
    }

    Row {
        id: rightArea
        anchors {
            right: parent.right
            rightMargin: 6
            top: parent.top
            bottom: parent.bottom
        }
    }
}
