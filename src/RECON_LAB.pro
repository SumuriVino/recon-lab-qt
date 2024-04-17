#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T13:49:25
#
#-------------------------------------------------

QT  += core5compat core gui sql network xml multimedia multimediawidgets webenginewidgets printsupport charts quickwidgets  quick positioning concurrent webenginecore
FILE_ROOT = Silicon

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RECON_LAB
TEMPLATE = app

SOURCES += main.cpp\
    Email_Files/email_search/email_search.cpp \
    Email_Files/emlx_mail/emlx_mail.cpp \
    app_related_files/app_related_files.cpp \
    custom_table_header/custom_table_header.cpp \
    custom_table_header/custom_table_header_widget.cpp \
    desktop_info/desktop_info.cpp \
    disk_manager/Disk_Manager/FileVault/disk_manager_filevault_decrypt.cpp \
    disk_manager/Disk_Manager/Format/disk_manager_format_disk.cpp \
    disk_manager/Disk_Manager/disk_manager.cpp \
    disk_manager/display_dialog_box/display_progress_dialog_box.cpp \
    disk_manager/imager_generic_functions/imager_helper_standard.cpp \
    disk_manager/imager_generic_functions/imager_helper_standard_xcode.mm \
    disk_manager/imager_generic_functions/imager_static_functions.cpp \
    disk_manager/imager_generic_functions/imager_static_functions_xcode.mm \
    encryption/encryptor_file_1/ccguess.c \
    encryption/encryptor_file_1/ccrypt.c \
    encryption/encryptor_file_1/ccryptlib.c \
    encryption/encryptor_file_1/encrypt_main.c \
    encryption/encryptor_file_1/getopt.c \
    encryption/encryptor_file_1/getopt1.c \
    encryption/encryptor_file_1/maketables.c \
    encryption/encryptor_file_1/platform.c \
    encryption/encryptor_file_1/readkey.c \
    encryption/encryptor_file_1/rijndael.c \
    encryption/encryptor_file_1/tables.c \
    encryption/encryptor_file_1/traverse.c \
    encryption/encryptor_file_1/unixcrypt3.c \
    encryption/encryptor_file_1/unixcryptlib.c \
    encryption/encryptor_file_1/xalloc.c \
    encryption/encryptor_file_2/encryptor_file_2.cpp \
    encryption/encryptor_file_2/qaesencryption.cpp \
    encryption/encryptor_string/encryptor_string.cpp \
    examiner_space/examiner_space.cpp \
    export/export_case/export_case.cpp \
    export/export_except_reports/export_rsync/export_rsync.cpp \
    export/export_except_reports/export_zip/thread_zip_file_export.cpp \
    export/export_for_reports/export_file.cpp \
    export/export_for_reports/thread_file_copier_module.cpp \
    export/export_hashes/export_hashes_as_csv/export_hashes_as_csv.cpp \
    export/export_hashes/export_hashes_as_sqlite/export_hashes_as_sqlite.cpp \
    export/export_hashes/export_hashes_as_vic/export_hashes_as_vic.cpp \
    export_dialog_box/export_dialog_box_1/export_dialog_box_1.cpp \
    export_dialog_box/export_dialog_box_2/export_dialog_box_2.cpp \
    export_dialog_box/export_dialog_box_3/export_dialog_box_3.cpp \
    face_extractor/cropped_image_display.cpp \
    face_extractor/facefinder.cpp \
    face_extractor/recognize_face.cpp \
    face_extractor/thread_face_find.cpp \
    file_search/exif_metadata_search/search_filters/exif_metadata_filters.cpp \
    file_search/exif_metadata_search/thread_exif_metadata_search.cpp \
    file_search/search_files_with_hash/search_files_with_hash.cpp \
    file_system_extraction/thread_face_analysis.cpp \
    file_system_extraction/thread_fire_analysis.cpp \
    file_system_extraction/thread_mail_emlx.cpp \
    file_system_extraction/thread_mail_mbox.cpp \
    file_system_extraction/thread_mail_pst.cpp \
    file_system_extraction/thread_optical_character_recognition.cpp \
    file_system_extraction/thread_skin_tone_detection.cpp \
    file_system_extraction/thread_unified_logs.cpp \
    file_system_extraction/thread_weapons.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/FAT/fat_template.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/GPT/gpt_parser.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/NTFS/ntfs_parser.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/Scrollbar64.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/Templates/gpt_template.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/connected_drives/connected_drive_list_popup.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/connected_drives/connected_drives.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/fat16_parser.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/fat32_parser.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/fat_directory_entry_template.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/fs_parser.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/fs_viewer_tsk.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/generic_table_template_viewer.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/get_address_dialog.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_complete.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_search_dialog.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/hex_widget/chunks_hexedit.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/hex_widget/commands_hexedit.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/hex_widget/qhexedit_hexedit.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/hexviewer_progress_bar.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/mbr_template.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/mft_entry_template.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/ntfs_vbr_template.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/tag_create_popup.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/templates_color.cpp \
    file_viewer/hex_viewer/hex_viewer_complete/utilities_function.cpp \
    file_viewer/hex_viewer/hex_viewer_unallocate/hex_viewer_unallocated.cpp \
    file_viewer/hex_viewer/hex_viewer_unallocate/thread_hex_view_unallocated.cpp \
    file_viewer/log_viewer/log_viewer.cpp \
    file_viewer/log_viewer/log_viewer_interface.cpp \
    file_viewer/log_viewer/log_viewer_right_click.cpp \
    file_viewer/plist_viewer/manage_plist_key/manage_plist_key.cpp \
    gallery_viewer/gallery_viewer.cpp \
    gallery_viewer/imagelistmodel.cpp \
    gallery_viewer/imagelistview.cpp \
    image_mounter/image_encase_logical.cpp \
    image_mounter/image_recon_fs_block.cpp \
    image_mounter/image_recon_mac_sharing_mode.cpp \
    image_mounter/image_time_machine_backup.cpp \
    ios_backup/uid_backup/uid_backup_extractor/ios_uid_backup_extractor.cpp \
    ios_backup/uid_backup/uid_backup_parser/ios_uid_backup_parser.cpp \
    ios_backup/xml_backup/xml_backup_parser/ios_xml_backup_parser.cpp \
    launcher/source_selector/source_selector_qml.cpp \
    maps/online_maps/online_maps_dialog_xcode.mm \
    previewer/image_preview.cpp \
    previewer/av_preview.cpp \
    previewer/blank.cpp \
    previewer/previewer_stack_module.cpp \
    previewer/detached_metadata_viewer.cpp \
    previewer/mdls/detach_mdls_dialog_module.cpp \
    previewer/mdls/full_mdls_dialog_module.cpp \
    previewer/detached_file_previewer.cpp \
    case_info/case_info_details.cpp \
    load_case.cpp \
    csv_reader/csv_reader.cpp \
    image_mounter/image_mounter.cpp \
    image_mounter/image_forensics.cpp \
    image_mounter/image_vault.cpp \
    previewer/detached_file_previewer_module.cpp \
    previewer/detached_metadata_viewer_module.cpp \
    image_mounter/image_fusion.cpp \
    narad_muni/narad_muni.cpp \
    recon_case/recon_case.cpp \
    recon_case/r_case_run_plugin.cpp \
    recon_case/r_case_k_search.cpp \
    recon_case/r_case_g_report.cpp \
    full_detailed_information/full_detailed_info.cpp \
    detach_detailed_information/detach_detailed_info.cpp \
    detach_detailed_information/detach_manager.cpp \
    launcher/case_launcher/case_wizard.cpp \
    message_box/file_open_cell_widget.cpp \
    message_box/message_dialog.cpp \
    recon_configuration/pages_app_configuration/app_configuration_page_preferences.cpp \
    recon_configuration/pages_recon_configuration/add_timzones/set_global_timezone.cpp \
    recon_configuration/pages_recon_configuration/file_signature/add_ext_category.cpp \
    recon_configuration/pages_recon_configuration/file_signature/edit_category_dialog.cpp \
    recon_configuration/pages_recon_configuration/file_signature/edit_signature_dialog.cpp \
    recon_configuration/pages_recon_configuration/file_signature/file_signature_widget.cpp \
    recon_configuration/pages_recon_configuration/file_signature/new_file_signature.cpp \
    recon_configuration/pages_recon_configuration/import_hashset/existing_hash_column_selection.cpp \
    recon_configuration/pages_recon_configuration/import_hashset/recon_configuration_page_point_hashset.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_apple_metadata.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_examiner_details.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_exif_metadata.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_external_applications.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_file_extension.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_file_size.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_filename_list.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_global_timezone.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_index_files.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_keyword_list.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_mime_types.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_preferences.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_system_password.cpp \
    recon_configuration/pages_recon_configuration/recon_configuration_page_volatility.cpp \
    recon_generic_functions/recon_helper_singular_time.cpp \
    recon_generic_functions/recon_static_functions.cpp \
    recon_generic_functions/recon_static_function_2.cpp \
    center_stackwidget_manager.cpp \
    container_qwidget.cpp \
    recon_generic_functions/recon_static_print_debug.cpp \
    report/report_essentials/htmltopdf/htmltopdf.cpp \
    report/report_essentials/htmltopdf/htmltopdf_webengine.cpp \
    report/report_essentials/htmltopdf/htmltopdf_wkhtmltopdf.cpp \
    run_plugin/run_plugin.cpp \
    add_notes/add_notes.cpp \
    hashset/hashset_display.cpp \
    hashset/hashset_storage.cpp \
    recon_configuration/recon_configuration.cpp \
    progress_bar/progress_bar_window.cpp \
    source_interface/select_source_interface/select_source_interface.cpp \
    source_interface/verify_source/verify_source.cpp \
    source_interface/verify_source/verify_source_slots.cpp \
    uncloseable.cpp \
    unified_logs/m_tableview.cpp \
    unified_logs/unified_logs_model.cpp \
    unified_logs/unified_logs_proxy_model.cpp \
    unified_logs/unified_logs_viewer.cpp \
    witness_info/witness_info_manager/witness_info_manager.cpp \
    witness_info/witness_info_manager/witness_info_manager_xcode.mm \
    witness_info/witness_info_store/witness_info_store.cpp \
    witness_info/witness_info_store/witness_info_store_xcode.mm \
    z_mainwindow/load_case/mainwindow_load_case_updation_configuration.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_data_amain_1.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_data_amain_2.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_data_artifacts_1.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_data_artifacts_2.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_data_artifacts_999.cpp \
    z_mainwindow/mainwindow.cpp \
    ui_custom_classes/flatcombo.cpp \
    ui_custom_classes/r_dockwidget.cpp \
    ui_custom_classes/r_tabwidget.cpp \
    ui_custom_classes/r_treewidget.cpp \
    ui_custom_classes/m_tablewidget.cpp \
    ui_custom_classes/m_treewidget.cpp \
    ui_custom_classes/m_textedit.cpp \
    ui_custom_classes/m_stackedwidget.cpp \
    ui_custom_classes/m_checkbox.cpp \
    add_tags/remove_multiple_tag_selecotor.cpp \
    launcher/case_launcher/pages/case_launcher_page_case_directory.cpp \
    launcher/case_launcher/pages/case_launcher_page_case_info.cpp \
    add_tags/add_tags.cpp \
    recon_case/r_case_filesystem.cpp \
    tablewidget_result_loader/slots_table_result_loader.cpp \
    tablewidget_result_loader/table_result_loader.cpp \
    center_next_previous_navigation.cpp \
    z_mainwindow/mainwindow_config.cpp \
    z_mainwindow/mainwindow_sources.cpp \
    z_mainwindow/mainwindow_viewers.cpp \
    z_mainwindow/mainwindow_global_items.cpp \
    z_mainwindow/mainwindow_display_manager.cpp \
    z_mainwindow/mainwindow_gui_creator.cpp \
    slider/qSlice.cpp \
    csv_reader/csv_reader_pub_functions.cpp \
    task_status/task_progress_bar.cpp \
    task_status/task_status_window.cpp \
    quick_look/quick_look.cpp \
    z_mainwindow/mainwindow_tablewidget_result_loader_interface.cpp \
    z_mainwindow/mainwindow_slots_functions_gui.cpp \
    z_mainwindow/mainwindow_slots_functions_tablewidget_result_loader_interface.cpp \
    z_mainwindow/mainwindow_slots_functions.cpp \
    launcher/plugin_selector/plugin_selector.cpp \
    launcher/plugin_selector/plugin_selector_comman_master.cpp \
    csv_reader/info_anthology/csv_reader_treewidget.cpp \
    csv_reader/info_anthology/csv_reader_thumbnail.cpp \
    csv_reader/info_anthology/csv_reader_textedit.cpp \
    csv_reader/info_anthology/csv_reader_tablewidget.cpp \
    csv_reader/info_anthology/csv_reader_stack.cpp \
    csv_reader/info_anthology/csv_reader_chat_viewer.cpp \
    recon_case/r_case_plugin_extraction.cpp \
    progress_bar/progress_bar_fixed_in_statusbar.cpp \
    exif_data/exif_data_document/exif_data_document_files.cpp \
    exif_data/exif_data_interface.cpp \
    exif_data/exif_data_audio_video/exif_data_audio_video.cpp \
    exif_data/exif_data_images/exif_data_image_files.cpp \
    file_types/slot_functions_filetypes_loader.cpp \
    file_types/filetypes_loader.cpp \
    case_info/case_info_display/case_info_display.cpp \
    case_info/case_info_display/m_custom_source_button.cpp \
    ui_custom_classes/m_hovered_pushbutton.cpp \
    plugin_viewer/manage_column/manage_plugin_columns.cpp \
    plugin_viewer/module_part/library_interface_module.cpp \
    plugin_viewer/display_file_module.cpp \
    plugin_viewer/m_treewidgetitem.cpp \
    plugin_viewer/on_pushbutton_click_file_module.cpp \
    plugin_viewer/plugin_viewer.cpp \
    plugin_viewer/progress_window_module.cpp \
    plugin_viewer/slot_functions_file_module.cpp \
    plugin_viewer/table_widget_click_file_module.cpp \
    plugin_viewer/usb_device_serial_list_module.cpp \
    plugin_viewer/usb_module.mm \
    file_viewer/plist_viewer/plist_viewer.cpp \
    file_viewer/plist_viewer/plist_viewer_interface.cpp \
    file_viewer/plist_viewer/plist_viewer_report_html_template.cpp \
    file_viewer/plist_viewer/plist_viewer_right_click.cpp \
    file_viewer/sqlite_viewer/CreatePlugin/sqlite_viewer_createpluginInfo_db.cpp \
    file_viewer/sqlite_viewer/DisplayGuiWidgets/SaveStates/sqlite_viewer_plugin_savestate.cpp \
    file_viewer/sqlite_viewer/DisplayGuiWidgets/SaveStates/sqlite_viewer_savestate.cpp \
    file_viewer/sqlite_viewer/Execute_Sql/sqlite_viewer_Execute_Sql.cpp \
    file_viewer/sqlite_viewer/Extraction/sqlite_viewer_basecoreclass.cpp \
    file_viewer/sqlite_viewer/Extraction/sqlite_viewer_load_savedstateclass.cpp \
    file_viewer/sqlite_viewer/Extraction/sqlite_viewer_saved_state.cpp \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_CSV.cpp \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_HTML_PDF.cpp \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_html_template.cpp \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_status.cpp \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_XML.cpp \
    file_viewer/sqlite_viewer/sqlite_viewer.cpp \
    file_viewer/sqlite_viewer/sqlite_viewer_manage_colums_type.cpp \
    file_viewer/sqlite_viewer/sqlite_viewer_starter.cpp \
    file_viewer/plist_viewer/plist_viewer_xcode_extraction.mm \
    csv_reader/csv_cutter/csv_plugin_info.cpp \
    csv_reader/csv_cutter/csv_list_maker.cpp \
    csv_reader/csv_cutter/csv_column_info.cpp \
    csv_reader/csv_cutter/csv_table_info.cpp \
    timeline_selection_window/timeline_selection_window.cpp \
    ui_custom_classes/m_hovered_pushbutton_with_signal.cpp \
    tablewidget_result_loader/actions_table_result_loader.cpp \
    ui_custom_classes/m_hovered_pushbutton_round_close.cpp \
    image_mounter/image_mounter_xcode.mm \
    activity_logs/activity_logs_display.cpp \
    activity_logs/activity_logs_storage.cpp \
    screenshots/screenshots_loader.cpp \
    screenshots/screenshots_slots.cpp \
    screenshots/screenshots.cpp \
    story_board/story_board.cpp \
    story_board/story_board_loader.cpp \
    story_board/story_board_loader_story_functions.cpp \
    z_mainwindow/mainwindow_fetaures_connection.cpp \
    launcher/source_viewer/add_new_source.cpp \
    launcher/source_viewer/load_case_source_displayer.cpp \
    launcher/source_selector/source_type/file_vault_image_class.cpp \
    launcher/source_selector/source_type/fusion_drive_class.cpp \
    launcher/source_selector/source_type/home_directory_class.cpp \
    launcher/source_selector/source_type/mounted_volumes_class.cpp \
    launcher/source_selector/source_selector.cpp \
    ui_custom_classes/m_barset.cpp \
    story_board/story_board_textedit.cpp \
    z_mainwindow/mainwindow_actions.cpp \
    artifacts_keyword_search/k_search/artifacts_keyword_search_loader.cpp \
    artifacts_keyword_search/k_search/slot_functions_artifacts_keyword_search_loader.cpp \
    artifacts_keyword_search/k_search/thread_artifacts_keyword_search.cpp \
    artifacts_keyword_search/artifacts_keyword_search_interface.cpp \
    file_search/f_search/thread_file_search_extraction.cpp \
    file_search/file_search_interface.cpp \
    filters/file_search_filters/regular_filters/regular_filters.cpp \
    filters/file_search_filters/regular_filters/regular_filters_slots.cpp \
    filters/file_search_filters/regular_filters/regular_filters_ui.cpp \
    filters/file_search_filters/spotlight_filters/slots_spotlight_filters.cpp \
    filters/file_search_filters/spotlight_filters/spotlight_filters.cpp \
    filters/file_search_filters/spotlight_filters/spotlight_filters_ui.cpp \
    filters/keyword_search_filters/keywords_list_selector.cpp \
    plugin_viewer/chat_viewer/chat_viewer.cpp \
    tags/notes_search/notes_search_loader.cpp \
    tags/notes_search/slot_notes_search.cpp \
    tags/tag_search/slots_tag_search.cpp \
    filters/redefined_result_filters/redefined_result_filters_plugins_tab_select.cpp \
    filters/redefined_result_filters/redefined_result_filters_plugin_tab_column_select.cpp \
    filters/redefined_result_filters/redefined_result_filters_interface.cpp \
    filters/redefined_result_filters/redefined_result_filters_add_new_filter.cpp \
    recon_case/r_case_file_search.cpp \
    artifacts_timeline/artifacts_timeline_saved/artifacts_timeline_saved_loader.cpp \
    artifacts_timeline/artifacts_timeline_saved/slot_artifacts_timeline_saved_loader.cpp \
    redefined_result/redefined_result_full/redefined_result_full_graphs.cpp \
    redefined_result/redefined_result_full/redefined_result_full_graphs_slots.cpp \
    redefined_result/redefined_result_full/redefined_result_full_loader.cpp \
    redefined_result/redefined_result_full/redefined_result_full_loader_slots.cpp \
    artifacts_timeline/artifacts_timeline_full/slot_artifacts_timeline_full_loader.cpp \
    artifacts_timeline/artifacts_timeline_full/artifacts_timeline_full_loader.cpp \
    artifacts_timeline/artifacts_timeline_full/artifacts_timeline_full_graphs.cpp \
    recon_generic_functions/recon_helper_singular.cpp \
    recon_generic_functions/recon_helper_singular_xcode.mm \
    redefined_result/redefined_result_saved/redefined_result_saved_loader.cpp \
    redefined_result/redefined_result_saved/redefined_result_saved_slots.cpp \
    save_result_dialog_box/save_result_dialog_box.cpp \
    file_viewer/sqlite_viewer/sqlite_viewer_functions.cpp \
    file_viewer/sqlite_viewer/sqlite_viewer_slots.cpp \
    global_report_interface/pages/report_launcher_page_report_case_info.cpp \
    global_report_interface/pages/report_launcher_page_report_type_selection.cpp \
    global_report_interface/global_report_interface.cpp \
    global_report_interface/report_launcher.cpp \
    report/report_essentials/report_generic_function.cpp \
    report/report_essentials/report_status.cpp \
    report/report_essentials/report_template_html_advance.cpp \
    report/report_essentials/report_template_html_standard.cpp \
    report/thread_report_builder/create_report_headers.cpp \
    report/thread_report_builder/thread_report_builder.cpp \
    report/csv_report_module.cpp \
    report/kml_report_module.cpp \
    report/pdf_report_module.cpp \
    report/xml_report_module.cpp \
    report/html_standard_report_module.cpp \
    report/html_advance_report_module.cpp \
    file_viewer/sqlite_viewer/thread_file_copier_mudule_sqlite_viewer.cpp \
    recon_case/r_case_content_search.cpp \
    filters/file_search_filters/apple_metadata_filters/apple_metadata_filters.cpp \
    filters/file_search_filters/apple_metadata_filters/slots_apple_metadata_filters.cpp \
    filters/file_search_filters/apple_metadata_filters/apple_metadata_filters_ui.cpp \
    file_system_post_launcher/file_system_post_launcher.cpp \
    launcher/case_launcher/pages/page_filesystem_n_modules.cpp \
    apple_attributes/apple_attributes.cpp \
    system_password_asker/system_password_asker.cpp \
    file_viewer/text_viewer/hexedit/chunks.cpp \
    file_viewer/text_viewer/hexedit/qhexedit.cpp \
    file_viewer/text_viewer/text_viewer.cpp \
    RAM_Analysis/ram_analysis_saved/ram_analysis_saved_loader.cpp \
    RAM_Analysis/ram_analysis_saved/ram_analysis_saved_slots.cpp \
    RAM_Analysis/ram_analysis.cpp \
    RAM_Analysis/ram_analysis_slots.cpp \
    saved_graphs_dialog_box/saved_graphs_dialog_box.cpp \
    saved_graphs_dialog_box/saved_graph_slots.cpp \
    saved_graphs_dialog_box/saved_graph_loader.cpp \
    display_loading_progress_bar/display_loading_progress_bar.cpp \
    file_system_extraction/thread_signature.cpp \
    file_system_extraction/thread_mime_type.cpp \
    file_system_extraction/thread_index_files.cpp \
    file_system_extraction/thread_hashes.cpp \
    file_system_extraction/thread_file_system.cpp \
    file_system_extraction/thread_exif_metadata.cpp \
    file_system_extraction/thread_apple_metadata.cpp \
    file_system_viewer/slots_file_system.cpp \
    file_system_viewer/slots_file_system_search.cpp \
    file_system_viewer/file_system.cpp \
    file_system_extraction/dt_search/dfile.cpp \
    file_system_extraction/dt_search/dstring.cpp \
    file_system_extraction/dt_search/dt_search_interface.cpp \
    file_system_extraction/dt_search/dtsearch.cpp \
    file_system_extraction/dt_search/dtsviewr.cpp \
    file_system_extraction/dt_search/settings.cpp \
    file_system_extraction/dt_search/stdafx.cpp \
    file_system_extraction/dt_search/xmlMaker.cpp \
    content_search/thread_content_search.cpp \
    content_search/content_search.cpp \
    file_system_extraction/thread_file_system_2.cpp \
    recon_generic_functions/recon_helper_standard.cpp \
    file_viewer/strings_viewer/strings_viewer.cpp \
    computer_resource_checker/free_ram_status.cpp \
    tags/tag_search/tag_search_loader.cpp \
    Email_Files/emlx_mail/slot_functions_emlx_mail.cpp \
    file_indexing/file_indexing_display.cpp \
    tags/bookmarks_search/bookmarks_search_loader.cpp \
    tags/bookmarks_search/slots_bookmarks_search_loader.cpp \
    thread_recon_lab_initial_settings.cpp \
    file_system_extraction/thread_apple_timestamps.cpp \
    file_viewer/text_viewer/text_viewer_aascii_unicode.cpp \
    initial_warning_message.cpp \
    thread_db_updation/thread_db_updation_bookmark.cpp \
    launcher/source_selector/source_widget/source_evidencer.cpp \
    launcher/source_selector/source_widget/source_widgets.cpp \
    global_connection_manager.cpp \
    super_timeline/super_timeline.cpp \
    file_export_dialog.cpp \
    z_mainwindow/recon_library/mainwindow_recon_library.cpp \
    z_mainwindow/recon_library/mainwindow_recon_library_updation.cpp \
    zipper/JlCompress.cpp \
    zipper/quazipnewinfo.cpp \
    zipper/qioapi.cpp \
    zipper/quaadler32.cpp \
    zipper/quacrc32.cpp \
    zipper/quagzipfile.cpp \
    zipper/quaziodevice.cpp \
    zipper/quazip.cpp \
    zipper/quazipdir.cpp \
    zipper/quazipfile.cpp \
    zipper/quazipfileinfo.cpp \
    zipper/unzip.c \
    zipper/zip.c \
    file_system_viewer/file_system_gallery_view.cpp \
    plugin_extraction/qobject_plugin_extraction.cpp \
    RAM_Analysis/ram_analysis_password.cpp \
    z_mainwindow/mainwindow_plugin_viewer.cpp \
    file_viewer/registry_viewer/registry_interface.cpp \
    file_viewer/registry_viewer/registry_interface_slots_file.cpp \
    file_viewer/registry_viewer/registry_viewer.cpp \
    file_viewer/registry_viewer/registry_parse.cpp \
    mobile_backup/mobile_backup_loader.cpp \
    mobile_backup/mobile_backup_slots_file.cpp \
    disk_images/disk_images_loader.cpp \
    disk_images/disk_images_slots_file.cpp \
    certificate.cpp \
    maps/offline_maps/offline_maps.cpp \
    maps/online_maps/online_maps_dialog.cpp \
    file_system_extraction/decompress_file_system.cpp \
    launcher/case_launcher/pages/case_launcher_page_timezone_selection.cpp \
    carving/carved_files/carved_file_display/carved_files_loader.cpp \
    carving/carved_files/carved_file_display/carved_files_slot_functions.cpp \
    carving/carved_files/thread_carve_files.cpp \
    carving/carved_passwords/carved_passwords_loader.cpp \
    carving/carved_passwords/carved_passwords_slot_functions.cpp \
    ram_images/ram_images_loader_slots_file.cpp \
    ram_images/ram_images_loader.cpp \
    carving/carved_data/carved_data_display/carved_data_loader.cpp \
    carving/carved_data/carved_data_display/carved_data_slot_functions.cpp \
    carving/carved_data/thread_carve_data.cpp \
    export_as_KML/export_as_kml.cpp \
    snapshots/snapshots.cpp \
    snapshots/snapshots_loader/snapshots_loader.cpp \
    snapshots/snapshots_loader/snapshots_loader_slot_functions.cpp \
    z_mainwindow/mainwindow_tsk_fuse.cpp \
    snapshots/thread_extract_snapshots.cpp \
    image_mounter/image_recon_logical.cpp \
    global_recon_file_info/global_recon_file_info.cpp \
    acquire_devices/ios_devices/acquire_iOS_devices.cpp \
    acquire_devices/android_devices/acquire_android_devices.cpp \
    recon_generic_functions/recon_helper_process.cpp \
    recon_generic_functions/recon_helper_standard_xcode.mm \
    image_mounter/image_iso.cpp \
    language_translater/language_translator.cpp \
    hashset/hashset_display_toolbar/hashset_display_toolbar.cpp \
    hashset/hashset_display_toolbar/thread_parse_nsrl_hashes.cpp \
    z_mainwindow/load_case/mainwindow_load_case.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_sources.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_recon_library.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_caseinfo.cpp \
    z_mainwindow/mainwindow_os_androidos.cpp \
    z_mainwindow/mainwindow_os_winos.cpp \
    z_mainwindow/mainwindow_os_macos.cpp \
    z_mainwindow/mainwindow_os_ios.cpp \
    file_search/regular_filter/thread_file_search_regular_filters.cpp \
    file_search/regular_filter/file_search_regular_filters.cpp \
    file_types/content_search_carved_files_filetypes_loader.cpp \
    z_mainwindow/load_case/mainwindow_load_case_updation_data_feature.cpp \
    tagged_file_export/tagged_file_export.cpp \
    tagged_file_export/tagged_file_export_slots.cpp \
    licensing/usb_device_serial_list.cpp \
    licensing/network_time.cpp \
    licensing/licensed_usb_drive_recogniser.cpp \
    licensing/license_locator.cpp


