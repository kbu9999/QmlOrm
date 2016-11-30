#include "qormmodel.h"
#include "qormloader.h"
#include "private/qormmodel_p.h"

#include "qorm.h"
#include "qormmetaattribute.h"
#include "qormmetaforeignkey.h"
#include "qormmetarelation.h"
#include "qormmetatable.h"
#include "qormsaver.h"

#include <QSqlQuery>

static void connectPropObject(QOrmObject *obj, QOrmModel *model, QList<QByteArray> props) {
    foreach(QString n, props) {
        QQmlProperty p(obj, n, qmlContext(obj));
        p.connectNotifySignal(model, SLOT(objectDataChanged()));
    }
}


QOrmModel::QOrmModel(QObject *parent) :
    QAbstractItemModel(parent),
    d(new QOrmModelPrivate())
{
    d->metatable = 0;
    d->ld = new QOrmLoader(this);
    d->limit = 0;
    d->pages = -1;
    d->currentpage = 0;

    connect(d->ld, &QOrmLoader::queryChanged, this, &QOrmModel::calcPages);
}

QOrmModel::~QOrmModel()
{
    clear();
    delete d;
}

QModelIndex QOrmModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (row >= 0 && row < d->list.count())  return createIndex(row, column, d->list.at(row));

    return QModelIndex();
}

QModelIndex QOrmModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int QOrmModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    //return d->metatable->listAttributes().count();
    return d->roles.count();
    return 0;
}

int QOrmModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return d->list.count();
}

QVariant QOrmModel::data(const QModelIndex &index, int role) const
{
    QOrmObject *dt = (QOrmObject*) index.internalPointer();
    if (index.isValid() && dt)
    {
        if (role > Qt::UserRole) {
            QQmlProperty prop(dt, d->roles.value(role), qmlContext(dt));
            return prop.read();
        }

        if (role == Qt::DisplayRole) {
            auto c = d->roles.values().at(index.column());
            QQmlProperty prop(dt, c, qmlContext(dt));
            return prop.read();
        }
    }

    return QVariant();
}
bool QOrmModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QOrmObject *dt = (QOrmObject*) index.internalPointer();
    if (index.isValid() && dt && role > Qt::UserRole)
    {
        if (role > Qt::UserRole) {
            QQmlProperty prop(dt, d->roles.value(role), qmlContext(dt));
            return prop.write(value);
        }

        if (role == Qt::DisplayRole) {
            auto c = d->roles.values().at(index.column());
            QQmlProperty prop(dt, c, qmlContext(dt));
            return prop.write(value);
        }
    }

    return false;
}
QVariant QOrmModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    if (section >= 0 && section < d->roles.count())
        return d->roles.value(Qt::UserRole + 1 + section);

    return QVariant();
}

bool QOrmModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(parent, row + 1, row + count);
    for(int i = 0; i < count; i++)
    {
        QObject* o = d->metatable->create(d->metatable);
        QOrmObject *oo = qobject_cast<QOrmObject*>(o);
        if (oo) {
            d->list.insert(row + i + 1, oo);
            connectPropObject(oo, this, d->roles.values());
        }
        else
            delete o;
    }
    endInsertRows();

    emit countChanged(rowCount());

    return true;
}

bool QOrmModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++) {
        QOrmObject *dt = d->list.at(row + i);
        if (!dt) continue;

        d->list.removeAt(row + i);
        dt->disconnect(this);
    }
    endRemoveRows();
    emit countChanged(d->list.count());
    return true;
}

bool QOrmModel::deleteRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++)
    {
        QOrmObject *dt = d->list.at(row);
        if (!dt) continue;

        d->list.removeOne(dt);
        d->deleted.append(dt);
        dt->disconnect(this);
    }
    endRemoveRows();
    emit countChanged(d->list.count());
    return true;
}

void QOrmModel::load()
{
    if (d->ld->query().isEmpty())
        d->ld->setQuery(d->metatable->sqlSelect());

    if (d->pages < 0) calcPages();
    if (d->limit < 0 || d->pages <= 0) setList(d->ld->loadAll());
    else setList(d->ld->loadAll(d->limit, d->limit * (d->currentpage - 1)));
}

