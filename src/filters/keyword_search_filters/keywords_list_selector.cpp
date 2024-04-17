#include "keywords_list_selector.h"
#include "ui_keywords_list_selector.h"

keywords_list_selector::keywords_list_selector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keywords_list_selector)
{
    ui->setupUi(this);
    setModal(true);
    setWindowModality(Qt::WindowModal);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->lineEdit_insert_keywords->setAttribute(Qt::WA_MacShowFocusRect,0);
    ui->lineEdit_insert_list->setAttribute(Qt::WA_MacShowFocusRect,0);

    ui->lineEdit_insert_keywords->hide();
    ui->lineEdit_insert_list->hide();

    bool_delete_all_keywords_of_list_item = false;
    bool_update_keyword_list_item = false;
    bool_keyword_updated = false;
    selected_keywords_row = 0;
    selected_list_row = 0;

    ui->pushButton_keyword_clipboard->hide();

    QString stylesheet = QString("QTableView::item:selected:!active {"
                                 "color:white;"
                                 " }"
                                 );


    ui->tableWidget_keyword->setStyleSheet(stylesheet);
    ui->tableWidget_keyword_list->setStyleSheet(stylesheet);

    ui->pushButton_keyword_add->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_keyword_remove->setIcon(QIcon("../icons/signs/remove.png"));
    ui->pushButton_keyword_edit->setIcon(QIcon("../icons/signs/edit.png"));
    ui->pushButton_keyword_clipboard->setIcon(QIcon("../icons/signs/clipboard.png"));

    ui->pushButton_list_add->setIcon(QIcon("../icons/signs/add.png"));
    ui->pushButton_list_edit->setIcon(QIcon("../icons/signs/edit.png"));
    ui->pushButton_list_remove->setIcon(QIcon("../icons/signs/remove.png"));

    message_dialog_object = new message_dialog(this);


}

keywords_list_selector::~keywords_list_selector()
{
    delete ui;
}

void keywords_list_selector::pub_display_keyword_list()
{
    display_keyword_list_table();
}

void keywords_list_selector::on_pushButton_done_clicked()
{
    this->hide();
    emit signal_pushbotton_done_clicked();
}

void keywords_list_selector::on_lineEdit_insert_list_returnPressed()
{
    if(ui->lineEdit_insert_list->text().toLower().trimmed().isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please enter a category name");
        message_dialog_object->show();
        return;
    }

    ui->lineEdit_insert_list->hide();

    if(bool_update_keyword_list_item)
    {
        list_database_manager_function( MACRO_Keyword_List_Selector_DB_UPDATE_int);
    }
    else
        list_database_manager_function(MACRO_Keyword_List_Selector_DB_INSERT_int);

    display_keyword_list_table();
    if(bool_update_keyword_list_item)
    {
        ui->tableWidget_keyword_list->selectRow(selected_keywords_row);
        bool_update_keyword_list_item = false;
    }
    else
    {
        ui->tableWidget_keyword_list->selectRow(ui->tableWidget_keyword_list->rowCount() -1);
    }

    while(ui->tableWidget_keyword->rowCount())
    {
        ui->tableWidget_keyword->removeRow(0);
    }

    ui->lineEdit_insert_list->clear();
}

void keywords_list_selector::on_lineEdit_insert_keywords_returnPressed()
{
    if(ui->lineEdit_insert_keywords->text().toLower().trimmed().isEmpty())
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please enter a keyword name");
        message_dialog_object->show();
        return;
    }

    ui->lineEdit_insert_keywords->hide();

    if(!ui->tableWidget_keyword_list->rowCount() > 0)
        return;


    if(bool_keyword_updated)
        keywords_database_manager_function( MACRO_Keyword_List_Selector_DB_UPDATE_int);
    else
        keywords_database_manager_function(MACRO_Keyword_List_Selector_DB_INSERT_int);

    on_tableWidget_keyword_list_cellClicked(ui->tableWidget_keyword_list->currentRow(),1);

    if(bool_keyword_updated)
    {
        ui->tableWidget_keyword->selectRow(selected_keywords_row);
        bool_keyword_updated = false;
    }
    else
    {
        ui->tableWidget_keyword->selectRow(ui->tableWidget_keyword->rowCount() -1);
    }

    ui->lineEdit_insert_keywords->clear();
}

