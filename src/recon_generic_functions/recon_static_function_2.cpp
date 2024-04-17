#include "recon_static_functions.h"


QString recon_static_functions::get_fs_extraction_source_path(narad_muni *narad_muniobj, QString src_type_intrnl, QString virtual_source_path, QString caller_func)
{

#ifdef MACRO_RECON_STATIC_APP_RUN_BY_FORENSO
    QString recon_home_filepath = narad_muniobj->get_field(MACRO_NARAD_RECON_Library_Home_Dir_Path_QString).toString() + "recon_home.txt";
    QFile fs_file(recon_home_filepath);
    if(fs_file.exists())
    {
        if(fs_file.open(QIODevice::ReadOnly))
        {
            QString file_data = fs_file.readLine();
            file_data = file_data.trimmed();
            fs_file.close();
            virtual_source_path = virtual_source_path + file_data;
        }
        else
        {
            recon_static_functions::app_debug("File System sourcepath File open ---FAILED---:"+ recon_home_filepath, caller_func);
            recon_static_functions::app_debug("Error: "+ fs_file.errorString(), caller_func);
        }
    }

#endif

    return virtual_source_path;
}

void recon_static_functions::clear_variables_witness_info_root_structure(struct_GLOBAL_witness_info_root &struct_root_info)
{
    struct_root_info.evidence_number.clear();
    struct_root_info.description.clear();

    struct_root_info.evidence_type.clear();
    struct_root_info.root_count_name.clear();
    struct_root_info.root_type.clear();
    struct_root_info.source_type_internal.clear();
    struct_root_info.root_name.clear();
    struct_root_info.root_path_1.clear();
    struct_root_info.root_path_2_for_fusion.clear();
    struct_root_info.root_disk_identifier_1.clear();
    struct_root_info.root_disk_node_1.clear();
    struct_root_info.root_disk_identifier_2_for_fusion.clear();
    struct_root_info.root_disk_node_2_for_fusion.clear();

    struct_root_info.root_recovery_passcode_plain.clear();
    struct_root_info.root_recovery_passcode_encrypted.clear();
    struct_root_info.root_evidence_password_plain.clear();
    struct_root_info.root_evidence_password_encrypted.clear();

    struct_root_info.home_directory_user_name.clear();
    struct_root_info.root_hash_md5.clear();
    struct_root_info.root_hash_sha1.clear();

    struct_root_info.source_count_name_all_QStringList.clear();
    struct_root_info.source_count_name_prime_QStringList.clear();
    struct_root_info.source_count_name_nested_QStringList.clear();

}


