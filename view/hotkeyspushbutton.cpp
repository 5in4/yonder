#include "hotkeyspushbutton.h"

HotkeysPushButton::HotkeysPushButton(int pos_in_array, QWidget *parent): AGQPushButton(pos_in_array, parent)
{

}


void HotkeysPushButton::enterEvent(QEvent *event) {
    if(oid != 0) {
        emit highlightButtons(oid, true);
    }
    QPushButton::enterEvent(event);
}


void HotkeysPushButton::leaveEvent(QEvent *event) {
    if(oid != 0) {
        emit highlightButtons(oid, false);
    }
    QPushButton::leaveEvent(event);
}


QSize HotkeysPushButton::sizeHint() const {
    QTextDocument rich_text_label;
    rich_text_label.setHtml(htmlText);
    return QSize(rich_text_label.size().width(), 30);
}
