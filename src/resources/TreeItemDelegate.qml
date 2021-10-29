import QtQuick 2.12
import "dragSpriteCreate.js" as Sprite


Item {
    id: delegate_base

    Text {
        id: delegate_text
        anchors.verticalCenter: parent.verticalCenter
        color: styleData.textColor
        elide: styleData.elideMode
        text: styleData.value
    }

    MouseArea {
        anchors.fill: parent
        drag.target: null
        property var sprite: null

        onPressed: {
            sprite = Sprite.createSprite(mouse.x, mouse.y)
            drag.target = sprite
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