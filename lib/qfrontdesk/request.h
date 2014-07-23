#ifndef REQUEST_H
#define REQUEST_H

#include "qfrontdesk_global.h"

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include <QStringList>

#include <QDebug>

#include "arguments.h"


class QFRONTDESKSHARED_EXPORT Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(QObject *parent = 0);

    bool isFree();
    bool setFree(bool status);
    QTcpSocket* getSocket();
    bool setSocket(int descriptor);

private:
    bool r_free;
    QTcpSocket *s;
    Arguments *arg;

signals:
    void finished();
    void route(QString, Arguments*, QTcpSocket*);

public slots:
    void discardClient();
    void readClient();
    
};

#endif // REQUEST_H
