var sprite_component;

function createSprite(mx, my) {
    var sprite;
    if (!sprite_component)
        sprite_component = Qt.createComponent("DragSprite.qml");
    var position = mapToItem(drag_area_item, mx, my)
    var defaultWidth = 80
    var defaultHeight = 30
    var item = sprite_component.createObject(
        drag_area_item,  // parent
        {
            width: defaultWidth,
            height: defaultHeight,
            x: position.x - defaultWidth/2,
            y: position.y - defaultHeight/2
        }  // initial arguments
    );
    return item
}
