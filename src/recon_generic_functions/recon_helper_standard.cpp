#include "recon_helper_standard.h"

recon_helper_standard::recon_helper_standard(QObject *parent)
{
    recon_static_functions::app_debug("start", Q_FUNC_INFO);

    initialize_image_extension_list();
    recon_static_functions::app_debug("rc standard 11", Q_FUNC_INFO);
    initialize_audio_extension_list();
    recon_static_functions::app_debug("rc standard 22", Q_FUNC_INFO);
    initialize_vedio_extension_list();


    recon_static_functions::app_debug("end", Q_FUNC_INFO);

}

recon_helper_standard::~recon_helper_standard()
{

}

bool recon_helper_standard::is_any_source_exist()
{
    bool bool_source_path_exists  = false;
    QList<struct_GLOBAL_witness_info_source> list_target_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_QList();
    for(qint64 count = 0; count < list_target_source_info.size(); count++)
    {
        struct_GLOBAL_witness_info_source obj = list_target_source_info.at(count);
        QString virtual_source_path = obj.virtual_source_path;
        if(QFileInfo(virtual_source_path).exists())
        {
            bool_source_path_exists = true;
            break ;
        }
    }

    return bool_source_path_exists;
}

bool recon_helper_standard::is_gallery_view_suppoprted_file(QString file_name, QString caller_func)
{
    if(file_name.endsWith(".png", Qt::CaseInsensitive) || file_name.endsWith(".jpg", Qt::CaseInsensitive)
            || file_name.endsWith(".gif", Qt::CaseInsensitive) || file_name.endsWith(".icns", Qt::CaseInsensitive)
            || file_name.endsWith(".tif", Qt::CaseInsensitive) || file_name.endsWith(".svg", Qt::CaseInsensitive)
            || file_name.endsWith(".bmp", Qt::CaseInsensitive) || file_name.endsWith(".ppm", Qt::CaseInsensitive)
            || file_name.endsWith(".xbm", Qt::CaseInsensitive)|| file_name.endsWith(".jpeg", Qt::CaseInsensitive)
            || file_name.endsWith(".heic", Qt::CaseInsensitive) || file_name.endsWith(".webp",Qt::CaseInsensitive))
    {
        return true;
    }

    return false;
}

bool recon_helper_standard::is_gallery_view_suppoprted_video_file(QString file_name)
{
    if(file_name.endsWith(".mp4",Qt::CaseInsensitive) || file_name.endsWith(".3gp",Qt::CaseInsensitive)
            || file_name.endsWith(".movie",Qt::CaseInsensitive) || file_name.endsWith(".mov",Qt::CaseInsensitive)
            || file_name.endsWith(".m4v",Qt::CaseInsensitive) || file_name.endsWith(".quicktime",Qt::CaseInsensitive)
            || file_name.endsWith(".mpeg",Qt::CaseInsensitive) || file_name.endsWith(".x-flv",Qt::CaseInsensitive)
            || file_name.endsWith(".mkv",Qt::CaseInsensitive))
    {
        return true;
    }

    return false;
}

QMap <QString ,QMap <QString ,QString> > recon_helper_standard::extract_apple_metadata_from_script(QString text_file_path , QString csv_file_path, QStringList apple_all_metadata_attribute_list)
{
    recon_static_functions::app_debug("Start" ,Q_FUNC_INFO);

    QMap <QString ,QMap <QString ,QString> > full_apple_mtdta_qmap;

    if(!QFileInfo(text_file_path).exists())
    {
        recon_static_functions::app_debug("text File not exist -----FAILED----" + text_file_path, Q_FUNC_INFO);
        return full_apple_mtdta_qmap;
    }
    if(!QFileInfo(csv_file_path).exists())
    {
        recon_static_functions::app_debug("csv File not exist -----FAILED----" + csv_file_path, Q_FUNC_INFO);
        return full_apple_mtdta_qmap;
    }


    QFile csv_file (csv_file_path);
    if(!csv_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        recon_static_functions::app_debug("csv File open -----FAILED----" + csv_file.fileName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + csv_file.errorString() , Q_FUNC_INFO);
        return full_apple_mtdta_qmap;
    }

    QByteArray csv_data_arr = csv_file.readAll().trimmed();
    QString csv_data = QString::fromLocal8Bit(csv_data_arr, csv_data_arr.size());
    csv_file.close();


    QStringList all_node_list = parse_spotlight_parser_file_for_each_inode(text_file_path);

    QMap<QString, QMap<QString, QString> >  pp_map = extract_apple_metadata_from_spotlight_text_file_data(all_node_list,apple_all_metadata_attribute_list);


    QStringList line_list = csv_data.split("\n");

    for(int ii = 0 ; ii < line_list.size() ; ii++)
    {
        QString eachline = line_list.at(ii);
        eachline = eachline.trimmed();

        QStringList final_data = eachline.split("\t");
        if(final_data.size() < 2)
            continue;

        QString full_file_path = final_data.at(1);
        if(!full_file_path.contains("/"))
            continue;

        QString inode_num = final_data.at(0);
        inode_num = inode_num.trimmed();


        QMap <QString ,QString> m_map;
        //m_map = extract_apple_metadata_from_spotlight_text_file_data(inode_num , txt_file_data);

        m_map = pp_map.value(inode_num);


        full_file_path.remove(0,(full_file_path.indexOf("/")));

        full_apple_mtdta_qmap.insert(full_file_path , m_map);
    }

    recon_static_functions::app_debug("End" ,Q_FUNC_INFO);

    return full_apple_mtdta_qmap;
}

bool recon_helper_standard::bool_is_permission_restricted_file(QString mfile_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Start " , Q_FUNC_INFO);


    if(mfile_path.contains("/.DocumentRevisions-V") || mfile_path.contains("/.Trashes") || mfile_path.contains("/.TemporaryItems"))
    {
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - end true " , Q_FUNC_INFO);
        return true;
    }

    return false;
}

QMap<QString, QMap<QString, QString> > recon_helper_standard::extract_apple_metadata_from_spotlight_text_file_data(QStringList each_inode_data_list, QStringList apple_all_attribute_name_list)
{
    recon_static_functions::debug_intensive(" start ",Q_FUNC_INFO);

    QMap<QString, QMap<QString, QString> > map_inode_map_key_value;


    for(int ls = 0; ls < each_inode_data_list.size(); ls++)
    {
        QString data = each_inode_data_list.at(ls);
        data.remove("\n "); // space at last
        data.remove("\r");

        QStringList metdata_list;
        if(data.contains("\n"))
            metdata_list = data.split("\n");

        if(metdata_list.isEmpty())
            continue;


        QString inode_no;
        for(int pp = 0; pp < metdata_list.size() ; pp++)
        {
            QString line = metdata_list.at(pp);
            line = line.trimmed();


            if(line.startsWith("Inode_Num -->"))
            {
                QStringList tt_list;
                tt_list = line.split("-->");

                inode_no = tt_list.at(1);
                inode_no = inode_no.trimmed();

                break;
            }
        }


        if(inode_no.isEmpty())
            continue;


        QMap <QString ,QString> m_Qmap;

        for(int dd = 0; dd < metdata_list.size() ; dd++)
        {
            QString line = metdata_list.at(dd);
            line = line.trimmed();


            if(!line.startsWith("kMD")) // getting help from apple_metadata.csv from bundle resources
                continue;

            QStringList arrow_splitted_list;
            arrow_splitted_list = line.split("-->");

            if(arrow_splitted_list.size() >= 1)
            {
                QString m_keys = arrow_splitted_list.at(0);
                m_keys = m_keys.trimmed();
                if(!apple_all_attribute_name_list.contains(m_keys))
                    continue;

                QString m_values = arrow_splitted_list.at(1);
                m_values = m_values.trimmed();

                if(m_keys.contains("Date") && !m_keys.contains("Dates"))
                {
                    m_values =  QString::number(global_recon_helper_singular_class_obj->convert_timestamp_readable_to_numeric_1(m_values));
                }
                else if(m_keys.contains("Dates"))
                {
                    QString values_str = m_values;
                    if(values_str.contains(MACRO_RECON_Splitter_6_comma))
                    {
                        QStringList split_list = values_str.split(MACRO_RECON_Splitter_6_comma);
                        QStringList value_converted_list;
                        for(int ii = 0; ii < split_list.size(); ii++)
                        {
                            QString used_dates = split_list.at(ii);
                            used_dates = used_dates.trimmed();

                            value_converted_list << QString::number(global_recon_helper_singular_class_obj->convert_timestamp_readable_to_numeric_1(used_dates));

                        }
                        m_values = value_converted_list.join(";");
                    }
                    else
                    {
                        m_values = QString::number(global_recon_helper_singular_class_obj->convert_timestamp_readable_to_numeric_1(values_str));
                    }
                }
                else if(m_keys.contains("ContentTypeTree") || m_keys.contains("WhereFroms"))
                {
                    QString values_str = m_values;
                    m_values = values_str.replace(MACRO_RECON_Splitter_6_comma, ";");
                }

                m_Qmap.insert(m_keys, m_values);

            }
        }

        //This check is necessary because text file contains multiple entry with same inode number
        //so to provide all the recquired extended attributes below code is done.
        if(map_inode_map_key_value.contains(inode_no))
        {

            bool bool_content_type_valid = true;
            QMapIterator<QString,QString> map_itr(m_Qmap);
            while(map_itr.hasNext())
            {
                map_itr.next();
                QString m_key = map_itr.key();
                QString m_value = map_itr.value();
                if(m_key == "kMDItemContentType")
                {
                    if(m_value.contains("INVALID"))
                    {
                        //if content_type is invalid then there is no need to insert entry in the main map
                        bool_content_type_valid = false;
                    }
                    break;
                }
            }

            if(bool_content_type_valid)
            {
                QMap<QString,QString> existing_map = map_inode_map_key_value.value(inode_no);
                QMapIterator<QString, QString> curr_map_itr(m_Qmap);

                while (curr_map_itr.hasNext())
                {
                    curr_map_itr.next();
                    QString curr_key = curr_map_itr.key();
                    QString curr_value = curr_map_itr.value();

                    if(existing_map.contains(curr_key))
                    {
                        QString exist_value = existing_map.value(curr_key);
                        if(exist_value.trimmed().isEmpty())
                        {
                            //If there is already an item with the key key, that item's value is replaced with value
                            //only if the 'existing value of the key' in 'existing map' is blank
                            existing_map.insert(curr_key,curr_value);
                        }
                    }
                    else
                    {
                        // if no such key is present then new entry of that key will get inserted in the existing map
                        existing_map.insert(curr_key,curr_value);
                    }
                }

                map_inode_map_key_value.insert(inode_no,existing_map);
            }

        }
        else
        {
            map_inode_map_key_value.insert(inode_no,m_Qmap);
        }

    }

    recon_static_functions::debug_intensive(" end ",Q_FUNC_INFO);

    return map_inode_map_key_value;
}

QString recon_helper_standard::pub_get_raw_data_for_image_file_preview(QString filepath)
{
    QImage q_image(filepath);
    q_image = q_image.scaled(200, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QString base_64_data;
    QByteArray arr;
    QBuffer buf(&arr);
    q_image.save(&buf, "JPG" );
    base_64_data = QString::fromLocal8Bit(arr.toBase64());

    return base_64_data;
}

bool recon_helper_standard::is_special_file_for_preview(QString filepath , QString display_file_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Start " , Q_FUNC_INFO);

    QString file_extension = mining_suffix(filepath,Q_FUNC_INFO);
    if(file_extension.endsWith("heic" , Qt::CaseInsensitive))
    {
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - end " , Q_FUNC_INFO);
        return true;
    }

    QString file_extension_2 = mining_suffix(display_file_path,Q_FUNC_INFO);
    if(file_extension_2.endsWith("heic" , Qt::CaseInsensitive))
    {
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - end " , Q_FUNC_INFO);
        return true;
    }



    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - end " , Q_FUNC_INFO);
    return false;
}

QStringList recon_helper_standard::get_tags_and_colours_list_from_db(QString command,QString destination_db_file, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Start " , Q_FUNC_INFO);

    QStringList list_gen;

    QFileInfo info(destination_db_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Db NOT Exists ---FAILED--- " + destination_db_file ,Q_FUNC_INFO);
        return list_gen;
    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return list_gen;
    }

    QSqlQuery query(*destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return list_gen;
    }

    while(query.next())
    {
        QString value_str = query.value(0).toString().trimmed();

        value_str.append("::");
        value_str.append(query.value(1).toString().trimmed());

        list_gen << value_str;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -End " , Q_FUNC_INFO);

    return list_gen;
}

void recon_helper_standard::notes_search_delete_dbs(QString result_feature_dir, QString tabname, QString record_no, QString notes_data, QString category, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString separate_db_path = result_feature_dir + "all_notes" + ".sqlite";
    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(separate_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + separate_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    QSqlQuery query(*destination_db);
    query.prepare("delete from notes where category = ? and record_no = ? and tab_name = ? and notes != ?");
    query.addBindValue(category);
    query.addBindValue(record_no);
    query.addBindValue(tabname);
    query.addBindValue(notes_data);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + separate_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return;
}

QString recon_helper_standard::get_adhoc_path_for_database_using_QFile(QString main_source_path)
{

    // Input Path : /Users/maruti/Library/Application Support/MobileSync/Backup/64f1899db1dc45affa697c8f1dd00dec6f5ba229/Manifest.db
    // Return Path : /Users/maruti/aaout put/Result_2018-Jan-03T15-13-11/tmp/m_sqlite/Manifest.db


    if(main_source_path.trimmed().isEmpty())
        return QString("");


    QString shm_source_path = main_source_path + "-shm";
    QString wal_source_path = main_source_path + "-wal";

    QFileInfo m_info(main_source_path);

    QString sql_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Adhoc_Path_Location_Miscellenous_In_Result_QString).toString();
    QString  avail_file = recon_static_functions::get_available_filename(m_info.fileName(), sql_path, Q_FUNC_INFO);


    QString main_dest_path = sql_path + avail_file;
    QString shm_dest_path = sql_path + avail_file + "-shm";
    QString wal_dest_path = sql_path + avail_file + "-wal";

    QFileInfo info_temp_check(main_dest_path);
    if(info_temp_check.exists())
    {
        QFile temp_file(main_dest_path);
        temp_file.remove();
    }
    QFile::copy(main_source_path,main_dest_path);

    QFileInfo shm_file_info(shm_source_path);
    QFileInfo wal_file_info(wal_source_path);

    if(shm_file_info.exists())
    {
        QFile::copy(shm_source_path,shm_dest_path);
    }
    if(wal_file_info.exists())
    {
        QFile::copy(wal_source_path,wal_dest_path);
    }

    return main_dest_path;
}

QString recon_helper_standard::get_adhoc_path_for_database_using_cp_without_sudo(QString source_db_path, QString destination_dir_path)
{
    // Input Source Path: /Library/Application\ Support/com.apple.TCC/TCC.db
    // Input Dest Path : /private/var/tmp/RECON_LAB_forenso/1670299564/full_disk_access/
    // Output : /private/var/tmp/RECON_LAB_forenso/1670299564/full_disk_access/TCC.db

    if(source_db_path.trimmed().isEmpty())
        return QString("");

    QDir tmp_dir(destination_dir_path);
    if(!tmp_dir.exists())
        tmp_dir.mkdir(destination_dir_path);

    QFileInfo source_info(source_db_path);
    QString source_file_name = source_info.fileName();

    QString wal_source_path = source_db_path + "-wal";
    QString shm_source_path = source_db_path + "-shm";

    QStringList cp_args;
    cp_args << source_db_path << shm_source_path << wal_source_path << destination_dir_path;

    QProcess proc;
    proc.start("cp", cp_args);
    proc.waitForStarted();

    while(1)
    {
        QApplication::processEvents();
        if(proc.state() == QProcess::NotRunning)
            break;
    }

    proc.close();

    QString tmp_db_path =  QString(destination_dir_path + "/" + source_file_name);

    QFile tmp_file(tmp_db_path);
    if(!tmp_file.exists())
    {
        return QString("");
    }

    return tmp_db_path;
}

QString recon_helper_standard::get_adhoc_path_for_database_using_cp_with_sudo(QString source_db_path, QString password, QString destination_dir_path, QString caller_func)
{
    // Input Source Path: /Library/Application\ Support/com.apple.TCC/TCC.db
    // Input Dest Path : /private/var/tmp/RECON_LAB_forenso/1670299564/full_disk_access/
    // Output : /private/var/tmp/RECON_LAB_forenso/1670299564/full_disk_access/TCC.db

    if(source_db_path.trimmed().isEmpty())
        return QString("");

    QDir tmp_dir(destination_dir_path);
    if(!tmp_dir.exists())
        tmp_dir.mkdir(destination_dir_path);

    QFileInfo source_info(source_db_path);
    QString source_file_name = source_info.fileName();

    QString shm_source_path = source_db_path + "-shm";
    QString wal_source_path = source_db_path + "-wal";

    QString script_avail_file_name = recon_static_functions::get_available_filename("m_script", destination_dir_path, caller_func);

    QString script_avail_file_path = destination_dir_path + script_avail_file_name;

    QFile scr_file(script_avail_file_path);

    QString dest_db_path =  QString(destination_dir_path + "/" + source_file_name);

    if(scr_file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QString cmd_copy = QString("echo ") + password + QString(" | sudo -S cp  \"") + source_db_path + QString("\" \"") +
                destination_dir_path  + QString("\"");

        QString cmd_copy_shm_file = QString("echo ") + password + QString(" | sudo -S cp  \"") + shm_source_path + QString("\" \"") +
                destination_dir_path  + QString("\"");

        QString cmd_copy_wal_file = QString("echo ") + password + QString(" | sudo -S cp  \"") + wal_source_path + QString("\" \"") +
                destination_dir_path  + QString("\"");

        QString cmd_change_permission = QString("echo ") + password + QString(" | sudo -S chmod 0777  \"") +  dest_db_path  + QString("\"");

        scr_file.write(cmd_copy.toLocal8Bit());
        scr_file.write("\n");
        scr_file.write(cmd_copy_shm_file.toLocal8Bit());
        scr_file.write("\n");
        scr_file.write(cmd_copy_wal_file.toLocal8Bit());
        scr_file.write("\n");
        scr_file.write(cmd_change_permission.toLocal8Bit());
        scr_file.flush();
        scr_file.close();

        QProcess proc;
        proc.start("sh", QStringList(script_avail_file_path));
        proc.waitForStarted();

        while(1)
        {
            QApplication::processEvents();
            if(proc.state() == QProcess::NotRunning)
                break;
        }

        proc.close();
    }

    if(QFileInfo(script_avail_file_path).exists())
    {
        QFile(script_avail_file_path).remove();
    }


    QFile tmp_file(dest_db_path);
    if(!tmp_file.exists())
    {
        return QString("");
    }

    return dest_db_path;
}

qint64 recon_helper_standard::get_time_zone_offset_with_sign(QString timezone_string,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  START",Q_FUNC_INFO + caller_func);

    // Input - "Asia/Kolkata-IST-GMT+5:30"
    // Output - 19800          ----   Not = +19800
    // Output - -18000

    if(!timezone_string.contains("-"))
        return MACRO_FAIL_VALUE_NUMERIC_int;

    QStringList list = timezone_string.split("-", Qt::SkipEmptyParts);
    QString timezone_name = list.at(0);


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName("../Resources/timezone.sqlite");
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  -destination_db->open() ---FAILED---- ",Q_FUNC_INFO + caller_func);
        recon_static_functions::app_debug("error " + destination_db->lastError().text(),Q_FUNC_INFO + caller_func);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return MACRO_FAIL_VALUE_NUMERIC_int;
    }

    QSqlQuery query(*destination_db);


    qint64 zone_offset_in_qint64 = get_time_zone_offset_without_sign(timezone_name ,*destination_db,Q_FUNC_INFO);
    query.prepare("select distinct name,code, timezone, off_seconds from timezone where name = ? AND off_seconds = ?");
    query.addBindValue(timezone_name);
    query.addBindValue(QString::number(zone_offset_in_qint64));
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query.exec()  ---FAILED---- ",Q_FUNC_INFO + caller_func);
        recon_static_functions::app_debug("Error - " + query.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(query.executedQuery(),Q_FUNC_INFO + caller_func);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return MACRO_FAIL_VALUE_NUMERIC_int;
    }

    qint64 system_time_zone_offset_seconds = MACRO_FAIL_VALUE_NUMERIC_int;
    if(query.next())
    {
        QString tmp_sec = query.value(3).toString();

        bool ok;
        system_time_zone_offset_seconds = tmp_sec.toULongLong(&ok);

        if(query.value(2).toString().contains("GMT-"))
        {
            system_time_zone_offset_seconds = system_time_zone_offset_seconds * -1;
        }
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    return system_time_zone_offset_seconds;

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - END",Q_FUNC_INFO + caller_func);
}

QString recon_helper_standard::get_complete_timezone_name_label(QString time_zone,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " START",Q_FUNC_INFO);


    // Input - "Asia/Kolkata"
    // Output - "Asia/Kolkata-IST-GMT+5:30"

    QString timezone_with_offset;

    if(time_zone.trimmed().isEmpty())
        return timezone_with_offset;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName("../Resources/timezone.sqlite");

    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " destination_db ---FAILED---  ../Resources/timezone.sqlite",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return timezone_with_offset;
    }

    qint64 zone_offset_in_qint64 = get_time_zone_offset_without_sign(time_zone,*destination_db,Q_FUNC_INFO);

    QSqlQuery query(*destination_db);

    query.prepare("select distinct name,code, timezone, off_seconds from timezone where name=? AND off_seconds=?");
    query.addBindValue(time_zone);
    query.addBindValue(QString::number(zone_offset_in_qint64));
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query ---FAILED--- ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error " + query.lastError().text(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return timezone_with_offset;
    }

    if(query.next())
    {
        timezone_with_offset = query.value(0).toString() + "-" + query.value(1).toString() + "-" + query.value(2).toString();

    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    return timezone_with_offset;

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " END",Q_FUNC_INFO);
}

bool recon_helper_standard::check_value_existence_in_db_by_dbreference(QString command,int pos,QString value_to_match,QSqlDatabase destination_db,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------db file - "  + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return false;
    }

    QStringList list_gen;

    while(query.next())
    {
        list_gen << query.value(pos).toString().trimmed();
    }

    bool bool_value_exist = false;
    for(int i = 0 ; i < list_gen.size() ; i++)
    {
        QString extract_value = list_gen.at(i);

        if(value_to_match.trimmed() == extract_value.trimmed())
        {
            bool_value_exist = true;
            break;
        }
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);


    if(bool_value_exist)
    {
        return true;
    }

    return false;

}

bool recon_helper_standard::check_value_existence_in_db_by_dbpath(QString command,int pos,QString value_to_match,QString destination_db_file,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    bool mstatus = false;

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return mstatus;
    //    }


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;

        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return mstatus;
    }

    mstatus = check_value_existence_in_db_by_dbreference(command, pos, value_to_match, *destination_db, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return mstatus;
}

QString recon_helper_standard::get_target_os_scheme_display_on_current_record_source(QString record_no, QString table_name, QString plugin_db_path, QString caller_func)
{
    QString source_count_name_of_current_record = get_source_count_name_on_current_record(record_no, table_name, plugin_db_path, Q_FUNC_INFO + caller_func);

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_of_current_record);

    return struct_target_info.os_scheme_display;
}

QString recon_helper_standard::get_target_os_scheme_internal_on_current_record_source(QString record_no, QString table_name, QString plugin_db_path, QString caller_func)
{
    QString source_count_name_of_current_record = get_source_count_name_on_current_record(record_no, table_name, plugin_db_path, Q_FUNC_INFO + caller_func);

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_of_current_record);

    return struct_target_info.os_scheme_internal;
}


QString recon_helper_standard::get_source_count_name_on_current_record(QString record_no, QString table_name, QString plugin_db_path, QString caller_func)
{
    QString command = "SELECT source_count_name FROM '" + table_name + "' WHERE INT = ?";
    QStringList arg_list;
    arg_list << record_no;
    QString source_count_name =  get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,plugin_db_path,Q_FUNC_INFO + caller_func);

    return source_count_name;
}

QString recon_helper_standard::pub_get_fs_display_path_according_recon_file_infopath(QString recon_file_infopath, QString source_count_name)
{
    //Input:- /46/46b15c3ded00f15cf453d321c92476f33433cf50
    //Output:- /CameraRollDomain/Media/PhotoData/CPLAssets/group82/71B6EEB1-6905-4ADB-8C59-4F7FF5C12EEC.JPG

    //OR

    //Input:- /Users/jermyn/Pictures/Photo Booth Library/Pictures/Photo on 1-28-13 at 9.11 AM #2.jpg
    //Output:- /Users/jermyn/Pictures/Photo Booth Library/Pictures/Photo on 1-28-13 at 9.11 AM #2.jpg

    recon_file_infopath = recon_file_infopath.trimmed();
    if(recon_file_infopath.isEmpty())
    {
        recon_static_functions::app_debug(" -----FAILED----- path empty, ios dir may be blank ,  Source - " + source_count_name, Q_FUNC_INFO);
        return recon_file_infopath;
    }

    QString fs_db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

    QString fs_disp_file_path =  get_string_from_db_with_addbindvalues_by_dbpath(QString("Select filepath From files Where recon_file_infopath = ?"), QStringList(recon_file_infopath), 0, fs_db_path, Q_FUNC_INFO);

    if(fs_disp_file_path.trimmed().isEmpty())
        return recon_file_infopath;

    return fs_disp_file_path;
}

QString recon_helper_standard::pub_get_result_version()
{
    QString case_info_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.sqlite";

    QString result_version=  get_string_from_db_by_dbpath(QString("Select app_version from casedetails"), 0, case_info_db_path, Q_FUNC_INFO);
    result_version = result_version.trimmed();

    if(result_version.isEmpty())
    {
        QString file_name = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_info.txt";

        QString m_key = MACRO_NARAD_Result_App_Version_QString;

        result_version = global_recon_helper_singular_class_obj->pub_get_value_from_source_textinfo_file_only(file_name, m_key);
    }

    return result_version;
}

QString recon_helper_standard::get_snapshots_mount_path(QString source_count_name, QString db_path)
{
    QString rcvd_snp_db_name = QFileInfo(db_path).fileName();
    rcvd_snp_db_name.chop(QString(".sqlite").size());

    QString snp_diff_index_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + "saved_snapshots_index.sqlite";
    QString cmd_save_index = QString("select secondary_source from tbl_saved_index where snapshot_lable_name = ? AND source_count_name = ?");
    QStringList args_list;
    args_list << rcvd_snp_db_name << source_count_name;
    QString actl_snpname =  get_string_from_db_with_addbindvalues_by_dbpath(cmd_save_index, args_list, 0, snp_diff_index_db_path, Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source tgt_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QList<struct_GLOBAL_snapshots_info>snapshots_QList = tgt_obj.snapshots_QList;

    for(int ii = 0 ; ii < snapshots_QList.size() ; ii++)
    {
        if(snapshots_QList.at(ii).snapshot_actual_name == actl_snpname)
        {
            return snapshots_QList.at(ii).snapshot_mount_path;
        }
    }

    return "";
}


void recon_helper_standard::update_narad_variable_with_exif_metadata_info(QString caller_func)
{
    recon_static_functions::app_debug("  - START",Q_FUNC_INFO);

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }

    //-Extended Attributes..
    QSqlQuery query_extended_attr(*destination_db);
    QString command_extended_attr = QString("select attribute_key,custom_column_name from tbl_exif_attributes");
    query_extended_attr.prepare(command_extended_attr);
    if(!query_extended_attr.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_extended_attr,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_extended_attr.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_filter_attributes;
    QStringList list_filter_custom_col_name;
    while(query_extended_attr.next())
    {
        list_filter_attributes << query_extended_attr.value(0).toString();
        list_filter_custom_col_name << query_extended_attr.value(1).toString();
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_All_Key_QStringList,list_filter_attributes);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_All_Custom_Column_Name_QStringList,list_filter_custom_col_name);

    //========
    command_extended_attr = QString("select attribute_key,custom_column_name from tbl_exif_attributes where bookmark_metadata = 1");
    query_extended_attr.prepare(command_extended_attr);
    if(!query_extended_attr.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_extended_attr,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_extended_attr.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_metadata_attributes;
    QStringList list_metadata_custom_col_name;
    while(query_extended_attr.next())
    {
        list_metadata_attributes << query_extended_attr.value(0).toString();
        list_metadata_custom_col_name << query_extended_attr.value(1).toString();
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Key_QStringList,list_metadata_attributes);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Metadata_Custom_Column_Name_QStringList,list_metadata_custom_col_name);
    //========

    //========
    command_extended_attr = QString("select attribute_key,custom_column_name from tbl_exif_attributes where bookmark_report = 1");
    query_extended_attr.prepare(command_extended_attr);
    if(!query_extended_attr.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_extended_attr,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_extended_attr.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_report_attributes;
    QStringList list_report_custom_col_name;
    while(query_extended_attr.next())
    {
        list_report_attributes << query_extended_attr.value(0).toString();
        list_report_custom_col_name << query_extended_attr.value(1).toString();
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Report_Key_QStringList,list_report_attributes);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Exif_Attributes_For_Report_Custom_Column_Name_QStringList,list_report_custom_col_name);
    //========





    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    recon_static_functions::app_debug("  - end",Q_FUNC_INFO);

}

