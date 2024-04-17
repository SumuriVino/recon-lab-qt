#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::display_case_tabs(QString tab_text, QString current_case_name,QString task_type, QString itemp2,QString db_path,QString caller_func)
{
    recon_static_functions::app_debug(" tab_text - " + tab_text, Q_FUNC_INFO);
    recon_static_functions::app_debug(" current_case_name - " + current_case_name, Q_FUNC_INFO);
    recon_static_functions::app_debug(" task_type - " + task_type, Q_FUNC_INFO);
    recon_static_functions::app_debug(" itemp2 - " + itemp2, Q_FUNC_INFO);
    recon_static_functions::app_debug(" db_path - " + db_path, Q_FUNC_INFO);
    recon_static_functions::app_debug(" Caller_func - " + caller_func, Q_FUNC_INFO);

    //    progress_bar_fixed_in_statusbar_obj->show();
    //    progress_bar_fixed_in_statusbar_obj->set_status_message("Please wait...");

    // pushButton_combobox_case_close->setEnabled(false);
    pushButton_combobox_category_close->setEnabled(false);
    pushButton_combobox_current_displayed_feature_close->setEnabled(false);

    textedit_detailed_information->clear();
    textedit_optical_character_recognition_information->clear();

    hex_displayer_metadata_obj->pub_set_data(QByteArray(""));
    hex_displayer_metadata_obj->pub_set_source_file_size(0);
    text_viewer_aascii_unicode_obj->pub_clear_textview();
    tree_exif_metadata_display->clear();
    stackwidget_previewer->setCurrentIndex(0);
    tree_detailed_info_apple_metadata->clear();
    maps_metadata_obj->pub_change_location(QString("") , QString(""),QString(""),QString(""));
    content_search_keyword_str.clear();

    QString plugin_clicked = "";
    QString active_case = current_case_name;

    bool bool_case_tab_exist = false;
    int index_case = 0;
    ///-- Stackwidget work..
    for(int i = 0; i < stack_widget_cases_plugins_and_global_items->count(); i++)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));
        if(widget_ptr->pub_get_case_name() == active_case)
        {
            bool_case_tab_exist = true;
            index_case = i;
            break;
        }
    }

    /// iterating through all items of Case Tree for setting
    /// selected item bold
    if(task_type != QString(MACRO_CASE_TREE_Source))
    {
        QTreeWidgetItem *currentItem =  treewidget_case_display_obj->currentItem();
        if(currentItem != NULL)
        {
            QFont font;
            font.setBold(false);
            QTreeWidgetItemIterator m_iter(treewidget_case_display_obj);

            while(*m_iter)
            {
                if ((*m_iter)->text(enum_global_case_tree_display_column_ZERO) == currentItem->text(enum_global_case_tree_display_column_ZERO) && (*m_iter)->text(enum_global_case_tree_display_column_FIRST) == currentItem->text(enum_global_case_tree_display_column_FIRST))
                {
                    font.setBold(true);
                    (*m_iter)->setFont(0,font);
                }
                else
                {
                    font.setBold(false);
                    (*m_iter)->setFont(0,font);
                }

                ++m_iter;
            }
        }
    }


    if(!bool_case_tab_exist)
    {
        //        tabwidget_plugins_and_global_items = new R_Tabwidget;
        //        connect(tabwidget_plugins_and_global_items,SIGNAL(currentChanged(int)),this,SLOT(slot_tabwidget_plugins_and_global_items_currentChanged(int)),Qt::QueuedConnection);

        //        tabwidget_plugins_and_global_items->setStyleSheet(
        //                    "QTabBar::close-button {"
        //                    " image: url(../icons/signs/cancel.png);"
        //                    "subcontrol-origin: padding;"
        //                    "subcontrol-position: right; "
        //                    "width: 13px;"
        //                    " height: 13px;"
        //                    "}"

        //                    "QTabWidget:tab-bar { left: 0.3em;} "

        //                    );

        ///- Tab Widget CloseableRequestConnection.
        //        connect(tabwidget_plugins_and_global_items,SIGNAL(tabCloseRequested(int)),this,SLOT(slot_tabwidget_plugins_close_requested(int)),Qt::QueuedConnection);
        //        tabwidget_plugins_and_global_items->setContentsMargins(0,2,0,0);
        //        tabwidget_cases->addTab(tabwidget_plugins_and_global_items,QIcon("../icons/new_case.png"),active_case);


        //- Initiated From here.
        QString feature_unique_name = QString(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE) + MACRO_RECON_Splitter_1_multiple +MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY + MACRO_RECON_Splitter_1_multiple + MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE;
        QWidget *widget_blank = new QWidget(this);
        Container_QWidget *container = new Container_QWidget(this);
        QVBoxLayout *vbox_layout = new QVBoxLayout;
        vbox_layout->addWidget(widget_blank);
        container->setLayout(vbox_layout);
        container->pub_set_feature_widget_name(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE);
        container->pub_set_category_name(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY);
        container->pub_set_case_name(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE);
        container->pub_set_widget_unique_name(feature_unique_name);
        center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);
        insert_struct_for_display_combobox_and_stackwidget(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE,MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY,MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE);
    }
    else
    {

        //        tabwidget_cases->setCurrentIndex(index_case);
    }

    plugin_clicked = tab_text;

    QString tab_text_without_record = tab_text;
    if(tab_text.contains("("))
    {
        tab_text_without_record = tab_text_without_record.remove(tab_text_without_record.indexOf("("), tab_text_without_record.size()).trimmed();
        tab_text_without_record = tab_text_without_record.trimmed();
    }

    bool bool_plugin_tab_exist = false;
    int index_plugin = 0;
    QString unique_pattrn = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + task_type + MACRO_RECON_Splitter_1_multiple + plugin_clicked;
    if(task_type != QString(MACRO_CASE_TREE_Source))
    {
        /// Records not removed in case of Source
        unique_pattrn = recon_static_functions::remove_record_count_from_feature_name(unique_pattrn);
    }

    for(int i = 0; i < stack_widget_cases_plugins_and_global_items->count(); i++)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        QString widget_unique_name = widget_ptr->pub_get_widget_unique_name();
        if(task_type != QString(MACRO_CASE_TREE_Source))
        {
            /// Records not removed in case of Source
            widget_unique_name =  recon_static_functions::remove_record_count_from_feature_name(widget_unique_name);
        }

        if(widget_unique_name == unique_pattrn)
        {
            bool_plugin_tab_exist = true;
            index_plugin = i;
            break;
        }

    }



    //==================== search and delete already existing in case tree - start ================================//

    if(bool_plugin_tab_exist)
        if(task_type == QString(MACRO_CASE_TREE_Notes_Parent) || task_type == QString(MACRO_CASE_TREE_Tags))
        {
            QTreeWidgetItemIterator m_iter(treewidget_case_display_obj);
            while(*m_iter)
            {
                if ((*m_iter)->text(enum_global_case_tree_display_column_ZERO) == tab_text)
                {
                    bool_plugin_tab_exist = false;


                    if((*m_iter)->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_CASE_TREE_Tags))
                    {
                        if(tab_text_without_record == QString(MACRO_Tag_Name_Bookmarks))
                        {
                            if(bookmarks_search_loader_obj != NULL)
                                delete bookmarks_search_loader_obj;

                            QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Tags + MACRO_RECON_Splitter_1_multiple +tab_text_without_record;

                            comman_remove_feature_from_combobox_and_stackwidget(feature_unique_name);

                            break;
                        }
                        else
                        {
                            if(tag_search_obj != NULL)
                                delete tag_search_obj;

                            QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Tags + MACRO_RECON_Splitter_1_multiple +tab_text_without_record;

                            comman_remove_feature_from_combobox_and_stackwidget(feature_unique_name);

                            break;
                        }
                    }

                    if((*m_iter)->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_CASE_TREE_Notes_Parent))
                    {
                        if(notes_search_loader_obj != NULL)
                            delete notes_search_loader_obj;

                        QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Notes_Parent + MACRO_RECON_Splitter_1_multiple + tab_text_without_record;

                        comman_remove_feature_from_combobox_and_stackwidget(feature_unique_name);

                        break;
                    }

                }

                ++m_iter;
            }
        }
    //==================== search and delete already existing in case tree - end ================================//







    QString metadata_show;

    if(bool_metadata_show)
        metadata_show = "1";
    else
        metadata_show = "0";


    if(task_type == QString(MACRO_COMBOBOX_FEATURE_Case_Info))
    {
        metadata_show = "0";

        if(!bool_plugin_tab_exist)
        {
            case_info_display_obj = new case_info_display(this);
            case_info_display_obj->pub_initialise_case_info_display();


            ///-Stackwidget insertion and display Work..
            QString feature_unique_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString()+ MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Case + MACRO_RECON_Splitter_1_multiple + tab_text;
            Container_QWidget *container = new Container_QWidget(this);
            QVBoxLayout *vbox_layout = new QVBoxLayout;
            vbox_layout->addWidget(case_info_display_obj);
            vbox_layout->setContentsMargins(0,0,0,0);
            container->setLayout(vbox_layout);
            container->pub_set_feature_widget_name(tab_text);
            container->pub_set_category_name(MACRO_CASE_TREE_Case);
            container->pub_set_case_name(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());
            container->pub_set_widget_unique_name(feature_unique_name);

            center_stackwidget_manager_obj->pub_set_container_widget_ptr_in_list(container);


            insert_struct_for_display_combobox_and_stackwidget(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Case,tab_text);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Case,tab_text);
        }

    }
    else if(itemp2 == QString("Parent RECON Plugin"))
    {

        if(bool_metadata_show)
            metadata_show = "1";
        else
            metadata_show = "0";

        if(!bool_plugin_tab_exist)
        {
            display_plugin(plugin_clicked);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),task_type,plugin_clicked);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Mobile_Backup_Parent))
    {
        if(bool_metadata_show)
            metadata_show = "1";
        else
            metadata_show = "0";

        if(tab_text.contains("("))
            tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());

        tab_text = tab_text.trimmed();

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_mobile_backup(tab_text);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Mobile_Backup_Parent,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Disk_Images_Parent))
    {
        if(bool_metadata_show)
            metadata_show = "1";
        else
            metadata_show = "0";

        if(tab_text.contains("("))
            tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());

        tab_text = tab_text.trimmed();

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_disk_images(tab_text);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Disk_Images_Parent,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Artifacts_Keyword_Search))
    {
        if(bool_metadata_show)
            metadata_show = "1";
        else
            metadata_show = "0";

        if(tab_text.contains("("))
            tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());

        tab_text = tab_text.trimmed();

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_keyword_search(tab_text);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Keyword_Search,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_File_Extensions_SubParent_User_Defined_Extensions)
            || task_type == QString(MACRO_CASE_TREE_File_Extensions_SubParent_All_Extensions) || task_type == QString(MACRO_CASE_TREE_MIME_Type)
            || task_type == QString(MACRO_CASE_TREE_File_Size_Parent)
            || task_type == QString(MACRO_CASE_TREE_Apple_Metadata_Parent)
            || task_type == QString(MACRO_CASE_TREE_File_Search)  || task_type == QString(MACRO_CASE_TREE_Content_Search)
            || task_type == QString(MACRO_CASE_TREE_File_Signature_Parent) || task_type == QString(MACRO_CASE_TREE_Hashset)
            || (task_type == QString(MACRO_CASE_TREE_Redefined_Result_Parent) && tab_text_without_record == QString(MACRO_CASE_TREE_Redefined_Result_Category_Locations))
            || task_type == QString(MACRO_CASE_TREE_Exif_Parent) || task_type == QString(MACRO_CASE_TREE_Exif_Metadata_Search)
            || task_type == QString(MACRO_CASE_TREE_Face_Analysis_PARENT)
            || task_type == QString(MACRO_CASE_TREE_Face_Search_PARENT)
            || task_type == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent)
            || task_type == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent)
            || task_type == QString(MACRO_CASE_TREE_Weapons_Parent)
            || task_type == QString(MACRO_CASE_TREE_Fire_Analysis_Parent)
            || task_type == QString(MACRO_CASE_TREE_Griffeye_Data_Parent)
            )
    {
        metadata_show = "1";

        if((task_type == QString(MACRO_CASE_TREE_Hashset))
                || (task_type == QString(MACRO_CASE_TREE_Weapons_Parent))
                || (task_type == QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
                || (task_type == QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
                || (task_type == QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
                || (task_type == QString(MACRO_CASE_TREE_Griffeye_Data_Parent))
                || (task_type == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent)))
        {
            tab_text = tab_text.mid(0, tab_text.indexOf(" ("));
            tab_text = tab_text.trimmed();
        }

        if(!bool_plugin_tab_exist)
        {

            prepare_display_for_filetypes_and_based(tab_text);

        }
        else
        {

            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);

            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(), task_type ,tab_text);

        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Source))
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {

            prepare_display_for_filesystem(tab_text , itemp2 , db_path);

        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);

            QString source_count_name;

            QStringList tmp_list = db_path.split("/", Qt::SkipEmptyParts);
            if(tmp_list.size() > 2)
                source_count_name = tmp_list.at(tmp_list.size() - 2);

            QString dir_path = itemp2;

            struct_GLOBAL_witness_info_source struct_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            if(struct_src_info.source_type_internal == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_File_Internal))
                dir_path = dir_path.remove(struct_src_info.source_name);


            QString src_cnt_name_label = QString(" [" + struct_src_info.source_count_name + "]");
            QString display_name = struct_src_info.source_name + " (" + struct_src_info.source_main_name + ")" + src_cnt_name_label;

            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Source,display_name);

            file_system *file_system_obj = map_file_system_obj.value(source_count_name);
            if(file_system_obj != NULL)
            {
                if(itemp2 == "0")
                {
                    //file_system_obj->pub_insert_please_wait_in_fs_display();
                }
                else
                {
                    current_fs_display_dir_path = dir_path;
                    file_system_obj->make_display_for_specified_path(dir_path);
                }
            }
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Artifacts_Timeline))
    {
        metadata_show = "0";
        show_metadata_widget(false);

        if(!bool_plugin_tab_exist)
        {
            prepare_display_artifacts_timeline_saved(tab_text_without_record);
        }
        else
        {

            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Timeline,tab_text_without_record);
        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_plist_viewer(tab_text , db_path);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Plist_Viewer,tab_text);

        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        metadata_show = "0";
        show_metadata_widget(false);
        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_hex_viewer(tab_text , db_path);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Hex_Viewer,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        metadata_show = "1";
        show_metadata_widget(true);

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_sqlite_viewer(tab_text , db_path);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Sqlite_Viewer,tab_text);

        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        metadata_show = "1";
        show_metadata_widget(true);

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_log_viewer(tab_text, db_path);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Log_Viewer,tab_text);
        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Tags))
    {
        metadata_show = "1";


        if(!bool_plugin_tab_exist)
        {
            if(tab_text_without_record == MACRO_Tag_Name_Bookmarks)
            {
                prepare_display_for_bookmark_search(tab_text_without_record);

            }
            else
            {
                prepare_display_for_tag_search(tab_text_without_record);
            }
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Tags,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Notes_Parent))
    {
        metadata_show = "1";


        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_notes_search(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Notes_Parent,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Redefined_Result_Parent))
    {
        metadata_show = "0";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_redefined_result_full(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Redefined_Result_Parent,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_Redefined_Result_Browser_History) || task_type == QString(MACRO_Redefined_Result_Messenger))
    {

        metadata_show = "1";


        if(!bool_plugin_tab_exist)
        {
            prepare_display_redefined_result_saved(tab_text_without_record,db_path,task_type);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Redefined_Result_Saved_Parent,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Screenshots))
    {
        metadata_show = "1";


        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_screenshots(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Screenshots,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Story_Board))
    {
        metadata_show = "0";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_storyboard(tab_text);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Story_Board,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Ram_Analysis_Saved))
    {

        metadata_show = "1";


        if(!bool_plugin_tab_exist)
        {
            prepare_display_ram_analysis_saved(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Ram_Analysis_Saved,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs) || task_type == QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs))
    {
        metadata_show = "1";


        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_saved_graphs(tab_text_without_record,db_path,task_type);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Graph_Result_Saved,tab_text_without_record);
        }

    }
    else if(task_type == MACRO_CASE_TREE_Artifacts_Timeline_Graph)
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_saved_timeline_graphs(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Artifacts_Timeline_Graph,tab_text_without_record);
        }

    }
    else if(task_type == MACRO_CASE_TREE_Saved_Maps)
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_saved_maps(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Saved_Maps,tab_text_without_record);
        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Email_Files_Parent))
    {

        metadata_show = "0";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_email_files(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Email_Files_Parent,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Carved_Files))
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_carved_files(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Carved_Files,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Carved_Password))
    {
        metadata_show = "1";
        if(!bool_plugin_tab_exist)
        {
            prepare_display_carved_password(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Carved_Password,tab_text_without_record);
        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Carved_Data))
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_carved_data(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Carved_Data,tab_text_without_record);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_registry_viewer(tab_text , db_path);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Registry_Viewer,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_Unified_Logs))
    {
        metadata_show = "1";
        show_metadata_widget(true);

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_unified_logs_viewer(tab_text, db_path);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Unified_Logs,tab_text);
        }
    }
    else if(task_type == QString(MACRO_CASE_TREE_RAM_Images))
    {
        metadata_show = "1";

        if(!bool_plugin_tab_exist)
        {
            prepare_display_for_ram_images(tab_text);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_RAM_Images,tab_text);
        }

    }
    else if(task_type == QString(MACRO_CASE_TREE_Snapshots))
    {
        metadata_show = "1";
        if(!bool_plugin_tab_exist)
        {
            prepare_display_snapshots(tab_text_without_record);
        }
        else
        {
            stack_widget_cases_plugins_and_global_items->setCurrentIndex(index_plugin);
            refresh_combobox_category_for_feature_list(global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Snapshots,tab_text_without_record);
        }
    }



    if(metadata_show == "1")
    {
        show_metadata_widget(true);
    }
    else if(metadata_show == "0")
    {
        show_metadata_widget(false);
    }

    // progress_bar_fixed_in_statusbar_obj->hide();

    // pushButton_combobox_case_close->setEnabled(true);
    pushButton_combobox_category_close->setEnabled(true);
    pushButton_combobox_current_displayed_feature_close->setEnabled(true);

}

