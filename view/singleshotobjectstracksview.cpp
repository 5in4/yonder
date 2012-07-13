#include "singleshotobjectstracksview.h"

SingleshotObjectsTracksView::SingleshotObjectsTracksView(QWidget *parent) :
    QTableView(parent)
{
    library_mime_type = "application/sg-singleshot-library-reference";
}

QModelIndexList SingleshotObjectsTracksView::getSelectedIndexes() {
    return this->selectedIndexes();
}


void SingleshotObjectsTracksView::startDrag(QMouseEvent *event) {
    QModelIndexList indices = this->selectedIndexes();
    QByteArray tracks;
    for(int i=0;i<indices.length(); i++) {
        int row = 0;
        tracks.append(this->model()->data(indices.at(i)).toByteArray());
        tracks.append(" ");
        }
    tracks.remove(tracks.length() - 1, 1);

    QMimeData *mimedata = new QMimeData();
    mimedata->setData("application/sg-singleshot-library-reference-drag", tracks);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    QPixmap pixmap(this->style()->standardPixmap(QStyle::SP_FileIcon));
    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void SingleshotObjectsTracksView::mouseMoveEvent(QMouseEvent *event) {
    this->startDrag(event);
}


void SingleshotObjectsTracksView::dragEnterEvent(QDragEnterEvent *event) {
    if(custom_model->oid == 0) {
        event->ignore();
        return;
    }
    if(event->mimeData()->hasFormat(library_mime_type)) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void SingleshotObjectsTracksView::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat(library_mime_type)) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void SingleshotObjectsTracksView::dropEvent(QDropEvent *event) {
    if(event->mimeData()->hasFormat(library_mime_type)) {

        QByteArray data(event->mimeData()->data(library_mime_type));
        QString data_parse(data);
        QStringList tracks = data_parse.split(" ");
        custom_model->database().transaction();
        for(int i=0; i<tracks.length(); i++) {
            custom_model->insertTrackIntoObject(tracks.at(i).toInt());
            this->resizeColumnsToContents();
            this->resizeRowsToContents();
        }
        custom_model->database().commit();
        custom_model->select();
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
    event->ignore();
}
