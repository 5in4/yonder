#ifndef HOTKEYSPUSHBUTTON_H
#define HOTKEYSPUSHBUTTON_H

#include <view/yonderpushbutton.h>

class HotkeysPushButton: public YonderPushButton
{
    Q_OBJECT
public:
    HotkeysPushButton(int pos_in_array, QWidget *parent);

    QSize sizeHint() const;
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

public slots:

signals:
    void highlightButtons(int oid, bool highlight);

private:


};

#endif // HOTKEYSPUSHBUTTON_H
