#ifndef QORM_H
#define QORM_H

#include <QtCore>
#include <QtQml>
#include <QSqlDatabase>

class QOrmObject;
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
    void setDatabase(QString value);

    QString user() const;
    void setUser(QString value);

    QString password() const;
    void setPassword(QString value);

    QString host() const;
    void setHost(QString value);

    QList<QOrmMetaTable *> listTables() const;
    QQmlListProperty<QOrmMetaTable> tables();
    void appendTable(QOrmMetaTable *t);
    void removeTable(QOrmMetaTable *t);

    QOrmObject *find(QOrmMetaTable *table, QVariant pk);
    void append(QOrmObject *obj);

    static QOrm *defaultOrm();

public slots:
    void connect();
    void commint();

private:
    void initFks(QOrmMetaTable *t);

signals:
    void databaseChanged(QString);
    void userChanged(QString);
    void passwordChanged(QString);
    void hostChanged(QString);
    void tablesChanged();
    void error(QString error);
    void connected();

private:
    QSqlDatabase m_db;
    QList<QOrmMetaTable*> m_tables;

    static QOrm * m_def;
};

#endif // QORM_H
