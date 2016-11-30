#include "qorm.h"

#include <QtSql>

#include "qormmetatable.h"
#include "qormmetaattribute.h"
#include "qormmetaforeignkey.h"
#include "qormmetarelation.h"
#include "qormobject.h"
#include "private/qormmetatable_p.h"
#include "private/qormqueryparser.h"

#include <QPair>

typedef QPair<QOrmObject*, QVariant> Pair;

/*static bool execQuery(QSqlQuery *q, QOrmMetaTable *t, QOrmObject *o) {
    for(QOrmMetaAttribute *attr : t->listAttributes()) {
        QString e = ":" + attr->attribute().replace(" ", "_");
        if (attr->isIndex())
            q->bindValue(e, o->indexes().at(attr->index()));
        else
            q->bindValue(e, o->property(attr->property().toLatin1()));
    }

    return q->exec();
}//*/

static void find_dependencies(QOrm *db, QOrmMetaTable *v) {
    for(QOrmMetaForeignKey *fka : v->listForeignkeys()) {
        if (fka->foreignKeyMetaTable()) continue;

        QOrmMetaTable *mfk = db->findTable(fka->foreignTable());
        if (!mfk) continue;

        fka->setForeignKeyMetaTable(mfk);
    }

    for(QOrmMetaRelation *r : v->listRelations()) {
        if (r->relationMetaTable()) continue;

        QOrmMetaTable *rmt = db->findTable(r->relationTableName());
        if (!rmt) continue;

        for(QOrmMetaForeignKey *fka : rmt->listForeignkeys()) {
            //qDebug()<<t->table()<<fka->foreignTable()<<rmt->table();
            if (v->table() == fka->foreignTable()) {
                r->setRelationMetaTable(rmt, fka);
            }
        }
    }
}

//QOrm *QOrm::m_def = NULL;

QOrm::QOrm() : QObject()
{
    m_started = false;
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

void QOrm::classBegin()
{
}

void QOrm::componentComplete()
{
    QOrmSelectParser sl;
    QOrmInsertParser ins;
    QOrmUpdateParser upd;
    QOrmDeleteParser del;

    for (auto t : m_tables) {
        if (!t)  continue;

        t->setDatabase(this);
        t->setDatabaseName(database());

        if (t->d->sqlSelect.isEmpty()) t->d->sqlSelect = sl.query(t);
        if (t->d->sqlInsert.isEmpty()) t->d->sqlInsert = ins.query(t);
        if (t->d->sqlUpdate.isEmpty()) t->d->sqlUpdate = upd.query(t);
        if (t->d->sqlDelete.isEmpty()) t->d->sqlDelete = del.query(t);
    }

    for (auto t : m_tables) {
        if (!t)  continue;
        find_dependencies(this, t);
    }
}

bool QOrm::connected() const
{
    return m_db.isOpen();
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

QOrmMetaTable *QOrm::findTable(QString tableName)
{
    for(auto i = m_tables.begin(); i != m_tables.end(); ++i) {
        if ((*i)->table() == tableName) return (*i);
    }
    return NULL;
}

QOrmObject *QOrm::find(QOrmMetaTable *table, QVariant pk)
{
    return table->find(pk);
}

/*void QOrm::append(QOrmObject *obj)
{
    if (!obj) return;

    if (!obj->metaTable()) {
        obj->setMetaTable(findTable(obj->tableName()));
    }

    auto t = obj->metaTable();
    if (!t) return;

    t->append(obj);
    for(QOrmMetaRelation *r : t->listRelations()) {
        for(QOrmObject *o : r->toList(obj)) {
            if (o->metaTable()) o->metaTable()->append(o);
        }
    } //*/
//}*/

void QOrm::deleted(QOrmObject *obj)
{
    if (!obj) return;
    auto t = obj->metaTable();
    if (!t) return;

    //TODO
}

void QOrm::__databaseOpen()
{
    if (!m_db.open()) {
        emit error(m_db.lastError().text());
        return;
    }
    //qDebug()<<"open";
    emit connectedChanged(true); //*/
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

void QOrm::newError(QString context, QString msg)
{
    emit error(context +": " + msg);
}

void QOrm::start_transaction()
{
    if (m_started) m_db.rollback();

    m_started = true;
    qDebug()<<"start transaction: "<<m_db.transaction();
}

void QOrm::end_transaction(bool exec)
{
    m_started = false;
    if (exec) m_db.commit();
    else m_db.rollback();
}

void QOrm::append(QOrmObject *obj)
{
    if (!obj) return;
    if (!obj->metaTable()) return;

    obj->metaTable()->append(obj);
}

#include <QJsonArray>

QVariantList QOrm::exec(QString query, QVariantMap data)
{
    QSqlQuery q;
    q.prepare(query);
    for(auto i = data.begin(); i != data.end(); i++)
        q.bindValue(":"+i.key(), i.value());

    if (!q.exec()) return QVariantList();
    QSqlRecord r = q.record();
    QJsonArray a;
    while(q.next()) {
        QJsonObject o;
        for(int i = 0; i < r.count(); i++)
           o.insert(r.fieldName(i),  QJsonValue::fromVariant(q.value(i)));

        a.append(o);
    }

    return a.toVariantList();
}

#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

static bool __opendb(QSqlDatabase *db) {
    bool bs = db->open();
    qDebug()<<"open "<<db<<bs;
    return bs;
}

void QOrm::connect(QString user, QString pass, QString db, QString host)
{
    if (!user.isEmpty()) setUser(user);
    if (!pass.isEmpty()) setPassword(pass);
    if (!db.isEmpty()) setDatabase(db);
    if (!host.isEmpty()) setHost(host);
    if (m_db.isOpen()) return;

    qDebug()<<"future"<<&m_db;
    /*QFutureWatcher<bool> _watcher;
    QObject::connect(&_watcher, &QFutureWatcher<bool>::finished, this, &QOrm::__databaseOpen);
    QFuture<bool> future = QtConcurrent::run(__opendb, &m_db);//, &QSqlDatabase::open);
    _watcher.setFuture(future);/*/
    QtConcurrent::run(this, &QOrm::__databaseOpen);

    /*if (!m_db.open()) {
        emit error(m_db.lastError().text());
        return;
    }

    emit connectedChanged(true); //*/
}