void recon_static_functions::clear_variables_witness_info_source_structure( struct_GLOBAL_witness_info_source &struct_source_info)
{

    struct_source_info.latest_backup_date_numeric_qint64 = 0;
    struct_source_info.timezone_numeric_qint64 = 0;
    struct_source_info.os_installer_timestamp_utc_numeric_qint64 = 0;

    struct_source_info.bool_recon_logical_image = false;
    struct_source_info.bool_source_nested = false;
    struct_source_info.bool_source_decompressed = false;

    struct_source_info.system_user_QStringList.clear();
    struct_source_info.configured_ids_QStringList.clear();
    struct_source_info.phone_no_msisdn_QStringList.clear();
    struct_source_info.imei_no_QStringList.clear();
    struct_source_info.sim_no_iccid_QStringList.clear();

    struct_source_info.snapshots_QList.clear();
    struct_source_info.system_account_QList.clear();

    struct_source_info.fuse_mount_path_tsk.clear();
    struct_source_info.fuse_mount_path_raw.clear();
    struct_source_info.fuse_mount_path_zip.clear();

    struct_source_info.source_path_1.clear();
    struct_source_info.source_path_2_for_fusion.clear();

    struct_source_info.evidence_number.clear();
    struct_source_info.description.clear();
    struct_source_info.source_hierarchy.clear();

    struct_source_info.root_count_name.clear();
    struct_source_info.evidence_type.clear();
    struct_source_info.root_type.clear();
    struct_source_info.root_name.clear();
    struct_source_info.root_path_1.clear();
    struct_source_info.root_path_2_for_fusion.clear();
    struct_source_info.root_disk_identifier.clear();

    struct_source_info.os_scheme_display.clear();
    struct_source_info.os_scheme_internal.clear();
    struct_source_info.source_type_internal.clear();
    struct_source_info.source_name.clear();
    struct_source_info.source_count_name.clear();
    struct_source_info.complete_source_name .clear();
    struct_source_info.root_recovery_passcode_plain.clear();

    struct_source_info.source_evidence_password_plain.clear();
    struct_source_info.source_evidence_password_encrypted.clear();
    struct_source_info.source_evidence_password_modified_symbol_hex.clear();

    struct_source_info.source_main_path.clear();
    struct_source_info.virtual_source_path.clear();
    struct_source_info.fs_extraction_source_path.clear();
    struct_source_info.source_disk_identifier.clear();

    struct_source_info.product_type.clear();
    struct_source_info.os_version_extended.clear();
    struct_source_info.os_version_full.clear();
    struct_source_info.build_version.clear();
    struct_source_info.latitude.clear();
    struct_source_info.longitude.clear();
    struct_source_info.country.clear();
    struct_source_info.city.clear();
    struct_source_info.display_name.clear();
    struct_source_info.device_name.clear();
    struct_source_info.device_id.clear();
    struct_source_info.guid.clear();
    struct_source_info.itune_version.clear();
    struct_source_info.serial_no.clear();
    struct_source_info.product_name.clear();
    struct_source_info.target_type.clear();
    struct_source_info.latest_backup_date_readable_QString.clear();
    struct_source_info.backup_size_byte_numeric_QString.clear();
    struct_source_info.timezone_readable_QString.clear();
    struct_source_info.parent_source_count_name.clear();
    struct_source_info.source_main_name.clear();
    struct_source_info.image_volume_type.clear();
    struct_source_info.image_volume_offset.clear();
    struct_source_info.image_volume_uuid.clear();
    struct_source_info.total_size_byte_numeric_QString.clear();
    struct_source_info.free_space_byte_numeric_QString.clear();
    struct_source_info.used_space_byte_numeric_QString.clear();
    struct_source_info.file_system_type.clear();
    struct_source_info.product_model.clear();
    struct_source_info.bluetooth_device_address.clear();
    struct_source_info.wifi_address.clear();
    struct_source_info.locale_language.clear();


}

void recon_static_functions::clear_variables_image_mounting_partiton_info(struct_global_image_mounter_partition_info &prtn_info)
{
    prtn_info.parent_type.clear();
    prtn_info.parent_name.clear();
    prtn_info.parent_identifier.clear();
    prtn_info.parent_node.clear();

    prtn_info.derived_into_node.clear();
    prtn_info.derived_into_identifier.clear();
    prtn_info.derived_into_name.clear();
    prtn_info.derived_into_uuid.clear();

    prtn_info.mount_path.clear();
    prtn_info.total_size_byte_numeric_QString.clear();
    prtn_info.free_size_byte_numeric_QString.clear();
    prtn_info.fs_type.clear();

    prtn_info.volume_offset.clear();
    prtn_info.volume_UUID.clear();

}

void recon_static_functions::clear_variables_image_mounting_image_info(struct_global_image_mounter_image_info &img_info)
{
    img_info.image_path_1.clear();
    img_info.image_name_1.clear();
    img_info.shadow_file_path_1.clear();
    img_info.disk_node_1.clear();
    img_info.disk_identifier_1.clear();
    img_info.osx_fuse_mount_point_1.clear();

    img_info.image_path_2.clear();
    img_info.image_name_2.clear();
    img_info.shadow_file_path_2.clear();
    img_info.disk_node_2.clear();
    img_info.disk_identifier_2.clear();
    img_info.osx_fuse_mount_point_2.clear();

    img_info.image_format_type.clear();
    img_info.password.clear();
    img_info.image_source_type_display.clear();
    img_info.image_source_type_internal.clear();

    img_info.list_struct_partition_info.clear();

}



