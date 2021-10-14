import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.5 as OldControls
import QtQml.Models 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1


ApplicationWindow {
    id: base_window
    title: "Tree Civ"
    width: 600
    height: 600
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

            OldControls.TableViewColumn {
                role: "display"
                title: "Name"
                width: base_window.width*2/3 - 10
                resizable: false
                movable: false
            }
//            TableViewColumn {
//                title: "Number of cities"
//                role: "filePermissions"
//                width: 100
//            }
        }
        ListView {
            id: resources_list_view
            Layout.fillHeight: true
            width: base_window.width/3
        }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: spacing
            Label {
                text: "Time to tick"
            }
            // TODO: Put loading bar here
            Item { Layout.fillWidth: true }
        }
    }
}