#include "recon_configuration_page_apple_metadata.h"
#include "ui_recon_configuration_page_apple_metadata.h"

recon_configuration_page_apple_metadata::recon_configuration_page_apple_metadata(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_apple_metadata)
{
    ui->setupUi(this);

    recon_static_functions::app_debug(" rc-config apple_metadata 00", Q_FUNC_INFO);

    recon_helper_standard_obj = new recon_helper_standard(this);

    recon_static_functions::app_debug(" rc-config apple_metadata 11", Q_FUNC_INFO);

    QObject::connect((QObject*)ui->tableWidget_apple_metadata->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_horizontal_header_section_clicked(int)));
    recon_static_functions::app_debug(" rc-config apple_metadata 22", Q_FUNC_INFO);

    ui->tableWidget_apple_metadata->hideColumn(Page_Apple_Metadata_Bookmark_Metadata);
    recon_static_functions::app_debug(" rc-config apple_metadata 33", Q_FUNC_INFO);
    ui->checkBox_select_all->hide();
    recon_static_functions::app_debug(" rc-config apple_metadata 44", Q_FUNC_INFO);

    ui->lineEdit_search->setPlaceholderText(global_lang_tr_obj.get_translated_string(ui->lineEdit_search->placeholderText()));
    recon_helper_standard_obj->translate_tablewidget_horizontal_header_item_all(ui->tableWidget_apple_metadata, Q_FUNC_INFO);



}

recon_configuration_page_apple_metadata::~recon_configuration_page_apple_metadata()
{
    delete ui;
}


void recon_configuration_page_apple_metadata::pub_set_caller_type(QString caller_type)
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

    prepare_apple_metadata_display();

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        this->layout()->setContentsMargins(0,0,0,0);
    }

}

void recon_configuration_page_apple_metadata::prepare_apple_metadata_display()
{
    ui->tableWidget_apple_metadata->setRowCount(0);

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString command = QString("select bookmark_filter,attribute_name,description,INT,metadata_attribute_name,custom_column_name,bookmark_metadata,bookmark_report from tbl_apple_attributes where is_default = 1 OR bookmark_filter = 1 OR bookmark_metadata = 1 OR bookmark_report = 1");

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


    int row = 0;
    while(query.next())
    {

        ui->tableWidget_apple_metadata->insertRow(row);
        ui->tableWidget_apple_metadata->setRowHeight(row, 23);

        QTableWidgetItem *item_bookmark_filter = new QTableWidgetItem;
        QTableWidgetItem *item_title = new QTableWidgetItem;
        QTableWidgetItem *item_attribute    = new QTableWidgetItem;
        QTableWidgetItem *item_description = new QTableWidgetItem;
        QTableWidgetItem *item_bookmark_metadata = new QTableWidgetItem;
        QTableWidgetItem *item_bookmark_report = new QTableWidgetItem;


        if(query.value(0).toString().trimmed() == QString::number(1))
            item_bookmark_filter->setCheckState(Qt::Checked);
        else
            item_bookmark_filter->setCheckState(Qt::Unchecked);

        if(query.value(6).toString().trimmed() == QString::number(1))
            item_bookmark_metadata->setCheckState(Qt::Checked);
        else
            item_bookmark_metadata->setCheckState(Qt::Unchecked);

        if(query.value(7).toString().trimmed() == QString::number(1))
            item_bookmark_report->setCheckState(Qt::Checked);
        else
            item_bookmark_report->setCheckState(Qt::Unchecked);


        item_title->setText(query.value(1).toString().trimmed());
        item_attribute->setText(query.value(4).toString().trimmed());
        item_description->setText(query.value(2).toString().trimmed());

        ui->tableWidget_apple_metadata->setItem(row , Page_Apple_Metadata_Bookmark_Display_Filter , item_bookmark_filter);
        ui->tableWidget_apple_metadata->setItem(row , Page_Apple_Metadata_Bookmark_Metadata , item_bookmark_metadata);
        ui->tableWidget_apple_metadata->setItem(row , Page_Apple_Metadata_Bookmark_Reporting , item_bookmark_report);
        ui->tableWidget_apple_metadata->setItem(row , Page_Apple_Metadata_Title , item_title);
        ui->tableWidget_apple_metadata->setItem(row , Page_Apple_Metadata_Attribute_Name , item_attribute);
        ui->tableWidget_apple_metadata->setItem(row , Page_Apple_Metadata_Description , item_description);

        row++;

    }


    ui->tableWidget_apple_metadata->setColumnWidth(Page_Apple_Metadata_Bookmark_Display_Filter,30);
    ui->tableWidget_apple_metadata->setColumnWidth(Page_Apple_Metadata_Bookmark_Metadata,30);
    ui->tableWidget_apple_metadata->setColumnWidth(Page_Apple_Metadata_Bookmark_Reporting,30);
    ui->tableWidget_apple_metadata->setColumnWidth(Page_Apple_Metadata_Title,180);
    ui->tableWidget_apple_metadata->setColumnWidth(Page_Apple_Metadata_Attribute_Name,220);
    ui->tableWidget_apple_metadata->setColumnWidth(Page_Apple_Metadata_Description,300);

    destination_db.close();
}


