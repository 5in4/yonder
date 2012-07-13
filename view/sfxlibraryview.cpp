#include "sfxlibraryview.h"

SfxLibraryView::SfxLibraryView(QWidget *parent) : QTableView(parent)
{
}

void SfxLibraryView::startDrag(QMouseEvent *event) {
    QModelIndexList indices = this->selectedIndexes();
    QByteArray tracks;
    for(int i=0;i<indices.length(); i++) {
        int row = 0;
        tracks.append(this->model()->data(indices.at(i)).toByteArray());
        tracks.append(" ");
        }
    tracks.remove(tracks.length() - 1, 1);

    if(tracks.isEmpty()) {
        return;
    }

    QMimeData *mimedata = new QMimeData();
    mimedata->setData("application/sg-sfx-library-reference", tracks);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    QPixmap pixmap(this->style()->standardPixmap(QStyle::SP_FileIcon));
    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void SfxLibraryView::mouseMoveEvent(QMouseEvent *event) {
    this->startDrag(event);
}
