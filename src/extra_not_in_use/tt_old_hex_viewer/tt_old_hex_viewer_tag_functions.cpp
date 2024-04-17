#include "tt_old_hex_viewer.h"

void hex_viewer::extract_block_name_list()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QStringList already_applied_tag_list = get_already_applied_tags_list_for_display_in_combobox();

    combobox_display_list.clear();
    combobox_display_list.append("-- Select Tag --");
    combobox_display_list.append(already_applied_tag_list);

    tag_name_with_start_end_index_list = already_applied_tag_list;

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_hex_viewer_tag_start_offset_clicked(int index_value)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    uint64_t index = (current_offset - previous_entered_size) + index_value;

    QString display_path_str = st_globl_recon_file_info_obj.display_file_path;
    QString recon_file_from =  st_globl_recon_file_info_obj.recon_file_from;
    QString recon_file_info_path = st_globl_recon_file_info_obj.recon_file_info_path;

    QString filename = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(display_path_str);


    QStringList values_to_insert;
    values_to_insert << filename << display_path_str << QString::number(index) << QString::number(current_page_count) << hex_viewer_display_name << recon_file_from << recon_file_info_path;
    QString command = "INSERT INTO hex_viewer_blocks(file_name,file_path,block_start_offset,start_page_no, viewer_display_name,recon_filefrom, recon_file_infopath)VALUES(?,?,?,?,?,?,?)";

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,values_to_insert,block_db_path,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("insert table hex_viewer_blocks in start Index ---FAILED---" + block_db_path,Q_FUNC_INFO);
    }

    emit signal_tag_start_index_clicked(true);
    emit signal_set_tag_start_offset(index);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_hex_viewer_tag_end_offset_clicked(QString tag_name, QString color_name, uint64_t start_index, uint64_t end_index)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString color_hex = get_tag_color_hex_from_tag_db(tag_name, color_name);

    if(color_hex.isEmpty())
        return;

    QString file_path_str = st_globl_recon_file_info_obj.display_file_path;

    QString command = "UPDATE hex_viewer_blocks SET block_name = ?, block_end_offset = ?, block_color = ? WHERE file_path = ? AND block_start_offset = ?";
    QStringList val_list;
    val_list << tag_name << QString::number(end_index) << color_hex << file_path_str << QString::number(start_index);

    if(!recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,val_list,block_db_path,Q_FUNC_INFO))
    {
        recon_static_functions::app_debug("insert table hex_viewer_blocks in End Index ---FAILED---" + block_db_path,Q_FUNC_INFO);
    }

    export_bytes_into_tag_temporatrily_and_insert_details_into_tag_search_db(tag_name,color_hex,start_index,end_index);

    emit signal_tag_start_index_clicked(false);

    QString tag_with_index_str = get_tag_name_with_start_and_end_offset_string(tag_name,QString::number(start_index),QString::number(end_index));
    tag_name_with_start_end_index_list << tag_with_index_str;

    while(comboBox_blocks_list->count() != 0)
    {
        comboBox_blocks_list->removeItem(0);
    }

    combobox_display_list.clear();
    combobox_display_list.append("-- Select Tag --");
    combobox_display_list.append(tag_name_with_start_end_index_list);

    comboBox_blocks_list->addItems(combobox_display_list);

    int tag_index = 0;
    if(combobox_display_list.contains(tag_with_index_str))
        tag_index = combobox_display_list.indexOf(tag_with_index_str);

    comboBox_blocks_list->setCurrentIndex(tag_index);
    emit signal_save_openwith_changes_to_bucket(MACRO_CASE_TREE_HEX_VIEWER);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_insert_tag_selected_bytes_info_in_db(int start_index, int end_index, QString tag_name, QString color_name)
{
    if(tag_name.isEmpty())
        return;

    QString color_hex = get_tag_color_hex_from_tag_db(tag_name,color_name);

    emit signal_add_selected_bytes_to_block_clicked(tag_name,color_hex);

    uint64_t start_offset = 0;
    uint64_t end_offset = 0;
    if(bool_is_from_disk_viewer)
    {
        start_offset =  start_index;
        end_offset   =  end_index;
    }
    else
    {
        start_offset = (current_offset - previous_entered_size) + start_index;
        end_offset = (current_offset - previous_entered_size) + end_index;
    }

    QString tag_with_index_str = get_tag_name_with_start_and_end_offset_string(tag_name,QString::number(start_offset),QString::number(end_offset));

    if(!tag_name_with_start_end_index_list.contains(tag_with_index_str))
    {
        QString disp_file_path_str = st_globl_recon_file_info_obj.display_file_path;
        QString recon_file_from    = st_globl_recon_file_info_obj.recon_file_from;
        QString recon_file_info_path_str = st_globl_recon_file_info_obj.recon_file_info_path;

        QString file_name = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(disp_file_path_str);

        if(bool_is_called_from_metadata_display)
            hex_viewer_display_name = file_name + QString(" [") + QString(MACRO_Metadata_TabName_Hex_View) + QString("-") + plugin_name_str  + "-" + tab_name_str + "-" + record_no + QString("]");
        else if(bool_is_called_from_detailed_info)
            hex_viewer_display_name = file_name + QString(" [") + QString(MACRO_Metadata_TabName_Detailed_Information) + QString("-") + QString(MACRO_Metadata_TabName_Hex_View) + QString("-") + plugin_name_str  + "-" + tab_name_str + "-" + record_no + QString("]");
        else if(plugin_name_str == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
        {
            hex_viewer_display_name = tab_name_str;
        }
        else
            hex_viewer_display_name = file_name + QString(" [") +  plugin_name_str + "-" + tab_name_str + "-" + record_no + QString("]");

        QString select_cmd = "Select INT from hex_viewer_blocks where file_name = ? "
                             "AND file_path = ? AND block_name = ? AND block_start_offset = ? "
                             "AND block_end_offset = ? AND block_color = ? "
                             "AND start_page_no = ? AND viewer_display_name = ?";
        QStringList arg_list;

        arg_list << file_name
                 << disp_file_path_str
                 << tag_name
                 << QString::number(start_offset)
                 << QString::number(end_offset)
                 << color_hex
                 << QString::number(current_page_count)
                 << hex_viewer_display_name ;

        QString INT_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(select_cmd,arg_list,0,block_db_path,Q_FUNC_INFO);
        if(!INT_str.trimmed().isEmpty())
        {
            QStringList values_to_update;
            values_to_update <<  tag_name << QString::number(start_offset) << QString::number(end_offset) << color_hex << QString::number(current_page_count);


            QString update_command = "UPDATE hex_viewer_blocks Set block_name = ?,block_start_offset = ?,block_end_offset = ? ,block_color = ?,start_page_no = ? "
                                     "Where viewer_display_name = ? AND INT = ?";

            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(update_command,values_to_update,block_db_path,Q_FUNC_INFO);
        }
        else
        {
            QStringList values_to_insert;
            values_to_insert << file_name << disp_file_path_str << tag_name << QString::number(start_offset) << QString::number(end_offset) << "" << color_hex << QString::number(current_page_count) << hex_viewer_display_name << recon_file_from << recon_file_info_path_str;
            QString command = "INSERT INTO hex_viewer_blocks(file_name,file_path,block_name,block_start_offset,block_end_offset,block_output_path,block_color,start_page_no, viewer_display_name,recon_filefrom, recon_file_infopath)VALUES(?,?,?,?,?,?,?,?, ?,?,?)";
            recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,values_to_insert,block_db_path,Q_FUNC_INFO);
        }
        tag_name_with_start_end_index_list << tag_with_index_str;

        export_bytes_into_tag_temporatrily_and_insert_details_into_tag_search_db(tag_name,color_hex,start_offset,end_offset);

    }

    while(comboBox_blocks_list->count() != 0)
    {
        comboBox_blocks_list->removeItem(0);
    }

    combobox_display_list.clear();
    combobox_display_list.append("-- Select Tag --");
    combobox_display_list.append(tag_name_with_start_end_index_list);

    comboBox_blocks_list->addItems(combobox_display_list);

    int tag_index = 0;
    if(combobox_display_list.contains(tag_with_index_str))
        tag_index = combobox_display_list.indexOf(tag_with_index_str);

    comboBox_blocks_list->setCurrentIndex(tag_index);

    //    if(bool_is_from_disk_viewer)
    //    {
    //       go_to_offset_for_disk_hex_viewer(start_offset);
    //    }

}