void MainWindow::common_treewidget_case_display_single_clicked(QTreeWidgetItem *item, int coloum)
{
    if(item->text(enum_global_case_tree_display_column_FIRST) == MACRO_COMBOBOX_FEATURE_Case_Info)
    {
        display_case_tabs(MACRO_COMBOBOX_FEATURE_Case_Info,item->text(enum_global_case_tree_display_column_SECOND),item->text(enum_global_case_tree_display_column_FIRST),"","",Q_FUNC_INFO);
        show_metadata_widget(false);
    }

    if(item->text(enum_global_case_tree_display_column_FIRST).contains(MACRO_CASE_TREE_Source) && !item->text(enum_global_case_tree_display_column_FIRST).contains(MACRO_CASE_TREE_Parent_Source))
    {
        struct_GLOBAL_witness_info_source struct_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(item->text(enum_global_case_tree_display_column_FIRST));

        QString source_name = struct_src_info.source_name;
        QString source_parent_name = struct_src_info.source_main_name;

        QString source_cnt_name_label = QString(" [" + struct_src_info.source_count_name + "]");
        QString display_name = source_name + " (" + source_parent_name + ")" + source_cnt_name_label;

        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()+ item->text(enum_global_case_tree_display_column_FIRST) + "/file_system.sqlite";

        if(!map_file_system_obj.isEmpty())
        {
            file_system *file_system_obj = map_file_system_obj.value(struct_src_info.source_count_name);
            if(file_system_obj != NULL)
                current_fs_display_dir_path = file_system_obj->pub_get_last_display_path_for_check();
        }

        // This is for stop making disply of same dir on double click on case tree
        if(current_fs_display_dir_path == item->text(enum_global_case_tree_display_column_THIRD)
                && combobox_category_navigation->currentText().trimmed() == QString(MACRO_CASE_TREE_Source)
                && combobox_current_displayed_feature_opened->currentText() == display_name)
        {
            return;
        }

        display_case_tabs(display_name,item->text(enum_global_case_tree_display_column_SECOND),QString(MACRO_CASE_TREE_Source),item->text(enum_global_case_tree_display_column_THIRD),fs_db_path,Q_FUNC_INFO);

    }



}

