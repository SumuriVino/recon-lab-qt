#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::create_and_retrieve_recon_result_dir_structure(QString recon_rslt_path)
{
    recon_static_functions::app_debug(" - Starts",Q_FUNC_INFO);

    create_adhoc_paths();

    QDir dir;

    //====================================== start =======================================//

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString, ( recon_rslt_path + "Lab_Conf/"));
    QString conf_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Parent_In_Result_QString).toString();
    dir.mkpath(conf_parent_path);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString, (conf_parent_path + "Case_Configuration/"));
    QString conf_path_case_config = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString();
    dir.mkpath(conf_path_case_config);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Chat_View_In_Result_QString, (conf_parent_path + "Chat_View/"));
    QString conf_path_chat_viewer = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Chat_View_In_Result_QString).toString();
    dir.mkpath(conf_path_chat_viewer);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString, (conf_parent_path + "CSVS/"));
    QString conf_path_csv_all = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_All_In_Result_QString).toString();
    dir.mkpath(conf_path_csv_all);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString, (conf_path_csv_all + "Plugins_CSV/"));
    QString conf_path_plgn_csv = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Plugins_In_Result_QString).toString();
    dir.mkpath(conf_path_plgn_csv);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString, (conf_path_csv_all + "Features_CSV/"));
    QString conf_path_ftr_csv = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_CSV_Feature_In_Result_QString).toString();
    dir.mkpath(conf_path_ftr_csv);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString, (conf_parent_path + "Manage_Columns/"));
    QString conf_manage_columns_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Manage_Column_In_Result_QString).toString();
    dir.mkpath(conf_manage_columns_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString, (conf_parent_path + "Redefined_Result_Filters/"));
    QString conf_redf_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Redefined_Result_Filter_In_Result_QString).toString();
    dir.mkpath(conf_redf_result);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString, (conf_parent_path + "Sources/"));
    QString conf_source = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString();
    dir.mkpath(conf_source);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Caches_In_Result_QString, (recon_rslt_path + "Caches/"));
    QString conf_caches = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Caches_In_Result_QString).toString();
    dir.mkpath(conf_caches);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString, (conf_parent_path + "Hashset/"));
    QString conf_hashset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString();
    dir.mkpath(conf_hashset);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Conf_Path_Location_Artifacts_Timeline_Full_In_Result_QString, (conf_parent_path + "Artifacts_Timeline/Artifacts_Timeline_Full/"));
    QString confg_artifacts_timeline_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
    dir.mkpath(confg_artifacts_timeline_path);

    //====================================== end =======================================//




    //======================================= start =====================================//

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Parent_In_Result_QString, ( recon_rslt_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features));
    QString featr_parent_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Parent_In_Result_QString).toString();
    dir.mkpath(featr_parent_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString, (featr_parent_path + "Hex_Viewer/"));
    QString path_hex_viewer = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString();
    dir.mkpath(path_hex_viewer);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString, (featr_parent_path + "Plist_Viewer/"));
    QString path_plist_viewer = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString();
    dir.mkpath(path_plist_viewer);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString, (featr_parent_path + "Log_Viewer/"));
    QString path_log_viewer = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString();
    dir.mkpath(path_log_viewer);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString, (featr_parent_path + "Unified_Logs/"));
    QString path_unified_log_viewer = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    dir.mkpath(path_unified_log_viewer);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString, (featr_parent_path + "Content_Search/"));
    QString path_content_search_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString();
    dir.mkpath(path_content_search_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Activity_Log_In_Result_QString, (featr_parent_path + "Activity_Logs/"));
    QString activity_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Activity_Log_In_Result_QString).toString();
    dir.mkpath(activity_logs_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString, (featr_parent_path + "Artifacts_Keyword_Search/"));
    QString path_keyword_search = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    dir.mkpath(path_keyword_search);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString, (featr_parent_path + "Artifacts_Timeline/Artifacts_Timeline_Full/"));
    QString full_artifacts_timelinepath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
    dir.mkpath(full_artifacts_timelinepath);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString, (featr_parent_path + "Artifacts_Timeline/Artifacts_Timeline_Saved/"));
    QString artifact_timeline_sv_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString();
    dir.mkpath(artifact_timeline_sv_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString, (artifact_timeline_sv_path + "Artifacts_Timeline_Saved_Graphs/"));
    QString artifact_timeline_saved_graph = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString();
    dir.mkpath(artifact_timeline_saved_graph);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString, (featr_parent_path + "Bookmarks_Search/"));
    QString bookmarks_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString();
    dir.mkpath(bookmarks_search_dir_path);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString, (featr_parent_path + "File_System/"));
    QString path_filesystem_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString();
    dir.mkpath(path_filesystem_dir);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString, (featr_parent_path + "File_Search/"));
    QString path_file_search = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString();
    dir.mkpath(path_file_search);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString, (featr_parent_path + "Exif_Metadata_Search/"));
    QString path_exif_metadata_search = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString();
    dir.mkpath(path_exif_metadata_search);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Hashset_In_Result_QString, (featr_parent_path + "Hashset/"));
    QString path_hashset = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hashset_In_Result_QString).toString();
    dir.mkpath(path_hashset);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString, (featr_parent_path + "Notes_Search/"));
    QString notes_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString();
    dir.mkpath(notes_search_dir_path);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString, (featr_parent_path + "Redefined_Results/Redefined_Results_Full/"));
    QString path_redf_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Full_In_Result_QString).toString();
    dir.mkpath(path_redf_result);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString, (featr_parent_path + "Redefined_Results/Redefined_Results_Saved/Messenger/"));
    QString redf_result_msg_saved_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString();
    dir.mkpath(redf_result_msg_saved_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString, (featr_parent_path + "Redefined_Results/Redefined_Results_Saved/Browser_History/"));
    QString redf_result_browser_saved_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString();
    dir.mkpath(redf_result_browser_saved_path);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString, (featr_parent_path + "Screenshots/"));
    QString recon_screenshots_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString();
    dir.mkpath(recon_screenshots_dir_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Screenshots_Images_Directory_In_Result_QString, (recon_screenshots_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images));
    QString recon_screenshots_img_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_Images_Directory_In_Result_QString).toString();
    dir.mkpath(recon_screenshots_img_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString, (featr_parent_path + "Story_Board/"));
    QString story_board_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString();
    dir.mkpath(story_board_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString, (featr_parent_path + "Saved_Maps/"));
    QString saved_maps_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString();
    dir.mkpath(saved_maps_dir_path);
    QString saved_map_images = saved_maps_dir_path + "Maps/";
    dir.mkpath(saved_map_images);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString, (featr_parent_path + "Tag_Search/"));
    QString tag_search_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString();
    dir.mkpath(tag_search_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString, (featr_parent_path + "SQLite_Viewer/"));
    QString path_sqlite_viewer = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString();
    dir.mkpath(path_sqlite_viewer);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString, (featr_parent_path + "Ram_Analysis_Saved/"));
    QString ram_analysis_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString();
    dir.mkpath(ram_analysis_dir_path);

    //global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Emlx_file_In_Result_QString, (featr_parent_path + MACRO_GLOBAL_RECON_AND_ARTIFACT_COMMON_RESULT_DIR_Emlx_Files));


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString, (featr_parent_path + "Email_Parser/"));
    QString email_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
    dir.mkpath(email_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Index_In_Result_QString, (featr_parent_path + "Saved_Graphs/"));
    QString saved_graph_index_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Index_In_Result_QString).toString();
    dir.mkpath(saved_graph_index_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString, (saved_graph_index_dir_path + "Browser_History/"));
    QString saved_graph_browser_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString();
    dir.mkpath(saved_graph_browser_dir_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_Images_Directory_In_Result_QString, (saved_graph_browser_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images));
    QString saved_graph_browser_img_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_Images_Directory_In_Result_QString).toString();
    dir.mkpath(saved_graph_browser_img_dir_path);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString, (saved_graph_index_dir_path + "Messenger/"));
    QString saved_graph_messenger_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString();
    dir.mkpath(saved_graph_messenger_dir_path);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_Images_Directory_In_Result_QString, (saved_graph_messenger_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images));
    QString saved_graph_messenger_images_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_Images_Directory_In_Result_QString).toString();
    dir.mkpath(saved_graph_messenger_images_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString, (featr_parent_path + "Chat_Viewer/"));
    QString chat_viewer_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString();
    dir.mkpath(chat_viewer_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString, (featr_parent_path + "Carved_Files/"));
    QString carver_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString();
    dir.mkpath(carver_dir_path);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString, (featr_parent_path + "Carved_Password/"));
    QString path_carved_password = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Password_In_Result_QString).toString();
    dir.mkpath(path_carved_password);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString, (featr_parent_path + "Mobile_Backup/"));
    QString path_ios_bckp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Mobile_Backup_In_Result_QString).toString();
    dir.mkpath(path_ios_bckp);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString, (featr_parent_path + "Registry_Viewer/"));
    QString registry_viewer_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString();
    dir.mkpath(registry_viewer_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString, (featr_parent_path + "Disk_Images/"));
    QString path_dsk_img_srcs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Images_In_Result_QString).toString();
    dir.mkpath(path_dsk_img_srcs);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString, (featr_parent_path + "Carved_Data/"));
    QString carver_data_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString();
    dir.mkpath(carver_data_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString, (featr_parent_path + "RAM_Images/"));
    QString path_ram_img_srcs = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Images_In_Result_QString).toString();
    dir.mkpath(path_ram_img_srcs);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString, (featr_parent_path + "Snapshots/"));
    QString snapshots_difference_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString();
    dir.mkpath(snapshots_difference_dir_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Snapshots_File_System_In_Result_QString, (path_filesystem_dir + "Snapshots/"));
    QString path_snapshots_filesystem_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_File_System_In_Result_QString).toString();
    dir.mkpath(path_snapshots_filesystem_dir);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Disk_Hex_View_In_Result_QString, (featr_parent_path + "Disk_Hex_View/"));
    QString path_disk_hex_view_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Hex_View_In_Result_QString).toString();
    dir.mkpath(path_disk_hex_view_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString, (featr_parent_path + "Face_Search/"));
    QString path_face_search = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Face_Search_In_Result_QString).toString();
    dir.mkpath(path_face_search);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString, (path_filesystem_dir + "Faces_Group/"));
    QString face_grp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString();
    dir.mkpath(face_grp);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Distinct_Faces_In_Result_QString, (face_grp + "Distinct_Faces/"));
    QString dstnct_face = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Distinct_Faces_In_Result_QString).toString();
    dir.mkpath(dstnct_face);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString, (featr_parent_path + "Examiner_Space/"));
    QString examiner_space_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString();
    dir.mkpath(examiner_space_dir_path);

    //===================================== end ========================================//




    //====================================== start =======================================//

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Parent_QString, ( recon_rslt_path + "Exported_Content/"));
    QString export_contnt_parent = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Parent_QString).toString();
    dir.mkpath(export_contnt_parent);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString, (export_contnt_parent + "Hex_Viewer/"));
    QString path_hex_viewer_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString();
    dir.mkpath(path_hex_viewer_content_export);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString, (export_contnt_parent + "Plist_Viewer/"));
    QString path_plist_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString();
    dir.mkpath(path_plist_content_export);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString, (export_contnt_parent + "Log_Viewer/"));
    QString path_log_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString();
    dir.mkpath(path_log_content_export);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString, (export_contnt_parent + "SQLite_Viewer/"));
    QString path_sqlite_viewer_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString();
    dir.mkpath(path_sqlite_viewer_content_export);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString, (export_contnt_parent + "Text_Viewer/"));
    QString path_text_viewer_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString();
    dir.mkpath(path_text_viewer_content_export);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString, (export_contnt_parent + "Strings_Viewer/"));
    QString path_strings_viewer_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString();
    dir.mkpath(path_strings_viewer_content_export);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString, (export_contnt_parent + "Carved_Data/"));
    QString path_carved_data_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Carved_Data_QString).toString();
    dir.mkpath(path_carved_data_content_export);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString, (export_contnt_parent + "Email_Parser_Tag/"));
    QString path_email_parser_content_export = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Email_Parser_Tag_QString).toString();
    dir.mkpath(path_email_parser_content_export);


    //====================================== end =======================================//




    //====================================== start =======================================//

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Parent_QString, ( recon_rslt_path + "Extracted_Content/"));
    QString extrct_contnt_parent = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Parent_QString).toString();
    dir.mkpath(extrct_contnt_parent);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString, (extrct_contnt_parent + "Android_Backup/"));
    QString extrct_contnt_andrd_bk = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Android_Backup_QString).toString();
    dir.mkpath(extrct_contnt_andrd_bk);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString, (extrct_contnt_parent + "iOS_Backup/"));
    QString extrct_contnt_ios_bkup = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_iOS_Backup_QString).toString();
    dir.mkpath(extrct_contnt_ios_bkup);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString, (extrct_contnt_parent + "Apple_Logs/"));
    QString extrct_contnt_apl_lg = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Logs_QString).toString();
    dir.mkpath(extrct_contnt_apl_lg);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Parent_QString, (extrct_contnt_parent + "Apple_Metadata/"));
    QString extrct_contnt_apl_metadata_prnt = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Parent_QString).toString();
    dir.mkpath(extrct_contnt_apl_metadata_prnt);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString, (extrct_contnt_apl_metadata_prnt + "Apple_Metadata_Python/"));
    QString extrct_contnt_apl_metadata_python_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Python_QString).toString();
    dir.mkpath(extrct_contnt_apl_metadata_python_str);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString, (extrct_contnt_apl_metadata_prnt + "Apple_Metadata_Reader_Metadata/"));
    QString extrct_contnt_apl_metadata_reader_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();
    dir.mkpath(extrct_contnt_apl_metadata_reader_metadata);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString, (extrct_contnt_apl_metadata_prnt + "Apple_Metadata_Reader_Timestamps/"));
    QString extrct_contnt_apl_metadata_reader_timestamps = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Timestamps_QString).toString();
    dir.mkpath(extrct_contnt_apl_metadata_reader_timestamps);

    //====================================== end =======================================//




    //====================================== start =======================================//

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Parent_QString, ( recon_rslt_path + "Exported_Files/"));
    QString export_file_parent = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Parent_QString).toString();
    dir.mkpath(export_file_parent);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_File_System_QString, (export_file_parent + "File_System/"));
    QString file_sys_export_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_File_System_QString).toString();
    dir.mkpath(file_sys_export_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Files_QString, (export_file_parent + "Files/"));
    QString files_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Files_QString).toString();
    dir.mkpath(files_exports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Hex_Viewer_QString, (export_file_parent + "Hex_Viewer/"));
    QString hex_export_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Hex_Viewer_QString).toString();
    dir.mkpath(hex_export_path);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Plugin_QString, (export_file_parent + "Plugins/"));
    QString plugin_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Plugin_QString).toString();
    dir.mkpath(plugin_exports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_SQLite_Viewer_QString, (export_file_parent + "SQLite_Viewer/"));
    QString sqlite_viewer_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_SQLite_Viewer_QString).toString();
    dir.mkpath(sqlite_viewer_exports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_RAM_Analysis_QString, (export_file_parent + "RAM_Analysis/"));
    QString ram_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_RAM_Analysis_QString).toString();
    dir.mkpath(ram_exports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Graphs_Browser_History_In_Result_QString, (export_file_parent + "Browser_History/"));
    QString browser_history_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Browser_History_In_Result_QString).toString();
    dir.mkpath(browser_history_exports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Graphs_Messenger_In_Result_QString, (export_file_parent + "Messenger/"));
    QString messenger_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Messenger_In_Result_QString).toString();
    dir.mkpath(messenger_exports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exported_File_Location_Graphs_Artifact_Timeline_In_Result_QString, (export_file_parent + "Artifact_Timeline/"));
    QString artifact_timeline_exports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Artifact_Timeline_In_Result_QString).toString();
    dir.mkpath(artifact_timeline_exports_dir);

    //==================================== End =========================================//




    //====================================== start =======================================//

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Location_Parent_In_Result_QString, (recon_rslt_path + "Reports/"));
    QString reports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Parent_In_Result_QString).toString();
    dir.mkpath(reports_dir);


    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Location_SQLite_Viewer_In_Result_QString, (reports_dir + "SQLite_Viewer/"));
    QString sqlite_viewer_reports = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_SQLite_Viewer_In_Result_QString).toString();
    dir.mkpath(sqlite_viewer_reports);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString, (reports_dir + "Global_Reports/"));
    QString glbl_reports = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Global_Report_In_Result_QString).toString();
    dir.mkpath(glbl_reports);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Location_Plugin_In_Result_QString, (reports_dir + "Plugin_Reports/"));
    QString plgn_reports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Plugin_In_Result_QString).toString();
    dir.mkpath(plgn_reports_dir);

    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Location_Timeline_In_Result_QString, (reports_dir + "Artifacts_Timeline/"));
    QString tmln_reports_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Report_Location_Timeline_In_Result_QString).toString();
    dir.mkpath(tmln_reports_dir);


    //==================================== End =========================================//




    QString temp_path_to_modules = recon_rslt_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS;
    dir.mkpath(temp_path_to_modules);


    QString win_config = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display);
    QDir m_dir;
    m_dir.mkpath(win_config);

    QString path_index_filesdir  = path_filesystem_dir + MACRO_Index_Files_Dir_Name_QString;
    dir.mkdir(path_index_filesdir);

    QString path_plist_viewer_tmp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Plist_viewer_In_Result_QString).toString();
    QString path_hex_viewer_tmp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Hex_viewer_In_Result_QString).toString();

    QString path_registry_viewer_tmp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Registry_Viewer_In_Result_QString).toString();
    QString path_log_viewer_tmp = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Log_viewer_In_Result_QString).toString();
    //==================================Config Start==================================//

    copy_configurations_in_result(conf_path_case_config);

    QString case_info_db = conf_path_case_config + "case_info.sqlite";
    if(!QFile::exists(case_info_db))
    {
        create_system_and_case_info_table_in_case_configuration_db(case_info_db);

    }


    QString export_case_status_db = conf_path_case_config + "export_case_status.sqlite";
    if(!QFile::exists(export_case_status_db))
    {
        QString command = "create table 'tbl_export_status' (category_name varchar(1024), list_name varchar(1024), check_status INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,export_case_status_db,Q_FUNC_INFO);

        prepare_feature_export_case_essentials();
    }

    //==================================Config End==================================//

    QString key_search_db_path = path_keyword_search + "artifacts_keyword_search.sqlite";
    if(!QFile::exists(key_search_db_path))
    {
        QString command_key_search = "create table 'keyword_search_index' (INT INTEGER PRIMARY KEY, search_tag varchar(30),table_name varchar(30),search_keywords varchar(100) ,creation_timestamp INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_key_search,key_search_db_path,Q_FUNC_INFO);
    }

    QString file_search_db_path = path_file_search + "index_file_search.sqlite";
    if(!QFile::exists(file_search_db_path))
    {
        QString command_file_search = "create table 'file_search_index' (INT INTEGER PRIMARY KEY, search_label_name varchar(1024), search_db_file_name varchar(1024), entered_file_name varchar(1024), entered_file_size varchar(1024) ,entered_creation_date_config varchar(1024), entered_modification_date_config varchar(1024), entered_added_date_config varchar(1024), entered_last_used_date_config varchar(1024), db_creation_timestamp INTEGER, is_radio_button_or_clicked varchar(100), mdfind_condition_str varchar(1024), total_records_extracted INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_file_search,file_search_db_path,Q_FUNC_INFO);
    }

    QString face_search_db_path = path_face_search + "index_face_search.sqlite";
    if(!QFile::exists(face_search_db_path))
    {
        QString command_face_search = "create table 'face_search_index' (INT INTEGER PRIMARY KEY, search_label_name varchar(1024), search_db_file_name varchar(1024), total_records_extracted INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_face_search,face_search_db_path,Q_FUNC_INFO);
    }

    QString cntnt_search_indx_db_path = path_content_search_dir + "index_content_search.sqlite";
    if(!QFile::exists(cntnt_search_indx_db_path))
    {
        QString command_search = "create table 'table_index_content_search' (INT INTEGER PRIMARY KEY, search_label_name varchar(1024), search_db_file_name varchar(1024), keywords_list varchar(1024), db_creation_timestamp INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_search,cntnt_search_indx_db_path,Q_FUNC_INFO);
    }


    QString exif_metadata_search_db_path = path_exif_metadata_search + "index_exif_metadata_search.sqlite";
    if(!QFile::exists(exif_metadata_search_db_path))
    {
        QString command_exif_metadata_search = "create table 'exif_metadata_search_index' (INT INTEGER PRIMARY KEY, search_label_name varchar(1024), search_db_file_name varchar(1024),processed_sources varchar(2048), total_records_extracted INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_exif_metadata_search,exif_metadata_search_db_path,Q_FUNC_INFO);
    }


    QString plugin_status_db = conf_path_case_config + "plugin_status.sqlite";
    if(!QFile::exists(plugin_status_db))
    {
        QString command = "create table 'tbl_plugin_status' (plugin_name varchar(1024), tab_name varchar(1024), run_status INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,plugin_status_db,Q_FUNC_INFO);
    }


    QString artifacts_timeline_db_path = full_artifacts_timelinepath + "artifacts_timeline_full.sqlite";
    if(!QFile::exists(artifacts_timeline_db_path))
    {
        QString command_artifacts_tiemline = "create table timeline_data (INT INTEGER PRIMARY KEY,plugin varchar(30),category varchar(30), table_name varchar(30), "
                                             "record_no INTEGER, stamp_name varchar(30), stamp_type varchar(30),timestamp INTEGER,item1 varchar(300), item2 varchar(300),os_scheme_display varchar(300),source_count_name varchar(300),"
                                             "loaded varchar(10), year_str varchar(100), month_year varchar(100), day_month_year varchar(100), hour_day_month_year varchar(100),bookmark,recon_tag_value varchar(30), notes varchar(100),blank_column varchar(100),"
                                             "notes_icon varchar(100),LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_artifacts_tiemline,artifacts_timeline_db_path,Q_FUNC_INFO);

        recon_helper_standard_obj->create_db_index_by_db_path("data_cat", "category", "timeline_data", artifacts_timeline_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("data_int", "INT", "timeline_data", artifacts_timeline_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("data_rec", "record_no", "timeline_data", artifacts_timeline_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("data_plg", "plugin", "timeline_data", artifacts_timeline_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("data_time", "timestamp", "timeline_data", artifacts_timeline_db_path, Q_FUNC_INFO);

    }

    QString artifacts_timeline_db_saved_event_db_path = confg_artifacts_timeline_path + "artifacts_timeline_config.sqlite";
    if(!QFile::exists(artifacts_timeline_db_saved_event_db_path))
    {
        QString command_artifacts_timeline_saved_events = "create table saved_events (INT INTEGER PRIMARY KEY,plugin varchar(30), category varchar(30),bookmark)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_artifacts_timeline_saved_events,artifacts_timeline_db_saved_event_db_path,Q_FUNC_INFO);

        QString command_artifacts_timeline_saved_events_timestamp = "create table saved_events_timestamp (INT INTEGER PRIMARY KEY,timestamp varchar(100),timestamp_type varchar(100), timestamp_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_artifacts_timeline_saved_events_timestamp,artifacts_timeline_db_saved_event_db_path,Q_FUNC_INFO);
    }


    QString artifact_timeline_saved = artifact_timeline_sv_path + "index_artifacts_timeline.sqlite";
    if(!QFile::exists(artifact_timeline_saved))
    {
        QString command_saved_artifacts = "create table saved_timeline(INT INTEGER PRIMARY KEY,db_name varchar(30),tab_name varchar(30))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_saved_artifacts,artifact_timeline_saved,Q_FUNC_INFO);
    }

    QString artifact_timeline_saved_graphs = artifact_timeline_saved_graph + "artifacts_timeline_saved_graphs.sqlite";
    if(!QFile::exists(artifact_timeline_saved_graphs))
    {
        QString command_saved_graphs = ("create table tags (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                        "record_no varchar(300),notes_icon varchar(300),plugin_name varchar(300), tab_name varchar(100),"
                                        "table_name varchar(200),recon_tag_value varchar(100),notes varchar(100),os_scheme_display varchar(100), source_count_name varchar(100),"
                                        "recon_filefrom varchar(10),recon_file_infopath varchar(500),"
                                        "item0 varchar(300),item1 varchar(300),item2 varchar(300),item3 varchar(300),item4 varchar(300),"
                                        "item5 varchar(300),item6 varchar(300),item7 varchar(300),item8 varchar(300),"
                                        "item9 varchar(300),item10 varchar(300),item11 varchar(300),item12 varchar(300),"
                                        "item13 varchar(300),item14 varchar(300),item15 varchar(300),item16 varchar(300),"
                                        "item17 varchar(300),item18 varchar(300),item19 varchar(300))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_saved_graphs,artifact_timeline_saved_graphs,Q_FUNC_INFO);
    }

    QString carver_index_db_path = carver_dir_path + "carved_files_index.sqlite";
    if(!QFile::exists(carver_index_db_path))
    {
        QString command = "create table 'table_carver_index' (INT INTEGER PRIMARY KEY, label_name varchar(1024), source_count_name varchar(10), carver_files_db_path varchar(1024))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,carver_index_db_path,Q_FUNC_INFO);
    }

    QString carved_password_index_db_path = path_carved_password + "carved_password_index.sqlite";
    if(!QFile::exists(carved_password_index_db_path))
    {
        QString command = "create table 'tbl_carved_password_index' (INT INTEGER PRIMARY KEY, label_name varchar(1024), source_count_name varchar(10), relative_db_path varchar(1024),"
                          "item1 varchar(30),item2 varchar(30),item3 varchar(30),item4 varchar(30),item5 varchar(30),"
                          "item6 varchar(30),item7 varchar(30),item8 varchar(30) ,item9 varchar(30),item10 varchar(30))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,carved_password_index_db_path,Q_FUNC_INFO);
    }

    QString plist_view_db_path = path_plist_viewer + "plist_viewer.sqlite";
    if(!QFile::exists(plist_view_db_path))
    {
        QString command_plist_view = "create table 'plist_viewer_index' (INT INTEGER PRIMARY KEY , viewer_display_name varchar(30),"
                                     "db_table_name varchar(30) ,file_name varchar(30),file_path varchar(30),recon_filefrom varchar(30),recon_file_infopath varchar(30),plugin_name varchar(30),"
                                     "category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_plist_view,plist_view_db_path,Q_FUNC_INFO);
    }
    QString plist_view_db_path_tmp = path_plist_viewer_tmp + "plist_viewer.sqlite";
    if(!QFile::exists(plist_view_db_path_tmp))
    {
        QString command_plist_view_tmp = "create table 'plist_viewer_index' (INT INTEGER PRIMARY KEY , viewer_display_name varchar(30),"
                                         "db_table_name varchar(30) ,file_name varchar(30),file_path varchar(30),recon_filefrom varchar(30),recon_file_infopath varchar(30),plugin_name varchar(30),"
                                         "category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_plist_view_tmp,plist_view_db_path_tmp,Q_FUNC_INFO);
    }

    QString saved_maps = saved_maps_dir_path + "saved_maps.sqlite";
    if(!QFile::exists(saved_maps))
    {
        QString command_saved_maps = ("create table tags (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                      "record_no varchar(300),notes_icon varchar(300),plugin_name varchar(300), tab_name varchar(100),"
                                      "table_name varchar(200),recon_tag_value varchar(100),notes varchar(100),os_scheme_display varchar(100), source_count_name varchar(100), recon_filefrom varchar(10),recon_file_infopath varchar(500),"
                                      "item0 varchar(300),item1 varchar(300),item2 varchar(300),item3 varchar(300),item4 varchar(300),"
                                      "item5 varchar(300),item6 varchar(300),item7 varchar(300),item8 varchar(300),"
                                      "item9 varchar(300),item10 varchar(300),item11 varchar(300),item12 varchar(300),"
                                      "item13 varchar(300),item14 varchar(300),item15 varchar(300),item16 varchar(300),"
                                      "item17 varchar(300),item18 varchar(300),item19 varchar(300))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_saved_maps,saved_maps,Q_FUNC_INFO);
    }




    QString hex_view_db_path = path_hex_viewer + "hex_viewer.sqlite";
    if(!QFile::exists(hex_view_db_path))
    {
        QString command_hex_view = "create table 'hex_viewer_index' (INT INTEGER PRIMARY KEY , viewer_display_name varchar(30),db_table_name varchar(30) ,file_name varchar(30),file_path varchar(30),plugin_name varchar(30),category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(30), recon_filefrom varchar(10),recon_file_infopath varchar(300))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_hex_view,hex_view_db_path,Q_FUNC_INFO);

        QString command_hex_view_blocks = "create table 'hex_viewer_blocks' (INT INTEGER PRIMARY KEY , file_name varchar(1000),file_path varchar(2048) ,block_name varchar(1024),block_start_offset varchar(1024),block_end_offset varchar(1024),block_output_path varchar(1024),block_color varchar(100),start_page_no INTEGER, viewer_display_name varchar(100),recon_filefrom varchar(10),recon_file_infopath varchar(300))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_hex_view_blocks,hex_view_db_path,Q_FUNC_INFO);
    }


    QString hex_view_db_path_tmp = path_hex_viewer_tmp + "hex_viewer.sqlite";
    if(!QFile::exists(hex_view_db_path_tmp))
    {
        QString command_hex_view_tmp = "create table 'hex_viewer_index' (INT INTEGER PRIMARY KEY , viewer_display_name varchar(30),db_table_name varchar(30) ,file_name varchar(30),file_path varchar(30),plugin_name varchar(30),category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(30), recon_filefrom varchar(10),recon_file_infopath varchar(300))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_hex_view_tmp,hex_view_db_path_tmp,Q_FUNC_INFO);

        QString command_hex_view_blocks_tmp = "create table 'hex_viewer_blocks' (INT INTEGER PRIMARY KEY , file_name varchar(1000),file_path varchar(2048) ,block_name varchar(1024),block_start_offset varchar(1024),block_end_offset varchar(1024),block_output_path varchar(1024),block_color varchar(100),start_page_no INTEGER, viewer_display_name varchar(100),recon_filefrom varchar(10),recon_file_infopath varchar(300))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_hex_view_blocks_tmp,hex_view_db_path_tmp,Q_FUNC_INFO);
    }
    QString log_view_db_path = path_log_viewer + "log_viewer.sqlite";
    if(!QFile::exists(log_view_db_path))
    {
        QString command_log_view = "create table 'log_viewer_index' (INT INTEGER PRIMARY KEY, viewer_display_name varchar(30),"
                                   "db_table_name varchar(30) ,file_name varchar(30),file_path varchar(30), log_file_data BLOB, recon_filefrom varchar(30),recon_file_infopath varchar(30),plugin_name varchar(30),"
                                   "category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_log_view,log_view_db_path,Q_FUNC_INFO);
    }


    QString log_view_db_path_tmp = path_log_viewer_tmp + "log_viewer.sqlite";
    if(!QFile::exists(log_view_db_path_tmp))
    {
        QString command_log_view_tmp = "create table 'log_viewer_index' (INT INTEGER PRIMARY KEY, viewer_display_name varchar(30),"
                                       "db_table_name varchar(30) ,file_name varchar(30),file_path varchar(30),log_file_data BLOB, recon_filefrom varchar(30),recon_file_infopath varchar(30),plugin_name varchar(30),"
                                       "category_name varchar(30),record_no varchar(30),creation_timestamp INTEGER, source_count_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_log_view_tmp,log_view_db_path_tmp,Q_FUNC_INFO);
    }

    QString hashset_db_path = path_hashset + "/searched_files.sqlite";
    if(!QFile::exists(hashset_db_path))
    {
        QString command_hashset = "create table 'files' (INT INTEGER PRIMARY KEY , file_name varchar(100),file_path varchar(500) ,suffix varchar(500),file_size varchar(30), modification_time INTEGER,creation_time INTEGER,access_time INTEGER,user_id varchar(30),group_id varchar(30), md5_hash varchar(500), hash_db_path varchar(500), tab_Record_no varchar(500),plugin_tabname varchar(500), plugin_name varchar(500),parent_plugin_name varchar(500))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_hashset,hashset_db_path,Q_FUNC_INFO);
    }



    QString search_hashset_index_db_path = path_hashset + "/index_searched_hashset.sqlite";
    if(!QFile::exists(search_hashset_index_db_path))
    {
        QString command_search_hashset_index = "create table 'searched_hashset_index' (INT INTEGER PRIMARY KEY , hashset_name VARCHAR(500), hashset_db_path VARCHAR(500))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_search_hashset_index,search_hashset_index_db_path,Q_FUNC_INFO);
    }



    QString sqlite_view_db_path = path_sqlite_viewer + "sqlite_viewer.sqlite";
    if(!QFile::exists(sqlite_view_db_path))
    {
        QString command_sqlite_view = "create table 'sv_index' (INT INTEGER PRIMARY KEY , viewer_display_name varchar(30),"
                                      "plugin_record_info varchar(300) ,file_name varchar(30),file_path varchar(30),recon_filefrom varchar(30),recon_file_infopath varchar(30),"
                                      "plugin_name varchar(30),category_name varchar(30),record_no varchar(30),"
                                      "creation_timestamp INTEGER,copied_file_path varchar(250),recreated_file_path varchar(250),"
                                      "destination_directory_path varchar(300),is_savestate varchar(10), savedstate_dest_dir_path varchar(1024),"
                                      "source_count_name varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_sqlite_view,sqlite_view_db_path,Q_FUNC_INFO);
    }

    QString registry_viewer_db_path = registry_viewer_dir_path + "registry_viewer_index.sqlite";
    if(!QFile::exists(registry_viewer_db_path))
    {
        QString command_reg_view = "create table 'tbl_registry_viewer_index' (INT INTEGER PRIMARY KEY ,"
                                   "display_tab_name varchar(30),source_file_name varchar(30),source_file_path varchar(300),recon_filefrom varchar(300),recon_file_infopath varchar(300),"
                                   "relative_db_path varchar(30) ,source_count_name varchar(30) , "
                                   "item1 varchar(30) ,  item2 varchar(30) ,  item3 varchar(30),  "
                                   "item4 varchar(30),  item5 varchar(30),  item6 varchar(30) ,  "
                                   "item7 varchar(30),  item8 varchar(30),  item9 varchar(30),  item10 varchar(30))";

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_reg_view,registry_viewer_db_path,Q_FUNC_INFO);
    }

    QString registry_viewer_tmp_db_path = path_registry_viewer_tmp + "registry_viewer_index.sqlite";
    if(!QFile::exists(registry_viewer_tmp_db_path))
    {
        QString command_reg_view = "create table 'tbl_registry_viewer_index' (INT INTEGER PRIMARY KEY ,"
                                   "display_tab_name varchar(30),source_file_name varchar(30),source_file_path varchar(300),recon_filefrom varchar(300),recon_file_infopath varchar(300),"
                                   "relative_db_path varchar(30) ,source_count_name varchar(30) , "
                                   "item1 varchar(30) ,  item2 varchar(30) ,  item3 varchar(30),  "
                                   "item4 varchar(30),  item5 varchar(30),  item6 varchar(30) ,  "
                                   "item7 varchar(30),  item8 varchar(30),  item9 varchar(30),  item10 varchar(30))";

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_reg_view,registry_viewer_tmp_db_path,Q_FUNC_INFO);
    }

    QString plugin_hidden_columns_db_path = conf_manage_columns_dir + "manage_columns.sqlite";
    if(!QFile::exists(plugin_hidden_columns_db_path))
    {
        QString command_plugin_hidden_columns = ("create table hidden_columns (plugin_name varchar(300), tab_name varchar(100), hidden_columns_nos varchar(100), hidden_column_name varchar(512) , os_scheme_display varchar(512) , file_type_category varchar(512))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_plugin_hidden_columns,plugin_hidden_columns_db_path,Q_FUNC_INFO);
    }



    QString tag_search_index_db_path = tag_search_dir_path + "index_tag_search.sqlite";
    if(!QFile::exists(tag_search_index_db_path))
    {
        QString command_tag_records = ("create table tag_search_index (INT INTEGER PRIMARY KEY,tag_name varchar(300),tag_db_name varchar(300),creation_timestamp INTEGER)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_tag_records,tag_search_index_db_path,Q_FUNC_INFO);
    }

    //=================================//
    QString redefined_result_db_path = path_redf_result + "index_redefined_result.sqlite";
    if(!QFile::exists(redefined_result_db_path))
    {
        QString command_redf_result = "create table 'redefined_result_index' (INT INTEGER PRIMARY KEY, display_name varchar(1000), db_name varchar(1000), creation_timestamp INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_redf_result,redefined_result_db_path,Q_FUNC_INFO);
    }

    create_redefined_result_db_table_and_insert_into_index_table();
    //=================================//


    QString redefined_saved_msg_path = redf_result_msg_saved_path + "index_messenger_saved.sqlite";
    if(!QFile::exists(redefined_saved_msg_path))
    {
        QString command_redf_result_msg = "create table saved_index(INT INTEGER PRIMARY KEY,db_name varchar(30),display_name varchar(30))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_redf_result_msg,redefined_saved_msg_path,Q_FUNC_INFO);
    }

    QString redefined_saved_browsers_path = redf_result_browser_saved_path + "index_browsers_history_saved.sqlite";
    if(!QFile::exists(redefined_saved_browsers_path))
    {
        QString command_redf_result_msg = "create table saved_index(INT INTEGER PRIMARY KEY,db_name varchar(30),display_name varchar(30))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_redf_result_msg,redefined_saved_browsers_path,Q_FUNC_INFO);
    }

    QString activity_logs_db_path = featr_parent_path +  QString("activity_logs/activity_logs.sqlite");
    if(!QFile::exists(activity_logs_db_path))
    {
        QString command_task ="CREATE TABLE activity_logs_entry (INT INTEGER PRIMARY KEY,category VARCHAR(100),task VARCHAR(100),added_timestamp INTEGER,start_timestamp INTEGER,end_timestamp INTEGER,time_taken INTEGER,status VARCHAR(100),task_comments VARCHAR(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_task,activity_logs_db_path,Q_FUNC_INFO);
    }


    QString bookmarkdb_path = bookmarks_search_dir_path + QString("bookmarks.sqlite");
    if(!QFile::exists(bookmarkdb_path))
    {
        QString command_bookmark = ("create table tags (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                    "record_no varchar(300),notes_icon varchar(300),plugin_name varchar(300), tab_name varchar(100),csv_plugin_name varchar(100),csv_tab_name varchar(100),"
                                    "table_name varchar(200),recon_tag_value varchar(100),notes varchar(100),os_scheme_display varchar(100), source_count_name varchar(100), feature_db_path varchar(100),"
                                    "item0 varchar(300),item1 varchar(300),item2 varchar(300),item3 varchar(300),item4 varchar(300),"
                                    "item5 varchar(300),item6 varchar(300),item7 varchar(300),item8 varchar(300),"
                                    "item9 varchar(300),item10 varchar(300),item11 varchar(300),item12 varchar(300),"
                                    "item13 varchar(300),item14 varchar(300),item15 varchar(300),item16 varchar(300),"
                                    "item17 varchar(300),item18 varchar(300),item19 varchar(300))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_bookmark,bookmarkdb_path,Q_FUNC_INFO);

        recon_helper_standard_obj->create_db_index_by_db_path("tag_int", "INT", "tags", bookmarkdb_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("tag_record", "record_no", "tags", bookmarkdb_path, Q_FUNC_INFO);

    }


    QString all_notes_db_path = notes_search_dir_path + "all_notes.sqlite";
    if(!QFile::exists(all_notes_db_path))
    {
        QString command_notes_search = ("create table notes (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                        "record_no varchar(300),plugin_name varchar(300), tab_name varchar(100),"
                                        "table_name varchar(200),recon_tag_value varchar(100),notes varchar(100),os_scheme_display varchar(100),source_count_name varchar(100),"
                                        "item0 varchar(300),item1 varchar(300),item2 varchar(300),item3 varchar(300),item4 varchar(300),"
                                        "item5 varchar(300),item6 varchar(300),item7 varchar(300),item8 varchar(300),"
                                        "item9 varchar(300),item10 varchar(300),item11 varchar(300),item12 varchar(300),"
                                        "item13 varchar(300),item14 varchar(300),item15 varchar(300),item16 varchar(300),"
                                        "item17 varchar(300),item18 varchar(300),item19 varchar(300))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_notes_search,all_notes_db_path,Q_FUNC_INFO);

        recon_helper_standard_obj->create_db_index_by_db_path("note_int", "INT", "notes", all_notes_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("note_rec", "record_no", "notes", all_notes_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("note_cat", "category", "notes", all_notes_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("note_tab", "tab_name", "notes", all_notes_db_path, Q_FUNC_INFO);
        recon_helper_standard_obj->create_db_index_by_db_path("note_not", "notes", "notes", all_notes_db_path, Q_FUNC_INFO);

    }


    QString screenshots_db_path = recon_screenshots_dir_path + "screenshots.sqlite";
    if(!QFile::exists(screenshots_db_path))
    {
        QString command_screenshot = ("create table tags (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                      "record_no varchar(300),notes_icon varchar(300),plugin_name varchar(300), tab_name varchar(100),"
                                      "table_name varchar(200),recon_tag_value varchar(100),notes varchar(100),os_scheme_display varchar(100), source_count_name varchar(100),"
                                      "recon_filefrom varchar(300),recon_file_infopath varchar(300),"
                                      "item0 varchar(300),item1 varchar(300),item2 varchar(300),item3 varchar(300),item4 varchar(300),"
                                      "item5 varchar(300),item6 varchar(300),item7 varchar(300),item8 varchar(300),"
                                      "item9 varchar(300),item10 varchar(300),item11 varchar(300),item12 varchar(300),"
                                      "item13 varchar(300),item14 varchar(300),item15 varchar(300),item16 varchar(300),"
                                      "item17 varchar(300),item18 varchar(300),item19 varchar(300),source_file varchar(300) )");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_screenshot,screenshots_db_path,Q_FUNC_INFO);

        // For Screenshot Only
        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , screenshots_db_path, Q_FUNC_INFO);
    }


    QString storyboard_db_path = story_board_dir_path + "story_board.sqlite";
    if(!QFile::exists(storyboard_db_path))
    {
        QString command_stroy_board = ("create table story_board (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                       "filename varchar(300),filepath varchar(300), display_name varchar(300), recon_filefrom varchar(10),recon_file_infopath varchar(500))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_stroy_board,storyboard_db_path,Q_FUNC_INFO);
    }

    QString saved_ram_analysis_db_path = ram_analysis_dir_path + "ram_analysis_saved.sqlite";
    if(!QFile::exists(saved_ram_analysis_db_path))
    {
        QString command = "create table tbl_saved_result(INT INTEGER PRIMARY KEY,plugin_name varchar(30),tab_name varchar(30), source_name varchar(30),operating_system, build_version varchar(30), artifacts varchar(30),command_output varchar(30),"
                          "result_name varchar(100), os_scheme_display varchar(100), bookmark varchar(2), source_count_name varchar(100), recon_tag_value varchar(100), notes varchar(100),source_file varchar(100),notes_icon varchar(100),blank_column varchar(100))";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,saved_ram_analysis_db_path,Q_FUNC_INFO);
    }


    QString graph_result_db_path = saved_graph_index_dir_path + "saved_graph_result.sqlite";
    if(!QFile::exists(graph_result_db_path))
    {
        QString command_graph_result = "create table 'graph_result_index' (INT INTEGER PRIMARY KEY, display_name varchar(1000), db_name varchar(1000), creation_timestamp INTEGER)";
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_graph_result,graph_result_db_path,Q_FUNC_INFO);

        QString command_insert_index = "INSERT INTO graph_result_index (display_name, db_name, creation_timestamp) VALUES (?,?,?)";
        QStringList category_name ;
        QStringList index_arg_list;

        category_name << MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs << MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs;

        for(int i = 0 ; i < category_name.size() ; i++)
        {
            QString tmp_category_str = category_name.at(i);
            tmp_category_str.replace(" ","_");

            index_arg_list.clear();
            index_arg_list << category_name.at(i) << tmp_category_str << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch());

            if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert_index,index_arg_list,graph_result_db_path,Q_FUNC_INFO))
            {
                recon_static_functions::app_debug("query Insert Into INDEX  ---FAILED---" ,Q_FUNC_INFO);
            }
        }
    }

    QString saved_graph_db_path_browser = saved_graph_browser_dir_path + "Browser_History.sqlite";
    if(!QFile::exists(saved_graph_db_path_browser))
    {
        QString command_saved_graphs = ("create table saved_graphs (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                        "notes_icon varchar(300),plugin_name varchar(300),tab_name varchar(30),"
                                        "graph_path varchar(300),recon_filefrom varchar(300) ,recon_file_infopath varchar(300),"
                                        "graph_name varchar(300),timestamp INTEGER,"
                                        "recon_tag_value varchar(100),notes varchar(100),source_count_name varchar(100),source_file varchar(100),LINKFILENAME VARCHAR(1000),blank_column varchar(300))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_saved_graphs,saved_graph_db_path_browser,Q_FUNC_INFO);

        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , saved_graph_db_path_browser, Q_FUNC_INFO);
    }


    QString saved_graph_db_path_messenger = saved_graph_messenger_dir_path + "Messenger.sqlite";
    if(!QFile::exists(saved_graph_db_path_messenger))
    {
        QString command_saved_graphs = ("create table saved_graphs (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                        "notes_icon varchar(300),plugin_name varchar(300),tab_name varchar(30),"
                                        "graph_path varchar(300),recon_filefrom varchar(300) ,recon_file_infopath varchar(300),"
                                        "graph_name varchar(300),timestamp INTEGER,"
                                        "recon_tag_value varchar(100),notes varchar(100),source_count_name varchar(100),source_file varchar(100),LINKFILENAME VARCHAR(1000) ,blank_column varchar(300))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_saved_graphs,saved_graph_db_path_messenger,Q_FUNC_INFO);

        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , saved_graph_db_path_messenger, Q_FUNC_INFO);
    }

    QString file_index_queue = path_filesystem_dir + "file_indexing_queue.sqlite";
    if(!QFile::exists(file_index_queue))
    {
        QString command_que = QString("create table files (bookmark INTEGER,INT INTEGER PRIMARY KEY, fs_record INTEGER,file_path varchar(500),source_count_name varchar(100),status INTEGER,plugin_name varchar(100))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_que,file_index_queue,Q_FUNC_INFO);
    }

    QString cropped_faces_queue = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Faces_Group_In_Result_QString).toString() + "face_info.sqlite";
    if(!QFile::exists(cropped_faces_queue))
    {
        QString command_que = QString("create table distinct_faces (INT INTEGER PRIMARY KEY, fs_record INTEGER, source_count_name varchar(100),file_path varchar(500), face_id INTEGER)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_que,cropped_faces_queue,Q_FUNC_INFO);

        QString command_que2 = QString("create table all_faces (INT INTEGER PRIMARY KEY, fs_record INTEGER,source_count_name varchar(100),face_id INTEGER)");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_que2,cropped_faces_queue,Q_FUNC_INFO);

    }

    QString filesystem_module_status_path = path_filesystem_dir + "case_fs_module_status.sqlite";
    if(!QFile::exists(filesystem_module_status_path))
    {
        QString command = QString("create table tbl_fs_module_status(INT INTEGER PRIMARY KEY, module_job_type varchar(100), run_status varchar(10))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command,filesystem_module_status_path,Q_FUNC_INFO);
    }

    QString z_roots_count = conf_source + "z_roots_count.sqlite";
    if(!QFile::exists(z_roots_count))
    {
        QString command_z_count = QString("create table z_count(INT INTEGER PRIMARY KEY, z_root_count_flag varchar(100), z_root_count INTERGER , z_active_status varchar(100))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_z_count,z_roots_count,Q_FUNC_INFO);
    }


    QString root_info = conf_source + "roots_info.sqlite";
    if(!QFile::exists(root_info))
    {

        QString command_root = QString("create table tbl_root_index(INT INTEGER PRIMARY KEY, root_count_name varchar(100),"
                                       "evidence_type varchar(100), root_type varchar(100), source_type_display varchar(100),"
                                       "source_type_internal varchar(100), root_name varchar(100), all_source_count_name varchar(100),"
                                       "prime_source_count_name varchar(100),nested_source_count_name varchar(100),root_path_1 varchar(100),"
                                       "root_path_2 Varchar(100), root_recovery_passcode varchar(100), root_evidence_password varchar(100), root_disk_identifier_1 varchar(100),"
                                       "root_disk_node_1 varchar(100), root_disk_identifier_2 varchar(100),root_disk_node_2 varchar(100),"
                                       "root_md5_hash varchar(100), root_sha1_hash varchar(100), home_directory_user_name varchar(100),"
                                       "evidence_number varchar(100) , description varchar(1000))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_root,root_info,Q_FUNC_INFO);
    }


    QString z_sources_count = conf_source + "z_sources_count.sqlite";
    if(!QFile::exists(z_sources_count))
    {
        QString command_z_count = QString("create table z_count(INT INTEGER PRIMARY KEY, z_source_count_flag varchar(100), z_source_count INTERGER , z_active_status varchar(100))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_z_count,z_sources_count,Q_FUNC_INFO);
    }

    QString src_info = conf_source + "sources_info.sqlite";
    if(!QFile::exists(src_info))
    {
        /*Note: used columns for future use
         * item0 - is used for disk node (like disk2s2) (version - 1.1.0)
         */

        QString command_fs_status = "create table fs_status(INT INTEGER PRIMARY KEY,root_count_name varchar(100),source_count_name varchar(100), os_scheme_display varchar(100),os_scheme_internal varchar(100),"
                +  QString(MACRO_JobType_Apple_Metadata).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Exif_Metadata).replace(" ", "_")  + " INTEGER, "
                + QString(MACRO_JobType_Signature_Analysis).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Mime_Type).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Hashes).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Text_Indexing).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Face_Analysis).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Optical_Character_Recognition).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Skin_Tone_Detection).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Weapons).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Fire_Analysis).replace(" ", "_") + " INTEGER, "
                + QString(MACRO_JobType_Unified_Logs).replace(" ", "_") + " INTEGER, "
                + "feature_x1 INTEGER, feature_x2 INTEGER, feature_x3 INTEGER, feature_x4 INTEGER, feature_x5 INTEGER)";

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_fs_status,src_info,Q_FUNC_INFO);


        QString command_tbl_source_index = "create table tbl_source_index(INT INTEGER PRIMARY KEY,root_count_name varchar(100),"
                                           "source_count_name varchar(100),source_disk_identifier varchar(500), source_hierarchy varchar(500),"
                                           "is_source_nested varchar(10),is_source_decompressed varchar(10),source_type_display varchar(100),"
                                           "source_type_internal varchar(100), source_evidence_password varchar(100), os_scheme_display varchar(100),os_scheme_internal varchar(100),role_type varchar(100),"
                                           "volume_offset INTEGER,volume_uuid varchar(100), users_list varchar(100),"
                                           "volume_type varchar(1024), total_size_byte_numeric varchar(1024), free_size_byte_numeric varchar(1024),"
                                           "used_size_byte_numeric varchar(1024),file_system_type varchar(1024), source_main_path varchar(100),"
                                           "source_main_name varchar(1024), source_name varchar(100), source_path_1 varchar(1000),"
                                           "source_path_2_for_fusion varchar(1000), virtual_source_path varchar(100), product_type varchar(100),"
                                           "product_name varchar(100), product_model varhar(100), display_name varchar(100), device_name VARCHAR(100),"
                                           "os_version_extended varchar(100), os_version_full varchar(100),"
                                           "lattitude varchar(100), longitude varchar(100),country varchar(100),city varchar(100),"
                                           "configured_id varchar(100), id_no varchar(100),guid varchar(100), target_type varchar(100),"
                                           "build_version varchar(100),itune_version VARCHAR(100), serial_no varchar(100),"
                                           "phone_no_msisdn varchar(100),imei varchar(100),sim_no_iccid varchar(100), "
                                           "latest_backup_date_readable varchar(100), latest_backup_date_numeric INTEGER,"
                                           "backup_size_byte_numeric varchar(100),timezone_readable varchar(100),timezone_numeric varchar(100),"
                                           "recon_logical_image varchar(100),bluetooth_device_address varchar(100),"
                                           "wifi_address varchar(100),locale_language varchar(100), installer_timestamp_utc_numeric varchar(100))";


        recon_helper_standard_obj->execute_db_command_by_dbpath(command_tbl_source_index,src_info,Q_FUNC_INFO);
    }

    create_apfs_snapshots_table_in_sources_db(src_info);



    QString chat_viewer_sqlite_path = chat_viewer_dir_path + "bubble_chat_bookmarks.sqlite";
    if(!QFile::exists(chat_viewer_sqlite_path))
    {
        QString cmd_create_table = QString("create table chat_viewer_info(INT INTEGER PRIMARY KEY, bookmark varchar(100),plugin_name varchar(100),tab_name varchar(100), file_name varchar(100) , configured_account varchar(100) , chat_buddy varchar(100))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(cmd_create_table,chat_viewer_sqlite_path,Q_FUNC_INFO);
    }


    QString mbl_bk_db_path = path_ios_bckp + "mobile_backup.sqlite";
    if(!QFile::exists(mbl_bk_db_path))
    {
        QString command_1 = QString("create table tbl_backup_list"
                                    "(bookmark INTEGER,INT INTEGER PRIMARY KEY,plugin_name varchar(100),tab_name varchar(100),source_type_display varchar(100),source_type_internal varchar(100),source_name varchar(100),source_path varchar(500),source_size INTEGER,"
                                    "root_count_name varchar(100), source_count_name varchar(100),extraction_status_as_source INTEGER,"
                                    "notes_icon varchar(1000),notes varchar(1000),recon_tag_value varchar(1000), is_nested varchar(10),is_decompressed varchar(10),recon_filefrom varchar(10),"
                                    "recon_file_infopath varchar(500),blank_column varchar(10),"
                                    "device_name varchar(500),device_id varchar(100),imei varchar(500),last_backup_date INTEGER,"
                                    "phone_serial_number varchar(500),phone_number varchar(500),product_type varchar(500),product_version varchar(500),itunes_version varchar(500),"
                                    "build_version varchar(100),display_name varchar(100),guid varchar(100),"
                                    "product_name varchar(100),target_type varchar(100),iccid varchar(100))");
        recon_helper_standard_obj->execute_db_command_by_dbpath(command_1,mbl_bk_db_path,Q_FUNC_INFO);
    }

    QString dsk_img_srcs_db_path = path_dsk_img_srcs + "disk_images.sqlite";
    if(!QFile::exists(dsk_img_srcs_db_path))
    {
        QString command_1 = QString("create table IF NOT EXISTS tbl_sources_list"
                                    "(bookmark INTEGER,INT INTEGER PRIMARY KEY,plugin_name varchar(100),tab_name varchar(100),source_type_display varchar(100),source_type_internal varchar(100),source_name varchar(100),source_path varchar(500),source_extension varchar(500),source_size INTEGER,"
                                    "root_count_name varchar(100), source_count_name varchar(100),extraction_status_as_source INTEGER,"
                                    "notes_icon varchar(1000),notes varchar(1000),recon_tag_value varchar(1000), is_nested varchar(10),is_decompressed varchar(10),recon_filefrom varchar(10),"
                                    "recon_file_infopath varchar(500),blank_column varchar(10))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_1,dsk_img_srcs_db_path,Q_FUNC_INFO);
    }

    QString ram_img_srcs_db_path = path_ram_img_srcs + "ram_images.sqlite";
    if(!QFile::exists(ram_img_srcs_db_path))
    {
        QString command_1 = QString("create table IF NOT EXISTS tbl_ram_list"
                                    "(bookmark INTEGER,INT INTEGER PRIMARY KEY,plugin_name varchar(100),tab_name varchar(100),source_type_display varchar(100),source_type_internal varchar(100),source_name varchar(100),source_path varchar(500),source_size INTEGER,"
                                    "root_count_name varchar(100), source_count_name varchar(100),extraction_status_as_source INTEGER,"
                                    "notes_icon varchar(1000),notes varchar(1000),recon_tag_value varchar(1000), is_nested varchar(10),is_decompressed varchar(10),recon_filefrom varchar(10),"
                                    "recon_file_infopath varchar(500),blank_column varchar(10))");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command_1,ram_img_srcs_db_path,Q_FUNC_INFO);
    }

    QString snapshots_diff_db_path = snapshots_difference_dir_path + "saved_snapshots_index.sqlite";
    if(!QFile::exists(snapshots_diff_db_path))
    {
        QString tm_cmd = QString("create table IF NOT EXISTS tbl_saved_index"
                                 "(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                                 "snapshot_lable_name varchar(100),primary_source varchar(100),"
                                 "secondary_source varchar(100),"
                                 "source_count_name varchar(100))");


        recon_helper_standard_obj->execute_db_command_by_dbpath(tm_cmd,snapshots_diff_db_path,Q_FUNC_INFO);
    }

    QString disk_hex_view_unalloc_db_path = path_disk_hex_view_dir + "disk_hex_view_unallocated.sqlite";
    if(!QFile::exists(disk_hex_view_unalloc_db_path))
    {
        QString tm_cmd = QString("create table IF NOT EXISTS tbl_saved_index"
                                 "(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                                 "saved_table_name varchar(100),is_complete_source varchar(10),extraction_status varchar(10),"
                                 "source_count_name varchar(100))");


        recon_helper_standard_obj->execute_db_command_by_dbpath(tm_cmd,disk_hex_view_unalloc_db_path,Q_FUNC_INFO);
    }

    QString hashset_dir_path     = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Hashset_In_Result_QString).toString();
    QString hashes_index_db_path = hashset_dir_path + "hashset_index.sqlite";;

    QString command_str = QString("create table IF NOT EXISTS hashset_indexes"
                                  "(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                                  "saved_db_name varchar(500),saved_db_path varchar(500),saved_table_name varchar(500),"
                                  "md5_hash_column varchar(100), sha1_hash_column varchar(100),"
                                  "show_files varchar(10),hide_files varchar(10), was_imported_external varchar(10), records_count varchar(500),"
                                  "recon_filefrom varchar(10),recon_file_infopath varchar(10))");

    recon_helper_standard_obj->execute_db_command_by_dbpath(command_str, hashes_index_db_path, Q_FUNC_INFO);




    QString andrd_db_path = extrct_contnt_andrd_bk + "android_backup_index.sqlite";
    if(!QFile::exists(andrd_db_path))
    {
        QString tm_cmd = QString("create table IF NOT EXISTS tbl_index"
                                 "(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                                 "source_count_name varchar(100), recon_filefrom varchar(10),recon_file_infopath varchar(10))");


        recon_helper_standard_obj->execute_db_command_by_dbpath(tm_cmd,andrd_db_path,Q_FUNC_INFO);
    }


    QString ios_backup_index_db_path = extrct_contnt_ios_bkup + "ios_backup_index.sqlite";
    if(!QFile::exists(ios_backup_index_db_path))
    {
        QString tm_cmd = QString("create table IF NOT EXISTS tbl_index"
                                 "(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                                 "source_count_name varchar(100), recon_filefrom varchar(10),recon_file_infopath varchar(10))");


        recon_helper_standard_obj->execute_db_command_by_dbpath(tm_cmd,ios_backup_index_db_path,Q_FUNC_INFO);
    }

    QString email_db_path = email_dir_path + "email_data.sqlite";
    if(!QFile::exists(email_db_path))
    {
        QString command = "create table if not exists emlx_data (bookmark INTEGER,INT INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "email_type VARCHAR(255),"
                          "emlx_subject varchar(255),"
                          "emlx_to varchar(1000),"
                          "emlx_from varchar(255),"
                          "emlx_date INTEGER,"
                          "emlx_cc varchar(1000),"
                          "emlx_msg_body text,"
                          "message_text text,"
                          "email_attachment varchar(255),"
                          "account_id varchar(255) , "
                          "mbox varchar(255),"
                          "file_raw_bytes BLOB,"
                          "file_name varchar(1024),"

                          "bookmarked_extended_attributes VARCHAR(1024),"
                          "bookmarked_exif_data VARCHAR(1024),"
                          "extended_attributes_key_value VARCHAR(1024),"
                          "extended_attributes_value_for_search VARCHAR(1024),"

                          "LINKFILENAME VARCHAR(1024),"

                          "preview_file_path varchar(1024),"
                          "export_file_paths varchar(1024),"

                          "recon_tag_value VARCHAR(1024),"
                          "notes_icon VARCHAR(1024),"
                          "tags_icon VARCHAR(1024),"
                          "os_scheme_display VARCHAR(1024),"
                          "source_count_name VARCHAR(1024),"
                          "source_file VARCHAR(1024),"
                          "notes VARCHAR(1024))";

        recon_helper_standard_obj->execute_db_command_by_dbpath(command,email_db_path,Q_FUNC_INFO);

        command = QString("create table if not exists exportfilelist"
                          "(INT INTEGER PRIMARY KEY,"
                          "record VARCHAR(150),"
                          "table_name VARCHAR(1000),"
                          "filepath VARCHAR(1000),"

                          "exp_newpath VARCHAR(1000),"
                          "exp_relativepath VARCHAR(1000),"
                          "exp_size INTEGER,"
                          "exp_hashmd5 VARCHAR(1000),"
                          "exp_hashsha1 VARCHAR(1000),"
                          "exp_LINKFILENAME varchar(500),"

                          "exp_time INTEGER)");

        recon_helper_standard_obj->execute_db_command_by_dbpath(command,email_db_path,Q_FUNC_INFO);

    }

    QString dest_atchmnts_location_emlx_parse = email_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments;
    dir.setPath(dest_atchmnts_location_emlx_parse);
    dir.mkpath(dest_atchmnts_location_emlx_parse);

    QString index_unified_logs_db_path = path_unified_log_viewer + "index_unified_logs.sqlite";
    if(!QFile::exists(index_unified_logs_db_path))
    {
        QString tm_cmd = QString("CREATE table IF NOT EXISTS tbl_index"
                                 "(INT INTEGER PRIMARY KEY,"
                                 "source_count_name varchar(100), db_name varchar(100))");


        recon_helper_standard_obj->execute_db_command_by_dbpath(tm_cmd,index_unified_logs_db_path,Q_FUNC_INFO);
    }

    create_recon_case_result_file(recon_rslt_path);

    recon_static_functions::app_debug(" - Ends",Q_FUNC_INFO);
}

