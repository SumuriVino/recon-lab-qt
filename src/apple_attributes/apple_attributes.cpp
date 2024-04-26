#include "apple_attributes.h"
#include "ui_apple_attributes.h"

apple_attributes::apple_attributes(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::apple_attributes)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    item_model = new QStandardItemModel(this);


    recon_helper_standard_obj = new recon_helper_standard(this);

    //ui->pushButton_cancel->hide();

    ui->pushButton_ok->setAttribute(Qt::WA_MacShowFocusRect,0);
    ui->lineEdit->setAttribute(Qt::WA_MacShowFocusRect,0);
}

apple_attributes::~apple_attributes()
{
    delete ui;
}

void apple_attributes::set_essentials() // set configuration database path
{

    if(page_caller == MACRO_CONFIGURATION_CALLER_TYPE_RECON_CONFIG)
    {
        destination_db_Path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + "recon_configuration.sqlite";
    }
    else if(page_caller == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        destination_db_Path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Result_Configuration_QString).toString() + "case_configuration/case_configuration.sqlite";
    }
    else if(page_caller == MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER)
        destination_db_Path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    if(item_model->rowCount() > 0)
    {
        ui->tableView_attributes->selectRow(0);
        ui->tableView_attributes->setFocus();
    }

    display_attributes();

}
void apple_attributes::pub_set_page_caller(QString caller_name) //set caller type eg pre or post launcher
{
    page_caller = caller_name;
}


void apple_attributes::display_attributes() //display all the apple attributes in the display so user can select which one to process
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_Path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" - dbOpen -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_Path , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select(destination_db);
    query_select.prepare("select bookmark_filter,attribute_name,description,INT,is_default,metadata_attribute_name,custom_column_name from tbl_apple_attributes ORDER BY bookmark_filter DESC");
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" -query_select execution-------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery(), Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    item_model->clear();
    ui->tableView_attributes->setModel(item_model);

    QStringList headerlist;
    headerlist  << "" << "INT" <<"Attribute" << "Description" << "Command" << "Custom Column Name" << "";
    item_model->setHorizontalHeaderLabels(headerlist);

    ui->tableView_attributes->horizontalHeader()->setStretchLastSection(true);

    int row = 0;
    while (query_select.next())
    {
        QString bookmark = query_select.value(0).toString();
        QString attribute = query_select.value(1).toString();
        QString description = query_select.value(2).toString();
        QString record = query_select.value(3).toString();
        QString is_default = query_select.value(4).toString();
        QString m_command = query_select.value(5).toString();
        QString m_custom_column_name= query_select.value(6).toString();


        QStandardItem *bookmark_item = new QStandardItem;
        QStandardItem *attribute_item = new QStandardItem(attribute);
        QStandardItem *description_item = new QStandardItem(description);
        QStandardItem *record_item = new QStandardItem(record);
        QStandardItem *cmd_item = new QStandardItem(m_command);
        QStandardItem *cstm_clmn_name_item = new QStandardItem(m_custom_column_name);
        QStandardItem *extra_item = new QStandardItem;

        bookmark_item->setCheckable(true);
        //        if(is_default == "1")
        //            bookmark_item->setCheckable(false);

        //        if(bookmark == "1" || is_default == "1")
        //            bookmark_item->setCheckState(Qt::Checked);

        if(bookmark == "1")
            bookmark_item->setCheckState(Qt::Checked);

        item_model->setItem(row,enum_apple_attributes_INT, record_item);
        item_model->setItem(row,enum_apple_attributes_Bookmark, bookmark_item);
        item_model->setItem(row,enum_apple_attributes_Name, attribute_item);
        item_model->setItem(row,enum_apple_attributes_Description, description_item);
        item_model->setItem(row,enum_apple_attributes_Command, cmd_item);
        item_model->setItem(row,enum_apple_attributes_Custom_Column_Name, cstm_clmn_name_item);
        item_model->setItem(row,enum_apple_attributes_Blank_Column, extra_item);

        ui->tableView_attributes->setRowHeight(row,20);
        row++;
    }

    destination_db.close();

    if(item_model->rowCount() > 0)
        ui->tableView_attributes->selectRow(0);

    ui->tableView_attributes->hideColumn(enum_apple_attributes_INT);
    ui->tableView_attributes->hideColumn(enum_apple_attributes_Command);
    ui->tableView_attributes->hideColumn(enum_apple_attributes_Custom_Column_Name);

    ui->tableView_attributes->setColumnWidth(enum_apple_attributes_Bookmark,30);
    ui->tableView_attributes->setColumnWidth(enum_apple_attributes_Name,200);
    ui->tableView_attributes->setColumnWidth(enum_apple_attributes_Description,500);
    ui->tableView_attributes->setColumnWidth(enum_apple_attributes_Blank_Column,10);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void apple_attributes::on_pushButton_ok_clicked() //update status of selected apple attributes in the database
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_Path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" - db Open -------FAILED------ " , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_Path , Q_FUNC_INFO);
        return;
    }

    for(int m_row = 0; m_row < item_model->rowCount(); m_row++)
    {
        QStandardItem *attribute_bookmark = item_model->item(m_row,enum_apple_attributes_Bookmark);
        QString record_no = item_model->item(m_row,enum_apple_attributes_INT)->text().trimmed();

        if(attribute_bookmark->checkState() == Qt::Checked)
        {
            QString cmd = "update tbl_apple_attributes set bookmark_filter = 1 where INT = ?";
            QStringList list_bk;
            list_bk << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd,list_bk,destination_db,Q_FUNC_INFO);
        }
        else
        {
            QString cmd = "update tbl_apple_attributes set bookmark_filter = 0 where INT = ?";
            QStringList list_ubk;
            list_ubk << record_no;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd,list_ubk,destination_db,Q_FUNC_INFO);
        }
    }

    destination_db.close();
    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

    emit signal_workdone_complete(true);
}

void apple_attributes::on_lineEdit_textChanged(const QString &arg1) //search in search box and change in display accordingly
{
    for(int m_row = 0; m_row < item_model->rowCount(); m_row++)
    {

        bool text_found = false;
        for(int m_column = 0 ; m_column < item_model->columnCount() ; m_column++)
        {
            if(m_column == enum_apple_attributes_Bookmark || m_column == enum_apple_attributes_INT || m_column == enum_apple_attributes_Command || m_column == enum_apple_attributes_Custom_Column_Name || m_column == enum_apple_attributes_Blank_Column)
                continue;

            QStandardItem *itemm = item_model->item(m_row,m_column);
            QString search_text = itemm->text();

            search_text = search_text.toLower();
            arg1.toLower();

            if(search_text.contains(arg1))
            {
                text_found = true;
                break;
            }
        }

        if(text_found)
        {
            ui->tableView_attributes->showRow(m_row);
        }
        else
        {
            ui->tableView_attributes->hideRow(m_row);
        }
    }
}

void apple_attributes::on_pushButton_cancel_clicked() // cancel if cancel button pressed
{
    emit signal_workdone_complete(false);
}

