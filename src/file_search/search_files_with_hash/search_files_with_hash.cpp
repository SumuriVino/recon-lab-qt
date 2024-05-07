#include "search_files_with_hash.h"
#include "ui_search_files_with_hash.h"

search_files_with_hash::search_files_with_hash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::search_files_with_hash)
{
    ui->setupUi(this);

    setWindowTitle(QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash));

    recon_helper_standard_obj = new recon_helper_standard(this);

    ui->tableWidget_extract_files_with_same_hash->hideColumn(enum_Search_Hashes_display_OS_Naming_Scheme);
    ui->tableWidget_extract_files_with_same_hash->hideColumn(enum_Search_Hashes_display_Source_Count_Name);
    ui->tableWidget_extract_files_with_same_hash->hideColumn(enum_Search_Hashes_display_Record);

    ui->tableWidget_extract_files_with_same_hash->setAlternatingRowColors(true);
    ui->tableWidget_extract_files_with_same_hash->setShowGrid(false);
    ui->tableWidget_extract_files_with_same_hash->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_Record , 100);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_File_Name , 200);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_File_Path , 300);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_Source_Name , 200);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_Hash_MD5 , 300);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_Source_Count_Name , 100);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_OS_Naming_Scheme , 100);
    ui->tableWidget_extract_files_with_same_hash->setColumnWidth(enum_Search_Hashes_display_Open_in_Source_Link , 100);

}

search_files_with_hash::~search_files_with_hash()
{
    delete ui;
}

void search_files_with_hash::pub_set_global_connection_manager_obj(global_connection_manager *obj)
{
    global_connection_manager_obj = obj;
}

void search_files_with_hash::pub_set_display_loading_progress_bar_obj(display_loading_progress_bar *object)
{
    display_loading_progress_bar_obj = object;
}

void search_files_with_hash::pub_refresh_tableWidget()
{
    ui->tableWidget_extract_files_with_same_hash->setRowCount(0);
}

