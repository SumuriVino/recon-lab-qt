#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::slot_update_bookmark_value_in_tag_search_for_filetypes(QStringList record_no_list,QString source_count_name, QString bookmark_status, QString plugin_name_str)
{
    // update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);

    QString bookmarks_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    QString command_str = QString("SELECT bookmark,recon_tag_value,notes,filename,filepath,file_size,mime_type,md5_hash,extension,file_parent_path FROM files"
                                  " Where INT = ");

    for(int rec_count = 0; rec_count < record_no_list.size(); rec_count++)
    {
        command_str.append(record_no_list.at(rec_count));

        if(rec_count < record_no_list.size() -1)
            command_str.append(" OR INT = ");
    }


    struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_dsply = struct_info.os_scheme_display;

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(fs_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + fs_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare(command_str);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    int count = 0;
    while(query.next())
    {
        if(record_no_list.size() > count)
        {
            QString record_no = record_no_list.at(count);

            QString command_already_record = "SELECT INT FROM tags WHERE source_count_name = ? AND record_no = ? AND plugin_name = ?";
            QStringList arg_list_for_tags;
            arg_list_for_tags << source_count_name << record_no << QString(MACRO_Plugin_Name_File_System);
            QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,bookmarks_db_path,Q_FUNC_INFO);

            if(INT_str.isEmpty())
            {
                QString file_path = query.value(4).toString().trimmed();

                QStringList arg_list;

                arg_list << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << QString("files")
                         << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                         << query.value(2).toString().trimmed() << os_schme_dsply << source_count_name << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << query.value(3).toString().trimmed() << file_path
                         << recon_static_functions::human_readable_size(query.value(5).toString().trimmed(), Q_FUNC_INFO)
                         << query.value(6).toString().trimmed()
                         << "";

                QStringList db_columns_list;
                db_columns_list << QString("item0") << QString("item1") << QString("item2") << QString("item3") << QString("item4");

                QString command_insert = get_insert_query_for_tag_search(db_columns_list);
                recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,bookmarks_db_path,Q_FUNC_INFO);

                emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_File_System) , QString(MACRO_Plugin_File_System_TAB_Name_Files) , QString(MACRO_Generic_Tag_Bookmarks), os_schme_dsply);
            }
        }

        count++;

    }

    source_db.close();

}

void MainWindow::slot_common_add_notes_name_into_case_tree(QString note_name)
{
    QTreeWidgetItem *sub_child;
    QString notes_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString() + "all_notes.sqlite";
    int notes_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, notes_db_path, QStringList(), Q_FUNC_INFO);

    QString icon_path = "../icons/Features/notes.png";

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Notes_Parent))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->childCount(); j++)
            {

                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->child(j);
                sub_child->setText(0, note_name + " (" + QString::number(notes_count) + ")");
                sub_child->setIcon(0,QIcon(icon_path));
                sub_child->setText(1,QString(MACRO_CASE_TREE_Notes_Parent));
                sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                ////if particular tag does not contain any records then text color of tag will becomes red.
                //            if(notes_count == 0)
                //            {
                //               sub_child->setForeground(0, Qt::red);
                //            }

                return;
            }

            sub_child = new QTreeWidgetItem;
            sub_child->setText(0, note_name + " (" + QString::number(notes_count) + ")");
            sub_child->setIcon(0,QIcon(icon_path));
            sub_child->setText(1,QString(MACRO_CASE_TREE_Notes_Parent));
            sub_child->setText(2,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_NOTES)->addChild(sub_child);
            break;
        }
    }
}

void MainWindow::slot_common_add_tag_name_into_case_tree(QString tag)
{
    common_add_tag_name_into_case_tree(tag);
}

void MainWindow::slot_add_notes_into_case_tree(QString notes_str)
{
    common_add_notes_name_into_case_tree(notes_str);
}

void MainWindow::slot_display_for_artifacts_timeline_saved_results_and_graphs(QString feature_name, QString tab_text)
{
    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + feature_name + MACRO_RECON_Splitter_1_multiple + feature_name;
    comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);

    if(feature_name == MACRO_CASE_TREE_Artifacts_Timeline_Graph)
        prepare_display_for_saved_timeline_graphs(feature_name);
    else if(feature_name == MACRO_CASE_TREE_Artifacts_Timeline)
        prepare_display_artifacts_timeline_saved(tab_text);
}

void MainWindow::slot_display_for_redefined_results_saved_results_and_graphs(QString feature_name,QString tab_text, QString db_path , QString plugin_name)
{
    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + feature_name + MACRO_RECON_Splitter_1_multiple + tab_text;
    comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);

    if(feature_name == MACRO_CASE_TREE_Graph_Result_Saved)
        prepare_display_for_saved_graphs(tab_text , db_path , plugin_name);
    else if(feature_name == MACRO_CASE_TREE_Redefined_Result_Saved_Parent)
        prepare_display_redefined_result_saved(tab_text ,db_path ,plugin_name);
}

void MainWindow::slot_display_for_saved_maps(QString tab_text)
{
    QString unique_pattrn = combobox_case_navigation->currentText() + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Saved_Maps + MACRO_RECON_Splitter_1_multiple + MACRO_CASE_TREE_Saved_Maps;
    comman_remove_feature_from_combobox_and_stackwidget(unique_pattrn);
    prepare_display_for_saved_maps(tab_text);
}


void MainWindow::slot_add_saved_timeline_graph_in_case_tree()
{
    QTreeWidgetItem *sub_child;
    QString saved_timeline_graphs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";
    int records_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, saved_timeline_graphs_db_path, QStringList(), Q_FUNC_INFO);

    QString icon_path = "../icons/Features/timeline.png";

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Graph_Result_Saved))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->childCount(); j++)
            {

                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->child(j);

                if(sub_child->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph) && sub_child->text(enum_global_case_tree_display_column_SECOND) == global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString())
                {
                    sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph) + " (" + QString::number(records_count) + ")");
                    sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
                    sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph));
                    sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                    return;
                }
            }

            sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Artifacts_Timeline_Graph) + " (" + QString::number(records_count) + ")");
            sub_child->setIcon(enum_global_case_tree_display_column_ZERO,QIcon(icon_path));
            sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph));
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            if(records_count != 0)
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_GRAPH_RESULT)->addChild(sub_child);

            break;
        }
    }
}

void MainWindow::slot_tree_exif_metadata_item_clicked(QTreeWidgetItem *item, int m_column)
{

    Q_UNUSED(m_column);

    if(item->text(enum_global_exif_data_interface_exif_data_KEY).isEmpty())
        return;

    QString source_count_name    = item->text(enum_global_exif_data_interface_exif_data_Plugin_Source_count_name).trimmed();

    QString tablename = item->text(enum_global_exif_data_interface_exif_data_Plugin_Table_Name);
    QString db_path   = item->text(enum_global_exif_data_interface_exif_data_Plugin_Db_Path);
    QString rec_no    = item->text(enum_global_exif_data_interface_exif_data_Plugin_Record_no);
    QString key   = item->text(enum_global_exif_data_interface_exif_data_KEY);
    QString value = item->text(enum_global_exif_data_interface_exif_data_Value);


    QString command_select = "select  bookmarked_exif_data from '" + tablename + "' where INT = ?";
    QStringList Int_value;
    Int_value << rec_no;
    QString stored_exif_data = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_select, Int_value, 0, db_path, Q_FUNC_INFO);

    if(!stored_exif_data.endsWith("\n"))
        stored_exif_data.append("\n");

    QString exif_metadata_str = QString(key + " : " + value + "\n");

    if(!item->checkState(enum_global_exif_data_interface_exif_data_KEY))
    {
        if(stored_exif_data.contains(exif_metadata_str))
        {
            stored_exif_data.remove(exif_metadata_str);

            QString command = "update '" + tablename + "' set bookmarked_exif_data = ? where INT = ?";
            QStringList list;

            list << stored_exif_data << rec_no;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);
        }
    }
    else
    {

        if(!stored_exif_data.contains(exif_metadata_str))
        {
            stored_exif_data +=  QString(key + " : " + value + "\n");

            qint64 child_count = item->childCount();
            for(int mm = 0; mm < child_count; mm++)
            {
                stored_exif_data += item->child(mm)->text(enum_global_exif_data_interface_exif_data_Value).trimmed() + "\n";
            }

            QString command = "update '" + tablename + "' set bookmarked_exif_data = ? where INT = ?";
            QStringList list;
            list << stored_exif_data << rec_no;

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,list,db_path, Q_FUNC_INFO);
        }
    }

    QString bookmark_status = QString(MACRO_FALSE_VALUE_NUMERIC_QString);

    bool check_status = check_any_exif_or_apple_metadata_item_selected();
    if(check_status)
        bookmark_status = QString(MACRO_TRUE_VALUE_NUMERIC_QString);


    handle_bookmark_from_detailed_info_exif_metadata_clicked(rec_no,bookmark_status);

}

