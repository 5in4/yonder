#ifndef YONDERGUI_H
#define YONDERGUI_H

#include <QMainWindow>
#include <QSplashScreen>
//#include <QtNetwork/QNetworkAccessManager>
//#include <QtNetwork/QNetworkReply>

#include <qmath.h>

#include <framestart.h>
#include <framegenerator.h>
#include <frameeditor.h>
#include <frameresourcebrowser.h>
#include <frameconfigure.h>
#include <framehelp.h>

#include <yondercore.h>

namespace Ui {
    class YonderGui;
}

class YonderGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit YonderGui(QSplashScreen *splash_screen, QWidget *parent = 0);
    ~YonderGui();

    Ui::YonderGui *ui;

    YonderCore *core;
    QSplashScreen *splash_screen;

    FrameStart *start_frame;
    FrameGenerator *generator_frame;
    FrameEditor *editor_frame;
    FrameResourceBrowser *resource_frame;
    FrameConfigure *configure_frame;
    FrameHelp *help_frame;

    QUrl webapp_url;

public slots:
    void applyStylesheet();

    void openWebappUrl();
    void openOnlineHelp();
    void openUpdater();

    void setSplashScreen(QString message);
    void stateLoading();
    void stateLoadingFailed();
    void stateLoaded();
    void stateRefreshed();

    void webappStarted(QUrl url);
    void webappStopped();

};

#endif // YONDERGUI_H
