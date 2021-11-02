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

    // TODO: To separate file
    MessageDialog {
        id: aboutDialog
        icon: StandardIcon.Information
        title: "About game"
        text: "This is text"
        informativeText: "This is informativeText"
    }

    menuBar: MenuBar {
        Menu {
            title: "&Game"
            MenuItem {
                text: "E&xit"
                onTriggered: Qt.quit()
            }
            MenuItem {
                text: "&Restart"
            }
        }
        Menu {
            // TODO: Add screens with real rules
            title: "&Help"
            MenuItem {
                text: "Resource system"
            }
            MenuItem {
                text: "How to explore"
            }
            MenuItem {
                text: "About game"
                onTriggered: aboutDialog.open()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
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
            width: base_window.width/3

            Item {
                id: drag_area_item
                height: 150
                width: parent.width

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
                    anchors.fill: parent
                    onEntered: console.log("entered " + drag.source.node_id)
                    onExited: console.log("exited")
                }
            }

            ListView {
                id: resources_list_view
                width: parent.width
                Layout.fillHeight: true
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