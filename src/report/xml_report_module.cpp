#include "xml_report_module.h"

xml_report_module::xml_report_module(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    report_generic_function_obj = new report_generic_function(this);
    export_file_obj = new export_file_module(this);
    bool_cancel_report = false;

}


void xml_report_module::pub_cancel_report()
{
    bool_cancel_report = true;
    export_file_obj->pub_cancel_export();

}

void xml_report_module::set_essentials()
{
    bool_cancel_report = false;

    fill_variable_from_narad_muni();

    report_generic_function_obj->pub_set_essentials();
    export_file_obj->set_essentials();

}
void xml_report_module::set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
    report_generic_function_obj->pub_set_narad_muni_for_report(mobj);
    export_file_obj->set_narad_muni_for_report(mobj);

}

void xml_report_module::clear_exportfilelist_table()
{
    export_file_obj->set_export_entry_null();
}

void xml_report_module::fill_variable_from_narad_muni()
{

    table_export_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList).toStringList();
    table_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList).toStringList();
    table_export_criteria_list_tablewidget   = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList).toStringList();

    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();
    module_name_str  = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();

    table_xml_report_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_XML_Report_Available_Tablewidget_QStringList).toStringList();

    tab_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList).toStringList();
    table_artifact_common_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Artifact_Common_Tablewidget_QStringList).toStringList();


    list_m_tablewidgets.clear();
    QList<QVariant> screen_tablewidget_list_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList).toList();
    for(int i=0; i < screen_tablewidget_list_list.size(); i++)
    {
        qint64 nn = screen_tablewidget_list_list.at(i).toLongLong();
        list_m_tablewidgets.append((m_tablewidget *) nn);
    }

    rt_xml_col_name_list.clear();
    QList<QVariant> xml_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_XML_Col_Name_QList).toList();
    for(int i=0; i < xml_col_name_list.size(); i++)
        rt_xml_col_name_list.append(xml_col_name_list.at(i).toStringList());


    rt_xml_data_type_list.clear();
    QList<QVariant> xml_data_type_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_XML_Data_Type_QList).toList();
    for(int i=0; i < xml_data_type_list.size(); i++)
        rt_xml_data_type_list.append(xml_data_type_list.at(i).toStringList());

    rt_xml_d_name_list.clear();
    QList<QVariant> xml_d_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_XML_Display_Name_QList).toList();
    for(int i=0; i < xml_d_name_list.size(); i++)
        rt_xml_d_name_list.append(xml_d_name_list.at(i).toStringList());

    rt_xml_order_list.clear();
    QList<QVariant> xml_order_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_XML_Order_QList).toList();
    for(int i=0; i < xml_order_list.size(); i++)
        rt_xml_order_list.append(xml_order_list.at(i).toStringList());


    tw_data_type_list_tablewidget.clear();
    QList<QVariant> tw_data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList).toList();
    for(int i=0; i < tw_data_type_tablewidget_list.size(); i++)
        tw_data_type_list_tablewidget.append(tw_data_type_tablewidget_list.at(i).toStringList());

}

