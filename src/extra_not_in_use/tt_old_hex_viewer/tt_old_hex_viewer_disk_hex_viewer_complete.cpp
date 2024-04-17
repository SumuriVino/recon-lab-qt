#include "tt_old_hex_viewer.h"

QList<struct_global_tsk_part_info_fuse> global_part_list;

///creates final list of data to be inserted in database
static TSK_WALK_RET_ENUM vs_act(TSK_VS_INFO *vs_info, const TSK_VS_PART_INFO *vs_partition, void *ptr)
{
    struct_global_tsk_part_info_fuse struct_info;

    struct_info.part_info_ptr = (TSK_VS_PART_INFO *)vs_partition;
    struct_info.size = vs_partition->len * vs_info->block_size;

    struct_info.bool_fs_found = false;
    struct_info.fs_name = "";
    struct_info.fs_info_ptr = NULL;
    struct_info.bool_partition_found = false;

    TSK_FS_INFO *vs_fs_opened;
    if(vs_partition->flags & TSK_VS_PART_FLAG_ALLOC)
    {
        struct_info.bool_partition_found = true;
        if (!(vs_fs_opened = tsk_fs_open_vol(vs_partition, TSK_FS_TYPE_DETECT))) {
            ;
        }
        else
        {
            struct_info.fs_info_ptr = vs_fs_opened;
            struct_info.bool_fs_found = true;
            struct_info.fs_name = tsk_fs_type_toname(vs_fs_opened->ftype);
        }
    }

    global_part_list.append(struct_info);

    return (TSK_WALK_RET_ENUM)0;
}


void hex_viewer::pub_set_file_data_for_complete_disk_viewer(QString disk_node)
{
    recon_static_functions::app_debug("start - dev node " + disk_node, Q_FUNC_INFO);

    groupBox_open_in_other_recon_viewers->hide();

    QString tmp_disk_node = disk_node;
    if(tmp_disk_node.startsWith("/dev/r"))
        tmp_disk_node.remove(0, QString("/dev/r").size());

    tmp_disk_node = tmp_disk_node.trimmed();

    if(tmp_disk_node.isEmpty())
    {
        QMessageBox::warning(this, recon_static_functions::get_app_name(), "No Disk found for 'Disk Hex Viewer'.");
        return;
    }

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);


    QString display_node_str = QString("/dev/" + tmp_disk_node);

    label_file_name->setText("<b>Device Name : </b>" + tmp_disk_node);
    label_file_path->setText("<b>Device Path : </b>" + display_node_str);

    pushButton_prev_goto_page->setEnabled(false);

    qint64 device_size = 0;
    if(bool_disk_viewer_complete_disk)
    {
        QString source_path = struct_source_info.source_path_1;
        if(struct_source_info.bool_source_nested)
        {
            struct_GLOBAL_witness_info_source prnt_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(struct_source_info.parent_source_count_name);
            source_path = recon_static_functions::get_complete_file_path(prnt_source_info.virtual_source_path,struct_source_info.source_path_1,Q_FUNC_INFO);
        }

        device_size = QFile(source_path).size();
    }
    else
        device_size = struct_source_info.total_size_byte_numeric_QString.toLongLong();

    total_file_size = device_size;

    label_file_size->setText("<b>Device Size : </b>" + recon_static_functions::human_readable_size(QString::number(device_size), Q_FUNC_INFO));

    bool_disk_hex_view_first_load = true;

    current_offset = 0;

    display_loading_progress_bar_non_cancelable_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_non_cancelable_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_non_cancelable_obj->show();

    tsk_start_image_processing(disk_node);

    if(bool_disk_viewer_unallocated_disk)
    {
        list_disk_hex_view_data.clear();

        prepare_disk_hex_view_for_unallocated();
    }
    else
    {
        get_blocks_for_complete_disk_hex_view(fs_info_ptr);
    }

    display_loading_progress_bar_non_cancelable_obj->hide();


    recon_static_functions::app_debug("end - dev node " + disk_node, Q_FUNC_INFO);

}


void hex_viewer::pub_set_disk_viewer_essentials(bool is_disk_viewer, bool is_complete_disk, bool bool_is_unallocated_disk , QString unalloc_tbl_name)
{
    bool_is_from_disk_viewer          = is_disk_viewer;
    bool_disk_viewer_complete_disk    = is_complete_disk;
    bool_disk_viewer_unallocated_disk = bool_is_unallocated_disk;

    disk_hex_view_unalloc_tbl_str = unalloc_tbl_name;

}

