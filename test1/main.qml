import QtQuick 2.3
import QtQuick.Controls 1.2
import OrmQuick 1.0

import "DB"
import "DB/Meta"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    OrmDataBase {
        id: db
        user: "root"
        password: "xtra2921"
        database: "rci_db"

        tables: [
            MetaCliente,
            MetaServicio,
            MetaFactura
        ]

        onConnectedChanged: {
            ld.item.load()
        }

        onError: console.log(error)
    }

    Component.onCompleted: {
        db.connect()
    }

    property Factura cli: Factura {
        emision: new Date()

        //onClienteChanged: console.log("cli: "+cli.cliente)
    }

    OrmRelationModel {
        id: rel
        metaTable: MetaCliente
        rootObject: cli.cliente
        property: "facturas"

        onCountChanged: console.log(count)
    }

    Loader {
        id: ld
        anchors.fill: parent

        source: "test_1.qml"

        Connections {
            target: ld.item
            ignoreUnknownSignals: true
            onClientChanged: {
                console.log("cl: "+cl)
                cl.loadFacturas()
                cli.cliente = cl;
                rel.rootObject = cl
                ld.sourceComponent = null
            }
        }
    }

    Timer {
        //running: true
        interval: 2000
        repeat: true
        onTriggered: {
            for(var i in db.tables) {
                var t = db.tables[i]
                console.log(t.table+": "+t.count())
            }
        }
    }

    Button {
        text: "clear"
        onClicked: ld.sourceComponent = null
    }
}