void QOrmModel::commit()
{
    QOrmSaver s(d->metatable->database());
    s.start();
    for(auto i : d->list) s.Save(i);
    s.commit();
}

void QOrmModel::revert()
{
    load();
}

QOrmObject* QOrmModel::at(int row) const
{
    if (row >= 0 && row < d->list.count()) return d->list.at(row);

    return NULL;
}

void QOrmModel::append(QOrmObject* obj)
{
    if (!obj) return;
    if (d->list.contains(obj)) return;

    int c = d->list.count();

    beginInsertRows(QModelIndex(), c, c);
    d->list.append(obj);
    endInsertRows();

    connectPropObject(obj, this, d->roles.values());

    //emit dataChanged(index(c,0), index(c, 0));
    emit countChanged(d->list.count());
}

void QOrmModel::remove(QOrmObject* obj)
{
    if (!obj) return;
    int c = d->list.indexOf(obj);
    if (c < 0) return;

    beginRemoveRows(QModelIndex(), c, c);
    d->list.removeAt(c);
    obj->disconnect(this);
    endRemoveRows();

    emit countChanged(d->list.count());
}

void QOrmModel::deleteOne(QOrmObject* obj)
{
    if (!obj) return;
    if (!obj->isDeleted()) return;

    d->deleted.append(obj);
    obj->disconnect(this);

    emit countChanged(d->list.count());
}

void QOrmModel::clear()
{
    d->ld->clearBindValues();

    foreach(QOrmObject *obj, d->list) obj->disconnect(this);
    foreach(QOrmObject *obj, d->deleted) obj->disconnect(this);

    d->list.clear();
    d->deleted.clear();

    beginResetModel();
    endResetModel();

    emit countChanged(0);
}

QHash<int, QByteArray> QOrmModel::roleNames() const
{
    //qDebug()<<d->roles;
    return d->roles;
}

void QOrmModel::objectDataChanged()
{
    int c = d->list.indexOf((QOrmObject*)sender());
    if (c < 0) return;

    emit dataChanged(index(c,0), index(c, 0));
}

void QOrmModel::setList(QList<QOrmObject *> lst)
{
    beginResetModel();
    foreach(QOrmObject *obj, d->list) obj->disconnect(this);
    foreach(QOrmObject *obj, lst) connectPropObject(obj, this, d->roles.values());
    d->list = lst;

    endResetModel();

    emit countChanged(d->list.count());
}

QList<QOrmObject *> QOrmModel::getList()
{
    return d->list;
}

void QOrmModel::calcPages()
{
    if (d->limit <= 0) return;
    if (!d->metatable) return;

    QString str = d->ld->query();
    if (str.contains("LIMIT"))
        str = str.remove(QRegularExpression("LIMIT.*"));
    if (str.contains("WHERE"))
        str.replace(QRegularExpression("SELECT(.*)FROM(.+)WHERE(.*)"), "SELECT COUNT(*) FROM \\2 WHERE \\3");
    else
        str.replace(QRegularExpression("SELECT(.*)FROM(.+)"), "SELECT COUNT(*) FROM \\2");

    QSqlQuery q;
    q.exec(str);
    if (q.next()) {
        d->pages = (int)(q.value(0).toInt() / d->limit);
        emit pagesChanged(d->pages);
    }

    if (d->pages > 0 && d->currentpage >= d->pages) setCurrentPage(d->pages - 1);
}

QOrmMetaTable *QOrmModel::metaTable() const
{
    return d->metatable;
}

void QOrmModel::setMetaTable(QOrmMetaTable *value)
{
    if (d->metatable) return;

    d->metatable = value;

    int j = 0;
    foreach(QOrmMetaAttribute *a, d->metatable->listAttributes()) {
        if (d->roles.values().contains(a->property().toLatin1())) continue;

        d->roles.insert(Qt::UserRole+1+j, a->property().toLatin1());
        j++;
    }

    foreach(QOrmMetaRelation *r, d->metatable->listRelations()) {
        if (d->roles.values().contains(r->property().toLatin1())) continue;

        d->roles.insert(Qt::UserRole+1+j, r->property().toLatin1());
        j++;
    }

    d->ld->setTable(d->metatable);

    emit metaTableChanged(d->metatable);
}