void search_files_with_hash::pub_search_files_having_same_hash(QString hash_md5, QString record, QString source_count_name, QString com_file_path)
{ // search files which has same hash value
    bool search_result_found = false;

    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait....");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    if(hash_md5.isEmpty())
    {
        display_loading_progress_bar_obj->pub_set_label_messsge("Hash calculate....");
        struct_global_file_hashes obj = recon_helper_standard_obj->pub_create_md5_and_sha1_hash(com_file_path);
        hash_md5 = obj.md5_hash;
        if(hash_md5.isEmpty())
        {
            display_loading_progress_bar_obj->hide();
            QMessageBox::information(this, recon_static_functions::get_app_name(), "Empty File Found.");
            return;
        }
    }

    display_loading_progress_bar_obj->pub_set_label_messsge("Searching....");

    QList<struct_GLOBAL_witness_info_source> added_sources_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int ii = 0; ii < added_sources_list.size(); ii++)
    {
        QCoreApplication::processEvents();

        struct_GLOBAL_witness_info_source trgt_info_obj = added_sources_list.at(ii);

        QString crnt_trgt_src_cnt_name = trgt_info_obj.source_count_name;

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + crnt_trgt_src_cnt_name + "/" + "file_system.sqlite";
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        db.setDatabaseName(db_path);
        if(!db.open())
        {
            recon_static_functions::app_debug("Database open ------FAILED------- ",Q_FUNC_INFO);
            recon_static_functions::app_debug("Error" + db.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        QSqlQuery query(db);

        QString command = "select INT,filename,filepath from files Where hash_md5 = ?";
        query.prepare(command);
        query.addBindValue(hash_md5);
        if(!query.exec())
        {
            recon_static_functions::app_debug(" hashes_database_open ---FAILED---",Q_FUNC_INFO);
            recon_static_functions::app_debug("  error " + query.lastError().text(),Q_FUNC_INFO);
            db.close();
            continue;
        }

        int tmp_count = 0;
        while(query.next())
        {
            if(tmp_count % 5 == 0)
            {
                tmp_count = 0;
                QCoreApplication::processEvents();
            }

            if(record == query.value("INT").toString() && crnt_trgt_src_cnt_name == source_count_name)
            {
                continue;
            }
            else
            {
                search_result_found = true;

                display_list_of_files_having_same_hash(query.value("INT").toString(), query.value("filename").toString(), query.value("filepath").toString(), trgt_info_obj.source_name, hash_md5, crnt_trgt_src_cnt_name, command);
            }

            tmp_count++;
        }

        db.close();
    }

    if(!search_result_found)
    {
        display_loading_progress_bar_obj->hide();
        QMessageBox::information(this, recon_static_functions::get_app_name(), "No matching file(s) found.");
        return;
    }

    display_loading_progress_bar_obj->hide();

    show();
}


void search_files_with_hash::display_list_of_files_having_same_hash(QString record, QString file_name, QString file_path, QString source_name, QString hash_md5, QString source_c_name, QString os_scheme)
{ //  display list of files on tablewidget which has the same hash value
    int row  = ui->tableWidget_extract_files_with_same_hash->rowCount();

    if(row < 0)
        return;

    QTableWidgetItem *item_record_no = new QTableWidgetItem;
    QTableWidgetItem *item_file_name = new QTableWidgetItem;
    QTableWidgetItem *item_file_path = new QTableWidgetItem;
    QTableWidgetItem *item_source_name = new QTableWidgetItem;
    QTableWidgetItem *item_hash_md5 = new QTableWidgetItem;
    QTableWidgetItem *item_source_count_name = new QTableWidgetItem;
    QTableWidgetItem *item_os_nm_scheme = new QTableWidgetItem;

    item_record_no->setText(record);
    item_file_name->setText(file_name);
    item_file_path->setText(file_path);
    item_source_name->setText(source_name);
    item_hash_md5->setText(hash_md5);
    item_source_count_name->setText(source_c_name);
    item_os_nm_scheme->setText(os_scheme);

    ui->tableWidget_extract_files_with_same_hash->insertRow(row);

    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_Record , item_record_no);
    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_File_Name , item_file_name);
    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_File_Path , item_file_path);
    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_Source_Name , item_source_name);
    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_Hash_MD5 , item_hash_md5);
    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_Source_Count_Name , item_source_count_name);
    ui->tableWidget_extract_files_with_same_hash->setItem(row , enum_Search_Hashes_display_OS_Naming_Scheme , item_os_nm_scheme);

    QString roww = QString::number(row);

    QLabel *label_open_in_source = new QLabel();
    label_open_in_source->setText("<a href="+ roww + " > " + QString("Open in Source") + " </a>");
    label_open_in_source->setTextInteractionFlags(Qt::TextBrowserInteraction);

    ui->tableWidget_extract_files_with_same_hash->setCellWidget(row, enum_Search_Hashes_display_Open_in_Source_Link , label_open_in_source);

    connect(label_open_in_source, SIGNAL(linkActivated(QString)), this, SLOT(slot_open_in_source_link_clicked(QString)));

}

void search_files_with_hash::slot_open_in_source_link_clicked(QString curr_row)
{ // action to open in source link clicked
    curr_row = curr_row.trimmed();
    if(curr_row.isEmpty())
        return;

    int row = curr_row.toInt();

    if(row < 0)
        return;

    QString record = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_Record)->text();
    QString file_name = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_File_Name)->text();
    QString file_path = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_File_Path)->text();
    QString source_name = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_Source_Name)->text();
    QString hash = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_Hash_MD5)->text();
    QString source_count_name = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_Source_Count_Name)->text();
    QString os_nm_scheme = ui->tableWidget_extract_files_with_same_hash->item(row , enum_Search_Hashes_display_OS_Naming_Scheme)->text();

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/" + "file_system.sqlite";

    global_connection_manager_obj->pub_set_files_list_having_same_hash(source_name, QString(MACRO_CASE_TREE_Source), file_path, record, db_path , os_nm_scheme);


}
