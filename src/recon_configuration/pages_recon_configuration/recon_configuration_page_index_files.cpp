#include "recon_configuration_page_index_files.h"
#include "ui_recon_configuration_page_index_files.h"

recon_configuration_page_index_files::recon_configuration_page_index_files(QWidget *parent) : QWidget(parent),
    ui(new Ui::recon_confuguration_page_index_files)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->pushButton_add_whitelist_category->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_whitelist_category->setIcon(QIcon("../icons/signs/remove.png"));

    ui->pushButton_add_whitelist_extension->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_whitelist_extension->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_clipboard_whitelist_extension->setIcon(QIcon("../icons/signs/clipboard.png"));

    ui->pushButton_add_blacklist_category->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_blacklist_category->setIcon(QIcon("../icons/signs/remove.png"));

    ui->pushButton_add_blacklist_extension->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_remove_blacklist_extension->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_clipboard_copy_blacklist_extension->setIcon(QIcon("../icons/signs/clipboard.png"));


    ui->pushButton_filesize_save->setIcon(QIcon("../icons/signs/save.png"));


    //    ui->radioButton_whitelist->setAutoExclusive(false);
    //    ui->radioButton_whitelist->setChecked(true);
    //    ui->radioButton_whitelist->setAutoExclusive(true);

    ui->radioButton_all_files->setAutoExclusive(false);
    ui->radioButton_all_files->setChecked(true);
    ui->radioButton_all_files->setAutoExclusive(true);

    tabwidget_current_index = 0;


    QRegularExpression rx1("\\d+");
    QValidator *validator1 = new QRegularExpressionValidator(rx1, this);
    ui->lineEdit_size->setValidator(validator1);
    QStringList size_unit_list;
    size_unit_list << QString(MACRO_Generic_Filter_Condition_Bytes)
                   << QString(MACRO_Generic_Filter_Condition_KB)
                   << QString(MACRO_Generic_Filter_Condition_MB)
                   << QString(MACRO_Generic_Filter_Condition_GB)
                   << QString(MACRO_Generic_Filter_Condition_TB);

    ui->lineEdit_size->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->comboBox_size_unit->addItems(size_unit_list);
    ui->lineEdit_size->clear();

    ui->lineEdit_size->hide();
    ui->pushButton_filesize_save->hide();
    ui->label_size->hide();
    ui->comboBox_size_unit->hide();

}

recon_configuration_page_index_files::~recon_configuration_page_index_files()
{
    delete ui;
}


void recon_configuration_page_index_files::pub_set_caller_type(QString type)
{

    caller_type = type;
    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + QString("case_configuration/case_configuration.sqlite");
    }
    else if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");


    QString command_str   = QString("CREATE TABLE IF NOT EXISTS tbl_selected_text_index_filter (INT integer primary key, select_status varchar(5), selected_filter varchar(10))");
    recon_helper_standard_obj->execute_db_command_by_dbpath(command_str, destination_db_file, Q_FUNC_INFO);

    //    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    //    {
    //        ui->lineEdit_size->show();
    //        ui->label_size->show();
    //        ui->comboBox_size_unit->show();
    //    }

    //    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
    //    {
    //        ui->lineEdit_size->show();
    //        ui->label_size->show();
    //        ui->comboBox_size_unit->show();
    //    }

    // on_radioButton_whitelist_clicked(true);

    set_fields_for_default_selection();

}

void recon_configuration_page_index_files::pub_set_config_called_from_toolbar(bool status)
{
    if(status)
    {
        ui->lineEdit_size->show();
        ui->label_size->show();
        ui->comboBox_size_unit->show();
    }
}


void recon_configuration_page_index_files::pub_enable_index_files_display(bool status)
{
    setEnabled(status);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Use_Filter_bool, status);

    if(!status)
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, status);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, status);
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_All_Files_Check_Status_bool, status);

    }
    else
    {
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, ui->radioButton_whitelist->isChecked());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, ui->radioButton_blacklist->isChecked());
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_All_Files_Check_Status_bool, ui->radioButton_all_files->isChecked());

    }

}

