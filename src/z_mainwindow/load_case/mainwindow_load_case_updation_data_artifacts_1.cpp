#include "z_mainwindow/mainwindow.h"

void MainWindow::update_load_case_artifacts_plugin_related_config(QString plgn_name, csv_reader *csv_reader_local)
{
    update_load_case_artifacts_plugin_entry_config(plgn_name, csv_reader_local);

    update_load_case_artifacts_plugin_export_case_config_for_reader(plgn_name, csv_reader_local);

}

void MainWindow::update_load_case_artifacts_plugin_entry_config(QString plgn_name, csv_reader *csv_reader_local)
{
    recon_static_functions::app_debug("start " + plgn_name,Q_FUNC_INFO);

    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    if(!QFileInfo(case_conf_db_path).exists())
        return;

    if(QFileInfo(case_conf_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_conf_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        dest_conf_db.setDatabaseName(case_conf_db_path);
        if(!dest_conf_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening conf  ----FAILED--- " + dest_conf_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_conf_db.lastError().text(),Q_FUNC_INFO);
            return;
        }

        QString cmd_ext_plgn = "select name from plugins";
        QStringList existing_plugin_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(cmd_ext_plgn, 0, dest_conf_db, Q_FUNC_INFO);



        struct_global_csv_reader_full_csv_info csv_st_obj = csv_reader_local->read_csv_value(plgn_name);
        QString prnt_name = csv_st_obj.plugin_parent_name;

        QString plugin_export_available = "0";
        if(csv_reader_local->is_export_available_in_plugin(plgn_name))
            plugin_export_available = "1";


        QString supported_os_schme_dsply = csv_reader_local->get_supported_os_scheme_list_of_plugin(plgn_name).join(MACRO_RECON_Splitter_5_resolution_scope);

        QString command_plugin;
        if(!existing_plugin_name_list.contains(plgn_name))
        {
            command_plugin = QString("insert into plugins(category,name,export_available,default_selected,supported_os_scheme_display_list) values(?,?,?,?,?)");
            QStringList list_add_bind_value;
            list_add_bind_value << prnt_name << plgn_name << plugin_export_available << "0" << supported_os_schme_dsply ;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_plugin, list_add_bind_value, dest_conf_db, Q_FUNC_INFO);
        }
        else
        {
            QString cmd_plgn_updt_plugn = "update plugins set category = '" + prnt_name + "', "
                    + "supported_os_scheme_display_list='" + supported_os_schme_dsply + "', "
                    + "export_available='" + plugin_export_available + "' "
                    + "where name = '" + plgn_name + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_plgn_updt_plugn, dest_conf_db, Q_FUNC_INFO);

        }


        dest_conf_db.close();

    }


    recon_static_functions::app_debug(" end " + plgn_name,Q_FUNC_INFO);
}

void MainWindow::update_load_case_artifacts_plugin_export_case_config_for_reader(QString plgn_name, csv_reader *csv_reader_local)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    struct_global_csv_reader_full_csv_info csv_st_obj = csv_reader_local->read_csv_value(plgn_name);
    QString prnt_name = csv_st_obj.plugin_parent_name;

    QString category_str;
    if(prnt_name == "Web Browsers")
    {
        category_str = "Internet History";
    }
    else if(prnt_name == "Messenger")
    {
        category_str = "Messages";
    }
    else if(prnt_name == "Email Artifacts")
    {
        category_str = "Emails";
    }
    else
    {
        category_str = "Other Artifacts";
    }

    QString export_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "export_case_status.sqlite";

    QString select_cmd = "SELECT list_name from tbl_export_status where category_name = ?";
    QStringList values;
    values << category_str;
    QStringList existing_plugin_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(select_cmd, values, 0, export_status_db_path, Q_FUNC_INFO);

    if(!existing_plugin_name_list.contains(plgn_name))
    {
        QString insert_cmd = "INSERT into tbl_export_status (category_name, list_name, check_status) VALUES(?,?,?)";
        QStringList values;
        values << category_str << plgn_name << QString::number(1);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(insert_cmd, values, export_status_db_path, Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}


void MainWindow::update_load_case_artifacts_tab_rename_config(QString plgn_name, QStringList old_tab_nm_list, QStringList new_tab_nm_list)
{
    recon_static_functions::app_debug("start " + plgn_name,Q_FUNC_INFO);

    // ======= in config start ========
    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    if(QFileInfo(case_conf_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_conf_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        dest_conf_db.setDatabaseName(case_conf_db_path);
        if(!dest_conf_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening conf  ----FAILED--- " + dest_conf_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_conf_db.lastError().text(),Q_FUNC_INFO);
        }

        if(dest_conf_db.isOpen())
        {
            QString slct_cmd_rec = "select INT from plugins where name = '" + plgn_name + "'";
            QString plgn_rec_no = recon_helper_standard_obj->get_string_from_db_by_dbreference(slct_cmd_rec, 0, dest_conf_db, Q_FUNC_INFO).trimmed();

            for(int ii = 0; ii < new_tab_nm_list.size(); ii++)
            {
                QString new_tab_name = new_tab_nm_list.at(ii);
                QString old_tab_name = old_tab_nm_list.at(ii);

                QString cmd_stat_updt = "update plugin_tabs set name = '" + new_tab_name + "' where plugin_no = '" + plgn_rec_no + "' AND name = '" + old_tab_name + "'";

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_stat_updt, dest_conf_db, Q_FUNC_INFO);

            }

            dest_conf_db.close();
        }
    }
    // ======= in config end ========


    // ======= already run tab main list start ========
    QString plgn_main_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    if(QFileInfo(plgn_main_status_db_path).exists())
    {
        for(int ii = 0; ii < new_tab_nm_list.size(); ii++)
        {
            QString new_tab_name = new_tab_nm_list.at(ii);
            QString old_tab_name = old_tab_nm_list.at(ii);

            QString cmd_stat_updt = "update tbl_plugin_status set tab_name = '" + new_tab_name + "' where plugin_name = '" + plgn_name + "' AND tab_name = '" + old_tab_name + "'";

            recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_stat_updt, plgn_main_status_db_path, Q_FUNC_INFO);
        }
    }
    // ======= already run tab main list end ========


    // ======= already run tab sourcewise start ========

    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(int zz = 0; zz < list_target_source_info.size(); zz++)
    {
        struct_GLOBAL_witness_info_source src_info_obj = list_target_source_info.at(zz);

        QString plgn_src_status_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + src_info_obj.source_count_name + QString("/plugin_status.sqlite");

        if(QFileInfo(plgn_src_status_db_path).exists())
        {
            for(int ii = 0; ii < new_tab_nm_list.size(); ii++)
            {
                QString new_tab_name = new_tab_nm_list.at(ii);
                QString old_tab_name = old_tab_nm_list.at(ii);

                QString cmd_stat_updt = "update plugin_status set tab_name = '" + new_tab_name + "' where plugin_name = '" + plgn_name + "' AND tab_name = '" + old_tab_name + "'";

                recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_stat_updt, plgn_src_status_db_path, Q_FUNC_INFO);

            }
        }
    }

    // ======= already run tab sourcewise end ========


    recon_static_functions::app_debug("end " + plgn_name,Q_FUNC_INFO);

}

void MainWindow::update_load_case_artifacts_tab_entry_config(QString plgn_name, csv_reader *csv_reader_local)
{
    recon_static_functions::app_debug("start " + plgn_name,Q_FUNC_INFO);

    // First check previous tab and update its info which is in another function

    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    if(!QFileInfo(case_conf_db_path).exists())
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
    dest_db.setDatabaseName(case_conf_db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + dest_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + dest_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    struct_global_csv_reader_full_csv_info csv_st_obj =  csv_reader_local->read_csv_value(plgn_name);

    QString slct_cmd_rec = "select INT from plugins where name = '" + plgn_name + "'";
    QString plgn_rec_no = recon_helper_standard_obj->get_string_from_db_by_dbreference(slct_cmd_rec, 0, dest_db, Q_FUNC_INFO).trimmed();
    if(plgn_rec_no.isEmpty())
    {
        dest_db.close();
        return;
    }

    QString slct_cmd_tab = "select name from plugin_tabs where plugin_no =" + plgn_rec_no;
    QStringList existing_tabs_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbreference(slct_cmd_tab, 0, dest_db, Q_FUNC_INFO);


    for(int zz = 0; zz < csv_st_obj.tab_name_QStringList_tablewidget.size(); zz++)
    {
        QString tb_nm = csv_st_obj.tab_name_QStringList_tablewidget.at(zz);
        QString exp_avail = csv_st_obj.tab_export_available_QStringList_tablewidget.at(zz);
        QString new_os_schme = csv_st_obj.tab_supported_os_scheme_QList_tablewidget.at(zz).join(MACRO_RECON_Splitter_5_resolution_scope);

        if(exp_avail != "1")
            exp_avail = "0";

        if(!existing_tabs_list.contains(tb_nm))
        {
            QString cmd_insrt_tb = "insert into plugin_tabs (plugin_no, name, export_available, default_selected, supported_os_scheme_display_list) values(?,?,?,?,?)";

            QStringList arg_list_tb;
            arg_list_tb << plgn_rec_no << tb_nm << exp_avail << "0" << new_os_schme;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_insrt_tb, arg_list_tb, dest_db, Q_FUNC_INFO);

        }
        else
        {
            QString cmd_tab_updt = "update plugin_tabs set export_available = '" + exp_avail + "', "
                    + "supported_os_scheme_display_list='" + new_os_schme + "' "
                    + "where plugin_no = '" + plgn_rec_no + "' AND name ='" + tb_nm + "'";
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_tab_updt, dest_db, Q_FUNC_INFO);
        }

    }

    dest_db.close();

    recon_static_functions::app_debug("end " + plgn_name,Q_FUNC_INFO);
}

