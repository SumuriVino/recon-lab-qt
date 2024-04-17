#include "html_standard_report_module.h"

html_standard_report_module::html_standard_report_module(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    report_generic_function_obj = new report_generic_function(this);
    
    export_file_obj = new export_file_module(this);

    bool_cancel_report = false;

    bool_export_file_for_pdf_report = false;

}


void html_standard_report_module::pub_cancel_report()
{
    bool_cancel_report = true;
    export_file_obj->pub_cancel_export();

}

void html_standard_report_module::set_essentials()
{
    bool_cancel_report = false;

    bool_export_file_for_pdf_report = false;

    clear_fields();
    
    fill_variable_from_narad_muni();

    report_generic_function_obj->pub_set_essentials();
    export_file_obj->set_essentials();
}

void html_standard_report_module::pub_set_bool_for_pdf_report()
{
    bool_export_file_for_pdf_report = true;
}


void html_standard_report_module::set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
    report_generic_function_obj->pub_set_narad_muni_for_report(mobj);
    export_file_obj->set_narad_muni_for_report(mobj);
}


void html_standard_report_module::clear_exportfilelist_table()
{
    export_file_obj->set_export_entry_null();
}

void html_standard_report_module::fill_variable_from_narad_muni()
{


    table_export_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList).toStringList();
    table_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList).toStringList();

    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();
    plugin_name_str     = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();


    table_html_report_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Html_Report_Available_Tablewidget_QStringList).toStringList();
    tab_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList).toStringList();
    table_name_list_textedit  = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Textedit_QStringList).toStringList();
    html_report_available_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Report_Available_Textedit_QStringList).toStringList();
    tab_name_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Textedit_QStringList).toStringList();
    table_name_list_stack  = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Stack_QStringList).toStringList();
    html_report_available_list_stack = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Report_Available_Stack_QStringList).toStringList();
    tab_name_list_stack    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Stack_QStringList).toStringList();
    parent_module_name_str = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Parent_Module_Name_QString).toString();
    table_artifact_common_list_tablewidget    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Artifact_Common_Tablewidget_QStringList).toStringList();
    table_report_html_header_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Report_Html_Header_Tablewidget_QStringList).toStringList();
    table_report_header_column_width_html_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Report_Header_Column_Width_Html_Tablewidget_QStringList).toStringList();
    table_export_criteria_list_tablewidget    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList).toStringList();

    artifact_common_list_textedit    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Artifact_Common_Textedit_QStringList).toStringList();
    report_html_header_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Header_Textedit_QStringList).toStringList();
    report_header_column_width_html_list_textedit = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Header_Column_Width_Html_Textedit_QStringList).toStringList();

    artifact_common_list_stack    = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Artifact_Common_Stack_QStringList).toStringList();
    report_html_header_list_stack = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Header_Stack_QStringList).toStringList();
    report_header_column_width_html_list_stack = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Header_Column_Width_Html_Stack_QStringList).toStringList();

    reports_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Reports_Dir_Path_QString).toString();

    destination_file_location = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_File_Location_QString).toString();


    list_m_tablewidgets.clear();
    QList<QVariant> screen_tablewidget_list_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList).toList();
    for(int i=0; i < screen_tablewidget_list_list.size(); i++)
    {
        qint64 nn = screen_tablewidget_list_list.at(i).toLongLong();
        list_m_tablewidgets.append((m_tablewidget *) nn);
    }

    rt_html_data_type_list_tablewidget.clear();
    QList<QVariant> html_data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Data_Type_Tablewidget_QList).toList();
    for(int i=0; i < html_data_type_tablewidget_list.size(); i++)
        rt_html_data_type_list_tablewidget.append(html_data_type_tablewidget_list.at(i).toStringList());

    rt_html_col_name_list_tablewidget.clear();
    QList<QVariant> html_col_name_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Col_Name_Tablewidget_QList).toList();
    for(int i=0; i < html_col_name_tablewidget_list.size(); i++)
        rt_html_col_name_list_tablewidget.append(html_col_name_tablewidget_list.at(i).toStringList());

    report_html_col_name_list_textedit.clear();
    QList<QVariant> report_html_col_name_textedit_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Col_Name_Textedit_QList).toList();
    for(int i=0; i < report_html_col_name_textedit_list.size(); i++)
        report_html_col_name_list_textedit.append(report_html_col_name_textedit_list.at(i).toStringList());

    report_html_datatype_list_textedit.clear();
    QList<QVariant> report_html_datatype_textedit_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Datatype_Textedit_QList).toList();
    for(int i=0; i < report_html_datatype_textedit_list.size(); i++)
        report_html_datatype_list_textedit.append(report_html_datatype_textedit_list.at(i).toStringList());

    report_html_col_name_list_stack.clear();
    QList<QVariant> report_html_col_name_stack_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Col_Name_Stack_QList).toList();
    for(int i=0; i < report_html_col_name_stack_list.size(); i++)
        report_html_col_name_list_stack.append(report_html_col_name_stack_list.at(i).toStringList());

    report_html_datatype_list_stack.clear();
    QList<QVariant> report_html_datatype_stack_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Datatype_Stack_QList).toList();
    for(int i=0; i < report_html_datatype_stack_list.size(); i++)
        report_html_datatype_list_stack.append(report_html_datatype_stack_list.at(i).toStringList());

    tw_data_type_list_tablewidget.clear();
    QList<QVariant> tw_data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList).toList();
    for(int i=0; i < tw_data_type_tablewidget_list.size(); i++)
        tw_data_type_list_tablewidget.append(tw_data_type_tablewidget_list.at(i).toStringList());


    rt_html_d_name_list_tablewidget.clear();
    QList<QVariant> html_d_name_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Display_Name_Tablewidget_QList).toList();
    for(int i=0; i < html_d_name_tablewidget_list.size(); i++)
        rt_html_d_name_list_tablewidget.append(html_d_name_tablewidget_list.at(i).toStringList());


    rt_html_order_list_tablewidget.clear();
    QList<QVariant> html_order_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Html_Order_Tablewidget_QList).toList();
    for(int i=0; i < html_order_tablewidget_list.size(); i++)
        rt_html_order_list_tablewidget.append(html_order_tablewidget_list.at(i).toStringList());


}

void html_standard_report_module::clear_fields()
{
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList , QStringList());
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList , QStringList());

}


void html_standard_report_module::create_tablewidget_html_report(QFile &file, QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked)
{
    for(int i_tab = 0 ; i_tab < table_name_list_tablewidget.size() ; i_tab++)
    {
        if(selected_tab_name != tab_name_list_tablewidget.at(i_tab))
            continue;

        QString is_report_available = table_html_report_available_list_tablewidget.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        QString table_name = table_name_list_tablewidget.at(i_tab);
        QString tab_name = tab_name_list_tablewidget.at(i_tab);

        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {
            file.write(report_template_html_standard::html_sub_module_header(tab_name).toLocal8Bit().data());

            //            QString is_common_artifact = table_artifact_common_list_tablewidget.at(i_tab);
            //            if(is_common_artifact == QString("1"))
            //            {
            //                QString artifacts_str = "<b>" + QObject::tr("Artifacts Source") + ": </b><br>";
            //                file.write(artifacts_str.toLocal8Bit());

            //                QStringList source_list = report_generic_function_obj->get_artifacts_source_list(table_name, report_scope,selected_tags_list);
            //                for(int cc = 0; cc < source_list.size(); cc++)
            //                {
            //                    file.write(QString(source_list.at(cc)).toLocal8Bit().data());
            //                    file.write("<br>");
            //                }
            //            }

            QString header_str = table_report_html_header_list_tablewidget.at(i_tab);
            QStringList headers = header_str.split("::", Qt::SkipEmptyParts);
            headers.insert(0,"Sr. No.");


            QString header_str_col_width = table_report_header_column_width_html_list_tablewidget.at(i_tab);
            recon_static_functions::app_debug("header_str_col_width " + header_str_col_width,Q_FUNC_INFO);
            QStringList  headers_column_width = header_str_col_width.split("::", Qt::SkipEmptyParts);
            headers_column_width.insert(0,"50");

            QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
            QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
            QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);
            bool bool_multiple_export = false;
            if(is_multiple_export == "2")
                bool_multiple_export = true;

            bool virtual_export_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);

            if(!bool_multiple_export)
            {
                if(!virtual_export_exist)
                {
                    if((is_export_available == QString("1")) && (tab_export_checked == false))
                    {
                        headers.removeLast();
                        headers_column_width.removeLast();
                    }
                }
            }

            file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_headers(headers,headers_column_width).toLocal8Bit().data());

            int sr_no = 0;
            for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
            {
                QString db_path = destination_db_file_list.at(db_count);

                QString connection_naam = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                dest_db.setDatabaseName(db_path);
                if(!dest_db.open())
                {
                    recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
                    continue ;
                }

                dest_db.transaction();


                QStringList cols_list = rt_html_col_name_list_tablewidget.at(i_tab);

                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {

                    QString command = "select " + cols_list.join(",") + ", source_count_name from '" + table_name + "'";
                    set_tablewidget_data_new(file, i_tab,  command,  table_name, tab_export_checked, is_export_available,  bool_multiple_export,0,db_path,dest_db);
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, db_path, selected_tags_list, Q_FUNC_INFO);

                    QString report_data;
                    if(get_plugin_name() == MACRO_Plugin_Name_File_System)
                    {
                        QString temp_db_dir_path = db_path;
                        QFileInfo file_info(db_path);
                        temp_db_dir_path.chop(file_info.fileName().size()); // "file_system.sqlite"
                        QString mime_db_path = temp_db_dir_path + "mime.sqlite";
                        QString hashes_db_path = temp_db_dir_path + "hashes.sqlite";
                        QString sign_db_path = temp_db_dir_path + "signature.sqlite";
                        QString exif_db_path  = temp_db_dir_path + "exif_metadata.sqlite";
                        QString apple_meta_db_path = temp_db_dir_path + "apple_metadata.sqlite";

                        QSqlQuery query_select(dest_db);
                        QString sign_db_attach_commmand = QString("ATTACH DATABASE '%1' AS signatureDB")
                                .arg(QDir::toNativeSeparators(sign_db_path));
                        query_select.exec(sign_db_attach_commmand);

                        QString exif_db_attach_commmand = QString("ATTACH DATABASE '%1' AS exif_metadataDB")
                                .arg(QDir::toNativeSeparators(exif_db_path));
                        query_select.exec(exif_db_attach_commmand);


                        QString apple_meta_db_attach_commmand = QString("ATTACH DATABASE '%1' AS apple_metadataDB")
                                .arg(QDir::toNativeSeparators(apple_meta_db_path));
                        query_select.exec(apple_meta_db_attach_commmand);

                        QString hash_db_attach_commmand = QString("ATTACH DATABASE '%1' AS hashesDB")
                                .arg(QDir::toNativeSeparators(hashes_db_path));
                        query_select.exec(hash_db_attach_commmand);

                        QString mime_db_attach_commmand = QString("ATTACH DATABASE '%1' AS mimeDB")
                                .arg(QDir::toNativeSeparators(mime_db_path));
                        query_select.exec(mime_db_attach_commmand);

                        for(int i = 0; i < total_record_list.size(); i++)
                        {
                            QString command = report_generic_function_obj->pub_get_fs_command(total_record_list.at(i));
                            set_tablewidget_data_new(file, i_tab,  command,  table_name, tab_export_checked, is_export_available,  bool_multiple_export,sr_no, db_path,dest_db);

                            sr_no++;
                        }

                        QString sign_db_detach_commmand = QString("DETACH DATABASE signatureDB");
                        query_select.exec(sign_db_detach_commmand);
                        QString exif_db_detach_commmand = QString("DETACH DATABASE exif_metadataDB");
                        query_select.exec(exif_db_detach_commmand);
                        QString apple_meta_db_detach_commmand = QString("DETACH DATABASE apple_metadataDB");
                        query_select.exec(apple_meta_db_detach_commmand);
                        QString hash_db_detach_commmand = QString("DETACH DATABASE hashesDB");
                        query_select.exec(hash_db_detach_commmand);
                        QString mime_db_detach_commmand = QString("DETACH DATABASE mimeDB");
                        query_select.exec(mime_db_detach_commmand);

                    }
                    else
                    {
                        if(!report_generic_function_obj->pub_is_tagtype_other_than_screenshot_or_saved_graph(get_plugin_name()))
                        {
                            for(int i = 0; i < total_record_list.size(); i++)
                            {
                                QString  command = QString("select " + cols_list.join(",") + " from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                                set_tablewidget_data_new(file, i_tab,  command,  table_name, tab_export_checked, is_export_available,  bool_multiple_export,sr_no, db_path,dest_db);

                                sr_no++;
                            }
                        }
                        else
                        {
                            for(int i = 0; i < total_record_list.size(); i++)
                            {
                                QString  command;
                                if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
                                {
                                    command = QString("select " + cols_list.join(",") + ", source_file, source_count_name from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                                }
                                else
                                {
                                    command = QString("select " + cols_list.join(",") + ", source_count_name from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                                }
                                set_tablewidget_data_new(file, i_tab,  command,  table_name, tab_export_checked, is_export_available,  bool_multiple_export,sr_no, db_path,dest_db);

                                sr_no++;
                            }

                        }
                    }


                }

                dest_db.commit();
                dest_db.close();
            }

            file.write(report_template_html_standard::html_table_end().toLocal8Bit().data());
            file.write("<br>");
            file.write("<br>");
            file.write("<br>");
        }

        if(selected_tab_name == MACRO_Tag_Name_Examiner_Comments)
        {
            if(report_scope == MACRO_REPORT_SCOPE_FULL_QString || report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
            {
                if(report_generic_function_obj->pub_is_data_available_in_examiner_notes())
                {
                    file.write(report_template_html_standard::html_sub_module_header(selected_tab_name).toLocal8Bit().data());

                    QString header_str = table_report_html_header_list_tablewidget.at(i_tab);
                    QStringList headers = header_str.split("::", Qt::SkipEmptyParts);
                    headers.insert(0,"Sr. No.");


                    QString header_str_col_width = table_report_header_column_width_html_list_tablewidget.at(i_tab);
                    recon_static_functions::app_debug("header_str_col_width " + header_str_col_width,Q_FUNC_INFO);
                    QStringList  headers_column_width = header_str_col_width.split("::", Qt::SkipEmptyParts);
                    headers_column_width.insert(0,"50");

                    QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
                    QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
                    QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);
                    bool bool_multiple_export = false;
                    if(is_multiple_export == "2")
                        bool_multiple_export = true;

                    bool virtual_export_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);

                    if(!bool_multiple_export)
                    {
                        if(!virtual_export_exist)
                        {
                            if((is_export_available == QString("1")) && (tab_export_checked == false))
                            {
                                headers.removeLast();
                                headers_column_width.removeLast();
                            }
                        }
                    }

                    file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
                    file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());
                    file.write(report_template_html_standard::html_table_headers(headers,headers_column_width).toLocal8Bit().data());

                    QString examiner_data = recon_helper_standard_obj->pub_fetch_examiner_comments_data();
                    QString var = "<tr><td class = \"wrapped\"> <strong> 1 </strong> </td> <td class = \"wrapped\"> <strong> " + examiner_data + " </strong> </td>";
                    file.write(var.toLocal8Bit().data());
                }


                file.write(report_template_html_standard::html_table_end().toLocal8Bit().data());
            }
        }
    }
}

