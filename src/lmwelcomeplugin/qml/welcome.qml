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
import widgets 1.0
import QtQuick.Controls 1.4

Rectangle {
    id: welcomeArea
    visible: true
    color: "#414c59"

    Column {
        spacing: 4
        anchors {
            left: parent.left
            top: parent.top
            margins: 4
        }

        Button {
            text: "New Project.."
            onClicked: {
                welcomeArea.newProject();
            }
        }
        Button {
            text: "Open Project.."
            onClicked: {
                welcomeArea.openProject();
            }
        }
    }

    function loadSession(sessionName) {
        linkMotionWelcome.loadSession(sessionName);
    }

    function loadProject(projectUrl) {
        linkMotionWelcome.loadProject(projectUrl);
    }

    function newProject() {
        linkMotionWelcome.newProject();
    }

    function openProject() {
        linkMotionWelcome.openProject();
    }

    Column {
        anchors.centerIn: parent
        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            source: "LM_logo_boxed_2.png"
        }

        Row {
            ScrollView {
                width: 400
                height: 500
                ListView {
                    model: sessionList
                    delegate: Item {
                        height: 50
                        width: 400

                        Column {
                            id: col
                            width: 400

                            Text {
                                function fullSessionName()
                                {
                                    var newSessionName = sessionName
                                    if (model.lastSession && sessionList.isDefaultVirgin())
                                        newSessionName = qsTr("%1 (last session)").arg(sessionName);
                                    else if (model.activeSession && !sessionList.isDefaultVirgin())
                                        newSessionName = qsTr("%1 (current session)").arg(sessionName);
                                    return newSessionName;
                                }
                                text: fullSessionName()
                                color: "white"
                                font.pixelSize: 18
                                font.family: "Nexa Light"

                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                welcomeArea.loadSession(sessionName);
                            }
                        }
                    }
                }
            }

            Column {
                ScrollView {
                    width: 400
                    height: 500
                    ListView {
                        model: projectList
                        delegate: Item {
                            height: 50
                            width: 400

                            Column {
                                id: col2
                                width: 400
                                Text {
                                    text: displayName
                                    color: "white"
                                    font.pixelSize: 18
                                    font.family: "Nexa Light"
                                }
                                Text {
                                    text: prettyFilePath
                                    color: "white"
                                    font.pixelSize: 10
                                    font.family: "Nexa Light"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    welcomeArea.loadProject(filePath);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