void recon_configuration_page_index_files::pub_update_value_in_narad_muni()
{

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, ui->radioButton_whitelist->isChecked());
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, ui->radioButton_blacklist->isChecked());

    qint64 size = ui->lineEdit_size->text().toLongLong();

    if(size == 0)
        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Less_Than_Size_InBytes_qint64, 0);
    else
    {
        if(ui->comboBox_size_unit->currentIndex() == 0)
            size = size;
        else if (ui->comboBox_size_unit->currentIndex() == 1)
            size = size * MACRO_Generic_Value_KB_in_byte_int;
        else if (ui->comboBox_size_unit->currentIndex() == 2)
            size = size * MACRO_Generic_Value_MB_in_byte_int;
        else if (ui->comboBox_size_unit->currentIndex() == 3)
            size = size * MACRO_Generic_Value_GB_in_byte_int;
        else if (ui->comboBox_size_unit->currentIndex() == 4)
            size = size * MACRO_Generic_Value_TB_in_byte_int;


        global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Less_Than_Size_InBytes_qint64, size);
    }
}

void recon_configuration_page_index_files::on_pushButton_add_whitelist_category_clicked()
{
    int row_count = -1;

    if(tabwidget_current_index == 0)
    {
        row_count = ui->tableWidget_whitelist_category->rowCount();

        if(ui->tableWidget_whitelist_category->item(row_count - 1, 0) == NULL && row_count > 0)
            return;
    }

    if(tabwidget_current_index == 1)
    {
        row_count = ui->tableWidget_blacklist_category->rowCount();

        if(ui->tableWidget_blacklist_category->item(row_count - 1, 0) == NULL && row_count > 0)
            return;
    }



    lineedit_new_whitelist_and_blacklist_category = new QLineEdit(this);
    connect(lineedit_new_whitelist_and_blacklist_category, SIGNAL(returnPressed()), this, SLOT(slot_whitelist_new_category_line_edit_return_pressed()));

    if(tabwidget_current_index == 0)
    {
        ui->tableWidget_whitelist_category->insertRow(row_count);
        ui->tableWidget_whitelist_category->setRowHeight(row_count, 23);
        ui->tableWidget_whitelist_category->setCellWidget(row_count, 0, lineedit_new_whitelist_and_blacklist_category);
        ui->tableWidget_whitelist_category->setCurrentCell(row_count, 0);
        ui->tableWidget_whitelist_extensions->setRowCount(0);
    }

    if(tabwidget_current_index == 1)
    {
        ui->tableWidget_blacklist_category->insertRow(row_count);
        ui->tableWidget_blacklist_category->setRowHeight(row_count, 23);
        ui->tableWidget_blacklist_category->setCellWidget(row_count, 0, lineedit_new_whitelist_and_blacklist_category);
        ui->tableWidget_blacklist_category->setCurrentCell(row_count, 0);
        ui->tableWidget_blacklist_extensions->setRowCount(0);
    }

}

void recon_configuration_page_index_files::slot_whitelist_new_category_line_edit_return_pressed()
{
    QString new_category = lineedit_new_whitelist_and_blacklist_category->text().trimmed();
    if(new_category.trimmed().isEmpty() || db_table_category_name.trimmed().isEmpty())
        return;


    if(tabwidget_current_index == 0)
    {
        if(ui->tableWidget_whitelist_category->currentRow() < 0)
            return;
    }


    if(tabwidget_current_index == 1)
    {
        if(ui->tableWidget_blacklist_category->currentRow() < 0)
            return;
    }


    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_category);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);


    QString command_str = QString("INSERT INTO ") + db_table_category_name + (" (bookmark,category) values (?,?)");
    QStringList value_list;
    value_list << QString::number(0) << new_category;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    if(tabwidget_current_index == 0)
    {
        ui->tableWidget_whitelist_category->removeCellWidget(ui->tableWidget_whitelist_category->currentRow(), 0);
        ui->tableWidget_whitelist_category->setItem(ui->tableWidget_whitelist_category->currentRow(), 0, item);
    }
    if(tabwidget_current_index == 1)
    {
        ui->tableWidget_blacklist_category->removeCellWidget(ui->tableWidget_blacklist_category->currentRow(), 0);
        ui->tableWidget_blacklist_category->setItem(ui->tableWidget_blacklist_category->currentRow(), 0, item);
    }

}

