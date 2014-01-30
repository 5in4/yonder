#include "solidtabwidget.h"

SolidTabWidget::SolidTabWidget(QWidget *parent) :
    QFrame(parent)
{
    widget_width = 110;
    sidebar_layout = new QVBoxLayout(this);
    tab_active = -1;

    webapp_running = new SolidTabButton(widget_width, this);
    webapp_running->setText("Webapp\nrunning");
    webapp_running->setCheckable(false);
    webapp_running->hide();

    progress_bar = new QProgressBar(this);
    progress_bar->setTextVisible(false);
    progress_bar->setMinimum(0);
    progress_bar->setMaximum(0);
    progress_bar->setValue(0);
    progress_bar->hide();

    applyStylesheet(true);

    quit_button = new SolidTabButton(widget_width, this);
    quit_button->setText(tr("Quit"));
    quit_button->setShortcut(tr("Ctrl+Q"));
    quit_button->setToolTip(tr("Quit Yonder\nCtrl+Q"));

}



/*!
 * Adds a tab with the given page and label to the tab widget, and returns the index of the tab in the tab bar.
 */
int SolidTabWidget::addTab(FrameObject *page, const QString &label) {
    return this->addTab(page, label, QString(""), QIcon());
}


int SolidTabWidget::addTab(FrameObject *page, const QString &label, const QString &shortcut) {
    return this->addTab(page, label, shortcut, QIcon());
}

/*!
 * Adds a tab with the given page, icon, and label to the tab widget, and returns the index of the tab in the tab bar.
 */
int SolidTabWidget::addTab(FrameObject *page, const QString &label, const QIcon &icon) {
    return this->addTab(page, label, QString(""), icon);
}


int SolidTabWidget::addTab(FrameObject *page, const QString &label, const QString &shortcut, const QIcon &icon) {
    tabs.append(new SolidTabButton(widget_width, this));
    tabs.last()->setText(label);
    tabs.last()->setIcon(icon);
    tabs.last()->setPage(page);
    tabs.last()->setPosition(tabs.count() - 1);

    if(!shortcut.isEmpty()) {
        tabs.last()->setShortcut(shortcut);
        tabs.last()->setToolTip(tr("Shortcut: %1").arg(shortcut));
    }

    connect(tabs.last(), SIGNAL(clicked(int)), this, SLOT(updateUi(int)));
    page->hide();
    this->populate();
    return tabs.count() - 1;
}


void SolidTabWidget::populate() {
    while(sidebar_layout->count() > 0) {
        sidebar_layout->removeItem(sidebar_layout->takeAt(0));
    }

    for(int i=0; tabs.count() > i; i++) {
        sidebar_layout->addWidget(tabs.at(i));
    }
    sidebar_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    sidebar_layout->addWidget(webapp_running);
    sidebar_layout->addWidget(progress_bar);
    sidebar_layout->addWidget(quit_button);
    sidebar_layout->update();
}


void SolidTabWidget::updateUi(int position) {
//    QSettings settings;
//    if(settings.value("Settings/lock_menu", 0).toBool()) {
//        if(QMessageBox::warning(this, tr("Menu locked"), tr("Sound is playing right now. Leaving Generator mode will stop all playback. Do you wish to proceed?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
//            return;
//        }
//    }

    // Set to position, if it's new
    if(tab_active != position) {
        for(int i=0; tabs.count() > i; i++) {
            if(tabs.at(i)->getPage()->isVisible()) {
                tabs.at(i)->getPage()->setVisible(false);
            }
        }

        tabs.at(position)->getPage()->setVisible(true);
        emit tabActivated(tabs.at(position)->getPage());
    }

    // Set all buttons to correct state
    for(int i=0; tabs.count() > i; i++) {
        tabs.at(i)->setChecked(false);
    }
    tabs.at(position)->setChecked(true);

    tab_active = position;
}


void SolidTabWidget::setActive(int position) {
    this->updateUi(position);
}


void SolidTabWidget::setTabEnabled(int position, bool enabled) {
    tabs.at(position)->setEnabled(enabled);
}


void SolidTabWidget::applyStylesheet(bool apply) {
    if(apply) {
        sidebar_layout->setMargin(0);
        this->setMaximumWidth(widget_width);
    } else {
        sidebar_layout->setMargin(12);
        this->setMaximumWidth(widget_width + 2 * sidebar_layout->margin());
    }
}






SolidTabButton::SolidTabButton(int widget_width, QWidget *parent) :
    QToolButton(parent)
{
    this->setCheckable(true);
    this->setMinimumSize(QSize(widget_width, 60));
    this->setLayoutDirection(Qt::LeftToRight);
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->setCursor(Qt::PointingHandCursor);
    connect(this, SIGNAL(clicked()), this, SLOT(emitClicked()));
}


void SolidTabButton::setPage(FrameObject *page) {
    this->page = page;
}


FrameObject* SolidTabButton::getPage() {
    return page;
}


void SolidTabButton::setPosition(int position) {
    this->position = position;
}


int SolidTabButton::getPosition() {
    return position;
}

void SolidTabButton::emitClicked() {
    emit clicked(position);
}
