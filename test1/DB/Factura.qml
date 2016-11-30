import QtQuick 2.0
import OrmQuick 1.0

import "Meta"

OrmObject {
    metaTable: MetaFactura

    property int idFactura
    property OrmObject cliente
    property Servicio servicio
    property string periodo
    property date emision
    property date vencimiento
}

