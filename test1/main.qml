import QtQuick 2.3
import QtQuick.Controls 1.2

import QtQuick.Orm 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Orm {
        tables: [
            OrmMetaTable {
                table: "Cliente"
                database: "rci_db"
                attributes:  [
                    OrmMetaAttribute{ property: "idCliente"; attribute: "idCliente"; index: 0 },
                    //OrmForeingKey{ property: "servicio"; attributes: [ "idServicio" ] },
                    OrmMetaAttribute{ property: "dni"; attribute: "dni" }
                ]
                component: Cliente { }
            }
        ]
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
        text: qsTr("Hello World")
        anchors.centerIn: parent

        anchors {
            centerIn: parent
        }
    }
}

