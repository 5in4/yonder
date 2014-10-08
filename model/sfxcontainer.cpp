#include "sfxcontainer.h"

SfxContainer::SfxContainer(QDjangoModel *parent) {
}


QString SfxContainer::name() const {
    return _name;
}


void SfxContainer::setName(const QString &name) {
    _name = name;
}


bool SfxContainer::isPlaylist() const {
    return _is_playlist;
}


void SfxContainer::setIsPlaylist(const bool &is_playlist) {
    _is_playlist = is_playlist;
}
