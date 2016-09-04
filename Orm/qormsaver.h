#ifndef QORMSACER_H
#define QORMSACER_H

#include <QtCore>

class QOrm;
class QOrmObject;

class QOrmSaver
{
public:
    QOrmSaver(QOrm *db);

    bool isValid();

    void start();
    void Save(QOrmObject *obj);
    void Delete(QOrmObject *obj);

    bool commit();
    void clear();
    //bool waitForFinnished();

    QString lastErrors();
    QStringList lastQuery();

private:
    class Private;
    friend class Private;
    Private *d;
};

#endif // QORMSACER_H
