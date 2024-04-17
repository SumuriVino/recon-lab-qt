#include "redefined_result_full_loader.h"

void redefined_result_full_loader::slot_line_edit_go_to_page_text_changed(QString text)
{
    QPalette palette;

    QString goto_page_str = text;
    int page = goto_page_str.toInt();
    if(page > stackwidget_obj->count() || page <= 0)
    {
        palette.setColor(QPalette::Text,Qt::red);
        linedit_go_to_page->setPalette(palette);
        return;
    }
    else
    {

    }

    return;
}

void redefined_result_full_loader::slot_pushButton_topurl_export_clicked()
{
    QString export_file_path ,filename ;

    filename = QString("graph-") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")) + ".png" ;

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        export_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Browser_History_In_Result_QString).toString();
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        export_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Messenger_In_Result_QString).toString();

    export_dialog_box_1_obj->show();

    pushButton_msg_pie_chart_topurl_export->setDisabled(true);
    pushButton_save_url_pie->setDisabled(true);

    export_dialog_box_1_obj->pub_set_export_type(QString(MACRO_Generic_Export_As_CSV));
    export_dialog_box_1_obj->pub_set_name_and_dir_path(filename,export_file_path);

}

void redefined_result_full_loader::insert_values_into_save_graph_db(QString graph_name, QString saved_graph_db_path, QString saved_graph_path)
{

    QString category_name ;
    QString recon_file_from_result = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        QFileInfo file(saved_graph_path);
        if(file.exists())
        {
            QString saved_graph_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            if(saved_graph_path.startsWith(saved_graph_dir_path))
                saved_graph_path.remove(0, saved_graph_dir_path.size());

            QDateTime date_created = file.birthTime();

            qint64 epch_time = date_created.toSecsSinceEpoch();

            category_name = MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs ;

            QStringList arg_list;
            if(!saved_graph_path.startsWith("/"))
                saved_graph_path.prepend("/");
            QString command_insert = "INSERT INTO saved_graphs(category,plugin_name, tab_name,recon_tag_value,bookmark,"
                                     "notes,graph_path,graph_name,timestamp,recon_filefrom , recon_file_infopath) VALUES (?,?,?,?, ?,?,?,?, ?,?,?)";
            arg_list << category_name << QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History) << QString(MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs) <<  QString(MACRO_Tag_Name_Saved_graphs)  << ""
                     <<""<< saved_graph_path << graph_name << QString::number(epch_time) << recon_file_from_result << saved_graph_path;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,saved_graph_db_path,Q_FUNC_INFO);

        }
    }

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        QFileInfo file(saved_graph_path);
        if(file.exists())
        {
            QString saved_graph_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
            if(saved_graph_path.startsWith(saved_graph_dir_path))
                saved_graph_path.remove(0, saved_graph_dir_path.size());

            QDateTime date_created = file.birthTime();

            qint64 epch_time = date_created.toSecsSinceEpoch();
            category_name = MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs ;

            if(!saved_graph_path.startsWith("/"))
                saved_graph_path.prepend("/");
            QStringList arg_list;
            QString command_insert = "INSERT INTO saved_graphs(category,plugin_name, tab_name,recon_tag_value,bookmark,"
                                     "notes,graph_path,graph_name,timestamp,recon_filefrom , recon_file_infopath) VALUES (?,?,?,?, ?,?,?,?, ?,?,?)";
            arg_list << category_name << QString(MACRO_Plugin_Name_Saved_Graphs_Messenger) << QString(MACRO_Plugin_Name_Saved_Graphs_Messenger) <<  QString(MACRO_Tag_Name_Saved_graphs)  << ""
                     <<""<< saved_graph_path << graph_name << QString::number(epch_time) << recon_file_from_result << saved_graph_path;
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command_insert,arg_list,saved_graph_db_path,Q_FUNC_INFO);

        }
    }

    emit signal_saved_graph_category_name(category_name);
}

void redefined_result_full_loader::slot_pushbutton_export_graph_clicked()
{
    QString export_file_path ,filepath, filename;

    filename = QString("graph-") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")) + ".png" ;

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        export_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Browser_History_In_Result_QString).toString();
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        export_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_File_Location_Graphs_Messenger_In_Result_QString).toString();


    if(export_file_path.endsWith("/"))
        filepath = export_file_path + filename;
    else
        filepath = export_file_path + QString("/") + filename;

    export_dialog_box_1_obj->show();

    pushButton_export_graph->setDisabled(true);
    pushButton_save_graph->setDisabled(true);

    export_dialog_box_1_obj->pub_set_export_type(QString(MACRO_Generic_Export_As_CSV));

    export_dialog_box_1_obj->pub_set_name_and_dir_path(filename,export_file_path);

}

void redefined_result_full_loader::slot_hovered_toolTip_on_barsets(bool status, int index, QBarSet *barset)
{
    QChart *chart = qobject_cast<QChart *>(barset->parent()->parent()->parent());

    m_barset *barset_m = dynamic_cast<m_barset *>(barset);

    if(status)
    {

        bool_check_barset = true;
        QMap<int, QString> map_insert_index_with_category = barset_m->get_tooltip();

        if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR && tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_pie_or_visit_count_graph)
        {
            chart->setToolTip(map_insert_index_with_category.value(index).trimmed() + QString(",") + QString::number(barset->at(index)));

        }else
        {
            chart->setToolTip(map_insert_index_with_category.value(index).trimmed());
        }

    }
    else
    {
        bool_check_barset = false;
        chart->setToolTip("");
    }
}