void MainWindow::slot_add_saved_maps_in_case_tree()
{
    QTreeWidgetItem *sub_child;
    QString saved_maps_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
    int records_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, saved_maps_db_path, QStringList(), Q_FUNC_INFO);


    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        if(treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_MAPS)->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Saved_Maps))
        {
            for(int j=0; j < treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_MAPS)->childCount(); j++)
            {

                sub_child = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_MAPS)->child(j);

                if(sub_child->text(enum_global_case_tree_display_column_FIRST) == QString(MACRO_Plugin_Name_Saved_Maps) && sub_child->text(enum_global_case_tree_display_column_SECOND) == global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString())
                {
                    sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Saved_Maps) + " (" + QString::number(records_count) + ")");
                    sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_Plugin_Name_Saved_Maps));
                    sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

                    return;
                }
            }

            sub_child = new QTreeWidgetItem;
            sub_child->setText(enum_global_case_tree_display_column_ZERO, QString(MACRO_CASE_TREE_Saved_Maps) + " (" + QString::number(records_count) + ")");
            sub_child->setText(enum_global_case_tree_display_column_FIRST,QString(MACRO_Plugin_Name_Saved_Maps));
            sub_child->setText(enum_global_case_tree_display_column_SECOND,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString());

            if(records_count != 0)
                treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_SAVED_MAPS)->addChild(sub_child);

            break;
        }
    }
}


void MainWindow::slot_update_bookmark_value_of_other_display_and_db(QString record_no,QString plugin_name,QString tab_name,QString status,QStringList applied_tags_list , QString fs_db_path, QString os_name_str, QString feature_where_from_called)
{

    if(feature_where_from_called == MACRO_CASE_TREE_Artifacts_Keyword_Search)
    {
        update_bookmark_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
        //update_bookmark_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list,os_name_str);
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
        //update_bookmark_value_in_notes_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Source)
    {
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Artifacts)
    {
        update_bookmark_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
        //update_bookmark_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list,os_name_str);
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
        //update_bookmark_value_in_notes_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Hashset)
    {
        update_bookmark_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
        //update_bookmark_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list,os_name_str);
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
        update_bookmark_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
        //update_bookmark_value_in_notes_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    }
    //    else if(feature_where_from_called == MACRO_Plugin_Name_Notes_Search)
    //    {
    //        update_bookmark_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
    //        update_bookmark_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list,os_name_str);
    //        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
    //        update_bookmark_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    //    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Content_Search)
    {
        //update_bookmark_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list,os_name_str);
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
        //update_bookmark_value_in_notes_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    }
    //    else if(feature_where_from_called == MACRO_CASE_TREE_Screenshots)
    //    {
    //        update_bookmark_value_in_notes_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    //    }
    else if(feature_where_from_called == MACRO_Tag_Name_Bookmarks)
    {
        update_bookmark_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
        //update_bookmark_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list,os_name_str);
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
        update_bookmark_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
        //update_bookmark_value_in_notes_search_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
        update_bookmark_value_in_ram_analysis_saved_db_and_display(record_no,plugin_name,tab_name,status,os_name_str);
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Ram_Analysis_Saved)
    {
        update_bookmark_value_in_bookmarks_db_and_display(record_no,plugin_name,tab_name,status,fs_db_path, os_name_str);
    }

    common_add_tag_name_into_case_tree(MACRO_Tag_Name_Bookmarks);

}

void MainWindow::slot_update_notes_value_of_other_display_and_db(QString record_no, QString plugin_name, QString tab_name, QString status, QStringList applied_tags_list, QString os_name_str, QString feature_where_from_called)
{
    recon_static_functions::debug_intensive(" START",Q_FUNC_INFO);

    if(feature_where_from_called == MACRO_CASE_TREE_Artifacts_Keyword_Search)
    {
        update_notes_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
    }
    else if(feature_where_from_called == MACRO_Plugin_Name_Tag_Search)
    {
        if(plugin_name == MACRO_Plugin_Name_Plist_Viewer)
        {
            update_notes_value_in_plist_content_info_db(record_no,plugin_name,tab_name,status, os_name_str);
        }
        else if(plugin_name == MACRO_Plugin_Name_Sqlite_Viewer)
        {
            update_notes_value_in_sqlite_content_info_db(record_no,plugin_name,tab_name,status, os_name_str);
        }
        else
        {
            update_notes_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
            update_notes_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
            update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);

            update_notes_value_in_notes_search_db_and_display_from_ram_analysis_saved(record_no,plugin_name,tab_name,status,os_name_str);
            update_notes_value_in_ram_analysis_saved_db_and_display_from_notes_search(record_no,plugin_name,tab_name,status, os_name_str);
        }
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Source)
    {
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Artifacts)
    {
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
        update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
    }
    else if(feature_where_from_called == MACRO_Tag_Name_Bookmarks)
    {
        update_notes_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
        update_notes_value_in_notes_search_db_and_display_from_ram_analysis_saved(record_no,plugin_name,tab_name,status,os_name_str);
        update_notes_value_in_ram_analysis_saved_db_and_display_from_notes_search(record_no,plugin_name,tab_name,status, os_name_str);

    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Hashset)
    {
        update_notes_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
        update_notes_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
    }
    else if(feature_where_from_called == MACRO_Plugin_Name_Notes_Search)
    {
        if(plugin_name == MACRO_Plugin_Name_Plist_Viewer)
        {
            update_notes_value_in_plist_content_info_db(record_no,plugin_name,tab_name,status, os_name_str);
        }
        else if(plugin_name == MACRO_Plugin_Name_Sqlite_Viewer)
        {
            update_notes_value_in_sqlite_content_info_db(record_no,plugin_name,tab_name,status, os_name_str);
        }
        else
        {
            update_notes_value_in_plugin_db(record_no,plugin_name,tab_name,status, os_name_str);
            update_notes_value_in_keyword_search_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
            update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
            update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);


            update_notes_value_in_notes_search_db_and_display_from_ram_analysis_saved(record_no,plugin_name,tab_name,status,os_name_str);
            update_notes_value_in_ram_analysis_saved_db_and_display_from_notes_search(record_no,plugin_name,tab_name,status, os_name_str);
        }
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Content_Search)
    {
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
    }
    else if(feature_where_from_called == MACRO_CASE_TREE_Ram_Analysis_Saved)
    {
        update_notes_value_in_notes_search_db_and_display_from_ram_analysis_saved(record_no,plugin_name,tab_name,status,os_name_str);
        update_notes_value_in_tag_search_db_and_display(record_no,plugin_name,tab_name,status,applied_tags_list, os_name_str);
        update_notes_value_in_bookmark_db_and_display(record_no,plugin_name,tab_name,status, os_name_str);
    }



    delete_entry_of_note_from_case_tree();

    recon_static_functions::debug_intensive(" END",Q_FUNC_INFO);

}

void MainWindow::slot_update_bookmark_value_from_sqlite_viewer(QString record_no, QString plugin_name, QString sql_tab_name, QString status, QString tag_name, QString display_col_values, QString file_path, QString os_name_str, QString sqliteviewer_db_table_name_of_tag, QString current_table_name, QString saved_state_db_path)
{
    //    recon_static_functions::app_debug(" -Start",Q_FUNC_INFO);

    //    QString bookmark_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Bookmark_Search_In_Result_QString).toString() + "bookmarks.sqlite";

    //    if(status == "1")
    //    {
    //        QStringList arg_list;
    //        arg_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << sqliteviewer_db_table_name_of_tag << current_table_name << record_no;
    //        QString INT_str = global_recon_555_helper_singular_class_obj->get_string_from_db_with_addbindvalue_by_dbpath("select INT from tags where plugin_name = ? AND tab_name = ? AND table_name = ? AND record_no = ?", arg_list, 0, bookmark_db_path, Q_FUNC_INFO);
    //        if(INT_str.trimmed().isEmpty())
    //        {
    //            QFileInfo info(file_path);
    //            QString command = "INSERT INTO tags(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,"
    //                              "os_scheme_display, item0, item1, item2, feature_db_path , csv_plugin_name , csv_tab_name) VALUES (?,?, ?,?,?,?,?, ?,?,?,? ,?,?)";

    //            QStringList arg_list;
    //            arg_list << plugin_name
    //                     << plugin_name
    //                     << sqliteviewer_db_table_name_of_tag
    //                     << current_table_name
    //                     << record_no
    //                     << tag_name
    //                     << os_name_str
    //                     << info.fileName()
    //                     << file_path
    //                     << display_col_values.replace(";", "\n")
    //                     << saved_state_db_path
    //                     << QString(MACRO_Plugin_Name_Sqlite_Viewer)
    //                     << QString(MACRO_Plugin_Name_Sqlite_Viewer);

    //            recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command,arg_list,bookmark_db_path,Q_FUNC_INFO);
    //        }
    //    }
    //    else if(status == "0")
    //    {
    //        QString command = QString("Delete from tags Where record_no=? AND plugin_name=? AND tab_name=?");
    //        QStringList arg_list;
    //        arg_list << record_no << plugin_name << sqliteviewer_db_table_name_of_tag;
    //        recon_static_functions::execute_db_command_with_addbindvalue_by_dbpath(command, arg_list, bookmark_db_path, Q_FUNC_INFO);
    //    }
    //    else
    //        return;

    //    common_add_tag_name_into_case_tree(QString(MACRO_Tag_Name_Bookmarks));

    recon_static_functions::app_debug(" -End",Q_FUNC_INFO);
}


void MainWindow::slot_update_hashset_menu_in_other_submenu_of_hashset_from_plugin(QString db_name, QString db_path)
{
    emit signal_update_hashset_submenu_of_file_system(db_name,db_path);
    emit signal_update_hashset_submenu_of_tablewidget_loader(db_name,db_path);
}

void MainWindow::slot_update_hashset_submenu_from_tablewidget_loader(QString db_name, QString db_path)
{
    emit signal_update_hashset_submenu_of_file_system(db_name,db_path);
    emit signal_update_hashset_submenu_of_plugin(db_name, db_path);
}

void MainWindow::slot_update_hashset_submenu_from_file_system(QString db_name, QString db_path)
{
    emit signal_update_hashset_submenu_of_tablewidget_loader(db_name,db_path);
    emit signal_update_hashset_submenu_of_plugin(db_name, db_path);
}

void MainWindow::slot_update_sqlite_db_for_remained_tags(QString plugin_name,  QString table_name,QString viewer_name, QString record_no, QStringList list_remained_tags, QString os_scheme)
{
    QString remained_tags_str = list_remained_tags.join(",");

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_SQlite_Viewer_In_Result_QString).toString() + "sqlite_viewer.sqlite";

    QString command = QString("select savedstate_dest_dir_path from sv_index where viewer_display_name = ?");

    QStringList list_values;
    list_values << viewer_name;
    QString dest_dir_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command, list_values,0, db_file, Q_FUNC_INFO);

    db_file = dest_dir_path + "/MainDataDisplay_SavedState.sqlite";

    slot_remove_entry_file_type_based_in_stack_and_refresh_widget(MACRO_CASE_TREE_Sqlite_Viewer,viewer_name);

    QString command2 = "update '" + table_name + "' set RECON_Examiner_Tags = ? where RECON_INT=?";
    QStringList arg_list;
    arg_list << remained_tags_str << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command2,arg_list,db_file,Q_FUNC_INFO);

}

