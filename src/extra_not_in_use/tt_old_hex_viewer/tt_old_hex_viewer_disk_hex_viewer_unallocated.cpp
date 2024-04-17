#include "tt_old_hex_viewer.h"

void hex_viewer::prepare_disk_hex_view_for_unallocated()
{
    recon_static_functions::app_debug(" Start", Q_FUNC_INFO);

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return;
    }

    disk_hex_view_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Hex_View_In_Result_QString).toString() + QString("disk_hex_view_unallocated.sqlite");

    QString pages_count_str = recon_helper_standard_obj->get_string_from_db_by_dbpath(QString("Select max(hex_page_count) from ") + disk_hex_view_unalloc_tbl_str, 0, disk_hex_view_db_path, Q_FUNC_INFO);
    total_no_of_pages = pages_count_str.toLongLong();

    if(total_no_of_pages < 0)
        total_no_of_pages = 0;

    qint64 page_count = 1;

    disk_data_arr = get_data_for_specified_page(page_count);

    if(disk_data_arr.size() > page_size)
        disk_data_arr = disk_data_arr.mid(0, page_size);

    struct_disk_hex_view_data disk_hex_view_data_obj;
    disk_hex_view_data_obj.bool_last_click_was_next = true;
    disk_hex_view_data_obj.current_page_index = page_count;
    disk_hex_view_data_obj.current_page_data  = disk_data_arr;
    disk_hex_view_data_obj.block_start_index  = 0;
    disk_hex_view_data_obj.last_processed_total_blocks = last_processed_total_blocks_unalloc;
    disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

    disk_data_arr.clear();
    list_disk_hex_view_data.clear();
    list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block

    current_offset = 0;
    previous_entered_size = disk_data_arr.size();

    set_file_data_for_unallocated_disk_hex_view();

    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);

}

QByteArray hex_viewer::get_data_for_specified_page(qint64 page_num)
{
    QByteArray page_data_arr;

    if(page_num < 0)
        return page_data_arr;

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    QSqlDatabase destination_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    destination_db.setDatabaseName(disk_hex_view_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text() , Q_FUNC_INFO);
        return page_data_arr;
    }

    QString cmd_select = QString("Select block_count From ") + disk_hex_view_unalloc_tbl_str + QString(" Where hex_page_count = ?");

    QSqlQuery query_destination (destination_db);

    query_destination.prepare(cmd_select);

    query_destination.addBindValue(QString::number(page_num));
    if(!query_destination.exec())
    {
        recon_static_functions::app_debug("Table creation -----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Query:" + query_destination.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_destination.lastError().text() , Q_FUNC_INFO);
        return page_data_arr;
    }

    disk_data_arr.clear();
    last_processed_total_blocks_unalloc = 0;

    int count = 0;
    int processed_blocks_count = 0;

    qint64 block_count = 0;
    QByteArray block_data_arr;
    while(query_destination.next())
    {
        count++;
        if(count % 10 == 0)
        {
            QCoreApplication::processEvents();
            count = 0;
        }
        processed_blocks_count++;

        block_count = query_destination.value("block_count").toLongLong();

        block_data_arr = get_data_for_specified_block(fs_info_ptr, block_count);

        disk_data_arr.append(block_data_arr);
    }
    destination_db.close();

    last_processed_total_blocks_unalloc = processed_blocks_count;

    qint64 data_size_entered = spinBox_page_size->text().trimmed().toLongLong();
    if(disk_data_arr.size() > data_size_entered)
        disk_data_arr = disk_data_arr.mid(0, data_size_entered);

    return disk_data_arr;
}

void hex_viewer::set_file_data_for_unallocated_disk_hex_view()
{
    if(list_disk_hex_view_data.size() < 0)
        return;

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return ;
    }

    struct_disk_hex_view_data disk_hex_view_data_obj =  list_disk_hex_view_data.at(0);
    QByteArray data_arr = disk_hex_view_data_obj.current_page_data;

    if(data_arr.isEmpty())
    {
        pushButton_selection_info_byte_count->setText("");
        pushButton_selection_info_offset->setText("");
        //        lineEdit_8_bit->clear();
        //        lineEdit_16_bit->clear();
        //        lineEdit_32_bit->clear();
        //        lineEdit_binary->clear();
        lineEdit_search->clear();
        //        lineEdit_mac_timestamp->clear();
        //        lineEdit_unix_timestamp->clear();
    }


    //current_offset        = page_size;
    //previous_entered_size = data_arr.size();
    //selected_data_size = data_arr.size();

    setdata(data_arr);


    current_page_count = disk_hex_view_data_obj.current_page_index;

    label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(total_no_of_pages));


    emit signal_current_page_details(current_page_count,0,current_offset-1);

    emit clear_block_index_list();

    fill_the_existing_blocks_details_in_list();

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(true);

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
    }
}

