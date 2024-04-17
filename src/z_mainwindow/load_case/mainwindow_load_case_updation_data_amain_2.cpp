#include "z_mainwindow/mainwindow.h"

void MainWindow::update_result_106_to_107()
{

    QString redefined_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "redefined_result_csv";

    QString redefined_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/redefined_result_csv";



    recon_static_functions::do_directory_blank_completely(redefined_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(redefined_csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(redefined_csv_dir_in_app_bundle, redefined_csv_dir_in_result, Q_FUNC_INFO);

    //====== Bookmark dbs
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    QString command1 = "update tags set plugin_name = 'Browser History' where plugin_name = 'Browser History Full'" ;
    QString command2 = "update tags set plugin_name = 'Messenger' where plugin_name = 'Messenger Full'" ;

    recon_helper_standard_obj->execute_db_command_by_dbpath(command1, bookmark_db_path, Q_FUNC_INFO);
    recon_helper_standard_obj->execute_db_command_by_dbpath(command2, bookmark_db_path, Q_FUNC_INFO);
    //====== Bookmark dbs




    //====== Tag dbs

    QString command3 = "select tag_db_name, tag_name from tag_search_index";
    QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command3, 0, tag_index_db, Q_FUNC_INFO);

    for(int db_count = 0; db_count < tag_db_name_list.size(); db_count++)
    {

        QString dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(db_count) + ".sqlite";
        QString command11 = "update tags set plugin_name = 'Browser History' where plugin_name = 'Browser History Full'" ;
        QString command22 = "update tags set plugin_name = 'Messenger' where plugin_name = 'Messenger Full'" ;

        recon_helper_standard_obj->execute_db_command_by_dbpath(command11, dest_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->execute_db_command_by_dbpath(command22, dest_db_path, Q_FUNC_INFO);

    }

    //====== Tag dbs


    // -Notes db
    QString notes_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QString command41 = "update notes set plugin_name = 'Browser History' where plugin_name = 'Browser History Full'" ;
    QString command42 = "update notes set plugin_name = 'Messenger' where plugin_name = 'Messenger Full'" ;

    recon_helper_standard_obj->execute_db_command_by_dbpath(command41, notes_db, Q_FUNC_INFO);
    recon_helper_standard_obj->execute_db_command_by_dbpath(command42, notes_db, Q_FUNC_INFO);
    // -Notes db

    //
    QString plist_viewer_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
    QString cmd = QString("ALTER TABLE plist_viewer_index ADD source_count_name varchar(100)");
    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, plist_viewer_db_path, Q_FUNC_INFO);


    QString select_cmd = "Select INT , source_name from 'plist_viewer_index'";
    QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd ,0,plist_viewer_db_path ,Q_FUNC_INFO);
    QStringList source_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd ,1,plist_viewer_db_path ,Q_FUNC_INFO);


    for(int tt = 0 ; tt < INT_list.size() ; tt++)
    {
        QStringList values_list;
        values_list << source_name_list.at(tt) << INT_list.at(tt) << source_name_list.at(tt);
        QString update_cmd = "UPDATE plist_viewer_index Set source_count_name = ? where INT = ? AND source_name = ?";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,values_list ,plist_viewer_db_path,Q_FUNC_INFO);

    }

}


