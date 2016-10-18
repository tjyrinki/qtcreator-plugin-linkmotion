import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Page {
    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Row {
        anchors.centerIn: parent

        spacing: 200

        CircularGauge {
            id: speedMeter
            value: 0
            minimumValue: 0
            maximumValue: 260

            style: CircularGaugeStyle {
                labelStepSize: 20
            }

            Timer {
                interval: 5000
                running: true
                repeat: true
                onTriggered: {
                    if (speedMeter.value === 0) {
                        speedMeter.value = speedMeter.maximumValue
                    } else {
                        speedMeter.value = 0;
                    }
                }
            }

            Behavior on value {
                NumberAnimation {
                    duration: 2500
                }
            }
        }

        Gauge {
            id: rpmMeter
            value: 0
            minimumValue: 0
            maximumValue: 7000
            minorTickmarkCount: 2
            tickmarkStepSize: 1000
            tickmarkAlignment: Qt.AlignRight

            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: {
                    if (rpmMeter.value === 0) {
                        rpmMeter.value = rpmMeter.maximumValue
                    } else {
                        rpmMeter.value = 0;
                    }
                }
            }

            Behavior on value {
                NumberAnimation {
                    duration: 500
                }
            }
        }
    }

}

