#include "thread_file_search_regular_filters.h"

thread_file_search_regular_filters::thread_file_search_regular_filters(QWidget *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

void thread_file_search_regular_filters::run()
{
    recon_static_functions::app_debug(" Start" , Q_FUNC_INFO);

    bool_cancel_extraction = false;

    insert_file_search_config_into_index_db();

    destination_db_path = create_file_search_destination_db();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("destination_db Db opening --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    destination_db->transaction();

    QSqlQuery query_insert(*destination_db);
    query_insert.prepare("INSERT INTO files(filesystem_record, source_count_name) VALUES (?,?)");

    total_records = 0;
    for(int src_count = 0; src_count < sources_list.size(); src_count++)
    {
        if(bool_cancel_extraction)
            break;


        QString source_count_name = sources_list.at(src_count);

        QString source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam + source_count_name);
        QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam + source_count_name);
        source_db.setDatabaseName(source_db_path);
        if(!source_db.open())
        {
            recon_static_functions::app_debug("source_db Db opening --------FAILED------ " + source_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + source_db.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        QSqlQuery query_select(source_db);

        if(!query_select.exec(search_command))
        {
            recon_static_functions::app_debug("Query execute  --------FAILED------ " + source_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug(search_command, Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + query_select.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        while(query_select.next())
        {

            if(bool_cancel_extraction)
                break;

            QString int_str = query_select.value(0).toString().trimmed();

            query_insert.addBindValue(int_str);
            query_insert.addBindValue(source_count_name);
            if(!query_insert.exec())
            {
                recon_static_functions::app_debug(" command -  " + query_insert.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
            }

            total_records++;
        }
    }

    destination_db->commit();

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    update_total_records_extracted_in_file_search_index_db();

    recon_static_functions::app_debug(" End" , Q_FUNC_INFO);
}

void thread_file_search_regular_filters::pub_set_search_command_sources_search_label(QString command, QStringList src_list, QString srch_label)
{
    search_command = command;
    sources_list = src_list;
    search_label = srch_label;

    search_db_name = search_label;
    search_db_name.replace(" ", "_");
}

void thread_file_search_regular_filters::pub_cancel_extraction()
{
    bool_cancel_extraction = true;
}


void thread_file_search_regular_filters::insert_file_search_config_into_index_db()
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    QString file_search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";
    QString command = "INSERT INTO file_search_index(search_label_name, search_db_file_name) "
                      "VALUES (?,?)";


    QStringList arg_list;
    arg_list << search_label << search_db_name;


    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, arg_list, file_search_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("File Search Index Db insertion --------FAILED------ " + file_search_db_path,Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("END ",Q_FUNC_INFO);
}

QString thread_file_search_regular_filters::create_file_search_destination_db()
{
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + search_db_name + ".sqlite";

    QString command = "CREATE TABLE files(INT INTEGER PRIMARY KEY, filesystem_record INTEGER, source_count_name VARCHAR(1024))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, db_path, Q_FUNC_INFO))
    {

        recon_static_functions::app_debug("File Search Db creation --------FAILED------ " + db_path,Q_FUNC_INFO);
        return "";
    }

    return db_path;
}

void thread_file_search_regular_filters::update_total_records_extracted_in_file_search_index_db()
{
    recon_static_functions::app_debug("Start  " + search_db_name,Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";

    QString command = "UPDATE file_search_index SET total_records_extracted = ? WHERE search_label_name = ?";
    QStringList arg_list;
    arg_list << QString::number(total_records) << search_db_name;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("end " + search_db_name,Q_FUNC_INFO);
}
