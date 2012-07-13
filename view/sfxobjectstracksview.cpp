#include "sfxobjectstracksview.h"

SfxObjectsTracksView::SfxObjectsTracksView(QWidget *parent) : QTableView(parent)
{
    library_mime_type = "application/sg-sfx-library-reference";
}

QModelIndexList SfxObjectsTracksView::getSelectedIndexes() {
    return this->selectedIndexes();
}


void SfxObjectsTracksView::startDrag(QMouseEvent *event) {
    QModelIndexList indices = this->selectedIndexes();
    QByteArray tracks;
    for(int i=0;i<this->selectedIndexes().count(); i++) {
        int row = 0;
        qDebug() << this->model()->data(indices.at(i));

            if(indices.at(i).column() == 2) {
                tracks.append(this->model()->data(indices.at(i)).toByteArray());
                tracks.append(" ");
            }
        }
    tracks.remove(tracks.length() - 1, 1);

    QMimeData *mimedata = new QMimeData();
    mimedata->setData("application/sg-sfx-library-reference-drag", tracks);
    qDebug() << tracks;
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    QPixmap pixmap(this->style()->standardPixmap(QStyle::SP_FileIcon));
    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void SfxObjectsTracksView::mouseMoveEvent(QMouseEvent *event) {
    this->startDrag(event);
}


void SfxObjectsTracksView::dragEnterEvent(QDragEnterEvent *event) {
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

void SfxObjectsTracksView::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat(library_mime_type)) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void SfxObjectsTracksView::dropEvent(QDropEvent *event) {
    if(event->mimeData()->hasFormat(library_mime_type)) {

        QByteArray data(event->mimeData()->data(library_mime_type));
        QString data_parse(data);
        QStringList tracks = data_parse.split(" ");
                custom_model->database().transaction();
        for(int i=0; i<tracks.length(); i++) {
            qDebug() << tracks[i];
            custom_model->insertRow(custom_model->rowCount());
            custom_model->setData(custom_model->index(custom_model->rowCount() - 1, 1), custom_model->oid);
            custom_model->setData(custom_model->index(custom_model->rowCount() - 1, 2), tracks[i]);
            custom_model->submitAll();
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
