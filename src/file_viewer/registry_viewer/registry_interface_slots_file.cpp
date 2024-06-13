#include "registry_interface.h"

#include "ui_registry_interface.h"

void registry_interface::on_pushButton_search_clicked()
{ // search any registry file on registry viewer table widget
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);


    emit signal_clear_all_metadata();
    draw_searched_items_in_table();


    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::on_tableWidget_searched_items_cellClicked(int row, int column)
{ // click on particular record for directly adding bookmark on it without right click menu
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    if(row < 0 || column < 0)
        return ;

    if(column != enum_table_widget_Bookmark_Index)
        return ;

    set_bookmark(row ,column);

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

void registry_interface::on_tableWidget_searched_items_itemClicked(QTableWidgetItem *item)
{ // set metadata of searched item clicked
    set_metadata_on_table_widget_click();
}

void registry_interface::on_tableWidget_registry_itemSelectionChanged()
{ // set metadata of that record which we select on tablewidget
    set_metadata_on_table_widget_click();
}

void registry_interface::on_tableWidget_searched_items_itemSelectionChanged()
{ // set metadata of that particluar record which we select after searching
    set_metadata_on_table_widget_click();
}

void registry_interface::on_pushButton_refresh_clicked()
{ // refresh button to refresh the records
    emit signal_clear_all_metadata();

    ui->lineEdit_searched_text->clear();
    ui->tableWidget_searched_items->setRowCount(0);

    pub_draw_registry_tree();

}

void registry_interface::on_tabWidget_tabBarClicked(int index)
{
    emit signal_clear_all_metadata();
}

void registry_interface::slot_registry_viewer_db_path_changed(QString dest_db_path)
{
    destination_db_path = dest_db_path;
}


void registry_interface::on_treeWidget_registry_itemClicked(QTreeWidgetItem *item, int column)
{ // registry item clicked on tree widget
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    emit signal_clear_all_metadata();
    set_metadata_on_treewidget_click(item);
    set_table_items(item);

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}


void registry_interface::on_tableWidget_registry_cellClicked(int row, int column)
{ // add bookmark on click directly

    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);


    if(row < 0 || column < 0)
        return ;


    if(column != enum_table_widget_Bookmark_Index)
        return ;

    set_bookmark(row ,column);

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}


void registry_interface::slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour)
{ // update tags on widget and in database
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);


    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}



void registry_interface::slot_registry_viewer_menu_right_clicked(QAction* current_clicked_action)
{ // right click actions as given below
    if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Bookmark))
    {
        action_bookmark_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Bookmarks))
    {
        action_remove_bookmarks_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Add_Note))
    {
        action_add_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Note))
    {
        action_remove_note_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Remove_Tag))
    {
        action_remove_tag_triggered();
    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Right_Click_Copy_To_Clipboard))
    {
        action_copy_to_clopboard_triggered();
    }
}


void registry_interface::slot_tags_submenu_clicked(QAction* current_clicked_action)
{
    action_submenu_tag_triggered(current_clicked_action->text());
}


void registry_interface::slot_act_create_tags()
{
    add_tags_module_obj->show();
}

