#ifndef QORMMETAFOREIGNKEY_H
#define QORMMETAFOREIGNKEY_H

#include <QtCore>
#include "qormmetaattribute.h"

class QOrmMetaForeignKey : public QOrmMetaAttribute
{
    Q_OBJECT
    Q_PROPERTY(QString foreignTable READ foreignTable WRITE setForeignTable)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
public:
    QOrmMetaForeignKey();
    virtual ~QOrmMetaForeignKey();

    bool load(QOrmObject *obj);
    bool isForeingkey() const;
    void setValue(QOrmObject *obj, QVariant v);
    void modified(QOrmObject *obj);

    QString foreignTable() const;
    void setForeignTable(QString value);

    QString query() const;
    void setQuery(QString value);

    void setForeignKeyMetaTable(QOrmMetaTable *mt);
    QOrmMetaTable *foreignKeyMetaTable() const;

signals:
    void queryChanged(QString value);

private:
    class Private;
    Private *d;
};


Q_DECLARE_METATYPE(QOrmMetaForeignKey*)

#endif // QORMMETAFOREIGNKEY_H
