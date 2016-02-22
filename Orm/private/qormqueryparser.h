#ifndef QORMQUERYPARSER_H
#define QORMQUERYPARSER_H

#include <QtCore>
#include "../qormmetatable.h"

class QOrmQueryParser
{
public:
    virtual QString query(QOrmMetaTable *meta) = 0;
};


class QOrmSelectParser : public QOrmQueryParser
{
public:
    QString query(QOrmMetaTable *meta);
};

class QOrmInsertParser : public QOrmQueryParser
{
public:
    QString query(QOrmMetaTable *meta);
};

class QOrmUpdateParser : public QOrmQueryParser
{
public:
    QString query(QOrmMetaTable *meta);
};

class QOrmDeleteParser : public QOrmQueryParser
{
public:
    QString query(QOrmMetaTable *meta);
};


#endif // QORMQUERYPARSER_H
