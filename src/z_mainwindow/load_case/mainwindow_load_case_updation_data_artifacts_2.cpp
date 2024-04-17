#include "z_mainwindow/mainwindow.h"


void MainWindow::update_result_plugin_whatsapp_140()
{
    QString plgn_real_whatsapp = "WhatsApp";
    QString plgn_tmp_whatsapp = plgn_real_whatsapp;
    plgn_tmp_whatsapp.replace(" ", "_");


    QString plgn_result_csv_whatsapp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_whatsapp +".csv";
    QString plgn_csv_app_bundle_whatsapp  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_whatsapp + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_whatsapp, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_whatsapp, plgn_result_csv_whatsapp);

    csv_reader *csv_reader_local_whatsapp = new csv_reader(this);
    csv_reader_local_whatsapp->do_job_plugin_specific(plgn_result_csv_whatsapp, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_whatsapp, csv_reader_local_whatsapp);
    update_load_case_artifacts_tab_entry_config(plgn_real_whatsapp, csv_reader_local_whatsapp);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in WhatsApp artifact data
    QString whatsapp_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_whatsapp + "/" + plgn_tmp_whatsapp + ".sqlite";
    if(QFileInfo(whatsapp_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_whatsaap = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_whatsaap.setDatabaseName(whatsapp_db_path);
        if(!destination_db_whatsaap.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_whatsapp + destination_db_whatsaap.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_whatsaap.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_whatsaap.isOpen())
        {
            QString new_tablename = "Messages";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"needs_push" <<"remote_resource_id" <<"recipient_count" <<"participant_hash" <<"media_enc_hash" <<"forwarded" << "preview_type" <<"send_count"
                                 <<"received_timestamp" <<"receipt_server_timestamp" << "receipt_device_timestamp" <<"read_device_timestamp"
                                <<"played_device_timestamp";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_whatsaap);
            //====


            QString new_tablename_2 = "contacts";

            //====
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"status_timestamp" <<"phone_label" <<"photo_id_timestamp" <<"keep_timestamp" <<"photo_timestamp"<<"thumb_timestamp"
                                   <<"whatsapp_id"<<"display_name"<<"is_whatsapp_user" <<"status" <<"raw_contact_id" <<"unseen_msg_count" <<"given_name" <<"family_name"
                                  <<"wa_name" <<"sort_name" <<"company" <<"title" <<"status_autodownload_disabled" <<"is_spam_reported" <<"is_business_synced";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_whatsaap);
            //====


            QString new_tablename_3 = "media_messages";

            //====
            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"media_name" <<"autotransfer_retry_enabled" <<"transferred" <<"suspicious_content" <<"has_streaming_sidecar" <<"thumbnail_height_width_ratio"
                                   <<"direct_path" <<"first_scan_sidecar" <<"first_scan_length" <<"mime_type" <<"file_length" <<"page_count" <<"enc_file_hash"
                                  <<"partial_media_hash" <<"partial_media_enc_hash" <<"is_animated_sticker";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_whatsaap);
            //====

            QString new_tablename_4 = "account";

            //====
            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"status_timestamp" <<"phone_label" <<"photo_id_timestamp" <<"keep_timestamp" <<"photo_timestamp"<<"thumb_timestamp"
                                   <<"display_name"<<"is_whatsapp_user" <<"status" <<"raw_contact_id" <<"unseen_msg_count" <<"given_name" <<"family_name"
                                  <<"wa_name" <<"sort_name" <<"company" <<"title" <<"status_autodownload_disabled" <<"is_spam_reported" <<"is_business_synced";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_whatsaap);

            destination_db_whatsaap.close();

        }
    }
}

void MainWindow::update_result_plugin_textme_140()
{
    QString plgn_real_textme = "TextMe";
    QString plgn_tmp_textme = plgn_real_textme;
    plgn_tmp_textme.replace(" ", "_");


    QString plgn_result_csv_textme = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_textme +".csv";
    QString plgn_csv_app_bundle_textme  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_textme + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_textme, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_textme, plgn_result_csv_textme);

    csv_reader *csv_reader_local_textme = new csv_reader(this);
    csv_reader_local_textme->do_job_plugin_specific(plgn_result_csv_textme, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_textme, csv_reader_local_textme);
    update_load_case_artifacts_tab_entry_config(plgn_real_textme, csv_reader_local_textme);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in TextMe artifact data
    QString textme_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_textme + "/" + plgn_tmp_textme + ".sqlite";
    if(QFileInfo(textme_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_textme = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_textme.setDatabaseName(textme_db_path);
        if(!destination_db_textme.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_textme + destination_db_textme.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_textme.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_textme.isOpen())
        {
            //====
            QString new_tablename = "Message";

            QStringList new_columns_name_list;
            new_columns_name_list <<"status" <<"uuid" ;

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_textme);
            //====


            //====
            QString new_tablename_2 = "calls";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"timestamp" <<"call_id" <<"type";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_textme);
            //====


            //====
            QString new_tablename_3 = "account";

            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"first_name" <<"profile_picture_path" <<"last_name" <<"country_code" <<"logged_in" <<"age"
                                   <<"birth_date" <<"last_message_uuid" <<"next_message_uuid" <<"preview_file_path" <<"export_file_paths";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_textme);
            //====


            //====
            QString new_tablename_4 = "contacts";

            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"last_name" <<"first_name";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_textme);
            //====

            //====
            QString new_tablename_5 = "attachments";

            QStringList new_columns_name_list_5;
            new_columns_name_list_5 <<"name";

            QStringList columns_field_type_list_5;
            columns_field_type_list_5 <<"varchar(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_5),new_columns_name_list_5, columns_field_type_list_5, destination_db_textme);
            //====

            //====
            QString new_tablename_6 = "stckr_package";

            QStringList new_columns_name_list_6;
            new_columns_name_list_6 <<"file_name" <<"file_path" <<"file_size" <<"stickers"
                                   << "author" << "url" << "weight" << "version" <<"sample_image";

            QStringList columns_field_type_list_6;
            columns_field_type_list_6 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_6),new_columns_name_list_6, columns_field_type_list_6, destination_db_textme);
            //====
            destination_db_textme.close();

        }
    }
}

void MainWindow::update_result_plugin_uc_browser_140()
{
    QString plgn_real_uc_browser = "UC Browser";
    QString plgn_tmp_uc_browser = plgn_real_uc_browser;
    plgn_tmp_uc_browser.replace(" ", "_");


    QString plgn_result_csv_uc_browser = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_uc_browser +".csv";
    QString plgn_csv_app_bundle_uc_browser  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_uc_browser + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_uc_browser, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_uc_browser, plgn_result_csv_uc_browser);

    csv_reader *csv_reader_local_uc_browser = new csv_reader(this);
    csv_reader_local_uc_browser->do_job_plugin_specific(plgn_result_csv_uc_browser, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_uc_browser, csv_reader_local_uc_browser);
    update_load_case_artifacts_tab_entry_config(plgn_real_uc_browser, csv_reader_local_uc_browser);
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in UC Browser artifact data
    QString textme_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_uc_browser + "/" + plgn_tmp_uc_browser + ".sqlite";
    if(QFileInfo(textme_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_textme = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_textme.setDatabaseName(textme_db_path);
        if(!destination_db_textme.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_uc_browser + destination_db_textme.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_textme.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_textme.isOpen())
        {
            //====
            QString new_tablename = "history";

            QStringList new_columns_name_list;
            new_columns_name_list <<"reference_count";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_textme);
            //====

            destination_db_textme.close();
        }
    }
}

void MainWindow::update_result_plugin_tv_140()
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

void MainWindow::update_result_plugin_brave_browser_140()
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