void hex_viewer::slot_export_clicked(QString output_path)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    export_tag_module_obj->hide();

    QFile myfile;
    QByteArray exported_data_arr;
    if(bool_is_from_disk_viewer)
    {
        if(list_disk_hex_view_data.size() <= 0)
            return;

        struct_disk_hex_view_data struct_disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

        exported_data_arr = struct_disk_hex_view_data_obj.current_page_data;
    }
    else
    {
        myfile.setFileName(file_path);
        if(!myfile.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error" + myfile.errorString(),Q_FUNC_INFO);
            return;
        }
    }

    QString name_of_tag = selected_block_name;

    if(name_of_tag.contains(" "))
        name_of_tag.replace(" ", "_");

    if(tag_end_index_in_file >= tag_start_index_in_file)
    {
        qint64 data_size = tag_end_index_in_file - tag_start_index_in_file;
        if(data_size == 0)
            data_size = 1;
        else
            data_size += 1;

        if(bool_is_from_disk_viewer)
        {
            exported_data_arr = exported_data_arr.mid(tag_start_index_in_file, data_size);
        }
        else
        {
            myfile.seek(tag_start_index_in_file);
            exported_data_arr = myfile.read(data_size + 1);
        }
    }
    else if(tag_start_index_in_file > tag_end_index_in_file)
    {
        qint64 data_size = tag_start_index_in_file - tag_end_index_in_file;
        if(bool_is_from_disk_viewer)
        {
            exported_data_arr = exported_data_arr.mid(tag_start_index_in_file, data_size);
        }
        else
        {
            myfile.seek(tag_end_index_in_file);
            exported_data_arr = myfile.read(data_size + 1);
        }
    }

    QString final_output_path = output_path + "/" + name_of_tag + "_" + QDateTime::currentDateTime().toString("MM-dd-yyyy-hh-mm-ss");

    QFile output_file(final_output_path);
    if(!output_file.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + final_output_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + output_file.errorString(),Q_FUNC_INFO);
        myfile.close();
        return;
    }

    output_file.write(exported_data_arr);

    output_file.flush();
    output_file.close();

    myfile.close();

    QString command = "UPDATE hex_viewer_blocks SET block_output_path=? WHERE block_name = ? AND block_start_offset = ? AND block_end_offset = ? AND file_path=?";
    QStringList arg_list;
    arg_list << final_output_path << selected_block_name << QString::number(tag_start_index_in_file) << QString::number(tag_end_index_in_file) << file_path;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,block_db_path,Q_FUNC_INFO);

    message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,QStringList(final_output_path), final_output_path);
    message_dialog_obj->show();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_export_selected_bytes(QByteArray selected_bytes)
{
    export_selected_bytes_obj->pub_set_essentials();
    export_selected_bytes_obj->pub_clear_line_edit_text();
    export_selected_bytes_obj->show();
    export_selected_bytes_arr = selected_bytes;
}

