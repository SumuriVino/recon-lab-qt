#include "sqlite_viewer_starter.h"

sqlite_viewer_starter::sqlite_viewer_starter(QWidget *parent)
{
    bool_cancel_extraction = false;
    recon_helper_standard_obj = new recon_helper_standard(this);

}

sqlite_viewer_starter::~sqlite_viewer_starter()
{

}

void sqlite_viewer_starter::pub_set_outsqlite_path(QString path)
{
    destination_sqliteview_index_db_path = path;
}

void sqlite_viewer_starter::pub_set_recon_result_dir_path(QString path)
{
    recon_result_dir_path = path;
}

void sqlite_viewer_starter::pub_set_sqlite_path(QString path)
{
    QString dependand_db_files_path = path;
    if(dependand_db_files_path.endsWith("-wal"))
        dependand_db_files_path.chop(QString("-wal").size());
    else if(dependand_db_files_path.endsWith("-shm"))
        dependand_db_files_path.chop(QString("-shm").size());

    destination_sqlite_path = dependand_db_files_path;
}

void sqlite_viewer_starter::pub_set_sqlite_viewer_name(QString name)
{
    bool_cancel_extraction = false;

    QStringList list_value = name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(list_value.size() > 0)
        sqlite_file_name = list_value.at(0);

    if(list_value.size() > 1)
        sqlite_plugin_name = list_value.at(1);

    if(list_value.size() > 2)
        sqlite_plugin_category_name = list_value.at(2);

    if(list_value.size() > 3)
        sqlite_plugin_record_no = list_value.at(3);



}

void sqlite_viewer_starter::pub_set_essentials()
{
    QString file_name = sqlite_file_name + " [" + sqlite_plugin_name +" "+ sqlite_plugin_record_no + "]" + " [" + source_count_name + "]";

    //- Display name..
    sqlite_viewer_display_name = file_name;

    file_name = sqlite_file_name + " " + sqlite_plugin_name + " " + sqlite_plugin_record_no + " " + source_count_name;

    //db_table_name.
    file_name = file_name.replace(" ","_");
    file_name = file_name.replace("-","_");
    //    file_name = file_name.replace(".","_");
    sqlite_view_db_table_name = file_name;

}

void sqlite_viewer_starter::pub_start_sqlite_extraction(QString destination_location_path)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    set_source_file_essential();
    set_destination_location(destination_location_path);
    copy_and_recreate_source_file();
    insert_entry_in_index_table();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void sqlite_viewer_starter::pub_set_source_count_name(QString name)
{
    source_count_name = name;
}

void sqlite_viewer_starter::pub_set_recon_file_info(struct_global_recon_file_info obj)
{
    st_globl_recon_file_info_obj = obj;
}


void sqlite_viewer_starter::pub_cancel_extraction()
{
    bool_cancel_extraction = true;
}

void sqlite_viewer_starter::insert_entry_in_index_table()
{
    QString command;
    command = "INSERT INTO 'sv_index' (viewer_display_name, plugin_record_info, file_name, file_path, plugin_name, category_name,"
              "record_no, creation_timestamp, copied_file_path, recreated_file_path, destination_directory_path, source_count_name,"
              "recon_filefrom , recon_file_infopath) values (?,?,?,?,?,?,?,?, ?,?,?,?,?,?)";

    QStringList values;
    QFileInfo file_info(destination_result_directory_path);

    values << sqlite_viewer_display_name
           << sqlite_view_db_table_name
           << sqlite_file_name
           << actual_source_file
           << sqlite_plugin_name
           << sqlite_plugin_category_name
           << sqlite_plugin_record_no
           << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch())
           << copied_source_file
           << recreate_source_file
           << file_info.fileName()
           << source_count_name
           << recon_file_from
           << recon_file_info_path;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,values, destination_sqliteview_index_db_path,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("sv_index query ------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug("Db File Path--->>>  " + destination_sqliteview_index_db_path, Q_FUNC_INFO);
        return;
    }
}

void sqlite_viewer_starter::set_source_file_essential()
{
    actual_source_file   = st_globl_recon_file_info_obj.display_file_path;
    recon_file_from      = st_globl_recon_file_info_obj.recon_file_from;
    recon_file_info_path = st_globl_recon_file_info_obj.recon_file_info_path;

}

void sqlite_viewer_starter::set_destination_location(QString path)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QTime time;
    time = QTime::currentTime();
    QString time_string = time.toString("HH-mm-ss");

    QDate m_date = QDate::currentDate();
    QString date_string = m_date.toString("MM-dd-yyyy");

    QDir dir;
    QString dir_name = recon_static_functions::get_sqlite_viewer_name().replace(" ","_") + date_string+ "T" + time_string;
    QString new_name = recon_static_functions::get_available_filename(dir_name , path,Q_FUNC_INFO);
    destination_result_directory_path = new_name;
    QString dir_path = path + new_name;
    dir.mkpath(dir_path);

    recon_static_functions::app_debug("END",Q_FUNC_INFO);

}

void sqlite_viewer_starter::copy_and_recreate_source_file()
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QFileInfo info(destination_sqlite_path);
    QString file_name = info.fileName();

    QString relative_path_1 = destination_result_directory_path;
    relative_path_1.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());
    relative_path_1.append( "/" + file_name) ;

    //-1.Copied original file
    copied_source_file.clear();
    copied_source_file = destination_result_directory_path + "/" + file_name;
    if(!QFile::copy(destination_sqlite_path,relative_path_1))
    {
        recon_static_functions::app_debug("Sorce file Copy to Result Dir ------FAILED------ ",Q_FUNC_INFO);
        return;
    }

    recreate_source_file.clear();

    //-1.Recreate sqlite for SQLite Viewer work with prefix SQLite Viewer + filename.
    recreate_source_file = destination_result_directory_path + "/" + recon_static_functions::get_sqlite_viewer_name().replace(" ","_") + "_" + file_name;
    QString relative_path_2 = destination_result_directory_path;
    relative_path_2.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());
    relative_path_2.append("/" + recon_static_functions::get_sqlite_viewer_name().replace(" ","_") + "_" + file_name);
    QFile::copy(destination_sqlite_path,relative_path_2);


    //-2. -wal and -shm copied db files will disappear after open database
    // due to sqlite dependancy on -shm and -wal files.
    QString sqlite_file_path = destination_sqlite_path;
    QString shm_src_path = sqlite_file_path + "-shm";
    QString wal_src_path = sqlite_file_path + "-wal";
    if(QFileInfo::exists(shm_src_path))
    {
        QString shm_dest_file_path = destination_result_directory_path;
        shm_dest_file_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());
        shm_dest_file_path.append( "/" + recon_static_functions::get_sqlite_viewer_name().replace(" ","_") + "_" + recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(shm_src_path)) ;
        if(!QFile::copy(shm_src_path,shm_dest_file_path))
        {
            recon_static_functions::app_debug("-shm file copy  ------FAILED------ ",Q_FUNC_INFO);
        }
    }

    if(QFileInfo::exists(wal_src_path))
    {
        QString wal_dest_file_path = destination_result_directory_path;
        wal_dest_file_path.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());
        wal_dest_file_path.append( "/" + recon_static_functions::get_sqlite_viewer_name().replace(" ","_") + "_" + recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(wal_src_path)) ;
        if(!QFile::copy(wal_src_path,wal_dest_file_path))
        {
            recon_static_functions::app_debug("-wal file copy  ------FAILED------ ",Q_FUNC_INFO);
        }
    }

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

