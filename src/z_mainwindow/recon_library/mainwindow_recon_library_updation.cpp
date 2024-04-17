#include "z_mainwindow/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::update_currently_created_configurations_with_previous_config(QString current_config_dir_path , QString last_latest_config_dir_path)
{
    recon_static_functions::app_debug("MAHATMA BUDHA -----CONFIG---- Start",Q_FUNC_INFO);

    if(current_config_dir_path.endsWith(QString("/")))
        current_config_dir_path.chop(1);

    if(last_latest_config_dir_path.endsWith(QString("/")))
        last_latest_config_dir_path.chop(1);


    current_config_dir_path  = current_config_dir_path.trimmed();
    last_latest_config_dir_path = last_latest_config_dir_path.trimmed();


    if(!current_config_dir_path.isEmpty() && !current_config_dir_path.startsWith(QString("/")))
        current_config_dir_path.prepend(QString("/"));

    if(!last_latest_config_dir_path.isEmpty() && !last_latest_config_dir_path.startsWith(QString("/")))
        last_latest_config_dir_path.prepend(QString("/"));

    QString tmp_previous_dir_path = last_latest_config_dir_path;
    QString previous_config_version = tmp_previous_dir_path.remove(0,tmp_previous_dir_path.lastIndexOf("/"));
    previous_config_version.remove("/");
    previous_config_version.replace("_" , ".");

    if(current_config_dir_path == last_latest_config_dir_path)
    {

        /*
         * if configurations are being updated for same version
         * then copy temprarily saved configuration to the current.
         */

        //remove currently created configuration dir
        recon_static_functions::do_directory_blank_completely(current_config_dir_path , Q_FUNC_INFO);

        //copy previous configurations for the same version
        QString tmp_saved_config_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString()
                + QString("tmp/") + recon_static_functions::get_recon_library_version() + QString("/");

        recon_static_functions::copy_directory(tmp_saved_config_dir_path , current_config_dir_path , Q_FUNC_INFO);

    }
    else
    {

        /*
         * if configurations are being updated for different version
         */

        QString current_config_db_path  = current_config_dir_path  + QString("/recon_configuration/recon_configuration.sqlite");
        QString previous_config_db_path = last_latest_config_dir_path + QString("/recon_configuration/recon_configuration.sqlite");


        if(!QFileInfo(previous_config_db_path).exists())
            return;


        QString connection_naam = QString(Q_FUNC_INFO) + QString("1");
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase current_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        current_config_db.setDatabaseName(current_config_db_path);
        if(!current_config_db.open())
        {
            recon_static_functions::app_debug("Current Db opening recon configuration --------FAILED------ " + current_config_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + current_config_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        connection_naam = QString(Q_FUNC_INFO) + QString("2");
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase previous_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        previous_config_db.setDatabaseName(previous_config_db_path);
        if(!previous_config_db.open())
        {
            recon_static_functions::app_debug("Previous Db opening recon configuration --------FAILED------ " + current_config_db_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + previous_config_db.lastError().text(),Q_FUNC_INFO);
            return;
        }


        ///====================Update Agency Details====================///

        QStringList tmp_blank_list;
        QString select_command_str = QString("Select agency_name , agency_address , examiner, examiner_phone, examiner_email, date_format From agency_details");
        QString insert_or_update_command_str = QString("Insert into agency_details(agency_name,agency_address,examiner,examiner_phone,examiner_email,date_format) values(?,?,?,?,?,?)");

        QString clear_dest_command = QString("delete from agency_details");
        recon_helper_standard_obj->execute_db_command_by_dbreference(clear_dest_command , current_config_db , Q_FUNC_INFO);

        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_agency_details,tmp_blank_list);

        ///====================Update Case Result Info====================///

        select_command_str = QString("Select INT, name, result_path From case_result_info");
        insert_or_update_command_str = QString("Insert into case_result_info(INT,name,result_path) values(?,?,?)");

        clear_dest_command = QString("delete from case_result_info");
        recon_helper_standard_obj->execute_db_command_by_dbreference(clear_dest_command , current_config_db , Q_FUNC_INFO);

        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_case_result_info,tmp_blank_list);


        ///====================Update Extended Attributes====================///

        select_command_str = QString("Select bookmark_filter,attribute_name, description, is_default, widget_type, metadata_attribute_name, data_type, custom_column_name,bookmark_metadata,bookmark_report"
                                     "  From tbl_apple_attributes Where (bookmark_filter='1' OR bookmark_metadata='1' OR bookmark_report='1')");

        insert_or_update_command_str =  QString("Update tbl_apple_attributes Set bookmark_filter=? , attribute_name=?, description=?, is_default=?, widget_type=?, data_type=?, custom_column_name=?, bookmark_metadata = ?, bookmark_report = ? Where metadata_attribute_name=?");

        //do not delete entries from current db update it from previous db
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_apple_attributes,tmp_blank_list);
        ///====================Update Extension Category====================///

        ///========
        //        select_command_str = QString("Select bookmark_filter,is_default,attribute_name, description, bookmark_metadata,bookmark_report"
        //                                     "  From tbl_exif_attributes Where (bookmark_filter='1' OR bookmark_metadata='1' OR bookmark_report='1') AND is_default='0'");

        //        insert_or_update_command_str =  QString("Update tbl_exif_attributes Set bookmark_filter=?, is_default=?, attribute_name=?, description=?,  bookmark_metadata = ?, bookmark_report = ? Where attribute_name=?");
        //        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_apple_attributes,tmp_blank_list);

        /// =======

        //Update existing First
        select_command_str = QString("Select bookmark, category_name, total_records From extension_category");
        insert_or_update_command_str = QString("Update extension_category Set bookmark=? , total_records=? Where category_name=?");

        //do not delete entries from current db update it from previous db
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_extension_category,tmp_blank_list);

        // insert user created
        select_command_str = QString("Select category_name From extension_category");
        QStringList all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select bookmark, category_name, total_records From extension_category");
        insert_or_update_command_str = QString("Insert into extension_category(bookmark,category_name,total_records) values(?,?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_extension_category,all_category_name_list);


        ///====================Update Extension List====================///
        // insert user created
        recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From extension_list"), current_config_db, Q_FUNC_INFO);
        all_category_name_list.clear();
        //        select_command_str = QString("Select category_name From extension_list");
        //        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select bookmark, category_name, extension_name From extension_list");
        insert_or_update_command_str = QString("Insert into extension_list(bookmark,category_name,extension_name) values(?,?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_extension_list,all_category_name_list);

        ///====================Update Signature Category====================///
        select_command_str = QString("Select INT From tbl_signature_category");
        all_category_name_list.clear();
        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select INT,category,category_status From tbl_signature_category");
        insert_or_update_command_str = QString("Insert into tbl_signature_category(category,category_status) values(?,?)");

        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_signature_category,all_category_name_list);

        ///====================Update Signature List====================///
        all_category_name_list.clear();
        select_command_str = QString("Select header From tbl_signature_list");
        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select category, label, header, signature_status,offset From tbl_signature_list");
        insert_or_update_command_str = QString("Insert into tbl_signature_list(category, label, header, signature_status,offset) values(?,?,?,?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_signature_list,all_category_name_list);



        ///====================Insert Hashset====================///
        select_command_str = QString("Select bookmark,table_name,column_name,database_path,is_created_by_user From tbl_hashset");
        insert_or_update_command_str = QString("Insert into tbl_hashset(bookmark,table_name,column_name,database_path,is_created_by_user) values(?,?,?,?,?)");
        QString command_pragma = QString("PRAGMA TABLE_INFO( '") + QString("tbl_hashset") + "')";
        QStringList prev_db_colname_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(command_pragma, 1, previous_config_db, Q_FUNC_INFO);

        if(prev_db_colname_list.contains("recon_filefrom") && prev_db_colname_list.contains("recon_file_infopath"))
        {
            select_command_str = QString("Select bookmark,table_name,column_name,database_path,is_created_by_user,recon_filefrom,recon_file_infopath From tbl_hashset");
            insert_or_update_command_str = QString("Insert into tbl_hashset(bookmark,table_name,column_name,database_path,is_created_by_user,recon_filefrom,recon_file_infopath) values(?,?,?,?,? ,?,?)");
        }

        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_hashset,all_category_name_list);


        ///====================Update Index Files Whitelist====================///
        //Update existing First
        select_command_str = QString("Select bookmark, category From tbl_index_files_whitelist_category");
        insert_or_update_command_str = QString("Update tbl_index_files_whitelist_category Set bookmark=? Where category=?");

        //do not delete entries from current db update it from previous db
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_index_files_whitelist_category,tmp_blank_list);

        // insert user created
        all_category_name_list.clear();
        select_command_str = QString("Select category From tbl_index_files_whitelist_category");
        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select bookmark, category From tbl_index_files_whitelist_category");
        insert_or_update_command_str = QString("Insert into tbl_index_files_whitelist_category(bookmark,category) values(?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_index_files_whitelist_category,all_category_name_list);


        ///====================Update Index Files Blacklist====================///
        //Update existing First
        select_command_str = QString("Select bookmark, category From tbl_index_files_blacklist_category");
        insert_or_update_command_str = QString("Update tbl_index_files_blacklist_category Set bookmark=? Where category=?");

        //do not delete entries from current db update it from previous db
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_index_files_blacklist_category,tmp_blank_list);

        // insert user created
        all_category_name_list.clear();
        select_command_str = QString("Select category From tbl_index_files_blacklist_category");
        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select bookmark, category From tbl_index_files_blacklist_category");
        insert_or_update_command_str = QString("Insert into tbl_index_files_blacklist_category(bookmark,category) values(?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_index_files_blacklist_category,all_category_name_list);


        ///====================Update Index Files Blacklist Extensions====================///

        recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From tbl_index_files_blacklist_extension"), current_config_db, Q_FUNC_INFO);
        all_category_name_list.clear();
        //select_command_str = QString("Select category From tbl_index_files_blacklist_extension");
        //all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);


        select_command_str = QString("Select bookmark,category,extension From tbl_index_files_blacklist_extension");
        insert_or_update_command_str = QString("Insert into tbl_index_files_blacklist_extension(bookmark,category,extension) values(?,?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_index_files_blacklist_extensions ,all_category_name_list);


        ///====================Update Index Files Whitelist Extensions====================///
        recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From tbl_index_files_whitelist_extension"), current_config_db, Q_FUNC_INFO);
        all_category_name_list.clear();
        //select_command_str = QString("Select category From tbl_index_files_whitelist_extension");
        //all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);


        select_command_str = QString("Select bookmark,category,extension From tbl_index_files_whitelist_extension");
        insert_or_update_command_str = QString("Insert into tbl_index_files_whitelist_extension(bookmark,category,extension) values(?,?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_index_files_whitelist_extensions,all_category_name_list);


        ///===================Insert Text Indexing Selected Filter====================///
        if(previous_config_db.tables().contains("tbl_selected_text_index_filter"))
        {
            QString command_str   = QString("CREATE TABLE IF NOT EXISTS tbl_selected_text_index_filter (INT integer primary key, select_status varchar(5), selected_filter varchar(10))");
            recon_helper_standard_obj->execute_db_command_by_dbreference(command_str, current_config_db, Q_FUNC_INFO);

            select_command_str = QString("Select select_status,selected_filter From tbl_selected_text_index_filter");
            insert_or_update_command_str = QString("Insert into tbl_selected_text_index_filter(select_status,selected_filter) values(?,?)");
            update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_selected_text_index_filter ,tmp_blank_list);
        }


        ///====================Update Exif Attributes====================///
        select_command_str = QString("Select bookmark_filter,bookmark_metadata,bookmark_report,attribute_key From tbl_exif_attributes Where (bookmark_filter='1' OR bookmark_metadata='1' OR bookmark_report='1')");
        insert_or_update_command_str =  QString("Update tbl_exif_attributes Set bookmark_filter=? , bookmark_metadata=?, bookmark_report=?  Where attribute_key=?");

        //do not delete entries from current db update it from previous db
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_exif_attributes,tmp_blank_list);


        ///===================Insert Volatility Path====================///
        select_command_str = QString("Select volatility_dir From tbl_volatility");
        insert_or_update_command_str = QString("Insert into tbl_volatility(volatility_dir) values(?)");

        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_volatility_path ,tmp_blank_list);

        ///===================Insert Extenral Applications====================///
        select_command_str = QString("Select application_name,application_path From tbl_external_applications");
        insert_or_update_command_str = QString("Insert into tbl_external_applications(application_name,application_path) values(?,?)");

        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_external_applications ,tmp_blank_list);

        recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From timezone_details"), current_config_db, Q_FUNC_INFO);
        select_command_str = get_select_query_for_timezone_details_table(previous_config_version);
        insert_or_update_command_str = QString("Insert into timezone_details(case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,examiner_selected_timezone_qstring,"
                                               "examiner_selected_timezone_qint64,examiner_selected_date_format_qstring,examiner_selected_date_format_int) values(?,?,?,?,?,?)");
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_timezone_details ,tmp_blank_list);


        ///====================Update Filetypes====================///

        //        //Update existing First
        //        select_command_str = QString("Select bookmark, category_name, extension_name,extension_type,signature_header,mime_type,extended_attribute From file_types");
        //        insert_or_update_command_str = QString("Update file_types Set bookmark=? , extension_name=?, signature_header=?, mime_type=? ,extended_attribute=?  Where category_name=? AND extension_type=?");


        //        //do not delete entries from current db update it from previous db
        //        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_filetypes,tmp_blank_list);

        //        // insert user created
        //        select_command_str = QString("Select category_name From file_types");
        //        all_category_name_list.clear();
        //        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        //        select_command_str = QString("Select bookmark, category_name, extension_name,extension_type,signature_header,mime_type,extended_attribute From file_types");
        //        insert_or_update_command_str = QString("Insert into file_types(bookmark, category_name, extension_name,extension_type,signature_header,mime_type,extended_attribute) values(?,?,?,?,?,?,?)");


        //        //do not delete entries from current because they are already updated firstly
        //        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_filetypes,all_category_name_list);

        ///====================Update Tags====================///

        //Update existing First
        select_command_str = QString("Select tag_name, selected_colour, is_default_tag, bookmark,color_hex From tags");
        insert_or_update_command_str = QString("Update tags Set selected_colour=?, is_default_tag=?, bookmark=?,color_hex=?  Where tag_name=?");

        //do not delete entries from current db update it from previous db
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_Tags,tmp_blank_list);

        // insert user created
        select_command_str = QString("Select tag_name From tags");
        all_category_name_list.clear();
        all_category_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(select_command_str , 0, current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select tag_name, selected_colour, is_default_tag, bookmark,color_hex From tags");
        insert_or_update_command_str = QString("Insert into tags(tag_name, selected_colour, is_default_tag, bookmark,color_hex) values(?,?,?,?,?)");
        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_Tags,all_category_name_list);

        select_command_str = get_select_query_for_plugin_template_table(previous_config_version);
        insert_or_update_command_str = QString("Insert into plugin_selection_template(template_name, plugin_name, selected_tabs_list,os_scheme_display) values(?,?,?,?)");
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_update_plugin_template,tmp_blank_list);



        ///====================Insert Keyword Category====================///
        recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From keyword_category"), current_config_db, Q_FUNC_INFO);
        select_command_str = QString("Select category_status,category From keyword_category");
        insert_or_update_command_str = QString("Insert into keyword_category(category_status,category) values(?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_keyword_search_category,all_category_name_list);

        ///====================Insert Keyword List====================///
        recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From keyword_list"), current_config_db, Q_FUNC_INFO);

        select_command_str = QString("Select category,keyword,keyword_status,is_regex From keyword_list");
        insert_or_update_command_str = QString("Insert into keyword_list(category,keyword,keyword_status,is_regex) values(?,?,?,?)");

        //do not delete entries from current because they are already updated firstly
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_keyword_search_keywords_list,all_category_name_list);


        ///===================Insert Preferences====================///
        select_command_str = get_select_query_for_tbl_preferences_table(previous_config_version);
        insert_or_update_command_str = QString("UPDATE tbl_preferences set value = ? where key = ?");
        update_current_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_recon_library_insert_into_preferences ,tmp_blank_list);

        current_config_db.close();
        previous_config_db.close();


    }

    recon_static_functions::app_debug("MAHATMA BUDHA -----CONFIG---- End",Q_FUNC_INFO);

}

