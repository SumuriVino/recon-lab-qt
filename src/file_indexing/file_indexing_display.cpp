#include "file_indexing_display.h"
#include "ui_file_indexing_display.h"

file_indexing_display::file_indexing_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file_indexing_display)
{
    ui->setupUi(this);

    //ui->pushButton_remove->setIcon(QIcon("../icons/signs/remove.png"));

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->tableWidget->hideColumn(enum_bookmark_column_index);
    ui->tableWidget->hideColumn(enum_fs_record_column_index);
    ui->tableWidget->hideColumn(enum_source_count_column_index);
    ui->tableWidget->hideColumn(enum_INT_index);

    ui->tableWidget->setColumnWidth(enum_bookmark_column_index, 30);
    ui->tableWidget->setColumnWidth(enum_filepath_column_index, 350);
    ui->tableWidget->setColumnWidth(enum_source_name_index, 300);

    ui->tableWidget->setColumnWidth(enum_status_column_index, 160);
    ui->tableWidget->setColumnWidth(enum_fs_record_column_index, 140);
    ui->tableWidget->setColumnWidth(enum_source_count_column_index, 160);


}

file_indexing_display::~file_indexing_display()
{
    delete ui;
}

void file_indexing_display::pub_set_essentials()
{ // set essentials database paths
    indexing_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";
}

void file_indexing_display::pub_display_selected_files()
{ // display selected files which are going to be indexed
    prepare_selected_files_display();
}


void file_indexing_display::on_pushButton_remove_clicked()
{ // remove button clicked to remove files from text indexing queue
    QModelIndexList selection_model_list = ui->tableWidget->selectionModel()->selectedRows();

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(ui->tableWidget->isRowHidden(selected_row_no))
            continue;

        QTableWidgetItem *item_fs_record = ui->tableWidget->item(selected_row_no, enum_fs_record_column_index);
        QTableWidgetItem *item_filepath  = ui->tableWidget->item(selected_row_no, enum_filepath_column_index);
        QTableWidgetItem *item_source_count = ui->tableWidget->item(selected_row_no, enum_source_count_column_index);

        QString command = QString("Delete From files Where fs_record = ? AND file_path = ? AND source_count_name = ?");

        QStringList value_list;
        value_list << item_fs_record->text().trimmed() << item_filepath->text().trimmed() <<  item_source_count->text().trimmed();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);

    }

    prepare_selected_files_display();

}

void file_indexing_display::on_pushButton_done_clicked()
{ // done button clicked after text indexing done
    emit signal_done_clicked();
}

void file_indexing_display::prepare_selected_files_display()
{ // prepare selected file display in tablwidget from databse which are select for text indexing
    ui->checkBox_show_processed_files->setChecked(false);
    ui->tableWidget->setRowCount(0);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(indexing_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command = QString("Select bookmark,fs_record,file_path,source_count_name,status,INT From files");

    QSqlQuery query_select(destination_db);

    query_select.prepare(command);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" select query ----FAILED---" + query_select.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + query_select.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    qint64 row_count = 0;

    while(query_select.next())
    {
        ui->tableWidget->insertRow(row_count);
        ui->tableWidget->setRowHeight(row_count, 23);

        QString bookmark_str  = query_select.value(0).toString().trimmed();
        QString fs_record_str = query_select.value(1).toString().trimmed();
        QString filepath_str  = query_select.value(2).toString().trimmed();
        QString source_count_str = query_select.value(3).toString().trimmed();
        QString status_str    = query_select.value(4).toString().trimmed();
        QString INT_str       = query_select.value(5).toString().trimmed();


        struct_GLOBAL_witness_info_source struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_str);
        QString source_name = struct_target_source_info.source_name.trimmed();

        QTableWidgetItem *item_bookmark  = new QTableWidgetItem;
        QTableWidgetItem *item_fs_record = new QTableWidgetItem;
        QTableWidgetItem *item_filepath  = new QTableWidgetItem;
        QTableWidgetItem *item_source_count = new QTableWidgetItem;
        QTableWidgetItem *item_status    = new QTableWidgetItem;
        QTableWidgetItem *item_source_name    = new QTableWidgetItem;
        QTableWidgetItem *item_INT    = new QTableWidgetItem;

        if(bookmark_str == QString::number(1))
            item_bookmark->setCheckState(Qt::Checked);
        else
            item_bookmark->setCheckState(Qt::Unchecked);


        item_fs_record->setText(fs_record_str);
        item_filepath->setText(filepath_str);
        item_source_count->setText(source_count_str);
        item_source_name->setText(source_name);

        if(status_str == "1")
        {
            item_status->setText(QString("Processed"));
            ui->tableWidget->hideRow(row_count);
        }
        else
            item_status->setText(QString("Not Processed"));

        item_INT->setData(Qt::EditRole,INT_str);

        ui->tableWidget->setItem(row_count, enum_bookmark_column_index , item_bookmark);
        ui->tableWidget->setItem(row_count, enum_filepath_column_index , item_filepath);
        ui->tableWidget->setItem(row_count, enum_status_column_index , item_status);
        ui->tableWidget->setItem(row_count, enum_source_name_index , item_source_name);
        ui->tableWidget->setItem(row_count, enum_fs_record_column_index , item_fs_record);
        ui->tableWidget->setItem(row_count, enum_source_count_column_index , item_source_count);
        ui->tableWidget->setItem(row_count, enum_INT_index , item_INT);

        row_count++;
    }

    ui->tableWidget->sortByColumn(enum_INT_index, Qt::DescendingOrder);

    destination_db.close();

}

void file_indexing_display::on_tableWidget_cellClicked(int row, int column)
{ // on tablewidget generally clicked on cells
    if(row < 0)
        return;

    if(column == enum_bookmark_column_index)
    {
        QTableWidgetItem *item_bookmark = ui->tableWidget->item(row, enum_bookmark_column_index);

        QTableWidgetItem *item_fs_record = ui->tableWidget->item(row, enum_fs_record_column_index);
        QTableWidgetItem *item_filepath  = ui->tableWidget->item(row, enum_filepath_column_index);
        QTableWidgetItem *item_source_count = ui->tableWidget->item(row, enum_source_count_column_index);

        QString command = QString("Update files set bookmark = ? Where fs_record = ? AND file_path = ? AND source_count_name = ?");

        QString bookmark_value;

        if(item_bookmark->checkState())
        {
            bookmark_value = QString::number(1);
        }
        else
        {
            bookmark_value = QString::number(0);
        }

        QStringList value_list;
        value_list << bookmark_value << item_fs_record->text().trimmed() << item_filepath->text().trimmed() <<  item_source_count->text().trimmed();

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, value_list, indexing_db_path, Q_FUNC_INFO);
    }

    QTableWidgetItem *item_filepath  = ui->tableWidget->item(row, enum_filepath_column_index);
    QString status_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select status from files Where file_path = ?"), QStringList(item_filepath->text().trimmed()), 0, indexing_db_path, Q_FUNC_INFO);
    status_str = status_str.trimmed();

    if(status_str == QString::number(1))
        ui->pushButton_remove->setEnabled(false);
    else
        ui->pushButton_remove->setEnabled(true);


}

void file_indexing_display::on_checkBox_show_processed_files_clicked(bool checked)
{ // check show processod files clicked which will show those files which are already indexed

    for(int row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, enum_status_column_index);
        if(item->text().trimmed() == QString("Processed"))
        {
            if(checked)
                ui->tableWidget->showRow(row);
            else
                ui->tableWidget->hideRow(row);
        }
    }
}