void MainWindow::slot_treewidget_case_display_expanded(QTreeWidgetItem *item_tree)
{

    if(item_tree == NULL || item_tree->parent() == NULL)
        return;


    if(item_tree->text(enum_global_case_tree_display_column_SECOND) == QString(MACRO_CASE_TREE_Parent_Source) && item_tree->childCount() == 1) // because we have statically added one blank child
    {
        if(item_tree->child(enum_global_case_tree_display_column_ZERO)->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_File_System_Blank_Tree_Item_Title))
        {

            ///remove blank child first
            item_tree->removeChild(item_tree->child(enum_global_case_tree_display_column_ZERO));
            QString source_count_name = item_tree->text(enum_global_case_tree_display_column_FIRST);
            QString file_parent_path  = item_tree->text(enum_global_case_tree_display_column_THIRD);


            QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                    + source_count_name + QString("/file_system.sqlite");



            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            source_db.setDatabaseName(fs_db_path);
            if(!source_db.open())
            {
                recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
                recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
                return;
            }

            struct_GLOBAL_witness_info_source target_info_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString command_rec = QString("Select INT from files Where file_parent_path = ?");
            QStringList rec_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(command_rec , QStringList(file_parent_path), 0 , source_db, Q_FUNC_INFO);

            for(int count = 0; count < rec_list.size(); count++)
            {
                if(count % 100 == 0)
                    QCoreApplication::processEvents();

                QString record_no = rec_list.at(count);
                if(!recon_helper_standard_obj->is_it_dir_record_no_by_dbreference(record_no,source_db, "files", target_info_obj.source_count_name ,Q_FUNC_INFO))
                    continue;

                QString command_file = QString("Select filepath from files Where INT = ?");
                QString dir_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_file , QStringList(record_no), 0 , source_db, Q_FUNC_INFO);
                QString dir_name = QFileInfo(dir_path).fileName();
                QIcon icon_path = recon_helper_standard_obj->get_qicon_of_record_no_by_dbreference(record_no,source_db, "files", target_info_obj.source_count_name ,Q_FUNC_INFO);

                QString fs_dir_path = dir_path;
                //                if(!fs_dir_path.isEmpty() && !fs_dir_path.endsWith("/"))
                //                    fs_dir_path.append("/");

                //                fs_dir_path.append(QString("%"));


                if(fs_dir_path.trimmed() != QString("/") && fs_dir_path.endsWith("/"))
                    fs_dir_path.chop(1);

                QString total_records = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(QString("Select count(*) from files Where file_parent_path = ?"), QStringList(fs_dir_path), 0, source_db, Q_FUNC_INFO);

                QString title_str = dir_name + " (" + total_records + ")";

                QTreeWidgetItem *item_val = new QTreeWidgetItem;
                item_val->setIcon(enum_global_case_tree_display_column_ZERO, icon_path);
                item_val->setText(enum_global_case_tree_display_column_ZERO, title_str);
                item_val->setText(enum_global_case_tree_display_column_FIRST, target_info_obj.source_count_name);
                item_val->setText(enum_global_case_tree_display_column_SECOND, QString(MACRO_CASE_TREE_Parent_Source));
                item_val->setText(enum_global_case_tree_display_column_THIRD, dir_path);


                // for virtual arrow in case tree directory name
                QTreeWidgetItem *item_blank_child_val = new QTreeWidgetItem;
                item_blank_child_val->setIcon(enum_global_case_tree_display_column_ZERO, icon_path);
                item_blank_child_val->setText(enum_global_case_tree_display_column_ZERO, QString(""));
                item_blank_child_val->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_File_System_Blank_Tree_Item_Title));
                item_val->addChild(item_blank_child_val);

                item_tree->addChild(item_val);

            }

            source_db.close();
        }
    }

}

void MainWindow::slot_treewidget_case_display_double_clicked(QTreeWidgetItem *item, int coloum)
{
    common_treewidget_case_display_double_clicked(item,coloum);

    activateWindow();
}

void MainWindow::slot_treewidget_case_display_single_clicked(QTreeWidgetItem *item, int coloum)
{
    common_treewidget_case_display_single_clicked(item,coloum);
}

void MainWindow::slot_action_examiner_space_triggered()
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    if(!bool_launcher_work_completed)
        return;

    examiner_space_obj->pub_set_essential();
    examiner_space_obj->pub_prepare_display();
    examiner_space_obj->show();

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

void MainWindow::slot_action_screenshot_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    screenshots_obj->clear_fields();
    screenshots_obj->set_screenshot();


    //    if(combobox_current_displayed_feature_opened->currentText() == MACRO_CASE_TREE_Screenshots)
    //    {
    //        QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_category_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_current_displayed_feature_opened->currentText();
    //        comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);
    //    }

    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Screenshots + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Screenshots;
    comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);

    action_screenshot->setDisabled(true);
    if(!screenshots_obj->pub_screenshot_show())
    {
        action_screenshot->setDisabled(false);
    }else
    {
        screenshots_obj->show();
    }

}

void MainWindow::slot_show_location_in_offline_maps(QString latitude , QString longitude, QString filepath, QString source_count_name)
{
    return;

    maps_metadata_obj->pub_change_location(latitude , longitude, filepath, source_count_name);
}

void MainWindow::slot_action_add_source_triggered()
{

    if(recon_case_obj->pub_is_file_system_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not add new source until previous source(s) have been added to case and apple timestamps have been extracted in case of Apple Filesystems.");
        return;
    }

    if(snapshots_obj->pub_is_snapshot_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not add new source until snapshots processing isn't finished.");
        return;
    }

    if(recon_case_obj->pub_get_mime_type_analysis_run_status())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not add new source until previous source(s) have been added.");
        return;
    }

    if(!bool_launcher_work_completed)
        return;

    comman_tree_add_source();
}

void MainWindow::slot_action_run_plugin_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    recon_case_obj->slot_right_click_run_plugins_clicked();
}

void MainWindow::slot_action_keyword_search_triggered()
{

    if(!bool_launcher_work_completed)
        return;

    if(recon_case_obj->pub_is_artifacts_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please wait task 'Artifacts Extraction' is running.");
        return;
    }

    recon_case_obj->pub_keyword_search_clicked();
}

void MainWindow::slot_action_artifacts_timeline_full_triggered()
{
    if(!bool_launcher_work_completed)
        return;


    if(recon_case_obj->pub_is_artifacts_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Please wait task 'Artifacts Extraction' is running.");
        return;
    }


    remove_category_page_from_stackwidget(MACRO_CASE_TREE_Artifacts_Timeline);

    show_metadata_widget(false);
    recon_case_obj->slot_right_clicked_artifacts_timeline_full();
}

void MainWindow::slot_action_global_report_triggered()
{
    if(!bool_launcher_work_completed)
        return;

    recon_case_obj->slot_right_click_global_report_clicked();
}

void MainWindow::slot_action_new_case_triggered()
{
}

void MainWindow::slot_openwith_after_sendto_bucket_master_interface(QString tab_text,QString viewer_name)
{
    recon_static_functions::app_debug("slot_openwith_after_sendto_bucket_master_interface START ",Q_FUNC_INFO);

    if(viewer_name != MACRO_CASE_TREE_Sqlite_Viewer)
    {
        recon_static_functions::app_debug("sqlite viewer doesn't math viewer name arg",Q_FUNC_INFO);
        return;
    }


    int enum_bucket = enum_global_case_tree_display_row_BUCKET ;

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Bucket))
        {
            for(int itr = 0; itr < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->childCount(); itr++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Sqlite_Viewer)
                {
                    for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->childCount(); j++)
                    {
                        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->child(j)->text(enum_global_case_tree_display_column_ZERO) == tab_text)
                        {
                            common_treewidget_case_display_double_clicked(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->child(j),enum_global_case_tree_display_column_ZERO);
                            break;
                        }
                    }
                }
            }
        }
    }

    recon_static_functions::app_debug("slot_openwith_after_sendto_bucket_master_interface END ",Q_FUNC_INFO);
}

void MainWindow::slot_launcher_load_Case_clicked(QString path)
{

    case_wizard_object->hide();

    emit signal_load_case_from_launcher_clicked(path);

    task_status_window_obj->hide();
    this->show();
}

void MainWindow::slot_tree_right_click_close_case()
{
    common_tree_close_case();
}
void MainWindow::slot_tree_right_click_add_source()
{
    comman_tree_add_source();
}
void MainWindow::comman_tree_add_source()
{
    add_new_source *add_new_source_obj = new add_new_source(this);
    connect(add_new_source_obj,SIGNAL(signal_update_recon_case(QList<struct_GLOBAL_witness_info_root>)),this,SLOT(slot_toolbar_add_source(QList<struct_GLOBAL_witness_info_root>)));
    add_new_source_obj->show();
}

void MainWindow::common_tree_close_case()
{
    QStringList plugin_list;
    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_ARTIFACTS_KEYWORD_SEARCH)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Artifacts_Keyword_Search))
        {
            remove_global_search_entry_from_tree();
        }



        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Source))
        {
            remove_media_path_entry_from_tree();
        }

    }


    ///--Delete the Stack Items for the Case..

    QString case_name = treewidget_case_display_obj->takeTopLevelItem(0)->text(enum_global_case_tree_display_column_ZERO).remove(MACRO_CASE_TREE_Case);

    remove_case_from_stackwidget(case_name);

    delete treewidget_case_display_obj->takeTopLevelItem(0);
}
void MainWindow::slot_combobox_feature_current_text_changed(QString arg1)
{
    if(combobox_current_displayed_feature_opened->currentIndex() == 0)
        return;

    if(bool_insert_data_in_stack)
        return;

    if(bool_combobox_items_added)
        return;

    slot_refresh_widgets();

    QString unique_pattern = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_category_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_current_displayed_feature_opened->currentText();

    //-Set stackwidget index to current by unique name.
    set_stackwidget_display_by_unique_name(unique_pattern);

    if(arg1 == MACRO_COMBOBOX_FEATURE_Case_Info)
    {
        show_metadata_widget(false);
        show_metadata_widget(false);
    }

    //-To update the List For the Navigation.
    if(!bool_update_navigation_list)
    {
        if(arg1 != MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE)
            emit signal_update_navigation_list_on_combobox_change(unique_pattern);
    }

    set_focus_on_case_treewidget_item(unique_pattern);
}


