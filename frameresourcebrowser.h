#ifndef FRAMERESOURCEBROWSER_H
#define FRAMERESOURCEBROWSER_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QUrl>
#include <QFile>
#include <QSqlQuery>
#include <QDesktopServices>

#include <frameobject.h>

namespace Ui {
class FrameResourceBrowser;
}

class FrameResourceBrowser : public FrameObject
{
    Q_OBJECT
    
public:
    explicit FrameResourceBrowser(QWidget *parent = 0);
    ~FrameResourceBrowser();

    QString config_path;
    QSqlDatabase res_db;

    QSqlTableModel *filter_model;
    QSqlTableModel *resource_model;

public slots:
    void applyFilter();
    void itemClicked(QModelIndex index);
    void openPage(QUrl url);
    void openPage(QString url_string);
    
private:
    Ui::FrameResourceBrowser *ui;
};

#endif // FRAMERESOURCEBROWSER_H
