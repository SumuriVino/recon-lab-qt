#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::display_plugin(QString plugin_name_with_record_count)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    QString tmp_plugin_str = plugin_name_with_record_count;
    QString plugin_name;
    if(tmp_plugin_str.contains("("))
        plugin_name = tmp_plugin_str.remove(tmp_plugin_str.indexOf("("),tmp_plugin_str.size()).trimmed();
    else
        plugin_name = tmp_plugin_str;

    Container_QWidget *container = new Container_QWidget(this);

    plugin_viewer *plugin_viewer_obj = new plugin_viewer(container);

    connect(plugin_viewer_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(plugin_viewer_obj,SIGNAL(signal_refresh_widgets()),this,SLOT(slot_refresh_widgets()),Qt::QueuedConnection);
    connect(plugin_viewer_obj,SIGNAL(signal_keyword_searched_clicked()),this,SLOT(slot_refresh_widgets()),Qt::QueuedConnection);
    connect(plugin_viewer_obj,SIGNAL(signal_viewer_extarction_completed(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString,QString)),Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_plugin_extraction(struct struct_global_task_type_and_title)), recon_case_obj, SLOT(slot_cancel_plugin_extraction(struct struct_global_task_type_and_title)),Qt::QueuedConnection);
    connect(this,SIGNAL(signal_goto_plugin_record(QString,QString)),plugin_viewer_obj,SLOT(slot_goto_record(QString,QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_show_file_in_viewer_master_frame(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)),Qt::QueuedConnection);
    connect(plugin_viewer_obj,SIGNAL(signal_openwith_after_sendto_bucket_master_frame(QString,QString)),this,SLOT(slot_openwith_after_sendto_bucket_master_interface(QString,QString)),Qt::QueuedConnection);
    connect(plugin_viewer_obj,SIGNAL(signal_hashset_finished(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(plugin_viewer_obj,SIGNAL(signal_change_bookmark_status_in_other_db_also(QString,QString,QString,QString,QStringList,QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList,QString, QString, QString)),Qt::QueuedConnection);
    connect(plugin_viewer_obj,SIGNAL(signal_change_notes_value_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(plugin_viewer_obj,SIGNAL(signal_open_in_detach_clicked(QString,QString,QString,int,QString, QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString, QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_add_current_record_info_in_tag_search_db(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_add_notes_added_in_db(QString,QString, QString, QString,QString, QString,QString, QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString, QString, QString, QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_remove_current_record_entry_from_tag_search_display(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_add_current_tag_into_loader_class_sub_menu(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_tablewidget_loader_and_file_system(bool,QString,QString)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_plugin(bool,QString,QString)),plugin_viewer_obj,SLOT(slot_update_tag_submenu(bool,QString,QString)));
    connect(plugin_viewer_obj, SIGNAL(signal_update_other_db_and_display_also(QString,QString,QString, QStringList, QString)), this, SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString, QStringList, QString)));
    connect(plugin_viewer_obj, SIGNAL(signal_update_created_hashset_db_into_other_submenu_from_plugin(QString, QString)), this, SLOT(slot_update_hashset_menu_in_other_submenu_of_hashset_from_plugin(QString, QString)));
    connect(this, SIGNAL(signal_update_hashset_submenu_of_plugin(QString,QString)),plugin_viewer_obj, SLOT(slot_update_plugin_hashset_submenu(QString, QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_go_to_artifact_source_in_file_system(QString,QString,QString)),this,SLOT(slot_go_to_artifact_source(QString,QString,QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool,QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(plugin_viewer_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));

    connect(plugin_viewer_obj,SIGNAL(signal_plugin_viewer_space_pressed_for_quicklook(QString)),this,SLOT(slot_plugin_viewer_space_pressed_for_quicklook(QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(plugin_viewer_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(plugin_viewer_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Artifacts + MACRO_RECON_Splitter_1_multiple + plugin_name_with_record_count;
    QVBoxLayout *vbox_layout = new QVBoxLayout(container);
    vbox_layout->addWidget(plugin_viewer_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(plugin_name_with_record_count);
    container->pub_set_category_name(MACRO_CASE_TREE_Artifacts);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts,plugin_name_with_record_count);


    ////-------------------
    plugin_viewer_obj->pub_set_plugin_name(plugin_name);
    plugin_viewer_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    plugin_viewer_obj->pub_set_recon_case_obj(recon_case_obj);
    plugin_viewer_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    plugin_viewer_obj->pub_set_essentials();
    plugin_viewer_obj->pub_set_display_default_tags();

    plugin_viewer_obj->pub_display_data();


    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::create_recon_necessary_paths()
{

    QString output_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Output_Directory_QString).toString();

    QString lib_recon_cnfg_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Version_Dir_Path_Recon_Configuration_QString).toString() + QString("recon_configuration.sqlite");

    QString cmd = "delete from case_result_info";
    recon_helper_standard_obj->execute_db_command_by_dbpath(cmd,lib_recon_cnfg_db_path,Q_FUNC_INFO);

    cmd = "insert into case_result_info(result_path) values(?)";
    QStringList list_cc;
    list_cc << output_dir;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(cmd,list_cc,lib_recon_cnfg_db_path,Q_FUNC_INFO);

    QString format_str = QString("yyyy-MMM-ddThh-mm-ss");
    QString local_current_time = QDateTime::currentDateTime().toString(format_str);

    QDateTime datetime;
    datetime.setTimeSpec(Qt::UTC);
    datetime = QDateTime::fromString(local_current_time, format_str);
    qint64 current_timestamp_utc = datetime.toSecsSinceEpoch();

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_UTC_qint64, current_timestamp_utc);

    struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString current_machine_timezone_str = strct_exm_obj.timezone_name_complete_QString;

    qint64 current_machine_timezone_offset = strct_exm_obj.timezone_offset_numeric_qint64;

    qint64 current_time_with_zone = current_timestamp_utc + current_machine_timezone_offset;

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timezone_QString, current_machine_timezone_str);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timezone_qint64, current_machine_timezone_offset);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Created_Timestamp_with_Timezone_qint64, current_time_with_zone);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Current_Machine_Timezone_QString, current_machine_timezone_str);

    QString recon_result_dir = output_dir + "/" + launched_case_name.replace(" ","_") + "_" + local_current_time + "/";
    QDir dir;
    dir.mkdir(recon_result_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Result_Dir_QString,recon_result_dir);
    recon_static_functions::set_result_path(recon_result_dir);

    create_and_retrieve_recon_result_dir_structure(recon_result_dir);
    insert_recent_load_case_result_info(recon_result_dir);

    //====================Print Info Start=================//
    global_recon_helper_singular_class_obj->pub_fill_destination_drive_info(Q_FUNC_INFO);
    struct_global_destination_drive_info destination_drive_info_obj = global_recon_helper_singular_class_obj->get_saved_destination_drive_info(Q_FUNC_INFO);
    recon_static_functions::print_destination_drive_info(destination_drive_info_obj, Q_FUNC_INFO);
    //====================Print Info End=================//


    text_viewer_aascii_unicode_obj->pub_set_essentials();


    file_indexing_display_obj->pub_set_essentials();
    file_indexing_display_obj->pub_display_selected_files();
}