void MainWindow::update_result_plugin_podcasts_140()
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
            //====
            QString new_tablename = "podcast";

            QStringList new_columns_name_list;
            new_columns_name_list <<"podcast" <<"next_episode_name" <<"auto_download" <<"auto_download_type" <<"deleted_played_episodes"
                                 <<"notifications" <<"keep_episodes" <<"show_type_setting" <<"author_name" <<"author_name"
                                <<"hidden_status" <<"category_name" <<"playlist_name" <<"is_visible" <<"sort_order"<<"update_average"
                               <<"updatest_dev" <<"next_episode_uuid" <<"podcast_file_info" <<"latitude" <<"longitude" <<"altitude"<<"file_type"<<"content_type"<<"fs_name"
                              <<"fs_node_count" <<"audio_channel_count" <<"export_file_paths" <<"pixel_height" <<"pixel_width"<<"wherefrom" <<"torrent_file_list"
                             <<"preview_file_path" <<"extended_attributes_key_value" <<"extended_attributes_value_for_search"

                            <<"content_modification_date" <<"content_creation_date" <<"date_added"  <<"fs_creation_date"
                           <<"first_used_date" <<"last_used_date" <<"used_dates" <<"fs_content_change_date";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                  <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                               <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER";
            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_podcasts);
            //====


            //====
            QString new_tablename_2 = "category";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"parent_uuid";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"varchar(1024)";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_podcasts);
            //====


            //====
            QString new_tablename_3 = "full_list_episode";

            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"episode_type" <<"Guid" <<"audio" <<"video" <<"metadata_identifier" <<"podcast_uuid" <<"episode_level"
                                   <<"episode_number" <<"is_new" <<"sent_notifications" <<"playstate_manually_set" <<"supress_auto_download"
                                  <<"author_name" <<"episodes_status" <<"duration_time" <<"file_type_info" <<"itunes_title"
                                 <<"season_number" <<"item_description_without_html" <<"item_description_has_html" <<"is_from_itunes_sync" <<"unplayed_tab"
                                <<"manually_added" <<"episodes_file_info" <<"latitude"   <<"longitude" <<"altitude" <<"file_type" <<"content_type" <<"fs_name"
                               <<"fs_node_count" <<"audio_channel_count" <<"export_file_paths" <<"pixel_height" <<"pixel_width" <<"wherefrom" <<"torrent_file_list"
                              <<"preview_file_path" <<"extended_attributes_key_value" <<"extended_attributes_value_for_search"

                             <<"content_modification_date"    <<"content_creation_date" <<"date_added"  <<"fs_creation_date" <<"first_used_date"
                            <<"last_used_date" <<"used_dates" <<"fs_content_change_date" <<"metadata_timestamp" <<"download_date";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                  <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                               <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"

                              <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER"<<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_podcasts);
            //====

            //====
            QString new_tablename_4 = "playlist";

            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"hidden_status";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_podcasts);
            //====
            destination_db_podcasts.close();

        }
    }
}

void MainWindow::update_result_plugin_online_accounts_140()
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
    update_load_case_artifacts_tab_entry_config(plgn_real_online_accounts, csv_reader_local_online_accounts);

}

void MainWindow::update_result_plugin_skype_140()
{

    QString plgn_real_skype = "Skype";
    QString plgn_tmp_skype = plgn_real_skype;
    plgn_tmp_skype.replace(" ", "_");


    QString plgn_result_csv_skype = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_skype +".csv";
    QString plgn_csv_app_bundle_skype  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_skype + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_skype, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_skype, plgn_result_csv_skype);

    csv_reader *csv_reader_local_skype = new csv_reader(this);
    csv_reader_local_skype->do_job_plugin_specific(plgn_result_csv_skype, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_skype, csv_reader_local_skype);
    update_load_case_artifacts_tab_entry_config(plgn_real_skype, csv_reader_local_skype);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Skype artifact data
    QString skype_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_skype + "/" + plgn_tmp_skype + ".sqlite";

    if(QFileInfo(skype_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_skype = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_skype.setDatabaseName(skype_db_path);
        if(!destination_db_skype.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_skype + destination_db_skype.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_skype.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_skype.isOpen())
        {
            QString tablename = "call_members";


            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"original_arrival_time" << "compose_time" << "from_display_name" << "conversation_link" << "is_missed_call";

            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER" << "INTEGER" << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_skype);
            //====

            QString tablename_2 = "messages";

            //====
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"original_arrival_time" << "compose_time" << "receive_display_name" << "conversation_link" << "content_type" << "message_type";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "INTEGER" << "INTEGER" << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)"  << "VARCHAR(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_skype);
            //====

            destination_db_skype.close();

        }
    }


}

void MainWindow::update_result_plugin_icloud_140()
{
    QString plgn_real_icloud = "iCloud";
    QString plgn_tmp_icloud = plgn_real_icloud;
    plgn_tmp_icloud.replace(" ", "_");


    QString plgn_result_csv_icloud = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_icloud +".csv";
    QString plgn_csv_app_bundle_icloud  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_icloud + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_icloud, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_icloud, plgn_result_csv_icloud);

    csv_reader *csv_reader_local_icloud = new csv_reader(this);
    csv_reader_local_icloud->do_job_plugin_specific(plgn_result_csv_icloud, Q_FUNC_INFO);

    QStringList removable_plugin_list;
    removable_plugin_list << "Clients" << "Miscellaneous";
    update_load_case_artifacts_tab_remove_config(plgn_real_icloud,removable_plugin_list);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString icloud_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_icloud + "/" + plgn_tmp_icloud + ".sqlite";
    if(QFileInfo(icloud_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_icloud = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_icloud.setDatabaseName(icloud_db_path);
        if(!destination_db_icloud.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_icloud + destination_db_icloud.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_icloud.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_icloud.isOpen())
        {

            if(destination_db_icloud.tables().contains("clients"))
            {
                int index = destination_db_icloud.tables().indexOf("clients");
                QString cmd = QString("Drop table "  + destination_db_icloud.tables().at(index));

                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_icloud, Q_FUNC_INFO);
            }

            if(destination_db_icloud.tables().contains("miscellaneous"))
            {
                int index = destination_db_icloud.tables().indexOf("miscellaneous");
                QString cmd = QString("Drop table "  + destination_db_icloud.tables().at(index));
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_icloud, Q_FUNC_INFO);
            }
        }
        destination_db_icloud.close();
    }
}

void MainWindow::update_result_plugin_itunes_140()
{
    QString plgn_real_itunes = "iTunes";
    QString plgn_tmp_itunes = plgn_real_itunes;
    plgn_tmp_itunes.replace(" ", "_");


    QString plgn_result_csv_itunes = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_itunes +".csv";
    QString plgn_csv_app_bundle_itunes  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_itunes + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_itunes, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_itunes, plgn_result_csv_itunes);

    csv_reader *csv_reader_local_itunes = new csv_reader(this);
    csv_reader_local_itunes->do_job_plugin_specific(plgn_result_csv_itunes, Q_FUNC_INFO);

    QStringList removable_plugin_list;
    removable_plugin_list << "Devices";
    update_load_case_artifacts_tab_remove_config(plgn_real_itunes,removable_plugin_list);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString itunes_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_itunes + "/" + plgn_tmp_itunes + ".sqlite";
    if(QFileInfo(itunes_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_itunes = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_itunes.setDatabaseName(itunes_db_path);
        if(!destination_db_itunes.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_itunes + destination_db_itunes.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_itunes.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_itunes.isOpen())
        {
            QSqlQuery query_delete(destination_db_itunes);

            if(destination_db_itunes.tables().contains("Devices"))
            {
                int index = destination_db_itunes.tables().indexOf("Devices");
                QString cmd = QString("Drop table "  + destination_db_itunes.tables().at(index));
                recon_helper_standard_obj->execute_db_command_by_dbreference(cmd, destination_db_itunes, Q_FUNC_INFO);
            }
        }
        destination_db_itunes.close();
    }
}

void MainWindow::update_result_plugin_mail_140()
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

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Mail artifact data
    QString mail_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_mail + "/" + plgn_tmp_mail + ".sqlite";
    if(QFileInfo(mail_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_mail = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_mail.setDatabaseName(mail_db_path);
        if(!destination_db_mail.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_mail + destination_db_mail.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_mail.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_mail.isOpen())
        {
            //====
            QString new_tablename = "vip_contacts";

            QStringList new_columns_name_list;
            new_columns_name_list <<"message_count" << "recent_interaction_date";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"INTEGER";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_mail);
            //====
            destination_db_mail.close();

        }
    }
}

void MainWindow::update_result_plugin_skype_141()
{
    QString plgn_real_skype = "Skype";
    QString plgn_tmp_skype = plgn_real_skype;
    plgn_tmp_skype.replace(" ", "_");


    QString plgn_result_csv_skype = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_skype +".csv";
    QString plgn_csv_app_bundle_skype  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_skype + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_skype, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_skype, plgn_result_csv_skype);

    csv_reader *csv_reader_local_skype = new csv_reader(this);
    csv_reader_local_skype->do_job_plugin_specific(plgn_result_csv_skype, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_skype, csv_reader_local_skype);
    update_load_case_artifacts_tab_entry_config(plgn_real_skype, csv_reader_local_skype);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Skype artifact data
    QString skype_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_skype + "/" + plgn_tmp_skype + ".sqlite";
    if(QFileInfo(skype_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_skype = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_skype.setDatabaseName(skype_db_path);
        if(!destination_db_skype.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_skype + destination_db_skype.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_skype.lastError().text(),Q_FUNC_INFO);
        }


        if(destination_db_skype.isOpen())
        {
            QString tablename = "blocked_accounts";


            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"mail_id" << "thumbnail_url" << "fetched_date" << "assigned_phone4";

            QStringList columns_field_type_list;
            columns_field_type_list << "VARCHAR(1024)" << "VARCHAR(1024)" << "INTEGER"  << "VARCHAR(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_skype);
            //====

            QString tablename_2 = "call_members";

            //====
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"end_timestamp" << "connect_timestamp"<< "call_direction" << "call_type" << "call_state" ;

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "INTEGER" << "INTEGER" << "VARCHAR(1024)"  << "VARCHAR(1024)"  << "VARCHAR(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_skype);
            //====

            QString tablename_3 = "account";

            //====
            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"skype_user_account" << "skypename" << "province" << "phone_mobile" << "phone_home" << "phone_office" << "phone_other" << "gender" << "status_msg";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)" << "VARCHAR(1024)"  << "VARCHAR(1024)"  << "VARCHAR(1024)"  << "VARCHAR(1024)" << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_skype);
            //====

            QString tablename_4 = "contacts";

            //====
            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"mail_id" << "thumbnail_url" << "fetched_date" << "assigned_phone4";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 << "VARCHAR(1024)" << "VARCHAR(1024)" << "INTEGER"  << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_skype);
            //====

            //====
            QString tablename_5 = "messages";

            QStringList new_columns_name_list_5;
            new_columns_name_list_5 <<"valid_until_timestamp" << "location_timestamp";

            QStringList columns_field_type_list_5;
            columns_field_type_list_5 << "INTEGER" << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_5),new_columns_name_list_5, columns_field_type_list_5, destination_db_skype);
            //====

            destination_db_skype.close();

        }
    }
}

