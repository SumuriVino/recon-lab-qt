#include "select_source_interface.h"
#include "ui_select_source_interface.h"

select_source_interface::select_source_interface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select_source_interface)
{
    ui->setupUi(this);

}

select_source_interface::~select_source_interface()
{
    delete ui;
}

void select_source_interface::pub_put_prevoius_selected_source_list(QStringList source_count_name_list)
{
    prevoius_selected_src_count_name_list = source_count_name_list;
}

void select_source_interface::pub_set_ui_essentials()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    ui->tableWidget_source->setColumnWidth(enum_tablewidget_item_column_checkbox,30);
    ui->tableWidget_source->setColumnWidth(enum_tablewidget_item_column_source_count_name,80);
    ui->tableWidget_source->setColumnWidth(enum_tablewidget_item_column_complete_source_name,300);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void select_source_interface::pub_insert_data_in_table()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QStringList all_available_src_cnt_nm_list;

    ui->tableWidget_source->setRowCount(0);
    QList<struct_GLOBAL_witness_info_source> list_target_source_info =  global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int row_count = 0; row_count < list_target_source_info.size() ; row_count++)
    {
        ui->tableWidget_source->insertRow(row_count);


        QString source_count_name_str = list_target_source_info.at(row_count).source_count_name;
        QString complete_source_name_str = list_target_source_info.at(row_count).complete_source_name;
        all_available_src_cnt_nm_list << source_count_name_str;



        ui->tableWidget_source->setRowHeight(row_count, 22);

        QTableWidgetItem *item_checkbox_icon = new QTableWidgetItem;
        if(prevoius_selected_src_count_name_list.contains(source_count_name_str))
            item_checkbox_icon->setCheckState(Qt::Checked);
        else
            item_checkbox_icon->setCheckState(Qt::Unchecked);

        ui->tableWidget_source->setItem(row_count , enum_tablewidget_item_column_checkbox , item_checkbox_icon);



        QTableWidgetItem *item_src_cnt_nm = new QTableWidgetItem;
        item_src_cnt_nm->setText(source_count_name_str);
        ui->tableWidget_source->setItem(row_count , enum_tablewidget_item_column_source_count_name , item_src_cnt_nm);


        QTableWidgetItem *item_complete_source_nm = new QTableWidgetItem;
        item_complete_source_nm->setText(complete_source_name_str);
        ui->tableWidget_source->setItem(row_count , 2 , item_complete_source_nm);

    }


    if(all_available_src_cnt_nm_list == prevoius_selected_src_count_name_list)
        ui->checkBox_all_source->setChecked(true);
    else
        ui->checkBox_all_source->setChecked(false);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void select_source_interface::on_pushButton_ok_clicked()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);


    QStringList selected_source_count_nm_list;
    for(int ii = 0 ; ii < ui->tableWidget_source->rowCount() ; ii++)
    {
        if(ui->tableWidget_source->item(ii,enum_tablewidget_item_column_checkbox)->checkState() == 2)
        {
            selected_source_count_nm_list << ui->tableWidget_source->item(ii , enum_tablewidget_item_column_source_count_name)->text();
        }
        else
            continue ;
    }

    //    if(selected_source_count_nm_list.size() < 1)
    //    {
    //        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please select atleast one source");
    //        return ;
    //    }

    hide();


    emit signal_selected_source_list(selected_source_count_nm_list);

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);

}

void select_source_interface::on_pushButton_cancel_clicked()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    hide();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);

}

bool select_source_interface::is_all_source_selected()
{
    bool bool_all_checked = true;
    for(int row = 0; row < ui->tableWidget_source->rowCount(); row++)
    {
        QTableWidgetItem *item = ui->tableWidget_source->item(row, enum_tablewidget_item_column_checkbox);
        if(item->checkState() == Qt::Unchecked)
        {
            bool_all_checked = false;
            break;
        }
    }

    return bool_all_checked;
}

void select_source_interface::on_checkBox_all_source_clicked(bool checked)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    if(checked)
    {
        for(int ii = 0 ; ii < ui->tableWidget_source->rowCount() ; ii++)
        {
            QTableWidgetItem *item_checkbox_icon = new QTableWidgetItem;
            item_checkbox_icon->setText("");
            item_checkbox_icon->setCheckState(Qt::Checked);
            ui->tableWidget_source->setItem(ii , enum_tablewidget_item_column_checkbox , item_checkbox_icon);
        }
    }
    else
    {
        for(int ii = 0 ; ii < ui->tableWidget_source->rowCount() ; ii++)
        {
            QTableWidgetItem *item_checkbox_icon = new QTableWidgetItem;
            item_checkbox_icon->setText("");
            item_checkbox_icon->setCheckState(Qt::Unchecked);
            ui->tableWidget_source->setItem(ii , enum_tablewidget_item_column_checkbox , item_checkbox_icon);

        }

    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void select_source_interface::on_tableWidget_source_cellClicked(int row, int column)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    if(column == enum_tablewidget_item_column_checkbox)
    {
        if(is_all_source_selected())
        {
            ui->checkBox_all_source->setChecked(true);
        }
        else
        {
            ui->checkBox_all_source->setChecked(false);
        }
    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}
