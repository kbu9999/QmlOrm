#include "qormmetarelation.h"
#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormmetaforeignkey.h"
#include "qorm.h"

#include <QQmlListReference>
#include <QQmlProperty>

QOrmMetaRelation::QOrmMetaRelation(QObject *parent)
    : QObject(parent),
       m_reltable(0), m_fka(0)
{
}

QString QOrmMetaRelation::relationTableName() const
{
    return m_relationtablename;
}

void QOrmMetaRelation::setChildrenTableName(QString value)
{
    if (m_relationtablename == value) return;

    m_relationtablename = value;
    emit childrenTableNameChanged(m_relationtablename);
}

QString QOrmMetaRelation::query() const
{
    return ld.query();
}

void QOrmMetaRelation::setQuery(QString value)
{
    if (ld.query() == value) return;

    ld.setQuery(value);
    emit queryChanged(ld.query());
}

QString QOrmMetaRelation::property() const
{
    return m_property;
}

void QOrmMetaRelation::setProperty(QString value)
{
    if (m_property == value) return;

    m_property = value;
    emit propertyChanged(m_property);
}

void QOrmMetaRelation::setRelationMetaTable(QOrmMetaTable *rmt, QOrmMetaForeignKey *fka)
{
    m_reltable = rmt;
    m_fka = fka;

    if (ld.query().isEmpty()) {

        ld.setQuery(QString("%1 WHERE `%2` = :%2 ")
                    .arg(rmt->sqlSelect())
                    .arg(fka->attribute()));

        //qDebug()<<ld.query();
        //qDebug()<<ld.bindValues();
    }
    ld.setTable(rmt);
}

QOrmMetaTable *QOrmMetaRelation::relationMetaTable() const
{
    return m_reltable;
}

QOrmObject *QOrmMetaRelation::at(QOrmObject *obj, int index)
{
    QQmlListReference lst(obj, m_property.toLatin1(), qmlEngine(this));
    if (!lst.isValid()) return NULL;

    return qobject_cast<QOrmObject*>(lst.at(index));
}

void QOrmMetaRelation::append(QOrmObject *obj, QOrmObject *add)
{
    QQmlListReference lst(obj, m_property.toLatin1(), qmlEngine(this));
    if (!lst.isValid()) return;

    lst.append(add);

    //if (!add->isSaved()) QOrm::defaultOrm()->append(add);
}

void QOrmMetaRelation::remove(QOrmObject *obj, int index)
{
    QQmlListReference lst(obj, m_property.toLatin1(), qmlEngine(this));
    if (!lst.isValid()) return;

    QObjectList tmp;
    for (int i = 0; i < lst.count(); i++)
        if (i != index)
            tmp.append(lst.at(i));

    lst.clear();

    for(QObject *o : tmp) lst.append(o);
}

QList<QOrmObject *> QOrmMetaRelation::toList(QOrmObject *obj)
{
    QList<QOrmObject *> l;
    QQmlListReference lst(obj, m_property.toLatin1(), qmlEngine(this));
    if (!lst.isValid()) return l;

    for (int i = 0; i < lst.count(); i++)
        l.append((QOrmObject*)lst.at(i));

    return l;
}

void QOrmMetaRelation::load(QOrmObject *obj)
{
    QQmlListReference lst(obj, m_property.toLatin1(), qmlEngine(this));
    if (!lst.isValid()) return;

    ld.addBindObject(obj);
    for(QOrmObject *o : ld.loadAll()) {
        lst.append(o);
    }
}

