#include "file_signature_widget.h"
#include "ui_file_signature_widget.h"

file_signature_widget::file_signature_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file_signature_widget)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    add_ext_category_popup = new add_ext_category(this);
    connect(add_ext_category_popup, SIGNAL(signal_category_added()), this, SLOT(slot_category_added()));

    edit_category_dialog_popup = new edit_category_dialog(this);
    connect(edit_category_dialog_popup, SIGNAL(signal_edit_category(QString)), this, SLOT(slot_edit_category(QString)));

    new_file_signature_popup = new new_file_signature(this);
    connect(new_file_signature_popup, SIGNAL(signal_signature_added()),this, SLOT(slot_signature_added()));

    edit_signature_dialog_popup = new edit_signature_dialog(this);
    connect(edit_signature_dialog_popup, SIGNAL(signal_signature_edit(QString,QString, QString)), this, SLOT(slot_edit_signature(QString,QString, QString)));

    QIcon icon_edit(QPixmap("../icons/signs/edit.png"));
    QIcon icon_add(QPixmap("../icons/signs/add.png"));
    QIcon icon_remove(QPixmap("../icons/signs/remove.png"));
    QIcon icon_clipboard(QPixmap("../icons/signs/clipboard.png"));

    ui->pushButton_category_add->setIcon(icon_add);
    //ui->pushButton_category_edit->setIcon(icon_edit);
    ui->pushButton_category_remove->setIcon(icon_remove);

    ui->pushButton_signature_add->setIcon(icon_add);
    ui->pushButton_signature_clipboard->setIcon(icon_clipboard);
    ui->pushButton_signature_edit->setIcon(icon_edit);
    ui->pushButton_signature_remove->setIcon(icon_remove);

    ui->tableWidget_signature_category->verticalHeader()->setHighlightSections(false);
    ui->tableWidget_signature_category->horizontalHeader()->setHighlightSections(false);

    ui->tableWidget_signature_list->verticalHeader()->setHighlightSections(false);
    ui->tableWidget_signature_list->horizontalHeader()->setHighlightSections(false);

    ui->tableWidget_signature_list->hideColumn(3);
    ui->tableWidget_signature_list->resizeColumnToContents(0);

    ui->pushButton_signature_clipboard->setDisabled(true);


    ui->tableWidget_signature_list->verticalHeader()->setFixedWidth(25);
    ui->tableWidget_signature_category->verticalHeader()->setFixedWidth(25);

    ui->pushButton_signature_clipboard->hide();

    message_dialog_object = new message_dialog;

    recon_helper_standard_obj->translate_tablewidget_horizontal_header_item_all(ui->tableWidget_signature_category, Q_FUNC_INFO);
    recon_helper_standard_obj->translate_tablewidget_horizontal_header_item_all(ui->tableWidget_signature_list, Q_FUNC_INFO);

}

file_signature_widget::~file_signature_widget()
{
    delete ui;
}


void file_signature_widget::pub_set_caller_type(QString caller_type)
{
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

    add_ext_category_popup->pub_set_destination_db_file(destination_db_file);
    new_file_signature_popup->pub_set_destination_db_file(destination_db_file);

    create_category_list();

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        this->layout()->setContentsMargins(2,0,0,0);
    }
}

void file_signature_widget::pub_initialise_signature_details()
{
}

void file_signature_widget::pub_set_ui_awake_status(bool status)
{
    ui->tableWidget_signature_category->setEnabled(status);
    ui->tableWidget_signature_list->setEnabled(status);

    ui->pushButton_category_add->setEnabled(status);
    //ui->pushButton_category_edit->setEnabled(status);
    ui->pushButton_category_remove->setEnabled(status);


    ui->pushButton_signature_add->setEnabled(status);
    ui->pushButton_signature_edit->setEnabled(status);
    ui->pushButton_signature_remove->setEnabled(status);

}