void MainWindow::slot_update_hex_and_text_db_for_removed_tags(QString plugin_name,  QString start_index,QString end_index, QString viewer_name, QStringList list_remained_tags, QString os_scheme)
{
    QString remained_tags_str = list_remained_tags.join(",");

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    if(plugin_name == MACRO_Plugin_Name_Hex_Viewer)
    {
        QString db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + "hex_viewer.sqlite";

        QString command;
        QStringList arg_list;
        command = "delete from hex_viewer_blocks where block_start_offset = ? AND block_end_offset = ? AND block_name = ? AND viewer_display_name = ?";
        arg_list << start_index << end_index << remained_tags_str << viewer_name;

        slot_remove_entry_file_type_based_in_stack_and_refresh_widget(MACRO_CASE_TREE_Hex_Viewer,viewer_name);

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_file,Q_FUNC_INFO);

        QString dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + viewer_name.replace(" ", "_");

        if(recon_static_functions::remove_safely_blank_dir_OR_file(dir_path, Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("Blank Directory Removed Safely--" + dir_path  , Q_FUNC_INFO);
        }

    }

}

void MainWindow::slot_update_other_display_and_db_for_remained_tags(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme)
{
    QString remained_tags_str = list_remained_tags.join(",");

    update_remained_tags_in_tag_search_db_and_display(plugin_name,tab_name,record_no,list_remained_tags,os_scheme);
    update_remained_tags_in_plugin_db(remained_tags_str,plugin_name,tab_name,record_no, os_scheme);
    update_bookmark_db_and_display_for_current_record(remained_tags_str,plugin_name,tab_name,record_no,os_scheme);
    update_remained_tag_in_content_search_db_and_display(remained_tags_str, plugin_name, tab_name, record_no,os_scheme);
    update_remained_tag_in_ram_analysis_saved_db_and_display(plugin_name,tab_name,record_no,remained_tags_str,os_scheme);
    update_story_board_display_on_adding_tags();
    update_remained_tag_in_graph_result_saved_db_and_display(plugin_name,tab_name,record_no,remained_tags_str);

}


void MainWindow::slot_update_other_display_and_db_for_remained_tags_in_plist_viewer(QString plugin_name, QString tab_name, QString record_no, QStringList list_remained_tags, QString os_scheme)
{
    update_remained_tags_in_tag_search_db_and_display(plugin_name,tab_name,record_no,list_remained_tags,os_scheme);
}

void MainWindow::slot_add_notes_added_in_database_for_plist_viewer(QString record_no, QString tab_name, QString os_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString viewer_display_name, QString note_text)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(exported_content_info_db);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,notes,file_name,file_path,key,value, keypath,type,source_count_name "
                  "FROM plist_content_info WHERE file_path = ? AND recon_tag_value = ? AND viewer_display_name =  ? AND record_no = ?");
    query.addBindValue(file_path);
    query.addBindValue(clicked_tag_name);
    query.addBindValue(viewer_display_name);
    query.addBindValue(record_no);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(query.next())
    {
        QStringList arg_list;

        arg_list << QString(MACRO_Plugin_Name_Plist_Viewer) << QString(MACRO_Plugin_Name_Plist_Viewer) << tab_name << QString("plist_content_info")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << note_text << os_name << query.value(9).toString().trimmed() << query.value(3).toString().trimmed()
                 << query.value(4).toString().trimmed() << query.value(5).toString().trimmed() << query.value(8).toString().trimmed()
                 << query.value(6).toString().trimmed() << viewer_display_name;


        QString command_insert = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,bookmark,"
                                 "notes,os_scheme_display,source_count_name,item0,item1, item2, item3,item4, item5)  VALUES (?,?,?,?,?,  ?,?,?,?,?, ?,?,?,?,?, ?)";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_Plist_Viewer) ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes), os_name);
    }

    source_db.close();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::slot_add_notes_added_in_database_for_sqlite_viewer(QString record_no, QString tab_name, QString os_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString note_text)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(exported_content_info_db);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,notes,file_name,file_path,source_count_name, db_table_name, viewer_display_name, sqlite_query_data "
                  "FROM sv_content_info WHERE file_path = ? AND recon_tag_value = ?  AND record_no_in_sqliteviewer = ? AND sql_tab_name = ?");
    query.addBindValue(file_path);
    query.addBindValue(clicked_tag_name);
    query.addBindValue(record_no);
    query.addBindValue(tab_name);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(query.next())
    {
        QStringList arg_list;

        arg_list << QString(MACRO_Plugin_Name_Sqlite_Viewer) << QString(MACRO_Plugin_Name_Sqlite_Viewer) << tab_name << QString("sv_content_info")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << note_text << os_name << query.value(5).toString().trimmed() << query.value(3).toString().trimmed()
                 << query.value(4).toString().trimmed() << query.value(6).toString().trimmed() << query.value(7).toString().trimmed() << query.value(8).toString().trimmed();

        QString command_insert = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,bookmark,"
                                 "notes,os_scheme_display,source_count_name,item0,item1, item2, item3, item4)  VALUES (?,?,?,?,?,  ?,?,?,?,?, ?,?,?,?,?)";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_Sqlite_Viewer) ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes), os_name);
    }

    source_db.close();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::slot_add_notes_added_in_database_for_screenshot(QString record_no, QString tab_name, QString file_path, QString clicked_tag_name, QString notes_db_path, QString note_text)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString screenshots_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(screenshots_db);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + screenshots_db,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,notes,item0,item1, item2, os_scheme_display "
                  "FROM tags WHERE item1 = ?  AND plugin_name =  ? AND tab_name = ? AND INT = ?");
    query.addBindValue(file_path);
    query.addBindValue(QString(MACRO_Plugin_Name_Screenshots));
    query.addBindValue(QString(MACRO_Plugin_Name_Screenshots));
    query.addBindValue(record_no);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(query.next())
    {
        QStringList arg_list;

        arg_list << QString(MACRO_Plugin_Name_Screenshots) << QString(MACRO_Plugin_Name_Screenshots) << QString(MACRO_Plugin_Name_Screenshots) << QString("screenshots")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << note_text << query.value(6).toString().trimmed() << "" << query.value(3).toString().trimmed()
                 << query.value(4).toString().trimmed() << global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(query.value(5).toString().trimmed(), Q_FUNC_INFO) << ""
                 <<"" << "";


        QString command_insert = "INSERT INTO notes(category,plugin_name,tab_name,table_name,record_no,recon_tag_value,bookmark,"
                                 "notes,os_scheme_display,source_count_name,item0,item1, item2, item3,item4, item5)  VALUES (?,?,?,?,?,  ?,?,?,?,?, ?,?,?,?,?, ?)";

        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_Screenshots) ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes), query.value(6).toString().trimmed());
    }

    source_db.close();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}