void hex_viewer::slot_export_bytes_save_clicked(QString file_name,QString file_path)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    export_selected_bytes_obj->hide();

    if(file_name.contains("."))
        file_name.replace(".","_");

    if(file_name.contains(" "))
        file_name.replace(" ","_");

    QString total_path = file_path + "/" + file_name;

    QFile myfile(total_path);
    if(!myfile.open(QIODevice::ReadWrite))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + total_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return;
    }

    myfile.write(export_selected_bytes_arr);
    myfile.flush();
    myfile.close();

    message_dialog_obj->pub_set_file_path_message(MACRO_MSG_INFORMATION,QStringList(total_path), file_path);
    message_dialog_obj->show();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_pushbutton_export_clicked()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString current_block_name = comboBox_blocks_list->currentText();
    if(comboBox_blocks_list->currentIndex() < 1)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please select a tag to export");
        message_dialog_obj->show();
        return;
    }

    QStringList splitted_list = current_block_name.split("--");

    if(splitted_list.size() < 2)
        return;

    QString tag_name = splitted_list.at(0);

    QString index_info_str = splitted_list.at(1);

    QStringList index_splitted_list = index_info_str.split(" ");

    if(index_splitted_list.size() < 4)
        return;

    selected_block_name = tag_name;

    tag_start_index_in_file = index_splitted_list.at(1).toULongLong();
    tag_end_index_in_file = index_splitted_list.at(3).toULongLong();

    export_tag_module_obj->pub_set_recon_result_dir_path(recon_result_dir_path);
    export_tag_module_obj->pub_clear_line_edit_text();
    export_tag_module_obj->show();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::slot_pushbutton_remove_block_clicked()
{

    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    if(comboBox_blocks_list->currentIndex() < 1)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please select a tag to remove");
        message_dialog_obj->show();
        return;
    }

    QString current_block_name = comboBox_blocks_list->currentText();

    QStringList splitted_list = current_block_name.split("--");

    if(splitted_list.size() < 2)
        return;

    QString tag_name = splitted_list.at(0);

    QString index_info_str = splitted_list.at(1);

    QStringList index_splitted_list = index_info_str.split(" ");

    if(index_splitted_list.size() < 4)
        return;

    QString start_index_str = index_splitted_list.at(1);
    QString end_index_str = index_splitted_list.at(3);

    QString file_path_str = st_globl_recon_file_info_obj.display_file_path;

    QString command = "DELETE FROM hex_viewer_blocks WHERE block_name = ? AND block_start_offset = ? AND block_end_offset = ? AND file_path = ?";
    QStringList arg_list;
    arg_list << tag_name << start_index_str << end_index_str << file_path_str;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbpath(command,arg_list,block_db_path,Q_FUNC_INFO);

    comboBox_blocks_list->removeItem(comboBox_blocks_list->currentIndex());

    tag_name_with_start_end_index_list = get_already_applied_tags_list_for_display_in_combobox();

    fill_the_existing_blocks_details_in_list();

    remove_this_tag_entry_in_other_db_and_display(tag_name, start_index_str, end_index_str, file_path_str);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

