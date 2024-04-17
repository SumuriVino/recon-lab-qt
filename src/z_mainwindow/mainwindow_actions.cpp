#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_action_run_artifacts_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_run_plugin_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_ram_analysis_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_ram_analysis_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_text_indexing_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_text_indexing_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_hashset_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_hashset_from_toolbar_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_file_search_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_file_search_regular_filters_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_content_search_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_content_search_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_exif_metadata_search_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_exif_metadata_filters_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_apple_metadata_search_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_file_apple_metadata_filters_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_artifacts_keyword_search_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_keyword_search_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_recognize_face_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_recognize_face_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_artifacts_timeline_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_artifacts_timeline_full_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_super_timeline_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_super_timeline_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_tagged_file_export_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_global_tagged_file_export_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_export_case_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_export_case_from_toolbar_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_global_report_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_global_report_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::on_action_story_board_mainmenu_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    slot_action_story_board_triggered();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);

}

void MainWindow::remove_entry_form_the_feature_combobox(QString item_name)
{
    int index =  0;
    for(int com = 0; com < combobox_current_displayed_feature_opened->count(); com++)
    {
        if(combobox_current_displayed_feature_opened->itemText(com) == item_name)
        {
            index = com;
            break;
        }

    }
    //            combobox_current_displayed_feature_opened->removeItem(combobox_current_displayed_feature_opened->currentIndex());
    combobox_current_displayed_feature_opened->removeItem(index);

}

void MainWindow::update_remained_tags_in_notes_search_db_and_display(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme)
{
    QString remained_tags_str = list_remained_tags.join(",");

    for(int i = 0; i < list_remained_tags.size(); i++)
    {
        QString tag_name = list_remained_tags.at(i);
        if(tag_name.isEmpty())
            continue;


        QString notes_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

        QString command = "UPDATE notes SET recon_tag_value=? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << remained_tags_str << plugin_name << tab_name << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,notes_result_db_path,Q_FUNC_INFO);

    }

}

void MainWindow::remove_selected_plugins_from_display_on_add_source()
{
    ///--- Remove Plugin Category
    for(int count = 0; count < combobox_category_navigation->count(); count++)
    {
        if(combobox_category_navigation->itemText(count) == QString(MACRO_CASE_TREE_Artifacts))
        {
            /// Do not use break here. Remove all items for Plugin
            combobox_category_navigation->removeItem(count);
        }
    }



    ///--- Remove Selected Plugins from Features

    QStringList feature_list = center_stackwidget_manager_obj->pub_get_feature_name_list(QString(MACRO_CASE_TREE_Artifacts));

    for(int count = 0; count < feature_list.size(); count++)
    {
        QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+  QString(MACRO_RECON_Splitter_1_multiple)
                + QString(MACRO_CASE_TREE_Artifacts) + QString(MACRO_RECON_Splitter_1_multiple) + feature_list.at(count);

        center_stackwidget_manager_obj->pub_remove_entry_feature_form_list(feature_unique_name);
    }
}


void MainWindow::remove_case_details_from_display_on_add_source()
{

    ///--- Remove Plugin Category
    for(int count = 0; count < combobox_category_navigation->count(); count++)
    {
        if(combobox_category_navigation->itemText(count) == QString(MACRO_CASE_TREE_Case))
        {
            /// Do not use break here. Remove all items for Plugin
            combobox_category_navigation->removeItem(count);
        }
    }


    ///--- Remove case details from Features

    QStringList feature_list = center_stackwidget_manager_obj->pub_get_feature_name_list(QString(MACRO_CASE_TREE_Case));

    for(int count = 0; count < feature_list.size(); count++)
    {

        QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()
                + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Case + MACRO_RECON_Splitter_1_multiple + feature_list.at(count);


        center_stackwidget_manager_obj->pub_remove_entry_feature_form_list(feature_unique_name);
    }

}


void MainWindow::close_feature_without_save(QString unique_pattern)
{
    recon_static_functions::app_debug("close_feature_without_save START",Q_FUNC_INFO);

    QStringList list = unique_pattern.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
    if(list.size() < 3)
        return;

    QString viewer_type = list.at(1);
    QString viewer_name = list.at(2);

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait....");


    int enum_bucket = enum_global_case_tree_display_row_BUCKET ;

    bool check_feature_is_saved = false;
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

                            check_feature_is_saved = true;
                            emit signal_statusbar_progress_bar_show_hide(false, "");
                            return;
                        }
                    }
                }
            }
        }
    }

    if(!check_feature_is_saved)
    {
        if(viewer_type == MACRO_CASE_TREE_Sqlite_Viewer)
        {
            remove_right_click_sqlite_info(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite",viewer_name);

        }
    }
    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug("close_feature_without_save END",Q_FUNC_INFO);

}

