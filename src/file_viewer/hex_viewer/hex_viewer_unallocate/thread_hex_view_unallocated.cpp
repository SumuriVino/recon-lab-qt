#include "thread_hex_view_unallocated.h"


QList<struct_global_tsk_part_info_unalloc> global_part_unalloc_list;

///creates final list of data to be inserted in database
static TSK_WALK_RET_ENUM vs_act(TSK_VS_INFO *vs_info, const TSK_VS_PART_INFO *vs_partition, void *ptr)
{
    struct_global_tsk_part_info_unalloc struct_info;

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

    global_part_unalloc_list.append(struct_info);

    return (TSK_WALK_RET_ENUM)0;
}


thread_hex_view_unallocated::thread_hex_view_unallocated()
{
    recon_helper_standard_obj = new recon_helper_standard();

}

thread_hex_view_unallocated::~thread_hex_view_unallocated()
{
    if(recon_helper_standard_obj)
        delete recon_helper_standard_obj;
}

void thread_hex_view_unallocated::pub_set_essentials(QString source_count_str, bool bool_complete_src)
{
    source_count_name       = source_count_str;
    bool_is_complete_source = bool_complete_src;


    struct_source_info = global_witness_info_manager_class_obj->pub_get_source_structure_according_source_count_name(source_count_name);

    source_device_node_str.clear();

    QString tmp_idntf = struct_source_info.source_disk_identifier;

    if(bool_is_complete_source)
    {
        int tmp_index = tmp_idntf.lastIndexOf("s");
        if(tmp_index > 4) // (input : disk4s1 and output : disk4)
            tmp_idntf = tmp_idntf.mid(0, tmp_index);
    }

    current_table_name = tmp_idntf;

    current_table_name.replace(" ", "_");
    current_table_name.replace("-", "_");
    current_table_name.replace("/", "_");

    source_device_node_str = "/dev/r" + tmp_idntf;

}

void thread_hex_view_unallocated::pub_set_cancel_extraction_status(bool status)
{
    bool_cancel_extraction = status;
}

QString thread_hex_view_unallocated::pub_get_source_count_name()
{
    return source_count_name;
}

bool thread_hex_view_unallocated::pub_get_is_complete_disk_status()
{
    return bool_is_complete_source;
}

QString thread_hex_view_unallocated::pub_get_current_table_name()
{
    return current_table_name;
}

void thread_hex_view_unallocated::run()
{
    ///no need to process same source again
    if(check_source_already_processed_completely())
        return;


    /// do not use return for cancel extraction in this fun
    bool_cancel_extraction = false;

    emit signal_PBAR_thread_disk_hex_view_unallocated_started(MACRO_JobType_Processing_Unallocated_Data);

    create_destination_table();

    command_insert = QString("Insert into ") + current_table_name + QString("(block_count,data_size_bytes,source_count_name,hex_page_count) Values(?,?,?,?)");
    destination_db.transaction();

    tsk_start_image_processing(source_device_node_str);

    destination_db.commit();

    insert_destination_index();

    destination_db.close();

    emit signal_PBAR_thread_disk_hex_view_unallocated_finished(MACRO_JobType_Processing_Unallocated_Data);

}

