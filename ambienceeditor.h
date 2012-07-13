#ifndef AMBIENCEEDITOR_H
#define AMBIENCEEDITOR_H

#include <QtCore>
#include <QtGui>

#include <vlc/vlc.h>

#include <delegate/aghotkeyactiondelegate.h>
#include <agmediacontainer.h>
#include <delegate/agtagbyiddelegate.h>
#include <delegate/qdoublespinboxdelegate.h>
#include <atmospheremanager.h>
#include <sfxmanager.h>
#include <singleshotmanager.h>
#include <hotkeysmanager.h>
#include <musicmanager.h>

namespace Ui {
    class AmbienceEditor;
}

class AmbienceEditor : public QDialog
{
    Q_OBJECT

public:
    explicit AmbienceEditor(AtmosphereManager *atmosphere, SfxManager *sfx, MusicManager *music, SingleshotManager *singleshot, HotkeysManager *hotkeys, QWidget *parent = 0);
    ~AmbienceEditor();

    AtmosphereManager *atmosphere;
    SfxManager *sfx;
    MusicManager *music;
    HotkeysManager *hotkeys;
    SingleshotManager *singleshot;

    libvlc_instance_t *inst;
    AGMediaContainer *media_preview;

public slots:
    void accept();

    void atmosphereObjectAdd();
    void atmosphereObjectRemove();
    void atmosphereUiRefresh();
    void atmosphereObjectsTracksRemove();

    void sfxObjectAdd();
    void sfxObjectRemove();
    void sfxUiRefresh();
    void sfxObjectsTracksRemove();
    void sfxObjectsTrackCheck(QModelIndex i1, QModelIndex i2);

    void musicPlaylistAdd();
    void musicPlaylistRemove();
    void musicUiRefresh();
    void musicPlaylistsTracksRemove();

    void singleshotObjectAdd();
    void singleshotObjectRemove();
    void singleshotUiRefresh();
    void singleshotObjectsTracksRemove();

    void hotkeysHotkeyAdd();
    void hotkeysHotkeyRemove();
    void hotkeysUiRefresh();
    void hotkeysHotkeyActionsRemove();

    void previewPlayPause(bool state);
    void previewEnqueue(QString mime, int tid);
    void previewStop(int channel);
    void previewSetSeek(int time, int length);

private:
    Ui::AmbienceEditor *ui;

};

#endif // AMBIENCEEDITOR_H
