import QtQuick 2.8

import LinkMotion.IC.Base.Layouts 1.0
import LinkMotion.IC.Base.Core 1.0
import LinkMotion.IC.Base.Conf 1.0
import LinkMotion.IC.Base.Gauges 1.0

import Clock 1.0

import CarApp 1.0
import MediaApp 1.0

VirtualClusterWindow {
    background: Rectangle {
        anchors.fill: parent
        color: "#1a171b"
    }

    VehicleConfiguration {
        id: vehicleConfiguration
    }

    BasicICLayout {
        id: mainLayout
        anchors.fill: parent
        anchors {
            topMargin: 10
            bottomMargin: 10
            leftMargin: 30
            rightMargin: 30
        }

        leftArea: [
            SpeedoMeter {
                minimumValue: vehicleConfiguration.speed.min
                maximumValue: vehicleConfiguration.speed.max
                value: vehicleConfiguration.speed.value
            }
        ]

        rightArea: [
            RPMMeter {
                minimumValue: vehicleConfiguration.rpm.min
                maximumValue: vehicleConfiguration.rpm.max
                value: vehicleConfiguration.rpm.value
            },
            ClockLabel {
                font.pointSize: 14
                color: "#eee"
                anchors.horizontalCenter: parent.horizontalCenter
            },
            FuelGauge {
                minimumValue: vehicleConfiguration.fuel.min
                maximumValue: vehicleConfiguration.fuel.max
                value: vehicleConfiguration.fuel.value
            }
        ]

        middleArea: [
            Indicators {
                sourcePrefix: "qrc:/indicators/"
                model: vehicleConfiguration.indicators.primary
            },

            VirtualClusterAppContainer {
                width: 300
                height: 300
                apps: [
                    CarApp {},
                    MediaApp {}
                ]
            },

            Indicators {
                sourcePrefix: "qrc:/indicators/"
                model: vehicleConfiguration.indicators.secondary
            }
        ]
    }

}