QStringList recon_helper_standard::get_export_path_list_of_plugin_current_record(QString plugin_db_file,QStringList export_col_name_list,QString table_name, QString record_no, QString recon_result_dir_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QStringList all_path_list;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(plugin_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + plugin_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return all_path_list;
    }

    QString file_path_column;
    for(int j = 0; j < export_col_name_list.size(); j++)
    {
        file_path_column = export_col_name_list.at(j);

        QSqlQuery query_select_export(*destination_db);
        query_select_export.prepare("select " + file_path_column + " from '" + table_name + "' where INT =?");
        query_select_export.addBindValue(record_no);


        if(!query_select_export.exec())
        {
            recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + plugin_db_file,caller_func);
            recon_static_functions::app_debug("Error - " + query_select_export.lastError().text() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select_export.executedQuery() ,Q_FUNC_INFO);
            continue;
        }


        while(query_select_export.next())
        {
            QString file_path = query_select_export.value(0).toString();
            if(file_path.trimmed().isEmpty())
                continue;

            QString os_schme_dsply = get_target_os_scheme_display_on_current_record_source(record_no , table_name,  plugin_db_file,  Q_FUNC_INFO);

            if(file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)
                    || file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
            {
                QStringList temp_path_list;
                if(file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    temp_path_list = file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                }
                else
                {
                    temp_path_list = file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                }


                for(int i = 0; i < temp_path_list.size() ; i++)
                {
                    QString temp_file_path = temp_path_list.at(i);

                    if(os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
                    {
                        QString target_initial_path = get_virtual_source_path_of_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);
                        if(temp_file_path.startsWith("/"))
                            temp_file_path.remove(0,1);

                        temp_file_path = target_initial_path + temp_file_path;


                    }
                    else if(os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display))
                    {

                        QString target_initial_path = get_virtual_source_path_of_current_record(record_no , table_name,  plugin_db_file,Q_FUNC_INFO);

                        if(temp_file_path.startsWith("/"))
                            temp_file_path.remove(0,1);


                        temp_file_path = target_initial_path + temp_file_path;

                    }
                    else
                    {
                        QString target_initial_path = get_virtual_source_path_of_current_record(record_no,table_name,plugin_db_file,Q_FUNC_INFO);
                        temp_file_path = recon_static_functions::get_complete_file_path(target_initial_path, temp_file_path, Q_FUNC_INFO);
                    }

                    all_path_list << temp_file_path;
                }

            }
            else
            {
                if(os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_macOS_Display))
                {
                    QString target_initial_path = get_virtual_source_path_of_current_record(record_no , table_name,  plugin_db_file,Q_FUNC_INFO);
                    if(target_initial_path.startsWith("/"))
                        target_initial_path.remove(0,1);

                    file_path = target_initial_path + file_path;
                }
                else if(os_schme_dsply == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_winOS_Display))
                {

                    QString target_initial_path = get_virtual_source_path_of_current_record(record_no , table_name,  plugin_db_file,Q_FUNC_INFO);

                    if(file_path.startsWith("/"))
                        file_path.remove(0,1);


                    file_path = target_initial_path + file_path;

                }
                else
                {
                    QString target_initial_path = get_virtual_source_path_of_current_record(record_no , table_name,  plugin_db_file,Q_FUNC_INFO);
                    file_path = recon_static_functions::get_complete_file_path(target_initial_path, file_path, Q_FUNC_INFO);
                }

                if(!file_path.startsWith("/"))
                    file_path.prepend("/");

                all_path_list << file_path;
            }

        } // while ends
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return all_path_list;
}

struct_global_artifacts_export_fileinfo recon_helper_standard::get_export_path_list_and_display_path_list_for_plugin_current_record(QString plugin_db_file,QStringList export_col_name_list,QString table_name, QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    struct_global_artifacts_export_fileinfo obj;
    QStringList all_path_list;
    QStringList display_file_path_list;


    QStringList recon_file_info_path_list;
    QStringList recon_file_frominfo_list;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(plugin_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + plugin_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString file_path_column;
    for(int j = 0; j < export_col_name_list.size(); j++)
    {
        file_path_column = export_col_name_list.at(j);

        QSqlQuery query_select_export(*destination_db);
        query_select_export.prepare("select " + file_path_column + " , source_count_name from '" + table_name + "' where INT =?");
        query_select_export.addBindValue(record_no);

        if(!query_select_export.exec())
        {
            recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + plugin_db_file,caller_func);
            recon_static_functions::app_debug("Error - " + query_select_export.lastError().text() ,Q_FUNC_INFO);
            recon_static_functions::app_debug(query_select_export.executedQuery() ,Q_FUNC_INFO);
            continue;
        }

        while(query_select_export.next())
        {
            QString file_path = query_select_export.value(0).toString();
            if(file_path.trimmed().isEmpty())
                continue;

            QString source_count_name = query_select_export.value("source_count_name").toString();

            struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
            QString virtual_source_path = struct_target_info.virtual_source_path;
            QString os_schme_intrnl = struct_target_info.os_scheme_internal;

            if(file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)
                    || file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
            {
                QStringList temp_path_list;
                if(file_path.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    temp_path_list = file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                }
                else
                {
                    temp_path_list = file_path.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                }


                for(int i = 0; i < temp_path_list.size() ; i++)
                {
                    QString temp_file_path = temp_path_list.at(i);
                    QString file_info_path = temp_file_path;
                    if(!file_info_path.startsWith("/"))
                        file_info_path.prepend("/");


                    temp_file_path = recon_static_functions::get_complete_file_path(virtual_source_path, temp_file_path, Q_FUNC_INFO);
                    if(!temp_file_path.startsWith("/"))
                        temp_file_path.prepend("/");

                    recon_file_info_path_list << file_info_path;
                    recon_file_frominfo_list << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);

                    if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                            || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                    {
                        display_file_path_list << pub_get_fs_display_path_according_recon_file_infopath(file_info_path,source_count_name);
                    }
                    else
                        display_file_path_list << file_info_path;

                    all_path_list << temp_file_path;
                }

            }
            else
            {

                QString file_info_path = file_path.trimmed();
                if(!file_info_path.startsWith("/") && !file_info_path.isEmpty())
                    file_info_path.prepend("/");

                file_path = recon_static_functions::get_complete_file_path(virtual_source_path, file_path, Q_FUNC_INFO);
                file_path = file_path.trimmed();
                if(!file_path.startsWith("/") && !file_path.isEmpty())
                    file_path.prepend("/");

                recon_file_info_path_list << file_info_path;
                recon_file_frominfo_list  << QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main);

                if(os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
                        || os_schme_intrnl == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
                {
                    display_file_path_list << pub_get_fs_display_path_according_recon_file_infopath(file_info_path,source_count_name);
                }
                else
                    display_file_path_list << file_info_path;


                all_path_list << file_path;
            }

        } // while ends
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    obj.displayed_file_path = display_file_path_list;
    obj.complete_export_filepaths = all_path_list;

    obj.recon_file_info_path_list = recon_file_info_path_list;
    obj.recon_filefrom_list = recon_file_frominfo_list;

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return obj;
}

QStringList recon_helper_standard::get_keyword_list_for_search_from_db(QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " START",Q_FUNC_INFO);

    QString recon_config_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QStringList keywords_list;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(recon_config_db_path);
    if (!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " db open ------FAILED-------" + recon_config_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return keywords_list;
    }



    QString command_str = QString("Select category from keyword_category Where category_status = ?");
    QStringList value_list;
    value_list << QString::number(1);

    QStringList keyword_category_list;
    keyword_category_list =  get_stringlist_from_db_with_addbindvalues_by_dbreference(command_str, value_list, 0, *destination_db, Q_FUNC_INFO);

    for(int count = 0; count < keyword_category_list.size(); count++)
    {
        command_str = QString("Select keyword from keyword_list Where category = ? AND keyword_status = ?");
        value_list.clear();
        value_list << keyword_category_list.at(count) << QString::number(1); ;

        keywords_list <<  get_stringlist_from_db_with_addbindvalues_by_dbreference(command_str, value_list, 0, *destination_db, Q_FUNC_INFO);
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " END ",Q_FUNC_INFO);
    return keywords_list;

}

void recon_helper_standard::update_narad_variable_with_apple_metadata_info(QString caller_func)
{
    recon_static_functions::app_debug("  - START",Q_FUNC_INFO);

    QString destination_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" +  destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return;
    }



    //-Extended Attributes..
    QSqlQuery query_extended_attr(*destination_db);
    QString command_extended_attr = QString("select metadata_attribute_name,custom_column_name from tbl_apple_attributes");
    query_extended_attr.prepare(command_extended_attr);
    if(!query_extended_attr.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_extended_attr,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_extended_attr.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_filter_attributes;
    QStringList list_filter_custom_col_name;
    while(query_extended_attr.next())
    {
        list_filter_attributes << query_extended_attr.value(0).toString();
        list_filter_custom_col_name << query_extended_attr.value(1).toString();
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_All_Key_QStringList,list_filter_attributes);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_All_Custom_Column_Name_QStringList,list_filter_custom_col_name);

    //========
    command_extended_attr = QString("select metadata_attribute_name,custom_column_name from tbl_apple_attributes where bookmark_metadata = 1");
    query_extended_attr.prepare(command_extended_attr);
    if(!query_extended_attr.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_extended_attr,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_extended_attr.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_metadata_attributes;
    QStringList list_metadata_custom_col_name;
    while(query_extended_attr.next())
    {
        list_metadata_attributes << query_extended_attr.value(0).toString();
        list_metadata_custom_col_name << query_extended_attr.value(1).toString();
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Key_QStringList,list_metadata_attributes);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Metadata_Custom_Column_Name_QStringList,list_metadata_custom_col_name);
    //========

    //========
    command_extended_attr = QString("select metadata_attribute_name,custom_column_name from tbl_apple_attributes where bookmark_report = 1");
    query_extended_attr.prepare(command_extended_attr);
    if(!query_extended_attr.exec())
    {
        recon_static_functions::app_debug(" query  --------FAILED------ db file - ",Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command_extended_attr,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query_extended_attr.lastError().text(),Q_FUNC_INFO);
    }

    QStringList list_report_attributes;
    QStringList list_report_custom_col_name;
    while(query_extended_attr.next())
    {
        list_report_attributes << query_extended_attr.value(0).toString();
        list_report_custom_col_name << query_extended_attr.value(1).toString();
    }
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Report_Key_QStringList,list_report_attributes);
    global_narad_muni_class_obj->set_field(MACRO_NARAD_Apple_Attributes_For_Report_Custom_Column_Name_QStringList,list_report_custom_col_name);
    //========




    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("  - end",Q_FUNC_INFO);

}

QString recon_helper_standard::get_virtual_source_path_of_current_record(QString record_no, QString table_name, QString plugin_db_path, QString caller_func)
{
    QString source_count_name_of_current_record = get_source_count_name_on_current_record(record_no, table_name, plugin_db_path, Q_FUNC_INFO + caller_func);

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name_of_current_record);

    QString virtual_source_path = struct_target_info.virtual_source_path;

    return virtual_source_path;
}

bool recon_helper_standard::is_it_dir_record_no_by_dbpath(QString record_no, QString destination_db_path, QString table_name, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    bool bool_dir = false;

    //    if(!QFileInfo(destination_db_path).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return bool_dir;
    //    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);

    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " db open --------FAILED----- " + destination_db->databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);

        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);
        return bool_dir;
    }

    bool_dir = is_it_dir_record_no_by_dbreference(record_no, *destination_db, table_name, source_count_name, Q_FUNC_INFO);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);

    return bool_dir;
}

bool recon_helper_standard::is_it_dir_record_no_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    // this function is lengthy bcoz of speed performance

    bool bool_dir = false;
    struct_GLOBAL_witness_info_source ttobj =  global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(ttobj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || ttobj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        bool bool_is_dir_col_exist = destination_db.record(table_name).contains("is_dir");
        if(bool_is_dir_col_exist)
        {
            QString cmd_itm = "select is_dir from " + table_name + " where INT = ?";
            QString dir_val =  get_string_from_db_with_addbindvalues_by_dbreference(cmd_itm, QStringList(record_no), 0, destination_db, Q_FUNC_INFO);
            if(dir_val.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
            {
                bool_dir = true;
            }
        }
        else if(destination_db.record(table_name).contains("filepath") && destination_db.record(table_name).contains("id_path"))
        {
            QString dmn_path =  get_string_from_db_with_addbindvalues_by_dbreference("select filepath from files where INT = ?" ,QStringList(record_no), 0, destination_db, Q_FUNC_INFO);
            QString idpath =  get_string_from_db_with_addbindvalues_by_dbreference("select id_path from files where filepath = ?" ,QStringList(dmn_path), 0, destination_db, Q_FUNC_INFO);
            if(idpath.isEmpty())
            {
                bool_dir = true;
            }
            else
            {
                // may come here for decompressed files
                QString file_path = get_file_infopath_by_dbreference((record_no),destination_db,table_name,source_count_name,Q_FUNC_INFO);
                QFileInfo info(file_path);
                bool_dir = info.isDir();
            }
        }
        else
        {
            // like file search for iOS, which dont contain idpath column
            QString file_path = get_file_infopath_by_dbreference((record_no),destination_db,table_name,source_count_name,Q_FUNC_INFO);
            QFileInfo info(file_path);
            bool_dir = info.isDir();
        }
    }
    else
    {
        QString command = "select recon_filefrom, recon_file_infopath, is_dir from '" + table_name +  "' where INT = '" + record_no + "'";
        QString file_from_str =  get_string_from_db_by_dbreference(command,0,destination_db,(recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
        int file_from = file_from_str.toInt();
        if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_TSK_Fuse)
        {
            bool bool_is_dir_col_exist = destination_db.record(table_name).contains("is_dir");
            if(bool_is_dir_col_exist)
            {
                QString cmd_itm = "select is_dir from " + table_name + " where INT = ?";
                QString dir_val =  get_string_from_db_with_addbindvalues_by_dbreference(cmd_itm, QStringList(record_no), 0, destination_db, Q_FUNC_INFO);
                if(dir_val.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                {
                    bool_dir = true;
                }
            }
        }
        else
        {
            QString file_path = get_file_infopath_by_dbreference((record_no),destination_db,table_name,source_count_name,Q_FUNC_INFO);
            QFileInfo info(file_path);
            if(info.exists())
            {
                //            QMimeDatabase mime_database_obj;
                //            QString mime_name = mime_database_obj.mimeTypeForFile(file_path, QMimeDatabase::MatchContent).name();
                //            if(mime_name.startsWith(QString("inode/directory")) || info.isDir())
                //            {
                //                bool_dir = true;
                //            }

                bool_dir = info.isDir();
            }
            else
            {
                bool bool_is_dir_col_exist = destination_db.record(table_name).contains("is_dir");
                if(bool_is_dir_col_exist)
                {
                    QString cmd_itm = "select is_dir from " + table_name + " where INT = ?";
                    QString dir_val =  get_string_from_db_with_addbindvalues_by_dbreference(cmd_itm, QStringList(record_no), 0, destination_db, Q_FUNC_INFO);
                    if(dir_val.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                    {
                        bool_dir = true;
                    }
                }
            }
        }

    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);

    return bool_dir;
}


QIcon recon_helper_standard::get_qicon_of_record_no_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func)
{

    struct_GLOBAL_witness_info_source mobj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QFileIconProvider icon_provide;
    QIcon icon_path = icon_provide.icon(QFileIconProvider::File);

    if(mobj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID
            || mobj.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
    {
        if(is_it_dir_record_no_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO))
        {
            icon_path = icon_provide.icon(QFileIconProvider::Folder);
        }
        else
        {
            QString full_filepath = get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);
            QFileInfo info(full_filepath);
            icon_path = icon_provide.icon(info);
        }

    }
    else
    {
        QString command = "select recon_filefrom, recon_file_infopath, is_dir from '" + table_name +  "' where INT = ?";
        QString file_from_str =  get_string_from_db_with_addbindvalues_by_dbreference(command, QStringList(record_no), 0, destination_db, (recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
        int file_from = file_from_str.toInt();
        if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_TSK_Fuse)
        {
            bool bool_is_dir_col_exist = destination_db.record(table_name).contains("is_dir");
            if(bool_is_dir_col_exist)
            {
                QString cmd_itm = "select is_dir from " + table_name + " where INT = ?";
                QString dir_val =  get_string_from_db_with_addbindvalues_by_dbreference(cmd_itm, QStringList(record_no), 0, destination_db, Q_FUNC_INFO);
                if(dir_val.trimmed() == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
                {
                    icon_path = icon_provide.icon(QFileIconProvider::Folder);
                }
            }
        }
        else
        {
            QString full_filepath = get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);
            QFileInfo info(full_filepath);
            if(info.exists())
            {
                icon_path = icon_provide.icon(info);
            }
            else if(is_it_dir_record_no_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO))
            {
                icon_path = icon_provide.icon(QFileIconProvider::Folder);
            }
        }
    }

    return icon_path;
}

struct_global_recon_file_info recon_helper_standard::pub_get_recon_file_info_by_dbreference(QString plugin_name ,QString record_no, QSqlDatabase &destination_db, QString table_name , QString source_count_name, QString caller_func)
{
    //    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    struct_global_recon_file_info obj;

    //    obj.db_path.clear();
    //    obj.recon_file_from.clear();
    //    obj.display_file_path.clear();
    //    obj.recon_file_info_path.clear();
    //    obj.id_path.clear();
    //    obj.complete_file_path.clear();
    //    obj.bool_file_from_result = false;
    //    obj.bool_file_is_dir = false;

    //    QString select_cmd;


    //    if(plugin_name == QString(MACRO_Plugin_Name_File_System))
    //    {
    //        select_cmd = ("Select  filepath, recon_filefrom ,recon_file_infopath ,id_path From '"+ table_name +"' Where INT = ?");
    //        obj.id_path  = get_string_from_db_with_addbindvalues_by_dbreference(select_cmd, QStringList(record_no), 3, destination_db, Q_FUNC_INFO);
    //        obj.bool_file_from_result = false;
    //        obj.bool_file_is_dir = is_it_dir_record_no_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);
    //    }
    //    else if(plugin_name == QString(MACRO_Plugin_Name_Carved_Files))
    //    {
    //        select_cmd = ("Select  item1, recon_filefrom ,recon_file_infopath  From '"+ table_name +"' Where INT = ?");
    //        obj.bool_file_from_result = true;
    //    }
    //    else
    //        return obj;

    //    obj.db_path = destination_db.databaseName();

    //    QString recon_file_from;
    //    QString recon_fileinfo_path;
    //    obj.display_file_path  = get_string_from_db_with_addbindvalues_by_dbreference(select_cmd, QStringList(record_no), 0, destination_db, Q_FUNC_INFO);


    //    struct_GLOBAL_derived_source_info struct_target_source_info = global_target_m_sources_info_obj->pub_get_target_info_structure_according_source_count_name(source_count_name);
    //    QString complete_file_path = get_file_infopath_by_dbreference(record_no,destination_db,table_name,source_count_name,Q_FUNC_INFO);

    //    if(struct_target_source_info.bool_source_decompressed)
    //    {
    //        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    //        recon_fileinfo_path = complete_file_path;
    //        if(recon_fileinfo_path.startsWith(result_dir_path))
    //            recon_fileinfo_path.remove(0,result_dir_path.size());


    //        if(!recon_fileinfo_path.trimmed().isEmpty() && !recon_fileinfo_path.startsWith("/"))
    //            recon_fileinfo_path.prepend("/");

    //        recon_fileinfo_path = recon_fileinfo_path;
    //        recon_file_from = QString::number(enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result);

    //    }
    //    else
    //    {
    //        recon_file_from     = get_string_from_db_with_addbindvalues_by_dbreference(select_cmd, QStringList(record_no), 1, destination_db, Q_FUNC_INFO);
    //        recon_fileinfo_path = get_string_from_db_with_addbindvalues_by_dbreference(select_cmd, QStringList(record_no), 2, destination_db, Q_FUNC_INFO);
    //    }


    //    obj.recon_file_from = recon_file_from;
    //    obj.recon_file_info_path = recon_fileinfo_path;
    //    obj.complete_file_path = complete_file_path;


    //    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_plist_viewer_tag_entry(QString file_path, QString tag_name, QString plist_viewer_name, QString record_no, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);


    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Plist_Viewer_QString).toString() + "plist_viewer_content_info.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(exported_content_info_db);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);


    query_select.prepare("SELECT file_name,file_path,file_size,key,value,recon_tag_value,color_hex,type,notes, keypath from plist_content_info WHERE file_path = ? AND viewer_display_name = ? AND record_no = ?");
    query_select.addBindValue(file_path);
    query_select.addBindValue(plist_viewer_name);
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;

    if(query_select.next())
    {
        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        QString complete_sourec_name = struct_target_info.complete_source_name;

        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_sourec_name +"<br>" ;
        if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ struct_target_info.source_name +"<br>";
        }

        metadata_str += "<br>";

        metadata_str += "<b>" + QObject::tr("Key") + ":</b>" +QString("\t") + query_select.value(3).toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("Type") + ":</b>" +QString("\t") + query_select.value(7).toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("Value") + ":</b>" +QString("\t") + query_select.value(4).toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("Key Path") + ":</b>" +QString("\t") + query_select.value(9).toString() +"<br><br>";

        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(0).toString() +"<br>" ;
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br>";

        metadata_str += "<b>" + QObject::tr("File Size") + ":</b>" +QString("\t") + recon_static_functions::human_readable_size(query_select.value(2).toLongLong(), Q_FUNC_INFO) +"\n<br> <br>";


        QString tag_text = "";
        QStringList list_tags = query_select.value(5).toString().split(",", Qt::SkipEmptyParts);


        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {

            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);


            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";



        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(8).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_log_viewer_tag_entry(QString file_path, QString tag_name, QString log_viewer_name, QString record_no, QString source_count_name,QString tagged_data, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Log_Viewer_QString).toString() + "log_viewer_content_info.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(exported_content_info_db);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare(" SELECT tagged_data, file_name, file_path, file_size,"
                         " recon_tag_value, notes from log_content_info"
                         " WHERE file_path = ? AND viewer_display_name = ? AND record_no = ? AND tagged_data = ?");

    query_select.addBindValue(file_path);
    query_select.addBindValue(log_viewer_name);
    query_select.addBindValue(record_no);
    query_select.addBindValue(tagged_data);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;

    if(query_select.next())
    {
        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        QString complete_source_name = struct_target_info.complete_source_name;

        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" + QString("\t") + complete_source_name +"<br>" ;
        if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ struct_target_info.source_name +"<br>";
        }

        metadata_str += "<br>";

        metadata_str += "<b>" + QObject::tr("Tagged Data") + ":</b>" + QString("\t") + query_select.value("tagged_data").toString() +"<br>";

        metadata_str += "<br>";

        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" + QString("\t") + query_select.value("file_name").toString() +"<br>" ;
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" + QString("\t") + query_select.value("file_path").toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("File Size") + ":</b>" + QString("\t") + recon_static_functions::human_readable_size(query_select.value("file_size").toLongLong(), Q_FUNC_INFO) +"\n<br> <br>";


        QString tag_text = "";
        QStringList list_tags = query_select.value("recon_tag_value").toString().split(",", Qt::SkipEmptyParts);

        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";
        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" + QString("\t") + query_select.value("notes").toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_screenshots_tag_entry(QString file_path, QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();

    QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(screenshot_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + screenshot_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("SELECT item0,item1,item2,recon_tag_value,notes from tags WHERE INT = ?");
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }
    QString metadata_str;
    QString coplete_file_path = get_file_infopath_by_dbreference(record_no,*destination_db,"tags","",Q_FUNC_INFO);

    if(query_select.next())
    {
        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + QString(MACRO_Plugin_Name_Screenshots) +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(0).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + coplete_file_path +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Date Created") + ":</b>" +QString("\t") +  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone( query_select.value(2).toString(), Q_FUNC_INFO) +"<br>";
        metadata_str += QString("\n<br>");

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(4).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
        obj.preview_file_path = coplete_file_path;
        obj.file_path = coplete_file_path;
    }



    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_saved_maps_entry(QString file_path, QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();

    QString timeline_saved_graph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(timeline_saved_graph_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + timeline_saved_graph_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("SELECT plugin_name,item0,item1,item2,item3,notes,item4 from tags WHERE INT = ?");
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }
    QString metadata_str;

    if(query_select.next())
    {
        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + query_select.value(0).toString().trimmed() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + query_select.value(2).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Latitude")  + ":</b>" +QString("\t") + query_select.value(3).toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("Longitude") + ":</b>" +QString("\t") + query_select.value(4).toString() +"<br><br>";
        metadata_str += QString("\n<br>");

        QString complete_path = get_file_infopath_by_dbreference(record_no,*destination_db,"tags","",Q_FUNC_INFO);
        obj.preview_file_path = complete_path;
        obj.file_path = complete_path;

        metadata_str += "<b>" + QObject::tr("Map Saved Path") + ":</b>" +QString("\t") + complete_path +"<br><br>";

        metadata_str += QString("\n<br>");

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(5).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
    }



    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_unified_logs_tag_entry(QString record_no, QString source_count_name,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    QString unified_logs_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString();
    QString unified_index_logs_db_path = unified_logs_dir_path + "/index_unified_logs.sqlite";
    QString select_command = QString("SELECT db_name from tbl_index WHERE source_count_name = ?");

    QStringList arg_list;
    arg_list << source_count_name;

    QString unified_logs_db = get_string_from_db_with_addbindvalues_by_dbpath(select_command,arg_list, 0, unified_index_logs_db_path, Q_FUNC_INFO);

    struct_global_set_metadata_info st_set_meta_obj;
    recon_static_functions::clear_variables_detailed_information(st_set_meta_obj);

    QString unified_logs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Unified_Logs_In_Result_QString).toString() + unified_logs_db;

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(unified_logs_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + unified_logs_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    QString command = QString("SELECT INT, category, timestamp_readable, process_id, process, event_type, event_message,"
                              " process_path, process_image_uuid, message_type, library, library_path, thread_id, activity_id, trace_id,"
                              " subsystem, boot_uuid, source_file, recon_tag_value from log_data WHERE INT = ?");

    query_select.prepare(command);

    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;

    if(query_select.next())
    {
        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        QString complete_source_name = struct_target_info.complete_source_name;

        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" + QString("\t") + complete_source_name +"<br>" ;

        QString tag_text = "";
        QStringList list_tags = query_select.value("recon_tag_value").toString().split(",", Qt::SkipEmptyParts);

        metadata_str += "<br>";
        metadata_str += "<b>" + QObject::tr("Timestamp") + ":</b>" + QString("\t") + query_select.value("timestamp_readable").toString() +"<br><br>";

        metadata_str += "<b>" + QObject::tr("Process ID") + ":</b>" + QString("\t") + query_select.value("process_id").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Process") + ":</b>" + QString("\t") + query_select.value("process").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Process Path") + ":</b>" + QString("\t") + query_select.value("process_path").toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("Process UUID") + ":</b>" + QString("\t") + query_select.value("process_image_uuid").toString() + "<br><br>";

        metadata_str += "<b>" + QObject::tr("Category") + ":</b>" + QString("\t") + query_select.value("category").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Event Type") + ":</b>" + QString("\t") + query_select.value("event_type").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Event Message") + ":</b>" + QString("\t") + query_select.value("event_message").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Message Type") + ":</b>" + QString("\t") + query_select.value("message_type").toString() + "<br><br>";

        metadata_str += "<b>" + QObject::tr("Library") + ":</b>" + QString("\t") + query_select.value("library").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Library Path") + ":</b>" + QString("\t") + query_select.value("library_path").toString() + "<br><br>";

        metadata_str += "<b>" + QObject::tr("Thread ID") + ":</b>" + QString("\t") + query_select.value("thread_id").toString() + "<br>";
        //            metadata_str += "<b>" + QObject::tr("Activity ID") + ":</b>" + QString("\t") + query_select.value("activity_id").toString() + "<br>";
        //            metadata_str += "<b>" + QObject::tr("Trace Id") + ":</b>" + QString("\t") + query_select.value("trace_id").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Subsystem") + ":</b>" + QString("\t") + query_select.value("subsystem").toString() + "<br>";
        metadata_str += "<b>" + QObject::tr("Boot UUID") + ":</b>" + QString("\t") + query_select.value("boot_uuid").toString() + "<br><br>";
        metadata_str += "<b>" + QObject::tr("Source File") + ":</b> <br>" + QString("\t") + QString("/private/var/db/uuidtext") + "<br>" + QString("/private/var/db/diagnostics") +"<br><br>";

        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";

        obj.metadata = metadata_str;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;

    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_saved_timeline_graph_entry(QString file_path, QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();

    QString timeline_saved_graph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(timeline_saved_graph_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + timeline_saved_graph_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("SELECT plugin_name,item0,item1,item2,item3,notes from tags WHERE INT = ?");
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }
    QString metadata_str;

    if(query_select.next())
    {
        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + query_select.value(0).toString().trimmed() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br><br>";

        QString complete_path = get_file_infopath_by_dbreference(record_no,*destination_db ,"tags","",Q_FUNC_INFO);
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + complete_path +"<br><br>";

        metadata_str += "<b>" + QObject::tr("Duration") + ":</b>" +QString("\t") + query_select.value(4).toString() +"<br><br>";

        metadata_str += QString("\n<br>");

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(5).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
        obj.preview_file_path = complete_path;
        obj.file_path = complete_path;

    }



    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_saved_timeline_graph_entry_storyboard(QString file_path, QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();

    QString timeline_saved_graph_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString() + "artifacts_timeline_saved_graphs.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(timeline_saved_graph_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + timeline_saved_graph_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("SELECT plugin_name,item0,item1,item2,item3,notes from tags WHERE item1 = ? AND INT = ?");
    QString relative_path = file_path;
    if(relative_path.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString()))
        relative_path.remove(0 , global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString().size());
    query_select.addBindValue(relative_path);
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }
    QString metadata_str;

    if(query_select.next())
    {
        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + query_select.value(0).toString().trimmed() +"<br><br>";
        //metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br><br>";

        //        QString t_relative_path_str = query_select.value(2).toString();
        //        if(!t_relative_path_str.startsWith(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString()))
        //            t_relative_path_str.prepend(global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Timeline_Saved_Graphs_In_Result_QString).toString());

        //        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + t_relative_path_str +"<br><br>";
        //        metadata_str += "<b>" + QObject::tr("Date Created") + ":</b>" +QString("\t") + convert_epoch_to_custom( query_select.value(3).toString(), Q_FUNC_INFO) +"<br>";
        //        metadata_str += QString("\n<br>");

        metadata_str += "<b>" + QObject::tr("Duration") + ":</b>" +QString("\t") + query_select.value(4).toString() +"<br><br>";

        metadata_str += QString("\n<br>");
        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(5).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
    }



    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}


