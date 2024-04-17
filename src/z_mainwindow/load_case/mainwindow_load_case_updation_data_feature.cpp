#include "z_mainwindow/mainwindow.h"


void MainWindow::update_result_feature_hex_viewer_123(QString db_path)
{
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase hex_dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    hex_dest_db.setDatabaseName(db_path);
    if(hex_dest_db.open())
    {
        hex_dest_db.transaction();

        QString select_cmd_1 = "Select distinct source_count_name from hex_viewer_index";
        QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd_1 ,0 ,hex_dest_db,Q_FUNC_INFO);

        for(int ii = 0 ; ii < source_count_name_list.size() ; ii++)
        {
            QString source_count_name_str = source_count_name_list.at(ii);
            struct_GLOBAL_witness_info_source target_source_info =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);

            //==== Target Source Structure has been updated so we are using MACROs
            //==== intstead of static os_scheme_internal value===========///

            if(target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            {
                QString select_cmd_2 = "Select distinct file_name, viewer_display_name , db_table_name , recon_file_infopath from hex_viewer_index where source_count_name = ?";
                QSqlQuery select_qry(hex_dest_db);
                select_qry.prepare(select_cmd_2);
                select_qry.addBindValue(source_count_name_str);
                if(select_qry.exec())
                {
                    while(select_qry.next())
                    {
                        QString display_viewer_name_str = select_qry.value("viewer_display_name").toString();
                        QString previous_d_viewer_str = display_viewer_name_str;
                        QString uid_file_name = select_qry.value("file_name").toString();
                        QString db_table_name_str = select_qry.value("db_table_name").toString();

                        QString recon_file_info_path_str = select_qry.value("recon_file_infopath").toString();
                        QString display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(recon_file_info_path_str,source_count_name_str);
                        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);

                        ///prevoius tab name --  662bc19b13aecef58a7e855d0316e4cf61e2642b [File System 1994]
                        ///now      --  com.apple.springboard.plist [File System 1994]
                        if(display_viewer_name_str.startsWith(uid_file_name))
                        {
                            display_viewer_name_str.remove(0 , uid_file_name.size());
                            display_viewer_name_str.prepend(display_file_name);
                        }


                        ///prevoius table name --  662bc19b13aecef58a7e855d0316e4cf61e2642b_File_System_1994
                        ///now      --  com_apple_springboard_plist_File_System_1994
                        if(db_table_name_str.startsWith(uid_file_name))
                        {
                            db_table_name_str.remove(0 , uid_file_name.size());
                            QString table_name = display_file_name;
                            table_name = table_name.replace(" ","_");
                            table_name = table_name.replace("-","_");
                            table_name = table_name.replace(".","_");
                            db_table_name_str.prepend(table_name);
                        }

                        QString update_cmd = "UPDATE hex_viewer_index set file_name = ?,file_path = ? , viewer_display_name = ?, db_table_name = ? where viewer_display_name = ? AND source_count_name = ?";
                        QStringList update_arg_list;
                        update_arg_list << display_file_name << display_file_path << display_viewer_name_str << db_table_name_str  << previous_d_viewer_str << source_count_name_str;
                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_cmd,update_arg_list,hex_dest_db,Q_FUNC_INFO);
                    }
                }
            }
        }
        hex_dest_db.commit();
        hex_dest_db.close();
    }
}

void MainWindow::update_result_feature_plist_viewer_123(QString db_path)
{
    QString connection_naam_2 = QString(Q_FUNC_INFO) + "2";
    QSqlDatabase::removeDatabase(connection_naam_2);
    QSqlDatabase plist_dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_2);
    plist_dest_db.setDatabaseName(db_path);
    if(plist_dest_db.open())
    {
        plist_dest_db.transaction();

        QString select_cmd_1 = "Select distinct source_count_name from plist_viewer_index";
        QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_cmd_1 ,0 ,plist_dest_db,Q_FUNC_INFO);

        for(int ii = 0 ; ii < source_count_name_list.size() ; ii++)
        {
            QString source_count_name_str = source_count_name_list.at(ii);
            struct_GLOBAL_witness_info_source target_source_info =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_str);

            //==== Target Source Structure has been updated so we are using MACROs
            //==== intstead of static os_scheme_internal value===========///

            if(target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
            {
                QString select_cmd_2 = "Select distinct file_path ,file_name, viewer_display_name , db_table_name , recon_file_infopath from plist_viewer_index where source_count_name = ?";
                QSqlQuery select_qry(plist_dest_db);
                select_qry.prepare(select_cmd_2);
                select_qry.addBindValue(source_count_name_str);
                if(select_qry.exec())
                {
                    while(select_qry.next())
                    {
                        QString display_viewer_name_str = select_qry.value("viewer_display_name").toString();
                        QString previous_d_viewer_str = display_viewer_name_str;
                        QString uid_file_name = select_qry.value("file_name").toString();
                        QString db_table_name_str = select_qry.value("db_table_name").toString();
                        QString previous_table_name_str = db_table_name_str;


                        QString recon_file_info_path_str = select_qry.value("recon_file_infopath").toString();
                        QString display_file_path = recon_helper_standard_obj->pub_get_fs_display_path_according_recon_file_infopath(recon_file_info_path_str,source_count_name_str);
                        QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_file_path);

                        ///prevoius tab name --  662bc19b13aecef58a7e855d0316e4cf61e2642b [File System 1994]
                        ///now      --  com.apple.springboard.plist [File System 1994]
                        if(display_viewer_name_str.startsWith(uid_file_name))
                        {
                            display_viewer_name_str.remove(0 , uid_file_name.size());
                            display_viewer_name_str.prepend(display_file_name);
                        }


                        ///prevoius table name --  662bc19b13aecef58a7e855d0316e4cf61e2642b_File_System_1994
                        ///now      --  com_apple_springboard_plist_File_System_1994
                        if(db_table_name_str.startsWith(uid_file_name))
                        {
                            db_table_name_str.remove(0 , uid_file_name.size());
                            QString table_name = display_file_name;
                            table_name = table_name.replace(" ","_");
                            table_name = table_name.replace("-","_");
                            table_name = table_name.replace(".","_");
                            db_table_name_str.prepend(table_name);
                        }

                        QString update_cmd = "UPDATE plist_viewer_index set file_name = ?,file_path = ? , viewer_display_name = ?, db_table_name = ? where viewer_display_name = ? AND source_count_name = ?";
                        QStringList update_arg_list;
                        update_arg_list << display_file_name << display_file_path << display_viewer_name_str << db_table_name_str  << previous_d_viewer_str << source_count_name_str;


                        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(update_cmd,update_arg_list,plist_dest_db,Q_FUNC_INFO);

                        ///alter table name from uid to domain name
                        QString alter_table_cmd = "ALTER table '" + previous_table_name_str + "'" + "RENAME TO '" +  db_table_name_str +"'";
                        recon_helper_standard_obj->execute_db_command_by_dbreference(alter_table_cmd,plist_dest_db,Q_FUNC_INFO);
                    }
                }
            }
        }
        plist_dest_db.commit();
        plist_dest_db.close();
    }
}