void hex_viewer::fill_the_existing_blocks_details_in_list()
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(block_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QList<struct_global_hex_viewer_start_and_end_index> list_values;
    QString file_path_str = st_globl_recon_file_info_obj.display_file_path;

    QSqlQuery query_select_blocks_info(destination_db);

    //QString command = QString("SELECT block_name,block_start_offset,block_end_offset,block_color FROM hex_viewer_blocks WHERE file_path = ?");
    QString command = QString("SELECT block_name,block_start_offset,block_end_offset,block_color FROM hex_viewer_blocks WHERE file_path = ? AND start_page_no = ?");

    if(bool_is_from_disk_viewer)
        command = QString("SELECT block_name,block_start_offset,block_end_offset,block_color FROM hex_viewer_blocks WHERE file_path = ? AND viewer_display_name = ? AND start_page_no = ?");


    query_select_blocks_info.prepare(command);
    query_select_blocks_info.addBindValue(file_path_str);

    if(bool_is_from_disk_viewer)
    {
        if(bool_disk_viewer_unallocated_disk)
            query_select_blocks_info.addBindValue(QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Unallocated));
        else
            query_select_blocks_info.addBindValue(QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete));


        int page_no = 0;
        if(list_disk_hex_view_data.size() > 0)
        {
            page_no = list_disk_hex_view_data.at(0).current_page_index;
        }
        // query_select_blocks_info.addBindValue(QString::number(page_no));

    }
    query_select_blocks_info.addBindValue(QString::number(current_page_count));


    if(!query_select_blocks_info.exec())
    {
        recon_static_functions::app_debug("query_select_blocks_info execution-----FAILED----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select_blocks_info.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(query_select_blocks_info.executedQuery() ,Q_FUNC_INFO);
        destination_db.close();
        return;
    }

    uint64_t page_size = spinBox_page_size->text().toULongLong();

    while(query_select_blocks_info.next())
    {
        struct_global_hex_viewer_start_and_end_index struct_obj;
        struct_obj.current_block_name                   = query_select_blocks_info.value(0).toString();
        struct_obj.block_actual_start_offset_in_file    = query_select_blocks_info.value(1).toULongLong();
        struct_obj.block_actual_end_offset_in_file      = query_select_blocks_info.value(2).toULongLong();

        QColor tmp_color;
        tmp_color.setNamedColor(query_select_blocks_info.value(3).toString());

        struct_obj.block_highlighting_color             = tmp_color;

        uint64_t start_index_of_page                    = query_select_blocks_info.value(1).toULongLong()%page_size;
        uint64_t end_index_of_page                      = query_select_blocks_info.value(2).toULongLong()%page_size;

        struct_obj.start_index                          = start_index_of_page;
        struct_obj.end_index                            = end_index_of_page;

        list_values.append(struct_obj);
    }

    destination_db.close();

    if(list_values.size() <= 0)
        label_block_start_end_index->setText("");

    emit signal_set_blocks_list_for_this_file(list_values);

    hex_edit_obj->set_block_db_path(block_db_path);

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}