void MainWindow::update_result_plugin_viber_141()
{
    QString plgn_real_viber = "Viber";
    QString plgn_tmp_viber = plgn_real_viber;
    plgn_tmp_viber.replace(" ", "_");


    QString plgn_result_csv_viber = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_viber +".csv";
    QString plgn_csv_app_bundle_viber  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_viber + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_viber, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_viber, plgn_result_csv_viber);

    csv_reader *csv_reader_local_viber = new csv_reader(this);
    csv_reader_local_viber->do_job_plugin_specific(plgn_result_csv_viber, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_viber, csv_reader_local_viber);
    update_load_case_artifacts_tab_entry_config(plgn_real_viber, csv_reader_local_viber);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in viber artifact data
    QString viber_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_viber + "/" + plgn_tmp_viber + ".sqlite";
    if(QFileInfo(viber_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_viber = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_viber.setDatabaseName(viber_db_path);
        if(!destination_db_viber.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_viber + destination_db_viber.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_viber.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_viber.isOpen())
        {
            //====
            QString new_tablename = "all_contacts";

            QStringList new_columns_name_list;
            new_columns_name_list <<"is_ab_contact" <<"isviber" <<"encrypted_number" <<"encrypted_member_id"
                                 <<"viber_id" <<"dowload_id" <<"contact_flags" <<"date_of_birth"
                                <<"configured_account" <<"member_id" <<"contact_id" <<"timestamp";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                  <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_viber);
            //====

            //====
            QString new_tablename_2 = "accounts";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"token" << "email"<< "nick_name"<< "download_id"<< "photo_path";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_viber);
            //====

            //====
            QString new_tablename_3 = "calls";

            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"configured_account";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 <<"varchar(1024)" ;


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_viber);
            //====


            //====
            QString new_tablename_4 = "uploaded_files";

            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"object_id" <<"checksum" <<"encryption_parameter" <<"sender"
                                   <<"receiver" <<"is_read" <<"target_file_path" <<"configured_account"
                                  <<"modification_timestamp" <<"creation_date" <<"last_accessed_date";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_viber);
            //====

            //====
            QString new_tablename_5 = "downloaded_files";

            QStringList new_columns_name_list_5;
            new_columns_name_list_5 <<"download_id" <<"checksum" <<"temp_file_name" <<"sender"
                                   <<"receiver" <<"is_read" <<"target_file_path" <<"configured_account"
                                  <<"modification_timestamp" <<"creation_date" <<"last_accessed_date";

            QStringList columns_field_type_list_5;
            columns_field_type_list_5 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_5),new_columns_name_list_5, columns_field_type_list_5, destination_db_viber);
            //====

            //====
            QString new_tablename_6 = "viber_messages";

            QStringList new_columns_name_list_6;
            new_columns_name_list_6 <<"is_read" <<"direction" <<"has_attach" <<"thumbnail_path"
                                   <<"sticker_id" <<"client_flag" <<"chat_type" <<"events_flags"
                                  <<"chats_flags" <<"event_token" <<"sort_order" <<"attachment_details"
                                 <<"modification_timestamp" <<"creation_date" <<"last_accessed_date";

            QStringList columns_field_type_list_6;
            columns_field_type_list_6 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_6),new_columns_name_list_6, columns_field_type_list_6, destination_db_viber);
            //====

            //====
            QString new_tablename_7 = "media_messages";

            QStringList new_columns_name_list_7;
            new_columns_name_list_7 <<"configured_acc" <<"target_file_path" <<"original_file_size" <<"thumbnail_details"
                                   <<"last_modified" <<"creation_date" <<"last_accessed_date";

            QStringList columns_field_type_list_7;
            columns_field_type_list_7 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"INTEGER" <<"INTEGER" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_7),new_columns_name_list_7, columns_field_type_list_7, destination_db_viber);
            //====

            //====
            QString new_tablename_8 = "viber_contacts";

            QStringList new_columns_name_list_8;
            new_columns_name_list_8 <<"contact_id" <<"is_ab_contact" <<"viber_id" <<"dowload_id"
                                   <<"contact_flags" <<"date_of_birth" <<"configured_account"
                                  <<"timestamp";

            QStringList columns_field_type_list_8;
            columns_field_type_list_8 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_8),new_columns_name_list_8, columns_field_type_list_8, destination_db_viber);
            //====

            //====
            QString new_tablename_9 = "location_messages";

            QStringList new_columns_name_list_9;
            new_columns_name_list_9 <<"viber_account" <<"location_title" <<"domain" <<"alternative_url"
                                   <<"thumb_details" <<"alternative_location_text" <<"alternative_location_url"
                                  <<"alt_location_text" <<"location_custom_title" <<"media_type" <<"media_url"
                                 <<"uri" <<"location_action";

            QStringList columns_field_type_list_9;
            columns_field_type_list_9 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_9),new_columns_name_list_9, columns_field_type_list_9, destination_db_viber);
            //====

            destination_db_viber.close();

        }
    }
}
void MainWindow::update_result_plugin_voice_mail_142()
{
    QString plgn_real_voice_mail = "Voice Mail";
    QString plgn_tmp_voice_mail = plgn_real_voice_mail;
    plgn_tmp_voice_mail.replace(" ", "_");


    QString plgn_result_csv_voice_mail = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_voice_mail +".csv";
    QString plgn_csv_app_bundle_voice_mail  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_voice_mail + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_voice_mail, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_voice_mail, plgn_result_csv_voice_mail);

    csv_reader *csv_reader_local_voice_mail = new csv_reader(this);
    csv_reader_local_voice_mail->do_job_plugin_specific(plgn_result_csv_voice_mail, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_voice_mail, csv_reader_local_voice_mail);
    update_load_case_artifacts_tab_entry_config(plgn_real_voice_mail, csv_reader_local_voice_mail);

}

void MainWindow::update_result_plugin_knowledgec_142()
{
    QString plgn_real_knowledgec = "KnowledgeC";
    QString plgn_tmp_knowledgec = plgn_real_knowledgec;
    plgn_tmp_knowledgec.replace(" ", "_");


    QString plgn_result_csv_knowledgec = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_knowledgec +".csv";
    QString plgn_csv_app_bundle_knowledgec  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_knowledgec + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_knowledgec, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_knowledgec, plgn_result_csv_knowledgec);

    csv_reader *csv_reader_local_knowledgec = new csv_reader(this);
    csv_reader_local_knowledgec->do_job_plugin_specific(plgn_result_csv_knowledgec, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_knowledgec, csv_reader_local_knowledgec);
    update_load_case_artifacts_tab_entry_config(plgn_real_knowledgec, csv_reader_local_knowledgec);

}

void MainWindow::update_result_plugin_skype_142()
{
    QString plgn_real_skype = "Skype";
    QString plgn_tmp_skype = plgn_real_skype;
    plgn_tmp_skype.replace(" ", "_");

    QString plgn_result_csv_skype = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_skype +".csv";
    QString plgn_csv_app_bundle_skype  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_skype + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_skype, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_skype, plgn_result_csv_skype);

    csv_reader *csv_reader_local_skype = new csv_reader(this);
    csv_reader_local_skype->do_job_plugin_specific(plgn_result_csv_skype, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_skype, csv_reader_local_skype);
    update_load_case_artifacts_tab_entry_config(plgn_real_skype, csv_reader_local_skype);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString skype_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_skype + "/" + plgn_tmp_skype + ".sqlite";
    if(QFileInfo(skype_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_skype);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }
    }
}