void MainWindow::update_case_fs_module_status_to_133()
{
    recon_static_functions::app_debug(" -Start", Q_FUNC_INFO);

    QString case_fs_module_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + QString("case_fs_module_status.sqlite");

    QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + QString("sources_info.sqlite");


    QString command_str;
    QStringList status_list;

    QStringList tmp_values_list;

    //========Update-Exif Metadata run status-Start===========//

    command_str = QString("Select Exif_Metadata From fs_status");

    status_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str, 0, sources_db_path, Q_FUNC_INFO);
    if(status_list.contains(QString("1")))
    {
        tmp_values_list.clear();
        command_str = QString("Insert into tbl_fs_module_status(module_job_type,run_status) Values(?,?)");
        tmp_values_list << QString("Exif Metadata") <<  QString("1");

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, tmp_values_list, case_fs_module_status_db_path, Q_FUNC_INFO);
    }
    //========Update-Exif Metadata run status-End===========//



    //========Update-Hashes run status-Start===========//

    command_str = QString("Select Hashes From fs_status");
    status_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_str, 0, sources_db_path, Q_FUNC_INFO);
    if(status_list.contains(QString("1")))
    {
        tmp_values_list.clear();
        command_str = QString("Insert into tbl_fs_module_status(module_job_type,run_status) Values(?,?)");
        tmp_values_list << QString("Hashes") <<  QString("1");

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_str, tmp_values_list, case_fs_module_status_db_path, Q_FUNC_INFO);
    }
    //========Update-Hashes run status-End===========//

    recon_static_functions::app_debug(" -End", Q_FUNC_INFO);
}

void MainWindow::update_result_feature_emlx_mail_146()
{
    //========CSV Update-Start===========//

    //    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "email_raw_tag_csv";

    //    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/email_raw_tag_csv";

    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "emlx_mail_tag_csv";
    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/emlx_mail_tag_csv";

    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    //========CSV Update-End===========//


    //========CSV Update-Start===========//
    QString  mail_csv_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString();
    csv_dir_in_result = mail_csv_dir_path + "emlx_files_csv";
    csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/emlx_mail_csv";

    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);

    recon_static_functions::copy_directory(csv_dir_in_app_bundle, mail_csv_dir_path + "emlx_mail_csv", Q_FUNC_INFO);
    //========CSV Update-End===========//

    //========Update Feature dir -Start===========//
    QString old_dir_path_in_rslt = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "Lab_Features/" + "Emlx_Files/";
    QString new_dir_path_in_rslt = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "Lab_Features/EMLX_Mail/";

    recon_static_functions::copy_directory(old_dir_path_in_rslt, new_dir_path_in_rslt, Q_FUNC_INFO);

    recon_static_functions::do_directory_blank_completely(old_dir_path_in_rslt, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(old_dir_path_in_rslt, Q_FUNC_INFO);
    //========Update Feature dir -End===========//




    //========Plugin Name Update-Start===========//
    QStringList tags_db_path_list;
    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";
    tags_db_path_list << bookmark_db_path;

    QString tag_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString();
    QString tag_index_db_path = tag_search_dir + "index_tag_search.sqlite";
    QString command = "Select tag_db_name from tag_search_index";
    QStringList tag_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command , 0 ,tag_index_db_path, Q_FUNC_INFO);

    for(int i = 0 ; i <  tag_db_name_list.size(); i++)
    {
        QString db_name = tag_db_name_list.at(i);
        tags_db_path_list << tag_search_dir + db_name + ".sqlite";
    }

    // Update tags SET plugin_name="EMLX Mail", csv_plugin_name="EMLX Mail",category="EMLX Mail" Where  plugin_name="Email File"
    QString cmd_update = "Update tags SET plugin_name=?,csv_plugin_name=?,category=? Where plugin_name='Email File'";
    QStringList vals_list;
    vals_list << QString(MACRO_Plugin_Name_Email_Parser)
              << QString(MACRO_Plugin_Name_Email_Parser)
              << QString(MACRO_Plugin_Name_Email_Parser);


    for(int i = 0 ; i <  tags_db_path_list.size(); i++)
    {
        QString tag_db_path = tags_db_path_list.at(i);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_update,vals_list,tag_db_path,Q_FUNC_INFO);
    }
    //========Plugin Name Update-End===========//


}