void recon_configuration_page_index_files::on_pushButton_remove_whitelist_category_clicked()
{
    if(db_table_category_name.trimmed().isEmpty())
        return;

    int current_row = -1;

    if(tabwidget_current_index == 0)
    {
        current_row = ui->tableWidget_whitelist_category->currentRow();

        if(current_row < 0)
            return;

        if(ui->tableWidget_whitelist_category->item(current_row, 0) == NULL)
        {
            ui->tableWidget_whitelist_category->removeRow(current_row);
            ui->tableWidget_whitelist_category->setCurrentCell( -1, 0);

            return;
        }
    }


    if(tabwidget_current_index == 1)
    {
        current_row = ui->tableWidget_blacklist_category->currentRow();

        if(current_row < 0)
            return;

        if(ui->tableWidget_blacklist_category->item(current_row, 0) == NULL)
        {
            ui->tableWidget_blacklist_category->removeRow(current_row);
            ui->tableWidget_blacklist_category->setCurrentCell( -1, 0);
            return;
        }
    }

    if(current_row < 0)
        return;

    QString category_str;

    if(tabwidget_current_index == 0)
        category_str = ui->tableWidget_whitelist_category->item(current_row , 0)->text();

    if(tabwidget_current_index == 1)
        category_str = ui->tableWidget_blacklist_category->item(current_row , 0)->text();


    category_str = category_str.trimmed();

    QString command_str = QString("Delete From ") + db_table_category_name + (" Where category = ?");
    QStringList value_list;
    value_list << category_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, value_list, destination_db_file, Q_FUNC_INFO);


    command_str = QString("Delete From ") + db_table_extension_name + (" Where category = ?");

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, value_list, destination_db_file, Q_FUNC_INFO);

    prepare_whitelist_and_blacklist_category_tablewidget_display();

}

void recon_configuration_page_index_files::on_pushButton_add_whitelist_extension_clicked()
{

    int row_count = -1;

    if(tabwidget_current_index == 0)
    {
        row_count = ui->tableWidget_whitelist_extensions->rowCount();

        if(ui->tableWidget_whitelist_extensions->item(row_count - 1, 0) == NULL && row_count > 0)
            return;
    }

    if(tabwidget_current_index == 1)
    {
        row_count = ui->tableWidget_blacklist_extensions->rowCount();

        if(ui->tableWidget_blacklist_extensions->item(row_count - 1, 0) == NULL && row_count > 0)
            return;
    }

    if(row_count < 0)
        return;

    if(tabwidget_current_index == 0 && ui->tableWidget_whitelist_category->currentRow() < 0)
        return;

    if(tabwidget_current_index == 1 && ui->tableWidget_blacklist_category->currentRow() < 0)
        return;

    lineedit_new_whitelist_and_blacklist_extension = new QLineEdit(this);
    connect(lineedit_new_whitelist_and_blacklist_extension, SIGNAL(returnPressed()), this, SLOT(slot_whitelist_and_blacklist_new_extension_line_edit_return_pressed()));

    if(tabwidget_current_index == 0)
    {
        ui->tableWidget_whitelist_extensions->insertRow(row_count);
        ui->tableWidget_whitelist_extensions->setRowHeight(row_count, 23);
        ui->tableWidget_whitelist_extensions->setCellWidget(row_count, 0, lineedit_new_whitelist_and_blacklist_extension);
        ui->tableWidget_whitelist_extensions->setCurrentCell(row_count, 0);
    }

    if(tabwidget_current_index == 1)
    {
        ui->tableWidget_blacklist_extensions->insertRow(row_count);
        ui->tableWidget_blacklist_extensions->setRowHeight(row_count, 23);
        ui->tableWidget_blacklist_extensions->setCellWidget(row_count, 0, lineedit_new_whitelist_and_blacklist_extension);
        ui->tableWidget_blacklist_extensions->setCurrentCell(row_count, 0);
    }

}

