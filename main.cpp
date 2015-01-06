#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

QString NAME = "Yonder";
QString VERSION = "2.0.0-devel";
QString AUTHOR = "Dominik Seemann";
QString WEBADDRESS = "http://yonderfx.com/"; // always with trailing slash!
QString ACCEPTED_MIMETYPES = "*.mp3 *.ogg *.flac *.aac *.wav";
QString YONDER_MIMETYPE = "*.yfx";

int main(int argc, char *argv[])
{
    qDebug() << QString("%1 %2").arg(NAME, VERSION);

    QApplication a(argc, argv);
    a.setOrganizationName("yonderfx.com");
    a.setOrganizationDomain("com.yonderfx");
    a.setApplicationName(NAME);

    //QPixmap splash(":/application/icons/splash.png");
    //QSplashScreen *splash_screen = new QSplashScreen(splash);
    //splash_screen->show();
    //YonderGui w(splash_screen);
    //w.show();
    //splash_screen->finish(&w);

    QQmlApplicationEngine e;
    e.load(QUrl(QStringLiteral("qrc:/yondergui.qml")));

    return a.exec();
}
