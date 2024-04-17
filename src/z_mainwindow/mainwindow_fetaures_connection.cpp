#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::r_case_connections()
{
    connect(recon_case_obj, SIGNAL(signal_remove_plugin_entry_from_display_tree(QStringList)), this, SLOT(slot_remove_plugin_entry_from_display_tree(QStringList)),Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_plugin_extraction(struct struct_global_task_type_and_title)), recon_case_obj, SLOT(slot_cancel_plugin_extraction(struct struct_global_task_type_and_title)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_remove_sub_process_status_window_object(struct_global_task_type_and_title)), task_status_window_obj, SLOT(slot_remove_sub_process_status_window_object(struct_global_task_type_and_title)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_remove_category_and_feature_box_for_run_plugin()), this, SLOT(slot_remove_category_and_feature_box_for_run_plugin()),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_set_progress_bar_max_value(struct_global_task_type_and_title, int)), task_status_window_obj, SLOT(slot_set_progress_bar_max_value(struct_global_task_type_and_title, int)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_current_plugin_extraction_finished_or_cancelled(QString)), this, SLOT(slot_current_plugin_extraction_finished_or_cancelled(QString)),Qt::QueuedConnection);
    // connect(this, SIGNAL(signal_right_click_run_plugins_clicked()), r_case, SLOT(slot_right_click_run_plugins_clicked()),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_show_progressbar(bool)), this, SLOT(slot_show_progressbar(bool)),Qt::QueuedConnection);

    connect(recon_case_obj, SIGNAL(signal_keyword_search_extraction_start(QString)), this, SLOT(slot_keyword_search_extraction_start(QString)),Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_keyword_search(struct_global_task_type_and_title)), recon_case_obj, SLOT(slot_cancel_keyword_search(struct_global_task_type_and_title)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_keyword_search_finished_or_cancelled(QString)), this, SLOT(slot_right_click_keyword_search_finished_or_cancelled(QString)),Qt::QueuedConnection);

    connect(recon_case_obj, SIGNAL(signal_artifacts_timeline_full_clicked(QString)), this, SLOT(slot_right_click_artifacts_timeline_full(QString)),Qt::QueuedConnection);

    connect(recon_case_obj, SIGNAL(signal_global_report_extraction_start(QString)), this, SLOT(slot_global_report_extraction_start(QString)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_global_report_finished_or_cancelled(QStringList)), this, SLOT(slot_right_click_global_report_finished_or_cancelled(QStringList)),Qt::QueuedConnection);
    connect(recon_case_obj,SIGNAL(signal_add_tags_in_case_tree(QString)),this,SLOT(slot_common_add_tag_name_into_case_tree(QString)));


    connect(recon_case_obj, SIGNAL(signal_extracted_records_by_plugin(qint64)), this, SLOT(slot_extracted_records_by_plugin(qint64)),Qt::QueuedConnection);


    connect(recon_case_obj, SIGNAL(signal_set_progressbar_value(int,QString)), this, SLOT(slot_set_progressbar_value(int,QString)));

    connect(treewidget_case_display_obj,SIGNAL(signal_tree_run_plugins()),recon_case_obj,SLOT(slot_right_click_run_plugins_clicked()),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_artifacts_timeline_full()),recon_case_obj,SLOT(slot_right_clicked_artifacts_timeline_full()),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_tree_global_report()),recon_case_obj,SLOT(slot_right_click_global_report_clicked()),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_remove_selected_item()),this,SLOT(slot_right_click_remove_selected_item()),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_add_to_text_indexing(QString, QString)),this,SLOT(slot_right_click_add_to_text_indexing(QString, QString)),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_remove_story(QString)),this,SLOT(slot_right_click_remove_story(QString)),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_carve_unallocated_space(QString, QString, bool)),this,SLOT(slot_carve_files_clicked(QString, QString, bool)),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_snapshot_clicked(QString)),this,SLOT(slot_right_click_snapshot_clicked(QString)),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_disk_hex_view(QString, QString, bool)),this,SLOT(slot_right_click_disk_hex_view(QString, QString, bool)),Qt::QueuedConnection);
    connect(treewidget_case_display_obj,SIGNAL(signal_right_click_view_forensic_image_in_hex_viewer(QString, bool)),this,SLOT(slot_right_click_view_forensic_image_in_hex_viewer(QString, bool)),Qt::QueuedConnection);

    connect(recon_case_obj, SIGNAL(signal_remove_hashset_entry_from_status_window(struct_global_task_type_and_title)), task_status_window_obj, SLOT(slot_remove_hashset_status_window_object(struct_global_task_type_and_title)),Qt::QueuedConnection);

    connect(recon_case_obj, SIGNAL(signal_redefined_result_finished_or_cancelled(QString)), this, SLOT(slot_redefined_result_finished_or_cancelled(QString)));
    connect(recon_case_obj, SIGNAL(signal_change_display_message_on_task_progress_bar(QString,QString,QString,bool,int)), this, SLOT(slot_change_display_message_of_task_bar(QString,QString,QString,bool,int)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_file_search_finished_or_cancelled(QString)), this, SLOT(slot_file_search_finished_or_cancelled(QString)),Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_file_search(struct_global_task_type_and_title)), recon_case_obj, SLOT(slot_cancel_file_search_thread_extraction(struct_global_task_type_and_title)));

    connect(recon_case_obj, SIGNAL(signal_content_search_extraction_start(QString)), this, SLOT(slot_content_search_extraction_start(QString)),Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_content_search_extraction_finished_or_cancelled(QString)), this, SLOT(slot_content_search_extraction_finished_or_cancelled(QString)),Qt::QueuedConnection);
    connect(task_status_window_obj, SIGNAL(signal_cancel_content_search(struct_global_task_type_and_title)), recon_case_obj, SLOT(slot_cancel_content_search(struct_global_task_type_and_title)));
    connect(recon_case_obj, SIGNAL(signal_insert_redefined_result_entry_in_case_tree(QString)), this, SLOT(slot_redefined_result_finished_or_cancelled(QString)));
    connect(recon_case_obj, SIGNAL(signal_remove_entry_file_type_based_in_stack_and_refresh_widget(QString, QString)), this, SLOT(slot_remove_entry_file_type_based_in_stack_and_refresh_widget(QString, QString)));
    connect(recon_case_obj, SIGNAL(signal_disable_case_tree_sidebar(bool)), this, SLOT(slot_disable_case_tree_sidebar(bool)), Qt::QueuedConnection);
    connect(recon_case_obj, SIGNAL(signal_update_records_for_hashset_in_case_tree(QStringList, QString)), this, SLOT(slot_hashset_finished_or_cancelled_for_rcase(QStringList, QString)));
    connect(recon_case_obj, SIGNAL(signal_r_case_add_all_available_hashes_to_case_tree()), this, SLOT(slot_add_all_available_hashset_in_case_tree()));
    connect(recon_case_obj, SIGNAL(signal_set_search_label_with_records_in_progress_window_content_search(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(recon_case_obj, SIGNAL(signal_r_case_thread_exif_metadata_search_cancelled()), this, SLOT(slot_r_case_thread_exif_metadata_search_cancelled()));
    connect(recon_case_obj, SIGNAL(signal_prc_apple_metadata_prc_left_click_finished()), this, SLOT(slot_prc_apple_metadata_prc_left_click_finished()));
    connect(recon_case_obj, SIGNAL(signal_PBAR_artifacts_loading_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(recon_case_obj, SIGNAL(signal_PBAR_artifacts_loading_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(recon_case_obj, SIGNAL(signal_PBAR_artifacts_loading_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
}


void MainWindow::prepare_display_for_mobile_backup(QString tab_text)
{

    mobile_backup_loader *mobile_backup_loader_obj = new mobile_backup_loader(this);
    connect(mobile_backup_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(mobile_backup_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(mobile_backup_loader_obj,SIGNAL(signal_add_source_for_extraction(struct_GLOBAL_witness_info_source)),this,SLOT(slot_right_click_add_source(struct_GLOBAL_witness_info_source)));
    connect(mobile_backup_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(mobile_backup_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));


    mobile_backup_loader_obj->pub_set_recon_case_obj(recon_case_obj);
    mobile_backup_loader_obj->pub_set_essentials();
    mobile_backup_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Mobile_Backup_Parent);
    mobile_backup_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Mobile_Backup);
    mobile_backup_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Mobile_Backup_TAB_Name_Backup);
    mobile_backup_loader_obj->pub_set_display_tab_name_list(tab_list);
    mobile_backup_loader_obj->pub_create_ui();
    mobile_backup_loader_obj->pub_set_table_display_essentials();
    mobile_backup_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);


    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Mobile_Backup_Parent + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(mobile_backup_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Mobile_Backup_Parent);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Mobile_Backup_Parent,tab_text);






    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    mobile_backup_loader_obj->pub_populate_data_in_table();


}

void MainWindow::prepare_display_for_disk_images(QString tab_text)
{
    disk_images_loader *disk_images_loader_obj = new disk_images_loader(this);
    connect(disk_images_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(disk_images_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(disk_images_loader_obj,SIGNAL(signal_add_source_for_extraction(struct_GLOBAL_witness_info_source)),this,SLOT(slot_right_click_add_source(struct_GLOBAL_witness_info_source)));
    connect(disk_images_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(disk_images_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));


    disk_images_loader_obj->pub_set_recon_case_obj(recon_case_obj);
    disk_images_loader_obj->pub_set_essentials();
    disk_images_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Disk_Images_Parent);
    disk_images_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Disk_Images);
    disk_images_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Disk_Images_TAB_Name_Images);
    disk_images_loader_obj->pub_set_display_tab_name_list(tab_list);
    disk_images_loader_obj->pub_create_ui();
    disk_images_loader_obj->pub_set_table_display_essentials();
    disk_images_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);


    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Disk_Images_Parent + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(disk_images_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Disk_Images_Parent);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Disk_Images_Parent,tab_text);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    disk_images_loader_obj->pub_populate_data_in_table();
}

void MainWindow::prepare_display_for_keyword_search(QString tab_text)
{
    artifacts_keyword_search_loader *keyword_search_loader_obj = new artifacts_keyword_search_loader(this);
    connect(keyword_search_loader_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float )),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(keyword_search_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(keyword_search_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(keyword_search_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_plugin(QString,QString, QString, QString,QString, QString, QString,QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString,QString,QString, QString )));
    connect(keyword_search_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_fs(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString,QString,QString,QString,QString,QString)));
    connect(keyword_search_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(keyword_search_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),keyword_search_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),keyword_search_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(keyword_search_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(keyword_search_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    keyword_search_loader_obj->pub_set_essentials();
    keyword_search_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Artifacts_Keyword_Search);
    keyword_search_loader_obj->pub_set_plugin_name(MACRO_CASE_TREE_Artifacts_Keyword_Search);
    keyword_search_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    keyword_search_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    keyword_search_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    keyword_search_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    //    QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "artifacts_keyword_search_csv/";
    //    keyword_search_loader_obj->pub_set_csv_dir_path(csv_path);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Artifacts_Keyword_Search);
    keyword_search_loader_obj->pub_set_display_tab_name_list(tab_list);
    keyword_search_loader_obj->pub_create_ui();
    keyword_search_loader_obj->pub_set_table_display_essentials();
    keyword_search_loader_obj->configure_keyword_search_loader(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(),
                                                               tab_text,global_narad_muni_class_obj->get_field(MACRO_NARAD_Examiner_Selected_Timezone_qint64).toLongLong(),
                                                               tab_text);



    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple +MACRO_CASE_TREE_Artifacts_Keyword_Search + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(keyword_search_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Artifacts_Keyword_Search);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Keyword_Search,tab_text);






    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    keyword_search_loader_obj->pub_populate_data_in_table();
}

void MainWindow::prepare_display_ram_analysis_saved(QString tab_text)
{
    ram_analysis_saved_loader *ram_analysis_saved_loader_obj = new ram_analysis_saved_loader(this);
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString,QString)));
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),ram_analysis_saved_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(ram_analysis_saved_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(ram_analysis_saved_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";


    ram_analysis_saved_loader_obj->pub_set_essentials();
    ram_analysis_saved_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Ram_Analysis_Saved);
    ram_analysis_saved_loader_obj->pub_set_plugin_name(MACRO_CASE_TREE_Ram_Analysis_Saved);
    ram_analysis_saved_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    ram_analysis_saved_loader_obj->pub_set_ram_analysis_saved_display_db_path(destination_db_path);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Ram_Analysis_Saved);

    ram_analysis_saved_loader_obj->pub_set_display_tab_name_list(tab_list);
    ram_analysis_saved_loader_obj->pub_create_ui();
    ram_analysis_saved_loader_obj->pub_set_table_display_essentials();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Ram_Analysis_Saved + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(ram_analysis_saved_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Ram_Analysis_Saved);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Ram_Analysis_Saved,tab_text);



    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    ram_analysis_saved_loader_obj->populate_ram_analysis_saved_data_in_table();

}