void MainWindow::add_and_update_tags_in_database_for_ram_analysis_saved(QString tag_name, QString is_emitted_from_plugin, QString plugin_name_str, QString tab_name_str, QString record_no, QString dest_path, QString os_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(dest_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }
    QStringList arg_list;
    QSqlQuery query(source_db);
    query.prepare("Select INT,bookmark,recon_tag_value,notes,result_name,source_name,operating_system,build_version,artifacts,command_output from tbl_saved_result where INT = ?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select -------FAILED------- " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }
    if(!query.next())
    {
        recon_static_functions::app_debug(" --- No Record Exist---",Q_FUNC_INFO);
        return;
    }

    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name_str << tab_name_str << record_no << os_name;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QString command_insert = "INSERT INTO tags(record_no,bookmark,recon_tag_value,notes,"
                                 "item0,item1,item2,item3,item4,item5,plugin_name,category,tab_name,"
                                 "csv_plugin_name,csv_tab_name , os_scheme_display)"
                                 "VALUES (?,?,?,?, ?,?,?,?, ?,?,?,?,?,?,?,?)";

        ///record no , bookmark , tag , notes , result name , source name , operating system
        /// build version , artifacts , command output
        arg_list << query.value(0).toString()
                 << query.value(1).toString()
                 << query.value(2).toString()
                 << query.value(3).toString()
                 << query.value(4).toString()
                 << query.value(5).toString()
                 << query.value(6).toString()
                 << query.value(7).toString()
                 << query.value(8).toString()
                 << query.value(9).toString()
                 << plugin_name_str
                 << plugin_name_str
                 << tab_name_str
                 << plugin_name_str
                 << tab_name_str
                 << os_name;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tag_db_path,Q_FUNC_INFO);
    }
    else
    {
        QString command_update = "UPDATE tags SET recon_tag_value = ? WHERE record_no=?";
        QStringList arg_list;
        arg_list << tag_name << record_no;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,tag_db_path,Q_FUNC_INFO);

    }

    source_db.close();
    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}