void hex_viewer::slot_combobox_export_block_index_change(QString text)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    if(comboBox_blocks_list->currentIndex() < 1)
    {
        label_block_start_end_index->setText("");
        return;
    }

    QStringList splitted_list = text.split("--");

    if(splitted_list.size() < 2)
        return;

    QString tag_name = splitted_list.at(0);

    QString index_info_str = splitted_list.at(1);

    QStringList index_splitted_list = index_info_str.split(" ");

    if(index_splitted_list.size() < 4)
        return;

    QString start_index_offset = index_splitted_list.at(1);
    QString end_index_offset = index_splitted_list.at(3);
    QString file_path_str = st_globl_recon_file_info_obj.display_file_path;

    QString command ;
    QStringList arg_list;
    arg_list << tag_name << start_index_offset << end_index_offset << file_path_str;
    if(bool_is_from_disk_viewer)
    {
        command = "SELECT start_page_no FROM hex_viewer_blocks WHERE block_name = ? AND block_start_offset = ? AND block_end_offset = ? AND file_path=? AND viewer_display_name = ?";

        if(bool_disk_viewer_unallocated_disk)
            arg_list << QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Unallocated);
        else
            arg_list << QString(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete);
    }
    else
    {
        command = "SELECT start_page_no FROM hex_viewer_blocks WHERE block_name = ? AND block_start_offset = ? AND block_end_offset = ? AND file_path=?";
    }
    QString page_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,arg_list,0,block_db_path,Q_FUNC_INFO);

    QString display_str = start_index_offset + " - " + end_index_offset;
    label_block_start_end_index->setText(display_str);

    qint64 enter_page_index = page_no.toLongLong();

    qint64 data_size_entered = spinBox_page_size->text().trimmed().toLongLong();

    qint64 total_size = 0;
    QFile myfile;
    if(plugin_name_str == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
    {
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

        total_size = struct_source_info.total_size_byte_numeric_QString.toLongLong();
    }
    else
    {
        myfile.setFileName(file_path);
        if(!myfile.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("file open ----  FAILED ----",Q_FUNC_INFO);
            recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
            return;
        }

        total_size = myfile.size();
    }

    int temp_no_of_pages = 0;
    if(bool_is_from_disk_viewer)
    {
        temp_no_of_pages = total_no_of_pages;
    }
    else
    {
        temp_no_of_pages = total_size / data_size_entered;
        if((total_size % data_size_entered) != 0)
        {
            temp_no_of_pages += 1;
        }
    }

    qint64 page_end_offset      = enter_page_index * data_size_entered;
    qint64 page_start_offset    = page_end_offset - data_size_entered;

    QByteArray required_bytes;

    if(plugin_name_str == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
    {
        required_bytes = hex_edit_obj->data();
        if(required_bytes.trimmed().isEmpty() && list_disk_hex_view_data.size() > 0)
        {
            struct_disk_hex_view_data struct_disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
            required_bytes   = struct_disk_hex_view_data_obj.current_page_data;
        }
    }
    else
    {
        myfile.seek(page_start_offset);
        required_bytes = myfile.read(data_size_entered);
        myfile.close();
    }

    if(!bool_is_from_disk_viewer)
        current_offset = (page_start_offset + required_bytes.size());


    previous_entered_size = required_bytes.size();
    selected_data_size = required_bytes.size();

    setdata(required_bytes);

    uint64_t page_start = current_offset - previous_entered_size;

    if(page_start != 0)
    {
        emit signal_change_address_area_value(page_start-1);
    }
    else
    {
        emit signal_change_address_area_value(page_start);
    }

    current_page_count = enter_page_index;

    emit signal_current_page_details(current_page_count,(page_start),current_offset-1);

    fill_the_existing_blocks_details_in_list();

    uint64_t start_index = start_index_offset.toULongLong();

    start_index -= page_start;

    emit go_to_offset(start_index,selected_data_size);

    if(current_page_count >= temp_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
        pushButton_prev_goto_page->setEnabled(true);
    }
    else if(current_page_count <= 1)
    {
        pushButton_prev_goto_page->setEnabled(false);
        pushButton_next_goto_page->setEnabled(true);
    }
    else
    {
        pushButton_prev_goto_page->setEnabled(true);
        pushButton_next_goto_page->setEnabled(true);
    }
    label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(temp_no_of_pages));

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

QString hex_viewer::get_tag_color_hex_from_tag_db(QString tag_name,QString color_name)
{
    QString tags_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Case_Configuration_In_Result_QString).toString() + "case_configuration.sqlite";

    QString colour_hex;

    QString command = "SELECT color_hex FROM tags WHERE tag_name=? AND selected_colour=?";
    QStringList val_list;
    val_list << tag_name << color_name;

    colour_hex = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command,val_list,0,tags_db_path,Q_FUNC_INFO);

    return colour_hex;
}

