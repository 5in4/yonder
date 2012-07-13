#ifndef QDOUBLESPINBOXDELEGATE_H
#define QDOUBLESPINBOXDELEGATE_H

#include <QPainter>
#include <QDoubleSpinBox>
#include <QStyledItemDelegate>
#include <QApplication>

class QDoubleSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    QDoubleSpinBoxDelegate(int multiplier, QObject *parent = 0);

    int multiplier;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // QDOUBLESPINBOXDELEGATE_H