void MainWindow::slot_add_tag_in_other_db(QString tag_name,QString is_emitted_from_plugin,QString plugin_name,QString tab_name,QString record_no,QString db_path, QString os_name_str)
{

    if(tag_name.trimmed().isEmpty())
        return ;


    ///Check if it is file system Viewer
    if(plugin_name == QString(MACRO_Plugin_Name_File_System))
    {
        add_current_record_details_from_file_system_to_tag_search_db(tag_name,plugin_name,tab_name,record_no,tag_name,db_path,os_name_str);
        update_bookmark_db_and_display_for_current_record(tag_name,plugin_name,QString(MACRO_Plugin_File_System_TAB_Name_Files),record_no,os_name_str);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Ram_Analysis_Saved))
    {
        add_and_update_tags_in_database_for_ram_analysis_saved(tag_name,is_emitted_from_plugin,plugin_name,tab_name,record_no,db_path,os_name_str);   // Update tags search db
        update_tags_in_notes_search_db_and_display_from_feature(plugin_name,tab_name,record_no,tag_name,os_name_str);
        update_bookmark_db_and_display_for_current_record(tag_name,plugin_name,tab_name,record_no,os_name_str);
    }
    else
    {
        if(is_emitted_from_plugin != QString(MACRO_Tag_Signal_Emitted_From_Plugin))
        {
            QString record_no_int = record_no;
            add_tags_value_in_plugin_db(tag_name,plugin_name,tab_name,record_no_int, os_name_str);
        }


        add_current_record_details_in_tag_search_db(tag_name,plugin_name,tab_name,record_no,tag_name, os_name_str);

        update_keyword_search_db_for_current_record(tag_name,plugin_name,tab_name,record_no, os_name_str);   // Update keyword search db
        update_bookmark_db_and_display_for_current_record(tag_name,plugin_name,tab_name,record_no,os_name_str);
    }



    common_add_tag_name_into_case_tree(tag_name);
    delete_tags_from_tag_serach_other_than_current_tag_and_for_current_record(tag_name, record_no , plugin_name , tab_name);

    update_story_board_display_on_adding_tags();

}

void MainWindow::update_story_board_display_on_adding_tags()
{
    return;

    //    for(int i = 0 ; i < list_stroy_board_obj.size() ; i++)
    //    {
    //        story_board* story_board_for_update_obj = list_stroy_board_obj.at(i);
    //        if(story_board_for_update_obj != NULL)
    //            story_board_for_update_obj->pub_update_combobox_and_tablewidget_display();
    //    }
}


void MainWindow::slot_add_notes_added_in_database_for_plugin(QString bookmark, QString category, QString record_no, QString plugin_name, QString tab_name,QString table_name, QString note_data, QString resut_note_db_file, QString os_name)
{

    QString tmp_plg_name = plugin_name;
    tmp_plg_name.replace(" ","_");

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name);
    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name);
    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return;


    QStringList cols_type_list = st_csv_info_obj.display_data_type_QList_tablewidget.at(tab_index);

    int record_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString record_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(record_index);

    int tag_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    QString tag_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(tag_index);

    int bookmark_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);
    QString bookmark_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(bookmark_index);

    int scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    QString os_scheme_col_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(scheme_index);


    QStringList current_table_colum_list = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index);


    if(current_table_colum_list.contains(tag_column_name))
    {
        int index_tmp_tag = current_table_colum_list.indexOf(tag_column_name,0);
        current_table_colum_list.removeAt(index_tmp_tag);
    }

    if(current_table_colum_list.contains(record_column_name))
    {
        int index_tmp_record = current_table_colum_list.indexOf(record_column_name,0);
        current_table_colum_list.removeAt(index_tmp_record);
    }

    if(current_table_colum_list.contains(bookmark_column_name))
    {
        int index_tmp_bookmark = current_table_colum_list.indexOf(bookmark_column_name,0);
        current_table_colum_list.removeAt(index_tmp_bookmark);
    }

    if(current_table_colum_list.contains(os_scheme_col_name))
    {
        int index_os_scheme = current_table_colum_list.indexOf(os_scheme_col_name,0);
        current_table_colum_list.removeAt(index_os_scheme);
    }


    QString time_column_name;
    if(cols_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString))
    {
        int time_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        time_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(time_index);
    }

    QString size_column_name;
    if(cols_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString))
    {
        int size_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString,0);
        size_column_name = st_csv_info_obj.display_column_name_QList_tablewidget.at(tab_index).at(size_index);
    }

    QString command_already_record = "SELECT INT FROM notes WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display = ?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << plugin_name << tab_name << record_no << os_name;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,resut_note_db_file,Q_FUNC_INFO);

    QString rec_no_without_zeros = record_no;

    QStringList arg_list_for_notes;
    arg_list_for_notes << rec_no_without_zeros;

    QString previous_note_command = "select notes from '"+ table_name +"' where INT = ?";
    QString previous_note = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(previous_note_command, arg_list_for_notes, 0, plugins_destination_db_file,Q_FUNC_INFO);


    QString applied_tags_command = "select recon_tag_value from '"+ table_name +"' where INT = ?";
    QString applied_tags = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(applied_tags_command, arg_list_for_notes, 0, plugins_destination_db_file,Q_FUNC_INFO);

    if(INT_str.isEmpty() && ((previous_note == note_data) || (previous_note == "")))
    {
        QStringList notes_specific_arg_list;

        notes_specific_arg_list << category << plugin_name <<  tab_name << table_name << record_no << applied_tags << bookmark
                                << note_data << os_name << "";

        QStringList arg_list = get_column_value_list_from_plugin_db(plugins_destination_db_file,record_no,current_table_colum_list,time_column_name,size_column_name,table_name,notes_specific_arg_list);

        QString command_insert = get_insert_query_for_notes_search(current_table_colum_list, "notes");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,resut_note_db_file,Q_FUNC_INFO);

        recon_helper_standard_obj->notes_search_delete_dbs(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Notes_Search_In_Result_QString).toString(),tab_name, record_no, note_data, category, Q_FUNC_INFO );

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , plugin_name ,tab_name ,QString(MACRO_CASE_TREE_Notes_Category_All_Notes), os_name);

    }
}



void MainWindow::slot_update_tags_search_display_for_singal_record(QString record_str,QString plugin_name,QString tab_name,QString tag_name, QString os_scheme)
{
    emit signal_update_tags_search_display_for_singal_record(record_str,plugin_name,tab_name,tag_name,os_scheme);
}


