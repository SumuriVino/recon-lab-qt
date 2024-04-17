#include "report_generic_function.h"

report_generic_function::report_generic_function(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

}

void report_generic_function::pub_set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
}
void report_generic_function::fill_variable_from_narad_muni()
{
    plugin_name = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();
    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();

}

bool report_generic_function::pub_is_tagtype_other_than_screenshot_or_saved_graph(QString plugin_name)
{

    if(plugin_name == QString(MACRO_Plugin_Name_Artifacts_Timeline_Saved_Graph)
            || plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History)
            || plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger)
            || plugin_name == QString(MACRO_Plugin_Name_Screenshots)
            || plugin_name == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        return false;
    }


    return true;



}

void report_generic_function::pub_set_essentials()
{
    fill_variable_from_narad_muni();
}

QString report_generic_function::pub_get_fs_command(QString fs_record_no)
{

    html_report_file_system_signauture_key_list.clear();
    html_report_file_system_Mime_key_list.clear();
    QStringList apple_m_selected_cols_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();

    QString complete_command ;
    QStringList apple_m_temp_list;
    for(int i = 0 ; i < apple_m_selected_cols_list.size() ; i++)
    {
        QString column_name = apple_m_selected_cols_list.at(i);
        column_name.prepend("c.");
        apple_m_temp_list << column_name;
    }
    QString apple_m_cmd_str = apple_m_temp_list.join(",");
    if(!apple_m_cmd_str.trimmed().isEmpty())
        apple_m_cmd_str.append(",");


    QStringList exif_m_selected_cols_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Exif_Attributes_For_Report_Custom_Column_Name_QStringList).toStringList();
    QStringList exif_tmp_columns_list;
    for(int i = 0 ; i < exif_m_selected_cols_list.size() ; i++)
    {
        QString column_name = exif_m_selected_cols_list.at(i);
        column_name.prepend("e.");
        exif_tmp_columns_list << column_name;
    }
    QString exif_m_cmd_str = exif_tmp_columns_list.join(",");






    ///-------------join two partial command
    QString partial_command = apple_m_cmd_str + exif_m_cmd_str;

    if(partial_command.trimmed().isEmpty())
        partial_command.append(",");
    else
        partial_command.append(",").prepend(",");




    ////--------------final command

    complete_command = "Select "  + QString("a.INT, a.inode_no,a.filename, a.filepath,a.file_size,a.hash_md5 , a.hash_sha1 ,a.extension,a.ocr_text,a.z_date_time_modified,"
                                            "a.z_date_time_change,a.z_date_time_accessed,a.z_date_time_created,a.kMDItemDateAdded_Date_Added , "
                                            "a.kMDItemContentCreationDate_Created_Date , a.kMDItemContentModificationDate_Last_Modified_Date , "
                                            "a.kMDItemLastUsedDate_Last_Opened_Date , a.kMDItemUsedDates_Used_Dates , a.kMDItemUseCount_Used_Count ,a.recon_tag_value,a.notes,"
                                            "a.bookmarked_extended_attributes ") + partial_command
            + QString("b.mime_type,d.signature_name,d.signature_value , a.bookmarked_exif_data , a.source_count_name ")
            + QString(" From main.files as a LEFT OUTER join mimeDB.files as b ON (a.INT = b.filesystem_record) "
                      " LEFT OUTER join apple_metadataDB.files as c ON (a.INT = c.filesystem_record)  "
                      " LEFT OUTER join signatureDB.files as d ON (a.INT = d.filesystem_record)  "
                      " LEFT OUTER join exif_metadataDB.files as e ON (a.INT = e.filesystem_record)   "
                      " LEFT OUTER join hashesDB.files as f ON (a.INT = f.filesystem_record)     "
                      " Where (a.INT = " + fs_record_no +")");

    html_report_file_system_signauture_key_list  << "Signature Name" << "Signature Value";
    html_report_file_system_Mime_key_list << MACRO_CASE_TREE_MIME_Type;


    return complete_command;
}

qint64 report_generic_function::pub_get_total_record_of_table(QString table_name, QString report_scope, QStringList tags_list)
{
    qint64 total_record_count = 0;
    for(int db_count = 0 ; db_count < destination_db_file_list.size() ; db_count++)
    {

        QString dest_db_file = destination_db_file_list.at(db_count);

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase dest_db;
        dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        dest_db.setDatabaseName(dest_db_file);

        if(!dest_db.open())
        {
            recon_static_functions::app_debug(" -db open -----FAILED---" + dest_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" -Error" + dest_db.lastError().text(), Q_FUNC_INFO);
            continue;
        }

        QString cmd;

        qint64 single_db_record_count = 0;

        if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
        {
            cmd = QString("SELECT COUNT(*) FROM '" + table_name + "'");

            single_db_record_count = recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, dest_db, Q_FUNC_INFO).toLongLong();
        }
        else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
        {
            for(int i = 0; i < tags_list.size(); i++)
            {
                QString tag_str = tags_list.at(i);

                cmd = QString("SELECT COUNT(*) FROM '" + table_name + "' WHERE recon_tag_value LIKE" + "'%" + tag_str + "%'" + " OR bookmark = '1'");
                single_db_record_count += recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, dest_db, Q_FUNC_INFO).toLongLong();

            }
        }
        else if(report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
        {
            cmd = QString("SELECT COUNT(*) FROM '" + table_name + "'");
            single_db_record_count += recon_helper_standard_obj->get_string_from_db_by_dbreference(cmd, 0, dest_db, Q_FUNC_INFO).toLongLong();
        }

        total_record_count = total_record_count + single_db_record_count;

        dest_db.close();
    }

    return total_record_count;
}