void recon_configuration_page_index_files::slot_whitelist_and_blacklist_new_extension_line_edit_return_pressed()
{

    if(db_table_extension_name.trimmed().isEmpty())
        return;

    int current_row_category = -1;

    if(tabwidget_current_index == 0)
        current_row_category = ui->tableWidget_whitelist_category->currentRow();


    if(tabwidget_current_index == 1)
        current_row_category = ui->tableWidget_blacklist_category->currentRow();


    if(current_row_category < 0)
        return;

    QString category_str;

    if(tabwidget_current_index == 0)
        category_str = ui->tableWidget_whitelist_category->item(current_row_category , 0)->text().trimmed();

    if(tabwidget_current_index == 1)
        category_str = ui->tableWidget_blacklist_category->item(current_row_category , 0)->text().trimmed();

    if(category_str.isEmpty())
        return;

    QString new_extension = lineedit_new_whitelist_and_blacklist_extension->text().trimmed();

    if(new_extension.isEmpty())
        return;

    if(!new_extension.startsWith(QString(".")))
        new_extension.prepend(QString("."));

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_extension);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

    QString command_str = QString("INSERT INTO ") + db_table_extension_name + QString(" (bookmark,category,extension) values (?,?,?)");
    QStringList value_list;
    value_list << QString::number(0) << category_str << new_extension;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    if(tabwidget_current_index == 0)
    {
        ui->tableWidget_whitelist_extensions->removeCellWidget(ui->tableWidget_whitelist_extensions->currentRow(), 0);
        ui->tableWidget_whitelist_extensions->setItem(ui->tableWidget_whitelist_extensions->currentRow(), 0, item);
    }

    if(tabwidget_current_index == 1)
    {
        ui->tableWidget_blacklist_extensions->removeCellWidget(ui->tableWidget_blacklist_extensions->currentRow(), 0);
        ui->tableWidget_blacklist_extensions->setItem(ui->tableWidget_blacklist_extensions->currentRow(), 0, item);
    }

}

void recon_configuration_page_index_files::on_pushButton_remove_whitelist_extension_clicked()
{
    if(db_table_extension_name.trimmed().isEmpty())
        return;

    int current_row_category = -1;

    if(tabwidget_current_index == 0)
    {
        current_row_category = ui->tableWidget_whitelist_category->currentRow();
        if(current_row_category < 0)
            return;

        if(ui->tableWidget_whitelist_category->item(current_row_category, 0) == NULL)
        {
            ui->tableWidget_whitelist_category->removeRow(current_row_category);
            ui->tableWidget_whitelist_category->setCurrentCell( -1, 0);

            return;
        }
    }

    if(tabwidget_current_index == 1)
    {
        current_row_category = ui->tableWidget_blacklist_category->currentRow();
        if(current_row_category < 0)
            return;

        if(ui->tableWidget_blacklist_category->item(current_row_category, 0) == NULL)
        {
            ui->tableWidget_blacklist_category->removeRow(current_row_category);
            ui->tableWidget_blacklist_category->setCurrentCell( -1, 0);
            return;
        }
    }


    if(current_row_category < 0)
        return;

    QString category_str;

    if(tabwidget_current_index == 0)
    {
        category_str = ui->tableWidget_whitelist_category->item(current_row_category , 0)->text().trimmed();
    }

    if(tabwidget_current_index == 1)
        category_str = ui->tableWidget_blacklist_category->item(current_row_category , 0)->text().trimmed();

    if(category_str.isEmpty())
        return;


    int current_row_extension = -1;
    if(tabwidget_current_index == 0)
    {
        current_row_extension = ui->tableWidget_whitelist_extensions->currentRow();

        if(current_row_extension < 0)
            return;

        if(ui->tableWidget_whitelist_extensions->item(current_row_extension, 0) == NULL)
        {
            ui->tableWidget_whitelist_extensions->removeRow(current_row_extension);
            ui->tableWidget_whitelist_extensions->setCurrentCell( -1, 0);
            return;
        }

    }

    if(tabwidget_current_index == 1)
    {
        current_row_extension = ui->tableWidget_blacklist_extensions->currentRow();

        if(current_row_extension < 0)
            return;

        if(ui->tableWidget_blacklist_extensions->item(current_row_extension, 0) == NULL)
        {
            ui->tableWidget_blacklist_extensions->removeRow(current_row_extension);
            ui->tableWidget_blacklist_extensions->setCurrentCell( -1, 0);
            return;
        }
    }

    if(current_row_extension < 0)
        return;

    QString extension_str;
    if(tabwidget_current_index == 0)
        extension_str = ui->tableWidget_whitelist_extensions->item(current_row_extension , 0)->text();

    if(tabwidget_current_index == 1)
        extension_str = ui->tableWidget_blacklist_extensions->item(current_row_extension , 0)->text();

    extension_str = extension_str.trimmed();

    if(extension_str.isEmpty())
        return;

    QString command_str = QString("Delete from ") + db_table_extension_name + (" Where extension = ? AND category = ?");

    QStringList value_list;
    value_list << extension_str << category_str;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, value_list, destination_db_file, Q_FUNC_INFO);

    prepare_whitelist_and_blacklist_category_tablewidget_display();
}

