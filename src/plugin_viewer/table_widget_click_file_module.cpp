#include "plugin_viewer.h"
#include "ui_plugin_viewer.h"

bool plugin_viewer::is_column_of_apple_metadata(QString column_name)
{

    if(column_name.trimmed() == "Content Type" || column_name.trimmed() == "Date Added"
            || column_name.trimmed() == "FS Name" || column_name.trimmed() == "FS Node Count"
            || column_name.trimmed() == "WhereFrom" || column_name.trimmed() == "Latitude"
            || column_name.trimmed() == "Longitude" || column_name.trimmed() == "Altitude"
            || column_name.trimmed() == "Torrent File List" || column_name.trimmed() == "Content Modification Date"
            || column_name.trimmed() == "Content Creation Date" || column_name.trimmed() == "FS Content Change Date"
            || column_name.trimmed() == "FS Creation Date" || column_name.trimmed() == "Used Dates"
            || column_name.trimmed() == "Last Used Date"  || column_name.trimmed() == "File Type")
    {
        return true;
    }
    else
    {
        return false;
    }
}


void plugin_viewer::tablewidget_general_clicked(int tab_index, int row, int column)
{
    recon_static_functions::debug_intensive(" start" , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return;

    cancel_exporter_loop = false;


    if(list_tablewidgets.size() <= tab_index)
        return;


    m_tablewidget *my_tablewidget = list_tablewidgets.at(tab_index);

    if(my_tablewidget == NULL)
        return;

    //QString tb_nm       = ui->tabWidget->tabText(tab_index);
    QString tb_nm = get_exact_tab_name_from_ui(tab_index);
    QString table_name  = global_csv_reader_class_selected_obj->get_table_name_according_tab(get_plugin_name(), tb_nm);
    QString tab_name    =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(get_plugin_name(), table_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);

    QStringList tw_col_type_list = tw_data_type_list_tablewidget.at(tab_index);
    int INT_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    int os_scheme_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);
    QString os_nm_scheme = my_tablewidget->item(row, os_scheme_index)->text();
    QString INT_str = my_tablewidget->item(row, INT_index)->text();
    int bookmark_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(INT_str, table_name, destination_db_file, Q_FUNC_INFO);

    QString result_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    data_prepare_metadata = recon_helper_standard_obj->fetch_metadata_of_plugin(get_plugin_name(),tab_name,INT_str,result_dir,Q_FUNC_INFO).metadata;

    if(column == bookmark_index)
    {
        INT_str = my_tablewidget->item(row, INT_index)->text();
        if(my_tablewidget->item(row,column)->checkState())
        {
            recon_helper_standard_obj->pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display, tab_name, INT_str);

            global_connection_manager_obj->pub_set_bookmark_in_plugin_viewer_db(QStringList(INT_str), get_plugin_name(), tab_name);
            global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(QStringList(INT_str), get_plugin_name(), tab_name);
            global_connection_manager_obj->pub_set_bookmarks_in_artifacts_keyword_search_db(QStringList(INT_str), get_plugin_name(), tab_name);
        }
        else
        {
            recon_helper_standard_obj->pub_remove_bookmarked_record_to_map_for_detailed_info_bookmark_control(map_bookmarked_records_from_table_display, tab_name, INT_str);

            global_connection_manager_obj->pub_remove_bookmark_in_plugin_viewer_db(QStringList(INT_str), get_plugin_name(), tab_name);
            global_connection_manager_obj->pub_delete_bookmark_for_plugin_viewer(QStringList(INT_str), get_plugin_name(), tab_name,"");
            global_connection_manager_obj->pub_remove_bookmarks_in_artifacts_keyword_search_db(QStringList(INT_str), get_plugin_name(), tab_name);

        }

        check_bookmark_status = false;
    }

    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.db_file_path = destination_db_file;
    st_set_meta_obj.detailed_info = data_prepare_metadata;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = INT_str;
    st_set_meta_obj.display_searched_text = searched_keyword;
    st_set_meta_obj.plugin_name = get_plugin_name();
    st_set_meta_obj.tab_name = tab_name;

    emit signal_set_metadata_info(st_set_meta_obj);

    recon_static_functions::debug_intensive(" end" , Q_FUNC_INFO);

}

