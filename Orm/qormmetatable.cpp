#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormmetaforeignkey.h"
#include "qormobject.h"
#include "qorm.h"

#include "private/qormmetatable_p.h"
#include "private/qormqueryparser.h"
#include <QtCore>

QOrmMetaTable::QOrmMetaTable() :
    QOrmCache(),
    d(new Private())
{
    d->initialized = false;
    d->componenet = 0;
    d->indexcount = 0;
}

QOrmMetaTable::~QOrmMetaTable()
{
    delete d;
}

QString QOrmMetaTable::databaseName() const
{
    return d->databaseName;
}

QString QOrmMetaTable::table() const
{
    return d->table;
}

bool QOrmMetaTable::initialized() const
{
    return d->initialized;
}

void QOrmMetaTable::classBegin()
{

}

void QOrmMetaTable::componentComplete()
{
    for(QOrmMetaAttribute *a : d->attrs) {
        if (a->index() >= 0) d->indexcount++;

        QOrmMetaForeignKey *f = qobject_cast<QOrmMetaForeignKey*>(a);
        if (f) d->fks.append(f);
    }

    //qDebug()<<"fk count: "<<d->fks.count()<<" - index count: "<<d->indexcount;
}

QOrm *QOrmMetaTable::database() const
{
    return d->database;
}

void QOrmMetaTable::setDatabase(QOrm *value)
{
    if (d->database == value) return;
    d->database = value;
    emit databaseChanged(d->database);
}

QQmlComponent *QOrmMetaTable::component() const
{
    return d->componenet;
}

int QOrmMetaTable::indexCount() const
{
    return d->indexcount;
}

void QOrmMetaTable::setIndexCount(int count)
{
    if (d->indexcount == count) return;

    d->indexcount = count;
    emit indexCountChanged(count);
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

QList<QOrmMetaRelation *> QOrmMetaTable::listRelations()
{
    return d->rels;
}

QQmlListProperty<QOrmMetaAttribute> QOrmMetaTable::attributes()
{
    return QQmlListProperty<QOrmMetaAttribute>(this, d->attrs);
}

QQmlListProperty<QOrmMetaForeignKey> QOrmMetaTable::foreignkeys()
{
    return QQmlListProperty<QOrmMetaForeignKey>(this, d->fks);
}

QList<QOrmMetaForeignKey *> QOrmMetaTable::listForeignkeys()
{
    return d->fks;
}

QQmlListProperty<QOrmMetaRelation> QOrmMetaTable::relations()
{
    return QQmlListProperty<QOrmMetaRelation>(this, d->rels);
}

void QOrmMetaTable::connectAttributes(QOrmObject *obj)
{
    for(QOrmMetaAttribute *a : d->attrs)
    {
        //if (a->pos() < 0) continue;

        QQmlProperty p(obj, a->property(), qmlContext(obj));
        p.connectNotifySignal(a, SLOT(attr_modified()));

        if (a->isIndex()) a->modified(obj);
    }
}

QOrmObject *QOrmMetaTable::create(QObject *parent)
{
    auto context = parent? qmlContext(parent) : qmlContext(this);
    if (d->componenet->isReady())
        return (QOrmObject*)d->componenet->create(context);

    return 0;
}

void QOrmMetaTable::setTable(QString value)
{
    if (!d->table.isEmpty()) return;

    d->table = value;
    emit tableChanged(value);
}

int QOrmMetaTable::count()
{
    return QOrmCache::count();
}

void QOrmMetaTable::setDatabaseName(QString value)
{
    if (!d->databaseName.isEmpty()) return;

    d->databaseName = value;
    emit databaseNameChanged(value);
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

QUrl QOrmMetaTable::url() const
{
    return m_url;
}

void QOrmMetaTable::setUrl(QUrl value)
{
    if (m_url == value) return;

    m_url = value;
    emit urlChanged(m_url);

    QQmlComponent *cmp = new QQmlComponent(qmlEngine(this));
    cmp->loadUrl(value);

    if (!cmp->isNull())
        setComponenet(cmp);
    else
        delete cmp;
}

void QOrmMetaTable::setComponenet(QQmlComponent *value)
{
    if (d->componenet) return;

    d->componenet = value;
    emit componenetChanged(value);
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
    //if (obj->isSaved() || obj->isDeleted()) return;

    QVariant pk = obj->primaryKey();
    if (pk.isNull() || pk.toInt() == 0) {
        m_add.append(obj);
    }
    else
        m_cache[pk] = obj;

    connect(obj, &QObject::destroyed, this, &QOrmCache::remove);
}

void QOrmCache::remove(QObject *obj)
{
    qDebug()<<"remove: "<<obj;
    for(auto j = m_add.begin(); j != m_add.end(); ++j) {
        if (*j == obj) {
            m_add.erase(j);
            return;
        }
    }
    for (auto i = m_cache.begin(); i != m_cache.end(); ++i) {
        if (i.value() == obj) {
            m_cache.erase(i);
            return;
        }
    }
}

int QOrmCache::count()
{
    return m_cache.count();
}