void redefined_result_full_loader::slot_pushbutton_previous_clicked(bool is_clicked)
{

    if(stackwidget_index == 0)
    {
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

    label_show_date_time->setText(map_dates_labels.value(stackwidget_index));


    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

}

void redefined_result_full_loader::slot_export_graph_by_saved_name(QString given_name , QString selected_dir_path, QString exp_type_str)
{

    export_dialog_box_1_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();



    pushButton_msg_pie_chart_topurl_export->setEnabled(true);
    pushButton_save_url_pie->setEnabled(true);

    pushButton_save_graph->setEnabled(true);
    pushButton_export_graph->setEnabled(true);


    QString saved_name_str = given_name;
    if(saved_name_str.contains(" "))
        saved_name_str.replace(" ","_");

    if(!saved_name_str.endsWith("png"))
        saved_name_str = saved_name_str + ".png" ;

    if(!selected_dir_path.endsWith("/"))
        selected_dir_path.append("/");


    int current_index =  tabwidget_redefined_result->currentIndex();

    QString available_path;

    if(current_index == 1)
    {
        QString available_file =  recon_static_functions::get_available_filename(saved_name_str , selected_dir_path, Q_FUNC_INFO);


        if(selected_dir_path.endsWith("/"))
            available_path = selected_dir_path + available_file;
        else
            available_path = selected_dir_path + QString("/") + available_file;


        if(!available_path.endsWith("png"))
            available_path = available_path + ".png";



        QChartView *tmp_chartview =  (QChartView *)stackwidget_obj->currentWidget();
        if(tmp_chartview != NULL)
        {
            const auto dpr =  tmp_chartview->devicePixelRatioF();

            QPixmap pixmap(tmp_chartview->width() * dpr , tmp_chartview->height() * dpr);
            pixmap.setDevicePixelRatio(dpr);
            pixmap.fill(Qt::transparent);

            QPainter *paint = new QPainter(&pixmap);
            paint->setPen(*(new QColor(255,34,255,255)));
            tmp_chartview->render(paint);

            pixmap.save(available_path , "PNG");
            delete paint;

        }

    }
    else if(current_index == 2)
    {
        QString available_file =  recon_static_functions::get_available_filename(saved_name_str , selected_dir_path, Q_FUNC_INFO);


        if(selected_dir_path.endsWith("/"))
            available_path = selected_dir_path + available_file;
        else
            available_path = selected_dir_path + QString("/") + available_file;


        if(!available_path.endsWith("png"))
            available_path = available_path + ".png";


        const auto dpr =  chartView_for_pie_or_visit_count_obj->devicePixelRatioF();

        QPixmap pmap(chartView_for_pie_or_visit_count_obj->width() * dpr , chartView_for_pie_or_visit_count_obj->height() * dpr);
        pmap.setDevicePixelRatio(dpr);
        pmap.fill(Qt::transparent);

        QPainter *paint = new QPainter(&pmap);
        paint->setPen(*(new QColor(255,34,255,255)));
        chartView_for_pie_or_visit_count_obj->render(paint);

        pmap.save(available_path , "PNG");
        delete paint;

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

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
        QProcess *myProcess = new QProcess(this);
        myProcess->startDetached("open",QStringList(available_path));
    }

}

void redefined_result_full_loader::slot_export_graphs_cancel_results()
{
    pushButton_save_graph->setEnabled(true);
    pushButton_save_url_pie->setEnabled(true);

    pushButton_export_graph->setEnabled(true);
    pushButton_msg_pie_chart_topurl_export->setEnabled(true);

}

void redefined_result_full_loader::slot_pushButton_save_graph_clicked()
{
    save_graph_dialog_box_obj->pub_set_graph_name(QString("Saved Graph PNG-") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")));
    save_graph_dialog_box_obj->show();

}
void redefined_result_full_loader::slot_pushButton_save_url_pie_clicked()
{
    save_graph_dialog_box_obj->pub_set_graph_name(QString("Saved Graph PNG-") + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")));
    save_graph_dialog_box_obj->show();

}


void redefined_result_full_loader::slot_pushbutton_next_clicked(bool is_clicked)
{
    if(stackwidget_index == (stackwidget_obj->count() -1 ))
    {
        return;
    }

    linedit_go_to_page->clear();

    if(stackwidget_index == (stackwidget_obj->count() -2 ))
    {
        pushbutton_next->setEnabled(false);
    }

    pushbutton_previous->setEnabled(true);


    stackwidget_index++;
    stackwidget_obj->setCurrentIndex(stackwidget_index);
    label_show_date_time->setText(map_dates_labels.value(stackwidget_index));


    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

}

void redefined_result_full_loader::slot_pushbutton_go_to_page_clicked()
{
    QString goto_page_str = linedit_go_to_page->text();
    int page = goto_page_str.toInt();

    if(stackwidget_obj->count() == 1)
        return;

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


    label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

}

void redefined_result_full_loader::slot_radioButton_yearly_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);
    if(!checked)
        return;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    label_show_date_time->clear();
    label_show_pages->clear();
    linedit_go_to_page->clear();


    int count_stacks = stackwidget_obj->count();

    int start_remove_stack = 0;
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {

            if(bool_cancel_loading)
            {
                break;
            }
            QCoreApplication::processEvents();
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
            start_remove_stack++;

        }
    }


    map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_yearly);
    list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_yearly);
    for(int i = 0; i < list_of_charview.size();i++)
    {
        if(bool_cancel_loading)
        {
            break;
        }
        stackwidget_obj->addWidget(list_of_charview.at(i));
    }

    if(stackwidget_obj->count() <= 0)
    {
        display_loading_progress_bar_obj->hide();
        return;
    }

    stackwidget_index = enum_redefined_result_graphs_yearly;

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


    label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_radioButton_monthly_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    label_show_date_time->clear();
    label_show_pages->clear();
    linedit_go_to_page->clear();


    int count_stacks = stackwidget_obj->count();

    int start_remove_stack = 0;
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            if(bool_cancel_loading)
            {
                break;
            }
            QCoreApplication::processEvents();
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
            start_remove_stack++;

        }
    }

    map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_monthly);
    list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_monthly);
    for(int i = 0; i < list_of_charview.size();i++)
    {
        if(bool_cancel_loading)
        {
            break;
        }
        stackwidget_obj->addWidget(list_of_charview.at(i));
    }

    if(stackwidget_obj->count() <= 0)
    {
        display_loading_progress_bar_obj->hide();
        return;
    }

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



    label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_radioButton_daywise_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    label_show_date_time->clear();
    label_show_pages->clear();
    linedit_go_to_page->clear();


    int count_stacks = stackwidget_obj->count();
    int start_remove_stack = 0;
    if(count_stacks > 0)
    {
        for(int i = (count_stacks-1); i >= 0; i--)
        {
            if(bool_cancel_loading)
            {
                break;
            }

            QCoreApplication::processEvents();
            stackwidget_obj->removeWidget(stackwidget_obj->widget(i));
            delete stackwidget_obj->widget(i);
            start_remove_stack++;

        }
    }


    map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_daywise);
    list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_daywise);
    for(int i = 0; i < list_of_charview.size();i++)
    {
        if(bool_cancel_loading)
        {
            break;
        }
        stackwidget_obj->addWidget(list_of_charview.at(i));
    }

    if(stackwidget_obj->count() <= 0)
    {
        display_loading_progress_bar_obj->hide();
        return;
    }

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

    label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
    label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));

    display_loading_progress_bar_obj->hide();

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_radioButton_hourly_clicked(bool checked)
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    if(!checked)
        return;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    label_show_date_time->clear();
    label_show_pages->clear();
    linedit_go_to_page->clear();


    radiobutton_create_graph_yearly->setDisabled(true);
    radiobutton_create_graph_monthly->setDisabled(true);
    radiobutton_create_graph_daywise->setDisabled(true);
    tablewidget_accounts_configure_list->setDisabled(true);
    tablewidget_participants_list->setDisabled(true);

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

    map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_hourly);
    if(map_dates_labels.isEmpty())
    {
        stackwidget_index = 0;

        list_of_charview.clear();

        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
            create_graph_messenger_hourly();
        else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
            create_graph_hourly_common_function();

        map_of_chartview.insert(enum_redefined_result_graphs_hourly,list_of_charview);
        map_of_date_lables_map.insert(enum_redefined_result_graphs_hourly,map_dates_labels);
        stackwidget_obj->setCurrentIndex(stackwidget_index);
    }else
    {

        map_dates_labels = map_of_date_lables_map.value(enum_redefined_result_graphs_hourly);
        list_of_charview = map_of_chartview.value(enum_redefined_result_graphs_hourly);
        for(int i = 0; i < list_of_charview.size();i++)
            stackwidget_obj->addWidget(list_of_charview.at(i));

        stackwidget_index = 0;
        stackwidget_obj->setCurrentIndex(stackwidget_index);

        label_show_date_time->setText(map_dates_labels.value(stackwidget_index));
        label_show_pages->setText("Page " + QString::number(stackwidget_index+1) + " of " + QString::number(stackwidget_obj->count()));


    }

    radiobutton_create_graph_yearly->setDisabled(false);
    radiobutton_create_graph_monthly->setDisabled(false);
    radiobutton_create_graph_daywise->setDisabled(false);
    tablewidget_accounts_configure_list->setDisabled(false);
    tablewidget_participants_list->setDisabled(false);

    if(stackwidget_obj->count() <= 0)
    {
        display_loading_progress_bar_obj->hide();
        return;
    }

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

    display_loading_progress_bar_obj->hide();


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_pie_slice_doubleClicked()
{

    QPieSlice *slice = dynamic_cast<QPieSlice*> (sender());

    QString label_str = slice->label();
    //label_str.remove(0,label_str.indexOf("-") + 1);

    label_str.remove(label_str.lastIndexOf(","),label_str.size());

    label_str = label_str.trimmed();

    current_barset_participant = label_str;
    slot_act_show_data_of_current_barset_triggered();

    QString tabname = tabwidget_redefined_result->tabText(tabwidget_redefined_result->currentIndex());
    qmap_barset_participants_str.insert(tabname,current_barset_participant);
    qmap_configured_account_str.insert(tabname,selected_configured_account_name);
    qmap_bar_graph_radiobutton_type_str.insert(tabname,get_graph_type());
    qmap_bool_save_data_other_than_pie_and_visit_count_bool.insert(tabname,false);

}