void plugin_viewer::slot_tableWidget_cellClicked(int row, int column)
{
    recon_static_functions::debug_intensive(" start ",Q_FUNC_INFO);

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        return;
    }

    tablewidget_general_clicked(current_tab_index_only_for_tableview,row,column);

    recon_static_functions::debug_intensive(" end ",Q_FUNC_INFO);

}


void plugin_viewer::slot_tablewidget_current_cell_changed(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        return;
    }

    tablewidget_general_clicked(current_tab_index_only_for_tableview,currentRow, currentColumn);
}

void plugin_viewer::slot_spacebar_quicklook_pressed()
{
    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
    {
        return;
    }

    if(list_tablewidgets.size() <= current_tab_index_only_for_tableview)
        return;

    m_tablewidget *my_tablewidget = list_tablewidgets.at(current_tab_index_only_for_tableview);

    int row_no = my_tablewidget->currentRow();
    if(row_no < 0 || row_no > my_tablewidget->rowCount())
        return;

    QString record_no =  my_tablewidget->item(row_no,2)->text();


    QString complete_file_path = global_recon_file_info_obj->pub_get_recon_info_for_artifacts_first_file_by_dbpath(get_plugin_name(),current_tab_text,record_no,destination_db_file).complete_file_path;
    if(QFileInfo(complete_file_path).exists())
        emit signal_plugin_viewer_space_pressed_for_quicklook(complete_file_path);

}

void plugin_viewer::slot_tableWidget_doubleClicked(int row, int column)
{

    m_tablewidget *m_tablewidget_obj = (m_tablewidget*) sender();

    m_tablewidget_obj->clearSelection();

    QList <QAction *> act_list = menu->actions();

    for(int i = 0 ; i < act_list.size() ; i++)
    {

        if(act_list.at(i) == act_copy_to_clipboard)
            continue;
        else
            act_list.at(i)->setDisabled(true);

    }

    m_tablewidget_obj->item(row,column)->setSelected(true);
}


void plugin_viewer::slot_m_treewidget_itemclicked(QTreeWidgetItem *item, int column)
{
    recon_static_functions::debug_intensive(" start",Q_FUNC_INFO);

    int tree_tab_index =  global_csv_reader_class_selected_obj->get_tab_index_as_per_tabwidget(get_plugin_name(),current_tab_text);
    treewidget_general_clicked(tree_tab_index,item, column);


    recon_static_functions::debug_intensive(" end",Q_FUNC_INFO);

}