// all the right click action functions of registry viewer window
void registry_interface::slot_add_tags(QString tag_data, QString colour)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;

    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    QStringList INT_list;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    emit signal_save_openwith_changes_to_bucket(MACRO_CASE_TREE_Registry_Viewer);

    QList<struct_global_tagging_registry_viewer> registry_viewer_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int row = model_index.row();

        if(row < 0 && row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
        QIcon icon(icon_path);
        QTableWidgetItem *tag_item = new QTableWidgetItem;
        tag_item->setIcon(icon);
        m_tablewidget_obj->setItem(row , enum_table_widget_Tag_Icon_Index ,tag_item);


        QTableWidgetItem *tag_text_item = new QTableWidgetItem;
        tag_text_item->setText(tag_data);
        m_tablewidget_obj->setItem(row , enum_table_widget_Tag_Text_Index ,tag_text_item);


        struct_global_tagging_registry_viewer s1;
        s1.INT_record = m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();
        INT_list << s1.INT_record;

        s1.plugin_name = QString(MACRO_Plugin_Name_Registry_Viewer);
        s1.tab_name = QString(MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer);
        s1.source_count_name = m_tablewidget_obj->item(row, enum_table_widget_source_count_name_Index)->text();

        s1.item0 = m_tablewidget_obj->item(row, enum_table_widget_file_path_Index)->text();
        s1.item1 = m_tablewidget_obj->item(row, enum_table_widget_node_path_Index)->text();
        s1.item2 = m_tablewidget_obj->item(row, enum_table_widget_node_name_Index)->text();
        s1.item3 = m_tablewidget_obj->item(row, enum_table_widget_Key_Index)->text();
        s1.item4 = m_tablewidget_obj->item(row, enum_table_widget_Type_Index)->text();
        s1.item5 = m_tablewidget_obj->item(row, enum_table_widget_Values_Index)->text();
        s1.item6 = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(m_tablewidget_obj->item(row, enum_table_widget_node_timestamp_Index)->text(), Q_FUNC_INFO);
        s1.item11 = display_tab_name;

        registry_viewer_record_list << s1;
    }

    global_connection_manager_obj->pub_set_tag_in_registry_viewer_db(INT_list , display_tab_name ,tag_data);
    global_connection_manager_obj->pub_add_tag_for_registry_viewer(registry_viewer_record_list,tag_data);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void registry_interface::slot_add_notes_pushbutton_clicked(QString note_text)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    add_notes_obj->hide();


    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    emit signal_save_openwith_changes_to_bucket(MACRO_CASE_TREE_Registry_Viewer);


    QList<struct_global_tagging_registry_viewer> registry_viewer_record_list;

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return ;


    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    QStringList INT_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {

        QModelIndex model_index = selection_model_list.at(i);


        int row = model_index.row();

        if(row < 0 && row >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(row))
            continue;


        QIcon iconNotes("../icons/notes.png");

        QTableWidgetItem *table_w_item_notes = new QTableWidgetItem;
        table_w_item_notes->setIcon(iconNotes);
        m_tablewidget_obj->setItem(row , enum_table_widget_Notes_Icon_Index ,table_w_item_notes);


        QTableWidgetItem *table_w_item_text = new QTableWidgetItem;
        table_w_item_text->setText(note_text);
        m_tablewidget_obj->setItem(row, enum_table_widget_note_Text_Index , table_w_item_text);


        struct_global_tagging_registry_viewer s1;
        s1.INT_record = m_tablewidget_obj->item(row, enum_table_widget_INT_Index)->text();
        INT_list << s1.INT_record;

        s1.plugin_name = QString(MACRO_Plugin_Name_Registry_Viewer);
        s1.tab_name = QString(MACRO_Plugin_Registry_Viewer_TAB_Name_Registry_Viewer);
        s1.source_count_name = m_tablewidget_obj->item(row, enum_table_widget_source_count_name_Index)->text();

        s1.item0 = m_tablewidget_obj->item(row, enum_table_widget_file_path_Index)->text();
        s1.item1 = m_tablewidget_obj->item(row, enum_table_widget_node_path_Index)->text();
        s1.item2 = m_tablewidget_obj->item(row, enum_table_widget_node_name_Index)->text();
        s1.item3 = m_tablewidget_obj->item(row, enum_table_widget_Key_Index)->text();
        s1.item4 = m_tablewidget_obj->item(row, enum_table_widget_Type_Index)->text();
        s1.item5 = m_tablewidget_obj->item(row, enum_table_widget_Values_Index)->text();
        s1.item6 = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(m_tablewidget_obj->item(row, enum_table_widget_node_timestamp_Index)->text(), Q_FUNC_INFO);
        s1.item11 = display_tab_name;

        registry_viewer_record_list << s1;

    }

    global_connection_manager_obj->pub_set_notes_in_registry_viewer_db(INT_list,display_tab_name ,note_text);
    global_connection_manager_obj->pub_add_notes_for_registry_viewer_db(registry_viewer_record_list ,note_text);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug(" -End " , Q_FUNC_INFO);
}

