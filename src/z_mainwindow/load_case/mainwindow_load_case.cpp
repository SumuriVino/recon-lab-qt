#include "z_mainwindow/mainwindow.h"

void MainWindow::slot_loadcase_launched(QString out_path)
{
    recon_static_functions::app_debug(" start",Q_FUNC_INFO);

    bool_loading_result_operation_running = true;

    if(!out_path.trimmed().endsWith("/"))
        out_path.append("/");

    disk_manager_obj->pub_stop_disk_arbitration();

    global_narad_muni_class_obj->set_field(MACRO_NARAD_RECON_Result_Dir_QString,out_path);
    recon_static_functions::set_result_path(out_path);
    create_and_retrieve_recon_result_dir_structure(out_path);

    QString rst_vrsn = recon_helper_standard_obj->pub_get_result_version();

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Result_App_Version_QString,rst_vrsn);

    bool_need_to_update_result_for_load_case = need_to_update_result_for_load_case();

    update_result_necessity_for_case_info();

    update_result_necessity_for_configuration();
    case_info_details_obj->pub_extract_full_caseinfo();

    update_result_necessity_for_sources();

    insert_recent_load_case_result_info(out_path);

    file_indexing_display_obj->pub_set_essentials();
    file_indexing_display_obj->pub_display_selected_files();

    recognize_face_obj->pub_set_task_status_window(task_status_window_obj);

    // Pick the previously indexed files by giving the path to ixlib.ilb for DT Search
    QString path_to_xilib = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + MACRO_Index_Files_Dir_Name_QString;

    DString homeDir, privateDir;
    privateDir = path_to_xilib.toLocal8Bit().data();

    dt_search_interface_obj = new dt_search_interface(cin, cout, homeDir, privateDir);
    dt_search_interface_obj->init();

    global_witness_info_manager_class_obj->pub_fill_witness_info_QList_from_store();

    prepare_root_sources_path_for_load_case();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    //========== updte data start ====
    //global_target_sources_info_class_obj is used in it
    update_result_necessity_for_data();
    //========== update data end ====

    QString plugin_db_path;
    plugin_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    QString command = "select distinct plugin_name from tbl_plugin_status";
    QStringList plugin_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,db_path,Q_FUNC_INFO);

    QStringList plugin_withtab = recon_helper_standard_obj->get_total_extracted_plugin_with_extracted_tab_list(Q_FUNC_INFO);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Selected_Plugins_With_Tab_After_Extraction_QStringList,plugin_withtab);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Selected_Plugins_With_Tab_For_Extraction_QStringList,plugin_withtab);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Selected_Plugins_QStringList,plugin_list);

    //====================Print Info Examiner Start=================//
    global_recon_helper_singular_class_obj->pub_fill_destination_drive_info(Q_FUNC_INFO);
    struct_global_destination_drive_info destination_drive_info_obj = global_recon_helper_singular_class_obj->get_saved_destination_drive_info(Q_FUNC_INFO);
    recon_static_functions::print_destination_drive_info(destination_drive_info_obj, Q_FUNC_INFO);

    //====================Print Info Examiner End=================//


    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QList<struct_GLOBAL_witness_info_root> list_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

    //====================Print Info Root-Source Start=================//
    QStringList root_count_name_list_to_print;
    for(int jj = 0; jj < list_root_info.size(); jj++)
    {
        root_count_name_list_to_print << list_root_info.at(jj).root_count_name;
    }


    QStringList src_cnt_name_list_to_print;
    for(int ii = 0; ii < list_target_source_info.size(); ii++)
    {
        src_cnt_name_list_to_print << list_target_source_info.at(ii).source_count_name;
    }

    recon_static_functions::print_witness_root_info(list_root_info, root_count_name_list_to_print);
    recon_static_functions::print_witness_source_info(list_target_source_info, src_cnt_name_list_to_print);

    //====================Print Info Root-Source End=================//


    recon_case_obj = new RECON_case(this);
    connect(recon_case_obj, SIGNAL(signal_filesystem_extraction_finished()), this, SLOT(slot_filesystem_extraction_finish_on_load_case()));
    connect(recon_case_obj, SIGNAL(signal_email_files_case_tree_add_finished()), this, SLOT(slot_email_files_case_tree_add_finish_on_load_case()));
    connect(recon_case_obj, SIGNAL(signal_google_mail_files_case_tree_add_finished()), this, SLOT(slot_google_mail_files_case_tree_add_finish_on_load_case()));
    bool_fs_extraction_finish_on_load_case = false;
    bool_email_files_case_tree_add_finish_on_load_case = false;
    bool_google_mail_files_case_tree_add_finish_on_load_case = false;
    connect(recon_configuration_after_launch_obj, SIGNAL(signal_recon_config_apply_button_clicked()), recon_case_obj, SLOT(slot_recon_config_apply_button_clicked()));

    recon_case_obj->set_progress_bar_object(progress_bar_fixed_in_statusbar_obj);
    recon_case_obj->set_essentials();
    recon_case_obj->pub_set_task_status_window_object(task_status_window_obj);
    recon_case_obj->pub_set_dt_search_obj(dt_search_interface_obj);
    recon_case_obj->set_ios_parser_map(map_ios_uid_backup_parser_obj);

    facefinder_obj->pub_set_task_status_window(task_status_window_obj);

    recon_case_obj->pub_set_recognize_face_obj(recognize_face_obj);
    recon_case_obj->pub_set_face_finder_obj(facefinder_obj);
    recon_case_obj->pub_set_recon_case_runner_type(MACRO_RECON_CASE_RUNNER_TYPE_LOAD_CASE);
    struct_global_file_system_post_launcher_job_info st_obj;
    recon_case_obj->pub_set_file_system_selected_job_source_count_name(st_obj);
    recon_case_obj->pub_set_global_recon_file_info_obj(global_recon_file_info_obj);

    r_case_connections();

    case_tree_display();


    recon_case_obj->set_case_treewidget_obj(treewidget_case_display_obj);

    QStringList source_count_name_list;

    for(int i = 0 ; i < list_target_source_info.size(); i++)
    {
        source_count_name_list << list_target_source_info.at(i).source_count_name;
    }

    QString sources_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "/sources_info.sqlite";
    QString select_mime_cmd = "SELECT Mime_Type from fs_status WHERE source_count_name = ?";

    for (int kk = 0 ; kk < source_count_name_list.size(); kk++)
    {
        bool bool_source_applicable = recon_helper_standard_obj->source_applicable_for_extensive_module(source_count_name_list.at(kk));
        if(!bool_source_applicable)
        {
            continue;
        }

        QString mime_status = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_mime_cmd, QStringList(source_count_name_list.at(kk)),0,sources_info_db_path,Q_FUNC_INFO);
        if(mime_status == "0")
        {
            QMessageBox msgBox(QMessageBox::Information,"title","RECON LAB needs to prepare File Type Database. Please click OK to continue.",
                               QMessageBox::Ok, this,Qt::FramelessWindowHint);
            msgBox.exec();
            break;
        }
    }

    recon_case_obj->pub_set_FS_source_selected_source_count_name_list(source_count_name_list);

    recon_case_obj->file_system_extraction_initiator();

    QString face_search_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString()
            + QString("/index_face_search.sqlite");

    QString cmd = "SELECT search_label_name from face_search_index";

    QStringList face_search_label_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(cmd, 0, face_search_db_path, Q_FUNC_INFO);

    for(int ii = 0; ii < face_search_label_list.size(); ii++)
    {
        QCoreApplication::processEvents();
        QString path = face_search_label_list.at(ii);

        recon_case_obj->pub_add_matched_faces_to_case_tree_for_load_case(path);

    }

    for(int i = 0; i < plugin_list.size(); i++)
    {
        common_current_plugin_extraction_finished_or_cancelled(plugin_list.at(i));
    }

    QStringList keys_list =  recon_helper_standard_obj->get_keyword_search_namelist_from_result_db(Q_FUNC_INFO);
    for(int i = 0; i < keys_list.size(); i++)
    {
        common_right_click_keyword_search_finished_or_cancelled(keys_list.at(i));
    }

    QStringList file_search_name_list =  get_file_search_namelist_from_result_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString(), Q_FUNC_INFO);
    for(int i = 0; i < file_search_name_list.size(); i++)
    {
        common_file_search_finished_or_cancelled(file_search_name_list.at(i));
    }

    QStringList exif_metadata_search_name_list =  get_exif_metadata_search_namelist_from_result_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString(), Q_FUNC_INFO);
    for(int i = 0; i < exif_metadata_search_name_list.size(); i++)
    {
        common_exif_metadata_search_finished_or_cancelled(exif_metadata_search_name_list.at(i));
    }

    QStringList cntnt_search_name_list =  get_content_search_namelist_from_result_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString(), Q_FUNC_INFO);
    for(int i = 0; i < cntnt_search_name_list.size(); i++)
    {
        common_content_search_finished_or_cancelled(cntnt_search_name_list.at(i));
    }


    QString saved_artifacts_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + "index_artifacts_timeline.sqlite";
    QStringList saved_db_list = get_artifacts_timeline_saved_name_from_result_db(saved_artifacts_path,Q_FUNC_INFO);
    for(int i = 0; i < saved_db_list.size(); i++)
    {
        common_add_artifacts_timeline_name_into_case_tree(saved_db_list.at(i));
    }

    QString snapshots_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + "saved_snapshots_index.sqlite";
    QStringList snapshots_display_tabs_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("select snapshot_lable_name from tbl_saved_index"),0,snapshots_index_db_path,Q_FUNC_INFO);
    for(int i = 0; i < snapshots_display_tabs_list.size(); i++)
    {
        common_add_snapshots_diff_into_case_tree(snapshots_display_tabs_list.at(i));
    }

    QStringList plist_viewer_list =  recon_helper_standard_obj->get_plist_displayname_list_from_result_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite", Q_FUNC_INFO);
    for(int i = 0; i < plist_viewer_list.size(); i++)
    {
        common_viewer_extraction_finish_or_cancelled(plist_viewer_list.at(i),MACRO_CASE_TREE_Plist_Viewer);
    }


    QStringList sqlite_viewer_list =  recon_helper_standard_obj->get_sqlite_namelist_from_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite", Q_FUNC_INFO);
    for(int i = 0; i < sqlite_viewer_list.size(); i++)
    {
        common_viewer_extraction_finish_or_cancelled(sqlite_viewer_list.at(i),MACRO_CASE_TREE_Sqlite_Viewer);
    }

    QStringList hex_viewer_list =  recon_helper_standard_obj->get_hex_namelist_from_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite", Q_FUNC_INFO);
    for(int i = 0; i < hex_viewer_list.size(); i++)
    {
        common_viewer_extraction_finish_or_cancelled(hex_viewer_list.at(i),MACRO_CASE_TREE_Hex_Viewer);
    }

    QStringList log_viewer_list =  recon_helper_standard_obj->get_log_displayname_list_from_result_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite", Q_FUNC_INFO);
    for(int i = 0; i < log_viewer_list.size(); i++)
    {
        common_viewer_extraction_finish_or_cancelled(log_viewer_list.at(i),MACRO_CASE_TREE_Log_Viewer);
    }

    display_already_created_tags_name(out_path);

    display_already_created_bookmarks(out_path);

    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    int notes_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, notes_db_path, QStringList(), Q_FUNC_INFO);
    if(notes_count > 0)
    {
        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
    }

    QString screenshots_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
    int screenshots_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, screenshots_db_path, QStringList(), Q_FUNC_INFO);
    if(screenshots_count > 0)
    {
        common_add_screenshots_into_case_tree(MACRO_Plugin_Name_Screenshots);
    }

    QStringList redefined_result_list =  get_redefined_result_namelist_from_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "index_redefined_result.sqlite", Q_FUNC_INFO);
    for(int i = 0; i < redefined_result_list.size(); i++)
    {
        common_add_redefined_result_category_name_in_case_tree(redefined_result_list.at(i));
    }

    QString story_board_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + "story_board.sqlite";
    QStringList storyboard_list = get_story_board_namelist_from_db(story_board_db_path,Q_FUNC_INFO);
    for(int i = 0; i < storyboard_list.size(); i++)
    {
        common_add_story_board_into_case_tree(storyboard_list.at(i));
    }

    QStringList redefined_result_saved_list =  get_redefined_result_namelist_from_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString() + "index_redefined_result.sqlite", Q_FUNC_INFO);

    for(int i = 0; i < redefined_result_saved_list.size(); i++)
    {
        if(redefined_result_saved_list.at(i) == MACRO_Redefined_Result_Messenger)
        {
            QString saved_msg_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + "index_messenger_saved.sqlite";
            QStringList saved_db_list_msg = get_redefined_result_saved_name_from_result_db(saved_msg_path,Q_FUNC_INFO);

            for(int tab_no = 0; tab_no < saved_db_list_msg.size(); tab_no++)
            {
                common_add_redefined_saved_result_name_into_case_tree(saved_db_list_msg.at(tab_no),redefined_result_saved_list.at(i));
            }

        }

        if(redefined_result_saved_list.at(i) == MACRO_Redefined_Result_Browser_History)
        {
            QString saved_bros_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + "index_browsers_history_saved.sqlite";
            QStringList saved_db_list_bros = get_redefined_result_saved_name_from_result_db(saved_bros_path,Q_FUNC_INFO);

            for(int tab_no = 0; tab_no < saved_db_list_bros.size(); tab_no++)
            {
                common_add_redefined_saved_result_name_into_case_tree(saved_db_list_bros.at(tab_no),redefined_result_saved_list.at(i));
            }
        }
    }

    QString ram_analysis_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";
    int ram_analysis_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, ram_analysis_db_path, QStringList(), Q_FUNC_INFO);
    if(ram_analysis_count > 0)
    {
        common_add_ram_analysis_into_case_tree(QString(MACRO_CASE_TREE_Ram_Analysis_Saved));
    }

    QString carved_password_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString() + "carved_password_index.sqlite";
    QString command_1 = "select label_name from tbl_carved_password_index";
    QStringList carved_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command_1,0,carved_password_path,Q_FUNC_INFO);
    for(int i = 0; i < carved_list.size(); i++)
    {
        common_add_carved_password_into_case_tree(carved_list.at(i));
    }

    QStringList saved_graph_result_list =  get_saved_graph_result_namelist_from_db(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Index_In_Result_QString).toString() + "saved_graph_result.sqlite", Q_FUNC_INFO);
    for(int i = 0; i < saved_graph_result_list.size(); i++)
    {
        common_add_saved_graph_category_name_in_case_tree(saved_graph_result_list.at(i));
    }

    QString registry_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    QStringList tabs_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("select display_tab_name from tbl_registry_viewer_index"),0,registry_index_db_path,Q_FUNC_INFO);
    for(int i = 0; i < tabs_list.size(); i++)
    {
        common_viewer_extraction_finish_or_cancelled(tabs_list.at(i),MACRO_CASE_TREE_Registry_Viewer);
    }

    slot_add_saved_timeline_graph_in_case_tree();
    slot_add_saved_maps_in_case_tree();
    slot_add_carver_in_case_tree_on_finish();
    add_carved_data_in_case_tree_on_finish();

    //    progress_bar_fixed_in_statusbar_obj->set_progressbar_value(100);
    //    progress_bar_fixed_in_statusbar_obj->hide();
    //    progress_bar_fixed_in_statusbar_obj->set_move_progressbar_in_left_corner(false);

    this->activateWindow();

    recon_configuration_after_launch_obj->pub_set_essentials();
    recon_configuration_after_launch_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER);
    recon_configuration_after_launch_obj->pub_initialise_configuration();


    text_viewer_aascii_unicode_obj->pub_set_essentials();



    recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int count = 0;
    while(1)
    {
        count++;
        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }
        if(bool_fs_extraction_finish_on_load_case)
        {
            display_loading_progress_bar_non_cancelable_obj->hide();
            break;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);



    // do not change position of this boolean, it mean full tree is loaded before here
    bool_launcher_work_completed = true;
    // this line must be used at last
    bool_loading_result_operation_running = false;

    recon_static_functions::app_debug(" end",Q_FUNC_INFO);
}