void plugin_viewer::treewidget_general_clicked(int tab_index ,QTreeWidgetItem *item, int column)
{
    recon_static_functions::app_debug(" start ",Q_FUNC_INFO);

    if(list_treewidgets.size() <= tab_index)
        return;

    m_treewidget *my_treewidget = list_treewidgets.at(tab_index);

    my_treewidget->setCurrentItem(item,0);


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("  ---FAILED--- to open DB " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    QString table_name = table_name_list_tree.at(tab_index);
    QString tab_name = tab_name_list_tree.at(tab_index);

    QStringList display_name_list = md_header_list_tree.at(tab_index);
    QStringList cols_nam_list =  md_column_name_list_tree.at(tab_index);
    QStringList cols_type_list = md_data_type_list_tree.at(tab_index);
    int columns_count = cols_nam_list.size();

    QStringList tree_col_type_list = data_type_list_tree.at(tab_index);

    int INT_index = tree_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString record_no = item->text(INT_index);
    QString recevied_os_schme_dsply = recon_helper_standard_obj->get_target_os_scheme_display_on_current_record_source(record_no,table_name,destination_db_file,Q_FUNC_INFO);

    bool bool_add_bookmark = false;

    // it mean it is parent
    if(record_no.trimmed() == "")
    {
        recon_static_functions::app_debug(" record no is blank, so it is parent" + QString::number(column),Q_FUNC_INFO);

        // this column having no checkbox
        if(column != 0)
        {
            destination_db.close();
            return;
        }

        QSqlQuery query_update(destination_db);
        QString table_name = table_name_list_tree.at(tab_index);
        QString parent_column_name = mining_parent_column_for_tree(parent_child_list_tree.at(tab_index),column_name_list_tree.at(tab_index));

        m_treewidgetitem *m_item = dynamic_cast<m_treewidgetitem *>(item);

        if(m_item->previous_state() == item->checkState(0))
        {
            // state is not changed so no need to do anything
            destination_db.close();
            return;
        }
        else
        {
            // it mean state is changed
            m_item->set_state(item->checkState(0));

            // we are statically inserted checkbox on zeroth and parent on first position in tree
            if(item->checkState(0))
            {
                bool_add_bookmark = true;
                QString cmd_bookmark = QString("update ") + table_name + " set bookmark = 1 where " + parent_column_name + " = '" + item->text(1).trimmed() + "'";

                if(!query_update.exec(cmd_bookmark))
                {
                    recon_static_functions::app_debug(" ---FAILED--- query_update if",Q_FUNC_INFO);
                    recon_static_functions::app_debug("" + query_update.lastError().text(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("" + cmd_bookmark,Q_FUNC_INFO);
                }
            }
            if(!item->checkState(0))
            {
                bool_add_bookmark = false;
                QString cmd_bookmark = QString("update ") + table_name + " set bookmark = 0 where " + parent_column_name + " = '" + item->text(1).trimmed() + "'";

                if(!query_update.exec(cmd_bookmark))
                {
                    recon_static_functions::app_debug(" ---FAILED--- query_update if",Q_FUNC_INFO);
                    recon_static_functions::app_debug("" + query_update.lastError().text(),Q_FUNC_INFO);
                    recon_static_functions::app_debug("" + cmd_bookmark,Q_FUNC_INFO);
                }
            }

            QString parent_name_str = item->text(1).trimmed();
            QString cmd = "select INT from " + table_name + " where " + parent_column_name + "= ?";
            QStringList arg_list = QStringList(parent_name_str);
            QStringList record_count_list = recon_helper_standard_obj->get_stringlist_from_db_with_addbindvalues_by_dbreference(cmd ,arg_list,0 ,destination_db,Q_FUNC_INFO);

            QString base_tab_name =  global_csv_reader_class_selected_obj->get_tab_name_associated_with_widget(get_plugin_name() , table_name, MACRO_CSV_TEMPLATE_TAB_TYPE_TABLEWIDGET);
            if(bool_add_bookmark)
            {
                global_connection_manager_obj->pub_add_bookmark_for_plugin_viewer(record_count_list, get_plugin_name(), base_tab_name);
            }
            else
            {
                for(int ll = 0 ; ll < record_count_list.size() ; ll++)
                {
                    QString recrd_number = record_count_list.at(ll);
                    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(recrd_number,table_name,destination_db_file,Q_FUNC_INFO);
                    global_connection_manager_obj->pub_delete_bookmark_for_plugin_viewer(QStringList(recrd_number) ,get_plugin_name() ,base_tab_name ,source_count_name);
                }

            }

        }
        destination_db.close();
        return;
    }


    QStringList preview_col_list = md_preview_list_tree.at(tab_index);
    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);
        detached_file_previewer_module_popobj->set_current_index(2);
    }


    QSqlQuery query_select(destination_db);
    QString command = cols_nam_list.join(",");
    command = "select " + command + " from " + table_name + " where INT = " + record_no;

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" ---FAILED--- select query, table name - " + table_name,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_select.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command " + command,Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    data_prepare_metadata.clear();
    data_prepare_metadata += "<b>" + QObject::tr("Plugin") + ":</b>" +QString("\t")+ get_plugin_name() +"<br>" ;
    data_prepare_metadata += "<b>" + QObject::tr("Category") + ":</b>" +QString("\t")+ tab_name +"\n<br> <br>";

    QString source_count_name = recon_helper_standard_obj->get_source_count_name_on_current_record(record_no,table_name,destination_db_file,Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString complete_source_name = struct_target_info.complete_source_name;
    data_prepare_metadata += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +"\t"+ complete_source_name +"<br>";
    if(recevied_os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display))
    {
        QString source_name_str = struct_target_info.source_name;

        data_prepare_metadata += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ source_name_str +"<br>";
    }

    data_prepare_metadata += "<br>";

    current_media = QString("");
    current_media_linked_file_ios.clear();

    bool bool_is_it_virtual_export = false;
    while(query_select.next())
    {

        //        if(preview_index != -1)
        //        {
        //            QString current_media_path = QString::fromUtf8(query_select.value(preview_index).toString().toLocal8Bit().data());
        //        }


        for(int i=0 ; i < columns_count ; i++)
        {
            QString data;

            if(i == preview_index)
            {
                QStringList m_path_list = query_select.value(i).toString().split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                if(m_path_list.size() >= 1)
                {
                    current_media = m_path_list.at(0).toLocal8Bit();
                    if(current_media.startsWith("./Images/"))
                    {
                        bool_is_it_virtual_export = true;
                        current_media =  destination_file_location + current_media.remove(0,1);
                    }
                }
            }

            QString md_data_type = cols_type_list.at(i);
            QString md_data_type_for_tmpuse = md_data_type;

            QString our_column = display_name_list.at(i);
            QString our_value = query_select.value(i).toString();


            // hide extended attribute column which are blank
            bool colmn_atr =  is_column_of_apple_metadata(our_column);
            if(colmn_atr)
            {
                if(our_value.trimmed() == QString(""))
                    continue;

            }

            // if want to hide those item which are blank
            if(md_data_type.endsWith("Hide"))
            {
                md_data_type.remove("Hide");
                if(our_value.trimmed() == "")
                    continue;
            }

            if(md_data_type.startsWith("NewLine"))
            {
                md_data_type.remove("NewLine");
            }

            if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                data = "<b>" + our_column + ":</b>\t" + our_value + "\n";

            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
            {
                if((i-1) > 0)
                {
                    QString data_tp = cols_type_list.at(i-1);
                    data_tp.remove("NewLine");

                    if(data_tp == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                        data = QString("<b>") + our_column + ":</b>\t" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO) + "\n";
                    else
                        data = QString("<br><b>") + our_column + ":</b>\t" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO) + "\n";
                }
                else
                {
                    data = QString("<br><b>") + our_column + ":</b>\t" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO) + "\n";
                }
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                QString value = our_value;
                if(value == QString(""))
                    data = "<b>" + our_column + ":</b>\t" + "" + "\n";
                else if(value == QString("0"))
                    data = "<b>" + our_column + ":</b>\t" +  recon_static_functions::human_readable_size(value, Q_FUNC_INFO) + "\n";
                else
                    data = "<b>" + our_column + ":</b>\t" + recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)" + "\n";
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
            {

                if(!our_value.startsWith("/"))
                    our_value = our_value.prepend("/");

                our_value.replace(";/","<br>/");

                our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>/");

                data = QString("<br><b>") + our_column + ":</b>\t" + our_value + "\n";
            }

            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
                data = QString("<br><b>") + our_column + ":</b>\t<font color = \"blue\">" + our_value.replace("\n", "<br>") + "</font>\n";

            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
            {
                QString temp_2 = our_value;
                if(temp_2 != QString(""))
                {
                    QStringList temp;
                    if(our_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        temp = our_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                    }
                    else
                    {
                        temp = our_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                    }

                    if(temp.size() >= 1)
                    {
                        data = ("<br><b>" + our_column + ":</b>\n");

                        for(int i = 0 ; i< temp.size(); i++)
                        {
                            data.append("<br><b>" + QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(i), Q_FUNC_INFO) +"\n");

                        }
                    }
                }
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_QString)
            {
                data = "<b>" + our_column + ":</b>\t<font color = \"red\">" + our_value.toHtmlEscaped() +"</font>\n";
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
            {
                QString str = our_value;

                if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");
                }
                else
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");
                }

                str = str.replace("|","<br>");

                if(str.trimmed() == "")
                    data = "<b>" + our_column + ":</b>" + str;
                else
                {
                    data = "<b>" + our_column + ":</b><br>" + str +"\n";
                }

            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
            {
                QString str = our_value;
                QStringList timestamps_splitList;

                timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                {
                    if(tm_loop == 0)
                    {
                        data = QString("<br><b>") + our_column + ":</b>\t" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n";

                    }else{
                        data.append(QString("<br>\n") + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");
                    }

                }
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_METADATA_PREVIEW_QString)
            {
                // do not do any thing here
            }

            else
            {
                data = "<b>" + our_column + ":</b>\t" + our_value.toHtmlEscaped();

            }


            if(md_data_type_for_tmpuse.startsWith("NewLine"))
            {
                data_prepare_metadata += QString("<br>");
            }

            data_prepare_metadata += data + "<br>";
        }

    }


    destination_db.close();


    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    st_set_meta_obj.db_file_path = destination_db_file;
    st_set_meta_obj.detailed_info = data_prepare_metadata;
    st_set_meta_obj.source_count_name = source_count_name;
    st_set_meta_obj.record_no = record_no;
    st_set_meta_obj.display_searched_text = searched_keyword;
    st_set_meta_obj.plugin_name = get_plugin_name();
    st_set_meta_obj.tab_name = tab_name;


    emit signal_set_metadata_info(st_set_meta_obj);


    recon_static_functions::app_debug(" end ",Q_FUNC_INFO);
}

