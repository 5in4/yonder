#include "agqpushbutton.h"

AGQPushButton::AGQPushButton(int pos_in_array, QWidget *parent): QPushButton(parent)
{
    oid = 0;
    setPosInArray(pos_in_array);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(emitToggled(bool)));
}


/*!
 * Return position of object in the array it is created in
 */
int AGQPushButton::getPosInArray() {
    return pos_in_array;
}


/*!
 * Set position of object in the array it is created in
 */
void AGQPushButton::setPosInArray(int pos_in_array) {
    this->pos_in_array = pos_in_array;
}


/*!
 * Return object ID
 */
int AGQPushButton::getOID() {
    return oid;
}


/*!
 * Set object ID
 */
void AGQPushButton::setOID(int oid) {
    this->oid = oid;
}


/*!
 * Special toggled signal wich also emits pos_in_array
 */
void AGQPushButton::emitToggled(bool checked) {
    emit toggled(pos_in_array, checked);
}


/*!
 * Static unchecking without arguments slot
 */
void AGQPushButton::setUnChecked() {
    this->setChecked(false);
}


// Rich text styling

void AGQPushButton::setHtml(const QString &text)
{
    htmlText = text;
    isRichText = true;

//    QPalette palette;
//    palette.setBrush(QPalette::ButtonText, Qt::transparent);
//    setPalette(palette);
}

void AGQPushButton::setText(const QString &text)
{
    isRichText = false;
    QPushButton::setText(text);
}


QString AGQPushButton::text() const
{
    if (isRichText) {
        QTextDocument richText;
        richText.setHtml(htmlText);
        //return richText.toPlainText();
        return QString("");
    } else {
        return QPushButton::text();
    }
}

void AGQPushButton::paintEvent(QPaintEvent *event)
{
    if (isRichText) {
        QStylePainter p(this);

        QRect buttonRect = rect();
        QPoint point;

        QTextDocument richTextLabel;
        richTextLabel.setHtml(htmlText);

        QPixmap richTextPixmap(richTextLabel.size().width(), richTextLabel.size().height());
        richTextPixmap.fill(Qt::transparent);
        QPainter richTextPainter(&richTextPixmap);
        richTextLabel.drawContents(&richTextPainter, richTextPixmap.rect());

        if (!icon().isNull()) {
            //point = QPoint(buttonRect.x() + buttonRect.width() / 2 + iconSize().width() / 2 + 2, buttonRect.y() + buttonRect.height() / 2);
            point = QPoint(buttonRect.x() + richTextPixmap.width() / 2 + iconSize().width() + 2 , buttonRect.y() + buttonRect.height() / 2);
        } else {
            point = QPoint(buttonRect.x() + buttonRect.width() / 2 - 1, buttonRect.y() + buttonRect.height() / 2);
        }

        buttonRect.translate(point.x() - richTextPixmap.width() / 2, point.y() - richTextPixmap.height() / 2);

        p.drawControl(QStyle::CE_PushButton, getStyleOption());
        p.drawPixmap(buttonRect.left(), buttonRect.top(), richTextPixmap.width(), richTextPixmap.height(),richTextPixmap);
    } else {
        QPushButton::paintEvent(event);
    }
}

QStyleOptionButton AGQPushButton::getStyleOption() const
{
    QStyleOptionButton opt;
    opt.initFrom(this);
    opt.features = QStyleOptionButton::None;
    if (isFlat())
        opt.features |= QStyleOptionButton::Flat;
    if (menu())
        opt.features |= QStyleOptionButton::HasMenu;
    if (autoDefault() || isDefault())
        opt.features |= QStyleOptionButton::AutoDefaultButton;
    if (isDefault())
        opt.features |= QStyleOptionButton::DefaultButton;
    if (isDown() || (menu() && menu()->isVisible()))
        opt.state |= QStyle::State_Sunken;
    if (isChecked())
        opt.state |= QStyle::State_On;
    if (!isFlat() && !isDown())
        opt.state |= QStyle::State_Raised;
    opt.text = text();
    opt.icon = icon();
    opt.iconSize = iconSize();
    return opt;
}