void MainWindow::insert_record_entry_into_bookmarks_db_for_ram_analysis_saved(QString record_no, QString plugin_name, QString tab_name,QString status, QString bookmark_db_path, QString dest_db_path, QString os_name_str, QString tag_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(dest_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("Select INT,bookmark,recon_tag_value,notes,result_name,source_name,operating_system,build_version,artifacts,command_output from tbl_saved_result where INT = ?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query select ---FAILED--- " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("query Error " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(!query.next())
    {
        recon_static_functions::app_debug("--- No Record Exist---",Q_FUNC_INFO);
        source_db.close();
        return;
    }

    QStringList arg_list;
    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND tab_name=? AND record_no=?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,bookmark_db_path,Q_FUNC_INFO);
    if(INT_str.isEmpty())
    {
        QString command_insert = "INSERT INTO tags(record_no,bookmark,recon_tag_value,notes,"
                                 "item0,item1,item2,item3,item4,item5,plugin_name,category,tab_name,"
                                 "csv_plugin_name,csv_tab_name,os_scheme_display)"
                                 "VALUES (?,?,?,?, ?,?,?,?, ?,?,?,?,?,?,?,?)";

        ///record no , bookmark , tag , notes , result name , source name , operating system
        /// build version , artifacts , command output

        arg_list << query.value(0).toString()
                 << query.value(1).toString()
                 << query.value(2).toString()
                 << query.value(3).toString()
                 << query.value(4).toString()
                 << query.value(5).toString()
                 << query.value(6).toString()
                 << query.value(7).toString()
                 << query.value(8).toString()
                 << query.value(9).toString()
                 << plugin_name
                 << plugin_name
                 << tab_name
                 << plugin_name
                 << tab_name
                 << os_name_str;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,bookmark_db_path,Q_FUNC_INFO);
    }
    else
    {

        QString command_update = "UPDATE tags SET bookmark=?  WHERE record_no=? AND plugin_name=? AND tab_name=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << status << record_no << plugin_name << tab_name << os_name_str;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,bookmark_db_path,Q_FUNC_INFO);
    }
    source_db.close();

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);

}