void MainWindow::slot_remove_tag_entry_in_other_db(QString plugin_name, QString tab_name, QString record, QStringList list_tags_removed, QString os_scheme)
{
    update_tag_search_db_and_display_for_remove_tag(plugin_name,tab_name,record,list_tags_removed, os_scheme);

    delete_empty_tags_from_case_tree_display();


    if(plugin_name == QString(MACRO_Plugin_Name_Text_Viewer) || plugin_name == QString(MACRO_Plugin_Name_Strings_Viewer))
    {
        for(int count = 0; count < list_tags_removed.size(); count++)
            common_add_tag_name_into_case_tree(list_tags_removed.at(count));
    }

}

void MainWindow::slot_remove_tag_entry_in_notes_search_db_and_display(QString plugin_name, QString tab_name, QString record, QString note_val, QString os_scheme)
{
    update_notes_db_and_display_for_remove_tag(plugin_name,tab_name,record,note_val,os_scheme);

    if(plugin_name == QString(MACRO_Plugin_Name_Screenshots))
        update_screenshot_db_and_display(plugin_name,tab_name,record,note_val,os_scheme);

}

void MainWindow::slot_update_case_tab_record_number(QString plugin_name)
{
    if(plugin_name == QString(MACRO_Plugin_Name_Screenshots))
    {
        common_add_screenshots_into_case_tree(plugin_name);
    }
    else if (plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History) || plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        QString category_name;
        if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
            category_name = MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs ;
        else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
            category_name = MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs ;

        common_add_saved_graph_category_name_in_case_tree(category_name);
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph))
    {
        slot_add_saved_timeline_graph_in_case_tree();
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        slot_add_saved_maps_in_case_tree();
    }

}


void MainWindow::slot_update_tag_submenu_of_tablewidget_loader_and_file_system(bool status,QString new_tag,QString color_name)
{
    //    emit signal_update_tags_submenu_of_tablewidget_loader(status,new_tag,color_name);
    emit signal_update_tags_submenu_of_file_system(status,new_tag,color_name);
}

void MainWindow::slot_update_tag_submenu_of_plugin_and_file_system(bool status,QString new_tag, QString colour_name)
{
    emit signal_update_tags_submenu_of_plugin(status,new_tag,colour_name);
    emit signal_update_tags_submenu_of_file_system(status,new_tag,colour_name);
}

void MainWindow::slot_update_tag_submenu_of_plugin_and_tablewidget_loader(bool status,QString new_tag, QString colour_name)
{
    emit signal_update_tags_submenu_of_plugin(status,new_tag,colour_name);
    //    emit signal_update_tags_submenu_of_tablewidget_loader(status,new_tag,colour_name);
}

void MainWindow::slot_add_notes_added_in_database_for_hex_viewer(QString record_no, QString tab_name, QString os_name, QString start_offset, QString end_offset, QString file_path, QString clicked_tag_name, QString notes_db_path)
{
    recon_static_functions::app_debug(" -START",Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(exported_content_info_db);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }


    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark,recon_tag_value,notes,file_name,file_path,start_index,end_index,source_count_name,"
                  "content_file_path FROM hex_content_info WHERE file_path = ? AND recon_tag_value = ? AND start_index =  ? AND end_index = ?");
    query.addBindValue(file_path);
    query.addBindValue(clicked_tag_name);
    query.addBindValue(start_offset);
    query.addBindValue(end_offset);

    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(query.next())
    {
        QStringList arg_list;

        arg_list << QString(MACRO_Plugin_Name_Hex_Viewer) << QString(MACRO_Plugin_Name_Hex_Viewer) << tab_name << QString("hex_content_info")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << query.value(2).toString().trimmed() << os_name <<  query.value(7).toString().trimmed() << query.value(3).toString().trimmed() << query.value(4).toString().trimmed()
                 << start_offset << end_offset << "";

        QStringList db_columns_list;
        db_columns_list << QString("item0") << QString("item1") << QString("item2") << QString("item3") << QString("item4");

        QString command_insert = get_insert_query_for_notes_search(db_columns_list, "notes");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_Hex_Viewer) ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes), os_name);
    }

    source_db.close();

    recon_static_functions::app_debug(" -END",Q_FUNC_INFO);
}

void MainWindow::slot_add_notes_for_file_system(QString record_no, QString tab_name, QString tags_db_path, QString notes_text , QString fs_db_path, QString os_scheme)
{
    recon_static_functions::app_debug("START ",Q_FUNC_INFO);

    QStringList fs_db_path_split_list = fs_db_path.split("/", Qt::SkipEmptyParts);
    QString source_name;

    if(fs_db_path_split_list.size() > 2)
        source_name = fs_db_path_split_list.at(fs_db_path_split_list.size() -2);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(fs_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + fs_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);

    query.prepare("SELECT " + QString("bookmark,recon_tag_value,notes,filename,filepath,file_size,mime_type,md5_hash,extension,file_parent_path")
                  + " FROM files WHERE INT=?");

    query.addBindValue(record_no);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(!query.next())
    {
        recon_static_functions::app_debug(" -- No Record Exist---",Q_FUNC_INFO);
        return;
    }

    QString command_already_record = "SELECT INT FROM notes WHERE plugin_name=? AND tab_name=? AND record_no=? AND os_scheme_display =?";
    QStringList arg_list_for_tags;
    arg_list_for_tags << QString(MACRO_Plugin_Name_File_System) << tab_name << record_no << os_scheme;
    QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_already_record,arg_list_for_tags,0,tags_db_path,Q_FUNC_INFO);

    QStringList arg_list_for_notes;
    arg_list_for_notes << record_no;

    QString previous_note_command = "select notes from files where INT = ?";
    QString previous_note = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(previous_note_command, arg_list_for_notes, 0, fs_db_path,Q_FUNC_INFO);

    if(INT_str.isEmpty() && ((previous_note == notes_text) || (previous_note == "")))
    {
        QString file_path = query.value(4).toString().trimmed();

        QStringList arg_list;

        arg_list << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << QString("files")
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << query.value(2).toString().trimmed() << os_scheme <<source_name << query.value(3).toString().trimmed() << file_path
                 << recon_static_functions::human_readable_size(query.value(5).toString().trimmed(), Q_FUNC_INFO)
                 << query.value(6).toString().trimmed()
                 << "";

        QStringList db_columns_list;
        db_columns_list << QString("item0") << QString("item1") << QString("item2") << QString("item3") << QString("item4");

        QString command_insert = get_insert_query_for_notes_search(db_columns_list, "notes");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,tags_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_File_System) , QString(MACRO_Plugin_File_System_TAB_Name_Files) ,QString(MACRO_CASE_TREE_Notes_Category_All_Notes), os_scheme);
    }

    source_db.close();

    recon_static_functions::app_debug(" END",Q_FUNC_INFO);

}


