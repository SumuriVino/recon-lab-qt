#include "z_mainwindow/mainwindow.h"


void MainWindow::update_result_necessity_for_configuration()
{
    recon_static_functions::debug_intensive(" start ", Q_FUNC_INFO);

    if(!bool_need_to_update_result_for_load_case)
        return;

    QString version_number_of_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString().trimmed();

    if(version_number_of_result.isEmpty())
        return;


    if(version_number_of_result == "1.0.6" || version_number_of_result == "1.0.7" || version_number_of_result == "1.0.8" || version_number_of_result == "1.0.8.1"
            || version_number_of_result == "1.0.9" || version_number_of_result == "1.1.0" || version_number_of_result == "1.1.1"
            || version_number_of_result == "1.1.2" || version_number_of_result == "1.1.3" || version_number_of_result == "1.1.4"
            || version_number_of_result == "1.1.5" || version_number_of_result == "1.1.6" || version_number_of_result == "1.1.7"
            || version_number_of_result == "1.1.8" || version_number_of_result == "1.1.9" || version_number_of_result == "1.2.0"
            || version_number_of_result == "1.2.1" || version_number_of_result == "1.2.2" || version_number_of_result == "1.2.3"
            || version_number_of_result == "1.2.4" || version_number_of_result == "1.2.5" || version_number_of_result == "1.2.6"
            || version_number_of_result == "1.2.7" || version_number_of_result == "1.2.8" || version_number_of_result == "1.2.9"
            || version_number_of_result == "1.3.0" || version_number_of_result == "1.3.1" || version_number_of_result == "1.3.2"
            || version_number_of_result == "1.3.3" || version_number_of_result == "1.3.4" || version_number_of_result == "1.3.5")
    {
        update_configuration_106_to_136();
        update_configuration_136_to_139();
        update_configuration_139_to_142();
        update_configuration_142_to_143();
        update_configuration_143_to_148();
        update_configuration_148_to_150();
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();
    }
    else if(version_number_of_result == "1.3.6" || version_number_of_result == "1.3.7" || version_number_of_result == "1.3.8")
    {
        update_configuration_136_to_139();
        update_configuration_139_to_142();
        update_configuration_142_to_143();
        update_configuration_143_to_148();
        update_configuration_148_to_150();
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();
    }
    else if(version_number_of_result == "1.3.9" || version_number_of_result == "1.4.0" || version_number_of_result == "1.4.1")
    {
        update_configuration_139_to_142();
        update_configuration_142_to_143();
        update_configuration_143_to_148();
        update_configuration_148_to_150();
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();

    }
    else if(version_number_of_result == "1.4.2")
    {
        update_configuration_142_to_143();
        update_configuration_143_to_148();
        update_configuration_148_to_150();
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();

    }
    else if(version_number_of_result == "1.4.3" || version_number_of_result == "1.4.4" || version_number_of_result == "1.4.5"
            || version_number_of_result == "1.4.6" || version_number_of_result == "1.4.7")
    {
        update_configuration_143_to_148();
        update_configuration_148_to_150();
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();

    }
    else if(version_number_of_result == "1.4.8" || version_number_of_result == "1.4.9")
    {
        update_configuration_148_to_150();
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();
    }
    else if(version_number_of_result == "1.5.0" || version_number_of_result == "1.5.1")
    {
        update_configuration_150_to_152();
        update_configuration_152_to_155();
        update_configuration_155_to_158();
    }
    else if(version_number_of_result == "1.5.2" || version_number_of_result == "1.5.3" || version_number_of_result == "1.5.4")
    {
        update_configuration_152_to_155();
        update_configuration_155_to_158();
    }
    else if(version_number_of_result == "1.5.5" || version_number_of_result == "1.5.6" || version_number_of_result == "1.5.7")
    {
        update_configuration_155_to_158();
    }

    recon_static_functions::debug_intensive(" end ", Q_FUNC_INFO);

}


void MainWindow::update_configuration_136_to_139()
{
    QString config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString() + QString("Case_Configuration/case_configuration.sqlite");

    QString cmd_preferences = "CREATE TABLE tbl_preferences (key varchar(500), value varchar(500))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_preferences, config_db_path, Q_FUNC_INFO);

    QString command = "insert into tbl_preferences (key, value) values (?,?)";
    QStringList args_list;
    args_list << QString("MACRO_NARAD_Preferences_User_Opened_Files_bool") << QString::number(0);

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, args_list, config_db_path, Q_FUNC_INFO);


}