QString MainWindow::get_select_query_for_plugin_template_table(QString previous_version_number_of_config)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);
    QString select_command_str;
    if(previous_version_number_of_config == "1.0.6" || previous_version_number_of_config == "1.0.7" || previous_version_number_of_config == "1.0.8" || previous_version_number_of_config == "1.0.8.1"
            || previous_version_number_of_config == "1.0.9" || previous_version_number_of_config == "1.1.0" || previous_version_number_of_config == "1.1.1"
            || previous_version_number_of_config == "1.1.2" || previous_version_number_of_config == "1.1.3" || previous_version_number_of_config == "1.1.4"
            || previous_version_number_of_config == "1.1.5" || previous_version_number_of_config == "1.1.6" || previous_version_number_of_config == "1.1.7"
            || previous_version_number_of_config == "1.1.8" || previous_version_number_of_config == "1.1.9" || previous_version_number_of_config == "1.2.0"
            || previous_version_number_of_config == "1.2.1" || previous_version_number_of_config == "1.2.2" || previous_version_number_of_config == "1.2.3"
            || previous_version_number_of_config == "1.2.4" || previous_version_number_of_config == "1.2.5" || previous_version_number_of_config == "1.2.6"
            || previous_version_number_of_config == "1.2.7" || previous_version_number_of_config == "1.2.8" || previous_version_number_of_config == "1.2.9"
            || previous_version_number_of_config == "1.3.0" || previous_version_number_of_config == "1.3.1" || previous_version_number_of_config == "1.3.2"
            || previous_version_number_of_config == "1.3.3" || previous_version_number_of_config == "1.3.4" || previous_version_number_of_config == "1.3.5"
            || previous_version_number_of_config == "1.3.6" || previous_version_number_of_config == "1.3.7" || previous_version_number_of_config == "1.3.8"
            || previous_version_number_of_config == "1.3.9" || previous_version_number_of_config == "1.4.0" || previous_version_number_of_config == "1.4.1")
    {
        select_command_str = QString("Select template_name, plugin_name, selected_tabs_list,os_scheme From plugin_selection_template");
    }
    else
    {
        select_command_str = QString("Select template_name, plugin_name, selected_tabs_list,os_scheme_display From plugin_selection_template");
    }
    ///====================Update Plugin Template====================///

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return select_command_str;

}

