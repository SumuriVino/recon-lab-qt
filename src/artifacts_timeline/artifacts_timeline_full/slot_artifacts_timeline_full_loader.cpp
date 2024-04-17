#include "artifacts_timeline_full_loader.h"

void artifacts_timeline_full_loader::slot_line_edit_go_to_page_text_changed(QString text)
{
    QPalette palette;

    linedit_go_to_page->setPalette(palette);
    return;
}

void artifacts_timeline_full_loader::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading = cancel_status;
}


void artifacts_timeline_full_loader::slot_pushbutton_apply_clicked(bool is_clicked)
{
    recon_static_functions::app_debug("", Q_FUNC_INFO);

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    qint64 total_plugins_count = 0;
    for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
    {
        if(!treewidget_plugins_list->topLevelItem(tp)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
            continue;

        total_plugins_count++;
    }


    set_timeline();

    set_bookmarked_timestamp_type_list();

    map_bookmarked_plugin_with_tab_list.clear();

    qint64 plugins_covered_count = 0;

    map_of_pageno_and_timestamplabel.clear();
    map_of_struct_graph_data.clear();
    years_list.clear();
    month_year_list.clear();
    day_month_year_list.clear();
    hour_day_month_year_list.clear();



    for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
    {
        bookmarked_plugin_tab_list.clear();
        bookmarked_plugin_name.clear();

        if(!treewidget_plugins_list->topLevelItem(tp)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
            continue;

        QTreeWidgetItem *item = treewidget_plugins_list->topLevelItem(tp);

        QString plugin_name = item->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name);
        QStringList tab_list;
        for(int i = 0; i < item->childCount(); i++)
        {
            QTreeWidgetItem *item_tab = item->child(i);

            QStringList list;
            list << plugin_name << item_tab->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name);

            QString command1 = "update saved_events set bookmark = 0 where plugin = ? AND category = ?";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command1, list,destination_timeline_saved_db_path,Q_FUNC_INFO);

            if(!item_tab->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
                continue;

            tab_list << item_tab->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name);

            QString command = "update saved_events set bookmark = 1 where plugin = ? AND category = ?";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command, list,destination_timeline_saved_db_path,Q_FUNC_INFO);

        }

        bookmarked_plugin_tab_list = tab_list;
        bookmarked_plugin_name = plugin_name;

        plugins_covered_count++;


        map_bookmarked_plugin_with_tab_list.insert(plugin_name, tab_list);

    }

    set_graphs_essentials_for_plugins_data_only();


    QString fs_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();

    QStringList fs_db_path_list;
    //    for(int row = 0; row < tablewidget_filesystem_sources->rowCount(); row++)
    //    {
    //        QTableWidgetItem *item_source = tablewidget_filesystem_sources->item(row , 0);

    //        if(item_source->checkState() == Qt::Unchecked)
    //            continue;

    //        QTableWidgetItem *item_source_count_name = tablewidget_filesystem_sources->item(row , 1);

    //        QString fs_timeline_db_path = fs_result_dir_path  + "file_system_timeline_full_" + item_source_count_name->text() + ".sqlite";

    //        fs_db_path_list << fs_timeline_db_path;
    //    }

    if(!fs_db_path_list.isEmpty())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr(""),
                                   tr( QString(MACRO_Plugin_Name_File_System).toLocal8Bit() +  " timeline will take some time.\n"
                                                                                               "Do you want to proceed?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok)
        {
            set_graphs_essentials_for_filesystem_data_only(fs_db_path_list);
            map_bookmarked_plugin_with_tab_list.insert(QString(MACRO_Plugin_Name_File_System), QStringList(MACRO_Plugin_File_System_TAB_Name_Files));
        }
        else if (ret == QMessageBox::Cancel)
        {
            display_loading_progress_bar_non_cancelable_obj->hide();
            return;
        }
        else
        {
            display_loading_progress_bar_non_cancelable_obj->hide();
            return;
        }
    }



    if(map_bookmarked_plugin_with_tab_list.isEmpty() && fs_db_path_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();

        message_box_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please select atleast a plugin");
        message_box_obj->show();
        return;
    }

    if(bookmarked_timestamp_type_list.isEmpty() && fs_db_path_list.isEmpty())
    {
        display_loading_progress_bar_non_cancelable_obj->hide();

        message_box_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please select atleast a timestamp");
        message_box_obj->show();

        return;
    }

    int count_stacks = stackwidget_obj->count();
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
        }
    }


    if(m_tablewidget_obj != NULL)
        m_tablewidget_obj->setRowCount(0);


    create_graphs();
    stackwidget_index = 0;

    disable_enable_initial_ui(true);


    display_loading_progress_bar_non_cancelable_obj->hide();

    recon_static_functions::app_debug("", Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_m_treeWidget_plugins_itemClicked(QTreeWidgetItem *item, int column)
{

    treewidget_plugins_list->selectionModel()->clearSelection();

    item->setSelected(true);

    //-To Uncheck Parent if All Childs Get unchecked.

    if(item->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name) == 2)
    {
        //-To Check Toplevel Item if any Child get Checked.
        if(item->parent() != 0)
        {
            QString parent_name = item->parent()->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name);
            for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
            {
                if(treewidget_plugins_list->topLevelItem(tp)->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name) != parent_name)
                    continue;

                treewidget_plugins_list->topLevelItem(tp)->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Checked);
                break;
            }
        }
        else
        {
            for(int i = 0; i < item->childCount(); i++)
            {

                item->child(i)->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Checked);
            }

        }


    }
    else{

        if(item->parent() != 0)
        {
            QString parent_name = item->parent()->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name);
            for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
            {
                if(treewidget_plugins_list->topLevelItem(tp)->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name) != parent_name)
                    continue;

                int checked_tab_count = 0;
                for(int ch = 0; ch < treewidget_plugins_list->topLevelItem(tp)->childCount(); ch++)
                {
                    if(treewidget_plugins_list->topLevelItem(tp)->child(ch)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name) != Qt::Checked)
                        continue;

                    checked_tab_count++;
                }

                if(checked_tab_count == 0)
                {
                    treewidget_plugins_list->topLevelItem(tp)->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Unchecked);
                    break;
                }

            }

        }
        else // To uncheck if parent Get unchecked.
        {
            for(int i = 0; i < item->childCount(); i++)
            {
                //-To uncheck plugins Tabs.
                item->child(i)->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Unchecked);

            }
        }

    }

}

void artifacts_timeline_full_loader::slot_hovered_toolTip_on_barsets(bool status, int index, QBarSet *barset)
{
    QChart *chart = qobject_cast<QChart *>(barset->parent()->parent()->parent());

    m_barset *barset_m = dynamic_cast<m_barset *>(barset);

    if(status)
    {

        bool_check_barset = true;
        QMap<int, QString> map_insert_index_with_category = barset_m->get_tooltip();


        chart->setToolTip(map_insert_index_with_category.value(index));

    }
    else
    {
        bool_check_barset = false;
        chart->setToolTip("");
    }
}

void artifacts_timeline_full_loader::slot_pushbutton_previous_clicked(bool is_clicked)
{

    if(stackwidget_index <= 0)
    {
        pushbutton_previous->setEnabled(false);
        return;
    }
    linedit_go_to_page->clear();

    if(stackwidget_index == 1)
    {
        pushbutton_previous->setEnabled(false);
    }

    pushbutton_next->setEnabled(true);

    stackwidget_index--;
    stackwidget_obj->setCurrentIndex(stackwidget_index);

    //label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
    QMap<int, QString> current_graph_labels_qmaps;
    if(radiobutton_create_graph_yearly->isChecked())
    {
        current_graph_labels_qmaps =  map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_yearly);
    }
    else if(radiobutton_create_graph_monthly->isChecked())
    {
        current_graph_labels_qmaps = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_monthly);
    }
    else if(radiobutton_create_graph_daywise->isChecked())
    {
        current_graph_labels_qmaps = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_daywise);
    }
    else if(radiobutton_create_graph_hourly->isChecked())
    {
        current_graph_labels_qmaps  = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_hourly);
    }
    label_show_date_time->setText(current_graph_labels_qmaps.value(stackwidget_index));

    label_show_pages->setText("Page " + QString::number(stackwidget_index + 1) + " of " + QString::number(stackwidget_obj->count()));

}

void artifacts_timeline_full_loader::slot_pushbutton_export_graph_clicked()
{
    QString export_file_path,filename ;

    if(plugin_name == MACRO_Plugin_Name_Artifacts_Timeline)
        export_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Artifact_Timeline_In_Result_QString).toString();

    QChartView *tmp_chartview =  (QChartView *)stackwidget_obj->currentWidget();

    QString file_path;// = QFileDialog::getSaveFileName(this, tr("Export Graph"), export_file_path, tr("Image (*.png"));

    file_dialog_obj.setNameFilter(("*.png"));
    file_dialog_obj.setFileMode(QFileDialog::AnyFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();

        if(selected_files.size() > 0)
        {
            file_path = selected_files.at(0);

            if(!file_path.endsWith(".png"))
                file_path.append(".png");
        }
    }

    if(file_path.trimmed().isEmpty())
        return;


    QString dir_name_str = QFileInfo(file_path).absoluteDir().absolutePath();

    filename = recon_static_functions::get_available_filename(QFileInfo(file_path).fileName(),dir_name_str,Q_FUNC_INFO);

    if(dir_name_str.endsWith("/"))
        file_path = dir_name_str + filename;
    else
        file_path = dir_name_str + QString("/") + filename;



    const auto dpr =  tmp_chartview->devicePixelRatioF();

    QPixmap pmap(tmp_chartview->width() * dpr , tmp_chartview->height() * dpr);
    pmap.setDevicePixelRatio(dpr);

    QPainter *paint = new QPainter(&pmap);
    paint->setPen(*(new QColor(255,34,255,255)));
    tmp_chartview->render(paint);

    pmap.save(file_path , "PNG");
    delete paint;


    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("Graph has been exported. Do you want to open file?")));
    message_dialog_obj->show();
    recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
    int proc_counter = 0;
    while(1)
    {
        proc_counter++;
        if(proc_counter % 100 == 0)
        {
            proc_counter = 0;
            QCoreApplication::processEvents();
        }

        if(bool_message_box_clicked)
            break ;
    }
    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

    if(bool_message_box_yes_button_clicked)
    {
        QProcess myProcess ;
        myProcess.startDetached("open",QStringList(file_path));
    }

}