void redefined_result_full_loader::slot_pie_slice_pressed()
{
    QPieSlice *slice = dynamic_cast<QPieSlice*> (sender());

    QString label_str = slice->label();
    //label_str.remove(0,label_str.indexOf("-") + 1);

    label_str.remove(label_str.lastIndexOf(","),label_str.size());

    label_str = label_str.trimmed();

    current_barset_participant = label_str;

}

void redefined_result_full_loader::slot_barset_doubleClicked(int index)
{
    m_barset* barset = dynamic_cast<m_barset*> (sender());

    QMap<int,QString>map_barset = barset->get_tooltip();

    QString str = map_barset.value(index);


    ///clicked domain extract for visit count in Browser History
    current_clicked_domain.clear();
    if(!str.contains("\n"))
    {
        current_clicked_domain = str;
        slot_act_show_data_of_current_barset_triggered();
        QString tabname = tabwidget_redefined_result->tabText(tabwidget_redefined_result->currentIndex());
        qmap_domians_str.insert(tabname,current_clicked_domain);
        qmap_bool_save_data_other_than_pie_and_visit_count_bool.insert(tabname , false);
        return;
    }

    ///-----

    current_barset_category_str.clear();
    current_barset_participant.clear();

    ////do not use skip Qt::SkipEmptyParts - because sometimes last entry will remain empty
    QStringList list = str.split("\n");
    QString yr_str;
    if(list.size() > 2)
    {
        QString plugin_name = list.at(2);

        plugin_name.remove(0,plugin_name.indexOf("-")+1);
        plugin_name = plugin_name.trimmed();

        current_barset_participant = plugin_name;

        yr_str = list.at(1);
        yr_str.remove(0,yr_str.indexOf("-")+1);
        yr_str = yr_str.trimmed();


        current_barset_category_str = yr_str;
    }

    slot_act_show_data_of_current_barset_triggered();

    QString tabname = tabwidget_redefined_result->tabText(tabwidget_redefined_result->currentIndex());
    qmap_barset_participants_str.insert(tabname,current_barset_participant);
    qmap_barset_catgories_str.insert(tabname,current_barset_category_str);
    qmap_configured_account_str.insert(tabname,selected_configured_account_name);
    qmap_bar_graph_radiobutton_type_str.insert(tabname,get_graph_type());
    qmap_bool_save_data_other_than_pie_and_visit_count_bool.insert(tabname , true);

}