void MainWindow::update_load_case_artifacts_tab_remove_config(QString plgn_name,QStringList removable_tab_list)
{
    recon_static_functions::app_debug("start " + plgn_name,Q_FUNC_INFO);

    // ======= in config start ========
    QString case_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    if(QFileInfo(case_conf_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_conf_db = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        dest_conf_db.setDatabaseName(case_conf_db_path);
        if(!dest_conf_db.open())
        {
            recon_static_functions::app_debug(" Destination Db opening conf  ----FAILED--- " + dest_conf_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + dest_conf_db.lastError().text(),Q_FUNC_INFO);
        }

        if(dest_conf_db.isOpen())
        {
            QString slct_cmd_plg_no = "select INT from plugins where name = ?";

            QStringList plugin_name_list;
            plugin_name_list << plgn_name;

            QString plgn_rec_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(slct_cmd_plg_no, plugin_name_list, 0, dest_conf_db, Q_FUNC_INFO).trimmed();

            for(int ii = 0; ii < removable_tab_list.size(); ii++)
            {
                QString tab_name = removable_tab_list.at(ii);
                QString cmd_stat_remove = "Delete FROM plugin_tabs where name = ?  AND plugin_no = ?";

                QStringList values_list ;
                values_list << tab_name << plgn_rec_no;

                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(cmd_stat_remove, values_list, dest_conf_db, Q_FUNC_INFO);

            }

            dest_conf_db.close();
        }
    }
    // ======= in config end ========

    recon_static_functions::app_debug("end " + plgn_name,Q_FUNC_INFO);

}


void MainWindow::update_result_plugin_gmail_123()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_gmail = "Gmail";
    QString plgn_nm_tmp_gmail = plgn_nm_real_gmail;
    plgn_nm_tmp_gmail.replace(" ", "_");


    QString plgn_csv_file_in_result_gmail      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_gmail +".csv";
    QString plgn_csv_file_in_app_bundle_gmail  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_gmail + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_gmail, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_gmail, plgn_csv_file_in_result_gmail);

    csv_reader *csv_reader_local_gmail = new csv_reader(this);
    csv_reader_local_gmail->do_job_plugin_specific(plgn_csv_file_in_result_gmail, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_gmail, csv_reader_local_gmail);

    QStringList old_tab_list_gmail, new_tab_list_gmail;
    old_tab_list_gmail << "Mail Data";
    new_tab_list_gmail << "Messages";
    update_load_case_artifacts_tab_rename_config(plgn_nm_real_gmail, old_tab_list_gmail, new_tab_list_gmail);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_gmail, csv_reader_local_gmail);


    // in Gmail artifact data
    QString gmail_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_gmail + "/" + plgn_nm_tmp_gmail + ".sqlite";
    if(QFileInfo(gmail_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_gmail = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_gmail.setDatabaseName(gmail_db_path);
        if(!destination_db_gmail.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_gmail + destination_db_gmail.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_gmail.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_gmail.isOpen())
        {
            QString new_tablename = "messages";

            QString cmd_altr = "ALTER TABLE emlx_data RENAME TO " + new_tablename;
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_altr, destination_db_gmail, Q_FUNC_INFO);

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "display_name" << "bcc_list" << "reply_to_list" << "has_attachment" << "snippet_file" << "date_sent" << "date_received" << "permalink" << "mailbox";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "INTEGER" << "INTEGER" << "varchar(500)"  << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_gmail);
            //====

            QStringList existing_columns_name_list;
            existing_columns_name_list <<  "takeout_email_id" << "emlx_from" << "emlx_subject" << "emlx_to" << "emlx_cc" << "emlx_date" << "emlx_msg_body" << "email_attachment";


            QStringList new_columns_replace_name_list;
            new_columns_replace_name_list <<  "account_id" << "from_list" << "subject" << "to_list" << "cc_list" << "timestamp" << "snippet" << "email_attachments";

            QStringList columns_replace_field_type_list;
            columns_replace_field_type_list << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "INTEGER" << "varchar(500)" << "varchar(500)";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_replace_name_list, columns_replace_field_type_list, destination_db_gmail);


            QString cmd_updt = ("UPDATE '" + new_tablename +"' SET " + new_columns_replace_name_list.at(0) + "=" + existing_columns_name_list.at(0) + ", "
                                + new_columns_replace_name_list.at(1) + "=" + existing_columns_name_list.at(1) + ", "
                                + new_columns_replace_name_list.at(2) + "=" + existing_columns_name_list.at(2) + ", "
                                + new_columns_replace_name_list.at(3) + "=" + existing_columns_name_list.at(3) + ", "
                                + new_columns_replace_name_list.at(4) + "=" + existing_columns_name_list.at(4) + ", "
                                + new_columns_replace_name_list.at(5) + "=" + existing_columns_name_list.at(5) + ", "
                                + new_columns_replace_name_list.at(6) + "=" + existing_columns_name_list.at(6) + ", "
                                + new_columns_replace_name_list.at(7) + "=" + existing_columns_name_list.at(7));

            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, destination_db_gmail, Q_FUNC_INFO);

            destination_db_gmail.close();

        }
    }

}

void MainWindow::update_result_plugin_android_contacts_123()
{
    QString plgn_android_contacts = "Android Contacts";
    QString plgn_tmp_android_contacts = plgn_android_contacts;
    plgn_tmp_android_contacts.replace(" ", "_");

    QString plgn_csv_file_in_result_android_contacts      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_android_contacts +".csv";
    QString plgn_csv_file_in_app_bundle_android_contacts  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_android_contacts + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_android_contacts, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_android_contacts, plgn_csv_file_in_result_android_contacts);

    csv_reader *csv_reader_local_android_contacts = new csv_reader(this);
    csv_reader_local_android_contacts->do_job_plugin_specific(plgn_csv_file_in_result_android_contacts, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_android_contacts, csv_reader_local_android_contacts);
    update_load_case_artifacts_tab_entry_config(plgn_android_contacts, csv_reader_local_android_contacts);

}

void MainWindow::update_result_plugin_android_photos_123()
{
    QString plgn_nm_android_photos = "Android Photos";
    QString plgn_tmp_android_photos = plgn_nm_android_photos;
    plgn_tmp_android_photos.replace(" ", "_");


    QString plgn_result_csv_android_photos      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_android_photos +".csv";
    QString plgn_csv_app_bundle_android_photos  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_android_photos + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_android_photos, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_android_photos, plgn_result_csv_android_photos);

    csv_reader *csv_reader_local_photos = new csv_reader(this);
    csv_reader_local_photos->do_job_plugin_specific(plgn_result_csv_android_photos, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_android_photos, csv_reader_local_photos);
    update_load_case_artifacts_tab_entry_config(plgn_nm_android_photos, csv_reader_local_photos);

}

void MainWindow::update_result_plugin_android_installed_application_123()
{
    QString plgn_android_installed_apps = "Android Installed Applications";
    QString plgn_tmp_android_instl_apps = plgn_android_installed_apps;
    plgn_tmp_android_instl_apps.replace(" ", "_");


    QString plgn_result_csv_android_instl_apps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_android_instl_apps +".csv";
    QString plgn_csv_app_bundle_android_instl_apps  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_android_instl_apps + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_android_instl_apps, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_android_instl_apps, plgn_result_csv_android_instl_apps);

    csv_reader *csv_reader_local_android_instl_apps = new csv_reader(this);
    csv_reader_local_android_instl_apps->do_job_plugin_specific(plgn_result_csv_android_instl_apps, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_android_installed_apps, csv_reader_local_android_instl_apps);
    update_load_case_artifacts_tab_entry_config(plgn_android_installed_apps, csv_reader_local_android_instl_apps);

}

void MainWindow::update_result_plugin_sound_recorder_123()
{
    QString plgn_sound_recorder = "Sound Recorder";
    QString plgn_tmp_sound_recorder = plgn_sound_recorder;
    plgn_tmp_sound_recorder.replace(" ", "_");


    QString plgn_result_csv_sound_recorder = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_sound_recorder +".csv";
    QString plgn_csv_app_bundle_sound_recorder  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_sound_recorder + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_sound_recorder, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_sound_recorder, plgn_result_csv_sound_recorder);

    csv_reader *csv_reader_local_sound_recorder = new csv_reader(this);
    csv_reader_local_sound_recorder->do_job_plugin_specific(plgn_result_csv_sound_recorder, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_sound_recorder, csv_reader_local_sound_recorder);

}

void MainWindow::update_result_plugin_online_accounts_123()
{
    QString plgn_real_online_accounts = "Online Accounts";
    QString plgn_tmp_online_accounts = plgn_real_online_accounts;
    plgn_tmp_online_accounts.replace(" ", "_");


    QString plgn_result_csv_online_accounts = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_online_accounts +".csv";
    QString plgn_csv_app_bundle_online_accounts  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_online_accounts + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_online_accounts, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_online_accounts, plgn_result_csv_online_accounts);

    csv_reader *csv_reader_local_online_accounts = new csv_reader(this);
    csv_reader_local_online_accounts->do_job_plugin_specific(plgn_result_csv_online_accounts, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_online_accounts, csv_reader_local_online_accounts);

}

void MainWindow::update_result_plugin_airport_wireless_network_123()
{
    QString plgn_airport_wireless_ntwk = "Airport WirelessNetwork";
    QString plgn_tmp_airport_wireless_ntwk = plgn_airport_wireless_ntwk;
    plgn_tmp_airport_wireless_ntwk.replace(" ", "_");


    QString plgn_result_csv_airprt_wrlss_ntwk = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_airport_wireless_ntwk +".csv";
    QString plgn_csv_app_bundle_wrlss_ntwk  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_airport_wireless_ntwk + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_airprt_wrlss_ntwk, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_wrlss_ntwk, plgn_result_csv_airprt_wrlss_ntwk);

    csv_reader *csv_reader_local_wrlss_ntwk = new csv_reader(this);
    csv_reader_local_wrlss_ntwk->do_job_plugin_specific(plgn_result_csv_airprt_wrlss_ntwk, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_airport_wireless_ntwk,csv_reader_local_wrlss_ntwk );


    // in 'Airport WirelessNetwork' artifact data
    QString result_dir_path        = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString airport_wrlss_ntwk_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_airport_wireless_ntwk + "/" + plgn_tmp_airport_wireless_ntwk + ".sqlite";
    if(QFileInfo(airport_wrlss_ntwk_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_wireless_ntwk = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_wireless_ntwk.setDatabaseName(airport_wrlss_ntwk_db_path);
        if(!destination_db_wireless_ntwk.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_airport_wireless_ntwk + destination_db_wireless_ntwk.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_wireless_ntwk.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_wireless_ntwk.isOpen())
        {
            //====
            QString tablename = "wireless_network";

            QStringList new_columns_name_list;
            new_columns_name_list << "password" << "priority" ;

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)" << "varchar(500)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_wireless_ntwk);
            //====

            destination_db_wireless_ntwk.close();
        }
    }
}

void MainWindow::update_result_plugin_calendar_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_calendar = "Calendar";
    QString plgn_nm_tmp_calendar = plgn_nm_real_calendar;
    plgn_nm_tmp_calendar.replace(" ", "_");


    QString plgn_csv_file_in_result_calendar  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_calendar +".csv";
    QString plgn_csv_file_in_app_bundle_calendar  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_calendar + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_calendar, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_calendar, plgn_csv_file_in_result_calendar);

    csv_reader *csv_reader_local_calendar = new csv_reader(this);
    csv_reader_local_calendar->do_job_plugin_specific(plgn_csv_file_in_result_calendar, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_calendar, csv_reader_local_calendar);

    // in 'Calendar' artifact data
    QString calendar_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_calendar + "/" + plgn_nm_tmp_calendar + ".sqlite";
    if(QFileInfo(calendar_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_calendar = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_calendar.setDatabaseName(calendar_db_path);
        if(!destination_db_calendar.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_calendar + destination_db_calendar.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_calendar.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_calendar.isOpen())
        {
            //====
            QString tablename_1 = "db_events";

            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<  "description" << "last_date" << "deleted" << "duration" << "mutators" << "has_alarm";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "varchar(500)" << "INTEGER" << "varchar(500)" << "varchar(500)" << "varchar(500)"  << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_calendar);



            QString tabname_2 = "accounts";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 << "account_type" << "cal_display_name" << "calendar_timezone" << "visible" << "is_primary" << "deleted" << "max_reminders";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"  << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tabname_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_calendar);

            //====

            destination_db_calendar.close();

        }
    }

}

void MainWindow::update_result_plugin_camera_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_camera = "Camera";
    QString plgn_nm_tmp_camera = plgn_nm_real_camera;
    plgn_nm_tmp_camera.replace(" ", "_");


    QString plgn_csv_file_in_result_camera  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_camera +".csv";
    QString plgn_csv_file_in_app_bundle_camera  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_camera + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_camera, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_camera, plgn_csv_file_in_result_camera);

    csv_reader *csv_reader_local_camera = new csv_reader(this);
    csv_reader_local_camera->do_job_plugin_specific(plgn_csv_file_in_result_camera, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_camera, csv_reader_local_camera);

    // in 'Camera' artifact data
    QString camera_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_camera + "/" + plgn_nm_tmp_camera + ".sqlite";
    if(QFileInfo(camera_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_camera = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_camera.setDatabaseName(camera_db_path);
        if(!destination_db_camera.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_camera + destination_db_camera.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_camera.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_camera.isOpen())
        {
            //====
            QString tablename_1 = "photos_captured";

            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<  "mime_type" << "sha1" << "description" << "title" << "date_taken" << "mixed_date" << "location" << "exif_timestamp"
                                     << "address" << "length" << "orientation" << "gps_latitude" << "gps_longitude" << "exif_make" << "exif_model";


            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "INTEGER"  << "INTEGER" << "varchar(500)" << "INTEGER"
                                      << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"  << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_camera);



            QString tabname_2 = "videos_captured";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<  "mime_type" << "sha1" << "description" << "title" << "date_taken" << "mixed_date" << "location" << "exif_timestamp"
                                     << "address" << "length" << "orientation" << "gps_latitude" << "gps_longitude" << "exif_make" << "exif_model";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "INTEGER"  << "INTEGER" << "varchar(500)" << "INTEGER"
                                      << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"  << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tabname_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_camera);

            //====

            destination_db_camera.close();

        }
    }

}

