#ifndef QORMMETATABLE_P_H
#define QORMMETATABLE_P_H

class QOrmMetaTable::Private
{
public:
    QOrm* database;
    QString table;
    QString databaseName;
    bool initialized;
    int indexcount;
    QString sqlSelect;
    QString sqlInsert;
    QString sqlDelete;
    QString sqlUpdate;

    QList<QOrmMetaAttribute*> attrs;
    QList<QOrmMetaForeignKey*> fks;
    QList<QOrmMetaRelation*> rels;

    QQmlComponent* componenet;
};

#endif // QORMMETATABLE_P_H

