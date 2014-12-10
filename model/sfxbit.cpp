#include "sfxbit.h"

SfxBit::SfxBit(QDjangoModel *parent) : QDjangoModel(parent) {
    setForeignKey("container", new SfxContainer(this));
}


SfxContainer *SfxBit::container() const {
    return qobject_cast<SfxContainer*>(foreignKey("container"));
}


void SfxBit::setContainer(SfxContainer *container) {
    setForeignKey("container", container);
}


int SfxBit::order() const {
    return _order;
}


void SfxBit::setOrder(const int &order) {
    _order = order;
}


void SfxBit::tracks() {
//    QDjangoQuerySet<Track> tracks;
//    tracks = tracks.filter(QDjangoWhere("sfx_bít", QDjangoWhere::Equals, this->pk()));
////    foreach(const Track &track, tracks) {
////        track_list.append(track);
////    }
//    _track_list = QList<Track>();
//    return _track_list;
}