void xml_report_module::create_tablewidget_xml_report(QXmlStreamWriter *stream_xml, QString selected_tab_name, QString report_scope, QStringList selected_tags_list,bool tab_export_checked)
{
    for(int i_tab = 0 ; i_tab < table_name_list_tablewidget.size() ; i_tab++)
    {
        if(selected_tab_name != tab_name_list_tablewidget.at(i_tab))
            continue;

        QString is_report_available = table_xml_report_available_list_tablewidget.at(i_tab);
        if(is_report_available != QString("1"))
            continue;

        QString table_name = table_name_list_tablewidget.at(i_tab);
        QString tab_name = tab_name_list_tablewidget.at(i_tab);




        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {

            stream_xml->setAutoFormatting(true);
            stream_xml->writeStartElement(tab_name.replace(" ","_"));

            //            QString is_common_artifact = table_artifact_common_list_tablewidget.at(i_tab);
            //            if(is_common_artifact == QString("1"))
            //            {
            //                stream_xml->writeStartElement(QObject::tr("Artifacts_Source"));
            //                QStringList source_list = report_generic_function_obj->get_artifacts_source_list(table_name, report_scope,selected_tags_list);
            //                for(int cc=0; cc < source_list.size(); cc++)
            //                {
            //                    stream_xml->writeTextElement(QObject::tr("Source"), source_list.at(cc));
            //                }
            //                stream_xml->writeEndElement();
            //            }




            QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
            QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
            QStringList cols_list = rt_xml_col_name_list.at(i_tab);
            bool bool_multiple_export = false;
            if(is_multiple_export == "2")
                bool_multiple_export = true;

            int sr_no = 0;
            for(int db_count = 0 ; db_count < destination_db_file_list.size() ;db_count++)
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


                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    QString command = "select " + cols_list.join(",") + ", source_count_name  from '" + table_name + "'";
                    set_data_xml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export, 0,stream_xml,db_path,dest_db);
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {


                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, db_path, selected_tags_list, Q_FUNC_INFO);
                    if(get_plugin_name() == QString(MACRO_Plugin_Name_File_System))
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
                            QString fs_record_number = total_record_list.at(i);
                            QString command = report_generic_function_obj->pub_get_fs_command(fs_record_number);
                            set_data_xml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export,sr_no,stream_xml,db_path , dest_db);
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
                        for(int i = 0; i < total_record_list.size(); i++)
                        {
                            QString  command;
                            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
                            {
                                command = QString("select " + cols_list.join(",") + ", source_file , source_count_name  from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                            }
                            else
                            {
                                command = QString("select " + cols_list.join(",") + ", source_count_name  from '" + table_name + "' where INT=" + "'" + total_record_list.at(i) + "'");
                            }
                            set_data_xml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export,sr_no,stream_xml,db_path , dest_db);
                            sr_no++;
                        }
                    }
                }
                dest_db.close();
            }
            stream_xml->writeEndElement();//end tab
        }

        if(selected_tab_name == MACRO_Tag_Name_Examiner_Comments)
        {
            if(report_generic_function_obj->pub_is_data_available_in_examiner_notes())
            {
                QString examiner_notes_html_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "Examiner_Notes.html";
                QFile examiner_notes_html_QFile;
                examiner_notes_html_QFile.setFileName(examiner_notes_html_file_path);

                if(!examiner_notes_html_QFile.open(QFile::ReadOnly))
                {
                    recon_static_functions::app_debug(" File Open Examiner Notes Html -----FAILED-----" + examiner_notes_html_QFile.fileName(), Q_FUNC_INFO);
                    recon_static_functions::app_debug(" Error:" + examiner_notes_html_QFile.errorString(), Q_FUNC_INFO);
                    return;
                }

                examiner_notes_html_QFile.seek(0);

                QString read_examiner_notes_html_file = examiner_notes_html_QFile.readAll();

                QString examiner_notes_data;

                QTextDocument doc;

                doc.setHtml(read_examiner_notes_html_file);

                examiner_notes_data = doc.toPlainText();

                QStringList examiner_notes_data_list = examiner_notes_data.split("\n", Qt::SkipEmptyParts);

                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString || report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    stream_xml->setAutoFormatting(true);
                    stream_xml->writeStartElement(tab_name.replace(" ","_"));

                    for(int ii = 0; ii < examiner_notes_data_list.size(); ii++)
                    {
                        stream_xml->writeTextElement(QString("Notes").toLocal8Bit().data(), examiner_notes_data_list.at(ii).toLocal8Bit().data());
                    }

                    stream_xml->writeEndElement();
                }
            }
        }
    }
}


void xml_report_module::create_full_report_xml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open -----FAILED---" + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" error" + file.errorString() , Q_FUNC_INFO);
        return;
    }


    QXmlStreamWriter stream_xml(&file);

    stream_xml.writeStartElement(get_plugin_name().replace(" ","_"));

    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list))
    {
        stream_xml.writeTextElement("Data","No Data Available");
        stream_xml.writeEndElement();
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


        create_tablewidget_xml_report(&stream_xml, tabname,MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list,bool_exp);

    }



    stream_xml.writeEndElement();
    file.flush();
    file.close();
    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);
    return;

}

