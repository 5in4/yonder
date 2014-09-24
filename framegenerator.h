#ifndef FRAMEGENERATOR_H
#define FRAMEGENERATOR_H

#include <QPushButton>
#include <QListView>
#include <QGridLayout>

#include <frameobject.h>

#include <view/yonderpushbutton.h>

#include <yondercore.h>

namespace Ui {
class FrameGenerator;
}

class FrameGenerator : public FrameObject
{
    Q_OBJECT

public:
    explicit FrameGenerator(YonderCore *core, QWidget *parent = 0);
    ~FrameGenerator();

    void cleanGridLayout(QGridLayout *layout);

    YonderPushButton *music_play_pause;
    YonderPushButton *music_next;
    QListView *music_select_playlist;

    YonderCore *core;

    QList<YonderPushButton*> atmosphere_buttons;
    QList<YonderPushButton*> sfx_buttons;
    QList<YonderPushButton*> singleshot_buttons;

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

    //void sfxControl(int pos_in_array, bool checked);
    void sfxCreateButtons();
    void sfxSetVolume();

    void singleshotControl(int pos_in_array, bool checked);
    void singleshotCreateButtons();
    void singleshotSetVolume();

    void refreshUi();
    void refreshUi(bool project_set);

signals:
    void uiRefreshed();

private:
    Ui::FrameGenerator *ui;
};

#endif // FRAMEGENERATOR_H
