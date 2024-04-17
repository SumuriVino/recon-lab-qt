#include "sqlite_viewer_plugin_savestate.h"
#include "ui_sqlite_viewer_plugin_savestate.h"

sqlite_viewer_plugin_savestate::sqlite_viewer_plugin_savestate(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::sqlite_viewer_plugin_savestate)
{
    ui->setupUi(this);
}

sqlite_viewer_plugin_savestate::~sqlite_viewer_plugin_savestate()
{
    delete ui;
}


void sqlite_viewer_plugin_savestate::get_dbtable_list(QStringList tableList)
{
    list_Dbtable = tableList;
    displayTableData();
}

struct_global_sqlite_viewer_plugin_info sqlite_viewer_plugin_savestate::insertSelectedTableList_AND_CustomPath()
{
    listSelectedTable.clear();

    //- Get the list from the user for the plugins in the page.
    for(int row = 0; row < ui->tableWidget_dbTableList->rowCount(); row++)
    {
        if(ui->tableWidget_dbTableList->item(row,0)->checkState())
            listSelectedTable << ui->tableWidget_dbTableList->item(row,1)->text();
    }

    struct_global_sqlite_viewer_plugin_info obj_Info;
    obj_Info.selectedTableList = listSelectedTable;
    obj_Info.customPath = "";

    return obj_Info;
}



void sqlite_viewer_plugin_savestate::displayTableData()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);


    QTableWidgetItem *itemBookmark;
    QTableWidgetItem *itemTableName;

    int  count = 0;
    for(int i =0; i < list_Dbtable.size(); i++)
    {
        itemBookmark = new QTableWidgetItem;
        itemTableName = new QTableWidgetItem;

        itemBookmark->setCheckState(Qt::Checked);
        itemTableName->setText(list_Dbtable.at(i));

        ui->tableWidget_dbTableList->insertRow(count);
        ui->tableWidget_dbTableList->setRowHeight(count,25);
        ui->tableWidget_dbTableList->setItem(count,0,itemBookmark);
        ui->tableWidget_dbTableList->setItem(count,1,itemTableName);

        count++;
    }

    ui->tableWidget_dbTableList->setColumnWidth(0,50);
    recon_static_functions::app_debug("END",Q_FUNC_INFO);


}

void sqlite_viewer_plugin_savestate::on_pushButton_save_clicked()
{
    struct_global_sqlite_viewer_plugin_info obj_Info = insertSelectedTableList_AND_CustomPath();

    emit signal_export_as_plugin(obj_Info);

    this->hide();
}



