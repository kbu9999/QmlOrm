import QtQuick 2.3
import QtQuick.Controls 1.2
import OrmQuick 1.0

import "DB"
import "DB/Meta"

TableView {
    id: view

    signal clientChanged(Cliente cl)

    function load() {
        ld.load();
    }

    model: OrmModel {
        id: ld
        metaTable: MetaCliente
    }

    TableViewColumn { title: "idCliente"; role: "idCliente" }
    TableViewColumn { title: "Nombre"; role: "nombre" }

    onDoubleClicked: {
        var c = ld.at(currentRow)
        if (!c) return;

        ld.remove(c)
        clientChanged(c)
    }
}