void MainWindow::prepare_root_sources_path_for_load_case()
{
    QList<struct_GLOBAL_witness_info_root> list_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();
    QList<struct_GLOBAL_witness_info_source> list_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QStringList list_info_for_load_case_displayer;

    QStringList list_root_path;

    bool bool_all_root_exists = true;
    bool bool_any_root_type_image = false;


    for(int jj = 0; jj < list_root_info.size(); jj++)
    {
        struct_GLOBAL_witness_info_root st_root_info = list_root_info.at(jj);

        QString src_type_intrnl    = st_root_info.source_type_internal;

        // because we are saving AB,TAR and UFDR output in result already
        // because we are saving Android AB,Cellebrite TAR,Cellebrite UFDR and GrayKey zip output in result already

        // we have to point directly in result
        if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
        {
            continue;
        }

        QString root_count_name = st_root_info.root_count_name;
        QString rt_type    = st_root_info.root_type;
        QString root_path1 = st_root_info.root_path_1;
        QString root_path_2_for_fusion = st_root_info.root_path_2_for_fusion;
        QString evidence_type = st_root_info.evidence_type;


        if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
        {
            bool_any_root_type_image = true;
        }


        if(bool_all_root_exists)
        {
            if(!QFileInfo(root_path1).exists())
                bool_all_root_exists = false;

            if(rt_type == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RootType_FusionImage)
            {
                if(!QFileInfo(root_path_2_for_fusion).exists())
                    bool_all_root_exists = false;

                root_path1.append(MACRO_RECON_Splitter_6_comma).append(root_path_2_for_fusion);
            }
        }

        QString m_value = (root_count_name + MACRO_RECON_Splitter_1_multiple + root_path1 + MACRO_RECON_Splitter_1_multiple
                           + evidence_type + MACRO_RECON_Splitter_1_multiple + rt_type + MACRO_RECON_Splitter_1_multiple + src_type_intrnl);


        list_info_for_load_case_displayer << m_value;
    }


    if(bool_all_root_exists)
    {
        if(bool_any_root_type_image)
        {
            message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO,"Want to mount/locate sources which are not accessible?");
            message_dialog_object->pub_set_window_frameless(true);
            message_dialog_object->show();

            recon_static_functions::debug_conditional(" : running 1", Q_FUNC_INFO);
            bool_message_box_clicked = false;
            while(1)
            {
                QCoreApplication::processEvents();
                if(bool_message_box_clicked)
                    break;
            }

            message_dialog_object->pub_set_window_frameless(false);
            recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

            if(bool_message_box_yes_button_clicked)
            {
                slot_load_case_set_sources_path(list_source_info,true);
            }
            else
                slot_load_case_set_sources_path(list_source_info,false);

        }
        else
            slot_load_case_set_sources_path(list_source_info,true);
    }
    else
    {

        message_dialog_object->pub_set_message(MACRO_MSG_CHECK_YES_NO,"Want to mount/locate sources which are not accessible?");
        message_dialog_object->pub_set_window_frameless(true);
        message_dialog_object->show();

        recon_static_functions::debug_conditional(" : running 1", Q_FUNC_INFO);
        bool_message_box_clicked = false;
        while(1)
        {
            QCoreApplication::processEvents();
            if(bool_message_box_clicked)
                break;
        }

        message_dialog_object->pub_set_window_frameless(false);
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

        if(bool_message_box_yes_button_clicked)
        {
            load_case_source_displayer_obj->pub_set_list_of_source_path_and_type_in_page(list_info_for_load_case_displayer);
            load_case_source_displayer_obj->show();

            bool_display_load_case_page_ok_clicked = false;

            recon_static_functions::debug_conditional(" : running 2", Q_FUNC_INFO);
            while(1)
            {
                QCoreApplication::processEvents();
                if(bool_display_load_case_page_ok_clicked)
                    break;
            }
            recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

        }
        else
        {
            slot_load_case_set_sources_path(list_source_info,false);
        }
    }
}