void recon_configuration_page_index_files::on_pushButton_add_blacklist_category_clicked()
{
    on_pushButton_add_whitelist_category_clicked();
}

void recon_configuration_page_index_files::on_pushButton_remove_blacklist_category_clicked()
{
    on_pushButton_remove_whitelist_category_clicked();
}

void recon_configuration_page_index_files::on_pushButton_add_blacklist_extension_clicked()
{
    on_pushButton_add_whitelist_extension_clicked();
}

void recon_configuration_page_index_files::on_pushButton_remove_blacklist_extension_clicked()
{
    on_pushButton_remove_whitelist_extension_clicked();
}

void recon_configuration_page_index_files::set_table_name_on_stackwidget_index_change(int index)
{
    tabwidget_current_index = index;

    if(index == 0)
    {
        db_table_category_name  = QString("tbl_index_files_whitelist_category");
        db_table_extension_name = QString("tbl_index_files_whitelist_extension");
    }
    else if(index == 1)
    {
        db_table_category_name  = QString("tbl_index_files_blacklist_category");
        db_table_extension_name = QString("tbl_index_files_blacklist_extension");
    }
    else
    {
        db_table_category_name  = QString("");
        db_table_extension_name = QString("");
    }

    db_table_category_name  = db_table_category_name.trimmed();
    db_table_extension_name = db_table_extension_name.trimmed();

}

void recon_configuration_page_index_files::on_radioButton_whitelist_clicked(bool checked)
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, checked);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, !checked);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_All_Files_Check_Status_bool, !checked);

    clear_and_disable_whitelist_and_blacklist_for_all_files(false);

    ui->stackedWidget->setCurrentIndex(0);
    set_table_name_on_stackwidget_index_change(0);
    prepare_whitelist_and_blacklist_category_tablewidget_display();

    ui->tableWidget_whitelist_extensions->setRowCount(0);
    prepare_whitelist_and_blacklist_extension_tablewidget_display();

    insert_selected_text_index_filter_status(1);

}

void recon_configuration_page_index_files::on_radioButton_blacklist_clicked(bool checked)
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, checked);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, !checked);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_All_Files_Check_Status_bool, !checked);

    clear_and_disable_whitelist_and_blacklist_for_all_files(false);

    ui->stackedWidget->setCurrentIndex(1);
    set_table_name_on_stackwidget_index_change(1);
    prepare_whitelist_and_blacklist_category_tablewidget_display();
    ui->tableWidget_blacklist_extensions->setRowCount(0);

    prepare_whitelist_and_blacklist_extension_tablewidget_display();
    insert_selected_text_index_filter_status(2);

}

void recon_configuration_page_index_files::on_tableWidget_whitelist_category_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    prepare_whitelist_and_blacklist_extension_tablewidget_display();
}

void recon_configuration_page_index_files::on_tableWidget_blacklist_category_cellClicked(int row, int column)
{
    on_tableWidget_whitelist_category_cellClicked(row , column);
}