void redefined_result_full_loader::slot_barset_pressed(int index)
{
    ///For get current clicked data on right click on barset

    m_barset* barset = dynamic_cast<m_barset*> (sender());

    QMap<int,QString>map_barset = barset->get_tooltip();

    QString str = map_barset.value(index);



    ///clicked domain extract for visit count in Browser History
    current_clicked_domain.clear();
    if(!str.contains("\n"))
    {
        current_clicked_domain = str;
        qmap_domians_str.insert(str,current_clicked_domain);
        qmap_bool_save_data_other_than_pie_and_visit_count_bool.insert(str , false);
        return;
    }
    ///--------------

    current_barset_category_str.clear();
    current_barset_participant.clear();

    ////do not use skip Qt::SkipEmptyParts - because sometimes last entry will remain empty
    QStringList list = str.split("\n");
    QString yr_str;
    if(list.size() > 2)
    {
        QString plugin_name = list.at(2);

        plugin_name.remove(0,plugin_name.indexOf("-")+1);
        plugin_name = plugin_name.trimmed();

        current_barset_participant = plugin_name;

        yr_str = list.at(1);
        yr_str.remove(0,yr_str.indexOf("-")+1);
        yr_str = yr_str.trimmed();

        current_barset_category_str = yr_str;
        qmap_barset_participants_str.insert(yr_str,current_barset_participant);
        qmap_barset_catgories_str.insert(yr_str,current_barset_category_str);
        qmap_configured_account_str.insert(yr_str,selected_configured_account_name);
        qmap_bar_graph_radiobutton_type_str.insert(yr_str,get_graph_type());
        qmap_bool_save_data_other_than_pie_and_visit_count_bool.insert(yr_str , true);
    }




}

void redefined_result_full_loader::slot_act_show_data_of_current_barset_triggered()
{

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    bool_to_save_all_data_with_different_colour_barset = false;


    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    tab_saved_index = tabwidget_redefined_result->currentIndex();

    if(tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_pie_or_visit_count_graph)///work only for 3rd tab of both(Messanger and Browser history)
    {
        QString tab_name;
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
            tab_name = QString(current_barset_participant);
        else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
            tab_name = QString(current_clicked_domain);

        for(int i = 0; i < tabwidget_redefined_result->count(); i++)
        {
            if(tabwidget_redefined_result->tabText(i) == tab_name)
            {
                m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_redefined_result->widget(i));
                tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));
                display_loading_progress_bar_obj->hide();
                return;
            }
        }
        new_tab_name = tab_name;
        set_graphs_data_tablewidget_header();

        tabwidget_redefined_result->addTab(m_tablewidget_obj, tab_name);
        display_data_of_graph_in_tablewidget();

        tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));

    }
    else
    {
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR && !current_barset_participant.trimmed().isEmpty())
        {
            QString tab_name = QString(current_barset_participant + "-" + current_barset_category_str);

            for(int i = 0; i < tabwidget_redefined_result->count(); i++)
            {
                if(tabwidget_redefined_result->tabText(i) == tab_name)
                {
                    m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_redefined_result->widget(i));
                    tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));

                    display_loading_progress_bar_obj->hide();
                    return;
                }
            }
            new_tab_name = tab_name;
            set_graphs_data_tablewidget_header();

            tabwidget_redefined_result->addTab(m_tablewidget_obj, tab_name);

            display_data_of_graph_in_tablewidget();


        }
        else
        {
            for(int i = 0; i < tabwidget_redefined_result->count(); i++)
            {
                if(tabwidget_redefined_result->tabText(i) == current_barset_category_str)
                {
                    m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_redefined_result->widget(i));
                    tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));

                    display_loading_progress_bar_obj->hide();
                    return;
                }
            }
            new_tab_name = current_barset_category_str;
            set_graphs_data_tablewidget_header();

            tabwidget_redefined_result->addTab(m_tablewidget_obj, current_barset_category_str);

            display_data_of_graph_in_tablewidget();

        }
        tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));
    }



    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));
    connect(m_tablewidget_obj, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_main_table_widget_item_clicked(QTableWidgetItem*)));

    display_loading_progress_bar_obj->hide();

}

