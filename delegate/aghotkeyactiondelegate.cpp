#include "aghotkeyactiondelegate.h"

AGHotkeyActionDelegate::AGHotkeyActionDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void AGHotkeyActionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

    QSqlQuery query(QSqlDatabase::database());
    QString query_string = QString("SELECT id, \"atmosphere_objects\" AS `table`, name FROM atmosphere_objects WHERE `table` = (SELECT section FROM hotkeys_objects_actions WHERE `id` = %1) AND `id` = (SELECT aid FROM hotkeys_objects_actions WHERE `id` = %1) UNION SELECT id, \"music_objects\" AS `table`, name FROM music_objects WHERE `table` = (SELECT section FROM hotkeys_objects_actions WHERE `id` = %1) AND `id` = (SELECT aid FROM hotkeys_objects_actions WHERE `id` = %1) UNION SELECT id, \"sfx_objects\" AS `table`, name FROM sfx_objects WHERE `table` = (SELECT section FROM hotkeys_objects_actions WHERE `id` = %1) AND `id` = (SELECT aid FROM hotkeys_objects_actions WHERE `id` = %1) UNION SELECT id, \"singleshot_objects\" AS `table`, name FROM singleshot_objects WHERE `table` = (SELECT section FROM hotkeys_objects_actions WHERE `id` = %1) AND `id` = (SELECT aid FROM hotkeys_objects_actions WHERE `id` = %1) UNION SELECT id, \"special_objects\" AS `table`, name FROM special_objects WHERE `table` = (SELECT section FROM hotkeys_objects_actions WHERE `id` = %1) AND `id` = (SELECT aid FROM hotkeys_objects_actions WHERE `id` = %1)").arg(index.data().toInt());
    query.prepare(query_string);
    query.exec();
    query.first();

    QString manager("Special action");
    if(query.value(1) == "atmosphere_objects") {
        manager = tr("Atmosphere");
    }
    if(query.value(1) == "music_objects") {
        manager = tr("Music playlist");
    }
    if(query.value(1) == "singleshot_objects") {
        manager = tr("Singleshot");
    }
    if(query.value(1) == "sfx_objects") {
        manager = tr("Sfx");
    }

    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);
    QString draw_string = QString(" %1: %2").arg(manager, query.value(2).toString());
    painter->save();
    painter->drawText(option.rect, Qt::AlignVCenter, draw_string);
    painter->restore();
    //return QStyledItemDelegate::paint(painter, option, index);
}

//QSize AGHotkeyActionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    return QStyledItemDelegate::sizeHint(option, index);
//}