QString recon_static_functions::get_complete_file_path(QString target_path, QString filepath, QString caller_func)
{
    target_path = target_path.trimmed();
    if(!target_path.isEmpty() && !target_path.endsWith("/"))
    {
        target_path.append("/");
    }

    filepath = filepath.trimmed();
    if(!filepath.isEmpty() && !filepath.startsWith("/"))
    {
        filepath.prepend("/");
    }

    if(target_path.endsWith("/") && filepath.startsWith("/"))
        target_path.chop(1);

    return QString(target_path + filepath);
}


QStringList recon_static_functions::get_system_user_list_of_macOS(QString path, QString src_type_intrnl, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    QStringList users_list;
    QDirIterator iter(path + "Users", QDir::AllEntries | QDir::Files | QDir::Hidden | QDir::System, QDirIterator::NoIteratorFlags);

    while(iter.hasNext())
    {
        QFileInfo info(iter.next());

        if(info.isFile())
            continue;

        if(info.fileName() == "." || info.fileName() == ".." || info.fileName() == "Shared")
            continue;

        if(src_type_intrnl != MACRO_GLOBAL_COMMON_RECON_AND_ARTIFACT_SourceType_TimeMachineBackupFolder_Internal)
        {
            QFile file(info.filePath() + "/Library/Preferences/com.apple.finder.plist");
            if(!file.exists())
                continue;
        }


        users_list.append(info.fileName());
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return users_list;
}


QStringList recon_static_functions::get_mounted_media_list(QString caller_func)
{
    QStringList list;

    QDirIterator iter("/Volumes/");
    while(iter.hasNext())
    {
        QString temp = iter.next();
        QFileInfo info(temp);

        if(!info.isDir())
            continue;

        QDir mdir(temp);

        if(mdir.count() < 3)
            continue;


        if(info.isSymLink() || info.isHidden() || info.isRoot())
            continue;

        if(temp.endsWith("."))
            continue;

        list << temp;
    }

    return list;
}

QStringList recon_static_functions::get_associated_tabs(QString plgn_name, QStringList plugin_and_associated_tabs_list, QString caller_func)
{
    // Input : ("iPhoto;++::++;List View", "Photos;++::++;All Photos(##)Faces(##)Preferences(##)Moments")
    // Input : ("Photos;++::++;All Photos[$$]1(##)Preferences[$$]0(##)Moments[$$]1")

    // Output : ("All Photos", "Faces", "Preferences", "Moments")

    QStringList tabs_list;
    for(int itr=0; itr < plugin_and_associated_tabs_list.size(); itr++)
    {
        QString tmp_str = plugin_and_associated_tabs_list.at(itr);

        QStringList list_holder = tmp_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(list_holder.size() < 2)
            continue;

        QString plugin_name = list_holder.at(0);

        if(plugin_name != plgn_name){
            continue;
        }

        tmp_str = list_holder.at(1);

        QStringList tabs_info = tmp_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);
        for(int tt = 0; tt < tabs_info.size(); tt++)
        {
            QString tb_nm = tabs_info.at(tt);
            tb_nm = tb_nm.trimmed();
            if(tb_nm.contains(MACRO_RECON_Splitter_3_dollar_in_brace))
            {
                tb_nm.remove(tb_nm.indexOf(MACRO_RECON_Splitter_3_dollar_in_brace),tb_nm.size());
            }
            tabs_list << tb_nm.trimmed();
        }

        return tabs_list;
    }

    return tabs_list;
}



QString recon_static_functions::get_exportfilelist_command()
{
    QString command = QString("CREATE TABLE exportfilelist"
                              "(INT INTEGER PRIMARY KEY,"
                              "record VARCHAR(150),"
                              "table_name VARCHAR(1000),"
                              "filepath VARCHAR(100),"
                              "exp_newpath VARCHAR(100),"
                              "exp_relativepath VARCHAR(100),"
                              "exp_size INTEGER,"
                              "exp_hashmd5 VARCHAR(100),"
                              "exp_hashsha1 VARCHAR(100),"
                              "exp_LINKFILENAME varchar(500),"
                              "exp_time INTEGER)");


    return command;
}