void redefined_result_full_loader::slot_act_show_all_data_triggered()
{
    if(current_barset_category_str.trimmed().isEmpty())
        return;

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    bool_to_save_all_data_with_different_colour_barset = true;


    tab_saved_index = tabwidget_redefined_result->currentIndex();

    for(int i = 0; i < tabwidget_redefined_result->count(); i++)
    {
        if(tabwidget_redefined_result->tabText(i) == current_barset_category_str)
        {
            m_tablewidget_obj = dynamic_cast <m_tablewidget*>(tabwidget_redefined_result->widget(i));
            tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));
            display_loading_progress_bar_obj->hide();
            return;
        }
    }


    m_tablewidget_obj = new m_tablewidget(this);
    m_tablewidget_obj->horizontalHeader()->setStretchLastSection(true);
    m_tablewidget_obj->setSelectionMode(QAbstractItemView::ExtendedSelection);

    tabwidget_redefined_result->addTab(m_tablewidget_obj, current_barset_category_str);

    new_tab_name = current_barset_category_str;

    set_graphs_data_tablewidget_header();

    current_barset_participant.clear();
    QMap<QString, QString>::iterator iter;
    for(iter = map_of_partipants_with_color.begin() ; iter != map_of_partipants_with_color.end(); iter++)
    {
        if(bool_cancel_loading)
        {
            break;
        }
        current_barset_participant = iter.key();

        qmap_barset_participants_str.insert(current_barset_participant,current_barset_participant);
        qmap_bar_graph_radiobutton_type_str.insert(current_barset_participant,get_graph_type());
        qmap_bool_save_data_other_than_pie_and_visit_count_bool.insert(current_barset_participant , true);

        display_data_of_graph_in_tablewidget();
    }

    tabwidget_redefined_result->setCurrentIndex(tabwidget_redefined_result->indexOf(m_tablewidget_obj));

    connect(m_tablewidget_obj,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tablewidget_cell_clicked(int,int)));
    connect(m_tablewidget_obj,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_tableWidget_doubleClicked(int,int)));
    connect(m_tablewidget_obj, SIGNAL(signal_bookmarked_key_pressed()), this, SLOT(slot_bookmark_key_pressed_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_arrow_key_pressed()), this, SLOT(slot_arrow_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(signal_control_e_key_pressed()), this, SLOT(slot_control_e_key_pressed_on_tablewidget()));
    connect(m_tablewidget_obj, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(slot_tablewidget_current_cell_changed(int,int,int,int)));
    connect((QObject*)m_tablewidget_obj->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sort_tablewidget_display(int)));


    display_loading_progress_bar_obj->hide();

}

void redefined_result_full_loader::slot_pushbutton_save_clicked(bool value)
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    int current_tab_index = tabwidget_redefined_result->currentIndex();
    QString current_tab_name = tabwidget_redefined_result->tabText(current_tab_index);

    save_result_dialog_box_obj->pub_set_name(current_tab_name + "-" + QDateTime::currentDateTime().toString(("yyyy-MMM-ddThh-mm-ss")));


    save_result_dialog_box_obj->show();
    recon_static_functions::app_debug("-Ends- " , Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_pushbutton_close_tab_clicked(bool value)
{
    QString current_tab_name = tabwidget_redefined_result->tabText(tabwidget_redefined_result->currentIndex());

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        if(tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_Events || tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_stacked_graph || tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_pie_or_visit_count_graph)
            return;

        if(tab_custom_header_widgets_for_redefine_result_Messenger_QMap.contains(current_tab_name))
            tab_custom_header_widgets_for_redefine_result_Messenger_QMap.remove(current_tab_name);
    }
    else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        if(tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_Events || tabwidget_redefined_result->currentIndex() == enum_redefined_result_tab_stacked_graph)
            return;

        if(tab_custom_header_widgets_for_redefine_result_browser_history_QMap.contains(current_tab_name))
            tab_custom_header_widgets_for_redefine_result_browser_history_QMap.remove(current_tab_name);
    }

    tabwidget_redefined_result->removeTab(tabwidget_redefined_result->currentIndex());

    tabwidget_redefined_result->setCurrentIndex(tab_saved_index);
    slot_tabbar_clicked(tab_saved_index);


}

void redefined_result_full_loader::slot_link_label_accounts_list(QString text)
{
    if(text == "Show")
    {
        label_account_configure_list_link->setText("<a href = "+ QString("Hide") + " > " + QString("Accounts") + " </a>");
        tablewidget_accounts_configure_list->show();
    }
    else if(text == "Hide")
    {
        label_account_configure_list_link->setText("<a href = "+ QString("Show") + " > " + QString("Accounts") + " </a>");
        tablewidget_accounts_configure_list->hide();
    }
}

void redefined_result_full_loader::slot_link_label_participants_list(QString text)
{
    if(text == "Show")
    {
        label_participants_list_link->setText("<a href = "+ QString("Hide") + " > " + QString("Participants") + " </a>");
        tablewidget_participants_list->show();
    }
    else if(text == "Hide")
    {
        label_participants_list_link->setText("<a href = "+ QString("Show") + " > " + QString("Participants") + " </a>");
        tablewidget_participants_list->hide();
    }
}