void MainWindow::slot_clear_all_metadata()
{
    bool_metadata_info_already_cleared = true;

    textedit_detailed_information->clear();
    textedit_optical_character_recognition_information->clear();
    hex_displayer_metadata_obj->pub_set_data(QByteArray(""));
    hex_displayer_metadata_obj->pub_set_source_file_size(0);
    text_viewer_aascii_unicode_obj->pub_clear_textview();
    strings_viewer_obj->pub_clear_data();
    tree_exif_metadata_display->clear();
    tree_detailed_info_apple_metadata->clear();
    maps_metadata_obj->pub_change_location(QString("") , QString(""),QString(""),QString(""));

    av_preview_object->stop_media();
    stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_BLANK);
    //  quick_look_obj->pub_set_preview_filepath("");



    detailed_info_pane_file_list.clear();
    detailed_info_pane_file.clear();
    detailed_info_pane_file_source_count_name.clear();
    detailed_info_pane_file_record_no.clear();
    detailed_info_pane_file_table_name.clear();
    detailed_info_pane_file_plugin_name.clear();


}

void MainWindow::slot_set_metadata_info(struct_global_set_metadata_info m_obj)
{

    slot_clear_all_metadata();

    st_metadata_obj = m_obj;
    bool_metadata_info_already_cleared = false;
    set_metadata_info_detailed_info();


    QString source_count_name = m_obj.source_count_name;
    QString rec_no_str        = m_obj.record_no;
    QString db_path           = m_obj.db_file_path;
    QString tab_name_str      = m_obj.tab_name;
    QString plugin_name = m_obj.plugin_name;


    if(plugin_name == QString(MACRO_Plugin_Name_File_System)  || plugin_name == QString(MACRO_Plugin_Name_Log_Viewer))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_file_system_by_db_path(rec_no_str,db_path,source_count_name);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Carved_Files))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_carved_files_by_db_path(rec_no_str,db_path,source_count_name);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Screenshots))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_screenshots(rec_no_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_timeline_graphs(rec_no_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_saved_maps(rec_no_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_messenger_saved_graphs(rec_no_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_browser_histroy_saved_graphs(rec_no_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Strings_Viewer))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_string_viewer(rec_no_str,source_count_name);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Text_Viewer))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_text_viewer(rec_no_str,source_count_name);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Hex_Viewer))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_hex_viewer(rec_no_str,source_count_name);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Snapshots))
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_snapshots_by_db_path(rec_no_str,m_obj.tab_name,m_obj.db_file_path,source_count_name);
    }
    else
    {
        st_globl_recon_file_info_obj = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(plugin_name , tab_name_str, rec_no_str,db_path);
    }

    m_obj.final_file_path = st_globl_recon_file_info_obj.complete_file_path;
    m_obj.bool_file_from_result = st_globl_recon_file_info_obj.bool_file_from_result;

    //==========prepare quicklook path-Start=========
    struct_GLOBAL_witness_info_source struct_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString filepath_str = st_globl_recon_file_info_obj.display_file_path;
    QString cmplt_filepath_str = st_globl_recon_file_info_obj.complete_file_path;

    if(struct_target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || struct_target_source_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        QString link_filename_path = get_quicklook_link_path_for_ios_quicklook(filepath_str, cmplt_filepath_str , rec_no_str, source_count_name);
        m_obj.quicklook_file_path = link_filename_path;
    }
    else
    {
        m_obj.quicklook_file_path = cmplt_filepath_str;
    }
    //==========prepare quicklook path-End=========


    if(QString(m_obj.final_file_path).isEmpty() || !QFileInfo(m_obj.final_file_path).exists())
        return;



    st_metadata_obj = m_obj;

    set_metadata_info_previewers();
    set_metadata_info_quicklook();
    set_metadata_information();

}

void MainWindow::set_metadata_information()
{
    if(st_metadata_obj.bool_file_from_result && (st_metadata_obj.plugin_name != QString(MACRO_Plugin_Name_Carved_Files)))
        return;

    if(bool_metadata_info_already_cleared)
        return;

    if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Detailed_Information)
    {
        set_metadata_info_detailed_info();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Optical_Character_Recognition)
    {
        set_metadata_info_optical_character_recognition();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Apple_Meatadta)
    {
        set_metadata_info_apple_metadata();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Exif_Meatadta)
    {
        set_metadata_info_exif_metadata();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Text_View)
    {
        set_metadata_info_text_viewer();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Hex_View)
    {
        set_metadata_info_hex_viewer();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Strings)
    {
        set_metadata_info_strings_viewer();
    }
    else if(tabwidget_metadata_viewer->tabText(tabwidget_metadata_viewer->currentIndex()) == MACRO_Metadata_TabName_Maps)
    {
        set_metadata_info_maps();
    }



}


void MainWindow::set_metadata_info_text_viewer()
{
    QString file_path = st_metadata_obj.final_file_path;
    QString source_count_name = st_metadata_obj.source_count_name;
    QString record_no = st_metadata_obj.record_no;
    QString content_search_text = st_metadata_obj.content_searched_text;


    text_viewer_aascii_unicode_obj->pub_clear_textview();
    strings_viewer_obj->pub_clear_data();

    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(st_metadata_obj.plugin_name))
        return ;


    if(!QFileInfo(file_path).exists())
        return;


    text_viewer_aascii_unicode_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    text_viewer_aascii_unicode_obj->pub_set_source_count_name(source_count_name);
    text_viewer_aascii_unicode_obj->pub_set_essentials();
    text_viewer_aascii_unicode_obj->pub_hide_search_navigate_buttons(true);
    text_viewer_aascii_unicode_obj->pub_set_global_manager_obj(global_connection_manager_obj);


    if(!content_search_text.isEmpty())
        text_viewer_aascii_unicode_obj->pub_hide_search_navigate_buttons(false);

    text_viewer_aascii_unicode_obj->pub_load_file_in_text_view(file_path, record_no);
    text_viewer_aascii_unicode_obj->pub_highlight_keyword_hit(content_search_text);


}


void MainWindow::set_metadata_info_maps()
{
    QString file_path = st_metadata_obj.final_file_path;
    QString source_count_name = st_metadata_obj.source_count_name;
    QString plugin_name = st_metadata_obj.plugin_name;

    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name))
        return ;

    if(!QFileInfo(file_path).exists())
        return;


    if(plugin_name == QString(MACRO_Plugin_Name_Hex_Viewer))
        return;

    maps_metadata_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    maps_metadata_obj->pub_change_location("" , "", file_path, source_count_name);

}

void MainWindow::set_metadata_info_exif_metadata()
{
    QString file_path = st_metadata_obj.final_file_path;
    QString source_count_name = st_metadata_obj.source_count_name;
    QString record_no = st_metadata_obj.record_no;
    QString plugin_name = st_metadata_obj.plugin_name;
    QString tab_name = st_metadata_obj.tab_name;
    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);
    QString display_file_path  = st_globl_recon_file_info_obj.display_file_path;


    tree_exif_metadata_display->clear();
    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name))
        return ;

    if(!QFileInfo(file_path).exists())
        return;


    if(plugin_name == QString(MACRO_Plugin_Name_Hex_Viewer))
        return;

    exif_interface_obj->pub_set_result_dir(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString());
    exif_interface_obj->pub_set_exif_data_in_case_tree_display(file_path,record_no , st_globl_recon_file_info_obj.db_path , tree_exif_metadata_display,plugin_name, table_name, source_count_name,display_file_path);

}

void MainWindow::set_metadata_info_apple_metadata()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    tree_detailed_info_apple_metadata->clear();
    if(st_globl_recon_file_info_obj.recon_file_from.toInt() == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result)
        return;

    QString source_count_name = st_metadata_obj.source_count_name;

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    if(struct_source_info.file_system_type != MACRO_FILE_SYSTEM_TYPE_HFS
            && struct_source_info.file_system_type != MACRO_FILE_SYSTEM_TYPE_APFS)
    {
        return;
    }

    QString file_path = st_metadata_obj.final_file_path;
    QString record_no = st_metadata_obj.record_no;
    QString plugin_name = st_metadata_obj.plugin_name;
    QString tab_name = st_metadata_obj.tab_name;
    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);


    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name))
        return ;


    if(!QFileInfo(file_path).exists())
        return;


    if(plugin_name == QString(MACRO_Plugin_Name_Hex_Viewer) || plugin_name == QString(MACRO_Plugin_Name_Carved_Files))
        return;


    QString destination_db_path  = st_globl_recon_file_info_obj.db_path;
    QString command = QString("select bookmarked_extended_attributes from " + table_name + " where INT = ") + record_no;
    QString bookmarked_items = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,destination_db_path,Q_FUNC_INFO);

    QStringList ext_attribute_list;

    process_apple_metadata = new QProcess(this);

    QString file_system_apple_metadata = global_narad_muni_class_obj->get_field(MACRO_NARAD_Extracted_Content_Location_Apple_Metadata_Reader_Apple_Metadata_QString).toString();

    QString tmp_apple_timestamps_db_path = file_system_apple_metadata + struct_source_info.source_count_name + QString("/apple_metadata.sqlite");
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    struct_global_desktop_info pwd_obj = global_desktop_info_class_obj->pub_get_desktop_info(Q_FUNC_INFO);

    QString mac_version_examiner_machine = pwd_obj.os_version_full;
    QString mac_version_source = struct_source_info.os_version_extended;

    QString disp_filepath = st_globl_recon_file_info_obj.display_file_path;
    ext_attribute_list = recon_helper_standard_obj->get_apple_metadata_from_result_apple_metadata_db(disp_filepath,struct_source_info.source_count_name);

    if(ext_attribute_list.isEmpty())
        return;

    QString m_file_name = QFileInfo(st_globl_recon_file_info_obj.display_file_path).fileName();

    QTreeWidgetItem *root_item = new QTreeWidgetItem();
    root_item->setText(enum_mainwindow_apple_metadata_KEY, m_file_name);
    root_item->setText(enum_mainwindow_apple_metadata_Value, "");
    tree_detailed_info_apple_metadata->addTopLevelItem(root_item);

    for(int i = 0 ; i < ext_attribute_list.size() ; i++)
    {
        QStringList keyvalue_list = ext_attribute_list.at(i).split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        QString key,value;
        if(keyvalue_list.size() > 1)
        {
            key = keyvalue_list.at(0);
            value = keyvalue_list.at(1);
        }

        if(value.trimmed().isEmpty())
            continue;



        QTreeWidgetItem *tree_child = new QTreeWidgetItem();
        tree_child->setText(enum_mainwindow_apple_metadata_KEY,key);
        if(bookmarked_items.contains(key+" :"))
            tree_child->setCheckState(enum_mainwindow_apple_metadata_KEY,Qt::Checked);
        else
            tree_child->setCheckState(enum_mainwindow_apple_metadata_KEY,Qt::Unchecked);


        if(value.contains("\n"))
        {
            QStringList sub_values_list = value.split("\n", Qt::SkipEmptyParts);

            int count = 0;
            while(count < sub_values_list.size())
            {
                QString sub_value = sub_values_list.at(count);

                if(count == 0)
                {
                    tree_child->setText(enum_mainwindow_apple_metadata_Value, sub_value);
                    tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Table_Name, table_name);
                    tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Db_Path, destination_db_path);
                    tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Record_no, record_no);
                    tree_child->setText(enum_mainwindow_apple_metadata_File_Path, file_path);
                    tree_child->setText(enum_mainwindow_apple_metadata_File_Path, st_globl_recon_file_info_obj.display_file_path);


                    root_item->addChild(tree_child);
                }
                else
                {
                    QTreeWidgetItem *tree_sub_child = new QTreeWidgetItem;
                    tree_sub_child->setText( enum_mainwindow_apple_metadata_Value, sub_value);

                    tree_child->addChild(tree_sub_child);

                }
                count++;
            }
        }
        else
        {

            tree_child->setText(enum_mainwindow_apple_metadata_Value, value);
            tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Table_Name, table_name);
            tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Db_Path, destination_db_path);
            tree_child->setText(enum_mainwindow_apple_metadata_Plugin_Record_no, record_no);
            tree_child->setText(enum_mainwindow_apple_metadata_File_Path, file_path);

            root_item->addChild(tree_child);
        }

    }


    tree_detailed_info_apple_metadata->expandAll();
    tree_detailed_info_apple_metadata->setColumnWidth(enum_mainwindow_apple_metadata_KEY,300);
    tree_detailed_info_apple_metadata->setColumnWidth(enum_mainwindow_apple_metadata_Value,400);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Table_Name,true);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Db_Path,true);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_Plugin_Record_no,true);
    tree_detailed_info_apple_metadata->setColumnHidden(enum_mainwindow_apple_metadata_File_Path,true);

    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);

}