void MainWindow::update_result_features_150()
{

    //=============Update  CSV-Start============//
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString).toString();
    QString csv_dir_in_app_bundle = "../Resources/CSVS/";
    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    //=============Update  CSV-End============//


    QStringList columns_name_list;
    QStringList columns_field_type_list;
    QString table_name;
    QStringList columns_ocr_text_list;
    QStringList columns_field_ocr_text_type_list;
    columns_ocr_text_list  << "ocr_text";
    columns_field_ocr_text_type_list << "varchar(5000)";

    ///======Update Filesystem Database===========///
    table_name = "files";
    columns_name_list << "fs_module_face_analysis_run_status" << "fs_module_optical_character_recognition_run_status"
                      << "fs_module_skin_tone_detection_run_status" << "fs_module_weapons_run_status" << "fs_module_fire_analysis_run_status";
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

                QString cmd_update = "UPDATE '" + table_name +"' SET '"+columns_name_list.at(ii) +"' = '0'";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_update, destination_db, Q_FUNC_INFO);
            }
        }

        for(int ii  = 0 ; ii < columns_ocr_text_list.size() ; ii++)
        {
            if(!destination_db.record(table_name).contains(columns_ocr_text_list.at(ii)))
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_ocr_text_list.at(ii) + "' '" + columns_field_ocr_text_type_list.at(ii) + "'");
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);

                QString cmd_update = "UPDATE '" + table_name +"' SET '" + columns_ocr_text_list.at(ii) + "' = ''";
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_update, destination_db, Q_FUNC_INFO);
            }
        }

        destination_db.close();
    }

}