void redefined_result_full_loader::slot_tablewidget_accounts_configures_item_clicked(int row, int coloum)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    tablewidget_accounts_configure_list->selectRow(row);
    selected_configured_account_name  =  tablewidget_accounts_configure_list->item(row,coloum)->text().trimmed();


    tablewidget_participants_list->setRowCount(0);

    tablewidget_participants_list->setColumnCount(1);
    tablewidget_participants_list->verticalHeader()->hide();
    QStringList header_receiver;
    header_receiver << "Participants";
    tablewidget_participants_list->setHorizontalHeaderLabels(header_receiver);



    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(QString(Macro_Redefined_Result_Messenger_All_Participants));

    tablewidget_participants_list->insertRow(0);
    tablewidget_participants_list->setRowHeight(0,21);
    tablewidget_participants_list->setItem(0,0,item);

    tablewidget_participants_list->selectRow(0);


    QStringList listofcolors = get_colors_stringlist_from_db();
    QStringList listof_all_participants = map_of_accounts_with_particiapnts.value(selected_configured_account_name);

    int j = 1;
    for(int i = 0; i < listof_all_participants.size() ; i++)
    {

        if(listof_all_participants.at(i).trimmed().isEmpty())
            continue;

        QString name = listof_all_participants.at(i);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        item->setText(name);

        QString color_name;
        if(listofcolors.size() > j)
        {
            color_name = listofcolors.at(j-1);
            QString icon_path = "../icons/Features/" + QString(MACRO_Plugin_Name_Redefined_Result).replace(" ","_") + "/" + color_name.toLower().replace(" ","_") + ".png";

            item->setIcon(QIcon(icon_path));
        }

        item->setToolTip(color_name);
        map_of_partipants_with_color.insert(name,color_name);
        tablewidget_participants_list->insertRow(j);
        tablewidget_participants_list->setRowHeight(j,21);
        tablewidget_participants_list->setItem(j,0,item);

        j++;
    }

    create_piechart_for_selected_account_for_messenger();
    create_graphs_messenger();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_tablewidget_participants_list_cell_single_clicked(int row, int coloum)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    map_of_partipants_with_color.clear();
    QString participant_name =  tablewidget_participants_list->currentItem()->text();

    if(participant_name != QString(Macro_Redefined_Result_Messenger_All_Participants))
    {
        map_of_partipants_with_color.insert(participant_name,tablewidget_participants_list->item(row,coloum)->toolTip());
    }else
    {
        for(int i = 0; i < tablewidget_participants_list->rowCount() ; i++)
        {
            QString name = tablewidget_participants_list->item(i,coloum)->text();
            if(name == QString(Macro_Redefined_Result_Messenger_All_Participants))
                continue;

            QString color = tablewidget_participants_list->item(i,coloum)->toolTip();

            map_of_partipants_with_color.insert(name,color);
        }

    }

    create_graphs_messenger();
    recon_static_functions::app_debug("Ends",Q_FUNC_INFO);
}

void redefined_result_full_loader::slot_hovered_toolTip_on_pieslices(QPieSlice *slice, bool status)
{
    QChart *chart = qobject_cast<QChart *>(slice->parent()->parent()->parent());

    if(status)
    {
        chart->setToolTip(slice->label());
    }
    else
    {
        chart->setToolTip("");
    }
}

void redefined_result_full_loader::slot_tabbar_clicked(int index)
{

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        if(index == enum_redefined_result_tab_Events)
        {
            m_tablewidget_obj = dynamic_cast<m_tablewidget*> (tabwidget_redefined_result->widget(index));

            pushbutton_timeline->setHidden(false);
            checkbox_timeline->setHidden(false);
            label_account_configure_list_link->setHidden(true);
            label_participants_list_link->setHidden(true);

            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);

            tablewidget_participants_list->setHidden(true);
            tablewidget_accounts_configure_list->setHidden(true);

            pushButton_close_tab->setHidden(true);

            pushButton_save->setHidden(true);


        }
        else if(index == enum_redefined_result_tab_stacked_graph)
        {
            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);


            label_account_configure_list_link->setHidden(false);
            label_participants_list_link->setHidden(false);
            if(tablewidget_accounts_configure_list->rowCount() > 0)
                tablewidget_accounts_configure_list->setRowHidden(0,false);

            pushbutton_timeline->setHidden(true);
            checkbox_timeline->setHidden(true);
            pushButton_close_tab->setHidden(true);


            pushButton_save->setHidden(true);


        }
        else if(index == enum_redefined_result_tab_pie_or_visit_count_graph)
        {
            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);

            label_account_configure_list_link->setHidden(false);
            label_participants_list_link->setHidden(true);
            tablewidget_participants_list->setHidden(true);
            pushbutton_timeline->setHidden(true);
            checkbox_timeline->setHidden(true);

            tablewidget_accounts_configure_list->setHidden(false);
            if(tablewidget_accounts_configure_list->rowCount() > 0)
                tablewidget_accounts_configure_list->setRowHidden(0,true);

            pushButton_close_tab->setHidden(true);
            pushButton_save->setHidden(true);

        }
        else
        {
            m_tablewidget_obj = dynamic_cast<m_tablewidget*> (tabwidget_redefined_result->widget(index));

            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);

            label_account_configure_list_link->setHidden(true);
            label_participants_list_link->setHidden(true);

            tablewidget_participants_list->setHidden(true);
            tablewidget_accounts_configure_list->setHidden(true);

            pushbutton_timeline->setHidden(true);
            checkbox_timeline->setHidden(true);
            pushButton_close_tab->setHidden(false);

            pushButton_save->setHidden(false);



        }

    }
    else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        if(index == enum_redefined_result_tab_Events)
        {
            m_tablewidget_obj = dynamic_cast<m_tablewidget*> (tabwidget_redefined_result->widget(index));

            pushbutton_timeline->setHidden(false);
            checkbox_timeline->setHidden(false);

            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);

            pushButton_close_tab->setHidden(true);
            pushButton_save->setHidden(true);


        }
        else if(index == enum_redefined_result_tab_stacked_graph)
        {
            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);
            pushbutton_timeline->setHidden(true);

            checkbox_timeline->setHidden(true);

            pushButton_close_tab->setHidden(true);
            pushButton_save->setHidden(true);

        }
        else if(index == enum_redefined_result_tab_pie_or_visit_count_graph)
        {
            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);
            pushbutton_timeline->setHidden(true);
            checkbox_timeline->setHidden(true);
            pushButton_close_tab->setHidden(true);
            pushButton_save->setHidden(true);

        }
        else
        {
            m_tablewidget_obj = dynamic_cast<m_tablewidget*> (tabwidget_redefined_result->widget(index));

            lineEdit_search->setHidden(true);
            pushbutton_show_all->setHidden(true);
            pushButton_search->setHidden(true);

            pushbutton_timeline->setHidden(true);
            checkbox_timeline->setHidden(true);
            pushButton_close_tab->setHidden(false);
            pushButton_save->setHidden(false);

        }

    }

    ////when accounts or participants list make show on tab click then graphs window get resized and because of this 'tabs shifted from their position'.
    /// so this code is done to show exact tab's content.
    if(index >= 0)
        tabwidget_redefined_result->setCurrentIndex(index);

}

