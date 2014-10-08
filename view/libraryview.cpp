#include "libraryview.h"


extern QString ACCEPTED_MIMETYPES;

LibraryView::LibraryView(QWidget *parent) : QTableView(parent) {
}


void LibraryView::setModel(QAbstractItemModel *model) {
    _model = (QDjangoTableModel<Track>*) model;
    QTableView::setModel(model);
}

void LibraryView::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat(mime_type_accepted)) {
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void LibraryView::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat(mime_type_accepted)) {
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}

void LibraryView::dropEvent(QDropEvent *event) {
    QStringList accepted = ACCEPTED_MIMETYPES.replace("*.", "").split(" ");
    if(event->mimeData()->hasFormat(mime_type_accepted)) {
        QList<QUrl> urls = event->mimeData()->urls();
        QList<QUrl>::iterator url;
        for(url=urls.begin(); url!=urls.end(); ++url) {
            QString path = url->toLocalFile();
            if(accepted.contains(path.split(".").last())) {
                _model->setData(_model->index(0, 0), QVariant(path), Qt::EditRole);
            }
        }
        event->acceptProposedAction();
        return;
    }
    event->ignore();
}
