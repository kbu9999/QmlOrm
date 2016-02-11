#include "qorm.h"

#include <QtSql>

#include "qormmetatable.h"
#include "qormmetaattribute.h"


QOrm *QOrm::m_def = NULL;

QOrm::QOrm() : QObject()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    if (!QOrm::m_def)
        QOrm::m_def = this;
}

QString QOrm::database() const
{
    return m_db.databaseName();
}

QString QOrm::user() const
{
    return m_db.userName();
}

QString QOrm::password() const
{
    return m_db.password();
}

QString QOrm::host() const
{
    return m_db.hostName();
}

QOrmMetaTable *QOrm::findTable(QString type)
{
    return m_mapTables.value(type, NULL);
}

void QOrm::appendTable(QString type, QOrmMetaTable *t)
{
    if (m_mapTables.contains(type)) return;

    m_mapTables.insert(type, t);
    m_tables = m_mapTables.values();

    emit tablesChanged();
}

void QOrm::removeTable(QString type)
{
    if (m_mapTables.contains(type)) return;

    m_mapTables.remove(type);
    emit tablesChanged();
}

QQmlListProperty<QOrmMetaTable> QOrm::tables()
{
    return QQmlListProperty<QOrmMetaTable>(this, m_tables);
}

void QOrm::setDatabase(QString value)
{
    if (m_db.databaseName() == value) return;
    m_db.setDatabaseName(value);

    emit databaseChanged(value);
}

void QOrm::setUser(QString value)
{
    if (m_db.userName() == value) return;
    m_db.setUserName(value);

    emit userChanged(value);
}

void QOrm::setPassword(QString value)
{
    if (m_db.password() == value) return;
    m_db.setPassword(value);

    emit passwordChanged(value);
}

void QOrm::setHost(QString value)
{
    if (m_db.hostName() == value) return;
    m_db.setHostName(value);

    emit hostChanged(value);
}

QOrm *QOrm::defaultOrm()
{
    if (!m_def)
        m_def = new QOrm();

    return m_def;
}

void QOrm::connect()
{
    m_db.open();
}
