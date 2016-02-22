#include "qormqueryparser.h"

#include "../qormmetaattribute.h"

QString QOrmSelectParser::query(QOrmMetaTable *meta)
{
    QStringList attr;
    for(QOrmMetaAttribute *a : meta->listAttributes())
        attr << "`" + a->attribute() + "`";

    return QString("SELECT %3 FROM `%1`.`%2` ").arg(meta->database())
             .arg(meta->table())
             .arg(attr.join(", "));
}


QString QOrmInsertParser::query(QOrmMetaTable *meta)
{
    QStringList attr, val;
    for(QOrmMetaAttribute *a: meta->listAttributes())
    {
        //if (!meta->attribute(i).isWriteable()) continue;

        attr << QString("`%1`").arg(a->attribute());
        val << QString(":%1").arg(a->attribute().replace(" ", "_"));
    }

    return QString("INSERT INTO `%1`.`%2` (%3) VALUES (%4); ")
            .arg(meta->database())
            .arg(meta->table())
            .arg(attr.join(", "))
            .arg(val.join(", "));
}


QString QOrmUpdateParser::query(QOrmMetaTable *meta)
{
    QStringList set, whe;
    for(QOrmMetaAttribute *a : meta->listAttributes())
    {
        //if (!a.isWriteable()) continue;

        QString v = QString("`%1` = :%2")
               .arg(a->attribute())
               .arg(a->attribute().replace(" ", "_"));

        if (a->isIndex() && !a->isForeingkey()) whe << v;
        else set << v;
    }

    return QString("UPDATE `%1`.`%2` SET %3 WHERE %4; ")
            .arg(meta->database())
            .arg(meta->table())
            .arg(set.join(", "))
            .arg(whe.join(" AND "));
}


QString QOrmDeleteParser::query(QOrmMetaTable *meta)
{
    QOrmMetaAttribute* a = meta->listAttributes().at(0);
    QString ret = "DELETE FROM `%1`.`%2` WHERE `%3` = :%4; ";
    ret = ret.arg(meta->database());
    ret = ret.arg(meta->table());
    ret = ret.arg(a->attribute());
    ret = ret.arg(a->attribute().replace(" ", "_"));
    return ret;
}
