#ifndef SINGLESHOTLIBRARYVIEW_H
#define SINGLESHOTLIBRARYVIEW_H

#include <QtCore>
#include <QtGui>

class SingleshotLibraryView : public QTableView
{
    Q_OBJECT
public:
    explicit SingleshotLibraryView(QWidget *parent = 0);

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // SINGLESHOTLIBRARYVIEW_H