void MainWindow::update_result_features_151()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //=======================face_info db update starts=======================//

    QString face_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString()
            + QString("face_info.sqlite");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString file_system_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();

    if(file_system_result_dir.startsWith(result_dir_path))
    {
        file_system_result_dir = file_system_result_dir.remove(0, result_dir_path.size());
    }

    if(!file_system_result_dir.startsWith("/"))
    {
        file_system_result_dir = file_system_result_dir.prepend("/");
    }

    if(file_system_result_dir.endsWith("/"))
    {
        file_system_result_dir.chop(QString("/").size());
    }

    QStringList values_list;

    if(QFileInfo(face_info_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(face_info_db_path);
        destination_db.transaction();

        if(destination_db.open())
        {
            QString select_command = QString("select fs_record ,file_path from distinct_faces");
            QSqlQuery query(destination_db);
            query.prepare(select_command);

            if(query.exec())
            {
                while(query.next())
                {
                    QString fs_record = query.value("fs_record").toString();

                    QString file_path = query.value("file_path").toString();

                    QString updated_file_path = file_system_result_dir + file_path ;

                    QString command_str = "Update distinct_faces SET file_path = ?  Where fs_record = ? And file_path = ? " ;

                    values_list << updated_file_path << fs_record << file_path;

                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_str,values_list,destination_db,Q_FUNC_INFO);

                    values_list.clear();
                }
                destination_db.commit();
                destination_db.close();
            }
            else
            {
                recon_static_functions::app_debug(" select query ----FAILED---- " + destination_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug(" select query ----executedQuery---- " + query.executedQuery() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + query.lastError().text() , Q_FUNC_INFO);
                destination_db.commit();
                destination_db.close();

            }
        }
    }

    //===================face_info db update ends===================//

    //===================face_search dir and db update starts===================//

    QString face_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString();

    QString face_search_index_db = face_search_dir_path + "index_face_search.sqlite";
    QStringList search_label_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select search_label_name from face_search_index"), 0, face_search_index_db, Q_FUNC_INFO);

    for(int ii = 0; ii <  search_label_name_list.size(); ii++)
    {
        QString searched_face_dir_name = search_label_name_list.at(ii);
        searched_face_dir_name = searched_face_dir_name.replace(" ", "_");

        QString searched_face_dir_path = face_search_dir_path + searched_face_dir_name;

        QString previous_searched_face_db_path = face_search_dir_path + searched_face_dir_name + ".sqlite";

        if(!QDir(searched_face_dir_path).exists())
        {
            QDir dir_fs;
            if(!dir_fs.mkdir(searched_face_dir_path))
            {
                recon_static_functions::app_debug("face_search_directory_path  ----FAILED---- " + searched_face_dir_path, Q_FUNC_INFO);
                continue;
            }

            QString searched_face_db_path = searched_face_dir_path + ("/record.sqlite");

            if (!QFile::copy(previous_searched_face_db_path, searched_face_db_path))
            {
                recon_static_functions::app_debug("searched_face_db_copy  ----FAILED---- " , Q_FUNC_INFO);
                continue;
            }

            QFile::remove(previous_searched_face_db_path);


            QString cmd_create_filter_tabel = "create table filter(INT INTEGER PRIMARY KEY, searched_face_pic_path varchar(200))";

            recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_create_filter_tabel, searched_face_db_path, Q_FUNC_INFO);
        }

        QString update_cmd = "update face_search_index set search_db_file_name = 'record.sqlite'";
        recon_helper_standard_obj->execute_db_command_by_dbpath(update_cmd, face_search_index_db, Q_FUNC_INFO);
    }

    //===================face_search dir and db update ends===================//

    //==================Extracted iOS backup Dir rename for Cellebrite TAR and UFDR Start ==============//

    QString ios_bkup_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString()  + QString("ios_backup_index.sqlite");

    QString connection_naam = QString(Q_FUNC_INFO) + "_1";
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(ios_bkup_index_db);
    if(destination_db.open())
    {
        QSqlQuery select_query(destination_db);

        QString cmd_str = QString("SELECT source_count_name,recon_file_infopath FROM tbl_index");

        if(!select_query.exec(cmd_str))
        {
            recon_static_functions::app_debug("Select query  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("Executed query - " + select_query.executedQuery() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error " + select_query.lastError().text() , Q_FUNC_INFO);
        }

        while(select_query.next())
        {
            QString old_partial_bkup_path = select_query.value("recon_file_infopath").toString().trimmed();
            QString src_cnt_name = select_query.value("source_count_name").toString().trimmed();

            QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString old_complete_bkup_path = recon_static_functions::get_complete_file_path(result_dir,old_partial_bkup_path,Q_FUNC_INFO);

            QStringList unlock_args;
            unlock_args << "-a" << "l" << old_complete_bkup_path;
            recon_helper_process_obj->run_command_with_arguments("SetFile",unlock_args,Q_FUNC_INFO);

            QString new_complete_bkup_path =  old_complete_bkup_path + "_upto_150";
            QFile::rename(old_complete_bkup_path,new_complete_bkup_path);

            QString new_partial_bkup_path = old_partial_bkup_path + "_upto_150";

            QStringList values;
            values << new_partial_bkup_path << src_cnt_name;
            QString cmd_update = "UPDATE tbl_index SET recon_file_infopath = ? WHERE source_count_name = ?";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_update,values,destination_db,Q_FUNC_INFO);

        }

        destination_db.close();
    }
    else
    {
        recon_static_functions::app_debug("DB ios backup open  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
    }
    //==================Extracted iOS backup Dir rename for Cellebrite TAR and UFDR End ==============//

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_features_153()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    //==================Extracted Android backup Dir rename for ADB Android AB Start ==============//

    QString android_bkup_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString()  + QString("android_backup_index.sqlite");
    if(QFileInfo(android_bkup_index_db).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(android_bkup_index_db);
        if(destination_db.open())
        {
            QSqlQuery select_query(destination_db);

            QString cmd_str = QString("SELECT source_count_name,recon_file_infopath FROM tbl_index");

            if(select_query.exec(cmd_str))
            {
                while(select_query.next())
                {
                    QString old_partial_bkup_path = select_query.value("recon_file_infopath").toString().trimmed();
                    QString src_cnt_name = select_query.value("source_count_name").toString().trimmed();

                    QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                    QString old_complete_bkup_path = recon_static_functions::get_complete_file_path(result_dir,old_partial_bkup_path,Q_FUNC_INFO);

                    QStringList unlock_args;
                    unlock_args << "-a" << "l" << old_complete_bkup_path;
                    recon_helper_process_obj->run_command_with_arguments("SetFile",unlock_args,Q_FUNC_INFO);

                    QString new_complete_bkup_path =  old_complete_bkup_path + "_upto_152";
                    QFile::rename(old_complete_bkup_path,new_complete_bkup_path);

                    QString new_partial_bkup_path = old_partial_bkup_path + "_upto_152";

                    QStringList values;
                    values << new_partial_bkup_path << src_cnt_name;
                    QString cmd_update = "UPDATE tbl_index SET recon_file_infopath = ? WHERE source_count_name = ?";
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_update,values,destination_db,Q_FUNC_INFO);

                    QStringList lock_args;
                    lock_args << "-a" << "L" << new_complete_bkup_path;
                    recon_helper_process_obj->run_command_with_arguments("SetFile",lock_args,Q_FUNC_INFO);

                }
            }
            else
            {
                recon_static_functions::app_debug("Select query  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Executed query - " + select_query.executedQuery() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + select_query.lastError().text() , Q_FUNC_INFO);
            }

            destination_db.close();
        }

    }
    //==================Extracted Android backup Dir rename for ADB Android AB End ==============//


    //==================Extracted iOS backup Dir rename for GrayKey iOS Backup Start ==============//

    QString ios_bkup_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString()  + QString("ios_backup_index.sqlite");

    if(QFileInfo(ios_bkup_index_db).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO) + "_1";
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(ios_bkup_index_db);
        if(dest_db.open())
        {
            QSqlQuery select_query(dest_db);

            QString cmd_str = QString("SELECT source_count_name,recon_file_infopath FROM tbl_index");

            if(select_query.exec(cmd_str))
            {
                while(select_query.next())
                {
                    QString old_partial_bkup_path = select_query.value("recon_file_infopath").toString().trimmed();
                    QString src_cnt_name = select_query.value("source_count_name").toString().trimmed();

                    QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                    QString old_complete_bkup_path = recon_static_functions::get_complete_file_path(result_dir,old_partial_bkup_path,Q_FUNC_INFO);

                    QStringList unlock_args;
                    unlock_args << "-a" << "l" << old_complete_bkup_path;
                    recon_helper_process_obj->run_command_with_arguments("SetFile",unlock_args,Q_FUNC_INFO);

                    QString old_backup_absolute_path = QFileInfo(old_complete_bkup_path).absolutePath();

                    QString new_complete_bkup_path =  old_backup_absolute_path + "_upto_152";
                    QFile::rename(old_backup_absolute_path,new_complete_bkup_path);

                    QString new_partial_absolute_bkup_path = QFileInfo(old_partial_bkup_path).absolutePath();
                    QString source_dir_name = QDir(old_partial_bkup_path).dirName();
                    QString new_partial_bkup_path = new_partial_absolute_bkup_path + "_upto_152";
                    QString new_partial_backup_path_src_name = new_partial_bkup_path + "/" + source_dir_name;

                    QStringList values;
                    values << new_partial_backup_path_src_name << src_cnt_name;
                    QString cmd_update = "UPDATE tbl_index SET recon_file_infopath = ? WHERE source_count_name = ?";
                    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_update,values,dest_db,Q_FUNC_INFO);


                    QStringList lock_args;
                    lock_args << "-a" << "L" << new_complete_bkup_path + "/" + source_dir_name;
                    recon_helper_process_obj->run_command_with_arguments("SetFile",lock_args,Q_FUNC_INFO);

                }
            }
            else
            {
                recon_static_functions::app_debug("Select query  ---FAILED--- " + dest_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Executed query - " + select_query.executedQuery() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + select_query.lastError().text() , Q_FUNC_INFO);
            }

            dest_db.close();
        }
    }
    //==================Extracted iOS backup Dir rename for GrayKey iOS Backup End ==============//

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);

}

