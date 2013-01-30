#ifndef MUSICLIBRARYVIEW_H
#define MUSICLIBRARYVIEW_H

#include <QTableView>
#include <QMimeData>
#include <QDrag>

class MusicLibraryView: public QTableView
{
Q_OBJECT
public:
    MusicLibraryView(QWidget *parent);

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // MUSICLIBRARYVIEW_H