QString hex_viewer::get_tag_name_with_start_and_end_offset_string(QString tag_name, QString start_index_str, QString end_index_str)
{
    QString final_str;

    final_str = tag_name + "--( " + start_index_str + " - " + end_index_str + " )";

    return final_str;
}

QStringList hex_viewer::get_already_applied_tags_list_for_display_in_combobox()
{
    QStringList tags_list_with_start_and_end_index;

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(block_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Destination Db opening --------FAILED------ " + block_db_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + destination_db.lastError().text(),Q_FUNC_INFO);
        return tags_list_with_start_and_end_index;
    }

    QString file_path_str = st_globl_recon_file_info_obj.display_file_path;

    QSqlQuery query(destination_db);

    QString command;
    if(bool_is_from_disk_viewer)
    {
        command = QString("SELECT block_name,block_start_offset,block_end_offset FROM hex_viewer_blocks  WHERE file_path=? AND viewer_display_name = ?");
    }
    else
    {
        command = QString("SELECT block_name,block_start_offset,block_end_offset FROM hex_viewer_blocks  WHERE file_path=?");
    }

    query.prepare(command);
    query.addBindValue(file_path_str);

    if(bool_is_from_disk_viewer)
    {
        if(bool_disk_viewer_unallocated_disk)
            query.addBindValue(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Unallocated);
        else
            query.addBindValue(MACRO_Plugin_Disk_Hex_Viewer_TAB_Name_Complete);
    }

    if(!query.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query.executedQuery(),Q_FUNC_INFO);
        destination_db.close();
        return tags_list_with_start_and_end_index;
    }

    while(query.next())
    {
        tags_list_with_start_and_end_index << get_tag_name_with_start_and_end_offset_string(query.value(0).toString(),query.value(1).toString(),query.value(2).toString());
    }
    destination_db.close();

    return tags_list_with_start_and_end_index;

}

void hex_viewer::export_bytes_into_tag_temporatrily_and_insert_details_into_tag_search_db(QString tag_name, QString color_hex, uint64_t start_index, uint64_t end_index)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);


    QByteArray selected_array;

    if(bool_is_from_disk_viewer)
    {
        if(list_disk_hex_view_data.size() <= 0)
            return;

        struct_disk_hex_view_data struct_disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

        QByteArray current_data   = struct_disk_hex_view_data_obj.current_page_data;

        qint64 out_size = (end_index - (start_index + 1));

        selected_array = current_data.mid(start_index, out_size);
    }
    else
    {
        QFile myfile(file_path);
        if(!myfile.open(QIODevice::ReadOnly))
        {
            recon_static_functions::app_debug("file open ---FAILED--- " + file_path,Q_FUNC_INFO);
            recon_static_functions::app_debug("error" + myfile.errorString(),Q_FUNC_INFO);
            return;
        }

        myfile.seek(start_index);

        selected_array = myfile.read(end_index - start_index + 1);

        myfile.close();
    }

    QString file_dir_name = hex_viewer_display_name;
    file_dir_name.replace(" ","_");

    QString content_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + file_dir_name;
    QDir dir(content_dir_path);

    if(!dir.exists())
        dir.mkdir(content_dir_path);

    QString tag_name_tmp = tag_name;

    QString file_name = tag_name_tmp.replace(" ","_") + "_" + QString::number(start_index) + "_" + QString::number(end_index);

    QString export_file_path = content_dir_path + "/" + file_name;

    QFile file_write(export_file_path);
    if(!file_write.open(QIODevice::WriteOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + export_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file_write.errorString(),Q_FUNC_INFO);
        return;
    }

    file_write.write(selected_array);
    file_write.flush();
    file_write.close();


    // Copy full bytearray to string
    std::string std_str(selected_array.constData(), selected_array.length());
    QString selected_data_str = QString::fromStdString(std_str);


    QString content_export_db_path = insert_exported_content_info_in_db(tag_name, color_hex, start_index, end_index, export_file_path);

    QString command = "SELECT INT FROM hex_content_info ORDER BY INT DESC LIMIT 1";
    QString record_count = recon_helper_standard_obj->get_string_from_db_by_dbpath(command,0,content_export_db_path,Q_FUNC_INFO);

    struct_GLOBAL_witness_info_source struct_tgt_src_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    QString file_path_str = st_globl_recon_file_info_obj.display_file_path;


    QList<struct_global_tagging_tag_notes_db_updation> list_st_global_tagging_db_updation;
    struct_global_tagging_tag_notes_db_updation obj;
    obj.plugin_name = QString(MACRO_Plugin_Name_Hex_Viewer);
    if(bool_is_from_disk_viewer)
        obj.category = hex_viewer_display_name;
    else
        obj.category = QString(MACRO_Plugin_Name_Hex_Viewer);

    obj.tab_name = hex_viewer_display_name;
    obj.table_name = "hex_content_info";
    obj.record_no = record_count;


    obj.item0 = QFileInfo(file_path_str).fileName();
    obj.item1 = file_path_str;
    obj.item2 = QString::number(start_index);
    obj.item3 = QString::number(end_index);
    obj.item4 = selected_data_str;


    obj.source_count_name = source_count_name;
    obj.os_scheme_display = struct_tgt_src_info.os_scheme_display;

    obj.csv_plugin_name = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.csv_tab_name = QString(MACRO_Plugin_Name_Hex_Viewer);
    obj.recon_tag_value = tag_name;


    list_st_global_tagging_db_updation << obj;

    global_connection_manager_obj->pub_add_tags_for_hex_viewer(list_st_global_tagging_db_updation , tag_name);
    emit signal_save_openwith_changes_to_bucket(QString(MACRO_CASE_TREE_HEX_VIEWER));

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}