QString recon_static_functions::get_tags_on_add_or_remove_bookmarks(QString tag_value, QString status, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    if(status == "1")
    {

        if(!tag_value.contains(QString(MACRO_Tag_Name_Bookmarks)))
            tag_value.append(","+ QString(MACRO_Tag_Name_Bookmarks));

        if(tag_value.startsWith(","))
            tag_value.remove(0,1);
    }else
    {
        if(tag_value.contains(QString(MACRO_Tag_Name_Bookmarks)))
            tag_value.remove(tag_value.indexOf(QString(MACRO_Tag_Name_Bookmarks)), QString(MACRO_Tag_Name_Bookmarks).size());

        tag_value.replace(",,", ",");

        if(tag_value.endsWith(","))
            tag_value.chop(1);
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);
    return tag_value;
}




QStringList recon_static_functions::get_unmatched_varible_list_of_parentlist(QStringList parent_list, QStringList child_list, QString caller_func)
{
    QStringList final_lst;

    for(int iter = 0; iter < parent_list.size(); iter++)
    {
        if(child_list.contains(parent_list.at(iter)))
        {
            continue;
        }

        final_lst << parent_list.at(iter);
    }

    return final_lst;
}


QString recon_static_functions::get_partial_command_forsearch(QStringList search_cols_list, QStringList search_string_list, QString caller_func)
{
    QString partial_command;

    if(search_cols_list.size() > 0)
    {
        partial_command = " where ";

        for(int k = 0 ; k < search_cols_list.size() ; k++)
        {
            if(search_cols_list.at(k) == "os_scheme_display")
                continue ;

            for(int j = 0; j < search_string_list.size() ; j++)
            {

                partial_command  = partial_command + search_cols_list.at(k) + " LIKE ? OR ";

            }

        }

        partial_command.remove(partial_command.size() - 4 , 4); // OR
    }

    return partial_command;

}






QString recon_static_functions::get_tags_icon_path(QString colour, QString data, QString caller_func)
{
    QString icon_path;

    if(data.contains(","))
        icon_path = "../icons/tag_icons/multiple.png";
    else
        icon_path = "../icons/tag_icons/"+ colour.replace(" ","_").toLower() + ".png";

    return icon_path;
}

QString recon_static_functions::convert_bytes_to_kilobytes_for_display(QString received_bytes, QString caller_func)
{
    QString kb_size_string;

    if(received_bytes.trimmed().isEmpty())
        return kb_size_string;


    qint64 bytes_qint64 = received_bytes.toLongLong();
    bool ok;
    qint64 file_size =  received_bytes.toLongLong(&ok);

    if(bytes_qint64 <= 1023)
    {
        double kb_size = file_size / (1024.0);
        kb_size_string =  QString("%1").arg(kb_size, 0,'f');
    }
    else
    {
        qint64 kb_size = file_size / (1024);
        int base_value = 10; //decimal
        int req_digit_count = 8;
        kb_size_string =  QString("%1").arg(kb_size, req_digit_count,base_value, QChar('0'));

    }

    return kb_size_string;
}

