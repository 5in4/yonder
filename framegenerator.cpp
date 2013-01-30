#include "framegenerator.h"
#include "ui_framegenerator.h"

extern QString NAME;
extern QString VERSION;
extern QString ACTION_KEY;
extern QString AUTHOR;
extern QString WEBADDRESS;
extern QString ACCEPTED_MIMETYPES;

FrameGenerator::FrameGenerator(YonderCore *core, QWidget *parent) :
    FrameObject(parent),
    ui(new Ui::FrameGenerator)
{
    ui->setupUi(this);

    this->core = core;

    music_play_pause = new AGQPushButton(0, this);
    music_play_pause->setText(tr("Play/Pause"));
    music_play_pause->setIcon(QIcon(":/application/icons/media-playback-start.png"));
    music_play_pause->setCheckable(true);
    music_play_pause->setStandardStyle();
    ui->music_control_layout->addWidget(music_play_pause);

    music_next = new AGQPushButton(0, this);
    music_next->setText(tr("Next"));
    music_next->setIcon(QIcon(":/application/icons/media-skip-forward.png"));
    music_next->setStandardStyle();
    ui->music_control_layout->addWidget(music_next);

    music_select_playlist = ui->music_select_playlist;

    QSettings settings;

    ui->atmosphereVolume->setValue(settings.value("Settings/atmosphere_volume", 50).toInt());
    ui->musicVolume->setValue(settings.value("Settings/music_volume", 50).toInt());
    ui->sfxVolume->setValue(settings.value("Settings/sfx_volume", 50).toInt());
    ui->singleshotVolume->setValue(settings.value("Settings/singleshot_volume", 50).toInt());

    // s&s
    connect(ui->atmosphereVolume, SIGNAL(valueChanged(int)), this, SLOT(atmosphereSetVolume()));
    connect(ui->music_select_playlist, SIGNAL(activated(QModelIndex)), this, SLOT(musicSetPlaylist(QModelIndex)));
    connect(ui->music_select_playlist, SIGNAL(clicked(QModelIndex)), this, SLOT(musicSetPlaylist(QModelIndex)));
    connect(music_next, SIGNAL(clicked()), this, SLOT(musicNext()));
    connect(music_play_pause, SIGNAL(toggled(bool)), this, SLOT(musicPlayPause()));
    connect(ui->musicVolume, SIGNAL(valueChanged(int)), this, SLOT(musicSetVolume()));
    connect(ui->sfxVolume, SIGNAL(valueChanged(int)), this, SLOT(sfxSetVolume()));
    connect(ui->singleshotVolume, SIGNAL(valueChanged(int)), this, SLOT(singleshotSetVolume()));
}

FrameGenerator::~FrameGenerator()
{
    QSettings settings;

    settings.setValue("Settings/singleshot_volume", ui->singleshotVolume->value());
    settings.setValue("Settings/sfx_volume", ui->sfxVolume->value());
    settings.setValue("Settings/music_volume", ui->musicVolume->value());
    settings.setValue("Settings/atmosphere_volume", ui->atmosphereVolume->value());

    settings.sync();
    delete ui;
}



void FrameGenerator::refreshUi() {
    refreshUi(true);
}

void FrameGenerator::refreshUi(bool project_set) {
    qDebug() << "refreshing ui";

    connect(core->music->container.at(0), SIGNAL(starting(int)), this, SLOT(musicTrackChanged()));

    connect(core->music->container.at(0), SIGNAL(trackPosition(int, int)), this, SLOT(musicSetSeek(int,int)));

    singleshotSetVolume();
    atmosphereSetVolume();
    musicSetVolume();
    sfxSetVolume();


    if(project_set) {
        atmosphereCreateButtons();
        sfxCreateButtons();
        singleshotCreateButtons();

        ui->music_select_playlist->setModel(core->music->objects_model);
        ui->music_select_playlist->setModelColumn(1);
        musicSetPlaylist(0);
    } else {
        qDebug() << "Generators not summoned, skipping button creation";
    }
    emit uiRefreshed();
}


