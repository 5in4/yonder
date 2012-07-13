#ifndef RESOURCEBROWSER_H
#define RESOURCEBROWSER_H

#include <QtCore>
#include <QtSql>
#include <QDialog>

#include <delegate/agresourcedelegate.h>

namespace Ui {
    class ResourceBrowser;
}

class ResourceBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit ResourceBrowser(QString config_path, QWidget *parent = 0);
    ~ResourceBrowser();

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
    Ui::ResourceBrowser *ui;
};

#endif // RESOURCEBROWSER_H
