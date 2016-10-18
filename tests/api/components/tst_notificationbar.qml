import QtQuick 2.7
import QtTest 1.0
import Components 1.0

TestCase {
    name: "API_Smoke_NotificationBar"

    NotificationBar { }

    function test_can_create_instance() {
        compare(true, true, "Was able to create an instance.")
    }
}

