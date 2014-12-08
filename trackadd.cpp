#include "trackadd.h"
#include "ui_trackadd.h"

TrackAdd::TrackAdd(int sfx_bit_id, YonderCore *core, QWidget *parent) : QDialog(parent), ui(new Ui::TrackAdd) {
    ui->setupUi(this);

    this->core = core;
    this->sfx_bit_id = sfx_bit_id;

    connect(this, SIGNAL(accepted()), this, SLOT(sfxBitAddTracks()));

    ui->tracks->setModel(core->model_library);
}


void TrackAdd::sfxBitAddTracks() {
    QList<int> track_ids;

    QModelIndexList indexes = ui->tracks->selectionModel()->selectedIndexes();
    if(indexes.size() == 0) {
        return;
    }

    QModelIndexList::iterator iter;
    for(iter=indexes.begin(); iter!=indexes.end(); ++iter) {
        QModelIndex mi = *iter;
        if(mi.column() != 3) {
            continue;
        }
        track_ids.append(mi.data().toInt());
    }

    core->sfxBitAddTracks(sfx_bit_id, track_ids);
}


TrackAdd::~TrackAdd() {
    delete ui;
}