void recon_configuration_page_apple_metadata::on_checkBox_select_all_clicked(bool checked)
{
    for(int row = 0; row < ui->tableWidget_apple_metadata->rowCount(); row++)
    {
        if(checked)
        {
            ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Bookmark_Display_Filter)->setCheckState(Qt::Checked);
            ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Bookmark_Metadata)->setCheckState(Qt::Checked);
            ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Bookmark_Reporting)->setCheckState(Qt::Checked);
        }
        else
        {
            ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Bookmark_Display_Filter)->setCheckState(Qt::Unchecked);
            ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Bookmark_Metadata)->setCheckState(Qt::Unchecked);
            ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Bookmark_Reporting)->setCheckState(Qt::Unchecked);
        }
    }

    QString command_str = QString("Update tbl_apple_attributes set bookmark_filter = ?,bookmark_metadata = ?,bookmark_report = ? ");
    QStringList value_list;

    if(checked)
        value_list << QString::number(1) << "1" << "1";
    else
        value_list << QString::number(0) << "0" << "0";

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list , destination_db_file , Q_FUNC_INFO);

}

void recon_configuration_page_apple_metadata::on_lineEdit_search_textChanged(const QString &arg1)
{
    QString text_str = arg1.trimmed();

    for(int row = 0; row < ui->tableWidget_apple_metadata->rowCount(); row++)
    {
        if(ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Title)->text().contains(text_str , Qt::CaseInsensitive)
                || ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Attribute_Name)->text().contains(text_str , Qt::CaseInsensitive)
                || ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Description)->text().contains(text_str , Qt::CaseInsensitive))
            ui->tableWidget_apple_metadata->showRow(row);
        else
            ui->tableWidget_apple_metadata->hideRow(row);
    }

}

void recon_configuration_page_apple_metadata::on_tableWidget_apple_metadata_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    QString command_str;
    if(column == Page_Apple_Metadata_Bookmark_Display_Filter)
    {
        command_str = QString("Update tbl_apple_attributes set bookmark_filter = ? Where metadata_attribute_name = ?");
    }
    else if(column == Page_Apple_Metadata_Bookmark_Metadata)
    {
        command_str = QString("Update tbl_apple_attributes set bookmark_metadata = ? Where metadata_attribute_name = ?");
    }
    else if(column == Page_Apple_Metadata_Bookmark_Reporting)
    {
        command_str = QString("Update tbl_apple_attributes set bookmark_report = ? Where metadata_attribute_name = ?");
    }

    if(command_str.trimmed().isEmpty())
        return;

    QString metadata_attribute_name = ui->tableWidget_apple_metadata->item(row , Page_Apple_Metadata_Attribute_Name)->text();

    QString bookmark_str;
    if(ui->tableWidget_apple_metadata->item(row , column)->checkState() == Qt::Checked)
        bookmark_str = QString::number(1);
    else
        bookmark_str = QString::number(0);

    QStringList value_list;
    value_list << bookmark_str << metadata_attribute_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list , destination_db_file , Q_FUNC_INFO);

}

void recon_configuration_page_apple_metadata::slot_horizontal_header_section_clicked(int column)
{

    return;


}