void MainWindow::update_result_plugin_clock_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_clock = "Clock";
    QString plgn_nm_tmp_clock = plgn_nm_real_clock;
    plgn_nm_tmp_clock.replace(" ", "_");


    QString plgn_csv_file_in_result_clock      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_clock +".csv";
    QString plgn_csv_file_in_app_bundle_clock  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_clock + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_clock, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_clock, plgn_csv_file_in_result_clock);

    csv_reader *csv_reader_local_clock = new csv_reader(this);
    csv_reader_local_clock->do_job_plugin_specific(plgn_csv_file_in_result_clock, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_clock, csv_reader_local_clock);


    // in Clock artifact data
    QString clock_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_clock + "/" + plgn_nm_tmp_clock + ".sqlite";
    if(QFileInfo(clock_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_clock = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_clock.setDatabaseName(clock_db_path);
        if(!destination_db_clock.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_clock + destination_db_clock.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_clock.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_clock.isOpen())
        {
            QString tablename = "alarm";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "year" << "month" << "day" << "ringtone" << "vibrate" << "enabled" << "daysofweek" << "label" << "delete_after_use";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"  << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_clock);
            //====

            destination_db_clock.close();

        }
    }
}

void MainWindow::update_result_plugin_current_wallpaper_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_current_wallpaper = "Current Wallpaper";
    QString plgn_nm_tmp_current_wallpaper = plgn_nm_current_wallpaper;
    plgn_nm_tmp_current_wallpaper.replace(" ", "_");


    QString plgn_csv_result_current_wallpaper = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_current_wallpaper +".csv";
    QString plgn_csv_app_bundle_current_wallpaper  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_current_wallpaper + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_result_current_wallpaper, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_current_wallpaper, plgn_csv_result_current_wallpaper);

    csv_reader *csv_reader_local_current_wallpaper = new csv_reader(this);
    csv_reader_local_current_wallpaper->do_job_plugin_specific(plgn_csv_result_current_wallpaper, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_current_wallpaper, csv_reader_local_current_wallpaper);


    // in 'Current Wallpaper' artifact data
    QString current_wallpaper_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_current_wallpaper + "/" + plgn_nm_tmp_current_wallpaper + ".sqlite";
    if(QFileInfo(current_wallpaper_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_current_wallpaper = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_current_wallpaper.setDatabaseName(current_wallpaper_db_path);
        if(!destination_db_current_wallpaper.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_current_wallpaper + destination_db_current_wallpaper.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_current_wallpaper.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_current_wallpaper.isOpen())
        {
            QString tablename = "current_wallpaper";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "file_exists" << "width" << "height" << "crop_top" << "crop_bottom" << "crop_left" << "crop_right" << "backup";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_current_wallpaper);
            //====

            destination_db_current_wallpaper.close();

        }
    }
}

void MainWindow::update_result_plugin_google_chrome_123()
{

    QString plgn_google_chrome = "Google Chrome";
    QString plgn_nm_tmp_google_chrome = plgn_google_chrome;
    plgn_nm_tmp_google_chrome.replace(" ", "_");


    QString plgn_csv_result_google_chrome  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_google_chrome +".csv";
    QString plgn_csv_app_bundle_google_chrome = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_google_chrome + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_result_google_chrome, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_google_chrome, plgn_csv_result_google_chrome);

    csv_reader *csv_reader_local_google_chrome = new csv_reader(this);
    csv_reader_local_google_chrome->do_job_plugin_specific(plgn_csv_result_google_chrome, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_google_chrome, csv_reader_local_google_chrome);

}

void MainWindow::update_result_plugin_google_drive_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_google_drive = "Google Drive";
    QString plgn_nm_tmp_google_drive = plgn_nm_real_google_drive;
    plgn_nm_tmp_google_drive.replace(" ", "_");


    QString plgn_csv_in_result_google_drive = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_google_drive +".csv";
    QString plgn_csv_in_app_bundle_google_drive = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_google_drive + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_in_result_google_drive, Q_FUNC_INFO);
    QFile::copy(plgn_csv_in_app_bundle_google_drive, plgn_csv_in_result_google_drive);

    csv_reader *csv_reader_local_google_drive = new csv_reader(this);
    csv_reader_local_google_drive->do_job_plugin_specific(plgn_csv_in_result_google_drive, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_google_drive, csv_reader_local_google_drive);

    update_load_case_artifacts_tab_entry_config(plgn_nm_real_google_drive, csv_reader_local_google_drive);


    // in 'Google Drive' artifact data
    QString google_drive_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_google_drive + "/" + plgn_nm_tmp_google_drive + ".sqlite";
    if(QFileInfo(google_drive_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_google_drive = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_google_drive.setDatabaseName(google_drive_db_path);
        if(!destination_db_google_drive.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_google_drive + destination_db_google_drive.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_google_drive.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_google_drive.isOpen())
        {
            QString tablename = "uploaded_files";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "user_id" << "modified_by_me_date" << "shared_with_me_date" << "viewed_by_me_date" << "recency_date"
                                   << "storage_used" << "hidden" << "mime_type" << "starred" << "is_folder" << "is_owner" << "is_trashed";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "varchar(500)" << "varchar(500)"
                                    << "varchar(500)" << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_google_drive);
            //====

            destination_db_google_drive.close();

        }
    }

}

void MainWindow::update_result_plugin_launchpad_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_launchpad = "Launchpad";
    QString plgn_nm_tmp_launchpad = plgn_nm_real_launchpad;
    plgn_nm_tmp_launchpad.replace(" ", "_");


    QString plgn_csv_file_in_result_launchpad      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_launchpad +".csv";
    QString plgn_csv_file_in_app_bundle_launchpad  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_launchpad + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_launchpad, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_launchpad, plgn_csv_file_in_result_launchpad);

    csv_reader *csv_reader_local_launchpad = new csv_reader(this);
    csv_reader_local_launchpad->do_job_plugin_specific(plgn_csv_file_in_result_launchpad, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_launchpad, csv_reader_local_launchpad);


    // in Launchpad artifact data
    QString launchpad_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_launchpad + "/" + plgn_nm_tmp_launchpad + ".sqlite";
    if(QFileInfo(launchpad_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_launchpad = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_launchpad.setDatabaseName(launchpad_db_path);
        if(!destination_db_launchpad.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_launchpad + destination_db_launchpad.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_launchpad.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_launchpad.isOpen())
        {
            QString tablename = "applications";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "intent" << "container" << "cellX" << "cellY" << "spanX" << "spanY" << "item_type" << "app_widgets_id"
                                   << "icon_package" << "icon_resources" << "icon" << "uri" << "display_mode" << "profield";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                    << "varchar(500)" << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                    << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_launchpad);
            //====

            destination_db_launchpad.close();

        }
    }

}

void MainWindow::update_result_plugin_messages_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_messages = "Messages";
    QString plgn_nm_tmp_messages = plgn_nm_real_messages;
    plgn_nm_tmp_messages.replace(" ", "_");


    QString plgn_csv_file_in_result_messages  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_messages +".csv";
    QString plgn_csv_file_in_app_bundle_messages  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_messages + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_messages, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_messages, plgn_csv_file_in_result_messages);

    csv_reader *csv_reader_local_messages = new csv_reader(this);
    csv_reader_local_messages->do_job_plugin_specific(plgn_csv_file_in_result_messages, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_messages, csv_reader_local_messages);


    // in Messages artifact data
    QString messages_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_messages + "/" + plgn_nm_tmp_messages + ".sqlite";
    if(QFileInfo(messages_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_messages = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_messages.setDatabaseName(messages_db_path);
        if(!destination_db_messages.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_messages + destination_db_messages.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_messages.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_messages.isOpen())
        {
            QString tablename = "chatdb";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "service_center" << "sent_date" ;


            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)" << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_messages);
            //====

            destination_db_messages.close();

        }
    }

}

void MainWindow::update_result_plugin_bluetooth_123()
{

    QString plgn_nm_real_bluetooth = "Bluetooth";
    QString plgn_nm_tmp_bluetooth = plgn_nm_real_bluetooth;
    plgn_nm_tmp_bluetooth.replace(" ", "_");


    QString plgn_csv_file_in_result_bluetooth = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_bluetooth +".csv";
    QString plgn_csv_file_in_app_bundle_bluetooth  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_bluetooth + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_bluetooth, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_bluetooth, plgn_csv_file_in_result_bluetooth);

    csv_reader *csv_reader_local_bluetooth = new csv_reader(this);
    csv_reader_local_bluetooth->do_job_plugin_specific(plgn_csv_file_in_result_bluetooth, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_bluetooth, csv_reader_local_bluetooth);

}

void MainWindow::update_result_plugin_google_keep_123()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_google_keep = "Google Keep";
    QString plgn_nm_tmp_gmail = plgn_nm_real_google_keep;
    plgn_nm_tmp_gmail.replace(" ", "_");


    QString plgn_csv_file_in_result_google_keep      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_gmail +".csv";
    QString plgn_csv_file_in_app_bundle_google_keep  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_gmail + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_google_keep, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_google_keep, plgn_csv_file_in_result_google_keep);

    csv_reader *csv_reader_local_google_keep = new csv_reader(this);
    csv_reader_local_google_keep->do_job_plugin_specific(plgn_csv_file_in_result_google_keep, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_google_keep, csv_reader_local_google_keep);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_google_keep, csv_reader_local_google_keep);


    // in 'Google Kepp' artifact data
    QString google_keep_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_gmail + "/" + plgn_nm_tmp_gmail + ".sqlite";
    if(QFileInfo(google_keep_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_google_keep = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_google_keep.setDatabaseName(google_keep_db_path);
        if(!destination_db_google_keep.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_google_keep + destination_db_google_keep.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_google_keep.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_google_keep.isOpen())
        {
            QString new_tablename = "keep_notes";

            QString cmd_altr = "ALTER TABLE tbl_google_keep RENAME TO " + new_tablename;
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_altr, destination_db_google_keep, Q_FUNC_INFO);

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "synced_text" << "is_checked" << "is_deleted" << "created_time" << "last_updated_time" << "is_archived"
                                   << "is_trashed" << "color" << "user_edit_timestamp" << "attachment_name" << "attachment_path" << "attachment_size";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "INTEGER" << "INTEGER" << "varchar(500)"
                                    << "varchar(500)" << "varchar(500)" << "INTEGER" << "varchar(500)" << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_google_keep);
            //====

            QStringList existing_columns_name_list;
            existing_columns_name_list <<  "takeout_email_id";


            QStringList new_columns_replace_name_list;
            new_columns_replace_name_list <<  "account_id";

            QStringList columns_replace_field_type_list;
            columns_replace_field_type_list << "varchar(500)";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_replace_name_list, columns_replace_field_type_list, destination_db_google_keep);


            QString cmd_updt = ("UPDATE '" + new_tablename +"' SET " + new_columns_replace_name_list.at(0) + "=" + existing_columns_name_list.at(0));
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_updt, destination_db_google_keep, Q_FUNC_INFO);

            destination_db_google_keep.close();

        }
    }

}

void MainWindow::update_result_plugin_wifi_location_123()
{
    QString plgn_wifi_location = "Wi-Fi Location";
    QString plgn_tmp_wifi_location = plgn_wifi_location;
    plgn_tmp_wifi_location.replace(" ", "_");

    QString plgn_csv_file_in_result_wifi_location      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_wifi_location +".csv";
    QString plgn_csv_file_in_app_bundle_wifi_location  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_wifi_location + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_wifi_location, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_wifi_location, plgn_csv_file_in_result_wifi_location);

    csv_reader *csv_reader_local_wifi_location = new csv_reader(this);
    csv_reader_local_wifi_location->do_job_plugin_specific(plgn_csv_file_in_result_wifi_location, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_wifi_location, csv_reader_local_wifi_location);
    update_load_case_artifacts_tab_entry_config(plgn_wifi_location, csv_reader_local_wifi_location);

}

void MainWindow::update_result_plugin_printer_spool_123()
{
    QString plgn_printer_spool = "Printer Spool";
    QString plgn_tmp_printer_spool = plgn_printer_spool;
    plgn_tmp_printer_spool.replace(" ", "_");

    QString plgn_csv_file_in_result_printer_spool      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_printer_spool +".csv";
    QString plgn_csv_file_in_app_bundle_printer_spool  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_printer_spool + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_printer_spool, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_printer_spool, plgn_csv_file_in_result_printer_spool);

    csv_reader *csv_reader_local_printer_spool = new csv_reader(this);
    csv_reader_local_printer_spool->do_job_plugin_specific(plgn_csv_file_in_result_printer_spool, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_printer_spool, csv_reader_local_printer_spool);
    update_load_case_artifacts_tab_entry_config(plgn_printer_spool, csv_reader_local_printer_spool);

}

