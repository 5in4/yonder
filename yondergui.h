#ifndef YONDERGUI_H
#define YONDERGUI_H

#include <QMainWindow>
#include <QSplashScreen>
#include <QFontDatabase>
#include <QSignalMapper>
#include <QErrorMessage>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include <qmath.h>

#include <yondercore.h>
#include <model/qdjangotablemodel.h>
#include <model/track.h>


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

    QSignalMapper *mapper_menu;

    QUrl webapp_url;

    QString soundbank_path;

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



    //refactored
    void soundbankOpen();
    void soundbankCreate();
    void showAbout();
    void soundbankAddFilesMusic();
    void soundbankAddFiles();
    void soundbankAddStream();

    void editorLibraryMusic(bool show);
};

#endif // YONDERGUI_H
