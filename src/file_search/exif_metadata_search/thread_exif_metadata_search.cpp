#include "thread_exif_metadata_search.h"

thread_exif_metadata_search::thread_exif_metadata_search(QWidget *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}


void thread_exif_metadata_search::run()
{
    recon_static_functions::app_debug(" Start" , Q_FUNC_INFO);

    bool_cancel_extraction = false;

    insert_file_search_config_into_index_db();



    destination_db_path = create_file_search_destination_db();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("destination_db Db opening --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    destination_db.transaction();

    emit signal_PBAR_thread_exif_metadata_search_started(MACRO_JobType_Exif_Metadata_Search);

    QSqlQuery query_insert(destination_db);
    query_insert.prepare("INSERT INTO files(filesystem_record, source_count_name, make, model,author, latitude, longitude) VALUES (?,?,?,?, ?,?,?)");

    processed_records = 0;
    for(int src_count = 0; src_count < sources_list.size(); src_count++)
    {
        if(bool_cancel_extraction)
            break;


        QString source_count_name = sources_list.at(src_count);

        QString source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/exif_metadata.sqlite";

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

        QString count_command = "select count(*) from files where ";
        count_command.append(where_condition_command);
        QString total_records_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(QString(count_command) , 0, source_db, Q_FUNC_INFO);
        total_records_num = total_records_str.toLongLong();


        while(query_select.next())
        {

            if(bool_cancel_extraction)
                break;

            QString int_str = query_select.value("filesystem_record").toString().trimmed();

            QString make = query_select.value("make").toString().trimmed();

            QString model = query_select.value("model").toString().trimmed();

            QString author = query_select.value("author").toString().trimmed();

            QString latitude = query_select.value("latitude").toString().trimmed();

            QString longitude = query_select.value("longitude").toString().trimmed();


            query_insert.addBindValue(int_str);
            query_insert.addBindValue(source_count_name);
            query_insert.addBindValue(make);
            query_insert.addBindValue(model);
            query_insert.addBindValue(author);
            query_insert.addBindValue(latitude);
            query_insert.addBindValue(longitude);


            if(!query_insert.exec())
            {
                recon_static_functions::app_debug(" command -  " + query_insert.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
            }

            processed_records++;

            if(processed_records % 100 == 0)
            {
                qint64 progress_val = ((processed_records * 100) / total_records_num);

                if(progress_val > 99)
                    progress_val = 99;

                emit signal_PBAR_thread_exif_metadata_search_value(MACRO_JobType_Exif_Metadata_Search, sources_list.at(src_count), true, processed_records, progress_val, false);
            }
        }
    }

    destination_db.commit();

    destination_db.close();
    QSqlDatabase::removeDatabase(connection_naam);

    update_total_records_extracted_in_file_search_index_db();

    emit signal_PBAR_thread_exif_metadata_search_finished(MACRO_JobType_Exif_Metadata_Search);

    recon_static_functions::app_debug(" End" , Q_FUNC_INFO);
}

void thread_exif_metadata_search::pub_set_search_command_sources_search_label(QString command,QString where_statement, QStringList src_list, QString srch_label)
{
    search_command = command;
    where_condition_command = where_statement;

    sources_list = src_list;
    search_label = srch_label;

    search_db_name = search_label;
    search_db_name.replace(" ", "_");

    if(!search_db_name.isEmpty())
        search_db_name.append(".sqlite");

}


void thread_exif_metadata_search::pub_cancel_extraction()
{

    bool_cancel_extraction = true;


}

void thread_exif_metadata_search::insert_file_search_config_into_index_db()
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);
    QString file_search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + "index_exif_metadata_search.sqlite";
    QString command = "INSERT INTO exif_metadata_search_index(search_label_name, search_db_file_name,processed_sources) "
                      "VALUES (?,?,?)";

    QString processed_sources;
    for(int i = 0; i < sources_list.size(); i++)
    {
        if(bool_cancel_extraction)
            break;

        processed_sources.append(sources_list.at(i) + MACRO_RECON_Splitter_1_multiple);

    }

    if(processed_sources.endsWith(";++::++;"))
    {
        processed_sources.chop(8);
    }


    QStringList arg_list;
    arg_list << search_label << search_db_name << processed_sources ;


    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, arg_list, file_search_db_path, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("Exif Metadata Search Index Db insertion --------FAILED------ " + file_search_db_path,Q_FUNC_INFO);
    }


    recon_static_functions::app_debug("END ",Q_FUNC_INFO);

}


QString thread_exif_metadata_search::create_file_search_destination_db()
{

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + search_db_name;

    QString command = "CREATE TABLE files(INT INTEGER PRIMARY KEY, filesystem_record INTEGER, source_count_name VARCHAR(1024), make VARCHAR(1024), model VARCHAR(1024), author VARCHAR(1024), latitude VARCHAR(1024),longitude VARCHAR(1024))";
    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command, db_path, Q_FUNC_INFO))
    {

        recon_static_functions::app_debug("Exif Metadata Search Db creation --------FAILED------ " + db_path,Q_FUNC_INFO);
        return "";
    }

    return db_path;

}

void thread_exif_metadata_search::update_total_records_extracted_in_file_search_index_db()
{
    recon_static_functions::app_debug("Start  " + search_label,Q_FUNC_INFO);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + "index_exif_metadata_search.sqlite";

    QString command = "UPDATE exif_metadata_search_index SET total_records_extracted = ? WHERE search_label_name = ?";
    QStringList arg_list;
    arg_list << QString::number(processed_records) << search_label;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("end " + search_label,Q_FUNC_INFO);

}


