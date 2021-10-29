import QtQuick 2.12


Item {
    z: 1000

    Drag.active: true
    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2

    Rectangle {
        anchors.fill: parent
        color: "#800000FF"
    }

    Text {
        id: floating_text
        anchors.centerIn: parent
        text: "resource"
    }
}