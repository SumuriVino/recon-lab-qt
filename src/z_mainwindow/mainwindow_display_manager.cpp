#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionShow_Sidebar_triggered()
{
    dockwidget_case_tree_obj->setAllowedAreas(Qt::RightDockWidgetArea);
    dockwidget_case_tree_obj->setFloating(false);
    dockwidget_case_tree_obj->show();
}

void MainWindow::on_actionHide_Sidebar_triggered()
{
    dockwidget_case_tree_obj->hide();

}

void MainWindow::on_actionShow_Detailed_Information_triggered()
{
    bool_metadata_show = true;
    show_metadata_widget(true);
}

void MainWindow::on_actionHide_Detailed_Information_triggered()
{
    bool_metadata_show = false;

    show_metadata_widget(false);
}

void MainWindow::on_actionRedefined_Result_triggered()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    redefined_result_filters_interface_obj = new redefined_result_filters_interface(this);
    redefined_result_filters_interface_obj->pub_display_plugins_category_list_interface();

    redefined_result_filters_interface_obj->show();

    recon_static_functions::app_debug("-Ends",Q_FUNC_INFO);

}


void MainWindow::case_tree_widget_combobox_list_display()
{
    bool_case_tree_combobox_filling = true;

    combobox_case_tree_display->insertItem(0,QIcon(QString("../icons/signs/") + QString(MACRO_COMBOBOX_CATGORY_All_CASE_FEATURE).replace(" ","_") + ".png"),MACRO_COMBOBOX_CATGORY_All_CASE_FEATURE);
    //    QTreeWidgetItem *child = new QTreeWidgetItem;
    QTreeWidgetItem *child = NULL;
    QTreeWidgetItemIterator iter_tree(treewidget_case_display_obj);

    int count = 1;
    while (*iter_tree)
    {
        QString item_text_first  = (*iter_tree)->text(enum_global_case_tree_display_column_FIRST);
        QString item_text_parent = (*iter_tree)->text(enum_global_case_tree_display_column_SECOND);

        child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);

        if(item_text_first == QString(MACRO_COMBOBOX_FEATURE_Case_Info))
        {
            QString icon_path = "../icons/case_info.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Parent_Source))
        {
            QString icon_path = "../icons/Features/Source.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Mobile_Backup_Parent))
        {
            QString icon_path = "../icons/Features/mobile_backup.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Disk_Images_Parent))
        {
            QString icon_path = "../icons/Features/disk_images.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Ram_Images_Parent))
        {
            QString icon_path = "../icons/Features/ram_images.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Apple_Metadata_Parent))
        {
            QString icon_path = "../icons/Features/apple_metadata.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Exif_Parent))
        {
            QString icon_path = "../icons/Features/exif_data.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_MIME_Type))
        {
            QString icon_path = "../icons/Features/mime_type.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_File_Extensions))
        {
            QString icon_path = "../icons/Features/extensions.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_File_Signature_Parent))
        {
            QString icon_path = "../icons/Features/file_signatures.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_File_Size_Parent))
        {
            QString icon_path = "../icons/Features/file_size.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent =="Parent " +  QString(MACRO_CASE_TREE_Unified_Logs))
        {
            QString icon_path = "../icons/Features/unified_logs.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Snapshots_Parent))
        {
            QString icon_path = "../icons/Features/snapshots.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Hashset))
        {
            QString icon_path = "../icons/Features/hashset.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Griffeye_Data_Parent))
        {
            QString icon_path = "../icons/Features/griffeye.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Face_Analysis_PARENT))
        {
            QString icon_path = "../icons/Features/face_analysis.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }

        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Face_Search_PARENT))
        {
            QString icon_path = "../icons/Features/face_search.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Optical_Character_Recognition_Parent))
        {
            QString icon_path = "../icons/Features/optical_character_recognition.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Skin_Tone_Detection_Parent))
        {
            QString icon_path = "../icons/Features/skin_tone_detection.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Weapons_Parent))
        {
            QString icon_path = "../icons/Features/weapons.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Fire_Analysis_Parent))
        {
            QString icon_path = "../icons/Features/fire_analysis.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent " + QString(MACRO_CASE_TREE_Email_Files_Parent))
        {
            QString icon_path = "../icons/Features/email_files.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Artifacts Keyword Search")
        {
            QString icon_path = "../icons/Features/artifacts_keyword_search.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent File Search")
        {
            QString icon_path = "../icons/Features/file_search.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Content Search")
        {
            QString icon_path = "../icons/Features/content_search.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Timeline")
        {
            QString icon_path = "../icons/Features/timeline_full.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Bucket")
        {
            QString icon_path = "../icons/Features/bucket.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Plist Viewer")
        {
            QString icon_path = "../icons/Features/plist_viewer.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Hex Viewer")
        {
            QString icon_path = "../icons/Features/hex_viewer.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent SQLite Viewer")
        {
            QString icon_path = "../icons/Features/sqlite_viewer.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Registry Viewer")
        {
            QString icon_path = "../icons/Features/registry_viewer.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Log Viewer")
        {
            QString icon_path = "../icons/Features/log_viewer.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Tags")
        {
            QString icon_path = "../icons/Features/tags.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Notes")
        {
            QString icon_path = "../icons/Features/notes.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Redefined Result")
        {
            QString icon_path = "../icons/Features/redefined_result.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Redefined Result Saved")
        {
            QString icon_path = "../icons/Features/redefined_result.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Screenshots")
        {
            QString icon_path = "../icons/Features/screenshots.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Story_Board_Parent))
        {
            QString icon_path = "../icons/Features/story_board.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Carved_Password_Parent))
        {
            QString icon_path = "../icons/Features/carved_password.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Carved_Files_Parent))
        {
            QString icon_path = "../icons/Features/carved_files.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == QString(MACRO_CASE_TREE_Carved_Data_Parent))
        {
            QString icon_path = "../icons/Features/carved_data.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Ram Analysis Saved")
        {
            QString icon_path = "../icons/Features/ram_analysis.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Graph Result Saved")
        {
            QString icon_path = "../icons/Features/graphs.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Saved Maps")
        {
            QString icon_path = "../icons/Features/locations.png";
            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;
        }
        else if(item_text_parent == "Parent Plugins")
        {
            QString icon_path;
            QString item_text_zero = child->text(enum_global_case_tree_display_column_ZERO);
            if(item_text_zero == MACRO_CASE_TREE_Artifacts)
                icon_path = "../icons/" + item_text_zero.replace(" ","_") + ".png";
            else
                icon_path = "../icons/plugins/" + item_text_zero.replace(" ","_") + ".png";

            combobox_case_tree_display->insertItem(count,QIcon(icon_path),child->text(enum_global_case_tree_display_column_ZERO));
            count++;

            //            break;
        }


        ++iter_tree;
    }

    bool_case_tree_combobox_filling = false;

}

void MainWindow::insert_struct_for_display_combobox_and_stackwidget(QString case_name, QString category, QString current_tab_name)
{
    recon_static_functions::app_debug(" -START " , Q_FUNC_INFO);
    bool_insert_data_in_stack = true;
    bool_update_navigation_list = true;



    //-Insertion of widget in stackwidget.
    QString unique_string = case_name + MACRO_RECON_Splitter_1_multiple +category + MACRO_RECON_Splitter_1_multiple + current_tab_name;
    Container_QWidget *widget =  center_stackwidget_manager_obj->pub_get_container_widget_ptr(unique_string);
    QString current_page_unique_name = widget->pub_get_widget_unique_name();
    stack_widget_cases_plugins_and_global_items->insertWidget(stack_widget_cases_plugins_and_global_items->count(),widget);

    //-Insert Item into the navigation list.
    if(current_tab_name != MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE)
        emit signal_insert_new_entry_in_navigation_list(current_page_unique_name);

    //-Insert items in Combobox too.
    insert_items_in_naavigatin_combobox(case_name,category);

    bool_insert_data_in_stack = false;

    refresh_combobox_category_for_feature_list(case_name,category,current_tab_name);
    bool_update_navigation_list = false;

    recon_static_functions::app_debug(" -END " , Q_FUNC_INFO);
}

QTreeWidgetItem *MainWindow::get_treewidget_item_from_item_name(QString item_name)
{
    ///iterate complete tree here...
    QTreeWidgetItem *child = new  QTreeWidgetItem;
    QTreeWidgetItemIterator iter_tree(treewidget_case_display_obj);

    while (*iter_tree)
    {
        if ((*iter_tree)->text(enum_global_case_tree_display_column_ZERO) == item_name)
        {
            child = dynamic_cast<QTreeWidgetItem *>(*iter_tree);

            break;
        }
        ++iter_tree;
    }
    return child;
}


void MainWindow::set_focus_on_case_treewidget_item(QString unique_name)
{
    treewidget_case_display_obj->clearSelection();

    QStringList list_name = unique_name.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);
    if(list_name.size() != 3)
        return;

    for(int i_case = 0; i_case < treewidget_case_display_obj->topLevelItemCount(); i_case++)
    {
        if(!treewidget_case_display_obj->topLevelItem(i_case)->text(enum_global_case_tree_display_column_ZERO).contains(MACRO_CASE_TREE_Case))
            continue;

        for(int j_category = 0; j_category < treewidget_case_display_obj->topLevelItem(i_case)->childCount(); j_category++)
        {
            if(treewidget_case_display_obj->topLevelItem(i_case)->child(j_category)->text(enum_global_case_tree_display_column_ZERO) != list_name.at(1) || treewidget_case_display_obj->topLevelItem(i_case)->child(j_category)->text(enum_global_case_tree_display_column_ZERO) == MACRO_CASE_TREE_Source)
                continue;

            for(int k_feature = 0; k_feature < treewidget_case_display_obj->topLevelItem(i_case)->child(j_category)->childCount(); k_feature++)
            {
                if(!treewidget_case_display_obj->topLevelItem(i_case)->child(j_category)->child(k_feature)->text(enum_global_case_tree_display_column_ZERO).contains(list_name.at(2)))
                    continue;

                treewidget_case_display_obj->topLevelItem(i_case)->child(j_category)->child(k_feature)->setSelected(true);
                return;
            }
        }
    }
}

void MainWindow::insert_items_in_naavigatin_combobox(QString case_name, QString category)
{
    recon_static_functions::app_debug(" -START " , Q_FUNC_INFO);

    QStringList case_list = center_stackwidget_manager_obj->pub_get_case_name_list();
    QStringList category_list = center_stackwidget_manager_obj->pub_get_category_name_list(case_name);
    QStringList feature_list = center_stackwidget_manager_obj->pub_get_feature_name_list(category);

    //-Insertion in Case Combo
    for(int i = 0; i < case_list.size(); i++)
    {
        int check_case =  combobox_case_navigation->findText(case_list.at(i),Qt::MatchFixedString);
        if(check_case != -1)
            continue;

        //            combobox_case_navigation->addItem(QIcon("../icons/signs/case.png"),case_list.at(i));
        combobox_case_navigation->addItem(case_list.at(i));
        break;
    }

    //-Insertion in Category Combo
    for(int j = 0; j < category_list.size(); j++)
    {
        int check_case =  combobox_category_navigation->findText(category_list.at(j),Qt::MatchFixedString);
        if(check_case != -1)
            continue;

        //            QString icon_path = QString("../icons/signs/") + category_list.at(j).trimmed().replace(" ","_") + ".png";
        //            combobox_category_navigation->addItem(QIcon(icon_path),category_list.at(j));
        combobox_category_navigation->addItem(category_list.at(j));
        break;
    }

    //-Insertion in Feature Combo
    for(int k = 0; k < feature_list.size(); k++)
    {
        int check_case =  combobox_current_displayed_feature_opened->findText(feature_list.at(k),Qt::MatchFixedString);
        if(check_case != -1)
            continue;

        //            QString feature_str = feature_list.at(k);
        //            QString icon_path = "../icons/signs/" + feature_str.replace(" ","_") + ".png";
        combobox_current_displayed_feature_opened->addItem(feature_list.at(k));
        break;
    }

    enable_navigation_combobox_category_and_feature(0,false,combobox_category_navigation);
    enable_navigation_combobox_category_and_feature(0,false,combobox_current_displayed_feature_opened);

    recon_static_functions::app_debug(" -END " , Q_FUNC_INFO);
}

void MainWindow::refresh_combobox_category_for_feature_list(QString case_name, QString category_text , QString feature_name)
{
    if(sender() && sender()->metaObject() && (sender()->metaObject() != pushButton_topbar_previous->metaObject()) && (sender()->metaObject() != pushButton_topbar_next->metaObject()))
    {

        QString previos_str = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_category_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + combobox_current_displayed_feature_opened->currentText();
        QString current_str = case_name + MACRO_RECON_Splitter_1_multiple + category_text + MACRO_RECON_Splitter_1_multiple + feature_name;
        emit signal_set_center_next_previous_navigation_list_page_unique_name(previos_str, current_str);

    }

    bool_update_navigation_list = true;

    current_category_name = category_text;

    combobox_case_navigation->setCurrentText(case_name);
    combobox_category_navigation->setCurrentText(category_text);

    combobox_current_displayed_feature_opened->clear();

    QStringList features_list =  center_stackwidget_manager_obj->pub_get_feature_name_list(category_text);
    combobox_current_displayed_feature_opened->addItems(features_list);

    if(combobox_current_displayed_feature_opened->count() > 0)
        combobox_current_displayed_feature_opened->setCurrentText(feature_name);


    for(int count = 0; count < combobox_current_displayed_feature_opened->count(); count++)
    {
        QString feature_name_str = combobox_current_displayed_feature_opened->itemText(count).trimmed();
        /// Records not removed in case of Source
        feature_name_str = recon_static_functions::remove_record_count_from_feature_name(feature_name_str);

        if(feature_name_str == feature_name)
        {
            combobox_current_displayed_feature_opened->setCurrentIndex(count);
            break;
        }
    }


    if(combobox_current_displayed_feature_opened->count() >= 1 || combobox_current_displayed_feature_opened->count() == 0)
    {
        if(combobox_current_displayed_feature_opened->itemText(0).trimmed() != QString(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
        {
            combobox_current_displayed_feature_opened->insertItem(0,MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE);
            enable_navigation_combobox_category_and_feature(0,false,combobox_current_displayed_feature_opened);
        }
    }

    bool_update_navigation_list = false;

    if(category_text.contains(MACRO_CASE_TREE_Artifacts))
        set_current_tree_item_highlighted(case_name,category_text,feature_name);

    if(feature_name == MACRO_COMBOBOX_FEATURE_Case_Info)
    {
        show_metadata_widget(false);
    }


}

void MainWindow::remove_category_page_from_stackwidget(QString page_category)
{
    recon_static_functions::debug_intensive(" start ",Q_FUNC_INFO);

    int plugins_index = -1;
    for(int i = 0; i < combobox_category_navigation->count(); i++)
    {
        if(combobox_category_navigation->itemText(i).trimmed() == page_category.trimmed())
        {
            plugins_index = i;
            break;
        }
    }

    if(plugins_index != -1)
    {
        combobox_category_navigation->setCurrentIndex(plugins_index);
        slot_pushButton_combobox_category_close_clicked();
    }

    recon_static_functions::debug_intensive(" end ",Q_FUNC_INFO);

}

void MainWindow::remove_case_from_stackwidget(QString case_name)
{
    if(combobox_case_navigation->currentText() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_CASE)
        return;

    slot_refresh_widgets();

    //    QString case_name = combobox_case_navigation->currentText();

    for(int i = (stack_widget_cases_plugins_and_global_items->count() -1) ; i >= 0; i--)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        if(widget_ptr->pub_get_case_name() == case_name)
        {
            stack_widget_cases_plugins_and_global_items->removeWidget(widget_ptr);
            break;
        }

    }

    center_stackwidget_manager_obj->pub_remove_case_entry_from_list(case_name);
    combobox_case_navigation->removeItem(combobox_case_navigation->currentIndex());
}

void MainWindow::set_stackwidget_display_by_unique_name(QString unique_pattern)
{
    recon_static_functions::debug_intensive(" -START " , Q_FUNC_INFO);

    for(int i = 0; i < stack_widget_cases_plugins_and_global_items->count(); i++)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->widget(i));

        if(unique_pattern.endsWith(MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE))
        {
            if(widget_ptr->pub_get_feature_widget_name() == MACRO_TOP_NAVIGATION_BAR_COMBOBOX_SELECT_FEATURE)
            {
                stack_widget_cases_plugins_and_global_items->setCurrentIndex(i);
                break;
            }
        }
        else
        {
            if(widget_ptr->pub_get_widget_unique_name() == unique_pattern)
            {
                stack_widget_cases_plugins_and_global_items->setCurrentIndex(i);
                break;
            }
        }

    }
    recon_static_functions::debug_intensive(" -END " , Q_FUNC_INFO);
}
void MainWindow::show_metadata_widget(bool status)
{
    if(status)
    {
        QList<int > list_size;
        list_size <<23 <<200 << 250;
        splitter_case_tab_and_metadata_display->widget(MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int)->setHidden(false);
    }
    else
    {
        QList<int > list_size;
        list_size <<23 <<200 << 0;
        splitter_case_tab_and_metadata_display->widget(MACRO_CASE_DISPLAY_WIDGET_METADATA_TABS_int)->setHidden(true);
    }

}

void MainWindow::remove_media_path_entry_from_tree()
{

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Source))
        {
            for(int jj = 0; jj < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->childCount(); jj++)
            {
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->removeChild(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SOURCES)->child(jj));
            }
        }
    }

}


void MainWindow::on_actionSend_To_Bucket_triggered()
{
    recon_static_functions::app_debug("on_actionSend_to_Bucket_triggered START",Q_FUNC_INFO);

    QString viewer_type;
    if(combobox_category_navigation->currentText() == MACRO_CASE_TREE_Sqlite_Viewer)
        viewer_type = MACRO_CASE_TREE_Sqlite_Viewer;
    else if(combobox_category_navigation->currentText() == MACRO_CASE_TREE_Hex_Viewer)
        viewer_type = MACRO_CASE_TREE_Hex_Viewer;
    else if(combobox_category_navigation->currentText() == MACRO_CASE_TREE_Plist_Viewer)
        viewer_type = MACRO_CASE_TREE_Plist_Viewer;
    else if(combobox_category_navigation->currentText() == MACRO_CASE_TREE_Log_Viewer)
        viewer_type = MACRO_CASE_TREE_Log_Viewer;

    if(viewer_type.trimmed().isEmpty() || (viewer_type != MACRO_CASE_TREE_Sqlite_Viewer && viewer_type != MACRO_CASE_TREE_Hex_Viewer && viewer_type != MACRO_CASE_TREE_Plist_Viewer && viewer_type != MACRO_CASE_TREE_Log_Viewer))
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

        common_viewer_extraction_finish_or_cancelled(combobox_current_displayed_feature_opened->currentText(),MACRO_CASE_TREE_Hex_Viewer);

        emit signal_hex_viewer_db_path_changed(hex_db_path_recon);
    }
    else if(viewer_type == MACRO_CASE_TREE_Plist_Viewer)
    {
        QString plist_db_path_recon = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Plist_viewer_In_Result_QString).toString() + "plist_viewer.sqlite";
        common_viewer_extraction_finish_or_cancelled(combobox_current_displayed_feature_opened->currentText(),MACRO_CASE_TREE_Plist_Viewer);
        insert_plist_file_entry_from_tmp_to_recon(combobox_current_displayed_feature_opened->currentText(),plist_viewer_tmp_db_path,plist_db_path_recon);
        emit signal_plist_viewer_db_path_changed(plist_db_path_recon);
    }
    else if(viewer_type == MACRO_CASE_TREE_Log_Viewer)
    {
        QString log_db_path_recon = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Log_viewer_In_Result_QString).toString() + "log_viewer.sqlite";
        common_viewer_extraction_finish_or_cancelled(combobox_current_displayed_feature_opened->currentText(),MACRO_CASE_TREE_Log_Viewer);
        insert_log_file_index_table_entry_from_tmp_db_to_recon_db(combobox_current_displayed_feature_opened->currentText(),log_viewer_tmp_db_path,log_db_path_recon);
        emit signal_log_viewer_db_path_changed(log_db_path_recon);
    }

    emit signal_statusbar_progress_bar_show_hide(false, "");

    recon_static_functions::app_debug("on_actionSend_to_Bucket_triggered END",Q_FUNC_INFO);

}


void MainWindow::on_actionPlugin_Summary_triggered()
{
    recon_static_functions::app_debug("on_actionPlugin_Summary_triggered START",Q_FUNC_INFO);

    if(combobox_category_navigation->currentText() == MACRO_CASE_TREE_Artifacts)
    {
        Container_QWidget *widget_ptr = dynamic_cast<Container_QWidget *>(stack_widget_cases_plugins_and_global_items->currentWidget());

        QString plugin_name = widget_ptr->pub_get_widget_unique_name();

        plugin_name.remove(0, plugin_name.lastIndexOf(MACRO_RECON_Splitter_1_multiple) + sizeof(QString(MACRO_RECON_Splitter_1_multiple)));

    }

    recon_static_functions::app_debug("on_actionPlugin_Summary_triggered END",Q_FUNC_INFO);
}

