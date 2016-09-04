#include "orm_plugin.h"

#include <qqml.h>

#include "qorm.h"
#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormmetaforeignkey.h"
#include "qormobject.h"
#include "qormloader.h"
#include "qormmetarelation.h"
#include "qormmodel.h"

void OrmPlugin::registerTypes(const char *uri)
{
    // @uri OrmQuick
    qmlRegisterType<QOrm>(uri, 1, 0, "OrmDataBase");

    qmlRegisterType<QOrmMetaTable>(uri, 1, 0, "OrmMetaTable");
    qmlRegisterType<QOrmMetaAttribute>(uri, 1, 0, "OrmMetaAttribute");
    qmlRegisterType<QOrmMetaForeignKey>(uri, 1, 0, "OrmMetaForeignKey");
    qmlRegisterType<QOrmMetaRelation>(uri, 1, 0, "OrmMetaRelation");

    qmlRegisterType<QOrmLoader>(uri, 1, 0, "OrmLoader");
    qmlRegisterType<QOrmModel>(uri, 1, 0, "OrmModel");
    qmlRegisterType<QOrmRelationModel>(uri, 1, 0, "OrmRelationModel");
    qmlRegisterType<QOrmFilterModel>(uri, 1, 0, "OrmFilterModel");

    qmlRegisterType<QOrmObject>(uri, 1, 0, "OrmObject");
}


