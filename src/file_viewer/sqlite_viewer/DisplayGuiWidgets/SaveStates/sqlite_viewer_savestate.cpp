#include "sqlite_viewer_savestate.h"
#include "ui_sqlite_viewer_savestate.h"

sqlite_viewer_savestate::sqlite_viewer_savestate(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::sqlite_viewer_savestate)
{
    ui->setupUi(this);
    ui->checkBox_customPath->setChecked(false);
    ui->lineEdit_custom_Path->setEnabled(false);
    ui->pushButton_custom_path_open->setEnabled(false);

    connect(ui->tableWidget_dbTableList->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_tableWidget_horizontal_header_clicked(int)));
}

sqlite_viewer_savestate::~sqlite_viewer_savestate()
{
    delete ui;
}

void sqlite_viewer_savestate::get_dbtable_list(QStringList tableList)
{
    list_Dbtable = tableList;
    display_table_data();
}

struct_global_sqlite_viewer_plugin_info sqlite_viewer_savestate::insert_selected_table_list_and_custom_path()
{ // insert selected table list and custom paths in database
    listSelectedTable.clear();

    //- Get the list from the user for the plugins in the page.
    for(int row = 0; row < ui->tableWidget_dbTableList->rowCount(); row++)
    {
        if(ui->tableWidget_dbTableList->item(row,0)->checkState())
            listSelectedTable << ui->tableWidget_dbTableList->item(row,1)->text();
    }

    struct_global_sqlite_viewer_plugin_info obj_Info;
    obj_Info.selectedTableList = listSelectedTable;

    if(ui->checkBox_customPath->isChecked() && ui->lineEdit_custom_Path->text() != "")
        obj_Info.customPath = ui->lineEdit_custom_Path->text();
    else
        obj_Info.customPath = "";

    return obj_Info;
}

void sqlite_viewer_savestate::display_table_data()
{ // display table data on tablewidget
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QStringList hdrs_list;
    hdrs_list  << "" << "Tables";
    ui->tableWidget_dbTableList->setHorizontalHeaderLabels(hdrs_list);

    ui->tableWidget_dbTableList->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
    ui->tableWidget_dbTableList->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

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

void sqlite_viewer_savestate::on_pushButton_save_clicked()
{ // save button clicked to save the data in database
    if(destination_dir_path.endsWith("SavedStateSqlite"))
    {
        QMessageBox msgBox;
        msgBox.setText("The current result save state.");
        msgBox.setInformativeText("If saved on Default Path will Delete Previous Result. Do you want to alter same result ?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Save)
        {
            struct_global_sqlite_viewer_plugin_info obj_Info = insert_selected_table_list_and_custom_path();

            ///-1. For  Save as sqlite
            emit signal_sqlite_savestate_clicked(obj_Info);

            this->hide();
        }

        if(ret == QMessageBox::Cancel)
        {
            this->hide();
            return;
        }

        if(ret == QMessageBox::Discard)
        {
            return;
        }
    }
    else
    {
        struct_global_sqlite_viewer_plugin_info obj_Info = insert_selected_table_list_and_custom_path();

        ///-1. For  Save as sqlite
        emit signal_sqlite_savestate_clicked(obj_Info);

        this->hide();
    }
}

void sqlite_viewer_savestate::on_checkBox_customPath_toggled(bool checked)
{
    ui->lineEdit_custom_Path->setEnabled(checked);
    ui->pushButton_custom_path_open->setEnabled(checked);
}

void sqlite_viewer_savestate::on_pushButton_custom_path_open_clicked()
{// custom path select output directory
    QString home_dir = getenv("HOME");

    QString path = QFileDialog::getExistingDirectory(this, "Output Directory", home_dir, QFileDialog::ShowDirsOnly);

    if(path.trimmed() == QString(""))
        return;

    ui->lineEdit_custom_Path->setText(path);
}

void sqlite_viewer_savestate::slot_tableWidget_horizontal_header_clicked(int column)
{ // horizontal header
    if(column == 0)
    {
        if(ui->tableWidget_dbTableList->horizontalHeaderItem(0)->checkState())
        {
            ui->tableWidget_dbTableList->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
            ui->tableWidget_dbTableList->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

            for(int row = 0 ; row < ui->tableWidget_dbTableList->rowCount(); row++)
            {
                ui->tableWidget_dbTableList->item(row,0)->setCheckState(Qt::Unchecked);
            }
        }
        else
        {
            ui->tableWidget_dbTableList->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
            ui->tableWidget_dbTableList->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

            for(int row = 0 ; row < ui->tableWidget_dbTableList->rowCount(); row++)
            {
                ui->tableWidget_dbTableList->item(row,0)->setCheckState(Qt::Checked);
            }
        }
    }
}
