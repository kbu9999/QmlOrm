#ifndef QORMTABLEINFO_H
#define QORMTABLEINFO_H

#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <QQmlListProperty>
#include <QQmlComponent>

class QOrmObject;

class QOrmMetaAttribute;
class QOrmMetaForeignKey;
class QOrmMetaRelation;

class QOrmCache : public QObject
{
private:
    QOrmObject *find(QVariant pk);

    void append(QOrmObject *obj);
    void remove(QObject *obj);

    QMap<QVariant, QOrmObject*> m_cache;

    friend class QOrm;
};

class QOrmMetaTable : public QOrmCache, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString table READ table WRITE setTable NOTIFY tableChanged)
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QQmlListProperty<QOrmMetaAttribute> attributes READ attributes NOTIFY attributesChanged)
    //Q_PROPERTY(QQmlListProperty<QOrmMetaForeignKey> foreignkeys READ foreignkeys)
    //Q_PROPERTY(QQmlListProperty<QOrmMetaRelation> relations READ relations NOTIFY relationsChanged)
    Q_PROPERTY(QString sqlSelect READ sqlSelect WRITE setSqlSelect NOTIFY sqlSelectChanged)
    Q_PROPERTY(QString sqlInsert READ sqlInsert WRITE setSqlInsert NOTIFY sqlInsertChanged)
    Q_PROPERTY(QString sqlDelete READ sqlDelete WRITE setSqlDelete NOTIFY sqlDeleteChanged)
    Q_PROPERTY(QString sqlUpdate READ sqlUpdate WRITE setSqlUpdate NOTIFY sqlUpdateChanged)
    Q_CLASSINFO("DefaultProperty", "attributes")
public:
    QOrmMetaTable();
    virtual ~QOrmMetaTable();

    QString database() const;
    QString table() const;

    QString sqlSelect() const;
    QString sqlInsert() const;
    QString sqlUpdate() const;
    QString sqlDelete() const;

    QList<QOrmMetaAttribute *> listAttributes();
    QQmlListProperty<QOrmMetaAttribute>  attributes();
    QQmlListProperty<QOrmMetaForeignKey> foreignkeys();
    QQmlListProperty<QOrmMetaRelation> relations();

    void classBegin();
    void componentComplete();
    void connectAttributes(QOrmObject *obj);

public slots:
    void setTable(QString value);
    void setDatabase(QString value);
    void setSqlSelect(QString value);
    void setSqlInsert(QString value);
    void setSqlDelete(QString value);
    void setSqlUpdate(QString value);

signals:
    void tableChanged(QString);
    void databaseChanged(QString);
    void attributesChanged();
    void relationsChanged();
    void sqlSelectChanged(QString);
    void sqlInsertChanged(QString);
    void sqlDeleteChanged(QString);
    void sqlUpdateChanged(QString);

private:
    class Private;
    Private *d;
    friend class QOrm;
};

#endif // QORMTABLEINFO_H
