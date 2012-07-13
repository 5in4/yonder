#include "agresourcedelegate.h"

AGResourceDelegate::AGResourceDelegate(QTableView *parent) :
    QStyledItemDelegate(parent)
{
    this->parent = parent;
    formatting = "<b style=\"font-size: 14px;\">%1</b><p>%2</p><a style=\"font-size: 10.5px;\" href=\"%3\">%3</a>";
}

void AGResourceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

    QTextDocument doc;
    QTextOption doc_opt;
    doc_opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    doc.setDefaultTextOption(doc_opt);

    QString paint_string = optionV4.text;

    QSqlQuery query(QSqlDatabase::database("res_db"));
    QString query_string = QString("SELECT `url`, `title`, `description` FROM online_resources WHERE id = \"%1\"").arg(index.data().toString());
    query.exec(query_string);

    if(query.next()) {
        paint_string = QString(formatting).arg(query.value(1).toString(), query.value(2).toString(), query.value(0).toString());
    }

    doc.setHtml(paint_string);
    doc.setTextWidth(parent->width());


    /// Painting item without text
    optionV4.text = QString();

    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;



    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();

}

QSize AGResourceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    QTextOption doc_opt;
    doc_opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    doc.setDefaultTextOption(doc_opt);

    QString paint_string = optionV4.text;
    QSqlQuery query(QSqlDatabase::database("res_db"));
    QString query_string = QString("SELECT `url`, `title`, `description` FROM online_resources WHERE id = \"%1\"").arg(index.data().toString());
    query.exec(query_string);
    if(query.next()) {
        paint_string = QString(formatting).arg(query.value(1).toString(), query.value(2).toString(), query.value(0).toString());
    }
    doc.setHtml(paint_string);
    doc.setTextWidth(optionV4.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
//    return QSize(120, 120);
}

