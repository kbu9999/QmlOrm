#include "orm_plugin.h"

#include <qqml.h>

#include "qorm.h"
#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormobject.h"

static QObject *createOrm(QQmlEngine *engine, QJSEngine *e) {
    Q_UNUSED(engine)
    Q_UNUSED(e)
    return QOrm::defaultOrm();
}

void OrmPlugin::registerTypes(const char *uri)
{
    // @uri QtQuick.Orm
    qmlRegisterSingletonType<QOrm>(uri, 1, 0, "Orm", createOrm);
    qmlRegisterType<QOrmMetaTable>(uri, 1, 0, "OrmMetaTable");
    qmlRegisterType<QOrmMetaAttribute>(uri, 1, 0, "OrmMetaAttribute");
    qmlRegisterType<QOrmObject>(uri, 1, 0, "OrmObject");
}


