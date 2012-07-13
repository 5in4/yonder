#include <resourcebrowser.h>
#include <ui_resourcebrowser.h>

extern QString WEBADDRESS;

ResourceBrowser::ResourceBrowser(QString config_path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResourceBrowser)
{
    ui->setupUi(this);

    this->config_path = config_path;

    QString db_absolute_name = QString("%1/yonder_resource.db").arg(config_path);
    QFile res_db_dump_raw(QString(":/database/dump/resource.sql"));
    res_db_dump_raw.open(QIODevice::ReadOnly);

    res_db = QSqlDatabase::addDatabase("QSQLITE", "res_db");
    res_db.setDatabaseName(":memory:");
    res_db.open();

    QSqlQuery res_db_fill(res_db);
    char buf[2048];
    while (res_db_dump_raw.readLine(buf, sizeof(buf)) != -1) {
        QString res_db_dump(buf);
        res_db_fill.exec(res_db_dump);
    }

    res_db_dump_raw.close();

    resource_model = new QSqlTableModel(this, res_db);
    resource_model->setTable("online_resources");
    resource_model->setSort(2, Qt::AscendingOrder);
    resource_model->select();

    ui->resource_view->setModel(resource_model);
    ui->resource_view->setModelColumn(2);

    ui->checkbox_loop->setCheckState(Qt::PartiallyChecked);
    ui->checkbox_music->setCheckState(Qt::PartiallyChecked);
    ui->checkbox_sfx->setCheckState(Qt::PartiallyChecked);

    filter_model = new QSqlTableModel(this, res_db);
    filter_model->setTable("licenses");
    filter_model->select();
    ui->combobox_license->setModel(filter_model);
    ui->combobox_license->setModelColumn(1);
    ui->combobox_license->hide();

    connect(ui->intro_label, SIGNAL(linkActivated(QString)), this, SLOT(openPage(QString)));
    connect(ui->lineedit_search, SIGNAL(textChanged(QString)), this, SLOT(applyFilter()));
    connect(ui->checkbox_loop, SIGNAL(stateChanged(int)), this, SLOT(applyFilter()));
    connect(ui->checkbox_music, SIGNAL(stateChanged(int)), this, SLOT(applyFilter()));
    connect(ui->checkbox_sfx, SIGNAL(stateChanged(int)), this, SLOT(applyFilter()));
    connect(ui->resource_view, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
    connect(ui->resource_view, SIGNAL(activated(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
    connect(ui->resource_data_view, SIGNAL(anchorClicked(QUrl)), this, SLOT(openPage(QUrl)));
}

void ResourceBrowser::openPage(QUrl url) {
    QDesktopServices::openUrl(url);
}


void ResourceBrowser::openPage(QString url_string) {
    openPage(QUrl(url_string));
}


void ResourceBrowser::itemClicked(QModelIndex index) {
    QString resource_url = resource_model->index(index.row(), 1).data().toString();
    QString resource_title = resource_model->index(index.row(), 2).data().toString();
    QString resource_description = resource_model->index(index.row(), 3).data().toString();
    QString resource_artist = resource_model->index(index.row(), 5).data().toString();
    QString resource_template = QString("<h2>%1</h2><p><i>%2</i> <a href=\"%3\">visit site</a></p><p>%4</p>").arg(resource_title, resource_artist, resource_url, resource_description);
    ui->resource_data_view->setText(resource_template);
}


void ResourceBrowser::applyFilter() {
    QString filter;
    QStringList search_substrings = ui->lineedit_search->text().split(" ");
    foreach(QString sub, search_substrings) {
        if(sub.isEmpty()) {
            continue;
        }
        QString raw_filter = QString("(url LIKE \"%%1%\" OR title LIKE \"%%1%\" OR description LIKE \"%%1%\" OR artist LIKE \"%%1%\" OR tags LIKE \"%%1%\")").arg(sub);
        filter.append(raw_filter);
        filter.append(" AND ");
    }
    filter.remove(-5, 5);

    if(ui->checkbox_loop->checkState() != Qt::PartiallyChecked) {
        if(filter.length() != 0) {
            filter.append(" AND");
        }
        filter.append(QString(" (loop = \"%1\")").arg(ui->checkbox_loop->checkState()));
    }
    if(ui->checkbox_music->checkState() != Qt::PartiallyChecked) {
        if(filter.length() != 0) {
            filter.append(" AND");
        }
        filter.append(QString(" (music = \"%1\")").arg(ui->checkbox_music->checkState()));
    }
    if(ui->checkbox_sfx->checkState() != Qt::PartiallyChecked) {
        if(filter.length() != 0) {
            filter.append(" AND");
        }
        filter.append(QString(" (sfx = \"%1\")").arg(ui->checkbox_sfx->checkState()));
    }

    resource_model->setFilter(filter);
    resource_model->select();
    ui->resource_view->setCurrentIndex(resource_model->index(0,2));
}


ResourceBrowser::~ResourceBrowser()
{
    res_db.close();
    delete ui;
}
