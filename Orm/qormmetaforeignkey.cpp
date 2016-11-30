#include "qormmetaforeignkey.h"
#include "qormmetatable.h"

#include <QQmlProperty>

class QOrmMetaForeignKey::Private
{
public:
    QString foreigntable;
    QOrmMetaTable *fktable;
    QOrmLoader ld;

    Private() : fktable(0) {
    }
};

QOrmMetaForeignKey::QOrmMetaForeignKey() :
    QOrmMetaAttribute(),
    d(new Private)
{
    connect(&d->ld, &QOrmLoader::queryChanged, this, &QOrmMetaForeignKey::queryChanged);
}

QOrmMetaForeignKey::~QOrmMetaForeignKey()
{
    delete d;
}

bool QOrmMetaForeignKey::load(QOrmObject *obj)
{
    //if (!obj || !d->fktable) return false;

    //QVariant v = obj->indexes().at(index());
    QQmlProperty p(obj, property(), qmlContext(obj));
    if (p.propertyTypeCategory() != QQmlProperty::Object) return false;

    //QOrmObject *ofk = d->ld.loadOne(v);
    QOrmObject *ofk = 0;
    d->ld.addBindObject(obj);

    auto lst  = d->ld.loadAll();
    if (!lst.isEmpty()) ofk = lst.first(); //*/
    if (!ofk) return false;

    return p.write(QVariant::fromValue(ofk));
}

bool QOrmMetaForeignKey::isForeingkey() const
{
    return true;
}

void QOrmMetaForeignKey::setValue(QOrmObject *obj, QVariant v)
{
    if (index()>= 0) obj->setIndexValue(index(), v);
}

void QOrmMetaForeignKey::modified(QOrmObject *obj)
{
    QQmlProperty p(obj, property(), qmlContext(obj));
    QOrmObject *fko = p.read().value<QOrmObject*>();
    QVariant nv;
    if (fko) nv = fko->primaryKey();

    if (index()>= 0) obj->setIndexValue(index(), nv);
}

QString QOrmMetaForeignKey::foreignTable() const
{
    return d->foreigntable;
}

void QOrmMetaForeignKey::setForeignTable(QString value)
{
    if (!d->foreigntable.isEmpty()) return;

    d->foreigntable = value;
}

QString QOrmMetaForeignKey::query() const
{
    return d->ld.query();
}

void QOrmMetaForeignKey::setQuery(QString value)
{
    d->ld.setQuery(value);
}

void QOrmMetaForeignKey::setForeignKeyMetaTable(QOrmMetaTable *mt)
{
    if (d->fktable) return;

    d->fktable = mt;

    auto fka = d->fktable->listAttributes().first();
    if (d->ld.query().isEmpty())
        d->ld.setQuery(QString("%1 WHERE `%2` = :%3 LIMIT 1")
                       .arg(d->fktable->sqlSelect())
                       .arg(fka->attribute())
                       .arg(attribute()));

    d->ld.setTable(d->fktable);
}

QOrmMetaTable *QOrmMetaForeignKey::foreignKeyMetaTable() const
{
    return d->fktable;
}