QString MainWindow::get_select_query_for_timezone_details_table(QString previous_version_number_of_config)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);
    QString select_command_str;
    if(previous_version_number_of_config == "1.0.6" || previous_version_number_of_config == "1.0.7" || previous_version_number_of_config == "1.0.8" || previous_version_number_of_config == "1.0.8.1"
            || previous_version_number_of_config == "1.0.9" || previous_version_number_of_config == "1.1.0" || previous_version_number_of_config == "1.1.1"
            || previous_version_number_of_config == "1.1.2" || previous_version_number_of_config == "1.1.3" || previous_version_number_of_config == "1.1.4"
            || previous_version_number_of_config == "1.1.5" || previous_version_number_of_config == "1.1.6" || previous_version_number_of_config == "1.1.7"
            || previous_version_number_of_config == "1.1.8" || previous_version_number_of_config == "1.1.9" || previous_version_number_of_config == "1.2.0"
            || previous_version_number_of_config == "1.2.1" || previous_version_number_of_config == "1.2.2" || previous_version_number_of_config == "1.2.3"
            || previous_version_number_of_config == "1.2.4" || previous_version_number_of_config == "1.2.5" || previous_version_number_of_config == "1.2.6"
            || previous_version_number_of_config == "1.2.7" || previous_version_number_of_config == "1.2.8" || previous_version_number_of_config == "1.2.9"
            || previous_version_number_of_config == "1.3.0" || previous_version_number_of_config == "1.3.1" || previous_version_number_of_config == "1.3.2"
            || previous_version_number_of_config == "1.3.3" || previous_version_number_of_config == "1.3.4" || previous_version_number_of_config == "1.3.5"
            || previous_version_number_of_config == "1.3.6" || previous_version_number_of_config == "1.3.7" || previous_version_number_of_config == "1.3.8"
            || previous_version_number_of_config == "1.3.9" || previous_version_number_of_config == "1.4.0" || previous_version_number_of_config == "1.4.1")
    {
        select_command_str = QString("Select case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,user_selected_timezone_qstring,"
                                     "user_selected_timezone_qint64,selected_date_format_qstring,selected_date_format_qint64 From timezone_details");
    }
    else
    {
        select_command_str = QString("Select case_created_machine_timezone_qstring, case_created_machine_timezone_qint64, examiner_selected_timezone_qstring,"
                                     " examiner_selected_timezone_qint64, examiner_selected_date_format_qstring,examiner_selected_date_format_int From timezone_details");
    }


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return select_command_str;

}

