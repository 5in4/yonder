#include "agpreviewpushbutton.h"

AGPreviewPushButton::AGPreviewPushButton(QWidget *parent) :
    QPushButton(parent)
{
    library_mime_type.append("application/sg-atmosphere-library-reference");
    library_mime_type.append("application/sg-atmosphere-library-reference-drag");
    library_mime_type.append("application/sg-sfx-library-reference");
    library_mime_type.append("application/sg-sfx-library-reference-drag");
    library_mime_type.append("application/sg-singleshot-library-reference");
    library_mime_type.append("application/sg-singleshot-library-reference-drag");
    library_mime_type.append("application/sg-music-library-reference");
    library_mime_type.append("application/sg-music-library-reference-drag");
}

void AGPreviewPushButton::dragEnterEvent(QDragEnterEvent *event) {
    foreach(QString type, library_mime_type) {
        if(event->mimeData()->hasFormat(type)) {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

void AGPreviewPushButton::dragMoveEvent(QDragMoveEvent *event) {
    foreach(QString type, library_mime_type) {
        if(event->mimeData()->hasFormat(type)) {
            QByteArray data(event->mimeData()->data(type));
            QString data_parse(data);
            if (!data_parse.contains(" ")) {
                event->acceptProposedAction();
                return;
            }
        }
    }
    event->ignore();
}

void AGPreviewPushButton::dropEvent(QDropEvent *event) {
    foreach(QString type, library_mime_type) {
        if(event->mimeData()->hasFormat(type)) {
            QByteArray data(event->mimeData()->data(type));
            QString data_parse(data);
            //qDebug() << data_parse << type;
            emit fileDropped(type, data_parse.toInt());
    //        QStringList tracks = data_parse.split(" ");
    //        custom_model->database().transaction();
    //        for(int i=0; i<tracks.length(); i++) {
    //             custom_model->insertTrackIntoObject(tracks.at(i).toInt());
    //            this->resizeColumnsToContents();
    //            this->resizeRowsToContents();
    //        }
    //        custom_model->database().commit();
    //        custom_model->select();

            event->acceptProposedAction();
        }
    }
    event->ignore();
}