void html_standard_report_module::create_textwidget_html_report(QFile &file, QString selected_tab_name, QString report_scope, QStringList selected_tags_list)
{
    for(int i_tab = 0 ; i_tab < table_name_list_textedit.size() ; i_tab++)
    {
        if(selected_tab_name != tab_name_list_textedit.at(i_tab))
            continue;
        
        QString is_report_available = html_report_available_list_textedit.at(i_tab);
        if(is_report_available != QString("1"))
            continue;
        
        
        QString table_name = table_name_list_textedit.at(i_tab);
        QString tab_name = tab_name_list_textedit.at(i_tab);
        
        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {
            file.write(report_template_html_standard::html_sub_module_header(tab_name).toLocal8Bit().data());
            
            QString is_common_artifact = artifact_common_list_textedit.at(i_tab);
            if(is_common_artifact == QString("1"))
            {
                QString artifacts_str = "<b>" + QObject::tr("Artifacts Source") + ": </b><br>";
                file.write(artifacts_str.toLocal8Bit());
                
                QStringList source_list = report_generic_function_obj->pub_get_artifacts_source_list(table_name, report_scope,selected_tags_list);
                for(int cc = 0; cc < source_list.size(); cc++)
                {
                    file.write(QString(source_list.at(cc)).toLocal8Bit().data());
                    file.write("<br>");
                }
            }
            
            QString header_str = report_html_header_list_textedit.at(i_tab);
            QStringList headers = header_str.split("::", Qt::SkipEmptyParts);
            headers.insert(0,"Sr. No.");
            
            QString header_str_col_width = report_header_column_width_html_list_textedit.at(i_tab);
            QStringList  headers_column_width = header_str_col_width.split("::", Qt::SkipEmptyParts);
            headers_column_width.insert(0,"50");
            
            file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_headers(headers,headers_column_width).toLocal8Bit().data());
            
            
            QStringList cols_list = report_html_col_name_list_textedit.at(i_tab);
            QStringList cols_data_type_list =  report_html_datatype_list_textedit.at(i_tab);
            
            int textedit_column_pos = -1;
            for(int j=0; j < cols_list.size(); j++)
            {
                if(cols_data_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_TEXTEDIT_QString)
                {
                    textedit_column_pos = j;
                    break;
                }
            }
            
            if(textedit_column_pos == -1)
            {
                recon_static_functions::app_debug(" ------FAILED--- Textedit column not found " , Q_FUNC_INFO);
                return;
            }
            
            for(int i = 0 ; i < destination_db_file_list.size() ; i++)
            {
                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    QString command = "select " + cols_list.join(",") + " from '" + table_name + "'";
                    QString report_data = set_textedit_data(command,textedit_column_pos,0,destination_db_file_list.at(i));
                    file.write(report_data.toLocal8Bit().data());
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, destination_db_file_list.at(i), selected_tags_list, Q_FUNC_INFO);
                    for(int i = 0; i < total_record_list.size(); i++)
                    {
                        QString command = QString("select " + cols_list.join(",") + " from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                        QString report_data = set_textedit_data(command,textedit_column_pos,0,destination_db_file_list.at(i));
                        file.write(report_data.toLocal8Bit().data());
                    }
                }
            }
            file.write(report_template_html_standard::html_table_end().toLocal8Bit().data());

            file.write("<br>");
            file.write("<br>");
            file.write("<br>");
        }
        
    }
}

void html_standard_report_module::create_stackwidget_html_report(QFile &file, QString selected_tab_name, QString report_scope, QStringList selected_tags_list)
{
    for(int i_tab = 0 ; i_tab < table_name_list_stack.size() ; i_tab++)
    {
        if(selected_tab_name != tab_name_list_stack.at(i_tab))
            continue;
        
        QString is_report_available = html_report_available_list_stack.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        QString table_name = table_name_list_stack.at(i_tab);
        QString tab_name = tab_name_list_stack.at(i_tab);
        
        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {
            file.write(report_template_html_standard::html_sub_module_header(tab_name).toLocal8Bit().data());
            
            QString is_common_artifact = artifact_common_list_stack.at(i_tab);
            if(is_common_artifact == QString("1"))
            {
                QString artifacts_str = "<b>" + QObject::tr("Artifacts Source") + ": </b><br>";
                file.write(artifacts_str.toLocal8Bit());
                
                QStringList source_list = report_generic_function_obj->pub_get_artifacts_source_list(table_name, report_scope,selected_tags_list);
                for(int cc = 0; cc < source_list.size(); cc++)
                {
                    file.write(QString(source_list.at(cc)).toLocal8Bit().data());
                    file.write("<br>");
                }
            }
            
            QString header_str = report_html_header_list_stack.at(i_tab);
            QStringList headers = header_str.split("::", Qt::SkipEmptyParts);
            headers.insert(0,"Sr. No.");
            
            QString header_str_col_width = report_header_column_width_html_list_stack.at(i_tab);
            QStringList  headers_column_width = header_str_col_width.split("::", Qt::SkipEmptyParts);
            headers_column_width.insert(0,"50");
            
            
            file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_headers(headers,headers_column_width).toLocal8Bit().data());
            
            
            QStringList cols_list = report_html_col_name_list_stack.at(i_tab);
            QStringList cols_data_type_list =  report_html_datatype_list_stack.at(i_tab);
            
            int stack_column_pos = -1;
            for(int j=0; j < cols_list.size(); j++)
            {
                if(cols_data_type_list.at(j) == MACRO_CSV_TEMPLATE_DATATYPE_STACK_QString)
                {
                    stack_column_pos = j;
                    break;
                }
            }
            
            if(stack_column_pos == -1)
            {
                recon_static_functions::app_debug("------FAILED--- Stack column not found " , Q_FUNC_INFO);
                return;
            }
            
            for(int i = 0 ; i < destination_db_file_list.size() ; i++)
            {

                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    QString command = "select " + cols_list.join(",") + " from '" + table_name + "'";
                    QString report_data = set_stack_data(command,stack_column_pos, 0 ,destination_db_file_list.at(i));
                    file.write(report_data.toLocal8Bit().data());
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, destination_db_file_list.at(i), selected_tags_list, Q_FUNC_INFO);
                    for(int i = 0; i < total_record_list.size(); i++)
                    {
                        QString command = QString("select " + cols_list.join(",") + " from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                        QString report_data = set_stack_data(command,stack_column_pos,0,destination_db_file_list.at(i));
                        file.write(report_data.toLocal8Bit().data());
                    }
                }
            }
            file.write(report_template_html_standard::html_table_end().toLocal8Bit().data());

            file.write("<br>");
            file.write("<br>");
            file.write("<br>");
        }
    }
}

