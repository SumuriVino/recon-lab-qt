#include "csv_report_module.h"

csv_report_module::csv_report_module(QObject *parent)
{

    recon_helper_standard_obj = new recon_helper_standard(this);

    report_generic_function_obj = new report_generic_function(this);
    export_file_obj = new export_file_module(this);
    bool_cancel_report = false;
    bool_os_scheme_ios_found = false;


}


void csv_report_module::pub_cancel_report()
{
    bool_cancel_report = true;
    export_file_obj->pub_cancel_export();

}

void csv_report_module::set_essentials()
{
    bool_cancel_report = false;

    fill_variable_from_narad_muni();
    report_generic_function_obj->pub_set_essentials();
    export_file_obj->set_essentials();

}
void csv_report_module::set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
    report_generic_function_obj->pub_set_narad_muni_for_report(mobj);
    export_file_obj->set_narad_muni_for_report(mobj);
}



void csv_report_module::clear_exportfilelist_table()
{
    export_file_obj->set_export_entry_null();
}

void csv_report_module::fill_variable_from_narad_muni()
{

    table_export_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Available_Tablewidget_QStringList).toStringList();
    table_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList).toStringList();

    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();
    plugin_name_str  = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();
    table_csv_report_available_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_CSV_Report_Available_Tablewidget_QStringList).toStringList();
    tab_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tab_Name_Tablewidget_List_QStringList).toStringList();
    parent_module_name_str = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Parent_Module_Name_QString).toString();
    table_artifact_common_list_tablewidget   = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Artifact_Common_Tablewidget_QStringList).toStringList();
    table_export_criteria_list_tablewidget   = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Export_Criteria_Tablewidget_QStringList).toStringList();



    list_m_tablewidgets.clear();
    QList<QVariant> screen_tablewidget_list_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList).toList();
    for(int i=0; i < screen_tablewidget_list_list.size(); i++)
    {
        qint64 nn = screen_tablewidget_list_list.at(i).toLongLong();
        list_m_tablewidgets.append((m_tablewidget *) nn);
    }

    rt_csv_col_name_list.clear();
    QList<QVariant> csv_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_CSV_Col_Name_QList).toList();
    for(int i=0; i < csv_col_name_list.size(); i++)
        rt_csv_col_name_list.append(csv_col_name_list.at(i).toStringList());


    rt_csv_data_type_list.clear();
    QList<QVariant> csv_data_type_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_CSV_Data_Type_QList).toList();
    for(int i=0; i < csv_data_type_list.size(); i++)
        rt_csv_data_type_list.append(csv_data_type_list.at(i).toStringList());



    rt_csv_d_name_list.clear();
    QList<QVariant> csv_d_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_CSV_Display_Name_QList).toList();
    for(int i=0; i < csv_d_name_list.size(); i++)
        rt_csv_d_name_list.append(csv_d_name_list.at(i).toStringList());


    rt_csv_order_list.clear();
    QList<QVariant> csv_order_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_CSV_Order_QList).toList();
    for(int i=0; i < csv_order_list.size(); i++)
        rt_csv_order_list.append(csv_order_list.at(i).toStringList());


    tw_data_type_list_tablewidget.clear();
    QList<QVariant> data_type_tablewidget_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList).toList();
    for(int i=0; i < data_type_tablewidget_list.size(); i++)
        tw_data_type_list_tablewidget.append(data_type_tablewidget_list.at(i).toStringList());

}

