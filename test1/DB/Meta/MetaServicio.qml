pragma Singleton
import QtQuick 2.0
import OrmQuick 1.0

OrmMetaTable {
    table: "Servicio"
    attributes:  [
        OrmMetaAttribute{ attribute: "idServicio"; index: 0 },
        OrmMetaAttribute{ property: "nombre"; attribute: "Servicio" },
        OrmMetaAttribute{ attribute: "precio" },
        OrmMetaAttribute{ attribute: "reconexion" },
        OrmMetaAttribute{ attribute: "download" },
        OrmMetaAttribute{ attribute: "upload" }
    ]

    url: "../Servicio.qml"
}