void html_standard_report_module::create_full_report_standard_html(QString file_path,  QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Start " , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug("file open ---FAILED---  " + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString() , Q_FUNC_INFO);
        return;
    }

    QString tmp_str = get_plugin_name();
    QString icon_path = QString("./resources/plugins/") +  tmp_str.replace(" ","_") + ".png";
    file.write(report_template_html_standard::html_module_header(icon_path,parent_module_name_str, get_plugin_name()).toLocal8Bit().data());    file.write("<br>");
    file.write("<br>");
    file.write("<br>");

    bool bool_chat_available = false;
    if(global_csv_reader_class_selected_obj->is_plugin_available_for_chat_view(get_plugin_name()))
    {
        QString account = "Configured_Account";
        QString dir_path_to_itr = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + get_plugin_name().replace(" ","_") + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Chat_View_Page_QString ;
        if(QFileInfo(dir_path_to_itr).exists())
        {

            QString tab_name = global_csv_reader_class_selected_obj->get_chat_view_tab_name(get_plugin_name());

            file.write(report_template_html_standard::html_sub_module_header(tab_name).toLocal8Bit().data());
            file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
            file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());


            QFileInfo file_info(file_path);
            QString file_path_dir = file_path;
            QString tm_str = file_info.fileName();
            file_path_dir.chop(tm_str.size());

            QString dest_dir = file_path_dir + "Files/";
            QDir dir;
            dir.mkpath(dest_dir);


            QDirIterator itr(dir_path_to_itr);
            while(itr.hasNext())
            {
                QFileInfo file_info(itr.next());

                if(file_info.fileName() == QString(".") || file_info.fileName() == QString(".."))
                    continue ;

                if(file_info.isDir() && file_info.filePath().contains(account))
                {

                    QString account_info = file_info.filePath() + "/configured_account.txt";
                    QByteArray account_info_arr;
                    QFile ch_file(account_info);
                    if(!ch_file.open(QIODevice::ReadOnly))
                        continue ;

                    account_info_arr =  ch_file.readAll();
                    ch_file.close();

                    QDirIterator chat_dir_itr(file_info.filePath() + "/");
                    while(chat_dir_itr.hasNext())
                    {

                        QFileInfo cht_file_info(chat_dir_itr.next());
                        if(cht_file_info.fileName() == QString(".") || cht_file_info.fileName() == QString(".."))
                            continue ;

                        if(cht_file_info.filePath().contains("chat_buddy"))
                        {
                            QDirIterator t_itr(cht_file_info.filePath() + "/");
                            while(t_itr.hasNext())
                            {

                                QFileInfo file_info(t_itr.next());
                                if(file_info.fileName() == QString(".") || file_info.fileName() == QString(".."))
                                    continue ;

                                if(file_info.filePath().endsWith("chat_buddy_data.html"))
                                {

                                    bool_chat_available = true;
                                    QString dir_path = file_info.filePath();
                                    dir_path.chop(file_info.fileName().size());

                                    ///-------copy files dir
                                    recon_static_functions::copy_directory(dir_path + "Files" , dest_dir , Q_FUNC_INFO);
                                    ///-------copy files dir



                                    QString chat_b_info_file_path = dir_path + "chat_buddy_info.txt";
                                    QByteArray chat_b_data_arr;
                                    QFile ch_file(chat_b_info_file_path);
                                    if(!ch_file.open(QIODevice::ReadOnly))
                                    {
                                        continue ;
                                    }

                                    chat_b_data_arr =  ch_file.readAll();
                                    file.write(QString("<br><b>" + QObject::tr("Participants") + "</b><br>"+ account_info_arr + " , " + chat_b_data_arr ).toLocal8Bit());
                                    ch_file.close();




                                    ///-------write data in file
                                    QString html_file_path = dir_path + "chat_buddy_data.html";
                                    QFile source_file(html_file_path);
                                    if(!source_file.open(QIODevice::ReadOnly))
                                        continue;


                                    QByteArray data_arr;
                                    data_arr =  source_file.readAll();
                                    file.write(data_arr);
                                    source_file.close();



                                    file.flush();

                                }

                            }

                        }


                    }


                }


            }

        }
    }




    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list))
    {


        if(bool_chat_available)
        {
            file.flush();
            file.close();
            return;
        }


        QString artifacts_str = "<b><font color=\"red\">" +  QObject::tr("No Data Available") + "</font></b>";
        file.write(artifacts_str.toLocal8Bit());
        file.flush();
        file.close();
        return;
    }


    //=============== export start ==================//
    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);
        if(tabs_export_status_list.at(pp) == "1")
            export_file_obj->export_file(MACRO_REPORT_SCOPE_FULL_QString,tabname,selected_tags_list);
    }
    //=============== export end ==================//

    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        if(!report_generic_function_obj->pub_is_data_available_in_tab(QStringList(tabname), MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list))
            continue;

        bool bool_exp = false;
        if(tabs_export_status_list.at(pp) == "1")
            bool_exp = true;

        create_tablewidget_html_report(file,tabname,MACRO_REPORT_SCOPE_FULL_QString,selected_tags_list,bool_exp);

        create_textwidget_html_report(file,tabname, MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list);

        create_stackwidget_html_report(file,tabname,MACRO_REPORT_SCOPE_FULL_QString,selected_tags_list);

    }

    file.flush();
    file.close();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);

}