void csv_report_module::create_table_widget_csv_report(QTextStream &stream_csv,QString selected_tab_name, QString report_scope, QStringList selected_tags_list, bool tab_export_checked)
{
    for(int i_tab = 0 ; i_tab < table_name_list_tablewidget.size() ; i_tab++)
    {
        if(selected_tab_name != tab_name_list_tablewidget.at(i_tab))
            continue;


        QString is_report_available = table_csv_report_available_list_tablewidget.at(i_tab);
        if(is_report_available != QString("1"))
            continue;


        QString table_name = table_name_list_tablewidget.at(i_tab);
        QString tab_name = tab_name_list_tablewidget.at(i_tab);


        if(report_generic_function_obj->pub_is_data_available_in_table(table_name, report_scope,selected_tags_list))
        {
            stream_csv <<"\n";
            stream_csv << tab_name <<"\n";
            stream_csv <<"\n";

            //            QString is_common_artifact = table_artifact_common_list_tablewidget.at(i_tab);
            //            if(is_common_artifact == QString("1"))
            //            {
            //                stream_csv <<QObject::tr("Artifacts Source") + "\n";
            //                QStringList source_list = report_generic_function_obj->get_artifacts_source_list(table_name, report_scope,selected_tags_list);
            //                for(int cc=0; cc < source_list.size(); cc++)
            //                {
            //                    stream_csv << QString::number(cc +1) + "," + source_list.at(cc) +"\n";
            //                }

            //                stream_csv <<"\n";
            //            }




            QStringList headers = rt_csv_d_name_list.at(i_tab);
            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            {
                headers.append("Source File");
            }
            QStringList line;
            QString col_head ="Sr. No.";
            QString temp_line = QString("\"") + QObject::tr(col_head.toLocal8Bit().data()) + "\"";
            line << temp_line;

            line << QString("\"") + QObject::tr(QString(MACRO_Generic_Complete_Source_Name).toLocal8Bit().data()) + "\"";

            bool_os_scheme_ios_found = false;
            for(int ii = 0 ; ii < destination_db_file_list.size(); ii++)
            {
                QString db_path = destination_db_file_list.at(ii);

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

                struct_GLOBAL_witness_info_source struct_info;
                QSqlQuery query_select(dest_db);
                QString command;

                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    command = QString("select source_count_name  from '" + table_name +"'");

                    if(!query_select.exec(command))
                    {
                        recon_static_functions::app_debug(" Execute query ---FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                        recon_static_functions::app_debug("error " + query_select.lastError().text() , Q_FUNC_INFO);
                        recon_static_functions::app_debug("cmd " + command , Q_FUNC_INFO);
                        dest_db.close();
                        return;
                    }

                    while(query_select.next())
                    {
                        struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                        if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                        {
                            bool_os_scheme_ios_found = true;
                            break;
                        }

                    }

                    if(bool_os_scheme_ios_found)
                        break;
                }

                if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {

                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, db_path, selected_tags_list, Q_FUNC_INFO);
                    QString record_no;

                    for(int i = 0 ;i < total_record_list.size() ; i++)
                    {
                        record_no = total_record_list.at(i);
                        command = QString("select  source_count_name  from '" + table_name + "' where INT=" + "'" + record_no + "'");

                        if(!query_select.exec(command))
                        {
                            recon_static_functions::app_debug(" Execute query ---FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                            recon_static_functions::app_debug("error " + query_select.lastError().text() , Q_FUNC_INFO);
                            recon_static_functions::app_debug("cmd " + command , Q_FUNC_INFO);
                            dest_db.close();
                            return;
                        }

                        while(query_select.next())
                        {
                            struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                            {
                                bool_os_scheme_ios_found = true;
                                break;
                            }

                        }

                        if(bool_os_scheme_ios_found)
                            break;
                    }

                }

                if(bool_os_scheme_ios_found)
                    break;
            }

            if(bool_os_scheme_ios_found)
            {
                line << QString("\"") + QObject::tr(QString(MACRO_Generic_Display_Name).toLocal8Bit().data()) + "\"";
            }


            if(get_plugin_name() != MACRO_Plugin_Name_File_System)
            {
                for(int h = 0 ; h < headers.size() ; h++)
                {
                    col_head = headers.at(h);
                    QString temp_line = QString("\"") + QObject::tr(col_head.toLocal8Bit().data()) + "\"";
                    line << temp_line;
                }


            }
            else
            {
                QString note_str =  headers.takeLast();
                QString tags_str =  headers.takeLast();

                QStringList apple_m_columns_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList).toStringList();
                QStringList exif_columns_keys_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList).toStringList();

                QStringList custom_header_col;
                custom_header_col << "Bookmarked Apple MetaData" << apple_m_columns_keys_list << exif_columns_keys_list
                                  << MACRO_CASE_TREE_MIME_Type << "Signature Value" << "Signature Name" << "Bookmarked Exif Data"
                                  << tags_str << note_str ;

                headers << custom_header_col;

                csv_custom_col_report_list = custom_header_col;
                for(int h = 0 ; h < headers.size() ; h++)
                {
                    col_head = headers.at(h);
                    QString temp_line = QString("\"") + QObject::tr(col_head.toLocal8Bit().data()) + "\"";
                    line << temp_line;
                }
            }
            QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
            QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);
            QStringList cols_list = rt_csv_col_name_list.at(i_tab);

            bool bool_multiple_export = false;
            if(is_multiple_export == "2")
                bool_multiple_export = true;


            if(is_export_available == QString::number(1) && tab_export_checked == true)
            {
                QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
                if(!custom_col_name_list.isEmpty())
                {
                    if(get_plugin_name() != MACRO_Plugin_Name_File_System)
                        line << "\"" + QObject::tr("More Apple Metadata") + "\"";
                }

                line << "\"" + QObject::tr("Export File Detail") + "\"";
            }



            stream_csv << line.join(",");
            stream_csv<< "\n";

            int sr_no = 0;
            for(int db_count = 0 ; db_count < destination_db_file_list.size(); db_count++)
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
                    set_table_data(i_tab,  command, table_name, tab_export_checked, is_export_available, bool_multiple_export, 0, stream_csv,db_path , dest_db);
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, db_path, selected_tags_list, Q_FUNC_INFO);

                    if(get_plugin_name() == MACRO_Plugin_Name_File_System)
                    {
                        QString temp_db_dir_path = db_path;
                        QFileInfo file_info(db_path);
                        temp_db_dir_path.chop(file_info.fileName().size()); // "file_system.sqlite"

                        QString mime_db_path        = temp_db_dir_path + "mime.sqlite";
                        QString hashes_db_path      = temp_db_dir_path + "hashes.sqlite";
                        QString sign_db_path        = temp_db_dir_path + "signature.sqlite";
                        QString exif_db_path        = temp_db_dir_path + "exif_metadata.sqlite";
                        QString apple_meta_db_path  = temp_db_dir_path + "apple_metadata.sqlite";

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
                            set_table_data(i_tab,  command, table_name, tab_export_checked, is_export_available, bool_multiple_export, sr_no, stream_csv,db_path, dest_db);
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
                            QString record_no = total_record_list.at(i);
                            QString command;
                            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
                            {
                                command = QString("select " + cols_list.join(",") + ", source_file, source_count_name  from '" + table_name + "' where INT=" + "'" + record_no + "'");
                            }
                            else
                            {
                                command = QString("select " + cols_list.join(",") + ", source_count_name  from '" + table_name + "' where INT=" + "'" + record_no + "'");
                            }
                            set_table_data(i_tab,  command, table_name, tab_export_checked, is_export_available, bool_multiple_export, sr_no, stream_csv,db_path , dest_db);
                            sr_no++;
                        }
                    }
                }

                dest_db.close();
            }

            stream_csv<< "\n\n";
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

                QString read_examiner_notes_html_file = examiner_notes_html_QFile.readAll();
                QString examiner_notes_data;

                QTextDocument doc;

                doc.setHtml(read_examiner_notes_html_file);

                examiner_notes_data = doc.toPlainText();

                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString || report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {

                    stream_csv << "\n";
                    stream_csv << selected_tab_name <<"\n";
                    stream_csv << "\n";

                    QStringList line1;
                    QString col_head = "Sr. No.";
                    QString temp_line = QString("\"") + QObject::tr(col_head.toLocal8Bit().data()) + "\"";
                    line1 << temp_line;
                    line1 << QString("\"") + QObject::tr(QString(MACRO_Tag_Name_Examiner_Comments).toLocal8Bit().data()) + "\"";

                    stream_csv << line1.join(",");
                    stream_csv << "\n";

                    QStringList line2;
                    QString col_val = "1";
                    QString line = QString("\"") + QObject::tr(col_val.toLocal8Bit().data()) + "\"";
                    line2 << line;
                    line2<< QString("\"") + QObject::tr(examiner_notes_data.toLocal8Bit().data()) + "\"";
                    stream_csv << line2.join(",");

                    stream_csv << "\n\n\n";
                }

                examiner_notes_html_QFile.close();

            }
        }

    }
}