///opens image and its partitions
int hex_viewer::tsk_start_image_processing(QString image_path_str)
{
    ///opening source Image
    if(img_info_ptr)
        tsk_img_close(img_info_ptr);

    img_info_ptr = tsk_img_open_sing(image_path_str.toStdString().c_str(), TSK_IMG_TYPE_DETECT, 0);
    if(!img_info_ptr)
    {
        recon_static_functions::app_debug("----FAILED---- Image is faulty " + image_path_str, Q_FUNC_INFO);
        return -1;
    }

    recon_static_functions::app_debug("======Image Opened Succesfully=====", Q_FUNC_INFO);

    ///opening volumes in Image
    global_part_list.clear();

    if(bool_disk_viewer_complete_disk)
        list_disk_hex_view_data.clear();


    if(vs_info_ptr)
        tsk_vs_close(vs_info_ptr);

    vs_info_ptr = tsk_vs_open(img_info_ptr, 0, TSK_VS_TYPE_DETECT);
    if(!vs_info_ptr)
    {
        recon_static_functions::app_debug("No further volumes found, trying for filesystem directly.", Q_FUNC_INFO);

        // in case image is the partition itself, so jump to filesystem directly
        int ret = tsk_process_volume_filesystem(img_info_ptr , 0);
        if(ret < 0)
        {
            recon_static_functions::app_debug("----FAILED---- No partition and filesystem found", Q_FUNC_INFO);
            tsk_img_close(img_info_ptr);
            return -1;
        }
    }
    else
    {
        /// work for multi partitioned image
        int count = 0;
        for(TSK_PNUM_T part_num = 0; part_num < vs_info_ptr->part_count; part_num++)
        {
            count++;
            if(count % 10 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            if(bool_disk_hex_data_processed)
                break;


            const TSK_VS_PART_INFO *vs_part;
            if ((vs_part = tsk_vs_part_get(vs_info_ptr, part_num)) == NULL)
            {
                recon_static_functions::app_debug("Error getting volume", Q_FUNC_INFO);
                continue;
            }

            // ignore the metadata partitions
            if (vs_part->flags & TSK_VS_PART_FLAG_META)
            {
                continue;
            }
            // could do something with unallocated volumes
            else if (vs_part->flags & TSK_VS_PART_FLAG_UNALLOC)
            {
            }
            else
            {
                int ret = tsk_process_volume_filesystem(img_info_ptr, (vs_part->start * vs_info_ptr->block_size));
                if(ret == -1)
                {
                    // We could do more fancy error checking here to see the cause
                    // of the error or consider the allocation status of the volume...
                    tsk_error_reset();
                }
            }
        }

        if(vs_info_ptr->part_count > 1)
        {
            if(tsk_vs_part_walk(vs_info_ptr, 0, (vs_info_ptr->part_count - 1), TSK_VS_PART_FLAG_ALL, vs_act, NULL))
            {
                tsk_vs_close(vs_info_ptr);
                tsk_img_close(img_info_ptr);
                return -1;
            }
        }
    }


    return 0;
}

///Process Filesystem inside a partition
int hex_viewer::tsk_process_volume_filesystem(TSK_IMG_INFO * img_info, TSK_OFF_T start)
{
    if(fs_info_ptr)
        tsk_fs_close(fs_info_ptr);

    fs_info_ptr = tsk_fs_open_img(img_info, start, TSK_FS_TYPE_DETECT);
    if(!fs_info_ptr)
    {
        recon_static_functions::app_debug("----FAILED---- Error opening file system in partition", Q_FUNC_INFO);
        return -1;
    }

//    if(bool_disk_viewer_complete_disk)
//        get_blocks_for_complete_disk_hex_view(fs_info_ptr);


    return 0;
}

///Extracts data for complete disk view
int hex_viewer::get_blocks_for_complete_disk_hex_view(TSK_FS_INFO *fs_info_ptr)
{
    if(fs_info_ptr == NULL)
        return -1;

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return -1;
    }



    if(fs_info_ptr->block_count > 0 && page_size > 0)
    {
        TSK_FS_BLOCK *block_info = NULL;

        if(fs_info_ptr->block_size != tsk_default_block_size)
            tsk_default_block_size = fs_info_ptr->block_size;

        disk_hex_viewer_total_pages = fs_info_ptr->block_count;

        int count = 0;
        bool_disk_hex_data_processed = false;

        ///blocks will move forward and append data in array
        disk_data_arr.clear();
        int processed_blocks_count = 0;
        for(qint64 blk_count = 0; blk_count < tsk_default_block_size; blk_count++)
        {
            count++;
            if(count % 10 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            qint64 block_addr = fs_info_ptr->first_block + blk_count;
            block_info = tsk_fs_block_get(fs_info_ptr, NULL, block_addr);

            if(block_info == NULL)
                continue;

            processed_blocks_count++;

            ///final data
            QByteArray tmp_arr = QByteArray::fromRawData(block_info->buf, sizeof(block_info->buf));
            disk_data_arr.append(tmp_arr);

            tsk_fs_block_free(block_info);
        }

        if(disk_data_arr.size() > page_size)
            disk_data_arr = disk_data_arr.mid(0, page_size);

        struct_disk_hex_view_data disk_hex_view_data_obj;

        disk_hex_view_data_obj.bool_last_click_was_next = true;
        disk_hex_view_data_obj.current_page_index = 1;
        disk_hex_view_data_obj.current_page_data  = disk_data_arr;
        disk_hex_view_data_obj.block_start_index  = 0;
        disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;
        disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

        disk_data_arr.clear();
        list_disk_hex_view_data.clear();
        list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block

        current_offset = 0;
        previous_entered_size = disk_data_arr.size();
        set_file_data_for_complete_hex_view();

        return 0;
    }
    return -1;
}

int hex_viewer::get_next_block_for_complete_disk_hex_view(TSK_FS_INFO *fs_info_ptr)
{
    if(fs_info_ptr == NULL)
        return -1;

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return -1;
    }

    if(list_disk_hex_view_data.size() <= 0)
        return -1;


    int ret_val = -1;
    if(fs_info_ptr->block_count > 0 && page_size > 0)
    {
        TSK_FS_BLOCK *block_info = NULL;

        qint64 total_block_count = (qint64)fs_info_ptr->block_count;

        int count = 0;
        bool_disk_hex_data_processed = false;
        qint64 page_count = 0;

        qint64 block_start_index = 0;

        struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
        page_count        = disk_hex_view_data_obj.current_page_index;

        previous_entered_size = disk_hex_view_data_obj.current_page_data.size();

        if(!disk_hex_view_data_obj.bool_last_click_was_next)
            block_start_index = disk_hex_view_data_obj.block_start_index + disk_hex_view_data_obj.last_processed_total_blocks;
        else
            block_start_index = disk_hex_view_data_obj.block_start_index + 1;

        ///blocks will move forward and append data in array
        disk_data_arr.clear();
        int processed_blocks_count = 0;
        for(qint64 blk_count = block_start_index; blk_count < total_block_count; blk_count++)
        {
            count++;
            if(count % 10 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            processed_blocks_count++;

            qint64 block_addr = fs_info_ptr->first_block + blk_count;
            block_info = tsk_fs_block_get(fs_info_ptr, NULL, block_addr);

            if(block_info == NULL)
                continue;

            ///final data
            QByteArray tmp_arr = QByteArray::fromRawData(block_info->buf, sizeof(block_info->buf));
            disk_data_arr.append(tmp_arr);

            tsk_fs_block_free(block_info);

            if(disk_data_arr.size() >= page_size)
            {
                page_count++;

                if(disk_data_arr.size() > page_size)
                    disk_data_arr = disk_data_arr.mid(0, page_size);

                bool_disk_hex_data_processed = true;
                struct_disk_hex_view_data disk_hex_view_data_obj;

                disk_hex_view_data_obj.bool_last_click_was_next = true;
                disk_hex_view_data_obj.current_page_index = page_count;
                disk_hex_view_data_obj.current_page_data  = disk_data_arr;
                disk_hex_view_data_obj.block_start_index  = blk_count;
                disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;

                disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

                disk_data_arr.clear();
                list_disk_hex_view_data.clear();
                list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block
                break;
            }
        }

        if(!bool_disk_hex_data_processed)
        {
            page_count++;
            bool_disk_hex_data_processed = true;

            if(disk_data_arr.size() > page_size)
                disk_data_arr = disk_data_arr.mid(0, page_size);

            struct_disk_hex_view_data disk_hex_view_data_obj;

            disk_hex_view_data_obj.bool_last_click_was_next = true;
            disk_hex_view_data_obj.current_page_index = page_count;
            disk_hex_view_data_obj.current_page_data  = disk_data_arr;
            disk_hex_view_data_obj.block_start_index  = total_block_count;
            disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;
            disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

            disk_data_arr.clear();
            list_disk_hex_view_data.clear();
            list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block
        }

        index_list_size = 0;

        if((total_file_size - current_offset) < previous_entered_size)
            previous_entered_size = total_file_size - current_offset;

        lineedit_goto_page->clear();
        if(current_offset < total_file_size)
        {
            struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

            setdata(disk_hex_view_data_obj.current_page_data);

            selected_data_size = disk_hex_view_data_obj.current_page_data.size();

            current_page_count = disk_hex_view_data_obj.current_page_index;

            tmp_no_of_pages = total_file_size / page_size;

            if((total_file_size % page_size) == 0)
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
                total_no_of_pages = tmp_no_of_pages;
            }
            else
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
                total_no_of_pages = tmp_no_of_pages + 1;
            }

            emit signal_change_address_area_value(current_offset-1);
            emit signal_current_page_details(current_page_count,current_offset,(current_offset + previous_entered_size - 1));

            current_offset = current_page_count * selected_data_size;
            pushButton_prev_goto_page->setEnabled(true);
        }

        ret_val = 0;
    }

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(true);

    emit set_searched_data_clear(0);

    return ret_val;
}