QString html_standard_report_module::set_tablewidget_data(int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int sr_no, QString destination_db_file,  QSqlDatabase &dest_db)
{

    QString report_data;

    QSqlQuery query_select(dest_db);
    QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);
    QStringList col_d_name_list = rt_html_d_name_list_tablewidget.at(i_tab);
    bool bool_virtual_datatype_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);


    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" ---FAILED--- query for select"  + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" command " + command , Q_FUNC_INFO);
        dest_db.close();
        return report_data;
    }

    int local_count = sr_no;

    while(query_select.next())
    {
        if(bool_cancel_report)
            break ;

        local_count++;

        QString num = QString::number(local_count);
        QStringList tabledata;
        tabledata << num;

        emlx_source_file.clear();
        if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            emlx_source_file = query_select.value(12).toString();

        QString exported_file_detail;
        QString exported_file_preview;


        int report_preview = 0;

        if((is_export_available == QString("1")) && (tab_export_checked == true))
        {
            if(get_plugin_name() == MACRO_Plugin_Name_Snapshots)
            {
                get_export_detail_html(dest_db, query_select.value("INT").toString(),table_name);
            }
            else
            {
                get_export_detail_html(dest_db, query_select.value(0).toString(),table_name);
            }

            if(bool_multiple_export)
            {
                report_preview = 2;
            }
            else
            {
                report_preview = 1;

                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList().size() > 0)
                    exported_file_detail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList().at(0);
                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList().size() > 0)
                    exported_file_preview = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList().at(0);

            }
        }

        QString record_number;
        QString temp_data;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());
        if(report_generic_function_obj->pub_is_tagtype_other_than_screenshot_or_saved_graph(get_plugin_name()))
        {
            temp_data.append("<b>" + QString("Source Name") + " : </b>" + struct_info.complete_source_name + "<br>");

            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
            {
                temp_data.append("<b>" + QObject::tr(MACRO_Generic_Display_Name) + ":</b>" + QString("\t") + struct_info.source_name +"<br>");
            }

        }

        if(get_plugin_name() == MACRO_Plugin_Name_File_System)
        {

            QString title_value;
            QString title_name;

            ///NOTE::--do not change order--

            ///1.--start-- columns are kept in csv
            for(int x = 0 ; x < col_d_name_list.size() - 2; x++)///skip tag and notes column which are taken in step 3
            {
                QString our_value = query_select.value(x).toString();
                QString rpt_data_type = data_type_list.at(x);


                if(our_value.trimmed().isEmpty())
                    continue ;

                if(rpt_data_type.endsWith("Hide"))
                {
                    rpt_data_type.remove("Hide");
                    if(our_value.trimmed().isEmpty())
                        continue;
                }
                else if(rpt_data_type.startsWith("NewLine"))
                {
                    rpt_data_type.remove("NewLine");
                    temp_data.append("<br>");
                }

                ///Html Report*/
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = our_value;
                }
                title_value = our_value;
                title_name = "<b>" + col_d_name_list.at(x) + " : </b>";

                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else if(value == QString("0"))
                        title_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        title_value = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";


                    temp_data.append(title_name + title_value + "<br>");

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    if(!our_value.trimmed().isEmpty())
                    {
                        title_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);
                        temp_data.append(title_name + title_value + "<br>");
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {

                    title_value.clear();
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
                            title_name = (QString("<b> <u>" + col_d_name_list.at(x) + "</u></b>"));
                            for(int k = 0 ; k< temp.size(); k++)
                            {
                                title_value.append("<br>");
                                title_value.append("<b>"+  QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                            }

                        }

                        temp_data.append(title_name + title_value + "<br>");

                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                    {
                        title_value = our_value.toHtmlEscaped();

                    }
                    else
                    {
                        title_name.clear();
                    }

                }
                else
                {
                    if(!our_value.trimmed().isEmpty())
                    {
                        title_value = our_value.toHtmlEscaped();
                        temp_data.append(title_name + title_value + "<br>");
                    }

                }


            }
            ///1.--end--



            ///-----Bookmarked Apple Metadata
            QString bookmarked_apple_m_value = query_select.value(col_d_name_list.size()).toString();
            if(!bookmarked_apple_m_value.trimmed().isEmpty())
            {

                temp_data.append("<br><b>" + QString("Bookmarked Apple MetaData") + "</b><br>");
                if(!bookmarked_apple_m_value.startsWith("\n"))
                    bookmarked_apple_m_value.replace("\n" , "<br>");

                temp_data.append(bookmarked_apple_m_value + "<br>");
            }
            ///-----Bookmarked Apple Metadata

            ///-----Bookmarked Exif Data
            QString exif_data_value = query_select.value("bookmarked_exif_data").toString();
            if(!exif_data_value.trimmed().isEmpty())
            {
                temp_data.append("<br><b>" + QString("Bookmarked Exif Data") + "</b><br>");
                if(!exif_data_value.startsWith("\n"))
                    exif_data_value.replace("\n" , "<br>");


                temp_data.append(exif_data_value + "<br>");
            }
            ///-----Bookmarked Exif Data

            int counter = 1;
            ///----Apple metadata
            QStringList apple_m_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList).toStringList();
            bool bool_apple_metadata_value_exists = false;
            for(int i = 1 ; i <= apple_m_keys_list.size(); i++)
            {
                if(!query_select.value(i + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_apple_metadata_value_exists = true;
                    break;
                }
            }
            if(bool_apple_metadata_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_Apple_Metadata_Parent) + "</b><br>");

            for(int key_value_count = 0 ; key_value_count < apple_m_keys_list.size() ; key_value_count++)
            {


                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();



                if(!our_value.isEmpty())
                {
                    QString key_name = apple_m_keys_list.at(key_value_count);
                    QString qvalue = our_value;

                    title_name = "<b>" + key_name + " : </b>";
                    qvalue = our_value;

                    if(key_name.contains("kMDItemFSSize"))
                    {
                        QString value =  our_value;
                        if(value == QString(""))
                            qvalue = "";
                        else if(value == QString("0"))
                            qvalue =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                        else
                            qvalue = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";

                    }
                    else if(key_name.contains("Date"))
                    {
                        qvalue = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(qvalue, Q_FUNC_INFO);
                    }


                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }


                counter++;
            }
            ///----Apple metadata




            ///----Exif
            QStringList exif_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList).toStringList();
            bool bool_exif_value_exists = false;
            for(int i = 1 ; i <= exif_keys_list.size() ; i++)
            {
                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_exif_value_exists = true;
                    break;
                }
            }
            if(bool_exif_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_Metadata_TabName_Exif_Meatadta) + "</b><br>");


            for(int key_value_count = 0 ; key_value_count < exif_keys_list.size() ; key_value_count++)
            {
                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = exif_keys_list.at(key_value_count);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///----Exif



            ///------Mime types
            //            bool bool_mime_value_exists = false;
            //            for(int i = 0 ;i < report_generic_function_obj->html_report_file_system_Mime_key_list.size() ;i++)
            //            {
            //                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
            //                {
            //                    bool_mime_value_exists = true;
            //                    break;
            //                }
            //            }
            //            if(bool_mime_value_exists)
            //                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_MIME_Type) + "</b><br>");
            temp_data.append("<br>");
            for(int key_value_count = 0 ;key_value_count < report_generic_function_obj->html_report_file_system_Mime_key_list.size() ;key_value_count++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = report_generic_function_obj->html_report_file_system_Mime_key_list.at(key_value_count);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///------Mime types





            ///----signatures
            bool bool_signatures_value_exists = false;
            for(int i = 1 ; i <= report_generic_function_obj->html_report_file_system_signauture_key_list.size() ; i++)
            {
                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_signatures_value_exists = true;
                    break;
                }
            }
            if(bool_signatures_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_File_Signature_Parent) + "</b><br>");

            for(int sg = 0 ; sg < report_generic_function_obj->html_report_file_system_signauture_key_list.size() ; sg++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = report_generic_function_obj->html_report_file_system_signauture_key_list.at(sg);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///----signatures



            ///2.--start-- tags and Notes columns in report and also kept in csv
            temp_data.append("<br>");
            for(int x = 2 ; x > 0; x--)
            {

                QString rpt_data_type = data_type_list.at(col_d_name_list.size() - x);
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
                {

                    QString tag_value = query_select.value(col_d_name_list.size() - 2).toString();
                    if(tag_value.trimmed().isEmpty())
                        continue ;

                    QString tag_text = "";
                    QStringList list_tags = tag_value.split(",", Qt::SkipEmptyParts);

                    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                    {
                        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                    }

                    if(tag_text.endsWith(","))
                        tag_text.chop(1);

                    title_value =  tag_text ;

                    temp_data.append( "<b> Tag : </b>" + title_value + "<br>");

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString)
                {
                    QString notes_value = query_select.value(col_d_name_list.size() - 1).toString();
                    if(notes_value.trimmed().isEmpty())
                        continue ;

                    title_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";
                    temp_data.append( "<b> Examiner Notes : </b>" + title_value + "<br>");
                }

            }
            ///2 --end-- tags and Notes column in report and also kept in csv

        }
        else
        {
            QString emlx_source_file_value;
            QString source_title_str;
            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            {
                QString emlx_file_name = QFileInfo(emlx_source_file).fileName();
                QString eml_dest_file_path = reports_dir_path + "/Files/" + emlx_file_name;
                if(emlx_file_name.endsWith(".emlx") || emlx_file_name.endsWith(".eml"))
                {
                    QString relative_path = "./Files/" + recon_helper_standard_obj->pub_get_html_formated_filename(emlx_file_name,Q_FUNC_INFO);

                    QString hyperlink = QString("<a href=") + "\"" + relative_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                    emlx_source_file_value = hyperlink + emlx_source_file + "</a>";
                    source_title_str = "<b> Source File : </b>";
                }
            }

            for(int y = 0 ; y < col_d_name_list.size() ; y++)
            {

                int ref_index = y;
                QString title_value;
                QString title_name;


                QString rpt_data_type = data_type_list.at(ref_index);
                QString rpt_data_type_for_tmpuse = rpt_data_type;

                QString our_column = col_d_name_list.at(ref_index);
                QString our_value = query_select.value(ref_index).toString();
                if(our_value.trimmed().isEmpty())
                    continue ;

                title_name = "<b>" + col_d_name_list.at(ref_index) + " : </b>";


                bool colmn_atr =  report_generic_function_obj->pub_is_column_of_apple_attribute(our_column);
                // hide extended attribute column which has no value
                if(colmn_atr)
                {
                    if(our_value.trimmed() == QString(""))
                        continue;
                }



                ///------------ Use Format as per CSV DataType
                if(rpt_data_type_for_tmpuse.endsWith("Hide"))
                {
                    rpt_data_type.remove("Hide");
                    if(our_value.trimmed().isEmpty())
                        continue;

                    if(rpt_data_type.startsWith("NewLine"))
                        rpt_data_type.remove("NewLine");

                }
                else if(rpt_data_type_for_tmpuse.startsWith("NewLine"))
                {
                    rpt_data_type.remove("NewLine");
                    temp_data.append("<br>");
                }
                else if(rpt_data_type_for_tmpuse.contains("_Person", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(7);
                }
                else if(rpt_data_type_for_tmpuse.contains("_Icon", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(5);
                }
                else if(rpt_data_type_for_tmpuse.contains("_WebIcon", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(QString("_WebIcon").size());
                }

                ///------------ Use Format as per CSV DataType


                ///Html Report*/
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = our_value;
                    title_value = record_number;
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString)
                {
                    QString extended_attr = our_value;
                    if(extended_attr.trimmed() == QString(""))
                        continue;

                    extended_attr = extended_attr.trimmed();
                    extended_attr.replace("\n   ","<br>&nbsp;");
                    extended_attr.replace("\n","<br>&nbsp;");
                    if(extended_attr != "")
                    {
                        title_name = "";
                        title_value = QString("<b><u>" + QObject::tr("Bookmarked Apple Metadata") + ": </u></b><br>" + "\t" + extended_attr + "<br>" );
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    title_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else if(value == QString("0"))
                        title_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        title_value = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    title_value.clear();
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
                            title_name = (QString("<b> <u>" + our_column + "</u></b>"));
                            for(int k = 0 ; k< temp.size(); k++)
                            {
                                title_value.append("<br>");
                                title_value.append("<b>"+  QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                            }

                        }
                    }
                    else
                    {
                        //continue;
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
                {

                    QString str = our_value;

                    if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");

                    }
                    else if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                    {
                        str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");
                    }
                    str = str.replace("|","<br>");

                    title_value = str;

                    title_value.prepend("<br>");
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
                {
                    QString str = our_value;

                    QStringList timestamps_splitList;

                    timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                    title_value.clear();
                    for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                    {
                        title_value.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");

                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
                {
                    if(!our_value.startsWith("/"))
                        our_value = our_value.prepend("/");

                    our_value.replace(";/","<br>/");

                    our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>/");

                    title_value = our_value;

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
                {

                    QString notes_value = query_select.value(ref_index).toString();
                    if(notes_value.trimmed().isEmpty())
                    {
                        title_name.clear();
                        continue ;
                    }
                    else
                        title_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";


                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECON_VIEWERS_TAG_EXPORTED_DATA_QString)
                {
                    if(get_plugin_name() == QString(MACRO_Plugin_Name_Hex_Viewer) || get_plugin_name() == QString(MACRO_Plugin_Name_Registry_Viewer))
                        title_value = report_generic_function_obj->pub_get_hex_viewer_format_for_html_report(our_value);
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_QString)
                {
                    title_value = "<font color = \"red\">" + our_value.toHtmlEscaped() + "</font>";
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
                {
                    temp_data.append("<br>");

                    QString tag_text = "";
                    QStringList list_tags = our_value.split(",", Qt::SkipEmptyParts);
                    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                    {
                        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                    }

                    if(tag_text.endsWith(","))
                        tag_text.chop(1);

                    title_value =  tag_text ;
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString)
                {
                    if(bool_virtual_datatype_exist)
                    {
                        QString virtual_image_file_path = report_generic_function_obj->pub_export_virtual_file(our_value , get_plugin_name() , destination_file_location , reports_dir_path);
                        exported_file_preview = get_virtual_preview_file(virtual_image_file_path);
                    }
                    title_name.clear();

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                    {
                        title_value = our_value.toHtmlEscaped();

                    }
                    else
                    {
                        title_name.clear();
                    }

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_QString)
                {
                    if(!our_value.startsWith("\n"))
                        our_value.replace("\n" , "<br>");

                    title_value.append("<br>" + our_value + "<br>");
                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString)
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else
                    {
                        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                        QString str_val = our_value;
                        str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                        title_value = str_val.trimmed();

                    }
                }
                else
                    title_value = our_value.toHtmlEscaped();



                ////start -----append  column name and value
                temp_data.append(title_name + title_value + "<br>");
                ////start -----append  column name and value
            }

            temp_data.append("<br>" + source_title_str + emlx_source_file_value + "<br>");
            emlx_source_file_value.clear();
            source_title_str.clear();

            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
            {
                QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
                if(!custom_col_name_list.isEmpty())
                {
                    if(is_export_available == QString::number(1) && tab_export_checked == true)
                    {
                        QString extended_list = recon_helper_standard_obj->get_apple_metadata_key_value_from_result(record_number, table_name, destination_db_file, Q_FUNC_INFO);

                        if(!extended_list.isEmpty())
                        {
                            temp_data += "<br><br><b><u><i>More Apple Metadata</u></i></b>"+ extended_list;
                        }
                    }
                }
            }


        }



        ///1.------
        if(!bool_multiple_export)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == true))
            {
                temp_data.append(exported_file_detail);
            }
        }
        ///1.------



        ///2.------
        tabledata << temp_data;
        ///2.------


        ///3.------
        if(bool_virtual_datatype_exist)
        {
            tabledata << exported_file_preview;
        }
        ///3.------



        ///--------report data
        if(!bool_multiple_export)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == true))
            {
                tabledata  << exported_file_preview;
            }
            report_data +=   report_template_html_standard::html_table_row_data(tabledata,0);
        }
        else if(bool_multiple_export && (is_export_available == QString("1")) && (tab_export_checked == true))
        {
            QString table_crt;

            QStringList multiple_export_filedetail ;
            multiple_export_filedetail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList();

            QStringList multiple_export_flepreview;
            multiple_export_flepreview = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList();

            for(int pp = 0; pp < multiple_export_filedetail.size(); pp++)
            {

                if(multiple_export_filedetail.at(pp).trimmed() == "")
                    continue;

                table_crt +=
                        "<table  border = \"0\" border-color = \"RED\" width = \"100%\">"
                        "<tr>"
                        "<td width = \"80%\">" + multiple_export_filedetail.at(pp)  + "</td>"
                                                                                      "<td align=\"center\">" + multiple_export_flepreview.at(pp) + "</td>"
                                                                                                                                                    "</tr>"
                                                                                                                                                    "</table>";
            }

            QString temp_data;
            for(int i = 0; i < tabledata.size(); i++)
            {
                if(!bool_virtual_datatype_exist)
                {
                    if(i == tabledata.size() - 1)
                        temp_data +=  QString("<td>" + tabledata.at(i) + table_crt + "</td>");
                    else
                        temp_data +=  QString("<td>" + tabledata.at(i) + "</td>");
                }
                else
                {
                    if(i == 0 || i == tabledata.size() - 1)
                    {
                        temp_data +=  QString("<td>" + tabledata.at(i) + "</td>");
                    }
                    else
                        temp_data +=  QString("<td>" + tabledata.at(i) + table_crt + "</td>");
                }
            }


            temp_data += QString("<tr>");
            report_data += temp_data;
        }
        else
        {
            report_data +=   report_template_html_standard::html_table_row_data(tabledata,0);
        }
        ///--------report data

    }

    return report_data;
}