QString csv_report_module::get_export_detail_csv_multiple_export( QSqlDatabase dest_db, QString column, QString tablename)
{

    QSqlQuery query_export_detail(dest_db);

    QString export_detail;

    QString cmnd = QString("select exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time from exportfilelist where (table_name = '") + tablename + "' AND record = " + column + ")";

    if(!query_export_detail.exec(cmnd))
    {
        recon_static_functions::app_debug(" ---FAILED--- query " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_export_detail.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("cmnd " + cmnd, Q_FUNC_INFO);
        return "";
    }

    bool bool_export_detail = false;
    while(query_export_detail.next())
    {

        QString temp = query_export_detail.value(1).toString();
        QString file_size;
        if(temp == QString(""))
        {
            bool_export_detail = true;
            file_size = QString("");
        }
        else
            file_size = recon_static_functions::human_readable_size(temp, Q_FUNC_INFO) + " (" + temp + QObject::tr(" bytes") + ")";


        if(bool_export_detail)
            export_detail = QString("\"") + QString("File Does not Exist") + "\"";
        else
        {
            QString f_relative_path = query_export_detail.value(0).toString();
            QString relative_path =  QObject::tr("New Path") + ": " + f_relative_path + "      ";

            QString size =  QObject::tr("Size")+  ": " + file_size + "    ";

            QString f_md5_string  = query_export_detail.value(2).toString();
            QString md5_string = QObject::tr("MD5")+  ": " + f_md5_string + "      ";

            QString f_sha1_string = query_export_detail.value(3).toString();
            QString sha1_string =  QObject::tr("SHA1") + ": " + f_sha1_string + "      ";


            QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
            QString numeric_timezone_value;
            if(machine_timezone_string.contains("GMT"))
            {
                QStringList tmp_list = machine_timezone_string.split("GMT");
                numeric_timezone_value = tmp_list.at(1);
            }

            QString f_export_time = QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString());
            QString export_time = QObject::tr("Export Time") +": " + f_export_time +" "+ numeric_timezone_value;
            export_detail.append(relative_path).append(size).append(md5_string).append(sha1_string).append(export_time).append("\n");

            export_detail.prepend("\"").append("\"");
        }
    }

    return export_detail;

}

