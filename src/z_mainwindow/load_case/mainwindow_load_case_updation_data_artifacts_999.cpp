#include "z_mainwindow/mainwindow.h"


void MainWindow::update_result_plugin_books_999()
{
    QString plgn_real_books = "Books";
    QString plgn_tmp_books = plgn_real_books;
    plgn_tmp_books.replace(" ", "_");


    QString plgn_result_csv_books = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_books +".csv";
    QString plgn_csv_app_bundle_books  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_books + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_books, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_books, plgn_result_csv_books);

    csv_reader *csv_reader_local_books = new csv_reader(this);
    csv_reader_local_books->do_job_plugin_specific(plgn_result_csv_books, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_books, csv_reader_local_books);
    update_load_case_artifacts_tab_entry_config(plgn_real_books, csv_reader_local_books);

}

void MainWindow::update_result_plugin_facebook_999()
{
    QString plgn_real_facebook = "Facebook";
    QString plgn_tmp_facebook = plgn_real_facebook;
    plgn_tmp_facebook.replace(" ", "_");


    QString plgn_result_csv_facebook = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_facebook +".csv";
    QString plgn_csv_app_bundle_facebook  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_facebook + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_facebook, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_facebook, plgn_result_csv_facebook);

    csv_reader *csv_reader_local_facebook = new csv_reader(this);
    csv_reader_local_facebook->do_job_plugin_specific(plgn_result_csv_facebook, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_real_facebook, csv_reader_local_facebook);
    update_load_case_artifacts_tab_entry_config(plgn_real_facebook, csv_reader_local_facebook);

}


void MainWindow::update_result_plugin_system_logs_999()
{
    QString plgn_system_logs = "System Logs";
    QString plgn_tmp_system_logs = plgn_system_logs;
    plgn_tmp_system_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_system_logs  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_system_logs +".csv";
    QString plgn_csv_file_in_app_bundle_system_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_system_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_system_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_system_logs, plgn_csv_file_in_result_system_logs);

    csv_reader *csv_reader_local_system_logs = new csv_reader(this);
    csv_reader_local_system_logs->do_job_plugin_specific(plgn_csv_file_in_result_system_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_system_logs, csv_reader_local_system_logs);
    update_load_case_artifacts_tab_entry_config(plgn_system_logs, csv_reader_local_system_logs);

}

void MainWindow::update_result_plugin_mount_devices_logs_999()
{
    QString plgn_mount_devices_logs = "Mount Devices Logs";
    QString plgn_tmp_mount_devices_logs = plgn_mount_devices_logs;
    plgn_tmp_mount_devices_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_mount_devices_logs  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_mount_devices_logs +".csv";
    QString plgn_csv_file_in_app_bundle_mount_devices_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_mount_devices_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_mount_devices_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_mount_devices_logs, plgn_csv_file_in_result_mount_devices_logs);

    csv_reader *csv_reader_local_mount_devices_logs = new csv_reader(this);
    csv_reader_local_mount_devices_logs->do_job_plugin_specific(plgn_csv_file_in_result_mount_devices_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_mount_devices_logs, csv_reader_local_mount_devices_logs);
    update_load_case_artifacts_tab_entry_config(plgn_mount_devices_logs, csv_reader_local_mount_devices_logs);

}

void MainWindow::update_result_plugin_screen_sharing_logs_999()
{
    QString plgn_screen_sharing_logs = "Screen Sharing Logs";
    QString plgn_tmp_screen_sharing_logs = plgn_screen_sharing_logs;
    plgn_tmp_screen_sharing_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_screen_sharing_logs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_screen_sharing_logs +".csv";
    QString plgn_csv_file_in_app_bundle_screen_sharing_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_screen_sharing_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_screen_sharing_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_screen_sharing_logs, plgn_csv_file_in_result_screen_sharing_logs);

    csv_reader *csv_reader_local_screen_sharing_logs = new csv_reader(this);
    csv_reader_local_screen_sharing_logs->do_job_plugin_specific(plgn_csv_file_in_result_screen_sharing_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_screen_sharing_logs, csv_reader_local_screen_sharing_logs);
    update_load_case_artifacts_tab_entry_config(plgn_screen_sharing_logs, csv_reader_local_screen_sharing_logs);

}


void MainWindow::update_result_plugin_applications_track_logs_999()
{
    QString plgn_app_track_logs = "Applications Track Logs";
    QString plgn_tmp_app_track_logs = plgn_app_track_logs;
    plgn_tmp_app_track_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_app_track_logs  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_app_track_logs +".csv";
    QString plgn_csv_file_in_app_bundle_app_track_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_app_track_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_app_track_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_app_track_logs, plgn_csv_file_in_result_app_track_logs);

    csv_reader *csv_reader_local_app_track_logs = new csv_reader(this);
    csv_reader_local_app_track_logs->do_job_plugin_specific(plgn_csv_file_in_result_app_track_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_app_track_logs, csv_reader_local_app_track_logs);
    update_load_case_artifacts_tab_entry_config(plgn_app_track_logs, csv_reader_local_app_track_logs);

}

void MainWindow::update_result_plugin_login_window_logs_999()
{
    QString plgn_login_window_logs = "Login Window Logs";
    QString plgn_tmp_login_window_logs = plgn_login_window_logs;
    plgn_tmp_login_window_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_login_window_logs  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_login_window_logs +".csv";
    QString plgn_csv_file_in_app_bundle_login_window_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_login_window_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_login_window_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_login_window_logs, plgn_csv_file_in_result_login_window_logs);

    csv_reader *csv_reader_local_login_window_logs = new csv_reader(this);
    csv_reader_local_login_window_logs->do_job_plugin_specific(plgn_csv_file_in_result_login_window_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_login_window_logs, csv_reader_local_login_window_logs);
    update_load_case_artifacts_tab_entry_config(plgn_login_window_logs, csv_reader_local_login_window_logs);

}