void MainWindow::create_dialog_for_storyboard_file_save()
{
    //-Create Dialog for create Storyboard file.
    dialog_save_storyboard = new QDialog(this);
    dialog_save_storyboard->setWindowModality(Qt::WindowModal);

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_storyboard = new QVBoxLayout(dialog_save_storyboard);

    lineedit_storyboard_filename = new QLineEdit(this);
    lineedit_storyboard_filename->setPlaceholderText(QString("Filename..."));

    connect(lineedit_storyboard_filename,SIGNAL(textChanged(QString)),SLOT(slot_lineedit_filename_storyboard_text_changed(QString)));

    pushbutton_create_storyboard = new QPushButton(QString("Create"),this);
    pushbutton_create_storyboard->setFixedWidth(80);
    pushbutton_create_storyboard->setFont(font_s);
    connect(pushbutton_create_storyboard,SIGNAL(clicked()),this,SLOT(slot_storyboard_save_clicked()));
    pushbutton_create_storyboard->setEnabled(false);

    lineedit_storyboard_filename->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_create_storyboard->setAttribute(Qt::WA_MacShowFocusRect,false);

    QPushButton *cancel_button = new QPushButton("Cancel",this);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(slot_storyboard_cancel_clicked()));
    cancel_button->setFixedWidth(80);


    QHBoxLayout *hbox_pushbutton_layout = new QHBoxLayout;

    hbox_pushbutton_layout->addStretch(1);
    hbox_pushbutton_layout->addWidget(pushbutton_create_storyboard);
    hbox_pushbutton_layout->addWidget(cancel_button);


    vboxlayout_storyboard->addWidget(lineedit_storyboard_filename);
    vboxlayout_storyboard->addLayout(hbox_pushbutton_layout);
    vboxlayout_storyboard->setContentsMargins(9,9,9,6);

    dialog_save_storyboard->setLayout(vboxlayout_storyboard);
    dialog_save_storyboard->setWindowTitle(QString(MACRO_Plugin_Name_Story_Board));
    dialog_save_storyboard->setMinimumWidth(200);
    dialog_save_storyboard->setFont(font_s);

    dialog_save_storyboard->setFixedSize(QSize(350, 100));

    pushbutton_create_storyboard->setFocus();

}


