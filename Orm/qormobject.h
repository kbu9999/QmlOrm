#ifndef QORMOBJECT_H
#define QORMOBJECT_H

#include <QObject>
#include <QVariant>
#include <QQmlComponent>
#include <qqml.h>

class QOrmMetaTable;

class QOrmObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool saved READ isSaved NOTIFY savedChanged)
    Q_PROPERTY(bool deleted READ isDeleted NOTIFY deletedChanged)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)
    Q_PROPERTY(bool dirt READ isDirt NOTIFY dirtChanged)
    Q_PROPERTY(QVariant primaryKey READ primaryKey NOTIFY primaryKeyChanged)
    Q_PROPERTY(QVariantList indexes READ indexes NOTIFY indexesChanged)
    Q_PROPERTY(QQmlComponent* tableComponent READ tableComponent
               WRITE setTableComponent NOTIFY tableComponentChanged CONSTANT FINAL)
    Q_CLASSINFO("DefaultProperty", "table")
public:
    explicit QOrmObject();
    virtual ~QOrmObject();

    Q_INVOKABLE void load(QVariant pk);
    Q_INVOKABLE void save();
    Q_INVOKABLE void clear();

    bool isSaved() const;
    bool isDeleted() const;
    bool isEmpty() const;
    bool isDirt() const;

    QVariant primaryKey();
    QVariantList indexes();
    QQmlComponent *tableComponent();

    void setTableComponent(QQmlComponent *value);

    void loadChildren();
    void loadForeignKeys();
    void loadAllForeignKeys();

    static QOrmMetaTable *qmlAttachedProperties(QObject *o);

signals:
    void afterLoad();
    void beforeLoad();
    void loaded(bool status);
    void saved();
    void savedChanged(bool value);
    void deletedChanged(bool value);
    void emptyChanged(bool value);
    void dirtChanged(bool value);
    void primaryKeyChanged(QVariant value);
    void indexesChanged();
    void tableComponentChanged();

private:
    void setAsLoaded();
    void setAsDeleted();
    void modified();

private:
    class Private;
    Private *d;

    friend class QOrmAttributeInfo;
};

typedef QList<QOrmObject*> QOrmObjectList;

QML_DECLARE_TYPE(QOrmObject)
QML_DECLARE_TYPEINFO(QOrmObject, QML_HAS_ATTACHED_PROPERTIES)

#endif // QORMOBJECT_H