int hex_viewer::get_previous_block_for_complete_disk_hex_view(TSK_FS_INFO *fs_info_ptr)
{
    if(fs_info_ptr == NULL)
        return -1;

    qint64 page_size = spinBox_page_size->text().toLongLong();
    if(page_size <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return -1;
    }


    int ret_val = -1;
    if(fs_info_ptr->block_count > 0 && page_size > 0)
    {
        TSK_FS_BLOCK *block_info = NULL;

        int count = 0;
        bool_disk_hex_data_processed = false;
        qint64 page_count = 0;

        qint64 block_end_index   = 0;
        qint64 blocks_per_page = tsk_default_block_size;
        if(list_disk_hex_view_data.size() > 0)
        {
            struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

            page_count = disk_hex_view_data_obj.current_page_index - 1;

            if(page_count <= 0)
                page_count = 1;

            //            if(disk_hex_view_data_obj.bool_last_click_was_next)
            //                block_end_index = (page_count * blocks_per_page) - blocks_per_page;
            //            else
            //                block_end_index = page_count * blocks_per_page;

            block_end_index = page_count * blocks_per_page;

        }

        if(block_end_index <= 0)
            block_end_index = 0;

        qint64 block_start_index = block_end_index - blocks_per_page;
        if(block_start_index <= 0)
            block_start_index = 0;

        ///blocks will move backwards and prepend data in array
        disk_data_arr.clear();
        int processed_blocks_count = 0;
        for(qint64 blk_count = block_start_index; blk_count < block_end_index; blk_count++)
        {
            count++;
            if(count % 10 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            qint64 block_addr = fs_info_ptr->first_block + blk_count;
            block_info = tsk_fs_block_get(fs_info_ptr, NULL, block_addr);

            if(block_info == NULL)
                continue;

            processed_blocks_count++;

            ///final data
            QByteArray tmp_arr = QByteArray::fromRawData(block_info->buf, sizeof(block_info->buf));

            disk_data_arr.append(tmp_arr);

            tsk_fs_block_free(block_info);
        }

        if(disk_data_arr.size() > page_size)
            disk_data_arr = disk_data_arr.mid(0, page_size);

        struct_disk_hex_view_data disk_hex_view_data_obj;

        disk_hex_view_data_obj.bool_last_click_was_next = false;
        disk_hex_view_data_obj.current_page_index = page_count;

        if(disk_hex_view_data_obj.current_page_index <= 0)
            disk_hex_view_data_obj.current_page_index = 1;

        disk_hex_view_data_obj.current_page_data  = disk_data_arr;
        disk_hex_view_data_obj.block_start_index  = block_start_index;
        disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;
        disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

        disk_data_arr.clear();
        list_disk_hex_view_data.clear();
        list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block


        lineedit_goto_page->clear();
        if(list_disk_hex_view_data.size() > 0)
        {
            struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

            current_page_count = disk_hex_view_data_obj.current_page_index;

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

            current_offset += page_size;
            QString text_display = QString::number(0) + "/" + QString::number(page_size - 1);
            pushButton_selection_info_offset->setText(text_display);

            tmp_no_of_pages = disk_hex_viewer_total_pages;

            if((total_file_size % page_size) == 0)
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
                total_no_of_pages = tmp_no_of_pages;
            }
            else
            {
                label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
                total_no_of_pages = tmp_no_of_pages + 1;
            }
            previous_entered_size = page_size;

            pushButton_next_goto_page->setEnabled(true);
        }

        ret_val = 0;
    }

    pushButton_next_goto_page->setEnabled(true);
    pushButton_prev_goto_page->setEnabled(true);

    emit set_searched_data_clear(0);

    return ret_val;

}