void xml_report_module::create_tag_report_xml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open -----FAILED---" + file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" error" + file.errorString() , Q_FUNC_INFO);
        return;
    }


    QXmlStreamWriter stream_xml(&file);

    stream_xml.writeStartElement(get_plugin_name().replace(" ","_"));

    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list))
    {
        stream_xml.writeTextElement("Data","No Data Available");
        stream_xml.writeEndElement();
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


        create_tablewidget_xml_report(&stream_xml, tabname,MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list,bool_exp);

    }



    stream_xml.writeEndElement();
    file.close();
    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);
    return;

}

void xml_report_module::set_data_xml(int i_tab, QString command,QString table_name, bool tab_export_checked,QString is_export_available, bool bool_multiple_export,int count, QXmlStreamWriter *stream_xml,  QString destination_db_file , QSqlDatabase &dest_db)
{

    QSqlQuery query_select(dest_db);
    query_select.prepare(command);
    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" - select query -----FAILED---" + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" - QUEREY " + query_select.executedQuery() , Q_FUNC_INFO);
        dest_db.close();
        return;
    }


    QStringList data_type_list = rt_xml_data_type_list.at(i_tab);
    QStringList col_d_list = rt_xml_d_name_list.at(i_tab);

    QString tab_name = tab_name_list_tablewidget.at(i_tab);
    stream_xml->setAutoFormatting(true);
    QString tab_detail = tab_name.replace(" ", "_") + "_Detail";
    stream_xml->writeStartElement(tab_detail);


    QStringList xml_custom_col_report_list;
    if(get_plugin_name() == QString(MACRO_Plugin_Name_File_System))
    {
        QStringList apple_columns_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList).toStringList();
        QStringList exif_columns_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList).toStringList();
        QStringList custom_header_col;

        custom_header_col << "Bookmarked Apple MetaData" << apple_columns_keys_list << exif_columns_keys_list
                          << MACRO_CASE_TREE_MIME_Type << "Signature Value" << "Signature Name" << "Bookmarked Exif Data";

        xml_custom_col_report_list << custom_header_col;
    }


    while(query_select.next())
    {
        if(bool_cancel_report)
            break ;

        count++;
        QString num = QString::number(count);
        stream_xml->writeTextElement(QObject::tr("Sr._No."), num);

        emlx_source_file.clear();
        if(get_plugin_name() == QString(MACRO_Plugin_Name_Email_Parser))
            emlx_source_file = query_select.value(12).toString();

        QString record_number;

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());
        stream_xml->writeTextElement(QString("Source_Name").toLocal8Bit().data(), struct_info.complete_source_name);

        if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            stream_xml->writeTextElement(QString(MACRO_Generic_Display_Name).replace(" " , "_").toLocal8Bit().data(),struct_info.source_name);
        }


        if(get_plugin_name() == QString(MACRO_Plugin_Name_File_System))
        {

            int apple_metadata_col_name_count = 0;
            int apple_metadata_qry_offset = col_d_list.size();
            for(int i = 0 ; i < col_d_list.size() - 2 ; i++)
            {
                QString tag = col_d_list.at(i);
                if(tag.contains(" "))
                    tag.replace(" ", "_");

                QString value = query_select.value(i).toString();
                if(value.trimmed().isEmpty())
                    continue ;

                QString m_datatype = data_type_list.at(i);

                if(m_datatype.endsWith("Hide"))
                {
                    m_datatype.remove("Hide");
                    if(value.trimmed().isEmpty())
                        continue ;
                }

                if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = value;
                    stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(value, Q_FUNC_INFO);
                    stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    if(value != QString(""))
                    {
                        stream_xml->writeStartElement(tag);
                        QStringList temp;
                        QString temp_value = query_select.value(i).toString();
                        if(temp_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                        {
                            temp = temp_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        }
                        else
                        {
                            temp = temp_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                        }

                        for(int v =0 ; v < temp.size(); v++)
                        {
                            stream_xml->writeTextElement(QObject::tr("Date "), global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(v), Q_FUNC_INFO));
                        }

                        stream_xml->writeEndElement();
                    }
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {

                    if(value == QString(""))
                        value = "";
                    else if(value == QString("0"))
                        value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                    else
                        value = (recon_static_functions::human_readable_size(value, Q_FUNC_INFO) + " (" + value + " bytes)");

                    stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                    {
                        stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                    }
                    else
                    {
                        continue ;
                    }
                }
                else
                {
                    stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                }

            }

            for(int custm_col_index = apple_metadata_qry_offset ; custm_col_index < xml_custom_col_report_list.size() + apple_metadata_qry_offset; custm_col_index++)
            {

                QString tag = xml_custom_col_report_list.at(apple_metadata_col_name_count);
                if(tag.contains(" "))
                    tag.replace(" ", "_");
                QString value = query_select.value(custm_col_index).toString();

                // QString column_name = query_select.record().fieldName(apple_metadata_col_name_count);
                QString column_name = query_select.record().fieldName(custm_col_index);

                if(column_name.contains("Date", Qt::CaseInsensitive))
                {
                    if(!value.trimmed().isEmpty())
                    {
                        value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(value, Q_FUNC_INFO);
                        stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                    }
                }
                else
                {
                    if(!value.trimmed().isEmpty())
                        stream_xml->writeTextElement(tag.toLocal8Bit().data(), value);
                }

                apple_metadata_col_name_count++;
            }




            if(data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString))
            {
                QString tag_value = query_select.value("recon_tag_value").toString();
                if(!tag_value.trimmed().isEmpty())
                    stream_xml->writeTextElement(QString("Tag").replace(" ","_").toLocal8Bit().data(), tag_value);
            }

            if(data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString))
            {
                QString notes_value = query_select.value("notes").toString();
                if(!notes_value.trimmed().isEmpty())
                {
                    stream_xml->writeTextElement(QString("Examiner_Notes").toLocal8Bit().data(), notes_value);
                }
            }

            if(is_export_available == QString::number(1) && tab_export_checked == true)
            {
                set_export_detail_xml(dest_db, stream_xml, query_select.value(0).toString(), table_name);
            }


        }
        else
        {

            QString emlx_source_col_name;
            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            {
                emlx_source_file_value = emlx_source_file;
                emlx_source_col_name    = "Source_File";
            }

            for(int i = 0 ; i < col_d_list.size() ; i++)
            {
                QString column_name = col_d_list.at(i);
                if(column_name.contains(" "))
                    column_name.replace(" ", "_");

                QString column_value = query_select.value(i).toString();
                if(column_value.trimmed().isEmpty())
                    continue ;

                QString m_datatype = data_type_list.at(i);

                QString our_column = column_name;
                our_column = our_column.replace("_"," ");
                bool colmn_atr =  report_generic_function_obj->pub_is_column_of_apple_attribute(our_column);

                // hide extended attribute column which has no value
                if(colmn_atr)
                {
                    if(column_value.trimmed() == QString(""))
                        continue;
                }

                if(m_datatype.endsWith("Hide"))
                {
                    m_datatype.remove("Hide");
                    if(column_value.trimmed().isEmpty())
                        continue ;
                }


                if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = column_value;
                    stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString)
                {
                    stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    column_value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(column_value, Q_FUNC_INFO);
                    stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    if(column_value == QString(""))
                        column_value = "";
                    else if(column_value == QString("0"))
                        column_value =  recon_static_functions::human_readable_size(column_value, Q_FUNC_INFO);
                    else
                        column_value = (recon_static_functions::human_readable_size(column_value, Q_FUNC_INFO) + " (" + column_value + " bytes)");

                    stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    if(column_value != QString(""))
                    {
                        stream_xml->writeStartElement(column_name);
                        QStringList temp;
                        if(column_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                        {
                            temp = column_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        }
                        else
                        {
                            temp = column_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                        }

                        for(int v =0 ; v < temp.size(); v++)
                        {
                            stream_xml->writeTextElement(QObject::tr("Date "), global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(v), Q_FUNC_INFO));
                        }

                        stream_xml->writeEndElement();
                    }
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
                {
                    stream_xml->writeStartElement(column_name.toLocal8Bit().data());

                    QString str = query_select.value(i).toString();

                    QStringList timestamps_splitList;

                    timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                    for(int tm_loop = 0 ; tm_loop < timestamps_splitList.size(); tm_loop++)
                    {
                        QString date = timestamps_splitList.at(tm_loop);
                        date = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(date, Q_FUNC_INFO);
                        stream_xml->writeTextElement(QObject::tr("Info"), date);
                    }

                    stream_xml->writeEndElement();
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
                {
                    stream_xml->writeStartElement(column_name.toLocal8Bit().data());

                    QString str = query_select.value(i).toString();

                    if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        QStringList temp = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        for(int v =0 ; v < temp.size(); v++)
                        {
                            QString str = temp.at(v);
                            stream_xml->writeTextElement(QObject::tr("Info"), str);
                        }
                    }
                    else
                    {
                        QStringList temp;
                        if(str.contains(";;"))
                            temp = str.split(";;", Qt::SkipEmptyParts);
                        else
                        {
                            temp = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                        }

                        for(int v =0 ; v < temp.size(); v++)
                        {
                            QString str = temp.at(v);
                            str = str.replace(":- ;",":- ");

                            str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,", ");

                            //                                    str = str.replace(";",", ");
                            //                                   QString("<![CDATA[" + str + "<br />]]");

                            stream_xml->writeTextElement(QObject::tr("Info"), str.replace("|","\t"));

                        }
                    }

                    stream_xml->writeEndElement();
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
                {
                    stream_xml->writeStartElement(column_name.toLocal8Bit().data());

                    QString str_val = query_select.value(i).toString();
                    if(!str_val.startsWith("/"))
                        str_val = str_val.prepend("/");

                    QStringList temp;
                    if(str_val.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        temp = str_val.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        for(int v =0 ; v < temp.size(); v++)
                        {
                            QString str = temp.at(v);
                            if(!str.startsWith("/"))
                                str = str.prepend("/");

                            stream_xml->writeTextElement(QObject::tr("Source"), str);
                        }
                    }
                    else if(str_val.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                    {
                        temp = str_val.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                        for(int v =0 ; v < temp.size(); v++)
                        {
                            QString str = temp.at(v);
                            if(!str.startsWith("/"))
                                str = str.prepend("/");

                            stream_xml->writeTextElement(QObject::tr("Source"), str);
                        }

                    }
                    else
                    {
                        stream_xml->writeTextElement(column_name.toLocal8Bit().data(), str_val);
                    }

                    stream_xml->writeEndElement();
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
                {

                    if(!column_value.trimmed().isEmpty())
                    {
                        stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                    }

                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                    {
                        stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                    }
                    else
                    {
                        continue ;
                    }
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_QString)
                {
                    if(!column_value.trimmed().isEmpty())
                        stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                }
                else if(m_datatype == MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString)
                {
                    if(column_value == QString(""))
                        column_value = "";
                    else
                    {
                        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                        QString str_val = column_value;
                        str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                        stream_xml->writeTextElement(column_name.toLocal8Bit().data(), str_val);

                    }
                }
                else
                {
                    stream_xml->writeTextElement(column_name.toLocal8Bit().data(), column_value);
                }
            }

            if(!emlx_source_col_name.isEmpty() && !emlx_source_file_value.isEmpty())
            {
                stream_xml->writeTextElement(emlx_source_col_name.toLocal8Bit().data(),emlx_source_file_value);
                emlx_source_col_name.clear();
                emlx_source_file_value.clear();
            }

            if(struct_info.os_scheme_display == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
            {
                QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
                if(!custom_col_name_list.isEmpty())
                {
                    if(is_export_available == QString::number(1) && tab_export_checked == true)
                    {
                        QString extended_attr = get_apple_metadata_key_value_for_xml_report(record_number,table_name,destination_db_file);
                        if(!extended_attr.isEmpty())
                        {
                            write_users_apple_metadata_in_xml_file(extended_attr, stream_xml);
                        }
                    }
                }
            }

            if(is_export_available == QString::number(1) && tab_export_checked == true)
                set_export_detail_xml(dest_db, stream_xml, query_select.value(0).toString(), table_name);

        }
    }

    stream_xml->writeEndElement();
}

void xml_report_module::set_export_detail_xml(QSqlDatabase dest_db,  QXmlStreamWriter *stream_xml,QString column, QString table_name)
{
    stream_xml->writeStartElement(QObject::tr("Exported_File_Detail"));

    QSqlQuery query_export_detail(dest_db);
    QString cmnd = QString("select exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time from exportfilelist where (table_name = '") + table_name + "' AND record = " + column + ")";
    if(!query_export_detail.exec(cmnd))
    {
        recon_static_functions::app_debug(" - query3333333333333333 ----FAILED---- query_export_detail" + query_export_detail.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Error " + query_export_detail.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(" -Query " + query_export_detail.executedQuery() , Q_FUNC_INFO);
    }

    bool bool_export_detail = false;
    while(query_export_detail.next())
    {

        //                        QString f_name = query_export_detail.value(1).toString().trimmed();
        QString f_relative_path = QString::fromUtf8(query_export_detail.value(0).toString().toLocal8Bit().data());
        QString f_size = recon_static_functions::human_readable_size(query_export_detail.value(1).toString(), Q_FUNC_INFO)
                + " (" + query_export_detail.value(1).toString() + QObject::tr(" bytes") + ")";
        QString f_md5_string  = query_export_detail.value(2).toString();
        QString f_sha1_string = query_export_detail.value(3).toString();
        //QString f_export_time = query_export_detail.value(4).toString();
        QString f_export_time = QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString());

        QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
        QString numeric_timezone_value;
        if(machine_timezone_string.contains("GMT"))
        {
            QStringList tmp_list = machine_timezone_string.split("GMT");
            numeric_timezone_value = tmp_list.at(1);
        }

        if(f_relative_path != QString(""))
        {
            bool_export_detail = true;
            stream_xml->writeStartElement(QObject::tr("File_Detail"));
            //                            stream.writeTextElement(QObject::tr("File_Name"), f_name);
            stream_xml->writeTextElement(QObject::tr("New_Path"), f_relative_path);
            stream_xml->writeTextElement(QObject::tr("Size"), f_size);
            stream_xml->writeTextElement(QObject::tr("MD5"), f_md5_string);
            stream_xml->writeTextElement(QObject::tr("SHA1"), f_sha1_string);
            stream_xml->writeTextElement(QObject::tr("Export_Time"), f_export_time + " "+ numeric_timezone_value);
            stream_xml->writeEndElement();
        }
    }

    if(bool_export_detail)
        stream_xml->writeEndElement();
    else
    {
        stream_xml->writeTextElement(QObject::tr("sr."),QObject::tr("File_does_not_exist"));
        stream_xml->writeEndElement();
    }

}

QString xml_report_module::get_plugin_name()
{
    return module_name_str;
}

void xml_report_module::write_users_apple_metadata_in_xml_file(QString extended_attr_str, QXmlStreamWriter *stream_xml)
{
    QStringList attr_list_for_files = extended_attr_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(attr_list_for_files.size() <= 0)
        return;

    stream_xml->writeStartElement(QObject::tr("More_Apple_Metadata"));

    int count = 1;
    for(int i = 0; i < attr_list_for_files.size(); i++)
    {
        if(attr_list_for_files.at(i).isEmpty())
            continue;

        QString file_name = QString("File_") + QString::number(count);
        stream_xml->writeStartElement(QObject::tr(file_name.toLocal8Bit().data()));

        count++;
        QString attr_str_for_one_file = attr_list_for_files.at(i);

        QStringList attr_list_for_multiple_keys = attr_str_for_one_file.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);

        for(int j = 0; j < attr_list_for_multiple_keys.size(); j++)
        {
            QString one_key_value_pair = attr_list_for_multiple_keys.at(j);

            if(one_key_value_pair.isEmpty())
                continue;

            QStringList key_value_splitted_list = one_key_value_pair.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);

            if(key_value_splitted_list.size() < 2)
                continue;

            QString key = key_value_splitted_list.at(0);

            key.replace(" ","_");

            QString value = key_value_splitted_list.at(1);

            stream_xml->writeTextElement(key.toLocal8Bit().data(), value);
        }

        stream_xml->writeEndElement();
    }

    stream_xml->writeEndElement();
}