void MainWindow::slot_load_case_set_sources_path(QList<struct_GLOBAL_witness_info_source> list_received_source_info_for_updation, bool bool_want_to_mount_root_source)
{

    recon_static_functions::app_debug("Arjun start ",Q_FUNC_INFO);


    // In source structure 'FS Extraction source path' should also be updated
    // bcoz we used this variable in FS Module threads

    bool_display_load_case_page_ok_clicked = true;

    load_case_source_displayer_obj->hide();

    map_ios_uid_backup_parser_obj->clear();
    map_key_src_cnt_name_val_raw_fuse_mnt_path.clear();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QList<struct_GLOBAL_witness_info_source> list_source_info_from_result = global_witness_info_manager_class_obj->pub_get_source_structure_QList();

    QStringList processed_images_list;

    QString root_count_name;

    for(int i = 0; i < list_received_source_info_for_updation.size(); i++)
    {
        struct_GLOBAL_witness_info_source struct_received_obj = list_received_source_info_for_updation.at(i);

        if((struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
            || struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal)
                && !bool_want_to_mount_root_source)
        {
            clear_disk_node_for_roots_and_sources_structures(list_source_info_from_result , struct_received_obj);
            continue;
        }

        QString src_type_intrnl = struct_received_obj.source_type_internal;

        struct_GLOBAL_witness_info_source struct_from_rslt_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(struct_received_obj.source_count_name);

        if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ForensicsImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FileVaultImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_FusionImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONFSBlockImage_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_OpticalDiscImage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Sparseimage_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_DMG_Internal
                || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_EncaseLogicalImage_L01_Internal)
        {

            root_count_name = struct_received_obj.root_count_name;

            QString source_path = struct_received_obj.source_path_1;

            QString hierarchy_str = struct_from_rslt_obj.source_hierarchy;
            if(hierarchy_str.trimmed().isEmpty())
                hierarchy_str = struct_from_rslt_obj.root_count_name;


            if(struct_received_obj.bool_source_nested)
            {
                QString immediate_parent;
                if(hierarchy_str.contains(MACRO_RECON_Splitter_4_forward))
                {
                    QStringList tmp_split_list = hierarchy_str.split(MACRO_RECON_Splitter_4_forward);
                    immediate_parent = tmp_split_list.last();

                    struct_GLOBAL_witness_info_source prnt_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(immediate_parent);

                    if(struct_received_obj.bool_source_decompressed)
                    {
                        source_path = global_recon_helper_singular_class_obj->get_complete_file_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), source_path, Q_FUNC_INFO);
                    }
                    else
                    {
                        source_path = global_recon_helper_singular_class_obj->get_complete_file_path(prnt_source_info.virtual_source_path, source_path, Q_FUNC_INFO);
                    }
                }
            }

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Type_Internal_QString,src_type_intrnl);

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_1_QString,source_path);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString,struct_received_obj.source_path_2_for_fusion);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RUNTIME_Root_Recovery_Passcode_Plain_QString,struct_received_obj.root_recovery_passcode_plain);


            if(processed_images_list.contains(source_path))
                continue;

            processed_images_list << source_path;


            set_device_mounting();

            QStringList source_count_name_list_to_be_updated;


            for(int count = 0; count < list_source_info_from_result.size(); count++)
            {
                struct_GLOBAL_witness_info_source rslt_obj = list_source_info_from_result.at(count);

                if(rslt_obj.root_count_name.trimmed() == hierarchy_str && !rslt_obj.bool_source_nested)
                    source_count_name_list_to_be_updated << rslt_obj.source_count_name;
                else if((rslt_obj.source_hierarchy + rslt_obj.source_path_1) == (hierarchy_str + struct_from_rslt_obj.source_path_1))
                {
                    source_count_name_list_to_be_updated << rslt_obj.source_count_name;
                }
            }


            struct_global_image_mounter_image_info obj_mount_info = image_mounter_obj->pub_get_struct_image_info_by_image_path(source_path);
            QList<struct_GLOBAL_witness_info_root> update_root_info_list = get_updated_root_structure_list(obj_mount_info);
            global_witness_info_manager_class_obj->pub_set_updated_root_structure_QList(update_root_info_list);


            /// According to Er. Manoj Kumar - Jan-02-2019
            /// According to rememberance, they are similar in sequence and counting
            /// Note: Update when the partitions mounted from one image and sources found from same image are same.
            if(obj_mount_info.list_struct_partition_info.size() != source_count_name_list_to_be_updated.size())
                continue;

            // Partition Update
            for(int k = 0; k < obj_mount_info.list_struct_partition_info.size(); k++)
            {
                struct_global_image_mounter_partition_info struct_lastly_mounted_partition_info = obj_mount_info.list_struct_partition_info.at(k);

                for(int count = 0; count < list_source_info_from_result.size(); count++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.at(count);

                    if(st_rslt_obj.source_count_name.trimmed() == source_count_name_list_to_be_updated.at(k))
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;
                        st_rslt_obj.source_path_2_for_fusion = struct_received_obj.source_path_2_for_fusion;

                        QString vrtl_path = struct_lastly_mounted_partition_info.mount_path;
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        st_rslt_obj.root_disk_identifier = struct_lastly_mounted_partition_info.parent_identifier;
                        if(struct_lastly_mounted_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive) || struct_lastly_mounted_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
                            st_rslt_obj.source_disk_identifier = struct_lastly_mounted_partition_info.derived_into_identifier;
                        else
                            st_rslt_obj.source_disk_identifier = struct_lastly_mounted_partition_info.parent_identifier;

                        map_key_src_cnt_name_val_raw_fuse_mnt_path.insert(st_rslt_obj.source_count_name, obj_mount_info.osx_fuse_mount_point_1);

                        list_source_info_from_result.replace(count, st_rslt_obj);
                        global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);

                    }
                }
            }

        }
        else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupImage_Internal)
        {
            root_count_name = struct_received_obj.root_count_name;

            QString source_path = struct_received_obj.source_path_1;

            QString hierarchy_str = struct_from_rslt_obj.source_hierarchy;
            if(hierarchy_str.trimmed().isEmpty())
                hierarchy_str = struct_from_rslt_obj.root_count_name;


            if(struct_received_obj.bool_source_nested)
            {
                QString immediate_parent;
                if(hierarchy_str.contains(MACRO_RECON_Splitter_4_forward))
                {
                    QStringList tmp_split_list = hierarchy_str.split(MACRO_RECON_Splitter_4_forward);
                    immediate_parent = tmp_split_list.last();

                    struct_GLOBAL_witness_info_source prnt_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(immediate_parent);

                    if(struct_received_obj.bool_source_decompressed)
                    {
                        source_path = global_recon_helper_singular_class_obj->get_complete_file_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), source_path, Q_FUNC_INFO);
                    }
                    else
                    {
                        source_path = global_recon_helper_singular_class_obj->get_complete_file_path(prnt_source_info.virtual_source_path, source_path, Q_FUNC_INFO);
                    }
                }
            }

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Type_Internal_QString,src_type_intrnl);

            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_1_QString,source_path);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_Path_2_For_Fusion_QString,struct_received_obj.source_path_2_for_fusion);
            global_narad_muni_class_obj->set_field(MACRO_NARAD_Source_RUNTIME_Root_Recovery_Passcode_Plain_QString,struct_received_obj.root_recovery_passcode_plain);


            if(processed_images_list.contains(source_path))
                continue;

            processed_images_list << source_path;


            set_device_mounting();

            QStringList source_count_name_list_to_be_updated;


            for(int count = 0; count < list_source_info_from_result.size(); count++)
            {
                struct_GLOBAL_witness_info_source rslt_obj = list_source_info_from_result.at(count);

                if(rslt_obj.root_count_name.trimmed() == hierarchy_str && !rslt_obj.bool_source_nested)
                    source_count_name_list_to_be_updated << rslt_obj.source_count_name;
                else if((rslt_obj.source_hierarchy + rslt_obj.source_path_1) == (hierarchy_str + struct_from_rslt_obj.source_path_1))
                {
                    source_count_name_list_to_be_updated << rslt_obj.source_count_name;
                }
            }


            struct_global_image_mounter_image_info obj_mount_info = image_mounter_obj->pub_get_struct_image_info_by_image_path(source_path);
            QList<struct_GLOBAL_witness_info_root> update_root_info_list = get_updated_root_structure_list(obj_mount_info);
            global_witness_info_manager_class_obj->pub_set_updated_root_structure_QList(update_root_info_list);


            /// According to Er. Manoj Kumar - Jan-02-2019
            /// According to rememberance, they are similar in sequence and counting
            /// Note: Update when the partitions mounted from one image and sources found from same image are same.
            if(obj_mount_info.list_struct_partition_info.size() != source_count_name_list_to_be_updated.size())
                continue;

            // Partition Update
            for(int k = 0; k < obj_mount_info.list_struct_partition_info.size(); k++)
            {
                struct_global_image_mounter_partition_info struct_lastly_mounted_partition_info = obj_mount_info.list_struct_partition_info.at(k);

                for(int count = 0; count < list_source_info_from_result.size(); count++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.at(count);

                    if(st_rslt_obj.source_count_name.trimmed() == source_count_name_list_to_be_updated.at(k))
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;
                        st_rslt_obj.source_path_2_for_fusion = struct_received_obj.source_path_2_for_fusion;

                        struct_lastly_mounted_partition_info.mount_path = recon_helper_standard_obj->get_new_mount_path_of_time_machine_backup_image_and_snapshots(struct_lastly_mounted_partition_info.mount_path,QString(MACRO_Mount_Path_Time_Machine_Backup_Image_Main));

                        if(struct_lastly_mounted_partition_info.mount_path.isEmpty())
                            continue;

                        QString vrtl_path = struct_lastly_mounted_partition_info.mount_path;
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        st_rslt_obj.root_disk_identifier = struct_lastly_mounted_partition_info.parent_identifier;
                        if(struct_lastly_mounted_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive) || struct_lastly_mounted_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS,Qt::CaseInsensitive))
                            st_rslt_obj.source_disk_identifier = struct_lastly_mounted_partition_info.derived_into_identifier;
                        else
                            st_rslt_obj.source_disk_identifier = struct_lastly_mounted_partition_info.parent_identifier;

                        map_key_src_cnt_name_val_raw_fuse_mnt_path.insert(st_rslt_obj.source_count_name, obj_mount_info.osx_fuse_mount_point_1);

                        list_source_info_from_result.replace(count, st_rslt_obj);
                        global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);

                    }
                }
            }
        }
        else
        {
            if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_MountedVolumes_Internal)
            {
                bool bool_current_src_found = false;

                QList<struct_GLOBAL_witness_info_root> lt_root_info = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        QString mounted_disk_node = global_recon_helper_singular_class_obj->get_disk_node_of_mounted_path(vrtl_path , Q_FUNC_INFO);

                        QString disk_identif = mounted_disk_node;
                        if(disk_identif.startsWith(QString("/dev/")))
                            disk_identif.remove(0,QString("/dev/").size());

                        st_rslt_obj.root_disk_identifier = disk_identif;
                        st_rslt_obj.source_disk_identifier = disk_identif;

                        //==================== Update root strcture info start ===============//
                        for(int jj = 0; jj < lt_root_info.size(); jj++)
                        {
                            struct_GLOBAL_witness_info_root st_root_info = lt_root_info.takeAt(jj);

                            if(st_root_info.root_count_name == struct_received_obj.root_count_name)
                            {
                                st_root_info.root_disk_node_1 = mounted_disk_node;
                                st_root_info.root_disk_identifier_1 = disk_identif;
                                break;
                            }

                            lt_root_info.insert(jj, st_root_info);
                        }
                        //==================== Update root strcture info end ===============//

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }

                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
                global_witness_info_manager_class_obj->pub_set_updated_root_structure_QList(lt_root_info);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GoogleTakeout_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_Folder_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONLogicalImage_Folder_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RECONMACSharingMode_Folder_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal
                    || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_Folder_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }
                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_iTunesiOSBackup_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);


                        ios_uid_backup_parser *ios_parser_obj = new ios_uid_backup_parser(this);
                        ios_parser_obj->pub_set_backup_path(vrtl_path);
                        ios_parser_obj->pub_set_os_version(st_rslt_obj.os_version_extended);
                        ios_parser_obj->pub_parse_backup();
                        map_ios_uid_backup_parser_obj->insert(st_rslt_obj.source_count_name,ios_parser_obj);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }
                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }
                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }

                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);


                        ios_uid_backup_parser *ios_parser_obj = new ios_uid_backup_parser(this);
                        ios_parser_obj->pub_set_backup_path(vrtl_path);
                        ios_parser_obj->pub_set_os_version(st_rslt_obj.os_version_extended);
                        ios_parser_obj->pub_parse_backup();
                        map_ios_uid_backup_parser_obj->insert(st_rslt_obj.source_count_name,ios_parser_obj);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }

                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }

                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString vrtl_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        st_rslt_obj.virtual_source_path = vrtl_path;
                        st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }
                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
            else if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_macOSHomeDirectory_Internal)
            {
                bool bool_current_src_found = false;
                for(int j = 0; j < list_source_info_from_result.size(); j++)
                {
                    struct_GLOBAL_witness_info_source st_rslt_obj = list_source_info_from_result.takeAt(j);

                    if(struct_received_obj.source_count_name == st_rslt_obj.source_count_name)
                    {
                        st_rslt_obj.root_path_1 = struct_received_obj.root_path_1;
                        st_rslt_obj.source_path_1 = struct_received_obj.source_path_1;

                        QString home_dir_path = get_virtual_source_path_for_load_case_except_images(struct_received_obj);
                        QFileInfo info1(home_dir_path);
                        if(info1.exists())
                        {
                            QString user;
                            QStringList tmp_users_list = struct_received_obj.system_user_QStringList;
                            if(tmp_users_list.size() <= 0)
                                user = "";
                            else
                                user = tmp_users_list.at(0);

                            QString vrtl_path = soft_mount(home_dir_path,user);
                            st_rslt_obj.virtual_source_path = vrtl_path;
                            st_rslt_obj.fs_extraction_source_path = recon_static_functions::get_fs_extraction_source_path(global_narad_muni_class_obj,src_type_intrnl,vrtl_path, Q_FUNC_INFO);

                        }

                        bool_current_src_found = true;
                    }

                    list_source_info_from_result.insert(j, st_rslt_obj);

                    if(bool_current_src_found)
                        break;
                }

                global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            }
        }
    }


    update_tsk_mount_path_for_source();
    update_snapshot_mount_path_for_load_case();
    update_raw_fuse_mount_path_for_source();

    recon_static_functions::app_debug("Arjun End ",Q_FUNC_INFO);

}

