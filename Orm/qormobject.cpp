#include "qormobject.h"

#include "qorm.h"
#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormmetaforeignkey.h"
#include "qormmetarelation.h"

//#include "qormloader.h"
#include "qormsaver.h"

#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class QOrmObject::Private
{
public:
    QVector<QVariant> indexes;
    bool empty, saved, deleted, dirt;
    QString tableName;
    QOrmMetaTable *meta;
    Private() :
        empty(true), saved(false), deleted(false), dirt(false),
        meta(0)
    {
    }
};

static QOrmMetaRelation *findRelation(QOrmMetaTable *mt, QOrmMetaTable *meta) {
    if (!mt) return NULL;

    for(QOrmMetaRelation *r : mt->listRelations()) {
        if (r->relationMetaTable() == meta)
            return r;
    }

    return NULL;
}

static QOrmMetaForeignKey *findForeignKey(QOrmMetaTable *mt, QOrmMetaTable *meta) {
    if (!mt) return NULL;

    for(QOrmMetaForeignKey *fka : mt->listForeignkeys()) {
        if (fka->foreignKeyMetaTable() == meta)
            return fka;
    }

    return NULL;
}

QOrmObject::QOrmObject(QObject *parent)  :
    QObject(parent),
    d(new Private())
{
    d->indexes = QVector<QVariant>(1, QVariant());
}

QOrmObject::~QOrmObject()
{
    delete d;
}

void QOrmObject::clear()
{
    if (d->meta)
        d->indexes = QVector<QVariant>(d->meta->indexCount());

    d->deleted = false;
    d->saved = false;
    d->dirt = false;
    d->empty = true;

    emit deletedChanged(d->deleted);
    emit savedChanged(d->saved);
    emit dirtChanged(d->dirt);
    emit emptyChanged(d->empty);
}

bool QOrmObject::save()
{
    QOrmSaver s(d->meta->database());
    s.start();
    s.Save(this);
    return s.commit();
}

bool QOrmObject::reload() 
{
    return QOrmLoader::reload(this);
}

void QOrmObject::loadRelation(QOrmMetaTable *meta)
{
    auto tmp = findRelation(d->meta, meta);
    if (!tmp) return;

    tmp->load(this);
}

void QOrmObject::append(QOrmMetaTable *meta, QOrmObject *obj)
{
    auto tmp = findRelation(d->meta, meta);
    if (!tmp) return;

    tmp->append(this, obj);
}

void QOrmObject::remove(QOrmMetaTable *meta, int index)
{
    auto tmp = findRelation(d->meta, meta);
    if (!tmp) return;

    tmp->remove(this, index);
}

void QOrmObject::loadAllForeignKeys()
{
    for(QOrmMetaForeignKey *fka : d->meta->listForeignkeys()) {
        fka->blockOnLoad();
        fka->load(this);
        fka->unBlockOnLoad();
    }
}

void QOrmObject::deleteLater()
{
    QObject::deleteLater();
}

QVariant QOrmObject::primaryKey()
{
    if (d->indexes.isEmpty())
        return QVariant();

    return d->indexes.at(0);
}

QVariantList QOrmObject::indexes()
{
    return d->indexes.toList();
}

void QOrmObject::setIndexValue(int i, QVariant v)
{
    if (i >= d->indexes.length()) return;

    d->indexes[i] = v;
    emit indexesChanged();

    if (i == 0)
        emit primaryKeyChanged(v);
}

void QOrmObject::setMetaTable(QOrmMetaTable *mt)
{
    if (d->meta) return;

    d->meta = mt;
    d->indexes = QVector<QVariant>(mt->indexCount(), QVariant());
    mt->connectAttributes(this);
}

QOrmMetaTable *QOrmObject::metaTable() const
{
    return d->meta;
}

bool QOrmObject::loadForeignKey(QOrmMetaTable *meta)
{
    auto fka = findForeignKey(d->meta, meta);
    if (!fka) return false;

    return fka->load(this);
}

bool QOrmObject::isSaved() const
{
    return d->saved;
}

bool QOrmObject::isDeleted() const
{
    return d->deleted;
}

bool QOrmObject::isEmpty() const
{
    return d->empty;
}

bool QOrmObject::isDirt() const
{
    return d->dirt;
}

bool QOrmObject::needPrimaryKey()
{
    return (primaryKey().isNull() || primaryKey() == 0) && !isSaved();
}

void QOrmObject::modified()
{
    if (d->deleted)
        return;

    if (d->empty)
        d->empty = false;

    d->dirt = true;

    emit dirtChanged(d->dirt);
    emit emptyChanged(d->empty);
}

void QOrmObject::setAsLoaded()
{
    if (d->deleted) return;
    d->dirt = false;
    d->saved = true;
    d->empty = false;

    savedChanged(d->saved);
    dirtChanged(d->dirt);
    emptyChanged(d->empty);
}

void QOrmObject::setAsDeleted()
{
    if (d->empty)
        return;

    d->deleted = true;

    deletedChanged(d->deleted);
}