QString html_standard_report_module::get_plugin_name()
{
    return plugin_name_str;
}

QString html_standard_report_module::get_virtual_preview_file(QString virtual_file_path)
{

    QString  resource_dir = "./resources/";
    QFileInfo info66(virtual_file_path);
    QString new_relative_path = QString("./") + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images) + info66.fileName();

    QString image;
    QString hyperlink;
    QString virtual_preview;
    if(recon_helper_standard_obj->check_is_it_image_file(virtual_file_path,virtual_file_path, Q_FUNC_INFO))
    {
        image = QString("<img src=") + "\"" + new_relative_path + "\"" + " alt=\"No Preview\"" + " width=\"142\" height=\"150\">";
        hyperlink = QString("<a href=") + "\"" + new_relative_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
        virtual_preview =  hyperlink + image + "</a>";
    }
    else
    {
        image = QString("<a href=") + "\"" + new_relative_path + "\"" + QString(" target=\"_blank\"") + " >" + "<img src=" + "\"" + resource_dir + "open_file_link.png\"" + "style=\" width=\"90\" height=\"100\"\">" + "</a>";
        hyperlink = QString("<a href=") + "\"" + new_relative_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
        virtual_preview =  hyperlink + image + "</a>";
    }

    return virtual_preview;

}

QString html_standard_report_module::set_textedit_data(QString command, int textedit_col_pos, int sr_no, QString destination_db_file)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    QString report_data;

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error" + dest_db.lastError().text() , Q_FUNC_INFO);
        return report_data;
    }

    QSqlQuery query_select(dest_db);

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" ---FAILED--- query" + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error" +  query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("command - " + command , Q_FUNC_INFO);
        dest_db.close();
        return report_data;
    }

    int local_count = sr_no;

    local_count++;
    QString num = QString::number(local_count);
    QStringList tabledata;
    tabledata << num;

    QString data;

    while(query_select.next())
    {
        if(bool_cancel_report)
            break;

        data.append(query_select.value(textedit_col_pos).toString() + "<br>");
    }

    tabledata << data;

    report_data +=   report_template_html_standard::html_table_row_data(tabledata,0);

    dest_db.close();

    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);
    return report_data;
}

QString html_standard_report_module::set_stack_data(QString command, int textedit_col_pos, int sr_no, QString destination_db_file)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);


    QString report_data;
    if(bool_cancel_report)
        return report_data;


    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" db open -----FAILED--- " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error"  + dest_db.lastError().text(), Q_FUNC_INFO);
        return report_data;
    }

    QSqlQuery query_select(dest_db);

    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug("---FAILED--- query  " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" command " + command , Q_FUNC_INFO);
        dest_db.close();
        return report_data;
    }

    int local_count = sr_no;

    local_count++;
    QString num = QString::number(local_count);
    QStringList tabledata;
    tabledata << num;


    QString file_path = destination_file_location + "/screenshot.png";
    QFileInfo info(file_path);
    if(!info.exists())
        file_path = destination_file_location + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + "screenshot.png";


    // right now this is only for first tab(Screenshot Plugin), In future if we want to use tab 2 then do code for it
    QDir dir;
    dir.mkpath(reports_dir_path + "/Images");

    QString preview_file_path = reports_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + "screenshot.png";


    QFile::copy(file_path,preview_file_path);
    QString image = QString("<img src=") + "\"" + preview_file_path + "\"" + " alt=\"No Preview\"" + " width=\"500\" height=\"350\">"; //140 150 2880 × 900
    QString hyperlink = QString("<a href=") + "\"" + preview_file_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + preview_file_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");

    QString temp_detail = hyperlink + image;


    tabledata << temp_detail;
    report_data +=   report_template_html_standard::html_table_row_data(tabledata,0);


    dest_db.close();

    recon_static_functions::app_debug(" Ends" , Q_FUNC_INFO);
    return report_data;
}

void html_standard_report_module::get_export_detail_html(QSqlDatabase dest_db, QString column, QString tablename)
{

    QSqlQuery query_export_detail(dest_db);

    QString cmnd = QString("select exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time,filepath,exp_LINKFILENAME from exportfilelist where (table_name = '") + tablename + "' AND record = " + column + ")";

    if(!query_export_detail.exec(cmnd))
    {
        recon_static_functions::app_debug("  ---FAILED--- " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_export_detail.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" command " + cmnd, Q_FUNC_INFO);
        return ;
    }

    QStringList exp_detail_list;
    QStringList exp_preview_list;



    while(query_export_detail.next())
    {

        QString exp_detail;
        QString exp_preview;


        QString relative_path = query_export_detail.value(0).toString();
        relative_path = relative_path.trimmed();

        QString org_filepath = query_export_detail.value(5).toString();
        QString link_file = query_export_detail.value(6).toString();

        QString relative_softlink_path;
        if(!link_file.trimmed().isEmpty())
            relative_softlink_path = "./files/Links/" + query_export_detail.value(6).toString();


        QFileInfo info8(org_filepath);
        QString original_filename = info8.fileName();

        if(relative_path == QString(""))
        {
            if(org_filepath.trimmed() == "")
            {
                //                exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                //                        + QString("<b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");

                if(get_plugin_name() == QString(MACRO_Plugin_Name_Email_Parser))
                {
                    ////Note::For 'EMLX Files' Feature 'Auto Export' is Enabled
                    ///To prevent unnecessary text "File does not exist" if In actual record does not contains attachment file
                    exp_detail = "";
                }
                else
                {
                    exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                            + QString("<b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");

                }
            }
            else
            {
                exp_detail = QString("<br><b> <u> <i>" + QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                        + "<b>"+  QObject::tr("Original File") + ": </b>" +"\t"+ original_filename +"\t"
                        + QString("<br><b> <font color = \"red\">" + QObject::tr("File does not exist") + "</font></b> <br><br>");
            }


            exp_detail_list << exp_detail;
            exp_preview_list << exp_preview;
        }
        else
        {
            QString image, hyperlink;
            QString new_relativepath;
            if(!link_file.trimmed().isEmpty())
            {
                new_relativepath = relative_softlink_path;
            }
            else
            {
                new_relativepath = relative_path;
            }

            QString  resource_dir = "./resources/";


            QString file_name = "";

            if(!link_file.trimmed().isEmpty())
            {
                file_name = relative_softlink_path;
            }
            else
            {
                file_name = relative_path;
            }

            file_name = file_name.remove("./files/");

            //new_relativepath = recon_helper_standard_obj->pub_get_html_formated_filename(new_relativepath,Q_FUNC_INFO);
            if(bool_export_file_for_pdf_report)
            {
                //No need to do anything...
            }
            else
            {
                new_relativepath = recon_helper_standard_obj->pub_get_html_formated_filename(new_relativepath,Q_FUNC_INFO);
            }

            QString filename_with_extn = new_relativepath;

            if(recon_helper_standard_obj->check_is_it_image_file(org_filepath,filename_with_extn, Q_FUNC_INFO))
            {
                if(recon_helper_standard_obj->is_special_file_for_preview(org_filepath,filename_with_extn , Q_FUNC_INFO))
                {
                    QString image_raw_data_str = recon_helper_standard_obj->pub_get_raw_data_for_image_file_preview(org_filepath);

                    image = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\"")
                            + " >" + QString("<img src=data:image/png;base64,") + image_raw_data_str  + ">";

                    hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");

                }
                else
                {
                    image = QString("<img src=") + "\"" + new_relativepath + "\"" + " alt=\"No Preview\"" + " width=\"142\" height=\"150\">";
                    hyperlink = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + new_relativepath + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                }
            }
            else
            {
                image = QString("<a href=") + "\"" + new_relativepath + "\"" + QString(" target=\"_blank\"") + " >" + "<img src=" + "\"" + resource_dir + "open_file_link.png\"" + "style=\" width=\"90\" height=\"100\"\">" + "</a>";
            }

            QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
            QString numeric_timezone_value;
            if(machine_timezone_string.contains("GMT"))
            {
                QStringList tmp_list = machine_timezone_string.split("GMT");
                numeric_timezone_value = tmp_list.at(1);
            }

            exp_detail = QString("<br><b> <u> <i>"+  QObject::tr("Exported File Detail") + "</i></u> </b>") +"<br>"
                    + "<b>"+  QObject::tr("Original File") + ": </b>" +"\t"+ original_filename +"\t<br>"
                    + "<b>"+  QObject::tr("Exported File") + ": </b>" +"\t"+ relative_path +"\t<br>"
                    + "<b>"+  QObject::tr("Size") + ": </b>" + recon_static_functions::human_readable_size(query_export_detail.value(1).toString(), Q_FUNC_INFO)
                    + " (" + query_export_detail.value(1).toString() + QObject::tr(" bytes") + ")" + "\t<br>"
                    + "<b>"+  QObject::tr("MD5") + ": </b>" +"\t"+ query_export_detail.value(2).toString() +"<br>"
                    +"<b>"+  QObject::tr("SHA1") + ": </b>" +"\t"+ query_export_detail.value(3).toString() +"\t<br>"
                    + "<b>"+  QObject::tr("Export Time") + ": </b>" +"\t"+ QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString()) + "&nbsp;" + numeric_timezone_value + "<br><br>";

            exp_preview =  hyperlink + image + "</a>";

            exp_detail_list << exp_detail;
            exp_preview_list << exp_preview;


        }
    }

    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList , exp_detail_list);
    narad_muni_for_report_obj->set_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList , exp_preview_list);

    return;
}

