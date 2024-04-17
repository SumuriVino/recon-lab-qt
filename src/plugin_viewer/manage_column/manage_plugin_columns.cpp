#include "manage_plugin_columns.h"
#include "ui_manage_plugin_columns.h"

manage_plugin_columns::manage_plugin_columns(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::manage_plugin_columns)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    setWindowModality(Qt::ApplicationModal);

    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_popup_horizontal_header_clicked(int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slot_on_tableWidget_popup_cellClicked(int,int)));

}

manage_plugin_columns::~manage_plugin_columns()
{
    delete ui;
}

void manage_plugin_columns::draw_table_manage_plugins_columns(QStringList var)
{
    QStringList hdrs_list;
    hdrs_list << "" << "Columns";

    QStringList columny_list;
    columny_list.clear();


    for(int i = 0; i < (var.size() - 1); i++)
    {
        columny_list << var.at(i);
    }

    ui->tableWidget->setRowCount(0);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(hdrs_list);

    if(popup_hid_col_name_lst_var.size() == columny_list.size())
    {
        ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
        ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
        ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

    }

    QTableWidgetItem *itemBookmark;
    QTableWidgetItem *itemcolName;

    int  count = 0;
    bool bool_any_item_unchecked = false;
    for(int i =0; i < columny_list.size(); i++)
    {
        itemBookmark = new QTableWidgetItem;
        itemcolName = new QTableWidgetItem;


        if(popup_hid_col_name_lst_var.contains(columny_list.at(i)))
        {
            itemBookmark->setCheckState(Qt::Unchecked);
            bool_any_item_unchecked = true;
        }
        else
        {
            itemBookmark->setCheckState(Qt::Checked);

        }

        itemcolName->setText(columny_list.at(i));

        ui->tableWidget->insertRow(count);
        ui->tableWidget->setRowHeight(count,25);
        ui->tableWidget->setItem(count,0,itemBookmark);
        ui->tableWidget->setItem(count,1,itemcolName);

        count++;
    }

    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,150);


    if(bool_any_item_unchecked)
    {

        ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
        ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
        ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Checked);
    }
}

void manage_plugin_columns::slot_manage_columns_checkstate()
{

    bool all_columns_checked = true;

    for(int row = 0 ; row < ui->tableWidget->rowCount(); row++)
    {
        if(ui->tableWidget->item(row,0)->checkState() == Qt::Unchecked)
        {
            all_columns_checked = false;
            break;
        }

    }


    if(all_columns_checked)
    {
        ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
        ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Checked);
    }
    else
    {
        ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
        ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);
    }

}

void manage_plugin_columns::slot_popup_horizontal_header_clicked(int column)
{
    if(column == 0)
    {
        if(ui->tableWidget->horizontalHeaderItem(0)->checkState())
        {

            ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
            ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

            for(int row = 0 ; row < ui->tableWidget->rowCount(); row++)
            {

                if(ui->tableWidget->item(row,0)->checkState() == Qt::Unchecked){
                    continue;
                }


                ui->tableWidget->item(row,0)->setCheckState(Qt::Unchecked);

                emit ui->tableWidget->cellClicked(row,0);
            }

        }
        else
        {

            ui->tableWidget->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
            ui->tableWidget->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

            for(int row = 0 ; row < ui->tableWidget->rowCount(); row++)
            {
                ui->tableWidget->item(row,0)->setCheckState(Qt::Checked);

                emit ui->tableWidget->cellClicked(row,0);
            }

        }
    }
}


void manage_plugin_columns::slot_on_tableWidget_popup_cellClicked(int row,int column)
{

    if(column == 0)
    {
        int check_status = ui->tableWidget->item(row,column)->checkState();
        emit signal_manage_column_clicked(row,check_status);
    }

}

