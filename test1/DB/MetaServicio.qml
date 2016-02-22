pragma Singleton
import QtQuick 2.0
import QtQuick.Orm 1.0

OrmMetaTable {
    table: "Servicio"
    database: "rci_db"
    attributes:  [
        OrmMetaAttribute{ attribute: "idServicio"; index: 0 },
        OrmMetaForeignKey{ property: "servicio"; attribute: "idServicio" },
        OrmMetaAttribute{ property: "nombre"; attribute: "Servicio" },
        OrmMetaAttribute{ attribute: "precio" },
        OrmMetaAttribute{ attribute: "reconexion" },
        OrmMetaAttribute{ attribute: "download" },
        OrmMetaAttribute{ attribute: "upload" }
    ]
}