bool recon_static_functions::check_is_it_plist_file_by_signature(QString filepath, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start ",Q_FUNC_INFO);

    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " file open---FAILED----" +filepath  , Q_FUNC_INFO);
        recon_static_functions::app_debug(file.errorString(),  Q_FUNC_INFO);
        return false;
    }

    QByteArray dataread = file.read(4096);
    file.close();

    QByteArray plist_header_fst;
    plist_header_fst.append(0x62);
    plist_header_fst.append(0x70);
    plist_header_fst.append(0x6C);
    plist_header_fst.append(0x69);
    plist_header_fst.append(0x73);
    plist_header_fst.append(0x74);

    QByteArray plist_header_scnd, plist_header_thrd;

    plist_header_scnd.append(QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n").toStdString());
    plist_header_scnd.append(QString("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n").toStdString());
    plist_header_scnd.append(QString("<plist version=\"1.0\">").toStdString());

    plist_header_thrd.append(QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n").toStdString());
    plist_header_thrd.append(QString("<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n").toStdString());
    plist_header_thrd.append(QString("<plist version=\"1.0\">").toStdString());

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end ",Q_FUNC_INFO);

    if(dataread.startsWith(plist_header_scnd) || dataread.startsWith(plist_header_fst) || dataread.startsWith(plist_header_thrd))
    {
        return true;
    }


    return false;
}


bool recon_static_functions::get_associated_tabs_with_export(QString plgn_name, QString received_tab_name, QStringList plugin_and_associated_tabs_list, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " ,Q_FUNC_INFO);

    // Input :  ("File System;++::++;Files[$$]1", "Photos;++::++;All Photos[$$]1(##)Preferences[$$]0(##)Moments[$$]1")

    for(int itr=0; itr < plugin_and_associated_tabs_list.size(); itr++)
    {
        QString tmp_str = plugin_and_associated_tabs_list.at(itr);

        QStringList list_holder;
        list_holder.clear();

        list_holder = tmp_str.split(MACRO_RECON_Splitter_1_multiple, Qt::SkipEmptyParts);

        if(list_holder.size() < 2)
            continue;

        QString plugin_name = list_holder.at(0);

        if(plugin_name != plgn_name){
            continue;
        }

        tmp_str = list_holder.at(1);

        QStringList tabs_info = tmp_str.split(MACRO_RECON_Splitter_2_hash_in_brace, Qt::SkipEmptyParts);
        for(int tt = 0; tt < tabs_info.size(); tt++)
        {
            QString tab_and_exp_info = tabs_info.at(tt);
            tab_and_exp_info = tab_and_exp_info.trimmed();
            if(tab_and_exp_info.contains(MACRO_RECON_Splitter_3_dollar_in_brace))
            {
                QStringList mm_tb_nm = tab_and_exp_info.split(MACRO_RECON_Splitter_3_dollar_in_brace, Qt::SkipEmptyParts);
                if(mm_tb_nm.size() <= 1)
                    return false;

                QString new_tab_name = mm_tb_nm.at(0).trimmed();
                QString exp_status = mm_tb_nm.at(1).trimmed();
                if(received_tab_name.trimmed() == new_tab_name)
                {
                    if(plugin_name == QString(MACRO_Plugin_Name_Email_Parser))
                        return true;

                    if(exp_status == "1")
                        return true;
                    else
                        return false;
                }

            }
        }

        return false;
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " ,Q_FUNC_INFO);

    return false;
}



QString recon_static_functions::get_sqlite_viewer_name()
{
    return QString(MACRO_Plugin_Name_Sqlite_Viewer);
}


QString recon_static_functions::get_tag_icon_path_from_colour_name(QString colour, QString caller_func)
{
    QString icon_path;
    icon_path = "../icons/tag_icons/"+ colour.replace(" ","_").toLower() + ".png";

    return icon_path;
}

void recon_static_functions::copy_tablewidget_data_to_clipboard(m_tablewidget *my_tablewidget, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Starts---", Q_FUNC_INFO);

    QModelIndexList selection_model_list = my_tablewidget->selectionModel()->selectedIndexes();

    QString clipboard_data_str;
    QString clipboard_header_str;

    QApplication::clipboard()->clear(QClipboard::Clipboard);


    for(int i = 0; i < selection_model_list.size(); i++)
    {
        if(i % 500 == 0)
            QCoreApplication::processEvents();

        QModelIndex model_index = selection_model_list.at(i);

        int selected_row_no = model_index.row();
        int selected_column_no = model_index.column();

        if(my_tablewidget->isRowHidden(selected_row_no))
            continue;

        if(my_tablewidget->item(selected_row_no, selected_column_no) == NULL || my_tablewidget->isColumnHidden(selected_column_no))
            continue;

        if(selected_column_no == (model_index.model()->columnCount() - 1))// this is empty column in plugin that added in last
        {
            clipboard_data_str.append("\n");
            clipboard_header_str.append("\n");

            continue;
        }

        QString header_str = my_tablewidget->horizontalHeaderItem(selected_column_no)->text().trimmed();

        if(header_str.isEmpty() || header_str == QString(Macro_Generic_OS_Scheme) || header_str == QString(MACRO_Generic_OS_Name))
            continue;

        clipboard_header_str.append(header_str + "\t");

        clipboard_data_str.append(my_tablewidget->item(selected_row_no, selected_column_no)->text() + "\t");
    }


    clipboard_header_str.remove(clipboard_header_str.indexOf("\n"), clipboard_header_str.size());
    clipboard_header_str = clipboard_header_str.trimmed();

    QString combine_data_str;
    if(clipboard_header_str.contains("\t"))
        combine_data_str = clipboard_header_str.trimmed() + "\n" + clipboard_data_str.trimmed();
    else
        combine_data_str = clipboard_data_str.trimmed();


    QApplication::clipboard()->setText(combine_data_str, QClipboard::Clipboard);

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Ends---", Q_FUNC_INFO);

}



QString recon_static_functions::remove_record_count_from_feature_name(QString str)
{
    if(str.contains(" ("))
        str = str.remove(str.lastIndexOf(" ("), str.size());
    str = str.trimmed();

    return str;
}

bool recon_static_functions::check_existance_of_action_in_menu(QMenu *menu_hashset_add_files, QString db_name, QString db_path)
{

    QList <QAction*>list_actions = menu_hashset_add_files->actions();

    for(int k = 0; k < list_actions.size() ; k++)
    {
        QAction *act = list_actions.at(k);
        if((act->text() == db_name) && (act->toolTip() == db_path))
        {
            return true;
        }

    }

    return false;
}


QString recon_static_functions::extract_user_name_from_sourcefile(QString path, QString received_initial_path_from_pub)
{
    path.remove(0,received_initial_path_from_pub.size());
    QStringList list = path.split("/", Qt::SkipEmptyParts);
    return list.at(1);
}

bool recon_static_functions::is_live_imager_module_list_file_present(QString mount_path)
{
    // MDFJDFDF3454JKK89K
    QFileInfo info(mount_path + "MDFJDFDF3454JKK89K");
    if(info.exists())
    {
        // recon_static_functions::app_debug(prepare_callerfun(caller_func) + " Is it Live Image? --->>> YES", Q_FUNC_INFO);
        return true;
    }
    else
    {
        //   recon_static_functions::app_debug(prepare_callerfun(caller_func) + " Is it Live Image? --->>> NO", Q_FUNC_INFO);
        return false;
    }

    return false;
}

QString recon_static_functions::get_icon_path_from_colour_name(QString colour , QString caller_func)
{
    QString icon_path;
    icon_path = "../icons/tag_icons/"+ colour.replace(" ","_").toLower() + ".png";
    return icon_path;
}

QString recon_static_functions::get_filesystem_db_create_command(QString table_name)
{
    QString command_str = "CREATE TABLE '"+ table_name +"' (bookmark INTEGER,INT INTEGER PRIMARY KEY,filename VARCHAR(300),filepath VARCHAR(300),"
                                                        "file_parent_path VARCHAR(300), inode_no INTEGER, mime_type VARCHAR(300), "
                                                        "extension VARCHAR(300), file_signature VARCHAR(200), signature_name VARCHAR(200),ocr_text varchar(5000),"
                                                        "z_date_time_modified INTEGER, z_date_time_change INTEGER, z_date_time_accessed INTEGER,"
                                                        "z_date_time_created INTEGER, file_size INTEGER, md5_hash VARCHAR(500),"
                                                        "source_count_name VARCHAR(300), recon_tag_value VARCHAR(100),notes VARCHAR(500),"
                                                        "fileID VARCHAR(200), id_path VARCHAR(200), flags VARCHAR(200),"
                                                        "extracted_file_path VARCHAR(1024), bookmarked_extended_attributes VARCHAR(1024),"
                                                        "bookmarked_exif_data VARCHAR(1024), exif_make VARCHAR(1024),exif_model VARCHAR(1024),"
                                                        "exif_latitude INTEGER,exif_longitude INTEGER, exif_data_update_status VARCHAR(10),"
                                                        "notes_icon VARCHAR(1024), is_seen VARCHAR(100),"
                                                        "extended_attribute_update_status VARCHAR(10), source_file VARCHAR(500),"
                                                        "apple_metadata VARCHAR(500), exif_metadata VARCHAR(500), hashset_name VARCHAR(500),"
                                                        "hash_md5 VARCHAR(500), hash_sha1 VARCHAR(500), extra_dt_no_of_hits INTEGER,"
                                                        "extra_dt_keyword_name VARCHAR(500), extra_dt_total_word_count INTEGER,"
                                                        "extra_dt_hit_locations INTEGER, LINKFILENAME VARCHAR(500), is_dir varchar(5),"
                                                        "permission_restricted_file VARCHAR(500), decompression_status varchar(300),"
                                                        "recon_filefrom varchar(300), recon_file_infopath varchar(300),"
                                                        "fs_module_apple_metadata_run_status varchar(10),"
                                                        "fs_module_exif_metadata_run_status varchar(10),"
                                                        "fs_module_signature_analysis_run_status varchar(10),"
                                                        "fs_module_mime_type_analysis_run_status varchar(10),"
                                                        "fs_module_hashset_run_status varchar(10),"
                                                        "fs_module_face_analysis_run_status varchar(10),"
                                                        "fs_module_skin_tone_detection_run_status varchar(10),"
                                                        "fs_module_optical_character_recognition_run_status varchar(10),"
                                                        "fs_module_fire_analysis_run_status varchar(10),"
                                                        "fs_module_weapons_run_status varchar(10)";
    ///======get extended attribute columns=====///

    QString apl_attributes_csv_path = QString("../Resources/apple_attributes/apple_attributes_list.csv");
    //apl_attributes_csv_path = recon_static_functions::decrypt_library(apl_attributes_csv_path, Q_FUNC_INFO);

    QFile file(apl_attributes_csv_path);

    bool file_open_status = true;
    if(!file.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug(" File Open-------FAILED------ " + file.fileName(),Q_FUNC_INFO);
        recon_static_functions::app_debug(" error :" + file.errorString() , Q_FUNC_INFO);

        file_open_status = false;
        ///do not return
    }

    file.readLine(); // Column Title Line
    int count = 0;

    if(file_open_status)
    {
        while(!file.atEnd())
        {
            count++;

            if(count%10 == 0)
                QCoreApplication::processEvents();

            QString line = file.readLine();

            if(line.endsWith("\n"))
                line.chop(1);
            if(line.endsWith(",,,"))
                line.chop(3);

            QStringList list = line.split(",,,", Qt::SkipEmptyParts);

            QString attribute_name, description, widget_type, kmditem_command,command_type;

            if(list.size() > 0)
            {
                attribute_name = list.at(0);
                attribute_name.remove("\"");
            }
            if(list.size() > 1)
            {
                description = list.at(1);
                description.remove("\"");
            }
            if(list.size() > 2)
            {
                widget_type = list.at(2);
                widget_type.remove("\"");
            }
            if(list.size() > 3)
            {
                kmditem_command = list.at(3);
                kmditem_command.remove("\"");
            }

            if(list.size() > 4)
            {
                command_type = list.at(4);
                command_type.remove("\"");
            }

            kmditem_command = kmditem_command.trimmed();
            if(kmditem_command == QString("TYPE YOUR OWN QUERY"))
                continue;

            QString attribute_str = attribute_name;
            QString column_name = kmditem_command + "_" + attribute_str.replace(" ", "_");

            if(widget_type == QString::number(MACRO_Apple_Attributes_Widget_Type_REAL_VALUE_int)
                    || widget_type == QString::number(MACRO_Apple_Attributes_Widget_Type_SECONDS_VALUE_int)) // for value in flot e.g. 20.05 secods
            {
                command_str.append(QString(",") + column_name + QString(" REAL"));
            }
            else if(widget_type == QString::number(MACRO_Apple_Attributes_Widget_Type_Timestamp_int)) // for timestamp type
            {
                command_str.append(QString(",") + column_name + QString(" INTEGER"));
            }
            else
            {
                command_str.append(QString(",") + column_name + QString(" VARCHAR(200)"));
            }

            if(command_str.endsWith(","))
                command_str.chop(1);

        }
        file.close();
    }
    command_str.append(")");

    // file.remove();
    return command_str;

}


