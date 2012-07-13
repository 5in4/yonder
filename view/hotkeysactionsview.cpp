#include "hotkeysactionsview.h"

HotkeysActionsView::HotkeysActionsView(QWidget *parent) :
    QTableView(parent)
{

}

QModelIndexList HotkeysActionsView::getSelectedIndexes() {
    return selectedIndexes();
}


void HotkeysActionsView::setOID(int oid) {
    this->oid = oid;
}

void HotkeysActionsView::dragEnterEvent(QDragEnterEvent *event) {
    if(oid == 0) {
        event->ignore();
        return;
    }
    if(event->mimeData()->hasFormat("application/sg-action-atmosphere-reference") || event->mimeData()->hasFormat("application/sg-action-sfx-reference") || event->mimeData()->hasFormat("application/sg-action-singleshot-reference") || event->mimeData()->hasFormat("application/sg-action-music-reference") || event->mimeData()->hasFormat("application/sg-action-special-reference")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void HotkeysActionsView::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat("application/sg-action-atmosphere-reference") || event->mimeData()->hasFormat("application/sg-action-sfx-reference") || event->mimeData()->hasFormat("application/sg-action-singleshot-reference") || event->mimeData()->hasFormat("application/sg-action-music-reference") || event->mimeData()->hasFormat("application/sg-action-special-reference")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void HotkeysActionsView::dropEvent(QDropEvent *event) {
    if(event->mimeData()->hasFormat("application/sg-action-atmosphere-reference")) {
        QByteArray data(event->mimeData()->data("application/sg-action-atmosphere-reference"));
        QString data_parse(data);
        QStringList objects = data_parse.split(" ");
        for(int i=0; i<objects.length(); i++) {
            model()->insertRow(model()->rowCount());
            model()->setData(model()->index(model()->rowCount() -1, 1), oid);
            model()->setData(model()->index(model()->rowCount() -1, 2), "atmosphere_objects");
            model()->setData(model()->index(model()->rowCount() -1, 3), objects.at(i));
            model()->submit();

        }
        event->acceptProposedAction();
    } else if(event->mimeData()->hasFormat("application/sg-action-sfx-reference")) {
        QByteArray data(event->mimeData()->data("application/sg-action-sfx-reference"));
        QString data_parse(data);
        QStringList objects = data_parse.split(" ");
        for(int i=0; i<objects.length(); i++) {
            model()->insertRow(model()->rowCount());
            model()->setData(model()->index(model()->rowCount() -1, 1), oid);
            model()->setData(model()->index(model()->rowCount() -1, 2), "sfx_objects");
            model()->setData(model()->index(model()->rowCount() -1, 3), objects.at(i));
            model()->submit();

        }
        event->acceptProposedAction();
    } else if(event->mimeData()->hasFormat("application/sg-action-singleshot-reference")) {
        QByteArray data(event->mimeData()->data("application/sg-action-singleshot-reference"));
        QString data_parse(data);
        QStringList objects = data_parse.split(" ");
        for(int i=0; i<objects.length(); i++) {
            model()->insertRow(model()->rowCount());
            model()->setData(model()->index(model()->rowCount() -1, 1), oid);
            model()->setData(model()->index(model()->rowCount() -1, 2), "singleshot_objects");
            model()->setData(model()->index(model()->rowCount() -1, 3), objects.at(i));
            model()->submit();

        }
        event->acceptProposedAction();
    } else if(event->mimeData()->hasFormat("application/sg-action-music-reference")) {
        QByteArray data(event->mimeData()->data("application/sg-action-music-reference"));
        QString data_parse(data);
        QStringList objects = data_parse.split(" ");
        for(int i=0; i<objects.length(); i++) {
            model()->insertRow(model()->rowCount());
            model()->setData(model()->index(model()->rowCount() -1, 1), oid);
            model()->setData(model()->index(model()->rowCount() -1, 2), "music_objects");
            model()->setData(model()->index(model()->rowCount() -1, 3), objects.at(i));
            model()->submit();

        }
        event->acceptProposedAction();
    } else if(event->mimeData()->hasFormat("application/sg-action-special-reference")) {
        QByteArray data(event->mimeData()->data("application/sg-action-special-reference"));
        QString data_parse(data);
        QStringList objects = data_parse.split(" ");
        for(int i=0; i<objects.length(); i++) {
            model()->insertRow(model()->rowCount());
            model()->setData(model()->index(model()->rowCount() -1, 1), oid);
            model()->setData(model()->index(model()->rowCount() -1, 2), "special_objects");
            model()->setData(model()->index(model()->rowCount() -1, 3), objects.at(i));
            model()->submit();

        }
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
    event->ignore();
}
