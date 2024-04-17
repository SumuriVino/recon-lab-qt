#include "plugin_viewer.h"
#include "ui_plugin_viewer.h"



void plugin_viewer::on_pushButton_generate_report_clicked()
{

    bool_cancel_loading_reports = false;

    cancel_exporter_loop = false;

    if(current_tab_index_only_for_tableview == MACRO_FAIL_VALUE_NUMERIC_int)
        return;

    QString report_scope;
    QString report_type;


    report_scope = ui->comboBox_report_content_selection->currentText().trimmed();

    if(ui->comboBox_report_type_selection->currentText() == QString("HTML"))
        report_type = MACRO_REPORT_TYPE_HTML_STANDARD_QString;
    else
        report_type = ui->comboBox_report_type_selection->currentText().trimmed();



    if(report_scope == MACRO_REPORT_SCOPE_FULL_QString || report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
    {
        int index = -1;

        if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || report_type == MACRO_REPORT_TYPE_PDF_QString)
        {
            index = table_html_report_available_list_tablewidget.indexOf("1",0);
            if(index == -1)
            {
                index = html_report_available_list_textedit.indexOf("1",0);
            }
            if(index == -1)
            {
                index = html_report_available_list_stack.indexOf("1",0);
            }

        }
        else if(report_type == MACRO_REPORT_TYPE_XML_QString)
            index = table_xml_report_available_list_tablewidget.indexOf("1",0);
        else if(report_type == MACRO_REPORT_TYPE_CSV_QString)
            index = table_csv_report_available_list_tablewidget.indexOf("1",0);
        else if(report_type == MACRO_REPORT_TYPE_KML_QString)
            index = table_kml_report_available_list_tablewidget.indexOf("1",0);

        if(index == -1)
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING,"This report is not available for " + get_plugin_name());
            message_dialog_object->show();

            return;
        }
    }

    if(report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
    {
        int tab_index = current_tab_index_only_for_tableview;

        // we support screen item report of only tablewidget,
        // so returning from here if tab widget index is out of max tablewidget count
        //        QString obj_name = ui->tabWidget->widget(tab_index)->objectName();
        //        obj_name.remove("tab_");
        //        int tab_index_actual = obj_name.toInt();
        if(list_tablewidgets.size() <= 0 || tab_index > (list_tablewidgets.size() - 1))
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING, QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString) + " report is not available for this category. ");
            message_dialog_object->show();

            return;
        }


        QString is_report_available;

        if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || MACRO_REPORT_TYPE_PDF_QString)
            is_report_available = table_html_report_available_list_tablewidget.at(tab_index);
        else if(report_type == MACRO_REPORT_TYPE_XML_QString)
            is_report_available = table_xml_report_available_list_tablewidget.at(tab_index);
        else if(report_type == MACRO_REPORT_TYPE_CSV_QString)
            is_report_available = table_csv_report_available_list_tablewidget.at(tab_index);
        else if(report_type == MACRO_REPORT_TYPE_KML_QString)
            is_report_available = table_kml_report_available_list_tablewidget.at(tab_index);

        if(is_report_available != QString("1"))
        {
            message_dialog_object->pub_set_message(MACRO_MSG_WARNING, QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString) + " report is not available for this plugin.");
            message_dialog_object->show();
            return;
        }
    }


    disable_action_widgets(true);

    QString mm_type = report_type;

    QDir dir;
    QString mm_scope = report_scope;
    QString report_currently_dir_name = get_plugin_name().replace(" ","_") + "_" + mm_type.replace(" ","_") +"_" + mm_scope.replace(" ","_") + "_" + QDateTime::currentDateTime().toString("yyyy-MMM-ddThh-mm-ss");
    QString reports_dir_path = reports_location_path + report_currently_dir_name + "/";
    dir.mkpath(reports_dir_path);

    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString || report_type == MACRO_REPORT_TYPE_PDF_QString)
    {

        QString new_resource_path = reports_dir_path + "resources";
        QString new_resource_path_plugin = reports_dir_path + "resources/plugins";
        dir.mkpath(new_resource_path);
        dir.mkpath(new_resource_path_plugin);


        QFile file;

        //file.copy("../icons/agency_logo.png", new_resource_path + "/agency_logo.png");
        QString agency_logo_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "agency_logo.png";
        file.copy(agency_logo_path, new_resource_path + "/agency_logo.png");

        file.copy("../Resources/graphics/html_reports/css/chat_template.css", new_resource_path + "/chat_template.css");
        file.copy("../Resources/graphics/html_reports/style.css", new_resource_path + "/style.css");
        file.copy("../icons/open_file_link.png", new_resource_path + "/open_file_link.png");
        file.copy("../icons/person1.png", new_resource_path + "/person1.png");
        file.copy("../icons/webicon.png", new_resource_path + "/webicon.png");
        file.copy("../icons/appicon.png", new_resource_path + "/appicon.png");
        file.copy((("../icons/plugins/") + get_plugin_name().replace(" ","_") + ".png"), new_resource_path + "/plugins/" + (get_plugin_name().replace(" ","_") + ".png"));

        global_narad_muni_class_obj->set_field(MACRO_NARAD_Case_Agency_Logo_QString,(new_resource_path + "/agency_logo.png"));

    }

    //======================================== Start =========================================//
    recon_static_functions::app_debug(" updating narad muni and passing to report class START", Q_FUNC_INFO);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Tabwidget_CurrentIndex_int,current_tab_index_only_for_tableview);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Checkbox_Export_bool,ui->checkBox_export->isChecked());

    QList<QVariant> talwidget_list;
    for(int i=0; i< list_tablewidgets.size(); i++)
    {
        qint64 barset = (qint64)(list_tablewidgets.at(i));
        talwidget_list.append(barset);
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList,talwidget_list);
    recon_static_functions::app_debug(" updating narad muni and passing to report class END", Q_FUNC_INFO);
    //======================================== End =========================================//




    //======================================== start plugin info =========================================//

    QStringList tabs_list;
    tabs_list << tab_name_list_tablewidget;
    tabs_list << tab_name_list_textedit;
    tabs_list << tab_name_list_stack;
    tabs_list << tab_name_list_chat_viewer;

    if(report_scope == QString(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString))
    {
        tabs_list.clear();
        int mi_tab = current_tab_index_only_for_tableview;
        if(tab_name_list_tablewidget.size() > mi_tab)
        {
            QString current_tab_name = tab_name_list_tablewidget.at(mi_tab);
            tabs_list << current_tab_name;
        }
    }







    QString tabs_info;
    for(int tt = 0; tt < tabs_list.size(); tt++)
    {
        QString tb_nm_str = tabs_list.at(tt);

        if(global_csv_reader_class_selected_obj->is_tab_type_chat_view(get_plugin_name(),tb_nm_str))
        {
            tb_nm_str.prepend(MACRO_RECON_Splitter_2_hash_in_brace).append(MACRO_RECON_Splitter_3_dollar_in_brace).append("0");
        }
        else
        {
            if(ui->checkBox_export->isChecked() && global_csv_reader_class_selected_obj->is_export_available_in_tab(tb_nm_str, get_plugin_name()))
                tb_nm_str.prepend(MACRO_RECON_Splitter_2_hash_in_brace).append(MACRO_RECON_Splitter_3_dollar_in_brace).append("1");
            else
                tb_nm_str.prepend(MACRO_RECON_Splitter_2_hash_in_brace).append(MACRO_RECON_Splitter_3_dollar_in_brace).append("0");
        }

        tabs_info.append(tb_nm_str);
    }
    if(tabs_info.startsWith(MACRO_RECON_Splitter_2_hash_in_brace))
        tabs_info.remove(0,QString(MACRO_RECON_Splitter_2_hash_in_brace).size());







    QStringList plugin_info_list;
    plugin_info_list << QString(get_plugin_name() + MACRO_RECON_Splitter_1_multiple + tabs_info);



    //======================================== end plugin info =========================================//


    QString report_file_path = reports_dir_path + report_currently_dir_name;
    if(report_type == MACRO_REPORT_TYPE_HTML_STANDARD_QString)
    {
        report_file_path.append(".html");
    }
    else if(report_type == MACRO_REPORT_TYPE_PDF_QString)
    {
        report_file_path.append(".pdf");
    }
    else if(report_type == MACRO_REPORT_TYPE_CSV_QString)
    {
        report_file_path.append(".csv");
    }
    else if(report_type == MACRO_REPORT_TYPE_XML_QString)
    {
        report_file_path.append(".xml");
    }
    else if(report_type == MACRO_REPORT_TYPE_KML_QString)
    {
        report_file_path.append(".kml");
    }


    QStringList selected_tags_list = recon_helper_standard_obj->extract_all_tags_from_db(0, QStringList(), destination_db_file, Q_FUNC_INFO);
    QString chat_bk_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
    qint64 bookmarked_chat_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true,chat_bk_db_path,QStringList("chat_viewer_info"),Q_FUNC_INFO);
    if(bookmarked_chat_record_count != 0)
    {
        if(!selected_tags_list.contains(MACRO_Tag_Name_Bookmarks))
            selected_tags_list.append(MACRO_Tag_Name_Bookmarks);
    }




    struct_global_global_report_info rpt_obj;
    rpt_obj.report_maker = MACRO_REPORT_MAKER_PLUGIN_int;
    rpt_obj.report_name = "MyreportName";
    rpt_obj.report_type_list << report_type;
    rpt_obj.report_scope = report_scope;
    rpt_obj.report_path_list << report_file_path;
    rpt_obj.selected_tags_list << selected_tags_list;
    rpt_obj.report_plugin_info_list << plugin_info_list;
    rpt_obj.narad_muni_report_obj = global_narad_muni_class_obj;

    thread_report_builder_obj->pub_set_essentials(rpt_obj);

    display_loading_progress_bar_for_reports_from_plugin_obj->pub_set_progress_bar_title("REPORTS");
    display_loading_progress_bar_for_reports_from_plugin_obj->pub_set_label_messsge("Building Reports......");
    display_loading_progress_bar_for_reports_from_plugin_obj->show();
    display_loading_progress_bar_for_reports_from_plugin_obj->pub_set_progress_bar_indefinite_value();


    thread_report_builder_obj->start();

    disable_action_widgets(false);
}


