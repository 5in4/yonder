#include "request.h"

Request::Request(QObject *parent) :
    QObject(parent)
{
    r_free = true;
}

bool Request::isFree() {
    return r_free;
}


bool Request::setFree(bool status) {
    r_free = status;
    return true;
}


QTcpSocket* Request::getSocket() {
    if(!s) {
        return 0;
    }
    return s;
}

bool Request::setSocket(int descriptor) {
    r_free = false;
    s = new QTcpSocket(this);
    return s->setSocketDescriptor(descriptor);
}

void Request::discardClient() {
    s->deleteLater();
    delete arg;
    r_free = true;
}



void Request::readClient() {
    QTextStream os(s);
    os.setAutoDetectUnicode(true);
    if(s->canReadLine()) {
        QStringList tokens = QString(s->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if(tokens.at(0) == "GET") {
            QStringList path = tokens.at(1).split("?");

            arg = new Arguments(this);
            if(path.length() > 1) {
                QStringList variables = path.at(1).split("&");
                for(int i=0; i<variables.length(); i++) {
                    arg->set(QString(variables.at(i).split("=").at(0)), variables.at(i).split("=").at(1));
                }
            }
            emit route(path.at(0), arg, s);
        }
        if (s->state() == QTcpSocket::UnconnectedState) {
            delete s;
        }
    }

}
