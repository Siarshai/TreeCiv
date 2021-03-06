import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.5 as OldControls
import QtQml.Models 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1
import Qt.labs.qmlmodels 1.0


ApplicationWindow {
    id: base_window
    title: "Tree"

    width: 650
    height: 800

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    visible: true

    MessageDialog {
        id: aboutDialog
        icon: StandardIcon.Information
        title: "About Game"
        text: "What to do:"
        informativeText: "- Expand tree on the left (you can double click items for that)\n- Drag resources and drop them into drop area on the right\n- Locate nest on the tree\n- Press '+' button to upgrade it (it depletes resources)\n- Repeat"
    }

    MessageDialog {
        id: aboutResourceGrowthDialog
        icon: StandardIcon.Information
        title: "About Resource Growth"
        text: "How do resources grow?"
        informativeText: "Each tick (see loading bar below) a random resource appears on the random branch. Then amount of random resource node of each type increased by 1. Branches have certain capacity - tree won't grow beyond certain threshold."
    }

    MessageDialog {
        id: insufficientFundsNotification
        icon: StandardIcon.Information
        title: "Insufficient funds"
        text: "Nest upgrade requires 3 leafs and 3 acorns"
    }

    menuBar: MenuBar {
        Menu {
            title: "&Game"
            MenuItem {
                text: "About &Game"
                onTriggered: aboutDialog.open()
            }
            MenuItem {
                text: "About &Resource Growth"
                onTriggered: aboutResourceGrowthDialog.open()
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

        OldControls.TreeView {
            id: main_tree_view
            Layout.fillHeight: true
            width: base_window.width*3/4
            implicitWidth: base_window.width*3/4
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

            onDoubleClicked: {
                if (main_tree_view.isExpanded(index))
                    main_tree_view.collapse(index)
                else
                    main_tree_view.expand(index)
            }

            OldControls.TableViewColumn {
                role: "delegate_type"
                title: "Tree"
                width: main_tree_view.width - 5
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
            width: base_window.width/4
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
                    color: "lightgray"
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
        onShow_insufficient_funds_message: {
            insufficientFundsNotification.open()
        }
    }
}