void MainWindow::set_metadata_info_detailed_info()
{
    QString detailed_info = st_metadata_obj.detailed_info;
    QString searched_text = st_metadata_obj.display_searched_text;


    textedit_detailed_information->clear();

    textedit_detailed_information->appendHtml(detailed_info);
    textedit_detailed_information->moveCursor(QTextCursor::Start);
    highlight_search_text(searched_text);

}

void MainWindow::set_metadata_info_optical_character_recognition()
{
    recon_static_functions::app_debug("Start", Q_FUNC_INFO);

    QString record_no = st_metadata_obj.record_no;
    QString source_count_name  = st_metadata_obj.source_count_name;
    QString searched_text = st_metadata_obj.display_searched_text;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QString metadata_str;

    QString cmd_ocr = "SELECT ocr_text from files WHERE INT = ?";
    QStringList bindValue;
    bindValue << record_no;

    QString ocr_text = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_ocr, bindValue, 0, fs_db_path, Q_FUNC_INFO);

    if(!ocr_text.isEmpty())
    {
        metadata_str += "<b>" + QObject::tr("OCR Text") + ":</b>" +QString("\t") + ocr_text + "\n<br>";
        metadata_str += "<br>";
    }

    textedit_optical_character_recognition_information->clear();
    textedit_optical_character_recognition_information->appendHtml(metadata_str);
    textedit_optical_character_recognition_information->moveCursor(QTextCursor::Start);

    highlight_search_text(searched_text);
}

void MainWindow::set_metadata_info_previewers()
{
    QString file_path = st_metadata_obj.final_file_path;
    QString plugin_name = st_metadata_obj.plugin_name;

    QString display_file_path = st_globl_recon_file_info_obj.display_file_path;

    if(plugin_name == QString(MACRO_Plugin_Name_Hex_Viewer))
        return;

    av_preview_object->stop_media();
    stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_BLANK);

    QFileInfo info(file_path);

    if(!info.exists() || file_path.trimmed().isEmpty())
        return;

    QMimeType mime_type = mime_database_obj.mimeTypeForFile(file_path, QMimeDatabase::MatchContent);
    QString mime_name = mime_type.name();
    QStringList mime_extensions_list = mime_type.suffixes();

    bool bool_suffix_was_blank = false;
    QString suffix = recon_helper_standard_obj->mining_suffix(file_path, Q_FUNC_INFO);
    suffix = suffix.trimmed();


    if(suffix.isEmpty())
    {

        if(mime_name.trimmed().startsWith("text/")
                || mime_name.trimmed().startsWith("audio/") || mime_name.trimmed().startsWith("video/")
                || recon_helper_standard_obj->check_is_it_image_file(file_path,"", Q_FUNC_INFO))
        {
            if(mime_extensions_list.size() > 0)
            {
                suffix = mime_extensions_list.at(0);
                suffix.prepend(".");
            }
            bool_suffix_was_blank = true;

            file_path = create_link_path_for_preview(suffix,file_path);

        }
    }

    if(bool_suffix_was_blank)
    {
        mime_type = mime_database_obj.mimeTypeForFile(file_path, QMimeDatabase::MatchContent);
        mime_name = mime_type.name();
        mime_extensions_list = mime_type.suffixes();
    }


    if(recon_helper_standard_obj->check_is_it_image_file(file_path,display_file_path, Q_FUNC_INFO))
    {
        if(info.size() > (100 * 1024 * 1024)) // 100MB 104857600 bytes
            return;

        stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_IMAGE);
        image_preview_object->set_image_by_path(file_path);
    }
    else if(mime_name.startsWith("video/"))
    {

        stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_Audio_Video);
        av_preview_object->load_source(file_path);
    }
    else if(mime_name.startsWith("audio/"))
    {

        stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_Audio_Video);
        av_preview_object->load_source(file_path);
    }


}

QString MainWindow::create_link_path_for_preview(QString extension_str, QString file_path)
{
    QString link_file_path;
    QString link_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Preview_Links_In_Result_QString).toString();

    bool dir_exist = false;
    QDir link_dir(link_dir_path);
    if(!link_dir.exists())
    {
        dir_exist = link_dir.mkpath(link_dir_path);
    }
    else
        dir_exist = true;

    if(dir_exist)
    {
        link_file_path = link_dir_path + QFileInfo(file_path).fileName() + extension_str;

        if(QFileInfo(link_file_path).exists())
            QFile::remove(link_file_path);

        QFile::link(file_path, link_file_path);
    }
    else
        link_file_path = file_path;

    return link_file_path.trimmed();
}

void MainWindow::set_metadata_info_quicklook()
{

    QString file_path = st_metadata_obj.quicklook_file_path;

    QString plugin_name = st_metadata_obj.plugin_name;

    if(plugin_name == QString(MACRO_Plugin_Name_Hex_Viewer))
        return;


    quick_look_file_preview_path = file_path;

    if(bool_is_lab_running_from_root)
    {
        socket_quicklook->write(file_path.toLocal8Bit().data());
        socket_quicklook->flush();
        return;
    }


    if(quick_look_obj->pub_get_preview_closed_status())
    {
        return;
    }

    quick_look_obj->pub_set_preview_filepath(file_path);

}

void MainWindow::set_metadata_info_strings_viewer()
{
    QString file_path = st_metadata_obj.final_file_path;
    QString source_count_name = st_metadata_obj.source_count_name;
    QString record_no = st_metadata_obj.record_no;
    QString content_search_text = st_metadata_obj.content_searched_text;


    strings_viewer_obj->pub_clear_data();

    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(st_metadata_obj.plugin_name))
        return ;


    if(!QFileInfo(file_path).exists())
        return;

    strings_viewer_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);

    strings_viewer_obj->pub_set_source_count_name(source_count_name);
    strings_viewer_obj->pub_set_essentials();
    strings_viewer_obj->pub_hide_search_navigate_buttons(true);
    strings_viewer_obj->pub_set_global_manager_obj(global_connection_manager_obj);

    if(!content_search_text.isEmpty())
        strings_viewer_obj->pub_hide_search_navigate_buttons(false);

    strings_viewer_obj->pub_show_file_data(file_path, record_no);
    strings_viewer_obj->pub_set_highlight_keyword(content_search_text);

}

