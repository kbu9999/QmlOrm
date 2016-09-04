#include "qormmetaattribute.h"
#include "qormobject.h"
#include "qormmetatable.h"
#include "qorm.h"
#include "qormloader.h"

#include <QtCore>

class QOrmMetaAttribute::Private
{
public:
    int pos;
    QString attribute;
    QString property;
    int index;
    bool isNullable;
    bool readonly;

    bool valid;

    bool onLoad;
};

QOrmMetaAttribute::QOrmMetaAttribute() :
    QObject(),
    d(new Private)
{
    d->valid = false;

    d->pos = -1;
    d->index = -1;
    d->isNullable = false;

    d->readonly = false;

    d->onLoad = false;
}

QOrmMetaAttribute::~QOrmMetaAttribute()
{
    delete d;
}

bool QOrmMetaAttribute::isValid() const
{
    return d->valid;
}

int QOrmMetaAttribute::pos() const
{
    return d->pos;
}

QString QOrmMetaAttribute::attribute() const
{
    return d->attribute;
}

QString QOrmMetaAttribute::property() const
{
    return d->property;
}

int QOrmMetaAttribute::index() const
{
    return d->index;
}

bool QOrmMetaAttribute::isIndex() const
{
    return d->index >= 0;
}

bool QOrmMetaAttribute::isForeingkey() const
{
    return false;
}

bool QOrmMetaAttribute::isNullable() const
{
    return d->isNullable;
}

bool QOrmMetaAttribute::readOnly() const
{
    return d->readonly;
}

void QOrmMetaAttribute::blockOnLoad()
{
    d->onLoad = true;
}

void QOrmMetaAttribute::unBlockOnLoad()
{
    d->onLoad = false;
}

void QOrmMetaAttribute::setValue(QOrmObject *obj, QVariant v)
{
    QQmlProperty p(obj, property(), qmlContext(obj));
    blockOnLoad();
    p.write(v);
    unBlockOnLoad();

    //TODO modificar lista de propiedades modificadas del obj
    if (d->index < 0) return;

    obj->setIndexValue(d->index, v);
}

void QOrmMetaAttribute::modified(QOrmObject *obj)
{
    if (d->index < 0) return;

    QQmlProperty p(obj, property(), qmlContext(obj));
    obj->setIndexValue(d->index, p.read());
}

void QOrmMetaAttribute::setPos(int value)
{
    if (d->pos == value) return;

    d->pos = value;
    emit posChanged(value);
}

void QOrmMetaAttribute::setAttribute(QString value)
{
    if (d->attribute == value) return;

    d->attribute = value;
    emit attributeChanged(value);

    if (d->property.isEmpty()) setProperty(value);
}

void QOrmMetaAttribute::setProperty(QString value)
{
    if (d->property == value) return;

    d->property = value;
    emit propertyChanged(value);
}

void QOrmMetaAttribute::setIndex(int value)
{
    if (d->index == value) return;

    d->index = value;
    emit indexChanged(value);
    emit isIndexChanged(isIndex());
}

void QOrmMetaAttribute::setIsNullable(bool value)
{
    if (d->isNullable == value) return;

    d->isNullable = value;
    emit isNullableChanged(value);
}

void QOrmMetaAttribute::setReadOnly(bool value)
{
    if (d->readonly == value) return;
    d->readonly = value;
    emit readOnlyChanged(d->readonly);
}

void QOrmMetaAttribute::attr_modified()
{
    if (d->onLoad) return;

    QOrmObject *o = qobject_cast<QOrmObject*>(sender());
    if (!o) return;

    modified(o);
    o->modified();
}