struct_global_fetch_metadata recon_helper_standard::fetch_metadata_of_plugin(QString plugin_name_str,QString tab_name_str,QString record_no_str,QString result_dir_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    QString metadata_str;

    QString tmp_plg_name = plugin_name_str;
    tmp_plg_name.replace(" ","_");

    QString destination_plugin_dir_path = result_dir_path + "/" + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS + tmp_plg_name;
    QString plugins_destination_db_file = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite" ;


    struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);


    int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);

    if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
        return obj;

    QString table_name = st_csv_info_obj.table_name_QStringList_tablewidget.at(tab_index);
    QStringList display_name_list = st_csv_info_obj.metadata_header_QList_tablewidget.at(tab_index);
    QStringList cols_nam_list =  st_csv_info_obj.metadata_column_name_QList_tablewidget.at(tab_index);
    QStringList cols_type_list = st_csv_info_obj.metadata_data_type_QList_tablewidget.at(tab_index);
    int items_count = cols_nam_list.size();


    QStringList preview_col_list = st_csv_info_obj.metadata_preview_QList_tablewidget.at(tab_index);

    int preview_index = -1;
    if(preview_col_list.size() > 0)
    {
        preview_index = cols_nam_list.indexOf(preview_col_list.at(0),0);

    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(plugins_destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + plugins_destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString source_count_name = get_source_count_name_on_current_record(record_no_str, table_name, plugins_destination_db_file, Q_FUNC_INFO);


    QSqlQuery query_select(*destination_db);
    QString command = cols_nam_list.join(",");

    query_select.prepare("select " + command + " from '" + table_name + "' where INT = ?");
    query_select.addBindValue(record_no_str);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select  ------FAILED-----",Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery() ,Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    metadata_str += "<b>" + QString("Plugin") + ":</b>" +QString("\t")+ plugin_name_str +"<br>" ;
    metadata_str += "<b>" + QString("Category") + ":</b>" +QString("\t")+ tab_name_str +"\n<br> <br>";

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString complete_source_name = struct_target_info.complete_source_name;
    metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +"\t"+ complete_source_name +"<br>";
    QString source_name_str = struct_target_info.source_name;

    QString os_nm_scheme_dsply = get_target_os_scheme_display_on_current_record_source(record_no_str , table_name,  plugins_destination_db_file,  Q_FUNC_INFO);
    QString os_nm_scheme_intrnl = get_target_os_scheme_internal_on_current_record_source(record_no_str , table_name,  plugins_destination_db_file,  Q_FUNC_INFO);
    if(os_nm_scheme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
    {
        metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ source_name_str +"<br>";
    }

    metadata_str += "<br>";




    QString current_media;


    bool bool_is_it_virtual_export = false;
    while(query_select.next())
    {
        for(int i=0 ; i < items_count ; i++)
        {
            QString data;

            QString md_data_type = cols_type_list.at(i);
            QString md_data_type_for_tmpuse = md_data_type;

            QString our_column = display_name_list.at(i);
            QString our_value = query_select.value(i).toString();

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

            if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_OS_SCHEME_DISPLAY_NAME_QString)
                continue;

            if(i == preview_index)
            {
                if(query_select.value(i).toString().contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple)
                        || query_select.value(i).toString().contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                {
                    QStringList m_path_list;
                    if(query_select.value(i).toString().contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                    {
                        m_path_list = query_select.value(i).toString().split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, Qt::SkipEmptyParts);
                    }
                    else
                    {
                        m_path_list = query_select.value(i).toString().split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);
                    }


                    if(m_path_list.size() >= 1)
                    {
                        current_media = m_path_list.at(0);

                        if((current_media.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images))) || (current_media.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments)))
                                || (current_media.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Files))))
                        {
                            bool_is_it_virtual_export = true;
                            current_media =  destination_plugin_dir_path + current_media.remove(0,1);
                        }
                        else
                        {
                            if(os_nm_scheme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
                                    || os_nm_scheme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML))
                            {
                                int indx = current_media.lastIndexOf(".");

                                if(indx >= 1)
                                {
                                    current_media.remove(indx, current_media.size());
                                }

                                if(!current_media.trimmed().isEmpty() && !current_media.trimmed().startsWith("/"))
                                    current_media.prepend("/");

                                QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");
                                current_media =  get_string_from_db_with_addbindvalues_by_dbpath("select filepath from files where id_path = ?" , QStringList(current_media), 0, fs_db_path, Q_FUNC_INFO);

                            }


                        }

                    }

                }

            }

            if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
                data = "<b>" + our_column + ":</b>\t" + our_value + "\n";
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_PREPEND_COUNT_QString)
                data = "<b>" + our_column + ":</b>\t" + our_value + "\n";


            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
            {
                if((i-1) > 0)
                {
                    QString data_tp = cols_type_list.at(i-1);
                    data_tp.remove("NewLine");

                    if(data_tp == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
                        data = QString("<b>") + our_column + ":</b>\t" +   global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO) + "\n";
                    else
                        data = QString("<br><b>") + our_column + ":</b>\t" +   global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO) + "\n";

                }
                else
                {
                    data = QString("<br><b>") + our_column + ":</b>\t" +   global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(our_value, Q_FUNC_INFO) + "\n";
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

                if(our_value.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    our_value = our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple, "<br>/");
                }
                else
                {
                    our_value = our_value.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, "<br>/");

                }


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
                            data.append("<br><b>" + QObject::tr("Date ") + ": </b>" +   global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(temp.at(i), Q_FUNC_INFO) +"\n");

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
                timestamps_splitList = str.split(";", Qt::SkipEmptyParts);

                for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                {
                    if(tm_loop == 0)
                    {
                        data = QString("<br><b>") + our_column + ":</b>\t" +  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n";

                    }
                    else
                    {
                        data.append(QString("<br>\n") +   global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");
                    }

                }
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_METADATA_PREVIEW_QString)
            {
                if(global_csv_reader_class_selected_obj->is_tab_support_multiple_export(tab_name_str,plugin_name_str))
                {
                    QString preview_filepath = our_value;

                    if(os_nm_scheme_intrnl == QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID))
                    {
                        preview_filepath = preview_filepath.remove(preview_filepath.lastIndexOf("."), preview_filepath.size());
                    }

                    if(!preview_filepath.startsWith("./" + QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Images))
                            && !preview_filepath.trimmed().isEmpty())
                        data = QString("<br><b>") + our_column + ":</b>\t" + preview_filepath + "\n";
                }
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
            {
                QString tag_text = "";
                QStringList list_tags = our_value.split(",", Qt::SkipEmptyParts);
                for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                {
                    QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
                    tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                }

                if(tag_text.endsWith(","))
                    tag_text.chop(1);

                data = QString("<b>") + our_column + ":</b>\t" + tag_text + "\n";
            }
            else if(md_data_type == MACRO_CSV_TEMPLATE_DATATYPE_FILE_IN_RESULT_DIR_QString)
            {
                QString value = our_value;
                if(value == QString(""))
                    data = "<b>" + our_column + ":</b>\t" + "" + "\n";
                else
                {
                    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
                    QString str_val = our_value;
                    str_val = recon_static_functions::get_complete_file_path(result_dir_path , str_val, Q_FUNC_INFO);
                    data = "<b>" + our_column + ":</b>\t" + str_val + "\n";
                }

            }
            else
            {
                if(our_column == QString("Artifacts Source File"))
                {
                    if(!our_value.trimmed().isEmpty())
                        data = "<b>" + our_column + ":</b>\t" + our_value.toHtmlEscaped();
                }
                else
                    data = "<b>" + our_column + ":</b>\t" + our_value.toHtmlEscaped();
            }


            if(md_data_type_for_tmpuse.startsWith("NewLine"))
            {
                metadata_str += QString("<br>");
            }

            metadata_str += data + "<br>";
        }
    }


    if(st_csv_info_obj.tab_export_available_QStringList_tablewidget.at(tab_index) == QString("1"))
    {
        QString extended_attr_data =  get_apple_metadata_key_value_from_result(record_no_str,table_name,plugins_destination_db_file, Q_FUNC_INFO);

        if(!extended_attr_data.trimmed().isEmpty())
        {
            metadata_str += "<br>";
            metadata_str += "<b><u><i>More Apple Metadata</u></i></b>";
            metadata_str += extended_attr_data;
        }
    }

    obj.metadata = metadata_str;
    obj.bool_virtual_export = bool_is_it_virtual_export;
    obj.file_path = current_media;

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Ends " , Q_FUNC_INFO);

    return obj;
}
struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_hex_viewer(QString record_no, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);


    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(exported_content_info_db);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("SELECT file_name,file_path,start_index,end_index,recon_tag_value,"
                         "content_file_path,notes,hex_data from hex_content_info WHERE INT = ?");
    query_select.addBindValue(record_no);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;

    if(query_select.next())
    {
        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString complete_source_name = struct_target_info.complete_source_name;
        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name +"<br><br>" ;

        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + query_select.value("file_path").toString() +"<br><br>";

        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value("file_name").toString() +"<br>" ;

        metadata_str += "<b>" + QObject::tr("Start index") + ":</b>" +QString("\t") + query_select.value("start_index").toString() +"<br>";
        metadata_str += "<b>" + QObject::tr("End index") + ":</b>" +QString("\t") + query_select.value("end_index").toString() +"<br>";
        metadata_str += "<br><b>" + QObject::tr("Tagged Content") + ":</b>" +QString("\t") + query_select.value("hex_data").toString() +"<br>";

        QString tag_text = "<font color = \"" + query_select.value(6).toString() +"\">" + query_select.value("recon_tag_value").toString() + "</font>";

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value("notes").toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
        QString content_f_pth = query_select.value("content_file_path").toString().trimmed();
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        if(!content_f_pth.isEmpty() && !content_f_pth.startsWith(result_dir_path))
            content_f_pth.prepend(result_dir_path);

        obj.file_path = content_f_pth;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_CSV_and_INT_based(QString table_name, int tab_index, QString record_no, QString feature_plugin, QString db_path,QString source_count_name , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Start " , Q_FUNC_INFO);
    struct_global_fetch_metadata metadata_obj;

    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    if(!struct_target_info.source_count_name.trimmed().isEmpty())
    {
        QString complete_source_name = struct_target_info.complete_source_name;
        metadata_obj.metadata += "<b>" + QObject::tr(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name +"<br>";
        if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_obj.metadata += "<b>" + QObject::tr(MACRO_Generic_Display_Name) + ":</b>" + QString("\t") + struct_target_info.source_name +"<br>";
        }
    }



    struct_global_csv_reader_full_csv_info md_csv_redaer_obj = global_csv_reader_class_selected_obj->read_csv_value(feature_plugin);
    if(md_csv_redaer_obj.metadata_header_QList_tablewidget.size() < tab_index && md_csv_redaer_obj.metadata_column_name_QList_tablewidget.size() < tab_index && md_csv_redaer_obj.metadata_data_type_QList_tablewidget.size() < tab_index)
        return metadata_obj;



    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return metadata_obj;
    }


    QStringList display_column_list = md_csv_redaer_obj.metadata_header_QList_tablewidget.at(tab_index);
    QStringList coulumn_data_type_list = md_csv_redaer_obj.metadata_data_type_QList_tablewidget.at(tab_index);


    QSqlQuery query_select(*destination_db);
    struct_global_csv_reader_full_csv_info md_csv_redaer_obj_1 = global_csv_reader_class_selected_obj->read_csv_value(feature_plugin);
    QString cmd_1;
    if(md_csv_redaer_obj_1.metadata_column_name_QList_tablewidget.size() > 0)
    {
        QStringList db_cols_nam_list =  md_csv_redaer_obj_1.metadata_column_name_QList_tablewidget.at(0);
        cmd_1 = db_cols_nam_list.join(",");
        cmd_1 = "select " + cmd_1 + " from '" + table_name + "' where INT = '" + record_no + "'";
    }

    query_select.prepare(cmd_1);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " ---FAILED--- select query",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -" + query_select.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery() ,Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return metadata_obj;
    }



    QString temp_metadata_str;
    if(query_select.next())
    {
        for(int y = 0 ; y < display_column_list.size() ; y++)
        {

            int index_value = y;
            QString c_value;
            QString d_value;

            QString rpt_data_type = coulumn_data_type_list.at(index_value);
            QString rpt_data_type_for_tmpuse = rpt_data_type;

            QString display_value = display_column_list.at(index_value);
            QString column_value = query_select.value(index_value).toString();
            if(column_value.trimmed().isEmpty())
                continue ;

            d_value = "<b>" + display_value + " : </b>";


            ///------------ Use Format as per CSV DataType
            if(rpt_data_type_for_tmpuse.endsWith("Hide"))
            {
                rpt_data_type.remove("Hide");
                if(column_value.trimmed() == "")
                    continue;
            }
            else if(rpt_data_type_for_tmpuse.startsWith("NewLine"))
            {
                rpt_data_type.remove("NewLine");
                temp_metadata_str.append("<br>");
            }
            else if(rpt_data_type_for_tmpuse.contains("_Person"))
            {
                rpt_data_type.remove("_Person");
            }
            else if(rpt_data_type_for_tmpuse.contains("_Icon"))
            {
                rpt_data_type.remove("_Icon");
            }


            ///------------ Use Format as per CSV DataType


            if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
            {
                QString record_number = column_value;
                c_value = record_number;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
            {
                c_value =  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(column_value, Q_FUNC_INFO);

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                QString value =  column_value;
                if(value == QString(""))
                    c_value = "";
                else if(value == QString("0"))
                    c_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                else
                    c_value = recon_static_functions::human_readable_size(column_value, Q_FUNC_INFO) + " (" + column_value + " bytes)";

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
            {

                QString str = column_value;
                if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");
                }
                else if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");
                }
                str = str.replace("|","<br>");

                c_value = str;

                c_value.prepend("<br>");
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
            {
                QString str = column_value;

                QStringList timestamps_splitList;

                timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                c_value.clear();
                for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                {
                    c_value.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");

                }

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
            {
                QString m_val = column_value;
                if(!m_val.startsWith("/"))
                    m_val = m_val.prepend("/");

                m_val.replace(";/","<br>/");

                m_val.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>/");

                c_value = m_val;

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {

                QString notes_value = query_select.value(index_value).toString();
                if(notes_value.trimmed().isEmpty())
                {
                    d_value.clear();
                    continue ;
                }
                else
                    c_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";


            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_QString)
            {
                c_value = "<font color = \"red\">" + column_value.toHtmlEscaped() + "</font>";
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
            {
                temp_metadata_str.append("<br>");

                QString tag_text = "";
                QStringList list_tags = column_value.split(",", Qt::SkipEmptyParts);
                for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                {
                    QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                    tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                }

                if(tag_text.endsWith(","))
                    tag_text.chop(1);

                c_value =  tag_text ;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECON_VIEWERS_TAG_EXPORTED_DATA_QString)
            {
                c_value = get_hex_viewer_format_for_metadata(column_value);
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_APPLE_METADATA_QString)
            {
                column_value.replace("\n", "<br>");
                c_value = "<br>" + column_value;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_Exif_Data_QString)
            {
                column_value.replace("\n" , "<br>");
                c_value.append("<br>" + column_value + "<br>");
            }
            else
                c_value = column_value.toHtmlEscaped();



            ////start -----append  column name and value
            temp_metadata_str.append(d_value + c_value + "<br>");
            ////start -----append  column name and value
        }
    }



    metadata_obj.metadata += temp_metadata_str;
    metadata_obj.file_path = "";
    metadata_obj.preview_file_path = "";

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return metadata_obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_sqlite_viewer_tag_entry(QString file_path, QString tag_name, QString record_no,QString db_table_name, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Starts " , Q_FUNC_INFO);


    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_SQLite_Viewer_QString).toString() + "sqlite_viewer_content_info.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(exported_content_info_db);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "  DB open ------FAILED-----" + exported_content_info_db,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    if(tag_name == QString(MACRO_Tag_Name_Bookmarks))
    {
        query_select.prepare("SELECT file_name,file_path,file_size,recon_tag_value,notes,db_table_name, sqlite_query_data, sql_tab_name from sv_content_info WHERE file_path = ? AND bookmark = ? AND record_no_in_sqliteviewer =  ? AND db_table_name =  ?");
        query_select.addBindValue(file_path);
        query_select.addBindValue("1");
        query_select.addBindValue(record_no);
        query_select.addBindValue(db_table_name);

    }
    else if(tag_name.endsWith(MACRO_CASE_TREE_Notes_Category_All_Notes))
    {
        query_select.prepare("SELECT file_name,file_path,file_size,recon_tag_value,notes,db_table_name, sqlite_query_data, sql_tab_name from sv_content_info WHERE file_path = ? AND notes = ? AND record_no_in_sqliteviewer =  ? AND db_table_name =  ?");
        query_select.addBindValue(file_path);
        QString notes_text = tag_name;
        QStringList tm_list = notes_text.split(MACRO_RECON_Splitter_1_multiple);
        if(tm_list.size() > 0)
            notes_text = tm_list.at(0);


        query_select.addBindValue(notes_text);
        query_select.addBindValue(record_no);
        query_select.addBindValue(db_table_name);
    }
    else
    {
        query_select.prepare("SELECT file_name,file_path,file_size,recon_tag_value,notes,db_table_name, sqlite_query_data, sql_tab_name from sv_content_info WHERE file_path = ? AND recon_tag_value = ? AND record_no_in_sqliteviewer =  ? AND db_table_name =  ?");
        query_select.addBindValue(file_path);
        query_select.addBindValue(tag_name);
        query_select.addBindValue(record_no);
        query_select.addBindValue(db_table_name);


    }


    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;

    if(query_select.next())
    {
        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString complete_source_name = struct_target_info.complete_source_name;

        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name +"<br>" ;
        if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ struct_target_info.source_name +"<br>";
        }

        metadata_str += "<br>";

        metadata_str += "<b>" + QObject::tr("Tab Name") + ":</b>" +QString("\t") + query_select.value(7).toString() +"\n<br> <br>";

        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(0).toString() +"<br>" ;
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br><br>";

        metadata_str += "<b>" + QObject::tr("File Size") + ":</b>" +QString("\t") + recon_static_functions::human_readable_size(query_select.value(2).toLongLong(), Q_FUNC_INFO) +"\n<br> <br>";

        metadata_str += "<b>" + QObject::tr("Db Table Name") + ":</b>" +QString("\t") + query_select.value(5).toString() +"\n<br> <br>";

        QStringList executed_query_list = query_select.value(6).toString().split(";", Qt::SkipEmptyParts);

        for(int i = 0 ; i < executed_query_list.size() ; i++)
        {
            QStringList list = executed_query_list.at(i).split(": ", Qt::SkipEmptyParts);
            if(list.size() < 2)
                continue;

            metadata_str += "<b>" +list.at(0) + ":</b>" +QString("\t") + list.at(1) +"\n<br><br>";


        }

        QString tag_text = "";
        QStringList list_tags = query_select.value(3).toString().split(",", Qt::SkipEmptyParts);


        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {

            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);

            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";


        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";


        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(4).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
        obj.file_path = query_select.value(1).toString();
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Ends " , Q_FUNC_INFO);

    return obj;
}


struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_content_search(QString record_no, QString db_path, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start" , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    record_no = record_no.trimmed();

    if(record_no.isEmpty())
        return obj;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);
    query_select.prepare("SELECT file_name, file_path, keyword_name, no_of_hits, hit_locations, recon_tag_value, notes, source_count_name FROM files WHERE INT = ?");
    query_select.addBindValue(record_no);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;

    if(query_select.next())
    {
        struct_GLOBAL_witness_info_source struct_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(query_select.value(7).toString());
        QString complete_source_name = struct_info.complete_source_name;
        metadata_str += "<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name + "<br>";
        if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ struct_info.source_name +"<br>";
        }

        metadata_str += "<br>";
        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(0).toString() +"<br>" ;
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br><br>";

        metadata_str += "<b>" + QObject::tr("Keyword Name") + ":</b>" +QString("\t") + query_select.value(2).toString() + "\n<br>";
        metadata_str += "<b>" + QObject::tr("No of hits") + ":</b>" +QString("\t") + query_select.value(3).toString() + "<br>";

        metadata_str += "<br>";

        QStringList hit_locations_list = query_select.value(4).toString().split(",", Qt::SkipEmptyParts);
        for(int  i = 0; i < hit_locations_list.size(); i++)
        {
            metadata_str += "<b>" + QObject::tr("Hit location ") + ":</b>" +QString("\t") + hit_locations_list.at(i) +"<br>";
        }


        metadata_str += "<br>";

        QString tag_text = "";
        QStringList list_tags = query_select.value(5).toString().split(",", Qt::SkipEmptyParts);
        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            if(list_tags.at(tag_i).isEmpty())
                continue;

            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";
        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(6).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
        obj.file_path = query_select.value(1).toString();
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end" , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_features_plugins(QString table_name, int feature_tab_index, QString record_no, QString feature_plugin, QString db_path, QString command, struct_GLOBAL_witness_info_source struct_info , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Start " , Q_FUNC_INFO);
    struct_global_fetch_metadata metadata_obj;

    /// --------source info
    if(!struct_info.source_count_name.trimmed().isEmpty())
    {
        QString complete_source_name = struct_info.complete_source_name;
        metadata_obj.metadata += "<b>" + QObject::tr(MACRO_Generic_Complete_Source_Name) + ":</b>" +QString("\t") + complete_source_name +"<br>";
        if(struct_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_obj.metadata += "<b>" + QObject::tr(MACRO_Generic_Display_Name) + ":</b>" + QString("\t") + struct_info.source_name +"<br>";
        }
    }
    /// --------source info



    struct_global_csv_reader_full_csv_info md_csv_redaer_obj = global_csv_reader_class_selected_obj->read_csv_value(feature_plugin);
    if(md_csv_redaer_obj.metadata_header_QList_tablewidget.size() < feature_tab_index && md_csv_redaer_obj.metadata_column_name_QList_tablewidget.size() < feature_tab_index && md_csv_redaer_obj.metadata_data_type_QList_tablewidget.size() < feature_tab_index)
        return metadata_obj;



    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return metadata_obj;
    }


    QStringList display_column_list = md_csv_redaer_obj.metadata_header_QList_tablewidget.at(feature_tab_index);
    QStringList db_cols_nam_list =  md_csv_redaer_obj.metadata_column_name_QList_tablewidget.at(feature_tab_index);
    QStringList coulumn_data_type_list = md_csv_redaer_obj.metadata_data_type_QList_tablewidget.at(feature_tab_index);


    ///note: if part - is only for INT based
    QSqlQuery query_select(*destination_db);
    if(command.trimmed().isEmpty())
    {
        command = db_cols_nam_list.join(",");
        command = "select " + command + " from " + table_name + " where INT = ?";
        query_select.prepare(command);
        query_select.addBindValue(record_no);

    }
    else
    {
        ///note: else part - is as per need (i.e with multile addbindvalues and pass as an argument from function calling)
        query_select.prepare(command);
    }

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " ---FAILED--- select query",Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -" + query_select.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select.executedQuery() ,Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return metadata_obj;
    }



    QString temp_metadata_str;
    if(query_select.next())
    {
        for(int y = 0 ; y < display_column_list.size() ; y++)
        {

            int index_value = y;
            QString c_value;
            QString d_value;

            QString rpt_data_type = coulumn_data_type_list.at(index_value);
            QString rpt_data_type_for_tmpuse = rpt_data_type;

            QString display_value = display_column_list.at(index_value);
            QString column_value = query_select.value(index_value).toString();
            if(column_value.trimmed().isEmpty())
                continue ;

            d_value = "<b>" + display_value + " : </b>";


            ///------------ Use Format as per CSV DataType
            if(rpt_data_type_for_tmpuse.endsWith("Hide"))
            {
                rpt_data_type.remove("Hide");
                if(column_value.trimmed() == "")
                    continue;
            }
            else if(rpt_data_type_for_tmpuse.startsWith("NewLine"))
            {
                rpt_data_type.remove("NewLine");
                temp_metadata_str.append("<br>");
            }
            else if(rpt_data_type_for_tmpuse.contains("_Person"))
            {
                rpt_data_type.remove("_Person");
            }
            else if(rpt_data_type_for_tmpuse.contains("_Icon"))
            {
                rpt_data_type.remove("_Icon");
            }


            ///------------ Use Format as per CSV DataType


            ///Html Report*/
            if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_RECORD_QString)
            {
                QString record_number = column_value;
                c_value = record_number;
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIME_QString)
            {
                c_value =  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(column_value, Q_FUNC_INFO);

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SIZE_QString)
            {
                QString value =  column_value;
                if(value == QString(""))
                    c_value = "";
                else if(value == QString("0"))
                    c_value =  recon_static_functions::human_readable_size(value, Q_FUNC_INFO);
                else
                    c_value = recon_static_functions::human_readable_size(column_value, Q_FUNC_INFO) + " (" + column_value + " bytes)";

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_COLUMN_SEPERATOR_QString)
            {

                QString str = column_value;
                if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");
                }
                else if(str.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon))
                {
                    str = str.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");
                }
                str = str.replace("|","<br>");

                c_value = str;

                c_value.prepend("<br>");
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TIMESTAMP_SEPERATOR_QString)
            {
                QString str = column_value;

                QStringList timestamps_splitList;
                timestamps_splitList = str.split(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon, Qt::SkipEmptyParts);

                c_value.clear();
                for(int tm_loop = 0; tm_loop < timestamps_splitList.size(); tm_loop++)
                {
                    c_value.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(timestamps_splitList.at(tm_loop), Q_FUNC_INFO) + "\n");

                }

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_ARTIFACT_SOURCE_QString)
            {
                QString m_val = column_value;
                if(!m_val.startsWith("/"))
                    m_val = m_val.prepend("/");


                if(m_val.contains(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple))
                {
                    m_val = m_val.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_1_Multiple,"<br>");
                }

                m_val.replace(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Splitter_2_Semicolon,"<br>");

                c_value = m_val;

            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_NOTES_QString)
            {

                QString notes_value = query_select.value(index_value).toString();
                if(notes_value.trimmed().isEmpty())
                {
                    d_value.clear();
                    continue ;
                }
                else
                    c_value  = "<font color = \"blue\">" + notes_value.replace("\n", "<br>") + "</font>";


            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_SPECIAL_COLOR_QString)
            {
                c_value = "<font color = \"red\">" + column_value.toHtmlEscaped() + "</font>";
            }
            else if(rpt_data_type == MACRO_CSV_TEMPLATE_DATATYPE_TAG_QString)
            {
                temp_metadata_str.append("<br>");

                QString tag_text = "";
                QStringList list_tags = column_value.split(",", Qt::SkipEmptyParts);
                for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
                {
                    QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO);
                    tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite", Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
                }

                if(tag_text.endsWith(","))
                    tag_text.chop(1);

                c_value =  tag_text ;
            }
            else
                c_value = column_value.toHtmlEscaped();



            ////start -----append  column name and value
            temp_metadata_str.append(d_value + c_value + "<br>");
            ////start -----append  column name and value
        }
    }



    metadata_obj.metadata += temp_metadata_str;
    metadata_obj.file_path = "";
    metadata_obj.preview_file_path = "";


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);


    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    return metadata_obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_carved_files(QString db_path, QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("SELECT item0,item1,item3,item4,item5,recon_tag_value,notes,source_count_name from files WHERE record_no = ?");

    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }
    QString metadata_str;

    QString conf_db_path = QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite");

    if(query_select.next())
    {
        QString source_count_name = query_select.value(7).toString();

        struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        QString complete_source_name = struct_target_info.complete_source_name;
        metadata_str += QString("\n<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>\t") + complete_source_name + "\n<br><br>";

        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" +QString("\t") + QString(MACRO_Plugin_Name_Carved_Files) +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + record_no +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t") + query_select.value(0).toString() +"<br>";
        QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
        QString file_path = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,query_select.value(1).toString(), Q_FUNC_INFO);
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t") +file_path +"<br><br>";
        metadata_str += "<b>" + QObject::tr("File Size") + ":</b>" +QString("\t") + recon_static_functions::human_readable_size(query_select.value(2).toLongLong(), Q_FUNC_INFO) + " (" + query_select.value(2).toString() + " bytes)"+ "<br>";
        //        metadata_str += QString("<br>");
        metadata_str += "<b>" + QObject::tr("File Type") + ":</b>" +QString("\t") + query_select.value(3).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Offset") + ":</b>" +QString("\t") + query_select.value(4).toString() +"<br><br>";

        QString tag_text = query_select.value(5).toString();
        QString colour =  get_colour_name_of_tag_from_db(tag_text , conf_db_path , Q_FUNC_INFO);
        tag_text = "<font color = \"" +  get_color_hex_from_color_name(colour , conf_db_path, Q_FUNC_INFO) +"\">" + tag_text + "</font>";

        metadata_str += QString("<br><b>Tag:</b>\t") + tag_text + "<br> <br>";

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(6).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
    }


    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return obj;
}

