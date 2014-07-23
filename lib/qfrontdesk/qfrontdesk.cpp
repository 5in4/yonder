#include "qfrontdesk.h"

QFrontDesk::QFrontDesk(int port, int slots_max, QString static_serve_path, QObject *parent) : QTcpServer(parent)
{
    this->port = port;

    rm = new RequestManager(slots_max, this);

    // ip determination from http://doc-snapshot.qt-project.org/4.8/network-fortuneserver-server-cpp.html
    QList<QHostAddress> ip_addressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ip_addressesList.size(); ++i) {
        if (ip_addressesList.at(i) != QHostAddress::LocalHost &&
                ip_addressesList.at(i).toIPv4Address()) {
            ip_address = ip_addressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ip_address.isEmpty()) {
        ip_address = QHostAddress(QHostAddress::LocalHost).toString();
    }

    //g_engine = new Engine(this);
    //g_loader = FileSystemTemplateLoader::Ptr(new FileSystemTemplateLoader());

    this->static_serve_path = static_serve_path;
}


QFrontDesk::~QFrontDesk() {
    qDebug() << "QFrontDesk closing";
}

void QFrontDesk::setActive(bool active) {
    if(active && !isListening()) {
        if(listen(QHostAddress(ip_address), port)) {
            qDebug() << "QFrontDesk started on" << QString("http://%1:%2/").arg(ip_address, QString("%1").arg(port));
        } else {
            qDebug() << "QFrontDesk: Error while starting server";
        }
        return;
    }
    if(!active && isListening()) {
        close();
        qDebug() << "QFrontDesk stopped";
        return;
    }
}


void QFrontDesk::incomingConnection(int socket) {
    //qDebug() << "incoming connection";
    r = rm->getFreeSlot();
    if(!r) {
        qDebug() << "no free slots";
        s = new QTcpSocket(this);
        s->setSocketDescriptor(socket);

        //todo
        QTextStream os(s);
        os << "HTTP/1.0 503 Service Unavailable\r\n Content-Type: text/html;"
              "charset=\"utf-8\"\r\n"
              "\r\n";
        os << "<!DOCTYPE html><html><body><h1>No free slots</h1><p>Try again later</p></body></html>";
        s->close();
        s->deleteLater();
        return;
    }
    r->setSocket(socket);
    connect(r->getSocket(), SIGNAL(readyRead()), r, SLOT(readClient()));
    connect(r->getSocket(), SIGNAL(disconnected()), r, SLOT(discardClient()));
    connect(r, SIGNAL(route(QString,Arguments*,QTcpSocket*)) , this, SLOT(resolveRoute(QString,Arguments*,QTcpSocket*)));
}


void QFrontDesk::resolveRoute(QString route, Arguments *arg, QTcpSocket *s) {
    QTextStream os(s);
    os.setAutoDetectUnicode(true);
    //QDataStream os(s);
    QByteArray content;

    // sort out static files
    if(QDir(static_serve_path).exists() && route.split("/").at(1) == QString("static")) {
        QFile static_file(QString("%1/%2").arg(static_serve_path, route.split("/").at(2)));
        if(static_file.exists() && route.split("/").length() == 3) {

            QString mimetype;
            static_file.open(QIODevice::ReadOnly);
            QString file_name = static_file.fileName();
            QString file_suffix = file_name.split(".").last();
            if(file_suffix.isEmpty()) {
                mimetype = QString("text/plain");
            }

            mimetype = (file_suffix == "png") ? "image/png" : "";

            os << QString("HTTP/1.0 200 OK\r\n Content-Type: %1;\r\n\r\n").arg(mimetype);
            os.flush();
            s->write(static_file.readAll());

            static_file.close();

            s->close();
            return;
       }
    } else {
        content.append(routing(route, arg));
    }

   if(content.length() == 0) {
        os << "HTTP/1.0 404 Not Found\r\n Content-Type: text/html;"
              "charset=\"utf-8\"\r\n"
              "\r\n"
              "<!DOCTYPE html><html><h1>404 Not Found</h1><p>The requested URL was not found on this server.";
    } else {
       os << "HTTP/1.0 200 OK\r\n Content-Type: text/html; charset=\"utf-8\"\r\n\r\n";
       os << content;
    }
    s->close();
    return;


}


//void QFrontDesk::setTemplateDirs(QStringList dirs) {
//    g_loader->setTemplateDirs(dirs);
//    return g_engine->addTemplateLoader(g_loader);
//}


//Template QFrontDesk::loadTemplate(QString tpl) {
//    return g_engine->loadByName(tpl);
//}


QString QFrontDesk::getUrl() {
    return QString("http://%1:%2/").arg(ip_address, QString("%1").arg(port));
}
