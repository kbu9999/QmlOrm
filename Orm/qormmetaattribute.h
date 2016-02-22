#ifndef QATTRIBUTEINFO_H
#define QATTRIBUTEINFO_H

#include <QString>
#include <QVariant>

class QOrmObject;
class QOrmMetaTable;

class QOrmMetaAttribute : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(int pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QString attribute READ attribute WRITE setAttribute NOTIFY attributeChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(bool isIndex READ isIndex NOTIFY isIndexChanged)
    Q_PROPERTY(bool isForeingkey READ isForeingkey NOTIFY isForeingkeyChanged)
    Q_PROPERTY(bool isNullable READ isNullable WRITE setIsNullable NOTIFY isNullableChanged)
    Q_CLASSINFO("DefaultProperty", "attribute")
public:
    QOrmMetaAttribute();
    virtual ~QOrmMetaAttribute();

    bool isValid() const;
    int pos() const;
    QString attribute() const;
    QString property() const;
    int index() const;
    bool isIndex() const;
    virtual bool isForeingkey() const;
    bool isNullable() const;

    virtual void setValue(QOrmObject *obj, QVariant v);

protected slots:
    void setPos(int value);
    void setAttribute(QString value);
    void setProperty(QString value);
    void setIndex(int value);
    void setIsNullable(bool value);

    virtual void modified();

signals:
    void posChanged(int);
    void attributeChanged(QString);
    void propertyChanged(QString);
    void indexChanged(int);
    void isIndexChanged(bool);
    void isForeingkeyChanged(bool);
    void isNullableChanged(bool);

private:
    class Private;
    Private *d;
};

class QOrmMetaForeignKey : public QOrmMetaAttribute
{
    Q_OBJECT
    Q_PROPERTY(QString foreignTable READ foreignTable WRITE setForeignTable)
public:
    QOrmMetaForeignKey();
    virtual ~QOrmMetaForeignKey();

    bool isForeingkey() const;
    void setValue(QOrmObject *obj, QVariant v);
    void modified();

    QString foreignTable() const;
    void setForeignTable(QString value);

    void setForeignMeta(QOrmMetaTable *fkmeta);

private:
    QString m_foreigntable;
    QOrmMetaTable *m_fktable;
};

Q_DECLARE_METATYPE(QOrmMetaAttribute*)
Q_DECLARE_METATYPE(QOrmMetaForeignKey*)
#endif // QATTRIBUTEINFO_H
