#include "sqlite_viewer_manage_colums_type.h"
#include "ui_sqlite_viewer_manage_colums_type.h"

sqlite_viewer_manage_colums_type::sqlite_viewer_manage_colums_type(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::sqlite_viewer_manage_colums_type)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    setWindowTitle("Columns Selection");

}

sqlite_viewer_manage_colums_type::~sqlite_viewer_manage_colums_type()
{
    delete ui;
}


void sqlite_viewer_manage_colums_type::pub_set_widget_and_tab_name(QTableWidget *tableWidget_ptr,QString tab_name)
{
    ui->tabWidget->insertTab(0,tableWidget_ptr,tab_name);
    ui->tabWidget->setCurrentIndex(0);
}