qint64 plugin_viewer::get_total_record_of_db_according_search(QStringList search_string_list)
{
    qint64 total_record = 0;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_db_file);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("---FAILED--- to open DB" + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + destination_db.lastError().text(),Q_FUNC_INFO);
        return total_record;
    }

    QSqlQuery query_select(destination_db);

    for(int  i = 0 ; i < tab_name_list_tablewidget.size() ; i++)
    {

        QString table_name = table_name_list_tablewidget.at(i);
        QStringList search_cols_list = ms_search_local_list_tablewidget.at(i);


        QString partial_command = recon_static_functions::get_partial_command_forsearch(search_cols_list,search_string_list, Q_FUNC_INFO);
        QString command = "select count(*) from " + table_name + partial_command;

        query_select.prepare(command);
        for(int k = 0 ; k < search_cols_list.size() ; k++)
        {
            for(int i = 0; i < search_string_list.size(); i++)
            {
                query_select.addBindValue(search_string_list.at(i));
            }
        }

        if(!query_select.exec())
        {
            recon_static_functions::app_debug("---FAILED--- to exec query " + destination_db.databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + query_select.lastError().text(),Q_FUNC_INFO);
            recon_static_functions::app_debug("query " + query_select.lastQuery(),Q_FUNC_INFO);
        }
        if(query_select.next())
        {
            total_record += query_select.value(0).toLongLong();
        }

    }

    return total_record;

}