QString MainWindow::get_select_query_for_tbl_preferences_table(QString previous_version_number_of_config)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);
    QString select_command_str;
    if(previous_version_number_of_config == "1.0.6" || previous_version_number_of_config == "1.0.7" || previous_version_number_of_config == "1.0.8" || previous_version_number_of_config == "1.0.8.1"
            || previous_version_number_of_config == "1.0.9" || previous_version_number_of_config == "1.1.0" || previous_version_number_of_config == "1.1.1"
            || previous_version_number_of_config == "1.1.2" || previous_version_number_of_config == "1.1.3" || previous_version_number_of_config == "1.1.4"
            || previous_version_number_of_config == "1.1.5" || previous_version_number_of_config == "1.1.6" || previous_version_number_of_config == "1.1.7"
            || previous_version_number_of_config == "1.1.8" || previous_version_number_of_config == "1.1.9" || previous_version_number_of_config == "1.2.0"
            || previous_version_number_of_config == "1.2.1" || previous_version_number_of_config == "1.2.2" || previous_version_number_of_config == "1.2.3"
            || previous_version_number_of_config == "1.2.4" || previous_version_number_of_config == "1.2.5" || previous_version_number_of_config == "1.2.6"
            || previous_version_number_of_config == "1.2.7" || previous_version_number_of_config == "1.2.8" || previous_version_number_of_config == "1.2.9"
            || previous_version_number_of_config == "1.3.0" || previous_version_number_of_config == "1.3.1" || previous_version_number_of_config == "1.3.2"
            || previous_version_number_of_config == "1.3.3" || previous_version_number_of_config == "1.3.4" || previous_version_number_of_config == "1.3.5"
            || previous_version_number_of_config == "1.3.6" || previous_version_number_of_config == "1.3.7" || previous_version_number_of_config == "1.3.8")
    {
        select_command_str = "";
    }
    else
    {
        select_command_str = QString("Select key,value from tbl_preferences");
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);
    return select_command_str;

}

