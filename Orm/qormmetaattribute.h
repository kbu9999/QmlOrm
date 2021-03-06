#ifndef QATTRIBUTEINFO_H
#define QATTRIBUTEINFO_H

#include <QString>
#include <QVariant>
#include <QMetaProperty>
#include <qormloader.h>

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
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
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
    bool readOnly() const;


    void blockOnLoad();
    void unBlockOnLoad();

    virtual void setValue(QOrmObject *obj, QVariant v);
    virtual void modified(QOrmObject *obj);

protected slots:
    void setPos(int value);
    void setAttribute(QString value);
    void setProperty(QString value);
    void setIndex(int value);
    void setIsNullable(bool value);
    void setReadOnly(bool value);
    void attr_modified();

signals:
    void posChanged(int);
    void attributeChanged(QString);
    void propertyChanged(QString);
    void indexChanged(int);
    void isIndexChanged(bool);
    void isForeingkeyChanged(bool);
    void isNullableChanged(bool);
    void readOnlyChanged(bool value);

private:
    class Private;
    Private *d;
};

Q_DECLARE_METATYPE(QOrmMetaAttribute*)
#endif // QATTRIBUTEINFO_H
