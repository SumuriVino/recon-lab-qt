#include "remove_multiple_tag_selecotor.h"
#include "ui_remove_multiple_tag_selecotor.h"

remove_multiple_tag_selecotor::remove_multiple_tag_selecotor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::remove_multiple_tag_selecotor)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

remove_multiple_tag_selecotor::~remove_multiple_tag_selecotor()
{
    delete ui;
}

void remove_multiple_tag_selecotor::pub_set_initialisation(QStringList tag_list, QStringList icons_list, QString table, QString record_INT)
{
    recived_tag_list = tag_list;
    recived_icons_list = icons_list;
    table_name = table;
    record = record_INT;
    display_tag_table();
}

void remove_multiple_tag_selecotor::on_pushButton_remove_tag_clicked()
{
    QStringList list_remaining_tags;
    QStringList list_removed_tags;
    for(int i = 0; i < ui->tableWidget_remove_tag_list->rowCount(); i++)
    {
        if(i == 0)
            continue;

        if(ui->tableWidget_remove_tag_list->item(i,0)->checkState() != 0)
        {
            list_removed_tags << ui->tableWidget_remove_tag_list->item(i,0)->text();
            continue;
        }
        list_remaining_tags << ui->tableWidget_remove_tag_list->item(i,0)->text();
    }


    emit signal_tag_list_to_be_removed(list_removed_tags,table_name,record);

    emit signal_remaining_tags_list(list_remaining_tags,table_name,record);

    this->hide();
}

void remove_multiple_tag_selecotor::display_tag_table()
{
    while(ui->tableWidget_remove_tag_list->rowCount())
        ui->tableWidget_remove_tag_list->removeRow(0);

    recived_tag_list.insert(0,"Select All");
    recived_icons_list.insert(0,"../icons/tag_icons/multiple.png");

    for(int li = 0; li< recived_tag_list.size();li++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setCheckState(Qt::Checked);

        if(recived_icons_list.size() == recived_tag_list.size())
            item->setIcon(QIcon(recived_icons_list.at(li)));

        item->setText(recived_tag_list.at(li));


        ui->tableWidget_remove_tag_list->insertRow(li);
        ui->tableWidget_remove_tag_list->setRowHeight(li,26);
        ui->tableWidget_remove_tag_list->setItem(li,0,item);

    }
}

void remove_multiple_tag_selecotor::on_tableWidget_remove_tag_list_cellClicked(int row, int column)
{
    if(row == 0)
    {
        for(int i = 0; i < ui->tableWidget_remove_tag_list->rowCount(); i++)
        {
            if(ui->tableWidget_remove_tag_list->item(0,0)->checkState() == 0)
                ui->tableWidget_remove_tag_list->item(i,0)->setCheckState(Qt::Unchecked);
            else
                ui->tableWidget_remove_tag_list->item(i,0)->setCheckState(Qt::Checked);
        }
    }

}