void MainWindow::common_right_click_remove_selected_clicked()
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    for(int i = 0; i < treewidget_case_display_obj->selectedItems().count() ; i++)
    {
        QTreeWidgetItem *child = treewidget_case_display_obj->selectedItems().at(i);

        if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Plist_Viewer)
        {
            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
            QString unique_name = QString(MACRO_CASE_TREE_Plist_Viewer) + QString(MACRO_RECON_Splitter_1_multiple) + child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            right_click_remove_widget_by_unique_name(unique_name, child->text(enum_global_case_tree_display_column_ZERO).trimmed());

            remove_right_click_plist_info(dbpath ,child->text(enum_global_case_tree_display_column_ZERO).trimmed());
        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Hex_Viewer)
        {
            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

            QString unique_name = QString(MACRO_CASE_TREE_Hex_Viewer) + QString(MACRO_RECON_Splitter_1_multiple) + child->text(enum_global_case_tree_display_column_ZERO).trimmed();
            right_click_remove_widget_by_unique_name(unique_name, child->text(enum_global_case_tree_display_column_ZERO).trimmed());

            remove_right_click_hexviewer_info(dbpath ,child->text(enum_global_case_tree_display_column_ZERO).trimmed());

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Sqlite_Viewer)
        {
            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

            QString unique_name = QString(MACRO_CASE_TREE_Sqlite_Viewer) + QString(MACRO_RECON_Splitter_1_multiple) + child->text(enum_global_case_tree_display_column_ZERO).trimmed();
            right_click_remove_widget_by_unique_name(unique_name, child->text(enum_global_case_tree_display_column_ZERO).trimmed());

            remove_right_click_sqlite_info(dbpath ,child->text(enum_global_case_tree_display_column_ZERO).trimmed());

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Log_Viewer)
        {
            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";
            QString unique_name = QString(MACRO_CASE_TREE_Log_Viewer) + QString(MACRO_RECON_Splitter_1_multiple) + child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            right_click_remove_widget_by_unique_name(unique_name, child->text(enum_global_case_tree_display_column_ZERO).trimmed());

            remove_right_click_log_info(dbpath ,child->text(enum_global_case_tree_display_column_ZERO).trimmed());
        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Artifacts_Keyword_Search)
        {
            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();

            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString() + "artifacts_keyword_search.sqlite";

            QString unique_name = QString(MACRO_CASE_TREE_Artifacts_Keyword_Search) + QString(MACRO_RECON_Splitter_1_multiple) + child_text;
            right_click_remove_widget_by_unique_name(unique_name, child_text);

            remove_right_click_keyword_search_info(dbpath ,child_text);


        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Artifacts_Timeline)
        {
            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();
            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();


            ///remove widget first then saved results in dbs
            QString unique_name = QString(MACRO_CASE_TREE_Artifacts_Timeline) + QString(MACRO_RECON_Splitter_1_multiple) + child_text;
            right_click_remove_widget_by_unique_name(unique_name, child_text);

            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + "index_artifacts_timeline.sqlite";
            remove_right_click_artifacts_timeline_saved_info(dbpath ,child_text);

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_Plugin_Name_Messenger_Full_RR)
        {

            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();

            ///remove widget first then saved results in dbs
            QString unique_name = QString(MACRO_CASE_TREE_Redefined_Result_Saved_Parent) + QString(MACRO_RECON_Splitter_1_multiple) + child_text;
            right_click_remove_widget_by_unique_name(unique_name, child_text);

            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + "index_messenger_saved.sqlite";
            remove_right_click_redefined_result_messenger_saved_info(dbpath ,child_text);

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_Plugin_Name_Browser_History_Full_RR)
        {
            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();
            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();


            ///remove widget first then saved results in dbs
            QString unique_name = QString(MACRO_CASE_TREE_Redefined_Result_Saved_Parent) + QString(MACRO_RECON_Splitter_1_multiple) + child_text;
            right_click_remove_widget_by_unique_name(unique_name, child_text);

            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + "index_browsers_history_saved.sqlite";
            remove_right_click_redefined_result_browsers_saved_info(dbpath ,child_text);

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Content_Search)
        {
            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            QString tmp_var = child_text;


            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();



            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";

            QString unique_name = QString(MACRO_CASE_TREE_Content_Search) + QString(MACRO_RECON_Splitter_1_multiple) + tmp_var;
            right_click_remove_widget_by_unique_name(unique_name, tmp_var);

            remove_right_click_content_serach_info(dbpath ,child_text);

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_File_Search)
        {
            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            QString tmp_var = child_text;

            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();

            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";

            QString unique_name = QString(MACRO_CASE_TREE_File_Search) + QString(MACRO_RECON_Splitter_1_multiple) + tmp_var;
            right_click_remove_widget_by_unique_name(unique_name, tmp_var);

            remove_right_click_file_search_info(dbpath ,child_text);
        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Registry_Viewer)
        {

            emit signal_statusbar_progress_bar_show_hide(true ,"Please wait.....");

            QString display_tab_name = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            QString registry_viewer_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
            QString command = "Select relative_db_path from tbl_registry_viewer_index where display_tab_name  = ?";
            QString relative_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command ,QStringList(display_tab_name),0,registry_viewer_index_db_path,Q_FUNC_INFO);
            QString registry_db_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path,Q_FUNC_INFO);


            QString unique_name = QString(MACRO_CASE_TREE_Registry_Viewer) + QString(MACRO_RECON_Splitter_1_multiple) + display_tab_name;
            right_click_remove_widget_by_unique_name(unique_name, display_tab_name);

            remove_entries_for_registry_viewer(display_tab_name , registry_db_path , registry_viewer_index_db_path);

            emit signal_statusbar_progress_bar_show_hide(false, "");

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Carved_Files)
        {
            emit signal_statusbar_progress_bar_show_hide(true ,"Please wait.....");

            QString display_tab_name = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            if(display_tab_name.contains(" ("));
            {
                QStringList tm_list;
                tm_list = display_tab_name.split(" (",Qt::SkipEmptyParts);
                if(tm_list.size() >= 1)
                {
                    display_tab_name = tm_list.at(0);
                }
            }

            QString unique_name = QString(MACRO_CASE_TREE_Carved_Files) + QString(MACRO_RECON_Splitter_1_multiple) + display_tab_name;
            right_click_remove_widget_by_unique_name(unique_name, display_tab_name);

            remove_entries_for_carved_files(display_tab_name);

            emit signal_statusbar_progress_bar_show_hide(false, "");

        }
        else if(child->parent()->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Exif_Metadata_Search)
        {
            QString child_text = child->text(enum_global_case_tree_display_column_ZERO).trimmed();

            QString tmp_var = child_text;

            if(child_text.contains(" ("))
                child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

            child_text = child_text.trimmed();

            QString dbpath = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + "index_exif_metadata_search.sqlite";

            QString unique_name = QString(MACRO_CASE_TREE_Exif_Metadata_Search) + QString(MACRO_RECON_Splitter_1_multiple) + tmp_var;
            right_click_remove_widget_by_unique_name(unique_name, tmp_var);

            remove_right_click_exif_metadata_search_info(dbpath ,child_text);
        }

        if(child)
            delete child;
    }

    recon_static_functions::app_debug(" End",Q_FUNC_INFO);

}

void MainWindow::remove_entries_for_carved_files(QString display_tab_name)
{
    recon_static_functions::app_debug(" Start" ,Q_FUNC_INFO);

    QString carved_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString();
    QString carved_files_index_db_path = carved_dir_path  + "carved_files_index.sqlite";
    QString lable_name_in_db;
    if(display_tab_name.contains(" ("));
    {
        QStringList tm_list;
        tm_list = display_tab_name.split(" (",Qt::SkipEmptyParts);
        if(tm_list.size() >= 1)
        {
            lable_name_in_db = tm_list.at(0);
        }
    }

    QString command = "select  source_count_name, carver_files_db_path   from table_carver_index where label_name = ?";
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command ,QStringList(lable_name_in_db),0,carved_files_index_db_path,Q_FUNC_INFO);


    QString dir_name = lable_name_in_db;
    QString dir_path = carved_dir_path + source_count_name + "/" +  dir_name ;
    //// delete dir of respective result
    if(QFileInfo(dir_path).isDir())
    {
        recon_static_functions::do_directory_blank_completely(dir_path, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(dir_path, Q_FUNC_INFO);
    }

    QString delete_cmd = "Delete from table_carver_index where label_name = ?";
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(delete_cmd,QStringList(lable_name_in_db),carved_files_index_db_path,Q_FUNC_INFO);


    recon_static_functions::app_debug("---Ends-----" ,Q_FUNC_INFO);

}