void MainWindow::slot_add_notes_added_in_database_for_content_search(QString record_no, QString tab_name, QString os_name, QString file_path, QString notes_db_path, QString source_db_path)
{
    recon_static_functions::app_debug("START",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(source_db_path);

    if(!source_db.open())
    {
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("source Db opening ----FAILED---- " + source_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QSqlQuery query(source_db);
    query.prepare("SELECT bookmark, recon_tag_value, notes, file_name, file_path, no_of_hits, keyword_name,"
                  " hit_locations, source_count_name "
                  "FROM files WHERE file_path = ? AND INT = ?");
    query.addBindValue(file_path);
    query.addBindValue(record_no);

    if(!query.exec())
    {
        recon_static_functions::app_debug("query select FAILED " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("query Error " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("EXecuted query " + query.executedQuery(),Q_FUNC_INFO);
        source_db.close();
        return;
    }

    if(query.next())
    {
        QStringList arg_list;

        QString indexed_db_path = source_db_path;
        if(indexed_db_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
            indexed_db_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());

        if(!indexed_db_path.startsWith("/"))
            indexed_db_path.prepend("/");

        arg_list << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_Name_File_System) << QString(MACRO_Plugin_File_System_TAB_Name_Files) << "files"
                 << record_no << query.value(1).toString().trimmed() << query.value(0).toString().trimmed()
                 << query.value(2).toString().trimmed() << os_name << query.value(8).toString().trimmed() << query.value(3).toString().trimmed() << query.value(4).toString().trimmed()
                 << indexed_db_path << query.value(5).toString().trimmed() << query.value(6).toString().trimmed() << query.value(7).toString().trimmed();

        QStringList db_columns_list;
        db_columns_list << QString("item0") << QString("item1") << QString("item2") << QString("item3") << QString("item4") << QString("item5");

        QString command_insert = get_insert_query_for_notes_search(db_columns_list, "notes");
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,notes_db_path,Q_FUNC_INFO);

        common_add_notes_name_into_case_tree(MACRO_CASE_TREE_Notes_Category_All_Notes);
        emit signal_update_tags_search_display_for_singal_record(record_no , QString(MACRO_Plugin_Name_File_System) ,tab_name , QString(MACRO_CASE_TREE_Notes_Category_All_Notes), os_name);
    }

    source_db.close();

    recon_static_functions::app_debug("END",Q_FUNC_INFO);
}

void MainWindow::slot_change_notes_value_in_index_k_search_db_and_display(QString record_no, QString plugin_name_str, QString tab_name_str, QString text, QStringList applied_tags_list, QString keyword_db_path,QString os_scheme, QString feature_name)
{
    QString indexed_db_name;
    QStringList tmp_list = keyword_db_path.split("/", Qt::SkipEmptyParts);
    if(tmp_list.size() > 0)
        indexed_db_name = tmp_list.at(tmp_list.size() - 1);

    if(indexed_db_name.endsWith(".sqlite"))
        indexed_db_name.replace(".sqlite", "");

    QString contnt_srch_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(contnt_srch_index_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query(*destination_db);
    query.prepare("SELECT search_label_name FROM table_index_content_search WHERE search_db_file_name = ?");
    query.addBindValue(indexed_db_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    if(query.next())
    {
        QString display_name = query.value(0).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + indexed_db_name + ".sqlite";

        QString command = "UPDATE files SET notes=? WHERE plugin_name=? AND tab_name=? AND INT=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << text << plugin_name_str << tab_name_str << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);
}

void MainWindow::slot_change_bookmark_value_in_index_k_search_db_and_display(QString record_no, QString plugin_name_str, QString tab_name_str, QString status, QStringList applied_tags_list, QString keyword_db_path,QString os_scheme, QString feature_name)
{
    QString indexed_db_name;
    QStringList tmp_list = keyword_db_path.split("/", Qt::SkipEmptyParts);
    if(tmp_list.size() > 0)
        indexed_db_name = tmp_list.at(tmp_list.size() - 1);

    if(indexed_db_name.endsWith(".sqlite"))
        indexed_db_name.replace(".sqlite", "");

    QString contnt_srch_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(contnt_srch_index_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query(*destination_db);
    query.prepare("SELECT search_label_name FROM table_index_content_search WHERE search_db_file_name = ?");
    query.addBindValue(indexed_db_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    if(query.next())
    {
        QString display_name = query.value(0).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + indexed_db_name + ".sqlite";

        QString command = "UPDATE files SET bookmark=? WHERE plugin_name=? AND tab_name=? AND INT=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << status << plugin_name_str << tab_name_str << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);
}

void MainWindow::slot_change_tag_value_in_index_k_search_db_and_display(QString record_no, QString plugin_name_str, QString tab_name_str, QString applied_tag_name, QString keyword_db_path,QString os_scheme, QString feature_name)
{
    QString indexed_db_name;
    QStringList tmp_list = keyword_db_path.split("/", Qt::SkipEmptyParts);
    if(tmp_list.size() > 0)
        indexed_db_name = tmp_list.at(tmp_list.size() - 1);

    if(indexed_db_name.endsWith(".sqlite"))
        indexed_db_name.replace(".sqlite", "");

    QString contnt_srch_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + "index_content_search.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(contnt_srch_index_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query(*destination_db);
    query.prepare("SELECT search_label_name FROM table_index_content_search WHERE search_db_file_name = ?");
    query.addBindValue(indexed_db_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    if(query.next())
    {
        QString display_name = query.value(0).toString();

        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Content_Search_In_Result_QString).toString() + indexed_db_name + ".sqlite";

        QString command = "UPDATE files SET recon_tag_value = ? WHERE plugin_name=? AND tab_name=? AND INT=? AND os_scheme_display =?";
        QStringList arg_list;
        arg_list << applied_tag_name << plugin_name_str << tab_name_str << record_no << os_scheme;
        recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,db_path,Q_FUNC_INFO);

    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);
}

void MainWindow::slot_thread_carved_files_separate_process_finished()
{
    bool_thread_carve_files_seprate_process = false;

    emit signal_carved_files_separate_process_PBAR_finished(MACRO_JobType_Carved_Files);

    ///stop process
    if(process_carve_files != nullptr)
    {
        if(process_carve_files->state() == QProcess::Running)
            process_carve_files->terminate();

        if(process_carve_files->state() == QProcess::NotRunning)
            delete process_carve_files;
    }

    ///stop thread
    if(thread_carve_files_obj != nullptr)
    {
        if(thread_carve_files_obj->isRunning())
            thread_carve_files_obj->quit();

        if(!thread_carve_files_obj->isRunning())
            delete thread_carve_files_obj;
    }
}

void MainWindow::slot_right_click_snapshot_clicked(QString source_count_name)
{

    if(recon_case_obj->pub_is_file_system_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Can not process snapshots until source(s) have been added to case and apple timestamps have been extracted in case of Apple Filesystems.");
        return;
    }

    if(snapshots_obj->pub_is_snapshot_thread_running())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "Snapshots already running.");
        return;
    }

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    if(struct_source_info.snapshots_QList.size() <= 0)
    {
        QMessageBox::information(this, recon_static_functions::get_app_name(), "Snapshots not available for '" + struct_source_info.complete_source_name + "'");
        return ;
    }

    snapshots_obj->pub_set_mutex_for_fs_main_db(recon_case_obj->pub_get_fs_db_exclusive_mutex());
    snapshots_obj->pub_set_source_count_name_and_source_db_path(source_count_name);
    snapshots_obj->pub_create_ui();
    snapshots_obj->show();
}

void MainWindow::slot_tablewidget_goto_record_for_snapshots(QString tab_name, QString snanpshot_tempered_name, QString record, QString feature_name)
{
    display_case_tabs(snanpshot_tempered_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),QString(MACRO_CASE_TREE_Snapshots),feature_name,"",Q_FUNC_INFO);
    emit signal_goto_record_snapshots(record,tab_name);
}


void MainWindow::slot_carve_files_clicked(QString source_count_name, QString complete_file_path, bool bool_carve_unallocated)
{

    source_count_name  = source_count_name.trimmed();
    complete_file_path = complete_file_path.trimmed();
    if(complete_file_path.isEmpty() || source_count_name.isEmpty())
        return;

    if(bool_thread_carve_files_seprate_process)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Carving File is already in process, Please wait...");
        return;
    }


    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    ///=============Set Essentials Database-Start===============///
    QString destination_db_file = set_carver_essentials_database(struct_source_info, bool_carve_unallocated , complete_file_path);
    destination_db_file = destination_db_file.trimmed();
    if(destination_db_file.isEmpty())
        return;
    ///=============Set Essentials Database-End===============///



    ///====Start Separate carving process===///
    disconnect(this, SIGNAL(signal_carved_files_separate_process_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(this, SIGNAL(signal_carved_files_separate_process_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));

    disconnect(this, SIGNAL(signal_carved_files_separate_process_PBAR_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(this, SIGNAL(signal_carved_files_separate_process_PBAR_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));

    disconnect(this, SIGNAL(signal_carved_files_separate_process_PBAR_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(this, SIGNAL(signal_carved_files_separate_process_PBAR_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));

    disconnect(task_status_window_obj, SIGNAL(signal_cancel_carved_files_extraction(QString)), this, SLOT(slot_carver_separate_process_cancelled(QString)));
    connect(task_status_window_obj, SIGNAL(signal_cancel_carved_files_extraction(QString)), this, SLOT(slot_carver_separate_process_cancelled(QString)));

    process_carve_files    = new QProcess(this);
    process_carve_files->setReadChannel(QProcess::StandardOutput);
    connect(process_carve_files, SIGNAL(readyReadStandardError()), this, SLOT(slot_carver_process_data()));

    thread_carve_files_obj = new thread_carve_files(this);
    connect(thread_carve_files_obj, SIGNAL(finished()), this, SLOT(slot_thread_carved_files_separate_process_finished()));

    thread_carve_files_obj->pub_set_current_source_info(struct_source_info);
    thread_carve_files_obj->pub_set_separate_process_obj(process_carve_files);
    thread_carve_files_obj->pub_set_essential_db_path(destination_db_file);

    thread_carve_files_obj->start();

    bool_thread_carve_files_seprate_process = true;
}

void MainWindow::slot_carver_process_data()
{
    QString data_str = QString::fromLocal8Bit(process_carve_files->readAllStandardError()).trimmed();

    if(data_str == QString("Start_Clicked"))
    {
        emit signal_carved_files_separate_process_started(QString(MACRO_JobType_Carved_Files));
    }
    else if(data_str.contains("progress:;"))
    {
        // "\"progress:;99;Source1;12558\""
        QStringList value_split_list = data_str.split(";");
        if(value_split_list.size() > 3)
        {
            QString progress_val_str = value_split_list.at(1); //99
            qint64  progress_val     = progress_val_str.toInt();

            QString source_count_name = value_split_list.at(2); //Source1
            struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

            QString total_files_str  = value_split_list.at(3); //12558\"
            if(total_files_str.endsWith("\""))
                total_files_str.chop(1);
            total_files_str = total_files_str.trimmed();

            qint64 total_files_found = total_files_str.toInt();

            emit signal_carved_files_separate_process_PBAR_value(MACRO_JobType_Carved_Files, struct_source_info.source_name, true, total_files_found, progress_val,false);
        }
    }
    else if(data_str == QString("carver_thread_finished"))
    {

        slot_add_carver_in_case_tree_on_finish();

        slot_thread_carved_files_separate_process_finished();
    }
}

void MainWindow::slot_carver_separate_process_cancelled(QString job_type)
{
    Q_UNUSED(job_type);

    slot_thread_carved_files_separate_process_finished();

    slot_add_carver_in_case_tree_on_finish();
    emit signal_carved_files_separate_process_PBAR_finished(MACRO_JobType_Carved_Files);
}


QString MainWindow::set_carver_essentials_database(struct_GLOBAL_witness_info_source struct_source_info, bool bool_carve_unallocated , QString complete_file_path)
{
    QString carver_essentials_tmp_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Carved_Files_In_Result_QString).toString();
    QDir carve_dir(carver_essentials_tmp_dir);
    if(!carve_dir.exists())
        carve_dir.mkpath(carver_essentials_tmp_dir);

    QString destination_db_file = carver_essentials_tmp_dir + "carver_essential.sqlite";
    if(QFile(destination_db_file).exists())
        QFile::remove(destination_db_file);

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);

    destination_db.setDatabaseName(destination_db_file);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }

    QSqlQuery query(destination_db);

    QString command = "create table carver_essential (INT INTEGER PRIMARY KEY, key varchar(500), value varchar(500))";
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }

    ///insert source count name
    query.clear();
    query.prepare("Insert into carver_essential(key,value)values(?,?)");
    query.addBindValue("source_count_name");
    query.addBindValue(struct_source_info.source_count_name);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(source_count_name)  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }

    ///complete file path
    if(bool_carve_unallocated)
        complete_file_path = "/dev/r" + struct_source_info.source_disk_identifier;

    query.clear();
    query.prepare("Insert into carver_essential(key,value)values(?,?)");
    query.addBindValue("complete_file_path");
    query.addBindValue(complete_file_path);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(complete_file_path)  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }

    ///Output Path
    QString output_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + struct_source_info.source_count_name;

    query.clear();
    query.prepare("Insert into carver_essential(key,value)values(?,?)");
    query.addBindValue("output_path");
    query.addBindValue(output_path);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(output_path)  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }


    ///Unallocated Status
    QString unallocated_str = QString::number(0);
    if(bool_carve_unallocated)
        unallocated_str = QString::number(1);

    query.clear();
    query.prepare("Insert into carver_essential(key,value)values(?,?)");
    query.addBindValue("bool_carve_unallocated");
    query.addBindValue(unallocated_str);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(bool_carve_unallocated)  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }

    ///total_image_size_to_process
    qint64 total_image_size_to_process = 0;
    if(bool_carve_unallocated)
    {
        QString command = "Select total_size from tbl_source_index where source_count_name = ?";
        QString sources_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "sources_info.sqlite";
        QString source_size_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,QStringList(struct_source_info.source_count_name) ,0, sources_db_path, Q_FUNC_INFO);

        total_image_size_to_process = source_size_str.toLongLong();
    }
    else
        total_image_size_to_process = QFileInfo(complete_file_path).size();

    query.clear();
    query.prepare("Insert into carver_essential(key,value)values(?,?)");
    query.addBindValue("total_image_size_to_process");
    query.addBindValue(QString::number(total_image_size_to_process));
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(total_image_size_to_process)  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }



    ///RECON Result Dir
    QString recon_result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    query.clear();
    query.prepare("Insert into carver_essential(key,value)values(?,?)");
    query.addBindValue("recon_result_dir");
    query.addBindValue(recon_result_dir);
    if(!query.exec())
    {
        recon_static_functions::app_debug(" query(recon_result_dir)  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);
        return QString("");
    }


    destination_db.close();

    return destination_db_file;
}

void MainWindow::slot_add_carver_in_case_tree_on_finish()
{

    QString carver_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";

    QStringList carver_labels_list = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(QString("Select label_name From table_carver_index"), 0, carver_index_db_path, Q_FUNC_INFO);

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_CARVED_FILES);

        if(item->text(0) == QString(MACRO_CASE_TREE_Carved_Files))
        {
            QString item_text_with_records;

            for(int num = 0; num < carver_labels_list.size(); num++)
            {

                QString records_count_str;
                QString carver_label_str = carver_labels_list.at(num);

                QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                QString carver_files_db_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(QString("Select carver_files_db_path From table_carver_index Where label_name = ?"), QStringList(carver_label_str), 0, carver_index_db_path, Q_FUNC_INFO);
                carver_files_db_path =  global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,carver_files_db_path , Q_FUNC_INFO);

                bool bool_is_child_found = false;
                int child_position = -1;
                for(int i = 0; i < item->childCount(); i++)
                {
                    QTreeWidgetItem *child_item = item->child(i);
                    if(child_item->text(0).startsWith(carver_label_str))
                    {
                        child_position = i;
                        bool_is_child_found = true;
                        break;
                    }
                }

                if(bool_is_child_found)
                    continue;

                QString db_cmd = QString("SELECT COUNT(*) FROM files");
                records_count_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(db_cmd , 0 , carver_files_db_path , Q_FUNC_INFO);
                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  carver_label_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  carver_label_str + QString(" (") + records_count_str + QString(")");
                }

                QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Carved_Files_Parent));
                child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                item->addChild(child_item_second);
            }

            item->setExpanded(false);
            break;
        }
    }

}

