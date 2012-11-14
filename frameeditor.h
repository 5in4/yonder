#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <frameobject.h>
#include <delegate/aghotkeyactiondelegate.h>
#include <agmediacontainer.h>
#include <delegate/agtagbyiddelegate.h>
#include <delegate/qdoublespinboxdelegate.h>
#include <mediamanager.h>
#include <atmospheremanager.h>
#include <sfxmanager.h>
#include <singleshotmanager.h>
#include <hotkeysmanager.h>
#include <musicmanager.h>


namespace Ui {
class FrameEditor;
}

class FrameEditor : public FrameObject
{
    Q_OBJECT
    
public:
    explicit FrameEditor(MediaManager *media, QWidget *parent = 0);
    ~FrameEditor();
    
    AtmosphereManager *atmosphere;
    SfxManager *sfx;
    MusicManager *music;
    HotkeysManager *hotkeys;
    SingleshotManager *singleshot;

    MediaContainer *media_preview;

public slots:

    void setManagers(AtmosphereManager *atmosphere, SfxManager *sfx, MusicManager *music, SingleshotManager *singleshot, HotkeysManager *hotkeys);

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
    void previewStop();
    void previewSetSeek(int time, int length);

private:
    Ui::FrameEditor *ui;
};

#endif // FRAMEEDITOR_H