bool registry_interface::eventFilter(QObject *obj, QEvent *event)
{ // create right click event
    if(event == NULL)
        return false;

    if(obj == ui->tableWidget_searched_items)
    {
        QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
        if(selection_model_list.size() < 1)
        {
            emit signal_clear_all_metadata();
            return false;
        }
    }

    if(obj == ui->tableWidget_registry || obj == ui->tableWidget_searched_items)
    {
        QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
        if(m_tablewidget_obj == NULL)
            return false;

        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
        if(selection_model_list.size() < 1)
            return false;

        if(event->type() == QEvent::ContextMenu)
        {
            mainMenu->exec(QCursor::pos());
            return true;
        }
    }

    return false;
}

void registry_interface::slot_abouttoshow_tags_in_submenu()
{
    create_tags_submenu();
}

void registry_interface::slot_sort_tablewidget_display(int column_index)
{ // display and sort the tablewidget data records
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);


    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();
    if(m_tablewidget_obj == NULL)
        return;
    //Return if clicked column index other then Bookmark, Tags or notes
    if(column_index != enum_table_widget_Bookmark_Index && column_index != enum_table_widget_Notes_Icon_Index  && column_index != enum_table_widget_Tag_Icon_Index)
    {
        m_tablewidget_obj->sortByColumn(column_index,Qt::AscendingOrder);
        m_tablewidget_obj->setSortingEnabled(false);
        return;
    }

    m_tablewidget_obj->clearSelection();

    if(m_tablewidget_obj->rowCount() <= 1)
        return;


    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_tablewidget_obj->rowCount() - 1;

    if(bool_sort_ascending)
    {
        for(qint64 row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->item(row, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_table_widget_Bookmark_Index)
                icon_set =  m_tablewidget_obj->item(row, column_index)->icon();



            m_tablewidget_obj->item(row, column_index)->checkState();

            // Check for checkstate OR iCon
            if(m_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                if(column_index != enum_table_widget_Bookmark_Index)
                    icon_shiftrow_acs =  m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->icon();

                // If row is already on 0th position then increment and continue
                if(m_tablewidget_obj->item(row_shift_pos_ascending, column_index)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = takeRow(destRow);

                // Shifting of rows from lower to upper
                setRow(destRow, sourceItems);
                setRow(sourceRow, destItems);


                row_shift_pos_ascending++;
            }
        }

        bool_sort_ascending = false;
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(m_tablewidget_obj->item(row, column_index) == NULL)
                continue;

            if(m_tablewidget_obj->item(row_shift_pos_descending, column_index) == NULL)
                continue;

            QIcon icon_set;
            if(column_index != enum_table_widget_Bookmark_Index)
                icon_set = m_tablewidget_obj->item(row, column_index)->icon();

            if(m_tablewidget_obj->item(row, column_index)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                if(column_index != enum_table_widget_Bookmark_Index)
                    icon_shiftrow_desc =  m_tablewidget_obj->item(row_shift_pos_descending, column_index)->icon();

                // If row is already on last position then decrement and continue
                if(m_tablewidget_obj->item(row_shift_pos_descending, column_index)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                // take whole rows
                QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = takeRow(destRow);

                // Shifting of rows from upper to lower
                setRow(destRow, sourceItems);
                setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }

        bool_sort_ascending = true;
    }


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

QList<QTableWidgetItem*> registry_interface::takeRow(int row)
{
    QList<QTableWidgetItem*> rowItems;

    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();

    for(int col = 0; col < m_tablewidget_obj->columnCount(); ++col)
    {
        rowItems << m_tablewidget_obj->takeItem(row, col);
    }
    return rowItems;
}

// sets the whole row
void registry_interface::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
    QTableWidget *m_tablewidget_obj = get_m_tablewidget_obj_for_current_tab();

    for(int col = 0; col < m_tablewidget_obj->columnCount(); ++col)
    {
        m_tablewidget_obj->setItem(row, col, rowItems.at(col));
    }
}
