#ifndef LIBRARYVIEW_H
#define LIBRARYVIEW_H

#include <QTableView>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QHeaderView>

#include <QDebug>

#include "model/qdjangotablemodel.h"
#include "model/track.h"

class LibraryView : public QTableView
{
    Q_OBJECT
public:
    explicit LibraryView(QWidget *parent = 0);

    void setModel(QAbstractItemModel * model);

private:
    QString mime_type_accepted = "text/uri-list";
    QDjangoTableModel<Track> * _model;
    QHeaderView *_header_vertical;
    QHeaderView *_header_horizontal;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // LIBRARYVIEW_H