void artifacts_timeline_full_loader::slot_pushbutton_save_graph_clicked()
{
    save_graph_dialog_box_obj->pub_clear_graph_name();

    save_graph_dialog_box_obj->pub_set_graph_name(QString("Saved Graph PNG-") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")));
    save_graph_dialog_box_obj->show();

}

void artifacts_timeline_full_loader::slot_save_graph_name_value(QString graph_name)
{

    save_graph_dialog_box_obj->hide();

    graph_name = graph_name.trimmed();
    if(graph_name.contains(" "))
        graph_name.replace(" ","_");


    QString timeline_save_dir = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString();
    graph_name = recon_static_functions::get_available_filename(graph_name, timeline_save_dir, Q_FUNC_INFO);

    QString timeline_images_dir = timeline_save_dir + "Images/";
    QDir images_dir(timeline_images_dir);
    if(!images_dir.exists())
        images_dir.mkdir(timeline_images_dir);


    QString saved_filepath = timeline_images_dir + graph_name + ".png";

    QChartView *tmp_chartview =  (QChartView *)stackwidget_obj->widget(stackwidget_obj->currentIndex());

    if(!tmp_chartview)
        return;


    const auto dpr =  tmp_chartview->devicePixelRatioF();

    QPixmap pmap(tmp_chartview->width() * dpr , tmp_chartview->height() * dpr);
    pmap.setDevicePixelRatio(dpr);
    pmap.fill(Qt::transparent);

    QPainter *paint = new QPainter(&pmap);
    paint->setPen(*(new QColor(255,34,255,255)));
    tmp_chartview->render(paint);

    pmap.save(saved_filepath , "PNG");
    delete paint;


    QString duration_str = label_show_date_time->text().trimmed();

    QString saved_graphs_db_path = timeline_save_dir + "/artifacts_timeline_saved_graphs.sqlite";
    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    QDateTime date_created = QFileInfo(saved_filepath).birthTime();
    qint64 epch_time = date_created.toSecsSinceEpoch();


    QString relative_saved_file_str = saved_filepath;
    if(relative_saved_file_str.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString()))
        relative_saved_file_str.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString().size());

    QString recon_file_info_path = saved_filepath;
    if(recon_file_info_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString()))
        recon_file_info_path.remove(0,global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString().size());


    if(!recon_file_info_path.startsWith("/"))
        recon_file_info_path.prepend("/");

    QString command = QString("Insert into tags(bookmark,record_no,category,plugin_name,tab_name,item0,item1,item2,item3,recon_tag_value,recon_filefrom,recon_file_infopath) values(?,?,?,?,?,?,?, ?,?,?,?,?)");
    QStringList value_list;
    value_list << QString::number(0) << "" << QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph) << QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
               << QString(MACRO_Plugin_Artifacts_Timeline_Saved_Graph_TAB_Name_Artifacts_Timeline_Graph) << QFileInfo(saved_filepath).fileName() << relative_saved_file_str
               << QString::number(epch_time) << duration_str << QString(MACRO_Tag_Name_Saved_graphs) <<recon_file_from_result  << recon_file_info_path;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command , value_list , saved_graphs_db_path, Q_FUNC_INFO);

    QString update_cmd = "UPDATE tags set record_no = INT";
    recon_helper_standard_obj->execute_db_command_by_dbpath(update_cmd,saved_graphs_db_path,Q_FUNC_INFO);

    emit signal_add_saved_timeline_graph_in_case_tree();

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("Graph has been saved. Do you want to open it?")));
    message_dialog_obj->show();

    recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
    int proc_counter = 0;
    while(1)
    {
        proc_counter++;
        if(proc_counter % 100 == 0)
        {
            proc_counter = 0;
            QCoreApplication::processEvents();
        }

        if(bool_message_box_clicked)
            break ;

    }
    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

    if(bool_message_box_yes_button_clicked)
    {
        emit signal_display_after_save_results(MACRO_CASE_TREE_Artifacts_Timeline_Graph , graph_name);
    }

}

void artifacts_timeline_full_loader::slot_pushbutton_next_clicked(bool is_clicked)
{

    if(stackwidget_index == (stackwidget_obj->count() -1))
    {
        pushbutton_next->setEnabled(false);
        return;
    }

    linedit_go_to_page->clear();

    if(stackwidget_index == (stackwidget_obj->count() - 2))
    {
        pushbutton_next->setEnabled(false);
    }

    pushbutton_previous->setEnabled(true);

    stackwidget_index++;
    stackwidget_obj->setCurrentIndex(stackwidget_index);
    //label_show_date_time->setText(map_dates_labels.value(stackwidget_index));

    QMap<int, QString> current_graph_labels_qmaps;
    if(radiobutton_create_graph_yearly->isChecked())
    {
        current_graph_labels_qmaps =  map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_yearly);
    }
    else if(radiobutton_create_graph_monthly->isChecked())
    {
        current_graph_labels_qmaps = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_monthly);
    }
    else if(radiobutton_create_graph_daywise->isChecked())
    {
        current_graph_labels_qmaps = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_daywise);
    }
    else if(radiobutton_create_graph_hourly->isChecked())
    {
        current_graph_labels_qmaps  = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_hourly);
    }
    label_show_date_time->setText(current_graph_labels_qmaps.value(stackwidget_index));

    label_show_pages->setText("Page " + QString::number(stackwidget_index + 1) + " of " + QString::number(stackwidget_obj->count()));

}

QString artifacts_timeline_full_loader::get_command_for_open_in_tableview()
{

    m_tablewidget_obj->hideColumn(os_name_index);

    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")
    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select ");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }

    db_cmd.append(" Where (");

    for(int tab = 0; tab < bookmarked_timestamp_type_list.size(); tab++)
    {
        db_cmd.append(" stamp_type = ? OR ");
    }

    if(db_cmd.endsWith("OR "))
        db_cmd.chop(3);

    db_cmd.append(") AND (");


    QStringList tabs_list;
    QMap<QString, QStringList>::iterator iter;
    for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
    {
        tabs_list << iter.value();
        QString plugin_name = iter.key();
        db_cmd.append(" plugin = ? OR ");
    }
    if(db_cmd.endsWith("OR "))
        db_cmd.chop(3);

    db_cmd.append(")");

    db_cmd.append(" AND (");
    for(int tab = 0; tab < tabs_list.size(); tab++)
    {
        db_cmd.append(" category = ? OR ");
    }
    if(db_cmd.endsWith("OR "))
        db_cmd.chop(QString("OR ").size());

    db_cmd.append(")");


    db_cmd.append(" AND timestamp >= ? AND timestamp <= ? ORDER BY timestamp ASC");

    return db_cmd;
}


void artifacts_timeline_full_loader::slot_pushbutton_open_in_tableview_clicked()
{

    pushbutton_show_all->setHidden(true);

    pushButton_export_table_data->setHidden(false);

    pushButton_save->setHidden(false);

    pushButton_close_tab->setHidden(false);
    tabwidget_timeline_data_obj->tabBar()->show();


    current_barset_time_str = label_show_date_time->text();

    current_barset_time_str = current_barset_time_str.trimmed();
    if(current_barset_time_str.isEmpty())
        return;


    bookmark_column_index = 0;
    notes_column_index    = 1;
    tag_column_index      = 2;
    timestamp_index = 3;
    stamp_type_index = 4;
    record_no_index   = 5;
    plugin_name_index = 6;
    tab_name_index    = 7;
    os_name_index     = 8;
    item_1_index = 9;
    item_2_index = 10;

    source_count_name_index = 12;
    index_for_INT_only_for_artifact_timeline = 13;


    for(int i = 0; i < tabwidget_timeline_data_obj->count(); i++)
    {
        if(tabwidget_timeline_data_obj->tabText(i) == current_barset_time_str)
        {
            m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_timeline_data_obj->widget(i));
            tabwidget_timeline_data_obj->setCurrentIndex(tabwidget_timeline_data_obj->indexOf(m_tablewidget_obj));
            return;
        }
    }

    m_tablewidget_obj = new m_tablewidget(this);

    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    create_rightclick_options();
    m_tablewidget_obj->pub_set_id(1);




    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    //connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));

    tabwidget_timeline_data_obj->addTab(m_tablewidget_obj, current_barset_time_str);

    if(tabwidget_timeline_data_obj->count() > 1)
        tabwidget_timeline_data_obj->setCurrentIndex(tabwidget_timeline_data_obj->count() - 1);

    tab_saved_index = tabwidget_timeline_data_obj->currentIndex();

    new_tab_name = tabwidget_timeline_data_obj->tabText(tab_saved_index);

    set_graphs_data_tables_header();

    m_tablewidget_obj->hideColumn(source_count_name_index);
    m_tablewidget_obj->hideColumn(os_name_index);
    m_tablewidget_obj->hideColumn(index_for_INT_only_for_artifact_timeline);

    bookmarked_plugin_name.clear();
    bookmarked_plugin_tab_list.clear();

    bookmarked_plugin_name = current_barset_plugin;

    insert_data_in_table_on_show_in_tableview_clicked();

    struct_artifacts_timeline_table_view_info obj;
    obj.tab_str = current_barset_time_str;
    obj.timestamp_str = current_barset_time_str;
    if(radiobutton_create_graph_yearly->isChecked())
        obj.graph_type = "year_str";

    if(radiobutton_create_graph_monthly->isChecked())
        obj.graph_type = "month_year";

    if(radiobutton_create_graph_daywise->isChecked())
        obj.graph_type = "day_month_year";

    if(radiobutton_create_graph_hourly->isChecked())
        obj.graph_type = "hour_day_month_year";

    obj.is_open_in_tableview_clicked = true;
    struct_artifacts_timeline_table_view_info_QList.append(obj);

}