void MainWindow::update_result_plugin_photos_130()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_photos = "Photos";
    QString plgn_nm_tmp_photos = plgn_nm_real_photos;
    plgn_nm_tmp_photos.replace(" ", "_");


    QString plgn_csv_file_in_result_photos  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_photos +".csv";
    QString plgn_csv_file_in_app_bundle_photos  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_photos + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_photos, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_photos, plgn_csv_file_in_result_photos);

    csv_reader *csv_reader_local_photos = new csv_reader(this);
    csv_reader_local_photos->do_job_plugin_specific(plgn_csv_file_in_result_photos, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_photos, csv_reader_local_photos);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_photos, csv_reader_local_photos);


    // in photos artifact data
    QString photos_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_photos + "/" + plgn_nm_tmp_photos + ".sqlite";
    if(QFileInfo(photos_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_fs_events.setDatabaseName(photos_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_photos + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {

            QString table_name = "moments";

            QStringList new_columns_name_list;
            new_columns_name_list << "moments_title";

            QStringList columns_field_type_list;
            columns_field_type_list << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);


            destination_db_fs_events.close();
        }
    }
}

void MainWindow::update_result_plugin_microsoft_edge_130()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_microsoft_edge = "Microsoft Edge";
    QString plgn_nm_tmp_microsoft_edge = plgn_nm_real_microsoft_edge;
    plgn_nm_tmp_microsoft_edge.replace(" ", "_");


    QString plgn_csv_file_in_result_microsoft_edge  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_microsoft_edge +".csv";
    QString plgn_csv_file_in_app_bundle_microsoft_edge  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_microsoft_edge + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_microsoft_edge, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_microsoft_edge, plgn_csv_file_in_result_microsoft_edge);

    csv_reader *csv_reader_local_microsoft_edge = new csv_reader(this);
    csv_reader_local_microsoft_edge->do_job_plugin_specific(plgn_csv_file_in_result_microsoft_edge, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_microsoft_edge, csv_reader_local_microsoft_edge);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_microsoft_edge, csv_reader_local_microsoft_edge);


    // in microsoft_edge artifact data
    QString microsoft_edge_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_microsoft_edge + "/" + plgn_nm_tmp_microsoft_edge + ".sqlite";
    if(QFileInfo(microsoft_edge_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_fs_events.setDatabaseName(microsoft_edge_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_microsoft_edge + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {

            QString table_name = "history";

            QStringList new_columns_name_list;
            new_columns_name_list << "id"<< "title"<< "visit_count"<< "typed_count"<< "hidden"<< "last_visit_time"<< "visit_time"<< "last_visit_date";

            QStringList columns_field_type_list;
            columns_field_type_list << "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);

            QString table_name_1 = "downloads";

            QStringList new_columns_name_list_1;
            new_columns_name_list_1 << "id"<< "suffix"<< "received_bytes"<< "total_bytes"<< "state"<< "referrer"
                                    << "site_url"<< "tab_url"<< "tab_referrer_url"<< "where_from"<< "latitude"
                                    << "longitude"<< "altitude"<< "torrent_file_list"<< "date_used"<< "file_type"
                                    << "date_added"<< "last_used_date"<< "download_start_time"<< "download_end_time";



            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"
                                      << "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"
                                      << "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"
                                      <<"INTEGER"<<"INTEGER"<<"INTEGER"<<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_fs_events);


            QString table_name_2 = "cookies";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 << "value"<< "host_key"<< "is_secure"<< "has_expires"<< "is_persistent"<< "priority";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_fs_events);

            destination_db_fs_events.close();
        }
    }
}

void MainWindow::update_result_plugin_brave_browser_130()
{
    QString plgn_real_brave_browser = "Brave Browser";
    QString plgn_tmp_brave_browser = plgn_real_brave_browser;
    plgn_tmp_brave_browser.replace(" ", "_");


    QString plgn_result_csv_brave_browser = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_brave_browser +".csv";
    QString plgn_csv_app_bundle_brave_browser  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_brave_browser + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_brave_browser, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_brave_browser, plgn_result_csv_brave_browser);

    csv_reader *csv_reader_local_brave_browser = new csv_reader(this);
    csv_reader_local_brave_browser->do_job_plugin_specific(plgn_result_csv_brave_browser, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_brave_browser, csv_reader_local_brave_browser);
    update_load_case_artifacts_tab_entry_config(plgn_real_brave_browser, csv_reader_local_brave_browser);

}

void MainWindow::update_result_plugin_utorrent_web_130()
{
    QString plgn_real_utorrent_web = "uTorrent Web";
    QString plgn_tmp_utorrent_web = plgn_real_utorrent_web;
    plgn_tmp_utorrent_web.replace(" ", "_");


    QString plgn_result_csv_utorrent_web = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_utorrent_web +".csv";
    QString plgn_csv_app_bundle_utorrent_web  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_utorrent_web + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_utorrent_web, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_utorrent_web, plgn_result_csv_utorrent_web);

    csv_reader *csv_reader_local_utorrent_web = new csv_reader(this);
    csv_reader_local_utorrent_web->do_job_plugin_specific(plgn_result_csv_utorrent_web, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_utorrent_web, csv_reader_local_utorrent_web);
    update_load_case_artifacts_tab_entry_config(plgn_real_utorrent_web, csv_reader_local_utorrent_web);

}



void MainWindow::update_result_plugin_bittorrent_web_136()
{
    QString plgn_real_bittorrent_web = "BitTorrent Web";
    QString plgn_tmp_bittorrent_web = plgn_real_bittorrent_web;
    plgn_tmp_bittorrent_web.replace(" ", "_");


    QString plgn_result_csv_bittorrent_web = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_bittorrent_web +".csv";
    QString plgn_csv_app_bundle_bittorrent_web  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_bittorrent_web + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_bittorrent_web, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_bittorrent_web, plgn_result_csv_bittorrent_web);

    csv_reader *csv_reader_local_bittorrent_web = new csv_reader(this);
    csv_reader_local_bittorrent_web->do_job_plugin_specific(plgn_result_csv_bittorrent_web, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_bittorrent_web, csv_reader_local_bittorrent_web);
    update_load_case_artifacts_tab_entry_config(plgn_real_bittorrent_web, csv_reader_local_bittorrent_web);

}

void MainWindow::update_result_plugin_translate_130()
{
    QString plgn_real_translate = "Translate";
    QString plgn_tmp_translate = plgn_real_translate;
    plgn_tmp_translate.replace(" ", "_");


    QString plgn_result_csv_translate = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_translate +".csv";
    QString plgn_csv_app_bundle_translate  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_translate + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_translate, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_translate, plgn_result_csv_translate);

    csv_reader *csv_reader_local_translate = new csv_reader(this);
    csv_reader_local_translate->do_job_plugin_specific(plgn_result_csv_translate, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_translate, csv_reader_local_translate);
    update_load_case_artifacts_tab_entry_config(plgn_real_translate, csv_reader_local_translate);

}


void MainWindow::update_result_plugin_exe_files_125()
{
    QString plgn_exe_files = "Exe Files";
    QString plgn_tmp_exe_files = plgn_exe_files;
    plgn_tmp_exe_files.replace(" ", "_");

    QString plgn_csv_file_in_result_exe_files  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_exe_files +".csv";
    QString plgn_csv_file_in_app_bundle_exe_files  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_exe_files + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_exe_files, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_exe_files, plgn_csv_file_in_result_exe_files);

    csv_reader *csv_reader_local_exe_files = new csv_reader(this);
    csv_reader_local_exe_files->do_job_plugin_specific(plgn_csv_file_in_result_exe_files, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_exe_files, csv_reader_local_exe_files);
    update_load_case_artifacts_tab_entry_config(plgn_exe_files, csv_reader_local_exe_files);

}

void MainWindow::update_result_plugin_link_files_123()
{
    QString plgn_link_files = "Link Files";
    QString plgn_tmp_link_files = plgn_link_files;
    plgn_tmp_link_files.replace(" ", "_");

    QString plgn_csv_file_in_result_link_files  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_link_files +".csv";
    QString plgn_csv_file_in_app_bundle_link_files  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_link_files + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_link_files, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_link_files, plgn_csv_file_in_result_link_files);

    csv_reader *csv_reader_local_link_files = new csv_reader(this);
    csv_reader_local_link_files->do_job_plugin_specific(plgn_csv_file_in_result_link_files, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_link_files, csv_reader_local_link_files);
    update_load_case_artifacts_tab_entry_config(plgn_link_files, csv_reader_local_link_files);

}

void MainWindow::update_result_plugin_viber_123()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_viber = "Viber";
    QString plgn_nm_tmp_viber = plgn_nm_real_viber;
    plgn_nm_tmp_viber.replace(" ", "_");


    QString plgn_csv_file_in_result_viber  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_viber +".csv";
    QString plgn_csv_file_in_app_bundle_viber  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_viber + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_viber, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_viber, plgn_csv_file_in_result_viber);

    csv_reader *csv_reader_local_viber = new csv_reader(this);
    csv_reader_local_viber->do_job_plugin_specific(plgn_csv_file_in_result_viber, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_viber, csv_reader_local_viber);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_viber, csv_reader_local_viber);


    // in 'Viber' artifact data
    QString viber_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_viber + "/" + plgn_nm_tmp_viber + ".sqlite";
    if(QFileInfo(viber_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_viber = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_viber.setDatabaseName(viber_db_path);
        if(!destination_db_viber.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_viber + destination_db_viber.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_viber.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_viber.isOpen())
        {
            //====
            QString new_tablename_1 = "viber_contacts";
            QStringList new_columns_name_list_1;

            new_columns_name_list_1 << "update_timestamp" << "encrypted_number" << "member_id" << "encrypted_member_id" << "joined_date";


            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "INTEGER"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "INTEGER";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_viber);
            //====

            QString new_tablename_2 = "all_contacts";
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<  "numbers_name" << "joined_date" << "contact_lookup_key" << "Contact_hash" << "low_display_name"
                                     << "has_number"  << "contact_detail";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                      << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_viber);


            QString new_tablename_3 = "accounts";
            QStringList new_columns_name_list_3;
            new_columns_name_list_3 << "update_timestamp"  << "account_name";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 << "INTEGER" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_viber);


            QString new_tablename_4 = "viber_messages";
            QStringList new_columns_name_list_4;
            new_columns_name_list_4 << "message_info";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4  << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_viber);


            QString new_tablename_5 = "media_messages";
            QStringList new_columns_name_list_5;
            new_columns_name_list_5 << "file_hash" << "media_type" << "height" << "width" << "meta_data" << "content_length" << "content_type"
                                    << "thumbnail_type" << "thumbnail_height" << "thumbnail_width" << "silent" << "domain";

            QStringList columns_field_type_list_5;
            columns_field_type_list_5  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                       << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_5),new_columns_name_list_5, columns_field_type_list_5, destination_db_viber);

            destination_db_viber.close();

        }
    }

}

void MainWindow::update_result_plugin_vlc_123()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_vlc = "VLC";
    QString plgn_nm_tmp_vlc = plgn_nm_real_vlc;
    plgn_nm_tmp_vlc.replace(" ", "_");


    QString plgn_csv_file_in_result_vlc      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_vlc +".csv";
    QString plgn_csv_file_in_app_bundle_vlc  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_vlc + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_vlc, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_vlc, plgn_csv_file_in_result_vlc);

    csv_reader *csv_reader_local_vlc = new csv_reader(this);
    csv_reader_local_vlc->do_job_plugin_specific(plgn_csv_file_in_result_vlc, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_vlc, csv_reader_local_vlc);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_vlc, csv_reader_local_vlc);


    // in 'VLC' artifact data
    QString vlc_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_vlc + "/" + plgn_nm_tmp_vlc + ".sqlite";
    if(QFileInfo(vlc_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_vlc = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_vlc.setDatabaseName(vlc_db_path);
        if(!destination_db_vlc.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_vlc + destination_db_vlc.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_vlc.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_vlc.isOpen())
        {
            //====
            QString new_tablename_1 = "media_library";
            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<  "codec" << "bitrate" << "samplerate" << "nb_channels" << "language" << "playlist_name" << "favorite" << "present"
                                     << "removable" << "external" << "play_count" << "last_played_date" << "real_last_played_date" << "insertion_date"
                                     << "release_date" << "last_modification_date" << "width" << "height" << "fps_num" << "fps_den" << "sar_num" << "sar_den";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                      << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "INTEGER"
                                      << "INTEGER" << "INTEGER"  << "INTEGER" << "INTEGER" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                      << "varchar(500)"<< "varchar(500)" << "varchar(500)";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_vlc);
            //====

            QString new_tablename_2 = "album_art";
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<  "duration" << "no_of_tracks" << "artist" << "export_file_paths" << "LINKFILENAME" << "bookmarked_extended_attributes"
                                     << "bookmarked_exif_data" << "extended_attributes_key_value" << "extended_attributes_value_for_search";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "varchar(500)"  << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                      << "varchar(500)" << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_vlc);

            QString new_tablename_3 = "playlist";
            QStringList new_columns_name_list_3;
            new_columns_name_list_3 << "suffix" ;

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_vlc);
            destination_db_vlc.close();

        }
    }

}