void MainWindow::update_configuration_106_to_136()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString case_confg_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString result_dir_CSVS_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString();
    QString cmd_add_column = "ALTER TABLE plugin_tabs ADD COLUMN supported_os_list";
    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_add_column,case_confg_db_path,Q_FUNC_INFO);

    QString select_cmnd = "select name from plugins";
    QStringList extracted_plugin_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(select_cmnd,0,case_confg_db_path,Q_FUNC_INFO);

    csv_reader *csv_rdr_obj = new csv_reader(this);
    csv_rdr_obj->set_csv_dir_path(result_dir_CSVS_path);
    csv_rdr_obj->do_job_plugin_all();

    for(int ii = 0; ii < extracted_plugin_name_list.size(); ii++)
    {
        int plugin_count = ii + 1;

        QString plugin_name = extracted_plugin_name_list.at(ii);
        QString select_plugin_no = "select name from plugin_tabs where plugin_no = ?";
        QStringList rslt_cnfg_tab_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_plugin_no,QStringList(QString::number(plugin_count)),0,case_confg_db_path,Q_FUNC_INFO);

        QStringList rslt_csv_tab_list = csv_rdr_obj->get_total_tab_list(plugin_name);
        for(int ii = 0; ii < rslt_csv_tab_list.size(); ii++)
        {
            QString final_tabs = rslt_csv_tab_list.at(ii);
            if(rslt_cnfg_tab_list.contains(final_tabs))
            {
                QStringList tab_supported_os_list = csv_rdr_obj->get_supported_os_scheme_list_of_tab(plugin_name,final_tabs);
                QString tab_supported_str = tab_supported_os_list.join(MACRO_RECON_Splitter_5_resolution_scope);
                QString command_tabs = QString("Update plugin_tabs Set supported_os_list = ? Where plugin_no = ? AND name = ?");
                QStringList value_list;
                value_list << tab_supported_str << QString::number(plugin_count) << final_tabs;
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_tabs,value_list,case_confg_db_path,Q_FUNC_INFO);
            }
        }
    }
    //------------------------------------------------Update case_configuration_database END--------------------------------------------------//


    //=======================================Replace Splitter in Plugin Tab START=====================================//

    for(int itr = 0; itr < extracted_plugin_name_list.size(); itr++)
    {
        QString plugin_name = extracted_plugin_name_list.at(itr);

        QString select_native_os_cmd = "select native_os_list from plugins where name = ?";
        QString native_os = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_native_os_cmd,QStringList(plugin_name),0,case_confg_db_path,Q_FUNC_INFO);
        if(native_os.contains(MACRO_RECON_Splitter_1_multiple))
        {
            native_os.replace(MACRO_RECON_Splitter_1_multiple,MACRO_RECON_Splitter_5_resolution_scope);
        }

        QString select_supported_os_cmd = "select supported_os_list from plugins where name = ?";
        QString supported_os_list = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_supported_os_cmd,QStringList(plugin_name),0,case_confg_db_path,Q_FUNC_INFO);
        if(supported_os_list.contains(MACRO_RECON_Splitter_1_multiple))
        {
            supported_os_list.replace(MACRO_RECON_Splitter_1_multiple,MACRO_RECON_Splitter_5_resolution_scope);
        }

        QString update_cmd  = "Update plugins SET native_os_list = ?, supported_os_list = ? where name = ?";
        QStringList val_list;
        val_list << native_os  << supported_os_list << plugin_name;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd,val_list,case_confg_db_path,Q_FUNC_INFO);
    }
    //=======================================Replace Splitter in Plugin Tab END=====================================//



    recon_static_functions::app_debug("End",Q_FUNC_INFO);


}
void MainWindow::update_configuration_139_to_142()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString() + QString("Case_Configuration/case_configuration.sqlite");

    QString examiner_selected_timezone_str_cmd = "ALTER TABLE timezone_details RENAME COLUMN user_selected_timezone_qstring to examiner_selected_timezone_qstring";
    recon_helper_standard_obj->execute_db_command_by_dbpath(examiner_selected_timezone_str_cmd,config_db_path,Q_FUNC_INFO);

    QString examiner_selected_timezone_qint64_cmd = "ALTER TABLE timezone_details RENAME COLUMN user_selected_timezone_qint64 to examiner_selected_timezone_qint64";
    recon_helper_standard_obj->execute_db_command_by_dbpath(examiner_selected_timezone_qint64_cmd,config_db_path,Q_FUNC_INFO);

    QString examiner_selected_date_format_str_cmd = "ALTER TABLE timezone_details RENAME COLUMN selected_date_format_qstring to examiner_selected_date_format_qstring";
    recon_helper_standard_obj->execute_db_command_by_dbpath(examiner_selected_date_format_str_cmd,config_db_path,Q_FUNC_INFO);

    QString examiner_selected_date_format_qint64_cmd = "ALTER TABLE timezone_details RENAME COLUMN selected_date_format_qint64 to examiner_selected_date_format_int";
    recon_helper_standard_obj->execute_db_command_by_dbpath(examiner_selected_date_format_qint64_cmd,config_db_path,Q_FUNC_INFO);

    QString supported_os_scheme_display_list_plugins_tab_cmd = "ALTER TABLE plugin_tabs RENAME COLUMN supported_os_list to supported_os_scheme_display_list";
    recon_helper_standard_obj->execute_db_command_by_dbpath(supported_os_scheme_display_list_plugins_tab_cmd,config_db_path,Q_FUNC_INFO);

    QString table_name_rename_cmd = "ALTER TABLE plugins RENAME TO m_plugins";
    recon_helper_standard_obj->execute_db_command_by_dbpath(table_name_rename_cmd,config_db_path,Q_FUNC_INFO);

    QString create_new_table_command = "CREATE TABLE plugins (INT INTEGER PRIMARY KEY,category varchar(100),name VARCHAR(100),export_available varchar(100),is_plugin_run varchar(100),is_supported varchar(100),default_selected varchar(100),supported_os_scheme_display_list varchar(100) )";
    recon_helper_standard_obj->execute_db_command_by_dbpath(create_new_table_command,config_db_path,Q_FUNC_INFO);

    QString inster_cmd = "INSERT INTO plugins (INT,category,name,export_available,is_plugin_run,is_supported,default_selected,supported_os_scheme_display_list)  SELECT INT,category,name,export_available,is_plugin_run,is_supported,default_selected,supported_os_list from m_plugins";
    recon_helper_standard_obj->execute_db_command_by_dbpath(inster_cmd,config_db_path,Q_FUNC_INFO);

    QString drop_table_cmd = "DROP TABLE m_plugins";
    recon_helper_standard_obj->execute_db_command_by_dbpath(drop_table_cmd,config_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void MainWindow::update_configuration_142_to_143()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString destination_db_file =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ---FAILED---",Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db_file ,Q_FUNC_INFO);
    }

    QSqlQuery query_timezone_details(destination_db);
    if(!query_timezone_details.exec("select case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,examiner_selected_timezone_qstring,examiner_selected_timezone_qint64,examiner_selected_date_format_int,examiner_selected_date_format_qstring from timezone_details"))
    {
        recon_static_functions::app_debug(" timezone query_timezone_details ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_timezone_details.lastError().text(),Q_FUNC_INFO);
    }

    if(query_timezone_details.exec() && !query_timezone_details.next())
    {
        QStringList val_list;

        val_list << "" << "" << "UTC +0000" << "0" << "0" << "yyyy-MMM-dd";

        QString command_timezone_details = QString("insert into 'timezone_details' (case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,"
                                                   "examiner_selected_timezone_qstring,examiner_selected_timezone_qint64,examiner_selected_date_format_int,"
                                                   "examiner_selected_date_format_qstring) values(?,?,?,?,?,?)");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_timezone_details,val_list,destination_db,Q_FUNC_INFO);

    }

    destination_db.close();


    recon_static_functions::app_debug("End", Q_FUNC_INFO);
}


