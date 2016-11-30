#ifndef QORMABSTRACTMODEL_H
#define QORMABSTRACTMODEL_H

#include <QObject>
#include <qqml.h>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

class QOrmMetaTable;
class QOrmModelPrivate;
class QOrmLoader;
class QOrmObject;

class QOrmModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QOrmMetaTable* metaTable READ metaTable WRITE setMetaTable NOTIFY metaTableChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QOrmLoader* loader READ loader)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(int pages READ pages NOTIFY pagesChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
public:
    explicit QOrmModel(QObject *parent = 0);
    virtual ~QOrmModel();

    QOrmMetaTable *metaTable() const;
    void setMetaTable(QOrmMetaTable* value);
    QOrmLoader *loader() const;
    int count() const;

    int limit() const;
    void setLimit(int value);

    int currentPage() const;
    void setCurrentPage(int value);

    int pages() const;

    Q_INVOKABLE bool isValid();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData (int section, Qt::Orientation orientation, int role ) const;

    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    Q_INVOKABLE bool deleteRows(int row, int count, const QModelIndex &parent = QModelIndex());
    inline bool deleteRow(int row, const QModelIndex &parent = QModelIndex());

    Q_INVOKABLE virtual void load();
    Q_INVOKABLE void commit();
    Q_INVOKABLE virtual void revert(); //or reload

    Q_INVOKABLE QOrmObject* at(int row) const;
    Q_INVOKABLE void append(QOrmObject *obj);
    Q_INVOKABLE void remove(QOrmObject *obj);
    Q_INVOKABLE void deleteOne(QOrmObject *obj);
    Q_INVOKABLE void clear();

    QHash<int, QByteArray> roleNames() const;

protected slots:
    void objectDataChanged();

protected:
    void setList(QList<QOrmObject*> lst);
    QList<QOrmObject*> getList();

signals:
    void metaTableChanged(QOrmMetaTable* metaTable);
    void countChanged(int count);
    void limitChanged(int value);
    void pagesChanged(int value);
    void currentPageChanged(int value);

private:
    void calcPages();

    QOrmModelPrivate *d;
};

inline bool QOrmModel::deleteRow(int row, const QModelIndex &parent) { return deleteRows(row, 1, parent); }

class QOrmFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterProperty READ filterProperty WRITE setFilterProperty NOTIFY filterPropertyChanged)
public:
    explicit QOrmFilterModel(QObject *parent = 0);

    Q_INVOKABLE QOrmObject *at(int row);

    QString filterProperty() const;
    void setFilterProperty(QString value);

protected:
    void updateFilterRole();

signals:
    void filterPropertyChanged(QString value);

private:
    QString m_filterproperty;
};


class QOrmRelationModel : public QOrmModel
{
    Q_OBJECT
    Q_PROPERTY(QOrmObject* rootObject READ rootObject WRITE setRootObject NOTIFY rootObjectChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(bool autoSumbit READ autoSumbit WRITE setAutoSumbit NOTIFY autoSumbitChanged)
public:
    explicit QOrmRelationModel(QObject *parent = 0);

    QString property() const;
    void setProperty(QString value);
    QOrmObject *rootObject() const;
    void setRootObject(QOrmObject* value);
    bool autoSumbit() const;
    void setAutoSumbit(bool value);

public slots:
    bool submit();
    void update();

signals:
    void rootObjectChanged(QOrmObject* root);
    void propertyChanged(QString propertyName);
    void autoSumbitChanged(bool value);

private:
    QString m_property;
    QOrmObject* m_rootObject;
    bool m_autosumbit;
};

Q_DECLARE_METATYPE(QOrmModel*)
Q_DECLARE_METATYPE(QOrmFilterModel*)

#endif // QORMABSTRACTMODEL_H