void xml_report_module::create_screen_report_xml(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

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


    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QXmlStreamWriter stream_xml(&file);


    stream_xml.writeStartElement(get_plugin_name().replace(" ","_"));

    //=============== is data available start ==================//
    bool bool_data_available = false;
    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        bool_data_available = true;
    }
    if(!bool_data_available)
    {
        stream_xml.writeTextElement("Data","No Data Available");
        stream_xml.writeEndElement();
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


    stream_xml.setAutoFormatting(true);
    stream_xml.writeStartElement(tab_nam.replace(" ","_"));
    //    QString is_common_artifact = table_artifact_common_list_tablewidget.at(i_tab);
    //    if(is_common_artifact == QString("1"))
    //    {
    //        stream_xml.writeStartElement(QObject::tr("Artifacts_Source"));
    //        QStringList source_list = report_generic_function_obj->get_artifacts_source_list(table_name, MACRO_REPORT_SCOPE_SCREEN_QString,QStringList(""));
    //        for(int cc=0; cc < source_list.size(); cc++)
    //        {
    //            stream_xml.writeTextElement(QObject::tr("Source"), source_list.at(cc));

    //        }
    //        stream_xml.writeEndElement();
    //    }

    //    QStringList headers;
    //    headers = header_str.split("::", Qt::SkipEmptyParts);






    QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
    QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
    bool bool_multiple_export = false;
    if(is_multiple_export == "2")
        bool_multiple_export = true;

    QStringList cols_list = rt_xml_col_name_list.at(i_tab);
    QStringList data_type_list = rt_xml_data_type_list.at(i_tab);
    QStringList col_d_name_list = rt_xml_d_name_list.at(i_tab);
    QStringList col_order_list = rt_xml_order_list.at(i_tab);

    QStringList col_type_list = tw_data_type_list_tablewidget.at(i_tab);
    int INT_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString INT_str;


    int count = 0;
    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        for(int db_count = 0 ; db_count < destination_db_file_list.size() ;db_count++)
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

            INT_str = list_m_tablewidgets.at(i_tab)->item(i, INT_index)->text();
            QString command = "select " + cols_list.join(",") + ",source_count_name , source_file from '" + table_name  + "' where INT = " + INT_str ;
            set_data_xml(i_tab,command,table_name, tab_export_checked,is_export_available,bool_multiple_export, count,&stream_xml,db_path , dest_db);
            count++;

            dest_db.close();
        }

    }

    stream_xml.writeEndElement();//end tab
    stream_xml.writeEndElement();//end module

    file.close();


    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);
}


