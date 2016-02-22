#include "qorm.h"

#include <QtSql>

#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormobject.h"
#include "private/qormqueryparser.h"

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

QOrm *QOrm::m_def = NULL;

QOrm::QOrm() : QObject()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    if (!QOrm::m_def)
        QOrm::m_def = this;
}

QString QOrm::database() const
{
    return m_db.databaseName();
}

QString QOrm::user() const
{
    return m_db.userName();
}

QString QOrm::password() const
{
    return m_db.password();
}

QString QOrm::host() const
{
    return m_db.hostName();
}

void QOrm::appendTable(QOrmMetaTable *t)
{
    if (m_tables.contains(t)) return;

    m_tables.append(t);
    emit tablesChanged();
}

void QOrm::removeTable(QOrmMetaTable *t)
{
    if (!m_tables.contains(t)) return;

    m_tables.removeOne(t);
    emit tablesChanged();
}

QOrmObject *QOrm::find(QOrmMetaTable *table, QVariant pk)
{
    return table->find(pk);
}

void QOrm::append(QOrmObject *obj)
{
    if (!obj) return;

    obj->table()->append(obj);
}

QQmlListProperty<QOrmMetaTable> QOrm::tables()
{
    return QQmlListProperty<QOrmMetaTable>(this, m_tables);
}

void QOrm::setDatabase(QString value)
{
    if (m_db.databaseName() == value) return;
    m_db.setDatabaseName(value);

    emit databaseChanged(value);
}

void QOrm::setUser(QString value)
{
    if (m_db.userName() == value) return;
    m_db.setUserName(value);

    emit userChanged(value);
}

void QOrm::setPassword(QString value)
{
    if (m_db.password() == value) return;
    m_db.setPassword(value);

    emit passwordChanged(value);
}

void QOrm::setHost(QString value)
{
    if (m_db.hostName() == value) return;
    m_db.setHostName(value);

    emit hostChanged(value);
}

QList<QOrmMetaTable *> QOrm::listTables() const
{
    return m_tables;
}

QOrm *QOrm::defaultOrm()
{
    if (!m_def)
        m_def = new QOrm();

    return m_def;
}

void QOrm::connect()
{
    if (m_db.isOpen()) return;

    if (!m_db.open()) {
        emit error(m_db.lastError().text());
        return;
    }

    QOrmSelectParser sl;
    QOrmInsertParser ins;
    QOrmUpdateParser upd;
    QOrmDeleteParser del;

    QMap<QString, QOrmMetaTable*> map;
    foreach (QOrmMetaTable *t, m_tables)
    {
        if (t->d->sqlSelect.isEmpty()) t->d->sqlSelect = sl.query(t);
        if (t->d->sqlInsert.isEmpty()) t->d->sqlInsert = ins.query(t);
        if (t->d->sqlUpdate.isEmpty()) t->d->sqlUpdate = upd.query(t);
        if (t->d->sqlDelete.isEmpty()) t->d->sqlDelete = del.query(t);
    }
}

void QOrm::initFks(QOrmMetaTable *t)
{
    for(QOrmMetaAttribute *a : t->listAttributes())
    {
        QOrmMetaForeignKey *fk = qobject_cast<QOrmMetaForeignKey*>(a);
        if (!fk) continue;

        int i = obj->metaObject()->indexOfProperty(fk->property().toLatin1());
        if (i < 0) continue;

        QMetaProperty mp = obj->metaObject()->property(i);
        const QMetaObject *qclass = mp.enclosingMetaObject();
        if (!qclass) {
            error(QString("fkinit: %2 is not a object").arg(mp.name()));
            continue;
        }
        QOrmObject *ofk = qobject_cast<QOrmObject*>(mp.enclosingMetaObject()->newInstance());
        if (!ofk) {
            error(QString("fkinit: %1 is not a OrmObject").arg(mp.name()));
            continue;
        }

        fk->setForeignMeta(ofk->table());
        fk->setMetaProperty(mp);
        delete ofk;
    }
}
