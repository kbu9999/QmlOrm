pragma Singleton
import QtQuick 2.0
import OrmQuick 1.0

OrmMetaTable {
    table: "Factura"
    attributes:  [
        OrmMetaAttribute{ attribute: "idFactura"; index: 0 },
        OrmMetaForeignKey{
            property: "cliente"; attribute: "idCliente";
            foreignTable: "Cliente"; index: 1 },
        OrmMetaForeignKey{
            property: "servicio"; attribute: "idServicio";
            foreignTable: "Servicio"; index: 2 },
        OrmMetaAttribute{ attribute: "periodo" },
        OrmMetaAttribute{ property: "emision"; attribute: "fechaEmision" },
        OrmMetaAttribute{ property: "vencimiento"; attribute: "fechaVencimiento" }
    ]

    url: "../Factura.qml"
}