QString MainWindow::get_virtual_source_path_for_load_case_except_images(struct_GLOBAL_witness_info_source struct_received_obj)
{
    QString virtual_src_path;
    if(struct_received_obj.bool_source_nested)
    {
        QString source_path = struct_received_obj.source_path_1;
        if(struct_received_obj.bool_source_decompressed)
        {
            source_path = global_recon_helper_singular_class_obj->get_complete_file_path(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString(), source_path, Q_FUNC_INFO);
        }
        else
        {
            QString prnt_src_count = struct_received_obj.parent_source_count_name;
            struct_GLOBAL_witness_info_source prnt_struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(prnt_src_count);
            source_path = global_recon_helper_singular_class_obj->get_complete_file_path(prnt_struct_source_info.virtual_source_path,source_path, Q_FUNC_INFO);
        }
        virtual_src_path = source_path;
    }
    else
    {
        virtual_src_path = struct_received_obj.source_path_1;

        if(struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_ADBAndroidBackup_AB_Internal)
        {
            QString src_cnt_nm = struct_received_obj.source_count_name;

            QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();
            QString db_path = bkup_location + "android_backup_index.sqlite";


            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            source_db.setDatabaseName(db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
                return virtual_src_path;
            }


            QString cmd = "select INT from tbl_index where source_count_name = ?";
            QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd, QStringList(src_cnt_nm) ,0, source_db, Q_FUNC_INFO);

            virtual_src_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db , "tbl_index", src_cnt_nm, Q_FUNC_INFO);

            source_db.close();
        }
        else if(struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_TAR_Internal)
        {
            QString src_cnt_nm = struct_received_obj.source_count_name;

            QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
            QString db_path = bkup_location + "ios_backup_index.sqlite";


            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            source_db.setDatabaseName(db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
                return virtual_src_path;
            }


            QString cmd = "select INT from tbl_index where source_count_name = ?";
            QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd, QStringList(src_cnt_nm) ,0, source_db, Q_FUNC_INFO);

            virtual_src_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db , "tbl_index", src_cnt_nm, Q_FUNC_INFO);

            virtual_src_path = virtual_src_path + "/" + MACRO_CELLEBRITE_iOS_BACKUP_TAR_EXTRACTED_DIR_NAME;

            source_db.close();
        }
        else if(struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_CellebriteiOSBackup_UFDR_Internal)
        {
            QString src_cnt_nm = struct_received_obj.source_count_name;

            QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
            QString db_path = bkup_location + "ios_backup_index.sqlite";


            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            source_db.setDatabaseName(db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
                return virtual_src_path;
            }


            QString cmd = "select INT from tbl_index where source_count_name = ?";
            QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd, QStringList(src_cnt_nm) ,0, source_db, Q_FUNC_INFO);

            virtual_src_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db , "tbl_index", src_cnt_nm, Q_FUNC_INFO);

            source_db.close();
        }
        else if(struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyiOSBackup_Internal)
        {
            QString src_cnt_nm = struct_received_obj.source_count_name;

            QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
            QString db_path = bkup_location + "ios_backup_index.sqlite";


            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            source_db.setDatabaseName(db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
                return virtual_src_path;
            }


            QString cmd = "select INT from tbl_index where source_count_name = ?";
            QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd, QStringList(src_cnt_nm) ,0, source_db, Q_FUNC_INFO);

            virtual_src_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db , "tbl_index", src_cnt_nm, Q_FUNC_INFO);

            source_db.close();

        }
        else if(struct_received_obj.source_type_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_GrayKeyAndroidBackup_Internal)
        {
            QString src_cnt_nm = struct_received_obj.source_count_name;

            QString bkup_location = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();
            QString db_path = bkup_location + "android_backup_index.sqlite";


            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            source_db.setDatabaseName(db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug("Database open ----FAILED---- " + source_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("Error " + source_db.lastError().text() , Q_FUNC_INFO);
                return virtual_src_path;
            }


            QString cmd = "select INT from tbl_index where source_count_name = ?";
            QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(cmd, QStringList(src_cnt_nm) ,0, source_db, Q_FUNC_INFO);

            virtual_src_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record_no, source_db , "tbl_index", src_cnt_nm, Q_FUNC_INFO);

            source_db.close();
        }
    }

    QString src_type_intrnl = struct_received_obj.source_type_internal;

    if(src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal
            || src_type_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_RAMImage_Internal)
    {
        ///dont add "/" at end
        if(!virtual_src_path.trimmed().isEmpty())
            virtual_src_path = QFileInfo(virtual_src_path).absoluteDir().absolutePath();
    }
    else
    {
        if(!virtual_src_path.trimmed().isEmpty() && !virtual_src_path.endsWith("/"))
            virtual_src_path.append("/");
    }

    return virtual_src_path;
}


