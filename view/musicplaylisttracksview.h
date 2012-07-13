#ifndef MUSICPLAYLISTTRACKSVIEW_H
#define MUSICPLAYLISTTRACKSVIEW_H

#include <QtCore>
#include <QtGui>

#include <model/objectstracksmodel.h>

class MusicPlaylistTracksView: public QTableView
{
Q_OBJECT
public:
    explicit MusicPlaylistTracksView(QWidget *parent = 0);
    QModelIndexList getSelectedIndexes();

    QString library_mime_type;
    ObjectsTracksModel *custom_model;

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // MUSICPLAYLISTTRACKSVIEW_H
