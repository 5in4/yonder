#include "frameobject.h"

FrameObject::FrameObject(QWidget *parent) :
    QFrame(parent)
{
}


void FrameObject::setVisible(bool visible) {
    if(visible == true) {
        emit activated();
    } else {
        emit deactivated();
    }
    QFrame::setVisible(visible);
}


void FrameObject::refresh() {
    qDebug() << "refreshing FrameObject";
}
