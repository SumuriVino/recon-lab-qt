#include "global_recon_file_info.h"

global_recon_file_info::global_recon_file_info(QObject *parent) : QObject(parent)
{

    recon_helper_standard_obj = new recon_helper_standard(this);

}


struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_file_system_by_dbreference(QString record_no, QSqlDatabase &dest_db,QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "files";

    QString select_cmd = QString("Select  filepath, recon_filefrom ,recon_file_infopath ,id_path From  '" + table_name + "' Where INT = ?");
    QSqlQuery select_query(dest_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }


    struct_GLOBAL_witness_info_source struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);



    if(select_query.next())
    {
        QString recon_file_from;
        QString recon_fileinfo_path;
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,dest_db,table_name,source_count_name,Q_FUNC_INFO);

        if(struct_target_source_info.bool_source_decompressed)
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

            recon_fileinfo_path = complete_file_path;
            if(recon_fileinfo_path.startsWith(result_dir_path))
                recon_fileinfo_path.remove(0,result_dir_path.size());


            recon_file_from = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
        }
        else
        {
            recon_file_from = select_query.value("recon_filefrom").toString();
            recon_fileinfo_path = select_query.value("recon_file_infopath").toString();
        }
        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("filepath").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;
        st_glbl_rcn_f_info_obj.bool_file_from_result = false;
        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = select_query.value("id_path").toString();
        st_glbl_rcn_f_info_obj.source_count_name = source_count_name;
        st_glbl_rcn_f_info_obj.bool_file_is_dir = recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no, dest_db,table_name,source_count_name,Q_FUNC_INFO);
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = dest_db.databaseName();
    }


    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;

}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_file_system_by_db_path(QString record_no , QString fs_db_path, QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    st_glbl_rcn_f_info_obj = pub_get_recon_info_for_file_system_by_dbreference(record_no ,destination_db,source_count_name);

    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_snapshots_by_db_path(QString record_no ,QString display_tab_name, QString fs_db_path, QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(QString(MACRO_Plugin_Name_Snapshots) , display_tab_name);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(fs_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }


    QString select_cmd = QString("Select  filepath, recon_filefrom ,recon_file_infopath ,id_path From  '" + table_name + "' Where INT = ?");
    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }


    struct_GLOBAL_witness_info_source struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(select_query.next())
    {
        QString recon_file_from;
        QString recon_fileinfo_path;
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);

        if(struct_target_source_info.bool_source_decompressed)
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

            recon_fileinfo_path = complete_file_path;
            if(recon_fileinfo_path.startsWith(result_dir_path))
                recon_fileinfo_path.remove(0,result_dir_path.size());


            recon_file_from = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
        }
        else
        {
            recon_file_from = select_query.value("recon_filefrom").toString();
            recon_fileinfo_path = select_query.value("recon_file_infopath").toString();
        }
        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("filepath").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;
        st_glbl_rcn_f_info_obj.bool_file_from_result = false;
        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = select_query.value("id_path").toString();
        st_glbl_rcn_f_info_obj.source_count_name = source_count_name;
        st_glbl_rcn_f_info_obj.bool_file_is_dir = recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no, destination_db,table_name,source_count_name,Q_FUNC_INFO);
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }

    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_carved_files_by_db_path(QString record_no, QString dest_db_path, QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "files";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }
    struct_GLOBAL_witness_info_source struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString select_cmd = ("Select  item1, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        if(struct_target_source_info.bool_source_decompressed)
        {
            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

            recon_fileinfo_path = complete_file_path;
            if(recon_fileinfo_path.startsWith(result_dir_path))
                recon_fileinfo_path.remove(0,result_dir_path.size());

            recon_file_from = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
        }
        else
        {
            recon_file_from = select_query.value("recon_filefrom").toString();
            recon_fileinfo_path = select_query.value("recon_file_infopath").toString();
        }
        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("item1").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = source_count_name;
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_screenshots(QString record_no)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "tags";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    QString select_cmd = ("Select  item1, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,"",Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("item1").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = "";
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_saved_maps(QString record_no)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "tags";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    QString select_cmd = ("Select  item1, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,"",Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("item1").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = "";
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_artifacts_timeline_graphs(QString record_no)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "tags";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    QString select_cmd = ("Select  item1, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,"",Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("item1").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = "";
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_messenger_saved_graphs(QString record_no)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "saved_graphs";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    QString  select_cmd = ("Select  graph_path, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");
    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,"",Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("graph_path").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = "";
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_browser_histroy_saved_graphs(QString record_no)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "saved_graphs";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    QString  select_cmd = ("Select  graph_path, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");
    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,"",Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("graph_path").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = "";
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_string_viewer(QString record_no, QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "string_content_info";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite" ;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }
    QString select_cmd = ("Select  file_path, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("file_path").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = source_count_name;
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_text_viewer(QString record_no, QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString table_name = "text_content_info";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite" ;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }
    QString select_cmd = ("Select  file_path, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }

    if(select_query.next())
    {
        QString complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);

        QString recon_file_from , recon_fileinfo_path ;
        recon_file_from = select_query.value("recon_filefrom").toString();
        recon_fileinfo_path = select_query.value("recon_file_infopath").toString();

        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
            recon_fileinfo_path.prepend("/");


        st_glbl_rcn_f_info_obj.display_file_path = select_query.value("file_path").toString();
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_fileinfo_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_file_from;

        st_glbl_rcn_f_info_obj.bool_file_from_result = true;

        st_glbl_rcn_f_info_obj.complete_file_path = complete_file_path;
        st_glbl_rcn_f_info_obj.id_path = "";
        st_glbl_rcn_f_info_obj.source_count_name = source_count_name;
        st_glbl_rcn_f_info_obj.db_table_name = table_name;
        st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_hex_viewer(QString record_no, QString source_count_name)
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);


    QString table_name = "hex_content_info";
    QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(dest_db_path);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open --FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }
    QString  select_cmd = ("Select content_file_path  From  '" + table_name +"' Where INT = ?");

    QSqlQuery select_query(destination_db);
    select_query.prepare(select_cmd);
    select_query.addBindValue(record_no);
    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();

        return st_glbl_rcn_f_info_obj;
    }


    if(select_query.next())
    {
        QString content_file_path = select_query.value("content_file_path").toString();
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        if(!content_file_path.isEmpty() && !content_file_path.startsWith(result_dir_path))
            content_file_path.prepend(result_dir_path);

        st_glbl_rcn_f_info_obj.complete_file_path = content_file_path;
        st_glbl_rcn_f_info_obj.source_count_name = source_count_name;
    }
    destination_db.close();

    recon_static_functions::app_debug(" End", Q_FUNC_INFO);

    return st_glbl_rcn_f_info_obj;
}

QList<struct_global_recon_file_info> global_recon_file_info::pub_get_recon_info_for_artifacts_all_file(QString plugin_name_str, QString tab_name_str , QString record_no)
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QList<struct_global_recon_file_info> st_recon_file_qlist;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString artifacts_db_file_path = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    if(!QFileInfo(artifacts_db_file_path).exists())
        return st_recon_file_qlist;


    bool bool_is_virtual_export = false;
    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return st_recon_file_qlist;


    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);


    QList<QStringList>md_preview_list_tablewidget;
    struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    md_preview_list_tablewidget = obj.metadata_preview_QList_tablewidget;
    QStringList preview_col_list = md_preview_list_tablewidget.at(tab_index);

    QList<QStringList>md_column_name_list_tablewidget;
    md_column_name_list_tablewidget = obj.metadata_column_name_QList_tablewidget;
    QStringList cols_nam_list =  md_column_name_list_tablewidget.at(tab_index);


    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
    }
    QString command = cols_nam_list.join(",");
    command = "select " + command + " from " + table_name + " where INT = " + record_no;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(artifacts_db_file_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return st_recon_file_qlist;
    }

    if(preview_index > -1)
    {
        QString media_file_path_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(command,preview_index,destination_db,Q_FUNC_INFO);
        QStringList m_path_list = media_file_path_str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

        if(m_path_list.size() >= 1)
        {
            QString complete_f_path;
            QString current_media_relative_path = QString::fromLocal8Bit(m_path_list.at(0).toLocal8Bit());

            if((current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images)))
                    || (current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
                    || (current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
            {

                bool_is_virtual_export = true;
                //                complete_f_path =  destination_plugin_dir_path + current_media_relative_path.remove(0,1);

                //                st_glbl_rcn_f_info_obj.complete_file_path = complete_f_path;
                //                st_recon_file_qlist << st_glbl_rcn_f_info_obj;

                for(int jj = 0; jj < m_path_list.size() ; jj++)
                {
                    QString tmp_f_path = QString::fromLocal8Bit(m_path_list.at(jj).toLocal8Bit());
                    complete_f_path =  destination_plugin_dir_path + tmp_f_path.remove(0,1);
                    st_glbl_rcn_f_info_obj.display_file_path = tmp_f_path;
                    st_glbl_rcn_f_info_obj.complete_file_path = complete_f_path;
                    st_recon_file_qlist << st_glbl_rcn_f_info_obj;
                }
            }
            else
            {
                bool_is_virtual_export = false;

                QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
                for(int j = 0; j < export_col_name_list.size(); j++)
                {
                    QString file_path_column = export_col_name_list.at(j);
                    QString cmd = "select " + file_path_column + " , source_count_name from '" + table_name + "' where INT =?";

                    QString media_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd,QStringList(record_no),0,destination_db,Q_FUNC_INFO);
                    if(media_file_path.trimmed().isEmpty())
                        continue;

                    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd,QStringList(record_no),1,destination_db,Q_FUNC_INFO);

                    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
                    QString virtual_source_path = struct_target_info.virtual_source_path;
                    QString os_schme_intrnl = struct_target_info.os_scheme_internal;

                    if(media_file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        QStringList temp_path_list = media_file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

                        for(int i = 0; i < temp_path_list.size() ; i++)
                        {
                            QString relative_path = temp_path_list.at(i);
                            QString file_info_path = relative_path;
                            if(!file_info_path.startsWith("/"))
                                file_info_path.prepend("/");

                            complete_f_path = recon_static_functions::get_complete_file_path(virtual_source_path, relative_path, Q_FUNC_INFO);
                            if(!complete_f_path.startsWith("/"))
                                complete_f_path.prepend("/");

                            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                                    || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                            {
                                st_glbl_rcn_f_info_obj.display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_info_path,source_count_name);
                            }
                            else
                                st_glbl_rcn_f_info_obj.display_file_path = file_info_path;

                            st_glbl_rcn_f_info_obj.complete_file_path = complete_f_path;
                            st_recon_file_qlist << st_glbl_rcn_f_info_obj;
                        }

                    }
                    else
                    {
                        QString file_info_path = media_file_path;
                        if(!file_info_path.startsWith("/"))
                            file_info_path.prepend("/");

                        complete_f_path = recon_static_functions::get_complete_file_path(virtual_source_path, media_file_path, Q_FUNC_INFO);
                        if(!complete_f_path.startsWith("/"))
                            complete_f_path.prepend("/");

                        if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                                || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                        {
                            st_glbl_rcn_f_info_obj.display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_info_path,source_count_name);
                        }
                        else
                        {
                            st_glbl_rcn_f_info_obj.display_file_path = file_info_path;

                        }

                        st_glbl_rcn_f_info_obj.complete_file_path = complete_f_path;

                        st_recon_file_qlist << st_glbl_rcn_f_info_obj;

                    }
                }
            }
        }
    }

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);
    return st_recon_file_qlist;

}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_artifacts_first_file_by_dbpath(QString plugin_name_str, QString tab_name_str , QString record_no, QString artifacts_db_file_path)
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");
    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugin_db_path = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    if(!QFileInfo(plugin_db_path).exists())
        return st_glbl_rcn_f_info_obj;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(plugin_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    st_glbl_rcn_f_info_obj = pub_get_recon_info_for_artifacts_first_file_by_dbreference(plugin_name_str,tab_name_str,record_no,destination_db);

    destination_db.close();

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);
    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_artifacts_first_file_by_dbreference(QString plugin_name_str, QString tab_name_str, QString record_no,QSqlDatabase &destination_db)
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    QStringList all_path_list;
    QStringList source_count_name_list;
    QStringList display_file_path_list;

    bool bool_is_virtual_export = false;

    QStringList recon_file_info_path_list;
    QStringList recon_filefrom_list;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return st_glbl_rcn_f_info_obj;


    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");
    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;



    st_glbl_rcn_f_info_obj.db_path = destination_db.databaseName();

    QList<QStringList>md_preview_list_tablewidget;
    struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    md_preview_list_tablewidget = obj.metadata_preview_QList_tablewidget;
    QStringList preview_col_list = md_preview_list_tablewidget.at(tab_index);

    QList<QStringList>md_column_name_list_tablewidget;
    md_column_name_list_tablewidget = obj.metadata_column_name_QList_tablewidget;
    QStringList cols_nam_list =  md_column_name_list_tablewidget.at(tab_index);


    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
    }
    QString command = cols_nam_list.join(",");
    command = "select " + command + " from " + table_name + " where INT = " + record_no;

    if(preview_index > -1)
    {
        QString media_file_path_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(command,preview_index,destination_db,Q_FUNC_INFO);
        QStringList m_path_list = media_file_path_str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

        if(m_path_list.size() >= 1)
        {
            QString complete_f_path;
            QString current_media_relative_path = m_path_list.at(0).toLocal8Bit();

            if((current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images)))
                    || (current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
                    || (current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
            {



                bool_is_virtual_export = true;

                complete_f_path =  destination_plugin_dir_path + current_media_relative_path.remove(0,1);

                all_path_list << complete_f_path;
                recon_filefrom_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
                recon_file_info_path_list << current_media_relative_path;
                display_file_path_list << current_media_relative_path;
                source_count_name_list << "";

            }
            else
            {
                bool_is_virtual_export = false;

                // QStringList export_col_name_list = st_csv_info_obj.export_path_column_QList_tablewidget.at(tab_index);
                QStringList export_col_name_list  = st_csv_info_obj.metadata_preview_QList_tablewidget.at(tab_index);
                for(int j = 0; j < export_col_name_list.size(); j++)
                {
                    QString file_path_column = export_col_name_list.at(j);
                    QString cmd = "select " + file_path_column + " , source_count_name from '" + table_name + "' where INT =?";

                    QString media_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd,QStringList(record_no),0,destination_db,Q_FUNC_INFO);
                    if(media_file_path.trimmed().isEmpty())
                        continue;

                    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd,QStringList(record_no),1,destination_db,Q_FUNC_INFO);

                    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
                    QString virtual_source_path = struct_target_info.virtual_source_path;
                    QString os_schme_intrnl = struct_target_info.os_scheme_internal;

                    if(media_file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        QStringList temp_path_list = media_file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

                        for(int i = 0; i < temp_path_list.size(); i++)
                        {
                            QString relative_path = temp_path_list.at(i);
                            QString file_info_path = relative_path;
                            if(!file_info_path.startsWith("/"))
                                file_info_path.prepend("/");


                            complete_f_path = recon_static_functions::get_complete_file_path(virtual_source_path, relative_path, Q_FUNC_INFO);
                            if(!complete_f_path.startsWith("/"))
                                complete_f_path.prepend("/");

                            recon_file_info_path_list << file_info_path;
                            recon_filefrom_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);

                            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                            {
                                ///preview file path column appended with virtual extension
                                /// if we use export file path column, no need to this
                                QString suffix = recon_helper_standard_obj->mining_suffix(complete_f_path , Q_FUNC_INFO);
                                suffix = suffix.trimmed();
                                if(complete_f_path.endsWith(suffix))
                                    complete_f_path.chop(suffix.size() + 1);

                                if(file_info_path.endsWith(suffix))
                                    file_info_path.chop(suffix.size() + 1);

                                display_file_path_list << recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_info_path,source_count_name);

                            }
                            else
                                display_file_path_list << file_info_path;

                            all_path_list << complete_f_path;
                            source_count_name_list << source_count_name;
                            break ;
                        }

                    }
                    else
                    {
                        QString file_info_path = media_file_path;
                        if(!file_info_path.startsWith("/"))
                            file_info_path.prepend("/");

                        complete_f_path = recon_static_functions::get_complete_file_path(virtual_source_path, media_file_path, Q_FUNC_INFO);
                        if(!complete_f_path.startsWith("/"))
                            complete_f_path.prepend("/");


                        recon_file_info_path_list << file_info_path;
                        recon_filefrom_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);

                        if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                        {
                            ///preview file path column appended with virtual extension
                            /// if we use export file path column, no need to this
                            QString suffix = recon_helper_standard_obj->mining_suffix(complete_f_path , Q_FUNC_INFO);
                            suffix = suffix.trimmed();
                            if(complete_f_path.endsWith(suffix))
                                complete_f_path.chop(suffix.size() + 1);

                            if(file_info_path.endsWith(suffix))
                                file_info_path.chop(suffix.size() + 1);

                            display_file_path_list << recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_info_path,source_count_name);

                        }
                        else
                            display_file_path_list << file_info_path;

                        all_path_list << complete_f_path;
                        source_count_name_list << source_count_name;

                    }
                }
            }
        }
    }

    if(all_path_list.size() > 0)
    {
        if(all_path_list.size() == display_file_path_list.size()
                && all_path_list.size() == recon_file_info_path_list.size()
                && all_path_list.size() == recon_filefrom_list.size()
                && all_path_list.size() == source_count_name_list.size())
        {
            st_glbl_rcn_f_info_obj.display_file_path = display_file_path_list.at(0);
            st_glbl_rcn_f_info_obj.recon_file_info_path = recon_file_info_path_list.at(0);
            st_glbl_rcn_f_info_obj.id_path = recon_file_info_path_list.at(0);
            st_glbl_rcn_f_info_obj.recon_file_from = recon_filefrom_list.at(0);
            st_glbl_rcn_f_info_obj.complete_file_path = all_path_list.at(0);
            st_glbl_rcn_f_info_obj.bool_file_from_result = bool_is_virtual_export;
            st_glbl_rcn_f_info_obj.source_count_name = source_count_name_list.at(0);
            st_glbl_rcn_f_info_obj.db_table_name = table_name;
        }
    }


    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);
    return st_glbl_rcn_f_info_obj;
}

