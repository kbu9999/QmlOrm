#include "qormloader.h"
#include "qormmetaattribute.h"
#include "qormmetatable.h"
#include "qorm.h"

#include <QtSql>

static void loadAttr(QOrmMetaTable *meta, QOrmObject *obj, QSqlQuery query, QSqlRecord row)
{
    for(QOrmMetaAttribute *a : meta->listAttributes()) {
        QVariant v = query.value(row.indexOf(a->attribute()));
        a->setValue(obj, v);
    }
}

static QList<QOrmObject*> fromQuery(QOrmLoader *ld, QSqlQuery q)
{
    QList<QOrmObject*> lst;
    QSqlRecord row = q.record();
    QOrmObject* obj = 0;

    while(q.next())
    {
        QVariant pk = q.value(0);
        obj = QOrm::defaultOrm()->find(ld->table(), pk);
        if(!obj)
            obj =  (QOrmObject*)ld->component()->create();

        obj->beforeLoad();
        obj->blockSignals(true); 

        //TODO conectar señal loaded a cada propiedad de obj
        loadAttr(ld->table(), obj, q, row);

        obj->blockSignals(false);
        lst.append(obj);
    }

    return lst;
}

QOrmLoader::QOrmLoader(QObject *parent) : QObject(parent),
    m_meta(NULL), m_component(NULL)
{
}

QOrmMetaTable *QOrmLoader::table() const
{
    return m_meta;
}

QQmlComponent *QOrmLoader::component() const
{
    return m_component;
}

void QOrmLoader::setComponent(QQmlComponent *value)
{
    if (m_component == value) return;

    QObject* o = value->create();
    QOrmObject *obj = qobject_cast<QOrmObject*>(o);
    if (!obj) {
        qDebug()<<"the Component not conatains a OrmObject";
        delete o;
        return;
    }
    if (!obj->table()) {
        qDebug()<<"The OrmObject not have a valid MetaTable";
        delete o;
        return;
    }

    m_meta = obj->table();
    m_component = value;
    emit componentChanged(m_component);
    delete o;

    setQuery(m_meta->sqlSelect());
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

QList<QOrmObject *> QOrmLoader::listResult() const
{
    return m_result;
}

QQmlListProperty<QOrmObject> QOrmLoader::result()
{
    return QQmlListProperty<QOrmObject>(this, m_result);
}

void QOrmLoader::load()
{
    if (!m_meta || !m_component) return;
    if (m_query.isEmpty()) setQuery(m_meta->sqlSelect());

    QSqlQuery q;
    q.prepare(m_meta->sqlSelect());

    for (auto i = m_bindvalues.constBegin(); i != m_bindvalues.constEnd(); ++i) {
        q.bindValue(i.key(), i.value());
    }

    if (!q.exec()) {
        emit error("sql: "+q.lastError().text());
        return;
    }

    if (!q.next()) {
        emit error("sql: no results");
        return;
    }

    m_result = fromQuery(this, q);
    for(QOrmObject *o : m_result)
    {
        if (o->isSaved()) continue;

        o->loadAllForeignKeys();
        o->afterLoad();
        o->setAsLoaded();

        QOrm::defaultOrm()->append(o);
    }

    emit resultChanged();
}

void QOrmLoader::clearResult()
{
    m_result.clear();

    emit resultChanged();
}

void QOrmLoader::clearBindValues()
{
    m_bindvalues.clear();
}

void QOrmLoader::addBindValue(QString name, QVariant value)
{
    m_bindvalues[name] = value;
}

void QOrmLoader::addBindObject(QOrmObject *obj)
{
    if (!obj) return;

    QOrmMetaTable *m = obj->table();
    for(QOrmMetaAttribute *attr : m->listAttributes()) {
        QString e = ":" + m->table() + "_" + attr->attribute().replace(" ", "_");
        auto i = m_bindvalues.find(e);
        if (i == m_bindvalues.end())
            i = m_bindvalues.find(":" + attr->attribute().replace(" ", "_"));

        if (i != m_bindvalues.end())  {
            const QVariant v = obj->property(attr->property().toLatin1());
            i->setValue(v);
        }
    }
}

