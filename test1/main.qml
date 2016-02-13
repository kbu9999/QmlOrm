import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Orm 1.0

import "DB"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Label {
        text: qsTr("Hello World")
        anchors.centerIn: parent

        anchors {
            centerIn: parent
        }
    }

    Component.onCompleted: {
        var obj = MetaCliente.createObject()
        console.log(obj)
    }
}

