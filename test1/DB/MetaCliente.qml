pragma Singleton
import QtQuick 2.0
import QtQuick.Orm 1.0

OrmMetaTable {
    table: "Cliente"
    database: "rci_db"
    attributes:  [
        OrmMetaAttribute{ property: "idCliente"; attribute: "idCliente"; index: 0 },
        OrmMetaForeignKey{ property: "servicio"; attribute: "idServicio"; foreignTable: "Servicio" },
        OrmMetaAttribute{ attribute: "dni" },
        OrmMetaAttribute{ attribute: "nombre" },
        OrmMetaAttribute{ attribute: "localidad" },
        OrmMetaAttribute{ attribute: "barrio" },
        OrmMetaAttribute{ attribute: "direccion" },
        OrmMetaAttribute{ attribute: "codpost" },
        OrmMetaAttribute{ property: "tel"; attribute: "telefono" }
    ]
}