void MainWindow::slot_filesystem_extraction_finish_on_load_case()
{
    bool_fs_extraction_finish_on_load_case = true;
}

QStringList MainWindow::get_redefined_result_saved_name_from_result_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive(" Start " , Q_FUNC_INFO);

    QString command = "select display_name from saved_index";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}

void MainWindow::slot_email_files_case_tree_add_finish_on_load_case()
{
    bool_email_files_case_tree_add_finish_on_load_case = true;
}

void MainWindow::slot_google_mail_files_case_tree_add_finish_on_load_case()
{
    bool_google_mail_files_case_tree_add_finish_on_load_case = true;
}

QStringList MainWindow::get_redefined_result_namelist_from_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive("  Start " , Q_FUNC_INFO);

    QString command = "select display_name from 'redefined_result_index'";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}

QStringList MainWindow::get_content_search_namelist_from_result_db(QString result_feature_path, QString caller_func)
{
    recon_static_functions::debug_intensive(" Start " , Q_FUNC_INFO);

    QString destination_db_file = result_feature_path + "index_content_search.sqlite";
    QString command = "select search_label_name from 'table_index_content_search'";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}
QStringList MainWindow::get_story_board_namelist_from_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive(" Start " , Q_FUNC_INFO);

    QString command = "select display_name from 'story_board'";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}