void MainWindow::update_configuration_143_to_148()
{
    QString config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString() + QString("Case_Configuration/case_configuration.sqlite");

    QString select_cmd = QString("SELECT value from tbl_preferences where key = ?");
    QStringList args_list;
    args_list << QString("MACRO_NARAD_Preferences_User_Opened_Files_bool");

    QString value = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, config_db_path , Q_FUNC_INFO);

    if(value == QString("1"))
    {
        value = QString("TRUE");
    }
    else
    {
        value = QString("FALSE");
    }


    QString update_cmd = "UPDATE tbl_preferences SET key = ? , value = ? where key = ?";
    args_list.clear();
    args_list << QString("Highlight User Opened Files") << value << QString("MACRO_NARAD_Preferences_User_Opened_Files_bool");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, config_db_path, Q_FUNC_INFO);


}
void MainWindow::update_configuration_148_to_150()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString rslt_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString() + QString("Case_Configuration/case_configuration.sqlite");
    QString new_tag = QString("SkinTone");

    QString cmd_select = "Select selected_color, tag_name from tags";
    QStringList tag_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_select, 1, rslt_config_db_path, Q_FUNC_INFO);

    if(!tag_list.contains(new_tag))
    {
        QString cmd_insert_tag = " INSERT into tags (tag_name, selected_colour, is_default_tag, color_hex) VALUES(?,'Skin', '1', '#ECBCB4')";
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd_insert_tag, QStringList(new_tag), rslt_config_db_path, Q_FUNC_INFO);
    }

    QString result_dir_tag_icon_in_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + "icons/tag_icons";
    QString result_dir_tag_icon_in_app_bundle = "../icons/tag_icons";
    recon_static_functions::do_directory_blank_completely(result_dir_tag_icon_in_result, Q_FUNC_INFO);
    recon_static_functions::remove_safely_blank_dir_OR_file(result_dir_tag_icon_in_result, Q_FUNC_INFO);
    recon_static_functions::copy_directory(result_dir_tag_icon_in_app_bundle, result_dir_tag_icon_in_result, Q_FUNC_INFO);


    QString src_lib_config_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    QString m_key_nd = QString("Minimum File Size For Skin Tone Detection in KB");

    QString m_key_face = QString("Minimum File Size For Face Analysis in KB");
    QString m_key_ocr = QString("Minimum File Size For Optical Character Recognition in KB");
    QString m_key_text = QString("Maximum File Size For Text View in MB");
    QString m_key_weapons = QString("Minimum File Size For Weapons in KB");
    QString m_key_fire = QString("Minimum File Size For Fire Analysis in KB");

    QString select_cmd = QString("SELECT value from tbl_preferences where key = ?");
    QStringList args_list;

    args_list << m_key_nd;
    QString m_value_nd = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, src_lib_config_db_file , Q_FUNC_INFO);

    args_list.clear();
    args_list << m_key_face;
    QString m_value_face = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, src_lib_config_db_file , Q_FUNC_INFO);

    args_list.clear();
    args_list << m_key_ocr;
    QString m_value_ocr = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, src_lib_config_db_file , Q_FUNC_INFO);

    args_list.clear();
    args_list << m_key_text;
    QString m_value_text = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, src_lib_config_db_file , Q_FUNC_INFO);

    args_list.clear();
    args_list << m_key_weapons;
    QString m_value_weapons = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, src_lib_config_db_file , Q_FUNC_INFO);

    args_list.clear();
    args_list << m_key_fire;
    QString m_value_fire = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd , args_list , 0, src_lib_config_db_file , Q_FUNC_INFO);

    QString cmd_slct_key = " SELECT key from tbl_preferences";
    QStringList result_key_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd_slct_key, 0, rslt_config_db_path, Q_FUNC_INFO);

    QString update_cmd = "INSERT into tbl_preferences (key ,value) values(?,?)";

    if(!result_key_list.contains(m_key_nd))
    {
        args_list.clear();
        args_list << m_key_nd << m_value_nd;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, rslt_config_db_path, Q_FUNC_INFO);
    }

    if(!result_key_list.contains(m_key_face))
    {
        args_list.clear();
        args_list << m_key_face << m_value_face;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, rslt_config_db_path, Q_FUNC_INFO);
    }

    if(!result_key_list.contains(m_key_ocr))
    {
        args_list.clear();
        args_list << m_key_ocr << m_value_ocr;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, rslt_config_db_path, Q_FUNC_INFO);
    }

    if(!result_key_list.contains(m_key_text))
    {
        args_list.clear();
        args_list << m_key_text << m_value_text;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, rslt_config_db_path, Q_FUNC_INFO);
    }
    if(!result_key_list.contains(m_key_weapons))
    {
        args_list.clear();
        args_list << m_key_weapons << m_value_weapons;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, rslt_config_db_path, Q_FUNC_INFO);
    }

    if(!result_key_list.contains(m_key_fire))
    {
        args_list.clear();
        args_list << m_key_fire << m_value_fire;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_cmd, args_list, rslt_config_db_path, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}


