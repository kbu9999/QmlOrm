import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Orm 1.0

import "DB"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Cliente {
        id: cl
    }
    Cliente {
    }

    Component.onCompleted: {
        /*var cmp = cl.tableComponent
        var v = cmp.createObject();
        console.log(v)
        console.log(v.component.createObject()) //*/
        console.log("+++"+MetaCliente.cmp)
        console.log(MetaCliente.create())
    }

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
        id: txt
        text: qsTr("Hello World")
        anchors.centerIn: parent

        anchors {
            centerIn: parent
        }
    }
}