QString hex_viewer::insert_exported_content_info_in_db(QString tag_name, QString color_hex, uint64_t start_index, uint64_t end_index, QString export_file_path)
{
    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

    QFileInfo info(exported_content_info_db);
    if(!info.exists())
    {
        QString command = "CREATE TABLE 'hex_content_info'(INT INTEGER PRIMARY KEY,bookmark,file_name VARCHAR(1000), file_path VARCHAR(1000),"
                          "file_size VARCHAR(1000), start_index VARCHAR(1000), end_index VARCHAR(1000), recon_tag_value VARCHAR(1000),"
                          "color_hex VARCHAR(1000), content_file_path VARCHAR(1000), source_count_name VARCHAR(1000), viewer_display_name VARCHAR(1000), notes VARCHAR(1000), hex_data BLOB,"
                          "LINKFILENAME VARCHAR(1000), source_file VARCHAR(1000) , recon_filefrom varchar(10),recon_file_infopath varchar(300))";

        if(!recon_helper_standard_obj->execute_db_command_by_dbpath(command,exported_content_info_db,Q_FUNC_INFO))
        {
            recon_static_functions::app_debug("CREATE table hex_content_info in hex viewer ---FAILED---" + exported_content_info_db,Q_FUNC_INFO);
        }


        QString export_file_cmd = recon_static_functions::get_exportfilelist_command();
        recon_helper_standard_obj->execute_db_command_by_dbpath(export_file_cmd , exported_content_info_db, Q_FUNC_INFO);

    }

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + exported_content_info_db , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return "";
    }



    QString disp_file_path_str = st_globl_recon_file_info_obj.display_file_path;
    QString recon_file_from =  st_globl_recon_file_info_obj.recon_file_from;
    QString recon_file_info_path = st_globl_recon_file_info_obj.recon_file_info_path;

    QString filename_str;
    qint64 file_size = 0;
    if(plugin_name_str == QString(MACRO_Plugin_Name_Disk_Hex_Viewer))
    {
        filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(disp_file_path_str);
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        file_size = struct_source_info.total_size_byte_numeric_QString.toLongLong();
    }
    else
    {
        filename_str = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(file_path);
        file_size = QFile(file_path).size();
    }

    QFile myfile(export_file_path);
    if(!myfile.open(QIODevice::ReadOnly))
    {
        recon_static_functions::app_debug("file open ---FAILED--- " + export_file_path,Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + myfile.errorString(),Q_FUNC_INFO);
        return exported_content_info_db;
    }

    QByteArray export_data_arr = myfile.readAll();

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();

    if(export_file_path.startsWith(result_dir_path))
        export_file_path.remove(0, result_dir_path.size());

    export_file_path = export_file_path.trimmed();
    if(!export_file_path.isEmpty() && !export_file_path.startsWith("/"))
        export_file_path.prepend("/");

    QSqlQuery query_insert(destination_db);
    query_insert.prepare("INSERT INTO 'hex_content_info'(bookmark,file_name, file_path, file_size, start_index, end_index, recon_tag_value,"
                         "color_hex, content_file_path, source_count_name, viewer_display_name, hex_data,source_file,recon_filefrom, recon_file_infopath) VALUES(?,?,?,?,? ,?,?,?,?,?, ?,?,?,?,?)");

    query_insert.addBindValue("0");
    query_insert.addBindValue(filename_str);
    query_insert.addBindValue(disp_file_path_str);
    query_insert.addBindValue(file_size);
    query_insert.addBindValue(QString::number(start_index));
    query_insert.addBindValue(QString::number(end_index));
    query_insert.addBindValue(tag_name);
    query_insert.addBindValue(color_hex);
    query_insert.addBindValue(export_file_path);
    query_insert.addBindValue(source_count_name);
    query_insert.addBindValue(hex_viewer_display_name);
    query_insert.addBindValue(export_data_arr.toHex());
    query_insert.addBindValue(disp_file_path_str);
    query_insert.addBindValue(recon_file_from);
    query_insert.addBindValue(recon_file_info_path);

    if(!query_insert.exec())
    {
        recon_static_functions::app_debug("query_index.exec() --------FAILED------ " ,Q_FUNC_INFO);
        recon_static_functions::app_debug("error -  " + query_insert.lastError().text(),Q_FUNC_INFO);
        recon_static_functions::app_debug("Query -  " + query_insert.executedQuery(),Q_FUNC_INFO);
    }

    myfile.close();
    destination_db.close();
    return exported_content_info_db;
}

