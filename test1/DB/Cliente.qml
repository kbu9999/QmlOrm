import QtQuick 2.0
import OrmQuick 1.0

import "Meta"

OrmObject {
    id: pcl
    metaTable: MetaCliente
    default property int idCliente : 0
    property Servicio servicio
    property int dni: 0
    property string nombre
    property string localidad
    property string barrio
    property string direccion
    property int codpost: 0
    property string tel: "(381) "

    property list<Factura> facturas

    function appendFactura(fact) {
        append(MetaFactura, fact);
    }

    function loadFacturas() {
        console.log("load fact")
        loadRelation(MetaFactura)
        console.log(facturas.length)
    }

    Component.onDestruction: {
        console.log("unload: "+idCliente)
        //pcl.deleteLater()
    }
}