void recon_configuration_page_index_files::prepare_whitelist_and_blacklist_category_tablewidget_display()
{

    if(db_table_category_name.trimmed().isEmpty())
        return;

    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command = QString("select category from ") + db_table_category_name;

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(tabwidget_current_index == 0)
        ui->tableWidget_whitelist_category->setRowCount(0);

    if(tabwidget_current_index == 1)
        ui->tableWidget_blacklist_category->setRowCount(0);

    int row = 0;
    while(query.next())
    {

        if(tabwidget_current_index == 0)
        {
            ui->tableWidget_whitelist_category->insertRow(row);
            ui->tableWidget_whitelist_category->setRowHeight(row, 23);
        }

        if(tabwidget_current_index == 1)
        {
            ui->tableWidget_blacklist_category->insertRow(row);
            ui->tableWidget_blacklist_category->setRowHeight(row, 23);
        }

        QTableWidgetItem *item_category  = new QTableWidgetItem;
        item_category->setText(query.value(0).toString().trimmed());

        if(tabwidget_current_index == 0)
        {
            ui->tableWidget_whitelist_category->setItem(row , 0  , item_category);
        }

        if(tabwidget_current_index == 1)
        {
            ui->tableWidget_blacklist_category->setItem(row , 0  , item_category);
        }

        row++;
    }

    destination_db.close();

    if(tabwidget_current_index == 0)
    {
        if(ui->tableWidget_whitelist_category->rowCount() > 0)
            ui->tableWidget_whitelist_category->selectRow(0);
    }

    if(tabwidget_current_index == 1)
    {
        if(ui->tableWidget_blacklist_category->rowCount() > 0)
            ui->tableWidget_blacklist_category->selectRow(0);
    }


    prepare_whitelist_and_blacklist_extension_tablewidget_display();


}

void recon_configuration_page_index_files::prepare_whitelist_and_blacklist_extension_tablewidget_display()
{
    int category_current_row = -1;

    if(tabwidget_current_index == 0)
    {
        category_current_row = ui->tableWidget_whitelist_category->currentRow();
        if(category_current_row < 0)
        {
            ui->tableWidget_whitelist_extensions->setRowCount(0);
            return;
        }
    }

    if(tabwidget_current_index == 1)
    {
        category_current_row = ui->tableWidget_blacklist_category->currentRow();
        if(category_current_row < 0)
        {
            ui->tableWidget_blacklist_extensions->setRowCount(0);
            return;
        }
    }

    QString category_str;

    if(tabwidget_current_index == 0)
        category_str = ui->tableWidget_whitelist_category->item(category_current_row, 0)->text().trimmed();

    if(tabwidget_current_index == 1)
        category_str = ui->tableWidget_blacklist_category->item(category_current_row, 0)->text().trimmed();


    if(category_str.isEmpty())
        return;


    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    if(db_table_extension_name.trimmed().isEmpty())
        return;


    QString command = QString("select extension from ") + db_table_extension_name + QString(" Where category = ?");

    QSqlQuery query(destination_db);
    query.prepare(command);
    query.addBindValue(category_str);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    if(tabwidget_current_index == 0)
        ui->tableWidget_whitelist_extensions->setRowCount(0);

    if(tabwidget_current_index == 1)
        ui->tableWidget_blacklist_extensions->setRowCount(0);

    int row = 0;
    while(query.next())
    {

        if(tabwidget_current_index == 0)
        {
            ui->tableWidget_whitelist_extensions->insertRow(row);
            ui->tableWidget_whitelist_extensions->setRowHeight(row, 23);
        }

        if(tabwidget_current_index == 1)
        {
            ui->tableWidget_blacklist_extensions->insertRow(row);
            ui->tableWidget_blacklist_extensions->setRowHeight(row, 23);
        }

        QTableWidgetItem *item_extension  = new QTableWidgetItem;
        item_extension->setText(query.value(0).toString().trimmed());

        if(tabwidget_current_index == 0)
        {
            ui->tableWidget_whitelist_extensions->setItem(row , 0  , item_extension);
        }

        if(tabwidget_current_index == 1)
        {
            ui->tableWidget_blacklist_extensions->setItem(row , 0  , item_extension);
        }

        row++;
    }



    destination_db.close();

    if(tabwidget_current_index == 0)
    {
        if(ui->tableWidget_whitelist_extensions->rowCount() > 0)
            ui->tableWidget_whitelist_extensions->selectRow(0);
    }

    if(tabwidget_current_index == 1)
    {
        if(ui->tableWidget_blacklist_extensions->rowCount() > 0)
            ui->tableWidget_blacklist_extensions->selectRow(0);
    }

}