struct_global_fetch_metadata recon_helper_standard::fetch_file_system_metadata_for_current_record(QString record , QString source_count_name, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start" , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    record = record.trimmed();
    source_count_name = source_count_name.trimmed();
    if(record.isEmpty() || source_count_name.isEmpty())
        return obj;


    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + struct_target_info.source_count_name + "/file_system.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(fs_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("select bookmark, INT,filename,filepath,extension,z_date_time_modified,z_date_time_change,z_date_time_accessed,z_date_time_created,"
                         "file_size,notes,recon_tag_value,exif_make,exif_model,id_path,inode_no,"
                         "kMDItemUseCount_Used_Count,kMDItemUsedDates_Used_Dates,kMDItemLastUsedDate_Last_Opened_Date,kMDItemContentCreationDate_Created_Date,kMDItemContentModificationDate_Last_Modified_Date,kMDItemDateAdded_Date_Added,"
                         "mime_type,bookmarked_extended_attributes,hash_md5,hash_sha1"
                         " From files where INT =  ?" );

    query_select.addBindValue(record);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);

        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;
    if(query_select.next())
    {
        QString complete_source_name = struct_target_info.complete_source_name;
        metadata_str += QString("\n<b>" + QString(MACRO_Generic_Complete_Source_Name) + ":</b>\t") + complete_source_name + "\n<br>";
        if(struct_target_info.os_scheme_display == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QString(MACRO_Generic_Display_Name) + ":</b>" +"\t"+ struct_target_info.source_name +"<br>";
        }

        metadata_str += "<br>";
        metadata_str += "<b>" + QObject::tr("Record No.") + ":</b>" +QString("\t")+ query_select.value("INT").toString() +"<br>" ;
        metadata_str += "<br>";

        metadata_str += "<b>" + QObject::tr("File Name") + ":</b>" +QString("\t")+ query_select.value("filename").toString() +"<br>" ;
        metadata_str += "<b>" + QObject::tr("File Path") + ":</b>" +QString("\t")+ query_select.value("filepath").toString() +"<br><br>";

        if(struct_target_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_UID)
        {
            QString idpath = query_select.value("id_path").toString();

            QFileInfo file_info(idpath);

            metadata_str += "<b>" + QObject::tr("UID File Name") + ":</b>" +QString("\t")+ file_info.fileName() +"<br>" ;
            metadata_str += "<b>" + QObject::tr("UID File Path") + ":</b>" +QString("\t")+ idpath +"<br><br>";
        }
        else if(struct_target_info.os_scheme_internal == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Internal_XML)
        {
            QString idpath = query_select.value("id_path").toString();
            metadata_str += "<b>" + QObject::tr("Preview File Path") + ":</b>" + QString("\t")+ idpath +"<br><br>";
        }

        if(struct_target_info.os_scheme_display != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
        {
            metadata_str += "<b>" + QObject::tr("Inode No./File ID") + ":</b>" +QString("\t")+ query_select.value("inode_no").toString() +"\n<br>";
        }


        if(query_select.value("file_size").toString().trimmed().isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("File Size") + ":</b>" +QString("\t") + "\n<br>";
        }
        else
            metadata_str += "<b>" + QObject::tr("File Size") + ":</b>" +QString("\t")+ recon_static_functions::human_readable_size(query_select.value("file_size").toString(), Q_FUNC_INFO) + " (" + query_select.value("file_size").toString() + " bytes)" + "\n<br>";

        metadata_str += "<b>" + QObject::tr("Mime Type") + ":</b>" +QString("\t")+ query_select.value("mime_type").toString() +"\n<br>";
        metadata_str += "<br>";

        QString ocr_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
                + source_count_name + QString("/optical_character_recognition.sqlite");

        QString cmd_ocr = "SELECT ocr_text from files WHERE filesystem_record = ?";
        QStringList bindValue;
        bindValue << query_select.value("INT").toString().trimmed();

        QString ocr_text = get_string_from_db_with_addbindvalues_by_dbpath(cmd_ocr, bindValue, 0, ocr_db_path, Q_FUNC_INFO);
        if(!ocr_text.isEmpty())
        {
            metadata_str += "<b>" + QObject::tr("OCR Text") + ":</b>" +QString("\t") + ocr_text + "\n<br>";
            metadata_str += "<br>";
        }

        QString md5_hash_str  = query_select.value("hash_md5").toString().trimmed();
        QString sha1_hash_str = query_select.value("hash_sha1").toString().trimmed();

        if(!md5_hash_str.isEmpty() || !sha1_hash_str.isEmpty())
        {
            QString case_conf_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";
            QStringList hashset_db_path_list =  get_stringlist_from_db_by_dbpath(QString("Select database_path from tbl_hashset"), 0, case_conf_db, Q_FUNC_INFO);

            hashset_db_data_list.clear();
            for(int count = 0; count < hashset_db_path_list.size(); count++)
            {
                struct_global_hashset_db_data hashset_obj;
                hashset_obj.hashset_db_path = hashset_db_path_list.at(count);
                hashset_obj.hashet_table_name  =  get_string_from_db_with_addbindvalues_by_dbpath(QString("Select table_name from tbl_hashset Where database_path = ?"), QStringList(hashset_obj.hashset_db_path), 0, case_conf_db, Q_FUNC_INFO);
                hashset_obj.hashet_column_name =  get_string_from_db_with_addbindvalues_by_dbpath(QString("Select column_name from tbl_hashset Where database_path = ?"), QStringList(hashset_obj.hashset_db_path), 0, case_conf_db, Q_FUNC_INFO);

                QString hash_name = QFileInfo(hashset_obj.hashset_db_path).fileName();
                hash_name.remove(QFileInfo(hashset_obj.hashset_db_path).suffix());
                if(hash_name.endsWith("."))
                    hash_name.chop(1);

                hashset_obj.hash_name_str = hash_name;

                hashset_db_data_list.append(hashset_obj);
            }


            ///get matched hashset names
            QStringList matched_hashset_name_list;
            for(int num = 0; num < hashset_db_data_list.size(); num++)
            {
                struct_global_hashset_db_data hashset_obj = hashset_db_data_list.at(num);
                QString command_str = "Select " + hashset_obj.hashet_column_name + " From " + hashset_obj.hashet_table_name + " Where " + hashset_obj.hashet_column_name + " = ?";

                QString matched_hash_str =  get_string_from_db_with_addbindvalues_by_dbpath(command_str, QStringList(md5_hash_str), 0, hashset_obj.hashset_db_path, Q_FUNC_INFO);
                matched_hash_str = matched_hash_str.trimmed();

                if(!matched_hash_str.isEmpty())
                    matched_hashset_name_list << hashset_obj.hash_name_str;
            }

            metadata_str += "<b>" + QObject::tr("Hashset Name") + ":</b>" + QString("\t")+ matched_hashset_name_list.join(",") +"\n<br>";
            metadata_str += "<b>" + QObject::tr("MD5")  + ":</b>" +QString("\t")+ md5_hash_str +"\n<br>";
            metadata_str += "<b>" + QObject::tr("SHA1") + ":</b>" +QString("\t")+ sha1_hash_str +"\n<br>";
            metadata_str += "<br>";
        }

        QString datemodified_str = query_select.value("z_date_time_modified").toString();
        if(!datemodified_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Date Modified") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(datemodified_str, Q_FUNC_INFO) +"<br>";

        QString datechanged_str = query_select.value("z_date_time_change").toString();
        if(!datechanged_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Date Change") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(datechanged_str, Q_FUNC_INFO) +"<br>";

        QString datesccessed_str = query_select.value("z_date_time_accessed").toString();
        if(!datesccessed_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Date Accessed") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(datesccessed_str, Q_FUNC_INFO) +"<br>";

        QString datecreated_str = query_select.value("z_date_time_created").toString();
        if(!datecreated_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Date Created") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(datecreated_str, Q_FUNC_INFO) +"<br>";

        metadata_str += "<br>";

        QString dateadded_str = query_select.value("kMDItemDateAdded_Date_Added").toString();
        if(!dateadded_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Date Added(Apple)") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(dateadded_str, Q_FUNC_INFO) +"<br>";



        QString ccd_str = query_select.value("kMDItemContentCreationDate_Created_Date").toString();
        if(!ccd_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Content Creation Date(Apple)") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(ccd_str, Q_FUNC_INFO) +"<br>";


        QString cmd_str = query_select.value("kMDItemContentModificationDate_Last_Modified_Date").toString();
        if(!cmd_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Content Modification Date(Apple)") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(cmd_str, Q_FUNC_INFO) +"<br>";


        QString lused_str = query_select.value("kMDItemLastUsedDate_Last_Opened_Date").toString();
        if(!lused_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Last Used Date(Apple)") + ":</b>" +QString("\t")+  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(lused_str, Q_FUNC_INFO) +"<br>";



        QStringList used_list = query_select.value("kMDItemUsedDates_Used_Dates").toString().split(";", Qt::SkipEmptyParts);
        QString useddate_str;
        for(int pp = 0; pp < used_list.size(); pp++)
        {
            useddate_str.append(global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(used_list.at(pp), Q_FUNC_INFO)).append("<br>");
        }

        if(!useddate_str.trimmed().isEmpty())
            metadata_str += "<br><b><I>" + QObject::tr("Used Dates(Apple)") + ":</b></I> <br>" + QString("\t") + useddate_str +"<br>";

        QString usecount_str = query_select.value("kMDItemUseCount_Used_Count").toString();
        if(!usecount_str.trimmed().isEmpty())
            metadata_str += "<b>" + QObject::tr("Use Count") + ":</b>" + QString("\t") + usecount_str +"<br>";


        QString bookmarked_apple_m_value = query_select.value("bookmarked_extended_attributes").toString();
        if(!bookmarked_apple_m_value.trimmed().isEmpty())
        {
            if(bookmarked_apple_m_value.contains("\n"))
                bookmarked_apple_m_value.replace("\n" , "<br>");

            metadata_str += "<br><b><I>" + QObject::tr("Bookmarked Apple Metadata") + ":</b><I><br>" +QString("\t")+ bookmarked_apple_m_value +"\n<br>";
        }

        QString fs_type = struct_target_info.file_system_type;
        fs_type = fs_type.toLower();

        QString tag_text = "";
        QStringList list_tags = query_select.value("recon_tag_value").toString().split(",", Qt::SkipEmptyParts);
        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            if(list_tags.at(tag_i).isEmpty())
                continue;

            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += "<br>";

        metadata_str += QString("\n<b>Tag:</b>\t") + tag_text + "\n<br> <br>";
        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value("notes").toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
        obj.file_path = query_select.value("filepath").toString();
        obj.preview_file_path = query_select.value("id_path").toString();
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Ends" , Q_FUNC_INFO);

    return obj;
}


struct_global_fetch_metadata recon_helper_standard::fetch_metadata_for_saved_graph(QString record_no, QString plugin_name, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start" , Q_FUNC_INFO);

    struct_global_fetch_metadata obj;
    obj.metadata.clear();
    obj.file_path.clear();

    record_no = record_no.trimmed();
    if(record_no.isEmpty())
        return obj;


    QString db_path;
    if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
        db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() +  "Browser_History.sqlite" ;
    else if(plugin_name == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
        db_path =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() +   "Messenger.sqlite";


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QSqlQuery query_select(*destination_db);

    query_select.prepare("Select INT,plugin_name,graph_name,graph_path,timestamp,recon_tag_value ,notes From saved_graphs WHERE INT = ?");
    query_select.addBindValue(record_no);

    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text(), Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return obj;
    }

    QString metadata_str;
    if(query_select.next())
    {

        metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" +QString("\t") + query_select.value(0).toString() +"<br>" ;
        metadata_str += "<b>" + QObject::tr("Plugin") + ":</b>" +QString("\t") + query_select.value(1).toString() +"<br>" ;
        metadata_str += "<br>";
        metadata_str += "<b>" + QObject::tr("Graph Name") + ":</b>" +QString("\t") + query_select.value(2).toString() + "\n<br>";
        metadata_str += "<br>";
        metadata_str += "<b>" + QObject::tr("Timestamp") + ":</b>" +QString("\t") +  global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(query_select.value(4).toString(), Q_FUNC_INFO) + "\n<br>";

        QString file_path = get_file_infopath_by_dbreference(record_no,*destination_db,"saved_graphs","",Q_FUNC_INFO);
        obj.file_path = file_path;
        obj.preview_file_path = file_path;
        metadata_str += "<br>";
        metadata_str += "<b>" + QObject::tr("Graph Path") + ":</b>" +QString("\t") + file_path + "\n<br>";

        QString tag_text = "";
        QStringList list_tags = query_select.value(5).toString().split(",", Qt::SkipEmptyParts);
        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            if(list_tags.at(tag_i).isEmpty())
                continue;

            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";
        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(6).toString() +"<br>" + "</font>\n";

        obj.metadata = metadata_str;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end" , Q_FUNC_INFO);

    return obj;
}

QString recon_helper_standard::fetch_metadata_for_ram_analysis_saved(QString record_no, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    QString ram_analysis_saved_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_RAM_Analysis_Saved_In_Result_QString).toString() + "ram_analysis_saved.sqlite";

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(ram_analysis_saved_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " DB open ------FAILED-----" + ram_analysis_saved_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return "";
    }

    QSqlQuery query_select(*destination_db);
    query_select.prepare("SELECT INT, source_name,operating_system,build_version,artifacts,result_name ,command_output ,notes , recon_tag_value ,plugin_name from tbl_saved_result WHERE INT = ?");
    query_select.addBindValue(record_no);
    if(!query_select.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query_select execution ---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + query_select.lastError().text() ,Q_FUNC_INFO);
        recon_static_functions::app_debug( query_select.executedQuery(), Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return "";
    }
    QString metadata_str;

    QString command_output;
    if(query_select.next())
    {


        metadata_str += "<b>" + QObject::tr("Plugin Name") + ":</b>" + QString("\t") + query_select.value(9).toString() +"<br><br>";

        metadata_str += "<b>" + QObject::tr("Record No") + ":</b>" + QString("\t") + query_select.value(0).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Source Name") + ":</b>" + QString("\t") + query_select.value(1).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Operating System") + ":</b>" + QString("\t") + query_select.value(2).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Build Version") + ":</b>" + QString("\t") + query_select.value(3).toString() +"<br><br>";
        metadata_str += "<b>" + QObject::tr("Artifacts") + ":</b>" + QString("\t") + query_select.value(4).toString() +"<br><br>";


        metadata_str += "<b>" + QObject::tr("Result Name") + ":</b>" + QString("\t") + query_select.value(5).toString() +"<br><br>";

        command_output  +=  "<b>" + QObject::tr("Command Output") + ":</b>" + QString("\t") + "<br>" + query_select.value(6).toString() +"<br><br>";
        if(command_output.contains("\n\n"))
            command_output.replace("\n\n","<br><br>");



        metadata_str += command_output ;
        metadata_str += QString("\n<br>");


        QString tag_text = "";
        QStringList list_tags = query_select.value(8).toString().split(",", Qt::SkipEmptyParts);
        for(int tag_i = 0; tag_i < list_tags.size(); tag_i++)
        {
            if(list_tags.at(tag_i).isEmpty())
                continue;

            QString colour =  get_colour_name_of_tag_from_db(list_tags.at(tag_i) , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO);
            tag_text += "<font color = \"" +  get_color_hex_from_color_name(colour , QString(global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite"), Q_FUNC_INFO) +"\">" + list_tags.at(tag_i) + "</font>" + ",";
        }

        if(tag_text.endsWith(","))
            tag_text.chop(1);

        metadata_str += QString("\n<br><b>Tag:</b>\t") + tag_text + "\n<br> <br>";

        metadata_str += "\n<b>" + QObject::tr("Examiner Notes") + ":</b>\t<font color = \"blue\">" +QString("\t") + query_select.value(7).toString() +"<br>" + "</font>\n";


    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);

    return metadata_str;
}


void recon_helper_standard::pub_enable_or_disable_right_clicks_depends_on_file_existance(struct_global_right_clicks_status_depends_on_file_existance obj , QMenu *menu , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    if(obj.selected_row_count > 1)
        return ;


    QString plugin_name_str = obj.plugin_name;
    QString feature_name_str = obj.feature_name;
    QString tab_name_str = obj.tab_name;
    QString record_no = obj.record_no;
    QString source_count_name = obj.source_count_name;
    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    recon_static_functions::app_debug(" ---feature name---      " + feature_name_str , Q_FUNC_INFO);
    recon_static_functions::app_debug(" ---plugin name---       " + plugin_name_str , Q_FUNC_INFO);
    recon_static_functions::app_debug(" ---record no---         " + record_no , Q_FUNC_INFO);
    recon_static_functions::app_debug(" ---source count name--- " + source_count_name , Q_FUNC_INFO);


    bool bool_is_dir = false;
    QString complete_path ;
    if(plugin_name_str == QString(MACRO_Plugin_Name_File_System))
    {
        QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/file_system.sqlite";

        complete_path = get_file_infopath_by_dbpath(record_no,fs_db_path,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name ,Q_FUNC_INFO);
        bool_is_dir = is_it_dir_record_no_by_dbpath(record_no,fs_db_path,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name ,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Screenshots))
    {
        QString screenshot_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Screenshots_In_Result_QString).toString() + "screenshots.sqlite";
        complete_path = get_file_infopath_by_dbpath(record_no,screenshot_db_path,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name ,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Browser_History))
    {
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Browser_History_In_Result_QString).toString() + "Browser_History.sqlite";
        complete_path = get_file_infopath_by_dbpath(record_no, db_path ,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Graphs_Messenger))
    {
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Graphs_Messenger_In_Result_QString).toString() + "Messenger.sqlite";
        complete_path = get_file_infopath_by_dbpath(record_no, db_path ,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Saved_Maps))
    {
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Saved_Maps_In_Result_QString).toString() + "saved_maps.sqlite";
        complete_path = get_file_infopath_by_dbpath(record_no, db_path ,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Carved_Files))
    {
        QStringList arg_list = obj.arguments_values_list;
        QString index_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Carved_Files_In_Result_QString).toString() + "carved_files_index.sqlite";
        QString cmd = "Select carver_files_db_path from table_carver_index where label_name = ?";
        QString relative_db_path =  get_string_from_db_with_addbindvalues_by_dbpath(cmd , arg_list,0,index_db_path , Q_FUNC_INFO);
        QString db_path_carved_files = global_recon_helper_singular_class_obj->get_complete_file_path(result_dir_path,relative_db_path , Q_FUNC_INFO);
        complete_path = get_file_infopath_by_dbpath(record_no, db_path_carved_files ,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Text_Viewer))
    {
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Text_Viewer_QString).toString() + "text_viewer_content_info.sqlite";
        complete_path = get_file_infopath_by_dbpath(record_no,db_path,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Strings_Viewer))
    {
        QString db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Strings_Viewer_QString).toString() + "string_viewer_content_info.sqlite";
        complete_path = get_file_infopath_by_dbpath(record_no,db_path,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Sqlite_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Plist_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Registry_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Hex_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Log_Viewer)
            || plugin_name_str == QString(MACRO_Plugin_Name_Messenger_Full_RR)
            || plugin_name_str == QString(MACRO_Plugin_Name_Browser_History_Full_RR)
            || plugin_name_str == QString(MACRO_Plugin_Name_Ram_Analysis_Saved)
            || plugin_name_str == QString(MACRO_Plugin_Name_Carved_Data)
            || plugin_name_str == QString(MACRO_Plugin_Name_Mobile_Backup)
            || plugin_name_str == QString(MACRO_Plugin_Name_Carved_Password)
            || plugin_name_str == QString(MACRO_Plugin_Name_Artifacts_Timeline))
    {
        complete_path = "";
    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser) || plugin_name_str == QString(MACRO_Plugin_Name_Email_Parser_Tag))
    {
        if(feature_name_str == MACRO_CASE_TREE_Story_Board
                || feature_name_str == MACRO_CASE_TREE_Tags
                || feature_name_str == MACRO_CASE_TREE_Notes_Parent)
        {
            foreach (QAction *action, menu->actions())
            {
                ///---right click are disable always...
                if(action->text() == QString(MACRO_Generic_Right_Click_Add_Files)
                        || action->text() == QString(MACRO_Generic_Right_Click_Quick_look)
                        || action->text() == QString(MACRO_Generic_Right_Click_Export_Filesystem_Records)
                        || action->text() == QString(MACRO_Generic_Right_Click_Add_Record_With_Files)
                        || action->text() == QString(MACRO_Generic_Right_Click_Export))
                {
                    action->setEnabled(false);
                }

            }
            recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
            return ;
        }


        QString emlx_file_result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Email_Parser_In_Result_QString).toString();
        QString attachment_dir_path = emlx_file_result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_Attachments;

        QString emlx_db_path = emlx_file_result_dir_path + "email_data.sqlite";
        QString command = QString("select email_attachment  From emlx_data Where source_file = ? AND INT = ?");
        QStringList arg_list = obj.arguments_values_list;
        arg_list << record_no;
        QString attachments_name_str =  get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,emlx_db_path,Q_FUNC_INFO);
        if(attachments_name_str.trimmed().isEmpty())
        {
            complete_path = attachments_name_str;
        }
        else if(attachments_name_str.contains(","))
        {
            QString file_name;
            QStringList attachment_split_list;
            attachment_split_list = attachments_name_str.split(",",Qt::SkipEmptyParts);
            for(int ii = 0 ; ii < attachment_split_list.size() ; ii++)
            {
                file_name = attachment_split_list.at(ii);
                complete_path = attachment_dir_path + file_name;
                if(QFileInfo(complete_path).exists())
                    break;
            }

        }
        else
        {
            complete_path =  attachment_dir_path + attachments_name_str;
        }

    }
    else if(plugin_name_str == QString(MACRO_Plugin_Name_Snapshots))
    {
        QStringList snapshot_db_name_list = obj.arguments_values_list;
        if(snapshot_db_name_list.size() == 1)
        {
            QString snp_diff_db_path  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Snapshots_Difference_In_Result_QString).toString() + snapshot_db_name_list.at(0) + ".sqlite";
            complete_path = get_file_infopath_by_dbpath(record_no, snp_diff_db_path ,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),source_count_name,Q_FUNC_INFO);
        }
    }
    else
    {
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return;

        QString tmp_plg_name = plugin_name_str;
        tmp_plg_name.replace(" ","_");

        QString destination_plugin_dir_path = result_dir_path + MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_RESULT_DIR_PLUGINS +  tmp_plg_name;
        QString plugin_db_file_path = destination_plugin_dir_path + "/" + tmp_plg_name + ".sqlite";
        if(!QFileInfo(plugin_db_file_path).exists())
            return ;


        struct_global_csv_reader_full_csv_info st_csv_info_obj = global_csv_reader_class_selected_obj->read_csv_value(plugin_name_str);
        int tab_index = st_csv_info_obj.tab_name_QStringList_tablewidget.indexOf(tab_name_str);
        if(st_csv_info_obj.table_name_QStringList_tablewidget.size() <= tab_index || tab_index < 0)
            return;

        QStringList export_col_name_list = st_csv_info_obj.export_path_column_name_QList_tablewidget.at(tab_index);
        struct_global_artifacts_export_fileinfo obj = get_export_path_list_and_display_path_list_for_plugin_current_record(plugin_db_file_path,export_col_name_list,global_csv_reader_class_selected_obj->get_table_name_according_tab(plugin_name_str,tab_name_str),record_no,Q_FUNC_INFO);
        QStringList all_path_list = obj.complete_export_filepaths;
        if(all_path_list.size() == 0)
            complete_path = "";

        //        for(int ii = 0 ; ii < all_path_list.size() ; ii++)
        //        {
        //            complete_path = all_path_list.at(ii);
        //            if(QFileInfo(complete_path).exists())
        //                break;
        //        }

        if(all_path_list.size() > 0)
            complete_path = all_path_list.at(0);
        else
            complete_path = "";
    }

    if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name_str) && obj.feature_name == QString(MACRO_CASE_TREE_Story_Board))
    {
        ///---add files and add files record right click are disable always...
        if(QFileInfo(complete_path).exists())
        {
            foreach (QAction *action, menu->actions())
            {
                if(action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
                {
                    action->setEnabled(true);
                    break;
                }
            }
        }
        else
        {
            foreach (QAction *action, menu->actions())
            {
                if(action->text() == QString(MACRO_Generic_Right_Click_Quick_look))
                {
                    action->setEnabled(false);
                    break;
                }
            }
        }

        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return ;
    }



    recon_static_functions::app_debug(" ---complete path---     " + complete_path , Q_FUNC_INFO);


    struct_GLOBAL_witness_info_source struct_target_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString os_schme_dsply = struct_target_info.os_scheme_display;

    if(bool_is_dir && QFileInfo(complete_path).exists())
    {
        foreach (QAction *action, menu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database)
                    || action->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Data)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                    || action->text() == QString(MACRO_Generic_Right_Click_Send_To_Bucket)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With))
            {
                if(action->menu())
                {
                    action->menu()->setEnabled(false);
                }
                else
                    action->setEnabled(false);

            }
        }
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return ;
    }
    else if(bool_is_dir && os_schme_dsply == MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_OS_SCHEME_iOS_Display)
    {
        foreach (QAction *action, menu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database)
                    || action->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Data)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                    || action->text() == QString(MACRO_Generic_Right_Click_Send_To_Bucket)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With))
            {
                if(action->menu())
                {
                    action->menu()->setEnabled(false);
                }
                else
                    action->setEnabled(false);

            }
        }
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return ;
    }
    else if(((bool_is_dir && (!QFileInfo(complete_path).exists()))))
    {
        foreach (QAction *action, menu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Export)
                    ||action->text() == QString(MACRO_Generic_Right_Click_Carve_Data)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Quick_look)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database)
                    || action->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                    || action->text() == QString(MACRO_Generic_Right_Click_Send_To_Bucket)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With)
                    || action->text() == QString(MACRO_Generic_Right_Click_Export_Filesystem_Records)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_Record_With_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As))
            {
                if(action->menu())
                {
                    action->menu()->setEnabled(false);
                }
                else
                    action->setEnabled(false);
            }


        }
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return ;
    }
    else if(QFileInfo(complete_path).isFile() && QFileInfo(complete_path).exists())
    {
        foreach (QAction *action, menu->actions())
        {

            if(action->text() == QString(MACRO_Generic_Right_Click_Export)
                    ||action->text() == QString(MACRO_Generic_Right_Click_Carve_Data)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Quick_look)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database)
                    || action->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                    || action->text() == QString(MACRO_Generic_Right_Click_Send_To_Bucket)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With)
                    || action->text() == QString(MACRO_Generic_Right_Click_Export_Filesystem_Records)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_Record_With_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As))
            {
                if(action->menu())
                {
                    action->menu()->setEnabled(true);
                }
                else
                    action->setEnabled(true);
            }

        }

        if(global_recon_helper_singular_class_obj->is_it_recon_custom_picture_plugin(plugin_name_str))
        {
            foreach (QAction *action, menu->actions())
            {

                if(action->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database)
                        || action->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash)
                        || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules)
                        || action->text() == QString(MACRO_Generic_Right_Click_Send_To_Bucket)
                        || action->text() == QString(MACRO_Generic_Right_Click_Open_With)
                        || action->text() == QString(MACRO_Generic_Right_Click_Export_Filesystem_Records))
                {
                    if(action->menu())
                    {
                        action->menu()->setEnabled(false);
                    }
                    else
                        action->setEnabled(false);
                }
            }
        }

        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return ;
    }
    else if(!QFileInfo(complete_path).exists())
    {
        foreach (QAction *action, menu->actions())
        {
            if(action->text() == QString(MACRO_Generic_Right_Click_Export)
                    ||action->text() == QString(MACRO_Generic_Right_Click_Carve_Data)
                    || action->text() == QString(MACRO_Generic_Right_Click_Carve_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Quick_look)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_File_To_Hashset_Database)
                    || action->text() == QString(MACRO_Generic_Right_Click_Search_File_With_Same_Hash)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Filesystem_Modules)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_To_Text_Indexing_Queue)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With_External_Application)
                    || action->text() == QString(MACRO_Generic_Right_Click_Send_To_Bucket)
                    || action->text() == QString(MACRO_Generic_Right_Click_Open_With)
                    || action->text() == QString(MACRO_Generic_Right_Click_Export_Filesystem_Records)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Add_Record_With_Files)
                    || action->text() == QString(MACRO_Generic_Right_Click_Run_Export_Hashes_As))
            {
                if(action->menu())
                {
                    action->menu()->setEnabled(false);
                }
                else
                    action->setEnabled(false);
            }


        }

        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return ;
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
}