void keywords_list_selector::on_pushButton_list_add_clicked()
{
    ui->lineEdit_insert_list->clear();
    ui->lineEdit_insert_list->show();
    ui->lineEdit_insert_list->setFocus();
}

void keywords_list_selector::on_pushButton_keyword_add_clicked()
{
    if(!ui->tableWidget_keyword_list->rowCount() > 0)
        return;

    ui->lineEdit_insert_keywords->clear();
    ui->lineEdit_insert_keywords->show();
    ui->lineEdit_insert_keywords->setFocus();
}

void keywords_list_selector::list_database_manager_function(int operation_type)
{
    recon_static_functions::app_debug(" - START",Q_FUNC_INFO);

    QString recon_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_config_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    if(operation_type == MACRO_Keyword_List_Selector_DB_INSERT_int)
    {

        ///check if category already exist

        QString command_str = QString("Select category_status from keyword_category Where category = ?");
        QStringList value_list;
        value_list << ui->lineEdit_insert_list->text().toLower().trimmed();

        QString existing_category_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_str , value_list, 0, destination_db, Q_FUNC_INFO);
        existing_category_str = existing_category_str.trimmed();
        if(!existing_category_str.isEmpty())
        {
            /// Category already exist
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Category already exist.");
            message_dialog_object->show();
            return;
        }


        query.prepare("insert into keyword_category (category, category_status) values(?, ?)");

        query.addBindValue(ui->lineEdit_insert_list->text().toLower().trimmed());
        query.addBindValue("1");
    }
    else if(operation_type ==  MACRO_Keyword_List_Selector_DB_UPDATE_int)
    {
        query.prepare("update keyword_category set category = ? where category = ?");
        query.addBindValue(ui->lineEdit_insert_list->text().toLower().trimmed());
        query.addBindValue(ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed());
        if(!query.exec())
        {
            recon_static_functions::app_debug(" query.exec()  ------FAILED-----",Q_FUNC_INFO);
            recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
        }

        query.prepare("update keyword_list set category = ? where category = ?");
        query.addBindValue(ui->lineEdit_insert_list->text().toLower().trimmed());
        query.addBindValue(ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed());
    }
    else if(operation_type ==  MACRO_Keyword_List_Selector_DB_DELETE_int)
    {
        query.prepare("delete from keyword_category where category = ?");
        query.addBindValue(ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed());
    }

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    recon_static_functions::app_debug(" - END",Q_FUNC_INFO);
}

void keywords_list_selector::keywords_database_manager_function(int operation_type)
{
    recon_static_functions::app_debug("keywords_database_manager_function - START",Q_FUNC_INFO);

    QString recon_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_config_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    if(operation_type == MACRO_Keyword_List_Selector_DB_INSERT_int)
    {

        ///check if keyword already exist in same category
        QString command_str = QString("Select INT from keyword_list Where category = ? AND keyword = ?");
        QStringList value_list;
        value_list << ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed()
                   << ui->lineEdit_insert_keywords->text().toLower().trimmed();

        QString existing_keyword_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_str , value_list, 0, destination_db, Q_FUNC_INFO);
        existing_keyword_str = existing_keyword_str.trimmed();
        if(!existing_keyword_str.isEmpty())
        {
            /// keyword already exist
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Keyword already exist.");
            message_dialog_object->show();
            return;
        }

        query.prepare("insert into keyword_list (category, keyword, keyword_status) values(?, ?, ?)");

        query.addBindValue(ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed());
        query.addBindValue(ui->lineEdit_insert_keywords->text().toLower().trimmed());
        query.addBindValue("1");

    }
    else if(operation_type ==  MACRO_Keyword_List_Selector_DB_UPDATE_int)
    {
        query.prepare("update keyword_list set keyword = ? where keyword = ?");
        query.addBindValue(ui->lineEdit_insert_keywords->text().toLower().trimmed());
        query.addBindValue(ui->tableWidget_keyword->item(ui->tableWidget_keyword->currentRow(),1)->text().trimmed());

    }
    else if(operation_type ==  MACRO_Keyword_List_Selector_DB_DELETE_int)
    {
        if(bool_delete_all_keywords_of_list_item)
        {
            query.prepare("delete from keyword_list where category = ?");

            query.addBindValue(ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed());

            bool_delete_all_keywords_of_list_item = false;
        }
        else
        {
            query.prepare("delete from keyword_list where category = ? AND keyword = ?");
            query.addBindValue(ui->tableWidget_keyword_list->item(ui->tableWidget_keyword_list->currentRow(),1)->text().trimmed());
            query.addBindValue(ui->tableWidget_keyword->item(ui->tableWidget_keyword->currentRow(),1)->text().trimmed());
        }
    }

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
        return;
    }

}