void MainWindow::update_configuration_150_to_152()
{
    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    source_db.setDatabaseName(case_conf_db_path);

    if(source_db.open())
    {

        //=======================Updation_in_plugins_table_Starts============================

        QSqlQuery query_select(source_db);
        QString cmd_tbl_plugins = "Select name, supported_os_scheme_display_list from plugins";

        if(query_select.exec(cmd_tbl_plugins))
        {
            while(query_select.next())
            {
                QString plgn_name = query_select.value("name").toString();
                QString plgn_os_scheme = query_select.value("supported_os_scheme_display_list").toString();

                if(plgn_os_scheme.contains(";++::++;"))
                {
                    plgn_os_scheme = plgn_os_scheme.replace(";++::++;","::");
                }
                else
                {
                    continue;
                }

                QString cmd_plgn_updt_plugn = "UPDATE plugins SET supported_os_scheme_display_list = '" + plgn_os_scheme
                        + "' WHERE name = '" + plgn_name + "'";

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_plgn_updt_plugn, source_db, Q_FUNC_INFO);
            }
        }
        else
        {
            recon_static_functions::app_debug(" select query ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(" select query ----executedQuery---- " + query_select.executedQuery() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:- " + query_select.lastError().text() , Q_FUNC_INFO);
        }


        //=======================Updation_in_plugins_table_Ends===============================

        //=======================Updation_in_plugin_tabs_table_Starts=========================
        query_select.clear();

        QString cmd_tbl_plugins_tabs = "Select name, supported_os_scheme_display_list from plugin_tabs";

        if(query_select.exec(cmd_tbl_plugins_tabs))
        {
            while(query_select.next())
            {
                QString plgn_tabs_name = query_select.value("name").toString();
                QString plgn_tabs_os_scheme = query_select.value("supported_os_scheme_display_list").toString();

                if(plgn_tabs_os_scheme.contains(";++::++;"))
                {
                    plgn_tabs_os_scheme = plgn_tabs_os_scheme.replace(";++::++;","::");
                }
                else
                {
                    continue;
                }

                QString cmd_plgn_updt_plugn = "UPDATE plugin_tabs SET supported_os_scheme_display_list = '" + plgn_tabs_os_scheme
                        + "' WHERE name = '" + plgn_tabs_name + "'";

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_plgn_updt_plugn, source_db, Q_FUNC_INFO);
            }
        }
        else
        {
            recon_static_functions::app_debug(" select query ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(" select query ----executedQuery---- " + query_select.executedQuery() , Q_FUNC_INFO);
            recon_static_functions::app_debug("Error:- " + query_select.lastError().text() , Q_FUNC_INFO);
        }


        //=======================Updation_in_plugin_tabs_table_Ends=========================

        source_db.close();
    }
    else
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:- " + source_db.lastError().text() , Q_FUNC_INFO);
    }

}