void FrameGenerator::hotkeysCreateButtons() {
    while(ui->hotkeysButtonLayout->count() > 0) {
        ui->hotkeysButtonLayout->removeItem(ui->hotkeysButtonLayout->takeAt(0));
    }
    this->repaint();

    while(hotkeys_buttons.length() > 0) {
        delete hotkeys_buttons.last();
        hotkeys_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(core->hotkeys->hotkeys.length());

    for(int i=0; i < core->hotkeys->hotkeys.length(); i++) {
        hotkeys_buttons.append(new HotkeysPushButton(i, this));
        hotkeys_buttons.at(i)->setOID(core->hotkeys->hotkeys.at(i)[0].toInt());
        hotkeys_buttons.at(i)->setCheckable(true);
        hotkeys_buttons.at(i)->setObjectName(core->hotkeys->hotkeys.at(i)[1]);
        hotkeys_buttons.at(i)->setStandardStyle();
        //hotkeys_buttons.at(i)->setPalette(QPalette(QColor(250, 250, 100)));
        //hotkeys_buttons.at(i)->setIcon(QIcon::fromTheme("media-playback-start"));

        connect(hotkeys_buttons.at(i), SIGNAL(highlightButtons(int,bool)), core->hotkeys, SLOT(highlightButtons(int,bool)));

        QString hk_btn_view;
        if(core->hotkeys->hotkeys.at(i)[2] == "") {
            hk_btn_view = "%1";
                    hotkeys_buttons.at(i)->setHtml(QString(hk_btn_view).arg(core->hotkeys->hotkeys.at(i)[1]));
        } else {
            hk_btn_view = "%1 <b>[%2%3]</b>";
            hotkeys_buttons.at(i)->setHtml(QString(hk_btn_view).arg(core->hotkeys->hotkeys.at(i)[1], ACTION_KEY, core->hotkeys->hotkeys.at(i)[2]));
        }

        ui->hotkeysButtonLayout->addWidget(hotkeys_buttons.at(i));
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        hotkeys_buttons.at(i)->setShortcut(QKeySequence(QString("%1%2").arg(ACTION_KEY, core->hotkeys->hotkeys.at(i)[2])));
        connect(hotkeys_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(hotkeysControl(int,bool)));
    }
}


void FrameGenerator::hotkeysControl(int pos_in_array, bool checked) {
    core->hotkeys->callHotkey(hotkeys_buttons.at(pos_in_array)->getOID(), checked);
}


void FrameGenerator::atmosphereCreateButtons() {
    while(ui->atmosphereButtonLayout->count() > 0) {
        ui->atmosphereButtonLayout->removeItem(ui->atmosphereButtonLayout->takeAt(0));
    }
    this->repaint();

    while(atmosphere_buttons.length() > 0) {
        delete atmosphere_buttons.last();
        atmosphere_buttons.removeLast();
    }


    int row = 0;
    int col = 0;
    int max = qSqrt(core->atmosphere->objects.length());

    for(int i=0; i < core->atmosphere->objects.length(); i++) {
        atmosphere_buttons.append(new AGQPushButton(i, this));
        atmosphere_buttons.at(i)->setOID(core->atmosphere->objects.at(i)[0].toInt());
        atmosphere_buttons.at(i)->setCheckable(true);
        atmosphere_buttons.at(i)->setStandardStyle();
        atmosphere_buttons.at(i)->setIcon(QIcon(":/application/icons/icon-atmosphere-start.png"));
        //atmosphere_buttons.at(i)->setPalette(QPalette(QColor(150, 200, 250)));
        atmosphere_buttons.at(i)->setObjectName(core->atmosphere->objects.at(i)[1]);
        atmosphere_buttons.at(i)->setText(core->atmosphere->objects.at(i)[1]);

        ui->atmosphereButtonLayout->addWidget(atmosphere_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        connect(atmosphere_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(atmosphereControl(int,bool)));
    }
}


void FrameGenerator::atmosphereControl(int pos_in_array, bool checked) {
    if(checked) {
        core->atmosphere->play(pos_in_array);
    } else {
        core->atmosphere->pause(pos_in_array);
    }
}


void FrameGenerator::atmosphereSetVolume() {
    core->atmosphere->setVolume(ui->atmosphereVolume->value());
}


void FrameGenerator::sfxCreateButtons() {
    while(ui->sfxButtonLayout->count() > 0) {
        ui->sfxButtonLayout->removeItem(ui->sfxButtonLayout->takeAt(0));
    }
    this->repaint();

    while(sfx_buttons.length() > 0) {
        delete sfx_buttons.last();
        sfx_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(core->sfx->objects.length());

    for(int i=0; i < core->sfx->objects.length(); i++) {
        sfx_buttons.append(new AGQPushButton(i, this));
        sfx_buttons.at(i)->setOID(core->sfx->objects.at(i)[0].toInt());
        sfx_buttons.at(i)->setCheckable(true);
        sfx_buttons.at(i)->setStandardStyle();
        sfx_buttons.at(i)->setIcon(QIcon(":/application/icons/icon-sfx-start.png"));
        //sfx_buttons.at(i)->setPalette(QPalette(QColor(250, 150, 150)));
        sfx_buttons.at(i)->setObjectName(core->sfx->objects.at(i)[1]);
        sfx_buttons.at(i)->setText(core->sfx->objects.at(i)[1]);

        ui->sfxButtonLayout->addWidget(sfx_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        connect(sfx_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(sfxControl(int,bool)));
    }
}

void FrameGenerator::sfxControl(int pos_in_array, bool checked) {
    if(checked) {
        core->sfx->play(pos_in_array);
    } else {
        core->sfx->pause(pos_in_array);
    }
}

void FrameGenerator::sfxSetVolume() {
    core->sfx->setVolume(ui->sfxVolume->value());
}


void FrameGenerator::singleshotCreateButtons() {
    while(ui->singleshotButtonLayout->count() > 0) {
        ui->singleshotButtonLayout->removeItem(ui->singleshotButtonLayout->takeAt(0));
    }
    this->repaint();

    while(singleshot_buttons.length() > 0) {
        delete singleshot_buttons.last();
        singleshot_buttons.removeLast();
    }

    int row = 0;
    int col = 0;
    int max = qSqrt(core->singleshot->objects.length());

    for(int i=0; i < core->singleshot->objects.length(); i++) {
        singleshot_buttons.append(new AGQPushButton(i, this));
        singleshot_buttons.at(i)->setOID(core->singleshot->objects.at(i)[0].toInt());
        singleshot_buttons.at(i)->setCheckable(true);
        singleshot_buttons.at(i)->setStandardStyle();
        singleshot_buttons.at(i)->setIcon(QIcon(":/application/icons/icon-singleshot-start.png"));
        //singleshot_buttons.at(i)->setPalette(QPalette(QColor(200, 250, 200)));
        singleshot_buttons.at(i)->setObjectName(core->singleshot->objects.at(i)[1]);
        singleshot_buttons.at(i)->setText(core->singleshot->objects.at(i)[1]);

        ui->singleshotButtonLayout->addWidget(singleshot_buttons.at(i), row, col);
        if(col < max) {
            col++;
        } else {
            col = 0;
            row++;
        }

        connect(singleshot_buttons.at(i), SIGNAL(toggled(int,bool)), this, SLOT(singleshotControl(int,bool)));
        connect(core->singleshot->container.at(i), SIGNAL(finished(int)), singleshot_buttons.at(i), SLOT(setUnChecked()));
    }
}


void FrameGenerator::singleshotControl(int pos_in_array, bool checked) {
    if(checked) {
        core->singleshot->play(pos_in_array);
    } else {
        core->singleshot->stop(pos_in_array);
    }
}


void FrameGenerator::singleshotSetVolume() {
    core->singleshot->setVolume(ui->singleshotVolume->value());
}

void FrameGenerator::musicNext() {
    core->music->next();
}


/*!
 * Toggles music playing state
 */
void FrameGenerator::musicPlayPause() {
    if(music_play_pause->isChecked()) {
        core->music->play();
    } else {
        core->music->pause();
    }
}


void FrameGenerator::musicTrackChanged() {
    QStringList tag = core->music->container.at(0)->getTagList();

    QString title;
    QString artist;
    QString album;

    if(tag.first().isEmpty()) {
        QFileInfo tag_file(core->music->container.at(0)->getCurrentFilename());
        title = tag_file.fileName();
    } else {
        title = tr("<b>%1</b> ").arg(tag.first());
    }

    if(!tag.at(1).isEmpty()) {
        artist = tr("by <i>%2</i> ").arg(tag.at(1));
    }

    if(!tag.last().isEmpty()) {
        album = tr("from <i>%3</i>").arg(tag.last());
    }


    ui->musicTagLabel->setText(QString("%1%2%3").arg(title, artist, album));
}

void FrameGenerator::musicSetPlaylist(QModelIndex index) {
    musicSetPlaylist(index.row());
}


void FrameGenerator::musicSetPlaylist(int index) {
    core->music->objects_tracks_model->selectObject(core->music->objects_model->data(core->music->objects_model->index(index, 0)).toInt());
}

void FrameGenerator::musicSetVolume() {
    core->music->setVolume(ui->musicVolume->value());
}


void FrameGenerator::musicSetSeek(int time, int length) {
    ui->musicSeek->setMaximum(length);
    ui->musicSeek->setValue(time);
}
