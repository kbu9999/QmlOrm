#ifndef QORMTABLEINFO_H
#define QORMTABLEINFO_H

#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <QQmlListProperty>
#include <QQmlComponent>

class QOrm;
class QOrmObject;

class QOrmMetaAttribute;
class QOrmMetaForeignKey;
class QOrmMetaRelation;

class QOrmCache : public QObject
{
public:
    QOrmObject *find(QVariant pk);

protected:
    void append(QOrmObject *obj);
    void remove(QObject *obj);
    int count();

    QMap<QVariant, QOrmObject*> m_cache;
    QList<QOrmObject*> m_add;

    friend class QOrm;
};

class QOrmMetaTable : public QOrmCache, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QOrm* database READ database NOTIFY databaseChanged)
    Q_PROPERTY(QString table READ table WRITE setTable NOTIFY tableChanged)
    Q_PROPERTY(QString databaseName READ databaseName NOTIFY databaseChanged)
    Q_PROPERTY(bool initialized READ initialized NOTIFY initializedChanged)
    Q_PROPERTY(int indexCount READ indexCount NOTIFY indexCountChanged)
    Q_PROPERTY(QQmlListProperty<QOrmMetaAttribute> attributes READ attributes NOTIFY attributesChanged)
    //Q_PROPERTY(QQmlListProperty<QOrmMetaForeignKey> foreignkeys READ foreignkeys)
    Q_PROPERTY(QQmlListProperty<QOrmMetaRelation> relations READ relations NOTIFY relationsChanged)
    Q_PROPERTY(QString sqlSelect READ sqlSelect WRITE setSqlSelect NOTIFY sqlSelectChanged)
    Q_PROPERTY(QString sqlInsert READ sqlInsert WRITE setSqlInsert NOTIFY sqlInsertChanged)
    Q_PROPERTY(QString sqlDelete READ sqlDelete WRITE setSqlDelete NOTIFY sqlDeleteChanged)
    Q_PROPERTY(QString sqlUpdate READ sqlUpdate WRITE setSqlUpdate NOTIFY sqlUpdateChanged)
    //Q_PROPERTY(QQmlComponent* component READ component WRITE setComponenet NOTIFY componenetChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_CLASSINFO("DefaultProperty", "attributes")
public:
    QOrmMetaTable();
    virtual ~QOrmMetaTable();

    bool initialized() const;
    void classBegin();
    void componentComplete();

    QOrm *database() const;
    void setDatabase(QOrm* value);

    QString databaseName() const;
    void setDatabaseName(QString value);

    QString table() const;
    void setTable(QString value);

    Q_INVOKABLE int count();

protected:
    QQmlComponent *component() const;
    void setComponenet(QQmlComponent* value);

public:
    int indexCount() const;
    void setIndexCount(int count);

    QString sqlSelect() const;
    void setSqlSelect(QString value);

    QString sqlInsert() const;
    void setSqlInsert(QString value);

    QString sqlUpdate() const;
    void setSqlDelete(QString value);

    QString sqlDelete() const;
    void setSqlUpdate(QString value);

    QUrl url() const;
    void setUrl(QUrl value);

    QQmlListProperty<QOrmMetaAttribute>  attributes();
    QList<QOrmMetaAttribute *> listAttributes();

    QQmlListProperty<QOrmMetaForeignKey> foreignkeys();
    QList<QOrmMetaForeignKey *> listForeignkeys();

    QQmlListProperty<QOrmMetaRelation> relations();
    QList<QOrmMetaRelation *> listRelations();

    void connectAttributes(QOrmObject *obj);

public slots:
    QOrmObject *create(QObject *parent = NULL);

signals:
    void databaseChanged(QOrm* database);
    void tableChanged(QString table);
    void databaseNameChanged(QString databaseName);
    void attributesChanged();
    void relationsChanged();
    void sqlSelectChanged(QString select);
    void sqlInsertChanged(QString insert);
    void sqlDeleteChanged(QString deleted);
    void sqlUpdateChanged(QString update);
    void initializedChanged(bool init);
    void componenetChanged(QQmlComponent* component);
    void urlChanged(QUrl value);
    void indexCountChanged(int value);

private:
    class Private;
    Private *d;
    friend class QOrm;

    QUrl m_url;
};

#endif // QORMTABLEINFO_H
