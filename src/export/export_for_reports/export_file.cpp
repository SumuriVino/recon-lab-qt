#include "export_file.h"

export_file_module::export_file_module(QObject *parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    bool_thread_copied_finished = false;
    th_file_copier_module_popobj = new thread_file_copier_module(this);
    connect(th_file_copier_module_popobj, SIGNAL(finished()), this, SLOT(slot_thread_file_copier_finished()));
    bool_cancel_export = false;
    export_rsync_obj = new export_rsync(this);

}
void export_file_module::pub_cancel_export()
{
    bool_cancel_export = true;
    th_file_copier_module_popobj->pub_stop_thread();

    export_rsync_obj->pub_cancel_rsync_copy(Q_FUNC_INFO);
}

void export_file_module::set_narad_muni_for_report(narad_muni *mobj)
{
    narad_muni_for_report_obj = mobj;
}

void export_file_module::set_essentials()
{
    bool_cancel_export = false;
    fill_variable_from_narad_muni();
    file_name_list.clear();
}

void export_file_module::export_queried_files(QString command_select, QString table_name, QString db_path)
{

    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    //export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    QString dest_path = reports_dir_path + "files";
    QString link_dir_path = dest_path + "/Links";

    QDir dir;
    dir.mkpath(link_dir_path);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase dest_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    dest_db.setDatabaseName(db_path);
    if(!dest_db.open())
    {
        recon_static_functions::app_debug("Db opening ---FAILED--- " + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + dest_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QSqlQuery query_select_export(dest_db);
    query_select_export.prepare(command_select);

    if(!query_select_export.exec())
    {
        recon_static_functions::app_debug(" --FAILED--- to execute query_select_export "  + dest_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + query_select_export.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug("command " + command_select , Q_FUNC_INFO);
        dest_db.close();
        return;
    }


    qint64 event_counter = 0;
    while(query_select_export.next())
    {

        if(bool_cancel_export)
            break ;

        event_counter++;

        if(event_counter % 500 == 0)
            QCoreApplication::processEvents();

        QStringList export_path_list, softlink_filename_list;

        QString file_path = query_select_export.value(0).toString().trimmed();
        QString record = query_select_export.value(1).toString().trimmed();
        QString softlink_file_name = query_select_export.value(2).toString().trimmed();
        QString source_count_name = query_select_export.value("source_count_name").toString().trimmed();

        if(bool_censored_image_clicked)
        {
            ///This file Name format is use same in Its script and File is saved with this filename format.
            /// so this "_censored_" is necessary to add between record_no and file name to match exactly with the file in its directory

            QString censored_file_name =  record + QString("_censored_") + QFileInfo(file_path).fileName();

            QString cmplt_censored_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/") + QString(MACRO_Censored_Skin_Tone_Images_Dir_Name_QString);
            cmplt_censored_file_path = cmplt_censored_file_path.append(censored_file_name);


            QString recon_tag_value = query_select_export.value("recon_tag_value").toString().trimmed();

            if(recon_tag_value == MACRO_Tag_Name_Skin_Tone && QFileInfo(cmplt_censored_file_path).exists())
            {
                file_path = cmplt_censored_file_path;
                bool_skin_tone_tag = true;
            }
            else
            {
                bool_skin_tone_tag = false;
            }

        }

        QString os_schme_intrnl = recon_helper_standard_obj->get_target_os_scheme_internal_on_current_record_source(record,table_name,db_path,Q_FUNC_INFO);

        if(file_path.isEmpty())
        {
            continue ;
        }
        else
        {
            if(file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
            {
                export_path_list = file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

                if(softlink_file_name.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    softlink_filename_list = softlink_file_name.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);

                // for doing equal size of export_path_list & softlink_filename_list for all OS
                for(int kk = softlink_filename_list.size(); kk < export_path_list.size(); kk++)
                    softlink_filename_list << "";
            }
            else
            {
                export_path_list << file_path;
                softlink_filename_list << softlink_file_name;
            }


            for(int i = 0 ; i < export_path_list.size() ; i++)
            {
                QString target_initial_path = recon_helper_standard_obj->get_virtual_source_path_of_current_record(record,table_name,db_path,Q_FUNC_INFO);
                if(target_initial_path.endsWith("/"))
                    target_initial_path.chop(1);

                QString complete_source_path = export_path_list.at(i);

                if(complete_source_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images))
                        || complete_source_path.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
                {
                    QFileInfo tt_info(db_path);
                    complete_source_path =  tt_info.absolutePath() + complete_source_path.remove(0,1);

                }
                else if(complete_source_path.startsWith("/" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
                {
                    QFileInfo tt_info(db_path);
                    complete_source_path =  tt_info.absolutePath() + complete_source_path;
                }
                else
                {
                    if(plugin_name == MACRO_Plugin_Name_Carved_Files)
                    {
                        QString result_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                        complete_source_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,complete_source_path, Q_FUNC_INFO);
                    }
                    else if(plugin_name == MACRO_Plugin_Name_File_System)
                    {

                        QString dest_dir_path = reports_dir_path;
                        if(!dest_dir_path.endsWith("/"))
                            dest_dir_path.append("/");

                        QString display_file_path = export_path_list.at(i);
                        QString m_file_name = QFileInfo(display_file_path).fileName();

                        struct_global_export_file_system_record obj;
                        obj.record_no = record;
                        obj.source_count_name = source_count_name;
                        obj.display_file_path = display_file_path;
                        obj.displayed_file_name = m_file_name;

                        complete_source_path = export_rsync_obj->pub_get_source_path_to_export_fs_record_by_dbreference(obj,dest_db,Q_FUNC_INFO);

                        if(bool_skin_tone_tag)
                        {
                            complete_source_path = file_path;
                        }

                        if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                                || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                        {
                            //note:---linkfilename not inserted in fs db
                            // so to make entry in 'exportfilelist' table for column 'exp_LINKFILENAME' this work has been done here
                            softlink_filename_list.clear();

                            if(recon_helper_standard_obj->check_is_it_image_file(complete_source_path,display_file_path, Q_FUNC_INFO))
                                softlink_filename_list << m_file_name;
                            else
                                softlink_filename_list << "";

                        }

                    }
                    else if(plugin_name == MACRO_Plugin_Name_Snapshots)
                    {
                        complete_source_path = recon_helper_standard_obj->get_file_infopath_by_dbreference(record,dest_db,table_name,source_count_name,Q_FUNC_INFO);
                    }
                    else
                    {
                        complete_source_path = recon_static_functions::get_complete_file_path(target_initial_path, complete_source_path, Q_FUNC_INFO);
                    }
                }

                QFileInfo info_main2(complete_source_path);
                if(!info_main2.exists())
                {
                    recon_static_functions::app_debug("file does not exist path " +  info_main2.filePath() , Q_FUNC_INFO);

                    QSqlQuery query_export_exist(dest_db);
                    QString cmd = QString("insert into exportfilelist (table_name,record, filepath) values (") +
                            "\"" + table_name + "\"," +
                            "\"" + record + "\"," +
                            "\"" + info_main2.filePath() + "\")";

                    if(!query_export_exist.exec(cmd))
                    {
                        recon_static_functions::app_debug("---FAILED--- to execute query_export_exist " + dest_db.databaseName(), Q_FUNC_INFO);
                        recon_static_functions::app_debug(" error " + query_export_exist.lastError().text() , Q_FUNC_INFO);
                        recon_static_functions::app_debug(" command " + cmd , Q_FUNC_INFO);
                    }

                    continue;

                }


                QString  new_name = recon_static_functions::get_available_stringname(info_main2.fileName(), file_name_list, Q_FUNC_INFO);
                QString dest_file_path = dest_path + "/" + new_name;

                //file_name_list <<  info_main2.fileName();
                file_name_list <<  new_name;


                // size of export_path_list and softlink_filename_list must be same, assured by development team(Er. Suman)
                file_copy_source_list << info_main2.filePath();
                file_copy_destination_list << dest_file_path;
                file_copy_recordno_list << record;
                file_copy_tablename_list << table_name;
                softlink_filepath_list << (QString(link_dir_path) + "/" +  softlink_filename_list.at(i));



                QFileInfo file_info(dest_file_path);
                QString file_relativepath = "./files/" + file_info.fileName();

                bool ok = true;
                QString export_time = QString::number(QDateTime::currentMSecsSinceEpoch()/1000);
                qint64 temp = export_time.toULongLong(&ok);
                //    temp -= system_time_offset;
                export_time = QString::number(temp);


                QSqlQuery query_export(dest_db);

                query_export.prepare("insert into exportfilelist (table_name,record,filepath,exp_newpath,"
                                     "exp_relativepath,exp_size,exp_time,exp_LINKFILENAME,exp_hashmd5,exp_hashsha1)"
                                     "values (?,?,?,?,?, ?,?,?,?,?)");

                query_export.addBindValue(table_name);
                query_export.addBindValue(record);
                query_export.addBindValue(info_main2.filePath());
                query_export.addBindValue(dest_file_path);
                query_export.addBindValue(file_relativepath);
                query_export.addBindValue(recon_static_functions::mining_size(info_main2.filePath(), Q_FUNC_INFO));
                query_export.addBindValue(export_time);
                query_export.addBindValue(softlink_filename_list.at(i));

                if(info_main2.isDir())
                {
                    query_export.addBindValue("Folder");
                    query_export.addBindValue("Folder");
                }
                else
                {
                    query_export.addBindValue(create_md5(info_main2.filePath()));
                    query_export.addBindValue(create_sha1(info_main2 .filePath()));
                }

                if(!query_export.exec())
                {
                    recon_static_functions::app_debug("---FAILED---- to execute query_export " + dest_db.databaseName() , Q_FUNC_INFO);
                    recon_static_functions::app_debug(" error " + query_export.lastError().text() , Q_FUNC_INFO);
                    recon_static_functions::app_debug("query " + query_export.lastQuery() , Q_FUNC_INFO);

                    continue;
                }
            }
        }
    }
    dest_db.close();

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void export_file_module::export_file(QString report_scope, QString tab_name,QStringList selected_tags_list)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    for(int ii = 0 ; ii < destination_db_file_list.size() ; ii++)
    {
        if(bool_cancel_export)
        {
            break;
        }
        QString db_path = destination_db_file_list.at(ii);

        QString table_name = global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name,tab_name);

        file_copy_source_list.clear();
        file_copy_destination_list.clear();
        file_copy_recordno_list.clear();
        file_copy_tablename_list.clear();
        softlink_filepath_list.clear();

        int table_index = table_name_list_tablewidget.indexOf(table_name);
        if(table_index < 0)
            return;

        QStringList export_col_name_list = export_path_list_tablewidget.at(table_index);

        QString file_path_column;

        for(int jj = 0; jj < export_col_name_list.size(); jj++)
        {
            file_path_column = export_col_name_list.at(jj);

            if(report_scope == MACRO_REPORT_SCOPE_FULL_QString || report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
            {
                QString command_select;
                if(report_scope == MACRO_REPORT_SCOPE_FULL_QString)
                {
                    command_select = QString("select " + file_path_column + ", INT ,LINKFILENAME ,source_count_name from '") + table_name + "'";
                    export_queried_files(command_select, table_name,db_path);
                }
                else if(report_scope == MACRO_REPORT_SCOPE_TAGS_QString)
                {
                    QStringList total_record_list = recon_helper_standard_obj->extract_all_record_no_for_selected_tags(table_name, destination_db_file_list.at(ii), selected_tags_list, Q_FUNC_INFO);
                    for(int mm = 0; mm < total_record_list.size(); mm++)
                    {
                        if(narad_muni_for_report_obj->get_field(MACRO_NARAD_Global_Report_Censor_Skin_Tone_Image_Checked_QString).toString() == "1")
                        {
                            bool_censored_image_clicked = true;

                            for(int kk = 0; kk < selected_tags_list.size(); kk++)
                            {
                                QString tag_name = selected_tags_list.at(kk);

                                if(tag_name == MACRO_Tag_Name_Skin_Tone)
                                {

                                    QString fs_db = destination_db_file_list.at(ii);
                                    QString connection_naam_1 = Q_FUNC_INFO + QString("_1");
                                    QSqlDatabase::removeDatabase(connection_naam_1);
                                    QSqlDatabase  fs_source_db;
                                    fs_source_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam_1);
                                    fs_source_db.setDatabaseName(fs_db);

                                    QSqlQuery query_select(fs_source_db);
                                    fs_source_db.transaction();
                                    if(!fs_source_db.open())
                                    {
                                        recon_static_functions::app_debug("Database open ----FAILED---- " + fs_source_db.databaseName() , Q_FUNC_INFO);
                                        recon_static_functions::app_debug("Error " + fs_source_db.lastError().text() , Q_FUNC_INFO);
                                        fs_source_db.commit();
                                        fs_source_db.close();
                                        continue;
                                    }

                                    query_select.prepare("SELECT INT,source_count_name,filename FROM files where recon_tag_value= '" + tag_name + "'");
                                    if(!query_select.exec())
                                    {
                                        recon_static_functions::app_debug(" select query ----FAILED---- " + query_select.executedQuery() , Q_FUNC_INFO);
                                        recon_static_functions::app_debug("Error " + query_select.lastError().text() , Q_FUNC_INFO);
                                        fs_source_db.close();
                                        fs_source_db.commit();
                                        fs_source_db.close();
                                        continue;
                                    }

                                    QString source_count_name;
                                    while(query_select.next())
                                    {
                                        QString record_no_str = query_select.value(0).toString() ;
                                        source_count_name = query_select.value(1).toString() ;
                                        QString filename = query_select.value(2).toString() ;
                                        QString full_filepath = recon_helper_standard_obj->get_file_infopath_by_dbpath(record_no_str,fs_db,"files",source_count_name,Q_FUNC_INFO);

                                        if(record_no_str != total_record_list.at(mm))
                                            continue;

                                        QString censored_images_Dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/") + QString(MACRO_Censored_Skin_Tone_Images_Dir_Name_QString);
                                        QString censored_file_name =  record_no_str + QString("_censored_") + filename;
                                        QString dest_file_path = censored_images_Dir_path + censored_file_name;

                                        recon_helper_standard_obj->get_censored_filepath(full_filepath, dest_file_path);

                                    }
                                    fs_source_db.commit();
                                    fs_source_db.close();
                                }

                            }

                        }
                        else
                        {
                            bool_censored_image_clicked = false;
                        }

                        command_select = QString("select " + file_path_column + ", INT, LINKFILENAME,source_count_name,recon_tag_value from '" + table_name + "' where INT=" + "'" + total_record_list.at(mm) + "'");
                        export_queried_files(command_select, table_name,db_path);
                    }

                }
            }

            if(report_scope == MACRO_REPORT_SCOPE_SCREEN_ITEMS_QString)
            {

                if(tw_data_type_list_tablewidget.size() <= current_tab_index)
                    return;

                if(list_m_tablewidgets.size() <= current_tab_index)
                    return;

                QStringList tw_col_type_list = tw_data_type_list_tablewidget.at(current_tab_index);
                int INT_index = tw_col_type_list.indexOf(MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString,0);

                for(int pp = 0; pp < list_m_tablewidgets.at(current_tab_index)->rowCount() ; pp++)
                {
                    if(pp%10 == 0)
                        QCoreApplication::processEvents();

                    QString search_int = list_m_tablewidgets.at(current_tab_index)->item(pp,INT_index)->text();
                    QString command_select = QString("select " + file_path_column + ", INT ,LINKFILENAME ,source_count_name from '" + table_name + "' where INT = ") + "\"" + search_int + "\"";

                    export_queried_files(command_select, table_name,db_path);
                }

            }
        }


        bool_thread_copied_finished = false;
        th_file_copier_module_popobj->set_destination_list(file_copy_destination_list);
        th_file_copier_module_popobj->set_source_list(file_copy_source_list);
        th_file_copier_module_popobj->set_destination_db(destination_db_file_list.at(ii));
        th_file_copier_module_popobj->set_tablename(file_copy_tablename_list);
        th_file_copier_module_popobj->set_record_no(file_copy_recordno_list);
        th_file_copier_module_popobj->set_softlink_filepath_list(softlink_filepath_list);
        th_file_copier_module_popobj->start();

        int count  = 0;
        recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
        while(1)
        {
            count++;
            if(bool_thread_copied_finished)
                break;

            if(bool_cancel_export)
            {
                break;
            }


            if(count%500 == 0)
                QCoreApplication::processEvents();

        }
        recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    }

    recon_static_functions::app_debug(" -Ends" , Q_FUNC_INFO);
    return;
}


void export_file_module::fill_variable_from_narad_muni()
{

    table_name_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Table_Name_Tablewidget_QStringList).toStringList();

    destination_db_file_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Destination_Db_File_List_QString).toStringList();

    reports_dir_path = narad_muni_for_report_obj->get_field(MACRO_NARAD_Reports_Dir_Path_QString).toString();
    if(!reports_dir_path.endsWith("/"))
        reports_dir_path.append("/");


    current_tab_index = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tabwidget_CurrentIndex_int).toInt();
    plugin_name = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Plugin_Name_QString).toString();

    list_m_tablewidgets.clear();
    QList<QVariant> screen_tablewidget_list_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Screen_Items_m_tablewidget_QList).toList();
    for(int i=0; i < screen_tablewidget_list_list.size(); i++)
    {
        qint64 nn = screen_tablewidget_list_list.at(i).toLongLong();
        list_m_tablewidgets.append((m_tablewidget *) nn);
    }


    export_path_list_tablewidget.clear();
    QList<QVariant> path_list_tablewidget = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Export_Path_Tablewidget_QList).toList();
    for(int i=0; i < path_list_tablewidget.size(); i++)
        export_path_list_tablewidget.append(path_list_tablewidget.at(i).toStringList());


    tw_data_type_list_tablewidget.clear();
    QList<QVariant> data_type_list = narad_muni_for_report_obj->get_field(MACRO_NARAD_Report_Tablewidget_Data_Type_QList).toList();
    for(int i=0; i < data_type_list.size(); i++)
        tw_data_type_list_tablewidget.append(data_type_list.at(i).toStringList());

}

