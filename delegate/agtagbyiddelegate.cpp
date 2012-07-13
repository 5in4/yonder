#include "agtagbyiddelegate.h"

AGTagByIDDelegate::AGTagByIDDelegate(QString identifier, QTableView *parent) :
    QStyledItemDelegate(parent)
{
    this->identifier = identifier;
    this->parent = parent;
    this->formatting = "<b style=\"font-size: 12.5px;\">%1</b><br /><i style=\"font-size: 10px\">%2 (%3)</i>";
}


void AGTagByIDDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

    QTextDocument doc;
    QTextOption doc_opt;
    doc_opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    doc.setDefaultTextOption(doc_opt);

    QString paint_string = optionV4.text;
    QSqlQuery query;
    QString query_string = QString("SELECT `artist`, `album`, `title`, `path` FROM %1_library WHERE id = \"%2\"").arg(this->identifier, index.data().toString());
    query.exec(query_string);
    if(query.next()) {
        QString artist = QString(query.value(0).toString());
        QString album = QString(query.value(1).toString());
        QString track = QString(query.value(2).toString());
        if(artist == "") {
            artist = tr("Unknown artist");
        }
        if(album == "") {
            album = tr("Unknown album");
        }
        if(track == "") {
            track = query.value(3).toString();
        }
        paint_string = QString(formatting).arg(track, artist, album);
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


QSize AGTagByIDDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QStyleOptionViewItemV4 optionV4 = option;
//    initStyleOption(&optionV4, index);

//    QTextDocument doc;
//    QTextOption doc_opt;
//    doc_opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
//    doc.setDefaultTextOption(doc_opt);

//    QString paint_string = optionV4.text;
//    QSqlQuery query(QSqlDatabase::database("res_db"));
//    QString query_string = QString("SELECT `artist`, `album`, `title` FROM %1_library WHERE id = \"%2\"").arg(this->identifier, index.data().toString());
//    query.exec(query_string);
//    if(query.next()) {
//        paint_string = QString(formatting).arg(query.value(2).toString(), query.value(0).toString(), query.value(1).toString());
//    }
//    doc.setHtml(paint_string);
//    doc.setTextWidth(optionV4.rect.width());
//    qDebug() << QSize(doc.idealWidth(), doc.size().height());
    //return QSize(doc.idealWidth(), doc.size().height());
    return QSize(120, 34);
}
