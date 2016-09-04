#ifndef QORMOBJECT_H
#define QORMOBJECT_H

#include <QObject>
#include <QVariant>
#include <QQmlComponent>
#include <qqml.h>
#include <QDebug>

class QOrmMetaTable;

class QOrmObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QOrmMetaTable* metaTable READ metaTable WRITE setMetaTable NOTIFY metaTableChanged)
    Q_PROPERTY(bool saved READ isSaved NOTIFY savedChanged)
    Q_PROPERTY(bool deleted READ isDeleted NOTIFY deletedChanged)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)
    Q_PROPERTY(bool dirt READ isDirt NOTIFY dirtChanged)
    Q_PROPERTY(QVariant primaryKey READ primaryKey NOTIFY primaryKeyChanged)
    Q_PROPERTY(QVariantList indexes READ indexes NOTIFY indexesChanged)
    Q_CLASSINFO("DefaultProperty", "tableName")
public:
    explicit QOrmObject(QObject *parent = 0);
    virtual ~QOrmObject();

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool save();
    Q_INVOKABLE bool reload();

    bool isSaved() const;
    bool isDeleted() const;
    bool isEmpty() const;
    bool isDirt() const;

    bool needPrimaryKey();
    QVariant primaryKey();
    QVariantList indexes();
    void setIndexValue(int i, QVariant v);

    void setMetaTable(QOrmMetaTable *mt);
    QOrmMetaTable *metaTable() const;

    Q_INVOKABLE void loadRelation(QOrmMetaTable *meta);
    Q_INVOKABLE void append(QOrmMetaTable *meta, QOrmObject *obj);
    Q_INVOKABLE void remove(QOrmMetaTable *meta, int index);

    Q_INVOKABLE bool loadForeignKey(QOrmMetaTable *meta);
    Q_INVOKABLE void loadAllForeignKeys();
    Q_INVOKABLE void deleteLater();

signals:
    void afterLoad();
    void beforeLoad();
    void loaded(bool status);
    void saved();
    void metaTableChanged(QOrmMetaTable* value);
    void tableNameChanged(QString value);
    void savedChanged(bool value);
    void deletedChanged(bool value);
    void emptyChanged(bool value);
    void dirtChanged(bool value);
    void primaryKeyChanged(QVariant value);
    void indexesChanged();

private:
    void setAsLoaded();
    void setAsDeleted();
    void modified();

private:
    class Private;
    Private *d;

    friend class QOrmMetaAttribute;
    friend class QOrmLoader;
    friend class QOrmSaver;
};

QML_DECLARE_TYPE(QOrmObject)

#endif // QORMOBJECT_H