void MainWindow::right_click_remove_widget_by_unique_name(QString unique_name, QString viewer_display_name)
{
    recon_static_functions::app_debug(" Start",Q_FUNC_INFO);

    for(int k = 0; k < stack_widget_cases_plugins_and_global_items->count() ; k++)
    {
        Container_QWidget *widget1 = (Container_QWidget*)stack_widget_cases_plugins_and_global_items->widget(k);
        QString temp_unique_name = widget1->pub_get_widget_unique_name();
        temp_unique_name.remove(0,temp_unique_name.indexOf(QString(MACRO_RECON_Splitter_1_multiple)) + sizeof(QString(MACRO_RECON_Splitter_1_multiple)));

        if(temp_unique_name == unique_name)
        {
            stack_widget_cases_plugins_and_global_items->removeWidget(widget1);
            emit signal_remove_navigation_list_entry_on_combobox_pusbutton_close(widget1->pub_get_widget_unique_name());

            center_stackwidget_manager_obj->pub_remove_entry_feature_form_list(widget1->pub_get_widget_unique_name());

            for(int kk = 0; kk < combobox_current_displayed_feature_opened->count(); kk++)
            {
                if(combobox_current_displayed_feature_opened->itemText(kk) == viewer_display_name)
                {
                    remove_entry_form_the_feature_combobox(combobox_current_displayed_feature_opened->itemText(kk));
                }
            }

            if(combobox_current_displayed_feature_opened->count() == 1)
            {
                combobox_category_navigation->removeItem(combobox_category_navigation->currentIndex());
                combobox_category_navigation->setCurrentIndex(combobox_category_navigation->count() -1 );
            }

        }
    }

    recon_static_functions::app_debug(" Ends",Q_FUNC_INFO);

}


