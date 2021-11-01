import QtQuick 2.12
import Qt.labs.qmlmodels 1.0
import "dragSpriteCreate.js" as Sprite


Item {
    id: delegate_base

    Text {
        id: delegate_text
        anchors.verticalCenter: parent.verticalCenter
        color: styleData.textColor
        elide: styleData.elideMode
        text: model ? model.display + " - " + model.level : ""
    }
}