void keywords_list_selector::display_keyword_list_table()
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString recon_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    if(!QFileInfo(recon_config_db_path).exists())
        return;

    ui->tableWidget_keyword_list->setRowCount(0);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_config_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    if(!query.exec("select category_status,category,INT from keyword_category"))
    {
        recon_static_functions::app_debug(" query.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QTableWidgetItem *item_enabled, *item_category, *item_INT;

    while(query.next())
    {
        item_enabled  = new QTableWidgetItem;
        item_category = new QTableWidgetItem;
        item_INT      = new QTableWidgetItem;

        int row = ui->tableWidget_keyword_list->rowCount();

        ui->tableWidget_keyword_list->insertRow(row);
        ui->tableWidget_keyword_list->setRowHeight(row, 23);

        if(query.value(0).toString() == QString::number(0))
            item_enabled->setCheckState(Qt::Unchecked);
        else
            item_enabled->setCheckState(Qt::Checked);

        item_category->setText(query.value(1).toString());
        item_INT->setText(query.value(2).toString());

        ui->tableWidget_keyword_list->setItem(row, 0, item_enabled);
        ui->tableWidget_keyword_list->setItem(row, 1, item_category);
        ui->tableWidget_keyword_list->setItem(row, 2, item_INT);
    }

    ui->tableWidget_keyword_list->hideColumn(2);
    ui->tableWidget_keyword_list->resizeColumnToContents(0);

    if(ui->tableWidget_keyword_list->rowCount() > 0)
    {
        on_tableWidget_keyword_list_cellClicked(0,1);
        ui->tableWidget_keyword_list->setCurrentCell(0,0);
    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);

}


void keywords_list_selector::on_tableWidget_keyword_list_cellClicked(int row, int column)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);


    QString current_cat_str = ui->tableWidget_keyword_list->item(row, 1)->text().trimmed();
    QString current_cat_int = ui->tableWidget_keyword_list->item(row, 2)->text().trimmed();

    if(current_cat_str.isEmpty())
        return;

    ui->tableWidget_keyword->setRowCount(0);
    QString recon_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_config_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    query.prepare("select keyword_status, keyword, INT from keyword_list where category = ?");
    query.addBindValue(current_cat_str);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QTableWidgetItem *item_enabled, *item_filename, *item_INT;
    while(query.next())
    {
        item_enabled = new QTableWidgetItem;
        item_filename = new QTableWidgetItem;
        item_INT = new QTableWidgetItem;

        if(query.value(0).toString() == "0")
        {
            item_enabled->setCheckState(Qt::Unchecked);
        }
        else
            item_enabled->setCheckState(Qt::Checked);

        item_filename->setText(query.value(1).toString());
        item_INT->setText(query.value(2).toString());


        int row = ui->tableWidget_keyword->rowCount();

        ui->tableWidget_keyword->insertRow(row);
        ui->tableWidget_keyword->setRowHeight(row, 23);
        ui->tableWidget_keyword->setItem(row, 0, item_enabled);
        ui->tableWidget_keyword->setItem(row, 1, item_filename);
        ui->tableWidget_keyword->setItem(row, 2, item_INT);

    }

    ui->tableWidget_keyword->hideColumn(2);
    ui->tableWidget_keyword->resizeColumnToContents(0);

    if(ui->tableWidget_keyword->rowCount() > 0)
        ui->tableWidget_keyword->setCurrentCell(0,0);


    if(column == 0)
    {
        QSqlQuery query1(destination_db);
        QString category_status;
        if(ui->tableWidget_keyword_list->item(row, 0)->checkState() == Qt::Checked)
        {
            category_status = "1";
        }
        else
        {
            category_status = "0";
        }

        query1.prepare("update keyword_category set category_status = ? where INT = ?");
        query1.addBindValue(category_status);
        query1.addBindValue(current_cat_int);
        if(!query1.exec())
        {
            recon_static_functions::app_debug(" query1.exec()  ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(query1.lastError().text() ,Q_FUNC_INFO);
            return;
        }
    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void keywords_list_selector::on_pushButton_list_remove_clicked()
{
    if(!ui->tableWidget_keyword_list->rowCount() > 0)
    {
        ui->lineEdit_insert_list->hide();
        ui->lineEdit_insert_keywords->hide();
        return;
    }

    if(ui->tableWidget_keyword_list->currentRow() < 0)
        return;

    bool_delete_all_keywords_of_list_item = true;
    list_database_manager_function( MACRO_Keyword_List_Selector_DB_DELETE_int);
    keywords_database_manager_function( MACRO_Keyword_List_Selector_DB_DELETE_int);

    display_keyword_list_table();

    if(ui->tableWidget_keyword_list->rowCount() > 0)
    {

        selected_list_row = ui->tableWidget_keyword_list->currentRow();
        on_tableWidget_keyword_list_cellClicked(selected_list_row,1);
    }

    ui->lineEdit_insert_list->clear();
}

void keywords_list_selector::on_pushButton_list_edit_clicked()
{
    if(ui->tableWidget_keyword_list->currentRow() < 0)
        return;

    bool_update_keyword_list_item = true;

    ui->lineEdit_insert_list->show();
    ui->lineEdit_insert_list->setFocus();
    selected_list_row = ui->tableWidget_keyword_list->currentRow();
    ui->lineEdit_insert_list->setText(ui->tableWidget_keyword_list->item(selected_list_row,1)->text().trimmed());
}

void keywords_list_selector::on_pushButton_keyword_remove_clicked()
{

    if(ui->tableWidget_keyword->currentRow() < 0)
        return;

    if(!ui->tableWidget_keyword->rowCount() > 0)
        return;

    int row_no = ui->tableWidget_keyword_list->currentRow();

    keywords_database_manager_function( MACRO_Keyword_List_Selector_DB_DELETE_int);
    //    on_tableWidget_keyword_list_cellClicked(ui->tableWidget_keyword_list->rowCount()-1,1);
    if(row_no >= 0)
        on_tableWidget_keyword_list_cellClicked(row_no,1);
}

void keywords_list_selector::on_pushButton_keyword_edit_clicked()
{
    if(ui->tableWidget_keyword->currentRow() < 0)
        return;

    if(!ui->tableWidget_keyword->rowCount() > 0)
        return;

    bool_keyword_updated = true;
    ui->lineEdit_insert_keywords->show();
    selected_keywords_row = ui->tableWidget_keyword->currentRow();

    ui->lineEdit_insert_keywords->setText(ui->tableWidget_keyword->item(selected_keywords_row,1)->text().trimmed());

}

void keywords_list_selector::on_tableWidget_keyword_cellClicked(int row, int column)
{
    if(column != 0)
        return;

    QString recon_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_config_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    QString current_keyword_int = ui->tableWidget_keyword->item(row, 2)->text().trimmed();

    QString check_state;

    if(ui->tableWidget_keyword->item(row, 0)->checkState() == Qt::Checked)
        check_state = "1";
    else
        check_state = "0";

    query.prepare("update keyword_list set keyword_status = ? where INT = ?");
    query.addBindValue(check_state);
    query.addBindValue(current_keyword_int);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query.exec()  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text() ,Q_FUNC_INFO);
    }

    destination_db.close();
}

void keywords_list_selector::on_pushButton_cancel_clicked()
{
    this->hide();
}
