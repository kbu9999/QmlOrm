#include "qormmetaattribute.h"
#include "qormobject.h"
#include "qormmetatable.h"
#include "qorm.h"

#include <QtCore>

class QOrmMetaAttribute::Private
{
public:
    int pos;
    QString attribute;
    QString property;
    int index;
    bool isNullable;

    bool valid;
};

QOrmMetaAttribute::QOrmMetaAttribute() :
    QObject(),
    d(new Private)
{
    d->valid = false;

    d->pos = -1;
    d->index = -1;
    d->isNullable = false;
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

void QOrmMetaAttribute::setValue(QOrmObject *obj, QVariant v)
{
    obj->setProperty(d->property.toLatin1(), v);

    //modificar lista de propiedades modificadas del obj
    //modificar indices del obj
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

void QOrmMetaAttribute::modified()
{
    QOrmObject *o = qobject_cast<QOrmObject*>(sender());
    qDebug()<<o;
    if (!o) return;
}

QOrmMetaForeignKey::QOrmMetaForeignKey() :
    QOrmMetaAttribute()
{
}

QOrmMetaForeignKey::~QOrmMetaForeignKey()
{
}

bool QOrmMetaForeignKey::isForeingkey() const
{
    return true;
}

void QOrmMetaForeignKey::setValue(QOrmObject *obj, QVariant v)
{
    if (!m_prop.isValid()) return;

    bool b = false;
    QOrmObject *ofk = QOrm::defaultOrm()->find(m_fktable, v);
    if (ofk) b = true;
    else ofk = fk.value<QOrmObject*>();
    if (!ofk) {
        qobject_cast<QOrmObject*>(m_prop.enclosingMetaObject()->newInstance());
        b = true;
    }

    ofk->load(v);
    if (b)
        m_prop.write(obj, ofk);
    else
        m_prop.notifySignal().invoke(obj, Q_ARG(QObject*, ofk));
}

void QOrmMetaForeignKey::modified()
{
    qDebug()<<"no implemented";
}

QString QOrmMetaForeignKey::foreignTable() const
{
    return m_foreigntable;
}

void QOrmMetaForeignKey::setForeignTable(QString value)
{
    if (!m_foreigntable.isEmpty()) return;

    m_foreigntable = value;
}

void QOrmMetaForeignKey::setForeignMeta(QOrmMetaTable *fkmeta)
{
    m_fktable = fkmeta;
}

void QOrmMetaForeignKey::setMetaProperty(QMetaProperty mprop)
{
    m_prop = mprop;
}
