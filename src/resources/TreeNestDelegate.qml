import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.qmlmodels 1.0
import "dragSpriteCreate.js" as Sprite


Item {
    id: delegate_base

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        spacing: 6

        Image {
            height: 40
            width: height
            source: "images/nest.png"
        }

        SmallVBar {}

        Column {
            Text {
                font.bold: true
                font.pixelSize: 12
                text: "Nest"
            }
            Text {
                font.pixelSize: 14
                text: model ? model.display : ""
            }
            Text {
                color: "darkgray"
                font.pixelSize: 10
                text: "Nest residents: " + (model ? model.display_auxiliary : "")
            }
        }

        SmallVBar {}

        Button {
            anchors.verticalCenter: parent.verticalCenter
            height: 24
            width: height
            text: "+"
            font.pixelSize: 20
            onClicked: {
                map_model.upgrade_nest_node_request(model.node_id)
            }
        }

        SmallVBar {}

        Text {
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
            color: "darkgray"
            text: "Level: " + (model ? model.level : "")
        }
    }
}