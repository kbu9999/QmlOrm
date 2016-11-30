#include "qormloader.h"
#include "qormmetaattribute.h"
#include "qormmetatable.h"
#include "qorm.h"

#include <QtSql>

static void loadAttr(QOrmMetaTable *meta, QOrmObject *obj, QSqlQuery query, QSqlRecord row)
{
    for(QOrmMetaAttribute *a : meta->listAttributes()) {
        a->blockOnLoad();
        auto i = row.indexOf(a->attribute());
        if (i >= 0) {
            QVariant v = query.value(i);

            if (v.type() == QVariant::ByteArray) {
                a->setValue(obj, v.toByteArray().toBase64());
            }
            else
                a->setValue(obj, v);
        }
        a->unBlockOnLoad();
    }
}

static QList<QOrmObject*> fromQuery (QOrmLoader *ld, QSqlQuery q, QObject *parent)
{
    QList<QOrmObject*> lst;
    QSqlRecord row = q.record();
    QOrmObject* obj = 0;

    while(q.next())
    {
        QVariant pk = q.value(0);
        obj = ld->table()->find(pk);
        if(!obj) {
            obj = (QOrmObject*)ld->table()->create(parent);
            //obj->setIndexCount(ld->table()->indexCount());
        }
        if (!obj) {
            qDebug()<<"no create obj";
            continue;
        }

        if (obj->isSaved()) {
            lst.append(obj);
            continue;
        }

        obj->beforeLoad();

        //TODO conectar señal loaded a cada propiedad de obj
        // no me acuerdo para que
        loadAttr(ld->table(), obj, q, row);

        lst.append(obj);
        ld->database()->append(obj);
    }

    return lst;
}

QOrmLoader::QOrmLoader(QObject *parent) : QObject(parent),
    m_table(NULL), m_loadforeignkeys(false)
{
}

QOrmLoader::~QOrmLoader()
{
}

QOrm *QOrmLoader::database() const
{
    if (!m_table) return NULL;

    return m_table->database();
}

bool QOrmLoader::isValid() const
{
    if (!m_table) return false;

    return m_table->database();// && m_table->component();
}

QOrmMetaTable *QOrmLoader::table() const
{
    return m_table;
}

void QOrmLoader::setTable(QOrmMetaTable *value)
{
    if (m_table) return;

    m_table = value;
    emit tableChanged(m_table);

    if (m_query.isEmpty())
        setQuery(m_table->sqlSelect());
}

QString QOrmLoader::query() const
{
    return m_query;
}

void QOrmLoader::setQuery(QString value)
{
    if (m_query == value) return;

    m_query = value;
    QRegularExpression re("(:\\w+)");
    QRegularExpressionMatchIterator i = re.globalMatch(m_query);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        m_bindvalues.insert(match.captured(1), QVariant());
    }

    emit queryChanged(m_query);
}

bool QOrmLoader::loadForeignkeys() const
{
    return m_loadforeignkeys;
}

void QOrmLoader::setLoadForeignkeys(bool value)
{
    if (m_loadforeignkeys == value) return;
    m_loadforeignkeys = value;
    emit loadForeignkeysChanged(m_loadforeignkeys);
}

QVariantMap QOrmLoader::bindValues() const
{
    return m_bindvalues;
}

void QOrmLoader::setBindValues(QVariantMap value)
{
    if (m_bindvalues == value) return;
    m_bindvalues = value;
    emit bindValuesChanged(m_bindvalues);
}

QOrmObject *QOrmLoader::loadOne(QVariant pk)
{
    QOrmObject *r = m_table->find(pk);
    if (!r) {
        r = (QOrmObject*)m_table->create(this);
        r->setIndexValue(0, pk);
        reload(r);
        if (r->isEmpty()) {
            delete r;
            return NULL;
        }
    }
    return r;
}

QVariantList QOrmLoader::load() {
    QVariantList lst;
    for(auto o : loadAll()) lst.append(QVariant::fromValue(o));
    return lst;
}

QList<QOrmObject *> QOrmLoader::loadAll(int limit, int offset)
{
    QList<QOrmObject *> result;

    if (!isValid()) return result;
    if (m_query.isEmpty()) setQuery(m_table->sqlSelect());

    QSqlQuery q;
    if (limit <= 0) q.prepare(m_query);
    else if (offset <= 0) q.prepare(m_query+QString(" LIMIT %2,%1").arg(limit).arg(offset));
    else q.prepare(m_query+QString("LIMIT %1").arg(limit));

    for (auto i = m_bindvalues.constBegin(); i != m_bindvalues.constEnd(); ++i)
        q.bindValue(i.key(), i.value());

    if (!q.exec()) {
        emit error("sql: "+q.lastError().text());
        m_table->database()->newError("loader-error", q.lastError().text());
        m_table->database()->newError("loader-query", q.lastQuery());
        return result;
    }

    //result = fromQuery(this, q, parent()? parent() : this);
    result = fromQuery(this, q, this);
    //qDebug()<<q.lastQuery()<<m_bindvalues;
    for(QOrmObject *o : result)
    {
        if (m_loadforeignkeys)
            o->loadAllForeignKeys();

        if (o->isSaved()) continue;

        o->afterLoad();
        o->setAsLoaded();

        o->loaded(true);
    }

    if (result.isEmpty()) {
        emit error("sql: no results");
        //m_table->database()->newError("loader warning", "no results");
    }

    return result;
}

bool QOrmLoader::reload(QOrmObject *o)
{
    if (!o ) return false;
    if (!o->metaTable()) return false;
    if (!o->metaTable()->database()) return false;

    QOrmMetaAttribute *pka = o->metaTable()->listAttributes().at(0);
    QString query = QString("%1 WHERE `%2` = ? LIMIT 1")
                .arg(o->metaTable()->sqlSelect())
                .arg(pka->attribute());

    QSqlQuery q;
    q.prepare(query);
    q.addBindValue(o->primaryKey());
    if (!q.exec()) return false;
    if (!q.next()) return false;

    loadAttr(o->metaTable(), o, q, q.record());

    o->setAsLoaded();
    //o->loaded(true);

    return true;
}

void QOrmLoader::clearBindValues()
{
    m_bindvalues.clear();

    emit bindValuesChanged(m_bindvalues);
}

void QOrmLoader::addBindValue(QString name, QVariant value)
{
    m_bindvalues[name] = value;
}

void QOrmLoader::addBindObject(QOrmObject *obj)
{
    if (!obj) return;

    QOrmMetaTable *m = obj->metaTable();
    if (!m) return;

    for(QOrmMetaAttribute *attr : m->listAttributes()) {
        QString e = ":" + m->table() + "_" + attr->attribute().replace(" ", "_");
        auto i = m_bindvalues.find(e);
        if (i == m_bindvalues.end())
            i = m_bindvalues.find(":" + attr->attribute().replace(" ", "_"));

        if (i != m_bindvalues.end())  {
            QVariant v;
            if (attr->isForeingkey()) v = obj->indexes().at(attr->index());
            else v = obj->property(attr->property().toLatin1());
            i->setValue(v);
        }
    }
}