HEADERS  += \
    Email_Files/email_search/email_search.h \
    Email_Files/emlx_mail/emlx_mail.h \
    app_related_files/app_related_files.h \
    custom_table_header/custom_table_header.h \
    custom_table_header/custom_table_header_widget.h \
    desktop_info/desktop_info.h \
    disk_manager/Disk_Manager/FileVault/disk_manager_filevault_decrypt.h \
    disk_manager/Disk_Manager/Format/disk_manager_format_disk.h \
    disk_manager/Disk_Manager/disk_manager.h \
    disk_manager/display_dialog_box/display_progress_dialog_box.h \
    disk_manager/imager_generic_functions/imager_helper_standard.h \
    disk_manager/imager_generic_functions/imager_static_functions.h \
    disk_manager/imager_global_headers.h \
    encryption/encryptor_file_1/ccrypt.h \
    encryption/encryptor_file_1/ccryptlib.h \
    encryption/encryptor_file_1/getopt.h \
    encryption/encryptor_file_1/gettext.h \
    encryption/encryptor_file_1/header.h \
    encryption/encryptor_file_1/lists.h \
    encryption/encryptor_file_1/main.h \
    encryption/encryptor_file_1/platform.h \
    encryption/encryptor_file_1/readkey.h \
    encryption/encryptor_file_1/rijndael.h \
    encryption/encryptor_file_1/tables.h \
    encryption/encryptor_file_1/traverse.h \
    encryption/encryptor_file_1/unixcrypt3.h \
    encryption/encryptor_file_1/unixcryptlib.h \
    encryption/encryptor_file_1/xalloc.h \
    encryption/encryptor_file_2/encryptor_file_2.h \
    encryption/encryptor_file_2/qaesencryption.h \
    encryption/encryptor_string/encryptor_string.h \
    examiner_space/examiner_space.h \
    export/export_case/export_case.h \
    export/export_except_reports/export_rsync/export_rsync.h \
    export/export_except_reports/export_zip/thread_zip_file_export.h \
    export/export_for_reports/export_file.h \
    export/export_for_reports/thread_file_copier_module.h \
    export/export_hashes/export_hashes_as_csv/export_hashes_as_csv.h \
    export/export_hashes/export_hashes_as_sqlite/export_hashes_as_sqlite.h \
    export/export_hashes/export_hashes_as_vic/export_hashes_as_vic.h \
    export_dialog_box/export_dialog_box_1/export_dialog_box_1.h \
    export_dialog_box/export_dialog_box_2/export_dialog_box_2.h \
    export_dialog_box/export_dialog_box_3/export_dialog_box_3.h \
    face_extractor/cropped_image_display.h \
    face_extractor/facefinder.h \
    face_extractor/recognize_face.h \
    face_extractor/thread_face_find.h \
    file_search/exif_metadata_search/search_filters/exif_metadata_filters.h \
    file_search/exif_metadata_search/thread_exif_metadata_search.h \
    file_search/search_files_with_hash/search_files_with_hash.h \
    file_system_extraction/thread_face_analysis.h \
    file_system_extraction/thread_fire_analysis.h \
    file_system_extraction/thread_mail_emlx.h \
    file_system_extraction/thread_mail_mbox.h \
    file_system_extraction/thread_mail_pst.h \
    file_system_extraction/thread_optical_character_recognition.h \
    file_system_extraction/thread_skin_tone_detection.h \
    file_system_extraction/thread_unified_logs.h \
    file_system_extraction/thread_weapons.h \
    file_viewer/hex_viewer/hex_viewer_complete/FAT/fat_template.h \
    file_viewer/hex_viewer/hex_viewer_complete/GPT/gpt_parser.h \
    file_viewer/hex_viewer/hex_viewer_complete/NTFS/ntfs_parser.h \
    file_viewer/hex_viewer/hex_viewer_complete/Scrollbar64.h \
    file_viewer/hex_viewer/hex_viewer_complete/Templates/gpt_template.h \
    file_viewer/hex_viewer/hex_viewer_complete/common_header.h \
    file_viewer/hex_viewer/hex_viewer_complete/connected_drives/connected_drive_list_popup.h \
    file_viewer/hex_viewer/hex_viewer_complete/connected_drives/connected_drives.h \
    file_viewer/hex_viewer/hex_viewer_complete/fat16_parser.h \
    file_viewer/hex_viewer/hex_viewer_complete/fat32_parser.h \
    file_viewer/hex_viewer/hex_viewer_complete/fat_directory_entry_template.h \
    file_viewer/hex_viewer/hex_viewer_complete/fs_parser.h \
    file_viewer/hex_viewer/hex_viewer_complete/fs_viewer_tsk.h \
    file_viewer/hex_viewer/hex_viewer_complete/generic_table_template_viewer.h \
    file_viewer/hex_viewer/hex_viewer_complete/get_address_dialog.h \
    file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_complete.h \
    file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_search_dialog.h \
    file_viewer/hex_viewer/hex_viewer_complete/hex_widget/chunks_hexedit.h \
    file_viewer/hex_viewer/hex_viewer_complete/hex_widget/commands_hexedit.h \
    file_viewer/hex_viewer/hex_viewer_complete/hex_widget/qhexedit_hexedit.h \
    file_viewer/hex_viewer/hex_viewer_complete/hexviewer_progress_bar.h \
    file_viewer/hex_viewer/hex_viewer_complete/mbr_template.h \
    file_viewer/hex_viewer/hex_viewer_complete/mft_entry_template.h \
    file_viewer/hex_viewer/hex_viewer_complete/ntfs_vbr_template.h \
    file_viewer/hex_viewer/hex_viewer_complete/tag_create_popup.h \
    file_viewer/hex_viewer/hex_viewer_complete/utilities_function.h \
    file_viewer/hex_viewer/hex_viewer_unallocate/hex_viewer_unallocated.h \
    file_viewer/hex_viewer/hex_viewer_unallocate/thread_hex_view_unallocated.h \
    file_viewer/log_viewer/log_viewer.h \
    file_viewer/log_viewer/log_viewer_interface.h \
    file_viewer/plist_viewer/manage_plist_key/manage_plist_key.h \
    gallery_viewer/gallery_viewer.h \
    gallery_viewer/imagelistmodel.h \
    gallery_viewer/imagelistview.h \
    header_global_variables_4.h \
    hivex.h \
    ios_backup/uid_backup/uid_backup_extractor/ios_uid_backup_extractor.h \
    ios_backup/uid_backup/uid_backup_parser/ios_uid_backup_parser.h \
    ios_backup/xml_backup/xml_backup_parser/ios_xml_backup_parser.h \
    ios_backup/xml_backup/xml_backup_parser/ios_xml_header_and_structure.h \
    narad_muni/narad_muni.h \
    narad_muni/narad_muni_sources.h \
    previewer/image_preview.h \
    previewer/av_preview.h \
    previewer/blank.h \
    previewer/previewer_stack_module.h \
    previewer/detached_metadata_viewer.h \
    previewer/mdls/detach_mdls_dialog_module.h \
    previewer/mdls/full_mdls_dialog_module.h \
    previewer/detached_file_previewer.h \
    case_info/case_info_details.h \
    load_case.h \
    csv_reader/csv_reader.h \
    image_mounter/image_mounter.h \
    recon_configuration/pages_app_configuration/app_configuration_page_preferences.h \
    recon_configuration/pages_recon_configuration/add_timzones/set_global_timezone.h \
    recon_configuration/pages_recon_configuration/file_signature/add_ext_category.h \
    recon_configuration/pages_recon_configuration/file_signature/edit_category_dialog.h \
    recon_configuration/pages_recon_configuration/file_signature/edit_signature_dialog.h \
    recon_configuration/pages_recon_configuration/file_signature/file_signature_widget.h \
    recon_configuration/pages_recon_configuration/file_signature/new_file_signature.h \
    recon_configuration/pages_recon_configuration/import_hashset/existing_hash_column_selection.h \
    recon_configuration/pages_recon_configuration/import_hashset/recon_configuration_page_point_hashset.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_apple_metadata.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_examiner_details.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_exif_metadata.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_external_applications.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_file_extension.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_file_size.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_filename_list.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_global_timezone.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_index_files.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_keyword_list.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_mime_types.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_preferences.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_system_password.h \
    recon_configuration/pages_recon_configuration/recon_configuration_page_volatility.h \
    report/report_essentials/htmltopdf/htmltopdf.h \
    report/report_essentials/htmltopdf/htmltopdf_webengine.h \
    report/report_essentials/htmltopdf/htmltopdf_wkhtmltopdf.h \
    source_interface/select_source_interface/select_source_interface.h \
    source_interface/verify_source/verify_source.h \
    ss_keychain/SSKeychain.h \
    previewer/detached_file_previewer_module.h \
    previewer/detached_metadata_viewer_module.h \
    recon_case/recon_case.h \
    full_detailed_information/full_detailed_info.h \
    detach_detailed_information/detach_detailed_info.h \
    detach_detailed_information/detach_manager.h \
    launcher/case_launcher/case_wizard.h \
    message_box/file_open_cell_widget.h \
    message_box/message_dialog.h \
    recon_generic_functions/recon_static_functions.h \
    center_stackwidget_manager.h \
    container_qwidget.h \
    run_plugin/run_plugin.h \
    add_notes/add_notes.h \
    hashset/hashset_display.h \
    hashset/hashset_storage.h \
    recon_configuration/recon_configuration.h \
    progress_bar/progress_bar_window.h \
    uncloseable.h \
    unified_logs/m_tableview.h \
    unified_logs/unified_logs_model.h \
    unified_logs/unified_logs_proxy_model.h \
    unified_logs/unified_logs_viewer.h \
    witness_info/witness_info_manager/witness_info_manager.h \
    witness_info/witness_info_store/witness_info_store.h \
    z_mainwindow/mainwindow.h \
    ui_custom_classes/flatcombo.h \
    ui_custom_classes/r_dockwidget.h \
    ui_custom_classes/r_tabwidget.h \
    ui_custom_classes/r_treewidget.h \
    ui_custom_classes/m_tablewidget.h \
    ui_custom_classes/m_treewidget.h \
    ui_custom_classes/m_textedit.h \
    ui_custom_classes/m_stackedwidget.h \
    ui_custom_classes/m_checkbox.h \
    add_tags/remove_multiple_tag_selecotor.h \
    launcher/case_launcher/pages/case_launcher_page_case_directory.h \
    launcher/case_launcher/pages/case_launcher_page_case_info.h \
    add_tags/add_tags.h \
    header_gui_settings.h \
    header_identical_2.h \
    header_global_functions.h \
    header_global_structures.h \
    tablewidget_result_loader/table_result_loader.h \
    center_next_previous_navigation.h \
    header_report.h \
    slider/qSlice.h \
    header_identical_3.h \
    task_status/task_progress_bar.h \
    task_status/task_status_window.h \
    quick_look/quick_look.h \
    header_global_enum.h \
    launcher/plugin_selector/plugin_selector.h \
    header_plugin_extraction_interface.h \
    progress_bar/progress_bar_fixed_in_statusbar.h \
    exif_data/exif_data_document/exif_data_document_files.h \
    exif_data/exif_data_interface.h \
    exif_data/exif_data_audio_video/exif_data_audio_video.h \
    exif_data/exif_data_images/exif_data_image_files.h \
    file_types/filetypes_loader.h \
    case_info/case_info_display/case_info_display.h \
    case_info/case_info_display/m_custom_source_button.h \
    ui_custom_classes/m_hovered_pushbutton.h \
    header_csv_template.h \
    plugin_viewer/manage_column/manage_plugin_columns.h \
    plugin_viewer/m_treewidgetitem.h \
    plugin_viewer/plugin_viewer.h \
    plugin_viewer/progress_window_module.h \
    plugin_viewer/usb_device_serial_list_module.h \
    file_viewer/plist_viewer/plist_viewer.h \
    file_viewer/plist_viewer/plist_viewer_interface.h \
    file_viewer/plist_viewer/plist_viewer_report_html_template.h \
    file_viewer/sqlite_viewer/DisplayGuiWidgets/SaveStates/sqlite_viewer_plugin_savestate.h \
    file_viewer/sqlite_viewer/DisplayGuiWidgets/SaveStates/sqlite_viewer_savestate.h \
    file_viewer/sqlite_viewer/Extraction/sqlite_viewer_basecoreclass.h \
    file_viewer/sqlite_viewer/Extraction/sqlite_viewer_load_savedstateclass.h \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_html_template.h \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_status.h \
    file_viewer/sqlite_viewer/sqlite_viewer.h \
    file_viewer/sqlite_viewer/sqlite_viewer_manage_colums_type.h \
    file_viewer/sqlite_viewer/sqlite_viewer_starter.h \
    csv_reader/csv_cutter/csv_plugin_info.h \
    csv_reader/csv_cutter/csv_list_maker.h \
    csv_reader/csv_cutter/csv_column_info.h \
    csv_reader/csv_cutter/csv_table_info.h \
    timeline_selection_window/timeline_selection_window.h \
    ui_custom_classes/m_hovered_pushbutton_with_signal.h \
    ui_custom_classes/m_hovered_pushbutton_round_close.h \
    activity_logs/activity_logs_display.h \
    activity_logs/activity_logs_storage.h \
    screenshots/screenshots_loader.h \
    screenshots/screenshots.h \
    story_board/story_board.h \
    story_board/story_board_loader.h \
    launcher/source_viewer/add_new_source.h \
    launcher/source_viewer/load_case_source_displayer.h \
    launcher/source_selector/source_type/file_vault_image_class.h \
    launcher/source_selector/source_type/fusion_drive_class.h \
    launcher/source_selector/source_type/home_directory_class.h \
    launcher/source_selector/source_type/mounted_volumes_class.h \
    launcher/source_selector/source_selector.h \
    ui_custom_classes/m_barset.h \
    story_board/story_board_textedit.h \
    artifacts_keyword_search/k_search/artifacts_keyword_search_loader.h \
    artifacts_keyword_search/k_search/thread_artifacts_keyword_search.h \
    artifacts_keyword_search/artifacts_keyword_search_interface.h \
    file_search/f_search/thread_file_search_extraction.h \
    file_search/file_search_interface.h \
    filters/file_search_filters/regular_filters/regular_filters.h \
    filters/file_search_filters/spotlight_filters/spotlight_filters.h \
    filters/keyword_search_filters/keywords_list_selector.h \
    plugin_viewer/chat_viewer/chat_viewer.h \
    tags/notes_search/notes_search_loader.h \
    filters/redefined_result_filters/redefined_result_filters_plugins_tab_select.h \
    filters/redefined_result_filters/redefined_result_filters_plugin_tab_column_select.h \
    filters/redefined_result_filters/redefined_result_filters_interface.h \
    filters/redefined_result_filters/redefined_result_filters_add_new_filter.h \
    artifacts_timeline/artifacts_timeline_saved/artifacts_timeline_saved_loader.h \
    redefined_result/redefined_result_full/redefined_result_full_loader.h \
    artifacts_timeline/artifacts_timeline_full/artifacts_timeline_full_loader.h \
    recon_generic_functions/recon_helper_singular.h \
    redefined_result/redefined_result_saved/redefined_result_saved_loader.h \
    save_result_dialog_box/save_result_dialog_box.h \
    global_report_interface/pages/report_launcher_page_report_case_info.h \
    global_report_interface/pages/report_launcher_page_report_type_selection.h \
    global_report_interface/global_report_interface.h \
    global_report_interface/report_launcher.h \
    report/report_essentials/report_generic_function.h \
    report/report_essentials/report_status.h \
    report/report_essentials/report_template_html_advance.h \
    report/report_essentials/report_template_html_standard.h \
    report/thread_report_builder/thread_report_builder.h \
    report/csv_report_module.h \
    report/kml_report_module.h \
    report/pdf_report_module.h \
    report/xml_report_module.h \
    report/html_standard_report_module.h \
    report/html_advance_report_module.h \
    file_viewer/sqlite_viewer/thread_file_copier_module_sqlite_viewer.h \
    header_macro_generic.h \
    filters/file_search_filters/apple_metadata_filters/apple_metadata_filters.h \
    file_system_post_launcher/file_system_post_launcher.h \
    launcher/case_launcher/pages/page_filesystem_n_modules.h \
    apple_attributes/apple_attributes.h \
    system_password_asker/system_password_asker.h \
    file_viewer/text_viewer/hexedit/chunks.h \
    file_viewer/text_viewer/hexedit/qhexedit.h \
    file_viewer/text_viewer/text_viewer.h \
    RAM_Analysis/ram_analysis_saved/ram_analysis_saved_loader.h \
    RAM_Analysis/ram_analysis.h \
    saved_graphs_dialog_box/saved_graphs_dialog_box.h \
    saved_graphs_dialog_box/saved_graph_loader.h \
    display_loading_progress_bar/display_loading_progress_bar.h \
    file_system_extraction/thread_signature.h \
    file_system_extraction/thread_mime_type.h \
    file_system_extraction/thread_index_files.h \
    file_system_extraction/thread_hashes.h \
    file_system_extraction/thread_file_system.h \
    file_system_extraction/thread_exif_metadata.h \
    file_system_extraction/thread_apple_metadata.h \
    file_system_viewer/file_system.h \
    file_system_extraction/dt_search/dt_search_interface.h \
    file_system_extraction/dt_search/dtsearch.h \
    file_system_extraction/dt_search/dtsviewr.h \
    file_system_extraction/dt_search/stdafx.h \
    content_search/thread_content_search.h \
    content_search/content_search.h \
    recon_generic_functions/recon_helper_standard.h \
    file_viewer/strings_viewer/strings_viewer.h \
    computer_resource_checker/free_ram_status.h \
    tags/tag_search/tag_search_loader.h \
    file_indexing/file_indexing_display.h \
    tags/bookmarks_search/bookmarks_search_loader.h \
    thread_recon_lab_initial_settings.h \
    file_system_extraction/thread_apple_timestamps.h \
    file_viewer/text_viewer/text_viewer_aascii_unicode.h \
    initial_warning_message.h \
    thread_db_updation/thread_db_updation_bookmark.h \
    launcher/source_selector/source_widget/source_evidencer.h \
    launcher/source_selector/source_widget/source_widgets.h \
    global_connection_manager.h \
    super_timeline/super_timeline.h \
    file_export_dialog.h \
    zipper/ioapi.h \
    zipper/JlCompress.h \
    zipper/minizip_crypt.h \
    zipper/quaadler32.h \
    zipper/quachecksum32.h \
    zipper/quacrc32.h \
    zipper/quagzipfile.h \
    zipper/quaziodevice.h \
    zipper/quazip_global.h \
    zipper/quazip.h \
    zipper/quazipdir.h \
    zipper/quazipfile.h \
    zipper/quazipfileinfo.h \
    zipper/quazipnewinfo.h \
    zipper/unzip.h \
    zipper/zip.h \
    plugin_extraction/qobject_plugin_extraction.h \
    file_viewer/registry_viewer/registry_viewer.h \
    file_viewer/registry_viewer/registry_interface.h \
    file_viewer/registry_viewer/registry_parse.h \
    mobile_backup/mobile_backup_loader.h \
    disk_images/disk_images_loader.h \
    maps/offline_maps/offline_maps.h \
    maps/online_maps/online_maps_dialog.h \
    file_system_extraction/decompress_file_system.h \
    launcher/case_launcher/pages/case_launcher_page_timezone_selection.h \
    carving/carved_files/carved_file_display/carved_files_loader.h \
    carving/carved_files/thread_carve_files.h \
    carving/carved_passwords/carved_passwords_loader.h \
    ram_images/ram_images_loader.h \
    carving/carved_data/carved_data_display/carved_data_loader.h \
    carving/carved_data/thread_carve_data.h \
    export_as_KML/export_as_kml.h \
    snapshots/snapshots.h \
    snapshots/snapshots_loader/snapshots_loader.h \
    snapshots/thread_extract_snapshots.h \
    global_recon_file_info/global_recon_file_info.h \
    acquire_devices/ios_devices/acquire_iOS_devices.h \
    acquire_devices/android_devices/acquire_android_devices.h \
    recon_generic_functions/recon_helper_process.h \
    header_global_variables_2.h \
    header_global_variables_3.h \
    header_global_variables_1.h \
    header_identical_1.h \
    language_translater/language_translator.h \
    hashset/hashset_display_toolbar/hashset_display_toolbar.h \
    hashset/hashset_display_toolbar/thread_parse_nsrl_hashes.h \
    file_search/regular_filter/thread_file_search_regular_filters.h \
    file_search/regular_filter/file_search_regular_filters.h \
    tagged_file_export/tagged_file_export.h \
    licensing/usb_device_serial_list.h \
    licensing/network_time.h \
    licensing/licensed_usb_drive_recogniser.h \
    licensing/license_locator.h


