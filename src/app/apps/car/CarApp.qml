import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

import LinkMotion.Base.Core 1.0
import LinkMotion.Base.Conf 1.0

VirtualClusterApp {
    id: carApp
	title: "Car"
	backgroundColor: "red"
	content: [
        Pane {
            ColumnLayout {
                anchors.fill: parent
                Column {
                    anchors {
                        centerIn: parent
                    }

                    Label {
                        text: vehicleConfiguration.speed.value
                        font.pixelSize: 100
                    }
                    Label {
                        text: qsTr("km/h")
                        font.pixelSize: 40
                    }
                }
            }
        },
        Pane {
            ColumnLayout {
                anchors.fill: parent
                Column {
                    anchors {
                        centerIn: parent
                    }

                    Label {
                        text: vehicleConfiguration.rpm.value
                        font.pixelSize: 100
                    }
                    Label {
                        text: qsTr("RPM")
                        font.pixelSize: 40
                    }
                }
            }
        },
        Pane {
            ColumnLayout {
                anchors.fill: parent
                Column {
                    anchors {
                        centerIn: parent
                    }

                    Label {
                        text: vehicleConfiguration.fuel.value
                        font.pixelSize: 100
                    }
                    Label {
                        text: qsTr("FUEL / l")
                        font.pixelSize: 40
                    }
                }
            }
	    }
	]

    VehicleConfiguration {
        id: vehicleConfiguration
    }
}
