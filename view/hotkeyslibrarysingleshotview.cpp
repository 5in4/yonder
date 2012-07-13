#include "hotkeyslibrarysingleshotview.h"

HotkeysLibrarySingleshotView::HotkeysLibrarySingleshotView(QWidget *parent) :
    QTableView(parent)
{
}

void HotkeysLibrarySingleshotView::startDrag(QMouseEvent *event) {
    QModelIndexList indices = this->selectedIndexes();
    QByteArray objects;
    for(int i=0;i<indices.length(); i++) {
        objects.append(model()->index(indices.at(i).row(), 0).data().toByteArray());
        objects.append(" ");
    }
    objects.remove(objects.length() - 1, 1);
    //qDebug() << objects;

    QMimeData *mimedata = new QMimeData();
    mimedata->setData("application/sg-action-singleshot-reference", objects);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    QPixmap pixmap(this->style()->standardPixmap(QStyle::SP_FileIcon));
    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void HotkeysLibrarySingleshotView::mouseMoveEvent(QMouseEvent *event) {
    this->startDrag(event);
}
