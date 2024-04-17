#include "thread_file_system.h"



void thread_file_system::parse_generations_table_data_document_revision()
{

    // Do not delete ---- parser of document revision ======


    //    QString db_filepath = doc_rev_dir_path + "/db-V1/db.sqlite";

    //    QString shm_source_file_path = db_filepath + "-shm";
    //    QString wal_source_file_path = db_filepath + "-wal";

    //    QString dest_temp_path     = adhoc_path_doc_revision + "db.sqlite";
    //    QString shm_dest_file_path = adhoc_path_doc_revision + "db.sqlite" + "-shm";
    //    QString wal_dest_file_path = adhoc_path_doc_revision + "db.sqlite" + "-wal";

    //    QFileInfo info_temp_check(dest_temp_path);
    //    if(info_temp_check.exists())
    //    {
    //        QFile temp_file(dest_temp_path);
    //        temp_file.remove();
    //    }
    //    QFile::copy(db_filepath,dest_temp_path);

    //    QFileInfo shm_files(shm_source_file_path);
    //    QFileInfo wal_files(wal_source_file_path);

    //    if(shm_files.exists())
    //    {
    //        QFile::copy(shm_source_file_path, shm_dest_file_path);
    //    }

    //    if(wal_files.exists())
    //    {
    //        QFile::copy(wal_source_file_path, wal_dest_file_path);
    //    }


    //    QSqlDatabase source_db;
    //    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    //    source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    //    source_db.setDatabaseName(dest_temp_path);
    //    if(!source_db.open())
    //    {
    //        recon_static_functions::app_debug("Source Db opening for table creation-------FAILED------" +  source_db.databaseName() , Q_FUNC_INFO);
    //        recon_static_functions::app_debug(source_db.lastError().text(), Q_FUNC_INFO);
    //        return;
    //    }


    //    QSqlQuery src_query(source_db);
    //    QString source_cmd = "select generation_id,generation_storage_id,generation_name,generation_client_id,generation_path,generation_options,generation_status,"
    //                         "generation_add_time,generation_size,generation_prunable from generations";

    //    src_query.prepare(source_cmd);

    //    if(!src_query.exec())
    //    {
    //        recon_static_functions::app_debug(" src_query execution -----FAILED------" + source_db.databaseName(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(src_query.lastError().text(), Q_FUNC_INFO);
    //        recon_static_functions::app_debug(src_query.executedQuery(), Q_FUNC_INFO);
    //        source_db.close();
    //        return;
    //    }

    //    ///generation table
    //    QString generation_id,generation_storage_id,generation_name,generation_client_id;
    //    QString generation_path,generation_options,generation_status;
    //    QString generation_add_time,generation_size,generation_prunable;

    //    QString complete_gen_filepath;

    //    ///files table
    //    QString file_row_id,file_name,file_parent_id,file_path,file_inode;
    //    QString file_last_seen,file_status,file_document_id;

    //    QString file_cmd = QString("select file_row_id,file_name,file_parent_id,file_path,file_inode,file_last_seen,file_status,file_document_id  from files Where file_storage_id = ?");
    //    QSqlQuery file_query(source_db);


    //    ///destination insert
    //    QSqlQuery query_insert(doc_rev_destination_db);

    //    QString cmd_insert = QString("insert into document_revision(bookmark,generation_id,generation_storage_id,generation_name,"
    //                                 "generation_client_id,generation_path,generation_options,generation_status,generation_add_time,"
    //                                 "generation_size,generation_prunable,file_row_id,file_name,file_parent_id,file_path,file_inode,"
    //                                 "file_last_seen,file_status,file_document_id,source_count_name) values(0,?,?,?,?,?, ?,?,?,?,? ,?,?,?,?,? ,?,?,?,?)");


    //    while(src_query.next())
    //    {

    //        generation_id   = src_query.value(0).toString().trimmed();
    //        generation_storage_id  = src_query.value(1).toString().trimmed();
    //        generation_name = src_query.value(2).toString().trimmed();
    //        generation_client_id  = src_query.value(3).toString().trimmed();
    //        generation_path = src_query.value(4).toString().trimmed();
    //        generation_options  = src_query.value(5).toString().trimmed();
    //        generation_status   = src_query.value(6).toString().trimmed();
    //        generation_add_time = src_query.value(7).toString().trimmed();
    //        generation_size = src_query.value(8).toString().trimmed();
    //        generation_prunable = src_query.value(9).toString().trimmed();

    //        complete_gen_filepath = doc_rev_dir_path + generation_path;

    //        file_query.prepare(file_cmd);
    //        file_query.addBindValue(generation_storage_id);

    //        if(!file_query.exec())
    //        {
    //            recon_static_functions::app_debug(" file_query execution -----FAILED------" + source_db.databaseName(), Q_FUNC_INFO);
    //            recon_static_functions::app_debug( " error " + file_query.lastError().text(), Q_FUNC_INFO);
    //            recon_static_functions::app_debug(file_query.executedQuery(), Q_FUNC_INFO);
    //        }


    //        if(file_query.next())
    //        {
    //            file_row_id = file_query.value(0).toString().trimmed();
    //            file_name   = file_query.value(1).toString().trimmed();
    //            file_parent_id = file_query.value(2).toString().trimmed();
    //            file_path  = file_query.value(3).toString().trimmed();
    //            file_inode = file_query.value(4).toString().trimmed();
    //            file_last_seen = file_query.value(5).toString().trimmed();
    //            file_status    = file_query.value(6).toString().trimmed();
    //            file_document_id  = file_query.value(7).toString().trimmed();
    //        }

    //        query_insert.prepare(cmd_insert);

    //        query_insert.addBindValue(generation_id);
    //        query_insert.addBindValue(generation_storage_id);
    //        query_insert.addBindValue(generation_name);
    //        query_insert.addBindValue(generation_client_id);
    //        query_insert.addBindValue(complete_gen_filepath);
    //        query_insert.addBindValue(generation_options);
    //        query_insert.addBindValue(generation_status);
    //        query_insert.addBindValue(generation_add_time);
    //        query_insert.addBindValue(generation_size);
    //        query_insert.addBindValue(generation_prunable);

    //        query_insert.addBindValue(file_row_id);
    //        query_insert.addBindValue(file_name);
    //        query_insert.addBindValue(file_parent_id);
    //        query_insert.addBindValue(file_path);
    //        query_insert.addBindValue(file_inode);
    //        query_insert.addBindValue(file_last_seen);
    //        query_insert.addBindValue(file_status);
    //        query_insert.addBindValue(file_document_id);
    //        query_insert.addBindValue(struct_target_info_ptr.source_count_name);

    //        if(!query_insert.exec())
    //        {
    //            recon_static_functions::app_debug(" insert execution -----FAILED------" + doc_rev_destination_db.databaseName(), Q_FUNC_INFO);
    //            recon_static_functions::app_debug( " error " + query_insert.lastError().text(), Q_FUNC_INFO);
    //            recon_static_functions::app_debug(query_insert.executedQuery(), Q_FUNC_INFO);
    //        }

    //    }

    //    source_db.close();
}