void hex_viewer::set_file_data_for_complete_hex_view()
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


    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    total_file_size = struct_source_info.total_size_byte_numeric_QString.toLongLong();

    //current_offset        = page_size;
    //previous_entered_size = data_arr.size();
    //selected_data_size = data_arr.size();

    setdata(data_arr);


    current_page_count = disk_hex_view_data_obj.current_page_index;

    tmp_no_of_pages = disk_hex_viewer_total_pages;

    if((total_file_size % page_size) == 0)
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
        total_no_of_pages = tmp_no_of_pages;
    }
    else
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
        total_no_of_pages = tmp_no_of_pages + 1;
    }

    emit signal_current_page_details(current_page_count,0,current_offset-1);

    emit clear_block_index_list();

    fill_the_existing_blocks_details_in_list();


    if(current_page_count < tmp_no_of_pages)
        pushButton_next_goto_page->setEnabled(true);

    if(current_page_count > 1)
        pushButton_prev_goto_page->setEnabled(true);

    if(current_page_count >= total_no_of_pages)
    {
        pushButton_next_goto_page->setEnabled(false);
    }
}

void hex_viewer::common_go_to_page_for_disk_hex_viewer(qint64 enter_page_index)
{
    qint64 data_size_entered = spinBox_page_size->text().trimmed().toLongLong();
    if(data_size_entered <= 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"Please insert a valid page size");
        message_dialog_obj->show();
        return;
    }

    if(list_disk_hex_view_data.size() < 0)
    {
        message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"No data available at enetered page.");
        message_dialog_obj->show();
        return;
    }

    lineEdit_go_to_offset->clear();

    struct_GLOBAL_witness_info_source struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);
    if(enter_page_index <= 0 || enter_page_index > total_no_of_pages)
    {
        set_palette_lineedit_goto_page(1);
        return;
    }

    if(bool_disk_viewer_unallocated_disk)
    {
        go_to_page_for_disk_hex_view_unallocated(enter_page_index);
    }
    else
    {
        struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

        TSK_FS_INFO *fs_info_ptr = disk_hex_view_data_obj.fs_info_ptr;

        if(fs_info_ptr->block_count > 0 && data_size_entered > 0)
        {
            TSK_FS_BLOCK *block_info = NULL;

            total_file_size = struct_source_info.total_size_byte_numeric_QString.toLongLong();

            int total_no_of_pages = disk_hex_viewer_total_pages;

            if((total_file_size % data_size_entered) != 0)
            {
                total_no_of_pages += 1;
            }

            qint64 blk_end_index   = enter_page_index * tsk_default_block_size;
            qint64 blk_start_index = blk_end_index - tsk_default_block_size;

            if(blk_start_index < 0)
                blk_start_index = 0;

            int count = 0;
            bool_disk_hex_data_processed = false;
            qint64 page_count = 0;

            ///fetch exact blocks for current page
            disk_data_arr.clear();
            QByteArray current_data_arr;
            int processed_blocks_count = 0;
            for(qint64 blk_count = blk_start_index; blk_count < blk_end_index; blk_count++)
            {
                count++;
                if(count % 10 == 0)
                {
                    QCoreApplication::processEvents();
                    count = 0;
                }
                processed_blocks_count++;

                qint64 block_addr = fs_info_ptr->first_block + blk_count;
                block_info = tsk_fs_block_get(fs_info_ptr, NULL, block_addr);

                if(block_info == NULL)
                    continue;

                ///final data
                QByteArray tmp_arr = QByteArray::fromRawData(block_info->buf, sizeof(block_info->buf));
                disk_data_arr.append(tmp_arr);

                tsk_fs_block_free(block_info);
            }


            page_count = enter_page_index;
            if(page_count <= 0)
                page_count = 1;

            bool_disk_hex_data_processed = true;

            if(disk_data_arr.size() > data_size_entered)
                disk_data_arr = disk_data_arr.mid(0, data_size_entered);

            struct_disk_hex_view_data disk_hex_view_data_obj;

            disk_hex_view_data_obj.bool_last_click_was_next = false;
            disk_hex_view_data_obj.current_page_index = page_count;
            disk_hex_view_data_obj.current_page_data  = disk_data_arr;
            disk_hex_view_data_obj.block_start_index  = blk_start_index;
            disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;
            disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

            disk_data_arr.clear();
            list_disk_hex_view_data.clear();
            list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block

            ///===============================================================
            ///get updated data
            disk_hex_view_data_obj = list_disk_hex_view_data.at(0);
            current_data_arr =   disk_hex_view_data_obj.current_page_data;

            qint64 page_end_offset      = (enter_page_index * data_size_entered);
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

            current_page_count = enter_page_index;

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

            label_no_of_pages->setText(QString::number(enter_page_index) + " of " + QString::number(total_no_of_pages));

        }
    }

}

