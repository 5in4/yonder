#ifndef SFXOBJECTSTRACKSVIEW_H
#define SFXOBJECTSTRACKSVIEW_H

#include <QtCore>
#include <QtGui>

#include <model/objectstracksmodel.h>

class SfxObjectsTracksView : public QTableView
{
    Q_OBJECT
public:
    explicit SfxObjectsTracksView(QWidget *parent = 0);
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

#endif // SFXOBJECTSTRACKSVIEW_H