void artifacts_timeline_full_loader::insert_data_in_table_on_show_in_tableview_clicked()
{

    bool bool_any_plugin_checked = false;

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();

    for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
    {
        bookmarked_plugin_tab_list.clear();
        bookmarked_plugin_name.clear();

        if(treewidget_plugins_list->topLevelItem(tp)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
        {
            bool_any_plugin_checked = true;
            break;
        }
    }

    /// for plugin
    if(bool_any_plugin_checked)
    {
        QString timeline_db_path = result_dir_path + "artifacts_timeline_full.sqlite";


        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db =  QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(timeline_db_path);
        if(!destination_db.open())
        {
            recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            return;
        }

        QSqlQuery query_index(destination_db);

        QString value_str = current_barset_time_str;

        QString command_str = get_command_for_open_in_tableview();

        query_index.prepare(command_str);
        for(int j = 0; j < bookmarked_timestamp_type_list.size(); j++)
        {
            query_index.addBindValue(bookmarked_timestamp_type_list.at(j));
        }

        QStringList tabs_list;
        QMap<QString, QStringList>::iterator iter;
        for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
        {
            tabs_list << iter.value();
            QString plugin_name = iter.key();
            query_index.addBindValue(plugin_name);
        }

        for(int tab = 0; tab < tabs_list.size(); tab++)
        {
            query_index.addBindValue(tabs_list.at(tab));
        }

        if(radiobutton_create_graph_yearly->isChecked())
        {
            QStringList value_split_list;
            value_split_list =  value_str.split("-");

            if(value_split_list.size() > 1)
            {
                QString start_year = value_split_list.at(0);
                QString end_year   = QString("31.12.") + value_split_list.at(1);


                QDateTime date_time;
                date_time = QDateTime::fromString(start_year, "yyyy");
                date_time.setTimeSpec(Qt::UTC);
                qint64 start_epch = date_time.toSecsSinceEpoch();
                if(start_epch < start_timestamp_utc_qint64)
                    start_epch = start_timestamp_utc_qint64;

                date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
                date_time.setTimeSpec(Qt::UTC);
                qint64 end_epch  = date_time.toSecsSinceEpoch();
                if(end_epch > end_timestamp_utc_qint64)
                    end_epch = end_timestamp_utc_qint64;

                query_index.addBindValue(QString::number(start_epch));
                query_index.addBindValue(QString::number(end_epch));

            }
            else
            {
                QString start_year = value_str; // by default starts from Jan 01
                QString end_year   = QString("31.12.") + value_str;

                QDateTime date_time;
                date_time = QDateTime::fromString(start_year, "yyyy");
                date_time.setTimeSpec(Qt::UTC);
                qint64 start_epch = date_time.toSecsSinceEpoch();
                if(start_epch < start_timestamp_utc_qint64)
                    start_epch = start_timestamp_utc_qint64;

                date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
                date_time.setTimeSpec(Qt::UTC);
                qint64 end_epch  = date_time.toSecsSinceEpoch();
                if(end_epch > end_timestamp_utc_qint64)
                    end_epch = end_timestamp_utc_qint64;


                query_index.addBindValue(QString::number(start_epch));
                query_index.addBindValue(QString::number(end_epch));
            }
        }

        if(radiobutton_create_graph_monthly->isChecked())
        {

            QString start_year = value_str; // by default starts from Jan 01
            QString end_year   = QString("31.12.") + value_str;

            QDateTime date_time;
            date_time = QDateTime::fromString(start_year, "yyyy");
            date_time.setTimeSpec(Qt::UTC);
            qint64 start_epch = date_time.toSecsSinceEpoch();
            if(start_epch < start_timestamp_utc_qint64)
                start_epch = start_timestamp_utc_qint64;

            date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
            date_time.setTimeSpec(Qt::UTC);
            qint64 end_epch  = date_time.toSecsSinceEpoch();
            if(end_epch > end_timestamp_utc_qint64)
                end_epch = end_timestamp_utc_qint64;

            query_index.addBindValue(QString::number(start_epch));
            query_index.addBindValue(QString::number(end_epch));

        }

        if(radiobutton_create_graph_daywise->isChecked())
        {

            QString start_year = QString("01-") + value_str + QString(" 00:00:00");
            QString end_year   = QString("31-12-") + value_str + QString(" 23:59:59");

            QStringList year_split_list = value_str.split("-");
            QString year_str;
            if(year_split_list.size() > 1)
                year_str = year_split_list.at(1);

            bool is_leap_year = QDate::isLeapYear(year_str.toInt());

            if(value_str.startsWith("Feb-") && is_leap_year)
                end_year   = QString("29-") + value_str;
            else if(value_str.startsWith("Feb-") && !is_leap_year)
                end_year   = QString("28-") + value_str;
            else
            {
                if(value_str.startsWith("Jan-") || value_str.startsWith("Mar-") || value_str.startsWith("May-") || value_str.startsWith("Jul-") || value_str.startsWith("Aug-")
                        || value_str.startsWith("Oct-") || value_str.startsWith("Dec-"))
                {
                    end_year = QString("31-") + value_str + QString(" 23:59:59");
                }
                else
                {
                    end_year = QString("30-") + value_str + QString(" 23:59:59");
                }
            }


            QDateTime date_time;
            date_time = QDateTime::fromString(start_year, "dd-MMM-yyyy hh:mm:ss");
            date_time.setTimeSpec(Qt::UTC);
            qint64 start_epch = date_time.toSecsSinceEpoch();
            if(start_epch < start_timestamp_utc_qint64)
                start_epch = start_timestamp_utc_qint64;

            date_time = QDateTime::fromString(end_year, "dd-MMM-yyyy hh:mm:ss");
            date_time.setTimeSpec(Qt::UTC);
            qint64 end_epch  = date_time.toSecsSinceEpoch();
            if(end_epch > end_timestamp_utc_qint64)
                end_epch = end_timestamp_utc_qint64;

            query_index.addBindValue(QString::number(start_epch));
            query_index.addBindValue(QString::number(end_epch));


        }

        if(radiobutton_create_graph_hourly->isChecked())
        {
            QString start_year = value_str + QString(" 00:00:00");
            QString end_year   = value_str + QString(" 23:59:59");

            QDateTime date_time;
            date_time = QDateTime::fromString(start_year, "ddd dd-MMM-yyyy hh:mm:ss");
            date_time.setTimeSpec(Qt::UTC);
            qint64 start_epch = date_time.toSecsSinceEpoch();
            if(start_epch < start_timestamp_utc_qint64)
                start_epch = start_timestamp_utc_qint64;


            date_time = QDateTime::fromString(end_year, "ddd dd-MMM-yyyy hh:mm:ss");
            date_time.setTimeSpec(Qt::UTC);
            qint64 end_epch  = date_time.toSecsSinceEpoch();
            if(end_epch > end_timestamp_utc_qint64)
                end_epch = end_timestamp_utc_qint64;

            query_index.addBindValue(QString::number(start_epch));
            query_index.addBindValue(QString::number(end_epch));

        }

        bool_cancel_loading = false;
        display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_obj->show();
        populate_data_of_graph_in_tablewidget(query_index);
        display_loading_progress_bar_obj->hide();

        destination_db.close();
        QSqlDatabase::removeDatabase(connection_naam);

    }


    /// for file system
    //    for(int row = 0; row < tablewidget_filesystem_sources->rowCount(); row++)
    //    {
    //        QTableWidgetItem *item_source = tablewidget_filesystem_sources->item(row , 0);

    //        if(item_source->checkState() == Qt::Unchecked)
    //            continue;

    //        bookmarked_plugin_tab_list.clear();

    //        QTableWidgetItem *item_source_count_name = tablewidget_filesystem_sources->item(row , 1);

    //        QString fs_timeline_db_path = result_dir_path  + "file_system_timeline_full_" + item_source_count_name->text() + ".sqlite";

    //        QString connection_naam = QString(Q_FUNC_INFO);
    //        QSqlDatabase::removeDatabase(connection_naam);
    //        QSqlDatabase destination_db =  QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    //        destination_db.setDatabaseName(fs_timeline_db_path);
    //        if(!destination_db.open())
    //        {
    //            recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db.databaseName(),Q_FUNC_INFO);
    //            recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
    //            QSqlDatabase::removeDatabase(connection_naam);
    //            return;
    //        }

    //        QSqlQuery query_index(destination_db);

    //        QString value_str = current_barset_time_str;

    //        QString command_str = get_command_for_open_in_tableview();

    //        query_index.prepare(command_str);


    //        if(radiobutton_create_graph_yearly->isChecked())
    //        {
    //            QStringList value_split_list;
    //            value_split_list =  value_str.split("-");

    //            if(value_split_list.size() > 1)
    //            {
    //                QString start_year = value_split_list.at(0);
    //                QString end_year   = QString("31.12.") + value_split_list.at(1);


    //                QDateTime date_time;
    //                date_time = QDateTime::fromString(start_year, "yyyy");
    //                date_time.setTimeSpec(Qt::UTC);
    //                qint64 start_epoch = date_time.toSecsSinceEpoch();

    //                date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
    //                date_time.setTimeSpec(Qt::UTC);
    //                qint64 end_epoch  = date_time.toSecsSinceEpoch();

    //                query_index.addBindValue(QString::number(start_epoch));
    //                query_index.addBindValue(QString::number(end_epoch));

    //            }
    //            else
    //            {
    //                QString start_year = value_str; // by default starts from Jan 01
    //                QString end_year   = QString("31.12.") + value_str;

    //                QDateTime date_time;
    //                date_time = QDateTime::fromString(start_year, "yyyy");
    //                date_time.setTimeSpec(Qt::UTC);
    //                qint64 start_epoch = date_time.toSecsSinceEpoch();

    //                date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
    //                date_time.setTimeSpec(Qt::UTC);
    //                qint64 end_epoch  = date_time.toSecsSinceEpoch();


    //                query_index.addBindValue(QString::number(start_epoch));
    //                query_index.addBindValue(QString::number(end_epoch));
    //            }
    //        }

    //        if(radiobutton_create_graph_monthly->isChecked())
    //        {

    //            QString start_year = value_str; // by default starts from Jan 01
    //            QString end_year   = QString("31.12.") + value_str;

    //            QDateTime date_time;
    //            date_time = QDateTime::fromString(start_year, "yyyy");
    //            date_time.setTimeSpec(Qt::UTC);
    //            qint64 start_epoch = date_time.toSecsSinceEpoch();

    //            date_time = QDateTime::fromString(end_year, "dd.MM.yyyy");
    //            date_time.setTimeSpec(Qt::UTC);
    //            qint64 end_epoch  = date_time.toSecsSinceEpoch();

    //            query_index.addBindValue(QString::number(start_epoch));
    //            query_index.addBindValue(QString::number(end_epoch));

    //        }

    //        if(radiobutton_create_graph_daywise->isChecked())
    //        {

    //            QString start_year = QString("01-") + value_str + QString(" 00:00:00");
    //            QString end_year   = QString("31-12-") + value_str + QString(" 23:59:59");

    //            QStringList year_split_list = value_str.split("-");
    //            QString year_str;
    //            if(year_split_list.size() > 1)
    //                year_str = year_split_list.at(1);

    //            bool is_leap_year = QDate::isLeapYear(year_str.toInt());

    //            if(value_str.startsWith("Feb-") && is_leap_year)
    //                end_year   = QString("29-") + value_str;
    //            else if(value_str.startsWith("Feb-") && !is_leap_year)
    //                end_year   = QString("28-") + value_str;
    //            else
    //            {
    //                if(value_str.startsWith("Jan-") || value_str.startsWith("Mar-") || value_str.startsWith("May-") || value_str.startsWith("Jul-") || value_str.startsWith("Aug-")
    //                        || value_str.startsWith("Oct-") || value_str.startsWith("Dec-"))
    //                {
    //                    end_year = QString("31-") + value_str + QString(" 23:59:59");
    //                }
    //                else
    //                {
    //                    end_year = QString("30-") + value_str + QString(" 23:59:59");
    //                }
    //            }


    //            QDateTime date_time;
    //            date_time = QDateTime::fromString(start_year, "dd-MMM-yyyy hh:mm:ss");
    //            date_time.setTimeSpec(Qt::UTC);
    //            qint64 start_epoch = date_time.toSecsSinceEpoch();

    //            date_time = QDateTime::fromString(end_year, "dd-MMM-yyyy hh:mm:ss");
    //            date_time.setTimeSpec(Qt::UTC);
    //            qint64 end_epoch  = date_time.toSecsSinceEpoch();

    //            query_index.addBindValue(QString::number(start_epoch));
    //            query_index.addBindValue(QString::number(end_epoch));


    //        }

    //        if(radiobutton_create_graph_hourly->isChecked())
    //        {
    //            QString start_year = value_str + QString(" 00:00:00");
    //            QString end_year   = value_str + QString(" 23:59:59");

    //            QDateTime date_time;
    //            date_time = QDateTime::fromString(start_year, "ddd dd-MMM-yyyy hh:mm:ss");
    //            date_time.setTimeSpec(Qt::UTC);
    //            qint64 start_epoch = date_time.toSecsSinceEpoch();


    //            date_time = QDateTime::fromString(end_year, "ddd dd-MMM-yyyy hh:mm:ss");
    //            date_time.setTimeSpec(Qt::UTC);
    //            qint64 end_epoch  = date_time.toSecsSinceEpoch();

    //            query_index.addBindValue(QString::number(start_epoch));
    //            query_index.addBindValue(QString::number(end_epoch));

    //        }

    //        populate_data_of_graph_in_tablewidget(query_index);


    //        destination_db.close();
    //        QSqlDatabase::removeDatabase(connection_naam);

    //    }

}

void artifacts_timeline_full_loader::slot_pushbutton_go_to_page_clicked()
{
    QString goto_page_str = linedit_go_to_page->text();
    int page = goto_page_str.toInt();

    QPalette palette;

    if(page > stackwidget_obj->count() || page <= 0)
    {
        palette.setColor(QPalette::Text,Qt::red);
        linedit_go_to_page->setPalette(palette);
        return;

    }else
    {
        stackwidget_index = (page -1);
    }

    if((stackwidget_index == 0) && (stackwidget_obj->count() == 1))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(false);
    }
    if((stackwidget_index == 0) && (stackwidget_obj->count() > 1))
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index > 0) && (stackwidget_index == (stackwidget_obj->count()-1)))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(true);
    }
    else
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(true);
    }

    stackwidget_obj->setCurrentIndex(stackwidget_index);


    label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));

    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

}

