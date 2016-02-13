#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormobject.h"
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

    QUrl url;
    QQmlComponent *component;

    QList<QOrmMetaAttribute*> attrs;
    QList<QOrmMetaRelation*> rels;
};

QOrmMetaTable::QOrmMetaTable(QObject *parent) :
    QObject(parent),
    d(new Private())
{
    d->component = 0;
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

QUrl QOrmMetaTable::url() const
{
    return d->url;
}

void QOrmMetaTable::connectAttributes(QOrmObject *obj)
{
    const QMetaObject *amo = obj->metaObject();
    /*QMetaMethod mv = amo.method(amo.indexOfMethod("setValue()"));
    QMetaMethod mi = amo.method(amo.indexOfMethod("setIndex()"));
    QMetaMethod mf = amo.method(amo.indexOfMethod("setForeignkey()")); */
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

QQmlComponent *QOrmMetaTable::component()
{
    if (d->component) {
        if (!d->component->isReady() && d->url.isValid()) {
            delete d->component;
            setComponent(new QQmlComponent(qmlEngine(this), d->url));
        }
    }
    return d->component;
}

QOrmObject *QOrmMetaTable::create()
{
    //qDebug()<<d->component;
    if (!d->component) return NULL;

    QOrmObject *o = qobject_cast<QOrmObject*>(d->component->create());
    if (!o) {
        delete o;
        o = NULL;
    }

    return o;
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

    if (value->isReady()) {
        QOrmObject *o = qobject_cast<QOrmObject*>(value->create());
        if (o) {
            QString type = o->metaObject()->className();
            QOrm::defaultOrm()->appendTable(type, this);
        }
        delete o;
    }
    d->component = value;
    emit componentChanged(value);
}

void QOrmMetaTable::setUrl(QUrl value)
{
    if (d->url == value) return;
    if (d->component)
        if (d->component->isReady())
            return;

    setComponent(new QQmlComponent(qmlEngine(this), d->url));

    d->url = value;
    emit urlChanged(d->url);
}