FORMS    += previewer/image_preview.ui \
    Email_Files/email_search/email_search.ui \
    Email_Files/emlx_mail/emlx_mail.ui \
    app_related_files/app_related_files.ui \
    disk_manager/Disk_Manager/FileVault/disk_manager_filevault_decrypt.ui \
    disk_manager/Disk_Manager/Format/disk_manager_format_disk.ui \
    disk_manager/Disk_Manager/disk_manager.ui \
    disk_manager/display_dialog_box/display_progress_dialog_box.ui \
    examiner_space/examiner_space.ui \
    export/export_case/export_case.ui \
    export/export_hashes/export_hashes_as_csv/export_hashes_as_csv.ui \
    export/export_hashes/export_hashes_as_sqlite/export_hashes_as_sqlite.ui \
    export/export_hashes/export_hashes_as_vic/export_hashes_as_vic.ui \
    export_dialog_box/export_dialog_box_1/export_dialog_box_1.ui \
    export_dialog_box/export_dialog_box_2/export_dialog_box_2.ui \
    export_dialog_box/export_dialog_box_3/export_dialog_box_3.ui \
    face_extractor/cropped_image_display.ui \
    face_extractor/recognize_face.ui \
    file_search/exif_metadata_search/search_filters/exif_metadata_filters.ui \
    file_search/search_files_with_hash/search_files_with_hash.ui \
    file_viewer/hex_viewer/hex_viewer_complete/FAT/fat_template.ui \
    file_viewer/hex_viewer/hex_viewer_complete/Templates/gpt_template.ui \
    file_viewer/hex_viewer/hex_viewer_complete/connected_drives/connected_drive_list_popup.ui \
    file_viewer/hex_viewer/hex_viewer_complete/fat_directory_entry_template.ui \
    file_viewer/hex_viewer/hex_viewer_complete/generic_table_template_viewer.ui \
    file_viewer/hex_viewer/hex_viewer_complete/get_address_dialog.ui \
    file_viewer/hex_viewer/hex_viewer_complete/hex_viewer_search_dialog.ui \
    file_viewer/hex_viewer/hex_viewer_complete/hexviewer_progress_bar.ui \
    file_viewer/hex_viewer/hex_viewer_complete/mbr_template.ui \
    file_viewer/hex_viewer/hex_viewer_complete/mft_entry_template.ui \
    file_viewer/hex_viewer/hex_viewer_complete/ntfs_vbr_template.ui \
    file_viewer/hex_viewer/hex_viewer_complete/tag_create_popup.ui \
    file_viewer/hex_viewer/hex_viewer_unallocate/hex_viewer_unallocated.ui \
    file_viewer/log_viewer/log_viewer_interface.ui \
    file_viewer/plist_viewer/manage_plist_key/manage_plist_key.ui \
    previewer/av_preview.ui \
    previewer/blank.ui \
    previewer/previewer_stack_module.ui \
    previewer/detached_metadata_viewer.ui \
    previewer/mdls/detach_mdls_dialog_module.ui \
    previewer/mdls/full_mdls_dialog_module.ui \
    previewer/detached_file_previewer.ui \
    load_case.ui \
    previewer/detached_file_previewer_module.ui \
    previewer/detached_metadata_viewer_module.ui \
    full_detailed_information/full_detailed_info.ui \
    detach_detailed_information/detach_detailed_info.ui \
    launcher/case_launcher/case_wizard.ui \
    message_box/message_dialog.ui \
    recon_configuration/pages_app_configuration/app_configuration_page_preferences.ui \
    recon_configuration/pages_recon_configuration/add_timzones/set_global_timezone.ui \
    recon_configuration/pages_recon_configuration/file_signature/add_ext_category.ui \
    recon_configuration/pages_recon_configuration/file_signature/edit_category_dialog.ui \
    recon_configuration/pages_recon_configuration/file_signature/edit_signature_dialog.ui \
    recon_configuration/pages_recon_configuration/file_signature/file_signature_widget.ui \
    recon_configuration/pages_recon_configuration/file_signature/new_file_signature.ui \
    recon_configuration/pages_recon_configuration/import_hashset/existing_hash_column_selection.ui \
    recon_configuration/pages_recon_configuration/import_hashset/recon_configuration_page_point_hashset.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_apple_metadata.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_examiner_details.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_exif_metadata.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_external_applications.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_file_extension.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_file_size.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_filename_list.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_global_timezone.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_index_files.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_keyword_list.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_mime_types.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_preferences.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_system_password.ui \
    recon_configuration/pages_recon_configuration/recon_configuration_page_volatility.ui \
    run_plugin/run_plugin.ui \
    add_notes/add_notes.ui \
    hashset/hashset_display.ui \
    hashset/hashset_storage.ui \
    recon_configuration/recon_configuration.ui \
    progress_bar/progress_bar_window.ui \
    source_interface/select_source_interface/select_source_interface.ui \
    source_interface/verify_source/verify_source.ui \
    uncloseable.ui \
    unified_logs/unified_logs_viewer.ui \
    z_mainwindow/mainwindow.ui \
    add_tags/remove_multiple_tag_selecotor.ui \
    launcher/case_launcher/pages/case_launcher_page_case_directory.ui \
    launcher/case_launcher/pages/case_launcher_page_case_info.ui \
    add_tags/add_tags.ui \
    task_status/task_progress_bar.ui \
    task_status/task_status_window.ui \
    launcher/plugin_selector/plugin_selector.ui \
    progress_bar/progress_bar_fixed_in_statusbar.ui \
    case_info/case_info_display/case_info_display.ui \
    plugin_viewer/manage_column/manage_plugin_columns.ui \
    plugin_viewer/plugin_viewer.ui \
    plugin_viewer/progress_window_module.ui \
    file_viewer/plist_viewer/plist_viewer_interface.ui \
    file_viewer/sqlite_viewer/DisplayGuiWidgets/SaveStates/sqlite_viewer_plugin_savestate.ui \
    file_viewer/sqlite_viewer/DisplayGuiWidgets/SaveStates/sqlite_viewer_savestate.ui \
    file_viewer/sqlite_viewer/Reports/sqlite_viewer_report_status.ui \
    file_viewer/sqlite_viewer/sqlite_viewer.ui \
    file_viewer/sqlite_viewer/sqlite_viewer_manage_colums_type.ui \
    timeline_selection_window/timeline_selection_window.ui \
    activity_logs/activity_logs_display.ui \
    screenshots/screenshots.ui \
    story_board/story_board.ui \
    launcher/source_viewer/add_new_source.ui \
    launcher/source_viewer/load_case_source_displayer.ui \
    launcher/source_selector/source_type/file_vault_image_class.ui \
    launcher/source_selector/source_type/fusion_drive_class.ui \
    launcher/source_selector/source_type/home_directory_class.ui \
    launcher/source_selector/source_type/mounted_volumes_class.ui \
    launcher/source_selector/source_selector.ui \
    artifacts_keyword_search/artifacts_keyword_search_interface.ui \
    file_search/file_search_interface.ui \
    filters/file_search_filters/regular_filters/regular_filters.ui \
    filters/keyword_search_filters/keywords_list_selector.ui \
    filters/redefined_result_filters/redefined_result_filters_plugins_tab_select.ui \
    filters/redefined_result_filters/redefined_result_filters_plugin_tab_column_select.ui \
    filters/redefined_result_filters/redefined_result_filters_interface.ui \
    filters/redefined_result_filters/redefined_result_filters_add_new_filter.ui \
    save_result_dialog_box/save_result_dialog_box.ui \
    global_report_interface/pages/report_launcher_page_report_case_info.ui \
    global_report_interface/pages/report_launcher_page_report_type_selection.ui \
    global_report_interface/global_report_interface.ui \
    global_report_interface/report_launcher.ui \
    report/report_essentials/report_status.ui \
    file_system_post_launcher/file_system_post_launcher.ui \
    launcher/case_launcher/pages/page_filesystem_n_modules.ui \
    apple_attributes/apple_attributes.ui \
    system_password_asker/system_password_asker.ui \
    file_viewer/text_viewer/text_viewer.ui \
    RAM_Analysis/ram_analysis.ui \
    saved_graphs_dialog_box/saved_graphs_dialog_box.ui \
    display_loading_progress_bar/display_loading_progress_bar.ui \
    file_system_viewer/file_system.ui \
    content_search/content_search.ui \
    file_viewer/strings_viewer/strings_viewer.ui \
    file_indexing/file_indexing_display.ui \
    file_viewer/text_viewer/text_viewer_aascii_unicode.ui \
    initial_warning_message.ui \
    launcher/source_selector/source_widget/source_evidencer.ui \
    launcher/source_selector/source_widget/source_widgets.ui \
    super_timeline/super_timeline.ui \
    file_export_dialog.ui \
    file_viewer/registry_viewer/registry_interface.ui \
    maps/offline_maps/offline_maps.ui \
    maps/online_maps/online_maps_dialog.ui \
    launcher/case_launcher/pages/case_launcher_page_timezone_selection.ui \
    export_as_KML/export_as_kml.ui \
    snapshots/snapshots.ui \
    acquire_devices/ios_devices/acquire_iOS_devices.ui \
    acquire_devices/android_devices/acquire_android_devices.ui \
    hashset/hashset_display_toolbar/hashset_display_toolbar.ui \
    file_search/regular_filter/file_search_regular_filters.ui \
    tagged_file_export/tagged_file_export.ui \
    licensing/license_locator.ui

