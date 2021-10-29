import QtQuick 2.12


Item {
    z: 1000

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