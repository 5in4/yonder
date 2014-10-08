#include "sfxbit.h"

SfxBit::SfxBit(QDjangoModel *parent) : QDjangoModel(parent) {

}


SfxContainer *SfxBit::container() const {
    return qobject_cast<SfxContainer*>(foreignKey("container"));
}


void SfxBit::setContainer(SfxContainer *container) {
    setForeignKey("container", container);
}


qint16 SfxBit::order() const {
    return m_order;
}


void SfxBit::setOrder(const qint16 order) {
    m_order = order;
}


SfxBaseType *SfxBit::type() const {
    return qobject_cast<SfxBaseType*>(foreignKey("type"));
}


void SfxBit::setType(SfxBaseType *type) {
    setForeignKey("type", type);
}