void report_generic_function::pub_get_only_tags_record_of_table_including_multiple(QString cmd, QStringList &record_no_list,QString destination_db_file)
{
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(destination_db_file);

    if(!dest_db.open())
    {
        recon_static_functions::app_debug(" -db open -----FAILED---" + destination_db_file, Q_FUNC_INFO);
        recon_static_functions::app_debug(" -" + dest_db.lastError().text(), Q_FUNC_INFO);

        return;
    }

    QSqlQuery query_getcount(dest_db);

    if(!query_getcount.exec(cmd))
    {
        recon_static_functions::app_debug(" -select query -----FAILED---" + destination_db_file, Q_FUNC_INFO);
        recon_static_functions::app_debug(" - command " + cmd, Q_FUNC_INFO);
        return;
    }

    while(query_getcount.next())
    {
        if(record_no_list.contains(query_getcount.value(0).toString()))
            continue;

        record_no_list << query_getcount.value(0).toString();
    }

    dest_db.close();
}

bool report_generic_function::pub_is_column_of_apple_attribute(QString column_name)
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


QStringList report_generic_function::pub_get_artifacts_source_list(QString table, QString report_scope,QStringList selected_tags_list_for_report)
{
    QStringList list;

    QString command;
    if(report_scope == MACRO_REPORT_SCOPE_FULL_QString || report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
        command = QString( "select source_file from '" + table + "' GROUP BY source_file");
    else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
        command = QString( "select source_file from '" + table + "' where recon_tag_value is not null and recon_tag_value is not ''GROUP BY source_file");

    for(int i = 0 ; i < destination_db_file_list.size() ; i++)
    {
        QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

        destination_db->setDatabaseName(destination_db_file_list.at(i));
        if(!destination_db->open())
        {
            recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + destination_db->databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
            if(destination_db != NULL)
                delete destination_db;
            QSqlDatabase::removeDatabase(connection_naam);
            continue ;
        }

        QSqlQuery query(*destination_db);

        if(!query.exec(command))
        {
            recon_static_functions::app_debug("  Failed to execute query ----FAILED--- "  + destination_db->databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" error " + query.lastError().text(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" command " + query.executedQuery() , Q_FUNC_INFO);
            destination_db->close();
            if(destination_db != NULL)
                delete destination_db;
            QSqlDatabase::removeDatabase(connection_naam);
            continue ;
        }

        while(query.next())
        {
            list << query.value(0).toString();
        }

        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
    }
    return list;
}

bool report_generic_function::pub_is_data_available_in_examiner_notes()
{
    bool data_available = false;

    QString examiner_notes_html_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "Examiner_Notes.html";
    QFile examiner_notes_html_QFile;
    examiner_notes_html_QFile.setFileName(examiner_notes_html_file_path);

    if(!examiner_notes_html_QFile.open(QFile::ReadOnly))
    {
        recon_static_functions::app_debug(" File Open Examiner Notes Html -----FAILED-----" + examiner_notes_html_QFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:" + examiner_notes_html_QFile.errorString(), Q_FUNC_INFO);
        return data_available;
    }

    QByteArray file_data_array = examiner_notes_html_QFile.readAll();

    QString file_data = QString::fromLocal8Bit(file_data_array);

    if(!file_data.isEmpty()
            && file_data != "")
    {
        data_available = true;
    }

    examiner_notes_html_QFile.close();

    return data_available;
}


bool report_generic_function::pub_check_datatype_exist_in_datatypelist(const char* searched_data,QStringList data_type_list)
{
    for(int i = 0; i < data_type_list.size(); i++)
    {
        QString str = data_type_list.at(i);
        int index = str.indexOf("_");
        if(index >= 0)
        {
            str = str.remove(index,str.size());
        }
        if(QString(searched_data) == str)
            return true;
    }
    return false;
}

bool report_generic_function::pub_is_data_available_in_table(QString table_name, QString report_scope,QStringList selected_tags_list)
{
    if(report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
    {
        return true;
    }

    for(int i = 0 ; i < destination_db_file_list.size() ; i++)
    {
        QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

        destination_db->setDatabaseName(destination_db_file_list.at(i));
        if(!destination_db->open())
        {
            recon_static_functions::app_debug(" Destination Db opening ---FAILED--- " + destination_db->databaseName(),Q_FUNC_INFO);
            recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
            if(destination_db != NULL)
                delete destination_db;
            QSqlDatabase::removeDatabase(connection_naam);
            continue ;
        }

        if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
        {
            QString command = QString( "select INT from '") + table_name + "'";
            QStringList received_value = recon_helper_standard_obj->get_stringlist_from_db_by_dbpath(command, 0, destination_db_file_list.at(i), Q_FUNC_INFO);
            if(received_value.size() >= 1)
            {
                destination_db->close();
                if(destination_db != NULL)
                    delete destination_db;
                QSqlDatabase::removeDatabase(connection_naam);

                return true;
            }
        }
        else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
        {

            QStringList final_tags = recon_helper_standard_obj->extract_all_tags_from_db(1, QStringList(table_name), destination_db_file_list.at(i), Q_FUNC_INFO);

            for(int kk = 0; kk < selected_tags_list.size(); kk++)
            {
                if(final_tags.contains(selected_tags_list.at(kk)))
                {
                    destination_db->close();
                    if(destination_db != NULL)
                        delete destination_db;
                    QSqlDatabase::removeDatabase(connection_naam);

                    return true;
                }
            }
        }


        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

    }

    return false;
}

bool report_generic_function::pub_is_data_available_in_tab(QStringList tab_list, QString report_scope, QStringList selected_tags_list)
{
    bool bool_data_available = false;
    for(int tab_no = 0; tab_no < tab_list.size(); tab_no++)
    {
        QString tab_name = tab_list.at(tab_no);
        QString tblname = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name, tab_name);
        if(pub_is_data_available_in_table(tblname, report_scope, selected_tags_list))
        {
            bool_data_available = true;
            break;
        }

        if(tab_name == MACRO_Tag_Name_Examiner_Comments)
        {
            if(pub_is_data_available_in_examiner_notes())
            {
                bool_data_available = true;
                break;
            }
        }

    }

    return bool_data_available;
}

QString report_generic_function::pub_get_hex_viewer_format_for_html_report(QString hex_bytes)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    QString final_str_with_bytes_and_ascii;

    final_str_with_bytes_and_ascii += "\n";

    QByteArray data_array = hex_bytes.toLocal8Bit();
    data_array = QByteArray::fromHex(data_array);

    for(int i = 0; i < data_array.size() ; i += Global_Variable_hex_viewer_BYTES_PER_LINE_int)
    {
        QString ascii_str;
        QByteArray ascii = data_array.mid(i,Global_Variable_hex_viewer_BYTES_PER_LINE_int);
        for(int idx=0 ;idx < ascii.size(); idx++)
        {
            if(((char)ascii[idx]<0x20) || ((char)ascii[idx]>0x7e))
            {
                ascii_str += QString(".");
            }
            else
            {
                ascii_str += QString(ascii.at(idx));
            }
        }

        QString hex_bytes_with_space;
        QByteArray hex_values = data_array.mid(i,Global_Variable_hex_viewer_BYTES_PER_LINE_int);
        for(int idx = 0 ; idx < hex_values.size(); idx++)
        {
            QByteArray byte_arr = hex_values.mid(idx,1);

            hex_bytes_with_space += QString::fromLocal8Bit(byte_arr.toHex().toUpper()) + QString("&emsp;");
        }

        if(hex_bytes_with_space.size() < 128)
        {
            int diff = (128 - hex_bytes_with_space.size())/8;

            for(int j = 0; j < diff; j++)
            {
                hex_bytes_with_space += " &emsp; &#160&#160";
            }
        }

        QString space_str = QString("&emsp;&emsp;&emsp;");

        final_str_with_bytes_and_ascii += hex_bytes_with_space + space_str + ascii_str + "\n";
    }

    recon_static_functions::app_debug(" end" , Q_FUNC_INFO);

    return final_str_with_bytes_and_ascii;
}


QString report_generic_function::pub_export_virtual_file(QString file_path , QString plugin_name , QString destination_file_location, QString reports_dir_path)
{
    if(file_path.trimmed().isEmpty())
        return "";

    if(!reports_dir_path.endsWith("/"))
        reports_dir_path.append("/");


    QFileInfo info(file_path);
    QString file_name = info.fileName();

    if((file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images))) || (file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
            || (file_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
        file_path =  destination_file_location + file_path.remove(0,1);
    else if(file_path.startsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Maps))
            || file_path.startsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images)))
    {
        file_path =  destination_file_location + file_path;
    }
    else if(file_path.startsWith(QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features)))
    {
        file_path =  narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + file_path;
    }
    else if(file_path.startsWith(QString("/") + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Lab_Features)))
    {
        file_path =  narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString() + file_path;
    }
    else if(plugin_name == QString(MACRO_Plugin_Name_Screenshots))
    {
        file_path = file_path;
    }

    QDir dirp;
    dirp.mkpath(reports_dir_path + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images));

    QString dest_loc = reports_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images;
    file_name = recon_static_functions::get_available_filename(file_name,dest_loc, Q_FUNC_INFO);
    QString dest_file = dest_loc + file_name;

    QFile file;
    file.copy(file_path,dest_file);
    return dest_file;
}
