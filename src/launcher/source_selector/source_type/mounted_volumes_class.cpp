#include "mounted_volumes_class.h"
#include "ui_mounted_volumes_class.h"

Mounted_Volumes_Class::Mounted_Volumes_Class(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Mounted_Volumes_Class)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);


}

Mounted_Volumes_Class::~Mounted_Volumes_Class()
{
    delete ui;
}

void Mounted_Volumes_Class::pub_set_id_and_name(int id,QString name)
{
    received_id = id;
    received_name = name;
}

int Mounted_Volumes_Class::pub_get_id()
{
    return received_id;
}

int Mounted_Volumes_Class::pub_get_window_height()
{
    return this->minimumHeight();
}

void Mounted_Volumes_Class::pub_set_essentials()
{
    prepare_mounted_volumes_list();
}


void Mounted_Volumes_Class::prepare_mounted_volumes_list()
{
    while(ui->tableWidget_media_list->rowCount())
        ui->tableWidget_media_list->removeRow(0);

    //-Insert Very First Item in list.
    //    QTableWidgetItem *item_root = new QTableWidgetItem;
    //    item_root->setText("/");
    //    item_root->setCheckState(Qt::Unchecked);

    //    ui->tableWidget_media_list->insertRow(0);
    //    ui->tableWidget_media_list->setRowHeight(0,23);
    //    ui->tableWidget_media_list->setItem(0,0,item_root);


    QStringList list_mounted_media =  global_recon_helper_singular_class_obj->get_mounted_media_list(Q_FUNC_INFO);

    int row_cnt = 0;
    for(int ii = 0; ii < list_mounted_media.size(); ii++)
    {
        QString vol_name = list_mounted_media.at(ii).trimmed();

        if(vol_name.trimmed() == "/")
        {

#ifndef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
            continue;
#endif
        }

        QTableWidgetItem *item_volumes = new QTableWidgetItem;
        item_volumes->setText(vol_name);
        item_volumes->setCheckState(Qt::Unchecked);

        ui->tableWidget_media_list->insertRow(row_cnt);
        ui->tableWidget_media_list->setRowHeight(row_cnt,23);
        ui->tableWidget_media_list->setItem(row_cnt,0,item_volumes);

        row_cnt++;
    }

    ui->tableWidget_media_list->selectRow(0);
}

void Mounted_Volumes_Class::on_pushButton_Add_clicked()
{
    QStringList list_media;
    for(int i = 0; i < ui->tableWidget_media_list->rowCount(); i++)
    {
        if(ui->tableWidget_media_list->item(i,0)->checkState() == Qt::Unchecked)
            continue;
        list_media << ui->tableWidget_media_list->item(i,0)->text();
    }

    emit signal_mounted_volumes_selected(list_media);
    this->hide();
}

void Mounted_Volumes_Class::on_pushButton_Cancel_clicked()
{
    this->hide();
}


