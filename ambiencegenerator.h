#ifndef AMBIENCEGENERATOR_H
#define AMBIENCEGENERATOR_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <qmath.h>

#include <framestart.h>
#include <framegenerator.h>
#include <frameeditor.h>
#include <frameresourcebrowser.h>
#include <frameconfigure.h>
#include <framehelp.h>

#include <mediamanager.h>
#include <atmospheremanager.h>
#include <hotkeysmanager.h>
#include <musicmanager.h>
#include <sfxmanager.h>
#include <singleshotmanager.h>

#include <webapp.h>

namespace Ui {
    class AmbienceGenerator;
}

class AmbienceGenerator : public QMainWindow
{
    Q_OBJECT

public:
    explicit AmbienceGenerator(QSplashScreen *splash_screen, QWidget *parent = 0);
    ~AmbienceGenerator();

    FrameStart *start_frame;
    FrameGenerator *generator_frame;
    FrameEditor *editor_frame;
    FrameResourceBrowser *resource_frame;
    FrameConfigure *configure_frame;
    FrameHelp *help_frame;

    QPointer<MediaManager> media;
    QPointer<AtmosphereManager> atmosphere;
    QPointer<HotkeysManager> hotkeys;
    QPointer<MusicManager> music;
    QPointer<SfxManager> sfx;
    QPointer<SingleshotManager> singleshot;

    QPointer<Webapp> webapp;

    QString config_path;
    QString project_path;
    QSqlDatabase db;

    QSplashScreen *splash_screen;

    bool project_set = false;

    Ui::AmbienceGenerator *ui;

public slots:
    void applyStylesheet();
    void ambienceEditor();
    void openOnlineHelp();

    void setProject(QString project_path);
    void refreshProject();

    void webappStart();
    void webappStop();
    void webappOpenUrl();

private:
    QNetworkAccessManager *update_manager;
    QNetworkReply *update_reply;

private slots:
    void checkUpdate();
    void checkUpdateProcess(QNetworkReply *rep);

protected:

};

#endif // AMBIENCEGENERATOR_H
