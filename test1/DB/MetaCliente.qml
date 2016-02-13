import QtQuick 2.0
import QtQuick.Orm 1.0

Component {
    OrmMetaTable {
        table: "Cliente"
        database: "rci_db"
        attributes:  [
            OrmMetaAttribute{ property: "idCliente"; attribute: "idCliente"; index: 0 },
            //OrmForeingKey{ property: "servicio"; attributes: [ "idServicio" ] },
            OrmMetaAttribute{ property: "dni"; attribute: "dni" }
        ]
    }
}
