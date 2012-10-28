#ifndef FRAMEGENERATOR_H
#define FRAMEGENERATOR_H

#include <QPushButton>
#include <QListView>

#include <frameobject.h>

#include <view/agqpushbutton.h>
#include <view/hotkeyspushbutton.h>

#include <atmospheremanager.h>
#include <sfxmanager.h>
#include <singleshotmanager.h>
#include <hotkeysmanager.h>
#include <musicmanager.h>

namespace Ui {
class FrameGenerator;
}

class FrameGenerator : public FrameObject
{
    Q_OBJECT
    
public:
    explicit FrameGenerator(QWidget *parent = 0);
    ~FrameGenerator();

    AGQPushButton *music_play_pause;
    AGQPushButton *music_next;
    QListView *music_select_playlist;

    AtmosphereManager *atmosphere;
    HotkeysManager *hotkeys;
    MusicManager *music;
    SfxManager *sfx;
    SingleshotManager *singleshot;

    QList<AGQPushButton*> atmosphere_buttons;
    QList<AGQPushButton*> sfx_buttons;
    QList<AGQPushButton*> singleshot_buttons;
    QList<HotkeysPushButton*> hotkeys_buttons;

public slots:
    void atmosphereControl(int pos_in_array, bool checked);
    void atmosphereCreateButtons();
    void atmosphereSetVolume();

    void musicSetPlaylist(QModelIndex index);
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

    void setSoundManagers(AtmosphereManager *atmosphere, SfxManager *sfx, MusicManager *music, SingleshotManager *singleshot);
    void setHotkeysManager(HotkeysManager *hotkeys);
    void refreshSoundUi();
    
private:
    Ui::FrameGenerator *ui;
};

#endif // FRAMEGENERATOR_H