void artifacts_timeline_full_loader::slot_radioButton_yearly_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;

    radiobutton_create_graph_monthly->setDisabled(true);
    radiobutton_create_graph_daywise->setDisabled(true);
    radiobutton_create_graph_hourly->setDisabled(true);

    label_show_date_time->clear();
    label_show_pages->clear();


    int count_stacks = stackwidget_obj->count();
    int start_remove_stack = 0;
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            QCoreApplication::processEvents();
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
            start_remove_stack++;

        }
    }


    map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_yearly);
    list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_yearly);
    for(int i = 0; i < list_of_charview.size();i++)
        stackwidget_obj->addWidget(list_of_charview.at(i));


    stackwidget_index = 0;

    stackwidget_obj->setCurrentIndex(stackwidget_index);

    if((stackwidget_index == 0) && (stackwidget_obj->count() == 1))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index == 0) && (stackwidget_obj->count() > 1))
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index > 0) && (stackwidget_index == (stackwidget_obj->count()-1)))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(true);
    }
    else
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(true);
    }


    label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));

    label_show_pages->setText("Page " + QString::number(1) + " of " + QString::number(stackwidget_obj->count()));
    stackwidget_index = 0;

    radiobutton_create_graph_monthly->setDisabled(false);
    radiobutton_create_graph_daywise->setDisabled(false);
    radiobutton_create_graph_hourly->setDisabled(false);


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_radioButton_monthly_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;

    radiobutton_create_graph_yearly->setDisabled(true);
    radiobutton_create_graph_daywise->setDisabled(true);
    radiobutton_create_graph_hourly->setDisabled(true);

    label_show_date_time->clear();
    label_show_pages->clear();


    int count_stacks = stackwidget_obj->count();
    int start_remove_stack = 0;
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            QCoreApplication::processEvents();
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
            start_remove_stack++;

        }
    }

    map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_monthly);
    list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_monthly);
    for(int i = 0; i < list_of_charview.size();i++)
        stackwidget_obj->addWidget(list_of_charview.at(i));


    stackwidget_index = 0;


    stackwidget_obj->setCurrentIndex(stackwidget_index);

    if((stackwidget_index == 0) && (stackwidget_obj->count() == 1))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index == 0) && (stackwidget_obj->count() > 1))
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index > 0) && (stackwidget_index == (stackwidget_obj->count()-1)))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(true);
    }
    else
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(true);
    }



    label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));

    /// if data is for single year (like this 2013 instead of 2011-2013)
    if(label_show_date_time->text().trimmed().isEmpty())
    {
        stackwidget_index = enum_artifacts_timeline_graphs_yearly;
        label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));
    }


    label_show_pages->setText("Page " + QString::number(1) + " of " + QString::number(stackwidget_obj->count()));
    stackwidget_index = 0;

    radiobutton_create_graph_yearly->setDisabled(false);
    radiobutton_create_graph_daywise->setDisabled(false);
    radiobutton_create_graph_hourly->setDisabled(false);


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_radioButton_daywise_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;

    radiobutton_create_graph_yearly->setDisabled(true);
    radiobutton_create_graph_monthly->setDisabled(true);
    radiobutton_create_graph_hourly->setDisabled(true);

    label_show_date_time->clear();
    label_show_pages->clear();


    int count_stacks = stackwidget_obj->count();
    int start_remove_stack = 0;

    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            QCoreApplication::processEvents();
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);

            start_remove_stack++;

        }
    }

    map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_daywise);
    list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_daywise);
    for(int i = 0; i < list_of_charview.size();i++)
        stackwidget_obj->addWidget(list_of_charview.at(i));

    stackwidget_index = 0;
    stackwidget_obj->setCurrentIndex(stackwidget_index);

    if((stackwidget_index == 0) && (stackwidget_obj->count() == 1))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index == 0) && (stackwidget_obj->count() > 1))
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(false);
    }
    else if((stackwidget_index > 0) && (stackwidget_index == (stackwidget_obj->count()-1)))
    {
        pushbutton_next->setEnabled(false);
        pushbutton_previous->setEnabled(true);
    }
    else
    {
        pushbutton_next->setEnabled(true);
        pushbutton_previous->setEnabled(true);
    }

    label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));

    label_show_pages->setText("Page " + QString::number(1) + " of " + QString::number(stackwidget_obj->count()));
    stackwidget_index = 0;


    radiobutton_create_graph_yearly->setDisabled(false);
    radiobutton_create_graph_monthly->setDisabled(false);
    radiobutton_create_graph_hourly->setDisabled(false);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_radioButton_hourly_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;


    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr(""),
                               tr( "Create graph hourly will take some time.\n"
                                   "Do you want to proceed?"),
                               QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok)
    {
        label_show_date_time->clear();
        label_show_pages->clear();


        radiobutton_create_graph_yearly->setDisabled(true);
        radiobutton_create_graph_monthly->setDisabled(true);
        radiobutton_create_graph_daywise->setDisabled(true);

        int count_stacks = stackwidget_obj->count();
        int start_remove_stack = 0;
        if(count_stacks > 0)
        {
            for(int i = (count_stacks-1); i >= 0; i--)
            {
                QCoreApplication::processEvents();
                stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
                delete stackwidget_obj->widget(i);
                start_remove_stack++;

            }
        }

        map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_hourly);
        if(map_of_pageno_and_timestamplabel.isEmpty())
        {
            list_of_charview.clear();
            stackwidget_index = 0;
            create_graph_hourly();

            map_of_chartview.insert(enum_artifacts_timeline_graphs_hourly,list_of_charview);
            map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.insert(enum_artifacts_timeline_graphs_hourly,map_of_pageno_and_timestamplabel);
            stackwidget_obj->setCurrentIndex(stackwidget_index);
        }else
        {

            map_of_pageno_and_timestamplabel = map_of_key_graphtype_value_mapsof_pageno_and_timestamplabel.value(enum_artifacts_timeline_graphs_hourly);
            list_of_charview = map_of_chartview.value(enum_artifacts_timeline_graphs_hourly);
            for(int i = 0; i < list_of_charview.size();i++)
                stackwidget_obj->addWidget(list_of_charview.at(i));

            stackwidget_index = 0;
            stackwidget_obj->setCurrentIndex(stackwidget_index);

            label_show_date_time->setText(map_of_pageno_and_timestamplabel.value(stackwidget_index));

            label_show_pages->setText("Page " + QString::number(1) + " of " + QString::number(stackwidget_obj->count()));
            stackwidget_index = 0;

        }

        radiobutton_create_graph_yearly->setDisabled(false);
        radiobutton_create_graph_monthly->setDisabled(false);
        radiobutton_create_graph_daywise->setDisabled(false);

        if((stackwidget_index == 0) && (stackwidget_obj->count() == 1))
        {
            pushbutton_next->setEnabled(false);
            pushbutton_previous->setEnabled(false);
        }
        else if((stackwidget_index == 0) && (stackwidget_obj->count() > 1))
        {
            pushbutton_next->setEnabled(true);
            pushbutton_previous->setEnabled(false);
        }
        else if((stackwidget_index > 0) && (stackwidget_index == (stackwidget_obj->count()-1)))
        {
            pushbutton_next->setEnabled(false);
            pushbutton_previous->setEnabled(true);
        }
        else
        {
            pushbutton_next->setEnabled(true);
            pushbutton_previous->setEnabled(true);
        }


    }
    else if (ret == QMessageBox::Cancel)
    {

    }
    else
    {

    }

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_tabbar_clicked(int index)
{

    if(index < 0)
        return;


    if(index == 0)
    {
        pushButton_save->setHidden(true);
        pushButton_close_tab->setHidden(true);
        pushbutton_show_all->setHidden(true);
        pushButton_export_table_data->setHidden(true);
        return;
    }

    m_tablewidget_obj = dynamic_cast<m_tablewidget*> (tabwidget_timeline_data_obj->widget(index));
    int tab_id = m_tablewidget_obj->pub_get_id();
    if(tab_id == 1)
    {

        pushButton_export_table_data->setHidden(false);
        pushButton_save->setHidden(false);

        pushButton_close_tab->setHidden(false);
        //pushbutton_show_all->setHidden(false);
    }
    else
    {
        pushButton_save->setHidden(false);

        pushButton_export_table_data->setHidden(false);
        pushButton_close_tab->setHidden(false);

        pushbutton_show_all->setHidden(true);

    }
}

void artifacts_timeline_full_loader::slot_sources_tabbar_clicked(int index)
{
    if(index < 0)
        return;

    //    if(index == 0)
    //    {
    //        ///filesytem
    //        for(int row = 0; row < tablewidget_timestamp_list->rowCount(); row++)
    //        {
    //            QTableWidgetItem *item = tablewidget_timestamp_list->item(row , 2); ///feature column

    //            if(item->text() != QString(MACRO_Plugin_Name_File_System))
    //                tablewidget_timestamp_list->hideRow(row);
    //            else
    //                tablewidget_timestamp_list->showRow(row);

    //        }


    //    }
    //    else if(index == 1)
    //    {
    //        ///plugin
    //        for(int row = 0; row < tablewidget_timestamp_list->rowCount(); row++)
    //        {
    //            QTableWidgetItem *item = tablewidget_timestamp_list->item(row , 2); ///feature column

    //            if(item->text() == QString(MACRO_Plugin_Name_File_System))
    //                tablewidget_timestamp_list->hideRow(row);
    //            else
    //                tablewidget_timestamp_list->showRow(row);
    //        }
    //    }
    //    else
    //    {

    //    }


}


void artifacts_timeline_full_loader::slot_lineEdit_plugin_search_textChanged(const QString &arg1)
{
    treewidget_plugins_list->collapseAll();

    QString text = arg1;
    for(int i = 0; i < treewidget_plugins_list->topLevelItemCount(); i++)
    {
        if(treewidget_plugins_list->topLevelItem(i)->text(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name).toLower().contains(text.toLower()))
            treewidget_plugins_list->topLevelItem(i)->setHidden(false);
        else
            treewidget_plugins_list->topLevelItem(i)->setHidden(true);
    }

}

