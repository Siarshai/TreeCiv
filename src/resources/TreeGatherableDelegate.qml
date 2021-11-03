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
            source: image_chooser.getImageForResourceType(model.resource_type)
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
            // Need to create separate draggable widget here (instead of dragging delegate itself)
            // because we need to drag it outside of TreeView. If we do it naively, delegate will
            // be drawn at z level DEEPER than TreeView and will be invisible outside TreeView.
            // Besides, dragged resource image just looks nicer.
            sprite = Sprite.createSprite(mouse.x, mouse.y)
            drag.target = sprite
            sprite.node_id = model.node_id
            sprite.resource_type = model.resource_type
            sprite.resource_amount = model.resource_amount
        }

        onReleased: {
            drag.target = null
            if (sprite) {
                // Need to call sprite.Drag.drop()
                // Otherwise DropArea onDropped() won't fire
                sprite.Drag.drop()
                sprite.destroy()
                sprite = null
            }
        }
    }
}