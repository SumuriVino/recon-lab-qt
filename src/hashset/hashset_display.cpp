#include "hashset_display.h"
#include "ui_hashset_display.h"

hashset_display::hashset_display(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hashset_display)
{
    ui->setupUi(this);

    m_tablewidget_obj = new m_tablewidget(this);

    QVBoxLayout *vb_lay1 = new QVBoxLayout;
    vb_lay1->addWidget(m_tablewidget_obj);
    vb_lay1->setSizeConstraint(QLayout::SetMinAndMaxSize);

    vb_lay1->setContentsMargins(8,8,8,8);
    setLayout(vb_lay1);

    m_tablewidget_obj->installEventFilter(this);
    m_tablewidget_obj->verticalHeader()->setFixedWidth(50);
    m_tablewidget_obj->verticalHeader()->setHighlightSections(false);
    m_tablewidget_obj->horizontalHeader()->setHighlightSections(false);
    m_tablewidget_obj->horizontalHeader()->stretchLastSection();
    m_tablewidget_obj->verticalHeader()->stretchLastSection();

    bool_file_exist_with_same_hash = false;

    message_dialog_object = new message_dialog(this);

    bool_is_from_hashset_db = false;

    setWindowTitle("Hashset: Searched File");

    recon_helper_standard_obj = new recon_helper_standard(this);

}

hashset_display::~hashset_display()
{
    delete ui;
}


void hashset_display::pub_set_essentials()
{
    hashset_master_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    recon_result_hashset_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hashset_In_Result_QString).toString() + "searched_files.sqlite";

}

void hashset_display::pub_set_is_from_other_hash_database(bool value)
{
    bool_is_from_hashset_db = value;
}