LIBS += -L$$PWD/../$$FILE_ROOT/ExtraLibs -ltsk.19 -lcrypto.3 -ldlib.19.24.0

#LIBS += -L/usr/local/lib/ -lmagic

INCLUDEPATH += /usr/local/ssl/include/
INCLUDEPATH += /opt/local/lib/include/
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /opt/local/include
INCLUDEPATH += /opt/homebrew/include/

LIBS += -framework IOKit
LIBS += -framework CoreServices
LIBS += -framework Foundation
LIBS += -framework Cocoa
LIBS += -framework Security
LIBS += -framework AppKit
LIBS += -framework Quartz
LIBS += -framework QuickLook
LIBS += -framework AVFoundation

LIBS += -L$$PWD/../$$FILE_ROOT/ExtraLibs -lhivex.0 -lz.1
LIBS += -L$$PWD/../$$FILE_ROOT/ExtraLibs/mlibs -ldtsearch64

# **************Face Analysis libs Start**********
LIBS += -L$$PWD/../$$FILE_ROOT/ExtraLibs/ -llapack

LIBS += -L$$PWD/../$$FILE_ROOT/ExtraLibs/ -lblas
# **************Face Analysis libs End**********

OBJECTIVE_SOURCES += \
    ss_keychain/SSKeychain.mm \
    recon_generic_functions/recon_static_functions_xcode.mm \
    quick_look/xcode_quick_look.mm \
    exif_data/exif_data_audio_video/xcode_extract_exif_data_audio_video.mm \
    exif_data/exif_data_document/xcode_extract_exif_data_documents.mm \
    exif_data/exif_data_images/xcode_extract_exif_data_images.mm \
    licensing/usb.mm