void MainWindow::remove_right_click_keyword_search_info(QString dbpath, QString viewer_display_name)
{
    QStringList arg_list;
    arg_list << viewer_display_name;
    QString command = "select table_name from keyword_search_index where search_tag = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, arg_list,0, dbpath, Q_FUNC_INFO);

    for(int j = 0; j < list.size(); j++)
    {
        QString k_search_sqlitepath = dbpath;
        k_search_sqlitepath = k_search_sqlitepath.remove(k_search_sqlitepath.lastIndexOf("/"), k_search_sqlitepath.size());

        k_search_sqlitepath = k_search_sqlitepath + "/" + list.at(j).trimmed() + ".sqlite";
        QFile file(k_search_sqlitepath);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from keyword_search_index where search_tag = ?", arg_list,dbpath, Q_FUNC_INFO);

}

void MainWindow::remove_right_click_artifacts_timeline_saved_info(QString dbpath, QString viewer_display_name)
{
    QString command = "select db_name from saved_timeline where tab_name = '" + viewer_display_name.replace(" " , "_") + "'";

    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, dbpath, Q_FUNC_INFO);
    for(int j = 0; j < list.size(); j++)
    {
        QString saved_timeline_sqlitepath = dbpath;
        saved_timeline_sqlitepath = saved_timeline_sqlitepath.remove(saved_timeline_sqlitepath.lastIndexOf("/"), saved_timeline_sqlitepath.size());

        saved_timeline_sqlitepath = saved_timeline_sqlitepath + "/" + list.at(j).trimmed();///db name already contains extension
        QFile file(saved_timeline_sqlitepath);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_by_dbpath("delete from saved_timeline where tab_name = '" + viewer_display_name + "'", dbpath, Q_FUNC_INFO);

}

void MainWindow::remove_right_click_redefined_result_messenger_saved_info(QString dbpath, QString viewer_display_name)
{

    QStringList arg_list;
    arg_list << viewer_display_name.replace(" " , "_");

    QString command = "select db_name from saved_index where display_name = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,arg_list, 0, dbpath, Q_FUNC_INFO);

    for(int j = 0; j < list.size(); j++)
    {
        QString saved_timeline_sqlitepath = dbpath;
        saved_timeline_sqlitepath = saved_timeline_sqlitepath.remove(saved_timeline_sqlitepath.lastIndexOf("/"), saved_timeline_sqlitepath.size());

        saved_timeline_sqlitepath = saved_timeline_sqlitepath + "/" + list.at(j).trimmed() + ".sqlite";
        QFile file(saved_timeline_sqlitepath);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from saved_index where display_name = ?", arg_list,dbpath, Q_FUNC_INFO);
}


void MainWindow::remove_right_click_redefined_result_browsers_saved_info(QString dbpath, QString viewer_display_name)
{

    QStringList arg_list;
    arg_list << viewer_display_name.replace(" " , "_");

    QString command = "select db_name from saved_index where display_name = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,arg_list ,0, dbpath, Q_FUNC_INFO);

    for(int j = 0; j < list.size(); j++)
    {
        QString saved_timeline_sqlitepath = dbpath;
        saved_timeline_sqlitepath = saved_timeline_sqlitepath.remove(saved_timeline_sqlitepath.lastIndexOf("/"), saved_timeline_sqlitepath.size());

        saved_timeline_sqlitepath = saved_timeline_sqlitepath + "/" + list.at(j).trimmed() + ".sqlite";
        QFile file(saved_timeline_sqlitepath);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from saved_index where display_name = ?",arg_list, dbpath, Q_FUNC_INFO);
}

void MainWindow::remove_right_click_plist_info(QString dbpath, QString viewer_display_name)
{
    QStringList arg_list;
    arg_list << viewer_display_name;

    QString command = "select db_table_name from plist_viewer_index where viewer_display_name = ?";

    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,arg_list ,0, dbpath, Q_FUNC_INFO);
    if(list.size() > 0)
    {
        recon_helper_standard_obj->execute_db_command_by_dbpath("drop table '" + list.at(0).trimmed() + "'", dbpath, Q_FUNC_INFO);
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from plist_viewer_index where viewer_display_name = ?",arg_list, dbpath, Q_FUNC_INFO);
}

void MainWindow::remove_right_click_sqlite_info(QString dbpath, QString viewer_display_name)
{
    QStringList arg_list;
    arg_list << viewer_display_name;

    QString command = "select destination_directory_path from sv_index where viewer_display_name = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, arg_list,0, dbpath, Q_FUNC_INFO);


    if(list.size() > 0)
    {
        QString dir_to_be_removed = list.at(0);
        if(!dir_to_be_removed.trimmed().startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString()))
            dir_to_be_removed.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString());

        recon_static_functions::do_directory_blank_completely(dir_to_be_removed, Q_FUNC_INFO);
        recon_static_functions::remove_safely_blank_dir_OR_file(dir_to_be_removed, Q_FUNC_INFO);
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from sv_index where viewer_display_name = ?", arg_list,dbpath, Q_FUNC_INFO);

}

void MainWindow::remove_right_click_hexviewer_info(QString dbpath, QString viewer_display_name)
{

    QStringList arg_list;
    arg_list << viewer_display_name;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from hex_viewer_index where viewer_display_name = ?",arg_list, dbpath, Q_FUNC_INFO);
    QString command = "select block_output_path from hex_viewer_blocks where viewer_display_name = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, arg_list ,0, dbpath, Q_FUNC_INFO);


    for(int j = 0; j < list.size(); j++)
    {
        QFile file(list.at(j));
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from hex_viewer_blocks where viewer_display_name = ?",arg_list, dbpath, Q_FUNC_INFO);
}

void MainWindow::remove_right_click_log_info(QString dbpath, QString viewer_display_name)
{
    QStringList arg_list;
    arg_list << viewer_display_name;

    QString command = "select db_table_name from log_viewer_index where viewer_display_name = ?";

    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command,arg_list ,0, dbpath, Q_FUNC_INFO);

    if(list.size() > 0)
    {
        recon_helper_standard_obj->execute_db_command_by_dbpath("drop table '" + list.at(0).trimmed() + "'", dbpath, Q_FUNC_INFO);
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from log_viewer_index where viewer_display_name = ?",arg_list, dbpath, Q_FUNC_INFO);
}

void MainWindow::remove_right_click_content_serach_info(QString dbpath, QString viewer_display_name)
{
    QString command = "select search_db_file_name from table_index_content_search where search_label_name = '" + viewer_display_name + "'";

    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, dbpath, Q_FUNC_INFO);
    for(int j = 0; j < list.size(); j++)
    {
        QString content_search_db_path = dbpath;
        content_search_db_path = content_search_db_path.remove(content_search_db_path.lastIndexOf("/"), content_search_db_path.size());

        content_search_db_path = content_search_db_path + "/" + list.at(j).trimmed() + ".sqlite";
        QFile file(content_search_db_path);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_by_dbpath("delete from table_index_content_search where search_label_name = '" + viewer_display_name + "'", dbpath, Q_FUNC_INFO);

}

void MainWindow::remove_right_click_file_search_info(QString dbpath, QString viewer_display_name)
{
    QStringList arg_list;
    arg_list << viewer_display_name;

    QString command = "select search_db_file_name from file_search_index where search_label_name = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, arg_list,0, dbpath, Q_FUNC_INFO);
    for(int j = 0; j < list.size(); j++)
    {
        QString file_search_db_path = dbpath;
        file_search_db_path = file_search_db_path.remove(file_search_db_path.lastIndexOf("/"), file_search_db_path.size());

        file_search_db_path = file_search_db_path + "/" + list.at(j).trimmed() + ".sqlite";
        QFile file(file_search_db_path);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from file_search_index where search_label_name = ?",arg_list, dbpath, Q_FUNC_INFO);
}


void MainWindow::slot_action_story_board_triggered()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!bool_launcher_work_completed)
        return;

    dialog_save_storyboard->show();

    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);

}

void MainWindow::slot_action_file_system_post_launcher_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    if(snapshots_obj->pub_is_snapshot_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please wait untill 'Snapshots' is running.");
        return;
    }

    file_system_post_launcher_obj->pub_set_recon_case_obj(recon_case_obj);
    file_system_post_launcher_obj->pub_display_source_info();
    file_system_post_launcher_obj->show();
}



