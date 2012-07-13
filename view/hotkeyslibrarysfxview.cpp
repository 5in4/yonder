#include "hotkeyslibrarysfxview.h"

HotkeysLibrarySfxView::HotkeysLibrarySfxView(QWidget *parent) :
    QTableView(parent)
{
}

void HotkeysLibrarySfxView::startDrag(QMouseEvent *event) {
    QModelIndexList indices = this->selectedIndexes();
    QByteArray objects;
    for(int i=0;i<indices.length(); i++) {
        objects.append(model()->index(indices.at(i).row(), 0).data().toByteArray());
        objects.append(" ");
    }
    objects.remove(objects.length() - 1, 1);
    //qDebug() << objects;

    QMimeData *mimedata = new QMimeData();
    mimedata->setData("application/sg-action-sfx-reference", objects);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    QPixmap pixmap(this->style()->standardPixmap(QStyle::SP_FileIcon));
    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void HotkeysLibrarySfxView::mouseMoveEvent(QMouseEvent *event) {
    this->startDrag(event);
}
