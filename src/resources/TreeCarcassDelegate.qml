import QtQuick 2.12
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
            source: "images/branch.png"
        }

        SmallVBar {}

        Column {
            Text {
                font.bold: true
                font.pixelSize: 12
                text: model ? model.display_auxiliary : ""
            }
            Text {
                font.pixelSize: 14
                text: model ? model.display : ""
            }
        }
    }
}