void artifacts_timeline_full_loader::slot_lineEdit_timestamp_search_textChanged(const QString &arg1)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    for(int i = 0; i < tablewidget_timestamp_list->rowCount(); i++)
    {
        if(i % 500 == 0)
        {
            QCoreApplication::processEvents();
        }

        tablewidget_timestamp_list->showRow(i);

        QString tmp_str = tablewidget_timestamp_list->item(i,enum_artifacts_timeline_tablewidget_timestamps_list_stamp_type)->text().trimmed();

        if(!tmp_str.toLower().contains(arg1))
            tablewidget_timestamp_list->hideRow(i);


        tmp_str = tablewidget_timestamp_list->item(i,enum_artifacts_timeline_tablewidget_timestamps_list_stamp_name)->text().trimmed();

        if(!tmp_str.toLower().contains(arg1))
            tablewidget_timestamp_list->hideRow(i);

    }



    recon_static_functions::app_debug("---Ends----",Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_fs_sources_horizontal_header_clicked(int column)
{
    if(column == 0)
    {
        if(tablewidget_filesystem_sources->horizontalHeaderItem(0)->checkState())
        {
            tablewidget_filesystem_sources->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
            tablewidget_filesystem_sources->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

            for(int row = 0 ; row < tablewidget_filesystem_sources->rowCount(); row++)
            {
                if(tablewidget_filesystem_sources->isRowHidden(row))
                    continue;

                tablewidget_filesystem_sources->item(row,0)->setCheckState(Qt::Unchecked);
            }
        }
        else
        {
            tablewidget_filesystem_sources->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
            tablewidget_filesystem_sources->horizontalHeaderItem(0)->setCheckState(Qt::Checked);

            for(int row = 0 ; row < tablewidget_filesystem_sources->rowCount(); row++)
            {
                if(tablewidget_filesystem_sources->isRowHidden(row))
                    continue;

                tablewidget_filesystem_sources->item(row,0)->setCheckState(Qt::Checked);
            }
        }
    }
}

void artifacts_timeline_full_loader::slot_refresh_table_data_on_show_all()
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);


    m_tablewidget_obj->setRowCount(0);



    int table_id = m_tablewidget_obj->pub_get_id();
    if(table_id == 1)
    {
        insert_data_in_table_on_show_in_tableview_clicked();
    }
    else
    {
        display_data(destination_timeline_db_path);
    }



    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_timestamps_horizontal_header_clicked(int column)
{

    if(column == 0)
    {
        if(tablewidget_timestamp_list->horizontalHeaderItem(0)->checkState())
        {

            tablewidget_timestamp_list->horizontalHeaderItem(0)->setIcon(QIcon("../icons/unchecked.png"));
            tablewidget_timestamp_list->horizontalHeaderItem(0)->setCheckState(Qt::Unchecked);

            for(int row = 0 ; row < tablewidget_timestamp_list->rowCount(); row++)
            {

                if(tablewidget_timestamp_list->isRowHidden(row))
                    continue;

                if(tablewidget_timestamp_list->item(row,0)->checkState() == Qt::Unchecked){
                    continue;
                }


                tablewidget_timestamp_list->item(row,0)->setCheckState(Qt::Unchecked);

                emit tablewidget_timestamp_list->cellClicked(row,0);
            }

        }
        else
        {


            tablewidget_timestamp_list->horizontalHeaderItem(0)->setIcon(QIcon("../icons/checked.png"));
            tablewidget_timestamp_list->horizontalHeaderItem(0)->setCheckState(Qt::Checked);



            for(int row = 0 ; row < tablewidget_timestamp_list->rowCount(); row++)
            {
                if(tablewidget_timestamp_list->isRowHidden(row))
                    continue;

                tablewidget_timestamp_list->item(row,0)->setCheckState(Qt::Checked);

                emit tablewidget_timestamp_list->cellClicked(row,0);
            }

        }
    }
}

void artifacts_timeline_full_loader::slot_checkBox_plugin_clicked(bool value)
{

    if(value)
    {
        for(int i = 0 ; i < treewidget_plugins_list->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *item = treewidget_plugins_list->topLevelItem(i);


            if(item->isHidden())
                continue;

            item->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Checked);

            for(int i = 0; i < item->childCount(); i++)
            {

                item->child(i)->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Checked);
            }

        }
    }
    else
    {
        for(int i = 0 ; i < treewidget_plugins_list->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *item = treewidget_plugins_list->topLevelItem(i);

            if(item->isHidden())
                continue;

            item->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Unchecked);

            for(int i = 0; i < item->childCount(); i++)
            {

                item->child(i)->setCheckState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name,Qt::Unchecked);
            }

        }
    }


}

void artifacts_timeline_full_loader::slot_barset_doubleClicked(int index)
{
    m_barset* barset = dynamic_cast<m_barset*> (sender());

    QMap<int,QString>map_barset = barset->get_tooltip();

    QString str = map_barset.value(index);

    current_barset_time_str.clear();
    current_barset_plugin.clear();

    QStringList list = str.split("\n", Qt::SkipEmptyParts);
    QString yr_str;
    if(list.size() > 2)
    {
        QString plugin_name = list.at(2);

        plugin_name.remove(0,plugin_name.indexOf("-")+1);
        plugin_name = plugin_name.trimmed();

        current_barset_plugin = plugin_name;

        yr_str = list.at(1);
        yr_str.remove(0,yr_str.indexOf("-")+1);
        yr_str = yr_str.trimmed();

        current_barset_time_str = yr_str;
    }


    slot_act_show_data_of_current_plugin_triggered();

}

void artifacts_timeline_full_loader::slot_barset_pressed(int index)
{

    m_barset* barset = dynamic_cast<m_barset*> (sender());

    QMap<int,QString>map_barset = barset->get_tooltip();

    QString str = map_barset.value(index);

    current_barset_time_str.clear();
    current_barset_plugin.clear();

    QStringList list = str.split("\n", Qt::SkipEmptyParts);
    QString yr_str;
    if(list.size() > 2)
    {
        QString plugin_name = list.at(2);

        plugin_name.remove(0,plugin_name.indexOf("-")+1);
        plugin_name = plugin_name.trimmed();

        current_barset_plugin = plugin_name;

        yr_str = list.at(1);
        yr_str.remove(0,yr_str.indexOf("-")+1);
        yr_str = yr_str.trimmed();

        current_barset_time_str = yr_str;
    }

}

void artifacts_timeline_full_loader::slot_act_show_data_of_current_plugin_triggered()
{
    is_clicked_to_save_all_data = false;

    if(current_barset_time_str.trimmed().isEmpty())
        return;

    tabwidget_timeline_data_obj->tabBar()->show();

    tab_saved_index = tabwidget_timeline_data_obj->currentIndex();

    QString tab_name = QString(current_barset_plugin) + "-" + QString(current_barset_time_str);

    new_tab_name = tab_name;

    for(int i = 0; i < tabwidget_timeline_data_obj->count(); i++)
    {
        if(tabwidget_timeline_data_obj->tabText(i) == tab_name)
        {
            m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_timeline_data_obj->widget(i));
            tabwidget_timeline_data_obj->setCurrentIndex(tabwidget_timeline_data_obj->indexOf(m_tablewidget_obj));
            return;
        }
    }


    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    create_rightclick_options();
    tabwidget_timeline_data_obj->addTab(m_tablewidget_obj, tab_name);

    pushButton_save->setHidden(false);
    pushButton_close_tab->setHidden(false);

    m_tablewidget_obj->pub_set_id(1);
    pushButton_export_table_data->setHidden(false);

    set_graphs_data_tables_header();

    bookmarked_plugin_name.clear();
    bookmarked_plugin_tab_list.clear();

    bookmarked_plugin_name = current_barset_plugin;

    bookmarked_plugin_tab_list = map_bookmarked_plugin_with_tab_list.value(current_barset_plugin);

    tabwidget_timeline_data_obj->setCurrentIndex(tabwidget_timeline_data_obj->indexOf(m_tablewidget_obj));

    tabwidget_timeline_data_obj->tabBar()->show();

    QString fs_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
    //    for(int row = 0; row < tablewidget_filesystem_sources->rowCount(); row++)
    //    {
    //        QTableWidgetItem *item_source = tablewidget_filesystem_sources->item(row , 0);

    //        if(item_source->checkState() == Qt::Unchecked)
    //            continue;

    //        QTableWidgetItem *item_source_count_name = tablewidget_filesystem_sources->item(row , 1);

    //        QString fs_timeline_db_path = fs_result_dir_path  + "file_system_timeline_full_" + item_source_count_name->text() + ".sqlite";

    //        display_data_of_graph_tablewidget(fs_timeline_db_path);

    //    }

    bool any_plugin_checked = false;
    for(int tp = 0; tp < treewidget_plugins_list->topLevelItemCount(); tp++)
    {
        if(treewidget_plugins_list->topLevelItem(tp)->checkState(enum_artifacts_timeline_treewidget_column_name_plugin_or_tab_name))
        {
            any_plugin_checked = true;
            break;
        }
    }

    if(any_plugin_checked)
    {
        bool_cancel_loading = false;
        display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
        display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
        display_loading_progress_bar_obj->show();
        QString plugin_timeline_db_path = fs_result_dir_path + "artifacts_timeline_full.sqlite";
        display_data_of_graph_tablewidget(plugin_timeline_db_path);
        display_loading_progress_bar_obj->hide();

    }

    struct_artifacts_timeline_table_view_info obj;
    obj.tab_str = tab_name;
    obj.timestamp_str = current_barset_time_str;

    if(radiobutton_create_graph_yearly->isChecked())
        obj.graph_type = "year_str";

    if(radiobutton_create_graph_monthly->isChecked())
        obj.graph_type = "month_year";

    if(radiobutton_create_graph_daywise->isChecked())
        obj.graph_type = "day_month_year";

    if(radiobutton_create_graph_hourly->isChecked())
        obj.graph_type = "hour_day_month_year";

    obj.is_open_in_tableview_clicked = false;
    struct_artifacts_timeline_table_view_info_QList.append(obj);

    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    //connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));

}

void artifacts_timeline_full_loader::slot_act_show_all_data_triggered()
{
    is_clicked_to_save_all_data  = true;

    if(current_barset_time_str.trimmed().isEmpty())
        return;

    tabwidget_timeline_data_obj->tabBar()->show();

    tab_saved_index = tabwidget_timeline_data_obj->currentIndex();

    new_tab_name = tabwidget_timeline_data_obj->tabText(tab_saved_index);

    for(int i = 0; i < tabwidget_timeline_data_obj->count(); i++)
    {
        if(tabwidget_timeline_data_obj->tabText(i) == current_barset_time_str)
        {
            m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_timeline_data_obj->widget(i));
            tabwidget_timeline_data_obj->setCurrentIndex(tabwidget_timeline_data_obj->indexOf(m_tablewidget_obj));
            return;
        }
    }



    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    create_rightclick_options();
    tabwidget_timeline_data_obj->addTab(m_tablewidget_obj, current_barset_time_str);


    pushButton_save->setHidden(false);
    pushButton_close_tab->setHidden(false);

    pushButton_export_table_data->setHidden(false);

    set_graphs_data_tables_header();

    bookmarked_plugin_name.clear();
    bookmarked_plugin_tab_list.clear();

    bookmarked_plugin_name = current_barset_plugin;

    QMap<QString, QStringList>::iterator iter;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QStringList plugins_list;
    for(iter = map_bookmarked_plugin_with_tab_list.begin() ; iter != map_bookmarked_plugin_with_tab_list.end(); iter++)
    {
        bookmarked_plugin_name = iter.key();
        bookmarked_plugin_tab_list = iter.value();
        plugins_list << bookmarked_plugin_name;

        QString tmp_plugin_name = bookmarked_plugin_name.toLower();
        tmp_plugin_name.replace(" ", "_");
        if(QFileInfo(destination_timeline_db_path).fileName().contains(tmp_plugin_name))
            display_data_of_graph_tablewidget(destination_timeline_db_path);
        else
        {
            QString fs_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Full_In_Result_QString).toString();
            QString timeline_db_path = fs_result_dir_path + "artifacts_timeline_full.sqlite";
            display_data_of_graph_tablewidget(timeline_db_path);
        }
    }

    display_loading_progress_bar_obj->hide();

    tabwidget_timeline_data_obj->setCurrentIndex(tabwidget_timeline_data_obj->indexOf(m_tablewidget_obj));
    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    //connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));


}

