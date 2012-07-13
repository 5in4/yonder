#include "qdoublespinboxdelegate.h"

QDoubleSpinBoxDelegate::QDoubleSpinBoxDelegate(int multiplier, QObject *parent): QStyledItemDelegate(parent)
{
    this->multiplier = multiplier;
}

QWidget *QDoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setDecimals(3);
    editor->setSuffix(this->tr("sec"));
    editor->setMinimum(0);
    editor->setMaximum(36000);

    return editor;
}


void QDoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble() / this->multiplier;

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}


void QDoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value() * this->multiplier;

    model->setData(index, value, Qt::EditRole);
    model->submit();
}

void QDoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void QDoubleSpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);
    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    painter->save();
    double paint_value = index.data().toDouble() / this->multiplier;
    QString paint_data = QString(this->tr("%1 sec")).arg(paint_value);
    painter->drawText(option.rect, Qt::AlignVCenter, paint_data);
    painter->restore();
}


QSize QDoubleSpinBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);

}