RESOURCES += \
    m_qml/offline_maps/offline_maps.qrc \
    m_qml/source_add/source_add.qrc \
    m_qml/splash_window/splash_window.qrc

DISTFILES +=

#-----Bundle Script-----
# Define the path of the bundle.sh script in the source code
bundle_script.source = $$PWD/../Deployment/bundle.sh

# Define the path of the destination in the build directory
bundle_script.target = $$OUT_PWD/bundle.sh

# Define the type of operation as a file copy, then change the file permissions
bundle_script.commands = $(COPY_FILE) $$shell_quote($$bundle_script.source) $$shell_quote($$bundle_script.target); chmod +x $$shell_quote($$bundle_script.target)

# Instruct qmake to always run this step
bundle_script.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += bundle_script

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$bundle_script.target

#-----Icons-----
# Define the path of the icons directory in the source code
icons.source = $$PWD/../icons

# Define the path of the icons directory in the build directory
icons.target = $$OUT_PWD/RECON_LAB.app/Contents/icons

# Define the type of the operation as directory creation and copy
icons.commands = mkdir -p $$shell_quote($$icons.target); $(COPY_DIR) $$shell_quote($$icons.source)/* $$shell_quote($$icons.target)

# Instruct qmake to always run this step
icons.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += icons

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$icons.target

#-----Resources-----
# Define the path of the resources directory in the source code
resources_files.source = $$PWD/../$$FILE_ROOT/Resources

# Define the path of the resources directory in the build directory
resources_files.target = $$OUT_PWD/RECON_LAB.app/Contents/Resources

# Define the type of the operation as copy
resources_files.commands = mkdir -p $$shell_quote($$resources_files.target); $(COPY_DIR) $$shell_quote($$resources_files.source)/* $$shell_quote($$resources_files.target)

# Instruct qmake to always run this step
resources_files.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += resources_files

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$resources_files.target

#-----Artifacts-----
# Define the path of the resources directory in the source code
artifacts_libs.source = $$PWD/../$$FILE_ROOT/Artifacts_Libs

# Define the path of the resources directory in the build directory
artifacts_libs.target = $$OUT_PWD/RECON_LAB.app/Contents/Artifacts_Libs/

# Define the type of the operation as copy
artifacts_libs.commands = mkdir -p $$shell_quote($$artifacts_libs.target); $(COPY_DIR) $$shell_quote($$artifacts_libs.source)/* $$shell_quote($$artifacts_libs.target)

# Instruct qmake to always run this step
artifacts_libs.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += artifacts_libs

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$artifacts_libs.target

#-----MacOS-----
# Define the path of the directory with Imager and other tools in the source code
macos_imager.source = $$PWD/../$$FILE_ROOT/MacOS

# Define the path of the MacOS directory in the build directory
macos_imager.target = $$OUT_PWD/RECON_LAB.app/Contents/MacOS/

# Define the type of the operation as copy
macos_imager.commands = mkdir -p $$shell_quote($$macos_imager.target); $(COPY_DIR) $$shell_quote($$macos_imager.source)/* $$shell_quote($$macos_imager.target)

# Instruct qmake to always run this step
macos_imager.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += macos_imager

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$macos_imager.target

#-----ZAlreadySigned-----
# Define the path of the directory with Imager and other tools in the source code
z_alreadysigned.source = $$PWD/../$$FILE_ROOT/ZAlreadySigned

# Define the path of the ZAlreadySigned directory in the build directory
z_alreadysigned.target = $$OUT_PWD/RECON_LAB.app/Contents/ZAlreadySigned/

# Define the type of the operation as copy
z_alreadysigned.commands = mkdir -p $$shell_quote($$z_alreadysigned.target); $(COPY_DIR) $$shell_quote($$z_alreadysigned.source)/* $$shell_quote($$z_alreadysigned.target)

# Instruct qmake to always run this step
z_alreadysigned.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += z_alreadysigned

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$z_alreadysigned.target

#-----ZNoNeedToSign-----
# Define the path of the directory with Imager and other tools in the source code
z_noneedtosign.source = $$PWD/../$$FILE_ROOT/ZNoNeedToSign

# Define the path of the ZNoNeedToSign directory in the build directory
z_noneedtosign.target = $$OUT_PWD/RECON_LAB.app/Contents/ZNoNeedToSign/

# Define the type of the operation as copy
z_noneedtosign.commands = mkdir -p $$shell_quote($$z_noneedtosign.target); $(COPY_DIR) $$shell_quote($$z_noneedtosign.source)/* $$shell_quote($$z_noneedtosign.target)

# Instruct qmake to always run this step
z_noneedtosign.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += z_noneedtosign

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$z_noneedtosign.target

#-----Extra Framework Libs-----
# Define the path of the directory with extra framework libs used by the plugins
extra_libs.source = $$PWD/../$$FILE_ROOT/ExtraLibs

# Define the path of the Frameworks directory in the build directory
extra_libs.target = $$OUT_PWD/RECON_LAB.app/Contents/Frameworks/

# Define the type of the operation as copy
extra_libs.commands = mkdir -p $$shell_quote($$extra_libs.target); $(COPY_DIR) $$shell_quote($$extra_libs.source)/* $$shell_quote($$extra_libs.target)

# Instruct qmake to always run this step
extra_libs.CONFIG = no_check_exist

# Add this operation to the build steps
QMAKE_EXTRA_TARGETS += extra_libs

# Instruct qmake to run this step before compiling the code
PRE_TARGETDEPS += $$extra_libs.target
