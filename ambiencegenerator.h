#ifndef AMBIENCEGENERATOR_H
#define AMBIENCEGENERATOR_H

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <qmath.h>

#ifdef __WIN32
#include <taglib.h>
#elif __linux__
#include <taglib/taglib.h>
#else
#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#endif

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

    MediaManager *media;
    AtmosphereManager *atmosphere;
    HotkeysManager *hotkeys;
    MusicManager *music;
    SfxManager *sfx;
    SingleshotManager *singleshot;

    Webapp *webapp;

    QString config_path;
    QString project_path;
    QSqlDatabase db;

    QSplashScreen *splash_screen;

    Ui::AmbienceGenerator *ui;

public slots:
    void applyStylesheet();
    void ambienceEditor();
    void openOnlineHelp();

    void setProject(QString project_path);

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
    void resizeEvent(QResizeEvent *event);

};

#endif // AMBIENCEGENERATOR_H