void redefined_result_full_loader::slot_pushButton_timeline_clicked()
{
    timeline_selection_window_obj->show();
}

void redefined_result_full_loader::slot_checkbox_timeline_toggled(bool checked)
{
    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->pub_set_hidden_cancel_button(true);
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->show();

    pushbutton_timeline->setEnabled(checked);
    if(checked)
    {
        redefined_result_populate_data_in_table();
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            emit tablewidget_accounts_configure_list->cellClicked(0,0);

            create_graphs_messenger();
        }
        else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        {
            set_graphs_essentials_for_data_only();
            create_graphs_browsers_history();
        }
        display_loading_progress_bar_obj->hide();

    }
    else
    {
       slot_pushbutton_showall_clicked();
    }

    display_loading_progress_bar_obj->hide();
    slot_custom_table_header_widget_search_field_return_pressed();

}

void redefined_result_full_loader::slot_time_line(qint64 start_time , qint64 end_time)
{
    start_timestamp_int =  global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(start_time);
    end_timestamp_int =  global_recon_helper_singular_class_obj->convert_UTC_numeric_to_case_timezone_numeric(end_time);


    timeline_selection_window_obj->hide();


    redefined_result_populate_data_in_table();

    if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {
        emit tablewidget_accounts_configure_list->cellClicked(0,0);

        create_graphs_messenger();
    }
    else if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        set_graphs_essentials_for_data_only();
        create_graphs_browsers_history();
    }

    slot_custom_table_header_widget_search_field_return_pressed();
}


void redefined_result_full_loader::slot_act_remove_tags()
{
    QModelIndexList selection_model_list = m_tablewidget_obj->selectionModel()->selectedRows();
    int tag_column_index = display_column_data_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString,0);
    if(tag_column_index == -1)
    {
        return;
    }

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();


    QStringList INT_List;

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

        INT_List << m_tablewidget_obj->item(selected_row_no, record_no_index)->text();


        QString icon_path =  "";
        QIcon icon(icon_path);
        m_tablewidget_obj->item(selected_row_no,tag_column_index)->setIcon(icon);

    }

    global_connection_manager_obj->pub_remove_tag_in_redefined_db(INT_List, plugin_name);
    global_connection_manager_obj->pub_delete_tag_for_redefined(INT_List, plugin_name);

    display_loading_progress_bar_non_cancelable_obj->hide();
}




void redefined_result_full_loader::slot_save_result_name_value(QString save_name)
{
    save_result_dialog_box_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString saved_results_db_path ;

    save_name.replace(" ","_");
    save_name.replace("/","_");

    QString new_name;
    if(plugin_name == QString(MACRO_Plugin_Name_Browser_History_Full_RR))
    {
        new_name = recon_static_functions::get_available_filename(save_name + ".sqlite" ,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() , Q_FUNC_INFO );
        saved_results_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Browser_History_In_Result_QString).toString() + new_name;
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Messenger_Full_RR))
    {
        new_name = recon_static_functions::get_available_filename(save_name + ".sqlite" ,global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() , Q_FUNC_INFO );
        saved_results_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Redefined_Results_Saved_Messenger_In_Result_QString).toString() + new_name;
    }

    new_name.remove(".sqlite");
    save_redefined_result_data_in_case_treewidget(new_name);

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
            break ;

    }
    recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

    if(bool_message_box_yes_button_clicked)
    {
        emit signal_display_data_for_redefined_results(MACRO_CASE_TREE_Redefined_Result_Saved_Parent , new_name , saved_results_db_path , plugin_name);
    }

}


