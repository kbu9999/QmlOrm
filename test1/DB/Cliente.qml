import QtQuick 2.0
import QtQuick.Orm 1.0

OrmObject {
    property int idCliente : 0
    property Servicio servicio
    property int dni: 0
    property string nombre
    property string localidad
    property string barrio
    property string direccion
    property int codpost: 0
    property string tel: "(381) "

    //database: "rci_db"

    OrmObject.table: "Cliente"

    Component.onCompleted: {
        console.log(OrmObject.attributes.length)
    }
}

