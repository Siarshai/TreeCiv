import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.5 as OldControls
import QtQml.Models 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1
import Qt.labs.qmlmodels 1.0


ApplicationWindow {
    id: base_window
    title: "Tree Civ"
    width: 600
    height: 800
    visible: true

    MessageDialog {
        id: aboutDialog
        icon: StandardIcon.Information
        title: "About Game"
        text: "What to do:"
        informativeText: "- Expand tree on the left\n- Drag resources and drop them into drop area on the right\n- Locate nest on the tree\n- Press '+' button to upgrade it (it depletes resources)\n- Repeat"
    }

    menuBar: MenuBar {
        Menu {
            title: "&Game"
            MenuItem {
                text: "About &Game"
                onTriggered: aboutDialog.open()
            }
            MenuItem {
                text: "E&xit"
                onTriggered: Qt.quit()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 6

        // TODO: Set stretch policy
        OldControls.TreeView {
            id: main_tree_view
            Layout.fillHeight: true
            width: base_window.width*2/3
            implicitWidth: base_window.width*2/3
            model: map_model

            selectionMode: OldControls.SelectionMode.NoSelection
            selection: null
            // Setting height only to itemDelegate does nothing.
            // TreeView gets row heights from row delegates (seems not specified in documentation)
            rowDelegate: Component {
                Item {
                    height: 52
                }
            }

            OldControls.TableViewColumn {
                role: "delegate_type"
                title: "Tree"
                width: base_window.width*2/3 - 10
                resizable: false
                movable: false
                delegate: Component {
                    Loader {
                        source: {
                            // When tree is collapsing model in dying delegates is null.
                            // We need not load anything. Returning empty string seems fine (?)
                            if (!model)
                                return ""
                            switch(model.delegate_type) {
                                case "carcass": return "TreeCarcassDelegate.qml"
                                case "nest": return "TreeNestDelegate.qml"
                                case "gatherable": return "TreeGatherableDelegate.qml"
                            }
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.rightMargin: 6
            width: base_window.width/3
            spacing: 6

            Text {
                text: "Resource drop area:"
            }

            Item {
                id: drag_area_item
                height: 150
                Layout.fillWidth: true

                Rectangle {
                    anchors.fill: parent
                    color: "#500000FF"
                }
                Image {
                    height: 50
                    width: 50
                    anchors.centerIn: parent
                    source: "images/move.png"
                    fillMode: Image.PreserveAspectCrop
                }

                DropArea {
                    id: drop_area
                    signal nodeTransferred(string uid)

                    anchors.fill: parent
                    onDropped: {
                        map_model.delete_node_by_uid(drop.source.node_id)
                        gathered_resources_model.addResource(drop.source.resource_type, drop.source.resource_amount)
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                height: 1
                width: 24
                color: "darkgray"
            }

            Text {
                text: "Gathered resources:"
            }

            ListView {
                id: resources_list_view
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: gathered_resources_model

                delegate: ItemDelegate {
                    id: delegate
                    width: parent.width

                    Label {
                        text: model.display
                        font.bold: true
                        width: parent.width
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                height: 1
                width: 24
                color: "darkgray"
            }

            Button {
                id: improve_nest_button
                Layout.fillWidth: true
                height: 24
                text: "Improve nest"
                onClicked: {

                }
            }

            Text {
                font.pixelSize: 10
                color: "darkgray"
                text: "Requires 3 leafs and 3 acorns"
            }
        }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: spacing
            Label {
                text: "Time to tick: "
            }
            ProgressBar {
                id: tick_bar
                value: 0.0
            }
            Item { Layout.fillWidth: true }
        }
    }

    Connections {
        target: map_model
        onUpdate_growth_progress_bar: {
            tick_bar.value = progress_amount
        }
    }
}