void plugin_viewer::slot_m_checkbox_toggled_textedit(bool checked)
{
    generic_checkbox_textedit(checked);

}

void plugin_viewer::generic_checkbox_textedit(bool checked)
{

    int index =  global_csv_reader_class_selected_obj->get_tab_index_as_per_tabwidget(get_plugin_name(),current_tab_text);

    if(index >= tab_name_list_textedit.size()){
        return;
    }


    QString bookmark_status = "0";
    if(checked)
    {
        bookmark_status = "1";
    }

    QString table_name = table_name_list_textedit.at(index);
    QStringList cols_nam_list =  column_name_list_textedit.at(index);
    QStringList cols_data_type_list =  data_type_list_textedit.at(index);

    QString txt_edt_col_nme;
    for(int j=0; j < cols_nam_list.size(); j++){
        if(cols_data_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString){
            txt_edt_col_nme = cols_nam_list.at(j);
        }
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.close();

}

void plugin_viewer::slot_m_checkbox_toggled_stack(bool checked)
{

    int index = global_csv_reader_class_selected_obj->get_tab_index_as_per_tabwidget(get_plugin_name(), current_tab_text);


    if(index >= tab_name_list_stack.size())
    {
        return;
    }

    QString bookmark_status = "0";
    if(checked)
    {
        bookmark_status = "1";
    }




    QString table_name = table_name_list_stack.at(index);
    QStringList cols_nam_list =  column_name_list_stack.at(index);
    QStringList cols_data_type_list =  data_type_list_stack.at(index);

    QString bk_col_nme;
    for(int j=0; j < cols_nam_list.size(); j++){
        if(cols_data_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString){
            bk_col_nme = cols_nam_list.at(j);
        }
    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("-------------------------FAILED----------------------- to open DB" + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("" + destination_db.lastError().text(),Q_FUNC_INFO);
        return;
    }

    destination_db.close();

}

QString plugin_viewer::get_exact_tab_name_from_ui(int tab_index)
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QString tab_name = ui->tabWidget->tabText(tab_index);
    QStringList tab_name_splitted_list = tab_name.split(MACRO_Plugin_tab_record_braces_open);
    tab_name = tab_name_splitted_list.at(0);
    tab_name = tab_name.trimmed();

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);
    return tab_name;

}