void MainWindow::update_result_plugin_facetime_142()
{
    QString plgn_real_facetime = "FaceTime";
    QString plgn_tmp_facetime = plgn_real_facetime;
    plgn_tmp_facetime.replace(" ", "_");

    QString plgn_result_csv_facetime = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_facetime +".csv";
    QString plgn_csv_app_bundle_facetime  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_facetime + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_facetime, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_facetime, plgn_result_csv_facetime);

    csv_reader *csv_reader_local_facetime = new csv_reader(this);
    csv_reader_local_facetime->do_job_plugin_specific(plgn_result_csv_facetime, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_facetime, csv_reader_local_facetime);
    update_load_case_artifacts_tab_entry_config(plgn_real_facetime, csv_reader_local_facetime);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in FaceTime artifact data
    QString facetime_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_facetime + "/" + plgn_tmp_facetime + ".sqlite";
    if(QFileInfo(facetime_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_facetime);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }


        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_facetime = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_facetime.setDatabaseName(facetime_db_path);
        if(!destination_db_facetime.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_facetime.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Plugin Name -  " + plgn_tmp_facetime, Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_facetime.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_facetime.isOpen())
        {
            QString tablename = "known_contacts";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list << "look_up_date";

            QStringList columns_field_type_list;
            columns_field_type_list << "INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_facetime);
            //====

            QString tablename_1 = "account";


            //====
            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<"first_name" << "last_name";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 << "VARCHAR(1024)" << "VARCHAR(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_facetime);
            //====
        }
    }
}

void MainWindow::update_result_plugin_notes_142()
{
    QString plgn_real_notes = "Notes";
    QString plgn_tmp_notes = plgn_real_notes;
    plgn_tmp_notes.replace(" ", "_");

    QString plgn_result_csv_notes = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_notes +".csv";
    QString plgn_csv_app_bundle_notes  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_notes + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_notes, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_notes, plgn_result_csv_notes);

    csv_reader *csv_reader_local_notes = new csv_reader(this);
    csv_reader_local_notes->do_job_plugin_specific(plgn_result_csv_notes, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_notes, csv_reader_local_notes);
    update_load_case_artifacts_tab_entry_config(plgn_real_notes, csv_reader_local_notes);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Notes artifact data
    QString notes_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_notes + "/" + plgn_tmp_notes + ".sqlite";
    if(QFileInfo(notes_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_notes = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_notes.setDatabaseName(notes_db_path);
        if(!destination_db_notes.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_notes + destination_db_notes.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_notes.lastError().text(),Q_FUNC_INFO);
        }


        if(destination_db_notes.isOpen())
        {
            QString attributes_key_value_rename_str_cmd = "ALTER TABLE active_attachments RENAME COLUMN ext_attributes_key_value to extended_attributes_key_value";
            recon_helper_standard_obj->execute_db_command_by_dbreference(attributes_key_value_rename_str_cmd,destination_db_notes,Q_FUNC_INFO);

            QString attributes_key_value_search_rename_str_cmd = "ALTER TABLE active_attachments RENAME COLUMN ext_attributes_value_for_search to extended_attributes_value_for_search";
            recon_helper_standard_obj->execute_db_command_by_dbreference(attributes_key_value_search_rename_str_cmd,destination_db_notes,Q_FUNC_INFO);

        }
    }
}

void MainWindow::update_result_plugin_safari_143()
{
    QString plgn_real_safari = "Safari";
    QString plgn_tmp_safari = plgn_real_safari;
    plgn_tmp_safari.replace(" ", "_");

    QString plgn_result_csv_safari = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_safari +".csv";
    QString plgn_csv_app_bundle_safari  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_safari + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_safari, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_safari, plgn_result_csv_safari);

    csv_reader *csv_reader_local_safari = new csv_reader(this);
    csv_reader_local_safari->do_job_plugin_specific(plgn_result_csv_safari, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_safari, csv_reader_local_safari);
    update_load_case_artifacts_tab_entry_config(plgn_real_safari, csv_reader_local_safari);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in safari artifact data
    QString safari_db_path = result_dir_path + "Artifacts_Result/" + plgn_real_safari + "/" + plgn_real_safari + ".sqlite";
    if(QFileInfo(safari_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_safari = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_safari.setDatabaseName(safari_db_path);
        if(!destination_db_safari.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_safari.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_safari.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_safari.isOpen())
        {
            //====
            QString new_tablename = "extensions";

            QStringList new_columns_name_list;
            new_columns_name_list <<"extension_name";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_safari);
            //====

            //====
            QString new_tablename_1 = "history";

            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<"redirected_url_destination";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_safari);
            //====


            //====
            QString new_tablename_2 = "bookmarks";

            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"icon_url";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_safari);
            //====
        }
    }

}

void MainWindow::update_result_plugin_messages_143()
{
    QString plgn_real_messages = "Messages";
    QString plgn_tmp_messages = plgn_real_messages;
    plgn_tmp_messages.replace(" ", "_");

    QString plgn_result_csv_messages = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_messages +".csv";

    QString plgn_csv_app_bundle_messages  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_messages + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_messages, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_messages, plgn_result_csv_messages);

    csv_reader *csv_reader_local_messages = new csv_reader(this);
    csv_reader_local_messages->do_job_plugin_specific(plgn_result_csv_messages, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_messages, csv_reader_local_messages);
    update_load_case_artifacts_tab_entry_config(plgn_real_messages, csv_reader_local_messages);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString messages_db_path = result_dir_path + "Artifacts_Result/" + plgn_real_messages + "/" + plgn_real_messages + ".sqlite";
    if(QFileInfo(messages_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_messages = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_messages.setDatabaseName(messages_db_path);
        if(!destination_db_messages.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_messages + destination_db_messages.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_messages.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_messages.isOpen())
        {
            //====
            QString new_tablename = "recent_chats";

            QStringList new_columns_name_list;
            new_columns_name_list <<"last_accessed";

            QStringList columns_field_type_list;
            columns_field_type_list <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_messages);
            //====

            //====
            QString new_tablename_1 = "accounts";

            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<"person_first_name" << "person_last_name" << "application_name";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_messages);
            //====

        }
    }
}


void MainWindow::update_result_plugin_tv_143()
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

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in TV artifact data
    QString tv_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_tv + "/" + plgn_tmp_tv + ".sqlite";
    if(QFileInfo(tv_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_tv);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_tv = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_tv.setDatabaseName(tv_db_path);
        if(!destination_db_tv.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_tv.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_tv.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_tv.isOpen())
        {
            QString new_tablename = "video";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"file_size" <<"suffix" <<"file_path" <<"date_modified" <<"artist" <<"album_artist" << "file_type" <<"content_type"<<"orientation"
                                 <<"author" <<"pixel_count" << "pixel_height" <<"pixel_width"<<"fs_name"<<"fs_node_count"<<"fs_creation_date"<<"fs_content_change_date"
                                <<"first_used_date"<<"last_used_date"<<"used_dates"<<"content_modification_date"<<"content_creation_date"<<"codec"<<"audio_bit_rate"
                               <<"video_bit_rate"<<"total_bit_rate"<<"audio_channel_count"<<"latitude"<<"longitude"<<"altitude"<<"wherefrom"<<"torrent_names"
                              <<"torrent_file_list"<<"preview_file_path"<<"export_file_paths"<<"LINKFILENAME"<<"bookmarked_extended_attributes"<<"bookmarked_exif_data"
                             <<"extended_attributes_key_value"<<"extended_attributes_value_for_search";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER"
                                  <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_tv);
            //====

            destination_db_tv.close();

        }
    }
}

void MainWindow::update_result_plugin_music_144()
{
    QString plgn_music = "Music";
    QString plgn_tmp_music = plgn_music;
    plgn_tmp_music.replace(" ", "_");

    QString plgn_csv_file_in_result_music = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_music +".csv";
    QString plgn_csv_file_in_app_bundle_music  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_music + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_music, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_music, plgn_csv_file_in_result_music);

    csv_reader *csv_reader_local_music = new csv_reader(this);
    csv_reader_local_music->do_job_plugin_specific(plgn_csv_file_in_result_music, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_music, csv_reader_local_music);
    update_load_case_artifacts_tab_entry_config(plgn_music, csv_reader_local_music);

}