void hashset_display::pub_search_file_in_hash_db(QString file_path, QString source_disp_path, QString selected_db_path, QString record_number, QString plugin_name, QString tab_name, QString parent_plugin_name)
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(hashset_master_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + hashset_master_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    if(selected_db_path == "All")
    {

        QString command = "SELECT database_path FROM tbl_hashset Where bookmark = 1";

        QStringList hashset_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,0,destination_db,Q_FUNC_INFO);
        if(hashset_db_path_list.isEmpty())
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Hashset","Database list is empty");
            message_dialog_object->show();
            destination_db.close();
            return;
        }

        QString md5_hash = recon_static_functions::create_md5(file_path, Q_FUNC_INFO);


        for(int i = 0; i < hashset_db_path_list.size(); i++)
        {
            QString hashset_db_path = hashset_db_path_list.at(i);
            insert_hashset_in_result(file_path,source_disp_path,md5_hash, hashset_db_path,record_number, plugin_name, tab_name,parent_plugin_name, true);
        }

    }
    else
    {

        QFile file(selected_db_path);
        if(!file.exists())
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"Selected database does not exist");
            message_dialog_object->show();
            destination_db.close();
            return;
        }

        QString md5_hash = recon_static_functions::create_md5(file_path, Q_FUNC_INFO);

        if(bool_is_from_hashset_db)
            insert_hashset_in_result(file_path, source_disp_path, md5_hash, selected_db_path,record_number, plugin_name, tab_name,parent_plugin_name,true);
        else
            insert_hashset_in_result(file_path, source_disp_path, md5_hash, selected_db_path,record_number, plugin_name, tab_name,parent_plugin_name,false);

    }

    destination_db.close();
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void hashset_display::insert_hashset_in_result(QString filepath, QString source_disp_path, QString md5_hash, QString hash_db_path,QString record_number, QString plugin_name, QString tab_name, QString parent_plugin_name, bool is_from_other_hash_db)
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    /*Note:
      * We are not providing any default hashset.
      * So it will always be other db (true).
     */
    is_from_other_hash_db = true;

    QString hash_column_name,hash_db_table_name;
    if(is_from_other_hash_db)
    {
        QString command = QString("Select table_name From tbl_hashset Where database_path = ?");
        QStringList value_list;
        value_list << hash_db_path;
        hash_db_table_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , value_list, 0, hashset_master_db_path, Q_FUNC_INFO);
        hash_db_table_name = hash_db_table_name.trimmed();

        if(hash_db_table_name.isEmpty())
            return;

        command = QString("Select column_name From tbl_hashset Where database_path = ?");
        hash_column_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command , value_list, 0, hashset_master_db_path, Q_FUNC_INFO);
        hash_column_name = hash_column_name.trimmed();

        if(hash_column_name.isEmpty())
            return;
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_result_hashset_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + recon_result_hashset_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlDatabase::removeDatabase(connection_naam + QString("_1"));
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + QString("_1"));
    source_db.setDatabaseName(hash_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("Source db opening---FAILED----" + hash_db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error ----  " + source_db.lastError().text() , Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    QSqlQuery query_select(source_db);

    //just to check if record exists.
    if(is_from_other_hash_db)
        query_select.prepare("Select "+ hash_column_name + " From " + hash_db_table_name + " Where " + hash_column_name + " = ?");
    else
        query_select.prepare("SELECT INT,md5,sha1 FROM files WHERE md5 = ?");

    query_select.addBindValue(md5_hash);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug("query_select execution---FAILED----" + hash_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error ----  " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("query " + query_select.executedQuery() , Q_FUNC_INFO);
        source_db.close();
        destination_db.close();
        return;
    }

    while (query_select.next())
    {
        bool_file_exist_with_same_hash = true;

        QFileInfo info_file(filepath);

        QString file_name       = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(source_disp_path);
        QString modification    = QString::number(info_file.lastModified().toSecsSinceEpoch());
        QString creation        = QString::number(info_file.birthTime().toSecsSinceEpoch());
        QString access          = QString::number(info_file.lastRead().toSecsSinceEpoch());
        QString size            = recon_static_functions::mining_size(filepath, Q_FUNC_INFO);
        QString suffix          = recon_helper_standard_obj->mining_suffix(filepath, Q_FUNC_INFO);
        QString user_id         = QString::number(info_file.ownerId());
        QString group_id        = QString::number(info_file.groupId());

        QStringList list;
        list << file_name << source_disp_path << suffix << modification << creation << access << size << user_id << group_id << md5_hash << hash_db_path << plugin_name << tab_name << record_number << parent_plugin_name;

        QString command = "insert into 'files' (file_name,file_path,suffix,modification_time,creation_time,access_time,file_size,user_id,group_id,md5_hash,hash_db_path,plugin_name,plugin_tabname,tab_Record_no, parent_plugin_name) values (?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?)";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);

        ///set hash match status to YES in hash DB
        command = QString("UPDATE files set hash_matched = ? Where md5_hash = ?");
        QStringList value_list;
        value_list << QString("YES") << md5_hash;
        QString db_hashset_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hashset_In_Result_QString).toString() + QFileInfo(hash_db_path).fileName();
        if(QFileInfo(db_hashset_path).exists())
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list,db_hashset_path , Q_FUNC_INFO);

        // Insert entry in searched index db so that we can insert entry in tree for hashset on load case
        QString searched_hash_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hashset_In_Result_QString).toString() + "index_searched_hashset.sqlite";
        QString command_search_index = "SELECT INT FROM searched_hashset_index WHERE hashset_name = ? AND hashset_db_path = ?";
        value_list.clear();
        value_list << QFileInfo(hash_db_path).fileName().replace(".sqlite", "") << hash_db_path;

        QString INT_STR = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_search_index, value_list, 0, searched_hash_index_db_path, Q_FUNC_INFO);
        if(INT_STR.isEmpty())
        {
            command_search_index = "insert into 'searched_hashset_index' (hashset_name,hashset_db_path) values (?,?)";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_search_index,value_list,searched_hash_index_db_path,Q_FUNC_INFO);
        }
    }

    source_db.close();
    destination_db.close();
    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);
}