void MainWindow::insert_case_info_in_db(QSqlDatabase db)
{
    QString command = "insert into agency_details (agency_name, agency_address, examiner, examiner_phone, examiner_email, date_format) values (?,?,?,?,?,?)";
    QStringList list;
    list << "" << "" << "" << "" << "" << "1";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,db,Q_FUNC_INFO);

}

void MainWindow::insert_exif_attr_in_db(QSqlDatabase db)
{

    // In future we may use csv file like apple metadata


    QStringList exf_prnt_list;
    exf_prnt_list << "Make/Model" << "Make/Model" << "Author" << "GPS" << "GPS";


    QStringList exf_desc_list;
    exf_desc_list << "Parse make and model of file" << "Parse make and model of file" << "Parse author of file" << "Parse GPS of file" << "Parse GPS of file";

    QStringList exf_title_list;
    exf_title_list << "Make of File" << "Model of File" << "Author of File" << "Latitude of File" << "Longitude of File";

    QStringList exf_key_list;
    exf_key_list << "Make" << "Model" << "Author" << "Latitude" << "Longitude";

    QStringList exf_custom_col_list;
    exf_custom_col_list << "make" << "model" << "author" << "latitude" << "longitude";



    for(int ii = 0; ii < exf_prnt_list.size(); ii++)
    {
        QString command = "insert into tbl_exif_attributes (bookmark_filter, bookmark_metadata, bookmark_report, is_default,parent_name,description,attribute_title,attribute_key,custom_column_name) values (?,?,?,?,?, ?,?,?,?)";
        QStringList list;
        list << "1" << "1" << "1" << "1" << exf_prnt_list.at(ii) << exf_desc_list.at(ii)<< exf_title_list.at(ii) << exf_key_list.at(ii) << exf_custom_col_list.at(ii);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,db,Q_FUNC_INFO);
    }

}