void redefined_result_full_loader::slot_save_graph_name_value(QString graph_name)
{
    save_graph_dialog_box_obj->hide();

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please Wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    QString tab_text;
    QString plugin_name_for_saved_graphs;
    QString saved_graph_image, filename ,saved_graph_db_path  ;

    graph_name = graph_name.trimmed();

    if(graph_name.contains(" "))
        graph_name.replace(" ","_");

    if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
    {
        tab_text = MACRO_Plugin_Saved_Graphs_Browser_History_TAB_Name_Browser_History_Graphs ;
        plugin_name_for_saved_graphs = MACRO_Plugin_Name_Saved_Graphs_Browser_History;
        graph_name.replace(" ","_");
        saved_graph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
        QString saved_graph_path_browser = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_Images_Directory_In_Result_QString).toString();
        saved_graph_image  = graph_name + ".png" ;

        filename = recon_static_functions::get_available_filename(saved_graph_image,saved_graph_path_browser,Q_FUNC_INFO);

        if(saved_graph_path_browser.endsWith("/"))
            saved_graph_image = saved_graph_path_browser + filename;
        else
            saved_graph_image = saved_graph_path_browser + QString("/") + filename;

    }
    else if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
    {

        tab_text = MACRO_Plugin_Saved_Graphs_Messenger_TAB_Name_Messenger_Graphs ;
        plugin_name_for_saved_graphs = MACRO_Plugin_Name_Saved_Graphs_Messenger;
        graph_name.replace(" ","_");
        saved_graph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
        QString saved_graph_path_messenger = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_Images_Directory_In_Result_QString).toString();

        saved_graph_image  = graph_name + ".png" ;

        filename = recon_static_functions::get_available_filename(saved_graph_image,saved_graph_path_messenger,Q_FUNC_INFO);

        if(saved_graph_path_messenger.endsWith("/"))
            saved_graph_image = saved_graph_path_messenger + filename;
        else
            saved_graph_image = saved_graph_path_messenger + QString("/") + filename;

    }

    int current_index =  tabwidget_redefined_result->currentIndex();

    if(current_index == 1)
    {
        QChartView *tmp_chartview =  (QChartView *)stackwidget_obj->currentWidget();
        if(tmp_chartview != NULL)
        {
            const auto dpr =  tmp_chartview->devicePixelRatioF();

            QPixmap pmap(tmp_chartview->width() * dpr , tmp_chartview->height() * dpr);
            pmap.setDevicePixelRatio(dpr);
            pmap.fill(Qt::transparent);

            QPainter *paint = new QPainter(&pmap);
            paint->setPen(*(new QColor(255,34,255,255)));
            tmp_chartview->render(paint);

            pmap.save(saved_graph_image , "PNG");
            delete paint;

            insert_values_into_save_graph_db(filename , saved_graph_db_path ,saved_graph_image);
        }

    }
    else if(current_index == 2)
    {

        const auto dpr =  chartView_for_pie_or_visit_count_obj->devicePixelRatioF();

        QPixmap pixmap(chartView_for_pie_or_visit_count_obj->width() * dpr , chartView_for_pie_or_visit_count_obj->height() * dpr);
        pixmap.setDevicePixelRatio(dpr);
        pixmap.fill(Qt::transparent);

        QPainter *paint = new QPainter(&pixmap);
        paint->setPen(*(new QColor(255,34,255,255)));
        chartView_for_pie_or_visit_count_obj->render(paint);

        pixmap.save(saved_graph_image , "PNG");
        delete paint;


        insert_values_into_save_graph_db(filename , saved_graph_db_path ,saved_graph_image);

    }

    display_loading_progress_bar_non_cancelable_obj->hide();

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
        emit signal_display_data_for_redefined_results(MACRO_CASE_TREE_Graph_Result_Saved , tab_text , saved_graph_db_path , plugin_name_for_saved_graphs);
    }

}

void redefined_result_full_loader::slot_custom_table_header_widget_search_field_return_pressed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int tab_index = tabwidget_redefined_result->currentIndex();
    QString current_tab_name = tabwidget_redefined_result->tabText(tab_index);

    QList<custom_table_header_widget *> custom_header_widgets_QList;

    if(feature_name == MACRO_CASE_TREE_Redefined_Result_Parent)
    {
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            if(tab_custom_header_widgets_for_redefine_result_Messenger_QMap.isEmpty())
                return;

            custom_header_widgets_QList = tab_custom_header_widgets_for_redefine_result_Messenger_QMap.value(current_tab_name);
        }

        if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        {
            if(tab_custom_header_widgets_for_redefine_result_browser_history_QMap.isEmpty())
                return;

            custom_header_widgets_QList = tab_custom_header_widgets_for_redefine_result_browser_history_QMap.value(current_tab_name);
        }
    }

    bool_stop_sorting_tags_bookmark_notes_after_search = false;

    QList<struct_search_column_text_for_file_type> search_text_col_QList;

    for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
    {
        custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

        if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
        {
            struct_search_column_text_for_file_type struct_search_column_text_for_file_type_obj;

            struct_search_column_text_for_file_type_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
            struct_search_column_text_for_file_type_obj.column_number = i;

            search_text_col_QList.append(struct_search_column_text_for_file_type_obj);
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

void redefined_result_full_loader::slot_custom_table_header_widget_search_field_text_changed()
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    slot_custom_table_header_widget_search_field_return_pressed();

    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);

    return;
}

void redefined_result_full_loader::slot_custom_table_header_widget_sorting_clicked(int column, int column_sort_order)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    int tab_index = tabwidget_redefined_result->currentIndex();
    QString current_tab_name = tabwidget_redefined_result->tabText(tab_index);

    QList<custom_table_header_widget *> custom_header_widgets_QList;

    if(feature_name == MACRO_CASE_TREE_Redefined_Result_Parent)
    {
        if(plugin_name == MACRO_Plugin_Name_Messenger_Full_RR)
        {
            if(tab_custom_header_widgets_for_redefine_result_Messenger_QMap.isEmpty())
                return;

            custom_header_widgets_QList = tab_custom_header_widgets_for_redefine_result_Messenger_QMap.value(current_tab_name);
        }

        if(plugin_name == MACRO_Plugin_Name_Browser_History_Full_RR)
        {
            if(tab_custom_header_widgets_for_redefine_result_browser_history_QMap.isEmpty())
                return;

            custom_header_widgets_QList = tab_custom_header_widgets_for_redefine_result_browser_history_QMap.value(current_tab_name);
        }
    }

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

void redefined_result_full_loader::all_column_text_search(QList<struct_search_column_text_for_file_type> search_text_all_column_list)
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
            struct_search_column_text_for_file_type struct_search_column_text_for_file_type_obj = search_text_all_column_list.at(column_count);

            if(!(m_tablewidget_obj->item(row, struct_search_column_text_for_file_type_obj.column_number)->text().trimmed().contains(struct_search_column_text_for_file_type_obj.search_text, Qt::CaseInsensitive)))
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
