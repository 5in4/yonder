#ifndef SOLIDTABWIDGET_H
#define SOLIDTABWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QList>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QString>
#include <QProgressBar>
#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>

#include <QDebug>

#include <frameobject.h>

class SolidTabButton : public QToolButton
{
    Q_OBJECT
public:
    explicit SolidTabButton(int widget_width = 110, QWidget *parent = 0);

    void setPage(FrameObject *page);
    FrameObject *getPage();

    void setPosition(int position);
    int getPosition();

public slots:
    void emitClicked();

signals:
    void clicked(int position);

private:
    FrameObject *page;
    int position;
};


class SolidTabWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SolidTabWidget(QWidget *parent = 0);

    QList<SolidTabButton*> tabs;
    QVBoxLayout* sidebar_layout;

    QProgressBar* progress_bar;
    SolidTabButton* webapp_running;
    SolidTabButton* quit_button;

    int tab_active;

    int widget_width;

    int addTab(FrameObject *page, const QString &label);
    int addTab(FrameObject *page, const QString &label, const QString &shortcut);
    int addTab(FrameObject *page, const QString &label, const QIcon &icon);
    int addTab(FrameObject *page, const QString &label, const QString &shortcut, const QIcon &icon);

    void setActive(int position);
    void setTabEnabled(int position, bool enabled);

    void applyStylesheet(bool apply);


private:
    void populate();

signals:
    void tabActivated(FrameObject *page);
    
public slots:
    void updateUi(int position);
    
};

#endif // SOLIDTABWIDGET_H