void hex_viewer::go_to_offset_for_disk_hex_viewer(qint64 offset)
{
    if(list_disk_hex_view_data.size() < 0)
        return;

    struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

    qint64 page_size = spinBox_page_size->text().toLongLong();

    uint64_t real_offset = offset % page_size;
    uint64_t start_offset = 0;

    int page_no = offset / page_size;
    if(page_no > 0)
        start_offset = page_size * page_no;
    else
        start_offset = 0;

    uint64_t end_offset = total_file_size - start_offset;

    current_page_count = disk_hex_view_data_obj.current_page_index;

    QByteArray selected_page_size_bytes = disk_hex_view_data_obj.current_page_data;
    setdata(selected_page_size_bytes);

    if(end_offset < page_size)
    {
        current_offset = start_offset + end_offset;
        previous_entered_size = end_offset;
    }
    else
    {
        current_offset = start_offset + page_size;
        previous_entered_size = page_size;
    }

    tmp_no_of_pages = disk_hex_viewer_total_pages;

    if((total_file_size % page_size) == 0)
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages));
        total_no_of_pages = tmp_no_of_pages;
    }
    else
    {
        label_no_of_pages->setText(QString::number(current_page_count) + " of " + QString::number(tmp_no_of_pages + 1));
        total_no_of_pages = tmp_no_of_pages + 1;
    }


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

    if(start_offset > 0)
        emit signal_change_address_area_value(start_offset-1);
    else
        emit signal_change_address_area_value(0);

    emit go_to_offset(real_offset, selected_page_size_bytes.size());

    emit signal_current_page_details(current_page_count,start_offset,current_offset-1);

}

