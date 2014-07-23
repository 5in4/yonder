#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "qfrontdesk_global.h"

#include <QObject>
#include <QList>

#include "request.h"

class QFRONTDESKSHARED_EXPORT RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestManager(int slots_max = 10, QObject *parent = 0);

    Request *getFreeSlot();

private:
   QList<Request*> requests;
   int slots_max;
signals:
    
public slots:
    
};

#endif // REQUESTMANAGER_H
