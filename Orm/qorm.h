#ifndef QORM_H
#define QORM_H

#include <QtCore>
#include <QtQml>
#include <QSqlDatabase>

class QOrmMetaTable;

class QOrm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QQmlListProperty<QOrmMetaTable> tables READ tables NOTIFY tablesChanged)
    Q_CLASSINFO("DefaultProperty", "tables")
public:
    QOrm();

    QString database() const;
    QString user() const;
    QString password() const;
    QString host() const;

    QOrmMetaTable *findTable(QString type);
    QList<QOrmMetaTable *> listTables() const;
    void appendTable(QString type, QOrmMetaTable *t);
    void removeTable(QString type);
    QQmlListProperty<QOrmMetaTable> tables();

    void setDatabase(QString value);
    void setUser(QString value);
    void setPassword(QString value);
    void setHost(QString value);

    static QOrm *defaultOrm();

public slots:
    void connect();

signals:
    void databaseChanged(QString);
    void userChanged(QString);
    void passwordChanged(QString);
    void hostChanged(QString);
    void tablesChanged();
    void error(QString);
    void connected();

private:
    QSqlDatabase m_db;
    QMap<QString, QOrmMetaTable*> m_mapTables;
    QList<QOrmMetaTable*> m_tables;

    static QOrm * m_def;
};

#endif // QORM_H