void MainWindow::update_result_os_scheme_142()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    //==================================os_schme_Start============================//
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
            QString command = "PRAGMA TABLE_INFO( '" + table_name_list.at(jj) + "')";
            QStringList column_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,destination_db_file_path,Q_FUNC_INFO);

            if(column_name_list.contains("os_naming_scheme"))
            {
                QString rename_cmd = "ALTER TABLE "+ table_name_list.at(jj)+ " RENAME COLUMN os_naming_scheme to os_scheme_display";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,destination_db_file_path,Q_FUNC_INFO);
            }

            if(column_name_list.contains("os_scheme"))
            {
                QString rename_cmd = "ALTER TABLE "+ table_name_list.at(jj)+ " RENAME COLUMN os_scheme to os_scheme_display";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,destination_db_file_path,Q_FUNC_INFO);
            }
        }
    }
    //==================================os_schme_End============================//

    //==================================Artifacts_Source_File_Start============================//
    QString destination_plugin_dir_path = result_dir_path + "Artifacts_Result/";
    QDirIterator artifact_iter_dir(destination_plugin_dir_path, QDir::Files | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
    while(artifact_iter_dir.hasNext())
    {
        QString destination_db_file_path = artifact_iter_dir.next();
        if(!destination_db_file_path.endsWith(".sqlite"))
        {
            continue;
        }
        QStringList table_name_list = recon_helper_standard_obj->get_total_tab_name_from_db_by_db_path(destination_db_file_path,Q_FUNC_INFO);
        for(int jj = 0; jj < table_name_list.size(); jj++)
        {
            QString command = "PRAGMA TABLE_INFO( '" + table_name_list.at(jj) + "')";
            QStringList column_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,1,destination_db_file_path,Q_FUNC_INFO);
            if(column_name_list.contains("artifacts_source_file"))
            {
                QString rename_cmd = "ALTER TABLE "+ table_name_list.at(jj)+ " RENAME COLUMN artifacts_source_file to artifacts_relation_source_file";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,destination_db_file_path,Q_FUNC_INFO);
            }
            else
            {
                QString add_new_column_command = "ALTER TABLE "+ table_name_list.at(jj) + " add column artifacts_relation_source_file VARCHAR(100)";
                recon_helper_standard_obj->execute_db_command_by_dbpath(add_new_column_command,destination_db_file_path,Q_FUNC_INFO);
            }
        }
    }
    //==================================Artifacts_Source_File_End============================//

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void MainWindow::update_result_features_157()
{
    //========CSV Update-Start===========//

    QString  feature_csv_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString();
    QString csv_dir_fs_in_result = feature_csv_dir_path + "file_system_csv";
    QString csv_dir_fs_in_app_bundle = "../Resources/CSVS/Features_CSV/file_system_csv";
    recon_static_functions::do_directory_blank_completely(csv_dir_fs_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_fs_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_fs_in_app_bundle, csv_dir_fs_in_result, Q_FUNC_INFO);

    QString csv_dir_snapshot_in_result = feature_csv_dir_path + "snapshots_csv";
    QString csv_dir_snapshot_in_app_bundle = "../Resources/CSVS/Features_CSV/snapshots_csv";
    recon_static_functions::do_directory_blank_completely(csv_dir_snapshot_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_snapshot_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_snapshot_in_app_bundle, csv_dir_snapshot_in_result, Q_FUNC_INFO);

    QString mobile_backup_csv_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString();
    QString csv_dir_in_result = mobile_backup_csv_dir_path + "mobile_backup_csv";
    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/mobile_backup_csv";
    recon_static_functions::do_directory_blank_completely(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);

    //========CSV Update-End===========//


    //========FS and Snapshot Db Update-Start=======//

    QStringList columns_name_list;
    columns_name_list << "date_time_last_modified" << "date_time_created" << "date_time_last_read"
                      << "date_last_modified" << "date_created" << "date_last_read" << "date_added" << "date_time_added"
                      << "z_date_time_created";

    QString table_name = "files";
    QStringList fs_and_snap_db_path_list;
    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/file_system.sqlite";
        fs_and_snap_db_path_list << fs_db_path;

        QString snapshot_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name;
        QDirIterator m_itr(snapshot_dir_path,QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
        while(m_itr.hasNext())
        {
            fs_and_snap_db_path_list << m_itr.next();
        }
    }

    for(int kk = 0; kk < fs_and_snap_db_path_list.size(); kk++)
    {
        for(int jj = 0; jj < columns_name_list.size(); jj++)
        {
            QString column_name = columns_name_list.at(jj);
            if(column_name == "date_time_last_modified")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN "+ columns_name_list.at(jj) +" to z_date_time_modified";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,fs_and_snap_db_path_list.at(kk),Q_FUNC_INFO);

            }
            else if(column_name == "date_time_created")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN "+ columns_name_list.at(jj) +" to z_date_time_change";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,fs_and_snap_db_path_list.at(kk),Q_FUNC_INFO);

            }
            else if(column_name == "date_time_last_read")
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN "+ columns_name_list.at(jj) +" to z_date_time_accessed";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,fs_and_snap_db_path_list.at(kk),Q_FUNC_INFO);

            }
            else if(column_name == "z_date_time_created")
            {
                QString cmd = QString("ALTER TABLE '" + table_name + "' ADD '"+ columns_name_list.at(jj) + "' 'INTEGER'");
                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, fs_and_snap_db_path_list.at(kk), Q_FUNC_INFO);
            }
            else
            {
                QString rename_cmd = "ALTER TABLE "+ table_name + " RENAME COLUMN "+ columns_name_list.at(jj) +" to " + columns_name_list.at(jj) +"_upto_157";
                recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,fs_and_snap_db_path_list.at(kk),Q_FUNC_INFO);
            }
        }
    }
    //========FS and Snapshot Db Update-End=======//

    //========Snapshot Difference Db Update-Start=======//
    QString saved_snap_dir_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString();
    QString saved_snap_diff_db_path = saved_snap_dir_db_path + "saved_snapshots_index.sqlite";

    QString cmd = "select snapshot_lable_name from tbl_saved_index";
    QStringList snap_diff_db_path_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd,0,saved_snap_diff_db_path,Q_FUNC_INFO);

    QStringList tables_name;
    tables_name << "tbl_created_files" << "tbl_deleted_files" << "tbl_modified_files";

    for(int ii = 0; ii < snap_diff_db_path_list.size(); ii++)
    {
        QString db_path = saved_snap_dir_db_path + snap_diff_db_path_list.at(ii) + ".sqlite";

        for(int kk = 0; kk < tables_name.size(); kk++)
        {
            for(int jj = 0; jj < columns_name_list.size(); jj++)
            {
                QString column_name = columns_name_list.at(jj);
                if(column_name == "date_time_last_modified")
                {
                    QString rename_cmd = "ALTER TABLE "+ tables_name.at(kk) + " RENAME COLUMN "+ columns_name_list.at(jj) +" to z_date_time_modified";
                    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,db_path,Q_FUNC_INFO);

                }
                else if(column_name == "date_time_created")
                {
                    QString rename_cmd = "ALTER TABLE "+ tables_name.at(kk) + " RENAME COLUMN "+ columns_name_list.at(jj) +" to z_date_time_change";
                    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,db_path +"",Q_FUNC_INFO);

                }
                else if(column_name == "date_time_last_read")
                {
                    QString rename_cmd = "ALTER TABLE "+ tables_name.at(kk) + " RENAME COLUMN "+ columns_name_list.at(jj) +" to z_date_time_accessed";
                    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,db_path,Q_FUNC_INFO);

                }
                else if(column_name == "z_date_time_created")
                {
                    QString cmd = QString("ALTER TABLE '" + tables_name.at(kk) + "' ADD '" + columns_name_list.at(jj) + "' 'INTEGER'");
                    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, db_path, Q_FUNC_INFO);
                }
                else
                {
                    QString rename_cmd = "ALTER TABLE "+ tables_name.at(kk) + " RENAME COLUMN "+ columns_name_list.at(jj) +" to " + columns_name_list.at(jj) +"_upto_157";
                    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_cmd,db_path,Q_FUNC_INFO);
                }
            }
        }
    }
    //========Snapshot Difference Db Update-End========//
}