void MainWindow::update_result_plugin_opera_mini_123()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_real_opera_mini = "Opera Mini";
    QString plgn_tmp_opera_mini = plgn_real_opera_mini;
    plgn_tmp_opera_mini.replace(" ", "_");


    QString plgn_csv_in_result_opera_mini      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_opera_mini +".csv";
    QString plgn_csv_in_app_bundle_opera_mini  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_opera_mini + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_in_result_opera_mini, Q_FUNC_INFO);
    QFile::copy(plgn_csv_in_app_bundle_opera_mini, plgn_csv_in_result_opera_mini);

    csv_reader *csv_reader_local_opera_mini = new csv_reader(this);
    csv_reader_local_opera_mini->do_job_plugin_specific(plgn_csv_in_result_opera_mini, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_opera_mini, csv_reader_local_opera_mini);
    update_load_case_artifacts_tab_entry_config(plgn_real_opera_mini, csv_reader_local_opera_mini);


    // in 'Opera Mini' artifact data
    QString opera_mini_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_opera_mini + "/" + plgn_tmp_opera_mini + ".sqlite";
    if(QFileInfo(opera_mini_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_opera_mini = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_opera_mini.setDatabaseName(opera_mini_db_path);
        if(!destination_db_opera_mini.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_real_opera_mini + destination_db_opera_mini.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_opera_mini.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_opera_mini.isOpen())
        {

            //====
            QString table_name_1 = "cookies";
            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<  "host_key" << "is_secure" << "is_http_only" << "creation_timestamp" << "expire_timestamp" << "last_access_timestamp";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "varchar(500)"  << "varchar(500)" << "varchar(500)"  << "INTEGER" << "INTEGER" << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_opera_mini);
            //====

            QString table_name_2 = "bookmarks";
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<  "date_modified" << "image_data" << "image_data_type" << "image_id" << "image_identifier" << "image_type"
                                     << "partner_id" << "partner_download_url" << "partner_url" << "ping_url" << "revision" << "server_position"
                                     << "favicon_url" << "nav_url";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "INTEGER" << "varchar(500)" << "varchar(500)" << "varchar(500)"  << "varchar(500)" << "varchar(500)"
                                      << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                      << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_opera_mini);

            //======//
            QString table_name_3 = "bookmarks_folders";
            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<  "type" << "id";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_opera_mini);

            destination_db_opera_mini.close();

        }
    }

}

void MainWindow::update_result_plugin_youtube_123()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_youtube = "YouTube";
    QString plgn_nm_tmp_youtube = plgn_nm_real_youtube;
    plgn_nm_tmp_youtube.replace(" ", "_");


    QString plgn_csv_file_in_result_youtube = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_youtube +".csv";
    QString plgn_csv_file_in_app_bundle_youtube  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_youtube + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_youtube, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_youtube, plgn_csv_file_in_result_youtube);

    csv_reader *csv_reader_local_youtube = new csv_reader(this);
    csv_reader_local_youtube->do_job_plugin_specific(plgn_csv_file_in_result_youtube, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_youtube, csv_reader_local_youtube);

    update_load_case_artifacts_tab_entry_config(plgn_nm_real_youtube, csv_reader_local_youtube);


    // in YouTube artifact data
    QString youtube_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_youtube + "/" + plgn_nm_tmp_youtube + ".sqlite";
    if(QFileInfo(youtube_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_youtube = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_youtube.setDatabaseName(youtube_db_path);
        if(!destination_db_youtube.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_youtube + destination_db_youtube.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_youtube.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_youtube.isOpen())
        {
            QString new_tablename = "offline_saved_videos";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<  "saved_timestamp" << "transfer_completed_timestamp" << "transfer_started_timestamp"
                                   << "metadata_timestamp" << "last_refresh_timestamp" << "last_playback_timestamp" << "streams_timestamp"
                                   << "video_added_timestamp" << "bytes_total" << "channel_id" << "bytes_transferred" << "preview_file_path"
                                   << "export_file_paths" << "LINKFILENAME" << "bookmarked_extended_attributes" << "bookmarked_exif_data"
                                   << "extended_attributes_key_value" << "extended_attributes_value_for_search";

            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER"
                                    << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)"
                                    << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_youtube);
            //====

            destination_db_youtube.close();

        }
    }
}

void MainWindow::update_result_plugin_user_logs_125()
{
    QString plgn_user_logs = "User Logs";
    QString plgn_tmp_user_logs = plgn_user_logs;
    plgn_tmp_user_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_user_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_user_logs +".csv";
    QString plgn_csv_file_in_app_bundle_user_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_user_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_user_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_user_logs, plgn_csv_file_in_result_user_logs);

    csv_reader *csv_reader_local_user_logs = new csv_reader(this);
    csv_reader_local_user_logs->do_job_plugin_specific(plgn_csv_file_in_result_user_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_user_logs, csv_reader_local_user_logs);
    update_load_case_artifacts_tab_entry_config(plgn_user_logs, csv_reader_local_user_logs);

}

void MainWindow::update_result_plugin_bluetooth_logs_125()
{
    QString plgn_bluetooth_logs = "Bluetooth Logs";
    QString plgn_tmp_bluetooth_logs = plgn_bluetooth_logs;
    plgn_tmp_bluetooth_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_bluetooth_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_bluetooth_logs +".csv";
    QString plgn_csv_file_in_app_bundle_bluetooth_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_bluetooth_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_bluetooth_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_bluetooth_logs, plgn_csv_file_in_result_bluetooth_logs);

    csv_reader *csv_reader_local_bluetooth_logs = new csv_reader(this);
    csv_reader_local_bluetooth_logs->do_job_plugin_specific(plgn_csv_file_in_result_bluetooth_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_bluetooth_logs, csv_reader_local_bluetooth_logs);
    update_load_case_artifacts_tab_entry_config(plgn_bluetooth_logs, csv_reader_local_bluetooth_logs);

}

void MainWindow::update_result_plugin_network_logs_125()
{
    QString plgn_network_logs = "Network Logs";
    QString plgn_tmp_network_logs = plgn_network_logs;
    plgn_tmp_network_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_network_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_network_logs +".csv";
    QString plgn_csv_file_in_app_bundle_network_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_network_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_network_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_network_logs, plgn_csv_file_in_result_network_logs);

    csv_reader *csv_reader_local_network_logs = new csv_reader(this);
    csv_reader_local_network_logs->do_job_plugin_specific(plgn_csv_file_in_result_network_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_network_logs, csv_reader_local_network_logs);
    update_load_case_artifacts_tab_entry_config(plgn_network_logs, csv_reader_local_network_logs);

}

void MainWindow::update_result_plugin_icloud_logs_125()
{
    QString plgn_icloud_logs = "iCloud Logs";
    QString plgn_tmp_icloud_logs = plgn_icloud_logs;
    plgn_tmp_icloud_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_icloud_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_icloud_logs +".csv";
    QString plgn_csv_file_in_app_bundle_icloud_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_icloud_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_icloud_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_icloud_logs, plgn_csv_file_in_result_icloud_logs);

    csv_reader *csv_reader_local_icloud_logs = new csv_reader(this);
    csv_reader_local_icloud_logs->do_job_plugin_specific(plgn_csv_file_in_result_icloud_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_icloud_logs, csv_reader_local_icloud_logs);
    update_load_case_artifacts_tab_entry_config(plgn_icloud_logs, csv_reader_local_icloud_logs);

}

void MainWindow::update_result_plugin_hfs_plus_devices_logs_125()
{
    QString plgn_hfs_plus_logs = "HFS+ Devices Logs";
    QString plgn_tmp_hfs_plus_logs = plgn_hfs_plus_logs;
    plgn_tmp_hfs_plus_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_hfs_plus_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_hfs_plus_logs +".csv";
    QString plgn_csv_file_in_app_bundle_hfs_plus_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_hfs_plus_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_hfs_plus_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_hfs_plus_logs, plgn_csv_file_in_result_hfs_plus_logs);

    csv_reader *csv_reader_local_hfs_plus_logs = new csv_reader(this);
    csv_reader_local_hfs_plus_logs->do_job_plugin_specific(plgn_csv_file_in_result_hfs_plus_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_hfs_plus_logs, csv_reader_local_hfs_plus_logs);
    update_load_case_artifacts_tab_entry_config(plgn_hfs_plus_logs, csv_reader_local_hfs_plus_logs);

}

void MainWindow::update_result_plugin_usb_logs_125()
{
    QString plgn_usb_logs = "USB Logs";
    QString plgn_tmp_usb_logs = plgn_usb_logs;
    plgn_tmp_usb_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_usb_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_usb_logs +".csv";
    QString plgn_csv_file_in_app_bundle_usb_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_usb_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_usb_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_usb_logs, plgn_csv_file_in_result_usb_logs);

    csv_reader *csv_reader_local_usb_logs = new csv_reader(this);
    csv_reader_local_usb_logs->do_job_plugin_specific(plgn_csv_file_in_result_usb_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_usb_logs, csv_reader_local_usb_logs);
    update_load_case_artifacts_tab_entry_config(plgn_usb_logs, csv_reader_local_usb_logs);

}

void MainWindow::update_result_plugin_wake_reason_logs_125()
{
    QString plgn_wake_reason_logs = "Wake Reason Logs";
    QString plgn_tmp_wake_reason_logs = plgn_wake_reason_logs;
    plgn_tmp_wake_reason_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_wake_reason_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_wake_reason_logs +".csv";
    QString plgn_csv_file_in_app_bundle_wake_reason_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_wake_reason_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_wake_reason_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_wake_reason_logs, plgn_csv_file_in_result_wake_reason_logs);

    csv_reader *csv_reader_local_wake_reason_logs = new csv_reader(this);
    csv_reader_local_wake_reason_logs->do_job_plugin_specific(plgn_csv_file_in_result_wake_reason_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_wake_reason_logs, csv_reader_local_wake_reason_logs);
    update_load_case_artifacts_tab_entry_config(plgn_wake_reason_logs, csv_reader_local_wake_reason_logs);

}