bool hashset_display::display_data_in_hashset_tablewidget()
{
    recon_static_functions::app_debug("Starts" , Q_FUNC_INFO);

    if(!bool_file_exist_with_same_hash)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"File with same hash does not exist in database");
        message_dialog_object->show();
        return false;
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(recon_result_hashset_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + recon_result_hashset_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error ----  " + destination_db.lastError().text() , Q_FUNC_INFO);
        return false;
    }

    QSqlQuery query_select(destination_db);
    query_select.prepare("SELECT file_name,file_path,suffix,modification_time,file_size,md5_hash,hash_db_path FROM files");
    if(!query_select.exec())
    {
        recon_static_functions::app_debug("query execute---FAILED----" + recon_result_hashset_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error ----  " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Query " + query_select.executedQuery() , Q_FUNC_INFO);
        destination_db.close();
        return false;
    }

    while(m_tablewidget_obj->rowCount())
        m_tablewidget_obj->removeRow(0);

    QStringList headerlist;
  //  headerlist <<"INT"<< "Hashset Name" << "File Name" <<  "MD5" <<  "File Size" <<"Date Modified" << "Hashset Path" <<"File Path";
    headerlist <<"INT"<< "Hashset Name" << "File Name" <<  "MD5" << "Hashset Path" <<"File Path";

    m_tablewidget_obj->setColumnCount(headerlist.size());
    m_tablewidget_obj->setHorizontalHeaderLabels(headerlist);

    int k = 0;
    while (query_select.next())
    {
        m_tablewidget_obj->insertRow(k);
        m_tablewidget_obj->setRowHeight(k,22);

        QFileInfo file(query_select.value(6).toString());
        QString dbname = file.fileName();
        if(dbname.endsWith(".sqlite"))
            dbname.remove(".sqlite");

        QTableWidgetItem *item_int = new QTableWidgetItem;
        QTableWidgetItem *item_filename = new QTableWidgetItem;
        QTableWidgetItem *item_suffix = new QTableWidgetItem;
//        QTableWidgetItem *item_file_size = new QTableWidgetItem;
//        QTableWidgetItem *item_date_lastmodified = new QTableWidgetItem;
        QTableWidgetItem *item_md5 = new QTableWidgetItem;
        QTableWidgetItem *item_filepath = new QTableWidgetItem;
        QTableWidgetItem *item_db_path = new QTableWidgetItem;
        QTableWidgetItem *item_db_name = new QTableWidgetItem;

        item_int->setText(QString::number(k));
        item_filename->setText(query_select.value(0).toString());
        item_filepath->setText(query_select.value(1).toString());
        item_suffix->setText(query_select.value(2).toString());

//        item_date_lastmodified->setText(global_recon_555_helper_singular_class_obj->convert_epoch_to_custom_for_display(query_select.value(3).toString(), Q_FUNC_INFO));
//        item_file_size->setText(recon_static_functions::human_readable_size(query_select.value(4).toString(), Q_FUNC_INFO));
        item_md5->setText(query_select.value(5).toString());
        item_db_path->setText(query_select.value(6).toString());
        item_db_name->setText(dbname);

        m_tablewidget_obj->setItem(k,enum_hashset_display_INT,item_int);
        m_tablewidget_obj->setItem(k,enum_hashset_display_Filename,item_filename);
//        m_tablewidget_obj->setItem(k,enum_hashset_display_Size,item_file_size);
//        m_tablewidget_obj->setItem(k,enum_hashset_display_Datemodified,item_date_lastmodified);
        m_tablewidget_obj->setItem(k,enum_hashset_display_Md5,item_md5);
        m_tablewidget_obj->setItem(k,enum_hashset_display_Path,item_filepath);
        m_tablewidget_obj->setItem(k,enum_hashset_display_DB_Name,item_db_name);
        m_tablewidget_obj->setItem(k,enum_hashset_display_DB_Path,item_db_path);

        k++;
    }

    m_tablewidget_obj->setColumnWidth(enum_hashset_display_Filename,200);
//    m_tablewidget_obj->setColumnWidth(enum_hashset_display_Size,75);
//    m_tablewidget_obj->setColumnWidth(enum_hashset_display_Datemodified,140);
    m_tablewidget_obj->setColumnWidth(enum_hashset_display_Md5,140);
    m_tablewidget_obj->setColumnWidth(enum_hashset_display_Path,400);
    m_tablewidget_obj->setColumnWidth(enum_hashset_display_DB_Name,200);
    m_tablewidget_obj->setColumnWidth(enum_hashset_display_DB_Path,500);
   // m_tablewidget_obj->horizontalHeader()->stretchLastSection();

    m_tablewidget_obj->hideColumn(enum_hashset_display_INT);

    bool_file_exist_with_same_hash = false;
    destination_db.close();

    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);
    return true;
}