QStringList csv_report_module::get_export_detail_csv_export(QSqlDatabase dest_db, QString column, QString tablename)
{
    QSqlQuery query_export_detail(dest_db);
    QString export_detail;
    QString cmnd = QString("select exp_relativepath,exp_size, exp_hashmd5, exp_hashsha1,exp_time from exportfilelist where (table_name = '") + tablename + "' AND record = " + column + ")";
    if(!query_export_detail.exec(cmnd))
    {
        recon_static_functions::app_debug(" ---FAILED--- query " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query_export_detail.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug("cmnd " + cmnd, Q_FUNC_INFO);
        return QStringList("");
    }

    bool bool_export_detail = false;

    while(query_export_detail.next())
    {

        QString temp = query_export_detail.value(1).toString();
        QString file_size;
        if(temp == QString(""))
        {
            bool_export_detail = true;
            file_size = QString("");
        }
        else
            file_size = recon_static_functions::human_readable_size(temp, Q_FUNC_INFO) + " (" + temp + QObject::tr(" bytes") + ")";


        if(bool_export_detail)
            export_detail = QString("\"") + QString("File Does not Exist") + "\"";
        else
        {
            QString machine_timezone_string = global_narad_muni_class_obj->get_field(MACRO_NARAD_Current_Machine_Timezone_QString).toString();
            QString numeric_timezone_value;
            if(machine_timezone_string.contains("GMT"))
            {
                QStringList tmp_list = machine_timezone_string.split("GMT");
                numeric_timezone_value = tmp_list.at(1);
            }

            QString f_relative_path = query_export_detail.value(0).toString();
            QString relative_path =  QObject::tr("New Path") + ": " + f_relative_path + "      ";

            QString size =  QObject::tr("Size")+  ": " + file_size + "    ";

            QString f_md5_string  = query_export_detail.value(2).toString();
            QString md5_string = QObject::tr("MD5")+  ": " + f_md5_string + "      ";

            QString f_sha1_string = query_export_detail.value(3).toString();
            QString sha1_string =  QObject::tr("SHA1") + ": " + f_sha1_string + "      ";

            QString f_export_time = QDateTime::currentDateTime().toString(narad_muni_for_report_obj->get_field(MACRO_NARAD_Examiner_Selected_Datetime_Format_QString).toString());
            QString export_time = QObject::tr("Export Time") +": " + f_export_time + " " + numeric_timezone_value  ;


            export_detail.append(relative_path).append(size).append(md5_string).append(sha1_string).append(export_time).append("\n");
            export_detail.prepend("\"").append("\"");
        }
    }

    return QStringList(export_detail);

}

void csv_report_module::create_full_report_csv(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug("file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString() , Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&file);

    if(parent_module_name_str.isEmpty())
        outputfile_csv << get_plugin_name() << "\n";
    else
        outputfile_csv << get_plugin_name() + QString(" (" + parent_module_name_str + ")") << "\n";
    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list))
    {
        outputfile_csv << QObject::tr("No Data Available");
        outputfile_csv<< "\n";
        outputfile_csv<< "\n";
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


        create_table_widget_csv_report(outputfile_csv,tabname, MACRO_REPORT_SCOPE_FULL_QString, selected_tags_list, bool_exp);
    }

    file.close();

    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);

}