void MainWindow::update_result_features_158()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);

        QString video_thumbnails_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/Video_Thumbnails/";

        QDir m_dir;
        if(m_dir.mkpath(video_thumbnails_path))
        {
            recon_static_functions::app_debug("Failed Create thumbnails -------FAILED------ " + video_thumbnails_path, Q_FUNC_INFO);
        }

        QString command_thumb_file = QString("create table files (filesystem_record INTEGER PRIMARY KEY, thumb_filepath varchar(1024))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_thumb_file, video_thumbnails_path, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_features_159()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QList<struct_GLOBAL_witness_info_source> target_info_list = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    for(int count = 0; count < target_info_list.size(); count++)
    {
        struct_GLOBAL_witness_info_source target_struct_obj = target_info_list.at(count);
        QString griffeye_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + target_struct_obj.source_count_name + "/griffeye_data.sqlite";

        if(!QFileInfo(griffeye_db_path).exists())
        {
            QString command_griffeye_data = QString("Create table files (filesystem_record INTEGER PRIMARY KEY, md5_hash varchar(300), griffeye_category varchar(200))");
            recon_helper_standard_obj->execute_db_command_by_dbpath(command_griffeye_data, griffeye_db_path, Q_FUNC_INFO);
        }
    }

    //================plist viewer db changes START ===================//
    QString plist_viewer_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";

    QStringList plist_viewer_db_path_list;
    plist_viewer_db_path_list << plist_viewer_feature_db_path;

    for(int jj = 0; jj < plist_viewer_db_path_list.size(); jj++ )
    {
        QString plist_viewer_db_path = plist_viewer_db_path_list.at(jj);
        if(QFileInfo(plist_viewer_db_path).exists())
        {
            QString connection_naam = QString(Q_FUNC_INFO) + QString::number(jj);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            destination_db.setDatabaseName(plist_viewer_db_path);
            if(destination_db.open())
            {
                QSqlQuery select_query(destination_db);
                QString select_command = "SELECT db_table_name from 'plist_viewer_index'";

                select_query.prepare(select_command);
                if(!select_query.exec())
                {
                    recon_static_functions::app_debug("Select query  ---FAILED--- " + destination_db.databaseName(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("Executed query - " + select_query.executedQuery() , Q_FUNC_INFO);
                    recon_static_functions::app_debug("Error " + select_query.lastError().text() , Q_FUNC_INFO);

                }

                QStringList plist_table_name_list;
                while(select_query.next())
                {
                    plist_table_name_list << select_query.value("db_table_name").toString();
                }

                QString column_name = QString("data_conversion_type");
                QString column_field = QString("VARCHAR(500)");

                for(int ii = 0; ii < plist_table_name_list.size(); ii++)
                {
                    QString plist_table_name = plist_table_name_list.at(ii);

                    if(!destination_db.record(plist_table_name).contains(column_name))
                    {
                        QString cmd = QString("ALTER TABLE '" + plist_table_name + "' ADD '"+ column_name + "' '" + column_field + "'");
                        recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
                    }
                }
                destination_db.close();
            }
        }
    }

    //================plist viewer db changes END ===================//

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void MainWindow::update_result_features_161()
{
    //======CSV Update START===========//

    QString features_csv_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString();
    QString emlx_mail_csv_dir_in_result = features_csv_dir_path + "emlx_mail_csv";
    QString email_parser_csv_file_in_app_bundle = "../Resources/CSVS/Features_CSV/email_parser_csv";

    recon_static_functions::do_directory_blank_completely(emlx_mail_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(emlx_mail_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(email_parser_csv_file_in_app_bundle, features_csv_dir_path + "email_parser_csv", Q_FUNC_INFO);

    QString emlx_mail_tag_csv_dir_in_result = features_csv_dir_path + "emlx_mail_tag_csv";
    QString email_tag_csv_file_in_app_bundle = "../Resources/CSVS/Features_CSV/email_parser_tag_csv";

    recon_static_functions::do_directory_blank_completely(emlx_mail_tag_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(emlx_mail_tag_csv_dir_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(email_tag_csv_file_in_app_bundle, features_csv_dir_path + "email_parser_tag_csv", Q_FUNC_INFO);

    QString unified_logs_csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "unified_logs_csv";
    QString unified_logs_csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/unified_logs_csv";
    recon_static_functions::copy_directory(unified_logs_csv_dir_in_app_bundle, unified_logs_csv_dir_in_result, Q_FUNC_INFO);

    //======CSV Update End===========//


    //======Update EMlX MAIL DIR RENAME AND DATABASE START===========//
    QString Lab_featr_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Parent_In_Result_QString).toString();
    QString old_emlx_feature_path = Lab_featr_path + "EMLX_Mail/";
    QString new_email_feature_dir_path = Lab_featr_path +  QString("Email_Parser/");


    QDir old_emlx_dir(old_emlx_feature_path);

    if(QFile(old_emlx_feature_path + "emlx_data.sqlite").exists())
    {
        QFile::remove(new_email_feature_dir_path + "email_data.sqlite");
        QFile::rename(old_emlx_feature_path + "emlx_data.sqlite",old_emlx_feature_path + "email_data.sqlite");

        QString tables_name = "emlx_data";


        QStringList column_name_list;
        column_name_list << "file_raw_bytes" << "file_name" << "email_type";

        QStringList columns_field_type_list;
        columns_field_type_list << "BLOB" << "VARCHAR(1024)" << "VARCHAR(255)";

        QString old_parser_db_path = old_emlx_feature_path + "/email_data.sqlite";

        for(int ii = 0 ; ii < column_name_list.size() ; ii++)
        {
            QString cmd = QString("ALTER TABLE '" + tables_name + "' ADD '" + column_name_list.at(ii) + "' '"  + columns_field_type_list.at(ii) + "'");

            recon_helper_standard_obj->execute_db_command_by_dbpath(cmd, old_parser_db_path, Q_FUNC_INFO);
        }


        recon_static_functions::copy_directory(old_emlx_feature_path,new_email_feature_dir_path,Q_FUNC_INFO);

        //======Update Email Bookmark Search Database Start===========//

        QString bookmark_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString();
        QString bookmark_search_db_path = bookmark_search_dir_path + "bookmarks.sqlite";

        QString command = "UPDATE tags SET category = 'Email Parser', plugin_name = 'Email Parser', tab_name = 'Email Data',"
                          "csv_plugin_name = 'Email Parser', csv_tab_name = 'Email Data' WHERE plugin_name = 'EMLX Mail'";

        recon_helper_standard_obj->execute_db_command_by_dbpath(command,bookmark_search_db_path,Q_FUNC_INFO);


        //======Update Email Bookmark Search Database End===========//


        //======Update Email Tag Search Database Start===========//

        QString command_email = "UPDATE tags SET category = 'Email Parser', plugin_name = 'Email Parser', tab_name = 'Email Data', "
                                "csv_plugin_name = 'Email Parser', csv_tab_name = 'Email Data' WHERE plugin_name = 'EMLX Mail'";

        QString command_email_tag = "UPDATE tags SET category = 'Email Tag Data', plugin_name = 'Email Parser Tag', "
                                    "tab_name = 'Email Tag Data', csv_plugin_name = 'Email Parser Tag', csv_tab_name = 'Email Tag Data' WHERE plugin_name = 'EMLX Mail Tag'";

        QString tag_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString();
        QString tag_search_index_db_path = tag_search_dir_path + "index_tag_search.sqlite";

        QString select_cmd = "SELECT tag_db_name FROM tag_search_index";
        QStringList tag_db_names = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmd,0,tag_search_index_db_path,Q_FUNC_INFO);

        for(int ii = 0; ii < tag_db_names.size(); ii++)
        {
            QString tag_db_name_path = tag_search_dir_path + tag_db_names.at(ii) + ".sqlite";

            recon_helper_standard_obj->execute_db_command_by_dbpath(command_email,tag_db_name_path,Q_FUNC_INFO);

            recon_helper_standard_obj->execute_db_command_by_dbpath(command_email_tag,tag_db_name_path,Q_FUNC_INFO);
        }

        //======Update Email Tag Search Database End===========//
        old_emlx_dir.removeRecursively();
    }

    //======Update EMlX MAIl DIR RENAME AND DATABASE END===========//


    //======Update RAW EMlX MAIl DIR RENAME START===========//
    QString old_exp_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString() + "EMLX_Mail_Tag/";
    QString new_email_exp_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString();
    QDir old_emlx_tag_dir(old_exp_dir_path);
    if(QFile::exists(old_exp_dir_path + "emlx_mail_tag_data.sqlite" ))
    {
        QFile::rename(old_exp_dir_path + "emlx_mail_tag_data.sqlite",old_exp_dir_path + "email_tag_data.sqlite");
        recon_static_functions::copy_directory(old_exp_dir_path,new_email_exp_dir_path,Q_FUNC_INFO);
    }

    old_emlx_tag_dir.removeRecursively();
    //======Update RAW EMlX MAIl DIR RENAME END===========//


    //========CSV Update-Starts===========//
    QString csv_dir_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString() + "log_viewer_csv";
    QString csv_dir_in_app_bundle = "../Resources/CSVS/Features_CSV/log_viewer_csv";

    recon_static_functions::copy_directory(csv_dir_in_app_bundle, csv_dir_in_result, Q_FUNC_INFO);
    //========CSV Update-END===========//

    //================Log Viewer db creation START ===================//

    QString log_viewer_sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString();

    QString log_view_db_path = log_viewer_sqlite_path + "log_viewer.sqlite";
    if(!QFile::exists(log_view_db_path))
    {
        QString command_log_view = " create table 'log_viewer_index' (INT INTEGER PRIMARY KEY, viewer_display_name varchar(30),"
                                   " db_table_name varchar(30),file_name varchar(30),file_path varchar(30), log_file_data BLOB, recon_filefrom varchar(30),recon_file_infopath varchar(30),plugin_name varchar(30),"
                                   " category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_log_view,log_view_db_path,Q_FUNC_INFO);
    }
    //================Log Viewer db creation END ===================//


    //======Copy Data from google_mail.sqlite to email_data.sqlite START=======//

    QString old_google_mail_feature_db_path = Lab_featr_path + QString("Google_Mail/") + QString("google_mail.sqlite");
    if(QFileInfo::exists(old_google_mail_feature_db_path))
    {
        QString new_email_feature_db_path = Lab_featr_path +  QString("Email_Parser/") + QString("email_data.sqlite");

        QString connection_naam = Q_FUNC_INFO;
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db.setDatabaseName(new_email_feature_db_path);
        if(destination_db.open())
        {
            QSqlQuery dest_query(destination_db);
            QString db_attach_cmd = QString("ATTACH DATABASE '%1' AS google_mailDB")
                    .arg(QDir::toNativeSeparators(old_google_mail_feature_db_path));

            if(dest_query.exec(db_attach_cmd))
            {
                QString command = "INSERT INTO main.emlx_data (bookmark,INT,emlx_subject,emlx_to,emlx_from,emlx_date,emlx_cc,"
                                  "emlx_msg_body,message_text,email_attachment,account_id,mbox,"
                                  "bookmarked_extended_attributes,bookmarked_exif_data,extended_attributes_key_value,"
                                  "extended_attributes_value_for_search,LINKFILENAME,preview_file_path,export_file_paths,"
                                  "recon_tag_value,notes_icon,tags_icon,os_scheme_display,source_count_name,source_file,notes) "
                                  "SELECT bookmark,INT,emlx_subject,emlx_to,emlx_from,emlx_date,emlx_cc,emlx_msg_body,message_text,"
                                  "email_attachment,account_id,mbox,bookmarked_extended_attributes,"
                                  "bookmarked_exif_data,extended_attributes_key_value,extended_attributes_value_for_search,"
                                  "LINKFILENAME,preview_file_path,export_file_paths,recon_tag_value,notes_icon,tags_icon,"
                                  "os_scheme_display,source_count_name,source_file,notes FROM google_mailDB.emlx_data";

                recon_helper_standard_obj->execute_db_command_by_dbreference(command,destination_db,Q_FUNC_INFO);
            }

            QString dettach_cmd = QString("DETACH DATABASE google_mailDB");
            dest_query.exec(dettach_cmd);
            destination_db.close();
        }

    }
    //======Copy Data from google_mail.sqlite to email_data.sqlite END=========//
}