void file_signature_widget::on_pushButton_category_add_clicked()
{

    int row_count = ui->tableWidget_signature_category->rowCount();

    if(ui->tableWidget_signature_category->item(row_count - 1, 1) == NULL && row_count > 0)
    {
        return;
    }

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setCheckState(Qt::Checked);


    ui->tableWidget_signature_category->insertRow(row_count);

    new_category_LineEdit = new QLineEdit(this);
    connect(new_category_LineEdit, SIGNAL(returnPressed()), this, SLOT(slot_new_category_line_edit_return_pressed()));

    ui->tableWidget_signature_category->setItem(row_count, 0, item);
    ui->tableWidget_signature_category->setCellWidget(row_count, 1, new_category_LineEdit);

    ui->tableWidget_signature_category->setCurrentCell(row_count, 1);

    while(ui->tableWidget_signature_list->rowCount())
        ui->tableWidget_signature_list->removeRow(0);

}

void file_signature_widget::slot_category_added()
{
    create_category_list();
}

void file_signature_widget::create_category_list()
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);

    while(ui->tableWidget_signature_category->rowCount())
        ui->tableWidget_signature_category->removeRow(0);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);
    QTableWidgetItem *item_enabled, *item_category, *item_INT;
    if(!query.exec("select category_status,category, INT from tbl_signature_category"))
    {
        recon_static_functions::app_debug(" query ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        return;
    }

    while(query.next())
    {
        item_enabled = new QTableWidgetItem;
        item_category = new QTableWidgetItem;
        item_INT = new QTableWidgetItem;

        int row = ui->tableWidget_signature_category->rowCount();

        ui->tableWidget_signature_category->insertRow(row);
        ui->tableWidget_signature_category->setRowHeight(row, 23);

        if(query.value(0).toString() == "0")
            item_enabled->setCheckState(Qt::Unchecked);
        else
            item_enabled->setCheckState(Qt::Checked);

        item_category->setText(query.value(1).toString());
        item_INT->setText(query.value(2).toString());

        ui->tableWidget_signature_category->setItem(row, 0, item_enabled);
        ui->tableWidget_signature_category->setItem(row, 1, item_category);
        ui->tableWidget_signature_category->setItem(row, 2, item_INT);

    }

    ui->tableWidget_signature_category->hideColumn(2);
    ui->tableWidget_signature_category->resizeColumnToContents(0);

    if(ui->tableWidget_signature_category->rowCount() > 0)
    {
        on_tableWidget_signature_category_cellClicked(0,0);
        ui->tableWidget_signature_category->setCurrentCell(0,0);
    }

    recon_static_functions::app_debug("- END",Q_FUNC_INFO);
}

void file_signature_widget::on_pushButton_signature_add_clicked()
{
    int crnt_row_catgry = ui->tableWidget_signature_category->currentRow();

    if(ui->tableWidget_signature_category->item(crnt_row_catgry,1) == NULL)
    {
        return;
    }
    if(crnt_row_catgry < 0)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Please select a Category");
        message_dialog_object->show();
        return;
    }

    list_signature.clear();

    for(int i = 0; i < ui->tableWidget_signature_list->rowCount(); i++)
    {
        list_signature << ui->tableWidget_signature_list->item(i,1)->text().trimmed();

    }

    new_file_signature_popup->set_list_signature(list_signature);


    QString category_name = ui->tableWidget_signature_category->item(crnt_row_catgry, 1)->text();;
    QString command_str_INT_retrieve = QString("select INT from tbl_signature_category where category=?");
    QStringList args;
    args << category_name;


    QString category_INT = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str_INT_retrieve, args, 0, destination_db_file, Q_FUNC_INFO);
    if(category_INT.trimmed().isEmpty())
    {
        return;
    }


    // current_category = ui->tableWidget_signature_category->item(row, 2)->text();

    new_file_signature_popup->set_current_category(category_INT);
    new_file_signature_popup->show();
}