void recon_configuration_page_index_files::on_lineEdit_size_returnPressed()
{
}

void recon_configuration_page_index_files::on_pushButton_filesize_save_clicked()
{
    on_lineEdit_size_returnPressed();
}

void recon_configuration_page_index_files::on_pushButton_clipboard_whitelist_extension_clicked()
{
    if(ui->tableWidget_whitelist_category->currentRow() < 0)
        return;

    QString category_str = ui->tableWidget_whitelist_category->item(ui->tableWidget_whitelist_category->currentRow(), 0)->text();

    QString clipboard_data =  QApplication::clipboard()->text();

    QStringList data_list = clipboard_data.split("\n", Qt::SkipEmptyParts);

    QStringList existing_white_list_extensions;

    for(int j = 0; j < ui->tableWidget_whitelist_extensions->rowCount(); j++)
    {
        QTableWidgetItem *item = ui->tableWidget_whitelist_extensions->item(j, 0);
        if(item != NULL)
            existing_white_list_extensions << item->text().trimmed();
    }

    if(data_list.size() > 0)
    {

        for(int i = 0 ; i < data_list.size(); i++)
        {
            QString new_extension = data_list.at(i);
            new_extension = new_extension.trimmed();

            if(new_extension.isEmpty())
                continue;

            if(!new_extension.startsWith(QString(".")))
                new_extension.prepend(QString("."));

            if(existing_white_list_extensions.contains(new_extension) || db_table_extension_name.trimmed().isEmpty())
                continue;

            QString command_str = QString("INSERT INTO ") + db_table_extension_name + QString(" (bookmark,category,extension) values (?,?,?)");
            QStringList value_list;
            value_list << QString::number(1) << category_str << new_extension;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
        }
    }

    prepare_whitelist_and_blacklist_extension_tablewidget_display();

}

void recon_configuration_page_index_files::on_pushButton_clipboard_copy_blacklist_extension_clicked()
{
    if(ui->tableWidget_blacklist_category->currentRow() < 0)
        return;

    QString category_str = ui->tableWidget_blacklist_category->item(ui->tableWidget_blacklist_category->currentRow(), 0)->text();

    QString clipboard_data =  QApplication::clipboard()->text();

    QStringList data_list = clipboard_data.split("\n", Qt::SkipEmptyParts);

    QStringList existing_black_list_extensions;

    for(int j = 0; j < ui->tableWidget_blacklist_extensions->rowCount(); j++)
    {
        QTableWidgetItem *item = ui->tableWidget_blacklist_extensions->item(j, 0);
        if(item != NULL)
            existing_black_list_extensions << item->text().trimmed();
    }

    if(data_list.size() > 0)
    {
        for(int i = 0 ; i < data_list.size(); i++)
        {
            QString new_extension = data_list.at(i);

            new_extension = new_extension.trimmed();
            if(new_extension.isEmpty())
                continue;

            if(!new_extension.startsWith(QString(".")))
                new_extension.prepend(QString("."));

            if(existing_black_list_extensions.contains(new_extension) || db_table_extension_name.trimmed().isEmpty())
                continue;

            QString command_str = QString("INSERT INTO ") + db_table_extension_name + QString(" (bookmark,category,extension) values (?,?,?)");
            QStringList value_list;
            value_list << QString::number(1) << category_str << new_extension;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);
        }
    }
    prepare_whitelist_and_blacklist_extension_tablewidget_display();

}

void recon_configuration_page_index_files::on_radioButton_all_files_clicked(bool checked)
{
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_All_Files_Check_Status_bool, checked);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Blacklist_Check_Status_bool, !checked);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Index_Files_Whitelist_Check_Status_bool, !checked);

    clear_and_disable_whitelist_and_blacklist_for_all_files(true);
    insert_selected_text_index_filter_status(0);

}