void MainWindow::update_result_107_to_109()
{

    ///copy latest filesystem CSV
    QString filesystem_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "File_System_csv";
    QString filesystem_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/File_System_csv";

    recon_static_functions::do_directory_blank_completely(filesystem_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(filesystem_csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(filesystem_csv_dir_in_app_bundle, filesystem_csv_dir_in_result, Q_FUNC_INFO);



    ///Update databases to Latest
    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);

        /// filesystem db
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/file_system.sqlite";
        QString cmd = QString("ALTER TABLE files ADD hashset_name varchar(500)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, fs_db_path, Q_FUNC_INFO);

        cmd = QString("ALTER TABLE files ADD hash_sha1 varchar(500)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, fs_db_path, Q_FUNC_INFO);

        /// hashes db
        QString hashes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/hashes.sqlite";
        cmd = QString("ALTER TABLE files ADD sha1_hash varchar(500)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, hashes_db_path, Q_FUNC_INFO);

    }
}

void MainWindow::update_result_109_to_111()
{
    QString filesystem_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "File_System_csv";
    QString filesystem_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/File_System_csv";

    recon_static_functions::do_directory_blank_completely(filesystem_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(filesystem_csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(filesystem_csv_dir_in_app_bundle, filesystem_csv_dir_in_result, Q_FUNC_INFO);


    QStringList columns_name_list;
    QStringList columns_field_type_list;
    QString table_name = "files";

    // bool bool_update_all_columns_values = false;
    columns_name_list << "decompression_status" << "recon_filefrom" << "recon_file_infopath";
    columns_field_type_list << "varchar(300)" << "varchar(10)" << "varchar(300)";


    QString filefrom_source_value =  QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);
    QString filefrom_result_value =  QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString filefrom_examniner_value = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path);
    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);
        QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/file_system.sqlite";

        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        destination_db.setDatabaseName(destination_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
        {
            if(!destination_db.record(table_name).contains(columns_name_list.at(ii)))
            {
                // bool_update_all_columns_values = true;
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
            }
        }

        //--Update columns values
        //        if(bool_update_all_columns_values)
        //        {
        /// upadtion work done after making columns so that updation can be done in signle query ...
        /// for all columns to make updation faster.

        QString cmd;

        if(target_struct_obj.os_scheme_internal == "iOS_UID")
        {
            cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=id_path , decompression_status='0' , recon_filefrom='" + filefrom_source_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
        }
        else
        {
            cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=filepath , decompression_status='0' , recon_filefrom='" + filefrom_source_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
        }
        //    }

        destination_db.close();
    }




    QStringList table_name_list;
    columns_name_list.clear();
    columns_field_type_list.clear();
    columns_name_list  << "recon_filefrom" << "recon_file_infopath";
    columns_field_type_list << "varchar(10)" << "varchar(300)";

    //===========Update hex viewer start ==============///
    QString hex_features_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    if(QFileInfo(hex_features_db_path).exists())
    {
        table_name_list << "hex_viewer_index" << "hex_viewer_blocks";

        ///add new columns in tables
        for(int count = 0; count < table_name_list.size(); count++)
        {
            QString table_name = table_name_list.at(count);

            for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, hex_features_db_path, Q_FUNC_INFO);

            }

            QString records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) From ") + table_name, 0, hex_features_db_path, Q_FUNC_INFO);
            if(!records_str.trimmed().isEmpty() && records_str != QString::number(0))
            {
                QString cmd = "UPDATE '" + table_name + "' SET recon_file_infopath=file_path , recon_filefrom='" + filefrom_source_value + "'";
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, hex_features_db_path, Q_FUNC_INFO);
            }
        }
    }
    ///Update hex viewer tmp database
    QString hex_tmp_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "tmp/hex_viewer/hex_viewer.sqlite";
    if(QFileInfo(hex_tmp_db_path).exists())
    {

        ///add new columns in tables
        for(int count = 0; count < table_name_list.size(); count++)
        {
            QString table_name = table_name_list.at(count);

            for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, hex_tmp_db_path, Q_FUNC_INFO);
            }
        }
    }

    ///Update hex viewer exported content database
    QString hex_exported_content_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "Exported_Content/Hex_Viewer/hex_viewer_content_info.sqlite";
    QFileInfo hex_v_file_info(hex_exported_content_db_path);
    if(hex_v_file_info.exists())
    {
        table_name_list.clear();
        table_name_list << "hex_content_info";

        ///add new columns in tables
        for(int count = 0; count < table_name_list.size(); count++)
        {
            QString table_name = table_name_list.at(count);

            for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, hex_exported_content_db_path, Q_FUNC_INFO);

            }

            QString records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) From ") + table_name, 0, hex_exported_content_db_path, Q_FUNC_INFO);
            if(!records_str.trimmed().isEmpty() && records_str != QString::number(0))
            {
                QString cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='" + filefrom_source_value + "'";
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, hex_exported_content_db_path, Q_FUNC_INFO);
            }
        }
    }
    //===========Update hex viewer end ==============///


    //===========Update plist viewer start ==============///
    QString plist_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
    if(QFileInfo(plist_index_db_path).exists())
    {
        QString plist_index_adhoc_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

        QStringList plist_databases_list;
        plist_databases_list << plist_index_db_path << plist_index_adhoc_db_path;

        for(int pp = 0 ; pp < plist_databases_list.size() ; pp++)
        {
            QSqlDatabase::removeDatabase(Q_FUNC_INFO);
            QSqlDatabase plist_dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
            plist_dest_db.setDatabaseName(plist_databases_list.at(pp));
            if(plist_dest_db.open())
            {
                QStringList colms_name_list , colms_field_type_list;
                colms_name_list << "recon_filefrom" << "recon_file_infopath";
                colms_field_type_list << "varchar(300)" << "varchar(300)";
                QString table_name = "plist_viewer_index";

                ///add new column
                for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
                {
                    if(!plist_dest_db.record(table_name).contains(colms_name_list.at(ii)))
                    {
                        QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                        recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, plist_dest_db, Q_FUNC_INFO);
                    }
                }

                ///
                QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='"+ filefrom_source_value + "'";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, plist_dest_db, Q_FUNC_INFO);

                plist_dest_db.close();
            }
            else
            {
                recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + plist_dest_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + plist_dest_db.lastError().text(),Q_FUNC_INFO);
            }

        }
    }

    QString plist_exported_content_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";
    QFileInfo plist_file_info(plist_exported_content_db_path);
    if(plist_file_info.exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase plist_export_content_dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        plist_export_content_dest_db.setDatabaseName(plist_exported_content_db_path);
        if(plist_export_content_dest_db.open())
        {
            QStringList colms_name_list , colms_field_type_list;
            colms_name_list << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list << "varchar(300)" << "varchar(300)";
            QString table_name = "plist_content_info";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!plist_export_content_dest_db.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, plist_export_content_dest_db, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='"+ filefrom_source_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, plist_export_content_dest_db, Q_FUNC_INFO);

            plist_export_content_dest_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + plist_export_content_dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + plist_export_content_dest_db.lastError().text(),Q_FUNC_INFO);
        }
    }
    //===========Update plist viewer end  ==============///


    //===========Update strings viewer start  ==============///
    QString exported_content_strings_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";

    QFileInfo string_v_file_ifno(exported_content_strings_info_db);
    if(string_v_file_ifno.exists())
    {
        table_name_list.clear();
        table_name_list << "string_content_info";

        ///add new columns in tables
        for(int count = 0; count < table_name_list.size(); count++)
        {
            QString table_name = table_name_list.at(count);

            for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, exported_content_strings_info_db, Q_FUNC_INFO);
            }

            QString records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) From ") + table_name, 0, exported_content_strings_info_db, Q_FUNC_INFO);
            if(!records_str.trimmed().isEmpty() && records_str != QString::number(0))
            {
                QString cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='" + filefrom_source_value + "'";
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, exported_content_strings_info_db, Q_FUNC_INFO);
            }

        }
    }
    //===========Update strings viewer end  ==============///


    //===========Update text viewer start  ==============///
    QString exported_content_text_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
    QFileInfo text_v_file_ifno(exported_content_text_info_db);
    if(text_v_file_ifno.exists())
    {

        table_name_list.clear();
        table_name_list << "text_content_info";

        ///add new columns in tables
        for(int count = 0; count < table_name_list.size(); count++)
        {
            QString table_name = table_name_list.at(count);

            for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, exported_content_text_info_db, Q_FUNC_INFO);

            }

            QString records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) From ") + table_name, 0, exported_content_text_info_db, Q_FUNC_INFO);
            if(!records_str.trimmed().isEmpty() && records_str != QString::number(0))
            {
                QString cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='" + filefrom_source_value + "'";
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, exported_content_text_info_db, Q_FUNC_INFO);
            }
        }
    }
    //===========Update text viewer end  ==============///


    //===========Update registry viewer start  ==============///
    QString registry_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    if(QFileInfo(registry_viewer_index_db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase dest_db_1 = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        dest_db_1.setDatabaseName(registry_viewer_index_db_path);
        if(dest_db_1.open())
        {
            QStringList colms_name_list , colms_field_type_list;
            colms_name_list << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list << "varchar(300)" << "varchar(300)";
            QString table_name = "tbl_registry_viewer_index";
            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db_1.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_1, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=source_file_path , recon_filefrom='"+ filefrom_source_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_1, Q_FUNC_INFO);

            dest_db_1.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db_1.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db_1.lastError().text(),Q_FUNC_INFO);
        }
    }

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString command = "Select relative_db_path from tbl_registry_viewer_index";
    QStringList all_relative_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command ,0,registry_viewer_index_db_path,Q_FUNC_INFO);
    for(int ll = 0 ; ll < all_relative_db_path_list.size() ; ll++)
    {
        QString dest_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,all_relative_db_path_list.at(ll),Q_FUNC_INFO);
        if(QFileInfo(dest_db_path).exists())
        {

            QSqlDatabase::removeDatabase(Q_FUNC_INFO);
            QSqlDatabase dest_db_1 = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
            dest_db_1.setDatabaseName(dest_db_path);
            if(dest_db_1.open())
            {
                QStringList colms_name_list , colms_field_type_list;
                colms_name_list << "recon_filefrom" << "recon_file_infopath";
                colms_field_type_list << "varchar(300)" << "varchar(300)";
                QString table_name = "tbl_registry_tree";

                ///add new column
                for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
                {
                    if(!dest_db_1.record(table_name).contains(colms_name_list.at(ii)))
                    {
                        QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                        recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_1, Q_FUNC_INFO);
                    }
                }

                /// update columns values
                QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='"+ filefrom_source_value + "'";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_1, Q_FUNC_INFO);

                dest_db_1.close();
            }
            else
            {
                recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db_1.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + dest_db_1.lastError().text(),Q_FUNC_INFO);
            }
        }

    }
    //===========Update registry viewer end  ==============///


    //===========Update saved map start  ==============///
    QString saved_maps_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
    if(QFileInfo(saved_maps_db_path).exists())
    {

        table_name_list.clear();
        table_name_list << "tags";

        ///add new columns in tables
        for(int count = 0; count < table_name_list.size(); count++)
        {
            QString table_name = table_name_list.at(count);

            for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, saved_maps_db_path, Q_FUNC_INFO);

                QString records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) From ") + table_name, 0, saved_maps_db_path, Q_FUNC_INFO);
                if(!records_str.trimmed().isEmpty() && records_str != QString::number(0))
                {
                    QString command = "select item4 ,INT from tags";
                    QStringList item1_coumn_values_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,saved_maps_db_path,Q_FUNC_INFO);
                    QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,saved_maps_db_path,Q_FUNC_INFO);
                    for(int at = 0 ; at < INT_list.size() ; at++)
                    {
                        QString record_no = INT_list.at(at);
                        QString value =  "/" + QString("Lab_Features/") + "Saved_Maps/" + item1_coumn_values_list.at(at);
                        QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath= '" + value + "' , recon_filefrom='" + filefrom_result_value + "' where INT =" + record_no;
                        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, saved_maps_db_path, Q_FUNC_INFO);
                    }
                    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, saved_maps_db_path, Q_FUNC_INFO);
                }
            }
        }
    }
    //===========Update saved map end  ==============///


    //===========Update sqlite viewer start  ==============///
    QString sqlite_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";
    if(QFileInfo(sqlite_viewer_index_db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase dest_db_2 = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        dest_db_2.setDatabaseName(sqlite_viewer_index_db_path);
        if(dest_db_2.open())
        {
            QStringList colms_name_list , colms_field_type_list;
            colms_name_list << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list << "varchar(300)" << "varchar(300)";
            QString table_name = "sv_index";
            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db_2.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_2, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='"+ filefrom_source_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_2, Q_FUNC_INFO);

            dest_db_2.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db_2.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db_2.lastError().text(),Q_FUNC_INFO);
        }
    }

    QString sqlite_viewer_exported_content_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";
    QFileInfo file_info(sqlite_viewer_exported_content_db_file) ;
    if(file_info.exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase dest_db_3 = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        dest_db_3.setDatabaseName(sqlite_viewer_exported_content_db_file);
        if(dest_db_3.open())
        {
            QStringList colms_name_list , colms_field_type_list;
            colms_name_list << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list << "varchar(300)" << "varchar(300)";
            QString table_name = "sv_content_info";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db_3.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_3, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=file_path , recon_filefrom='"+ filefrom_source_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_3, Q_FUNC_INFO);

            dest_db_3.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db_3.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db_3.lastError().text(),Q_FUNC_INFO);
        }

    }
    //===========Update sqlite viewer end  ==============///

    //===========Update Saved Graphs start  ==============///
    QString browser_graphs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
    if(QFileInfo(browser_graphs_db_path).exists())
    {
        QString messenger_graphs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
        QStringList graphs_db_path_list;
        graphs_db_path_list << browser_graphs_db_path << messenger_graphs_db_path;
        for(int gg = 0 ; gg < graphs_db_path_list.size() ; gg++)
        {

            QString dest_db_path = graphs_db_path_list.at(gg);

            QSqlDatabase::removeDatabase(Q_FUNC_INFO);
            QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
            dest_db.setDatabaseName(dest_db_path);
            if(dest_db.open())
            {
                QStringList colms_name_list , colms_field_type_list;
                colms_name_list << "recon_filefrom" << "recon_file_infopath";
                colms_field_type_list << "varchar(300)" << "varchar(300)";
                QString table_name = "saved_graphs";

                for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
                {
                    if(!dest_db.record(table_name).contains(colms_name_list.at(ii)))
                    {
                        QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                        recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
                    }
                }

                QString command = "select graph_path ,INT from '"+ table_name +"'";
                QStringList item1_coumn_values_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,0,dest_db,Q_FUNC_INFO);
                QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,1,dest_db,Q_FUNC_INFO);
                for(int at = 0 ; at < INT_list.size() ; at++)
                {
                    QString record_no = INT_list.at(at);
                    QString value = "/" + item1_coumn_values_list.at(at);
                    QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath='"+ value +"' , recon_filefrom='"+ filefrom_result_value + "' where INT = " + record_no;
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
                }

                dest_db.close();
            }
            else
            {
                recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
            }

        }
    }


    QString timeline_saved_graphs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
    if(QFileInfo(timeline_saved_graphs_db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        dest_db.setDatabaseName(timeline_saved_graphs_db_path);
        if(dest_db.open())
        {
            QStringList colms_name_list , colms_field_type_list;
            colms_name_list << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list << "varchar(300)" << "varchar(300)";
            QString table_name = "tags";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString command = "select item1 ,INT from tags";
            QStringList item1_coumn_values_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,0,dest_db,Q_FUNC_INFO);
            QStringList INT_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command,1,dest_db,Q_FUNC_INFO);
            for(int aa = 0 ; aa < INT_list.size() ; aa++)
            {
                QString record_no = INT_list.at(aa);
                QString value =  "/" + QString("Lab_Features/") + "Artifacts_Timeline/Artifacts_Timeline_Saved/Artifacts_Timeline_Saved_Graphs/" + item1_coumn_values_list.at(aa);
                QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath= '" + value + "' , recon_filefrom='" + filefrom_result_value + "' where INT=" + record_no;

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
            }

            QString update_cmd = "Update tags set tab_name = '" + QString("Artifacts Timeline Graph") +"'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(update_cmd, dest_db, Q_FUNC_INFO);

            dest_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        }
    }
    //===========Update Saved Graphs end  ==============///


    //===========Update Screen Shots start  ==============///
    QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
    if(QFileInfo(screenshot_db_path).exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase dest_db_s = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        dest_db_s.setDatabaseName(screenshot_db_path);
        if(dest_db_s.open())
        {
            QStringList colms_name_list , colms_field_type_list;
            colms_name_list << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list << "varchar(300)" << "varchar(300)";
            QString table_name = "tags";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db_s.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_s, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=item1 , recon_filefrom='"+ filefrom_examniner_value + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db_s, Q_FUNC_INFO);

            dest_db_s.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db_s.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db_s.lastError().text(),Q_FUNC_INFO);
        }
    }
    //===========Update Screen Shots end  ==============///

    //===========Update Carved Files Start  ==============///
    QString carver_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    if(QFileInfo(carver_index_db_path).exists())
    {
        QStringList carver_files_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select carver_files_db_path From table_carver_index"), 0, carver_index_db_path, Q_FUNC_INFO);
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

        table_name_list.clear();
        table_name_list << "files";

        int num = 0;
        for(int count = 0; count < carver_files_db_path_list.size(); count++)
        {
            num++;
            if(num % 10 == 0)
            {
                QCoreApplication::processEvents();
                num = 0;
            }

            QString carver_files_db_path_str = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path , carver_files_db_path_list.at(count), Q_FUNC_INFO);
            if(QFileInfo(carver_files_db_path_str).exists())
            {
                ///add new columns in tables
                for(int count = 0; count < table_name_list.size(); count++)
                {
                    QString table_name = table_name_list.at(count);

                    for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
                    {
                        QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, carver_files_db_path_str, Q_FUNC_INFO);

                    }

                    QString records_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select count(*) From ") + table_name, 0, carver_files_db_path_str, Q_FUNC_INFO);
                    if(!records_str.trimmed().isEmpty() && records_str != QString::number(0))
                    {
                        QString cmd = "UPDATE '" + table_name +"' SET recon_file_infopath=item1 , recon_filefrom='" + filefrom_result_value + "'";
                        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, carver_files_db_path_str, Q_FUNC_INFO);
                    }

                }
            }
        }
    }

    //===========Update Carved Files End  ==============///


    //===========Update Notes Search start  ==============///
    QString notes_search_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    if(QFileInfo(notes_search_db_path).exists())
    {
        QString update_cmd = "Update notes set tab_name = '"+ QString("Artifacts Timeline Graph") + "' where plugin_name = '" +  QString("Artifacts Timeline Graph") + "'";
        recon_helper_standard_obj->execute_db_command_by_dbpath(update_cmd, notes_search_db_path, Q_FUNC_INFO);
    }
    //===========Update Notes Search end ==============///

}