void plugin_viewer::on_pushButton_search_clicked()
{

    //    ui->textEdit_metadata->clear();
    //    ui->stackedWidget->setCurrentIndex(2);
    detached_file_previewer_module_popobj->set_current_index(2);

    disable_action_widgets(true);

    QString keyword = ui->lineEdit_search->text().trimmed();
    searched_keyword = keyword;

    QStringList search_string_list = recon_static_functions::get_search_string_list(keyword, Q_FUNC_INFO);
    if(search_string_list.size() <= 0)
    {
        disable_action_widgets(false);
        return;
    }


    total_covered_count = 0;
    total_record_count = recon_helper_standard_obj->get_db_total_record_by_dbpath(true, destination_db_file, QStringList(), Q_FUNC_INFO);

    total_record_count = get_total_record_of_db_according_search(search_string_list);


    //    progressbar_increment_step = total_record_count / 5;

    //    if(progressbar_increment_step <= 0)
    //        progressbar_increment_step = 1;



    for(int  i = 0 ; i < tab_name_list_tablewidget.size() ; i++)
    {
        if(i%10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_tablewidget.at(i);
        QString tab_name = tab_name_list_tablewidget.at(i);
        QStringList cols_nam_list =  tw_column_name_list_tablewidget.at(i);
        QStringList cols_type_list = tw_data_type_list_tablewidget.at(i);

        QStringList search_cols_list = ms_search_local_list_tablewidget.at(i);


        // adding notes because of notes visibily for display is not set, and we want to show icon of notes on_pushbutton_showall_click
        cols_nam_list.insert(cols_nam_list.size(), "notes");
        cols_type_list.insert(cols_type_list.size(), MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString);

        int timeline_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_TW_TIMELINE_QString,0);
        int notes_col_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString,0);

        int os_scheme_index = cols_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString,0);


        QString orderByValue;
        if(ui->checkBox_timeline->isChecked())
        {
            if((timeline_col_index > -1) && (cols_nam_list.size() > timeline_col_index))
            {
                orderByValue +=  " ORDER BY " + cols_nam_list.at(timeline_col_index) + " DESC";
            }
        }


        QString command = cols_nam_list.join(",");
        command = "select " + command + " from " + table_name;

        QString partial_command = recon_static_functions::get_partial_command_forsearch(search_cols_list,search_string_list, Q_FUNC_INFO);
        command = command + partial_command + orderByValue;

        QStringList cols_width_list;
        QStringList tmp_cols_width_list = tw_col_width_list_tablewidget.at(i);

        for(int jj = 0; jj < tmp_cols_width_list.size(); jj++)
        {
            if(cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_BOOKMARK_QString
                    || cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString
                    || cols_type_list.at(jj) == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {
                cols_width_list << tmp_cols_width_list.at(jj);
            }
            else
            {
                int cols_width = tmp_cols_width_list.at(jj).toInt();
                cols_width = cols_width + 25;
                cols_width_list << QString::number(cols_width);
            }
        }

//        draw_table(command , i, cols_nam_list.size(),cols_type_list, timeline_col_index, notes_col_index,os_scheme_index,cols_width_list,search_cols_list,search_string_list);

            draw_table(command , i, cols_nam_list.size(),cols_type_list, timeline_col_index, notes_col_index,os_scheme_index,cols_width_list,search_cols_list,search_string_list);
            QList<struct_plugin_search_column_text> search_text_col_QList;
            bool_sort_with_search = false;
            int tab_index = ui->tabWidget->currentIndex();

            if(tab_custom_header_widgets_QMap.size() <= tab_index)
                return;

            QList<custom_table_header_widget *> custom_header_widgets_QList;

            custom_header_widgets_QList = tab_custom_header_widgets_QMap.value(tab_index);

            for(int i = 0 ; i < custom_header_widgets_QList.size(); i++)
            {
                custom_table_header_widget *custom_table_header_widget_obj = custom_header_widgets_QList.at(i);

                if(custom_table_header_widget_obj->pub_get_search_text().trimmed().size() >= 1)
                {
                    struct_plugin_search_column_text struct_plugin_search_column_text_obj;
                    struct_plugin_search_column_text_obj.search_text = custom_table_header_widget_obj->pub_get_search_text().trimmed();
                    struct_plugin_search_column_text_obj.column_number = i;
                    search_text_col_QList.append(struct_plugin_search_column_text_obj);
                }
            }

            for(int ii = 0 ; ii < list_tablewidgets.size() ; ii++)
            {
                if(ui->tabWidget->currentIndex() == ii)
                {
                    current_table_widget = list_tablewidgets.at(ii);
                    break;
                }
            }
            all_column_text_search(search_text_col_QList);

            current_table_widget->horizontalHeader()->hide();
            current_table_widget->horizontalHeader()->show();

            emit signal_clear_all_metadata();

    }



    for(int  i = 0 ; i < tab_name_list_thumbnail.size() ; i++)
    {
        if(i % 10 == 0)
            QCoreApplication::processEvents();

        QString table_name = table_name_list_thumbnail.at(i);


        draw_thumbnail(table_name,i);

    }


    display_loading_progress_bar_obj->hide();

    emit signal_refresh_widgets();
    disable_action_widgets(false);

}


void plugin_viewer::on_pushButton_showall_clicked()
{
    bool_show_all_clicked = true;

    emit signal_refresh_widgets();

    ui->lineEdit_search->clear();

    detached_metadata_viewer_module_popobj->set_metadata_content("","");

    detached_file_previewer_module_popobj->set_window_title(get_plugin_name());
    detached_file_previewer_module_popobj->set_current_index(2);
    detached_file_previewer_module_popobj->set_image("","");

    searched_keyword.clear();
    data_prepare_metadata.clear();

    disable_action_widgets(true);

    populate_data_in_tablewidgets();
    populate_data_in_textedit();
    populate_data_in_stack();
    populate_data_in_tree();
    populate_data_in_thumbnail_viewer();

    disable_action_widgets(false);

}


void plugin_viewer::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked();
}


void plugin_viewer::on_pushButton_timeline_clicked()
{
    timeline_selection_window_obj->show();
}