QString recon_helper_standard::get_file_infopath_by_dbpath(QString record_no, QString destination_db_path, QString table_name, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QString final_path;


    //    if(!QFileInfo(destination_db_path).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_path,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return final_path;
    //    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(destination_db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(" Destination Db opening ----FAILED--- " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return final_path;
    }

    final_path = get_file_infopath_by_dbreference(record_no, *destination_db, table_name, source_count_name, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return final_path;
}

QString recon_helper_standard::get_file_infopath_by_dbreference(QString record_no, QSqlDatabase &destination_db, QString table_name, QString source_count_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QString command = "select recon_filefrom, recon_file_infopath from '" + table_name +  "' where INT = '" + record_no + "'";

    QSqlQuery query_select =  get_executed_sqlquery_from_db_by_dbreference(command,destination_db,(recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
    QString path_in_db = query_select.value("recon_file_infopath").toString();

    if(path_in_db.trimmed().isEmpty())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " ---FAILED--- path in db blank, it may be iOS dir , db path - " + destination_db.databaseName() + " record no - " + record_no, (recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
        return "";
    }

    QString file_from_str = query_select.value("recon_filefrom").toString();
    if(file_from_str.trimmed().isEmpty())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " ---FAILED--- file_from_str in db blank , db path - " + destination_db.databaseName() + " record no - " + record_no, (recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
        return "";
    }

    int file_from = file_from_str.toInt();

    struct_GLOBAL_witness_info_source tgt_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    QString init_path = tgt_obj.virtual_source_path;
    if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Main)
    {
        init_path = tgt_obj.virtual_source_path;
    }
    else if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_TSK_Fuse)
    {
        init_path = tgt_obj.fuse_mount_path_tsk;
    }
    else if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result)
    {
        init_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    }
    else if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Examiner_Selected_Path)
    {
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end 111" , Q_FUNC_INFO);
        return path_in_db;
    }
    else if(file_from == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Source_Snapshot)
    {
        init_path =  get_snapshots_mount_path(source_count_name,destination_db.databaseName());
    }


    if(init_path.trimmed().isEmpty())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " ---FAILED--- INIT path blank , db path - " + destination_db.databaseName() + " record no - " + record_no, (recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
        return "";
    }

    if(!QFileInfo(init_path).exists())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " ---FAILED--- INIT path not exist, db path - " + destination_db.databaseName() + " record no - " + record_no, (recon_static_functions::prepare_callerfun(caller_func) + Q_FUNC_INFO));
        return "";
    }


    QString final_path = global_recon_helper_singular_class_obj->get_complete_file_path(init_path, path_in_db, Q_FUNC_INFO);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return final_path;
}

qint64 recon_helper_standard::get_intvalue_from_db_with_addbindvalues_by_dbreference(QString command, QSqlDatabase &destination_db,int pos, QStringList values, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    qint64 total_count = 0;

    QSqlQuery query(destination_db);
    query.prepare(command);
    for(int i = 0; i < values.size(); i++)
    {
        query.addBindValue(values.at(i));
    }
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------db file - " + destination_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return total_count;
    }

    if(query.next())
    {
        total_count = query.value(pos).toInt();
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return total_count;
}

qint64 recon_helper_standard::get_intvalue_from_db_with_addbindvalues_by_dbpath(QString command, QString db_path,int pos, QStringList values, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    //    if(!QFileInfo(db_path).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + db_path,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return -1;
    //    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path);

    if (!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " db open ------FAILED-------" + db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return -1;
    }

    qint64 total_records = get_intvalue_from_db_with_addbindvalues_by_dbreference(command, *destination_db, pos, values, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return total_records;
}
bool recon_helper_standard::pub_check_path_is_directory_by_stat(QString filepath, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);
    bool bool_is_dir = false;
    struct stat s;
    if(stat(filepath.toLocal8Bit().data(),&s) == 0 )
    {
        if(s.st_mode & S_IFDIR)
        {
            //it's a directory
            bool_is_dir = true;
        }
    }
    else
    {
        //error
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Error " , Q_FUNC_INFO);
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " End " , Q_FUNC_INFO);
    return bool_is_dir;
}

bool recon_helper_standard::pub_check_path_is_file_by_stat(QString filepath, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);
    bool bool_is_file = false;
    struct stat s;
    if(stat(filepath.toLocal8Bit().data(),&s) == 0 )
    {
        if(s.st_mode & S_IFREG)
        {
            //it's a file
            bool_is_file = true;
        }
    }
    else
    {
        //error
        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Error " , Q_FUNC_INFO);
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " End " , Q_FUNC_INFO);
    return bool_is_file;
}

bool recon_helper_standard::pub_check_path_is_directory_by_fs_dbreference(QString filepath, QString fs_record_no, QSqlDatabase destination_db ,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    bool bool_is_directory = false;

    QString command = QString("Select is_dir from files Where INT = ?");

    QStringList value_list;
    value_list << fs_record_no;

    QString is_dir_str = get_string_from_db_with_addbindvalues_by_dbreference(command, value_list, 0, destination_db, Q_FUNC_INFO).trimmed();
    if(is_dir_str.isEmpty())
    {
        bool_is_directory = pub_check_path_is_directory_by_stat(filepath, Q_FUNC_INFO);
    }
    else if(is_dir_str == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
    {
        bool_is_directory = true;
    }

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bool_is_directory;
}

bool recon_helper_standard::pub_check_path_is_file_by_fs_dbreference(QString filepath, QString fs_record_no, QSqlDatabase destination_db ,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    bool bool_is_file = false;
    QStringList tables_list = destination_db.tables();
    if(!tables_list.contains("is_dir"))
    {
        bool_is_file = pub_check_path_is_file_by_stat(filepath, Q_FUNC_INFO);
    }
    else
    {
        QString command = QString("Select is_dir from files Where INT = ?");
        QStringList value_list;
        value_list << fs_record_no;

        QString is_file_str = get_string_from_db_with_addbindvalues_by_dbreference(command, value_list, 0, destination_db, Q_FUNC_INFO).trimmed();
        if(is_file_str.isEmpty())
        {
            bool_is_file = pub_check_path_is_file_by_stat(filepath, Q_FUNC_INFO);
        }
        else if(is_file_str == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
        {
            bool_is_file = true;
        }
    }

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bool_is_file;
}

bool recon_helper_standard::pub_check_path_is_directory_by_fs_dbpath(QString filepath, QString fs_record_no, QString fs_db_path ,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    bool bool_is_directory = false;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(fs_db_path);

    if (!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " db open ------FAILED-------" + fs_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return bool_is_directory;
    }

    bool_is_directory = pub_check_path_is_directory_by_fs_dbreference(filepath, fs_record_no, *destination_db, Q_FUNC_INFO);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bool_is_directory;
}

bool recon_helper_standard::pub_check_path_is_file_by_fs_dbpath(QString filepath, QString fs_record_no, QString fs_db_path ,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    bool bool_is_file = false;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(fs_db_path);

    if (!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " db open ------FAILED-------" + fs_db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return bool_is_file;
    }


    bool_is_file = pub_check_path_is_file_by_fs_dbreference(filepath, fs_record_no, *destination_db, Q_FUNC_INFO);


    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bool_is_file;
}

QSqlQuery recon_helper_standard::get_executed_sqlquery_from_db_by_dbreference(QString command,QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return query;
    }

    if(query.next())
    {
    }


    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return query;
}

QString recon_helper_standard::get_filename_by_filepath_without_fileinfo(QString filepath)
{
    filepath = filepath.trimmed();
    if(filepath.isEmpty())
        return QString("");

    QStringList path_split_list = filepath.split("/");

    QString filename = path_split_list.at(path_split_list.size() -1);
    filename = filename.trimmed();

    return filename;
}

QString recon_helper_standard::pub_get_absolute_dir_path_without_fileinfo(QString filepath)
{
    filepath = filepath.trimmed();
    if(filepath.isEmpty())
        return QString("");

    QStringList path_split_list = filepath.split("/");

    QString absolute_dir;
    if(path_split_list.size() > 0)
    {
        path_split_list.removeLast();
        absolute_dir = path_split_list.join("/");
    }


    if(!absolute_dir.trimmed().startsWith("/"))
        absolute_dir.prepend("/");

    return absolute_dir;
}

qint64 recon_helper_standard::get_db_total_record_by_dbpath(bool bool_process_db_all_tables, QString db_path, QStringList tables_list, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    //    if(!QFileInfo(db_path).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + db_path,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return -1;
    //    }

    QString connection_naam = QString::number(QDateTime::currentMSecsSinceEpoch()) + QString::number(QDateTime::currentSecsSinceEpoch()) + caller_func;
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(db_path);

    if (!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + "  db open ------FAILED-------" + db_path , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error - " + destination_db->lastError().text() ,Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return -1;
    }

    qint64 total_records = get_db_total_record_by_dbreference(bool_process_db_all_tables, *destination_db,tables_list, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);

    return total_records;
}

qint64 recon_helper_standard::get_db_total_record_by_dbreference(bool bool_process_db_all_tables, QSqlDatabase &destination_db,  QStringList tables_list, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " start ", Q_FUNC_INFO);

    qint64 total_records = 0;
    QSqlQuery query(destination_db);

    if(bool_process_db_all_tables)
        tables_list = destination_db.tables();

    for(int i = 0; i < tables_list.size() ; i++)
    {
        QString command = "SELECT Count(*)  from '" + tables_list.at(i).trimmed() + "'";

        query.prepare(command);
        if(!query.exec())
        {
            recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " execute query ------FAILED------ " , Q_FUNC_INFO);
            recon_static_functions::app_debug(" Error " + query.lastError().text() , Q_FUNC_INFO);
            recon_static_functions::app_debug(query.executedQuery() , Q_FUNC_INFO);
            continue;
        }

        if(query.next())
        {
            total_records += query.value(0).toInt();
        }

    }

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end ", Q_FUNC_INFO);

    return total_records;
}

bool recon_helper_standard::execute_db_command_by_dbreference(QString command, QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return false;
    }

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return true;

}
bool recon_helper_standard::execute_db_command_with_addbindvalues_by_dbreference(QString command, QStringList values, QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QSqlQuery query(destination_db);
    query.prepare(command);

    for(int i = 0; i < values.size(); i++)
    {
        query.addBindValue(values.at(i));
    }
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return false;

    }


    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return true;
}

bool recon_helper_standard::execute_db_command_by_dbpath(QString command, QString destination_db_file, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return false;
    //    }


    bool mstatus = false;
    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return mstatus;
    }

    mstatus = execute_db_command_by_dbreference(command, *destination_db, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return mstatus;
}

bool recon_helper_standard::execute_db_command_with_addbindvalues_by_dbpath(QString command, QStringList values, QString destination_db_file, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return false;
    //    }

    bool mstatus = false;
    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return mstatus;
    }

    mstatus = execute_db_command_with_addbindvalues_by_dbreference(command, values, *destination_db,caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return mstatus;
}


QStringList recon_helper_standard::get_stringlist_from_db_by_dbpath(QString command,int pos,QString destination_db_file,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QStringList list_gen;

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return list_gen;
    //    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return list_gen;
    }

    list_gen = get_stringlist_from_db_by_dbreference(command, pos, *destination_db, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return list_gen;
}



QStringList recon_helper_standard::get_stringlist_from_db_with_addbindvalues_by_dbpath(QString command, QStringList values, int pos, QString destination_db_file,QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QStringList list_gen;

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return list_gen;
    //    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return list_gen;
    }

    list_gen = get_stringlist_from_db_with_addbindvalues_by_dbreference(command, values, pos, *destination_db, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " End " , Q_FUNC_INFO);

    return list_gen;
}

QString recon_helper_standard::get_string_from_db_by_dbpath(QString command,int pos,QString destination_db_file,QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString str_gen;

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return str_gen;
    //    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch()) + QString::number(QDateTime::currentSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return str_gen;
    }

    str_gen = get_string_from_db_by_dbreference(command,pos, *destination_db,  caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    str_gen = str_gen.trimmed();

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return str_gen;
}

QString recon_helper_standard::get_string_from_db_by_dbpath_no_trimmed(QString command,int pos,QString destination_db_file,QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString str_gen;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return str_gen;
    }

    QSqlQuery query(*destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        destination_db->close();
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return str_gen;
    }

    if(query.next())
    {
        str_gen = query.value(pos).toString();
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return str_gen;
}

QString recon_helper_standard::get_string_from_db_with_addbindvalues_by_dbpath(QString command, QStringList values, int pos, QString destination_db_file,QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString str_gen;

    //    if(!QFileInfo(destination_db_file).exists())
    //    {
    //        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " file does not exist --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
    //        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    //        return str_gen;
    //    }

    QString connection_naam =  QString::number(QDateTime::currentMSecsSinceEpoch()) + QString::number(QDateTime::currentSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return str_gen;
    }

    str_gen = get_string_from_db_with_addbindvalues_by_dbreference(command, values, pos, *destination_db, caller_func);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    str_gen = str_gen.trimmed();

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return str_gen;
}

QString recon_helper_standard::get_string_from_db_by_dbreference(QString command,int pos,QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString str_gen;

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return str_gen;
    }

    if(query.next())
    {
        str_gen = query.value(pos).toString();
    }

    str_gen = str_gen.trimmed();

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return str_gen;
}

QString recon_helper_standard::get_string_from_db_with_addbindvalues_by_dbreference(QString command, QStringList values, int pos, QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString str_gen;

    QSqlQuery query(destination_db);
    query.prepare(command);
    for(int i = 0; i < values.size(); i++)
    {
        query.addBindValue(values.at(i));
    }

    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------ db file - " + destination_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return str_gen;
    }

    if(query.next())
    {
        str_gen = query.value(pos).toString();
    }

    str_gen = str_gen.trimmed();

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return str_gen;
}

QStringList recon_helper_standard::get_stringlist_from_db_by_dbreference(QString command,int pos,QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QStringList list_gen;

    QSqlQuery query(destination_db);
    query.prepare(command);
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------db file - " + destination_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return list_gen;
    }

    int count = 0;

    while(query.next())
    {
        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        list_gen << query.value(pos).toString().trimmed();
    }


    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return list_gen;
}

QStringList recon_helper_standard::get_stringlist_from_db_with_addbindvalues_by_dbreference(QString command, QStringList values, int pos, QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QStringList list_gen;

    QSqlQuery query(destination_db);
    query.prepare(command);
    for(int i = 0; i < values.size(); i++)
    {
        query.addBindValue(values.at(i));
    }
    if(!query.exec())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " query  --------FAILED------db file - " + destination_db.databaseName() ,Q_FUNC_INFO);
        recon_static_functions::app_debug(" command -  " + command,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + query.lastError().text(),Q_FUNC_INFO);
        return list_gen;
    }
    int count = 0;

    while(query.next())
    {
        count++;
        if(count % 50 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }

        list_gen << query.value(pos).toString().trimmed();
    }

    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return list_gen;
}

//===================

QString recon_helper_standard::get_color_hex_from_color_name(QString color_name , QString dbPath, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    QString command = "select color_hex from 'colors' where color_name = ?";
    QStringList list;
    list << color_name;
    QString color_hex = get_string_from_db_with_addbindvalues_by_dbpath(command,list,0,dbPath,Q_FUNC_INFO);


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + "  -Ends " , Q_FUNC_INFO);

    return color_hex;
}


QString recon_helper_standard::get_colour_name_of_tag_from_db(QString tag_name , QString dbPath, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    if(tag_name.trimmed().isEmpty())
        return "";


    QString command = "select selected_colour from 'tags' where tag_name = ?";
    QStringList list;
    list << tag_name;

    QString tag_colour = get_string_from_db_with_addbindvalues_by_dbpath(command,list,0,dbPath,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Ends " , Q_FUNC_INFO);
    return tag_colour;
}

QStringList recon_helper_standard::get_keyword_search_namelist_from_result_db( QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString result_feature_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Artifacts_Keyword_Search_In_Result_QString).toString();
    QString destination_db_file = result_feature_path + "artifacts_keyword_search.sqlite";
    QString command = "select search_tag from 'keyword_search_index'";
    QStringList list = get_stringlist_from_db_by_dbpath(command,0,destination_db_file,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return list;
}

QString recon_helper_standard::get_category_of_plugin_by_dbreference(QSqlDatabase &destiantion_db, QString plugin_name, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString command = "select category from 'plugins' where name = ?";
    QString str = get_string_from_db_with_addbindvalues_by_dbreference(command,QStringList(plugin_name),0,destiantion_db,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return str.trimmed();
}

QStringList recon_helper_standard::get_hex_namelist_from_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString command = "select viewer_display_name from 'hex_viewer_index'";
    QStringList list = get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return list;
}

QStringList recon_helper_standard::get_sqlite_namelist_from_db(QString result_path, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QString command = "select viewer_display_name from 'sv_index'";
    QStringList list = get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return list;
}

QStringList recon_helper_standard::extract_all_record_no_for_selected_tags(QString table_name, QString destination_db_path, QStringList tags_list, QString caller_func)
{
    QStringList total_record_list;
    QStringList vals_list;
    QStringList record_list;
    QString m_command;

    for(int i = 0; i < tags_list.size(); i++)
    {

        vals_list.clear();

        //vals_list <<  QString("'%") + tags_list.at(i) + QString("%'");
        //m_command   = QString("Select INT FROM '" + table_name + "' WHERE recon_tag_value LIKE ?");
        m_command   = QString("Select INT FROM '" + table_name + "' WHERE ");

        if(tags_list.at(i) == QString(MACRO_Tag_Name_Bookmarks))
        {
            vals_list << QString::number(1);

            // m_command.append(QString(" OR Bookmark = ?"));
            m_command.append(QString("Bookmark = ?"));
            record_list = get_stringlist_from_db_with_addbindvalues_by_dbpath(m_command,vals_list, 0, destination_db_path, Q_FUNC_INFO);
        }
        else
        {
            m_command.append(QString("recon_tag_value = ?"));
            // vals_list <<  QString("'%") + tags_list.at(i) + QString("%'");
            vals_list << tags_list.at(i);

            record_list = get_stringlist_from_db_with_addbindvalues_by_dbpath(m_command,vals_list, 0, destination_db_path, Q_FUNC_INFO);
        }

        total_record_list << record_list;
    }

    total_record_list.removeDuplicates();
    total_record_list.removeAll("");
    //total_record_list.sort();

    //===============================================================//
    // to sort stringlist according record number, like QEditRole in data model
    std::multiset<int> set_i;
    for(int pp = 0; pp < total_record_list.size(); pp++)
        set_i.insert(total_record_list.at(pp).toInt());

    total_record_list.clear();
    std::multiset<int>::iterator iter;
    for(iter = set_i.begin(); iter != set_i.end(); iter++)
        total_record_list.append(QString::number(*iter));
    //===============================================================//


    return total_record_list;
}

QStringList recon_helper_standard::get_total_extracted_plugin_with_extracted_tab_list(QString caller_func)
{
    QStringList total_extracted_plugin_with_extracted_tab_list;

    QString plugin_db_paths = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "plugin_status.sqlite";
    if(!QFile(plugin_db_paths).exists())
    {
        return total_extracted_plugin_with_extracted_tab_list;
    }

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(plugin_db_paths);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening ---FAILED---- " + destination_db->databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return total_extracted_plugin_with_extracted_tab_list;
    }

    QString cmd = "select distinct plugin_name from tbl_plugin_status";
    QStringList plugin_list = get_stringlist_from_db_by_dbreference(cmd, 0, *destination_db, Q_FUNC_INFO);

    for(int i = 0; i < plugin_list.size(); i++)
    {
        QString  command2 = QString("select tab_name from tbl_plugin_status where plugin_name = ?");

        QStringList list1;
        list1 << plugin_list.at(i);
        QStringList list_tab = get_stringlist_from_db_with_addbindvalues_by_dbreference(command2,list1,0,*destination_db,Q_FUNC_INFO);

        QString final_str = list_tab.join(MACRO_RECON_Splitter_2_hash_in_brace);
        final_str.prepend(MACRO_RECON_Splitter_1_multiple);
        final_str.prepend(plugin_list.at(i));

        total_extracted_plugin_with_extracted_tab_list << final_str;
    }

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    return total_extracted_plugin_with_extracted_tab_list;

}

QString recon_helper_standard::create_separate_tag_db_for_tag_search(QString tags_name,QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Starts",Q_FUNC_INFO);
    QString result_feature_directory = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Tag_Search_In_Result_QString).toString();
    QString tag_db_name = tags_name;
    tag_db_name.replace(" ","_");

    QString tags_result_db_path = result_feature_directory  + tag_db_name + ".sqlite";
    QFileInfo info_tag_db(tags_result_db_path);
    if(info_tag_db.exists())
    {
        return tags_result_db_path;
    }
    else
    {
        QString command_tag_records = ("create table tags (INT INTEGER PRIMARY KEY,bookmark,category varchar(300),"
                                       "record_no INTEGER,notes_icon varchar(300),plugin_name varchar(300), tab_name varchar(100),csv_plugin_name varchar(100),csv_tab_name varchar(100),"
                                       "table_name varchar(200),recon_tag_value varchar(100),notes varchar(100),os_scheme_display varchar(100), source_count_name varchar(100),"
                                       "item0 varchar(300),item1 varchar(300),item2 varchar(300),item3 varchar(300),item4 varchar(300),"
                                       "item5 varchar(300),item6 varchar(300),item7 varchar(300),item8 varchar(300),"
                                       "item9 varchar(300),item10 varchar(300),item11 varchar(300),item12 varchar(300),"
                                       "item13 varchar(300),item14 varchar(300),item15 varchar(300),item16 varchar(300),"
                                       "item17 varchar(300),item18 varchar(300),item19 varchar(300))");
        execute_db_command_by_dbpath(command_tag_records,tags_result_db_path,Q_FUNC_INFO + caller_func);


        create_db_index_by_db_path("tag_int", "INT", "tags", tags_result_db_path, Q_FUNC_INFO);
        create_db_index_by_db_path("tag_record", "record_no", "tags", tags_result_db_path, Q_FUNC_INFO);

        QString creation_time = QString::number(QDateTime::currentDateTimeUtc().toUTC().toSecsSinceEpoch());
        QString tag_index_db_path = result_feature_directory + "index_tag_search.sqlite";
        QString command_tag_name_index = "INSERT INTO tag_search_index(tag_name,tag_db_name,creation_timestamp) VALUES(?,?,?)";
        QStringList arg_list;
        arg_list << tags_name << tag_db_name << creation_time;
        execute_db_command_with_addbindvalues_by_dbpath(command_tag_name_index,arg_list,tag_index_db_path,Q_FUNC_INFO + caller_func);
    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Ends",Q_FUNC_INFO);

    return tags_result_db_path;
}