QString xml_report_module::get_apple_metadata_key_value_for_xml_report(QString record_no, QString table_name, QString db_path)
{
    QString command = "select extended_attributes_key_value from '" + table_name + "' where INT = ?" ;
    QStringList list_bind;
    list_bind << record_no;

    QString key_value_pair = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,list_bind,0,db_path,Q_FUNC_INFO);

    QXmlStreamReader xml(key_value_pair.toLocal8Bit());

    QString key_value_data;

    while(!xml.atEnd())
    {
        xml.readNext();

        if(xml.isStartElement())
        {
            if(xml.name() == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Extended_Attributes_Key_Value_xml_header))
            {
                xml.readNext();
            }
            else if(xml.name() == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Extended_Attributes_Key_Value_Artifact_Source_header))
            {
                while(!xml.atEnd())
                {
                    if(xml.isStartElement())
                    {
                        if(xml.name() == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Extended_Attributes_Key_Value_Artifact_Source_header))
                        {
                            key_value_data += QString(MACRO_RECON_Splitter_1_multiple);
                            xml.readNext();
                            continue;
                        }

                        QString key_name = xml.name().toString();
                        key_name.replace("_"," ");

                        QString value = xml.readElementText();
                        value.replace("\n","");

                        key_value_data += key_name;
                        key_value_data += QString(MACRO_RECON_Splitter_2_hash_in_brace);
                        key_value_data += value;

                        key_value_data += QString(MACRO_RECON_Splitter_3_dollar_in_brace);

                        xml.readNext();
                    }
                    else
                        xml.readNext();
                }
            }
            else
                xml.readNext();
        }
    }

    return key_value_data;
}