void file_signature_widget::on_tableWidget_signature_category_cellClicked(int row, int column)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    while(ui->tableWidget_signature_list->rowCount())
        ui->tableWidget_signature_list->removeRow(0);

    if(row < 0)
        return;

    if(ui->tableWidget_signature_category->item(row, 1) == NULL)
        return;


    QString category_name = ui->tableWidget_signature_category->item(row, 1)->text();;

    QString command_str_INT_retrieve = QString("select INT from tbl_signature_category where category=?");
    QStringList args;
    args << category_name;


    QString category_INT = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str_INT_retrieve, args, 0, destination_db_file, Q_FUNC_INFO);
    if(category_INT.trimmed().isEmpty())
    {
        return;
    }


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("select signature_status, label, header, INT from tbl_signature_list where category = ?");
    query.addBindValue(category_INT);
    QTableWidgetItem *item_status, *item_label, *item_header, *item_INT;
    if(!query.exec())
    {
        recon_static_functions::app_debug("  query ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery(),Q_FUNC_INFO);
        return;
    }


    while(query.next())
    {
        item_status = new QTableWidgetItem;
        item_label = new QTableWidgetItem;
        item_header = new QTableWidgetItem;
        item_INT = new QTableWidgetItem;

        if(query.value(0).toString() == "1")
            item_status->setCheckState(Qt::Checked);
        else
            item_status->setCheckState(Qt::Unchecked);

        item_label->setText(query.value(1).toString());
        item_header->setText(query.value(2).toString());
        item_INT->setText(query.value(3).toString());

        int row_in_list = ui->tableWidget_signature_list->rowCount();
        ui->tableWidget_signature_list->insertRow(row_in_list);
        ui->tableWidget_signature_list->setRowHeight(row_in_list, 23);

        ui->tableWidget_signature_list->setItem(row_in_list, 0, item_status);
        ui->tableWidget_signature_list->setItem(row_in_list, 1, item_label);
        ui->tableWidget_signature_list->setItem(row_in_list, 2, item_header);
        ui->tableWidget_signature_list->setItem(row_in_list, 3, item_INT);

    }


    ui->tableWidget_signature_list->resizeColumnToContents(0);
    ui->tableWidget_signature_list->setColumnWidth(1, 100);
    ui->tableWidget_signature_list->hideColumn(3);

    if(ui->tableWidget_signature_list->rowCount() > 0)
        ui->tableWidget_signature_list->setCurrentCell(0,0);


    if(column == 0)
    {
        QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
        QSqlDatabase destination_db1 = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
        destination_db1.setDatabaseName(destination_db_file);

        if(!destination_db1.open())
        {
            recon_static_functions::app_debug(" destination_db1 ------FAILED------- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QSqlQuery query1(destination_db1);
        QString category_status;
        if(ui->tableWidget_signature_category->item(row, 0)->checkState() == Qt::Checked)
        {
            category_status = "1";
        }
        else
        {
            category_status = "0";
        }


        query1.prepare("update tbl_signature_category set category_status = ? where INT = ?");

        query1.addBindValue(category_status);
        query1.addBindValue(category_INT);

        if(!query1.exec())
        {
            recon_static_functions::app_debug(" update ------FAILED------- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(query1.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(query1.executedQuery(),Q_FUNC_INFO);
        }

        destination_db1.close();
    }
    destination_db.close();


    recon_static_functions::app_debug(" END",Q_FUNC_INFO);
}

void file_signature_widget::on_pushButton_category_remove_clicked()
{

    int current_row = ui->tableWidget_signature_category->currentRow();

    if(current_row < 0)
        return;

    if(ui->tableWidget_signature_category->item(current_row, 1) == NULL)
    {
        ui->tableWidget_signature_category->removeRow(current_row);
        ui->tableWidget_signature_category->setCurrentCell( -1, 1);

        return;
    }

    QString category_name = ui->tableWidget_signature_category->item(current_row, 1)->text();


    QString command_str_INT_retrieve = QString("select INT from tbl_signature_category where category=?");
    QStringList args;
    args << category_name;


    QString category_INT = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str_INT_retrieve, args, 0, destination_db_file, Q_FUNC_INFO);
    if(category_INT.trimmed().isEmpty())
    {
        return;
    }


    QString command_str_extension = QString("DELETE FROM tbl_signature_list where category=?");

    QString command_str_category = QString("DELETE FROM tbl_signature_category where INT=?");



    QStringList value_list;
    value_list << category_INT;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str_extension , value_list, destination_db_file ,Q_FUNC_INFO);

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str_category , value_list, destination_db_file ,Q_FUNC_INFO);


    ui->tableWidget_signature_category->removeRow(current_row);

    if(ui->tableWidget_signature_category->rowCount() == 0)
    {
        while(ui->tableWidget_signature_list->rowCount())
            ui->tableWidget_signature_list->removeRow(0);

        return;
    }

    if(ui->tableWidget_signature_category->rowCount() - 1  < current_row)
    {
        ui->tableWidget_signature_category->setCurrentCell(ui->tableWidget_signature_category->rowCount() - 1, 1);
        on_tableWidget_signature_category_cellClicked(ui->tableWidget_signature_category->rowCount() - 1, 1);
    }
    else
    {
        ui->tableWidget_signature_category->setCurrentCell(current_row, 1);
        on_tableWidget_signature_category_cellClicked(current_row, 1);
    }


}