void MainWindow::slot_add_snapshot_in_case_tree(QString tab_text)
{
    common_add_snapshots_diff_into_case_tree(tab_text);
}

void MainWindow::slot_snapshot_display(QString snapshot_db_name)
{
    prepare_display_snapshots(snapshot_db_name);
}


void MainWindow::common_add_snapshots_diff_into_case_tree(QString snapshot_name)
{
    QTreeWidgetItem *sub_child;
    QString saved_differenc_snapshots_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_name + ".sqlite";
    qint64 record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, saved_differenc_snapshots_db_path, QStringList(), Q_FUNC_INFO);

    QString icon_path = "../icons/Features/snapshots.png";

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SNAPSHOTS)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Snapshots))
        {
            QTreeWidgetItem *sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, snapshot_name + " (" + QString::number(record_count) + ")");
            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_CASE_TREE_Snapshots));
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SNAPSHOTS)->addChild(sub_child);
        }
    }
}

void MainWindow::slot_action_hashset_from_toolbar_triggered()
{
    if(recon_case_obj->pub_get_hashset_run_status())
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), global_lang_tr_obj.get_translated_string("Hashset already running."));
        return;
    }

    hashset_display_toolbar_obj->pub_set_hashset_toolbar_essentials();
    hashset_display_toolbar_obj->show();
}

void MainWindow::slot_hashset_toolbar_apply_clicked(QStringList default_hashset_list)
{
    recon_static_functions::app_debug("start",Q_FUNC_INFO);

    if(default_hashset_list.size() > 0)
        slot_add_all_available_hashset_in_case_tree();

    recon_static_functions::app_debug("end",Q_FUNC_INFO);
    return;


    QString sources_conf_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
    QString cmd_already_run = QString("Select source_count_name From fs_status Where Hashes = ?");

    QStringList source_count_name_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(QString(cmd_already_run), QStringList(QString::number(0)), 0, sources_conf_db_path, Q_FUNC_INFO);
    source_count_name_list.removeAll("");
    if(source_count_name_list.isEmpty())
    {
        ///add hashsets directly to case tree
        slot_add_all_available_hashset_in_case_tree();
    }
    else
    {
        ///Run Hashes thread for unprocessed sources
        struct_global_file_system_post_launcher_job_info st_obj;
        st_obj.source_list_hashes = source_count_name_list;
        recon_case_obj->pub_set_file_system_selected_job_source_count_name(st_obj);
        recon_case_obj->pub_set_recon_case_runner_type(QString(MACRO_RECON_CASE_RUNNER_TYPE_POST_LAUNCH));
        recon_case_obj->pub_start_hashes_data_extraction();
    }


    recon_static_functions::app_debug("end",Q_FUNC_INFO);
}


void MainWindow::slot_action_recon_configuration_after_launcher_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    recon_configuration_after_launch_obj->show();

}

void MainWindow::slot_global_tagged_file_export_triggered()
{
    if(!bool_launcher_work_completed)
        return;


    bool bool_source_extracted = false;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(qint64 count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        QString source_count_name = obj.source_count_name;
        if(recon_case_obj->pub_is_source_extraction_completed(source_count_name))
        {
            bool_source_extracted = true;
            break ;
        }
    }


    if(!bool_source_extracted)
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION, global_lang_tr_obj.get_translated_string("Source not extracted."));
        message_dialog_object->show();
        return;
    }

    tagged_file_export_obj->pub_set_recon_case_obj(recon_case_obj);
    tagged_file_export_obj->configure_ui();
    tagged_file_export_obj->show();

}

void MainWindow::slot_file_search_regular_filter_work_done(QString cmd, QStringList sources, QString label)
{
    file_search_regular_filters_obj->hide();

    label = label.trimmed();

    ///===check for same search label already
    QString file_search_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_Search_In_Result_QString).toString() + "index_file_search.sqlite";
    QStringList search_db_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select search_db_file_name from file_search_index"), 0, file_search_index_db, Q_FUNC_INFO);
    label = recon_static_functions::get_available_stringname(label, search_db_name_list, Q_FUNC_INFO);

    label = label.trimmed();


    bool_file_search_regular_filters_cancel = false;
    file_search_regular_label = label;

    QString label_text = QString("Please Wait...");
    file_search_regular_filters_progress_bar_obj->pub_set_label_messsge(label_text);
    file_search_regular_filters_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    file_search_regular_filters_progress_bar_obj->show();

    thread_file_search_regular_filters_obj->pub_set_search_command_sources_search_label(cmd, sources, label);
    thread_file_search_regular_filters_obj->start();

}
void MainWindow::slot_thread_file_search_regular_filter_finish()
{
    file_search_regular_filters_progress_bar_obj->hide();

    if(!bool_file_search_regular_filters_cancel)
    {
        common_file_search_finished_or_cancelled(file_search_regular_label);
    }
}