void export_file_module::slot_thread_file_copier_finished()
{
    recon_static_functions::app_debug(" Thread file copier finished!" , Q_FUNC_INFO);

    bool_thread_copied_finished = true;
}


void export_file_module::set_export_entry_null()
{

    for(int i = 0 ; i < destination_db_file_list.size() ; i++)
    {
        recon_static_functions::app_debug(" -start" , Q_FUNC_INFO);

        QString connection_naam = QString(Q_FUNC_INFO);
        QSqlDatabase::removeDatabase(connection_naam);
        QSqlDatabase destination_db;
        destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
        destination_db.setDatabaseName(destination_db_file_list.at(i));
        if(!destination_db.open())
        {
            recon_static_functions::app_debug("---FAILED--- to open database" + destination_db.databaseName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(" error " + destination_db.lastError().text() , Q_FUNC_INFO);
            continue;
        }

        if(!destination_db.tables().contains("exportfilelist"))
        {
            destination_db.close();
            continue ;
        }

        QSqlQuery query_null(destination_db);


        QString command = QString("delete from exportfilelist");
        if(!query_null.exec(command))
        {
            recon_static_functions::app_debug("---FAILED--- to execute query_null" + destination_db.databaseName(), Q_FUNC_INFO);
            recon_static_functions::app_debug(" error " + query_null.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug("command " + command , Q_FUNC_INFO);
            destination_db.close();
            continue ;
        }
        recon_static_functions::app_debug(" -end" , Q_FUNC_INFO);
        destination_db.close();
    }
    return ;
}

QString export_file_module::create_md5(QString file_path)
{
    recon_static_functions::debug_intensive( " start " , Q_FUNC_INFO);

    unsigned char c[MD5_DIGEST_LENGTH];
    MD5_CTX md_context;
    int i, bytes;
    unsigned char data[1024];

    QFileInfo info(file_path);
    if(info.isDir())
        return QString("");



    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(" target_file.open---FAILED---" + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error :- " + target_file.errorString(),Q_FUNC_INFO);
        return QString("");

    }

    MD5_Init(&md_context);
    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        MD5_Update(&md_context, data, bytes);
    }


    QByteArray ar;
    MD5_Final(c, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        ar.append(c[i]);
    }


    target_file.close();

    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);

    return QString(ar.toHex());
}


QString export_file_module::create_sha1(QString file_path)
{
    recon_static_functions::debug_intensive( " start " , Q_FUNC_INFO);

    unsigned char c[SHA_DIGEST_LENGTH];
    SHA_CTX sha_context;
    int i, bytes;
    unsigned char data[1024];
    QString utf8_file_name = QString::fromUtf8(file_path.toLocal8Bit().data());

    QFileInfo info(file_path);
    if(info.isDir())
        return QString("");

    QFile target_file(file_path);

    if(!target_file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug( " target_file.open---FAILED---" + file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error :- " + target_file.errorString(),Q_FUNC_INFO);
        return QString("");

    }
    SHA1_Init(&sha_context);

    while ((bytes = target_file.read((char *)data,1024)) > 0)
        SHA1_Update(&sha_context, data, bytes);


    QByteArray ar;
    SHA1_Final(c, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
        ar.append(c[i]);


    target_file.close();


    recon_static_functions::debug_intensive(" end " , Q_FUNC_INFO);

    return QString(ar.toHex());
}