void MainWindow::insert_index_files_filter_in_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);


    QString white_category_docs = "Documents";

    QStringList white_category_list;
    white_category_list << (white_category_docs);


    for(int i = 0; i < white_category_list.size(); i++)
    {
        QString command = "insert into tbl_index_files_whitelist_category(bookmark,category) values(?,?)";
        QStringList list;
        list << QString("1") << white_category_list.at(i);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }


    QStringList white_docs_ext_list;
    white_docs_ext_list     << ".doc" << ".docx";
    white_docs_ext_list     << ".html";
    white_docs_ext_list     << ".odt" << ".ods" << ".ots" << ".pdf";
    white_docs_ext_list     << ".rtf" << ".txt";
    white_docs_ext_list     << ".xls" << ".xlxs" << ".xml";

    white_docs_ext_list.sort();

    for(int i = 0; i < white_docs_ext_list.size(); i++)
    {
        QString extension_type = white_docs_ext_list.at(i);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into tbl_index_files_whitelist_extension(bookmark,extension, category) values(?,?,?)";
        QStringList list;
        list << QString("1")  << white_docs_ext_list.at(i) << QString(white_category_docs);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    //====================================================//

    QString black_category_vdisk = "Virtual Disk";
    QString black_category_apl_disk_image = "Apple Disk Images";

    QStringList black_category_list;
    black_category_list << (black_category_vdisk)
                        << black_category_apl_disk_image;

    for(int i = 0; i < black_category_list.size(); i++)
    {
        QString command = "insert into tbl_index_files_blacklist_category(bookmark,category) values(?,?)";
        QStringList list;
        list << QString("1") << black_category_list.at(i);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    QStringList black_vdisk_ext_list,black_appl_disk_ext_list;
    black_vdisk_ext_list     << ".vdi"  << ".vmdk" << ".vhd" << ".iso";
    black_appl_disk_ext_list     << ".dmg"  << ".sparseimage";
    black_vdisk_ext_list.sort();
    black_appl_disk_ext_list.sort();
    for(int i = 0; i < black_vdisk_ext_list.size(); i++)
    {
        QString extension_type = black_vdisk_ext_list.at(i);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into tbl_index_files_blacklist_extension(bookmark,extension, category) values(?,?,?)";
        QStringList list;
        list << QString("1")  << black_vdisk_ext_list.at(i) << QString(black_category_vdisk);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }
    for(int i = 0; i < black_appl_disk_ext_list.size(); i++)
    {
        QString extension_type = black_appl_disk_ext_list.at(i);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into tbl_index_files_blacklist_extension(bookmark,extension, category) values(?,?,?)";
        QStringList list;
        list << QString("1")  << black_appl_disk_ext_list.at(i) << QString(black_category_apl_disk_image);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }


    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::insert_default_values_in_features_resources_db(QSqlDatabase feature_config_db)
{

    insert_file_signature_info_in_feature_resources_db(feature_config_db);
    QCoreApplication::processEvents();

    insert_colours_for_tags_details_in_feature_resources_db(feature_config_db);
    QCoreApplication::processEvents();

}

void MainWindow::insert_preferences_detail_in_db(QSqlDatabase feature_config_db)
{

    QString command = "delete from tbl_preferences";
    recon_helper_standard_obj->execute_db_command_by_dbreference(command, feature_config_db, Q_FUNC_INFO);

    command = "insert into tbl_preferences (key, value) values (?,?)";
    QStringList args_list;

    args_list << QString(MACRO_Result_Preferences_Key_Highlight_User_Opened_Files) << QString(MACRO_FALSE_VALUE_READABLE_QString);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);


    args_list.clear();
    args_list << QString(MACRO_Result_Preferences_Key_Minimum_File_Size_Skin_Tone_Detection_in_KB) << QString("02");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);

    args_list.clear();
    args_list << QString(MACRO_Result_Preferences_Key_Minimum_File_Size_Face_Analysis_in_KB) << QString("02");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);

    args_list.clear();
    args_list << QString(MACRO_Result_Preferences_Key_Minimum_File_Size_Optical_Character_Recognition_in_KB) << QString("02");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);

    args_list.clear();
    args_list << QString(MACRO_Result_Preferences_Key_Minimum_File_Size_Weapons_in_KB) << QString("02");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);

    args_list.clear();
    args_list << QString(MACRO_Result_Preferences_Key_Minimum_File_Size_Fire_Analysis_in_KB) << QString("02");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);

    args_list.clear();
    args_list << QString(MACRO_Result_Preferences_Key_Maximum_File_Size_Text_View_in_MB) << QString("20");
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);

}