QString html_standard_report_module::export_virtual_file(QString file_path)
{
    if(file_path.trimmed() == QString(""))
        return "";


    QFileInfo info(file_path);
    QString file_name = info.fileName();

    if((file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images))) || (file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
            || (file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
        file_path =  destination_file_location + file_path.remove(0,1);

    // QString source_file =  destination_file_location + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images + file_name;

    //    if(!QFile::exists(source_file))
    //        source_file =  destination_file_location + "Maps/" + file_name;

    QDir dirp;
    dirp.mkpath(reports_dir_path + QString("/") + (MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images));

    QString dest_loc = reports_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images;
    file_name = recon_static_functions::get_available_filename(file_name,dest_loc, Q_FUNC_INFO);
    QString dest_file = dest_loc + file_name;

    QFile file;

    //    if(!QFile(source_file).exists())
    //    {
    //        file.copy(file_path,dest_file);
    //    }
    //    else
    //    {
    // file.copy(source_file,dest_file);
    //    }


    file.copy(file_path,dest_file);

    return dest_file;
}

void html_standard_report_module::create_tag_report_standard_html(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" Starts", Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);

    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error "  + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QString tmp_str = get_plugin_name();
    QString icon_path = QString("./resources/plugins/") +  tmp_str.replace(" ","_") + ".png";
    file.write(report_template_html_standard::html_module_header(icon_path,parent_module_name_str, get_plugin_name()).toLocal8Bit().data());    file.write("<br>");
    file.write("<br>");
    file.write("<br>");


    bool bool_chat_available = false;
    if(global_csv_reader_class_selected_obj->is_plugin_available_for_chat_view(get_plugin_name()))
    {

        QString chat_viewer_bookmark_db_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Feature_Path_Location_Chat_Viewer_Bookmarks_In_Result_QString).toString() + "bubble_chat_bookmarks.sqlite";
        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath("Select INT from chat_viewer_info where plugin_name = ?", QStringList(get_plugin_name()),0,chat_viewer_bookmark_db_path,Q_FUNC_INFO);
        if(!INT_str.trimmed().isEmpty())
        {

            bool_chat_available = true;
            if(selected_tags_list.contains(MACRO_Tag_Name_Bookmarks))
            {


                QString tabname = global_csv_reader_class_selected_obj->get_chat_view_tab_name(get_plugin_name());
                file.write(report_template_html_standard::html_sub_module_header(tabname).toLocal8Bit().data());
                file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
                file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());

                QFileInfo file_info(file_path);
                QString file_path_dir = file_path;
                QString tm_str = file_info.fileName();
                file_path_dir.chop(tm_str.size());

                QString dest_dir = file_path_dir + "Files/";
                QDir dir;
                dir.mkpath(dest_dir);


                QString connection_naam = QString(Q_FUNC_INFO);
                QSqlDatabase::removeDatabase(connection_naam);
                QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
                dest_db.setDatabaseName(chat_viewer_bookmark_db_path);
                if(!dest_db.open())
                {
                    recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
                    return ;
                }


                QSqlQuery select_qry(dest_db);
                QString command = "SELECT  DISTINCT chat_buddy , configured_account FROM chat_viewer_info  where plugin_name = ? AND tab_name = ?";
                select_qry.prepare(command);
                select_qry.addBindValue(get_plugin_name());
                select_qry.addBindValue(tabname);
                if(!select_qry.exec())
                {
                    recon_static_functions::app_debug(" query_index.exec()  ------FAILED-----"+ dest_db.databaseName(), Q_FUNC_INFO);
                    recon_static_functions::app_debug(select_qry.lastError().text() ,Q_FUNC_INFO);
                    recon_static_functions::app_debug(select_qry.executedQuery() ,Q_FUNC_INFO);
                    dest_db.close();
                    return;
                }


                while(select_qry.next())
                {
                    QString chat_buddy_str = select_qry.value("chat_buddy").toString();
                    QString configured_account_str = select_qry.value("configured_account").toString();


                    QString account = "Configured_Account";
                    QString dir_path_to_itr = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + get_plugin_name().replace(" ","_") + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Chat_View_Page_QString ;
                    QDirIterator itr(dir_path_to_itr,QDirIterator::Subdirectories);

                    while(itr.hasNext())
                    {

                        QFileInfo file_info(itr.next());
                        if(file_info.fileName().endsWith("configured_account.txt") && file_info.filePath().contains(account))
                        {
                            QFile read_file(file_info.filePath());
                            if(!read_file.open(QIODevice::ReadOnly))
                            {
                                recon_static_functions::app_debug(" File Opened ------FAILED-----"+ read_file.fileName(), Q_FUNC_INFO);
                                recon_static_functions::app_debug(read_file.errorString() ,Q_FUNC_INFO);
                                continue ;
                            }

                            QString data_str = read_file.readAll();
                            if(data_str.contains(configured_account_str))
                            {

                                QString chat_buddy_dir_path = file_info.filePath();
                                chat_buddy_dir_path.chop(file_info.fileName().size());

                                QDirIterator chat_b_itr(chat_buddy_dir_path , QDirIterator::Subdirectories);
                                while(chat_b_itr.hasNext())
                                {

                                    QFileInfo chat_buddy_info(chat_b_itr.next());
                                    if(chat_buddy_info.fileName().endsWith(".txt") && chat_buddy_info.filePath().contains("chat_buddy"))
                                    {

                                        QFile ch_file(chat_buddy_info.filePath());
                                        if(!ch_file.open(QIODevice::ReadOnly))
                                        {
                                            recon_static_functions::app_debug(" File Opened ------FAILED-----"+ ch_file.fileName(), Q_FUNC_INFO);
                                            recon_static_functions::app_debug(ch_file.errorString() ,Q_FUNC_INFO);
                                            continue ;
                                        }

                                        QString data_str = ch_file.readAll();
                                        if(data_str.contains(chat_buddy_str))
                                        {

                                            QString chat_b_dir_path = chat_buddy_info.filePath();
                                            chat_b_dir_path.chop(chat_buddy_info.fileName().size());

                                            ///-------copy files dir
                                            recon_static_functions::copy_directory(chat_b_dir_path + "Files" , dest_dir , Q_FUNC_INFO);
                                            ///-------copy files dir


                                            ///-------write data in file
                                            QString html_file_path = chat_b_dir_path + "chat_buddy_data.html";
                                            QFile source_file(html_file_path);
                                            if(!source_file.open(QIODevice::ReadOnly))
                                                continue ;


                                            QByteArray data_arr;
                                            data_arr =  source_file.readAll();
                                            source_file.close();

                                            file.write(QString("<b><u>" + QObject::tr("Participants") + "</u></b><br>" + configured_account_str + QString(" , ") + chat_buddy_str).toLocal8Bit());
                                            file.write(data_arr);
                                            file.flush();
                                            ///-------write data in file

                                            break ;

                                        }


                                    }


                                }

                                break ;
                            }

                        }

                    }

                }

                dest_db.close();
            }
        }

    }




    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list))
    {

        if(bool_chat_available)
        {
            file.flush();
            file.close();
            return ;
        }

        QString artifacts_str = "<b><font color=\"red\">" +  QObject::tr("No Data Available") + "</font></b>";
        file.write(artifacts_str.toLocal8Bit());
        file.flush();
        file.close();
        return;
    }






    //=============== export start ==================//
    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);
        if(tabs_export_status_list.at(pp) == "1")
            export_file_obj->export_file(MACRO_REPORT_SCOPE_TAGS_QString,tabname,selected_tags_list);
    }
    //=============== export end ==================//

    for(int pp = 0; pp < selected_tablist.size(); pp++)
    {
        QString tabname = selected_tablist.at(pp);

        if(!report_generic_function_obj->pub_is_data_available_in_tab(QStringList(tabname), MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list))
            continue;

        bool bool_exp = false;
        if(tabs_export_status_list.at(pp) == "1")
            bool_exp = true;

        create_tablewidget_html_report(file,tabname,MACRO_REPORT_SCOPE_TAGS_QString,selected_tags_list,bool_exp);

        create_textwidget_html_report(file,tabname, MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list);

        create_stackwidget_html_report(file,tabname,MACRO_REPORT_SCOPE_TAGS_QString,selected_tags_list);

    }

    file.flush();
    file.close();
    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);
    return;
}


void html_standard_report_module::create_screen_report_standard_html(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);


    if((selected_tablist.size() < 1) || (tabs_export_status_list.size() < 1))
        return;

    int i_tab = -1;
    QString tab_nam = selected_tablist.at(0);
    for(int pp = 0 ; pp < table_name_list_tablewidget.size() ; pp++)
    {
        if(tab_nam == tab_name_list_tablewidget.at(pp))
        {
            i_tab = pp;
            break;
        }
    }

    if(i_tab == -1)
        return;

    if(list_m_tablewidgets.size() <= i_tab)
        return;


    QString command;

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug("  file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + file.errorString(), Q_FUNC_INFO);
        return;
    }


    QString icon_path = QString("./resources/plugins/")  + get_plugin_name().replace(" ","_") + ".png";
    file.write(report_template_html_standard::html_module_header(icon_path,parent_module_name_str, get_plugin_name()).toLocal8Bit().data());
    file.write("<br>");
    file.write("<br>");

    file.write(report_template_html_standard::html_sub_module_header(tab_nam).toLocal8Bit().data());

    //=============== is data available start ==================//
    bool bool_data_available = false;
    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        bool_data_available = true;
        break;
    }
    if(!bool_data_available)
    {
        QString artifacts_str = "<b><font color=\"red\">" +  QObject::tr("No Data Available") + "</font></b>";
        file.write(artifacts_str.toLocal8Bit());
        file.flush();
        file.close();
        return;
    }
    //=============== is data available end ==================//


    //=============== export start ==================//
    bool tab_export_checked = false;
    QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(get_plugin_name(), tab_nam);
    if(tabs_export_status_list.at(0) == "1")
        tab_export_checked = true;

    if(tab_export_checked)
    {
        export_file_obj->export_file(MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString,tab_nam,QStringList());
    }
    //=============== export end ==================//


    //    QString is_common_artifact = table_artifact_common_list_tablewidget.at(i_tab);
    //    if(is_common_artifact == QString("1"))
    //    {
    //        QString artifacts_str = "<b>" + QObject::tr("Artifacts Source") + ": </b><br>";
    //        file.write(artifacts_str.toLocal8Bit());
    //        QStringList source_list = report_generic_function_obj->get_artifacts_source_list(table_name, MACRO_REPORT_SCOPE_SCREEN_QString,QStringList(""));
    //        for(int cc = 0; cc < source_list.size(); cc++)
    //        {
    //            file.write(QString(source_list.at(cc)).toLocal8Bit().data());
    //            file.write("<br>");
    //        }
    //    }


    QStringList headers;
    QString header_str = table_report_html_header_list_tablewidget.at(i_tab);
    headers = header_str.split("::", Qt::SkipEmptyParts);
    headers.insert(0,"Sr. No.");


    QString header_str_col_width = table_report_header_column_width_html_list_tablewidget.at(i_tab);
    recon_static_functions::app_debug("header_str_col_width " + header_str_col_width,Q_FUNC_INFO);
    QStringList  headers_column_width = header_str_col_width.split("::", Qt::SkipEmptyParts);
    headers_column_width.insert(0,"50");

    QString is_export_available = table_export_available_list_tablewidget.at(i_tab);

    QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
    bool bool_multiple_export = false;
    if(is_multiple_export == "2")
        bool_multiple_export = true;

    QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);
    bool virtual_export_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);

    if(!bool_multiple_export)
    {
        if(!virtual_export_exist)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == false))
            {
                headers.removeLast();
                headers_column_width.removeLast();
            }
        }
    }

    file.write(report_template_html_standard::html_top_link().toLocal8Bit().data());
    file.write(report_template_html_standard::html_table_start().toLocal8Bit().data());


    file.write(report_template_html_standard::html_table_headers(headers,headers_column_width).toLocal8Bit().data());


    QStringList cols_list = rt_html_col_name_list_tablewidget.at(i_tab);

    QStringList col_type_list = tw_data_type_list_tablewidget.at(i_tab);
    int INT_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString INT_str;


    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
        {
            QString connection_naam = QString(Q_FUNC_INFO);
            QSqlDatabase::removeDatabase(connection_naam);
            QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
            dest_db.setDatabaseName(destination_db_file_list.at(db_count));
            if(!dest_db.open())
            {
                recon_static_functions::app_debug(" db open -----FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug(" error " + dest_db.lastError().text() , Q_FUNC_INFO);
                continue ;
            }

            INT_str = list_m_tablewidgets.at(i_tab)->item(i, INT_index)->text();
            command = "select " + cols_list.join(",") + ", source_count_name , source_file from '" + table_name  + "' where INT = " + INT_str ;

            set_tablewidget_data_new(file, i_tab,  command,  table_name, tab_export_checked, is_export_available,  bool_multiple_export,i,destination_db_file_list.at(db_count),dest_db);

            dest_db.close();
        }
    }


    file.write(report_template_html_standard::html_table_end().toLocal8Bit().data());
    file.flush();
    file.close();

    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);

}