void recon_helper_standard::create_db_index_by_db_path(QString index_name, QString column_name, QString table_name, QString db_path, QString caller_func)
{
    //  QString command = "create index '" + index_name + "' on '" + table_name + "' ('" + column_name + "')";
    QString command = "create index IF NOT EXISTS '" + index_name + "' on '" + table_name + "' ('" + column_name + "')";

    execute_db_command_by_dbpath(command,db_path, Q_FUNC_INFO);
}

void recon_helper_standard::create_db_index_by_db_reference(QString index_name, QString column_name, QString table_name, QSqlDatabase destination_db, QString caller_func)
{
    QString command = "create index IF NOT EXISTS '" + index_name + "' on '" + table_name + "' ('" + column_name + "')";

    execute_db_command_by_dbreference(command,destination_db, Q_FUNC_INFO);
}

qint64 recon_helper_standard::get_time_zone_offset_without_sign(QString country_timzone_str,QSqlDatabase &destination_db , QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    // Input : Asia/Kolkata    OR     Pacific Standard Time
    // Output : 19800


    QStringList args;
    args << "/usr/share/zoneinfo/" + country_timzone_str;
    bool_process_generic = false;

    QProcess *process_generic = new QProcess(this);
    connect(process_generic,SIGNAL(finished(int)),this,SLOT(slot_generic_process_finished(int)));

    process_generic->start("zdump", args);

    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(bool_process_generic || process_generic->state() == QProcess::NotRunning)
            break;
    }
    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + "  : infinite loop cross", Q_FUNC_INFO);


    // /usr/share/zoneinfo/Asia/Kolkata  Thu Aug 29 22:23:24 2019 IST
    QString command_output_str = QString::fromLocal8Bit(process_generic->readAll().trimmed());
    QStringList data_list = command_output_str.split(" ",Qt::SkipEmptyParts);
    if(data_list.size() == 0)
    {
        recon_static_functions::debug_intensive("mac zone info ---FAILED---"+ command_output_str + " Ends " , Q_FUNC_INFO);
        return MACRO_FAIL_VALUE_NUMERIC_int;
    }


    qint64 offset_qint64 = -1;
    QString mac_zone_info = data_list.last();
    if(mac_zone_info == "UTC")
    {
        // note UTC is used here if output is not desired
        // /usr/share/zoneinfo/Pacific/Pacific Standard Time  Thu Aug 29 10:22:54 2019 UTC

        QString select_cmd = "select off_seconds,timezone from timezone where name = ?";
        QStringList argment_values_list;
        argment_values_list << country_timzone_str;
        offset_qint64 = get_string_from_db_with_addbindvalues_by_dbreference(select_cmd,argment_values_list ,0,destination_db,Q_FUNC_INFO).toLongLong();
    }
    else
    {
        // /usr/share/zoneinfo/Pacific/Majuro  Thu Aug 29 22:23:24 2019 +12
        //                 OR
        // /usr/share/zoneinfo/Asia/Kolkata  Thu Aug 29 22:23:24 2019 IST

        QString select_cmd = "select off_seconds,timezone from timezone where name = ? AND (code = ? OR mac_zone_info1 = ?)";
        QStringList argment_values_list;
        argment_values_list << country_timzone_str << mac_zone_info << mac_zone_info;

        offset_qint64 = get_string_from_db_with_addbindvalues_by_dbreference(select_cmd,argment_values_list ,0,destination_db,Q_FUNC_INFO).toLongLong();
    }



    recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Ends " , Q_FUNC_INFO);

    return offset_qint64;


}

QStringList recon_helper_standard::get_tablist_of_plugin_from_case_configuration_by_dbreference(QSqlDatabase &destination_db,QString plugin_name, QString caller_func)
{
    QString command = "select INT from 'plugins' where name = ?";
    QStringList list1;
    list1 << plugin_name;
    QString cat_INT = get_string_from_db_with_addbindvalues_by_dbreference(command,list1,0,destination_db,Q_FUNC_INFO);

    QString command_1 = "select name from 'plugin_tabs' where plugin_no = ?";
    QStringList list2;
    list2 << cat_INT;
    QStringList tab_list = get_stringlist_from_db_with_addbindvalues_by_dbreference(command_1,list2,0,destination_db,Q_FUNC_INFO);

    return tab_list;
}

QStringList recon_helper_standard::get_plist_displayname_list_from_result_db(QString result_path, QString caller_func)
{
    QString command = "select viewer_display_name from 'plist_viewer_index'";
    QStringList list = get_stringlist_from_db_by_dbpath(command,0,result_path,Q_FUNC_INFO);
    return list;
}

QStringList recon_helper_standard::get_log_displayname_list_from_result_db(QString result_path, QString caller_func)
{
    QString select_command = "select viewer_display_name from 'log_viewer_index'";
    QStringList list = get_stringlist_from_db_by_dbpath(select_command,0,result_path,Q_FUNC_INFO);
    return list;
}

QStringList recon_helper_standard::extract_all_tags_from_db(int extraction_type, QStringList tables_list, QString destination_db_file, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QStringList final_tags;
    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening ---FAILED--- " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);
        return final_tags;
    }

    if(extraction_type == 0)
        tables_list = destination_db->tables();

    tables_list.removeAll("exportfilelist");

    for(int pp = 0; pp < tables_list.size(); pp++)
    {
        QString command = QString( "select recon_tag_value from '" + tables_list.at(pp) + "' where recon_tag_value is not null and recon_tag_value is not ''");
        QStringList received_value = get_stringlist_from_db_by_dbreference(command, 0, *destination_db, Q_FUNC_INFO);

        command = QString( "select count(*) from '" + tables_list.at(pp) + "' where bookmark = '1'");
        QString bk_val = get_string_from_db_by_dbreference(command, 0, *destination_db, Q_FUNC_INFO);
        if(bk_val.trimmed().toInt() >= 1)
        {
            received_value << MACRO_Tag_Name_Bookmarks;
        }

        received_value.removeAll("");
        received_value.removeDuplicates();

        for(int i = 0; i < received_value.size(); i++)
        {
            QString tmp_list = received_value.at(i);
            if(tmp_list.contains(","))
                final_tags <<  tmp_list.split(",", Qt::SkipEmptyParts);
            else
                final_tags << tmp_list;
        }
    }


    final_tags.removeAll("");
    final_tags.removeDuplicates();
    final_tags.sort();

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end ==== " + final_tags.join("----"), Q_FUNC_INFO);

    return final_tags;

}

QString recon_helper_standard::get_apple_metadata_key_value_from_result(QString record_no, QString table_name, QString db_path, QString caller_func)
{

    QString command = "select extended_attributes_key_value from '" + table_name + "' where INT = ?" ;
    QStringList list_bind;
    list_bind << record_no;

    QString key_value_pair = get_string_from_db_with_addbindvalues_by_dbpath(command,list_bind,0,db_path,Q_FUNC_INFO);

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
                            key_value_data += "<br>";
                            xml.readNext();
                            continue;
                        }

                        QString key_name = xml.name().toString();
                        key_name.replace("_"," ");

                        QString value = xml.readElementText();
                        value.replace("\n","");

                        key_value_data += "<b>";
                        key_value_data += key_name;
                        key_value_data += ": </b>";
                        key_value_data += value;
                        key_value_data += "<br>";

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


QString recon_helper_standard::get_hex_viewer_format_for_metadata(QString hex_bytes)
{
    recon_static_functions::app_debug(" Starts" , Q_FUNC_INFO);

    QString final_str_with_bytes_and_ascii;
    final_str_with_bytes_and_ascii += "<br>";
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
            hex_bytes_with_space += QString::fromLocal8Bit(byte_arr.toHex().toUpper()) + QString("\t");
        }

        if(hex_bytes_with_space.size() < 48)
        {
            int diff = (48 - hex_bytes_with_space.size());

            for(int j = 0; j < diff; j++)
            {
                hex_bytes_with_space.append("&nbsp;\t");
            }

        }


        ///in single cell final
        QString final_data_with_html_format = QString("<table><tbody><tr><td><p>") + hex_bytes_with_space + QString("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + ascii_str + QString("</p></td></tr></tbody></table>");

        ///in separete cell issue on metadat widget but when copy and paste on text file it is in correct format
        //QString tag = "<table><tbody><tr><td>" + hex_bytes_with_space + "</td><td><p>"+ QString("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + ascii_str +"</p></td></tr></tbody></table>";

        final_str_with_bytes_and_ascii += final_data_with_html_format;
    }
    recon_static_functions::app_debug(" end" , Q_FUNC_INFO);
    return final_str_with_bytes_and_ascii;
}

bool recon_helper_standard::check_is_it_image_file(QString file_path, QString display_file_path, QString caller_func)
{
    file_path = file_path.trimmed();
    if(file_path.isEmpty())
        return false;

    QFileInfo info(file_path);
    if(!info.exists())
        return false;

    if(check_is_image_by_mimetype(file_path,display_file_path, caller_func))
        return true;

    if(check_is_image_by_extn(QFileInfo(file_path).fileName(),display_file_path, caller_func))
        return true;

    return false;
}

bool recon_helper_standard::check_is_image_by_mimetype(QString filepath, QString display_file_path_str,QString caller_func)
{

    if(display_file_path_str.endsWith(".HEIC", Qt::CaseInsensitive))
        return true;

    QMimeDatabase obj;
    QString mime_type_for_file = obj.mimeTypeForFile(filepath, QMimeDatabase::MatchContent).name();

    if(mime_type_for_file.startsWith("image/"))
        return true;


    return false;
}

bool recon_helper_standard::check_is_image_by_extn(QString filename, QString display_file_path_str,QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    if(filename.endsWith(".") || filename.endsWith("..") || filename.trimmed() == QString(""))
        return false;

    filename = filename.toLower();
    filename = filename.trimmed();

    QString file_name = filename;

    if(!(file_name.contains(".")))
    {
        return false;
    }

    QStringList flist = file_name.split(".");

    if(flist.size() < 2 )
    {
        return false;
    }

    QString file_extension = flist.at(flist.size() - 1);

    QChar ch = file_extension.at(0);
    if(!ch.isDigit() && !ch.isLetter())
        return false;



    QString ext_from_list;

    int list_number = 0;
    int list_index = 0;

    if(ch.isDigit())
    {
        list_number = 1;
        list_index = ch.toLatin1() - '0';

        if(list_index < 0 || list_index > 9)
            return false;
    }
    else
    {
        list_number  = 2;
        list_index = ch.toLatin1() - 'a';

        if(list_index < 0 || list_index > 25)
            return false;

    }

    if(list_number == 1 && list_index >= 0)
    {
        for(int i = 0; i < numeric_image[list_index].size(); i++)
        {
            ext_from_list = numeric_image[list_index].at(i);
            ext_from_list = ext_from_list.remove(".");
            if(file_extension == ext_from_list)
            {
                return true;
            }
        }

    }

    if(list_number == 2 && list_index >= 0)
    {
        for(int i = 0; i < alphabetical_image[list_index].size(); i++)
        {
            ext_from_list = alphabetical_image[list_index].at(i);
            ext_from_list = ext_from_list.remove(".");
            if(file_extension == ext_from_list)
            {
                return true;
            }
        }

    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return false;

}

bool recon_helper_standard::check_is_video_by_extn(QString filename, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    if(filename.endsWith(".") || filename.endsWith(".."))
        return false;

    filename = filename.toLower();
    filename = filename.trimmed();

    QString file_name = filename;

    if(!(file_name.contains(".")))
    {
        return false;
    }

    QStringList flist = file_name.split(".");

    if(flist.size() < 2)
    {
        return false;
    }

    QString file_extension = flist.at(flist.size() - 1);

    QChar ch = file_extension.at(0);

    if(!ch.isDigit() && !ch.isLetter())
        return false;


    QString ext_from_list;

    int list_number = 0;
    int list_index = 0;

    if(ch.isDigit())
    {
        list_number = 1;
        list_index = ch.toLatin1() - '0';

        if(list_index < 0 || list_index > 9)
            return false;

    }
    else
    {
        list_number  = 2;
        list_index = ch.toLatin1() - 'a';

        if(list_index < 0 || list_index > 25)
            return false;

    }

    if(list_number == 1 && list_index >= 0)
    {
        for(int i = 0; i < numeric_vedio[list_index].size(); i++)
        {
            ext_from_list = numeric_vedio[list_index].at(i);
            ext_from_list = ext_from_list.remove(".");

            if(file_extension == ext_from_list)
            {
                return true;
            }
        }

    }

    if(list_number == 2 && list_index >= 0)
    {
        for(int i = 0; i < alphabetical_vedio[list_index].size(); i++)
        {
            ext_from_list = alphabetical_vedio[list_index].at(i);
            ext_from_list = ext_from_list.remove(".");
            if(file_extension == ext_from_list)
            {
                return true;
            }
        }

    }

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return check_is_audio_by_extn(filename, Q_FUNC_INFO);

}


bool recon_helper_standard::check_is_audio_by_extn(QString filename, QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);


    if(filename.endsWith(".") || filename.endsWith(".."))
        return false;

    filename = filename.toLower();
    filename = filename.trimmed();

    QString file_name = filename;

    if(!(file_name.contains(".")))
    {
        return false;
    }

    QStringList flist = file_name.split(".");

    if(flist.size() < 2)
    {
        return false;
    }

    QString file_extension = flist.at(flist.size() - 1);

    QChar ch = file_extension.at(0);

    if(!ch.isDigit() && !ch.isLetter())
        return false;


    QString ext_from_list;

    int list_number = 0;
    int list_index = 0;

    if(ch.isDigit())
    {
        list_number = 1;
        list_index = ch.toLatin1() - '0';

        if(list_index < 0 || list_index > 9)
            return false;

    }
    else
    {
        list_number  = 2;
        list_index = ch.toLatin1() - 'a';

        if(list_index < 0 || list_index > 25)
            return false;

    }

    if(list_number == 1 && list_index >= 0)
    {
        for(int i = 0; i < numeric_audio[list_index].size(); i++)
        {
            ext_from_list = numeric_audio[list_index].at(i);
            ext_from_list = ext_from_list.remove(".");
            if(file_extension == ext_from_list)
            {
                return true;
            }
        }

    }

    if(list_number == 2 && list_index >= 0)
    {
        for(int i = 0; i < alphabetical_audio[list_index].size(); i++)
        {
            ext_from_list = alphabetical_audio[list_index].at(i);
            ext_from_list = ext_from_list.remove(".");
            if(file_extension == ext_from_list)
            {
                return true;
            }
        }

    }


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return false;

}

void recon_helper_standard::initialize_image_extension_list()
{

    numeric_image['0' - '0'] << ".001" ;
    numeric_image['2' - '0'] << ".2bp" ;
    numeric_image['3' - '0'] << ".36";
    numeric_image['4' - '0'] << ".411";
    numeric_image['7' - '0'] << ".73i" ;
    numeric_image['8' - '0'] << ".8pbs" << ".8xi" ;

    alphabetical_image['a' - 'a'] << ".abm" << ".accountpicture-ms" << ".acorn" << ".acr" << ".adc" << ".afx" << ".agif" << ".agp" << ".aic" << ".ais" << ".albm" << ".apd" << ".apm" << ".apng" << ".aps" << ".apx" << ".arr" << ".art" << ".artwork" << ".arw" << ".asw" << ".avatar" << ".awd" << ".awd"     << ".abc" << ".ac5" << ".ac6" << ".af2" << ".af3" << ".ai" << ".art" << ".artb" << ".asy" << ".awg";
    alphabetical_image['b' - 'a'] << ".blkrt" << ".blz" << ".bm2" << ".bmc" << ".bmf" << ".bmp" << ".bmx" << ".bmz" << ".brk" << ".brn" << ".brt" << ".bss" << ".bti";
    alphabetical_image['c' - 'a']  << ".cag" << ".ccx" << ".cdd" << ".cdmm" << ".cdmt" << ".cdmtz" << ".cdmz" << ".cdr" << ".cdt" << ".cdx" << ".cdx" << ".cgm" << ".cil" << ".clarify" << ".cmx" << ".cnv" << ".cor" << ".csy" << ".cv5" << ".cvg" << ".cvi" << ".cvs" << ".cvx" << ".cxf"      << ".c4" << ".cal" << ".cals" << ".cam" << ".can" << ".cd5" << ".cdc" << ".cdg" << ".ce" << ".cimg" << ".cin" << ".cit" << ".colz" << ".cpbitmap" << ".cpc" << ".cpd" << ".cpg" << ".cps" << ".cpt" << ".cpx" << ".csf" << ".ct" << ".cut" ;
    alphabetical_image['d' - 'a']  << ".dcs" << ".ddrw" << ".ded" << ".design" << ".dhs" << ".dia" << ".dpp" << ".dpr" << ".dpx" << ".drawing" << ".drawit" << ".drw" << ".drw" << ".dsf" << ".dxb"      << ".dc2" << ".dcm" << ".dcx" << ".ddb" << ".dds" << ".ddt" << ".dgt" << ".dib" << ".dicom" << ".djv" << ".djvu" << ".dm3" << ".dmi" << ".dpx" << ".drz" << ".dt2" << ".dtw" << ".dvl" ;
    alphabetical_image['e' - 'a'] << ".ecw" << ".epp" << ".exr"  << ".emf" << ".emz" << ".ep" << ".eps" << ".epsf" << ".ezdraw";
    alphabetical_image['f' - 'a'] << ".fac" << ".face" << ".fal" << ".fax" << ".fbm" << ".fil" << ".fits" << ".fpg" << ".fpos" << ".fpx" << ".frm"     << ".fh10" << ".fh11" << ".fh7" << ".fh8" << ".fh9" << ".fhd" << ".fif" << ".fig" << ".fmv" << ".fs" << ".ft10" << ".ft11" << ".ft7" << ".ft8" << ".ft9" << ".ftn" << ".fxg";
    alphabetical_image['g' - 'a'] << ".g3" << ".gbr" << ".gcdp" << ".gfb" << ".gfie" << ".ggr" << ".gif" << ".gih" << ".gim" << ".gmbck" << ".gmspr" << ".gp4" << ".gpd" << ".gro" << ".grob" << ".gry"     << ".gdraw" << ".gem" << ".gks" << ".glox" << ".gls" << ".graffle" << ".gsd" << ".gstencil" << ".gtemplate";
    alphabetical_image['h' - 'a'] << ".hdp" << ".hdr" << ".hdrp" << ".hf" << ".hpi" << ".hr" << ".hrf"     << ".hgl" << ".hpg" << ".hpgl" << ".hpl" ;
    alphabetical_image['i' - 'a'] << ".i3d" << ".ic1" << ".ic2" << ".ic3" << ".ica" << ".icb" << ".icn" << ".icon" << ".icpr" << ".ilbm" << ".img" << ".imj" << ".info" << ".ink" << ".int" << ".ipick" << ".ipx" << ".itc2" << ".ithmb" << ".ivr" << ".ivue" << ".iwi"     << ".idea" << ".igt" << ".igx" << ".imd" << ".ink" << ".ink" ;
    alphabetical_image['j' - 'a'] << ".j" << ".j2c" << ".j2k" << ".jas" << ".jb2" << ".jbf" << ".jbig" << ".jbig2" << ".jbmp" << ".jbr" << ".jfi" << ".jfif" << ".jia" << ".jif" << ".jiff" << ".jng" << ".jp2" << ".jpc" << ".jpd" << ".jpe" << ".jpeg" << ".jpf" << ".jpg" << ".jpg2" << ".jps" << ".jpx" << ".jtf" << ".jwl" << ".jxr" ;
    alphabetical_image['k' - 'a'] << ".kdi" << ".kdk" << ".kfx" << ".kic" << ".kodak" << ".kpg";
    alphabetical_image['l' - 'a'] << ".lbm" << ".lif" << ".ljp"     << ".lmk" ;
    alphabetical_image['m' - 'a'] << ".mac" << ".mat" << ".max" << ".mbm" << ".mbm" << ".mcs" << ".met" << ".mic" << ".mip" << ".mix" << ".mng" << ".mnr" << ".mpf" << ".mpo" << ".mrb" << ".mrxs" << ".msk" << ".msp" << ".mxi" << ".myl"     << ".mgc" << ".mgcb" << ".mgmf" << ".mgmt" << ".mgmx" << ".mgs" << ".mgtx" << ".mmat" << ".mp" ;
    alphabetical_image['n' - 'a'] << ".ncd" << ".ncr" << ".nct" << ".neo" << ".nlm"     << ".nap" ;
    alphabetical_image['o' - 'a'] << ".oc3" << ".oc4" << ".oc5" << ".oci" << ".odi" << ".omf" << ".oplc" << ".ora" << ".ota" << ".otb" << ".oti"     << ".odg" << ".otg" << ".ovp" << ".ovr" ;
    alphabetical_image['p' - 'a'] << ".pac" << ".pal" << ".pap" << ".pat" << ".pbm" << ".pc1" << ".pc2" << ".pc3" << ".pcd" << ".pcx" << ".pdd" << ".pdn" << ".pe4" << ".pe4" << ".pfi" << ".pfr" << ".pgf" << ".pgm" << ".pi1" << ".pi2" << ".pi2" << ".pi3" << ".pi4" << ".pi5" << ".pi6" << ".pic" << ".pic" << ".pic" << ".picnc" << ".pict" << ".pictclipping" << ".pix" << ".pix" << ".pixadex" << ".pjpeg" << ".pjpg" << ".pm" << ".pm3" << ".pmg" << ".png" << ".pni" << ".pnm" << ".pns" << ".pnt" << ".pntg" << ".pop" << ".pov" << ".pov" << ".pp4" << ".pp5" << ".ppf" << ".ppm" << ".prw" << ".psb" << ".psd" << ".psdx" << ".pse" << ".psf" << ".psp" << ".pspbrush" << ".pspimage" << ".ptg" << ".ptk" << ".ptx" << ".ptx" << ".pvr" << ".pwp" << ".px" << ".pxd" << ".pxicon" << ".pxm" << ".pxr" << ".pzp"     << ".pat" << ".pcs" << ".pd" << ".pen" << ".pfd" << ".pl" << ".plt" << ".plt" << ".pmg" << ".pobj" << ".ps" << ".psid" << ".pws";
    alphabetical_image['q' - 'a'] << ".qif" << ".qmg" << ".qti" << ".qtif" ;
    alphabetical_image['r' - 'a'] << ".ras" << ".rcl" << ".rcu" << ".rgb" << ".rgb" << ".ric" << ".rif" << ".riff" << ".rix" << ".rle" << ".rli" << ".rpf" << ".rri" << ".rs" << ".rsb" << ".rsr"   << ".rdl";
    alphabetical_image['s' - 'a'] << ".s2mv" << ".sai" << ".sar" << ".sbp" << ".scg" << ".sci" << ".scn" << ".scp" << ".sct" << ".scu" << ".sdr" << ".sep" << ".sfc" << ".sff" << ".sfw" << ".sgi" << ".shg" << ".sid" << ".sig" << ".sim" << ".skitch" << ".skm" << ".skypeemoticonset" << ".sld" << ".smp" << ".sob" << ".spa" << ".spc" << ".spe" << ".sph" << ".spiff" << ".spj" << ".spp" << ".spr" << ".sprite" << ".spu" << ".sr" << ".ste" << ".sumo" << ".sun" << ".suniff" << ".sup" << ".sva"     << ".scv" << ".sda" << ".sk1" << ".sk2" << ".sketch" << ".slddrt" << ".smf" << ".std" << ".svf" << ".svg" << ".svgz" << ".sxd";
    alphabetical_image['t' - 'a'] << ".t2b" << ".taac" << ".tb0" << ".tbn" << ".tex" << ".tfc" << ".tg4" << ".tga" << ".thm" << ".thm" << ".thumb" << ".tif" << ".tif" << ".tiff" << ".tjp" << ".tm2" << ".tn" << ".tn1" << ".tn2" << ".tn3" << ".tny" << ".tpf" << ".tpi" << ".tps" << ".trif" << ".tub"     << ".tlc" << ".tne" << ".tpl" ;
    alphabetical_image['u' - 'a']<< ".u" << ".ufo" << ".urt" << ".usertile-ms"     << ".ufr" ;
    alphabetical_image['v' - 'a']<< ".v" << ".vda" << ".vff" << ".vic" << ".viff" << ".vna" << ".vpe" << ".vrphoto" << ".vss" << ".vst"     << ".vbr" << ".vec" << ".vml" << ".vsd" << ".vsdm" << ".vsdx" << ".vst" << ".vstm" << ".vstx" ;
    alphabetical_image['w' - 'a'] << ".wb1" << ".wbc" << ".wbd" << ".wbm" << ".wbmp" << ".wbz" << ".wdp" << ".webp" << ".wi" << ".wic" << ".wmp" << ".wpb" << ".wpe" << ".wvl"     << ".wmf" << ".wmz" << ".wpg" ;
    alphabetical_image['x' - 'a'] << ".xbm" << ".xcf" << ".xpm" << ".xwd"     << ".xar" << ".xmind" << ".xmmap" << ".xpr" ;
    alphabetical_image['y' - 'a'] << ".y" << ".ysp" << ".yuv"  << ".yal" ;
    alphabetical_image['z' - 'a'] << ".zif"     << ".zgm";


}


void recon_helper_standard::initialize_vedio_extension_list()
{

    numeric_vedio['2' - '0'] << ".264";
    numeric_vedio['3' - '0'] <<".3g2" <<".3gp" <<".3gp2" <<".3gpp" <<".3gpp2" <<".3mm" <<".3p2";
    numeric_vedio['6' - '0'] <<".60d";
    numeric_vedio['7' - '0']  <<".787";


    alphabetical_vedio['a' - 'a'] <<".aaf" <<".aec" <<".aep" <<".aepx" <<".aet" <<".aetx" <<".ajp" <<".ale" <<".am" <<".amc" <<".amv" <<".amx" <<".anim" <<".aqt" <<".arcut" <<".arf" <<".asf" <<".asx" <<".avb" <<".avc" <<".avd" <<".avi" <<".avp" <<".avs" <<".avs" <<".avv" <<".axm";
    alphabetical_vedio['b' - 'a'] <<".bdm" <<".bdmv" <<".bdt2" <<".bdt3" <<".bik" <<".bin" <<".bix" <<".bmc" <<".bmk" <<".bnp" <<".box" <<".bs4" <<".bsf" <<".bvr" <<".byu";
    alphabetical_vedio['c' - 'a'] <<".camproj" <<".camrec" <<".camv" <<".ced" <<".cel" <<".cine" <<".cip" <<".clpi" <<".cmmp" <<".cmmtpl" <<".cmproj" <<".cmrec" <<".cpi" <<".cst" <<".cvc" <<".cx3";
    alphabetical_vedio['d' - 'a'] <<".d2v" <<".d3v" <<".dat" <<".dav" <<".dce" <<".dck" <<".dcr" <<".dcr" <<".ddat" <<".dif" <<".dir" <<".divx" <<".dlx" <<".dmb" <<".dmsd" <<".dmsd3d" <<".dmsm" <<".dmsm3d" <<".dmss" <<".dmx" <<".dnc" <<".dpa" <<".dpg" <<".dream" <<".dsy" <<".dv" <<".dv-avi" <<".dv4" <<".dvdmedia" <<".dvr" <<".dvr-ms" <<".dvx" <<".dxr" <<".dzm" <<".dzp" <<".dzt";
    alphabetical_vedio['e' - 'a'] <<".edl" <<".evo" <<".eye" <<".ezt";
    alphabetical_vedio['f' - 'a'] <<".f4f" <<".f4p" <<".f4v" <<".fbr" <<".fbr" <<".fbz" <<".fcp" <<".fcproject" <<".ffd" <<".flc" <<".flh" <<".fli" <<".flv" <<".flx" <<".ftc";
    alphabetical_vedio['g' - 'a'] <<".gfp" <<".gl" <<".gom" <<".grasp" <<".gts" <<".gvi" <<".gvp";
    alphabetical_vedio['h' - 'a'] <<".h264" <<".hdmov" <<".hkm";
    alphabetical_vedio['m' - 'a'] <<".mov" <<".mkv" <<".mp4";

}