void file_signature_widget::slot_edit_category(QString cat)
{
    int row = ui->tableWidget_signature_category->currentRow();

    if(row < 0)
        return;

    QString current_cat = ui->tableWidget_signature_category->item(row, 2)->text();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("update tbl_signature_category set category = ? where INT = ?");
    query.addBindValue(cat);
    query.addBindValue(current_cat);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" update ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
    }


    create_category_list();

    ui->tableWidget_signature_category->setCurrentCell(row, 1);
    on_tableWidget_signature_category_cellClicked(row, 0);
}


void file_signature_widget::on_pushButton_signature_remove_clicked()
{
    if(ui->tableWidget_signature_list->rowCount() == 0)
        return;

    int row = ui->tableWidget_signature_list->currentRow();

    if(row < 0)
        return;

    QString current_signature_int = ui->tableWidget_signature_list->item(row, 3)->text();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("delete from tbl_signature_list where INT = ?");
    query.addBindValue(current_signature_int);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" update ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
    }
    row = ui->tableWidget_signature_category->currentRow();
    on_tableWidget_signature_category_cellClicked(row, 1);
}

void file_signature_widget::slot_signature_added()
{
    int   row = ui->tableWidget_signature_category->currentRow();
    on_tableWidget_signature_category_cellClicked(row, 1);
}

void file_signature_widget::slot_edit_signature(QString label, QString signature , QString offset)
{
    edit_signature_dialog_popup->hide();
    int row = ui->tableWidget_signature_list->currentRow();
    QString current_signature_int = ui->tableWidget_signature_list->item(row, 3)->text();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    query.prepare("update tbl_signature_list set label = ?, header = ? , offset = ? where INT = ?");
    query.addBindValue(label);
    query.addBindValue(signature);
    query.addBindValue(offset);
    query.addBindValue(current_signature_int);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" update ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
    }
    row = ui->tableWidget_signature_category->currentRow();
    on_tableWidget_signature_category_cellClicked(row, 1);
}

void file_signature_widget::on_pushButton_signature_edit_clicked()
{
    int row = ui->tableWidget_signature_list->currentRow();

    if(row < 0)
    {
        return;
    }

    QString label, header;

    label = ui->tableWidget_signature_list->item(row, 1)->text();
    header = ui->tableWidget_signature_list->item(row, 2)->text();

    QString command_str = QString("Select offset from tbl_signature_list Where header = '") + header + QString("'");

    QString offset_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(command_str , 0 , destination_db_file , Q_FUNC_INFO);

    edit_signature_dialog_popup->set_laber_header(label, header , offset_str);

    edit_signature_dialog_popup->show();
}

void file_signature_widget::on_tableWidget_signature_list_cellClicked(int row, int column)
{
    if(column)
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" destination_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(destination_db);

    QString current_signature_int = ui->tableWidget_signature_list->item(row, 3)->text();

    QString check_state;

    if(ui->tableWidget_signature_list->item(row, 0)->checkState() == Qt::Checked)
        check_state = "1";
    else
        check_state = "0";


    query.prepare("update tbl_signature_list set signature_status = ? where INT = ?");
    query.addBindValue(check_state);
    query.addBindValue(current_signature_int);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" update ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query.lastError().text(),Q_FUNC_INFO);
    }

}

void file_signature_widget::on_pushButton_signature_clipboard_clicked()
{

}

void file_signature_widget::slot_new_category_line_edit_return_pressed()
{
    QString new_category = new_category_LineEdit->text();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(new_category);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);


    if(new_category.trimmed().isEmpty())
        return;

    if(ui->tableWidget_signature_category->currentRow() < 0)
        return;


    QString command_str = QString("INSERT INTO tbl_signature_category (category_status, category) values (?,?)");



    QStringList value_list;
    value_list << "1" << new_category;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list, destination_db_file ,Q_FUNC_INFO);

    ui->tableWidget_signature_category->removeCellWidget(ui->tableWidget_signature_category->currentRow(), 1);
    ui->tableWidget_signature_category->setItem(ui->tableWidget_signature_category->currentRow(), 1, item);

}