void MainWindow::slot_carve_data_clicked(QString source_count_name, QString complete_file_path, QString plugin_name, QString tab_name, QString record_no)
{

    if(!QFileInfo(complete_file_path).exists())
        return;


    if(bool_thread_carve_data_running)
    {
        QMessageBox::warning(this,recon_static_functions::get_app_name(), "Carving Data is already in process, Please wait...");
        return;
    }

    carved_data_current_file = complete_file_path;

    process_carve_data    = new QProcess(this);
    process_carve_data->setReadChannel(QProcess::StandardOutput);
    connect(process_carve_data, SIGNAL(readyReadStandardError()), this, SLOT(slot_carver_data_process_data()));
    connect(process_carve_data, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_carver_data_process_data()));

    thread_carve_data_obj = new thread_carve_data(this);
    connect(thread_carve_data_obj, SIGNAL(finished()), this, SLOT(slot_thread_carved_data_finished()));

    disconnect(thread_carve_data_obj, SIGNAL(signal_PBAR_thread_carved_data_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));
    connect(thread_carve_data_obj, SIGNAL(signal_PBAR_thread_carved_data_started(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_started(QString)));

    disconnect(thread_carve_data_obj, SIGNAL(signal_PBAR_thread_carved_data_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(thread_carve_data_obj, SIGNAL(signal_PBAR_thread_carved_data_finished(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));

    disconnect(this, SIGNAL(signal_PBAR_thread_carve_data_carving_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(this, SIGNAL(signal_PBAR_thread_carve_data_carving_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    disconnect(thread_carve_data_obj, SIGNAL(signal_PBAR_thread_carve_data_processing_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)));
    connect(thread_carve_data_obj, SIGNAL(signal_PBAR_thread_carve_data_processing_value(QString,QString,bool,qint64,int,bool)), task_status_window_obj, SLOT(slot_PBAR_file_system_value(QString,QString,bool,qint64,int,bool)), Qt::QueuedConnection);

    disconnect(task_status_window_obj, SIGNAL(signal_cancel_carved_data_extraction(QString)), this, SLOT(slot_carve_data_cancelled(QString)));
    connect(task_status_window_obj, SIGNAL(signal_cancel_carved_data_extraction(QString)), this, SLOT(slot_carve_data_cancelled(QString)));

    disconnect(this, SIGNAL(signal_carved_data_cancelled(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));
    connect(this, SIGNAL(signal_carved_data_cancelled(QString)), task_status_window_obj, SLOT(slot_PBAR_file_system_finished(QString)));


    thread_carve_data_obj->pub_set_essentials(source_count_name,plugin_name, tab_name, record_no, complete_file_path);

    thread_carve_data_obj->pub_set_process_obj(process_carve_data);
    thread_carve_data_obj->start();

    bool_thread_carve_data_running = true;

}

void MainWindow::slot_thread_carved_data_finished()
{
    bool_thread_carve_data_running = false;

    carved_data_current_file.clear();


    ///stop processs
    if(process_carve_data->state() == QProcess::Running)
        process_carve_data->terminate();


    if(process_carve_data != NULL && process_carve_data->state() == QProcess::NotRunning)
        delete process_carve_data;


    ///stop thread
    if(thread_carve_data_obj->isRunning())
        thread_carve_data_obj->quit();


    if(thread_carve_data_obj != NULL && !thread_carve_data_obj->isRunning())
        delete thread_carve_data_obj;

    add_carved_data_in_case_tree_on_finish();

}

void MainWindow::slot_carver_data_process_data()
{
    QString data_str = QString::fromLocal8Bit(process_carve_data->readAllStandardError()).trimmed();
    if(data_str.trimmed().isEmpty())
        data_str = QString::fromLocal8Bit(process_carve_data->readAllStandardOutput()).trimmed();

    QString log_str = data_str;
    if(log_str.contains(" (") && log_str.contains("%) "))
    {
        QStringList tmp_split_list = log_str.split("%) ");
        QString tmp_str = tmp_split_list.at(0);
        if(tmp_str.contains(" ("))
        {
            tmp_split_list = tmp_str.split(" (");
            tmp_str = tmp_split_list.at(tmp_split_list.size() - 1);

            double dble_status = tmp_str.toDouble();

            ///converts double to int
            int status_num = std::round(dble_status);

            emit signal_PBAR_thread_carve_data_carving_value(MACRO_JobType_Carved_Data, QFileInfo(carved_data_current_file).fileName(), true, MACRO_Generic_Progress_No_Records_int, status_num, false);
        }
    }

}

void MainWindow::slot_carve_data_cancelled(QString job_type)
{
    Q_UNUSED(job_type);

    thread_carve_data_obj->pub_cancel_extraction(true);

    emit signal_carved_data_cancelled(job_type);
}

void MainWindow::add_carved_data_in_case_tree_on_finish()
{
    QString carved_data_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Data_In_Result_QString).toString() + "carved_data.sqlite";

    if(!QFileInfo(carved_data_db_path).exists())
        return;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(carved_data_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QStringList carved_data_labels_list = destination_db.tables();

    for(int ii = 0; ii < treewidget_case_display_obj->topLevelItemCount(); ii++)
    {
        QTreeWidgetItem *item = treewidget_case_display_obj->topLevelItem(ii)->child(enum_global_case_tree_display_row_CARVED_DATA);

        if(item->text(enum_global_case_tree_display_column_ZERO) == QString(MACRO_CASE_TREE_Carved_Data))
        {
            QString item_text_with_records;

            for(int num = 0; num < carved_data_labels_list.size(); num++)
            {

                QString records_count_str;
                QString carver_label_str = carved_data_labels_list.at(num);

                ///delete previous child
                for(int i = 0; i < item->childCount(); i++)
                {
                    QTreeWidgetItem *child_item = item->child(i);
                    QString child_text = child_item->text(enum_global_case_tree_display_column_ZERO).trimmed();
                    if(child_text.contains(" ("))
                        child_text = child_text.remove(child_text.lastIndexOf(" ("), child_text.size());

                    if(child_text == carver_label_str)
                    {
                        delete item->child(i);
                        break;
                    }
                }


                /// add new child with records
                QString db_cmd = QString("SELECT COUNT(*) FROM ") + carver_label_str;
                records_count_str = recon_helper_standard_obj->get_string_from_db_by_dbreference(db_cmd , 0 , destination_db , Q_FUNC_INFO);
                if(records_count_str.isEmpty())
                {
                    item_text_with_records =  carver_label_str + QString(" (0)");
                }
                else
                {
                    item_text_with_records =  carver_label_str + QString(" (") + records_count_str + QString(")");
                }

                QTreeWidgetItem *child_item_second = new QTreeWidgetItem;
                child_item_second->setText(enum_global_case_tree_display_column_ZERO, item_text_with_records);
                child_item_second->setText(enum_global_case_tree_display_column_FIRST, QString(MACRO_CASE_TREE_Carved_Data_Parent));
                child_item_second->setText(enum_global_case_tree_display_column_RECORD_COUNT, records_count_str);

                item->addChild(child_item_second);
            }

            item->setExpanded(false);
            break;
        }
    }

    destination_db.close();

}

void MainWindow::slot_run_fs_module_apple_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_apple_metadata(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_exif_metadata(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_exif_metadata(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_signature_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_signature_analysis(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_mime_type_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_mime_type_analysis(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_hashset(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_hashset(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_apple_metadata_already_running_status()
{
    emit signal_r_case_apple_metadata_running_status(recon_case_obj->pub_get_apple_metadata_run_status());
}

void MainWindow::slot_run_fs_module_exif_metadata_already_running_status()
{
    emit signal_r_case_exif_metadata_running_status(recon_case_obj->pub_get_exif_metadata_run_status());

}

void MainWindow::slot_run_fs_module_signature_analysis_already_running_status()
{
    emit signal_r_case_signature_analysis_running_status(recon_case_obj->pub_get_signature_analysis_run_status());

}

void MainWindow::slot_run_fs_module_mime_type_analysis_already_running_status()
{
    emit signal_r_case_mime_type_analysis_running_status(recon_case_obj->pub_get_mime_type_analysis_run_status());

}

void MainWindow::slot_run_fs_module_face_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_face_analysis(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_face_analysis_already_running_status()
{
    emit signal_r_case_face_analysis_running_status(recon_case_obj->pub_get_face_analysis_run_status());

}

void MainWindow::slot_run_fs_module_optical_character_recognition(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_optical_character_recognition(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_optical_character_recognition_already_running_status()
{
    emit signal_r_case_optical_character_recognition_running_status(recon_case_obj->pub_get_optical_character_recognition_run_status());

}


void MainWindow::slot_run_fs_module_weapons(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_weapons(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_weapons_already_running_status()
{
    emit signal_r_case_weapons_running_status(recon_case_obj->pub_get_weapons_run_status());

}

void MainWindow::slot_run_fs_module_fire_analysis(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_fire_analysis(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_fire_analysis_already_running_status()
{
    emit signal_r_case_fire_analysis_running_status(recon_case_obj->pub_get_fire_analysis_run_status());

}

void MainWindow::slot_run_fs_module_skin_tone_detection(QList<struct_global_run_fs_modules_details> run_fs_modules_details_list)
{
    recon_case_obj->pub_start_run_fs_module_skin_tone_detection(run_fs_modules_details_list);
}

void MainWindow::slot_run_fs_module_skin_tone_detection_already_running_status()
{
    emit signal_r_case_skin_tone_detection_running_status(recon_case_obj->pub_get_skin_tone_detection_run_status());

}

void MainWindow::slot_file_openwith_hex_viewer(struct_global_hex_viewer_open_file_data hex_viewer_data_obj)
{
    st_hex_viewer_open_file_data_obj = hex_viewer_data_obj;
    display_case_tabs(st_hex_viewer_open_file_data_obj.hex_viewer_display_name,global_narad_muni_class_obj->get_field(MACRO_NARAD_Case_Name_QString).toString(),MACRO_CASE_TREE_Hex_Viewer,"", st_hex_viewer_open_file_data_obj.hex_feature_db_path,Q_FUNC_INFO);
}


void MainWindow::slot_run_fs_module_hashset_already_running_status()
{
    emit signal_r_case_hashset_running_status(recon_case_obj->pub_get_hashset_run_status());

}