void MainWindow::prepare_display_for_filetypes_and_based(QString tab_text)
{
    QTreeWidgetItem *currentItem =  treewidget_case_display_obj->currentItem();

    QString filetype_str =  currentItem->text(enum_global_case_tree_display_column_ZERO);
    qint64 total_records_count = 0;

    if(filetype_str.contains("("))
    {
        QString tmp_file_type = currentItem->text(enum_global_case_tree_display_column_ZERO);

        QString records_count = tmp_file_type.mid(tmp_file_type.lastIndexOf("(") + 1, tmp_file_type.lastIndexOf(")")).trimmed();


        if(records_count.endsWith(QString(")")))
            records_count.chop(1);

        total_records_count = records_count.toInt();

        if(records_count == QString("0"))
            filetype_str = currentItem->text(enum_global_case_tree_display_column_ZERO);
        else
        {
            filetype_str = filetype_str.mid(0, filetype_str.lastIndexOf("(")).trimmed();
        }
    }

    QString source_name = currentItem->text(enum_global_case_tree_display_column_FIRST);
    source_name = source_name.trimmed();
    if(source_name.isEmpty())
        return;

    QCoreApplication::processEvents();

    filetypes_obj = new filetypes_loader(this);
    filetypes_obj->pub_set_face_finder_obj(facefinder_obj);

    connect(filetypes_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float)),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(filetypes_obj, SIGNAL(signal_disable_case_tree_sidebar(bool)), this, SLOT(slot_disable_case_tree_sidebar(bool)));
    connect(filetypes_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(filetypes_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(filetypes_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(filetypes_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(filetypes_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(filetypes_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(filetypes_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(filetypes_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(filetypes_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),filetypes_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(filetypes_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_exif_data_content_on_detatch_click(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(filetypes_obj,SIGNAL(signal_update_tag_search_for_bookmark(QString,QString,QString,QString,QStringList,QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList,QString, QString, QString)));
    connect(filetypes_obj,SIGNAL(signal_update_tag_search_for_notes(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)));
    connect(filetypes_obj,SIGNAL(signal_add_notes_from_file_system(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString, QString, QString,QString, QString, QString )));
    connect(filetypes_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(filetypes_obj,SIGNAL(signal_decompressed_files_view(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_decompressed_files_view(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(filetypes_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool,QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool,QString)));
    connect(filetypes_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),filetypes_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(filetypes_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(filetypes_obj, SIGNAL(signal_disable_case_tree_sidebar(bool)), this, SLOT(slot_disable_case_tree_sidebar(bool)), Qt::QueuedConnection);
    connect(filetypes_obj,SIGNAL(signal_update_tag_search_db_and_display_on_bookmark(QStringList,QString,QString,QString)),this,SLOT(slot_update_bookmark_value_in_tag_search_for_filetypes(QStringList,QString,QString,QString)));
    connect(filetypes_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));
    connect(filetypes_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(filetypes_obj, SIGNAL(signal_add_saved_maps_in_case_tree()), this, SLOT(slot_add_saved_maps_in_case_tree()));
    connect(filetypes_obj, SIGNAL(signal_carve_files_clicked(QString,QString, bool)), this, SLOT(slot_carve_files_clicked(QString,QString, bool)));
    connect(filetypes_obj, SIGNAL(signal_carve_data_clicked(QString,QString,QString,QString,QString)), this, SLOT(slot_carve_data_clicked(QString,QString,QString,QString,QString)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_hashset(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_hashset(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_weapons(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_weapons(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details>)));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details>)));

    connect(filetypes_obj, SIGNAL(signal_run_fs_module_apple_metadata_already_running_status()), this, SLOT(slot_run_fs_module_apple_metadata_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_exif_metadata_already_running_status()), this, SLOT(slot_run_fs_module_exif_metadata_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_signature_analysis_already_running_status()), this, SLOT(slot_run_fs_module_signature_analysis_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_mime_type_analysis_already_running_status()), this, SLOT(slot_run_fs_module_mime_type_analysis_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_hashset_already_running_status()), this, SLOT(slot_run_fs_module_hashset_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_face_analysis_already_running_status()), this, SLOT(slot_run_fs_module_face_analysis_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_optical_character_recognition_already_running_status()), this, SLOT(slot_run_fs_module_optical_character_recognition_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_weapons_already_running_status()), this, SLOT(slot_run_fs_module_weapons_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_fire_analysis_already_running_status()), this, SLOT(slot_run_fs_module_fire_analysis_already_running_status()));
    connect(filetypes_obj, SIGNAL(signal_run_fs_module_skin_tone_detection_already_running_status()), this, SLOT(slot_run_fs_module_skin_tone_detection_already_running_status()));

    connect(filetypes_obj, SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(filetypes_obj, SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(filetypes_obj, SIGNAL(signal_right_click_removed_record_refresh_in_case_tree(QStringList,QString)),recon_case_obj, SLOT(slot_right_click_remove_record_refresh_in_case_tree(QStringList,QString)));

    connect(this, SIGNAL(signal_r_case_apple_metadata_running_status(bool)), filetypes_obj, SLOT(slot_r_case_apple_metadata_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_exif_metadata_running_status(bool)), filetypes_obj, SLOT(slot_r_case_exif_metadata_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_signature_analysis_running_status(bool)), filetypes_obj, SLOT(slot_r_case_signature_analysis_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_mime_type_analysis_running_status(bool)), filetypes_obj, SLOT(slot_r_case_mime_type_analysis_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_hashset_running_status(bool)), filetypes_obj, SLOT(slot_r_case_hashset_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_face_analysis_running_status(bool)), filetypes_obj, SLOT(slot_r_case_face_analysis_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_optical_character_recognition_running_status(bool)), filetypes_obj, SLOT(slot_r_case_optical_character_recognition_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_weapons_running_status(bool)), filetypes_obj, SLOT(slot_r_case_weapons_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_fire_analysis_running_status(bool)), filetypes_obj, SLOT(slot_r_case_fire_analysis_running_status(bool)));
    connect(this, SIGNAL(signal_r_case_skin_tone_detection_running_status(bool)), filetypes_obj, SLOT(slot_r_case_skin_tone_detection_running_status(bool)));

    filetypes_obj->pub_set_recon_case_obj(recon_case_obj);
    filetypes_obj->pub_set_essentials();
    filetypes_obj->pub_set_case_tree_obj(treewidget_case_display_obj);

    filetypes_obj->pub_set_feature_name(QString(MACRO_CASE_TREE_Filetype_Parent));
    filetypes_obj->pub_set_plugin_name(QString(MACRO_Plugin_Name_File_System));
    filetypes_obj->pub_set_clicked_item_tab_text(tab_text);
    filetypes_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    filetypes_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);


    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_File_System_TAB_Name_Files);

    filetypes_obj->pub_set_display_tab_name_list(tab_list);
    filetypes_obj->pub_create_ui();
    filetypes_obj->pub_set_table_display_essentials();
    filetypes_obj->configure_filetypes_loader(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() , filetype_str , source_name, total_records_count, currentItem);

    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + QString(MACRO_RECON_Splitter_1_multiple) + source_name + QString(MACRO_RECON_Splitter_1_multiple) + tab_text;

    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(filetypes_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);

    container->pub_set_category_name(source_name);

    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(), source_name, tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    filetypes_obj->pub_populate_data_in_table();
}

Container_QWidget * MainWindow::get_currenly_loaded_display_obj()
{
    Container_QWidget *widget = (Container_QWidget *)stack_widget_cases_plugins_and_global_items->currentWidget();
    return widget;
}

void MainWindow::prepare_display_for_filesystem(QString tab_text , QString itemp2 , QString db_path)
{

    QString source_count_name;

    QStringList tmp_list = db_path.split("/", Qt::SkipEmptyParts);
    if(tmp_list.size() > 2)
        source_count_name = tmp_list.at(tmp_list.size() - 2);

    source_count_name = source_count_name.trimmed();


    if(source_count_name.isEmpty())
    {
        recon_static_functions::app_debug(" ---FAILED--- blank source count name ",Q_FUNC_INFO);
        return;
    }

    disable_case_tree_sidebar(true);

    file_system *file_system_obj = map_file_system_obj.value(source_count_name);
    if(file_system_obj == NULL)
    {
        file_system_obj = new file_system(this);
        connect(file_system_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
        connect(this, SIGNAL(signal_fs_double_clicked_dir_extraction_completed(QString)),file_system_obj, SLOT(slot_fs_double_clicked_dir_extraction_completed(QString)));
        connect(file_system_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
        //connect(file_system_obj,SIGNAL(signal_fs_item_clicked(QString,QString,QString,QString)),this,SLOT(slot_global_item_clicked(QString,QString,QString,QString)));
        connect(file_system_obj, SIGNAL(signal_add_saved_maps_in_case_tree()), this, SLOT(slot_add_saved_maps_in_case_tree()));
        connect(file_system_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
        connect(file_system_obj,SIGNAL(signal_fs_refresh_widgets()),this,SLOT(slot_refresh_widgets()));
        connect(file_system_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
        connect(file_system_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
        connect(file_system_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString,QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString,QString)));
        connect(file_system_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
        connect(this , SIGNAL(signal_goto_file_system_record(QString, QString, QString)) ,file_system_obj,SLOT(slot_go_to_file_system_record(QString,QString, QString)));
        connect(this , SIGNAL(signal_decompressed_files_view_file_system_record(QString)) ,file_system_obj,SLOT(slot_dir_navigator_pushbutton_clicked(QString)));

        connect(file_system_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
        connect(file_system_obj,SIGNAL(signal_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList,QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList,QString, QString, QString)));
        connect(file_system_obj,SIGNAL(signal_add_notes_from_file_system(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString, QString, QString,QString, QString, QString )));
        connect(file_system_obj,SIGNAL(signal_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)));
        connect(file_system_obj,SIGNAL(signal_file_system_removed_tags_list(QString, QString, QString, QStringList,QString)), this, SLOT(slot_remove_tag_entry_in_other_db(QString, QString, QString, QStringList,QString)));
        connect(file_system_obj,SIGNAL(signal_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
        connect(file_system_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
        connect(this,SIGNAL(signal_update_tags_submenu_of_file_system(bool,QString,QString)),file_system_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
        connect(file_system_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_tablewidget_loader(bool,QString,QString)));
        connect(this,SIGNAL(signal_update_hashset_submenu_of_file_system(QString, QString)),file_system_obj,SLOT(slot_update_hashset_submenu_of_file_system(QString, QString)));
        connect(file_system_obj,SIGNAL(signal_update_hashset_submenu_from_file_system(QString, QString)),this,SLOT(slot_update_hashset_submenu_from_file_system(QString, QString)));
        connect(file_system_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool,QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool,QString)));
        connect(file_system_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
        connect(file_system_obj, SIGNAL(signal_run_index_k_search_on_current_dir(QStringList, bool, QString, QString)), this, SLOT(slot_index_search_on_file_system_current_dir(QStringList, bool, QString, QString)));
        connect(file_system_obj, SIGNAL(signal_disable_case_tree_sidebar(bool)), this, SLOT(slot_disable_case_tree_sidebar(bool)), Qt::QueuedConnection);
        connect(file_system_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));
        connect(file_system_obj, SIGNAL(signal_carve_files_clicked(QString,QString, bool)), this, SLOT(slot_carve_files_clicked(QString,QString, bool)));
        connect(file_system_obj, SIGNAL(signal_carve_data_clicked(QString,QString,QString,QString,QString)), this, SLOT(slot_carve_data_clicked(QString,QString,QString,QString,QString)));

        connect(file_system_obj, SIGNAL(signal_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_hashset(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_hashset(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_weapons(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_weapons(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details>)));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details>)), this, SLOT(slot_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details>)));

        connect(file_system_obj, SIGNAL(signal_run_fs_module_face_analysis_already_running_status()), this, SLOT(slot_run_fs_module_face_analysis_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_apple_metadata_already_running_status()), this, SLOT(slot_run_fs_module_apple_metadata_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_exif_metadata_already_running_status()), this, SLOT(slot_run_fs_module_exif_metadata_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_signature_analysis_already_running_status()), this, SLOT(slot_run_fs_module_signature_analysis_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_mime_type_analysis_already_running_status()), this, SLOT(slot_run_fs_module_mime_type_analysis_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_hashset_already_running_status()), this, SLOT(slot_run_fs_module_hashset_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_optical_character_recognition_already_running_status()), this, SLOT(slot_run_fs_module_optical_character_recognition_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_weapons_already_running_status()), this, SLOT(slot_run_fs_module_weapons_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_fire_analysis_already_running_status()), this, SLOT(slot_run_fs_module_fire_analysis_already_running_status()));
        connect(file_system_obj, SIGNAL(signal_run_fs_module_skin_tone_detection_already_running_status()), this, SLOT(slot_run_fs_module_skin_tone_detection_already_running_status()));
        connect(file_system_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
        connect(file_system_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

        connect(this, SIGNAL(signal_r_case_face_analysis_running_status(bool)), file_system_obj, SLOT(slot_r_case_face_analysis_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_optical_character_recognition_running_status(bool)), file_system_obj, SLOT(slot_r_case_optical_character_recognition_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_weapons_running_status(bool)), file_system_obj, SLOT(slot_r_case_weapons_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_fire_analysis_running_status(bool)), file_system_obj, SLOT(slot_r_case_fire_analysis_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_skin_tone_detection_running_status(bool)), file_system_obj, SLOT(slot_r_case_skin_tone_detection_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_apple_metadata_running_status(bool)), file_system_obj, SLOT(slot_r_case_apple_metadata_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_exif_metadata_running_status(bool)), file_system_obj, SLOT(slot_r_case_exif_metadata_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_signature_analysis_running_status(bool)), file_system_obj, SLOT(slot_r_case_signature_analysis_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_mime_type_analysis_running_status(bool)), file_system_obj, SLOT(slot_r_case_mime_type_analysis_running_status(bool)));
        connect(this, SIGNAL(signal_r_case_hashset_running_status(bool)), file_system_obj, SLOT(slot_r_case_hashset_running_status(bool)));

        file_system_obj->pub_set_necessary();
        file_system_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
        file_system_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
        file_system_obj->pub_set_recon_case_obj(recon_case_obj);
        file_system_obj->pub_set_case_treewidget_obj(treewidget_case_display_obj);
        file_system_obj->set_source_count_name(source_count_name);
        file_system_obj->pub_set_fs_db_path(db_path);
        file_system_obj->pub_set_face_finder_obj(facefinder_obj);

        file_system_obj->set_essentials();

        map_file_system_obj.insert(source_count_name, file_system_obj);

    }

    QCoreApplication::processEvents();

    if(itemp2 == "0")
    {
        //  file_system_obj->pub_insert_please_wait_in_fs_display();
    }
    else
    {
        QString dir_path = itemp2;
        current_fs_display_dir_path = dir_path;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        if(struct_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal))
            dir_path = dir_path.remove(struct_info.source_name);

        file_system_obj->make_display_for_specified_path(dir_path);
    }


    ///-Stackwidget insertion and display Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Source + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout(container);
    vbox_layout->addWidget(file_system_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Source);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Source,tab_text);


    disable_case_tree_sidebar(false);

}

void MainWindow::prepare_display_for_artifacts_timeline_full(QString tab_text)
{

    slot_show_metadata(false);
    artifacts_timeline_full_loader *artifacts_timeline_full_loader_obj = new artifacts_timeline_full_loader(this);
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),artifacts_timeline_full_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),artifacts_timeline_full_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float )),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(artifacts_timeline_full_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));
    connect(artifacts_timeline_full_loader_obj, SIGNAL(signal_common_add_notes_name_into_case_tree(QString)), this, SLOT(slot_add_notes_into_case_tree(QString)));
    connect(artifacts_timeline_full_loader_obj, SIGNAL(signal_add_saved_timeline_graph_in_case_tree()), this, SLOT(slot_add_saved_timeline_graph_in_case_tree()));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(artifacts_timeline_full_loader_obj, SIGNAL(signal_display_after_save_results(QString , QString)), this, SLOT(slot_display_for_artifacts_timeline_saved_results_and_graphs(QString , QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_save_tabs_from_artifacts_timeline(QString)),this,SLOT(slot_save_tabs_in_artifacts_timleine_saved_case_tree(QString)));
    connect(artifacts_timeline_full_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_full_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));

    artifacts_timeline_full_loader_obj->pub_set_essentials();
    artifacts_timeline_full_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    artifacts_timeline_full_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    artifacts_timeline_full_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    QString artifacts_timeline_saved_event_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_config.sqlite";
    artifacts_timeline_full_loader_obj->pub_set_saved_events_db_path(artifacts_timeline_saved_event_db_path);

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString() + "artifacts_timeline_full.sqlite";
    artifacts_timeline_full_loader_obj->pub_set_display_db_path(db_path);
    artifacts_timeline_full_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Artifacts_Timeline);
    artifacts_timeline_full_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Artifacts_Timeline);
    artifacts_timeline_full_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Artifacts_Timeline);
    artifacts_timeline_full_loader_obj->pub_set_display_tab_name_list(tab_list);
    artifacts_timeline_full_loader_obj->pub_create_ui();
    artifacts_timeline_full_loader_obj->pub_set_table_display_essentials();



    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Artifacts_Timeline + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(artifacts_timeline_full_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Artifacts_Timeline);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Timeline,tab_text);




    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    artifacts_timeline_full_loader_obj->pub_initialize_and_display_plugins_and_timestamps();

}

void MainWindow::prepare_display_artifacts_timeline_saved(QString tab_text)
{
    slot_show_metadata(false);

    artifacts_timeline_saved_loader *artifacts_timeline_saved_loader_obj = new artifacts_timeline_saved_loader(this);

    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),artifacts_timeline_saved_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(artifacts_timeline_saved_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));
    connect(artifacts_timeline_saved_loader_obj, SIGNAL(signal_common_add_notes_name_into_case_tree(QString)), this, SLOT(slot_add_notes_into_case_tree(QString)));
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(artifacts_timeline_saved_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    artifacts_timeline_saved_loader_obj->pub_set_essentials();
    artifacts_timeline_saved_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    artifacts_timeline_saved_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    artifacts_timeline_saved_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + tab_db_name + ".sqlite";

    artifacts_timeline_saved_loader_obj->pub_set_display_db_path(db_path);
    artifacts_timeline_saved_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Artifacts_Timeline);
    artifacts_timeline_saved_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Artifacts_Timeline);
    artifacts_timeline_saved_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Artifacts_Timeline);

    artifacts_timeline_saved_loader_obj->pub_set_display_tab_name_list(tab_list);
    artifacts_timeline_saved_loader_obj->pub_create_ui();
    artifacts_timeline_saved_loader_obj->pub_set_table_display_essentials();

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Artifacts_Timeline + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(artifacts_timeline_saved_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Artifacts_Timeline);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Timeline,tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    artifacts_timeline_saved_loader_obj->populate_data_in_table();

}

void MainWindow::prepare_display_for_plist_viewer(QString tab_text , QString db_path)
{
    plist_viewer_interface *plist_viewer_interface_obj = new plist_viewer_interface(this);

    connect(plist_viewer_interface_obj,SIGNAL(signal_add_tags_in_case_tree(QString)),this,SLOT(slot_common_add_tag_name_into_case_tree(QString)));
    connect(plist_viewer_interface_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(this,SIGNAL(signal_plist_viewer_db_path_changed(QString)),plist_viewer_interface_obj,SLOT(slot_plist_viewer_db_path_changed(QString)));
    //    connect(plist_viewer_interface_obj,SIGNAL(signal_add_tag_from_plist_viewer(QString,QString, QString, QString,QString, QString, QString, QString, QString)),this, SLOT(slot_add_tag_in_database_for_plist_viewer(QString, QString, QString,QString, QString, QString, QString, QString, QString )));
    //    connect(plist_viewer_interface_obj,SIGNAL(signal_plist_viewer_removed_tags_list(QString, QString, QString, QStringList,QString)), this, SLOT(slot_remove_tag_entry_in_other_db(QString, QString, QString, QStringList,QString)));
    //    connect(plist_viewer_interface_obj,SIGNAL(signal_update_tag_search_db_and_display(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags_in_plist_viewer(QString,QString,QString,QStringList,QString)));
    connect(plist_viewer_interface_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(plist_viewer_interface_obj,SIGNAL(signal_plist_item_clicked(QString,QString,QString,QString)),this,SLOT(slot_global_item_clicked(QString,QString,QString,QString)));



    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple +MACRO_CASE_TREE_Plist_Viewer + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(plist_viewer_interface_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Plist_Viewer);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Plist_Viewer,tab_text);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();


    plist_viewer_interface_obj->set_viewer_display_name(tab_text.trimmed());
    plist_viewer_interface_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    plist_viewer_interface_obj->set_db_path(db_path);
    plist_viewer_interface_obj->set_initialisation();


}
void MainWindow::prepare_display_for_log_viewer(QString tab_text, QString db_path)
{
    log_viewer_interface *log_viewer_interface_obj = new log_viewer_interface(this);

    connect(log_viewer_interface_obj,SIGNAL(signal_add_tags_in_case_tree(QString)),this,SLOT(slot_common_add_tag_name_into_case_tree(QString)));
    connect(log_viewer_interface_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(this,SIGNAL(signal_log_viewer_db_path_changed(QString)),log_viewer_interface_obj,SLOT(slot_log_viewer_db_path_changed(QString)));
    connect(log_viewer_interface_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    connect(log_viewer_interface_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));

    //    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Log_Viewer + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(log_viewer_interface_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Log_Viewer);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Log_Viewer,tab_text);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    log_viewer_interface_obj->set_viewer_display_name(tab_text.trimmed());
    log_viewer_interface_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    log_viewer_interface_obj->set_db_path(db_path);
    log_viewer_interface_obj->set_initialisation();
    log_viewer_interface_obj->fetch_metadata_for_log_viewer();
}

void MainWindow::prepare_display_for_unified_logs_viewer(QString tab_text, QString db_path)
{
    unified_logs_viewer *unified_logs_viewer_obj = new unified_logs_viewer(this);

    connect(unified_logs_viewer_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Unified_Logs + MACRO_RECON_Splitter_1_multiple + tab_text;

    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(unified_logs_viewer_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Unified_Logs);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(), MACRO_CASE_TREE_Unified_Logs, tab_text);

    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_obj->show();

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    unified_logs_viewer_obj->set_viewer_display_name(tab_text.trimmed());
    unified_logs_viewer_obj->set_essentials_for_unified_logs();
    unified_logs_viewer_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    display_loading_progress_bar_obj->hide();

}

void MainWindow::prepare_display_for_hex_viewer(QString tab_text , QString db_path)
{
    QString complete_file_path = st_hex_viewer_open_file_data_obj.complete_file_path.trimmed();
    if(complete_file_path.isEmpty())
        return;

    hex_viewer_complete *hex_displyr_obj = new hex_viewer_complete(this);

    connect(hex_displyr_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    hex_displyr_obj->pub_set_global_connection_manager(global_connection_manager_obj);
    hex_displyr_obj->pub_set_hex_viewer_open_file_data_obj(st_hex_viewer_open_file_data_obj);
    hex_displyr_obj->pub_set_source_file_size(QFile(complete_file_path).size());
    hex_displyr_obj->pub_set_source_block_size(512);
    hex_displyr_obj->pub_set_file_path(complete_file_path, st_hex_viewer_open_file_data_obj.display_file_path);


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple +MACRO_CASE_TREE_Hex_Viewer + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(hex_displyr_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Hex_Viewer);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Hex_Viewer,tab_text);

}

void MainWindow::prepare_hex_viewer_display_structure_data(QTreeWidgetItem *item, QString db_path)
{
    QString item_text = item->text(enum_global_case_tree_display_column_ZERO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select(source_db);

    QString command_str = QString("Select INT,file_path,record_no,source_count_name,plugin_name,category_name,recon_filefrom,recon_file_infopath From hex_viewer_index Where viewer_display_name = ?");

    query_select.prepare(command_str);
    query_select.addBindValue(item_text);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(" query_select ----FAILED---- " + query_select.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error: " + query_select.lastError().text(),Q_FUNC_INFO);
        return;
    }

    if(query_select.next())
    {
        st_hex_viewer_open_file_data_obj.bool_is_file_in_bucket = true;
        st_hex_viewer_open_file_data_obj.hex_viewer_display_name = item_text;

        QString int_str = query_select.value("INT").toString().trimmed();

        st_hex_viewer_open_file_data_obj.display_file_path   = query_select.value("file_path").toString().trimmed();
        st_hex_viewer_open_file_data_obj.record_no_str       = query_select.value("record_no").toString().trimmed();
        st_hex_viewer_open_file_data_obj.source_count_name   = query_select.value("source_count_name").toString().trimmed();
        st_hex_viewer_open_file_data_obj.plugin_name_str     = query_select.value("plugin_name").toString().trimmed();
        st_hex_viewer_open_file_data_obj.plugin_tab_name_str = query_select.value("category_name").toString().trimmed();
        st_hex_viewer_open_file_data_obj.hex_feature_db_path = db_path;
        st_hex_viewer_open_file_data_obj.recon_filefrom      = query_select.value("recon_filefrom").toString().trimmed();
        st_hex_viewer_open_file_data_obj.recon_file_infopath = query_select.value("recon_file_infopath").toString().trimmed();

        struct_GLOBAL_witness_info_source derived_source_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(st_hex_viewer_open_file_data_obj.source_count_name);

        QString file_type = recon_static_functions::get_forensic_image_type(st_hex_viewer_open_file_data_obj.display_file_path);
        if(st_hex_viewer_open_file_data_obj.hex_viewer_display_name.contains(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete)
           || st_hex_viewer_open_file_data_obj.hex_viewer_display_name.contains(MACRO_Plugin_Forensic_Image_In_Hex_Viewer_TAB_Name))
        {
            if(file_type == MACRO_Target_Disk_ImageType_ENCASE_PHYSICAL || file_type == MACRO_Target_Disk_ImageType_ENCASE_LOGICAL)
            {
                st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "ewf1";
            }
            else if(file_type == MACRO_Target_Disk_ImageType_VMDK)
            {
                st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "vmdk1";
            }
            else if(file_type == MACRO_Target_Disk_ImageType_VHD)
            {
                st_hex_viewer_open_file_data_obj.complete_file_path = derived_source_info_obj.fuse_mount_path_raw + "vhdi1";
            }
            else
            {
                st_hex_viewer_open_file_data_obj.complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(int_str, source_db, QString("hex_viewer_index"), st_hex_viewer_open_file_data_obj.source_count_name, Q_FUNC_INFO);
            }
        }
        else
        {
            st_hex_viewer_open_file_data_obj.complete_file_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(int_str, source_db, QString("hex_viewer_index"), st_hex_viewer_open_file_data_obj.source_count_name, Q_FUNC_INFO);
        }

    }

    source_db.close();
}

void MainWindow::prepare_display_for_sqlite_viewer(QString tab_text , QString db_path)
{
    tag_search_loader *tag_search_obj = new tag_search_loader(this);
    sqlite_viewer *sqlite_viewer_obj = new sqlite_viewer(this);

    connect(sqlite_viewer_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(sqlite_viewer_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));

    connect(sqlite_viewer_obj,SIGNAL(signal_sqlite_viewer_metadata(QString,QString,QString,QString)),this,SLOT(slot_global_item_clicked(QString,QString,QString,QString)),Qt::QueuedConnection);
    connect(sqlite_viewer_obj,SIGNAL(signal_refresh_widgets()),this,SLOT(slot_refresh_widgets()),Qt::QueuedConnection);
    connect(sqlite_viewer_obj,SIGNAL(signal_show_metadata(bool)),this,SLOT(slot_show_metadata(bool)),Qt::QueuedConnection);
    connect(sqlite_viewer_obj,SIGNAL(signal_sqlite_viewer_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(sqlite_viewer_obj,SIGNAL(signal_sqlite_viewer_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),tag_search_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(sqlite_viewer_obj,SIGNAL(signal_remove_tag_entry_from_tag_search_db_and_display(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(sqlite_viewer_obj,SIGNAL(signal_update_tag_search_db_and_display(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags_in_plist_viewer(QString,QString,QString,QStringList,QString)));
    connect(sqlite_viewer_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(sqlite_viewer_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));
    connect(sqlite_viewer_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple +MACRO_CASE_TREE_Sqlite_Viewer + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(sqlite_viewer_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Sqlite_Viewer);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);

    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Sqlite_Viewer,tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    sqlite_viewer_obj->setWindowTitle(tab_text.trimmed());
    sqlite_viewer_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
    sqlite_viewer_obj->pub_set_recon_case_obj(recon_case_obj);
    sqlite_viewer_obj->set_recon_result_dir(db_path,tab_text);

}

void MainWindow::prepare_display_for_bookmark_search(QString tab_text)
{
    bookmarks_search_loader_obj = new bookmarks_search_loader(this);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float )),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    bookmarks_search_loader_obj->pub_set_essentials();
    bookmarks_search_loader_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
    bookmarks_search_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    bookmarks_search_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);


    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");

    //    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    bookmarks_search_loader_obj->pub_bookmarks_search_set_display_db_path(db_path);
    bookmarks_search_loader_obj->pub_set_feature_name(MACRO_Tag_Name_Bookmarks);
    bookmarks_search_loader_obj->pub_set_plugin_name(MACRO_Tag_Name_Bookmarks);
    bookmarks_search_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    //    QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "bookmarks_search_csv/";
    //    bookmarks_search_obj->pub_set_csv_dir_path(csv_path);

    QStringList tab_list;
    tab_list << QString(MACRO_Tag_Name_Bookmarks);

    bookmarks_search_loader_obj->pub_set_display_tab_name_list(tab_list);
    bookmarks_search_loader_obj->pub_create_ui();

    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),bookmarks_search_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));

    bookmarks_search_loader_obj->pub_set_table_display_essentials();

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Tags + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(bookmarks_search_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Tags);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Tags,tab_text);


    connect(bookmarks_search_loader_obj,SIGNAL(signal_tablewidget_goto_record_snapshots(QString,QString,QString,QString)),this,SLOT(slot_tablewidget_goto_record_for_snapshots(QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),bookmarks_search_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),bookmarks_search_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_plugin(QString,QString, QString, QString,QString, QString, QString,QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString,QString,QString, QString )));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_fs(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString,QString,QString,QString,QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_hex_viewer_entry(QString,QString,QString, QString, QString,QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_hex_viewer(QString,QString,QString,QString,QString,QString,QString, QString)));
    connect(bookmarks_search_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(bookmarks_search_loader_obj, SIGNAL(signal_add_notes_from_result_loader_for_content_search(QString,QString,QString,QString,QString,QString)), this, SLOT(slot_add_notes_added_in_database_for_content_search(QString,QString,QString,QString,QString,QString)));
    connect(bookmarks_search_loader_obj, SIGNAL(signal_change_notes_value_in_index_k_search_db_and_display(QString, QString, QString, QString, QStringList, QString, QString, QString)), this, SLOT(slot_change_notes_value_in_index_k_search_db_and_display(QString, QString, QString, QString, QStringList, QString, QString, QString)));
    connect(bookmarks_search_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_screenshot(QString,QString,QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_screenshot(QString,QString,QString,QString,QString,QString)));

    connect(bookmarks_search_loader_obj, SIGNAL(signal_disable_case_tree_sidebar(bool)), this, SLOT(slot_disable_case_tree_sidebar(bool)), Qt::QueuedConnection);
    connect(bookmarks_search_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    bookmarks_search_loader_obj->bookmarks_search_populate_data_in_table();
}

void MainWindow::prepare_display_for_tag_search(QString tab_text)
{
    tag_search_obj = new tag_search_loader(this);
    connect(tag_search_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float )),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(tag_search_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(tag_search_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    tag_search_obj->pub_set_essentials();
    tag_search_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    tag_search_obj->pub_set_recon_case_obj(recon_case_obj);

    tag_search_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);


    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";

    tag_search_obj->pub_tag_search_set_display_db_path(db_path);
    tag_search_obj->pub_tag_search_set_current_tag(tab_text);
    tag_search_obj->pub_set_feature_name(MACRO_Plugin_Name_Tag_Search);
    tag_search_obj->pub_set_plugin_name(MACRO_Plugin_Name_Tag_Search);
    tag_search_obj->pub_set_clicked_item_tab_text(tab_text);
    //    QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "tag_search_csv/";
    //    tag_search_obj->pub_set_csv_dir_path(csv_path);

    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Name_Tag_Search);

    tag_search_obj->pub_set_display_tab_name_list(tab_list);
    tag_search_obj->pub_create_ui();

    connect(tag_search_obj,SIGNAL(signal_tablewidget_goto_record_snapshots(QString,QString,QString,QString)),this,SLOT(slot_tablewidget_goto_record_for_snapshots(QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_add_notes_from_result_loader_for_plist_viewer(QString,QString,QString, QString, QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_plist_viewer(QString,QString,QString,QString,QString,QString,QString, QString)));
    connect(tag_search_obj,SIGNAL(signal_add_notes_from_result_loader_for_sqlite_viewer(QString,QString,QString, QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_sqlite_viewer(QString,QString,QString,QString,QString,QString,QString)));


    tag_search_obj->pub_set_table_display_essentials();

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Tags + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(tag_search_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Tags);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Tags,tab_text);


    connect(tag_search_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(tag_search_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(tag_search_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),tag_search_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(tag_search_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(tag_search_obj,SIGNAL(signal_update_sqlite_db_for_remained_tags(QString,QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_sqlite_db_for_remained_tags(QString,QString,QString,QString,QStringList,QString)));
    connect(tag_search_obj,SIGNAL(signal_update_hex_and_text_db_for_removed_tags(QString,QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_hex_and_text_db_for_removed_tags(QString,QString,QString,QString,QStringList,QString)));
    connect(tag_search_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(tag_search_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),tag_search_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(tag_search_obj,SIGNAL(signal_add_notes_from_result_loader_for_plugin(QString,QString, QString, QString,QString, QString, QString,QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString,QString,QString, QString )));
    connect(tag_search_obj,SIGNAL(signal_add_notes_from_result_loader_for_fs(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString,QString,QString,QString,QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_add_notes_from_result_loader_for_hex_viewer_entry(QString,QString,QString, QString, QString,QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_hex_viewer(QString,QString,QString,QString,QString,QString,QString, QString)));
    connect(tag_search_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(tag_search_obj, SIGNAL(signal_change_notes_value_in_index_k_search_db_and_display(QString, QString, QString, QString, QStringList, QString, QString, QString)), this, SLOT(slot_change_notes_value_in_index_k_search_db_and_display(QString, QString, QString, QString, QStringList, QString, QString, QString)));
    connect(tag_search_obj, SIGNAL(signal_add_notes_from_result_loader_for_content_search(QString,QString,QString,QString,QString,QString)), this, SLOT(slot_add_notes_added_in_database_for_content_search(QString,QString,QString,QString,QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_add_notes_from_result_loader_for_screenshot(QString,QString,QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_screenshot(QString,QString,QString,QString,QString,QString)));
    connect(tag_search_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    tag_search_obj->tag_search_populate_data_in_table();
}

void MainWindow::prepare_display_for_notes_search(QString tab_text)
{
    notes_search_loader_obj = new notes_search_loader(this);
    connect(notes_search_loader_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float )),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(notes_search_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    notes_search_loader_obj->pub_set_essentials();
    notes_search_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    notes_search_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    notes_search_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);

    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");

    //    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + tab_db_name + ".sqlite";
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    notes_search_loader_obj->pub_notes_search_set_display_db_path(db_path);
    notes_search_loader_obj->pub_set_feature_name(MACRO_Plugin_Name_Notes_Search);
    notes_search_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Notes_Search);
    notes_search_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    //    QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "notes_search_csv/";
    //    notes_search_loader_obj->pub_set_csv_dir_path(csv_path);

    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Name_Notes_Search);

    notes_search_loader_obj->pub_set_display_tab_name_list(tab_list);
    notes_search_loader_obj->pub_create_ui();
    notes_search_loader_obj->pub_set_table_display_essentials();

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Notes_Parent + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(notes_search_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Notes_Parent);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Notes_Parent,tab_text);

    connect(notes_search_loader_obj,SIGNAL(signal_tablewidget_goto_record_snapshots(QString,QString,QString,QString)),this,SLOT(slot_tablewidget_goto_record_for_snapshots(QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(notes_search_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(notes_search_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),notes_search_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(notes_search_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),notes_search_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),notes_search_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(notes_search_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(notes_search_loader_obj, SIGNAL(signal_change_notes_value_in_index_k_search_db_and_display(QString, QString, QString, QString, QStringList, QString, QString, QString)), this, SLOT(slot_change_notes_value_in_index_k_search_db_and_display(QString, QString, QString, QString, QStringList, QString, QString, QString)));
    connect(notes_search_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    notes_search_loader_obj->notes_search_populate_data_in_table();
}

void MainWindow::prepare_display_for_redefined_result_full(QString tab_text)
{
    redefined_result_full_loader *redefined_result_full_loader_obj = new redefined_result_full_loader(this);
    connect(redefined_result_full_loader_obj,SIGNAL(signal_progress_count_for_statusbar_progress_bar(qint64 ,qint64 ,float )),this,SLOT(slot_progress_count_for_progress_bar(qint64 ,qint64 ,float )),Qt::AutoConnection);
    connect(this , SIGNAL(signal_goto_redefined_result(QString)) ,redefined_result_full_loader_obj,SLOT(slot_tablewidget_goto_record(QString)));
    connect(redefined_result_full_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(redefined_result_full_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(redefined_result_full_loader_obj, SIGNAL(signal_display_data_for_redefined_results(QString,QString,QString ,QString)), this, SLOT(slot_display_for_redefined_results_saved_results_and_graphs(QString , QString , QString , QString)));
    connect(redefined_result_full_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(redefined_result_full_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(redefined_result_full_loader_obj,SIGNAL(signal_show_hide_metadata(bool)),this,SLOT(slot_show_metadata(bool)),Qt::AutoConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString,QString)));

    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),redefined_result_full_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);

    connect(redefined_result_full_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));


    connect(redefined_result_full_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));

    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),redefined_result_full_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));

    connect(redefined_result_full_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_plugin(QString,QString, QString, QString,QString, QString, QString,QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString,QString,QString, QString )));
    connect(redefined_result_full_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_fs(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString,QString,QString,QString,QString,QString)));

    connect(redefined_result_full_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(redefined_result_full_loader_obj,SIGNAL(signal_save_tabs_from_redefined_result(QString , QString)),this,SLOT(slot_save_tabs_in_redefined_result_saved_case_tree(QString , QString)));
    connect(redefined_result_full_loader_obj, SIGNAL(signal_saved_graph_category_name(QString)), this, SLOT(slot_saved_graph_category_name(QString)));


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();


    redefined_result_full_loader_obj->pub_set_essentials();
    redefined_result_full_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    redefined_result_full_loader_obj->pub_set_recon_case_obj(recon_case_obj);



    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");

    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + tab_db_name + ".sqlite";

    redefined_result_full_loader_obj->pub_set_destination_db_path(db_path);
    redefined_result_full_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Redefined_Result_Parent);
    redefined_result_full_loader_obj->pub_set_plugin_name(tab_text);
    redefined_result_full_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    //    QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "redefined_result_csv/";
    //    redefined_result_loader_obj->pub_set_csv_dir_path(csv_path);

    QStringList tab_list;
    tab_list << QString(tab_text);

    redefined_result_full_loader_obj->pub_set_display_tab_name_list(tab_list);
    redefined_result_full_loader_obj->pub_create_ui();


    redefined_result_full_loader_obj->pub_set_table_display_essentials();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Redefined_Result_Parent + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(redefined_result_full_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Redefined_Result_Parent);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Redefined_Result_Parent,tab_text);
    redefined_result_full_loader_obj->redefined_result_populate_data_in_table();
    redefined_result_full_loader_obj->pub_set_graph_on_display();


}

void MainWindow::prepare_display_for_screenshots(QString tab_text)
{
    QString db_file = tab_text;
    db_file.replace(" ", "_");
    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

    screenshots_loader *screenshots_loader_obj = new screenshots_loader(this);
    connect(screenshots_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_update_case_tab_record_number(QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_screenshot(QString,QString,QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_screenshot(QString,QString,QString,QString,QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(screenshots_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(screenshots_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(screenshots_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(screenshots_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),screenshots_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(screenshots_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(screenshots_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(screenshots_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(screenshots_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),screenshots_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),screenshots_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));
    connect(screenshots_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(screenshots_loader_obj, SIGNAL(signal_add_notes_from_result_loader_for_content_search(QString,QString,QString,QString,QString,QString)), this, SLOT(slot_add_notes_added_in_database_for_content_search(QString,QString,QString,QString,QString,QString)));
    connect(this,SIGNAL(signal_goto_screenshot_record(QString)),screenshots_loader_obj,SLOT(slot_goto_screenshot_record(QString)));
    connect(screenshots_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(screenshots_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(screenshots_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));




    screenshots_loader_obj->pub_set_essentials();

    screenshots_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Screenshots);
    screenshots_loader_obj->pub_set_plugin_name(MACRO_CASE_TREE_Screenshots);
    screenshots_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    screenshots_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    screenshots_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    screenshots_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    // QString csv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Feature_CSV_In_Result_QString).toString() + "screenshots_csv/";
    // screenshots_loader_obj->pub_set_csv_dir_path(csv_path);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    screenshots_loader_obj->pub_set_screenshot_display_db_path(destination_db_path);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Screenshots);

    screenshots_loader_obj->pub_set_display_tab_name_list(tab_list);
    screenshots_loader_obj->pub_create_ui();
    screenshots_loader_obj->pub_set_table_display_essentials();
    screenshots_loader_obj->screenshots_populate_data_in_table();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple +MACRO_CASE_TREE_Screenshots + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(screenshots_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Screenshots);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Screenshots,tab_text);

}

void MainWindow::prepare_display_for_storyboard(QString tab_text)
{
    story_board *story_board_obj = new story_board(this);
    connect(story_board_obj,SIGNAL(signal_storyboard_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(story_board_obj,SIGNAL(signal_quick_look_preview_filepath_from_table_result_loader(QString)),this,SLOT(slot_quick_look_preview_item(QString)));
    connect(story_board_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(this,SIGNAL(signal_refresh_storyboard_display()),story_board_obj,SLOT(slot_refresh_storyboard_display()));
    connect(story_board_obj,SIGNAL(signal_tablewidget_goto_record_snapshots(QString,QString,QString,QString)),this,SLOT(slot_tablewidget_goto_record_for_snapshots(QString, QString,QString, QString)),Qt::QueuedConnection);

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + "story_board.sqlite";

    QString command = "select filepath from story_board where display_name = ?";
    QStringList list;
    list << QString(tab_text);
    QString path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, list, 0, destination_db_path, Q_FUNC_INFO);

    if(!path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()) && !path.trimmed().isEmpty())
        path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + path;

    story_board_obj->pub_set_story_board_file_name_and_path(tab_text, path);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();


    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Story_Board + MACRO_RECON_Splitter_1_multiple + tab_text;

    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(story_board_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Story_Board);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Story_Board,tab_text);

    list_stroy_board_obj << story_board_obj;

    story_board_obj->pub_set_initialisation();
    story_board_obj->pub_set_previous_saved_data_in_textedit(path);
    story_board_obj->pub_add_and_save_case_details_in_textedit(path);

}

void MainWindow::prepare_display_redefined_result_saved(QString tab_text, QString db_path, QString plugin_name)
{
    redefined_result_saved_loader *redefined_result_loader_saved_obj = new redefined_result_saved_loader(this);
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_add_notes_from_result_loader_for_plugin(QString,QString, QString, QString,QString, QString, QString,QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString,QString,QString, QString )));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_add_notes_from_result_loader_for_fs(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString,QString,QString,QString,QString,QString)));
    connect(redefined_result_loader_saved_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString,QString)));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),redefined_result_loader_saved_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(redefined_result_loader_saved_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));


    redefined_result_loader_saved_obj->pub_set_essentials();
    redefined_result_loader_saved_obj->pub_set_destination_db_path(db_path);
    redefined_result_loader_saved_obj->pub_set_feature_name(MACRO_CASE_TREE_Redefined_Result_Saved_Parent);
    redefined_result_loader_saved_obj->pub_set_plugin_name(plugin_name);
    redefined_result_loader_saved_obj->pub_set_clicked_item_tab_text(tab_text);
    redefined_result_loader_saved_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    redefined_result_loader_saved_obj->pub_set_recon_case_obj(recon_case_obj);

    QStringList tab_list;
    tab_list << QString(tab_text);

    redefined_result_loader_saved_obj->pub_set_display_tab_name_list(tab_list);
    redefined_result_loader_saved_obj->pub_create_ui();
    redefined_result_loader_saved_obj->pub_set_table_display_essentials();

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Redefined_Result_Saved_Parent + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(redefined_result_loader_saved_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Redefined_Result_Saved_Parent);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Redefined_Result_Saved_Parent,tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();


    redefined_result_loader_saved_obj->redefined_saved_result_populate_data_in_table();

}

void MainWindow::prepare_display_carved_password(QString tab_text)
{
    carved_passwords_loader *carved_passwords_loader_obj = new carved_passwords_loader(this);
    connect(carved_passwords_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(carved_passwords_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);

    connect(this,SIGNAL(signal_goto_carved_password_record(QString)),carved_passwords_loader_obj,SLOT(slot_goto_carved_password_record(QString)));
    connect(carved_passwords_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(carved_passwords_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(carved_passwords_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    carved_passwords_loader_obj->pub_set_essentials();
    carved_passwords_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    carved_passwords_loader_obj->pub_set_recon_case_obj(recon_case_obj);


    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");

    QString index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString relative_db_path =  recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select relative_db_path from tbl_carved_password_index where label_name = ?",QStringList(tab_db_name),0,index_db_path,Q_FUNC_INFO);
    QString db_path = recon_static_functions::get_complete_file_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() , relative_db_path ,Q_FUNC_INFO);


    carved_passwords_loader_obj->pub_set_carved_passwords_display_db_path(db_path);
    carved_passwords_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Carved_Password);
    carved_passwords_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Carved_Password);
    carved_passwords_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Carved_Passwords_TAB_Name_Passwords);

    carved_passwords_loader_obj->pub_set_display_tab_name_list(tab_list);
    carved_passwords_loader_obj->pub_create_ui();
    carved_passwords_loader_obj->pub_set_table_display_essentials();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Carved_Password + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(carved_passwords_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Carved_Password);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);


    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Carved_Password,tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    carved_passwords_loader_obj->populate_data_in_table();

}

void MainWindow::prepare_display_for_registry_viewer(QString tab_text , QString index_db_path)
{
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString command = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name  = ?";
    QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command ,QStringList(tab_text),0,index_db_path,Q_FUNC_INFO);
    QString destination_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);


    registry_interface *registry_interface_obj = new registry_interface(this);
    connect(registry_interface_obj,SIGNAL(signal_save_openwith_changes_to_bucket(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));
    connect(registry_interface_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(this,SIGNAL(signal_registry_viewer_db_path_changed(QString)),registry_interface_obj,SLOT(slot_registry_viewer_db_path_changed(QString)));
    connect(registry_interface_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple +MACRO_CASE_TREE_Registry_Viewer + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(registry_interface_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Registry_Viewer);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Registry_Viewer,tab_text);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();


    ////---do not chnage order
    registry_interface_obj->pub_set_destination_db_path(destination_db_path);
    registry_interface_obj->set_viewer_display_name(tab_text.trimmed());
    registry_interface_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    registry_interface_obj->pub_draw_registry_tree();

}

void MainWindow::prepare_display_for_saved_graphs(QString tab_text, QString db_path , QString plugin_name)
{
    saved_graphs_loader *saved_graphs_loader_obj = new saved_graphs_loader(this);
    connect(saved_graphs_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_update_case_tab_record_number(QString)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);
    connect(saved_graphs_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_plugin(QString,QString, QString, QString,QString, QString, QString,QString, QString )),this, SLOT(slot_add_notes_added_in_database_for_plugin(QString, QString, QString,QString, QString, QString,QString,QString, QString )));
    connect(saved_graphs_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_fs(QString,QString, QString, QString,QString, QString)),this, SLOT(slot_add_notes_for_file_system(QString,QString,QString,QString,QString,QString)));
    connect(saved_graphs_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString,QString)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),saved_graphs_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(saved_graphs_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(saved_graphs_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(saved_graphs_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(saved_graphs_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(this,SIGNAL(signal_goto_saved_graphs_record(QString)),saved_graphs_loader_obj,SLOT(slot_goto_saved_graphs_record(QString)));
    connect(saved_graphs_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(saved_graphs_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    saved_graphs_loader_obj->pub_set_essentials();
    saved_graphs_loader_obj->pub_set_saved_graphs_display_db_path(db_path);
    saved_graphs_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Graph_Result_Saved);
    saved_graphs_loader_obj->pub_set_plugin_name(plugin_name);
    saved_graphs_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    saved_graphs_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    saved_graphs_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    saved_graphs_loader_obj->pub_set_recon_case_obj(recon_case_obj);

    QStringList tab_list;
    tab_list << QString(tab_text);

    saved_graphs_loader_obj->pub_set_display_tab_name_list(tab_list);
    saved_graphs_loader_obj->pub_create_ui();
    saved_graphs_loader_obj->pub_set_table_display_essentials();

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Graph_Result_Saved + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(saved_graphs_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Graph_Result_Saved);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Graph_Result_Saved,tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();


    saved_graphs_loader_obj->saved_graphs_populate_data_in_table();

}

void MainWindow::prepare_display_for_disk_hex_view(QString source_count_name, bool bool_is_complete_source, bool bool_is_unallocated_disk_view, QString unalloc_db_tbl_name)
{

    hex_viewer_unallocated *disk_hex_viewer_obj = new hex_viewer_unallocated(this);
    connect(disk_hex_viewer_obj,SIGNAL(signal_remove_tag_entry_from_tag_search_db_and_display(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(disk_hex_viewer_obj,SIGNAL(signal_remove_tag_entry_from_notes_db_and_display(QString,QString,QString,QString,QString)),this,SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));

    disk_hex_viewer_obj->pub_set_is_complete_source(bool_is_complete_source);
    disk_hex_viewer_obj->pub_set_source_count_name(source_count_name);
    disk_hex_viewer_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);

    disk_hex_viewer_obj->pub_set_is_unalloctaed_disk_view(bool_is_unallocated_disk_view);
    disk_hex_viewer_obj->pub_set_unallocated_db_table_name(unalloc_db_tbl_name);

    disk_hex_viewer_obj->pub_set_essentials();


    QString tab_text = disk_hex_viewer_obj->pub_get_hex_device_node();

    if(tab_text.startsWith("/dev/r"))
        tab_text.remove(0, QString("/dev/r").size());

    if(bool_is_unallocated_disk_view)
        tab_text.append(" - Unallocated");
    else
        tab_text.append(" - Complete");

    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Disk_Hex_Viewer + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(disk_hex_viewer_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Disk_Hex_Viewer);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Disk_Hex_Viewer,tab_text);

    disk_hex_viewer_obj->pub_show_disk_in_disk_viewer();

}

void MainWindow::prepare_display_for_saved_timeline_graphs(QString tab_text)
{
    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";

    screenshots_loader *saved_timeline_graph_loader_obj = new screenshots_loader(this);

    connect(saved_timeline_graph_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_update_case_tab_record_number(QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_screenshot(QString,QString,QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_screenshot(QString,QString,QString,QString,QString,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);

    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),saved_timeline_graph_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);

    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),saved_timeline_graph_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));

    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),saved_timeline_graph_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));

    connect(saved_timeline_graph_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(saved_timeline_graph_loader_obj, SIGNAL(signal_add_notes_from_result_loader_for_content_search(QString,QString,QString,QString,QString,QString)), this, SLOT(slot_add_notes_added_in_database_for_content_search(QString,QString,QString,QString,QString,QString)));

    connect(this,SIGNAL(signal_goto_screenshot_record(QString)),saved_timeline_graph_loader_obj,SLOT(slot_goto_screenshot_record(QString)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(saved_timeline_graph_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));



    saved_timeline_graph_loader_obj->pub_set_essentials();
    saved_timeline_graph_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    saved_timeline_graph_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);

    saved_timeline_graph_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Artifacts_Timeline_Graph);
    saved_timeline_graph_loader_obj->pub_set_plugin_name(MACRO_CASE_TREE_Artifacts_Timeline_Graph);
    saved_timeline_graph_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    saved_timeline_graph_loader_obj->pub_set_screenshot_display_db_path(destination_db_path);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph);

    saved_timeline_graph_loader_obj->pub_set_display_tab_name_list(tab_list);
    saved_timeline_graph_loader_obj->pub_create_ui();
    saved_timeline_graph_loader_obj->pub_set_table_display_essentials();
    saved_timeline_graph_loader_obj->screenshots_populate_data_in_table();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Artifacts_Timeline_Graph + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(saved_timeline_graph_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Artifacts_Timeline_Graph);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Timeline_Graph,tab_text);

}

void MainWindow::prepare_display_for_saved_maps(QString tab_text)
{
    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";

    screenshots_loader *saved_maps_loader_obj = new screenshots_loader(this);

    connect(saved_maps_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_update_case_tab_record_number(QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(saved_maps_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_screenshot(QString,QString,QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_screenshot(QString,QString,QString,QString,QString,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);

    connect(saved_maps_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(saved_maps_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),saved_maps_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(saved_maps_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(saved_maps_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);

    connect(saved_maps_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(saved_maps_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),saved_maps_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));

    connect(saved_maps_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),saved_maps_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));

    connect(saved_maps_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(saved_maps_loader_obj, SIGNAL(signal_add_notes_from_result_loader_for_content_search(QString,QString,QString,QString,QString,QString)), this, SLOT(slot_add_notes_added_in_database_for_content_search(QString,QString,QString,QString,QString,QString)));

    connect(this,SIGNAL(signal_goto_screenshot_record(QString)),saved_maps_loader_obj,SLOT(slot_goto_screenshot_record(QString)));
    connect(saved_maps_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(saved_maps_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(saved_maps_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(saved_maps_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    saved_maps_loader_obj->pub_set_essentials();
    saved_maps_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    saved_maps_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);

    saved_maps_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Saved_Maps);
    saved_maps_loader_obj->pub_set_plugin_name(MACRO_CASE_TREE_Saved_Maps);
    saved_maps_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    saved_maps_loader_obj->pub_set_creation_datetime_conversion(false);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    saved_maps_loader_obj->pub_set_screenshot_display_db_path(destination_db_path);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Saved_Maps);

    saved_maps_loader_obj->pub_set_display_tab_name_list(tab_list);
    saved_maps_loader_obj->pub_create_ui();
    saved_maps_loader_obj->pub_set_table_display_essentials();
    saved_maps_loader_obj->screenshots_populate_data_in_table();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Saved_Maps + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(saved_maps_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Saved_Maps);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Saved_Maps,tab_text);

}

void MainWindow::set_up_global_connection_manager()
{
    global_connection_manager_obj = new global_connection_manager(this);


    connect(global_connection_manager_obj, SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)), this, SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(global_connection_manager_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));
    connect(global_connection_manager_obj, SIGNAL(signal_common_add_notes_name_into_case_tree(QString)), this, SLOT(slot_common_add_notes_name_into_case_tree(QString)));
    connect(global_connection_manager_obj, SIGNAL(signal_go_to_source_for_search_file_with_same_hash(QString, QString, QString, QString, QString, QString)), this, SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
}

void MainWindow::set_up_global_recon_file_info()
{
    global_recon_file_info_obj = new global_recon_file_info(this);
}

void MainWindow::stop_rcase_all_threads()
{
    recon_case_obj->pub_stop_all_running_threads();
}

void MainWindow::prepare_display_for_email_files(QString tab_text)
{
    if(tab_text == QString(MACRO_CASE_TREE_Email_Files_Category_Email_Parser))
    {
        emlx_mail *emlx_mail_obj = new emlx_mail(this);
        connect(emlx_mail_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_common_add_tag_name_into_case_tree(QString)));
        connect(emlx_mail_obj, SIGNAL(signal_quick_look_preview_filepath_from_email_files(QString)), this, SLOT(slot_quick_look_preview_item_email_files(QString)));
        connect(this , SIGNAL(signal_goto_email_files_record(QString)) ,emlx_mail_obj,SLOT(slot_tablewidget_goto_record(QString)));
        connect(emlx_mail_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
        connect(emlx_mail_obj,SIGNAL(signal_go_to_source_in_file_system_from_emlx(QString, QString, QString)),this,SLOT(slot_go_to_artifact_source(QString, QString, QString)));


        QCoreApplication::processEvents();
        QCoreApplication::processEvents();

        emlx_mail_obj->pub_set_global_connection_manager_obj(global_connection_manager_obj);
        emlx_mail_obj->pub_set_essentials();
        emlx_mail_obj->pub_display_data_in_table();





        ///-Stackwidget Work..
        QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Email_Files_Parent + MACRO_RECON_Splitter_1_multiple + tab_text;
        Container_QWidget *container = new Container_QWidget(this);
        QVBoxLayout *vbox_layout = new QVBoxLayout;
        vbox_layout->addWidget(emlx_mail_obj);
        vbox_layout->setContentsMargins(0,0,0,0);
        container->setLayout(vbox_layout);
        container->pub_set_feature_widget_name(tab_text);
        container->pub_set_category_name(MACRO_CASE_TREE_Email_Files_Parent);
        container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
        container->pub_set_widget_unique_name(feature_unique_name);
        center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

        insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Email_Files_Parent,tab_text);

    }

}


void MainWindow::prepare_display_for_carved_files(QString tab_text)
{
    tab_text = tab_text.trimmed();
    if(tab_text.isEmpty())
        return;

    QString carver_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
    QString tmp_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select carver_files_db_path from table_carver_index Where label_name = ?"), QStringList(tab_text), 0, carver_index_db_path, Q_FUNC_INFO);

    tmp_db_path = tmp_db_path.trimmed();
    if(tmp_db_path.isEmpty())
        return;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    QString destination_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,tmp_db_path , Q_FUNC_INFO);

    destination_db_path = destination_db_path.trimmed();
    if(destination_db_path.isEmpty())
        return;


    carved_files_loader *carved_files_loader_obj = new carved_files_loader(this);

    connect(carved_files_loader_obj, SIGNAL(signal_common_add_tag_name_into_case_tree(QString)), this, SLOT(slot_update_case_tab_record_number(QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(carved_files_loader_obj,SIGNAL(signal_change_bookmark_value_in_other_db(QString,QString,QString,QString,QStringList, QString, QString, QString)),this,SLOT(slot_update_bookmark_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString, QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_add_notes_from_result_loader_for_screenshot(QString,QString,QString, QString, QString, QString)),this, SLOT(slot_add_notes_added_in_database_for_screenshot(QString,QString,QString,QString,QString,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_change_notes_in_other_db_also(QString,QString,QString,QString,QStringList, QString, QString)),this,SLOT(slot_update_notes_value_of_other_display_and_db(QString,QString,QString,QString,QStringList, QString, QString)),Qt::QueuedConnection);

    connect(carved_files_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(carved_files_loader_obj,SIGNAL(signal_add_tag_in_other_locations(QString,QString,QString,QString,QString,QString, QString)),this,SLOT(slot_add_tag_in_other_db(QString,QString,QString,QString,QString,QString, QString)));
    connect(this,SIGNAL(signal_update_tags_search_display_for_singal_record(QString, QString, QString, QString, QString)),carved_files_loader_obj,SLOT(slot_insert_record_in_tablewidget(QString, QString, QString, QString, QString)),Qt::QueuedConnection);
    connect(carved_files_loader_obj,SIGNAL(signal_show_file_in_viewer_table_result(QString,QString,QString)),this,SLOT(slot_show_file_in_viewer(QString,QString,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_viewer_extarction_completed_table_result(QString,QString)),this,SLOT(slot_viewer_extraction_finished_or_cancelled(QString, QString)),Qt::QueuedConnection);
    connect(carved_files_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);

    connect(carved_files_loader_obj,SIGNAL(signal_change_content_of_detach_window(QString,QString,QString,int,QString,QString)),this,SLOT(slot_change_all_detach_window_content(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(carved_files_loader_obj,SIGNAL(signal_update_tags_to_other_db(QString,QString,QString,QStringList,QString)),this,SLOT(slot_update_other_display_and_db_for_remained_tags(QString,QString,QString,QStringList,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_hashset_create_db_finish(QStringList)),this,SLOT(slot_hashset_finished_or_cancelled(QStringList)));
    connect(this,SIGNAL(signal_update_tags_submenu_of_tablewidget_loader(bool,QString,QString)),carved_files_loader_obj,SLOT(slot_update_tags_in_actions_menu_without_signal_emit(bool,QString,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_update_tag_submenu_of_plugins(bool,QString,QString)),this,SLOT(slot_update_tag_submenu_of_plugin_and_file_system(bool,QString,QString)));

    connect(carved_files_loader_obj,SIGNAL(signal_table_widget_loader_removed_tags_list(QString,QString,QString,QStringList,QString)),this,SLOT(slot_remove_tag_entry_in_other_db(QString,QString,QString,QStringList,QString)));
    connect(this,SIGNAL(signal_update_hashset_submenu_of_tablewidget_loader(QString,QString)),carved_files_loader_obj,SLOT(slot_update_hashset_submenu(QString,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_update_hashset_submenu_from_tablewidget_loader(QString,QString)),this,SLOT(slot_update_hashset_submenu_from_tablewidget_loader(QString,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_go_to_artifact_source_in_file_system_from_loader(QString,QString, QString)),this,SLOT(slot_go_to_artifact_source(QString,QString, QString)));

    connect(carved_files_loader_obj, SIGNAL(signal_update_display_on_remove_notes(QString,QString,QString,QString,QString)), this, SLOT(slot_remove_tag_entry_in_notes_search_db_and_display(QString,QString,QString,QString,QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_statusbar_progress_bar_show_hide(bool, QString)),this,SLOT(slot_statusbar_progressbar_show_hide(bool, QString)),Qt::AutoConnection);
    connect(carved_files_loader_obj, SIGNAL(signal_add_notes_from_result_loader_for_content_search(QString,QString,QString,QString,QString,QString)), this, SLOT(slot_add_notes_added_in_database_for_content_search(QString,QString,QString,QString,QString,QString)));

    connect(this,SIGNAL(signal_goto_carved_files_record(QString)),carved_files_loader_obj,SLOT(slot_goto_screenshot_record(QString)));
    connect(carved_files_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(carved_files_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(carved_files_loader_obj,SIGNAL(signal_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)),this,SLOT(slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data)));
    connect(carved_files_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));


    carved_files_loader_obj->pub_set_essentials();
    carved_files_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    carved_files_loader_obj->pub_set_recon_case_obj(recon_case_obj);
    carved_files_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);

    carved_files_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Carved_Files);
    carved_files_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Carved_Files);
    carved_files_loader_obj->pub_set_clicked_item_tab_text(tab_text);

    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    carved_files_loader_obj->pub_set_carved_files_display_db_path(destination_db_path);

    QStringList tab_list;
    tab_list << QString(MACRO_CASE_TREE_Carved_Files);

    carved_files_loader_obj->pub_set_display_tab_name_list(tab_list);
    carved_files_loader_obj->pub_create_ui();
    carved_files_loader_obj->pub_create_custom_ui();
    carved_files_loader_obj->pub_set_table_display_essentials();
    carved_files_loader_obj->populate_data_in_table();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Carved_Files + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(carved_files_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Carved_Files);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Carved_Files,tab_text);

}

void MainWindow::prepare_display_for_carved_data(QString tab_text)
{
    tab_text = tab_text.trimmed();
    if(tab_text.isEmpty())
        return;

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    destination_db_path = destination_db_path.trimmed();
    if(destination_db_path.isEmpty())
        return;


    carved_data_loader *carved_data_loader_obj = new carved_data_loader(this);
    connect(carved_data_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(carved_data_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(signal_goto_carved_data(QString)),carved_data_loader_obj,SLOT(slot_goto_screenshot_record(QString)));
    connect(carved_data_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString, QString,QString, QString,QString, QString)),Qt::QueuedConnection);
    connect(carved_data_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(carved_data_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    carved_data_loader_obj->pub_set_essentials();
    carved_data_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    carved_data_loader_obj->pub_set_recon_case_obj(recon_case_obj);
    carved_data_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_Carved_Data);
    carved_data_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Carved_Data);
    carved_data_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    carved_data_loader_obj->pub_set_carved_data_display_db_path(destination_db_path);
    QStringList tab_list;
    tab_list << QString(MACRO_Plugin_Carved_Data_TAB_Name_Carved_Data);
    carved_data_loader_obj->pub_set_display_tab_name_list(tab_list);
    carved_data_loader_obj->pub_create_ui();
    carved_data_loader_obj->pub_set_table_display_essentials();
    carved_data_loader_obj->populate_data_in_table();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Carved_Data + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(carved_data_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Carved_Data);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Carved_Data,tab_text);

}

void MainWindow::prepare_display_snapshots(QString tab_text)
{
    QString tab_db_name = tab_text;
    tab_db_name.replace(" ","_");



    QString dest_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + tab_text +".sqlite";
    snapshots_loader *snapshots_loader_obj = new snapshots_loader(this);

    connect(snapshots_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(snapshots_loader_obj,SIGNAL(signal_act_quick_look_triggered()),this,SLOT(slot_action_quick_look_triggered()));
    connect(this,SIGNAL(signal_goto_record_snapshots(QString,QString)),snapshots_loader_obj,SLOT(slot_goto_snapshots_record(QString, QString)),Qt::QueuedConnection);
    connect(snapshots_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));
    connect(snapshots_loader_obj,SIGNAL(signal_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),this,SLOT(slot_tablewidget_goto_record(QString,QString,QString,QString,QString, QString)),Qt::QueuedConnection);
    connect(snapshots_loader_obj,SIGNAL(signal_save_openwith_changes_to_bucket_for_full_detail_info(QString)),this,SLOT(slot_save_openwith_changes_to_bucket(QString)));

    snapshots_loader_obj->pub_set_feature_name(MACRO_Plugin_Name_Snapshots);
    snapshots_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_Snapshots);
    snapshots_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    snapshots_loader_obj->pub_set_essentials();
    snapshots_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    snapshots_loader_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);
    snapshots_loader_obj->pub_set_recon_case_obj(recon_case_obj);



    QStringList tab_list;
    tab_list << global_csv_reader_class_selected_obj->get_total_tab_list(MACRO_Plugin_Name_Snapshots);
    snapshots_loader_obj->pub_set_display_tab_name_list(tab_list);
    snapshots_loader_obj->pub_create_ui();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Snapshots + MACRO_RECON_Splitter_1_multiple +tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(snapshots_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_Snapshots);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);


    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Snapshots,tab_text);


    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    snapshots_loader_obj->pub_set_snapshots_difference_db_path(dest_db);
    snapshots_loader_obj->pub_set_selected_snapshot_name();
    snapshots_loader_obj->populate_data_in_table();

}

void MainWindow::prepare_display_for_ram_images(QString tab_text)
{
    tab_text = tab_text.trimmed();
    if(tab_text.isEmpty())
        return;

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString).toString() + "ram_images.sqlite";


    ram_images_loader *ram_images_loader_obj = new ram_images_loader(this);
    connect(ram_images_loader_obj,SIGNAL(signal_set_metadata_info(struct_global_set_metadata_info)),this,SLOT(slot_set_metadata_info(struct_global_set_metadata_info)));
    connect(ram_images_loader_obj,SIGNAL(signal_open_in_detach_clicked_table_result(QString,QString,QString,int,QString,QString)),this,SLOT(slot_open_window_for_detach_clicked(QString,QString,QString,int,QString,QString)),Qt::QueuedConnection);
    connect(ram_images_loader_obj, SIGNAL(signal_carve_data_clicked(QString,QString,QString,QString,QString)), this, SLOT(slot_carve_data_clicked(QString,QString,QString,QString,QString)));
    connect(ram_images_loader_obj, SIGNAL(signal_carve_files_clicked(QString,QString, bool)), this, SLOT(slot_carve_files_clicked(QString,QString, bool)));
    connect(ram_images_loader_obj,SIGNAL(signal_clear_all_metadata()),this,SLOT(slot_clear_all_metadata()));


    ram_images_loader_obj->pub_set_essentials();
    ram_images_loader_obj->pub_set_global_manager_obj(global_connection_manager_obj);
    ram_images_loader_obj->pub_set_recon_case_obj(recon_case_obj);
    ram_images_loader_obj->pub_set_feature_name(MACRO_CASE_TREE_RAM_Images);
    ram_images_loader_obj->pub_set_plugin_name(MACRO_Plugin_Name_RAM_Images);
    ram_images_loader_obj->pub_set_clicked_item_tab_text(tab_text);
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    ram_images_loader_obj->pub_set_ram_images_display_db_path(destination_db_path);
    ram_images_loader_obj->pub_set_display_tab_name_list(QStringList(MACRO_Plugin_RAM_Images_TAB_Name_Images));
    ram_images_loader_obj->pub_create_ui();
    ram_images_loader_obj->pub_set_table_display_essentials();
    ram_images_loader_obj->populate_data_in_table();


    ///-Stackwidget Work..
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_RAM_Images + MACRO_RECON_Splitter_1_multiple + tab_text;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(ram_images_loader_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(tab_text);
    container->pub_set_category_name(MACRO_CASE_TREE_RAM_Images);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);

    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_RAM_Images,tab_text);

}