void MainWindow::slot_hide_loading_file_search_regular_filters_display(bool status)
{
    bool_file_search_regular_filters_cancel = status;
    thread_file_search_regular_filters_obj->pub_cancel_extraction();
}

void MainWindow::slot_action_text_indexing_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    if(recon_case_obj->pub_is_file_system_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not process 'Text Indexing' until source(s) have been added to case and apple timestamps have been extracted in case of Apple Filesystems.");
        return;
    }

    if(recon_case_obj->pub_is_text_indexing_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Task 'Text Indexing' is already running.");
        return;
    }

    file_indexing_display_obj->show();
    file_indexing_display_obj->pub_display_selected_files();
}

void MainWindow::slot_text_indexing_done_clicked()
{
    file_indexing_display_obj->hide();

    recon_case_obj->pub_start_file_indexing_from_toolbar();
}


void MainWindow::slot_action_ram_analysis_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    ram_analysis *ram_analysis_obj = new ram_analysis(this);

    if(bool_is_international_version)
        ram_analysis_obj->pub_set_version_international();
    ram_analysis_obj->pub_set_essentials();
    ram_analysis_obj->pub_set_fill_sources();


    //Insert in centre stack
    QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Ram_Analysis + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Ram_Analysis;
    Container_QWidget *container = new Container_QWidget(this);
    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addWidget(ram_analysis_obj);
    vbox_layout->setContentsMargins(0,0,0,0);
    container->setLayout(vbox_layout);
    container->pub_set_feature_widget_name(MACRO_CASE_TREE_Ram_Analysis);
    container->pub_set_category_name(MACRO_CASE_TREE_Ram_Analysis);
    container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
    container->pub_set_widget_unique_name(feature_unique_name);
    center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
    connect(ram_analysis_obj, SIGNAL(signal_ram_analysis_saved_add_in_case_tree(QString)), this, SLOT(slot_ram_analysis_saved_in_case_tree(QString)));
    connect(ram_analysis_obj, SIGNAL(signal_ram_analysis_carved_password_add_in_case_tree(QString)), this, SLOT(slot_ram_analysis_carved_password_in_case_tree(QString)));
    insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Ram_Analysis,MACRO_CASE_TREE_Ram_Analysis);

}


void MainWindow::show_case_wizard()
{
    bool bool_full_disk_access = validate_full_disk_access_functionality();

    if(!bool_full_disk_access)
    {
        show_message_widget_for_full_disk_access();
        return;
    }

    initial_warning_message_obj->pub_show();

    case_wizard_object->show();
    case_wizard_object->activateWindow();

}

void MainWindow::slot_action_file_search_regular_filters_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    file_search_regular_filters_obj->pub_load_defaults();

    file_search_regular_filters_obj->show();

}

void MainWindow::slot_action_recognize_face_triggered()
{
    //    if(!bool_launcher_work_completed)
    //        return;

    //   QString run_status = recon_helper_standard_obj->pub_get_filesystem_modules_thread_run_status(QString(MACRO_JobType_Face_Analysis),Q_FUNC_INFO);

    //    if(run_status.isEmpty() || run_status == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    //    {
    //        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"No source has been processed for '" + QString(MACRO_JobType_Face_Analysis) + "' yet.");
    //        message_dialog_object->show();
    //        return;
    //    }

    recognize_face_obj->pub_set_task_status_window(task_status_window_obj);

    recognize_face_obj->pub_clear_fields();

    recognize_face_obj->show();
}

void MainWindow::slot_action_file_apple_metadata_filters_triggered()
{
    if(!bool_launcher_work_completed)
        return;


    QString source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";

    QString command = QString("select source_count_name, os_scheme_display, ")
            + QString(MACRO_JobType_Apple_Metadata).replace(" ", "_") + " "
            + "from fs_status";



    QStringList apl_meta_status_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command,2,source_db_path,Q_FUNC_INFO);

    if(!apl_meta_status_list.contains(QString(Macro_JobType_Completion_status_done)))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"No source has been processed for " + QString(MACRO_JobType_Apple_Metadata) + " yet.");
        message_dialog_object->show();
        return;
    }

    if(file_search_interface_apple_metadata_filters_obj != NULL)
        delete file_search_interface_apple_metadata_filters_obj;

    file_search_interface_apple_metadata_filters_obj = new file_search_interface(this);
    connect(file_search_interface_apple_metadata_filters_obj, SIGNAL(signal_start_file_search_thread(struct_global_selected_regular_filters_info, QString, QString, bool, QString, QString)), this, SLOT(slot_start_file_search_thread(struct_global_selected_regular_filters_info, QString, QString, bool, QString, QString)));

    file_search_interface_apple_metadata_filters_obj->pub_set_essentials();
    file_search_interface_apple_metadata_filters_obj->pub_set_bool_run_full_mode(true);
    file_search_interface_apple_metadata_filters_obj->pub_set_parent_dir_path("/");        // This will run in full mode
    file_search_interface_apple_metadata_filters_obj->pub_set_file_system_source_count_name("");       // Because this field will use only FS Current dir search mode not in full search mode
    file_search_interface_apple_metadata_filters_obj->pub_set_file_search_stack_widget_index(enum_global_stack_widget_file_search_apple_metadata);
    file_search_interface_apple_metadata_filters_obj->show();
}