void thread_hex_view_unallocated::create_destination_table()
{
    source_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Hex_View_In_Result_QString).toString() + "disk_hex_view_unallocated.sqlite";

    QSqlDatabase::removeDatabase(Q_FUNC_INFO);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", Q_FUNC_INFO);
    destination_db.setDatabaseName(source_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug("Database open ----FAILED---- " + destination_db.databaseName() , Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + destination_db.lastError().text() , Q_FUNC_INFO);
        return;
    }

    QString tm_cmd = QString("create table IF NOT EXISTS ") + current_table_name +
            QString("(bookmark INTEGER,INT INTEGER PRIMARY KEY,"
                    "block_count INTEGER,data_size_bytes INTEGER,hex_page_count INTEGER,"
                    "source_count_name varchar(100))");


    query_destination = QSqlQuery(destination_db);

    query_destination.prepare(tm_cmd);

    if(!query_destination.exec())
    {
        recon_static_functions::app_debug("Table creation -----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Query:" + query_destination.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_destination.lastError().text() , Q_FUNC_INFO);
    }

    ///empty if any data exists
    tm_cmd = QString("Delete From ") + current_table_name;
    query_destination.prepare(tm_cmd);
    if(!query_destination.exec())
    {
        recon_static_functions::app_debug("Table Empty -----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Query:" + query_destination.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_destination.lastError().text() , Q_FUNC_INFO);
    }

    ///empty if any data from index table
    tm_cmd = QString("Delete From tbl_saved_index Where saved_table_name = ? AND source_count_name = ?");
    query_destination.prepare(tm_cmd);

    query_destination.addBindValue(current_table_name);
    query_destination.addBindValue(source_count_name);

    if(!query_destination.exec())
    {
        recon_static_functions::app_debug("Table Empty -----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Query:" + query_destination.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_destination.lastError().text() , Q_FUNC_INFO);
    }

}

bool thread_hex_view_unallocated::check_source_already_processed_completely()
{
    QString hex_view_db_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Disk_Hex_View_In_Result_QString).toString() + "disk_hex_view_unallocated.sqlite";

    QString is_complete_str = QString::number(0);

    if(bool_is_complete_source)
        is_complete_str = QString::number(1);

    QString cmd_str = QString("Select extraction_status,saved_table_name From tbl_saved_index Where source_count_name = ? AND is_complete_source = ?");
    QStringList value_list;
    value_list << source_count_name << is_complete_str;
    QString run_status_str = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_str,value_list, 0, hex_view_db_path,Q_FUNC_INFO);

    run_status_str = run_status_str.trimmed();

    if(run_status_str == QString(MACRO_FALSE_VALUE_NUMERIC_QString))
    {
        current_table_name = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(cmd_str,value_list, 1, hex_view_db_path,Q_FUNC_INFO);
    }

    if(run_status_str == QString(MACRO_TRUE_VALUE_NUMERIC_QString))
        return true;
    else
        return false;
}

void thread_hex_view_unallocated::insert_destination_index()
{

    QString is_complete_source = QString::number(0);
    if(bool_is_complete_source)
        is_complete_source = QString::number(1);

    QString extraction_status_str = QString::number(1);
    if(bool_cancel_extraction)
        extraction_status_str = QString::number(0);

    QString cmd_index = QString("Insert into tbl_saved_index(saved_table_name, is_complete_source,extraction_status,source_count_name) Values(?,?,?,?)") ;

    query_destination.prepare(cmd_index);

    query_destination.addBindValue(current_table_name);
    query_destination.addBindValue(is_complete_source);
    query_destination.addBindValue(extraction_status_str);
    query_destination.addBindValue(source_count_name);

    if(!query_destination.exec())
    {
        recon_static_functions::app_debug("index table insertion -----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Query:" + query_destination.executedQuery(), Q_FUNC_INFO);
        recon_static_functions::app_debug("Error " + query_destination.lastError().text() , Q_FUNC_INFO);
        return;
    }
}

///opens image and its partitions
int thread_hex_view_unallocated::tsk_start_image_processing(QString image_path_str)
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
    global_part_unalloc_list.clear();

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

            if(bool_cancel_extraction)
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

    process_multi_partitioned_unallocated_data();

    return 0;
}

///Process Filesystem inside a partition
int thread_hex_view_unallocated::tsk_process_volume_filesystem(TSK_IMG_INFO * img_info, TSK_OFF_T start)
{
    if(fs_info_ptr)
        tsk_fs_close(fs_info_ptr);

    fs_info_ptr = tsk_fs_open_img(img_info, start, TSK_FS_TYPE_DETECT);
    if(!fs_info_ptr)
    {
        recon_static_functions::app_debug("----FAILED---- Error opening file system in partition", Q_FUNC_INFO);
        return -1;
    }

    get_unallocated_data_from_partition(fs_info_ptr);


    return 0;
}

