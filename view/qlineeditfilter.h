/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#ifndef QLINEEDITFILTER_H
#define QLINEEDITFILTER_H

#include <QToolButton>
#include <QStyle>
#include <QLineEdit>

class QToolButton;

class QLineEditFilter : public QLineEdit
{
    Q_OBJECT

public:
    QLineEditFilter(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateCloseButton(const QString &text);

private:
    QToolButton *clearButton;
};

#endif // QLINEEDITFILTER_H
