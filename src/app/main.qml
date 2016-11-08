import QtQuick 2.8

import LinkMotion.Base.Layouts 1.0
import LinkMotion.Base.Core 1.0
import LinkMotion.Base.Conf 1.0
import LinkMotion.Base.Gauges 1.0

import "apps/car"
import "apps/media"
import "apps/settings"

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
                    MediaApp {},
                    SettingsApp {}
                ]
            },

            Indicators {
                sourcePrefix: "qrc:/indicators/"
                model: vehicleConfiguration.indicators.secondary
            }
        ]
    }

}