QOrmLoader *QOrmModel::loader() const
{
    return d->ld;
}

int QOrmModel::count() const
{
    return d->list.count();
}

int QOrmModel::limit() const
{
    return d->limit;
}

void QOrmModel::setLimit(int value)
{
    if (d->limit == value) return;

    d->limit = value;
    emit limitChanged(d->limit);
}

int QOrmModel::currentPage() const
{
    return d->currentpage;
}

void QOrmModel::setCurrentPage(int value)
{
    if (d->currentpage == value) return;
    d->currentpage = value;
    emit currentPageChanged(d->currentpage);
}

int QOrmModel::pages() const
{
    return d->pages;
}

bool QOrmModel::isValid()
{
    return d->metatable != 0;
}

QOrmFilterModel::QOrmFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    connect(this, &QOrmFilterModel::sourceModelChanged, this, [=]{
        updateFilterRole();

        auto model = qobject_cast<QOrmModel*>(sourceModel());
        if (!model) return;

        connect(model, &QOrmModel::metaTableChanged, this, &QOrmFilterModel::updateFilterRole);
    } );

    //TODO disconnect old modelsource
}

QOrmObject *QOrmFilterModel::at(int row)
{
    QModelIndex i = mapToSource(index(row, 0));
    auto m = qobject_cast<QOrmModel*>(sourceModel());
    if (m && i.isValid())
        return (QOrmObject*)i.internalPointer();

    return NULL;
}

QString QOrmFilterModel::filterProperty() const
{
    return m_filterproperty;
}

void QOrmFilterModel::setFilterProperty(QString value)
{
    if (m_filterproperty == value) return;

    m_filterproperty = value;
    emit filterPropertyChanged(m_filterproperty);

    updateFilterRole();
}

void QOrmFilterModel::updateFilterRole()
{
    if (!sourceModel()) return;

    int role = sourceModel()->roleNames().key(m_filterproperty.toLatin1(), -1);
    if (role < 0) return;
    setFilterRole(role);
}

QOrmRelationModel::QOrmRelationModel(QObject *parent) :
    QOrmModel(parent)
{
    m_rootObject = 0;
}

QString QOrmRelationModel::property() const
{
    return m_property;
}

void QOrmRelationModel::setProperty(QString value)
{
    if (m_property == value) return;

    m_property = value;
    emit propertyChanged(value);

    if (m_rootObject) {
        m_rootObject->disconnect(this);

        QQmlProperty prop(m_rootObject, m_property, qmlEngine(m_rootObject));
        prop.connectNotifySignal(this, SLOT(update()));
    }
}

QOrmObject *QOrmRelationModel::rootObject() const
{
    return (QOrmObject*)m_rootObject;
}

void QOrmRelationModel::setRootObject(QOrmObject *value)
{
    if (!value) return;
    if (m_rootObject == value) return;

    if (m_rootObject) m_rootObject->disconnect(this);

    m_rootObject = value;
    emit rootObjectChanged(value);

    QQmlProperty prop(m_rootObject, m_property, qmlEngine(m_rootObject));
    prop.connectNotifySignal(this, SLOT(update()));

    //load();
    update();
}

bool QOrmRelationModel::autoSumbit() const
{
    return m_autosumbit;
}

void QOrmRelationModel::setAutoSumbit(bool value)
{
    if (m_autosumbit == value) return;
    m_autosumbit = value;
    emit autoSumbitChanged(m_autosumbit);
}

bool QOrmRelationModel::submit()
{
    QQmlListReference ref(m_rootObject, m_property.toLatin1(), qmlEngine(m_rootObject));
    if (!ref.isValid()) return false;

    blockSignals(true);
    auto tmp = getList();
    ref.clear();
    foreach(QOrmObject *obj, tmp) ref.append(obj);
    blockSignals(false);

    return true;
}

void QOrmRelationModel::update()
{
    QQmlListReference ref(m_rootObject, m_property.toLatin1(), qmlEngine(m_rootObject));
    if (!ref.isValid()) return;

    QList<QOrmObject*> lst;
    for (int i = 0; i < ref.count(); i++) lst.append((QOrmObject*)ref.at(i));

    //qDebug()<<lst.count()<<m_rootObject<<m_property<<ref.count();
    setList(lst);
}