int hex_viewer::get_next_block_for_unallocated_disk_hex_view()
{

    recon_static_functions::app_debug(" Starts", Q_FUNC_INFO);

    int ret_val = -1;

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return ret_val;
    }

    if(list_disk_hex_view_data.size() <= 0)
        return ret_val;


    struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

    qint64 page_count  = disk_hex_view_data_obj.current_page_index + 1;

    disk_data_arr = get_data_for_specified_page(page_count);

    if(disk_data_arr.size() > page_size)
        disk_data_arr = disk_data_arr.mid(0, page_size);

    disk_hex_view_data_obj.bool_last_click_was_next = true;
    disk_hex_view_data_obj.current_page_index = page_count;
    disk_hex_view_data_obj.current_page_data  = disk_data_arr;
    disk_hex_view_data_obj.last_processed_total_blocks = last_processed_total_blocks_unalloc;
    disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

    disk_data_arr.clear();
    list_disk_hex_view_data.clear();
    list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block

    index_list_size = 0;
    lineedit_goto_page->clear();

    setdata(disk_hex_view_data_obj.current_page_data);

    selected_data_size = disk_hex_view_data_obj.current_page_data.size();

    current_page_count = disk_hex_view_data_obj.current_page_index;

    label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(total_no_of_pages));

    emit signal_change_address_area_value(current_offset-1);
    emit signal_current_page_details(current_page_count,current_offset,(current_offset + previous_entered_size - 1));

    current_offset = current_page_count * selected_data_size;
    pushButton_prev_goto_page->setEnabled(true);

    ret_val = 0;

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(true);

    emit set_searched_data_clear(0);

    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);

    return ret_val;
}

int hex_viewer::get_previous_block_for_unallocated_disk_hex_view()
{
    recon_static_functions::app_debug(" Starts", Q_FUNC_INFO);

    int ret_val = -1;

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return ret_val;
    }

    if(list_disk_hex_view_data.size() <= 0)
        return ret_val;


    struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

    qint64 page_count  = disk_hex_view_data_obj.current_page_index - 1;
    if(page_count <= 0)
        page_count = 1;

    disk_data_arr = get_data_for_specified_page(page_count);

    if(disk_data_arr.size() > page_size)
        disk_data_arr = disk_data_arr.mid(0, page_size);

    disk_hex_view_data_obj.bool_last_click_was_next = false;
    disk_hex_view_data_obj.current_page_index = page_count;
    disk_hex_view_data_obj.current_page_data  = disk_data_arr;
    disk_hex_view_data_obj.last_processed_total_blocks = last_processed_total_blocks_unalloc;
    disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

    disk_data_arr.clear();
    list_disk_hex_view_data.clear();
    list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block

    lineedit_goto_page->clear();

    disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

    selected_data_size = page_size;
    current_page_count = disk_hex_view_data_obj.current_page_index;

    qint64 curr_off = current_offset - (previous_entered_size + page_size);
    if(curr_off < 0)
        curr_off = 0;

    current_offset = curr_off;

    setdata(disk_hex_view_data_obj.current_page_data);

    if(current_offset != 0)
    {
        emit signal_change_address_area_value(current_offset-1);
    }
    else
    {
        emit signal_change_address_area_value(current_offset);
    }

    emit signal_current_page_details(current_page_count,current_offset,(current_offset + (page_size -1)));


    ret_val = 0;

    label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(total_no_of_pages));

    previous_entered_size = page_size;
    current_offset = current_page_count * selected_data_size;

    pushButton_next_goto_page->setEnabled(true);

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(true);

    emit set_searched_data_clear(0);

    recon_static_functions::app_debug(" Ends", Q_FUNC_INFO);

    return ret_val;
}

QByteArray hex_viewer::get_data_for_specified_block(TSK_FS_INFO *fs_info_ptr, qint64 blk_count)
{
    QByteArray block_data_arr;

    if(fs_info_ptr == NULL)
        return block_data_arr;

    if(fs_info_ptr->block_count > 0 && blk_count > 0)
    {
        TSK_FS_BLOCK * block_info = tsk_fs_block_get(fs_info_ptr, NULL, (fs_info_ptr->first_block + blk_count));

        if(block_info != NULL)
        {
            //block_data_arr = QByteArray::fromRawData(block_info->buf, sizeof(block_info->buf));
            block_data_arr = block_info->buf;
        }

        tsk_fs_block_free(block_info);
    }
    return block_data_arr;
}