void MainWindow::update_result_111_to_112()
{
    recon_static_functions::debug_intensive(" start ", Q_FUNC_INFO);

    QString mb_bkp_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "Mobile_Backup_csv";
    QString  mb_bkp_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/Mobile_Backup_csv";

    recon_static_functions::do_directory_blank_completely(mb_bkp_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(mb_bkp_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(mb_bkp_csv_dir_in_app_bundle, mb_bkp_csv_dir_in_result, Q_FUNC_INFO);

    QString carved_data_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "carved_data_csv";
    QString carved_data_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/carved_data_csv";

    recon_static_functions::do_directory_blank_completely(carved_data_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(carved_data_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(carved_data_csv_dir_in_app_bundle, carved_data_csv_dir_in_result, Q_FUNC_INFO);

    QString ram_img_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "ram_images_csv";
    QString ram_img_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/ram_images_csv";

    recon_static_functions::do_directory_blank_completely(ram_img_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(ram_img_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(ram_img_csv_dir_in_app_bundle, ram_img_csv_dir_in_result, Q_FUNC_INFO);


    QString disk_img_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "Disk_Images_csv";
    QString  disk_img_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/Disk_Images_csv";

    recon_static_functions::do_directory_blank_completely(disk_img_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(disk_img_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(disk_img_csv_dir_in_app_bundle, disk_img_csv_dir_in_result, Q_FUNC_INFO);


    QString filefrom_source_value =  QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);
    QString filefrom_result_value =  QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);
    QString filefrom_examniner_value = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path);
    //  QList<struct_GLOBAL_derived_source_info> target_info_list = global_target_m_sources_info_obj->pub_get_general_target_info_structure_Qlist();



    //===========Update Mobile Backup start  ==============///
    QString mobile_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString() + "mobile_backup.sqlite";
    QFileInfo file_info_mbl(mobile_db_path);
    if(file_info_mbl.exists())
    {
        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        dest_db.setDatabaseName(mobile_db_path);
        if(dest_db.open())
        {
            QString table_name = "tbl_backup_list";

            QStringList colms_name_list;
            QStringList colms_field_type_list;

            colms_name_list  << "recon_filefrom" << "recon_file_infopath" << "is_nested" << "is_decompressed" << "root_count_name" << "source_size" << "source_path" << "source_name";
            colms_field_type_list << "varchar(10)" << "varchar(300)" << "varchar(10)" << "varchar(300)" << "varchar(300)" << "varchar(300)" << "varchar(300)" << "varchar(300)";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET recon_file_infopath = backup_filepath , recon_filefrom='"+ filefrom_source_value + "', is_nested = 1, is_decompressed = 0, root_count_name=item1, source_size=backup_filesize, source_path=backup_filepath";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);

            dest_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    //===========Update Mobile Backup end  ==============///


    //===========Update Disk Images start  ==============///
    QString disk_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    QFileInfo file_info_disk(disk_db_path);
    if(file_info_disk.exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO) + "1";
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(disk_db_path);
        if(dest_db.open())
        {
            QString table_name = "tbl_sources_list";

            QStringList colms_name_list;
            QStringList colms_field_type_list;
            colms_name_list   << "is_nested" << "is_decompressed";
            colms_field_type_list  << "varchar(10)" << "varchar(300)";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QString  cmd = "UPDATE '" + table_name +"' SET is_nested = 1, is_decompressed = 0";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);

            dest_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    //===========Update Disk images end  ==============///


    //=========== Case Configuration db start=========///
    QString case_confg_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    if(QFileInfo(case_confg_db_path).exists())
    {
        QString cmd_saved_tm_zone = "CREATE TABLE tbl_saved_timezones (INT INTEGER PRIMARY KEY,timezone varchar(100),timezone_offset INTEGER , start_date_epoch INTEGER,end_date_epoch INTEGER ,start_date_str varchar(100),end_date_str varchar(100),display_time varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_saved_tm_zone , case_confg_db_path,Q_FUNC_INFO);
    }
    //=========== Case Configuration db end=========///


    ///=========Update Storyboard - Start==========///
    QString storyboard_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + "story_board.sqlite";
    QFileInfo file_info_storyboard(storyboard_db_path);
    if(file_info_storyboard.exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO) + "2";
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(storyboard_db_path);
        if(dest_db.open())
        {
            QString table_name = "story_board";

            QStringList colms_name_list;
            QStringList colms_field_type_list;
            colms_name_list   << "recon_filefrom" << "recon_file_infopath";
            colms_field_type_list  << "varchar(10)" << "varchar(500)";

            ///add new column
            for(int ii  = 0 ; ii < colms_name_list.size() ; ii++)
            {
                if(!dest_db.record(table_name).contains(colms_name_list.at(ii)))
                {
                    QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ colms_name_list.at(ii) + "' '" + colms_field_type_list.at(ii) + "'");
                    recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, dest_db, Q_FUNC_INFO);
                }
            }

            /// update columns values
            QStringList story_filepath_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(QString("Select filepath from ") + table_name, 0, dest_db, Q_FUNC_INFO);

            QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

            QStringList val_list;
            QString update_command = "Update " + table_name + " Set recon_filefrom = ?, recon_file_infopath = ? Where filepath = ?";
            for(int count = 0; count < story_filepath_list.size(); count++)
            {
                QString filepath = story_filepath_list.at(count);

                QString recon_file_infopath = filepath;
                if(!recon_file_infopath.trimmed().isEmpty() && !recon_file_infopath.startsWith("/"))
                    recon_file_infopath.prepend("/");

                val_list.clear();
                val_list << recon_file_from_result << recon_file_infopath << filepath;

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_command, val_list, dest_db, Q_FUNC_INFO);
            }

            dest_db.close();
        }
        else
        {
            recon_static_functions::app_debug( " Destination Db opening --------FAILED------ " + dest_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    ///=========Update Storyboard - End==========///

    recon_static_functions::debug_intensive(" end ", Q_FUNC_INFO);

}

void MainWindow::update_result_112_to_114()
{
    QStringList columns_name_list;
    QStringList columns_field_type_list;
    QString table_name;

    ///======Update Filesystem Database===========///
    table_name = "files";
    columns_name_list << "fs_module_apple_metadata_run_status" << "fs_module_exif_metadata_run_status" << "fs_module_signature_analysis_run_status" << "fs_module_mime_type_analysis_run_status" << "fs_module_hashset_run_status";
    columns_field_type_list << "varchar(10)" << "varchar(10)" << "varchar(10)" << "varchar(10)" << "varchar(10)";

    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);
        QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/file_system.sqlite";

        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        destination_db.setDatabaseName(destination_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
        {
            if(!destination_db.record(table_name).contains(columns_name_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
            }
        }

        QString cmd = "UPDATE '" + table_name +"' SET fs_module_apple_metadata_run_status='0' , fs_module_exif_metadata_run_status='0', fs_module_signature_analysis_run_status='0',fs_module_mime_type_analysis_run_status='0',fs_module_hashset_run_status='0'";
        recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);

        destination_db.close();
    }

}

void MainWindow::update_result_114_to_116()
{
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "snapshots_csv";

    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/snapshots_csv";

    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);

    ///======Update Filesystem Database===========///
    QStringList columns_name_list;
    QStringList columns_field_type_list;
    QString table_name;

    table_name = "files";
    columns_name_list << "is_dir" ;
    columns_field_type_list << "varchar(10)";

    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);
        QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/file_system.sqlite";

        QSqlDatabase::removeDatabase(Q_FUNC_INFO);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
        destination_db.setDatabaseName(destination_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            continue;
        }

        for(int ii  = 0 ; ii < columns_name_list.size() ; ii++)
        {
            if(!destination_db.record(table_name).contains(columns_name_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(ii) + "' '" + columns_field_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
            }
        }
        destination_db.close();
    }

}

void MainWindow::update_result_116_to_117()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString registry_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    if(QFileInfo(registry_viewer_index_db_path).exists())
    {
        //main registry db update start

        QString connection_naam_1 = QString(Q_FUNC_INFO) + "1";
        QSqlDatabase::removeDatabase(connection_naam_1);
        QSqlDatabase destination_reg_index_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam_1);
        destination_reg_index_db.setDatabaseName(registry_viewer_index_db_path);
        if(!destination_reg_index_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_reg_index_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_reg_index_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QString cmd1 = "Select distinct source_count_name from tbl_registry_viewer_index where source_file_path is NULL";
        QStringList source_cont_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(cmd1 ,0 ,destination_reg_index_db,Q_FUNC_INFO);

        QString cmd2 = "Select display_tab_name from tbl_registry_viewer_index where source_file_path is NULL";
        QStringList display_tab_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(cmd2 ,0 ,destination_reg_index_db,Q_FUNC_INFO);

        if(display_tab_name_list.size() == 0)
        {
            destination_reg_index_db.close();
            return;
            recon_static_functions::app_debug(" end",Q_FUNC_INFO);
        }


        QStringList tb_name_list = display_tab_name_list;
        QStringList record_num_list;
        QStringList fs_filepath_list;
        QStringList fs_src_cnt_nm_list;
        for(int ii = 0 ; ii < display_tab_name_list.size() ; ii++)
        {
            if(ii % 50 == 0)
                QCoreApplication::processEvents();

            QString tm_rcord_no = display_tab_name_list.at(ii);
            QStringList tem_list =  tm_rcord_no.split("File System" , Qt::SkipEmptyParts);
            if(tem_list.size() > 1)
            {
                QString tem_str = tem_list.at(1);
                record_num_list << tem_str.remove("]").trimmed();
            }
        }
        if(record_num_list.size() != tb_name_list.size())
            return;

        for(int jj = 0 ; jj < source_cont_name_list.size() ; jj++)
        {

            if(jj % 50 == 0)
                QCoreApplication::processEvents();

            QString sourc_cnt_namestr = source_cont_name_list.at(jj);
            QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + sourc_cnt_namestr + "/file_system.sqlite";

            QString connection_naam_2 = QString(Q_FUNC_INFO) + "2";
            QSqlDatabase::removeDatabase(connection_naam_2);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_2);
            destination_db.setDatabaseName(destination_db_path);
            if(!destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }

            for(int kk  = 0 ; kk < record_num_list.size() ; kk++)
            {
                if(kk % 50 == 0)
                    QCoreApplication::processEvents();


                QString record_no = record_num_list.at(kk);
                QString display_tab_name = tb_name_list.at(kk);
                QString select_cmd_2 = "Select filepath , recon_filefrom , recon_file_infopath  , source_count_name from files where INT = ?";

                QSqlQuery select_qry(destination_db);
                select_qry.prepare(select_cmd_2);
                select_qry.addBindValue(record_no);
                if(!select_qry.exec())
                {
                    recon_static_functions::app_debug(" query select -------FAILED------- " + select_qry.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" query Error " + select_qry.lastError().text(),Q_FUNC_INFO);
                    continue;
                }

                if(select_qry.next())
                {
                    QString updte_cmd = "Update tbl_registry_viewer_index set source_file_path = ?  , recon_filefrom = ?, recon_file_infopath = ? where display_tab_name = ?";
                    QStringList arg_list_1;

                    QString filepath = select_qry.value("filepath").toString();
                    fs_filepath_list << filepath;
                    fs_src_cnt_nm_list << select_qry.value("source_count_name").toString();


                    arg_list_1 << filepath;
                    arg_list_1 << select_qry.value("recon_filefrom").toString();
                    arg_list_1 << select_qry.value("recon_file_infopath").toString();
                    arg_list_1 << display_tab_name;


                    destination_reg_index_db.transaction();
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(updte_cmd,arg_list_1 ,destination_reg_index_db ,Q_FUNC_INFO);


                    QString final_reg_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(),recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select relative_db_path from tbl_registry_viewer_index where display_tab_name = ?" ,QStringList(display_tab_name),0,registry_viewer_index_db_path,Q_FUNC_INFO),Q_FUNC_INFO);
                    QString connection_naam_21 = QString(Q_FUNC_INFO) + "21";
                    QSqlDatabase::removeDatabase(connection_naam_21);
                    QSqlDatabase reg_destdb = QSqlDatabase::addDatabase("QSQLITE", connection_naam_21);
                    reg_destdb.setDatabaseName(final_reg_db_path);
                    if(!reg_destdb.open())
                    {
                        recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + reg_destdb.databaseName(),Q_FUNC_INFO);
                        recon_static_functions::app_debug(" error -  " + reg_destdb.lastError().text(),Q_FUNC_INFO);
                        continue;
                    }

                    reg_destdb.transaction();
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference("update tbl_registry_tree set file_path = ?",QStringList(filepath) ,reg_destdb ,Q_FUNC_INFO);
                    reg_destdb.commit();
                    reg_destdb.close();
                }

            }
            destination_db.close();
        }
        destination_reg_index_db.commit();
        destination_reg_index_db.close();
        //main registry db update end



        //====== update for tagged items
        if(fs_filepath_list.size() != fs_src_cnt_nm_list.size())
            return;

        //====== upadte Bookmark db start
        QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
        QString connection_naam_3 = QString(Q_FUNC_INFO) + "3";
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase bookmark_destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_3);
        bookmark_destination_db.setDatabaseName(bookmark_db_path);
        if(!bookmark_destination_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + bookmark_destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + bookmark_destination_db.lastError().text(),Q_FUNC_INFO);
            return;
        }


        bookmark_destination_db.transaction();
        for(int ll = 0 ; ll < display_tab_name_list.size() ; ll++)
        {
            if(ll % 50 == 0)
                QCoreApplication::processEvents();


            QString display_tab_name = display_tab_name_list.at(ll);

            for(int mm = 0 ; mm < fs_filepath_list.size() ; mm++)
            {
                if(mm % 50 == 0)
                    QCoreApplication::processEvents();

                QString update_bk_cmd = "update tags set item0 = ? where plugin_name = ? AND source_count_name = ? AND item11 = ?" ;
                QSqlQuery bk_qury(bookmark_destination_db);
                bk_qury.prepare(update_bk_cmd);
                bk_qury.addBindValue(fs_filepath_list.at(mm));
                bk_qury.addBindValue("Registry Viewer");
                bk_qury.addBindValue(fs_src_cnt_nm_list.at(mm));
                bk_qury.addBindValue(display_tab_name);
                if(!bk_qury.exec())
                {
                    recon_static_functions::app_debug(" query select -------FAILED------- " + bk_qury.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" query Error " + bk_qury.lastError().text(),Q_FUNC_INFO);
                    continue;
                }
            }
        }
        bookmark_destination_db.commit();
        bookmark_destination_db.close();
        //====== upadte Bookmark db end



        //====== upadte tags dbs start
        QString command3 = "select tag_db_name, tag_name from tag_search_index";
        QString tag_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + "index_tag_search.sqlite";
        QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command3, 0, tag_index_db, Q_FUNC_INFO);
        for(int nn = 0; nn < tag_db_name_list.size(); nn++)
        {

            if(nn % 50 == 0)
                QCoreApplication::processEvents();

            QString dest_tag_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tag_db_name_list.at(nn) + ".sqlite";
            QString connection_naam_4 = QString(Q_FUNC_INFO) + "4";
            QSqlDatabase::removeDatabase(connection_naam_4);
            QSqlDatabase tag_destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_4);
            tag_destination_db.setDatabaseName(dest_tag_db_path);
            if(!tag_destination_db.open())
            {
                recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + tag_destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + tag_destination_db.lastError().text(),Q_FUNC_INFO);
                return;
            }


            tag_destination_db.transaction();
            for(int oo = 0 ; oo < display_tab_name_list.size() ; oo++)
            {

                if(oo % 50 == 0)
                    QCoreApplication::processEvents();

                QString display_tab_name = display_tab_name_list.at(oo);

                for(int pp = 0 ; pp < fs_filepath_list.size() ; pp++)
                {

                    if(pp % 50 == 0)
                        QCoreApplication::processEvents();

                    QString update_tg_cmd = "update tags set item0 = ? where plugin_name = ? AND source_count_name = ? AND item11 = ?" ;
                    QSqlQuery tag_qury(tag_destination_db);
                    tag_qury.prepare(update_tg_cmd);
                    tag_qury.addBindValue(fs_filepath_list.at(pp));
                    tag_qury.addBindValue("Registry Viewer");
                    tag_qury.addBindValue(fs_src_cnt_nm_list.at(pp));
                    tag_qury.addBindValue(display_tab_name);
                    if(!tag_qury.exec())
                    {
                        recon_static_functions::app_debug(" query select -------FAILED------- " + tag_qury.executedQuery(),Q_FUNC_INFO);
                        recon_static_functions::app_debug(" query Error " + tag_qury.lastError().text(),Q_FUNC_INFO);
                        continue;
                    }
                }
            }
            tag_destination_db.commit();
            tag_destination_db.close();

        }
        //====== upadte tags dbs end


        //====== upadte notes dbs start
        QString notes_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
        QString connection_naam_5 = QString(Q_FUNC_INFO) + "5";
        QSqlDatabase::removeDatabase(connection_naam_5);
        QSqlDatabase notes_db_path = QSqlDatabase::addDatabase("QSQLITE", connection_naam_3);
        notes_db_path.setDatabaseName(notes_db);
        if(!notes_db_path.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + notes_db_path.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + notes_db_path.lastError().text(),Q_FUNC_INFO);
            return;
        }

        notes_db_path.transaction();
        for(int qq = 0 ; qq < display_tab_name_list.size() ; qq++)
        {

            if(qq % 50 == 0)
                QCoreApplication::processEvents();

            QString display_tab_name = display_tab_name_list.at(qq);

            for(int rr = 0 ; rr < fs_filepath_list.size() ; rr++)
            {
                if(rr % 50 == 0)
                    QCoreApplication::processEvents();

                QString update_nots_cmd = "update notes set item0 = ? where plugin_name = ? AND source_count_name = ? AND item11 = ?" ;
                QSqlQuery nts_qury(notes_db_path);
                nts_qury.prepare(update_nots_cmd);
                nts_qury.addBindValue(fs_filepath_list.at(rr));
                nts_qury.addBindValue("Registry Viewer");
                nts_qury.addBindValue(fs_src_cnt_nm_list.at(rr));
                nts_qury.addBindValue(display_tab_name);
                if(!nts_qury.exec())
                {
                    recon_static_functions::app_debug(" query select -------FAILED------- " + nts_qury.executedQuery(),Q_FUNC_INFO);
                    recon_static_functions::app_debug(" query Error " + nts_qury.lastError().text(),Q_FUNC_INFO);
                    continue;
                }
            }
        }
        notes_db_path.commit();
        notes_db_path.close();
        //====== upadte notes dbs end
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void MainWindow::update_result_117_to_120()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString result_dir_path        = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // ====== disk image start======
    QString disk_images_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString() + "disk_images.sqlite";
    if(QFileInfo(disk_images_db_path).exists())
    {
        QString connection_naam_1 = QString(Q_FUNC_INFO) + "1";
        QSqlDatabase::removeDatabase(connection_naam_1);
        QSqlDatabase disk_images_dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_1);
        disk_images_dest_db.setDatabaseName(disk_images_db_path);
        if(disk_images_dest_db.open())
        {

            disk_images_dest_db.transaction();

            QString table_name     = "tbl_sources_list";
            QString column_str     = "source_extension";

            if(!disk_images_dest_db.record(table_name).contains(column_str))
            {
                QString disk_images_csv_dir_in_result      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "disk_images_csv";
                QString disk_iamges_csv_dir_in_app_bundle  = "../Resources/CSVS/Features_CSV/disk_images_csv";

                recon_static_functions::do_directory_blank_completely(disk_images_csv_dir_in_result, Q_FUNC_INFO);
                recon_static_functions::remove_safely_blank_dir_OR_file(disk_images_csv_dir_in_result, Q_FUNC_INFO);


                recon_static_functions::copy_directory(disk_iamges_csv_dir_in_app_bundle, disk_images_csv_dir_in_result, Q_FUNC_INFO);


                QStringList columns_name_list;
                columns_name_list <<  column_str;

                QStringList columns_field_type_list;
                columns_field_type_list << "varchar(500)" ;


                recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),columns_name_list, columns_field_type_list, disk_images_dest_db);

                QString select_cmd = "Select INT , source_path from '" + table_name + "'";
                QSqlQuery select_qry(disk_images_dest_db);
                select_qry.prepare(select_cmd);
                if(select_qry.exec())
                {

                    QString update_cmd = "UPDATE '" + table_name + "'" + " set source_extension = ? where INT = ?";

                    qint64 proc_counter = 0;
                    while(select_qry.next())
                    {

                        proc_counter++;
                        if(proc_counter % 50 == 0)
                            QApplication::processEvents();


                        QString record_no = select_qry.value("INT").toString();
                        QString dmg_file_path = select_qry.value("source_path").toString();
                        QString extension_str =  QFileInfo(dmg_file_path).suffix();

                        QStringList arg_list;
                        arg_list << extension_str << record_no;
                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_cmd ,arg_list,disk_images_dest_db ,Q_FUNC_INFO);
                    }
                }
            }

            disk_images_dest_db.commit();
            disk_images_dest_db.close();
        }
    }

    // ====== disk image end======

    // ======hashset start======

    QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    if(QFileInfo(case_conf_db).exists())
    {
        QString connection_naam_2 = QString(Q_FUNC_INFO) + "2";
        QSqlDatabase::removeDatabase(connection_naam_2);
        QSqlDatabase destination_db_case_conf = QSqlDatabase::addDatabase("QSQLITE",connection_naam_2);
        destination_db_case_conf.setDatabaseName(case_conf_db);
        if(!destination_db_case_conf.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_case_conf.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_case_conf.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_case_conf.isOpen())
        {
            QString result_conf_dir_path   = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString();

            QSqlQuery query_hashes_select(destination_db_case_conf);

            query_hashes_select.prepare("Select recon_filefrom,recon_file_infopath from tbl_hashset");

            if(query_hashes_select.exec())
            {
                QString recon_filefrom;
                QString recon_file_infopath;
                QString new_recon_file_infopath;

                QStringList value_list;
                QString command_update = QString("Update tbl_hashset Set recon_file_infopath = ? Where recon_filefrom = ? AND recon_file_infopath = ?");

                while(query_hashes_select.next())
                {
                    recon_filefrom  = query_hashes_select.value("recon_filefrom").toString().trimmed();
                    if(recon_filefrom == QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result))
                    {
                        recon_file_infopath  = query_hashes_select.value("recon_file_infopath").toString().trimmed();
                        if(recon_file_infopath.startsWith(QString("Hashset")))
                        {
                            new_recon_file_infopath = result_conf_dir_path + recon_file_infopath;
                            new_recon_file_infopath.remove(0,result_dir_path.size() -1);

                            value_list.clear();
                            value_list << new_recon_file_infopath << recon_filefrom << recon_file_infopath;
                            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_update, value_list, destination_db_case_conf, Q_FUNC_INFO);
                        }
                    }
                }
            }
            else
            {
                recon_static_functions::app_debug(" query execute ----FAILED--- " + query_hashes_select.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_hashes_select.lastError().text(),Q_FUNC_INFO);
            }

            ///create column recon_filefrom if not exist
            QString table_name     = "tbl_hashset";
            QString command_pragma = QString("PRAGMA TABLE_INFO( '") + table_name + "')";
            QStringList existing_colname_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_pragma, 1, destination_db_case_conf, Q_FUNC_INFO);

            QString column_str = "recon_filefrom";
            QString columns_field_type = "varchar(10)" ;
            if(!existing_colname_list.contains(column_str))
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ column_str + "' '" + columns_field_type + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_case_conf, Q_FUNC_INFO);

                cmd = "UPDATE '" + table_name +"' SET "+ column_str + "='2'";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_case_conf, Q_FUNC_INFO);
            }

            ///create column recon_file_infopath if not exist
            column_str = "recon_file_infopath";
            if(!existing_colname_list.contains(column_str))
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ column_str + "' '" + columns_field_type + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_case_conf, Q_FUNC_INFO);

                cmd = "UPDATE '" + table_name +"' SET "+ column_str + "=database_path";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_case_conf, Q_FUNC_INFO);
            }

            QSqlQuery query_hashes(destination_db_case_conf);

            query_hashes.prepare("Select table_name,column_name,database_path,recon_filefrom,recon_file_infopath From tbl_hashset");

            if(!query_hashes.exec())
            {
                recon_static_functions::app_debug(" query execute ----FAILED--- " + query_hashes.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug(" error -  " + query_hashes.lastError().text(),Q_FUNC_INFO);
            }
            else
            {
                QString hashset_conf_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString() + "hashset_index.sqlite";

                QFileInfo info(hashset_conf_index_db_path);

                bool bool_file_removed = false;
                if(info.exists() && info.size() <= 0)
                {
                    bool_file_removed = true;
                    QFile::remove(hashset_conf_index_db_path);
                }

                if(bool_file_removed || !info.exists())
                {
                    QString command_str = QString("create table IF NOT EXISTS hashset_indexes"
                                                  "(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                                                  "saved_db_name varchar(500),saved_db_path varchar(500),saved_table_name varchar(500),"
                                                  "md5_hash_column varchar(100), sha1_hash_column varchar(100),"
                                                  "show_files varchar(10),hide_files varchar(10), was_imported_external varchar(10), records_count varchar(500),"
                                                  "recon_filefrom varchar(10),recon_file_infopath varchar(10))");

                    recon_helper_standard_obj->execute_db_command_by_dbpath(command_str, hashset_conf_index_db_path, Q_FUNC_INFO);
                }

                QString command_insert = QString("Insert into hashset_indexes(saved_db_name,saved_db_path,saved_table_name,md5_hash_column,show_files,hide_files,recon_filefrom,recon_file_infopath) Values(?,?,?,?,?, ?,?,?)");

                QStringList value_list;

                while(query_hashes.next())
                {
                    QString table_name     = query_hashes.value("table_name").toString().trimmed();
                    QString column_name    = query_hashes.value("column_name").toString().trimmed();
                    QString database_path  = query_hashes.value("database_path").toString().trimmed();
                    QString recon_filefrom = query_hashes.value("recon_filefrom").toString().trimmed();
                    QString recon_file_infopath = query_hashes.value("recon_file_infopath").toString().trimmed();

                    QString db_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(database_path);

                    value_list.clear();
                    value_list << db_name << database_path << table_name << column_name << QString("1") << QString("0")
                               << recon_filefrom << recon_file_infopath;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert, value_list, hashset_conf_index_db_path, Q_FUNC_INFO);
                }
            }

            destination_db_case_conf.close();
        }

    }
    //========== hashset end======



    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_120_to_123()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);


    //====indexing start====
    QString indexing_queue_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "file_indexing_queue.sqlite";
    if(QFileInfo(indexing_queue_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db.setDatabaseName(indexing_queue_db_path);
        if(destination_db.open())
        {
            QString tablename = "files";

            QStringList new_columns_name_list;
            new_columns_name_list  << "plugin_name";
            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db);

            QString cmd_update = QString("Update files set plugin_name = ?");
            QStringList val_list;
            val_list << QString("File System");

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_update, val_list, destination_db, Q_FUNC_INFO);
            destination_db.close();

        }
        else
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        }
    }

    //====indexing end====

    //==== sqlite viewer start====
    QString sqlite_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";
    if(QFileInfo(sqlite_viewer_index_db_path).exists())
    {
        QString connection_naam_1 = QString(Q_FUNC_INFO) + "1";
        QSqlDatabase::removeDatabase(connection_naam_1);
        QSqlDatabase sqlite_index_dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_1);
        sqlite_index_dest_db.setDatabaseName(sqlite_viewer_index_db_path);
        if(sqlite_index_dest_db.open())
        {
            sqlite_index_dest_db.transaction();

            QString select_cmd_1 = "Select distinct source_count_name from sv_index";
            QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd_1 ,0 ,sqlite_index_dest_db,Q_FUNC_INFO);

            for(int ii = 0 ; ii < source_count_name_list.size() ; ii++)
            {
                QString source_count_name_str = source_count_name_list.at(ii);
                struct_GLOBAL_witness_info_source target_source_info =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);

                if(target_source_info.os_scheme_internal == "iOS_UID")
                {
                    QString select_cmd_2 = "Select distinct file_name, viewer_display_name , plugin_record_info , recon_file_infopath from sv_index where source_count_name = ?";
                    QSqlQuery select_qry(sqlite_index_dest_db);
                    select_qry.prepare(select_cmd_2);
                    select_qry.addBindValue(source_count_name_str);
                    if(select_qry.exec())
                    {
                        while(select_qry.next())
                        {
                            QString display_viewer_name_str = select_qry.value("viewer_display_name").toString();
                            QString previous_d_viewer_str = display_viewer_name_str;
                            QString uid_file_name = select_qry.value("file_name").toString();
                            QString plugin_record_info_str = select_qry.value("plugin_record_info").toString();


                            QString recon_file_info_path_str = select_qry.value("recon_file_infopath").toString();
                            QString display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(recon_file_info_path_str,source_count_name_str);
                            QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);

                            ///prevoius --  ff8fcf3dde9e81360b6ba51f0ea27c47b7e87844 [File System 2022]
                            ///now      --  main.db [File System 2022]

                            if(display_viewer_name_str.startsWith(uid_file_name))
                            {
                                display_viewer_name_str.remove(0 , uid_file_name.size());
                                display_viewer_name_str.prepend(display_file_name);
                            }

                            if(plugin_record_info_str.startsWith(uid_file_name))
                            {
                                plugin_record_info_str.remove(0 , uid_file_name.size());
                                plugin_record_info_str.prepend(display_file_name);
                            }

                            QString update_cmd = "UPDATE sv_index set file_name = ? , file_path = ? , viewer_display_name = ?, plugin_record_info = ? where viewer_display_name = ? AND source_count_name = ?";
                            QStringList arg_list;
                            arg_list << display_file_name << display_file_path << display_viewer_name_str << plugin_record_info_str  << previous_d_viewer_str << source_count_name_str;

                            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_cmd,arg_list,sqlite_index_dest_db,Q_FUNC_INFO);
                        }
                    }
                }
            }
            sqlite_index_dest_db.commit();
            sqlite_index_dest_db.close();
        }
    }
    //==== sqlite viewer end====


    //==== plist viewer start====
    QString plist_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
    if(QFileInfo(plist_viewer_index_db_path).exists())
    {
        update_result_feature_plist_viewer_123(plist_viewer_index_db_path);
    }



    ///Plist Viewer for Adhoc path
    QString plist_viewer_adhoc_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Plist_viewer_In_Result_QString).toString();
    if(QFileInfo(plist_viewer_adhoc_index_db_path).exists())
    {
        update_result_feature_plist_viewer_123(plist_viewer_adhoc_index_db_path);
    }
    //==== plist viewer end====



    //==== hex viewer start====
    QString hex_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    if(QFileInfo(hex_viewer_index_db_path).exists())
    {
        update_result_feature_hex_viewer_123(hex_viewer_index_db_path);
    }


    ///Hex Viewer for Adhoc path
    QString hex_viewer_adhoc_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    if(QFileInfo(hex_viewer_adhoc_index_db_path).exists())
    {
        update_result_feature_hex_viewer_123(hex_viewer_adhoc_index_db_path);
    }
    //==== hex viewer end====





    update_result_plugin_gmail_123();
    update_result_plugin_android_contacts_123();
    update_result_plugin_android_photos_123();
    update_result_plugin_android_installed_application_123();
    update_result_plugin_sound_recorder_123();
    update_result_plugin_online_accounts_123();
    update_result_plugin_airport_wireless_network_123();
    update_result_plugin_calendar_123();
    update_result_plugin_camera_123();
    update_result_plugin_clock_123();
    update_result_plugin_current_wallpaper_123();
    update_result_plugin_google_chrome_123();
    update_result_plugin_google_drive_123();
    update_result_plugin_launchpad_123();
    update_result_plugin_messages_123();
    update_result_plugin_bluetooth_123();
    update_result_plugin_youtube_123();
    update_result_plugin_wifi_location_123();
    update_result_plugin_printer_spool_123();
    update_result_plugin_google_keep_123();
    update_result_plugin_opera_mini_123();
    update_result_plugin_vlc_123();
    update_result_plugin_link_files_123();
    update_result_plugin_viber_123();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_123_to_125()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    //===================== CSV start===========================//
    QStringList plugins_csvs_list;
    plugins_csvs_list << "iPhoto" << "Photos" << "Mozilla Firefox" << "Safari" << "TorBrowser";
    for(int ii = 0 ; ii < plugins_csvs_list.size() ;ii++)
    {
        QString thumnail_plugin_name = plugins_csvs_list.at(ii);
        QString tm_plgn_nm = thumnail_plugin_name;
        tm_plgn_nm.replace(" ", "_");

        QString plugin_csv_f_path_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + tm_plgn_nm +".csv";
        QString plugin_csv_f_path_in_bundle  = "../Resources/CSVS/Plugins_CSV/" + tm_plgn_nm + ".csv";

        if(QFileInfo(plugin_csv_f_path_in_result).exists())
        {
            recon_static_functions::remove_safely_blank_dir_OR_file(plugin_csv_f_path_in_result, Q_FUNC_INFO);
            QFile::copy(plugin_csv_f_path_in_bundle, plugin_csv_f_path_in_result);
        }

    }
    //===================== CSV end===========================//


    update_result_plugin_bluetooth_logs_125();
    update_result_plugin_exe_files_125();
    update_result_plugin_hfs_plus_devices_logs_125();
    update_result_plugin_icloud_logs_125();
    update_result_plugin_network_logs_125();
    update_result_plugin_usb_logs_125();
    update_result_plugin_user_logs_125();
    update_result_plugin_wake_reason_logs_125();
    update_result_plugin_opera_mini_125();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_125_to_126()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_fs_events_126();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_126_to_128()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_voice_memos_128();
    update_result_plugin_clock_128();
    update_result_plugin_line_128();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_128_to_129()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    ///=========Replace Emlx Files CSV-Start============///
    QString feature_nm_real_emlx_files = "Emlx Files";
    QString feature_nm_tmp_emlx_files = feature_nm_real_emlx_files;
    feature_nm_tmp_emlx_files.replace(" ", "_");

    QString feature_csv_file_in_result_emlx_files = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "emlx_files_csv/"+feature_nm_tmp_emlx_files +".csv";
    QString feature_csv_file_in_app_bundle_emlx_files  = "../Resources/CSVS/Features_CSV/emlx_files_csv/" + feature_nm_tmp_emlx_files + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(feature_csv_file_in_result_emlx_files, Q_FUNC_INFO);
    QFile::copy(feature_csv_file_in_app_bundle_emlx_files, feature_csv_file_in_result_emlx_files);
    ///=========Replace Emlx Files CSV-End============///

    update_result_plugin_google_chrome_129();
    update_result_plugin_safari_129();
    update_result_plugin_mozilla_firefox_129();
    update_result_plugin_photos_129();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_129_to_130()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_translate_130();
    update_result_plugin_photos_130();
    update_result_plugin_utorrent_web_130();
    update_result_plugin_brave_browser_130();
    update_result_plugin_microsoft_edge_130();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void MainWindow::update_result_130_to_132()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_brave_browser_132();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_132_to_133()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    ///=========Replace FS CSV-Start============///
    QString filesystem_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "File_System_csv";
    QString filesystem_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/File_System_csv";

    recon_static_functions::do_directory_blank_completely(filesystem_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(filesystem_csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(filesystem_csv_dir_in_app_bundle, filesystem_csv_dir_in_result, Q_FUNC_INFO);
    ///=========Replace FS CSV-End============///



    ///=========Replace Artifacts Timeline CSV-Start============///
    QString timeline_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "artifacts_timeline_csv";
    QString timeline_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/artifacts_timeline_csv";

    recon_static_functions::do_directory_blank_completely(timeline_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(timeline_csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(timeline_csv_dir_in_app_bundle, timeline_csv_dir_in_result, Q_FUNC_INFO);
    ///=========Replace Artifacts Timeline CSV-End============///


    update_case_fs_module_status_to_133();
    update_result_plugin_utorrent_web_133();
    update_result_plugin_screen_time_133();
    update_result_plugin_tv_133();
    update_result_plugin_network_logs_133();
    update_result_plugin_user_logs_133();
    update_result_plugin_podcasts_133();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void MainWindow::update_result_133_to_135()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_stocks_135();
    update_result_plugin_brave_browser_135();
    update_result_plugin_mail_135();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void MainWindow::update_result_135_to_136()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);


    // These Function must be call first because we update all plugin information in below code.

    update_result_plugin_utorrent_web_136();
    update_result_plugin_podcasts_136();
    update_result_plugin_screen_time_136();
    update_result_plugin_brave_browser_136();
    update_result_plugin_bittorrent_web_136();



    //========================== 11 start ===============================//
    //Rename the Lab_Plugin directory in result directory to Artifacts_Result.
    //Or
    //Copy previous extracted plugins data to Artifacts_Result

    QString out_put_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString old_dir_path = out_put_dir_path + "Lab_Plugins/" ;
    QString new_dir_path = out_put_dir_path + "Artifacts_Result/";
    if(QDir(new_dir_path).exists())
    {
        recon_static_functions::copy_directory(old_dir_path, new_dir_path, Q_FUNC_INFO);
        recon_static_functions::do_directory_blank_completely(old_dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(old_dir_path,Q_FUNC_INFO);
    }
    else
    {
        QDir rename_dir;
        rename_dir.rename(old_dir_path,new_dir_path);
    }
    //========================== 11 end ===============================//

    //========================== winOS user and system information update - start ===============================//
    //Rename the Lab_Plugin directory for winOS user and system information in result case config.
    //Or
    //Copy previous extracted data in result case config.

    QString out_put_conf_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString()
            + QString("winOS") + QString("/winSource/");

    QString old_conf_dir_path = out_put_conf_dir_path + "Lab_Plugins/" ;
    QString new_conf_dir_path = out_put_conf_dir_path + "Artifacts_Result/";

    if(QDir(new_conf_dir_path).exists())
    {
        recon_static_functions::copy_directory(old_conf_dir_path, new_conf_dir_path, Q_FUNC_INFO);
        recon_static_functions::do_directory_blank_completely(old_conf_dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(old_conf_dir_path,Q_FUNC_INFO);
    }
    else
    {
        QDir rename_dir;
        rename_dir.rename(old_conf_dir_path,new_conf_dir_path);
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);

}

void MainWindow::update_result_136_to_137()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_fs_events_137();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_137_to_138()
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    update_result_plugin_connected_ios_devices_138();
    update_result_plugin_finder_138();
    update_result_plugin_recent_items_138();

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::update_result_138_to_139()
{

    ///=============Update Filesystem CSV-Start============///
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "file_system_csv";
    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/file_system_csv";
    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    ///=============Update Filesystem CSV-End============///

    update_result_plugin_reminders_139();
    update_result_plugin_photos_139();




}

void MainWindow::update_result_139_to_140()
{
    //===============================Apple_Metadata_Start===================================//
    QString aple_metadat_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Parent_QString).toString();
    QString apple_metadata_pyhton_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();

    QDir dir(aple_metadat_dir_path);
    QStringList internal_dir_list = dir.entryList();
    for(int ii = 0 ; ii < internal_dir_list.size(); ii++)
    {
        QString intnrl_dir = internal_dir_list.at(ii);
        if(!intnrl_dir.startsWith("Source"))
            continue;

        QString source_dir_path = aple_metadat_dir_path + intnrl_dir;
        QString dest_dir_path = apple_metadata_pyhton_dir_path + intnrl_dir;

        dir.mkpath(dest_dir_path);

        recon_static_functions::copy_directory(source_dir_path, dest_dir_path, Q_FUNC_INFO);

        recon_static_functions::do_directory_blank_completely(source_dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(source_dir_path, Q_FUNC_INFO);
    }
    //================================Apple_Metadata_End============================================//

    //================================Copy_Filesystem_dir_Start================================//
    QString file_system_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();

    QString file_system_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();
    QString file_system_apple_timestamps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString).toString();


    //Copy filesystem directory for apple timestamps
    QProcess process_copy;
    QStringList values_list;
    values_list << "-rf" << file_system_dir_path << file_system_apple_timestamps;

    process_copy.start("cp", values_list);
    if(!process_copy.waitForStarted(3000))
    {
        recon_static_functions::app_debug("Copy process -----FAILED-----", Q_FUNC_INFO);
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int counts = 0;
    while(1)
    {
        counts++;
        if(counts == 50)
        {
            counts = 0;
            QCoreApplication::processEvents();
        }
        if(process_copy.state() == QProcess::NotRunning)
            break;
    }

    //Copy filesystem directory for apple metadata
    values_list.clear();
    values_list << "-rf" << file_system_dir_path << file_system_apple_metadata;
    process_copy.start("cp", values_list);
    if(!process_copy.waitForStarted(3000))
    {
        recon_static_functions::app_debug("Copy process -----FAILED-----", Q_FUNC_INFO);
    }

    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int count = 0;
    while(1)
    {
        count++;
        if(count == 50)
        {
            count = 0;
            QCoreApplication::processEvents();
        }
        if(process_copy.state() == QProcess::NotRunning)
            break;
    }
    //================================Copy_Filesystem_dir_End================================//


    ///=============Update Carved Data CSV-Start============///
    QString carved_data_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "carved_data_csv";
    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/carved_data_csv/";
    recon_static_functions::do_directory_blank_completely(carved_data_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(carved_data_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, carved_data_csv_dir_in_result, Q_FUNC_INFO);
    ///=============Update Carved Data CSV-End============///





    update_result_plugin_tv_140();
    update_result_plugin_brave_browser_140();
    update_result_plugin_podcasts_140();
    update_result_plugin_online_accounts_140();
    update_result_plugin_uc_browser_140();
    update_result_plugin_whatsapp_140();
    update_result_plugin_textme_140();
    update_result_plugin_skype_140();
    update_result_plugin_icloud_140();
    update_result_plugin_itunes_140();
    update_result_plugin_mail_140();


}

void MainWindow::update_result_140_to_141()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    update_result_plugin_skype_141();
    update_result_plugin_viber_141();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void MainWindow::update_result_141_to_142()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=============Update  CSV-Start============//
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString).toString();
    QString csv_dir_in_app_bundle = "../Resources/CSVS/";
    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    //=============Update  CSV-End============//

    update_result_os_scheme_142();


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    QString command = "select distinct plugin_name from tbl_plugin_status where run_status = ?";
    QStringList plugin_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,QStringList("1"),0,db_path,Q_FUNC_INFO);
    for(int jj = 0; jj < plugin_list.size(); jj++)
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plugin_list.at(jj));
        if(plugin_obj != NULL)        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }
    }


    update_result_plugin_voice_mail_142();
    update_result_plugin_knowledgec_142();
    update_result_plugin_skype_142();

    update_result_plugin_facetime_142();
    update_result_plugin_notes_142();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_result_142_to_143()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    QStringList plugin_list;
    plugin_list << "Brave Browser" << "FaceTime" << "Google Drive" << "iTunes" << "Mail"
                <<"Maps" << "TeamViewer" << "Trash RecycleBin" << "Typed Words" << "Voice Mail";

    for(int ii = 0; ii < plugin_list.size(); ii++)
    {
        QString plgn_name = plugin_list.at(ii);
        QString plgn_tmp_name = plgn_name;
        plgn_tmp_name.replace(" ", "_");

        QString plgn_csv_file_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_name +".csv";
        QString plgn_csv_file_in_app_bundle  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_name + ".csv";
        recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result, Q_FUNC_INFO);
        QFile::copy(plgn_csv_file_in_app_bundle, plgn_csv_file_in_result);
    }


    update_result_plugin_safari_143();
    update_result_plugin_messages_143();
    update_result_plugin_tv_143();

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void MainWindow::update_result_143_to_144()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    update_result_plugin_airport_wirelessnetwork_144();
    update_result_plugin_maps_144();
    update_result_plugin_notifications_144();
    update_result_plugin_music_144();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_result_144_to_145()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QDirIterator iter_dir(result_dir_path, QDir::Files | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
    while(iter_dir.hasNext())
    {
        QString destination_db_file_path = iter_dir.next();
        if(!destination_db_file_path.endsWith(".sqlite"))
        {
            continue;
        }

        QStringList table_name_list = recon_helper_standard_obj->get_total_tab_name_from_db_by_db_path(destination_db_file_path,Q_FUNC_INFO);
        for(int jj = 0; jj < table_name_list.size(); jj++)
        {
            QString table_name = table_name_list.at(jj);
            QString command = "PRAGMA TABLE_INFO( '" + table_name_list.at(jj) + "')";
            QStringList column_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,destination_db_file_path,Q_FUNC_INFO);

            if(column_name_list.contains("source_type"))
            {
                QString renme_cmd = "ALTER TABLE " + table_name + " RENAME COLUMN source_type to source_type_internal";
                recon_helper_standard_obj->execute_db_command_by_dbpath(renme_cmd,destination_db_file_path,Q_FUNC_INFO);

                if(!column_name_list.contains("source_type_display"))
                {
                    QString cmd_add_column = "ALTER TABLE " + table_name + " ADD COLUMN source_type_display";
                    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_add_column,destination_db_file_path,Q_FUNC_INFO);
                }

                QString select_cmd = "select source_type_internal from " + table_name + "";
                QString recieved_src_type = recon_helper_standard_obj->get_string_from_db_by_dbpath(select_cmd,0,destination_db_file_path,Q_FUNC_INFO);

                QString src_type_dsply = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,0,Q_FUNC_INFO);
                QString src_type_intrnl = get_exact_source_type_from_src_type_partial_internal_145(recieved_src_type,1,Q_FUNC_INFO);

                QStringList args_list_1;
                QString update_cmd_1 = "Update " + table_name + " SET source_type_display = ?, source_type_internal = ?";
                args_list_1 << src_type_dsply << src_type_intrnl;
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd_1,args_list_1,destination_db_file_path,Q_FUNC_INFO);
            }
        }
    }


    //================================================CSV_Replace_Start================================================//

    QString disk_image_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "disk_images_csv";
    recon_static_functions::do_directory_blank_completely(disk_image_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(disk_image_csv_dir_in_result, Q_FUNC_INFO);
    QString disk_image_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/disk_images_csv";
    recon_static_functions::copy_directory(disk_image_csv_dir_in_app_bundle, disk_image_csv_dir_in_result, Q_FUNC_INFO);


    QString mobile_backup_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "mobile_backup_csv";
    recon_static_functions::do_directory_blank_completely(mobile_backup_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(mobile_backup_csv_dir_in_result, Q_FUNC_INFO);
    QString mobile_backup_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/mobile_backup_csv";
    recon_static_functions::copy_directory(mobile_backup_csv_dir_in_app_bundle, mobile_backup_csv_dir_in_result, Q_FUNC_INFO);


    QString ram_image_backup_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "ram_images_csv";
    recon_static_functions::do_directory_blank_completely(ram_image_backup_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(ram_image_backup_csv_dir_in_result, Q_FUNC_INFO);
    QString ram_image_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/ram_images_csv";
    recon_static_functions::copy_directory(ram_image_csv_dir_in_app_bundle, ram_image_backup_csv_dir_in_result, Q_FUNC_INFO);

    //================================================CSV_Replace_Start================================================//

    update_result_plugin_android_thumbnails_145();
    update_result_plugin_music_library_145();
    update_result_plugin_wifi_location_145();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void MainWindow::update_result_145_to_146()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_feature_emlx_mail_146();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_result_146_to_147()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);


    update_result_plugin_contacts_147();
    update_result_plugin_linkedin_147();
    update_result_plugin_airport_wireless_network_147();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}



void MainWindow::update_result_147_to_148()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=============Update  CSV-Start============//
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString).toString();
    QString csv_dir_in_app_bundle = "../Resources/CSVS/";
    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    //=============Update  CSV-End============//


    update_result_all_artifacts_for_domain_user_148();
    update_result_plugin_user_accounts_148();



    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_result_148_to_150()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=============Update  CSV-Start============//
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString).toString();
    QString csv_dir_in_app_bundle = "../Resources/CSVS/";
    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    //=============Update  CSV-End============//


    update_result_features_150();

    update_result_all_artifacts_for_system_user_150();

    update_result_plugin_reminders_150();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}

