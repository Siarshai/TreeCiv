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
            // TODO: To QENUM
            source: model.resource_type == 0 ? "images/leaf.png" : "images/acorn.png"
        }

        SmallVBar {}

        Column {
            Text {
                font.bold: true
                font.pixelSize: 12
                text: "Resource"
            }
            Text {
                font.pixelSize: 14
                text: model ? model.display : ""
            }
            Text {
                color: "darkgray"
                font.pixelSize: 10
                text: "(drag me)"
            }
        }

        SmallVBar {}

        Text {
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
            color: "darkgray"
            text: "Amount: " + (model ? model.resource_amount : "")
        }
    }

    MouseArea {
        anchors.fill: parent
        drag.target: null
        property var sprite: null

        onPressed: {
            sprite = Sprite.createSprite(mouse.x, mouse.y)
            drag.target = sprite
            sprite.node_id = model.node_id
        }

        property bool dragActive: drag.active
        onDragActiveChanged: {
            if (drag.active) {
                // TODO: Make sprite visible? See what looks better
            }
        }

        onReleased: {
            drag.target = null
            if (sprite) {
                sprite.destroy()
                sprite = null
            }
        }
    }
}