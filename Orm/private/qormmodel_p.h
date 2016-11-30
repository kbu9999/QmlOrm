#ifndef QORMABSTRACTMODELPRIVATE_H
#define QORMABSTRACTMODELPRIVATE_H

#include <QQmlListReference>

class QOrmModelPrivate
{
public:
    QHash<int, QByteArray> roles;
    QList<QOrmObject*> list;
    QList<QOrmObject*> deleted;
    int limit, pages, currentpage;


    QOrmLoader *ld;
    QOrmMetaTable* metatable;
};

#endif // QORMABSTRACTMODELPRIVATE_H