void MainWindow::insert_app_config_tbl_preferences_details_in_db(QSqlDatabase feature_config_db)
{
    QString command = "delete from tbl_preferences";
    recon_helper_standard_obj->execute_db_command_by_dbreference(command, feature_config_db, Q_FUNC_INFO);

    command = "insert into tbl_preferences (key, value) values (?,?)";
    QStringList args_list;

    args_list << QString(MACRO_App_Preferences_Key_Enable_Debug_Mode) << QString(MACRO_FALSE_VALUE_READABLE_QString);
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, args_list, feature_config_db, Q_FUNC_INFO);
}

void MainWindow::insert_extensions_in_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QStringList extensions_catagory_list;

    QString category_images = "Image";
    QString category_videos = "Video";
    QString category_audios = "Audio";
    QString category_documents = "Document";
    QString category_torrents = "Torrent";

    QString category_apple_office = "Apple Office";
    QString category_libre_office = "Libre Office";


    extensions_catagory_list << (category_images)
                             << (category_audios)
                             << (category_videos)
                             << (category_documents)
                             << (category_torrents)
                             << (category_apple_office)
                             << (category_libre_office);



    for(int i = 0; i < extensions_catagory_list.size(); i++)
    {
        QString command = "insert into extension_category(bookmark,INT,category_name) values(?,?,?)";
        QStringList list;
        list << QString("1") << QString::number(i+1) << extensions_catagory_list.at(i);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }




    QStringList list_image_extension;
    QStringList list_video_extension;
    QStringList list_audio_extension;
    QStringList list_documents_extension;
    QStringList list_torrents_extension;

    QStringList list_apple_office_extension;
    QStringList list_libre_office_extension;



    list_image_extension     << ".png"  << ".jpg" << ".jpeg" << ".ico" << ".gif";
    list_video_extension     << ".mp4"  << ".wav" << ".mov"  << ".3gp" << ".movie";
    list_audio_extension     << ".mp3"  << ".mp1" << ".mp2"  << ".ogg" << ".wma";
    list_documents_extension << ".docx" << ".doc" << ".txt"  << ".xls";
    list_torrents_extension  << ".torrent" ;

    list_apple_office_extension  << ".pages" ;
    list_libre_office_extension  << ".csv" << ".doc" << ".docx" << ".dotx" << ".fodp" << ".fods" << ".fodt"
                                 << ".odb" << ".odf" << ".odg" << ".odm" << ".odp" << ".ods" << ".otg" << ".otp"
                                 << ".ots" << ".ott" << ".oxt" << ".pdf" << ".pptx" << ".psw" << ".sda" << ".sdc"
                                 << ".sdd" << ".sdp" << ".sdw" << ".slk" << ".smf" << ".stc" << ".std" << ".sti"
                                 << ".stw" << ".sxc" << ".sxg" << ".sxi" << ".sxm" << ".sxw" << ".uof" << ".uop"
                                 << ".uos" << ".uot" << ".vsd" << ".vsdx" << ".wdb" << ".wps" << ".wri" << ".xls"
                                 << ".db";


    for(int i = 0; i < list_audio_extension.size(); i++)
    {
        QString extension_type = list_audio_extension.at(i);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_audio_extension.at(i) << QString(category_audios);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    for(int j = 0; j < list_video_extension.size(); j++)
    {
        QString extension_type = list_video_extension.at(j);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_video_extension.at(j) << QString(category_videos);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    for(int k = 0; k < list_image_extension.size(); k++)
    {
        QString extension_type = list_image_extension.at(k);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_image_extension.at(k) << QString(category_images);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    for(int l = 0; l < list_documents_extension.size(); l++)
    {
        QString extension_type = list_documents_extension.at(l);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_documents_extension.at(l) << QString(category_documents);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    for(int l = 0; l < list_torrents_extension.size(); l++)
    {
        QString extension_type = list_torrents_extension.at(l);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_torrents_extension.at(l) << QString(category_torrents);
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }


    for(int mm = 0; mm < list_apple_office_extension.size(); mm++)
    {
        QString extension_type = list_apple_office_extension.at(mm);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_apple_office_extension.at(mm) << QString(category_apple_office);


        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }

    for(int nn = 0; nn < list_libre_office_extension.size(); nn++)
    {
        QString extension_type = list_libre_office_extension.at(nn);

        if(extension_type.startsWith("."))
            extension_type = extension_type.mid(1 , extension_type.size()).toUpper();

        QString command = "insert into extension_list(bookmark,extension_name, category_name) values(?,?,?)";
        QStringList list;
        list << QString("1")  << list_libre_office_extension.at(nn) << QString(category_libre_office);


        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command,list,destination_db,Q_FUNC_INFO);
    }


    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::insert_colours_for_tags_details_in_feature_resources_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug(" START ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName("../Resources/tags/recon_tags.sqlite");

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_source_hash_set_details(source_db);


    ///for colors table insertion
    if(!query_source_hash_set_details.exec("select color_name,color_hex from colors"))
    {
        recon_static_functions::app_debug(" Source colours  ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_source_hash_set_details.lastError().text(),Q_FUNC_INFO);
        return;
    }
    while(query_source_hash_set_details.next())
    {
        QString command_insert = QString("insert into 'colors' (color_name,color_hex) values(?,?)");

        QStringList list_value;
        list_value << query_source_hash_set_details.value(0).toString() << query_source_hash_set_details.value(1).toString();

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_insert,list_value,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" Insert -----FAILED-----",Q_FUNC_INFO);
        }
    }

    source_db.close();

    recon_static_functions::app_debug(" END ",Q_FUNC_INFO);
}


void MainWindow::insert_plugins_info_in_config_db(QSqlDatabase destination_db, QString csv_dir_path)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    csv_reader *csv_reader_ptr = new csv_reader(this);
    csv_reader_ptr->set_csv_dir_path(csv_dir_path);
    csv_reader_ptr->do_job_plugin_all();

    QList<struct_global_csv_reader_full_csv_info> list_csv_info;
    list_csv_info = csv_reader_ptr->get_csv_info_list();

    int plugin_count = 0;

    for(int itr = 0; itr < list_csv_info.size(); itr++)
    {
        plugin_count++;

        struct_global_csv_reader_full_csv_info plg_obj = list_csv_info.at(itr);

        //-Insert Plugins.
        QString plugin_export_available = "0";
        if(csv_reader_ptr->is_export_available_in_plugin(plg_obj.plugin_name))
            plugin_export_available = "1";

        QString os_supported = csv_reader_ptr->get_supported_os_scheme_list_of_plugin(plg_obj.plugin_name).join(MACRO_RECON_Splitter_5_resolution_scope);

        QString command_plugin = QString("insert into plugins(category,name,export_available,default_selected,supported_os_scheme_display_list) values(?,?,?,?,?)");
        QStringList list_add_bind_value;
        list_add_bind_value << plg_obj.plugin_parent_name << plg_obj.plugin_name << plugin_export_available << "0" << os_supported ;

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_plugin,list_add_bind_value,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" Insert Plugins -----FAILED-----",Q_FUNC_INFO);
        }

        QStringList list_export_tabs = csv_reader_ptr->get_export_tab_list(plg_obj.plugin_name);

        QStringList tabs_list_for_launcher;
        tabs_list_for_launcher << plg_obj.tab_name_QStringList_tablewidget;
        tabs_list_for_launcher << plg_obj.tab_name_QStringList_textedit;
        tabs_list_for_launcher << plg_obj.tab_name_QStringList_stack;

        for(int ii = 0; ii < tabs_list_for_launcher.size(); ii++)
        {
            QStringList tabs_os_suppoted_list = csv_reader_ptr->get_supported_os_scheme_list_of_tab(plg_obj.plugin_name, tabs_list_for_launcher.at(ii));
            QString tab_os_supported_str = tabs_os_suppoted_list.join(MACRO_RECON_Splitter_5_resolution_scope);

            QString command_tabs = QString("insert into plugin_tabs(plugin_no,name,export_available,default_selected,supported_os_scheme_display_list) values(?,?,?,?,?)");

            QString is_export_available = "0";
            if(list_export_tabs.contains(tabs_list_for_launcher.at(ii)))
            {
                is_export_available = "1";
            }

            QStringList list_tab_values;
            list_tab_values << QString::number(plugin_count) << tabs_list_for_launcher.at(ii) << is_export_available << "0" << tab_os_supported_str;

            if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_tabs,list_tab_values,destination_db,Q_FUNC_INFO))
            {
                recon_static_functions::app_debug(" Insert Tabs -----FAILED-----",Q_FUNC_INFO);
            }
        }
    }

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::update_current_timezone_in_recon_configuration(QString db_path)
{
    struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString timezone_current_fetched = strct_exm_obj.timezone_name_complete_QString;

    QString command_update = "update timezone_details set case_created_machine_timezone_qstring = ?";
    QStringList list;
    list << timezone_current_fetched;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,list,db_path,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" update timezone_details ---FAILED---",Q_FUNC_INFO);
    }
}