void csv_report_module::create_tag_report_csv(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts" , Q_FUNC_INFO);

    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug("file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString() , Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&file);

    if(parent_module_name_str.isEmpty())
        outputfile_csv << get_plugin_name() << "\n";
    else
        outputfile_csv << get_plugin_name() + QString(" (" + parent_module_name_str + ")") << "\n";
    if(!report_generic_function_obj->pub_is_data_available_in_tab(selected_tablist, MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list))
    {
        outputfile_csv << QObject::tr("No Data Available");
        outputfile_csv<< "\n";
        outputfile_csv<< "\n";
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


        create_table_widget_csv_report(outputfile_csv,tabname, MACRO_REPORT_SCOPE_TAGS_QString, selected_tags_list, bool_exp);
    }

    file.close();

    recon_static_functions::app_debug("Ends" , Q_FUNC_INFO);
}

void csv_report_module::set_table_data(int i_tab,QString command,QString table_name, bool tab_export_checked,QString is_export_available, bool bool_multiple_export, int count,QTextStream &stream_csv, QString destination_db_file, QSqlDatabase &dest_db)
{

    QSqlQuery query_select(dest_db);
    if(!query_select.exec(command))
    {
        recon_static_functions::app_debug(" Execute query ---FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + query_select.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("cmd " + command , Q_FUNC_INFO);
        dest_db.close();
        return;
    }

    QStringList data_type_list = rt_csv_data_type_list.at(i_tab);
    QStringList col_d_name_list = rt_csv_d_name_list.at(i_tab);


    QStringList line;
    while(query_select.next())
    {
        if(bool_cancel_report)
            break;

        count++;
        QString num = QString::number(count);
        line.clear();
        line << num;

        emlx_source_file.clear();
        if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            emlx_source_file = query_select.value(12).toString();

        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());
        line << struct_info.complete_source_name;

        if(bool_os_scheme_ios_found)
        {
            if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
            {
                line << struct_info.source_name;
            }
            else
            {
                line << "";
            }
        }

        QString record_number;

        if(get_plugin_name() == MACRO_Plugin_Name_File_System)
        {
            ///1.-start::--FS Report---Depends on CSV AND Code both::
            int tag_notes_offset = col_d_name_list.size() - 2;

            for(int csv_col_index = 0 ; csv_col_index < col_d_name_list.size() - 2 ; csv_col_index++)
            {
                if(data_type_list.at(csv_col_index)  == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = query_select.value(csv_col_index).toString();
                }
                else if(data_type_list.at(csv_col_index) == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    QString value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_select.value(csv_col_index).toString(), Q_FUNC_INFO);
                    value = QString("\"") + value + "\"";
                    line << value;
                }
                else if(data_type_list.at(csv_col_index) == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    QString all_used_dates ;
                    QString temp_2 = query_select.value(csv_col_index).toString();
                    if(temp_2 != QString(""))
                    {
                        QString temp_value = query_select.value(csv_col_index).toString();
                        QStringList temp;
                        if(temp_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                        {
                            temp = temp_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        }
                        else
                        {
                            temp = temp_value.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                        }

                        if(temp.size() >= 1)
                        {
                            for(int i = 0 ; i< temp.size(); i++)
                            {
                                all_used_dates.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(i), Q_FUNC_INFO) +"\n");
                            }
                        }
                    }

                    line <<  QString("\"") + all_used_dates + "\"";
                }
                else if(data_type_list.at(csv_col_index) == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    QString value = recon_static_functions::human_readable_size(query_select.value(csv_col_index).toString(), Q_FUNC_INFO);
                    value = QString("\"") + value + "\"";
                    line << value;
                }
                else if(data_type_list.at(csv_col_index) == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                    {
                        QString value = (query_select.value(csv_col_index).toString());
                        value = QString("\"") + value + "\"";
                        line << value;
                    }
                    else
                    {
                        line << QString("\"") + "" + "\"";
                    }
                }
                else
                {
                    QString value = (query_select.value(csv_col_index).toString());
                    value = QString("\"") + value + "\"";
                    line << value;
                }
            }

            for(int custm_col_index = 0 ; custm_col_index < csv_custom_col_report_list.size() - 2; custm_col_index++)
            {

                QString value = (query_select.value(col_d_name_list.size() + custm_col_index).toString());
                if(!value.trimmed().isEmpty())
                {
                    value = QString("\"") + value + "\"";
                    QString column_name = query_select.record().fieldName(col_d_name_list.size() + custm_col_index);
                    if(column_name.contains("Date", Qt::CaseInsensitive))
                    {
                        line << global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(value, Q_FUNC_INFO);
                    }
                    else
                        line << value;
                }
                else
                {
                    line << QString("\"") + "" + "\"";
                }

            }



            if(data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString))
            {
                QString value = query_select.value("recon_tag_value").toString();
                value = QString("\"") + value + "\"";
                line << value;
            }

            if(data_type_list.contains(MACRO_CSV_TEMPLATE_DATATYPE_NOTES_VALUE_QString))
            {
                QString value = query_select.value("notes").toString();
                value = QString("\"") + value + "\"";
                line << value;

            }

            ///1.-end
        }
        else
        {

            for(int  i = 0 ; i < col_d_name_list.size() ; i++)
            {

                QString col_data_type = data_type_list.at(i);

                if(col_data_type  == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                {
                    record_number = query_select.value(i).toString();
                }
                if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString)
                {
                    QString bookmarked_ext_attr = query_select.value(i).toString();
                    QString value = query_select.value(i).toString();
                    value = QString("\"") + bookmarked_ext_attr + "\"";
                    line << value;

                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                {
                    QString value = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_select.value(i).toString(), Q_FUNC_INFO);
                    value = QString("\"") + value + "\"";
                    line << value;
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
                {
                    QString value = recon_static_functions::human_readable_size(query_select.value(i).toString(), Q_FUNC_INFO);
                    value = QString("\"") + value + "\"";
                    line << value;
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_MULTIPLE_DATES)
                {
                    QString all_used_dates ;
                    QString temp_2 = query_select.value(i).toString();
                    if(temp_2 != QString(""))
                    {
                        QStringList temp;
                        if(temp_2.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                        {
                            temp = temp_2.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        }
                        else
                        {
                            temp = temp_2.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                        }

                        if(temp.size() >= 1)
                        {
                            for(int i = 0 ; i< temp.size(); i++)
                            {
                                all_used_dates.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(i), Q_FUNC_INFO) +"\n");
                            }
                        }
                    }

                    line <<  QString("\"") + all_used_dates + "\"";
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
                {
                    QString info ;

                    QString str = query_select.value(i).toString();
                    QStringList timestamps_splitList;

                    timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                    for(int tm_loop = 0 ; tm_loop < timestamps_splitList.size(); tm_loop++)
                    {
                        info.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) +"\n");
                    }

                    line <<  QString("\"") + info + "\"";
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
                {
                    QString info ;

                    QString str = query_select.value(i).toString();

                    if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        QStringList temp = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                        if(temp.size() >= 1)
                        {
                            for(int i = 0 ; i< temp.size(); i++)
                            {
                                info.append((temp.at(i)) +"\n");
                            }
                        }

                        line <<  QString("\"") + info + "\"";
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

                        if(temp.size() >= 1)
                        {
                            for(int i = 0 ; i< temp.size(); i++)
                            {
                                QString str = temp.at(i);
                                str = str.replace(":- ;",":- ");

                                str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,", ");

                                info.append((str) +"\n");
                            }
                        }

                        line <<  QString("\"") + info.replace("|","\t") + "\"";
                    }
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
                {
                    QString value = (query_select.value(i).toString());

                    if(!value.startsWith("/"))
                        value = value.prepend("/");

                    value.replace(";/","\n/");

                    value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,", ");

                    value = QString("\"") + value + "\"";
                    line << value;

                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
                {
                    QString value = (query_select.value(i).toString());

                    value = QString("\"") + value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"\n") + "\"";

                    line << value;
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                {
                    struct_GLOBAL_witness_info_source struct_info_1 = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                    if(struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display
                            || struct_info_1.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_androidOS_Display)
                    {
                        QString value = (query_select.value(i).toString());
                        value = QString("\"") + value + "\"";
                        line << value;
                    }
                    else
                    {
                        line << QString("\"") + "" + "\"";
                    }
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_QString)
                {
                    QString value = (query_select.value(i).toString());
                    value = QString("\"") + value + "\"";
                    line << value;
                }
                else if(col_data_type == MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString)
                {
                    QString value = (query_select.value(i).toString());

                    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                    QString str_val = value;
                    str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                    str_val = QString("\"") + str_val + "\"";
                    line << str_val;

                }
                else
                {
                    QString value = (query_select.value(i).toString());
                    value = QString("\"") + value + "\"";
                    line << value;
                }

            }

            if(get_plugin_name() == MACRO_Plugin_Name_Email_Parser)
            {
                line << QString("\"") + emlx_source_file + "\"";
            }


            if(struct_info.os_scheme_display == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
            {
                QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();

                if(!custom_col_name_list.isEmpty())
                {
                    if(is_export_available == QString::number(1) && tab_export_checked == true)
                    {
                        ////insert here code for extended***list
                        QString extended_attr_str = get_apple_metadata_key_value_for_csv_report(record_number,table_name,destination_db_file);
                        if(!extended_attr_str.isEmpty())
                        {
                            line << write_users_apple_metadata_in_csv_file(extended_attr_str);
                        }
                        else
                            line << "\" \"";

                    }
                }
            }
            else
                line << "\" \"";


        }





        if(is_export_available == QString("1") && tab_export_checked == true)
        {
            line << get_export_detail_csv_export(dest_db, query_select.value(0).toString(),table_name);
        }


        stream_csv << line.join(",");
        stream_csv << "\n";

    }
}