void hex_viewer::go_to_page_for_disk_hex_view_unallocated(qint64 page_num)
{
    qint64 data_size_entered = spinBox_page_size->text().trimmed().toLongLong();

    struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

    disk_hex_view_data_obj.current_page_index = page_num;
    disk_hex_view_data_obj.current_page_data  = get_data_for_specified_page(page_num);
    disk_hex_view_data_obj.last_processed_total_blocks = last_processed_total_blocks_unalloc;
    disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

    list_disk_hex_view_data.clear();
    list_disk_hex_view_data.append(disk_hex_view_data_obj);


    ///get updated data
    disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
    QByteArray current_data_arr =   disk_hex_view_data_obj.current_page_data;

    qint64 page_end_offset      = (page_num * data_size_entered) * disk_hex_view_data_obj.last_processed_total_blocks;
    qint64 page_start_offset    = (page_end_offset - data_size_entered);

    current_offset = (page_start_offset + current_data_arr.size());
    previous_entered_size = current_data_arr.size();
    selected_data_size = current_data_arr.size();

    setdata(current_data_arr);

    uint64_t page_start = current_offset - previous_entered_size;

    if(page_start != 0)
    {
        emit signal_change_address_area_value(page_start-1);
    }
    else
    {
        emit signal_change_address_area_value(page_start);
    }

    current_page_count = page_num;

    emit signal_current_page_details(current_page_count,(page_start),current_offset-1);

    if(current_page_count >= total_no_of_pages)
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

    label_no_of_pages->setText(QString::number(page_num) + " of " + QString::number(total_no_of_pages));
}


hex_viewer::struct_search_page_info hex_viewer::get_available_page_info_for_diskview_unallocated_search(enum_search_clicked click_type,QString searched_type,QString searched_text)
{
    bool_cancel_loading = false;

    struct_search_page_info st_obj;
    st_obj.bool_text_match_found = false;
    st_obj.matched_page_count = -1;

    if(list_disk_hex_view_data.size() <= 0)
        return st_obj;

    bool bool_match_found = false;

    QByteArray searched_hex_arr = QByteArray::fromHex(searched_text.toLocal8Bit());

    struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
    qint64 current_page_pos = disk_hex_view_data_obj.current_page_index;


    if(current_page_pos <= 0)
        return st_obj;


    if(click_type == enum_search_pushbutton_search)
    {

    }
    else if(click_type == enum_search_pushbutton_previous)
    {
        current_page_pos -= 1;

    }
    else if(click_type == enum_search_pushbutton_next)
    {
        current_page_pos += 1;
    }



    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();

    int count = 0;
    bool_disk_hex_data_processed = false;

    ///fetch exact blocks for current page
    disk_data_arr.clear();
    for(qint64 page_count = current_page_pos; page_count < total_no_of_pages; page_count++)
    {
        if(bool_cancel_loading)
            break;

        count++;
        if(count % 10 == 0)
        {
            QCoreApplication::processEvents();
            count = 0;
        }

        disk_data_arr = get_data_for_specified_page(page_count);
        if(disk_data_arr.size() <= 0)
            continue;

        if(searched_type == "ASCII")
        {
            QString ascii;
            for(int pp = 0; pp < disk_data_arr.size(); pp++)
            {
                if(bool_cancel_loading)
                    break;


                if(!(((char)disk_data_arr[pp]<0x20) || ((char)disk_data_arr[pp]>0x7e)))
                {
                    ascii.append(disk_data_arr.at(pp));
                }
                else
                {
                    ascii.append(".");
                }
            }

            if(ascii.toLower().contains(searched_text.toLower()))
            {
                bool_match_found = true;
                st_obj.bool_text_match_found = true;
                st_obj.matched_page_count = page_count;

            }

        }
        else if(searched_type == "HEX")
        {
            if(disk_data_arr.toLower().contains(searched_hex_arr.toLower()))
            {
                bool_match_found = true;
                st_obj.bool_text_match_found = true;
                st_obj.matched_page_count = page_count;

            }
        }

        if(bool_match_found)
        {
            struct_disk_hex_view_data disk_hex_view_data_obj;

            disk_hex_view_data_obj.bool_last_click_was_next = false;
            disk_hex_view_data_obj.current_page_index = page_count;
            disk_hex_view_data_obj.current_page_data  = disk_data_arr;

            disk_hex_view_data_obj.block_start_index = 0;
            disk_hex_view_data_obj.last_processed_total_blocks = last_processed_total_blocks_unalloc;
            disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

            disk_data_arr.clear();
            list_disk_hex_view_data.clear();
            list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block
            break;
        }

    }

    display_loading_progress_bar_obj->hide();
    return st_obj;
}
