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

#include <view/agqpushbutton.h>
#include <ambienceeditor.h>
#include <resourcebrowser.h>
#include <projectselector.h>
#include <atmospheremanager.h>
#include <hotkeysmanager.h>
#include <musicmanager.h>
#include <sfxmanager.h>
#include <singleshotmanager.h>

namespace Ui {
    class AmbienceGenerator;
}

class AmbienceGenerator : public QMainWindow
{
    Q_OBJECT

public:
    explicit AmbienceGenerator(QSplashScreen *splash_screen, QWidget *parent = 0);
    ~AmbienceGenerator();

    QProgressBar *progress_bar;

    AtmosphereManager *atmosphere;
    HotkeysManager *hotkeys;
    MusicManager *music;
    SfxManager *sfx;
    SingleshotManager *singleshot;

    QList<AGQPushButton*> atmosphere_buttons;
    QList<AGQPushButton*> sfx_buttons;
    QList<AGQPushButton*> singleshot_buttons;
    QList<AGQPushButton*> hotkeys_buttons;

    QString config_path;
    QString project_path;
    QSqlDatabase db;

    QSplashScreen *splash_screen;

    Ui::AmbienceGenerator *ui;

public slots:
    void setProjectSlot();
    void ambienceEditor();
    void resourceBrowser();
    bool prepareExit();
    void openOnlineHelp();
    void showAbout();

    void atmosphereControl(int pos_in_array, bool checked);
    void atmosphereCreateButtons();
    void atmosphereSetVolume();

    void musicSetPlaylist(int index);
    void musicNext();
    void musicSetVolume();
    void musicSetSeek(int time, int length);
    void musicTrackChanged();
    void musicPlayPause();

    void sfxControl(int pos_in_array, bool checked);
    void sfxCreateButtons();
    void sfxSetVolume();

    void singleshotControl(int pos_in_array, bool checked);
    void singleshotCreateButtons();
    void singleshotSetVolume();

    void hotkeysCreateButtons();
    void hotkeysControl(int pos_in_array, bool checked);

private:
    void closeEvent(QCloseEvent *event);
    void setProject(QString project_path, bool force_dialog=false);

    QSettings settings;
    QNetworkAccessManager *update_manager;
    QNetworkReply *update_reply;

private slots:
    void checkUpdate();
    void checkUpdateProcess(QNetworkReply *rep);

};

#endif // AMBIENCEGENERATOR_H