void MainWindow::update_result_150_to_151()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_features_151();



    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_151_to_152()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_plugin_messages_152();
    update_result_plugin_linkedin_152();
    update_result_plugin_whatsapp_152();
    update_result_plugin_mail_152();
    update_result_plugin_airport_wireless_network_152();
    update_result_plugin_photos_152();
    update_result_plugin_knowledgec_152();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_152_to_153()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_features_153();

    update_result_plugin_messages_153();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_153_to_154()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_plugin_messages_154();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_154_to_155()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_plugin_daily_out_155();

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);


}


void MainWindow::update_result_155_to_157()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_features_157();
    update_result_plugin_safari_157();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);


}

void MainWindow::update_result_157_to_158()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_features_158();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}

void MainWindow::update_result_158_to_159()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_features_159();


    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);

}

void MainWindow::update_result_159_to_160()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_plugin_thumb_cache_160();
    update_result_plugin_keychain_160();

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_160_to_161()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    update_result_features_161();

    //    update_result_plugin_reminders_999(); //graykey support
    //    update_result_plugin_books_999();
    //    update_result_plugin_facebook_999();
    //    update_result_plugin_apple_agent_logs_999();
    //    update_result_plugin_applications_track_logs_999();
    //    update_result_plugin_login_window_logs_999();
    //    update_result_plugin_mount_devices_logs_999();
    //    update_result_plugin_notifications_logs_999();
    //    update_result_plugin_screen_sharing_logs_999();
    //    update_result_plugin_system_logs_999();
    //    update_result_plugin_truecaller_999();
    //    update_result_plugin_wifi_logs_999();
    //    update_result_plugin_google_locations_999();

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}
