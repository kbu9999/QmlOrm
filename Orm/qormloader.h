#ifndef ORMLOADER_H
#define ORMLOADER_H

#include <qormobject.h>

class QOrm;

class QOrmLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QOrm* database READ database NOTIFY databaseChanged)
    Q_PROPERTY(QOrmMetaTable* table READ table WRITE setTable NOTIFY tableChanged)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QVariantMap bindValues READ bindValues WRITE setBindValues NOTIFY bindValuesChanged)
    Q_PROPERTY(bool loadForeignKeys READ loadForeignkeys WRITE setLoadForeignkeys NOTIFY loadForeignkeysChanged)
public:
    explicit QOrmLoader(QObject *parent = 0);
    virtual ~QOrmLoader();

    QOrm *database() const;

    bool isValid() const;

    QOrmMetaTable *table() const;
    void setTable(QOrmMetaTable* value);

    QString query() const;
    void setQuery(QString value);

    bool loadForeignkeys() const;
    void setLoadForeignkeys(bool value);

    QVariantMap bindValues() const;
    void setBindValues(QVariantMap value);

    Q_INVOKABLE QVariantList load();
    QList<QOrmObject *> loadAll(int limit = 0, int offset = 0);
    static bool reload(QOrmObject *ld);

signals:
    void databaseChanged(QOrm* db);
    void tableChanged(QOrmMetaTable* table);
    void queryChanged(QString query);
    void bindValuesChanged(QVariantMap values);
    void loadForeignkeysChanged(bool value);
    void error(QString error);

public slots:
    QOrmObject *loadOne(QVariant pk);
    void clearBindValues();
    void addBindValue(QString name, QVariant value);
    void addBindObject(QOrmObject *obj);

private:
    QOrmMetaTable *m_table;
    QString m_query;
    QVariantMap m_bindvalues;
    bool m_loadforeignkeys;
};

#endif // ORMLOADER_H
