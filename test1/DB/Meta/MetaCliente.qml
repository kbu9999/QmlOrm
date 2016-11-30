pragma Singleton
import QtQuick 2.0
import OrmQuick 1.0

OrmMetaTable {
    table: "Cliente"
    //database: "RCInternet2"
    attributes:  [
        OrmMetaAttribute{ property: "idCliente"; attribute: "idCliente"; index: 0 },
        OrmMetaForeignKey{
            property: "servicio"; attribute: "idServicio";
            foreignTable: "Servicio"; index: 1 },
        OrmMetaAttribute{ attribute: "dni" },
        OrmMetaAttribute{ attribute: "nombre" },
        OrmMetaAttribute{ attribute: "localidad" },
        OrmMetaAttribute{ attribute: "barrio" },
        OrmMetaAttribute{ attribute: "direccion" },
        OrmMetaAttribute{ attribute: "codpost" },
        OrmMetaAttribute{ property: "tel"; attribute: "telefono" }
    ]

    relations: [
        OrmMetaRelation { property: "facturas"; relationTableName: "Factura" }
    ]

    url: "../Cliente.qml"
}

