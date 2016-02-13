#include "qormobject.h"

#include "qorm.h"
#include "qormmetatable.h"
#include "qormmetaattribute.h"

//#include "qormloader.h"
//#include "qormsaver.h"

#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class QOrmObject::Private
{
public:
    QVector<QVariant> indexes;
    bool empty, saved, deleted, dirt;

    Private() :
        empty(true), saved(false), deleted(false), dirt(false)
    {
    }
};

QOrmObject::QOrmObject()  :
    d(new Private())
{
    d->indexes = QVector<QVariant>();
    //metatable->connectAttributes(this);
    QOrmMetaTable *meta = QOrm::defaultOrm()->findTable(metaObject()->className());
    if (meta) {
        meta->connectAttributes(this);
    }
}

QOrmObject::~QOrmObject()
{
    /*blockSignals(true);
    //for(QOrmChildrens c : metaTable()->childrens())
    //    for (QOrmObject * o : c.list(this)) removeChildren(o);

    int c = metaTable()->foreignKeysCount();
    for (int i = 0; i < c; i++)
    {
        QOrmAttributeInfo fk = metaTable()->foreignKey(i);
        fk.writeForeingKey(this, NULL);
    }
    metaTable()->cache()->remove(this);
    blockSignals(false); //*/
    delete d;
}

void QOrmObject::load(QVariant pk)
{
    qDebug()<<pk;

    emit loaded(false);
}

void QOrmObject::save()
{
}

void QOrmObject::clear()
{
    //d->indexes = QVector<QVariant>(metaTable()->indexCount());

    d->deleted = false;
    d->saved = false;
    d->dirt = false;
    d->empty = true;

    emit deletedChanged(d->deleted);
    emit savedChanged(d->saved);
    emit dirtChanged(d->dirt);
    emit emptyChanged(d->empty);
}

void QOrmObject::loadChildren()
{
}

void QOrmObject::loadForeignKeys()
{
}

void QOrmObject::loadAllForeignKeys()
{
    //for (int i = 0; i < metaTable()->foreignKeysCount(); ++i)
    //    d->loadFKs(this, metaTable()->foreignKey(i));
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