void MainWindow::update_result_plugin_airport_wirelessnetwork_144()
{
    QString plgn_real_airport_wireless_network = "Airport WirelessNetwork";
    QString plgn_tmp_airport_wireless_network = plgn_real_airport_wireless_network;
    plgn_tmp_airport_wireless_network.replace(" ", "_");

    QString plgn_result_csv_airport_wireless_network = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_airport_wireless_network +".csv";
    QString plgn_csv_app_bundle_airport_wireless_network  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_airport_wireless_network + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_airport_wireless_network, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_airport_wireless_network, plgn_result_csv_airport_wireless_network);

    csv_reader *csv_reader_local_airport_wireless_network = new csv_reader(this);
    csv_reader_local_airport_wireless_network->do_job_plugin_specific(plgn_result_csv_airport_wireless_network, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_airport_wireless_network, csv_reader_local_airport_wireless_network);
    update_load_case_artifacts_tab_entry_config(plgn_real_airport_wireless_network, csv_reader_local_airport_wireless_network);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // in Airport WirelessNetwork artifact data
    QString airport_wireless_network_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_airport_wireless_network + "/" + plgn_tmp_airport_wireless_network + ".sqlite";
    if(QFileInfo(airport_wireless_network_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_airport_wireless_network);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_airport_wireless_network = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_airport_wireless_network.setDatabaseName(airport_wireless_network_db_path);
        if(!destination_db_airport_wireless_network.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_airport_wireless_network.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error :-  " + destination_db_airport_wireless_network.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_airport_wireless_network.isOpen())
        {
            QString new_tablename = "wireless_network";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"add_reason" <<"is_hidden" <<"date_updated" <<"joined_by_user_timestamp" <<"joined_by_system_timestamp" <<"date_added"
                                 <<"roaming_profile_type" <<"temporary_disabled" << "system_mode" <<"captive_network"<<"auto_join_disabled";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_airport_wireless_network);
            //====

            destination_db_airport_wireless_network.close();
        }
    }
}

void MainWindow::update_result_plugin_maps_144()
{
    QString plgn_real_maps = "Maps";
    QString plgn_tmp_maps = plgn_real_maps;
    plgn_tmp_maps.replace(" ", "_");

    QString plgn_result_csv_maps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_maps +".csv";
    QString plgn_csv_app_bundle_maps  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_maps + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_maps, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_maps, plgn_result_csv_maps);

    csv_reader *csv_reader_local_maps = new csv_reader(this);
    csv_reader_local_maps->do_job_plugin_specific(plgn_result_csv_maps, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_maps, csv_reader_local_maps);
    update_load_case_artifacts_tab_entry_config(plgn_real_maps, csv_reader_local_maps);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // in Maps artifact data
    QString maps_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_maps + "/" + plgn_tmp_maps + ".sqlite";
    if(QFileInfo(maps_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_maps);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_maps = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_maps.setDatabaseName(maps_db_path);
        if(!destination_db_maps.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_maps.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_maps.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_maps.isOpen())
        {
            QString new_tablename = "locations_bookmarked";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"date_created" <<"date_modified" <<"wikipedia_details"<< "description" << "Phone" << "altitude" << "is_new"
                                 << "postal_code" << "Region" << "Address" << "Date_accessed" << "is_dirty" << "collection_name" << "additional_type"
                                 << "image_url" << "original_name" << "formatted_address";

            QStringList columns_field_type_list;
            columns_field_type_list <<"INTEGER" <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   << "varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" << "INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_maps);
            //====

            QString new_tablename_1 = "recent_searches" ;

            //====
            QStringList new_columns_name_list_1;
            new_columns_name_list_1 <<"date_created" <<"date_modified" <<"wikipedia_details" << "collection_name" << "description" << "is_dirty"
                                   << "is_new" << "phone" << "region" << "additional_type" << "postal_code" << "original_name" << "address" << "image_url"
                                   << "altitude" << "date_accessed";

            QStringList columns_field_type_list_1;
            columns_field_type_list_1 <<"INTEGER" <<"INTEGER" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER"  ;

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_1),new_columns_name_list_1, columns_field_type_list_1, destination_db_maps);
            //====

            QString new_tablename_2 = "recent_directions";

            //====
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"direction";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 << "varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_maps);
            //====

            destination_db_maps.close();

        }
    }
}

void MainWindow::update_result_plugin_notifications_144()
{
    QString plgn_real_notifications = "Notifications";
    QString plgn_tmp_notifications = plgn_real_notifications;
    plgn_tmp_notifications.replace(" ", "_");

    QString plgn_result_csv_notifications = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_notifications +".csv";
    QString plgn_csv_app_bundle_notifications  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_notifications + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_notifications, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_notifications, plgn_result_csv_notifications);

    csv_reader *csv_reader_local_notifications = new csv_reader(this);
    csv_reader_local_notifications->do_job_plugin_specific(plgn_result_csv_notifications, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_notifications, csv_reader_local_notifications);
    update_load_case_artifacts_tab_entry_config(plgn_real_notifications, csv_reader_local_notifications);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // Notifications artifact data
    QString notifications_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_notifications + "/" + plgn_tmp_notifications + ".sqlite";
    if(QFileInfo(notifications_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_notifications);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_notifications = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_notifications.setDatabaseName(notifications_db_path);
        if(!destination_db_notifications.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_notifications.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_notifications.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_notifications.isOpen())
        {
            QString new_tablename = "notifications";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"sub_title" <<"uti" <<"category" <<"path";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_notifications);
            //====

            destination_db_notifications.close();

        }
    }
}
void MainWindow::update_result_plugin_wifi_location_145()
{
    QString plgn_real_wifi_location = "Wi-Fi Location";
    QString plgn_tmp_wifi_location = plgn_real_wifi_location;
    plgn_tmp_wifi_location.replace(" ", "_");

    QString plgn_result_csv_wifi_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_wifi_location +".csv";
    QString plgn_csv_app_bundle_wifi_location  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_wifi_location + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_wifi_location, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_wifi_location, plgn_result_csv_wifi_location);

    csv_reader *csv_reader_local_notifications = new csv_reader(this);
    csv_reader_local_notifications->do_job_plugin_specific(plgn_result_csv_wifi_location, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_wifi_location, csv_reader_local_notifications);
    update_load_case_artifacts_tab_entry_config(plgn_real_wifi_location, csv_reader_local_notifications);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // Wi-Fi Location artifact data
    QString wifi_location_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_wifi_location + "/" + plgn_tmp_wifi_location + ".sqlite";
    if(QFileInfo(wifi_location_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_wifi_location);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_wifi_location = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_wifi_location.setDatabaseName(wifi_location_db_path);
        if(!destination_db_wifi_location.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_wifi_location.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error -  " + destination_db_wifi_location.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_wifi_location.isOpen())
        {
            QString new_tablename = "location";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"map_url";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_wifi_location);
            //====

            destination_db_wifi_location.close();

        }
    }
}

void MainWindow::update_result_plugin_music_library_145()
{
    QString plgn_music_library = "Music Library";
    QString plgn_tmp_music_library = plgn_music_library;
    plgn_tmp_music_library.replace(" ", "_");

    QString plgn_csv_file_in_result_music_library      = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_music_library +".csv";
    QString plgn_csv_file_in_app_bundle_music_library  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_music_library + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_music_library, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_music_library, plgn_csv_file_in_result_music_library);

    csv_reader *csv_reader_local_music_library = new csv_reader(this);
    csv_reader_local_music_library->do_job_plugin_specific(plgn_csv_file_in_result_music_library, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_music_library, csv_reader_local_music_library);
    update_load_case_artifacts_tab_entry_config(plgn_music_library, csv_reader_local_music_library);

}

void MainWindow::update_result_plugin_android_thumbnails_145()
{
    QString plgn_android_thumbnails = "Android Thumbnails";
    QString plgn_tmp_android_thumbnails = plgn_android_thumbnails;
    plgn_tmp_android_thumbnails.replace(" ", "_");


    QString plgn_result_csv_android_thumbnails = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_android_thumbnails +".csv";
    QString plgn_csv_app_bundle_android_thumbnails  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_android_thumbnails + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_android_thumbnails, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_android_thumbnails, plgn_result_csv_android_thumbnails);

    csv_reader *csv_reader_local_android_thumbnails = new csv_reader(this);
    csv_reader_local_android_thumbnails->do_job_plugin_specific(plgn_result_csv_android_thumbnails, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_android_thumbnails, csv_reader_local_android_thumbnails);
    update_load_case_artifacts_tab_entry_config(plgn_android_thumbnails, csv_reader_local_android_thumbnails);

}



