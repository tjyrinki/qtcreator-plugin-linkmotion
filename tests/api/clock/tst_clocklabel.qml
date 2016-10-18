import QtQuick 2.7
import QtTest 1.0
import Clock 1.0

TestCase {
    id: testMain
    name: "API_Smoke_ClockLabel"

    ClockLabel {
        objectName: "ClockLabel"
    }

    function test_can_create_instance() {
        compare(true, true, "Was able to create an instance.")
    }

    function test_has_property_blickEnabled() {
        var clockLabel = findChild(testMain, "ClockLabel")
        compare(clockLabel.blinkEnabled, false, "blinkEnabled is false by default.");
        clockLabel.blinkEnabled = true;
        compare(clockLabel.blinkEnabled, true, "blinkEnabled can be set to true.");
    }
}