void artifacts_timeline_full_loader::slot_pushbutton_close_tab_clicked(bool value)
{
    if(tabwidget_timeline_data_obj->currentIndex() <= 0)
        return;

    QString m_current_tab_name = tabwidget_timeline_data_obj->tabText(tabwidget_timeline_data_obj->currentIndex());

    if(tab_custom_header_widgets_QMap.contains(m_current_tab_name))
        tab_custom_header_widgets_QMap.remove(m_current_tab_name);

    tabwidget_timeline_data_obj->removeTab(tabwidget_timeline_data_obj->currentIndex());
    slot_tabbar_clicked(tabwidget_timeline_data_obj->currentIndex());

}

void artifacts_timeline_full_loader::slot_pushbutton_save_clicked(bool value)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    int current_tab_index = tabwidget_timeline_data_obj->currentIndex();
    QString current_tab_name = tabwidget_timeline_data_obj->tabText(current_tab_index);

    for(int ii = 0; ii < struct_artifacts_timeline_table_view_info_QList.size(); ii++)
    {
        struct_artifacts_timeline_table_view_info obj = struct_artifacts_timeline_table_view_info_QList.at(ii);
        if(current_tab_name == obj.tab_str)
        {
            index_struct_artifacts_timeline_table_view_info = ii;
            if(obj.is_open_in_tableview_clicked)
            {
                is_clicked_to_save_all_data = true;
            }
            break;
        }
    }

    save_result_dialog_box_obj->pub_set_name(current_tab_name + "-" + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")));
    save_result_dialog_box_obj->show();
    recon_static_functions::app_debug("-Ends- " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_pushbutton_export_table_data_clicked(bool value)
{
    QString filename = tabwidget_timeline_data_obj->tabText(tabwidget_timeline_data_obj->currentIndex());

    QString lab_export_dir_path = getenv("HOME") + QString("/Desktop") + "/LAB_Export";
    export_dialog_box_2_obj->pub_set_exported_file_name_and_dir_path(filename,lab_export_dir_path);

    export_dialog_box_2_obj->show();
}

void artifacts_timeline_full_loader::slot_table_data_export_dialog_ok_clicked(QString given_name , QString selected_dir_path, QString export_type_str , QString export_scope)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);

    export_dialog_box_2_obj->hide();


    report_scope_str = export_scope;

    given_name = given_name.trimmed();
    selected_dir_path = selected_dir_path.trimmed();

    if(given_name.isEmpty() || selected_dir_path.isEmpty())
        return;

    if(!selected_dir_path.endsWith("/"))
        selected_dir_path.append("/");

    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    QString filepath;
    if(export_type_str == QString(MACRO_Generic_Export_As_CSV))
    {
        filepath = selected_dir_path + given_name + ".csv";

        QString available_filename = recon_static_functions::get_available_filename(QFileInfo(filepath).fileName() , selected_dir_path, Q_FUNC_INFO);
        filepath = selected_dir_path + available_filename;

        export_timeline_data_as_csv(filepath);
    }
    else if(export_type_str == QString(MACRO_Generic_Export_As_SQLite))
    {
        filepath = selected_dir_path + given_name + ".sqlite";

        QString available_filename = recon_static_functions::get_available_filename(QFileInfo(filepath).fileName() , selected_dir_path, Q_FUNC_INFO);
        filepath = selected_dir_path + available_filename;

        export_timeline_data_as_sqlite(filepath);
    }


    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);

}

void artifacts_timeline_full_loader::open_exported_file(QString file_path)
{
    report_status obj;
    if(obj.exec())
    {
        QStringList arg;
        arg << file_path;

        QProcess *myProcess = new QProcess(this);
        myProcess->startDetached("open",arg);
    }
}


void artifacts_timeline_full_loader::slot_save_result_name_value(QString save_name)
{

    save_result_dialog_box_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    save_name.replace(" ", "_");
    QString new_name = recon_static_functions::get_available_filename(save_name + ".sqlite" ,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() , Q_FUNC_INFO );
    new_name.remove(".sqlite");
    save_timeline_data_in_case_treewidget(new_name);

    display_loading_progress_bar_non_cancelable_obj->hide();

    bool_message_box_yes_button_clicked = false;
    bool_message_box_clicked = false;
    message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("Result has been saved. Do you want to open it?")));
    message_dialog_obj->show();

    recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
    int proc_counter = 0;
    while(1)
    {
        proc_counter++;
        if(proc_counter % 100 == 0)
        {
            proc_counter = 0;
            QCoreApplication::processEvents();
        }

        if(bool_message_box_clicked)
            break;
    }
    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

    if(bool_message_box_yes_button_clicked)
    {
        emit signal_display_after_save_results(MACRO_CASE_TREE_Artifacts_Timeline , new_name);
    }

}


QString artifacts_timeline_full_loader::create_db_for_saved_artifacts(QString db_name)
{
    QString  saved_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_In_Result_QString).toString() + db_name;
    if(QFile::exists(saved_path))
    {
        return QString("");
    }

    QString command = "create table timeline_data (INT INTEGER,plugin varchar(30), category varchar(30), table_name varchar(30), "
                      "record_no INTEGER, stamp_name varchar(30), stamp_type varchar(30),timestamp INTEGER,item1 varchar(300), item2 varchar(300),os_scheme_display varchar(300),"
                      "recon_tag_value varchar(30), notes varchar(100),notes_icon varchar(30), result_name varchar(30),blank_column varchar(100),bookmark, loaded varchar(10), year_str varchar(100),"
                      "month_year varchar(100), day_month_year varchar(100), hour_day_month_year varchar(100) ,source_file varchar(100),source_count_name varchar(100))";

    recon_helper_standard_obj->execute_db_command_by_dbpath(command,saved_path,Q_FUNC_INFO);
    return saved_path;
}

QString artifacts_timeline_full_loader::prepare_command(QStringList tab_list)
{

    /*Select INT,bookmark,notes_icon,recon_tag_value,timestamp,stamp_type,record_no,plugin,
 * category,os_scheme_display,item1,item2,notes,source_count_name,INT From 'timeline_data'
 * where plugin = ? AND ( category = ? OR  category = ? OR  category = ? OR  category = ? OR  category = ? )
 * AND ( stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR
 * stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR
 * stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? OR  stamp_type = ? ) AND year_str = ?  AND
 * timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC */



    /// prepairing Query from CSV Parsed data (Select "tabs" from "table")

    QString db_cmd;

    if(!db_columns_list.isEmpty())
        db_cmd = QString("Select source_file ,");

    for(int i = 0; i < db_columns_list.size(); i++)
    {
        db_cmd.append(db_columns_list.at(i));
        db_cmd.append(QString(","));
    }


    if(db_cmd.endsWith(QString(",")))
        db_cmd.chop(1);

    if(!table_name_list.isEmpty())
    {
        db_cmd.append(QString(" From '"));
        db_cmd.append(table_name_list.at(0) + "'");
    }

    db_cmd.append("where plugin = ? AND (");

    for(int tab = 0; tab < tab_list.size(); tab++)
    {
        db_cmd.append(" category = ? OR ");
    }


    if(db_cmd.endsWith("OR "))
    {
        db_cmd.chop(3);
        db_cmd.append(") AND (");
    }

    for(int tab = 0; tab < bookmarked_timestamp_type_list.size(); tab++)
    {
        db_cmd.append(" stamp_type = ? OR ");
    }

    if(db_cmd.endsWith("OR "))
    {
        db_cmd.chop(3);
        db_cmd.append(")");
    }

    QString command_type ;
    struct_artifacts_timeline_table_view_info obj = struct_artifacts_timeline_table_view_info_QList.at(index_struct_artifacts_timeline_table_view_info);
    if(is_clicked_to_save_all_data)
    {
        if(obj.graph_type == "year_str")
        {
            command_type = " AND " +  obj.graph_type + " >= ?" + " AND " +  obj.graph_type + " <= ?" ;
        }
        else
        {
            command_type = " AND " +  obj.graph_type + " LIKE ? ";
        }
    }
    else
    {
        command_type = " AND " +  obj.graph_type + " = ? ";
    }

    db_cmd.append(command_type) ;


    QString command_time = " AND timestamp  >= ? AND timestamp <= ? ORDER BY timestamp ASC";
    db_cmd.append(command_time);

    return db_cmd;
}

void artifacts_timeline_full_loader::extract_and_save_result(QString plugin_str,QStringList tabs_list, QString result_name)
{

    ///artifatcs timeline db used here as a source
    QSqlDatabase source_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    source_db.setDatabaseName(destination_timeline_db_path);
    if(!source_db.open())
    {
        recon_static_functions::app_debug("destination db open ---FAILED--- " + source_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + source_db.lastError().text(), Q_FUNC_INFO);
        return;
    }



    QString select_command = prepare_command(tabs_list);
    QSqlQuery query_index(source_db);
    query_index.prepare(select_command);

    query_index.addBindValue(plugin_str);

    for(int tab = 0; tab < tabs_list.size(); tab++)
    {
        query_index.addBindValue(tabs_list.at(tab));
    }

    for(int j = 0; j < bookmarked_timestamp_type_list.size(); j++)
    {
        query_index.addBindValue(bookmarked_timestamp_type_list.at(j));
    }

    struct_artifacts_timeline_table_view_info obj = struct_artifacts_timeline_table_view_info_QList.at(index_struct_artifacts_timeline_table_view_info);
    if(is_clicked_to_save_all_data)
    {
        if(obj.graph_type == "year_str")
        {
            QString temp_str = obj.timestamp_str;
            QStringList temp_str_list = temp_str.split("-");
            QString start_str = temp_str_list.at(0);
            QString end_str = temp_str_list.at(1);
            query_index.addBindValue(start_str);
            query_index.addBindValue(end_str);
        }
        else
        {
            QString str = obj.timestamp_str;
            str.prepend("%");
            str.append("%");
            query_index.addBindValue(str);
        }
    }
    else
    {
        query_index.addBindValue(obj.timestamp_str);
    }
    query_index.addBindValue(QString::number(start_timestamp_utc_qint64));
    query_index.addBindValue(QString::number(end_timestamp_utc_qint64));
    insert_values_in_save_result_db(query_index,result_name);

    source_db.close();
}