void MainWindow::update_result_plugin_linkedin_147()
{
    QString plgn_real_linkedin = "LinkedIn";
    QString plgn_tmp_linkedin = plgn_real_linkedin;
    plgn_tmp_linkedin.replace(" ", "_");


    QString plgn_result_csv_linkedin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_linkedin +".csv";
    QString plgn_csv_app_bundle_linkedin  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_linkedin + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_linkedin, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_linkedin, plgn_result_csv_linkedin);

    csv_reader *csv_reader_local_linkedin = new csv_reader(this);
    csv_reader_local_linkedin->do_job_plugin_specific(plgn_result_csv_linkedin, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_entry_config(plgn_real_linkedin, csv_reader_local_linkedin);
    update_load_case_artifacts_tab_entry_config(plgn_real_linkedin, csv_reader_local_linkedin);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();


    // Linkedin artifact data
    QString linkedin_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_linkedin + "/" + plgn_tmp_linkedin + ".sqlite";
    if(QFileInfo(linkedin_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_linkedin);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_linkedin = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_linkedin.setDatabaseName(linkedin_db_path);
        if(!destination_db_linkedin.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_linkedin.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_linkedin.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_linkedin.isOpen())
        {
            QString new_tablename = "account";

            //====

            QStringList new_columns_name_list;
            new_columns_name_list <<"app_name" <<"full_name" <<"server_address" <<"service_type" <<"password" <<"domain"
                                 << "category"<<"user_account_details";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_linkedin);
            //====

            QString new_tablename_2 = "attachments";

            //====
            QStringList new_columns_name_list_2;
            new_columns_name_list_2 <<"file_exist" <<"file_path" <<"identifier" <<"sender" <<"receiver" <<"bookmarked_extended_attributes"
                                   <<"bookmarked_exif_data" <<"extended_attributes_key_value" <<"extended_attributes_value_for_search"
                                  <<"preview_file_path" <<"export_file_paths" <<"LINKFILENAME"  <<"current_file_path";

            QStringList columns_field_type_list_2;
            columns_field_type_list_2 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                     <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                    <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_2),new_columns_name_list_2, columns_field_type_list_2, destination_db_linkedin);
            //====

            QString new_tablename_3 = "messages";

            //====
            QStringList new_columns_name_list_3;
            new_columns_name_list_3 <<"identifier" <<"title" <<"url";

            QStringList columns_field_type_list_3;
            columns_field_type_list_3 <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_3),new_columns_name_list_3, columns_field_type_list_3, destination_db_linkedin);
            //====

            QString new_tablename_4 = "contact_list";

            //====
            QStringList new_columns_name_list_4;
            new_columns_name_list_4 <<"contact_name" <<"type" <<"app_name" <<"identifier" <<"contact_notes" <<"url"
                                   <<"modified_time";

            QStringList columns_field_type_list_4;
            columns_field_type_list_4  <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                      <<"varchar(1024)" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename_4),new_columns_name_list_4, columns_field_type_list_4, destination_db_linkedin);

            //====

            destination_db_linkedin.close();

        }
    }
}

void MainWindow::update_result_plugin_contacts_147()
{
    QString plgn_real_contacts= "Contacts";
    QString plgn_tmp_contacts = plgn_real_contacts;
    plgn_tmp_contacts.replace(" ", "_");

    QString plgn_result_csv_contacts = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_contacts +".csv";
    QString plgn_csv_app_bundle_contacts  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_contacts + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_contacts, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_contacts, plgn_result_csv_contacts);

    csv_reader *csv_reader_contacts = new csv_reader(this);
    csv_reader_contacts->do_job_plugin_specific(plgn_result_csv_contacts, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_contacts, csv_reader_contacts);
    update_load_case_artifacts_tab_entry_config(plgn_real_contacts, csv_reader_contacts);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // Contacts artifact data
    QString contacts_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_contacts + "/" + plgn_tmp_contacts + ".sqlite";
    if(QFileInfo(contacts_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_contacts);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_contacts = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_contacts.setDatabaseName(contacts_db_path);
        if(!destination_db_contacts.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_contacts.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error -  " + destination_db_contacts.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_contacts.isOpen())
        {
            QString new_tablename = "db_contacts";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list <<"app_name"<< "full_name" << "contact_app_username" << "contact_type" << "contact_name" << "contact_profile_picture_url" << "account_identifier" <<
                                    "contact_notes" << "street_address" << "modified_time";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"INTEGER";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_contacts);
            //====

            destination_db_contacts.close();

        }
    }
}

void MainWindow::update_result_plugin_airport_wireless_network_147()
{
    QString plgn_real_airport_wireless_network= "Airport WirelessNetwork";
    QString plgn_tmp_airport_wireless_network = plgn_real_airport_wireless_network;
    plgn_tmp_airport_wireless_network.replace(" ", "_");

    QString plgn_result_csv_airport_wireless_network = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_airport_wireless_network +".csv";
    QString plgn_csv_app_bundle_airport_wireless_network  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_airport_wireless_network + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_airport_wireless_network, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_airport_wireless_network, plgn_result_csv_airport_wireless_network);

    csv_reader *csv_reader_airport_wireless_network = new csv_reader(this);
    csv_reader_airport_wireless_network->do_job_plugin_specific(plgn_result_csv_airport_wireless_network, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_airport_wireless_network, csv_reader_airport_wireless_network);
    update_load_case_artifacts_tab_entry_config(plgn_real_airport_wireless_network, csv_reader_airport_wireless_network);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // Airport Wireless Network artifact data
    QString airport_wireless_network_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_airport_wireless_network + "/" + plgn_tmp_airport_wireless_network + ".sqlite";

    if(QFileInfo(airport_wireless_network_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_airport_wireless_network);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_airport_wireless_network = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_airport_wireless_network.setDatabaseName(airport_wireless_network_db_path);
        if(!destination_db_airport_wireless_network.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_airport_wireless_network.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error -  " + destination_db_airport_wireless_network.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_airport_wireless_network.isOpen())
        {
            QString new_tablename = "wireless_network";

            //====
            QStringList new_columns_name_list;
            new_columns_name_list << "trig_disconnect_count" << "supported_security_type_str" << "wifi_network_attribute_is_potentially_moving_count"
                                  << "network_of_interest_work_state_count" << "known_bss_updated" << "prev_joined" << "last_roamed";

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)" <<"varchar(1024)"
                                   << "INTEGER" << "INTEGER" << "INTEGER";


            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(new_tablename),new_columns_name_list, columns_field_type_list, destination_db_airport_wireless_network);
            //====

            destination_db_airport_wireless_network.close();

        }
    }
}

void MainWindow::update_result_all_artifacts_for_domain_user_148()
{
    //=============Update_User_Account_for_Feature-Start============//
    QString db_output_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display) + "/";

    QString user_accounts_db_path_for_check =  "User_Accounts/User_Accounts.sqlite";

    QDirIterator user_account_iter_dir(db_output_dir_path, QDir::Files | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
    while(user_account_iter_dir.hasNext())
    {
        QString user_accounts_db_path_for_feature = user_account_iter_dir.next();
        if(!user_accounts_db_path_for_feature.endsWith(user_accounts_db_path_for_check))
        {
            continue;
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_user_accounts = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_user_accounts.setDatabaseName(user_accounts_db_path_for_feature);
        if(!destination_db_user_accounts.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_user_accounts.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error -  " + destination_db_user_accounts.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_user_accounts.isOpen())
        {
            QString tablename = "domain_users_accounts";

            QStringList new_columns_name_list;
            new_columns_name_list << "user_names";


            QStringList old_columns_name_list;
            old_columns_name_list << "user_name";

            recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbreference(QStringList(tablename),old_columns_name_list, new_columns_name_list,destination_db_user_accounts);

            //====
            new_columns_name_list.clear();
            new_columns_name_list << "registry_node_path_domain_user" << "domain_user" ;

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_user_accounts);
            //====

            destination_db_user_accounts.close();


        }
    }
    //=============Update_User_Account_for_Feature-end============//


    //==================================Domain_User_Start============================//

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

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
            QString add_new_column_command = "ALTER TABLE "+ table_name_list.at(jj) + " add column domain_user VARCHAR(100)";
            recon_helper_standard_obj->execute_db_command_by_dbpath(add_new_column_command,destination_db_file_path,Q_FUNC_INFO);

        }
    }

    //==================================Domain_User_End============================//

}


void MainWindow::update_result_plugin_user_accounts_148()
{
    QString plgn_real_user_accounts= "User Accounts";
    QString plgn_tmp_user_accounts = plgn_real_user_accounts;
    plgn_tmp_user_accounts.replace(" ", "_");

    QString plgn_result_csv_user_accounts = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_user_accounts +".csv";
    QString plgn_csv_app_bundle_user_accounts  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_user_accounts + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_user_accounts, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_user_accounts, plgn_result_csv_user_accounts);

    csv_reader *csv_reader_user_accounts = new csv_reader(this);
    csv_reader_user_accounts->do_job_plugin_specific(plgn_result_csv_user_accounts, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_user_accounts, csv_reader_user_accounts);
    update_load_case_artifacts_tab_entry_config(plgn_real_user_accounts, csv_reader_user_accounts);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // User Accounts artifact data
    QString user_accounts_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_user_accounts + "/" + plgn_tmp_user_accounts + ".sqlite";

    if(QFileInfo(user_accounts_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_real_user_accounts);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_user_accounts = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_user_accounts.setDatabaseName(user_accounts_db_path);
        if(!destination_db_user_accounts.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_user_accounts.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error -  " + destination_db_user_accounts.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_user_accounts.isOpen())
        {
            QString tablename = "domain_users_accounts";

            QStringList new_columns_name_list;
            new_columns_name_list << "user_names";


            QStringList old_columns_name_list;
            old_columns_name_list << "user_name";

            recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbreference(QStringList(tablename),old_columns_name_list, new_columns_name_list,destination_db_user_accounts);

            //====
            new_columns_name_list.clear();
            new_columns_name_list << "registry_node_path_domain_user" << "domain_user" ;

            QStringList columns_field_type_list;
            columns_field_type_list <<"varchar(1024)" <<"varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_user_accounts);
            //====

            destination_db_user_accounts.close();

        }
    }
}