struct_global_recon_file_info global_recon_file_info::pub_get_recon_info_for_artifacts_file_exist_any(QString plugin_name_str, QString tab_name_str, QString record_no)
{
    recon_static_functions::app_debug(" start " , Q_FUNC_INFO);

    struct_global_recon_file_info st_glbl_rcn_f_info_obj;
    clear_recon_file_st_variable(st_glbl_rcn_f_info_obj);

    QString file_path_that_exist;
    QString source_cnt_nm_1;
    QString display_file_path;

    bool bool_is_virtual_export = false;

    QString recon_file_info_path;
    QString recon_filefrom;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return st_glbl_rcn_f_info_obj;


    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");
    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString artifacts_db_file_path = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;
    if(!QFileInfo(artifacts_db_file_path).exists())
        return st_glbl_rcn_f_info_obj;


    st_glbl_rcn_f_info_obj.db_path = artifacts_db_file_path;

    QList<QStringList>md_preview_list_tablewidget;
    struct_global_csv_reader_full_csv_info obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
    md_preview_list_tablewidget = obj.metadata_preview_QList_tablewidget;
    QStringList preview_col_list = md_preview_list_tablewidget.at(tab_index);

    QList<QStringList>md_column_name_list_tablewidget;
    md_column_name_list_tablewidget = obj.metadata_column_name_QList_tablewidget;
    QStringList cols_nam_list =  md_column_name_list_tablewidget.at(tab_index);


    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
    }
    QString command = cols_nam_list.join(",");
    command = "select " + command + " from " + table_name + " where INT = " + record_no;


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(artifacts_db_file_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" DB open ------FAILED-----" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        return st_glbl_rcn_f_info_obj;
    }

    if(preview_index > -1)
    {
        QString media_file_path_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(command,preview_index,destination_db,Q_FUNC_INFO);
        QStringList m_path_list = media_file_path_str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

        if(m_path_list.size() >= 1)
        {
            QString complete_f_path;
            QString current_media_relative_path = m_path_list.at(0).toLocal8Bit();

            if((current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images)))
                    || (current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
                    || (current_media_relative_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
            {

                bool_is_virtual_export = true;

                complete_f_path =  destination_plugin_dir_path + current_media_relative_path.remove(0,1);

                file_path_that_exist = complete_f_path;
                recon_filefrom = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
                recon_file_info_path = current_media_relative_path;
                display_file_path = current_media_relative_path;
                source_cnt_nm_1 = "";

            }
            else
            {
                bool_is_virtual_export = false;

                QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
                for(int j = 0; j < export_col_name_list.size(); j++)
                {
                    QString file_path_column = export_col_name_list.at(j);
                    QString cmd = "select " + file_path_column + " , source_count_name from '" + table_name + "' where INT =?";

                    QString media_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd,QStringList(record_no),0,destination_db,Q_FUNC_INFO);
                    if(media_file_path.trimmed().isEmpty())
                        continue;

                    QString src_cnt_nm_2 = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd,QStringList(record_no),1,destination_db,Q_FUNC_INFO);

                    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_cnt_nm_2);
                    QString virtual_source_path = struct_target_info.virtual_source_path;
                    QString os_schme_intrnl = struct_target_info.os_scheme_internal;

                    if(media_file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        QStringList temp_path_list = media_file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

                        for(int i = 0; i < temp_path_list.size() ; i++)
                        {
                            QString relative_path = temp_path_list.at(i);
                            QString file_info_path = relative_path;
                            if(!file_info_path.startsWith("/"))
                                file_info_path.prepend("/");


                            complete_f_path = recon_static_functions::get_complete_file_path(virtual_source_path, relative_path, Q_FUNC_INFO);
                            if(!complete_f_path.startsWith("/"))
                                complete_f_path.prepend("/");

                            recon_file_info_path = file_info_path;
                            recon_filefrom = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);

                            if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                                    || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                            {
                                display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_info_path,src_cnt_nm_2);
                            }
                            else
                                display_file_path = file_info_path;

                            file_path_that_exist = complete_f_path;
                            source_cnt_nm_1 = src_cnt_nm_2;


                            if(QFileInfo(complete_f_path).exists())
                                break;

                        }

                    }
                    else
                    {
                        QString file_info_path = media_file_path;
                        if(!file_info_path.startsWith("/"))
                            file_info_path.prepend("/");


                        complete_f_path = recon_static_functions::get_complete_file_path(virtual_source_path, media_file_path, Q_FUNC_INFO);
                        if(!complete_f_path.startsWith("/"))
                            complete_f_path.prepend("/");


                        recon_file_info_path = file_info_path;
                        recon_filefrom = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);

                        if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                                || os_schme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                        {
                            display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(file_info_path,src_cnt_nm_2);
                        }
                        else
                            display_file_path = file_info_path;

                        file_path_that_exist = complete_f_path;
                        source_cnt_nm_1 = src_cnt_nm_2;

                    }
                }
            }
        }


        st_glbl_rcn_f_info_obj.display_file_path = display_file_path;
        st_glbl_rcn_f_info_obj.recon_file_info_path = recon_file_info_path;
        st_glbl_rcn_f_info_obj.id_path = recon_file_info_path;
        st_glbl_rcn_f_info_obj.recon_file_from = recon_filefrom;
        st_glbl_rcn_f_info_obj.complete_file_path = file_path_that_exist;
        st_glbl_rcn_f_info_obj.bool_file_from_result = bool_is_virtual_export;
        st_glbl_rcn_f_info_obj.source_count_name = source_cnt_nm_1;
        st_glbl_rcn_f_info_obj.db_table_name = table_name;

    }

    destination_db.close();

    recon_static_functions::app_debug(" end " , Q_FUNC_INFO);
    return st_glbl_rcn_f_info_obj;
}

void global_recon_file_info::clear_recon_file_st_variable(struct_global_recon_file_info &st_recn_f_info_obj)
{
    st_recn_f_info_obj.db_path.clear();
    st_recn_f_info_obj.recon_file_from.clear();
    st_recn_f_info_obj.display_file_path.clear();
    st_recn_f_info_obj.recon_file_info_path.clear();
    st_recn_f_info_obj.id_path.clear();
    st_recn_f_info_obj.complete_file_path.clear();
    st_recn_f_info_obj.db_table_name.clear();
    st_recn_f_info_obj.source_count_name.clear();
    st_recn_f_info_obj.bool_file_from_result = false;
    st_recn_f_info_obj.bool_file_is_dir = false;

}
