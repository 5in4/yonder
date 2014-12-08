#ifndef SFXBITTRACK_H
#define SFXBITTRACK_H

#include <QDjangoModel.h>
#include <QDjangoQuerySet.h>

#include "model/sfxbit.h"
#include "model/track.h"


class SfxBitTrack : public QDjangoModel {

    Q_OBJECT
    Q_PROPERTY(SfxBit* sfx_bit READ sfxBit WRITE setSfxBit NOTIFY sfxBitChanged)
    Q_PROPERTY(Track* track READ track WRITE setTrack NOTIFY trackChanged)

public:
    explicit SfxBitTrack(QDjangoModel *parent = 0);

    SfxBit *sfxBit() const;
    void setSfxBit(SfxBit *sfx_bit);

    Track *track() const;
    void setTrack(Track *track);
    void setTrack(int track_id);

signals:
    emit void sfxBitChanged();
    emit void trackChanged();
};

#endif // SFXBITTRACK_H