void hex_viewer::search_for_disk_hex_view_complete(QString search_type, QString searched_text)
{
    bool_cancel_loading = false;

    index_list_size = 0;

    uint64_t page_size = spinBox_page_size->text().toULongLong();

    pushButton_search_previous->setEnabled(true);
    pushButton_search_next->setEnabled(true);

    lineedit_goto_page->clear();
    lineEdit_go_to_offset->clear();

    label_search_results->clear();

    struct_search_page_info st_obj;
    if(bool_disk_viewer_unallocated_disk)
        st_obj = get_available_page_info_for_diskview_unallocated_search(enum_search_pushbutton_search,search_type, searched_text);
    else
        st_obj = get_available_page_info_for_diskview_complete_search(enum_search_pushbutton_search,search_type, searched_text);

    if(st_obj.bool_text_match_found)
    {
        common_go_to_page_for_disk_hex_viewer(st_obj.matched_page_count);

        if(search_type == "ASCII")
            emit search_ascii(searched_text, page_size,false, true);
        else if(search_type == "HEX")
            emit search_hex(searched_text.toLocal8Bit(), page_size,false, true);
    }

}

hex_viewer::struct_search_page_info hex_viewer::get_available_page_info_for_diskview_complete_search(enum_search_clicked click_type,QString searched_type,QString searched_text)
{

    struct_search_page_info st_obj;
    st_obj.bool_text_match_found = false;
    st_obj.matched_page_count = -1;


    if(list_disk_hex_view_data.size() <= 0)
        return st_obj;


    uint64_t data_size_entered = spinBox_page_size->text().toULongLong();

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();

    struct_disk_hex_view_data disk_hex_view_data_obj = list_disk_hex_view_data.at(0);

    bool bool_match_found = false;

    QByteArray searched_hex_arr = QByteArray::fromHex(searched_text.toLocal8Bit());

    bool_cancel_loading = false;
    display_loading_progress_bar_obj->pub_set_label_messsge("Please wait...");
    display_loading_progress_bar_obj->pub_set_progress_bar_indefinite_value();
    display_loading_progress_bar_obj->show();


    qint64 current_page = disk_hex_view_data_obj.current_page_index;

    if(click_type == enum_search_pushbutton_search)
    {

    }
    else if(click_type == enum_search_pushbutton_previous)
    {
        current_page -= 1;

    }
    else if(click_type == enum_search_pushbutton_next)
    {
        current_page += 1;
    }


    TSK_FS_INFO *fs_info_ptr = disk_hex_view_data_obj.fs_info_ptr;

    if(fs_info_ptr->block_count > 0)
    {

        TSK_FS_BLOCK *block_info = NULL;
        qint64 total_block_count = (qint64)fs_info_ptr->block_count;

        int count = 0;
        bool_disk_hex_data_processed = false;
        qint64 page_count = 1;

        ///fetch exact blocks for current page
        disk_data_arr.clear();
        int processed_blocks_count = 0;
        for(qint64 blk_count = 0; blk_count < total_block_count; blk_count++)
        {
            if(bool_cancel_loading)
                break;

            count++;
            if(count % 10 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }
            processed_blocks_count++;

            qint64 block_addr = fs_info_ptr->first_block + blk_count;
            block_info = tsk_fs_block_get(fs_info_ptr, NULL, block_addr);

            if(block_info == NULL)
                continue;

            ///final data
            QByteArray tmp_arr = QByteArray::fromRawData(block_info->buf, sizeof(block_info->buf));
            disk_data_arr.append(tmp_arr);

            tsk_fs_block_free(block_info);

            if(disk_data_arr.size() >= data_size_entered)
            {
                if(page_count < current_page)
                {
                    ///start search from current page (skip previous pages)
                    page_count++;

                    disk_data_arr.clear();
                    continue;
                }

                if(disk_data_arr.size() > data_size_entered)
                    disk_data_arr = disk_data_arr.mid(0, data_size_entered);

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
                    bool_disk_hex_data_processed = true;
                    struct_disk_hex_view_data disk_hex_view_data_obj;

                    disk_hex_view_data_obj.bool_last_click_was_next = false;
                    disk_hex_view_data_obj.current_page_index = page_count;
                    disk_hex_view_data_obj.current_page_data  = disk_data_arr;

                    disk_hex_view_data_obj.block_start_index = blk_count;
                    disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;
                    disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

                    disk_data_arr.clear();
                    list_disk_hex_view_data.clear();
                    list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block
                    break;
                }

                page_count++;
                disk_data_arr.clear();

            }
        }

        if(!bool_disk_hex_data_processed)
        {
            if(page_count <= 0)
                page_count = 1;

            bool_disk_hex_data_processed = true;

            if(disk_data_arr.size() > data_size_entered)
                disk_data_arr = disk_data_arr.mid(0, data_size_entered);

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
                disk_hex_view_data_obj.block_start_index  = 0;
                disk_hex_view_data_obj.last_processed_total_blocks = processed_blocks_count;
                disk_hex_view_data_obj.fs_info_ptr = fs_info_ptr;

                disk_data_arr.clear();
                list_disk_hex_view_data.clear();
                list_disk_hex_view_data.append(disk_hex_view_data_obj); //will store only current data block
            }
        }
    }

    display_loading_progress_bar_obj->hide();
    return st_obj;
}

