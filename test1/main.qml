import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Orm 1.0

import "DB"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Component.onCompleted: {
        Orm.user = "root"
        Orm.password = "raptor"
        Orm.database = "RCInternet2"

        Orm.connect();
        //save changed in db
        //Orm.commit()

        ld.load()
    }

    Connections{
        target: Orm
        onError: console.log(error)
    }

    OrmLoader {
        id: ld
        component: Cliente { }
        //query:
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Load")
                onTriggered: ld.load()
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    property Cliente cl: Cliente{ }

    Rectangle {
        x: 300
        width: 200
        height: 80
        color: "purple"
        Text {
            text: cl.dni + " : "+cl.nombre
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }

    ListView {
        anchors.fill: parent
        model: ld.result
        delegate: Rectangle {
            width: 200; height: 80
            color: "yellow"

            Text {
                text: dni + " : "+nombre
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    cl = ld.result[index]
                    ld.clearResult()
                }
            }
        }
    }
}

