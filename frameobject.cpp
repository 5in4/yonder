#include "frameobject.h"

FrameObject::FrameObject(QWidget *parent) :
    QFrame(parent)
{
}


void FrameObject::setVisible(bool visible) {
    if(visible == true) {
//        qDebug() << "frame activated";
        emit activated();
    } else {
//        qDebug() << "frame deactivated";
        emit deactivated();
    }
    QFrame::setVisible(visible);
}


void FrameObject::refresh() {
    qDebug() << "refreshing FrameObject";
}