void MainWindow::update_result_plugin_fs_events_126()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_fs_events = "FS Events";
    QString plgn_nm_tmp_fs_events = plgn_nm_real_fs_events;
    plgn_nm_tmp_fs_events.replace(" ", "_");


    QString plgn_csv_file_in_result_fs_events  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_fs_events +".csv";
    QString plgn_csv_file_in_app_bundle_fs_events  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_fs_events + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_fs_events, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_fs_events, plgn_csv_file_in_result_fs_events);

    csv_reader *csv_reader_local_fs_events = new csv_reader(this);
    csv_reader_local_fs_events->do_job_plugin_specific(plgn_csv_file_in_result_fs_events, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_fs_events, csv_reader_local_fs_events);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_fs_events, csv_reader_local_fs_events);


    // in FS Events artifact data
    QString fs_events_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_fs_events + "/" + plgn_nm_tmp_fs_events + ".sqlite";
    if(QFileInfo(fs_events_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_fs_events.setDatabaseName(fs_events_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_fs_events + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {
            QString table_name = "event_logs";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list << "event_date" << "source_modification_time" << "source_file_name";

            QStringList columns_field_type_list;
            columns_field_type_list << "varchar(500)" << "INTEGER" << "varchar(500)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);
            //====

            destination_db_fs_events.close();

        }
    }

}

void MainWindow::update_result_plugin_voice_memos_128()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_voice_memos = "Voice Memos";
    QString plgn_nm_tmp_voice_memos = plgn_nm_real_voice_memos;
    plgn_nm_tmp_voice_memos.replace(" ", "_");


    QString plgn_csv_file_in_result_voice_memos      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_voice_memos +".csv";
    QString plgn_csv_file_in_app_bundle_voice_memos  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_voice_memos + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_voice_memos, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_voice_memos, plgn_csv_file_in_result_voice_memos);

    csv_reader *csv_reader_local_voice_memos = new csv_reader(this);
    csv_reader_local_voice_memos->do_job_plugin_specific(plgn_csv_file_in_result_voice_memos, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_voice_memos, csv_reader_local_voice_memos);

    update_load_case_artifacts_tab_entry_config(plgn_nm_real_voice_memos, csv_reader_local_voice_memos);


    // in Voice Memos artifact data
    QString voice_memos_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_voice_memos + "/" + plgn_nm_tmp_voice_memos + ".sqlite";
    if(QFileInfo(voice_memos_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_voice_memos = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_voice_memos.setDatabaseName(voice_memos_db_path);
        if(!destination_db_voice_memos.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_voice_memos + destination_db_voice_memos.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_voice_memos.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_voice_memos.isOpen())
        {
            QString table_name = "memos";


            QStringList new_columns_name_list;
            new_columns_name_list << "eviction_date";


            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_voice_memos);

            destination_db_voice_memos.close();

        }
    }

}

void MainWindow::update_result_plugin_clock_128()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_clock = "Clock";
    QString plgn_nm_tmp_clock = plgn_nm_real_clock;
    plgn_nm_tmp_clock.replace(" ", "_");


    QString plgn_csv_file_in_result_clock  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_clock +".csv";
    QString plgn_csv_file_in_app_bundle_clock  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_clock + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_clock, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_clock, plgn_csv_file_in_result_clock);

    csv_reader *csv_reader_local_clock = new csv_reader(this);
    csv_reader_local_clock->do_job_plugin_specific(plgn_csv_file_in_result_clock, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_clock, csv_reader_local_clock);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_clock, csv_reader_local_clock);


    // in clock artifact data
    QString clock_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_clock + "/" + plgn_nm_tmp_clock + ".sqlite";
    if(QFileInfo(clock_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_fs_events.setDatabaseName(clock_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_clock + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {
            QString table_name = "alarm";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list << "fire_date" << "dismiss_date";

            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER" << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);
            //====

            destination_db_fs_events.close();

        }
    }
}

void MainWindow::update_result_plugin_line_128()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_line = "LINE";
    QString plgn_nm_tmp_line = plgn_nm_real_line;
    plgn_nm_tmp_line.replace(" ", "_");


    QString plgn_csv_file_in_result_line  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_line +".csv";
    QString plgn_csv_file_in_app_bundle_line  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_line + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_line, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_line, plgn_csv_file_in_result_line);

    csv_reader *csv_reader_local_line = new csv_reader(this);
    csv_reader_local_line->do_job_plugin_specific(plgn_csv_file_in_result_line, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_line, csv_reader_local_line);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_line, csv_reader_local_line);

}

void MainWindow::update_result_plugin_safari_129()
{

    QString plgn_nm_real_safari = "Safari";
    QString plgn_nm_tmp_safari = plgn_nm_real_safari;
    plgn_nm_tmp_safari.replace(" ", "_");


    QString plgn_csv_file_in_result_safari = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_safari +".csv";
    QString plgn_csv_file_in_app_bundle_safari  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_safari + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_safari, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_safari, plgn_csv_file_in_result_safari);

    csv_reader *csv_reader_local_safari = new csv_reader(this);
    csv_reader_local_safari->do_job_plugin_specific(plgn_csv_file_in_result_safari, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_safari, csv_reader_local_safari);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_safari, csv_reader_local_safari);

}

void MainWindow::update_result_plugin_google_chrome_129()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_google_chrome = "Google Chrome";
    QString plgn_nm_tmp_google_chrome = plgn_nm_real_google_chrome;
    plgn_nm_tmp_google_chrome.replace(" ", "_");


    QString plgn_csv_file_in_result_google_chrome  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_google_chrome +".csv";
    QString plgn_csv_file_in_app_bundle_google_chrome  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_google_chrome + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_google_chrome, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_google_chrome, plgn_csv_file_in_result_google_chrome);

    csv_reader *csv_reader_local_google_chrome = new csv_reader(this);
    csv_reader_local_google_chrome->do_job_plugin_specific(plgn_csv_file_in_result_google_chrome, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_google_chrome, csv_reader_local_google_chrome);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_google_chrome, csv_reader_local_google_chrome);


    // in google chrome artifact data
    QString google_chrome_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_google_chrome + "/" + plgn_nm_tmp_google_chrome + ".sqlite";
    if(QFileInfo(google_chrome_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_fs_events.setDatabaseName(google_chrome_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_google_chrome + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {
            QString table_name = "cookies";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list << "domain";

            QStringList columns_field_type_list;
            columns_field_type_list << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);
            //====

            destination_db_fs_events.close();

        }
    }
}

void MainWindow::update_result_plugin_photos_129()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_photos = "Photos";
    QString plgn_nm_tmp_photos = plgn_nm_real_photos;
    plgn_nm_tmp_photos.replace(" ", "_");


    QString plgn_csv_file_in_result_photos  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_photos +".csv";
    QString plgn_csv_file_in_app_bundle_photos  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_photos + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_photos, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_photos, plgn_csv_file_in_result_photos);

    csv_reader *csv_reader_local_photos = new csv_reader(this);
    csv_reader_local_photos->do_job_plugin_specific(plgn_csv_file_in_result_photos, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_photos, csv_reader_local_photos);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_photos, csv_reader_local_photos);


    // in mozilla firefox artifact data
    QString photos_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_photos + "/" + plgn_nm_tmp_photos + ".sqlite";
    if(QFileInfo(photos_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_fs_events.setDatabaseName(photos_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_photos + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {
            QString table_name = "all_photos";

            QStringList new_columns_name_list;
            new_columns_name_list << "face_area_points" << "timezone_name" <<"timezone_offset" << "original_latitude" <<"original_longitude"
                                  <<"original_colorspace" <<"exif_timestamp" <<"last_shared_date" <<"scence_analysis_timestamp";

            QStringList columns_field_type_list;
            columns_field_type_list << "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"<< "VARCHAR(1024)"
                                    <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);

            QString table_name_2 = "shared_albums";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 << "system_account" <<"start_date" <<"end_date" <<"creation_date";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "VARCHAR(1024)" <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_fs_events);

            QString table_name_3 = "memories";

            QStringList new_columns_name_list_3;
            new_columns_name_list_3 << "system_account";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_fs_events);


            QString table_name_4 = "projects";

            QStringList new_columns_name_list_4;
            new_columns_name_list_4 << "item_count" <<"photos_count" << "videos_count" <<"start_date" <<"end_date";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_fs_events);


            QString table_name_5 = "audio_video";

            QStringList new_columns_name_list_5;
            new_columns_name_list_5 << "Creator_Bundle_id" <<"original_latitude" << "original_longitude"
                                    <<"original_colorspace" << "face_area_points" <<"timezone_name"
                                   << "timezone_offset" <<"exif_timestamp" <<"last_shared_date" <<"scence_analysis_timestamp";




            QStringList columns_field_type_list_5;
            columns_field_type_list_5 << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)"
                                      << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)"
                                      << "VARCHAR(1024)" <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_5),new_columns_name_list_5, columns_field_type_list_5, destination_db_fs_events);

            destination_db_fs_events.close();
        }
    }
}

void MainWindow::update_result_plugin_mozilla_firefox_129()
{
    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_mozilla_fire_fox = "Mozilla Firefox";
    QString plgn_nm_tmp_mozilla_fire_fox = plgn_nm_real_mozilla_fire_fox;
    plgn_nm_tmp_mozilla_fire_fox.replace(" ", "_");


    QString plgn_csv_file_in_result_mozilla_firefox  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_mozilla_fire_fox +".csv";
    QString plgn_csv_file_in_app_bundle_mozilla_firefox  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_mozilla_fire_fox + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_mozilla_firefox, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_mozilla_firefox, plgn_csv_file_in_result_mozilla_firefox);

    csv_reader *csv_reader_local_mozilla_firefox = new csv_reader(this);
    csv_reader_local_mozilla_firefox->do_job_plugin_specific(plgn_csv_file_in_result_mozilla_firefox, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_nm_real_mozilla_fire_fox, csv_reader_local_mozilla_firefox);
    update_load_case_artifacts_tab_entry_config(plgn_nm_real_mozilla_fire_fox, csv_reader_local_mozilla_firefox);


    // in mozilla firefox artifact data
    QString mozilla_firefox_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_mozilla_fire_fox + "/" + plgn_nm_tmp_mozilla_fire_fox + ".sqlite";
    if(QFileInfo(mozilla_firefox_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_fs_events = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_fs_events.setDatabaseName(mozilla_firefox_db_path);
        if(!destination_db_fs_events.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_nm_real_mozilla_fire_fox + destination_db_fs_events.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_fs_events.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_fs_events.isOpen())
        {
            QString table_name = "downloads";

            QStringList new_columns_name_list;
            new_columns_name_list << "uid_file_path" <<"uid_file_name";

            QStringList columns_field_type_list;
            columns_field_type_list << "VARCHAR(1024)"<< "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name),new_columns_name_list, columns_field_type_list, destination_db_fs_events);

            QString table_name_2 = "bookmarks";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 << "url_hash";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(table_name_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_fs_events);

            destination_db_fs_events.close();

        }
    }
}



void MainWindow::update_result_plugin_opera_mini_125()
{
    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_real_opera_mini = "Opera Mini";
    QString plgn_tmp_opera_mini = plgn_real_opera_mini;
    plgn_tmp_opera_mini.replace(" ", "_");


    QString plgn_csv_in_result_opera_mini      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_opera_mini +".csv";
    QString plgn_csv_in_app_bundle_opera_mini  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_opera_mini + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_in_result_opera_mini, Q_FUNC_INFO);
    QFile::copy(plgn_csv_in_app_bundle_opera_mini, plgn_csv_in_result_opera_mini);

    csv_reader *csv_reader_local_opera_mini = new csv_reader(this);
    csv_reader_local_opera_mini->do_job_plugin_specific(plgn_csv_in_result_opera_mini, Q_FUNC_INFO);


    QStringList old_tab_list_operamini, new_tab_list_operamini;
    old_tab_list_operamini << "Synchronisation";
    new_tab_list_operamini << "Synchronization";
    update_load_case_artifacts_tab_rename_config(plgn_real_opera_mini, old_tab_list_operamini, new_tab_list_operamini);


    // in 'Opera Mini' artifact data
    QString opera_mini_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_opera_mini + "/" + plgn_tmp_opera_mini + ".sqlite";
    if(QFileInfo(opera_mini_db_path).exists())
    {
        QString connection_naam_3 = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_3);
        QSqlDatabase destination_db_opera_mini = QSqlDatabase::addDatabase("QSQLITE",connection_naam_3);
        destination_db_opera_mini.setDatabaseName(opera_mini_db_path);
        if(!destination_db_opera_mini.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_real_opera_mini + destination_db_opera_mini.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_opera_mini.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_opera_mini.isOpen())
        {
            QString new_tablename = "synchronization";

            QString cmd_altr = "ALTER TABLE synchronisation RENAME TO " + new_tablename;
            recon_helper_standard_obj->execute_db_command_by_dbreference(cmd_altr, destination_db_opera_mini, Q_FUNC_INFO);

            destination_db_opera_mini.close();
        }
    }
}



void MainWindow::update_result_plugin_network_logs_133()
{
    QString plgn_network_logs = "Network Logs";
    QString plgn_tmp_network_logs = plgn_network_logs;
    plgn_tmp_network_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_network_logs      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_network_logs +".csv";
    QString plgn_csv_file_in_app_bundle_network_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_network_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_network_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_network_logs, plgn_csv_file_in_result_network_logs);

    csv_reader *csv_reader_local_network_logs = new csv_reader(this);
    csv_reader_local_network_logs->do_job_plugin_specific(plgn_csv_file_in_result_network_logs, Q_FUNC_INFO);

    update_load_case_artifacts_tab_entry_config(plgn_network_logs, csv_reader_local_network_logs);

}

