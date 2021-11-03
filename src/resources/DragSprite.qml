import QtQuick 2.12


Item {
    z: 1000
    property var node_id: null
    property int resource_type: 0
    property int resource_amount: 0

    Drag.active: true
    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2

    Image {
        height: parent.height
        width: parent.width
        source: image_chooser.getImageForResourceType(model.resource_type)
    }
}