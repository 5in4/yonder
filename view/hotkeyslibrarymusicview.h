#ifndef HOTKEYSLIBRARYMUSICVIEW_H
#define HOTKEYSLIBRARYMUSICVIEW_H

#include <QtCore>
#include <QtGui>

class HotkeysLibraryMusicView : public QTableView
{
    Q_OBJECT
public:
    explicit HotkeysLibraryMusicView(QWidget *parent = 0);

signals:

public slots:

protected:
    void startDrag(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // HOTKEYSLIBRARYMUSICVIEW_H