void MainWindow::update_result_plugin_user_logs_133()
{
    QString plgn_user_logs = "User Logs";
    QString plgn_tmp_user_logs = plgn_user_logs;
    plgn_tmp_user_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_user_logs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_user_logs +".csv";
    QString plgn_csv_file_in_app_bundle_user_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_user_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_user_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_user_logs, plgn_csv_file_in_result_user_logs);

    csv_reader *csv_reader_local_user_logs = new csv_reader(this);
    csv_reader_local_user_logs->do_job_plugin_specific(plgn_csv_file_in_result_user_logs, Q_FUNC_INFO);

    update_load_case_artifacts_tab_entry_config(plgn_user_logs, csv_reader_local_user_logs);

}




void MainWindow::update_result_plugin_brave_browser_132()
{
    QString plgn_real_brave_browser = "Brave Browser";
    QString plgn_tmp_brave_browser = plgn_real_brave_browser;
    plgn_tmp_brave_browser.replace(" ", "_");


    QString plgn_result_csv_brave_browser = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_brave_browser +".csv";
    QString plgn_csv_app_bundle_brave_browser  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_brave_browser + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_brave_browser, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_brave_browser, plgn_result_csv_brave_browser);

    csv_reader *csv_reader_local_brave_browser = new csv_reader(this);
    csv_reader_local_brave_browser->do_job_plugin_specific(plgn_result_csv_brave_browser, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_brave_browser, csv_reader_local_brave_browser);
    update_load_case_artifacts_tab_entry_config(plgn_real_brave_browser, csv_reader_local_brave_browser);


    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Brave Browser artifact data
    QString brave_browser_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_brave_browser + "/" + plgn_tmp_brave_browser + ".sqlite";
    if(QFileInfo(brave_browser_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_brave_browser = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_brave_browser.setDatabaseName(brave_browser_db_path);
        if(!destination_db_brave_browser.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_brave_browser + destination_db_brave_browser.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_brave_browser.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_brave_browser.isOpen())
        {
            QString new_tablename = "login_data";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"date_synced "<<"password";

            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER" <<"VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_brave_browser);
            //====

            destination_db_brave_browser.close();

        }
    }
}

void MainWindow::update_result_plugin_podcasts_133()
{
    QString plgn_podcast = "Podcasts";
    QString plgn_tmp_podcast = plgn_podcast;
    plgn_tmp_podcast.replace(" ", "_");

    QString plgn_csv_file_in_result_podcast = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_podcast +".csv";
    QString plgn_csv_file_in_app_bundle_podcast  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_podcast + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_podcast, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_podcast, plgn_csv_file_in_result_podcast);

    csv_reader *csv_reader_local_podcast = new csv_reader(this);
    csv_reader_local_podcast->do_job_plugin_specific(plgn_csv_file_in_result_podcast, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_podcast, csv_reader_local_podcast);
    update_load_case_artifacts_tab_entry_config(plgn_podcast, csv_reader_local_podcast);

}

void MainWindow::update_result_plugin_utorrent_web_133()
{
    QString plgn_real_utorrent_web = "uTorrent Web";
    QString plgn_tmp_utorrent_web = plgn_real_utorrent_web;
    plgn_tmp_utorrent_web.replace(" ", "_");


    QString plgn_result_csv_utorrent_web = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_utorrent_web +".csv";
    QString plgn_csv_app_bundle_utorrent_web  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_utorrent_web + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_utorrent_web, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_utorrent_web, plgn_result_csv_utorrent_web);

    csv_reader *csv_reader_local_utorrent_web = new csv_reader(this);
    csv_reader_local_utorrent_web->do_job_plugin_specific(plgn_result_csv_utorrent_web, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_utorrent_web, csv_reader_local_utorrent_web);
    update_load_case_artifacts_tab_entry_config(plgn_real_utorrent_web, csv_reader_local_utorrent_web);

    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in uTorrent Web artifact data
    QString utorrent_web_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_utorrent_web + "/" + plgn_tmp_utorrent_web + ".sqlite";
    if(QFileInfo(utorrent_web_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_utorrent_web = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_utorrent_web.setDatabaseName(utorrent_web_db_path);
        if(!destination_db_utorrent_web.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_utorrent_web + destination_db_utorrent_web.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_utorrent_web.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_utorrent_web.isOpen())
        {
            QString new_tablename = "torrent_files";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"torrent_path" <<"file_exists" <<"file_size"
                                 <<"content_length" <<"author" << "file_type" << "content_type"<<"orientation"<<"pixel_count"<<"pixel_height"
                                <<"pixel_width" <<"created_timestamp" <<"completed_timestamp" <<"last_modified_timestamp" <<"fs_name"
                               <<"fs_node_count" <<"date_created"<< "attr_date_added" <<"fs_creation_date" <<"fs_content_change_date"
                              <<"first_used_date" <<"last_used_date" <<"used_dates" <<"content_modification_date"<<"content_creation_date"
                             <<"codec" << "audio_bit_rate" <<"video_bit_rate" <<"total_bit_rate" <<"audio_channel_count" <<"duration"
                            <<"latitude" <<"longitude" <<"altitude" <<"wherefrom" <<"torrent_names" <<"torrent_file_list" <<"bookmarked_extended_attributes"
                           <<"bookmarked_exif_data" <<"extended_attributes_key_value" <<"extended_attributes_value_for_search" <<"preview_file_path"
                          <<"export_file_paths"<<"LINKFILENAME";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varcahr(1024)" <<"varchar(100)" <<"varchar(200)"
                                   <<"varchar(1024)" <<"varchar(1024)" << "INTEGER"<< "INTEGER"<< "INTEGER" <<"varchar(100)"<<"varchar(100)"
                                  <<"INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER" << "INTEGER"
                                 <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varcahr(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varcahr(1024)"
                                <<"varchar(1024)" <<"varchar(1024)" <<"varcahr(1024)" <<"varchar(1024)" <<"varchar(5000)" <<"varcahr(5000)"
                               <<"varchar(1024)" <<"varchar(1024)" <<"varcahr(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varcahr(1024)"
                              <<"varcahr(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_utorrent_web);
            //====

            destination_db_utorrent_web.close();

        }
    }


}

void MainWindow::update_result_plugin_screen_time_133()
{
    QString plgn_real_screen_time = "Screen Time";
    QString plgn_tmp_screen_time = plgn_real_screen_time;
    plgn_tmp_screen_time.replace(" ", "_");


    QString plgn_result_csv_screen_time = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_screen_time +".csv";
    QString plgn_csv_app_bundle_screen_time  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_screen_time + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_screen_time, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_screen_time, plgn_result_csv_screen_time);

    csv_reader *csv_reader_local_screen_time = new csv_reader(this);
    csv_reader_local_screen_time->do_job_plugin_specific(plgn_result_csv_screen_time, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_screen_time, csv_reader_local_screen_time);
    update_load_case_artifacts_tab_entry_config(plgn_real_screen_time, csv_reader_local_screen_time);

}

void MainWindow::update_result_plugin_tv_133()
{
    QString plgn_real_tv = "TV";
    QString plgn_tmp_tv = plgn_real_tv;
    plgn_tmp_tv.replace(" ", "_");


    QString plgn_result_csv_tv = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_tv +".csv";
    QString plgn_csv_app_bundle_tv  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_tv + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_tv, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_tv, plgn_result_csv_tv);

    csv_reader *csv_reader_local_tv = new csv_reader(this);
    csv_reader_local_tv->do_job_plugin_specific(plgn_result_csv_tv, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_tv, csv_reader_local_tv);
    update_load_case_artifacts_tab_entry_config(plgn_real_tv, csv_reader_local_tv);

}


void MainWindow::update_result_plugin_mail_135()
{
    QString plgn_real_mail = "Mail";
    QString plgn_tmp_mail = plgn_real_mail;
    plgn_tmp_mail.replace(" ", "_");


    QString plgn_result_csv_mail = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_mail +".csv";
    QString plgn_csv_app_bundle_mail  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_mail + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_mail, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_mail, plgn_result_csv_mail);

    csv_reader *csv_reader_local_mail = new csv_reader(this);
    csv_reader_local_mail->do_job_plugin_specific(plgn_result_csv_mail, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_mail, csv_reader_local_mail);
    update_load_case_artifacts_tab_entry_config(plgn_real_mail, csv_reader_local_mail);

}

void MainWindow::update_result_plugin_brave_browser_135()
{
    QString plgn_real_brave_browser = "Brave Browser";
    QString plgn_tmp_brave_browser = plgn_real_brave_browser;
    plgn_tmp_brave_browser.replace(" ", "_");


    QString plgn_result_csv_brave_browser = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_brave_browser +".csv";
    QString plgn_csv_app_bundle_brave_browser  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_brave_browser + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_brave_browser, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_brave_browser, plgn_result_csv_brave_browser);

    csv_reader *csv_reader_local_brave_browser = new csv_reader(this);
    csv_reader_local_brave_browser->do_job_plugin_specific(plgn_result_csv_brave_browser, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_brave_browser, csv_reader_local_brave_browser);
    update_load_case_artifacts_tab_entry_config(plgn_real_brave_browser, csv_reader_local_brave_browser);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Brave Browser artifact data
    QString brave_browser_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_brave_browser + "/" + plgn_tmp_brave_browser + ".sqlite";
    if(QFileInfo(brave_browser_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_brave_browser = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_brave_browser.setDatabaseName(brave_browser_db_path);
        if(!destination_db_brave_browser.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_brave_browser + destination_db_brave_browser.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_brave_browser.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_brave_browser.isOpen())
        {
            //====
            QString new_tablename = "history";

            QStringList new_columns_name_list;
            new_columns_name_list <<"hidden";

            QStringList columns_field_type_list;
            columns_field_type_list <<"VARCHAR(100)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_brave_browser);
            //====

            //====
            QString new_tablename_2 = "downloads";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"guid" <<"etag" <<"current_path" <<"interrupt_reason" <<"creation_date" <<"modification_timestamp";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"VARCHAR(100)" <<"VARCHAR(100)" <<"VARCHAR(1024)" <<"VARCHAR(1024)" << "INTEGER" << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_brave_browser);
            //====

            //====
            QString new_tablename_3 = "topsites";

            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"url_rank";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 <<"VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_brave_browser);
            //====

            //====
            QString new_tablename_4 = "autofill";

            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"guid" <<"company_name" <<"street_address" <<"city" <<"state" <<"email_id" <<"zipcode" <<"contact_no" <<"country_code" <<"date_modified";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 <<"VARCHAR(100)" <<"VARCHAR(100)" <<"VARCHAR(1024)" <<"VARCHAR(1024)" <<"VARCHAR(100)" <<"VARCHAR(100)" <<"VARCHAR(1024)" <<"VARCHAR(1024)" <<"VARCHAR(1024)" << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_brave_browser);

            //====

            //====
            QString new_tablename_5 = "bookmarks";

            QStringList new_columns_name_list_5;
            new_columns_name_list_5 <<"sync_timestamp";

            QStringList columns_field_type_list_5;
            columns_field_type_list_5 << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_5),new_columns_name_list_5, columns_field_type_list_5, destination_db_brave_browser);
            //====
            destination_db_brave_browser.close();

        }
    }
}

void MainWindow::update_result_plugin_stocks_135()
{
    QString plgn_real_stocks = "Stocks";
    QString plgn_tmp_stocks = plgn_real_stocks;
    plgn_tmp_stocks.replace(" ", "_");


    QString plgn_result_csv_stocks = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_stocks +".csv";
    QString plgn_csv_app_bundle_stocks  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_stocks + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_stocks, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_stocks, plgn_result_csv_stocks);

    csv_reader *csv_reader_local_utorrent_web = new csv_reader(this);
    csv_reader_local_utorrent_web->do_job_plugin_specific(plgn_result_csv_stocks, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_stocks, csv_reader_local_utorrent_web);
    update_load_case_artifacts_tab_entry_config(plgn_real_stocks, csv_reader_local_utorrent_web);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in stocks artifact data
    QString stocks_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_stocks + "/" + plgn_tmp_stocks + ".sqlite";
    if(QFileInfo(stocks_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_stocks = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_stocks.setDatabaseName(stocks_db_path);
        if(!destination_db_stocks.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_stocks + destination_db_stocks.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_stocks.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_stocks.isOpen())
        {
            QString new_tablename = "stocks_data";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"currency" <<"open_fmt" <<"yearLow_fmt" <<"yearHigh_fmt" <<"low_fmt" <<"high_fmt" <<"averageVolume_fmt";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)"<<"varchar(1024)"<<"varchar(1024)"<<"varchar(1024)"<<"varchar(1024)"<<"varchar(1024)"<<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_stocks);
            //====

            destination_db_stocks.close();

        }
    }
}

