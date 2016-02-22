#ifndef ORMLOADER_H
#define ORMLOADER_H

#include <qormobject.h>

class QOrmLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlComponent* component READ component WRITE setComponent NOTIFY componentChanged)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QQmlListProperty<QOrmObject> result READ result NOTIFY resultChanged)
    Q_PROPERTY(QVariantMap bindValues READ bindValues WRITE setBindValues NOTIFY bindValuesChanged)
public:
    explicit QOrmLoader(QObject *parent = 0);

    QOrmMetaTable *table() const;

    QQmlComponent *component() const;
    void setComponent(QQmlComponent *value);

    QString query() const;
    void setQuery(QString value);

    QVariantMap bindValues() const;
    void setBindValues(QVariantMap value);

    QList<QOrmObject *> listResult() const;

private:
    QQmlListProperty<QOrmObject> result();

signals:
    void componentChanged(QQmlComponent *value);
    void queryChanged(QString value);
    void resultChanged();
    void bindValuesChanged(QVariantMap value);
    void error(QString error);

public slots:
    void load();
    void clearResult();
    void clearBindValues();
    void addBindValue(QString name, QVariant value);
    void addBindObject(QOrmObject *obj);

private:
    QOrmMetaTable *m_meta;
    QQmlComponent *m_component;
    QString m_query;
    QList<QOrmObject *> m_result;
    QVariantMap m_bindvalues;
};

#endif // ORMLOADER_H