void artifacts_timeline_full_loader::insert_values_in_save_result_db(QSqlQuery &query_index, QString result_name)
{

    ///saved artifatcs timeline
    QString db_path = saved_db_path;
    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open ---FAILED--- " + db_path, Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }
    QSqlQuery insert_query(destination_db);

    if(!query_index.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_index.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_index.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return ;
    }

    qint64 counter = 0;

    while(query_index.next())
    {
        counter++;

        if(counter % 500 == 0)
            QCoreApplication::processEvents();

        insert_query.prepare("insert into timeline_data(INT,bookmark,recon_tag_value,timestamp,stamp_type,record_no,"
                             "plugin,category,os_scheme_display,item1,item2,notes,source_count_name,result_name,source_file)"
                             "values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");


        ///selected values ----INT,bookmark,notes_icon,recon_tag_value,timestamp,stamp_type,record_no,plugin,category,os_scheme_display,item1,item2,notes,source_count_name,INT
        insert_query.addBindValue(query_index.value("INT").toString());//INT
        insert_query.addBindValue(query_index.value("bookmark").toString());//bookmark
        insert_query.addBindValue(query_index.value("recon_tag_value").toString());//tag value
        insert_query.addBindValue(query_index.value("timestamp").toString());//timestamp
        insert_query.addBindValue(query_index.value("stamp_type").toString());//stamp type
        insert_query.addBindValue(query_index.value("record_no").toString());//record no.
        insert_query.addBindValue(query_index.value("plugin").toString());//plugin name
        insert_query.addBindValue(query_index.value("category").toString());//tab name
        insert_query.addBindValue(query_index.value("os_scheme_display").toString());
        insert_query.addBindValue(query_index.value("item1").toString());//item1
        insert_query.addBindValue(query_index.value("item2").toString());//item2
        insert_query.addBindValue(query_index.value("notes").toString());//notes
        insert_query.addBindValue(query_index.value("source_count_name").toString());//source_count_name
        insert_query.addBindValue(result_name);
        insert_query.addBindValue(query_index.value("source_file").toString());//source_count_name

        if(!insert_query.exec())
        {
            recon_static_functions::app_debug("insert_query  ----FAILED---- ",Q_FUNC_INFO);
            recon_static_functions::app_debug(" error- " + insert_query.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug(" query- " + insert_query.executedQuery(),Q_FUNC_INFO);
            continue ;
        }

    }
    destination_db.close();
}

void artifacts_timeline_full_loader::slot_add_notes_pushbutton_clicked(QString notes_value)
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

    add_notes_to_singal_record_obj->hide();

    notes_value = notes_value.trimmed();

    int bookmark_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString,0);

    if(bookmark_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

    QList<struct_global_tagging_artifact_timeline_record> timeline_record_list;

    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 100 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();

        if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
            continue;

        if(m_tablewidget_obj->isRowHidden(selected_row_no))
            continue;

        INT_List << m_tablewidget_obj->item(selected_row_no, index_for_INT_only_for_artifact_timeline)->text();

        struct_global_tagging_artifact_timeline_record s1;
        s1.plugin_record_no = m_tablewidget_obj->item(selected_row_no, record_no_index)->text();
        s1.plugin_name = m_tablewidget_obj->item(selected_row_no, plugin_name_index)->text();
        s1.tab_name = m_tablewidget_obj->item(selected_row_no, tab_name_index)->text();
        s1.source_count_name = m_tablewidget_obj->item(selected_row_no, source_count_name_index)->text();
        s1.item2 = m_tablewidget_obj->item(selected_row_no, 9)->text();
        s1.item3 = m_tablewidget_obj->item(selected_row_no, 10)->text();
        s1.item4 = m_tablewidget_obj->item(selected_row_no, 3)->text();
        s1.item5 = m_tablewidget_obj->item(selected_row_no, stamp_type_index)->text();

        timeline_record_list << s1;

        QIcon iconNotes("../icons/notes.png");
        m_tablewidget_obj->item(selected_row_no, notes_column_index)->setIcon(iconNotes);

    }

    global_connection_manager_obj->pub_set_notes_in_artifact_timeline_db(INT_List, notes_value);
    global_connection_manager_obj->pub_add_notes_for_artifact_timeline(timeline_record_list, notes_value);

    display_loading_progress_bar_non_cancelable_obj->hide();
}

void artifacts_timeline_full_loader::slot_goto_artifact_source_submenu_clicked(QAction *clicked_action)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString source_path_goto_source = clicked_action->toolTip();

    source_path_goto_source = source_path_goto_source.trimmed();
    if(source_path_goto_source.isEmpty())
        return;

    QString record_no_str   = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), record_no_index)->text();
    QString plugin_name_str = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), plugin_name_index)->text();
    QString tab_name_str    = m_tablewidget_obj->item(m_tablewidget_obj->currentRow(), tab_name_index)->text();

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;

    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);

    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);

    QString command = "SELECT source_count_name FROM '" + table_name + "' WHERE INT=?";
    QStringList arg_list;
    arg_list << record_no_str;
    QString source_count_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugins_destination_db_file,Q_FUNC_INFO);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    emit signal_go_to_artifact_source_in_file_system_from_loader(source_path_goto_source,fs_db_path,source_count_name);

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}


void artifacts_timeline_full_loader::slot_timeline_export_as_clicked(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString(MACRO_Generic_Export_As_CSV))
    {
        QString filename = tabwidget_timeline_data_obj->tabText(tabwidget_timeline_data_obj->currentIndex());
        export_dialog_box_2_obj->pub_set_export_type(QString(MACRO_Generic_Export_As_CSV));
        QString lab_export_dir_path = getenv("HOME") + QString("/Desktop") + "/LAB_Export";
        export_dialog_box_2_obj->pub_set_exported_file_name_and_dir_path(filename, lab_export_dir_path);

        export_dialog_box_2_obj->show();

    }
    else if(current_clicked_action->text() == QString(MACRO_Generic_Export_As_SQLite))
    {
        QString filename = tabwidget_timeline_data_obj->tabText(tabwidget_timeline_data_obj->currentIndex());
        export_dialog_box_2_obj->pub_set_export_type(QString(MACRO_Generic_Export_As_SQLite));
        QString lab_export_dir_path = getenv("HOME") + QString("/Desktop") + "/LAB_Export";
        export_dialog_box_2_obj->pub_set_exported_file_name_and_dir_path(filename, lab_export_dir_path);

        export_dialog_box_2_obj->show();
    }
}

