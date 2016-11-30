#ifndef QORM_H
#define QORM_H

#include <QtCore>
#include <QtQml>
#include <QSqlDatabase>

class QOrmObject;
class QOrmMetaTable;

class QOrm : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QQmlListProperty<QOrmMetaTable> tables READ tables NOTIFY tablesChanged)
    Q_CLASSINFO("DefaultProperty", "tables")
public:
    QOrm();

    void classBegin();
    void componentComplete();

    bool connected() const;

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

    QOrmMetaTable *findTable(QString tableName);
    QOrmObject *find(QOrmMetaTable *table, QVariant pk);

    void newError(QString context, QString msg);
    void start_transaction();
    void end_transaction(bool exec);

    void append(QOrmObject *obj);
    void remove(QOrmObject *obj);

    Q_INVOKABLE QVariantList exec(QString query, QVariantMap data = QVariantMap());

public slots:
    void connect(QString user="", QString pass="", QString db="", QString host="");
    //bool commint();
    //void append(QOrmObject *obj);
    void deleted(QOrmObject *obj);

signals:
    void connectedChanged(bool value);
    void databaseChanged(QString);
    void userChanged(QString);
    void passwordChanged(QString);
    void hostChanged(QString);
    void tablesChanged();
    void error(QString error);

private:
    void __databaseOpen();

    QSqlDatabase m_db;
    QList<QOrmMetaTable*> m_tables;
    bool m_started;
};

#endif // QORM_H