void hex_viewer::remove_this_tag_entry_in_other_db_and_display(QString tag_name, QString start_index_str, QString end_index_str, QString file_path_str)
{
    recon_static_functions::app_debug("start ",Q_FUNC_INFO);

    QString exported_content_info_db = global_narad_muni_class_obj->get_field(MACRO_NARAD_Exported_Content_Location_Hex_Viewer_QString).toString() + "hex_viewer_content_info.sqlite";

    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(exported_content_info_db);

    if(!destination_db.open())
    {
        recon_static_functions::app_debug("db open---FAILED----" + exported_content_info_db , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return;
    }

    QString command_select = "SELECT INT FROM hex_content_info WHERE file_path = ? AND start_index = ? AND end_index = ? AND recon_tag_value = ?";
    QStringList arg_list;
    arg_list << file_path_str << start_index_str << end_index_str << tag_name;
    QString record_no = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_select,arg_list,0,destination_db,Q_FUNC_INFO);

    QString os_schme_dsply = QString(MACRO_GLOBAL_RECON_AND_PLUGIN_COMMON_OS_SCHEME_otherOS_Display);
    if(bool_is_from_disk_viewer)
    {
        struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
        os_schme_dsply = struct_source_info.os_scheme_display;
    }
    emit signal_remove_tag_entry_from_tag_search_db_and_display(QString(MACRO_Plugin_Name_Hex_Viewer), hex_viewer_display_name, record_no, QStringList(tag_name), os_schme_dsply);


    /// REMOVE File from content Dir
    command_select = "SELECT content_file_path FROM hex_content_info WHERE file_path = ? AND start_index = ? AND end_index = ? AND recon_tag_value = ?";
    QString content_file_path = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_select,arg_list,0,destination_db,Q_FUNC_INFO);

    QString result_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_RECON_Result_Dir_QString).toString();
    if(!content_file_path.isEmpty() && !content_file_path.startsWith(result_dir_path))
        content_file_path.prepend(result_dir_path);

    /// REMOVE Notes entry from notes search db and display
    command_select = "SELECT notes FROM hex_content_info WHERE file_path = ? AND start_index = ? AND end_index = ? AND recon_tag_value = ?";
    QString notes_values = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbreference(command_select,arg_list,0,destination_db,Q_FUNC_INFO);

    notes_values = notes_values.trimmed();

    emit signal_remove_tag_entry_from_notes_db_and_display(QString(MACRO_Plugin_Name_Hex_Viewer), hex_viewer_display_name, record_no, notes_values, os_schme_dsply);

    /// REMOVE FILE
    QFile file_temp(content_file_path);

    if(file_temp.exists())
        file_temp.remove();

    /// REMOVE entry in content Dir db
    command_select = "DELETE FROM hex_content_info WHERE INT=?";
    arg_list.clear();
    arg_list << record_no;
    recon_helper_standard_obj->execute_db_command_with_addbindvalues_by_dbreference(command_select,arg_list,destination_db,Q_FUNC_INFO);

    destination_db.close();

    recon_static_functions::app_debug("end ",Q_FUNC_INFO);
}
