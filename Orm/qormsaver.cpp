#include "qormsaver.h"
#include "qorm.h"
#include "qormobject.h"
#include "qormmetaattribute.h"
#include "qormmetarelation.h"
#include "qormmetaforeignkey.h"
#include "qormmetatable.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

typedef QPair<QOrmObject*, QVariant> Pair;

class QOrmSaver::Private
{
public:
    QOrm *db;
    QList<Pair> toadd;
    QList<QOrmObject*> added;

    //QString query;
    QStringList errors;
    QStringList lastQuery;

    static bool save(QOrmSaver *saver, QOrmObject *o, QOrmMetaTable *t);
    static bool pushAll(QOrmSaver *saver, QOrmObject *obj, QOrmMetaTable *meta);
};

static bool execQuery(QSqlQuery *q, QOrmMetaTable *t, QOrmObject *o) {
    for(QOrmMetaAttribute *attr : t->listAttributes()) {
        QString e = ":" + attr->attribute().replace(" ", "_");
        if (attr->isIndex())
            q->bindValue(e, o->indexes().at(attr->index()));
        else
            q->bindValue(e, o->property(attr->property().toLatin1()));
    }

    return q->exec();
}

QOrmSaver::QOrmSaver(QOrm *db) :
    d(new Private)
{
    d->db = db;
}

bool QOrmSaver::isValid()
{
    return d->db;
}

void QOrmSaver::start()
{
    d->db->start_transaction();
}

QStringList QOrmSaver::lastQuery()
{
    return d->lastQuery;
}

void QOrmSaver::Save(QOrmObject *o)
{
    if (!o) return;
    if (d->added.contains(o)) return;

    //d->added.append(o);
    //d->save(this, o, o->metaTable());
    d->pushAll(this, o, o->metaTable());
}

void QOrmSaver::Delete(QOrmObject *obj)
{
    if (!obj) return;
    if (d->added.contains(obj)) return;

    d->added.append(obj);

    //TODO borrar de la db
    qWarning()<<"no implementado";
}

bool QOrmSaver::commit()
{
    if (!d->errors.isEmpty()) {
        qWarning()<<d->errors;
        d->db->end_transaction(false);

        for(Pair p : d->toadd) {
            QOrmMetaTable *t = p.first->metaTable();
            QOrmMetaAttribute *pka = t->listAttributes().first();
            pka->setValue(p.first, p.second);
        }

        //emit error(errors.join('\n'));
    }
    else {
        d->db->end_transaction(true);

        d->added.clear();

        for(Pair p : d->toadd) {
            d->db->append(p.first);
            QOrmLoader::reload(p.first);
        }

        d->errors.clear();
        d->toadd.clear();
        return true;
    }

    return false;
}

void QOrmSaver::clear()
{
    d->errors.clear();
    d->lastQuery.clear();
}

QString QOrmSaver::lastErrors()
{
    return d->errors.join('\n');
}

bool QOrmSaver::Private::save(QOrmSaver *saver, QOrmObject *o, QOrmMetaTable *t)
{
    saver->d->added.append(o);
    QSqlQuery q;
    if (!o->isSaved())
    {
        q.prepare(t->sqlInsert());
        for(QOrmMetaAttribute *a : t->listAttributes()) {
            if (a->isIndex()) a->modified(o);
        }
    }
    else  {
        if (o->isDirt())  q.prepare(t->sqlUpdate());
        else return true;
    }

    if (!execQuery(&q, t,o)) {
        saver->d->db->newError("saver error", q.lastError().text());
        saver->d->db->newError("saver query", q.lastQuery());

        saver->d->errors.append(q.lastError().text());
        saver->d->errors.append(q.lastQuery());
        return false;
    }

    if (o->needPrimaryKey() && q.numRowsAffected() > 0) {
        q.exec("SELECT LAST_INSERT_ID()");
        if (q.next())
        {
            QOrmMetaAttribute *pk = t->listAttributes().first();
            saver->d->toadd.append(Pair(o, o->primaryKey()));
            pk->setValue(o, q.value(0));
        }
    }

    return true;
}

bool QOrmSaver::Private::pushAll(QOrmSaver *saver, QOrmObject *obj, QOrmMetaTable *meta) {
    if (saver->d->added.contains(obj)) return true;

    for(QOrmMetaForeignKey *fka : meta->listForeignkeys()) {
        QQmlProperty p(obj, fka->property().toLatin1(), qmlEngine(obj));
        QOrmObject *o = p.read().value<QOrmObject*>();
        if (!o) continue;

        if (!pushAll(saver, o, o->metaTable())) return false;
    }

    if (!save(saver ,obj, obj->metaTable())) return false;

    for(QOrmMetaRelation *rel : meta->listRelations()) {
        foreach(auto o, rel->toList(obj))
            //if (!save(saver, o, o->metaTable())) return false;
            if (!pushAll(saver, o, o->metaTable())) return false;
    }

    return true;
}