void MainWindow::update_result_all_artifacts_for_system_user_150()
{
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString destination_plugin_dir_path = result_dir_path + "Artifacts_Result/";
    QDirIterator artifact_iter_dir(destination_plugin_dir_path, QDir::Files | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
    while(artifact_iter_dir.hasNext())
    {
        QString destination_db_file_path = artifact_iter_dir.next();
        if(destination_db_file_path.endsWith(".sqlite"))
        {
            QStringList table_name_list = recon_helper_standard_obj->get_total_tab_name_from_db_by_db_path(destination_db_file_path,Q_FUNC_INFO);
            for(int jj = 0; jj < table_name_list.size(); jj++)
            {
                recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList(table_name_list.at(jj)), QStringList("system_account"), QStringList("system_user"), destination_db_file_path);
            }
        }
    }
}

void MainWindow::update_result_plugin_reminders_150()
{
    QString plgn_real_reminders = "Reminders";
    QString plgn_tmp_reminders = plgn_real_reminders;
    plgn_tmp_reminders.replace(" ", "_");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    // in Reminders artifact data
    QString reminders_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_reminders + "/" + plgn_tmp_reminders + ".sqlite";
    if(QFileInfo(reminders_db_path).exists())
    {
        QStringList table_name_list = recon_helper_standard_obj->get_total_tab_name_from_db_by_db_path(reminders_db_path,Q_FUNC_INFO);
        for(int jj = 0; jj < table_name_list.size(); jj++)
        {
            recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList(table_name_list.at(jj)), QStringList("account_name"), QStringList("system_user"), reminders_db_path);
        }
    }
}

void MainWindow::update_result_plugin_messages_152()
{
    QString plgn_messages = "Messages";
    QString plgn_tmp_messages = plgn_messages;
    plgn_tmp_messages.replace(" ", "_");

    QString plgn_result_csv_messages = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_messages +".csv";
    QString plgn_csv_app_bundle_messages  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_messages + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_messages, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_messages, plgn_result_csv_messages);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString messages_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_messages + "/" + plgn_tmp_messages + ".sqlite";

    if(QFileInfo(messages_db_path).exists())
    {
        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_messages = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_messages.setDatabaseName(messages_db_path);

        if(!destination_db_messages.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + plgn_tmp_messages + destination_db_messages.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" error -  " + destination_db_messages.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_messages.isOpen())
        {
            QSqlQuery query_select(destination_db_messages);

            query_select.prepare("select INT, total_bytes from attachments");

            if(!query_select.exec())
            {
                recon_static_functions::app_debug("Destination db open ------FAILED ----------" + destination_db_messages.databaseName(), Q_FUNC_INFO);
                recon_static_functions::app_debug("Error :- " + query_select.lastError().text(),Q_FUNC_INFO);
                recon_static_functions::app_debug("Query :- " + query_select.executedQuery(),Q_FUNC_INFO);
                destination_db_messages.close();
                return;
            }

            while(query_select.next())
            {
                QString total_bytes = query_select.value("total_bytes").toString();

                if(total_bytes.isEmpty())
                {
                    continue;
                }

                QString record_no = query_select.value("INT").toString();

                QString command_update = "Update attachments Set file_size = ? where INT = ?";

                QStringList arg_list;
                arg_list << total_bytes << record_no;
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_update,arg_list,destination_db_messages,Q_FUNC_INFO);
            }

            recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbreference(QStringList("attachments"),QStringList("total_bytes"),QStringList("extra_not_in_use_0_152"),destination_db_messages);
            //rename archived_chats column filesize
            recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList("archived_chats"), QStringList("filesize"), QStringList("extra_not_in_use_0_152"), messages_db_path);
        }

        destination_db_messages.close();
    }

}

void MainWindow::update_result_plugin_linkedin_152()
{
    QString plgn_linkedin = "LinkedIn";
    QString plgn_tmp_linkedin = plgn_linkedin;
    plgn_tmp_linkedin.replace(" ", "_");

    QString plgn_result_csv_linkedin = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_linkedin +".csv";
    QString plgn_csv_app_bundle_linkedin  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_linkedin + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_linkedin, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_linkedin, plgn_result_csv_linkedin);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString linkedin_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_linkedin + "/" + plgn_tmp_linkedin + ".sqlite";
    QString rename_query = "ALTER TABLE account RENAME COLUMN Username TO username";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,linkedin_db_path,Q_FUNC_INFO);
}

void MainWindow::update_result_plugin_whatsapp_152()
{
    QString plgn_whatsapp = "WhatsApp";
    QString plgn_tmp_whatsapp = plgn_whatsapp;
    plgn_tmp_whatsapp.replace(" ", "_");

    QString plgn_result_csv_whatsapp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_whatsapp +".csv";
    QString plgn_csv_app_bundle_whatsapp  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_whatsapp + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_whatsapp, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_whatsapp, plgn_result_csv_whatsapp);

    csv_reader *csv_reader_local_whatsapp = new csv_reader(this);
    csv_reader_local_whatsapp->do_job_plugin_specific(plgn_result_csv_whatsapp, Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString whatsapp_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_whatsapp + "/" + plgn_tmp_whatsapp + ".sqlite";

    QString rename_query = "ALTER TABLE Messages RENAME TO wa_messages";

    if(recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO))
    {
        rename_query.clear();
        rename_query = "ALTER TABLE wa_messages RENAME TO messages";
        recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);
    }

    ///RENAME Contacts TAB column name
    rename_query.clear();
    rename_query = "ALTER TABLE contacts RENAME COLUMN FirstName TO first_name";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);

    rename_query.clear();
    rename_query = "ALTER TABLE contacts RENAME COLUMN FullName TO full_name";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);

    rename_query.clear();
    rename_query = "ALTER TABLE contacts RENAME COLUMN HighLightedName TO highlight_name";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);

    rename_query.clear();
    rename_query = "ALTER TABLE contacts RENAME COLUMN Nickname TO nick_name";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);

    rename_query.clear();
    rename_query = "ALTER TABLE contacts RENAME COLUMN IndexName TO index_name";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);

    rename_query.clear();
    rename_query = "ALTER TABLE contacts RENAME COLUMN LastModifiedDate TO last_modified_date";
    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,whatsapp_db_path,Q_FUNC_INFO);

}

void MainWindow::update_result_plugin_mail_152()
{
    QString plgn_mail = "Mail";
    QString plgn_tmp_mail = plgn_mail;
    plgn_tmp_mail.replace(" ", "_");

    QString plgn_result_csv_mail = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_mail +".csv";
    QString plgn_csv_app_bundle_mail  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_mail + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_mail, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_mail, plgn_result_csv_mail);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString mail_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_mail + "/" + plgn_tmp_mail + ".sqlite";

    QString rename_query = "ALTER TABLE messages RENAME COLUMN ROWID TO row_id";

    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,mail_db_path,Q_FUNC_INFO);

}

void MainWindow::update_result_plugin_airport_wireless_network_152()
{
    QString plgn_airport_wirelessnetwork = "Airport WirelessNetwork";
    QString plgn_tmp_airport_wirelessnetwork = plgn_airport_wirelessnetwork;
    plgn_tmp_airport_wirelessnetwork.replace(" ", "_");

    QString plgn_result_csv_airport_wirelessnetwork = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_airport_wirelessnetwork +".csv";
    QString plgn_csv_app_bundle_airport_wirelessnetwork  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_airport_wirelessnetwork + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_airport_wirelessnetwork, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_airport_wirelessnetwork, plgn_result_csv_airport_wirelessnetwork);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString airport_wirelessnetwork_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_airport_wirelessnetwork + "/" + plgn_tmp_airport_wirelessnetwork + ".sqlite";

    QString rename_query = "ALTER TABLE networks_list RENAME COLUMN UID TO uid";

    recon_helper_standard_obj->execute_db_command_by_dbpath(rename_query,airport_wirelessnetwork_db_path,Q_FUNC_INFO);
}

