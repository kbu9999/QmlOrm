import QtQuick 2.0
import OrmQuick 1.0

import "Meta"

OrmObject {
    metaTable: MetaServicio

    property int idServicio: 0
    property string nombre
    property double precio: 0
    property double reconexion: 0
    property string download
    property string upload
}