void html_standard_report_module::set_tablewidget_data_new(QFile &html_report_file,int i_tab, QString command, QString table_name, bool tab_export_checked, QString is_export_available, bool bool_multiple_export, int sr_no, QString destination_db_file,  QSqlDatabase &dest_db)
{
    QString report_data;

    QSqlQuery query_select(dest_db);
    QStringList data_type_list = rt_html_data_type_list_tablewidget.at(i_tab);
    QStringList col_d_name_list = rt_html_d_name_list_tablewidget.at(i_tab);
    bool bool_virtual_datatype_exist = report_generic_function_obj->pub_check_datatype_exist_in_datatypelist(MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_QString,data_type_list);


    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" ---FAILED--- query for select"  + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" command " + command , Q_FUNC_INFO);
        dest_db.close();
        return ;
    }

    int local_count = sr_no;

    while(query_select.next())
    {
        if(bool_cancel_report)
            break ;

        local_count++;

        QString num = QString::number(local_count);
        QStringList tabledata;
        tabledata << num;

        emlx_source_file.clear();
        if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            emlx_source_file = query_select.value(12).toString();

        QString exported_file_detail;
        QString exported_file_preview;


        int report_preview = 0;

        if((is_export_available == QString("1")) && (tab_export_checked == true))
        {
            if(get_plugin_name() == MACRO_Plugin_Name_Snapshots)
            {
                get_export_detail_html(dest_db, query_select.value("INT").toString(),table_name);
            }
            else
            {
                get_export_detail_html(dest_db, query_select.value(0).toString(),table_name);
            }

            if(bool_multiple_export)
            {
                report_preview = 2;
            }
            else
            {
                report_preview = 1;

                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList().size() > 0)
                    exported_file_detail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList().at(0);
                if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList().size() > 0)
                    exported_file_preview = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList().at(0);

            }
        }

        QString record_number;
        QString temp_data;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());
        if(report_generic_function_obj->pub_is_tagtype_other_than_screenshot_or_saved_graph(get_plugin_name()))
        {
            temp_data.append("<b>" + QString("Source Name") + " : </b>" + struct_info.complete_source_name + "<br>");

            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
            {
                temp_data.append("<b>" + QObject::tr(MACRO_Generic_Display_Name) + ":</b>" + QString("\t") + struct_info.source_name +"<br>");
            }

        }

        if(get_plugin_name() == MACRO_Plugin_Name_File_System)
        {

            QString title_value;
            QString title_name;

            ///NOTE::--do not change order--

            ///1.--start-- columns are kept in csv
            for(int x = 0 ; x < col_d_name_list.size() - 2; x++)///skip tag and notes column which are taken in step 3
            {
                QString our_value = query_select.value(x).toString();
                QString rpt_data_type = data_type_list.at(x);


                if(our_value.trimmed().isEmpty())
                    continue ;

                if(rpt_data_type.endsWith("Hide"))
                {
                    rpt_data_type.remove("Hide");
                    if(our_value.trimmed().isEmpty())
                        continue;
                }
                else if(rpt_data_type.startsWith("NewLine"))
                {
                    rpt_data_type.remove("NewLine");
                    temp_data.append("<br>");
                }

                ///Html Report*/


                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = our_value;
                }
                title_value = our_value;
                title_name = "<b>" + col_d_name_list.at(x) + " : </b>";

                int case_type_int = 0;
                if(rpt_data_type.contains("_"))
                {
                    case_type_int = rpt_data_type.split("_").at(0).toInt();
                }
                else
                    case_type_int = rpt_data_type.toInt();

                switch (case_type_int)
                {
                case  MACRO_CSV_TEMPLATE_DATATYPE_SIZE_INT:
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else if(value == QString("0"))
                        title_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        title_value = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";


                    temp_data.append(title_name + title_value + "<br>");
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_TIME_INT:
                {
                    if(!our_value.trimmed().isEmpty())
                    {
                        title_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);
                        temp_data.append(title_name + title_value + "<br>");
                    }
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES_INT:
                {
                    title_value.clear();
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
                            title_name = (QString("<b> <u>" + col_d_name_list.at(x) + "</u></b>"));
                            for(int k = 0 ; k< temp.size(); k++)
                            {
                                title_value.append("<br>");
                                title_value.append("<b>"+  QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                            }

                        }

                        temp_data.append(title_name + title_value + "<br>");

                    }
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_INT:
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                    {
                        title_value = our_value.toHtmlEscaped();

                    }
                    else
                    {
                        title_name.clear();
                    }
                    break;
                }
                default:
                {
                    if(!our_value.trimmed().isEmpty())
                    {
                        title_value = our_value.toHtmlEscaped();
                        temp_data.append(title_name + title_value + "<br>");
                    }
                    break;

                }
                }
            }
            ///1.--end--



            ///-----Bookmarked Apple Metadata
            QString bookmarked_apple_m_value = query_select.value(col_d_name_list.size()).toString();
            if(!bookmarked_apple_m_value.trimmed().isEmpty())
            {

                temp_data.append("<br><b>" + QString("Bookmarked Apple MetaData") + "</b><br>");
                if(!bookmarked_apple_m_value.startsWith("\n"))
                    bookmarked_apple_m_value.replace("\n" , "<br>");

                temp_data.append(bookmarked_apple_m_value + "<br>");
            }
            ///-----Bookmarked Apple Metadata

            ///-----Bookmarked Exif Data
            QString exif_data_value = query_select.value("bookmarked_exif_data").toString();
            if(!exif_data_value.trimmed().isEmpty())
            {
                temp_data.append("<br><b>" + QString("Bookmarked Exif Data") + "</b><br>");
                if(!exif_data_value.startsWith("\n"))
                    exif_data_value.replace("\n" , "<br>");


                temp_data.append(exif_data_value + "<br>");
            }
            ///-----Bookmarked Exif Data

            int counter = 1;
            ///----Apple metadata
            QStringList apple_m_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList).toStringList();
            bool bool_apple_metadata_value_exists = false;
            for(int i = 1 ; i <= apple_m_keys_list.size(); i++)
            {
                if(!query_select.value(i + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_apple_metadata_value_exists = true;
                    break;
                }
            }
            if(bool_apple_metadata_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_Apple_Metadata_Parent) + "</b><br>");

            for(int key_value_count = 0 ; key_value_count < apple_m_keys_list.size() ; key_value_count++)
            {


                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();



                if(!our_value.isEmpty())
                {
                    QString key_name = apple_m_keys_list.at(key_value_count);
                    QString qvalue = our_value;

                    title_name = "<b>" + key_name + " : </b>";
                    qvalue = our_value;

                    if(key_name.contains("kMDItemFSSize"))
                    {
                        QString value =  our_value;
                        if(value == QString(""))
                            qvalue = "";
                        else if(value == QString("0"))
                            qvalue =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                        else
                            qvalue = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";

                    }
                    else if(key_name.contains("Date"))
                    {
                        qvalue = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(qvalue, Q_FUNC_INFO);
                    }


                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }


                counter++;
            }
            ///----Apple metadata




            ///----Exif
            QStringList exif_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList).toStringList();
            bool bool_exif_value_exists = false;
            for(int i = 1 ; i <= exif_keys_list.size() ; i++)
            {
                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_exif_value_exists = true;
                    break;
                }
            }
            if(bool_exif_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_Metadata_TabName_Exif_Meatadta) + "</b><br>");


            for(int key_value_count = 0 ; key_value_count < exif_keys_list.size() ; key_value_count++)
            {
                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = exif_keys_list.at(key_value_count);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///----Exif



            ///------Mime types
            //            bool bool_mime_value_exists = false;
            //            for(int i = 0 ;i < report_generic_function_obj->html_report_file_system_Mime_key_list.size() ;i++)
            //            {
            //                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
            //                {
            //                    bool_mime_value_exists = true;
            //                    break;
            //                }
            //            }
            //            if(bool_mime_value_exists)
            //                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_MIME_Type) + "</b><br>");
            temp_data.append("<br>");
            for(int key_value_count = 0 ;key_value_count < report_generic_function_obj->html_report_file_system_Mime_key_list.size() ;key_value_count++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = report_generic_function_obj->html_report_file_system_Mime_key_list.at(key_value_count);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///------Mime types





            ///----signatures
            bool bool_signatures_value_exists = false;
            for(int i = 1 ; i <= report_generic_function_obj->html_report_file_system_signauture_key_list.size() ; i++)
            {
                if(!query_select.value(counter + col_d_name_list.size()).toString().isEmpty())
                {
                    bool_signatures_value_exists = true;
                    break;
                }
            }

            if(bool_signatures_value_exists)
                temp_data.append("<br><b>" + QString(MACRO_CASE_TREE_File_Signature_Parent) + "</b><br>");

            for(int sg = 0 ; sg < report_generic_function_obj->html_report_file_system_signauture_key_list.size() ; sg++)
            {

                QString our_value  = query_select.value(counter + col_d_name_list.size()).toString();
                QString key_name = report_generic_function_obj->html_report_file_system_signauture_key_list.at(sg);
                QString qvalue = our_value;

                title_name = "<b>" + key_name + " : </b>";
                qvalue = our_value;
                if(!our_value.trimmed().isEmpty())
                {
                    temp_data.append( "<b>" + key_name + " : </b>" + qvalue + "<br>");
                }

                counter++;
            }
            ///----signatures



            ///2.--start-- tags and Notes columns in report and also kept in csv
            temp_data.append("<br>");
            for(int x = 2 ; x > 0; x--)
            {

                QString rpt_data_type = data_type_list.at(col_d_name_list.size() - x);
                if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
                {

                    QString tag_value = query_select.value(col_d_name_list.size() - 2).toString();
                    if(tag_value.trimmed().isEmpty())
                        continue ;

                    QString tag_text = "";
                    QStringList list_tags = tag_value.split(",", Qt::SkipEmptyParts);

                    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                    {
                        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                    }

                    if(tag_text.endsWith(","))
                        tag_text.chop(1);

                    title_value =  tag_text ;

                    temp_data.append( "<b> Tag : </b>" + title_value + "<br>");

                }
                else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString)
                {
                    QString notes_value = query_select.value(col_d_name_list.size() - 1).toString();
                    if(notes_value.trimmed().isEmpty())
                        continue ;

                    title_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";
                    temp_data.append( "<b> Examiner Notes : </b>" + title_value + "<br>");
                }

            }
            ///2 --end-- tags and Notes column in report and also kept in csv

        }
        else
        {
            QString emlx_source_file_value;
            QString source_title_str;
            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            {
                QString emlx_file_name = QFileInfo(emlx_source_file).fileName();
                QString eml_dest_file_path = reports_dir_path + "/Files/" + emlx_file_name;
                if(emlx_file_name.endsWith(".emlx") || emlx_file_name.endsWith(".eml"))
                {
                    QString relative_path = "./Files/" + recon_helper_standard_obj->pub_get_html_formated_filename(emlx_file_name,Q_FUNC_INFO);

                    QString hyperlink = QString("<a href=") + "\"" + relative_path + "\"" + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10,scrollbars=yes');return false\">");
                    emlx_source_file_value = hyperlink + emlx_source_file + "</a>";
                    source_title_str = "<b> Source File : </b>";
                }
            }

            for(int y = 0 ; y < col_d_name_list.size() ; y++)
            {

                int ref_index = y;
                QString title_value;
                QString title_name;


                QString rpt_data_type = data_type_list.at(ref_index);
                QString rpt_data_type_for_tmpuse = rpt_data_type;

                QString our_column = col_d_name_list.at(ref_index);
                QString our_value = query_select.value(ref_index).toString();
                if(our_value.trimmed().isEmpty())
                    continue ;

                title_name = "<b>" + col_d_name_list.at(ref_index) + " : </b>";


                bool colmn_atr =  report_generic_function_obj->pub_is_column_of_apple_attribute(our_column);
                // hide extended attribute column which has no value
                if(colmn_atr)
                {
                    if(our_value.trimmed() == QString(""))
                        continue;
                }



                ///------------ Use Format as per CSV DataType
                if(rpt_data_type_for_tmpuse.endsWith("Hide"))
                {
                    rpt_data_type.remove("Hide");
                    if(our_value.trimmed().isEmpty())
                        continue;

                    if(rpt_data_type.startsWith("NewLine"))
                        rpt_data_type.remove("NewLine");

                }
                else if(rpt_data_type_for_tmpuse.startsWith("NewLine"))
                {
                    rpt_data_type.remove("NewLine");
                    temp_data.append("<br>");
                }
                else if(rpt_data_type_for_tmpuse.contains("_Person", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(7);
                }
                else if(rpt_data_type_for_tmpuse.contains("_Icon", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(5);
                }
                else if(rpt_data_type_for_tmpuse.contains("_WebIcon", Qt::CaseInsensitive))
                {
                    rpt_data_type.chop(QString("_WebIcon").size());
                }

                ///------------ Use Format as per CSV DataType


                ///Html Report*/
                int case_type_int = 0;
                if(rpt_data_type.contains("_"))
                {
                    case_type_int = rpt_data_type.split("_").at(0).toInt();
                }
                else
                    case_type_int = rpt_data_type.toInt();

                switch (case_type_int)
                {
                case MACRO_CSV_TEMPLATE_DATATYPE_RECORD_INT:
                {
                    record_number = our_value;
                    title_value = record_number;
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_INT:
                {
                    QString extended_attr = our_value;
                    if(extended_attr.trimmed() == QString(""))
                        continue;

                    extended_attr = extended_attr.trimmed();
                    extended_attr.replace("\n   ","<br>&nbsp;");
                    extended_attr.replace("\n","<br>&nbsp;");
                    if(extended_attr != "")
                    {
                        title_name = "";
                        title_value = QString("<b><u>" + QObject::tr("Bookmarked Apple Metadata") + ": </u></b><br>" + "\t" + extended_attr + "<br>" );
                    }

                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_TIME_INT:
                {
                    title_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO);
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_SIZE_INT:
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else if(value == QString("0"))
                        title_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        title_value = recon_static_functions::human_readable_size(our_value, Q_FUNC_INFO) + " (" + our_value + " bytes)";
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES_INT:
                {
                    title_value.clear();
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
                            title_name = (QString("<b> <u>" + our_column + "</u></b>"));
                            for(int k = 0 ; k< temp.size(); k++)
                            {
                                title_value.append("<br>");
                                title_value.append("<b>"+  QObject::tr("Date ") + ": </b>" + global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(k), Q_FUNC_INFO));
                            }

                        }
                    }
                    else
                    {
                        //continue;
                    }

                    break;

                }
                case MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_INT:
                {

                    QString str = our_value;

                    if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");

                    }
                    else if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                    {
                        str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");
                    }
                    str = str.replace("|","<br>");

                    title_value = str;

                    title_value.prepend("<br>");

                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_INT:
                {
                    QString str = our_value;

                    QStringList timestamps_splitList;

                    timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                    title_value.clear();
                    for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                    {
                        title_value.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");

                    }
                    break;

                }
                case MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_INT:
                {
                    if(!our_value.startsWith("/"))
                        our_value = our_value.prepend("/");

                    our_value.replace(";/","<br>/");

                    our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>/");

                    title_value = our_value;
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_NOTES_INT:
                {

                    QString notes_value = query_select.value(ref_index).toString();
                    if(notes_value.trimmed().isEmpty())
                    {
                        title_name.clear();
                        continue ;
                    }
                    else
                        title_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";

                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_RECON_VIEWERS_TAG_EXPORTED_DATA_INT:
                {
                    if(get_plugin_name() == QString(MACRO_Plugin_Name_Hex_Viewer) || get_plugin_name() == QString(MACRO_Plugin_Name_Registry_Viewer))
                        title_value = report_generic_function_obj->pub_get_hex_viewer_format_for_html_report(our_value);
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_INT:
                {
                    title_value = "<font color = \"red\">" + our_value.toHtmlEscaped() + "</font>";
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_TAG_INT:
                {
                    temp_data.append("<br>");

                    QString tag_text = "";
                    QStringList list_tags = our_value.split(",", Qt::SkipEmptyParts);
                    for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                    {
                        QString colour = recon_helper_standard_obj->get_colour_name_of_tag_from_db(list_tags.at(tag_i) , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                        tag_text += "<font color = \"" + recon_helper_standard_obj->get_color_hex_from_color_name(colour , narad_muni_for_report_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                    }

                    if(tag_text.endsWith(","))
                        tag_text.chop(1);

                    title_value =  tag_text ;
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_VIRTUAL_EXPORT_IMAGE_PATH_INT:
                {
                    if(bool_virtual_datatype_exist)
                    {
                        QString virtual_image_file_path = report_generic_function_obj->pub_export_virtual_file(our_value , get_plugin_name() , destination_file_location , reports_dir_path);
                        exported_file_preview = get_virtual_preview_file(virtual_image_file_path);
                    }
                    title_name.clear();

                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_INT:
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                    {
                        title_value = our_value.toHtmlEscaped();

                    }
                    else
                    {
                        title_name.clear();
                    }
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_INT:
                {
                    if(!our_value.startsWith("\n"))
                        our_value.replace("\n" , "<br>");

                    title_value.append("<br>" + our_value + "<br>");
                    break;
                }
                case MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_INT:
                {
                    QString value =  our_value;
                    if(value == QString(""))
                        title_value = "";
                    else
                    {
                        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                        QString str_val = our_value;
                        str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                        title_value = str_val.trimmed();

                    }
                    break;
                }
                default :
                {
                    title_value = our_value.toHtmlEscaped();
                }
                }

                ////start -----append  column name and value
                temp_data.append(title_name + title_value + "<br>");
                ////start -----append  column name and value
            }

            temp_data.append("<br>" + source_title_str + emlx_source_file_value + "<br>");
            emlx_source_file_value.clear();
            source_title_str.clear();

            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display)
            {
                QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
                if(!custom_col_name_list.isEmpty())
                {
                    if(is_export_available == QString::number(1) && tab_export_checked == true)
                    {
                        QString extended_list = recon_helper_standard_obj->get_apple_metadata_key_value_from_result(record_number, table_name, destination_db_file, Q_FUNC_INFO);

                        if(!extended_list.isEmpty())
                        {
                            temp_data += "<br><br><b><u><i>More Apple Metadata</u></i></b>"+ extended_list;
                        }
                    }
                }
            }


        }



        ///1.------
        if(!bool_multiple_export)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == true))
            {
                temp_data.append(exported_file_detail);
            }
        }
        ///1.------



        ///2.------
        tabledata << temp_data;
        ///2.------


        ///3.------
        if(bool_virtual_datatype_exist)
        {
            tabledata << exported_file_preview;
        }
        ///3.------



        ///--------report data
        if(!bool_multiple_export)
        {
            if((is_export_available == QString("1")) && (tab_export_checked == true))
            {
                tabledata  << exported_file_preview;
            }
            report_data +=   report_template_html_standard::html_table_row_data(tabledata,0);
        }
        else if(bool_multiple_export && (is_export_available == QString("1")) && (tab_export_checked == true))
        {
            QString table_crt;

            QStringList multiple_export_filedetail ;
            multiple_export_filedetail = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filedetail_QStringList).toStringList();

            QStringList multiple_export_flepreview;
            multiple_export_flepreview = narad_muni_for_report_obj->get_field(MACRO_NARAD_Export_HTML_Filepreview_QStringList).toStringList();

            for(int pp = 0; pp < multiple_export_filedetail.size(); pp++)
            {

                if(multiple_export_filedetail.at(pp).trimmed() == "")
                    continue;

                table_crt +=
                        "<table  border = \"0\" border-color = \"RED\" width = \"100%\">"
                        "<tr>"
                        "<td width = \"80%\">" + multiple_export_filedetail.at(pp)  + "</td>"
                                                                                      "<td align=\"center\">" + multiple_export_flepreview.at(pp) + "</td>"
                                                                                                                                                    "</tr>"
                                                                                                                                                    "</table>";
            }

            QString temp_data;
            for(int i = 0; i < tabledata.size(); i++)
            {
                if(!bool_virtual_datatype_exist)
                {
                    if(i == tabledata.size() - 1)
                        temp_data +=  QString("<td>" + tabledata.at(i) + table_crt + "</td>");
                    else
                        temp_data +=  QString("<td>" + tabledata.at(i) + "</td>");
                }
                else
                {
                    if(i == 0 || i == tabledata.size() - 1)
                    {
                        temp_data +=  QString("<td>" + tabledata.at(i) + "</td>");
                    }
                    else
                        temp_data +=  QString("<td>" + tabledata.at(i) + table_crt + "</td>");
                }
            }


            temp_data += QString("<tr>");
            report_data += temp_data;
        }
        else
        {
            report_data +=   report_template_html_standard::html_table_row_data(tabledata,0);
        }
        ///--------report data
        html_report_file.write(report_data.toLocal8Bit().data());

        report_data.clear();
    }
}