void MainWindow::slot_action_show_case_tree_sidebar()
{
    if(!bool_launcher_work_completed)
        return;

    if(dockwidget_case_tree_obj->isHidden())
        dockwidget_case_tree_obj->show();
    else
        dockwidget_case_tree_obj->hide();

}

void MainWindow::slot_action_show_detailed_information()
{

    if(!bool_launcher_work_completed)
        return;

    if(splitter_case_tab_and_metadata_display->widget(MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int)->isHidden())
        splitter_case_tab_and_metadata_display->widget(MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int)->setHidden(false);
    else
        splitter_case_tab_and_metadata_display->widget(MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int)->setHidden(true);
}

void MainWindow::slot_action_super_timeline_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    super_timeline_obj->set_essentials();

    super_timeline_obj->show();

}

void MainWindow::slot_action_activity_logs_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    return;


}

void MainWindow::slot_action_content_search_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    content_search_obj->pub_set_essentials();
    content_search_obj->pub_display_for_popup();
    content_search_obj->pub_display_keyword_list();
    content_search_obj->pub_clear_search_label();

    content_search_obj->show();
}



void MainWindow::remove_right_click_exif_metadata_search_info(QString dbpath, QString viewer_display_name)
{
    QStringList arg_list;
    arg_list << viewer_display_name;

    QString exif_metadata_search_feature_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString();

    QString command = "select search_db_file_name from exif_metadata_search_index where search_label_name = ?";
    QStringList list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbpath(command, arg_list,0, dbpath, Q_FUNC_INFO);
    for(int j = 0; j < list.size(); j++)
    {
        QString file_search_db_path = exif_metadata_search_feature_dir + "/" + list.at(j).trimmed();

        QFile file(file_search_db_path);
        if(file.exists())
            file.remove();
    }

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath("delete from exif_metadata_search_index where search_label_name = ?",arg_list, dbpath, Q_FUNC_INFO);
}

void MainWindow::slot_exif_metadata_filter_work_done(QString cmd, QString where_statement, QStringList sources_list, QString search_label)
{
    exif_metadata_filters_obj->hide();

    search_label = search_label.trimmed();


    ///===check for same search label already
    QString file_search_index_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Exif_Metadata_Search_In_Result_QString).toString() + "index_exif_metadata_search.sqlite";
    QStringList search_label_name_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select search_label_name from exif_metadata_search_index"), 0, file_search_index_db, Q_FUNC_INFO);
    search_label = recon_static_functions::get_available_stringname(search_label, search_label_name_list, Q_FUNC_INFO);

    search_label = search_label.trimmed();

    exif_metadata_search_label = search_label;


    thread_exif_metadata_search_obj->pub_set_search_command_sources_search_label(cmd, where_statement, sources_list, exif_metadata_search_label);
    thread_exif_metadata_search_obj->start();

}

void MainWindow::slot_r_case_thread_exif_metadata_search_cancelled()
{
    thread_exif_metadata_search_obj->pub_cancel_extraction();
}

void MainWindow::slot_thread_exif_metadata_filter_finish()
{
    common_exif_metadata_search_finished_or_cancelled(exif_metadata_search_label);

}

void MainWindow::slot_action_exif_metadata_filters_triggered()
{

    if(!bool_launcher_work_completed)
        return;


    QString run_status = recon_helper_standard_obj->pub_get_filesystem_modules_thread_run_status(QString(MACRO_JobType_Exif_Metadata),Q_FUNC_INFO);

    if(run_status.isEmpty() || run_status == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        message_dialog_object->pub_set_message(MACRO_MSG_INFORMATION,"No source has been processed for '" + QString(MACRO_JobType_Exif_Metadata) + "' yet.");
        message_dialog_object->show();
        return;
    }

    exif_metadata_filters_obj->pub_load_defaults();
    exif_metadata_filters_obj->show();

}


void MainWindow::slot_action_export_case_from_toolbar_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(!bool_launcher_work_completed)
        return;

    export_case_obj->pub_set_display_loading_progress_bar(display_loading_progress_bar_obj);
    export_case_obj->pub_prepare_display();
    export_case_obj->show();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}
