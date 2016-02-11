#include "qormmetatable.h"

#include "qorm.h"

#include <QtCore>

class QOrmMetaTable::Private
{
public:
    QString table;
    QString database;
    QString sqlSelect;
    QString sqlInsert;
    QString sqlDelete;
    QString sqlUpdate;

    QQmlComponent *component;

    QList<QOrmMetaAttribute*> attrs;
    QList<QOrmMetaRelation*> rels;
};

QOrmMetaTable::QOrmMetaTable(QObject *parent) :
    QObject(parent),
    d(new Private())
{
    //qDebug()<<parent;
}

QOrmMetaTable::~QOrmMetaTable()
{
    delete d;
}

QString QOrmMetaTable::database() const
{
    return d->database;
}

QString QOrmMetaTable::table() const
{
    return d->table;
}

QString QOrmMetaTable::sqlSelect() const
{
    return d->sqlSelect;
}

QString QOrmMetaTable::sqlInsert() const
{
    return d->sqlInsert;
}

QString QOrmMetaTable::sqlUpdate() const
{
    return d->sqlUpdate;
}

QString QOrmMetaTable::sqlDelete() const
{
    return d->sqlDelete;
}

QQmlListProperty<QOrmMetaAttribute> QOrmMetaTable::attributes()
{
    return QQmlListProperty<QOrmMetaAttribute>(this, d->attrs);
}

QQmlListProperty<QOrmMetaForeignKey> QOrmMetaTable::foreignkeys()
{
    return QQmlListProperty<QOrmMetaForeignKey>();
}

QQmlListProperty<QOrmMetaRelation> QOrmMetaTable::relations()
{
    return QQmlListProperty<QOrmMetaRelation>();
}

void QOrmMetaTable::connectAttributes(QOrmObject *obj)
{
    /*QMetaObject amo = obj->;
    QMetaMethod mv = amo.method(amo.indexOfMethod("setValue()"));
    QMetaMethod mi = amo.method(amo.indexOfMethod("setIndex()"));
    QMetaMethod mf = amo.method(amo.indexOfMethod("setForeignkey()"));
    for(QOrmAttributeInfo *a : d->attrs)
    {
        if (a->pos() < 0) continue;

        int i = d->mobj->indexOfProperty(a->propertyName().toLatin1());
        QMetaProperty prop = d->mobj->property(i);
        if (!prop.hasNotifySignal()) continue;

        if (a->isForeignKey())
            d->mobj->connect(obj, prop.notifySignalIndex() - 2, a, mf.methodIndex());
        else {
            if (a->isPrimaryKey())
                d->mobj->connect(obj, prop.notifySignalIndex() - 2, a, mi.methodIndex());
            else
                d->mobj->connect(obj, prop.notifySignalIndex() - 2, a, mv.methodIndex());
        }
    }*/
}

QQmlComponent *QOrmMetaTable::component() const
{
    return d->component;
}

void QOrmMetaTable::setTable(QString value)
{
    if (!d->table.isEmpty()) return;

    d->table = value;
    emit tableChanged(value);
}

void QOrmMetaTable::setDatabase(QString value)
{
    if (!d->database.isEmpty()) return;

    d->database = value;
    emit databaseChanged(value);
}

void QOrmMetaTable::setSqlSelect(QString value)
{
    if (d->sqlSelect == value) return;

    d->sqlSelect = value;
    emit sqlSelectChanged(value);
}

void QOrmMetaTable::setSqlInsert(QString value)
{
    if (d->sqlInsert == value) return;

    d->sqlInsert = value;
    emit sqlInsertChanged(value);
}

void QOrmMetaTable::setSqlDelete(QString value)
{
    if (d->sqlDelete == value) return;

    d->sqlDelete = value;
    emit sqlDeleteChanged(value);
}

void QOrmMetaTable::setSqlUpdate(QString value)
{
    if (d->sqlUpdate == value) return;

    d->sqlUpdate = value;
    emit sqlUpdateChanged(value);
}

void QOrmMetaTable::setComponent(QQmlComponent *value)
{
    if (d->component == value) return;

    auto obj = value->create();
    QString type = obj->metaObject()->className();
    delete obj;
    QOrm::defaultOrm()->appendTable(type, this);

    d->component = value;
    emit componentChanged(value);
}
