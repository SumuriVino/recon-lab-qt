#include "recon_configuration_page_exif_metadata.h"
#include "ui_recon_configuration_page_exif_metadata.h"

recon_configuration_page_exif_metadata::recon_configuration_page_exif_metadata(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recon_configuration_page_exif_metadata)
{
    ui->setupUi(this);

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->tableWidget_exif_metadata->hideColumn(Page_Exif_Metadata_Bookmark_Metadata);
    ui->tableWidget_exif_metadata->hideColumn(Page_Exif_Metadata_Attribute_Name);
    ui->checkBox_select_all->hide();

    ui->lineEdit_search->setPlaceholderText(global_lang_tr_obj.get_translated_string(ui->lineEdit_search->placeholderText()));
    recon_helper_standard_obj->translate_tablewidget_horizontal_header_item_all(ui->tableWidget_exif_metadata, Q_FUNC_INFO);

}

recon_configuration_page_exif_metadata::~recon_configuration_page_exif_metadata()
{
    delete ui;
}


void recon_configuration_page_exif_metadata::pub_set_caller_type(QString caller_type)
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

    prepare_exif_metadata_display();

    if(caller_type == MACRO_CONFIGURATION_CALLER_TYPE_CASE_LAUNCHER)
    {
        this->layout()->setContentsMargins(0,0,0,0);
    }

}

void recon_configuration_page_exif_metadata::prepare_exif_metadata_display()
{
    ui->tableWidget_exif_metadata->setRowCount(0);

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

    QString command = QString("select bookmark_filter,parent_name,description,INT,bookmark_metadata,bookmark_report from tbl_exif_attributes where is_default = 1 OR bookmark_filter = 1 OR bookmark_metadata = 1 OR bookmark_report = 1 group by parent_name");

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return;
    }


    int row = 0;
    while(query.next())
    {

        ui->tableWidget_exif_metadata->insertRow(row);
        ui->tableWidget_exif_metadata->setRowHeight(row, 23);

        QTableWidgetItem *item_bookmark_filter = new QTableWidgetItem;
        QTableWidgetItem *item_title = new QTableWidgetItem;
        QTableWidgetItem *item_description = new QTableWidgetItem;
        QTableWidgetItem *item_bookmark_metadata = new QTableWidgetItem;
        QTableWidgetItem *item_bookmark_report = new QTableWidgetItem;


        if(query.value(0).toString().trimmed() == QString::number(1))
            item_bookmark_filter->setCheckState(Qt::Checked);
        else
            item_bookmark_filter->setCheckState(Qt::Unchecked);

        if(query.value(4).toString().trimmed() == QString::number(1))
            item_bookmark_metadata->setCheckState(Qt::Checked);
        else
            item_bookmark_metadata->setCheckState(Qt::Unchecked);

        if(query.value(5).toString().trimmed() == QString::number(1))
            item_bookmark_report->setCheckState(Qt::Checked);
        else
            item_bookmark_report->setCheckState(Qt::Unchecked);


        item_title->setText(query.value(1).toString().trimmed());
        item_description->setText(query.value(2).toString().trimmed());

        ui->tableWidget_exif_metadata->setItem(row , Page_Exif_Metadata_Bookmark_Display_Filter , item_bookmark_filter);
        ui->tableWidget_exif_metadata->setItem(row , Page_Exif_Metadata_Bookmark_Metadata , item_bookmark_metadata);
        ui->tableWidget_exif_metadata->setItem(row , Page_Exif_Metadata_Bookmark_Reporting , item_bookmark_report);
        ui->tableWidget_exif_metadata->setItem(row , Page_Exif_Metadata_Title , item_title);
        ui->tableWidget_exif_metadata->setItem(row , Page_Exif_Metadata_Description , item_description);

        row++;

    }


    ui->tableWidget_exif_metadata->setColumnWidth(Page_Exif_Metadata_Bookmark_Display_Filter,30);
    ui->tableWidget_exif_metadata->setColumnWidth(Page_Exif_Metadata_Bookmark_Metadata,30);
    ui->tableWidget_exif_metadata->setColumnWidth(Page_Exif_Metadata_Bookmark_Reporting,30);
    ui->tableWidget_exif_metadata->setColumnWidth(Page_Exif_Metadata_Title,180);
    ui->tableWidget_exif_metadata->setColumnWidth(Page_Exif_Metadata_Description,300);

    destination_db.close();
}

void recon_configuration_page_exif_metadata::on_lineEdit_search_textChanged(const QString &arg1)
{
    QString text_str = arg1.trimmed();

    for(int row = 0; row < ui->tableWidget_exif_metadata->rowCount(); row++)
    {
        if(ui->tableWidget_exif_metadata->item(row , Page_Exif_Metadata_Title)->text().contains(text_str , Qt::CaseInsensitive)
                || ui->tableWidget_exif_metadata->item(row , Page_Exif_Metadata_Description)->text().contains(text_str , Qt::CaseInsensitive))
            ui->tableWidget_exif_metadata->showRow(row);
        else
            ui->tableWidget_exif_metadata->hideRow(row);
    }

}

void recon_configuration_page_exif_metadata::on_tableWidget_exif_metadata_cellClicked(int row, int column)
{
    if(row < 0 || column < 0)
        return;

    QString command_str;
    if(column == Page_Exif_Metadata_Bookmark_Display_Filter)
    {
        command_str = QString("Update tbl_exif_attributes set bookmark_filter = ? Where parent_name = ?");
    }
    else if(column == Page_Exif_Metadata_Bookmark_Metadata)
    {
        command_str = QString("Update tbl_exif_attributes set bookmark_metadata = ? Where parent_name = ?");
    }
    else if(column == Page_Exif_Metadata_Bookmark_Reporting)
    {
        command_str = QString("Update tbl_exif_attributes set bookmark_report = ? Where parent_name = ?");
    }

    if(command_str.trimmed().isEmpty())
        return;

    QString title_name = ui->tableWidget_exif_metadata->item(row , Page_Exif_Metadata_Title)->text();

    QString bookmark_str;
    if(ui->tableWidget_exif_metadata->item(row , column)->checkState() == Qt::Checked)
        bookmark_str = QString::number(1);
    else
        bookmark_str = QString::number(0);

    QStringList value_list;
    value_list << bookmark_str << title_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str , value_list , destination_db_file , Q_FUNC_INFO);


}
