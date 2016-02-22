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
    Q_PROPERTY(QOrmMetaTable* table READ table WRITE setTable NOTIFY tableChanged)
    Q_PROPERTY(bool saved READ isSaved NOTIFY savedChanged)
    Q_PROPERTY(bool deleted READ isDeleted NOTIFY deletedChanged)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)
    Q_PROPERTY(bool dirt READ isDirt NOTIFY dirtChanged)
    Q_PROPERTY(QVariant primaryKey READ primaryKey NOTIFY primaryKeyChanged)
    Q_PROPERTY(QVariantList indexes READ indexes NOTIFY indexesChanged)
    Q_CLASSINFO("DefaultProperty", "table")
public:
    explicit QOrmObject();
    virtual ~QOrmObject();

    Q_INVOKABLE void load(QVariant pk);
    Q_INVOKABLE void save();
    Q_INVOKABLE void clear();

    QOrmMetaTable *table() const;
    void setTable(QOrmMetaTable *value);

    bool isSaved() const;
    bool isDeleted() const;
    bool isEmpty() const;
    bool isDirt() const;

    QVariant primaryKey();
    QVariantList indexes();

    void loadChildren();
    void loadForeignKeys();
    void loadAllForeignKeys();

signals:
    void afterLoad();
    void beforeLoad();
    void loaded(bool status);
    void saved();
    void tableChanged(QOrmMetaTable *value);
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
};

QML_DECLARE_TYPE(QOrmObject)

#endif // QORMOBJECT_H