///Process unallocated space in multi-partition
int thread_hex_view_unallocated::process_multi_partitioned_unallocated_data()
{
    int count = 0;
    for(int part_count = 0; part_count < global_part_unalloc_list.size(); part_count++)
    {
        if(bool_cancel_extraction)
            break;

        count++;
        if(count % 10 == 0)
        {
            QCoreApplication::processEvents();
            count = 0;
        }

        struct_global_tsk_part_info_unalloc struct_info = global_part_unalloc_list.at(part_count);

        if(struct_info.bool_fs_found)
        {
            get_unallocated_data_from_partition(struct_info.fs_info_ptr);
        }
    }

    return 0;
}

///Extracts unallocated space from a partition
int thread_hex_view_unallocated::get_unallocated_data_from_partition(TSK_FS_INFO *fs_info_ptr)
{
    if(fs_info_ptr->block_count > 0)
    {
        TSK_FS_BLOCK *block_info = NULL;

        qint64 total_block_count = (qint64)fs_info_ptr->block_count;
        int count = 0;
        qint64 records_count = 0;

        QByteArray page_data_arr;
        qint64 page_count = 1;

        for(qint64 blk_count = 0; blk_count < total_block_count; blk_count++)
        {
            if(bool_cancel_extraction)
                break;

            count++;
            if(count % 10 == 0)
            {
                QCoreApplication::processEvents();
                count = 0;
            }

            qint64 block_addr = fs_info_ptr->first_block + blk_count;
            block_info = tsk_fs_block_get(fs_info_ptr, NULL, block_addr);
            if(block_info->flags & TSK_FS_BLOCK_FLAG_UNALLOC)
            {
                block_data_array = block_info->buf;
                //std::string std_str(data_array.constData(), data_array.length());
                //QString ascii_str = QString::fromStdString(std_str);
                if(block_data_array.size() > 0)
                {

                    ///current data
                    query_destination.prepare(command_insert);
                    query_destination.addBindValue(blk_count);
                    query_destination.addBindValue(block_data_array.size());
                    query_destination.addBindValue(source_count_name);
                    query_destination.addBindValue(page_count);

                    if(!query_destination.exec())
                    {
                        ///do not continue
                        recon_static_functions::app_debug("Insert(current) -----FAILED-----" + destination_db.databaseName(), Q_FUNC_INFO);
                        recon_static_functions::app_debug("Query:" + query_destination.executedQuery(), Q_FUNC_INFO);
                        recon_static_functions::app_debug("Error " + query_destination.lastError().text() , Q_FUNC_INFO);
                    }

                    records_count++;

                    page_data_arr.append(block_data_array);
                    if(page_data_arr.size() >= MACRO_HEX_VIEWER_DEFAULT_PAGE_SIZE_int)
                    {
                        page_count++;
                        // if(page_data_arr.size() > MACRO_HEX_VIEWER_DEFAULT_PAGE_SIZE)
                        // {
                        //   page_data_arr = page_data_arr.mid(MACRO_HEX_VIEWER_DEFAULT_PAGE_SIZE, (page_data_arr.size() - MACRO_HEX_VIEWER_DEFAULT_PAGE_SIZE));
                        // }

                        page_data_arr.clear();
                    }
                }
            }

            if(blk_count % 1000 == 0)
            {
                qint64 progress_val = ((blk_count * 100) / total_block_count);
                if(progress_val > 99)
                    progress_val = 99;

                emit signal_PBAR_thread_disk_hex_view_unallocated_value(MACRO_JobType_Processing_Unallocated_Data, struct_source_info.source_name, true, records_count, progress_val, false);
            }

            tsk_fs_block_free(block_info);
        }

        return 0;
    }

    return -1;
}