void recon_helper_standard::initialize_audio_extension_list()
{
    numeric_audio['2' - '0'] << ".2sf" << ".2sflib" ;
    numeric_audio['3' - '0'] << ".3ga";
    numeric_audio['4' - '0'] << ".4mp";
    numeric_audio['6' - '0'] << ".669" << ".6cm";
    numeric_audio['8' - '0'] << ".8cm" << ".8med" << ".8svx";


    alphabetical_audio['a' - 'a'] << ".a2m" << ".a52" << ".aa" << ".aa3" << ".aac" << ".aax" << ".ab" << ".abc" << ".abm" << ".ac3" << ".acd" << ".acd-bak" << ".acd-zip" << ".acm" << ".acp" << ".act" << ".adg" << ".adt" << ".adts" << ".adv" << ".afc" << ".agm" << ".agr" << ".ahx" << ".aif" << ".aifc" << ".aiff" << ".aimppl" << ".ais" << ".akp" << ".al" << ".alac" << ".alaw" << ".alc" << ".all" << ".als" << ".amf" << ".amr" << ".ams" << ".ams" << ".amxd" << ".amz" << ".aob" << ".ape" << ".apf" << ".apl" << ".aria" << ".ariax" << ".asd" << ".ase" << ".at3" << ".atrac" << ".au" << ".au" << ".aud" << ".aup" << ".avastsounds" << ".avr" << ".awb" << ".ay";
    alphabetical_audio['b' - 'a'] << ".b4s" << ".band" << ".bap" << ".bcs" << ".bdd" << ".bidule" << ".bonk" << ".box" << ".brstm" << ".bun" << ".bwf" << ".bwg" << ".bww";
    alphabetical_audio['c' - 'a'] << ".c01" << ".caf" << ".caff" << ".cda" << ".cdda" << ".cdlx" << ".cdo" << ".cdr" << ".cel" << ".cfa" << ".cfxr" << ".cgrp" << ".cidb" << ".ckb" << ".ckf" << ".cmf" << ".conform" << ".copy" << ".cpr" << ".cpt" << ".csh" << ".cts" << ".cwb" << ".cwp" ;
    alphabetical_audio['d' - 'a'] << ".d00" << ".d01" << ".dcf" << ".dcm" << ".dct" << ".ddt" << ".dewf" << ".df2" << ".dfc" << ".dff" << ".dig" << ".dig" << ".djr" << ".dls" << ".dm" << ".dmc" << ".dmf" << ".dmsa" << ".dmse" << ".dra" << ".drg" << ".ds" << ".ds2" << ".dsf" << ".dsm" << ".dsp" << ".dss" << ".dtm" << ".dts" << ".dtshd" << ".dvf" << ".dwa" << ".dwd" ;
    alphabetical_audio['e' - 'a'] << ".ear" << ".efa" << ".efe" << ".efk" << ".efq" << ".efs" << ".efv" << ".emd" << ".emp" << ".emx" << ".emy" << ".esps" << ".evr" << ".expressionmap" ;
    alphabetical_audio['f' - 'a'] << ".f2r" << ".f32" << ".f3r" << ".f4a" << ".f64" << ".far" << ".fda" << ".fdp" << ".fev" << ".fff" << ".flac" << ".flp" << ".fls" << ".frg" << ".fsb" << ".fsm" << ".ftm" << ".fzb" << ".fzf" << ".fzv";
    alphabetical_audio['g' - 'a'] << ".g721" << ".g723" << ".g726" << ".gbproj" << ".gbs" << ".gig" << ".gio" << ".gio" << ".gm" << ".gp5" << ".gpbank" << ".gpk" << ".gpx" << ".gro" << ".groove" << ".gsm" << ".gsm" ;
    alphabetical_audio['h' - 'a'] << ".h0" << ".hdp" << ".hma" << ".hmi" << ".hsb";
    alphabetical_audio['i' - 'a'] << ".iaa" << ".ics" << ".iff" << ".igp" << ".igr" << ".imf" << ".imp" << ".ins" << ".ins" << ".isma" << ".it" << ".iti" << ".itls" << ".its" ;
    alphabetical_audio['j' - 'a'] << ".jam" << ".jam" << ".jo" << ".jo-7z" ;
    alphabetical_audio['k' - 'a'] << ".k25" << ".k26" << ".kar" << ".kfn" << ".kin" << ".kit" << ".kmp" << ".koz" << ".koz" << ".kpl" << ".krz" << ".ksc" << ".ksf" << ".ksm" << ".kt2" << ".kt3" << ".ktp" ;
    alphabetical_audio['l' - 'a'] << ".l" << ".la" << ".lof" << ".logic" << ".lqt" << ".lso" << ".lvp" << ".lwv" ;
    alphabetical_audio['m' - 'a'] << ".m1a" << ".m3u" << ".m3u8" << ".m4a" << ".m4b" << ".m4p" << ".m4r" << ".ma1" << ".mbr" << ".mdl" << ".med" << ".mgv" << ".mid" << ".midi" << ".mini2sf" << ".minincsf" << ".miniusf" << ".mka" << ".mlp" << ".mmf" << ".mmm" << ".mmp" << ".mo3" << ".mod" << ".mogg" << ".mp1" << ".mp2" << ".mp3" << ".mp_" << ".mpa" << ".mpc" << ".mpdp" << ".mpga" << ".mpu" << ".mscx" << ".mscz" << ".msv" << ".mt2" << ".mt9" << ".mte" << ".mtf" << ".mti" << ".mtm" << ".mtp" << ".mts" << ".mu3" << ".mui" << ".mus" << ".mus" << ".mus" << ".musa" << ".mux" << ".mux" << ".muz" << ".mws" << ".mx3" << ".mx4" << ".mx5" << ".mx5template" << ".mxl" << ".mxmf" << ".myr" << ".mzp" ;
    alphabetical_audio['n' - 'a'] << ".nap" << ".nbs" << ".ncw" << ".nkb" << ".nkc" << ".nki" << ".nkm" << ".nks" << ".nkx" << ".nml" << ".note" << ".npl" << ".nra" << ".nrt" << ".nsa" << ".nsf" << ".nst" << ".ntn" << ".nvf" << ".nwc" ;
    alphabetical_audio['o' - 'a'] << ".obw" << ".odm" << ".ofr" << ".oga" << ".ogg" << ".okt" << ".oma" << ".omf" << ".omg" << ".omx" << ".opus" << ".orc" << ".ots" << ".ove" << ".ovw" << ".ovw" ;
    alphabetical_audio['p' - 'a'] << ".pac" << ".pat" << ".pbf" << ".pca" << ".pcast" << ".pcg" << ".pcm" << ".pd" << ".peak" << ".pek" << ".pho" << ".phy" << ".pk" << ".pkf" << ".pla" << ".pls" << ".plst" << ".ply" << ".pna" << ".pno" << ".ppc" << ".ppcx" << ".prg" << ".prg" << ".psf" << ".psm" << ".psy" << ".ptf" << ".ptm" << ".pts" << ".ptx" << ".pvc";
    alphabetical_audio['q' - 'a'] << ".qcp" ;
    alphabetical_audio['r' - 'a'] << ".r" << ".r1m" << ".ra" << ".ram" << ".raw" << ".rax" << ".rbs" << ".rbs" << ".rcy" << ".record" << ".rex" << ".rfl" << ".rgrp" << ".rip" << ".rmf" << ".rmi" << ".rmj" << ".rmm" << ".rmx" << ".rng" << ".rns" << ".rol" << ".rsn" << ".rso" << ".rta" << ".rti" << ".rtm" << ".rts" << ".rvx" << ".rx2";
    alphabetical_audio['s' - 'a'] << ".s3i" << ".s3m" << ".s3z" << ".saf" << ".sam" << ".sap" << ".sb" << ".sbg" << ".sbi" << ".sbk" << ".sc2" << ".scs11" << ".sd" << ".sd" << ".sd2" << ".sd2f" << ".sdat" << ".sdii" << ".sds" << ".sdt" << ".sdx" << ".seg" << ".seq" << ".ses" << ".sesx" << ".sf" << ".sf2" << ".sfap0" << ".sfk"  << ".sfpack" << ".sfs" << ".sgp" << ".shn" << ".sib" << ".sid" << ".slp" << ".slx" << ".sma" << ".smf" << ".smp" << ".smp" << ".snd" << ".snd" << ".snd" << ".sng" << ".sng" << ".sns" << ".sou" << ".sph" << ".sppack" << ".sprg" << ".spx" << ".sseq" << ".sseq" << ".ssnd" << ".stap" << ".stm" << ".stx" << ".sty" << ".sty" << ".svd" << ".svx" << ".sw" << ".swa" << ".swav" << ".sxt" << ".syh" << ".syn" << ".syn" << ".syw" << ".syx" ;
    alphabetical_audio['t' - 'a'] << ".tak" << ".tak" << ".td0" << ".tfmx" << ".tg" << ".thx" << ".tm2" << ".tm8" << ".tmc" << ".toc" << ".trak" << ".tsp" << ".tta" << ".tun" << ".txw";
    alphabetical_audio['u' - 'a'] << ".u" << ".u8" << ".uax" << ".ub" << ".ulaw" << ".ult" << ".ulw" << ".uni" << ".usf" << ".usflib" << ".ust" << ".uw" << ".uwf" ;
    alphabetical_audio['v' - 'a'] << ".v2m" << ".vag" << ".val" << ".vap" << ".vb" << ".vc3" << ".vdj" << ".vgm" << ".vgz" << ".vlc" << ".vmd" << ".vmf" << ".vmf" << ".vmo" << ".voc" << ".voi" << ".vox" << ".voxal" << ".vpl" << ".vpm" << ".vqf" << ".vrf" << ".vsq" << ".vtx" << ".vyf";
    alphabetical_audio['w' - 'a'] << ".w01" << ".w64" << ".wav" << ".wav" << ".wave" << ".wax" << ".wem" << ".wfb" << ".wfd" << ".wfm" << ".wfp" << ".wma" << ".wow" << ".wpk" << ".wpp" << ".wproj" << ".wrk" << ".wtpl" << ".wtpt" << ".wus" << ".wut" << ".wv" << ".wvc" << ".wve" << ".wwu" << ".wyz" ;
    alphabetical_audio['x' - 'a'] << ".xa" << ".xa" << ".xfs" << ".xi" << ".xm" << ".xmf" << ".xmi" << ".xmz" << ".xp" << ".xrns" << ".xsb" << ".xsp" << ".xspf" << ".xt" << ".xwb";
    alphabetical_audio['y' - 'a'] << ".ym" << ".yookoo" ;
    alphabetical_audio['z' - 'a'] << ".zpa" << ".zpl" << ".zvd" << ".zvr" ;
}

QString recon_helper_standard::mining_suffix(QString path, QString caller_func)
{

    //================================================//
    QString file_nm_1 = get_filename_by_filepath_without_fileinfo(path);
    if(file_nm_1.startsWith("."))
        file_nm_1.remove(0, 1);

    QString m_suffix;
    int indx = file_nm_1.lastIndexOf(".");
    if(indx != -1)
    {
        m_suffix = file_nm_1.remove(0, (indx + 1));
    }
    //================================================//

    return m_suffix;

    //    QString file_path = path;
    //    file_path = file_path.remove(0,file_path.lastIndexOf("/"));

    //    QString extension;
    //    if(file_path.contains("."))
    //        extension = file_path.remove(0,(file_path.lastIndexOf(".") + 1));
    //    else
    //        extension = "";

    //    return extension;

}

void recon_helper_standard::translate_tablewidget_horizontal_header_item_all(QTableWidget *tbl_wdgt, QString caller_func)
{
    int col_count = tbl_wdgt->columnCount();
    for(int ii = 0; ii < col_count; ii++)
    {
        tbl_wdgt->horizontalHeaderItem(ii)->setText(global_lang_tr_obj.get_translated_string(tbl_wdgt->horizontalHeaderItem(ii)->text()));
    }

}

QString recon_helper_standard::run_command_with_arguments_process_obj(QProcess *m_obj, QString command, QStringList arguments, QString caller_func)
{
    m_obj->start(command,arguments);
    if (!m_obj->waitForStarted())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "Error:" + m_obj->errorString() ,Q_FUNC_INFO);
        return "";
    }

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }
        if(m_obj->state() == QProcess::NotRunning)
        {
            break;
        }

        m_obj->waitForReadyRead();

    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);

    return QString::fromLocal8Bit(m_obj->readAll().trimmed());
}

void recon_helper_standard::pub_add_file_details_to_hex_feature_db_for_sendto_bucket(struct_global_hex_viewer_open_file_data hex_viewer_open_file_data_obj, QString caller_func)
{
    ///add tag details to feature DB
    QString hex_feature_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Hex_viewer_In_Result_QString).toString() + QString("hex_viewer.sqlite");

    //check if record already exist
    QStringList vals_list;
    vals_list << hex_viewer_open_file_data_obj.plugin_name_str
              << hex_viewer_open_file_data_obj.record_no_str
              << hex_viewer_open_file_data_obj.source_count_name;

    QString command_str = QString("Select viewer_display_name from hex_viewer_index Where plugin_name = ? AND record_no = ? AND source_count_name = ?");
    QString viewer_display_name = get_string_from_db_with_addbindvalues_by_dbpath(command_str,vals_list,0, hex_feature_db_path, caller_func);
    viewer_display_name = viewer_display_name.trimmed();
    if(viewer_display_name.isEmpty())
    {
        QString db_table_name = hex_viewer_open_file_data_obj.hex_viewer_display_name;
        db_table_name.replace(" ", "_");
        db_table_name.replace(".", "_");

        vals_list.clear();
        vals_list << hex_viewer_open_file_data_obj.hex_viewer_display_name
                  << db_table_name
                  << get_filename_by_filepath_without_fileinfo(hex_viewer_open_file_data_obj.display_file_path)
                  << hex_viewer_open_file_data_obj.display_file_path
                  << hex_viewer_open_file_data_obj.plugin_name_str
                  << hex_viewer_open_file_data_obj.plugin_tab_name_str
                  << hex_viewer_open_file_data_obj.record_no_str
                  << QString::number(QDateTime::currentDateTimeUtc().toSecsSinceEpoch())
                  << hex_viewer_open_file_data_obj.source_count_name
                  << hex_viewer_open_file_data_obj.recon_filefrom
                  << hex_viewer_open_file_data_obj.recon_file_infopath;

        command_str = QString("Insert into hex_viewer_index (viewer_display_name,db_table_name,file_name,file_path,plugin_name,category_name,record_no,creation_timestamp,"
                              "source_count_name,recon_filefrom,recon_file_infopath) Values(?,?,?,?,?, ?,?,?,?,?, ?)");


        execute_db_command_with_addbindvalues_by_dbpath(command_str, vals_list, hex_feature_db_path, caller_func);
    }
}

bool recon_helper_standard::check_record_is_decompressed_by_dbpath(QString record_no , QString db_path, QString caller_func)
{
    recon_static_functions::debug_intensive("start ", recon_static_functions::prepare_callerfun(caller_func));

    if(record_no.trimmed().isEmpty())
        return false;

    QString file_from_str = get_string_from_db_with_addbindvalues_by_dbpath(QString("Select recon_filefrom from files Where INT = ?"), QStringList(record_no),0,db_path,Q_FUNC_INFO);
    file_from_str = file_from_str.trimmed();
    if(file_from_str.isEmpty())
        return false;


    int file_from_num = file_from_str.toInt();
    if(file_from_num == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result)
    {
        recon_static_functions::debug_intensive("end ", recon_static_functions::prepare_callerfun(caller_func));
        return true;
    }

    recon_static_functions::debug_intensive("end ", recon_static_functions::prepare_callerfun(caller_func));
    return false;
}

bool recon_helper_standard::check_record_is_decompressed_by_dbreference(QString record_no , QSqlDatabase &destination_db, QString caller_func)
{
    recon_static_functions::debug_intensive("start ", recon_static_functions::prepare_callerfun(caller_func));

    if(record_no.trimmed().isEmpty())
        return false;

    QString file_from_str = get_string_from_db_with_addbindvalues_by_dbreference(QString("Select recon_filefrom from files Where INT = ?"), QStringList(record_no),0,destination_db,Q_FUNC_INFO);
    file_from_str = file_from_str.trimmed();
    if(file_from_str.isEmpty())
        return false;


    int file_from_num = file_from_str.toInt();
    if(file_from_num == enum_GLOBAL_COMMON_RECON_AND_ARTIFACT_FilePath_StartFrom_Result)
    {
        recon_static_functions::debug_intensive("end ", recon_static_functions::prepare_callerfun(caller_func));
        return true;
    }

    recon_static_functions::debug_intensive("end ", recon_static_functions::prepare_callerfun(caller_func));
    return false;
}

void recon_helper_standard::pub_create_disabled_empty_action_in_menu(QList<QMenu*> menu_list, QString caller_func)
{
    recon_static_functions::app_debug("start ", recon_static_functions::prepare_callerfun(caller_func));

    for(int count = 0; count < menu_list.size(); count++)
    {
        QMenu *menu_main = menu_list.at(count);

        QAction *act_empty = new QAction(QString(MACRO_Generic_Right_Click_Empty), this);
        act_empty->setEnabled(false);

        menu_main->addAction(act_empty);
    }

    recon_static_functions::app_debug("end ", recon_static_functions::prepare_callerfun(caller_func));
}

QString recon_helper_standard::pub_get_html_formated_filename(QString regular_name_str, QString caller_func)
{
    QString html_formated_name_str = regular_name_str.trimmed();

    //keep it at first always otherwise it will replace % again
    //html_formated_name_str.replace("%", "%25");

    html_formated_name_str.replace(" ","%20");
    html_formated_name_str.replace("!","%21");
    html_formated_name_str.replace("\"","%22");
    html_formated_name_str.replace("#","%23");
    html_formated_name_str.replace("$","%24");
    html_formated_name_str.replace("&","%26");
    html_formated_name_str.replace("'","%27");
    html_formated_name_str.replace("(","%28");
    html_formated_name_str.replace(")","%29");
    html_formated_name_str.replace("*","%2A");
    html_formated_name_str.replace("+","%2B");
    html_formated_name_str.replace("-","%2D");
    html_formated_name_str.replace(":","%3A");
    html_formated_name_str.replace(";","%3B");
    html_formated_name_str.replace("<","%3C");
    html_formated_name_str.replace("=","%3D");
    html_formated_name_str.replace(">","%3E");
    html_formated_name_str.replace("?","%3F");
    html_formated_name_str.replace("@","%40");
    html_formated_name_str.replace("{","%7B");
    html_formated_name_str.replace("|","%7C");
    html_formated_name_str.replace("}","%7D");
    html_formated_name_str.replace("\\","%5C");



    //    html_formated_name_str.replace("[","%5B");
    //    html_formated_name_str.replace("\"","%5C");
    //    html_formated_name_str.replace("]","%5D");
    //    html_formated_name_str.replace("^","%5E");

    //html_formated_name_str.replace("ƒ", "%83");
    //html_formated_name_str.replace("„", "%84");
    //html_formated_name_str.replace("…", "%85");
    //html_formated_name_str.replace("†", "%86");
    //html_formated_name_str.replace("‡", "%87");
    //html_formated_name_str.replace("ˆ", "%88");
    //html_formated_name_str.replace("‰", "%89");
    //html_formated_name_str.replace("Š", "%8A");
    //html_formated_name_str.replace("‹", "%8B");
    //html_formated_name_str.replace("Œ", "%8C");
    //html_formated_name_str.replace("Ž", "%8E");
    //html_formated_name_str.replace("‘", "%91");
    //html_formated_name_str.replace("’", "%92");
    //html_formated_name_str.replace("“", "%93");
    //html_formated_name_str.replace("”", "%94");
    //html_formated_name_str.replace("•", "%95");
    //html_formated_name_str.replace("–", "%96");
    //html_formated_name_str.replace("—", "%97");
    //html_formated_name_str.replace("˜", "%98");
    // html_formated_name_str.replace("™", "%99");
    //html_formated_name_str.replace("š", "%9A");
    //html_formated_name_str.replace("›", "%9B");
    // html_formated_name_str.replace("œ", "%9C");
    // html_formated_name_str.replace("ž", "%9E");
    // html_formated_name_str.replace("Ÿ", "%9F");
    //html_formated_name_str.replace("¡", "%A1");
    //html_formated_name_str.replace("¢", "%A2");
    //html_formated_name_str.replace("£", "%A3");
    //html_formated_name_str.replace("¤", "%A4");
    //html_formated_name_str.replace("¥", "%A5");
    //html_formated_name_str.replace("¦", "%A6");
    //html_formated_name_str.replace("§", "%A7");
    //html_formated_name_str.replace("¨", "%A8");
    //html_formated_name_str.replace("©", "%A9");
    //html_formated_name_str.replace("ª", "%AA");
    //html_formated_name_str.replace("«", "%AB");
    //html_formated_name_str.replace("¬", "%AC");
    //    html_formated_name_str.replace("®", "%AE");
    //    html_formated_name_str.replace("¯", "%AF");
    //    html_formated_name_str.replace("°", "%B0");
    //    html_formated_name_str.replace("±", "%B1");
    //    html_formated_name_str.replace("²", "%B2");
    //    html_formated_name_str.replace("³", "%B3");
    //    html_formated_name_str.replace("´", "%B4");
    //    html_formated_name_str.replace("µ", "%B5");
    //    html_formated_name_str.replace("¶", "%B6");
    //    html_formated_name_str.replace("·", "%B7");
    //    html_formated_name_str.replace("¸", "%B8");
    //    html_formated_name_str.replace("¹", "%B9");
    //    html_formated_name_str.replace("º", "%BA");
    //    html_formated_name_str.replace("»", "%BB");
    //    html_formated_name_str.replace("¼", "%BC");
    //    html_formated_name_str.replace("½", "%BD");
    //    html_formated_name_str.replace("¾", "%BE");
    //    html_formated_name_str.replace("¿", "%BF");
    //    html_formated_name_str.replace("À", "%C0");
    //    html_formated_name_str.replace("Á", "%C1");
    //    html_formated_name_str.replace("Â", "%C2");
    //    html_formated_name_str.replace("Ã", "%C3");
    //    html_formated_name_str.replace("Ä", "%C4");
    //    html_formated_name_str.replace("Å", "%C5");
    //    html_formated_name_str.replace("Æ", "%C6");
    //    html_formated_name_str.replace("Ç", "%C7");
    //    html_formated_name_str.replace("È", "%C8");
    //    html_formated_name_str.replace("É", "%C9");
    //    html_formated_name_str.replace("Ê", "%CA");
    //    html_formated_name_str.replace("Ë", "%CB");
    //    html_formated_name_str.replace("Ì", "%CC");
    //    html_formated_name_str.replace("Í", "%CD");
    //    html_formated_name_str.replace("Î", "%CE");
    //    html_formated_name_str.replace("Ï", "%CF");
    //    html_formated_name_str.replace("Ð", "%D0");
    //    html_formated_name_str.replace("Ñ", "%D1");
    //    html_formated_name_str.replace("Ò", "%D2");
    //    html_formated_name_str.replace("Ó", "%D3");
    //    html_formated_name_str.replace("Ô", "%D4");
    //    html_formated_name_str.replace("Õ", "%D5");
    //    html_formated_name_str.replace("Ö", "%D6");
    //    html_formated_name_str.replace("×", "%D7");
    //    html_formated_name_str.replace("Ø", "%D8");
    //    html_formated_name_str.replace("Ù", "%D9");
    //    html_formated_name_str.replace("Ú", "%DA");
    //    html_formated_name_str.replace("Û", "%DB");
    //    html_formated_name_str.replace("Ü", "%DC");
    //    html_formated_name_str.replace("Ý", "%DD");
    //    html_formated_name_str.replace("Þ", "%DE");
    //    html_formated_name_str.replace("ß", "%DF");
    //    html_formated_name_str.replace("à", "%E0");
    //    html_formated_name_str.replace("á", "%E1");
    //    html_formated_name_str.replace("â", "%E2");
    //    html_formated_name_str.replace("ã", "%E3");
    //    html_formated_name_str.replace("ä", "%E4");
    //    html_formated_name_str.replace("å", "%E5");
    //    html_formated_name_str.replace("æ", "%E6");
    //    html_formated_name_str.replace("ç", "%E7");
    //    html_formated_name_str.replace("è", "%E8");
    //    html_formated_name_str.replace("é", "%E9");
    //    html_formated_name_str.replace("ê", "%EA");
    //    html_formated_name_str.replace("ë", "%EB");
    //    html_formated_name_str.replace("ì", "%EC");
    //    html_formated_name_str.replace("í", "%ED");
    //    html_formated_name_str.replace("î", "%EE");
    //    html_formated_name_str.replace("ï", "%EF");
    //    html_formated_name_str.replace("ð", "%F0");
    //    html_formated_name_str.replace("ñ", "%F1");
    //    html_formated_name_str.replace("ò", "%F2");
    //    html_formated_name_str.replace("ó", "%F3");
    //    html_formated_name_str.replace("ô", "%F4");
    //    html_formated_name_str.replace("õ", "%F5");
    //    html_formated_name_str.replace("ö", "%F6");
    //    html_formated_name_str.replace("÷", "%F7");
    //    html_formated_name_str.replace("ø", "%F8");
    //    html_formated_name_str.replace("ù", "%F9");
    //    html_formated_name_str.replace("ú", "%FA");
    //    html_formated_name_str.replace("û", "%FB");
    //    html_formated_name_str.replace("ü", "%FC");
    //    html_formated_name_str.replace("ý", "%FD");
    //    html_formated_name_str.replace("þ", "%FE");
    //    html_formated_name_str.replace("ÿ", "%FF");
    //    html_formated_name_str.replace("_","%5F");
    //    html_formated_name_str.replace("`","%60");



    return html_formated_name_str;


    //html_formated_name_str.replace("~","%7E");
    //    html_formated_name_str.replace("`", "%80");


}

void recon_helper_standard::slot_generic_process_finished(int)
{
    bool_process_generic = true;
}

bool recon_helper_standard::pub_change_database_mode_to_wal_by_db_path(QString db_path_str, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    bool mstatus = false;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));
    destination_db->setDatabaseName(db_path_str);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + db_path_str,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != nullptr)
            delete destination_db;

        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return mstatus;
    }

    mstatus = pub_change_database_mode_to_wal_by_db_reference(*destination_db, caller_func);

    destination_db->close();
    if(destination_db != nullptr)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return mstatus;

}

bool recon_helper_standard::pub_change_database_mode_to_wal_by_db_reference(QSqlDatabase db_refrnce, QString caller_func)
{
    recon_static_functions::app_debug("Start ", recon_static_functions::prepare_callerfun(caller_func));

    QString export_file_cmd = QString("PRAGMA journal_mode=WAL;");
    bool status = execute_db_command_by_dbreference(export_file_cmd , db_refrnce, caller_func);

    recon_static_functions::app_debug("End ", recon_static_functions::prepare_callerfun(caller_func));

    return status;
}

QStringList recon_helper_standard::pub_get_records_bookmarked_from_table_display_by_db_refrence(QSqlDatabase source_db, QString table_name, QString records_col_str,QString caller_func)
{
    recon_static_functions::app_debug("Start ", recon_static_functions::prepare_callerfun(caller_func));
    QStringList records_list;

    QString command_str = QString("Select ") + records_col_str + QString(" From ") + table_name
            + QString(" Where bookmark='1' AND ") + QString("(bookmarked_extended_attributes IS NULL or bookmarked_extended_attributes is '') AND ")
            + QString("(bookmarked_exif_data IS NULL or bookmarked_exif_data is '')");


    records_list = get_stringlist_from_db_by_dbreference(command_str, 0, source_db, Q_FUNC_INFO);


    recon_static_functions::app_debug("End ", recon_static_functions::prepare_callerfun(caller_func));
    return  records_list;
}

void recon_helper_standard::pub_add_bookmarked_record_to_map_for_detailed_info_bookmark_control(QMap<QString,QStringList> &map_bookmarked_records,QString key_str, QString record_str)
{
    QStringList final_records_list;
    if(map_bookmarked_records.size() > 1)
    {
        bool key_matched = false;
        QMapIterator<QString,QStringList> itr(map_bookmarked_records);
        while (itr.hasNext())
        {
            itr.next();
            if(itr.key() == key_str)
            {
                key_matched = true;
                final_records_list = itr.value();
                break;
            }
        }
        final_records_list << record_str;

        if(key_matched)
        {
            map_bookmarked_records.remove(key_str);
        }

        map_bookmarked_records.insert(key_str, final_records_list);
    }
    else
    {
        final_records_list << record_str;
        map_bookmarked_records.insert(key_str, final_records_list);
    }

}

