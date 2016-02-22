#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormobject.h"
#include "qorm.h"

#include "private/qormqueryparser.h"

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

    QList<QOrmMetaAttribute*> attrs;
    QList<QOrmMetaRelation*> rels;
};

QOrmMetaTable::QOrmMetaTable() :
    QOrmCache(),
    d(new Private())
{
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

QList<QOrmMetaAttribute *> QOrmMetaTable::listAttributes()
{
    return d->attrs;
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
    const QMetaObject *amo = obj->metaObject();
    for(QOrmMetaAttribute *a : d->attrs)
    {
        if (a->pos() < 0) continue;

        int i = amo->indexOfProperty(a->property().toLatin1());
        QMetaProperty prop = amo->property(i);
        if (!prop.hasNotifySignal()) continue;

        amo->connect(obj, prop.notifySignalIndex() - 2,
                     a, a->metaObject()->indexOfMethod("modified()"));
    }
}

void QOrmMetaTable::classBegin()
{
}

void QOrmMetaTable::componentComplete()
{
    QOrm::defaultOrm()->appendTable(this);
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


QOrmObject *QOrmCache::find(QVariant pk)
{
    if (pk.isNull()) return NULL;

    auto i = m_cache.find(pk);
    if (i == m_cache.end()) return NULL;

    return i.value();
}

void QOrmCache::append(QOrmObject *obj)
{
    if (!obj) return;
    if (!obj->isSaved() || obj->isDeleted()) return;

    QVariant pk = obj->primaryKey();
    if (pk.isNull()) return;

    m_cache[pk] = obj;
    connect(obj, &QObject::destroyed, this, &QOrmCache::remove);
}

void QOrmCache::remove(QObject *obj)
{
    for (auto i = m_cache.begin(); i != m_cache.end(); ++i) {
        if (i.value() == obj) {
            m_cache.erase(i);
            break;
        }
    }
}
