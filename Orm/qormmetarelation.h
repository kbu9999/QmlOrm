#ifndef QORMMETARELATION_H
#define QORMMETARELATION_H

#include <QtCore>
#include "qormloader.h"

class QOrmObject;
class QOrmMetaForeignKey;

class QOrmMetaRelation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString relationTableName READ relationTableName WRITE setChildrenTableName NOTIFY childrenTableNameChanged)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
public:
    explicit QOrmMetaRelation(QObject *parent = 0);

    QString relationTableName() const;
    void setChildrenTableName(QString value);

    QString query() const;
    void setQuery(QString value);

    QString property() const;
    void setProperty(QString value);

    void setRelationMetaTable(QOrmMetaTable *rmt, QOrmMetaForeignKey *fka);
    QOrmMetaTable *relationMetaTable() const;

    QOrmObject *at(QOrmObject *obj, int index);
    void append(QOrmObject *obj, QOrmObject *add);
    void remove(QOrmObject *obj, int index);
    QList<QOrmObject *> toList(QOrmObject *obj);

signals:
    void childrenTableNameChanged(QString value);
    void queryChanged(QString value);
    void propertyChanged(QString value);

public slots:
    void load(QOrmObject *obj);

private:
    QString m_relationtablename;
    QString m_property;
    QOrmMetaTable *m_reltable;
    QOrmMetaForeignKey *m_fka;
    QOrmLoader ld;
};

#endif // QORMMETARELATION_H