void MainWindow::update_result_plugin_notifications_logs_999()
{
    QString plgn_notifications_logs = "Notifications Logs";
    QString plgn_tmp_notifications_logs = plgn_notifications_logs;
    plgn_tmp_notifications_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_notifications_logs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_notifications_logs +".csv";
    QString plgn_csv_file_in_app_bundle_notifications_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_notifications_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_notifications_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_notifications_logs, plgn_csv_file_in_result_notifications_logs);

    csv_reader *csv_reader_local_notifications_logs = new csv_reader(this);
    csv_reader_local_notifications_logs->do_job_plugin_specific(plgn_csv_file_in_result_notifications_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_notifications_logs, csv_reader_local_notifications_logs);
    update_load_case_artifacts_tab_entry_config(plgn_notifications_logs, csv_reader_local_notifications_logs);

}


void MainWindow::update_result_plugin_wifi_logs_999()
{
    QString plgn_wifi_logs = "Wi-Fi Logs";
    QString plgn_tmp_wifi_logs = plgn_wifi_logs;
    plgn_tmp_wifi_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_wifi_logs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_wifi_logs +".csv";
    QString plgn_csv_file_in_app_bundle_wifi_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_wifi_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_wifi_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_wifi_logs, plgn_csv_file_in_result_wifi_logs);

    csv_reader *csv_reader_local_wifi_logs = new csv_reader(this);
    csv_reader_local_wifi_logs->do_job_plugin_specific(plgn_csv_file_in_result_wifi_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_wifi_logs, csv_reader_local_wifi_logs);
    update_load_case_artifacts_tab_entry_config(plgn_wifi_logs, csv_reader_local_wifi_logs);

}

void MainWindow::update_result_plugin_apple_agent_logs_999()
{
    QString plgn_apple_agent_logs = "Apple Agent Logs";
    QString plgn_tmp_apple_agent_logs = plgn_apple_agent_logs;
    plgn_tmp_apple_agent_logs.replace(" ", "_");

    QString plgn_csv_file_in_result_apple_agent_logs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_apple_agent_logs +".csv";
    QString plgn_csv_file_in_app_bundle_apple_agent_logs  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_apple_agent_logs + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_apple_agent_logs, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_apple_agent_logs, plgn_csv_file_in_result_apple_agent_logs);

    csv_reader *csv_reader_local_apple_agent_logs = new csv_reader(this);
    csv_reader_local_apple_agent_logs->do_job_plugin_specific(plgn_csv_file_in_result_apple_agent_logs, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_apple_agent_logs, csv_reader_local_apple_agent_logs);
    update_load_case_artifacts_tab_entry_config(plgn_apple_agent_logs, csv_reader_local_apple_agent_logs);

}

void MainWindow::update_result_plugin_truecaller_999()
{
    QString plgn_truecaller = "Truecaller";
    QString plgn_tmp_truecaller = plgn_truecaller;
    plgn_tmp_truecaller.replace(" ", "_");

    QString plgn_csv_file_in_result_truecaller = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_truecaller +".csv";
    QString plgn_csv_file_in_app_bundle_truecaller  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_truecaller + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_csv_file_in_result_truecaller, Q_FUNC_INFO);
    QFile::copy(plgn_csv_file_in_app_bundle_truecaller, plgn_csv_file_in_result_truecaller);

    csv_reader *csv_reader_local_truecaller = new csv_reader(this);
    csv_reader_local_truecaller->do_job_plugin_specific(plgn_csv_file_in_result_truecaller, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_truecaller, csv_reader_local_truecaller);
    update_load_case_artifacts_tab_entry_config(plgn_truecaller, csv_reader_local_truecaller);

}



void MainWindow::update_result_plugin_music_999()
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
void MainWindow::update_result_plugin_google_locations_999()
{
    QString plgn_google_locations = "Google Locations";
    QString plgn_tmp_google_locations = plgn_google_locations;
    plgn_tmp_google_locations.replace(" ", "_");


    QString plgn_result_csv_google_locations = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_google_locations +".csv";
    QString plgn_csv_app_bundle_google_locations  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_google_locations + ".csv";
    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_google_locations, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_google_locations, plgn_result_csv_google_locations);

    csv_reader *csv_reader_local_google_locations = new csv_reader(this);
    csv_reader_local_google_locations->do_job_plugin_specific(plgn_result_csv_google_locations, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plgn_google_locations, csv_reader_local_google_locations);
    update_load_case_artifacts_tab_entry_config(plgn_google_locations, csv_reader_local_google_locations);
}

void MainWindow::update_result_plugin_reminders_999()
{
    QString plugin_name = "Reminders";
    QString plgn_tmp_nme = plugin_name;
    plgn_tmp_nme.replace(" ", "_");

    QString plgn_result_csv_reminders = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString() + plgn_tmp_nme +".csv";
    QString plgn_csv_app_bundle_reminders  = "../Resources/CSVS/Plugins_CSV/" + plgn_tmp_nme + ".csv";

    recon_static_functions::remove_safely_blank_dir_OR_file(plgn_result_csv_reminders, Q_FUNC_INFO);
    QFile::copy(plgn_csv_app_bundle_reminders, plgn_result_csv_reminders);

    csv_reader *csv_reader_local_reminders = new csv_reader(this);
    csv_reader_local_reminders->do_job_plugin_specific(plgn_result_csv_reminders, Q_FUNC_INFO);

    update_load_case_artifacts_plugin_related_config(plugin_name, csv_reader_local_reminders);
    update_load_case_artifacts_tab_entry_config(plugin_name, csv_reader_local_reminders);

}