void MainWindow::update_result_plugin_utorrent_web_136()
{
    QString plgn_real_utorrent_web = "uTorrent Web";
    QString plgn_tmp_utorrent_web = plgn_real_utorrent_web;
    plgn_tmp_utorrent_web.replace(" ", "_");


    QString plgn_result_csv_utorrent_web = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_utorrent_web +".csv";
    QString plgn_csv_app_bundle_utorrent_web  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_utorrent_web + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_utorrent_web, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_utorrent_web, plgn_result_csv_utorrent_web);

    csv_reader *csv_reader_local_utorrent_web = new csv_reader(this);
    csv_reader_local_utorrent_web->do_job_plugin_specific(plgn_result_csv_utorrent_web, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_utorrent_web, csv_reader_local_utorrent_web);
    update_load_case_artifacts_tab_entry_config(plgn_real_utorrent_web, csv_reader_local_utorrent_web);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in uTorrent Web artifact data
    QString utorrent_web_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_utorrent_web + "/" + plgn_tmp_utorrent_web + ".sqlite";
    if(QFileInfo(utorrent_web_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_utorrent_web = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_utorrent_web.setDatabaseName(utorrent_web_db_path);
        if(!destination_db_utorrent_web.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_utorrent_web + destination_db_utorrent_web.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_utorrent_web.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_utorrent_web.isOpen())
        {
            QString new_tablename = "torrent_files";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"target_path";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_utorrent_web);
            //====

            destination_db_utorrent_web.close();

        }
    }
}

void MainWindow::update_result_plugin_brave_browser_136()
{
    QString plgn_real_brave_browser = "Brave Browser";
    QString plgn_tmp_brave_browser = plgn_real_brave_browser;
    plgn_tmp_brave_browser.replace(" ", "_");


    QString plgn_result_csv_brave_browser = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_brave_browser +".csv";
    QString plgn_csv_app_bundle_brave_browser  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_brave_browser + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_brave_browser, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_brave_browser, plgn_result_csv_brave_browser);

    csv_reader *csv_reader_local_brave_browser = new csv_reader(this);
    csv_reader_local_brave_browser->do_job_plugin_specific(plgn_result_csv_brave_browser, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_brave_browser, csv_reader_local_brave_browser);
    update_load_case_artifacts_tab_entry_config(plgn_real_brave_browser, csv_reader_local_brave_browser);


    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    // in Brave Browser artifact data
    QString brave_browser_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_brave_browser + "/" + plgn_tmp_brave_browser + ".sqlite";
    if(QFileInfo(brave_browser_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_brave_browser = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_brave_browser.setDatabaseName(brave_browser_db_path);
        if(!destination_db_brave_browser.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_brave_browser + destination_db_brave_browser.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_brave_browser.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_brave_browser.isOpen())
        {
            //====
            QString new_tablename = "history";

            QStringList new_columns_name_list;
            new_columns_name_list <<"domain";

            QStringList columns_field_type_list;
            columns_field_type_list <<"VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_brave_browser);
            //====

            //====
            QString new_tablename_2 = "bookmarks";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"date_created"<<"last_visited_date"<<"custom_title" <<"domain" <<"display_uid" <<"sync_order"<<"order_no";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "INTEGER"<< "INTEGER"<<"VARCHAR(1024)"<<"VARCHAR(1024)"<<"VARCHAR(1024)"<<"VARCHAR(1024)"<<"VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_brave_browser);
            //====

            //====
            QString new_tablename_3 = "favicons";

            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"date_created"<<"domain" ;

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 << "INTEGER"<<"VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_brave_browser);
            //====

            destination_db_brave_browser.close();

        }
    }
}

void MainWindow::update_result_plugin_podcasts_136()
{
    QString plgn_real_podcasts = "Podcasts";
    QString plgn_tmp_podcasts = plgn_real_podcasts;
    plgn_tmp_podcasts.replace(" ", "_");


    QString plgn_result_csv_podcasts = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_podcasts +".csv";
    QString plgn_csv_app_bundle_podcasts  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_podcasts + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_podcasts, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_podcasts, plgn_result_csv_podcasts);

    csv_reader *csv_reader_local_podcasts = new csv_reader(this);
    csv_reader_local_podcasts->do_job_plugin_specific(plgn_result_csv_podcasts, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_podcasts, csv_reader_local_podcasts);
    update_load_case_artifacts_tab_entry_config(plgn_real_podcasts, csv_reader_local_podcasts);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Podcasts artifact data
    QString podcasts_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_podcasts + "/" + plgn_tmp_podcasts + ".sqlite";
    if(QFileInfo(podcasts_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_podcasts = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_podcasts.setDatabaseName(podcasts_db_path);
        if(!destination_db_podcasts.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_podcasts + destination_db_podcasts.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_podcasts.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_podcasts.isOpen())
        {
            QString new_tablename = "full_list_episode";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"publish_timestamp" <<"item_description"<<"itunes_subtitle";

            QStringList columns_field_type_list;
            columns_field_type_list <<"INTEGER"<<"varchar(1024)"<<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_podcasts);
            //====

            destination_db_podcasts.close();

        }
    }
}

void MainWindow::update_result_plugin_screen_time_136()
{
    QString plgn_real_screen_time = "Screen Time";
    QString plgn_tmp_screen_time = plgn_real_screen_time;
    plgn_tmp_screen_time.replace(" ", "_");


    QString plgn_result_csv_screen_time = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_screen_time +".csv";
    QString plgn_csv_app_bundle_screen_time  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_screen_time + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_screen_time, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_screen_time, plgn_result_csv_screen_time);

    csv_reader *csv_reader_local_screen_time = new csv_reader(this);
    csv_reader_local_screen_time->do_job_plugin_specific(plgn_result_csv_screen_time, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_screen_time, csv_reader_local_screen_time);
    update_load_case_artifacts_tab_entry_config(plgn_real_screen_time, csv_reader_local_screen_time);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Screen Time artifact data
    QString screen_time_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_screen_time + "/" + plgn_tmp_screen_time + ".sqlite";
    if(QFileInfo(screen_time_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_screen_time = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_screen_time.setDatabaseName(screen_time_db_path);
        if(!destination_db_screen_time.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_screen_time + destination_db_screen_time.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_screen_time.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_screen_time.isOpen())
        {
            QString new_tablename = "application_usage";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"preview_file_path";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_screen_time);
            //====


            QString new_tablename_2 = "notifications";

            //====
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"preview_file_path";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_screen_time);
            //====

            destination_db_screen_time.close();

        }
    }
}


void MainWindow::update_result_plugin_fs_events_137()
{
    QString plgn_fs_events = "FS Events";
    QString plgn_tmp_fs_events = plgn_fs_events;
    plgn_tmp_fs_events.replace(" ", "_");

    QString plgn_csv_file_in_result_fs_events  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_fs_events +".csv";
    QString plgn_csv_file_in_app_bundle_fs_events  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_fs_events + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_fs_events, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_fs_events, plgn_csv_file_in_result_fs_events);

}

void MainWindow::update_result_plugin_connected_ios_devices_138()
{
    QString plgn_connected_iOS_devices = "Connected iOS Devices";
    QString plgn_tmp_connected_iOS_devices = plgn_connected_iOS_devices;
    plgn_tmp_connected_iOS_devices.replace(" ", "_");

    QString plgn_csv_file_in_result_connected_iOS_devices  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_connected_iOS_devices +".csv";
    QString plgn_csv_file_in_app_bundle_connected_iOS_devices  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_connected_iOS_devices + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_connected_iOS_devices, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_connected_iOS_devices, plgn_csv_file_in_result_connected_iOS_devices);

}

void MainWindow::update_result_plugin_finder_138()
{
    QString plgn_finder = "Finder";
    QString plgn_tmp_finder = plgn_finder;
    plgn_tmp_finder.replace(" ", "_");

    QString plgn_csv_file_in_result_finder = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_finder +".csv";
    QString plgn_csv_file_in_app_bundle_finder  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_finder + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_finder, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_finder, plgn_csv_file_in_result_finder);

    csv_reader *csv_reader_local_finder = new csv_reader(this);
    csv_reader_local_finder->do_job_plugin_specific(plgn_csv_file_in_result_finder, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_finder, csv_reader_local_finder);

    QStringList old_tab_list_finder, new_tab_list_finder;
    old_tab_list_finder << "Recent Open Folder" << "Open Tabs";
    new_tab_list_finder << "Recently Opened Folders" << "Current Opened Tabs";
    update_load_case_artifacts_tab_rename_config(plgn_tmp_finder, old_tab_list_finder, new_tab_list_finder);
    update_load_case_artifacts_tab_entry_config(plgn_tmp_finder, csv_reader_local_finder);
}



void MainWindow::update_result_plugin_recent_items_138()
{
    QString plgn_recent_items = "Recent Items";
    QString plgn_tmp_recent_items = plgn_recent_items;
    plgn_tmp_recent_items.replace(" ", "_");

    QString plgn_csv_file_in_result_recent_items = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_recent_items +".csv";
    QString plgn_csv_file_in_app_bundle_recent_items  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_recent_items + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_recent_items, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_recent_items, plgn_csv_file_in_result_recent_items);

    csv_reader *csv_reader_local_recent_items = new csv_reader(this);
    csv_reader_local_recent_items->do_job_plugin_specific(plgn_csv_file_in_result_recent_items, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_recent_items, csv_reader_local_recent_items);

    update_load_case_artifacts_tab_entry_config(plgn_recent_items, csv_reader_local_recent_items);
}


void MainWindow::update_result_plugin_reminders_139()
{
    QString plgn_real_reminders = "Reminders";
    QString plgn_tmp_reminders = plgn_real_reminders;
    plgn_tmp_reminders.replace(" ", "_");


    QString plgn_result_csv_reminders = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_reminders +".csv";
    QString plgn_csv_app_bundle_reminders  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_reminders + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_reminders, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_reminders, plgn_result_csv_reminders);

    csv_reader *csv_reader_local_reminders = new csv_reader(this);
    csv_reader_local_reminders->do_job_plugin_specific(plgn_result_csv_reminders, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_reminders, csv_reader_local_reminders);
    update_load_case_artifacts_tab_entry_config(plgn_real_reminders, csv_reader_local_reminders);

    QString result_dir_path    = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Reminders artifact data
    QString reminders_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_reminders + "/" + plgn_tmp_reminders + ".sqlite";
    if(QFileInfo(reminders_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_reminders = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_reminders.setDatabaseName(reminders_db_path);
        if(!destination_db_reminders.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_reminders + destination_db_reminders.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_reminders.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_reminders.isOpen())
        {
            QString new_tablename = "db_reminders";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"display_date" << "last_banner_presentation_date" << "is_all_day" << "is_completed";

            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER" << "INTEGER" << "VARCHAR(1024)" << "VARCHAR(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_reminders);
            //====

            destination_db_reminders.close();

        }
    }


}

void MainWindow::update_result_plugin_photos_139()
{
    QString plgn_real_photos = "Photos";
    QString plgn_tmp_photos = plgn_real_photos;
    plgn_tmp_photos.replace(" ", "_");


    QString plgn_result_csv_photos = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_photos +".csv";
    QString plgn_csv_app_bundle_photos  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_photos + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_photos, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_photos, plgn_result_csv_photos);

    csv_reader *csv_reader_local_photos = new csv_reader(this);
    csv_reader_local_photos->do_job_plugin_specific(plgn_result_csv_photos, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_photos, csv_reader_local_photos);
    update_load_case_artifacts_tab_entry_config(plgn_real_photos, csv_reader_local_photos);

}


