//pragma Singleton
import QtQuick 2.0
import QtQuick.Orm 1.0

Orm {
    database: "rci_db"
    user: "kbu9999"
    password: "xtra2921"
    tables: [
        OrmMetaTable {
            table: "Cliente"
            database: "rci_db"
            attributes:  [
                OrmMetaAttribute{ property: "idCliente"; attribute: "idCliente"; index: 0 },
                //OrmForeingKey{ property: "servicio"; attributes: [ "idServicio" ] },
                OrmMetaAttribute{ property: "dni"; attribute: "dni" }
            ]
            component: Cliente {
            }
        }
    ]
}