void MainWindow::update_configuration_152_to_155()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString rslt_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    QString cmd_filename_category = "CREATE TABLE filename_category (INT INTEGER PRIMARY KEY,category VARCHAR(100),category_status varchar(100))";
    QString cmd_filename_list     = "CREATE TABLE filename_list (INT INTEGER PRIMARY KEY,category VARCHAR(100),filename varchar(100),filename_status varchar(100))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_filename_category,rslt_config_db_path,Q_FUNC_INFO);
    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_filename_list,rslt_config_db_path,Q_FUNC_INFO);

    QString lib_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    dest_db.setDatabaseName(rslt_config_db_path);
    if(dest_db.open())
    {
        QSqlQuery query(dest_db);
        QString db_attach_commmand = QString("ATTACH DATABASE '%1' AS libconfigDB")
                .arg(QDir::toNativeSeparators(lib_config_db_path));
        if(query.exec(db_attach_commmand))
        {

            query.clear();
            query.prepare("insert into main.filename_category Select * from libconfigDB.filename_category");
            query.exec();

            query.clear();
            query.prepare("insert into main.filename_list Select * from libconfigDB.filename_list");
            query.exec();

            QString db_dettach_commmand = QString("DETACH DATABASE libconfigDB");
            query.exec(db_dettach_commmand);

        }
        dest_db.close();
    }

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_configuration_155_to_158()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString case_result_configuration_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString("case_configuration.sqlite");

    QString cmd = "SELECT name from plugins where category != ? AND category != ? AND category != ?";
    QStringList values;
    values << "Email Artifacts" << "Messenger" << "Web Browsers";
    QStringList other_artifacts_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(cmd, values, 0, case_result_configuration_db, Q_FUNC_INFO);
    fill_export_case_status_database("Other Artifacts", other_artifacts_list);


    QString update_result_config_db_command =  QString("Update tbl_apple_attributes Set bookmark_filter = 1");
    recon_helper_standard_obj->execute_db_command_by_dbpath(update_result_config_db_command, case_result_configuration_db, Q_FUNC_INFO);

    recon_static_functions::app_debug("End", Q_FUNC_INFO);

}