void MainWindow::update_result_plugin_photos_152()
{
    recon_static_functions::app_debug("Starts", Q_FUNC_INFO);

    QString result_dir_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString plgn_nm_real_photos = "Photos";
    QString plgn_nm_tmp_photos = plgn_nm_real_photos;
    plgn_nm_tmp_photos.replace(" ", "_");

    QString plgn_csv_file_in_result_photos  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_nm_tmp_photos +".csv";
    QString plgn_csv_file_in_app_bundle_photos  = "../Resources/CSVS/Plugins_CSV/" + plgn_nm_tmp_photos + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_photos, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_photos, plgn_csv_file_in_result_photos);

    QString photos_db_path = result_dir_path + "Artifacts_Result/" + plgn_nm_tmp_photos + "/" + plgn_nm_tmp_photos + ".sqlite";

    if(QFileInfo(photos_db_path).exists())
    {
        QString table_name_photos = "all_photos";

        QStringList all_photos_columns_name_list;
        all_photos_columns_name_list << "Creator_Bundle_id" << "Height" << "Orientation" << "Width";

        QStringList new_columns_name_list;
        new_columns_name_list << "creator_bundle_id" << "height" << "orientation" << "width";

        recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList(table_name_photos),all_photos_columns_name_list,new_columns_name_list,photos_db_path);


        QString table_name_audio = "audio_video";

        QStringList audio_columns_name_list;
        audio_columns_name_list << "Creater_Bundle_id" << "Height" << "Orientation" << "Width" << "Creator_Bundle_id";

        QStringList audio_rename_columns_name_list;
        audio_rename_columns_name_list << "creater_bundle_id" << "height" << "orientation" << "width" << "creator_bundle_id";

        recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList(table_name_audio),audio_columns_name_list,audio_rename_columns_name_list,photos_db_path);


        QString table_name_camera = "camera_captures";

        QStringList camera_existing_columns_name_list;
        camera_existing_columns_name_list << "Creater_Bundle_id" << "Height" << "Orientation" << "Width" ;

        QStringList camera_rename_columns_name_list;
        camera_rename_columns_name_list << "creater_bundle_id" << "height" << "orientation" << "width";

        recon_helper_standard_obj->alter_db_tables_to_rename_column_by_dbpath(QStringList(table_name_camera),camera_existing_columns_name_list,camera_rename_columns_name_list,photos_db_path);
    }

    recon_static_functions::app_debug("Ends", Q_FUNC_INFO);
}

void MainWindow::update_result_plugin_knowledgec_152()
{
    QString plgn_real_knowledgec = "KnowledgeC";
    QString plgn_tmp_knowledgec = plgn_real_knowledgec;
    plgn_tmp_knowledgec.replace(" ", "_");

    QString plgn_result_csv_knowledgec = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_knowledgec +".csv";
    QString plgn_csv_app_bundle_knowledgec  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_knowledgec + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_knowledgec, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_knowledgec, plgn_result_csv_knowledgec);

    csv_reader *csv_reader_local_knowledgec = new csv_reader(this);
    csv_reader_local_knowledgec->do_job_plugin_specific(plgn_result_csv_knowledgec, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_knowledgec, csv_reader_local_knowledgec);
    update_load_case_artifacts_tab_entry_config(plgn_real_knowledgec, csv_reader_local_knowledgec);
}

void MainWindow::update_result_plugin_messages_153()
{
    QString plgn_real_messages = "Messages";
    QString plgn_tmp_messages = plgn_real_messages;
    plgn_tmp_messages.replace(" ", "_");

    QString plgn_result_csv_messages = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_messages +".csv";
    QString plgn_csv_app_bundle_messages  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_messages + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_messages, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_messages, plgn_result_csv_messages);

    csv_reader *csv_reader_local_messages = new csv_reader(this);
    csv_reader_local_messages->do_job_plugin_specific(plgn_result_csv_messages, Q_FUNC_INFO);

    update_load_case_artifacts_tab_entry_config(plgn_real_messages, csv_reader_local_messages);

}

void MainWindow::update_result_plugin_messages_154()
{
    QString plgn_real_messages = "Messages";
    QString plgn_tmp_messages = plgn_real_messages;
    plgn_tmp_messages.replace(" ", "_");

    QString plgn_result_csv_messages = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_messages +".csv";
    QString plgn_csv_app_bundle_messages  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_messages + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_messages, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_messages, plgn_result_csv_messages);
}

void MainWindow::update_result_plugin_daily_out_155()
{
    QString plgn_daily_out = "Daily Out";
    QString plgn_tmp_daily_out = plgn_daily_out;
    plgn_tmp_daily_out.replace(" ", "_");

    QString plgn_result_csv_daily_out = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_daily_out +".csv";

    QString plgn_csv_app_bundle_daily_out  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_daily_out + ".csv";

    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_daily_out, Q_FUNC_INFO);

    QFile::copy(plgn_csv_app_bundle_daily_out, plgn_result_csv_daily_out);

    csv_reader *csv_reader_user_daily_out = new csv_reader(this);
    csv_reader_user_daily_out->do_job_plugin_specific(plgn_result_csv_daily_out, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_daily_out, csv_reader_user_daily_out);
    update_load_case_artifacts_tab_entry_config(plgn_daily_out, csv_reader_user_daily_out);
}

void MainWindow::update_result_plugin_safari_157()
{
    QString plgn_safari = "Safari";
    QString plgn_tmp_safari = plgn_safari;
    plgn_tmp_safari.replace(" ", "_");

    QString plgn_result_csv_safari = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_safari +".csv";
    QString plgn_csv_app_bundle_safari  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_safari + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_safari, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_safari, plgn_result_csv_safari);
}

void MainWindow::update_result_plugin_thumb_cache_160()
{
    QString plgn_thumb_cache = "Thumb Cache";
    QString plgn_tmp_thumb_cache = plgn_thumb_cache;
    plgn_tmp_thumb_cache.replace(" ", "_");

    QString plgn_result_csv_thumb_cache = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_thumb_cache +".csv";
    QString plgn_csv_app_bundle_thumb_cache  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_thumb_cache + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_thumb_cache, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_thumb_cache, plgn_result_csv_thumb_cache);

    csv_reader *csv_reader_thumb_cache = new csv_reader(this);
    csv_reader_thumb_cache->do_job_plugin_specific(plgn_result_csv_thumb_cache, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_thumb_cache, csv_reader_thumb_cache);
    update_load_case_artifacts_tab_entry_config(plgn_thumb_cache, csv_reader_thumb_cache);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString thumb_cache_db_path = result_dir_path + "Artifacts_Result/" + plgn_tmp_thumb_cache + "/" + plgn_tmp_thumb_cache + ".sqlite";

    if(QFileInfo(thumb_cache_db_path).exists())
    {
        header_plugin_extraction_interface *plugin_obj = find_plugin_ptr_by_pluginname(plgn_thumb_cache);
        if(plugin_obj != NULL)
        {
            plugin_obj->pub_set_result_directory(result_dir_path);
            plugin_obj->pub_create_db_tables();
            plugin_obj->pub_remove_temporary_asthetics();
        }

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db_thumb_cache = QSqlDatabase::addDatabase("QSQLITE",connection_naam);
        destination_db_thumb_cache.setDatabaseName(thumb_cache_db_path);
        if(!destination_db_thumb_cache.open())
        {
            recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db_thumb_cache.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error -  " + destination_db_thumb_cache.lastError().text(),Q_FUNC_INFO);
        }

        if(destination_db_thumb_cache.isOpen())
        {
            QString tablename = "thumb_cache";

            QStringList new_columns_name_list;
            new_columns_name_list << "last_hit_date" << "generator" << "fs_id" << "folder" << "file_row_id";

            QStringList columns_field_type_list;
            columns_field_type_list <<"INTEGER" << "varchar(1024)" << "varchar(1024)" << "varchar(1024)" << "varchar(1024)";

            recon_helper_standard_obj->alter_db_tables_to_add_column_by_dbreference(QStringList(tablename),new_columns_name_list, columns_field_type_list, destination_db_thumb_cache);

            destination_db_thumb_cache.close();
        }
    }
}

void MainWindow::update_result_plugin_keychain_160()
{
    QString plgn_keychain = "Keychain";
    QString plgn_tmp_keychain = plgn_keychain;
    plgn_tmp_keychain.replace(" ", "_");

    QString plgn_result_csv_keychain = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_keychain +".csv";

    QString plgn_csv_app_bundle_keychain  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_keychain + ".csv";

    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_keychain, Q_FUNC_INFO);

    QFile::copy(plgn_csv_app_bundle_keychain, plgn_result_csv_keychain);

    csv_reader *csv_reader_user_keychain = new csv_reader(this);
    csv_reader_user_keychain->do_job_plugin_specific(plgn_result_csv_keychain, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_keychain, csv_reader_user_keychain);
    update_load_case_artifacts_tab_entry_config(plgn_keychain, csv_reader_user_keychain);
}