void MainWindow::insert_timezone_details(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString command_timezone_details = QString("insert into 'timezone_details' (case_created_machine_timezone_qstring,case_created_machine_timezone_qint64,examiner_selected_timezone_qstring,examiner_selected_timezone_qint64,examiner_selected_date_format_int,examiner_selected_date_format_qstring) values(?,?,?,?,?,?)");

    QStringList list_value;

    struct_global_desktop_info strct_exm_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString current_machine_timezone = strct_exm_obj.timezone_name_complete_QString;

    qint64 current_machine_timezone_offset = strct_exm_obj.timezone_offset_numeric_qint64;

    ///Static zero values are because by default UTC is used and UTC has Zero offset value to be added or subtracted.
    list_value << current_machine_timezone << QString::number(current_machine_timezone_offset) << QString(MACRO_User_Selected_Timezone_UTC) << QString("0") << QString("0") << QString("MMM-dd-yyyy") ;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_timezone_details,list_value,destination_db,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" Insert timezone_details -----FAILED-----",Q_FUNC_INFO);
    }

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::insert_file_signature_info_in_feature_resources_db(QSqlDatabase destination_db)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName("../Resources/features_resources/features_configuration.sqlite");
    QCoreApplication::processEvents();

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source_db ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_source_category(source_db);
    if(!query_source_category.exec("select category,category_status from tbl_signature_category"))
    {
        recon_static_functions::app_debug(" signature_category ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_source_category.lastError().text(),Q_FUNC_INFO);
    }
    while(query_source_category.next())
    {
        QString command_signature_category = QString("insert into 'tbl_signature_category' (category,category_status) values(?,?)");

        QStringList list_value;
        list_value << query_source_category.value(0).toString() << query_source_category.value(1).toString();

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_signature_category,list_value,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" Insert signature_category -----FAILED-----",Q_FUNC_INFO);
        }
    }

    QCoreApplication::processEvents();

    QSqlQuery query_signature_list(source_db);
    if(!query_signature_list.exec("select category,label,header,signature_status,offset from tbl_signature_list"))
    {
        recon_static_functions::app_debug(" signature_list ------FAILED------- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(query_signature_list.lastError().text(),Q_FUNC_INFO);
    }
    while(query_signature_list.next())
    {
        QString command_signature_list = QString("insert into 'tbl_signature_list' (category,label,header,signature_status,offset) values(?,?,?,?,?)");

        QStringList list_value_signature_list;
        list_value_signature_list << query_signature_list.value(0).toString() << query_signature_list.value(1).toString()<< query_signature_list.value(2).toString() <<  query_signature_list.value(3).toString() << query_signature_list.value(4).toString();

        if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_signature_list,list_value_signature_list,destination_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug(" Insert signature_list -----FAILED-----",Q_FUNC_INFO);
        }
    }

    source_db.close();
    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::insert_current_mounted_image_info_in_db()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    QString mounted_image_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "mounted_images/mounted_images_info.sqlite";

    struct_global_image_mounter_image_info struct_image_mount_info = image_mounter_obj->pub_get_struct_image_info_last_mounted();

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(mounted_image_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + mounted_image_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    /// Insert Image info
    QString command = "INSERT INTO images_info(image_path_1, image_name_1, shadow_file_path_1, disk_node_1, disk_identifier_1,"
                      "osx_fuse_mount_point_1, image_path_2, image_name_2, shadow_file_path_2, disk_node_2, disk_identifier_2,"
                      "osx_fuse_mount_point_2, image_format_type, image_password, image_source_type_display,image_source_type_internal) VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?)";

    QStringList arg_list;

    arg_list << struct_image_mount_info.image_path_1 << struct_image_mount_info.image_name_1 << struct_image_mount_info.shadow_file_path_1
             << struct_image_mount_info.disk_node_1 << struct_image_mount_info.disk_identifier_1 << struct_image_mount_info.osx_fuse_mount_point_1
             << struct_image_mount_info.image_path_2 << struct_image_mount_info.image_name_2 << struct_image_mount_info.shadow_file_path_2
             << struct_image_mount_info.disk_node_2 << struct_image_mount_info.disk_identifier_2 << struct_image_mount_info.osx_fuse_mount_point_2
             << struct_image_mount_info.image_format_type << struct_image_mount_info.password << struct_image_mount_info.image_source_type_display
             << struct_image_mount_info.image_source_type_internal;

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, arg_list,destination_db, Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" query insert images_info ---FAILED---",Q_FUNC_INFO);
    }


    command = "SELECT INT FROM images_info WHERE image_path_1 = ?";
    arg_list.clear();
    arg_list << struct_image_mount_info.image_path_1;

    QString INT_val = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command,arg_list,0,destination_db, Q_FUNC_INFO);

    if(!INT_val.isEmpty())
    {
        for(int i = 0; i < struct_image_mount_info.list_struct_partition_info.size(); i++)
        {
            struct_global_image_mounter_partition_info struct_partition_info = struct_image_mount_info.list_struct_partition_info.at(i);

            command = "INSERT INTO partition_info("
                      "image_id, volume_type, volume_name, volume_total_size_byte_numeric, volume_free_size_byte_numeric, volume_fs_type,"
                      "volume_identifier, volume_node, derived_into_disk_node, derived_into_disk_identifier,"
                      "derived_into_volume_name, volume_uuid, mount_path)"
                      "VALUES (?,?,?,?,?, ?,?,?,?,?, ?,?,?)";

            arg_list.clear();

            arg_list << INT_val << struct_partition_info.parent_type << struct_partition_info.parent_name
                     << struct_partition_info.total_size_byte_numeric_QString << struct_partition_info.free_size_byte_numeric_QString
                     << struct_partition_info.fs_type << struct_partition_info.parent_identifier
                     << struct_partition_info.parent_node << struct_partition_info.derived_into_node
                     << struct_partition_info.derived_into_identifier << struct_partition_info.derived_into_name
                     << struct_partition_info.derived_into_uuid << struct_partition_info.mount_path;

            if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command, arg_list,destination_db, Q_FUNC_INFO))
            {
                recon_static_functions::app_debug(" query insert partition_info ---FAILED---",Q_FUNC_INFO);
            }
        }
    }


    destination_db.close();

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);
}