QString csv_report_module::write_users_apple_metadata_in_csv_file(QString extended_attr_str)
{
    QString extended_attr_final_str;

    QString key_value_str;

    QStringList attr_list_for_files = extended_attr_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

    if(attr_list_for_files.size() <= 0)
        return extended_attr_final_str;

    for(int i = 0; i < attr_list_for_files.size(); i++)
    {
        if(attr_list_for_files.at(i).isEmpty())
            continue;

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
            QString value = key_value_splitted_list.at(1);

            key_value_str += key;
            key_value_str += " : ";
            key_value_str += value;

            if(j != (attr_list_for_multiple_keys.size() - 1))
                key_value_str += "\n";
        }

        if(i != (attr_list_for_files.size() - 1))
        {
            key_value_str += "\n";
            key_value_str += "\n";
        }
    }

    extended_attr_final_str = "\"" + key_value_str + "\"";

    return extended_attr_final_str;
}


void csv_report_module::create_screen_report_csv(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list)
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



    QStringList cols_list = rt_csv_col_name_list.at(i_tab);
    QStringList data_type_list = rt_csv_data_type_list.at(i_tab);

    QStringList col_d_name_list = rt_csv_d_name_list.at(i_tab);
    QStringList col_order_list = rt_csv_order_list.at(i_tab);
    int cols_size = cols_list.size();

    QString is_export_available = table_export_available_list_tablewidget.at(i_tab);
    QString is_multiple_export = table_export_criteria_list_tablewidget.at(i_tab);

    bool bool_multiple_export = false;
    if(is_multiple_export == "2")
        bool_multiple_export = true;


    QFile file;
    file.setFileName(file_path);
    if(!file.open(QIODevice::Append))
    {
        recon_static_functions::app_debug(" file open ---FAILED---" + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString(), Q_FUNC_INFO);
        return;
    }

    QTextStream outputfile_csv(&file);

    if(parent_module_name_str.isEmpty())
        outputfile_csv << get_plugin_name() << "\n";
    else
        outputfile_csv << get_plugin_name() + QString(" (" + parent_module_name_str + ")") << "\n";

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
        outputfile_csv << QObject::tr("No Data Available");
        outputfile_csv<< "\n";
        outputfile_csv<< "\n";
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

    //headers = header_str.split("::", Qt::SkipEmptyParts);


    QStringList headers = rt_csv_d_name_list.at(i_tab);
    headers.insert(0,"Sr. No.");

    headers.insert(1,MACRO_Generic_Complete_Source_Name);

    QStringList col_type_list_for_source_name = tw_data_type_list_tablewidget.at(i_tab);

    int int_index = col_type_list_for_source_name.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

    QString int_str;

    int counter = 0;
    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

        bool_os_scheme_ios_found = false;
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

            int_str = list_m_tablewidgets.at(i_tab)->item(i, int_index)->text();

            struct_GLOBAL_witness_info_source struct_info;
            QSqlQuery query_select(dest_db);
            QString cmd;

            cmd = "select source_count_name from " + table_name  + " where INT = " + int_str ;

            if(!query_select.exec(cmd))
            {
                recon_static_functions::app_debug(" Execute query ---FAILED---" + dest_db.databaseName() , Q_FUNC_INFO);
                recon_static_functions::app_debug("error " + query_select.lastError().text() , Q_FUNC_INFO);
                recon_static_functions::app_debug("cmd " + cmd , Q_FUNC_INFO);
                dest_db.close();
                return;
            }

            while(query_select.next())
            {
                struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value("source_count_name").toString());

                if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
                {
                    bool_os_scheme_ios_found = true;
                    break;
                }

            }

            if(bool_os_scheme_ios_found)
                break;

            counter++;
        }

        if(bool_os_scheme_ios_found)
            break;

    }

    if(bool_os_scheme_ios_found)
    {
        headers.insert(2,MACRO_Generic_Display_Name);
    }


    outputfile_csv <<"\n";
    outputfile_csv << tab_nam <<"\n";
    outputfile_csv <<"\n";




    //    QString is_common_artifact = table_artifact_common_list_tablewidget.at(i_tab);

    //    if(is_common_artifact == QString("1"))
    //    {
    //        outputfile_csv <<QObject::tr("Artifacts Source") + "\n";
    //        QStringList source_list = report_generic_function_obj->get_artifacts_source_list(table_name, MACRO_REPORT_SCOPE_SCREEN_QString,QStringList(""));
    //        for(int cc=0; cc < source_list.size(); cc++)
    //        {
    //            outputfile_csv <<QString::number(cc +1) + "," + source_list.at(cc) +"\n";
    //        }

    //        outputfile_csv <<"\n";
    //    }



    QStringList line;

    for(int h = 0 ; h < headers.size() ; h++)
    {
        QString col_head = headers.at(h);
        QString temp_line = QString("\"") + QObject::tr(col_head.toLocal8Bit().data()) + "\"";
        line << temp_line;
    }




    if(is_export_available == QString::number(1) && tab_export_checked == true)
    {
        QStringList custom_col_name_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
        if(!custom_col_name_list.isEmpty())
        {
            line << "\"" + QObject::tr("More Apple Metadata") + "\"";
        }

        line << "\"" + QObject::tr("Export File Detail") + "\"";
    }




    outputfile_csv << line.join(",");
    outputfile_csv<< "\n";

    QString command;

    QStringList col_type_list = tw_data_type_list_tablewidget.at(i_tab);

    int INT_index = col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);
    QString INT_str;


    int count = 0;
    for(int i = 0; i < list_m_tablewidgets.at(i_tab)->rowCount(); i++)
    {
        if(list_m_tablewidgets.at(i_tab)->isRowHidden(i))
            continue;

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

            INT_str = list_m_tablewidgets.at(i_tab)->item(i, INT_index)->text();

            command = "select " + cols_list.join(",") + ",source_count_name , source_file from " + table_name  + " where INT = " + INT_str ;

            set_table_data(i_tab,  command, table_name, tab_export_checked, is_export_available, bool_multiple_export, count, outputfile_csv,db_path,dest_db);
            count++;
        }
    }

    outputfile_csv<< "\n\n";

    file.close();

    recon_static_functions::app_debug(" -Ends", Q_FUNC_INFO);

}


QString csv_report_module::get_apple_metadata_key_value_for_csv_report(QString record_no, QString table_name, QString db_path)
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

QString csv_report_module::get_plugin_name()
{
    return plugin_name_str;
}