void MainWindow::update_currently_created_app_configurations_with_previous_app_config(QString current_config_dir_path , QString previous_config_dir_path)
{
    recon_static_functions::app_debug("-Start",Q_FUNC_INFO);

    if(current_config_dir_path.endsWith(QString("/")))
        current_config_dir_path.chop(1);

    if(previous_config_dir_path.endsWith(QString("/")))
        previous_config_dir_path.chop(1);


    current_config_dir_path  = current_config_dir_path.trimmed();
    previous_config_dir_path = previous_config_dir_path.trimmed();


    if(!current_config_dir_path.isEmpty() && !current_config_dir_path.startsWith(QString("/")))
        current_config_dir_path.prepend(QString("/"));

    if(!previous_config_dir_path.isEmpty() && !previous_config_dir_path.startsWith(QString("/")))
        previous_config_dir_path.prepend(QString("/"));

    QString tmp_previous_dir_path = previous_config_dir_path;
    QString previous_config_version = tmp_previous_dir_path.remove(0,tmp_previous_dir_path.lastIndexOf("/"));
    previous_config_version.remove("/");
    previous_config_version.replace("_" , ".");


    QString current_config_db_path  = current_config_dir_path  + QString("/app_configuration/app_configuration.sqlite");
    QString previous_config_db_path = previous_config_dir_path + QString("/app_configuration/app_configuration.sqlite");


    if(!QFileInfo(previous_config_db_path).exists())
        return;


    QString connection_naam = QString(Q_FUNC_INFO) + QString("1");
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase current_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    current_config_db.setDatabaseName(current_config_db_path);
    if(!current_config_db.open())
    {
        recon_static_functions::app_debug("Current Db opening app configuration --------FAILED------ " + current_config_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + current_config_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    connection_naam = QString(Q_FUNC_INFO) + QString("2");
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase previous_config_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    previous_config_db.setDatabaseName(previous_config_db_path);
    if(!previous_config_db.open())
    {
        recon_static_functions::app_debug("Previous Db opening app configuration --------FAILED------ " + current_config_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + previous_config_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QStringList tmp_blank_list;


    ///===================Insert tbl_preferences====================///
    recon_helper_standard_obj->execute_db_command_by_dbreference(QString("Delete From tbl_preferences"), current_config_db, Q_FUNC_INFO);
    QString select_command_str = get_select_query_for_app_config_preferences_table(previous_config_version);
    QString insert_or_update_command_str = QString("Insert into tbl_preferences(key,value) values(?,?)");
    update_current_app_config_from_previous_in_db(&current_config_db , &previous_config_db , select_command_str , insert_or_update_command_str , enum_mainwindow_app_configurations_preferences ,tmp_blank_list);
    ///===================Insert tbl_preferences====================///

    current_config_db.close();
    previous_config_db.close();

    recon_static_functions::app_debug("-End",Q_FUNC_INFO);

}


QString MainWindow::get_select_query_for_app_config_preferences_table(QString previous_version_number_of_config)
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);
    QString select_command_str;

    if(previous_version_number_of_config == "1.0.6" || previous_version_number_of_config == "1.0.7" || previous_version_number_of_config == "1.0.8" || previous_version_number_of_config == "1.0.8.1"
            || previous_version_number_of_config == "1.0.9" || previous_version_number_of_config == "1.1.0" || previous_version_number_of_config == "1.1.1"
            || previous_version_number_of_config == "1.1.2" || previous_version_number_of_config == "1.1.3" || previous_version_number_of_config == "1.1.4"
            || previous_version_number_of_config == "1.1.5" || previous_version_number_of_config == "1.1.6" || previous_version_number_of_config == "1.1.7"
            || previous_version_number_of_config == "1.1.8" || previous_version_number_of_config == "1.1.9" || previous_version_number_of_config == "1.2.0"
            || previous_version_number_of_config == "1.2.1" || previous_version_number_of_config == "1.2.2" || previous_version_number_of_config == "1.2.3"
            || previous_version_number_of_config == "1.2.4" || previous_version_number_of_config == "1.2.5" || previous_version_number_of_config == "1.2.6"
            || previous_version_number_of_config == "1.2.7" || previous_version_number_of_config == "1.2.8" || previous_version_number_of_config == "1.2.9"
            || previous_version_number_of_config == "1.3.0" || previous_version_number_of_config == "1.3.1" || previous_version_number_of_config == "1.3.2"
            || previous_version_number_of_config == "1.3.3" || previous_version_number_of_config == "1.3.4" || previous_version_number_of_config == "1.3.5"
            || previous_version_number_of_config == "1.3.6" || previous_version_number_of_config == "1.3.7" || previous_version_number_of_config == "1.3.8"
            || previous_version_number_of_config == "1.3.9" || previous_version_number_of_config == "1.4.0" || previous_version_number_of_config == "1.4.1"
            || previous_version_number_of_config == "1.4.2" || previous_version_number_of_config == "1.4.3" || previous_version_number_of_config == "1.4.4"
            || previous_version_number_of_config == "1.4.5" || previous_version_number_of_config == "1.4.6" || previous_version_number_of_config == "1.4.7")
    {
        select_command_str = "";
    }
    else
    {
        select_command_str = QString("Select key,value from tbl_preferences");
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

    return select_command_str;

}