void MainWindow::set_metadata_info_hex_viewer()
{

    QString file_path           = st_metadata_obj.final_file_path;
    QString source_count_name   = st_metadata_obj.source_count_name;
    QString content_search_text = st_metadata_obj.content_searched_text;

    //hex_viewer_metadata_obj->setdata("");
    hex_displayer_metadata_obj->pub_set_data(QByteArray(""));
    hex_displayer_metadata_obj->pub_set_source_file_size(0);


    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(st_metadata_obj.plugin_name))
        return ;

    QFileInfo info_file(file_path);
    if(!info_file.exists())
        return;

    QString display_file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(st_globl_recon_file_info_obj.display_file_path);
    QString hex_display_nm;
    if(!display_file_name.isEmpty())
        hex_display_nm = display_file_name + " [" + st_metadata_obj.plugin_name + " " + st_metadata_obj.record_no + "]";


    st_hex_viewer_open_file_data_obj.hex_viewer_display_name = hex_display_nm;

    st_hex_viewer_open_file_data_obj.bool_is_file_in_bucket = false;
    st_hex_viewer_open_file_data_obj.display_file_path   = st_globl_recon_file_info_obj.display_file_path;
    st_hex_viewer_open_file_data_obj.record_no_str       = st_metadata_obj.record_no;
    st_hex_viewer_open_file_data_obj.source_count_name   = st_metadata_obj.source_count_name;
    st_hex_viewer_open_file_data_obj.plugin_name_str     = st_metadata_obj.plugin_name;
    st_hex_viewer_open_file_data_obj.plugin_tab_name_str = st_metadata_obj.tab_name;
    st_hex_viewer_open_file_data_obj.hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
    st_hex_viewer_open_file_data_obj.recon_filefrom      = st_globl_recon_file_info_obj.recon_file_from;
    st_hex_viewer_open_file_data_obj.recon_file_infopath = st_globl_recon_file_info_obj.recon_file_info_path;
    st_hex_viewer_open_file_data_obj.complete_file_path  = st_globl_recon_file_info_obj.complete_file_path;

    hex_displayer_metadata_obj->pub_set_global_connection_manager(global_connection_manager_obj);
    hex_displayer_metadata_obj->pub_set_hex_viewer_open_file_data_obj(st_hex_viewer_open_file_data_obj);
    hex_displayer_metadata_obj->pub_set_source_file_size(info_file.size());
    hex_displayer_metadata_obj->pub_set_source_block_size(512);
    hex_displayer_metadata_obj->pub_set_file_path(file_path, st_hex_viewer_open_file_data_obj.display_file_path);
    hex_displayer_metadata_obj->pub_highlight_keyword(content_search_text);


    //    QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();

    //    hex_viewer_metadata_obj->pub_set_recon_file_info(st_globl_recon_file_info_obj);
    //    hex_viewer_metadata_obj->set_file_path_and_db_path(file_path, db_path);
    //    hex_viewer_metadata_obj->pub_set_essentials();
    //    hex_viewer_metadata_obj->pub_set_global_connection_manager(global_connection_manager_obj);
    //    hex_viewer_metadata_obj->pub_set_source_count_name(source_count_name);
    //    hex_viewer_metadata_obj->pub_set_comboBox_block_list();
    //    hex_viewer_metadata_obj->set_file_data(file_path);

    //hex_viewer_metadata_obj->pub_highlight_keyword(content_search_text);

    //    hex_viewer_metadata_obj->pub_set_called_from_metadata_display();

    //    hex_viewer_metadata_obj->pub_set_plugin_name(st_metadata_obj.plugin_name);
    //    hex_viewer_metadata_obj->pub_set_tab_name(st_metadata_obj.tab_name);
    //    hex_viewer_metadata_obj->pub_set_record_no(st_metadata_obj.record_no);

}


void MainWindow::slot_global_item_clicked(QString searched_text,QString data, QString current_media, QString source_count_name)
{
    textedit_detailed_information->clear();
    av_preview_object->stop_media();
    stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_BLANK);

    textedit_detailed_information->appendHtml(data);

    if(current_media != QString(""))
    {
        QFileInfo info(current_media);
        if(info.exists())
        {
            QString previewed_path = "";
            previewed_path = current_media;

            QString file_mime_type = mime_database_obj.mimeTypeForFile(previewed_path).name();


            if(recon_helper_standard_obj->check_is_it_image_file(previewed_path,"", Q_FUNC_INFO))
            {
                stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_IMAGE);
                image_preview_object->set_image_by_path(previewed_path);
            }
            else if(file_mime_type.startsWith("video/") || file_mime_type.startsWith("audio/"))
            {
                stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_Audio_Video);
                av_preview_object->load_source(previewed_path);
            }
        }
    }

    textedit_detailed_information->moveCursor(QTextCursor::Start);


    highlight_search_text(searched_text);

}

void MainWindow::common_treewidget_case_display_double_clicked(QTreeWidgetItem *item, int coloum)
{

    if(item->isDisabled())
    {
        return;
    }

    if(item->text(2).contains("Parent") || item->text(1) == MACRO_COMBOBOX_FEATURE_Case_Info)
    {
        return;
    }

    QString sqlite_path;
    QString task_type = item->parent()->text(0);
    if(item->parent()->text(0) == QString(MACRO_CASE_TREE_Plist_Viewer))
    {
        sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
    }
    else if(item->parent()->text(0) == QString(MACRO_CASE_TREE_Hex_Viewer))
    {
        sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";
        prepare_hex_viewer_display_structure_data(item , sqlite_path);
    }
    else if(item->parent()->text(0) == QString(MACRO_CASE_TREE_Sqlite_Viewer))
    {
        sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";
    }
    else if(item->parent()->text(0) == QString(MACRO_CASE_TREE_Registry_Viewer))
    {
        sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Registry_Viewer_In_Result_QString).toString() + "registry_viewer_index.sqlite";
    }
    else if(item->parent()->text(0) == QString(MACRO_CASE_TREE_Log_Viewer))
    {
        sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";
    }

    if(item->parent()->parent()->text(0) == MACRO_CASE_TREE_Artifacts)
    {
        task_type = MACRO_CASE_TREE_Artifacts;
    }
    else if(item->parent()->text(1) == QString(MACRO_CASE_TREE_Hashset))
    {
        task_type = QString(MACRO_CASE_TREE_Hashset);
    }


    if(item->text(1) == QString(MACRO_CASE_TREE_MIME_Type))
        task_type = item->text(1).trimmed();

    if(item->text(1) == QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
        task_type = item->text(1).trimmed();

    if(item->text(1) == MACRO_CASE_TREE_Redefined_Result_Saved_Parent)
    {
        QString tab_text = item->text(0);
        if(tab_text.contains( "("))
            tab_text = tab_text.remove(tab_text.lastIndexOf(" ("), tab_text.size());

        tab_text = tab_text.trimmed();

        QString tab_db_name = tab_text;

        tab_db_name.replace(" ","_");
        if(task_type == MACRO_Redefined_Result_Messenger)
            sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + tab_db_name + ".sqlite";

        if(task_type == MACRO_Redefined_Result_Browser_History)
            sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + tab_db_name + ".sqlite";

    }
    else if(item->text(1) == MACRO_CASE_TREE_Graph_Result_Saved)
    {
        task_type = item->text(0);
        if(task_type.contains( "("))
            task_type = task_type.remove(task_type.lastIndexOf(" ("), task_type.size());

        task_type = task_type.trimmed();

        if(task_type == MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs)
            sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";

        if(task_type == MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs)
            sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString()  + "Browser_History.sqlite";
    }
    else if(item->text(1) == MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
    {
        task_type = item->text(0);
        if(task_type.contains( "("))
            task_type = task_type.remove(task_type.lastIndexOf(" ("), task_type.size());

        if(task_type == MACRO_CASE_TREE_Artifacts_Timeline_Graph)
            sqlite_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
    }

    display_case_tabs(item->text(0),item->text(2),task_type,item->parent()->text(2),sqlite_path,Q_FUNC_INFO);

}

void MainWindow::slot_combobox_case_current_text_changed(QString text)
{
    current_case_name = text;

    if(bool_insert_data_in_stack)
        return;

    slot_refresh_widgets();

    bool_combobox_items_added = true;

    combobox_category_navigation->clear();

    QStringList category_list =  center_stackwidget_manager_obj->pub_get_category_name_list(text);
    combobox_category_navigation->addItems(category_list);

    bool_combobox_items_added = false;

    if(combobox_category_navigation->count() > 0)
        combobox_category_navigation->setCurrentText(0);
    if(combobox_category_navigation->count() >= 1 || combobox_category_navigation->count() == 0)
    {
        if(combobox_category_navigation->itemText(0).trimmed() != QString(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY))
        {
            combobox_category_navigation->insertItem(0,MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY);
            enable_navigation_combobox_category_and_feature(0,false,combobox_category_navigation);

        }
    }
}

void MainWindow::slot_combobox_case_tree_display_changed(QString text)
{
    if(bool_case_tree_combobox_filling)
        return;

    if(text == MACRO_COMBOBOX_CATGORY_All_CASE_FEATURE)
    {
        for(int i = 0; i < treewidget_case_display_obj->topLevelItemCount(); i++) //- Loop for every cases.
        {
            QTreeWidgetItem *item_case_super_toplevel = treewidget_case_display_obj->topLevelItem(i);

            item_case_super_toplevel->setHidden(false);

            for(int j = 0; j < item_case_super_toplevel->childCount(); j++) //-loop For Category parents
            {
                QTreeWidgetItem *item_case_category = item_case_super_toplevel->child(j);

                item_case_category->setHidden(false);

                for(int k = 0; k < item_case_category->childCount(); k++)
                {
                    item_case_category->child(k)->setHidden(false);
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < treewidget_case_display_obj->topLevelItemCount(); i++) //- Loop for every cases.
        {
            QTreeWidgetItem *item_case_super_toplevel = treewidget_case_display_obj->topLevelItem(i);

            for(int j = 0; j < item_case_super_toplevel->childCount(); j++) //-loop For Category parents
            {
                QTreeWidgetItem *item_case_category = item_case_super_toplevel->child(j);

                item_case_category->setHidden(true);
                if(item_case_category->text(enum_global_case_tree_display_column_ZERO) == text)
                {
                    item_case_category->setHidden(false);

                    for(int k = 0; k < item_case_category->childCount(); k++)
                    {
                        item_case_category->child(k)->setHidden(false);
                    }
                }
            }
        }
    }

}

void MainWindow::slot_combobox_category_current_text_changed(QString text)
{
    if(combobox_category_navigation->currentIndex() == 0)
        return;

    current_category_name = text;

    if(bool_insert_data_in_stack)
        return;

    slot_refresh_widgets();

    bool_combobox_items_added = true;

    combobox_current_displayed_feature_opened->clear();
    QStringList features_list =  center_stackwidget_manager_obj->pub_get_feature_name_list(text);
    combobox_current_displayed_feature_opened->addItems(features_list);

    if(text.contains(MACRO_CASE_TREE_Hex_Viewer) || text.contains(MACRO_CASE_TREE_Redefined_Result_Parent)
            || text.contains(MACRO_CASE_TREE_Email_Files_Parent) || text.contains(MACRO_CASE_TREE_Story_Board))
    {
        show_metadata_widget(false);
        show_metadata_widget(false);
    }
    else
    {
        // show_metadata_widget(true);
    }

    bool_combobox_items_added = false;

    if(combobox_current_displayed_feature_opened->count() > 0)
        combobox_current_displayed_feature_opened->setCurrentText(0);

    if(combobox_current_displayed_feature_opened->count() >= 1 || combobox_current_displayed_feature_opened->count() == 0)
    {
        if(combobox_current_displayed_feature_opened->itemText(0).trimmed() != QString(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
        {
            combobox_current_displayed_feature_opened->insertItem(0,MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE);
            enable_navigation_combobox_category_and_feature(0,false,combobox_current_displayed_feature_opened);
        }
    }

    if(text.contains(MACRO_CASE_TREE_Artifacts))
        set_current_tree_item_highlighted(combobox_case_navigation->currentText(),text,combobox_current_displayed_feature_opened->currentText());





}
void MainWindow::slot_action_load_case_triggered()
{
}

void MainWindow::slot_pushButton_combobox_case_close_clicked()
{
    if(combobox_case_navigation->currentText() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE)
        return;

    slot_refresh_widgets();
    QString case_name = combobox_case_navigation->currentText();

    for(int i = (stack_widget_cases_plugins_and_global_items->count() -1) ; i >= 0; i--)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        if(widget_ptr->pub_get_case_name() == case_name)
        {
            stack_widget_cases_plugins_and_global_items->removeWidget(widget_ptr);

            emit signal_remove_navigation_list_entry_on_combobox_pusbutton_close(case_name);
        }
    }
    center_stackwidget_manager_obj->pub_remove_case_entry_from_list(case_name);
    combobox_case_navigation->removeItem(combobox_case_navigation->currentIndex());
    current_fs_display_dir_path.clear();

}

void MainWindow::slot_pushButton_combobox_category_close_clicked()
{
    if(!bool_launcher_work_completed)
        return;

    if(combobox_category_navigation->currentText() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
        return;

    slot_refresh_widgets();

    QString unique_category_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_category_navigation->currentText();

    for(int i = (stack_widget_cases_plugins_and_global_items->count() -1) ; i >= 0; i--)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        if(widget_ptr->pub_get_widget_unique_name().contains(unique_category_pattrn))
        {
            stack_widget_cases_plugins_and_global_items->removeWidget(widget_ptr);

            emit signal_remove_navigation_list_entry_on_combobox_pusbutton_close(unique_category_pattrn);
        }

    }
    center_stackwidget_manager_obj->pub_remove_category_entry_from_list(unique_category_pattrn);

    combobox_category_navigation->removeItem(combobox_category_navigation->currentIndex());

    if(combobox_category_navigation->count() == 1 && combobox_category_navigation->currentText() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CATEGORY)
    {
        combobox_case_navigation->removeItem(combobox_case_navigation->currentIndex());
        combobox_case_navigation->setCurrentIndex(combobox_case_navigation->count() -1);
    }

    current_fs_display_dir_path.clear();

}

void MainWindow::slot_pushButton_combobox_feature_close_clicked()
{
    if(!bool_launcher_work_completed)
        return;

    if(combobox_current_displayed_feature_opened->currentText() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE)
        return;

    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_category_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_current_displayed_feature_opened->currentText();

    comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);

    current_fs_display_dir_path.clear();

}
void MainWindow::slot_pushButton_topbar_previous_clicked()
{
    if(!bool_launcher_work_completed)
        return;

    bool_update_navigation_list = true;

    int stack_index = stack_widget_cases_plugins_and_global_items->currentIndex();

    if(stack_index < 0)
        return;

    Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(stack_index));
    QString current_unique_name = widget_ptr->pub_get_widget_unique_name();

    QString previous_stack_unique_name = center_next_previous_navigation_obj->pub_get_previous_stackwidget_index_from_list(current_unique_name);


    if(previous_stack_unique_name == "No page")
        return;

    QStringList list_name = previous_stack_unique_name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(list_name.size() != 3)
        return;

    refresh_combobox_category_for_feature_list(list_name.at(0),list_name.at(1),list_name.at(2));

    //  get_treewidget_item_from_item_name(previous_stack_unique_name);

    bool_update_navigation_list = false;
}

void MainWindow::slot_pushButton_topbar_next_clicked()
{
    if(!bool_launcher_work_completed)
        return;

    bool_update_navigation_list = true;

    int stack_index = stack_widget_cases_plugins_and_global_items->currentIndex();
    if(stack_index < 0)
        return;

    Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(stack_index));
    QString current_unique_name = widget_ptr->pub_get_widget_unique_name();

    QString next_stack_unique_name = center_next_previous_navigation_obj->pub_get_next_stackwidget_index_from_list(current_unique_name);


    if(next_stack_unique_name == "No page")
    {
        pushButton_topbar_next->setEnabled(false);
        return;
    }
    else
        pushButton_topbar_next->setEnabled(true);

    QStringList list_name = next_stack_unique_name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(list_name.size() != 3)
        return;

    refresh_combobox_category_for_feature_list(list_name.at(0),list_name.at(1),list_name.at(2));

    // get_treewidget_item_from_item_name(next_stack_unique_name);

    bool_update_navigation_list = false;
}


void MainWindow::slot_change_metadata_on_click(QString data, QString current_media,QString searched_text, QString source_count_name)
{
    return;


    if(current_media.isEmpty())
    {
        tree_detailed_info_apple_metadata->clear();
    }

    maps_metadata_obj->pub_change_location(QString("") ,QString(""),QString("") ,QString(""));
    textedit_detailed_information->clear();
    av_preview_object->stop_media();
    stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_BLANK);

    textedit_detailed_information->appendHtml(data);


    if(current_media != QString(""))
    {
        QString previewed_path = "";

        if(current_media.contains(MACRO_RECON_Splitter_1_multiple))
        {
            previewed_path = current_media;
            previewed_path.remove(0,previewed_path.indexOf(MACRO_RECON_Splitter_1_multiple) + sizeof(QString(MACRO_RECON_Splitter_1_multiple)));
            current_media.remove(current_media.indexOf(MACRO_RECON_Splitter_1_multiple), current_media.size());
        }else
            previewed_path = current_media;

        QFileInfo info(current_media);
        if(info.exists())
        {
            //            if(recon_helper_func_obj->check_is_image_by_extn(previewed_path, Q_FUNC_INFO) )
            //            {
            //                stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_IMAGE);
            //                image_preview_object->set_image_by_path(previewed_path);
            //            }

            //            if(recon_helper_func_obj->check_is_vedio_by_extn(previewed_path, Q_FUNC_INFO))
            //            {
            //                stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_Audio_Video);
            //                av_preview_object->load_source(previewed_path);
            //            }


            QString mime_type_of_file;// = mime_type_obj.mime_type(current_media);

            if(mime_type_of_file.startsWith(QString("image")))
            {
                stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_IMAGE);
                image_preview_object->set_image_by_path(previewed_path);
            }

            if(mime_type_of_file.startsWith(QString("audio")) || mime_type_of_file.startsWith(QString("video")))
            {
                stackwidget_previewer->setCurrentIndex(enum_global_preview_stack_widget_Audio_Video);
                av_preview_object->load_source(previewed_path);
            }

        }

    }

    textedit_detailed_information->moveCursor(QTextCursor::Start);

    slot_change_hex_viewer_content_in_metadata_on_row_click(current_media,source_count_name);

    highlight_search_text(searched_text);
}