QStringList MainWindow::get_saved_graph_result_namelist_from_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive("  Start " , Q_FUNC_INFO);

    QString command = "select display_name from 'graph_result_index'";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}

QStringList MainWindow::get_file_search_namelist_from_result_db(QString result_feature_path, QString caller_func)
{
    recon_static_functions::debug_intensive(" Start " , Q_FUNC_INFO);

    QString destination_db_file = result_feature_path + "index_file_search.sqlite";
    QString command = "select search_label_name from 'file_search_index'";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}

QStringList MainWindow::get_artifacts_timeline_saved_name_from_result_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive(" Start " , Q_FUNC_INFO);

    QString command = "select db_name from saved_timeline";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);
    return list;
}

QStringList MainWindow::get_exif_metadata_search_namelist_from_result_db(QString result_feature_path, QString caller_func)
{
    recon_static_functions::debug_intensive(" Start "+ QString("Caller-") + caller_func, Q_FUNC_INFO);

    QString destination_db_file = result_feature_path + "index_exif_metadata_search.sqlite";
    QString command = "select search_label_name from 'exif_metadata_search_index'";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(" end " + QString("Caller-") + caller_func , Q_FUNC_INFO);
    return list;
}


void MainWindow::clear_disk_node_for_roots_and_sources_structures(QList<struct_GLOBAL_witness_info_source> &list_source_info_from_result , struct_GLOBAL_witness_info_source struct_received_obj)
{

    recon_static_functions::app_debug(" Start " , Q_FUNC_INFO);


    for(int pp = 0 ; pp < list_source_info_from_result.size() ; pp++)
    {
        struct_GLOBAL_witness_info_source st_src_rslt_obj = list_source_info_from_result.at(pp);

        if(st_src_rslt_obj.source_count_name == struct_received_obj.source_count_name)
        {
            st_src_rslt_obj.source_disk_identifier.clear();
            st_src_rslt_obj.root_disk_identifier.clear();
            list_source_info_from_result.replace(pp,st_src_rslt_obj);

            struct_GLOBAL_witness_info_root st_rt_svd_obj = global_witness_info_manager_class_obj->pub_get_root_structure_according_root_count_name(st_src_rslt_obj.root_count_name);
            st_rt_svd_obj.root_disk_identifier_1.clear();
            st_rt_svd_obj.root_disk_identifier_2_for_fusion.clear();
            st_rt_svd_obj.root_disk_node_1.clear();
            st_rt_svd_obj.root_disk_node_2_for_fusion.clear();

            QList<struct_GLOBAL_witness_info_root> qlist_rt_svd = global_witness_info_manager_class_obj->pub_get_root_structure_QList();

            for(int kk = 0;kk < qlist_rt_svd.size() ; kk++)
            {
                if(qlist_rt_svd.at(kk).root_count_name == st_rt_svd_obj.root_count_name)
                {
                    qlist_rt_svd.replace(kk,st_rt_svd_obj);

                    global_witness_info_manager_class_obj->pub_set_updated_root_structure_QList(qlist_rt_svd);
                    break;
                }
            }

            global_witness_info_manager_class_obj->pub_set_updated_source_structure_QList(list_source_info_from_result);
            break;
        }
    }


    recon_static_functions::app_debug(" End " , Q_FUNC_INFO);

}