void artifacts_timeline_full_loader::export_timeline_data_as_csv(QString filepath)
{

    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        if(selection_model_list.size() <= 0)
        {
            display_loading_progress_bar_obj->hide();
            QMessageBox::information(this,"Export ","Please select at least one item");
            return;
        }
    }

    QFile file(filepath);
    if (!file.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("File open -----FAILED----" + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + file.errorString() , Q_FUNC_INFO);
        return;
    }


    /// write data to CSV file
    QStringList header_list;


    /// creating file header
    header_list << "Sr. No.";
    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        //        if(m_tablewidget_obj->isColumnHidden(index_for_INT_only_for_artifact_timeline))
        //            continue ;

        QString title_str = m_tablewidget_obj->horizontalHeaderItem(column)->text();

        title_str = title_str.trimmed();
        if(title_str.isEmpty() || column == source_count_name_index || column == os_name_index || column == index_for_INT_only_for_artifact_timeline) // skip last(Source Count Name)
            continue;


        header_list << title_str;
    }

    QString header_str = header_list.join(",");

    file.write(header_str.toLocal8Bit());
    file.write(QString("\n").toLocal8Bit());


    QStringList values_list ;
    QString value_str;
    QStringList output_csv_values_list ;


    /// writing data to file
    int count = 0;
    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {

        ///Export Screen Items
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            count++;
            if(count % 200 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(m_tablewidget_obj->isRowHidden(row))
                continue;

            QString sr_no_str = QString::number(row + 1);

            QString timestamp_str = m_tablewidget_obj->item(row , timestamp_index)->text().trimmed();
            QString type_str = m_tablewidget_obj->item(row , stamp_type_index)->text().trimmed();
            QString record_str = m_tablewidget_obj->item(row , record_no_index)->text().trimmed();
            QString plugin_str = m_tablewidget_obj->item(row , plugin_name_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(row , tab_name_index)->text().trimmed();
            QString item_1_str = m_tablewidget_obj->item(row , item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(row , item_2_index)->text().trimmed();

            values_list.clear();

            values_list << sr_no_str << timestamp_str << type_str << record_str
                        << plugin_str << category_str << item_1_str << item_2_str;

            output_csv_values_list.clear();

            for(int ii = 0; ii < values_list.size(); ii++)
            {
                QString val_str = values_list.at(ii);
                //This is for string delimiter
                val_str.prepend("\"");
                val_str.append("\"");
                output_csv_values_list << val_str;
            }

            output_csv_values_list << QString("\n");

            value_str = output_csv_values_list.join(",");
            file.write(value_str.toLocal8Bit());
        }

    }
    else if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        ///Export Selected Items
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        for(int row = 0; row < selection_model_list.size(); row++)
        {
            if(row % 100 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
                continue;

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString sr_no_str = QString::number(row + 1);

            QString timestamp_str = m_tablewidget_obj->item(selected_row_no , timestamp_index)->text().trimmed();
            QString type_str = m_tablewidget_obj->item(selected_row_no , stamp_type_index)->text().trimmed();
            QString record_str = m_tablewidget_obj->item(selected_row_no , record_no_index)->text().trimmed();

            QString plugin_str = m_tablewidget_obj->item(selected_row_no , plugin_name_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(selected_row_no , tab_name_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(selected_row_no , item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(selected_row_no , item_2_index)->text().trimmed();
            values_list.clear();

            values_list << sr_no_str << timestamp_str << type_str << record_str
                        << plugin_str << category_str << item_1_str << item_2_str;

            output_csv_values_list.clear();

            for(int ii = 0; ii < values_list.size(); ii++)
            {
                QString val_str = values_list.at(ii);
                //This is for string delimiter
                val_str.prepend("\"");
                val_str.append("\"");
                output_csv_values_list << val_str;
            }

            output_csv_values_list << QString("\n");

            value_str = output_csv_values_list.join(",");

            file.write(value_str.toLocal8Bit());
        }

    }

    file.flush();
    file.close();

    display_loading_progress_bar_obj->hide();

    if(!filepath.isEmpty())
        open_exported_file(filepath);


}

void artifacts_timeline_full_loader::export_timeline_data_as_sqlite(QString filepath)
{
    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        if(selection_model_list.size() <= 0)
        {
            display_loading_progress_bar_obj->hide();
            QMessageBox::information(this,"Export","Please select at least one item");
            return;
        }
    }

    /// creating file header
    QStringList header_list;
    header_list << "Sr. No.";
    for(int column = 0; column < m_tablewidget_obj->columnCount(); column++)
    {
        QString title_str = m_tablewidget_obj->horizontalHeaderItem(column)->text();

        title_str = title_str.trimmed();
        if(title_str.isEmpty() || column == source_count_name_index || column == os_name_index || column == index_for_INT_only_for_artifact_timeline) // skip last(Source Count Name)
            continue;

        header_list << title_str;
    }

    ///=========Command Table Create and Insert-Start=========//
    QString col_str;
    QString cmd_table_create = "Create Table 'files' (";
    QString cmd_insert = QString("Insert into 'files' (");
    QString cmd_insert_val = QString(" Values(");
    for(int num = 0 ; num < header_list.size() ; num++)
    {
        col_str = header_list.at(num);
        col_str.replace(".", "");
        col_str.replace(" ", "_");
        col_str.replace("/", "_");
        col_str.replace("(", "_");
        col_str.replace(")", "");
        col_str.replace("__", "_");

        cmd_table_create.append(col_str);
        cmd_table_create.append(" varchar(1024),");

        cmd_insert.append(col_str);
        cmd_insert.append(",");
        cmd_insert_val.append("?,");

    }
    if(cmd_table_create.endsWith(","))
        cmd_table_create.chop(1);
    cmd_table_create.append(")");

    if(cmd_insert.endsWith(","))
        cmd_insert.chop(1);
    cmd_insert.append(")");

    if(cmd_insert_val.endsWith(","))
        cmd_insert_val.chop(1);
    cmd_insert_val.append(")");

    cmd_insert.append(cmd_insert_val);
    ///=========Command Table Create and Insert-Start=========//

    QString connection_naam_exp_db = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam_exp_db);
    QSqlDatabase dest_export_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_exp_db);
    dest_export_db.setDatabaseName(filepath);
    if(!dest_export_db.open())
    {
        recon_static_functions::app_debug(" Destination Db opening --------FAILED------ " + dest_export_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + dest_export_db.lastError().text(),Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam_exp_db);
        return;
    }

    QSqlQuery qry_export_db(dest_export_db);

    qry_export_db.prepare(cmd_table_create);

    if(!qry_export_db.exec())
    {
        recon_static_functions::app_debug(" table create command --------FAILED------ " + qry_export_db.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + qry_export_db.lastError().text(),Q_FUNC_INFO);

        dest_export_db.close();
        QSqlDatabase::removeDatabase(connection_naam_exp_db);
        return;
    }

    dest_export_db.transaction();


    QStringList values_list ;
    QString value_str;

    int count = 0;
    if(report_scope_str == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        ///Export Screen Items
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            count++;
            if(count % 200 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(m_tablewidget_obj->isRowHidden(row))
                continue;

            QString sr_no_str = QString::number(row + 1);

            QString timestamp_str = m_tablewidget_obj->item(row , timestamp_index)->text().trimmed();
            QString type_str = m_tablewidget_obj->item(row , stamp_type_index)->text().trimmed();
            QString record_str = m_tablewidget_obj->item(row , record_no_index)->text().trimmed();

            QString plugin_str = m_tablewidget_obj->item(row , plugin_name_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(row , tab_name_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(row , item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(row , item_2_index)->text().trimmed();

            qry_export_db.prepare(cmd_insert);

            qry_export_db.addBindValue(sr_no_str);
            qry_export_db.addBindValue(timestamp_str);
            qry_export_db.addBindValue(type_str);
            qry_export_db.addBindValue(record_str);

            qry_export_db.addBindValue(plugin_str);
            qry_export_db.addBindValue(category_str);
            qry_export_db.addBindValue(item_1_str);
            qry_export_db.addBindValue(item_2_str);

            if(!qry_export_db.exec())
            {
                recon_static_functions::app_debug("insert command --------FAILED------ " + qry_export_db.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + qry_export_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }
    }
    else if(report_scope_str == QString(MACRO_REPORT_SCOPE_SELECTED_ITEMS_QString))
    {
        ///Export Selected Items
        QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();

        for(int row = 0; row < selection_model_list.size(); row++)
        {
            if(row % 100 == 0)
                QCoreApplication::processEvents();

            QModelIndex model_index = selection_model_list.at(row);

            int selected_row_no = model_index.row();

            if(selected_row_no < 0 && selected_row_no >= m_tablewidget_obj->rowCount())
                continue;

            if(m_tablewidget_obj->isRowHidden(selected_row_no))
                continue;

            QString sr_no_str = QString::number(row + 1);

            QString timestamp_str = m_tablewidget_obj->item(selected_row_no , timestamp_index)->text().trimmed();
            QString type_str = m_tablewidget_obj->item(selected_row_no , stamp_type_index)->text().trimmed();
            QString record_str = m_tablewidget_obj->item(selected_row_no , record_no_index)->text().trimmed();

            QString plugin_str = m_tablewidget_obj->item(selected_row_no , plugin_name_index)->text().trimmed();
            QString category_str = m_tablewidget_obj->item(selected_row_no , tab_name_index)->text().trimmed();

            QString item_1_str = m_tablewidget_obj->item(selected_row_no , item_1_index)->text().trimmed();
            QString item_2_str = m_tablewidget_obj->item(selected_row_no , item_2_index)->text().trimmed();

            qry_export_db.prepare(cmd_insert);

            qry_export_db.addBindValue(sr_no_str);
            qry_export_db.addBindValue(timestamp_str);
            qry_export_db.addBindValue(type_str);
            qry_export_db.addBindValue(record_str);

            qry_export_db.addBindValue(plugin_str);
            qry_export_db.addBindValue(category_str);
            qry_export_db.addBindValue(item_1_str);
            qry_export_db.addBindValue(item_2_str);

            if(!qry_export_db.exec())
            {
                recon_static_functions::app_debug("insert command --------FAILED------ " + qry_export_db.executedQuery(),Q_FUNC_INFO);
                recon_static_functions::app_debug("error -  " + qry_export_db.lastError().text(),Q_FUNC_INFO);
                continue;
            }
        }
    }

    dest_export_db.commit();
    dest_export_db.close();

    display_loading_progress_bar_obj->hide();

    if(!filepath.isEmpty())
        open_exported_file(filepath);

}

void artifacts_timeline_full_loader::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int tab_index = tabwidget_timeline_data_obj->currentIndex();
    QString current_tab_name = tabwidget_timeline_data_obj->tabText(tab_index);

    if(tab_custom_header_widgets_QMap.size() < tab_index)
        return;

    QList<custom_table_header_widget *> custom_header_widgets_QList;

    custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(current_tab_name);

    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    QList<struct_search_column_text_for_artifact_timeline> search_text_col_QList;

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_artifact_timeline struct_search_column_text_for_artifact_timeline_obj;

            struct_search_column_text_for_artifact_timeline_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_artifact_timeline_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_artifact_timeline_obj);
            bool_stop_sorting_tags_bookmark_notes_after_search = true;
        }
    }

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    if(search_text_col_QList.size() < 1)
    {
        for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(row % 20 == 0)
                QCoreApplication::processEvents();

            m_tablewidget_obj->showRow(row);
        }
        bool_stop_sorting_tags_bookmark_notes_after_search = false;

        display_loading_progress_bar_obj->hide();
        return;
    }

    all_column_text_search(search_text_col_QList);

    pub_refresh_horizontal_header();

    emit signal_clear_all_metadata();
    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void artifacts_timeline_full_loader::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int tab_index = tabwidget_timeline_data_obj->currentIndex();
    QString current_tab_name = tabwidget_timeline_data_obj->tabText(tab_index);

    if(tab_custom_header_widgets_QMap.size() < tab_index)
        return;

    QList<custom_table_header_widget *> custom_header_widgets_QList;

    custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(current_tab_name);

    if(m_tablewidget_obj == NULL)
        return;

    if((column != enum_tablewidget_column_bookmark || !custom_header_widgets_QList.at(column)->pub_get_column_name().isEmpty())
            && (column != enum_tablewidget_column_notes || !custom_header_widgets_QList.at(column)->pub_get_column_name().isEmpty())
            && (column != enum_tablewidget_column_tags || !custom_header_widgets_QList.at(column)->pub_get_column_name().isEmpty()))
    {

        if(column_sort_order == 0)
            m_tablewidget_obj->sortItems(column, Qt::DescendingOrder);
        else
            m_tablewidget_obj->sortItems(column, Qt::AscendingOrder);

        pub_refresh_horizontal_header();
        return;
    }

    if(bool_stop_sorting_tags_bookmark_notes_after_search)
        return;


    m_tablewidget_obj->clearSelection();

    if(m_tablewidget_obj->rowCount() <= 1)
        return;

    qint64 row_shift_pos_ascending = 0;
    qint64 row_shift_pos_descending = m_tablewidget_obj->rowCount() - 1;

    if(column_sort_order == 1)
    {
        for(qint64 row = 0; row < m_tablewidget_obj->rowCount(); row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column)->icon();

            if(m_tablewidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_acs;
                icon_shiftrow_acs =  m_tablewidget_obj->item(row_shift_pos_ascending, column)->icon();

                if(m_tablewidget_obj->item(row_shift_pos_ascending, column)->checkState() || (icon_shiftrow_acs.availableSizes(icon_shiftrow_acs.Normal, icon_shiftrow_acs.Off).size() != 0))
                {
                    row_shift_pos_ascending++;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_ascending;

                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_ascending++;
            }
        }
    }
    else
    {
        for(qint64 row = 0; row < row_shift_pos_descending; row++)
        {
            if(m_tablewidget_obj->isRowHidden(row))
            {
                continue;
            }

            if(m_tablewidget_obj->item(row, column) == NULL)
                continue;

            if(m_tablewidget_obj->item(row_shift_pos_descending, column) == NULL)
                continue;

            QIcon icon_set;
            icon_set =  m_tablewidget_obj->item(row, column)->icon();

            if(m_tablewidget_obj->item(row, column)->checkState() || (icon_set.availableSizes(icon_set.Normal, icon_set.Off).size() != 0))
            {
                QIcon icon_shiftrow_desc;

                icon_shiftrow_desc =  m_tablewidget_obj->item(row_shift_pos_descending, column)->icon();

                if(m_tablewidget_obj->item(row_shift_pos_descending, column)->checkState() || (icon_shiftrow_desc.availableSizes(icon_shiftrow_desc.Normal, icon_shiftrow_desc.Off).size() != 0))
                {
                    row_shift_pos_descending--;
                    continue;
                }

                const int sourceRow = row;
                const int destRow = row_shift_pos_descending;

                QList<QTableWidgetItem*> sourceItems = m_tablewidget_obj->takeRow(sourceRow);
                QList<QTableWidgetItem*> destItems = m_tablewidget_obj->takeRow(destRow);

                m_tablewidget_obj->setRow(destRow, sourceItems);
                m_tablewidget_obj->setRow(sourceRow, destItems);

                row_shift_pos_descending--;
            }
        }
    }

    pub_refresh_horizontal_header();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}

void artifacts_timeline_full_loader::all_column_text_search(QList<struct_search_column_text_for_artifact_timeline> search_text_all_column_list)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    for(int row = 0; row < m_tablewidget_obj->rowCount(); row++)
    {
        if(bool_cancel_loading)
            break;

        if(row % 20 == 0)
            QCoreApplication::processEvents();

        bool hide_row = false;

        for(int column_count = 0; column_count < search_text_all_column_list.size(); column_count++)
        {
            struct_search_column_text_for_artifact_timeline struct_search_column_text_for_artifact_timeline_obj = search_text_all_column_list.at(column_count);

            if(!(m_tablewidget_obj->item(row, struct_search_column_text_for_artifact_timeline_obj.column_number)->text().trimmed().contains(struct_search_column_text_for_artifact_timeline_obj.search_text, Qt::CaseInsensitive)))
            {
                hide_row = true;

                break;
            }
        }

        if(hide_row)
        {
            m_tablewidget_obj->hideRow(row);
        }
        else
        {
            m_tablewidget_obj->showRow(row);
        }
    }

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}
