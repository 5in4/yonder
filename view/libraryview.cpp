#include "libraryview.h"


extern QString ACCEPTED_MIMETYPES;

LibraryView::LibraryView(QWidget *parent) : QTableView(parent) {
}


void LibraryView::setModel(QAbstractItemModel *model) {
    setShowGrid(false);

    _header_vertical = new QHeaderView(Qt::Vertical, this);
    _header_vertical->setVisible(false);
    setVerticalHeader(_header_vertical);

    _header_horizontal = new QHeaderView(Qt::Horizontal, this);
    _header_horizontal->setSortIndicatorShown(true);
    _header_horizontal->setStretchLastSection(true);
    setHorizontalHeader(_header_horizontal);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    _model = (QDjangoTableModel<Track>*) model;
    QTableView::setModel(model);
    hideColumn(2);
    hideColumn(3);
    hideColumn(4);
    hideColumn(5);
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