QString MainWindow::get_source_name_from_mount_strct_info(struct_global_image_mounter_partition_info struct_partition_info)
{
    QString parent_name_str;
    if(struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_CoreStorage, Qt::CaseInsensitive)
            || struct_partition_info.parent_type.contains(MACRO_Disk_Partition_Type_Apple_APFS, Qt::CaseInsensitive))
    {
        parent_name_str = struct_partition_info.derived_into_name;
    }

    if(parent_name_str.isEmpty())
    {
        if(!struct_partition_info.parent_name.isEmpty())
        {
            parent_name_str = struct_partition_info.parent_name;
        }
        else if(!struct_partition_info.parent_type.isEmpty())
        {
            parent_name_str = struct_partition_info.parent_type;
        }
        else
        {
            parent_name_str  = struct_partition_info.fs_type;
        }
    }

    return parent_name_str;
}

void MainWindow::remove_open_with_entries_for_sqlite_viewer(QString unique_pattern)
{
    recon_static_functions::app_debug(" START",Q_FUNC_INFO);

    QStringList list = unique_pattern.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
    if(list.size() < 2)
        return;

    QString viewer_type = list.at(0);
    QString viewer_name = list.at(1);

    int enum_bucket = enum_global_case_tree_display_row_BUCKET ;
    bool bool_entry_in_case_tree = false;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Bucket))
        {
            for(int itr = 0; itr < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->childCount(); itr++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Sqlite_Viewer))
                {
                    for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->childCount(); j++)
                    {
                        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->child(j)->text(enum_global_case_tree_display_column_ZERO) == viewer_name)
                        {
                            bool_entry_in_case_tree = true;
                            emit signal_statusbar_progress_bar_show_hide(false, "");
                            return;
                        }
                    }
                }
            }
        }
    }

    if(!bool_entry_in_case_tree)
    {
        if(viewer_type == MACRO_CASE_TREE_Sqlite_Viewer)
        {
            remove_right_click_sqlite_info(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite",viewer_name);
        }
    }

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

}

void MainWindow::create_apfs_snapshots_table_in_sources_db(QString sources_db_path)
{
    QString connection_naam = Q_FUNC_INFO;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(sources_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Sources Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QStringList tables_list = destination_db.tables();

    if(!tables_list.contains("snapshots"))
    {
        QString command_src = QString("create table snapshots(INT INTEGER PRIMARY KEY,snapshot_name varchar(100), source_count_name varchar(100),fs_run_status varchar(10))");
        recon_helper_standard_obj->execute_db_command_by_dbreference(command_src,destination_db,Q_FUNC_INFO);
    }

    destination_db.close();
}

QStringList MainWindow::get_snapshots_list_for_apfs(QString mount_path)
{
    QStringList args_list;

    //    args_list << "listlocalsnapshots" << mount_path;
    //    QString proc_out_data_string  = recon_helper_process_obj->run_command_with_arguments("tmutil", args_list, Q_FUNC_INFO);


    args_list << "apfs" << "listSnapshots" << mount_path;
    QString proc_out_data_string  = recon_helper_process_obj->run_command_with_arguments("diskutil", args_list, Q_FUNC_INFO);

    QStringList tmp_snp_list = proc_out_data_string.split("\n");
    tmp_snp_list.removeAll("");
    tmp_snp_list.removeDuplicates();

    QStringList apfs_snapshots_list;

    for(int ii = 0; ii < tmp_snp_list.size(); ii++)
    {
        QString line_str = tmp_snp_list.at(ii);

        //Name:        com.apple.TimeMachine.2022-06-06-130355.backup
        if(line_str.contains("Name:") && line_str.contains("com.apple.TimeMachine"))
            line_str = line_str.remove(0,line_str.indexOf("com.apple.TimeMachine"));

        line_str = line_str.trimmed();

        if(!line_str.startsWith("com.apple.TimeMachine"))
            continue;

        apfs_snapshots_list <<  line_str;
    }

    apfs_snapshots_list.removeDuplicates();

    return apfs_snapshots_list;
}


void MainWindow::create_system_and_case_info_table_in_case_configuration_db(QString case_info_db_path)
{
    recon_static_functions::app_debug("- START",Q_FUNC_INFO);

    QString destination_db_path_case_details = case_info_db_path;

    QString command = QString("create table casedetails (app_version varchar(50), build_version varchar(50), result_updated_for_versions varchar(500), caseno varchar(50), casename varchar(100), "
                              "location varchar(100), casenotes varchar(100), case_created_time_utc varchar(50),"
                              "case_created_time_with_zone varchar(100), machine_timezone_offset varchar(100), machine_timezone_string varchar(100))");


    if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,destination_db_path_case_details,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug(" create table casedetails ---FAILED---",Q_FUNC_INFO);
    }


    recon_static_functions::app_debug("- END",Q_FUNC_INFO);

}