void MainWindow::slot_open_window_for_detach_clicked(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index,QString searched_keyword, QString source_count_name)
{
    detach_window_manager_obj->pub_set_essentials();
    QString block_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Database_Hex_viewer_In_Result_QString).toString();
    detach_window_manager_obj->pub_set_hex_viewer_block_db_path(block_db_path);
    detach_window_manager_obj->pub_set_plugin_name_table_record_no(plugin_name_for_detach_ext_attribute, table_name_for_detach_ext_attribute, record_number_for_detach_ext_attribute);

    detach_window_manager_obj->pub_open_in_detach_clicked(metadata_value,hex_file_path,preview_path,preview_index,searched_keyword, source_count_name);
}


void MainWindow::slot_change_all_detach_window_content(QString metadata_value, QString hex_file_path, QString preview_path, int preview_index,QString searched_keyword,QString source_count_name)
{
    detach_window_manager_obj->pub_set_plugin_name_table_record_no(plugin_name_for_detach_ext_attribute, table_name_for_detach_ext_attribute, record_number_for_detach_ext_attribute);
    detach_window_manager_obj->pub_detach_content_change(metadata_value,hex_file_path,preview_path,preview_index,searched_keyword,source_count_name);
}


void MainWindow::slot_save_openwith_changes_to_bucket(QString viewer_type)
{
    recon_static_functions::app_debug("slot_save_openwith_changes_to_bucket START",Q_FUNC_INFO);

    if(viewer_type.trimmed().isEmpty() ||
            (viewer_type != MACRO_CASE_TREE_Sqlite_Viewer && viewer_type != MACRO_CASE_TREE_Hex_Viewer && viewer_type != MACRO_CASE_TREE_Plist_Viewer && viewer_type != MACRO_CASE_TREE_Registry_Viewer && viewer_type != MACRO_CASE_TREE_Log_Viewer))
    {
        return;
    }

    int enum_bucket = enum_global_case_tree_display_row_BUCKET ;

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Bucket))
        {
            for(int itr = 0; itr < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->childCount(); itr++)
            {
                if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->text(enum_global_case_tree_display_column_ZERO) == viewer_type)
                {
                    for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->childCount(); j++)
                    {
                        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_bucket)->child(itr)->child(j)->text(enum_global_case_tree_display_column_ZERO) == combobox_current_displayed_feature_opened->currentText())
                        {
                            return;
                        }
                    }
                }
            }
        }
    }

    emit signal_statusbar_progress_bar_show_hide(true, "Please wait....");

    if(viewer_type == MACRO_CASE_TREE_Sqlite_Viewer)
    {
        add_sqlite_viewer_in_treedisplay(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite");

    }
    else if(viewer_type == MACRO_CASE_TREE_Hex_Viewer)
    {
        QString hex_db_path_recon = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

        QString hex_display_name = st_hex_viewer_open_file_data_obj.hex_viewer_display_name;
        if(!hex_display_name.isEmpty())
            common_viewer_extraction_finish_or_cancelled(hex_display_name,MACRO_CASE_TREE_Hex_Viewer);


        emit signal_hex_viewer_db_path_changed(hex_db_path_recon);

        remove_right_click_hexviewer_info(hex_viewer_tmp_db_path, hex_display_name);
    }
    else if(viewer_type == MACRO_CASE_TREE_Plist_Viewer)
    {
        QString plist_db_path_recon = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
        common_viewer_extraction_finish_or_cancelled(combobox_current_displayed_feature_opened->currentText(),MACRO_CASE_TREE_Plist_Viewer);
        insert_plist_file_entry_from_tmp_to_recon(combobox_current_displayed_feature_opened->currentText(),plist_viewer_tmp_db_path,plist_db_path_recon);
        emit signal_plist_viewer_db_path_changed(plist_db_path_recon);

        remove_right_click_plist_info(plist_viewer_tmp_db_path,combobox_current_displayed_feature_opened->currentText());


    }

    else if(viewer_type == MACRO_CASE_TREE_Registry_Viewer)
    {
        QString display_tab_name = combobox_current_displayed_feature_opened->currentText();
        copy_from_tmp_for_registry_viewer(display_tab_name);
        common_viewer_extraction_finish_or_cancelled(display_tab_name,MACRO_CASE_TREE_Registry_Viewer);
    }
    else if(viewer_type == MACRO_CASE_TREE_Log_Viewer)
    {
        QString log_db_path_recon = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";

        common_viewer_extraction_finish_or_cancelled(combobox_current_displayed_feature_opened->currentText(),MACRO_CASE_TREE_Log_Viewer);
        insert_log_file_index_table_entry_from_tmp_db_to_recon_db(combobox_current_displayed_feature_opened->currentText(),log_viewer_tmp_db_path,log_db_path_recon);
        emit signal_log_viewer_db_path_changed(log_db_path_recon);
        remove_right_click_log_info(log_viewer_tmp_db_path,combobox_current_displayed_feature_opened->currentText());
    }
    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug("slot_save_openwith_changes_to_bucket END",Q_FUNC_INFO);

}