void recon_configuration_page_index_files::set_fields_for_default_selection()
{
    QString selected_index_filter = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select select_status from tbl_selected_text_index_filter"), 0, destination_db_file, Q_FUNC_INFO);

    /* selected_index_filter is 0 for All
     * selected_index_filter is 1 for WhiteList
     * selected_index_filter is 2 for BlackList
     */

    selected_index_filter = selected_index_filter.trimmed();
    if(selected_index_filter.isEmpty())
        selected_index_filter = QString::number(0);

    if(selected_index_filter == QString::number(0))
    {
        ui->radioButton_all_files->setChecked(true);
        on_radioButton_all_files_clicked(true);
    }
    else if(selected_index_filter == QString::number(1))
    {
        ui->radioButton_whitelist->setChecked(true);
        on_radioButton_whitelist_clicked(true);
    }
    else if(selected_index_filter == QString::number(2))
    {
        ui->radioButton_blacklist->setChecked(true);
        on_radioButton_blacklist_clicked(true);
    }
}

void recon_configuration_page_index_files::clear_and_disable_whitelist_and_blacklist_for_all_files(bool status)
{
    ui->tableWidget_blacklist_category->setEnabled(!status);
    ui->tableWidget_blacklist_extensions->setEnabled(!status);
    ui->tableWidget_whitelist_category->setEnabled(!status);
    ui->tableWidget_whitelist_extensions->setEnabled(!status);

    if(status)
    {
        ui->tableWidget_blacklist_category->setRowCount(0);
        ui->tableWidget_blacklist_extensions->setRowCount(0);

        ui->tableWidget_whitelist_category->setRowCount(0);
        ui->tableWidget_whitelist_extensions->setRowCount(0);

        ui->pushButton_add_blacklist_category->setEnabled(false);
        ui->pushButton_add_blacklist_extension->setEnabled(false);
        ui->pushButton_add_whitelist_category->setEnabled(false);
        ui->pushButton_add_whitelist_extension->setEnabled(false);

        ui->pushButton_remove_blacklist_category->setEnabled(false);
        ui->pushButton_remove_blacklist_extension->setEnabled(false);
        ui->pushButton_remove_whitelist_category->setEnabled(false);
        ui->pushButton_remove_whitelist_extension->setEnabled(false);

        ui->pushButton_clipboard_copy_blacklist_extension->setEnabled(false);
        ui->pushButton_clipboard_whitelist_extension->setEnabled(false);

    }
    else
    {
        ui->pushButton_add_blacklist_category->setEnabled(true);
        ui->pushButton_add_blacklist_extension->setEnabled(true);
        ui->pushButton_add_whitelist_category->setEnabled(true);
        ui->pushButton_add_whitelist_extension->setEnabled(true);

        ui->pushButton_remove_blacklist_category->setEnabled(true);
        ui->pushButton_remove_blacklist_extension->setEnabled(true);
        ui->pushButton_remove_whitelist_category->setEnabled(true);
        ui->pushButton_remove_whitelist_extension->setEnabled(true);

        ui->pushButton_clipboard_copy_blacklist_extension->setEnabled(true);
        ui->pushButton_clipboard_whitelist_extension->setEnabled(true);
    }

}

void recon_configuration_page_index_files::insert_selected_text_index_filter_status(int value)
{
    /* value = 0 for All
     * value = 1 for WhiteList
     * value = 2 for BlackList
     */

    QString selected_filter;
    if(value == 0)
    {
        selected_filter = "All" ;
    }
    else if(value == 1)
    {
        selected_filter = "WhiteList" ;
    }
    else if(value == 2)
    {
        selected_filter = "BlackList" ;
    }
    else
    {
        selected_filter.clear();
    }

    selected_filter = selected_filter.trimmed();

    if(selected_filter.isEmpty())
        return;

    QString cmd = QString("Delete from tbl_selected_text_index_filter");
    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, destination_db_file, Q_FUNC_INFO);

    cmd = QString("Insert into tbl_selected_text_index_filter (select_status,selected_filter) values(?,?)");
    QStringList value_list;
    value_list << QString::number(value) << selected_filter;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd , value_list, destination_db_file, Q_FUNC_INFO);

}
