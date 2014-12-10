#include "sfxbittrack.h"

SfxBitTrack::SfxBitTrack(QDjangoModel *parent) : QDjangoModel(parent) {
    setForeignKey("sfx_bit", new SfxBit(this));
    setForeignKey("track", new Track(this));
}


SfxBit *SfxBitTrack::sfxBit() const {
    return qobject_cast<SfxBit*>(foreignKey("sfx_bit"));
}


void SfxBitTrack::setSfxBit(SfxBit *sfx_bit) {
    setForeignKey("sfx_bit", sfx_bit);
}

Track *SfxBitTrack::track() const {
    return qobject_cast<Track*>(foreignKey("track"));
}


void SfxBitTrack::setTrack(Track *track) {
    setForeignKey("track", track);
}


void SfxBitTrack::setTrack(int track_id) {
    QDjangoQuerySet<Track> track_query;
    Track *t = track_query.get(QDjangoWhere("id", QDjangoWhere::Equals, track_id));
    setTrack(t);
}
