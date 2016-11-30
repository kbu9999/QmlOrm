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

    QtObject {
        id: p1

        default property alias data: p1.lst
        property list<OrmObject> lst

        lst: [
            Cliente { nombre: "lal" },
            Cliente { nombre: "lal2"}
        ]


    }

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
        metaTable: MetaFactura
        rootObject: Cliente {
            id: cl1
            nombre: "cl prueba"
            facturas: [
                Factura { cliente: cl1; emision: new Date(); periodo: "uno" },
                Factura { cliente: cl1; emision: new Date(); periodo: "dos" }
            ]
        }

        property: "facturas"

        onCountChanged: console.log("count", count)
    }

    Loader {
        id: ld
        anchors.fill: parent


        source: "test_1.qml"
        onItemChanged:  {
            if (!item) return

            item.model = p1.data
            item.clientChanged(clChanged)
            rel.insertRows(0, 1)
            rel.at(1).periodo = "tt3"

            console.log(rel.rootObject.facturas.length)
            rel.submit()
            console.log(rel.rootObject.facturas.length)
        }

        function  clChanged() {
            console.log("cl: "+cl)
            cl.loadFacturas()
            cli.cliente = cl;
            rel.rootObject = cl
            ld.sourceComponent = null


        }

    }

    ListView {
        anchors.fill: parent
        model: rel
        delegate: Text {
            text: periodo
            font.bold: true
            height: 80; width: 120
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