void MainWindow::update_tags_in_notes_search_db_and_display_from_feature(QString plugin_name, QString tab_name, QString record_no, QString tag_name, QString os_scheme)
{
    QString notes_result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";

    QString command = "UPDATE notes SET recon_tag_value=? WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display=?";
    QStringList arg_list;
    arg_list << tag_name << plugin_name << tab_name << record_no << os_scheme;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,notes_result_db_path,Q_FUNC_INFO);

}

void MainWindow::insert_record_entry_into_bookmarks_db_for_saved_graph_result(QString record_no, QString plugin_name, QString tab_name, QString status, QString bookmarks_db_path, QString saved_graphs_db_path, QString os_name_str, QString tag_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(saved_graphs_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,graph_name,graph_path,timestamp ,notes from saved_graphs WHERE INT=?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
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

    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND record_no=?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << record_no;

    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,bookmarks_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty())
    {
        QStringList arg_list;

        QString tab_name_csv;

        if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
            tab_name_csv = QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs);
        else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
            tab_name_csv = QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs);

        QString command_insert = "INSERT INTO tags(bookmark,recon_tag_value,plugin_name,category,tab_name,record_no,"
                                 "os_scheme_display,item0,item1,item2,item3,notes,csv_plugin_name,csv_tab_name) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)";



        arg_list << query.value(0).toString() << query.value(1).toString() << plugin_name
                 << QString(MACRO_CASE_TREE_Graph_Result_Saved) << tab_name << record_no << os_name_str
                 << query.value(2).toString() << query.value(3).toString()
                 << global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(4).toString().trimmed(), Q_FUNC_INFO)
                 << query.value(5).toString()
                 << query.value(6).toString() << plugin_name <<tab_name_csv;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,bookmarks_db_path,Q_FUNC_INFO);
    }
    else
    {
        QString command_update = "UPDATE tags SET notes = ? WHERE record_no=?";
        QStringList arg_list;
        arg_list << query.value(10).toString() << record_no;

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_update,arg_list,bookmarks_db_path,Q_FUNC_INFO);
    }
    source_db.close();
    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}


void MainWindow::update_bookmark_value_in_ram_analysis_saved_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    if(plugin_name != QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
        return ;

    QString RAM_analysis_Saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";

    QStringList arg_list;
    QString command = "UPDATE tbl_saved_result SET bookmark=? WHERE record_no=?";
    arg_list << status << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,RAM_analysis_Saved_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::update_bookmark_value_in_graph_result_saved_db_and_display(QString record_no, QString plugin_name, QString tab_name, QString status, QString os_scheme)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString dest_db_path;
    if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        dest_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
    }
    else
        return ;


    QString command = "UPDATE saved_graphs SET bookmark=? WHERE INT=?";
    QStringList arg_list;
    arg_list << status << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,dest_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::update_remained_tag_in_graph_result_saved_db_and_display(QString plugin_name, QString tab_name, QString record_no, QString tag_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);


    QString result_db_path ;
    if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        result_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
    }
    else
        return ;


    QString command = "UPDATE saved_graphs SET recon_tag_value=? WHERE INT=?";
    QStringList arg_list;
    arg_list << tag_name << record_no ;

    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,result_db_path,Q_FUNC_INFO);

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}

void MainWindow::add_and_update_tags_in_database_for_graph_result_saved(QString tag_name, QString is_emitted_from_plugin, QString plugin_name_str, QString tab_name_str, QString record_no, QString saved_graphs_db_path, QString os_name)
{
    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    QString tag_db_path = recon_helper_standard_obj->create_separate_tag_db_for_tag_search(tag_name,Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(saved_graphs_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,graph_name,graph_path,timestamp ,notes from saved_graphs WHERE INT=?");
    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select ------FAILED------ " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        source_db.close();
        return;
    }
    if(!query.next())
    {
        recon_static_functions::app_debug(" --- No Record Exist---",Q_FUNC_INFO);
        return;
    }

    QString command_already_record = "SELECT INT FROM tags WHERE plugin_name=? AND record_no=?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name_str << record_no;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tag_db_path,Q_FUNC_INFO);


    if(INT_str.isEmpty())
    {
        QStringList arg_list;
        QString tab_name_csv;


        if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
            tab_name_csv = QString(MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs);
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
            tab_name_csv = QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs);


        QString command_insert = "INSERT INTO tags(bookmark,recon_tag_value,plugin_name,category,tab_name,record_no,"
                                 "os_scheme_display,item0,item1,item2,item3,notes,csv_plugin_name,csv_tab_name) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)";



        arg_list << query.value(0).toString() << query.value(1).toString() << plugin_name_str
                 << QString(MACRO_CASE_TREE_Graph_Result_Saved) << tab_name_str << record_no << os_name
                 << query.value(2).toString() << query.value(3).toString()
                 << query.value(4).toString()
                 << global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(5).toString().trimmed(), Q_FUNC_INFO)
                 << query.value(6).toString()<< plugin_name_str << tab_name_csv;

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

void MainWindow::slot_lineedit_case_tree_search_text_changed(QString text)
{
    text = text.toLower().trimmed();
    if(text.isEmpty())
    {
        treewidget_case_display_obj->collapseAll();

        bool bool_first_child = true;;
        QTreeWidgetItemIterator parent_iter(treewidget_case_display_obj);
        while(*parent_iter)
        {
            QTreeWidgetItem *all_tree_items = *parent_iter;

            if(bool_first_child)
            {
                all_tree_items->setExpanded(true);
                bool_first_child = false;
            }

            all_tree_items->setHidden(false);
            ++parent_iter;
        }


        return;
    }



    QTreeWidgetItemIterator parent_iter(treewidget_case_display_obj);
    while(*parent_iter)
    {
        QTreeWidgetItem *all_tree_items = *parent_iter;

        all_tree_items->setHidden(true);
        ++parent_iter;
    }



    QList<QTreeWidgetItem *> list = treewidget_case_display_obj->findItems(text, Qt::MatchContains | Qt::MatchRecursive);

    for(int i = 0; i < list.size(); i++)
    {

        QTreeWidgetItem *m_tree = list.at(i);

        if(m_tree->text(enum_global_case_tree_display_column_FIRST).contains(QString(MACRO_Case_Launcher_Source)))
        {
            continue;
        }

        m_tree->setHidden(false);
        if(m_tree->parent() != NULL) // User Defined Extensions
        {

            m_tree->parent()->setHidden(false);
            m_tree->parent()->setExpanded(true);

            if(m_tree->parent()->parent() != NULL) // File Extensions
            {
                m_tree->parent()->parent()->setHidden(false);
                m_tree->parent()->parent()->setExpanded(true);

                if(m_tree->parent()->parent()->parent() != NULL)  // Case Name : gfrtyrt
                {
                    m_tree->parent()->parent()->parent()->setHidden(false);
                    m_tree->parent()->parent()->parent()->setExpanded(true);
                }

            }

        }

    }

}

void MainWindow::extract_apple_metadata_from_apple_metadata_reader(QString file_paths_str, QString source_cnt_name,QString virtual_source_path)
{
    if(bool_apple_metdatata_process_already_running)
    {
        return;
    }

    bool_apple_metdatata_process_already_running = true;

    QString config_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
    QString all_apple_attributes_command_str = QString("Select metadata_attribute_name From tbl_apple_attributes");
    QStringList attributes_name_list_all = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(all_apple_attributes_command_str, 0, config_db_path, Q_FUNC_INFO);

    QString all_apple_attributes_columnname_command_str = QString("Select custom_column_name From tbl_apple_attributes");
    QStringList column_name_list_all = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(all_apple_attributes_columnname_command_str, 0, config_db_path, Q_FUNC_INFO);

    recon_case_obj->pub_rcase_extract_apple_metadata_from_apple_metadata_reader(file_paths_str, source_cnt_name, virtual_source_path,attributes_name_list_all,column_name_list_all);

}

void MainWindow::slot_prc_apple_metadata_prc_left_click_finished()
{
    bool_apple_metdatata_process_already_running = false;
}

QString MainWindow::get_quicklook_link_path_for_ios_quicklook(QString display_filepath, QString complete_filepath, QString record_no_str, QString source_count_name)
{
    if(display_filepath.isEmpty() || !QFileInfo(complete_filepath).exists())
        return QString("");

    display_loading_progress_bar_non_cancelable_obj->show();
    QString cache_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_iOS_Preview_Cache_In_Result_QString).toString() + source_count_name;
    QDir cache_dir(cache_dir_path);
    if(!cache_dir.exists())
        cache_dir.mkpath(cache_dir_path);

    QString unique_filename_str = source_count_name + "_" + record_no_str + "_" + QFileInfo(display_filepath).fileName();
    QString link_filename_path =   cache_dir_path + "/" +  unique_filename_str;
    if(QFileInfo(link_filename_path).exists())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();
        return link_filename_path;
    }

    QFile::copy(complete_filepath, link_filename_path);

    display_loading_progress_bar_non_cancelable_obj->hide();

    return  link_filename_path;
}