void recon_helper_standard::pub_remove_bookmarked_record_to_map_for_detailed_info_bookmark_control(QMap<QString,QStringList> &map_bookmarked_records,QString key_str, QString record_str)
{
    if(map_bookmarked_records.size() > 1)
    {
        QStringList final_records_list;
        bool key_matched = false;
        QMapIterator<QString,QStringList> itr(map_bookmarked_records);
        while (itr.hasNext())
        {
            itr.next();
            if(itr.key() == key_str)
            {
                key_matched = true;
                final_records_list = itr.value();
                break;
            }
        }

        qint64 tmp_index = final_records_list.indexOf(record_str);
        if(tmp_index >= 0)
            final_records_list.removeAt(tmp_index);

        map_bookmarked_records.insert(key_str, final_records_list);
    }
}

bool recon_helper_standard::pub_check_bookmarked_record_in_map_for_detailed_info_bookmark_control(QMap<QString,QStringList> &map_bookmarked_records, QString key_str, QString record_str)
{
    bool bool_record_found = false;
    if(map_bookmarked_records.size() > 1)
    {
        QStringList final_records_list;
        QMapIterator<QString,QStringList> itr(map_bookmarked_records);
        while (itr.hasNext())
        {
            itr.next();
            if(itr.key() == key_str)
            {
                final_records_list = itr.value();
                break;
            }
        }

        if(final_records_list.contains(record_str))
            bool_record_found = true;
    }

    return  bool_record_found;
}

struct_global_file_hashes recon_helper_standard::pub_create_md5_and_sha1_hash(QString file_path)
{
    struct_global_file_hashes hashes_obj;

    unsigned char a_md5[MD5_DIGEST_LENGTH], a_sha1[SHA_DIGEST_LENGTH];
    int i, bytes;
    unsigned char data[1024];

    MD5_CTX md_context;
    MD5_Init(&md_context);

    SHA_CTX sha_context;
    SHA1_Init(&sha_context);

    ///open file and read data
    QFile target_file(file_path);
    if(!target_file.open(QIODevice::ReadOnly))
    {
        return hashes_obj;
    }

    qint64 file_total_size = target_file.size();
    qint64 data_read = 0;
    if(file_total_size <= 0)
    {
        return hashes_obj;
    }
    int count = 0;
    while ((bytes = target_file.read((char *)data,1024)) > 0)
    {
        if(count % 100 == 0)
        {
            count = 0;
            QCoreApplication::processEvents();
        }
        MD5_Update(&md_context, data, bytes);
        SHA1_Update(&sha_context, data, bytes);

        data_read += bytes;
        count++;
    }
    target_file.close();

    QByteArray md5_ar;
    MD5_Final(a_md5, &md_context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();
        md5_ar.append(a_md5[i]);
    }

    QByteArray sha1_ar;
    SHA1_Final(a_sha1, &sha_context);
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        if(i % 50 == 0)
            QCoreApplication::processEvents();
        sha1_ar.append(a_sha1[i]);
    }


    hashes_obj.md5_hash  = QString::fromLocal8Bit(md5_ar.toHex());
    hashes_obj.sha1_hash = QString::fromLocal8Bit(sha1_ar.toHex());
    return hashes_obj;
}


void recon_helper_standard::pub_set_filesystem_modules_thread_run_status(QString job_type, QString status, QString caller_func)
{
    recon_static_functions::app_debug("Start ", recon_static_functions::prepare_callerfun(caller_func));

    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "case_fs_module_status.sqlite";

    if(!QFileInfo(destination_db_path).exists())
        return;

    QString command_str =  QString("select run_status from tbl_fs_module_status where module_job_type = ?");
    QStringList list_vals;
    list_vals << job_type;

    QString status_str = get_string_from_db_with_addbindvalues_by_dbpath(command_str,list_vals,0,destination_db_path,caller_func);

    list_vals.clear();

    if(status_str.isEmpty())
    {
        command_str =  QString("insert into tbl_fs_module_status(module_job_type,run_status) values(?,?)");

        list_vals << job_type << status;

        execute_db_command_with_addbindvalues_by_dbpath(command_str,list_vals,destination_db_path,caller_func);
    }
    else if(status_str == status)
    {
        ///no need to do anything
    }
    else
    {
        command_str = QString("update tbl_fs_module_status set run_status = ? where module_job_type= ?");

        list_vals << status << job_type;

        execute_db_command_with_addbindvalues_by_dbpath(command_str, list_vals, destination_db_path, caller_func);
    }

    recon_static_functions::app_debug("End ", recon_static_functions::prepare_callerfun(caller_func));
}

QString recon_helper_standard::pub_get_filesystem_modules_thread_run_status(QString job_type, QString caller_func)
{
    recon_static_functions::app_debug("Start ", recon_static_functions::prepare_callerfun(caller_func));

    QString status_str;
    QString destination_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + "case_fs_module_status.sqlite";

    if(!QFileInfo(destination_db_path).exists())
        return status_str;

    QString command_str =  QString("select run_status from tbl_fs_module_status where module_job_type = ?");
    QStringList list_vals;
    list_vals << job_type;

    status_str = get_string_from_db_with_addbindvalues_by_dbpath(command_str,list_vals,0,destination_db_path,caller_func);

    status_str = status_str.trimmed();

    recon_static_functions::app_debug("End ", recon_static_functions::prepare_callerfun(caller_func));

    return status_str;
}

QString recon_helper_standard::pub_get_app_license_path()
{
    QString path_str = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString();
    if(path_str.isEmpty())
        return path_str;

    path_str = path_str + "license";

    return path_str;
}

QStringList recon_helper_standard::parse_spotlight_parser_file_for_each_inode(QString text_file_path)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QStringList all_node_list;

    // donot remove below commented line
    // this is for verification of splt variable
    //QString inode_spliiter = "------------------------------------------------------------";

    QString inode_spliiter = "------------------------------------------------------------";
    QStringList each_inode_list;
    int count = 0;
    bool bool_splitter_found = false;
    QFile file(text_file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("File Open -----FAILED---- " + file.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + file.errorString(), Q_FUNC_INFO);
        return all_node_list;
    }

    while(!file.atEnd())
    {
        QString line_str = file.readLine();

        if(line_str.startsWith(inode_spliiter))
        {
            bool_splitter_found = true;
            count++;
        }
        else
        {
            if(bool_splitter_found && count > 1)
            {
                all_node_list.append(each_inode_list.join(""));
                each_inode_list.clear();
            }

            if(line_str.isEmpty())
                continue;

            bool_splitter_found = false;
            each_inode_list << line_str;
        }
    }

    file.close();
    recon_static_functions::app_debug("End",Q_FUNC_INFO);
    return all_node_list;
}

QStringList recon_helper_standard::get_apple_metadata_from_result_apple_metadata_db(QString file_path , QString source_count_name)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    QStringList apple_metadata_list;
    QString case_config_db_file =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString apple_metadata_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + "/apple_metadata.sqlite";
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString() + source_count_name + QString("/file_system.sqlite");

    QStringList attribute_title_name;
    QStringList attribute_apple_name;
    QStringList custom_column_name;

    QString connection_naam1 = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam1);
    QSqlDatabase config_database = QSqlDatabase::addDatabase("QSQLITE", connection_naam1);
    config_database.setDatabaseName(case_config_db_file);
    if(!config_database.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + config_database.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + config_database.lastError().text(),Q_FUNC_INFO);
        return apple_metadata_list;
    }

    QSqlQuery column_query(config_database);

    QString column_command = QString("Select attribute_name ,metadata_attribute_name ,custom_column_name from tbl_apple_attributes");
    column_query.prepare(column_command);

    if(!column_query.exec())
    {
        recon_static_functions::app_debug(" query--------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(" query -" + column_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -" + column_query.lastError().text(),Q_FUNC_INFO);
        config_database.close();
        return apple_metadata_list;
    }

    while(column_query.next())
    {
        attribute_title_name <<  column_query.value("attribute_name").toString();
        attribute_apple_name <<  column_query.value("metadata_attribute_name").toString();
        custom_column_name <<  column_query.value("custom_column_name").toString();
    }
    config_database.close();



    QString connection_naam2 = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam2);
    QSqlDatabase apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam2);
    apple_metadata_db.setDatabaseName(apple_metadata_db_path);
    if(!apple_metadata_db.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + apple_metadata_db.lastError().text(),Q_FUNC_INFO);
        return apple_metadata_list;
    }

    QSqlQuery attach_query(apple_metadata_db);

    QString apple_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(fs_db_path));
    attach_query.prepare(apple_db_attach_commmand);

    if(!attach_query.exec())
    {
        recon_static_functions::app_debug(" query attach  --------FAILED------ " + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + attach_query.lastError().text(),Q_FUNC_INFO);
        apple_metadata_db.close();
        return apple_metadata_list;
    }

    QString tm_column_name = custom_column_name.join(",a.");
    tm_column_name.prepend("a.");

    QString command =  QString("Select " + tm_column_name + " from main.files as a join filesystemDB.files as b ON a.filesystem_record = b.INT where b.filepath = ?");

    QSqlQuery select_query(apple_metadata_db);
    select_query.prepare(command);
    select_query.addBindValue(file_path);

    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query select --------FAILED------" + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        apple_metadata_db.close();
        return apple_metadata_list;
    }

    while(select_query.next())
    {
        for(int jj = 0 ; jj < attribute_apple_name.size(); jj++)
        {
            QString value_str;

            QString keys = attribute_apple_name.at(jj);
            if(keys.contains("Date") && !keys.contains("Dates"))
            {
                value_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(select_query.value(jj).toString(),Q_FUNC_INFO);
                apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str;
            }
            else if(keys.contains("Dates"))
            {
                value_str = select_query.value(jj).toString();
                if(value_str.contains(";"))
                {
                    QStringList split_list = value_str.split(";");
                    QStringList value_converted_list;
                    for(int ii = 0; ii < split_list.size(); ii++)
                    {
                        QString used_dates = split_list.at(ii);
                        used_dates = used_dates.trimmed();
                        value_converted_list << global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(used_dates,Q_FUNC_INFO);
                    }
                    apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_converted_list.join("\n");
                }
                else
                {
                    value_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(value_str,Q_FUNC_INFO);
                    apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str;
                }
            }
            else if(keys.contains("ContentTypeTree") || keys.contains("WhereFroms"))
            {
                value_str = select_query.value(jj).toString();
                apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str.replace(";", "\n");
            }
            else
            {
                value_str = select_query.value(jj).toString();
                apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str;
            }
        }
    }

    apple_metadata_db.close();

    recon_static_functions::app_debug("end" , Q_FUNC_INFO);
    return apple_metadata_list;

}

QStringList recon_helper_standard::get_apple_metadata_from_result_extracted_content_apple_metadata_db(QString file_path , QString tmp_apple_metadata_db_path,QString fs_db_path)
{
    recon_static_functions::app_debug("Start" , Q_FUNC_INFO);

    QStringList apple_metadata_list;
    QString case_config_db_file =  global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";


    QStringList attribute_title_name;
    QStringList attribute_apple_name;
    QStringList custom_column_name;

    QString connection_naam1 = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam1);
    QSqlDatabase config_database = QSqlDatabase::addDatabase("QSQLITE", connection_naam1);
    config_database.setDatabaseName(case_config_db_file);
    if(!config_database.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + config_database.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + config_database.lastError().text(),Q_FUNC_INFO);
        return apple_metadata_list;
    }

    QSqlQuery column_query(config_database);

    QString column_command = QString("Select attribute_name ,metadata_attribute_name ,custom_column_name from tbl_apple_attributes");
    column_query.prepare(column_command);

    if(!column_query.exec())
    {
        recon_static_functions::app_debug(" query--------FAILED------",Q_FUNC_INFO);
        recon_static_functions::app_debug(" query -" + column_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -" + column_query.lastError().text(),Q_FUNC_INFO);
        config_database.close();
        return apple_metadata_list;
    }

    while(column_query.next())
    {
        attribute_title_name <<  column_query.value("attribute_name").toString();
        attribute_apple_name <<  column_query.value("metadata_attribute_name").toString();
        custom_column_name <<  column_query.value("custom_column_name").toString();
    }
    config_database.close();



    QString connection_naam2 = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam2);
    QSqlDatabase apple_metadata_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam2);
    apple_metadata_db.setDatabaseName(tmp_apple_metadata_db_path);
    if(!apple_metadata_db.open())
    {
        recon_static_functions::app_debug(" destination db open ----FAILED---" + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Error--- " + apple_metadata_db.lastError().text(),Q_FUNC_INFO);
        return apple_metadata_list;
    }

    QSqlQuery attach_query(apple_metadata_db);

    QString apple_db_attach_commmand = QString("ATTACH DATABASE '%1' AS filesystemDB")
            .arg(QDir::toNativeSeparators(fs_db_path));
    attach_query.prepare(apple_db_attach_commmand);

    if(!attach_query.exec())
    {
        recon_static_functions::app_debug(" query attach  --------FAILED------ " + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + attach_query.lastError().text(),Q_FUNC_INFO);
        apple_metadata_db.close();
        return apple_metadata_list;
    }

    QString tm_column_name = custom_column_name.join(",a.");
    tm_column_name.prepend("a.");

    QString command =  QString("Select " + tm_column_name + " from main.files as a join filesystemDB.files as b ON a.filesystem_record = b.INT where b.filepath = ?");

    QSqlQuery select_query(apple_metadata_db);
    select_query.prepare(command);
    select_query.addBindValue(file_path);

    if(!select_query.exec())
    {
        recon_static_functions::app_debug(" query select --------FAILED------" + apple_metadata_db.databaseName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" query -  " + select_query.executedQuery(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + select_query.lastError().text(),Q_FUNC_INFO);
        apple_metadata_db.close();
        return apple_metadata_list;
    }

    while(select_query.next())
    {
        for(int jj = 0 ; jj < attribute_apple_name.size(); jj++)
        {
            QString value_str;

            QString keys = attribute_apple_name.at(jj);
            if(keys.contains("Date") && !keys.contains("Dates"))
            {
                value_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(select_query.value(jj).toString(),Q_FUNC_INFO);
                apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str;
            }
            else if(keys.contains("Dates"))
            {
                value_str = select_query.value(jj).toString();
                if(value_str.contains(";"))
                {
                    QStringList split_list = value_str.split(";");
                    QStringList value_converted_list;
                    for(int ii = 0; ii < split_list.size(); ii++)
                    {
                        QString used_dates = split_list.at(ii);
                        used_dates = used_dates.trimmed();
                        value_converted_list << global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(used_dates,Q_FUNC_INFO);
                    }
                    apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_converted_list.join("\n");
                }
                else
                {
                    value_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_selected_timezone(value_str,Q_FUNC_INFO);
                    apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str;
                }
            }
            else if(keys.contains("ContentTypeTree") || keys.contains("WhereFroms"))
            {
                value_str = select_query.value(jj).toString();
                apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str.replace(";", "\n");
            }
            else
            {
                value_str = select_query.value(jj).toString();
                apple_metadata_list << attribute_apple_name.at(jj) + MACRO_RECON_Splitter_1_multiple + value_str;
            }
        }
    }

    apple_metadata_db.close();

    recon_static_functions::app_debug("end" , Q_FUNC_INFO);
    return apple_metadata_list;

}

QStringList recon_helper_standard::get_total_tab_name_from_db_by_db_path(QString db_path, QString caller_func)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);
    QStringList table_name_list;

    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(db_path);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " Destination Db opening --------FAILED------ " + db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

        recon_static_functions::debug_intensive(recon_static_functions::recon_static_functions::prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
        return table_name_list;
    }


    table_name_list = destination_db->tables();

    destination_db->close();

    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);

    return table_name_list;

}

int recon_helper_standard::get_maximum_root_count_index_from_db_and_text_file()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString z_roots_count_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.sqlite";
    int max_root_count_db_int = 0;
    if(QFile(z_roots_count_db_file).exists())
    {
        max_root_count_db_int = get_string_from_db_by_dbpath("select MAX(z_root_count) from z_count", 0, z_roots_count_db_file, Q_FUNC_INFO).toInt();
    }

    int max_root_count_txt_int = 0;
    QString z_roots_count_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_roots_count.txt";
    if(QFile(z_roots_count_txt_file).exists())
    {
        QFile z_root_count_file(z_roots_count_txt_file);
        if(z_root_count_file.open(QIODevice::ReadOnly))
        {
            while(!z_root_count_file.atEnd())
            {
                QString line_str = z_root_count_file.readLine().trimmed();
                line_str.remove(0, QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Root_Flag).size());
                max_root_count_txt_int = qMax(max_root_count_txt_int, line_str.toInt());
            }
            z_root_count_file.close();
        }
        else
        {
            recon_static_functions::app_debug("z_roots_count File open --------FAILED------" + z_roots_count_txt_file, Q_FUNC_INFO);
        }

    }

    int max_value = qMax(max_root_count_db_int, max_root_count_txt_int);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
    return max_value;

}

int recon_helper_standard::get_maximum_source_count_index_from_db_and_text_file()
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    QString z_sources_count_db_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.sqlite";
    int max_source_count_db_int = 0;
    if(QFile(z_sources_count_db_file).exists())
    {
        max_source_count_db_int = get_string_from_db_by_dbpath("select MAX(z_source_count) from z_count", 0, z_sources_count_db_file, Q_FUNC_INFO).toInt();
    }

    int max_source_count_txt_int = 0;
    QString z_sources_count_txt_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "z_sources_count.txt";
    if(QFile(z_sources_count_txt_file).exists())
    {
        QFile z_source_count_file(z_sources_count_txt_file);
        if(z_source_count_file.open(QIODevice::ReadOnly))
        {
            while(!z_source_count_file.atEnd())
            {
                QString line_str = z_source_count_file.readLine().trimmed();
                line_str.remove(0, QString(MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_Source_Flag).size());
                max_source_count_txt_int = qMax(max_source_count_txt_int, line_str.toInt());
            }
            z_source_count_file.close();
        }
        else
        {
            recon_static_functions::app_debug("z_source_count File open --------FAILED------" + z_sources_count_txt_file, Q_FUNC_INFO);
        }
    }

    int max_value = qMax(max_source_count_db_int, max_source_count_txt_int);

    recon_static_functions::app_debug("End",Q_FUNC_INFO);
    return max_value;

}


void recon_helper_standard::alter_db_tables_to_add_column_by_dbpath(QStringList table_list, QStringList new_colname_list, QStringList new_coltype_list, QString destination_db_file)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

    }

    alter_db_tables_to_add_column_by_dbreference(table_list , new_colname_list , new_coltype_list , *destination_db);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void recon_helper_standard::alter_db_tables_to_rename_column_by_dbpath(QStringList table_list, QStringList existing_colname_list, QStringList new_colname_list, QString destination_db_file)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);

    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    QString connection_naam = QString::number(rand()) + QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase *destination_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_naam));

    destination_db->setDatabaseName(destination_db_file);
    if(!destination_db->open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + destination_db_file,Q_FUNC_INFO);
        recon_static_functions::app_debug(" error -  " + destination_db->lastError().text(),Q_FUNC_INFO);
        if(destination_db != NULL)
            delete destination_db;
        QSqlDatabase::removeDatabase(connection_naam);

    }

    alter_db_tables_to_rename_column_by_dbreference(table_list , existing_colname_list , new_colname_list , *destination_db);

    destination_db->close();
    if(destination_db != NULL)
        delete destination_db;
    QSqlDatabase::removeDatabase(connection_naam);


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

void recon_helper_standard::alter_db_tables_to_add_column_by_dbreference(QStringList table_list, QStringList new_colname_list, QStringList new_coltype_list, QSqlDatabase &destination_db)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    for(int tt = 0; tt < table_list.size(); tt++)
    {
        QString table_name = table_list.at(tt);

        QString command_pragma = QString("PRAGMA TABLE_INFO( '") + table_name + "')";
        QStringList existing_colname_list = get_stringlist_from_db_by_dbreference(command_pragma, 1, destination_db, Q_FUNC_INFO);

        QStringList remaining_col_list;
        for(int count = 0; count < new_colname_list.size(); count++)
        {
            QString new_column_name = new_colname_list.at(count).trimmed();

            if(existing_colname_list.contains(new_column_name))
                continue;

            remaining_col_list << new_column_name + " " +  new_coltype_list.at(count);

        }

        for(int count = 0; count < remaining_col_list.size(); count++)
        {
            QString cmd = "ALTER TABLE '" + table_name + "' ADD " + remaining_col_list.at(count);
            execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
        }

    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);

}

void recon_helper_standard::alter_db_tables_to_rename_column_by_dbreference(QStringList table_list, QStringList existing_colname_list, QStringList new_colname_list, QSqlDatabase &destination_db)
{
    recon_static_functions::app_debug("Start",Q_FUNC_INFO);


    for(int tt = 0; tt < table_list.size(); tt++)
    {
        QString table_name = table_list.at(tt);

        for(int count = 0; count < existing_colname_list.size(); count++)
        {
            QString cmd = "ALTER TABLE '" + table_name + "' RENAME COLUMN " + existing_colname_list.at(count) + " to " + new_colname_list.at(count);
            execute_db_command_by_dbreference(cmd, destination_db, Q_FUNC_INFO);
        }
    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);
}

bool recon_helper_standard::source_applicable_for_extensive_module(QString  src_count_name)
{
    struct_GLOBAL_witness_info_source src_obj = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(src_count_name);

    if(QString(src_obj.role_type).toLower() == QString(MACRO_SOURCE_ROLE_TYPE_SYSTEM).toLower()
            || QString(src_obj.role_type).toLower() == QString(MACRO_SOURCE_ROLE_TYPE_PREBOOT).toLower()
            || QString(src_obj.role_type).toLower() == QString(MACRO_SOURCE_ROLE_TYPE_VM).toLower()
            || QString(src_obj.role_type).toLower() == QString(MACRO_SOURCE_ROLE_TYPE_UPDATE).toLower()
            || QString(src_obj.role_type).toLower() == QString(MACRO_SOURCE_ROLE_TYPE_RECOVERY).toLower())
    {
        return false;
    }
    else if(QString(src_obj.image_volume_type).toLower() == QString(MACRO_Disk_Partition_Type_EFI).toLower())
    {
        return false;
    }
    else
    {
        return true;
    }

}

QString recon_helper_standard::pub_fetch_examiner_comments_data()
{
    recon_static_functions::app_debug("Starts",Q_FUNC_INFO);

    QString examiner_data;

    QString examiner_notes_file_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Examiner_Space_In_Result_QString).toString() + "Examiner_Notes.html";
    QFile examiner_notes_QFile;
    examiner_notes_QFile.setFileName(examiner_notes_file_path);

    if(!examiner_notes_QFile.open(QFile::ReadOnly))
    {
        recon_static_functions::app_debug(" File Open Examiner Notes Html -----FAILED-----" + examiner_notes_QFile.fileName(), Q_FUNC_INFO);
        recon_static_functions::app_debug(" Error:" + examiner_notes_QFile.errorString(), Q_FUNC_INFO);
        return examiner_data;
    }

    examiner_notes_QFile.seek(0);
    QString read_examiner_notes_html_file = examiner_notes_QFile.readAll();

    examiner_notes_QFile.close();

    qint64 strt_index = 0;
    qint64 end_index = 0;

    strt_index = read_examiner_notes_html_file.indexOf("<body");
    end_index = read_examiner_notes_html_file.lastIndexOf("</body>");


    if(strt_index > 0 && end_index > 0)
    {
        end_index = end_index + QString("</body>").size();
        examiner_data = read_examiner_notes_html_file.mid(strt_index, end_index - strt_index);

        if(examiner_data.contains("\n"))
            examiner_data.remove("\n");
    }


    recon_static_functions::app_debug("End",Q_FUNC_INFO);

    return examiner_data;

}

QString recon_helper_standard::get_censored_filepath(QString src_file_path, QString dest_file_path)
{
    QStringList args;

    //    args << "../Resources/scripts_persistent/skin_tone_censor.py"<< src_file_path << dest_file_path;

    //    QString cmd_python_path = global_varibale_python_path;

    args <<  src_file_path << dest_file_path;

    QString skin_tone_censor_exe_path = "../ZAlreadySigned/pysafe/intelligence_army/skin_tone_censor";

    QProcess censor_proc;
    //    censor_proc.execute(cmd_python_path,args);
    censor_proc.execute(skin_tone_censor_exe_path,args);
    censor_proc.waitForStarted();
    censor_proc.waitForFinished(-1);
    censor_proc.close();

    //    QString censored_file_name = record_no_str + QString("_censored_") + QFileInfo(src_file_path).fileName();

    //    dest_file_path = dest_file_path.append(censored_file_name);

    return dest_file_path;

}

QString recon_helper_standard::get_tag_name_of_record_no_by_source_count_name(QString record_no, QString source_count_name)
{
    QString fs_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_File_System_In_Result_QString).toString()
            + source_count_name + QString("/file_system.sqlite");

    QString cmd = "SELECT recon_tag_value FROM files WHERE INT = '" + record_no + "'";
    QString tag_value = get_string_from_db_by_dbpath(cmd,0,fs_db_path,Q_FUNC_INFO);

    return tag_value;
}

QString recon_helper_standard::get_new_mount_path_of_time_machine_backup_image_and_snapshots(QString recieved_mount_path, QString m_type)
{
    //Input  : "/private/var/tmp/RECON_LAB_moksha/1656938803/RECON_mount_path/RECON_MNT_disk8s2_time_machine_img_June-06.dmg"
    //Output : "/private/var/tmp/RECON_LAB_moksha/1656938803/RECON_mount_path/RECON_MNT_disk8s2_time_machine_img_June-06.dmg/2022-06-06-151008.previous/Macintosh HD - Data"

    //OR

    //input - /private/var/tmp/RECON_LAB_jatayu/1657004625/apfs_snapshots/snapshot_mount
    //output - /private/var/tmp/RECON_LAB_jatayu/1657004625/apfs_snapshots/snapshot_mount/2022-06-06-130355.backup/Macintosh HD - Data

    QString new_mount_path;

    bool bool_root_file1 = false;
    bool bool_root_file2 = false;
    bool bool_root_file3 = false;

    QString snapshot_mount_path;

    if(QFileInfo(recieved_mount_path).isDir())
    {
        QString chck_str;
        if(m_type == QString(MACRO_Mount_Path_Time_Machine_Backup_Image_Main))
        {
            chck_str = ".previous";
        }
        else if(m_type == QString(MACRO_Mount_Path_Time_Machine_Backup_Image_Snapshots))
        {
            chck_str = ".backup";
        }

        QDirIterator mnt_iter(recieved_mount_path, QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);
        while(mnt_iter.hasNext())
        {
            QString m_filepath = mnt_iter.next().trimmed();
            if(m_filepath.endsWith(chck_str))
            {
                snapshot_mount_path = m_filepath;
                break;
            }
        }

        if(snapshot_mount_path.trimmed().isEmpty())
            return recieved_mount_path;

        QDirIterator snap_iter(snapshot_mount_path, QDir::AllDirs | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);
        while(snap_iter.hasNext())
        {
            QString machine_name_path = snap_iter.next().trimmed();

            QDirIterator final_iter(machine_name_path, QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot,QDirIterator::NoIteratorFlags);
            while(final_iter.hasNext())
            {
                QString root_dir_filepath = final_iter.next().trimmed();

                if(root_dir_filepath.endsWith("/usr"))
                {
                    bool_root_file1 = true;
                }
                if(root_dir_filepath.endsWith("/private"))
                {
                    bool_root_file2 = true;
                }
                if(root_dir_filepath.endsWith("/System"))
                {
                    bool_root_file3 = true;
                }
            }

            if(bool_root_file1 == true && bool_root_file2 == true && bool_root_file3 == true)
            {
                new_mount_path = machine_name_path;
                break;
            }
        }
    }

    return new_mount_path;
}
