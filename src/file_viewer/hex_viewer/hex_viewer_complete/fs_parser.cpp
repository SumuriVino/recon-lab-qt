#include "fs_parser.h"

fs_parser::fs_parser(QObject *parent) : QObject(parent)
{
    tsk_img_info = new TskImgInfo;
}

void fs_parser::pub_set_image_path(QString str)
{
    image_path = str;
}

void fs_parser::pub_set_output_directory(QString str)
{
    output_dir = str;

    dest_db_file_path = output_dir + "/fs_reader.sqlite";
}

void fs_parser::pub_read_image()
{
    markers_list.clear();

    if(open_image() == FAILURE)
    {
        recon_static_functions::app_debug("Failed to open Image", Q_FUNC_INFO);
        return;
    }

    open_vs();

    open_all_fs();

}

int fs_parser::pub_get_file_data(QString inode, int size, unsigned char *buf)
{
    TskFsFile *m_file = new TskFsFile;

    if(m_file->open(tsk_fs_info,m_file, inode.toInt()) == 0)
    {
        return m_file->read(0, (char *)buf, m_file->getMeta()->getSize(), TSK_FS_FILE_READ_FLAG_SLACK);

    }

}

bool fs_parser::pub_is_image_has_MBR()
{
    if(tsk_vs_info == NULL)
        return false;

    if(tsk_vs_info->getVsType() == TSK_VS_TYPE_DOS)
        return true;
    else
        return false;
}

QList<partition_detail> fs_parser::pub_get_partition_details()
{
    return list_tsk_part_info;
}

void fs_parser::slot_start_processing()
{
    pub_read_image();

    emit signal_work_finished();
}

int fs_parser::open_image()
{

    uint8_t ret =  tsk_img_info->open(image_path.toLocal8Bit().data(), TSK_IMG_TYPE_RAW, 0);

    if(ret == 0)
    {
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }

}

void fs_parser::open_vs()
{
    bool_no_vs = true;

    tsk_vs_info = new TskVsInfo;
    uint8_t ret =  tsk_vs_info->open(tsk_img_info, 0, TSK_VS_TYPE_DETECT);

    if(ret == 0)
    {
        bool_no_vs = false;
    }
}

void fs_parser::open_all_fs()
{
    if(bool_no_vs)
        return;

    for(int i = 0; i < tsk_vs_info->getPartCount(); i++)
    {
        struct partition_detail part_info;

        const TskVsPartInfo *part =   tsk_vs_info->getPart(i);

        part_info.bool_has_fs = false;
        part_info.tsk_part_info = part;

        TskFsInfo *m_fs_info = new TskFsInfo;

        int ret = m_fs_info->open(part, TSK_FS_TYPE_DETECT);

        if(ret != 0)
        {
            list_tsk_part_info.append(part_info);
            continue;
        }

        part_info.block_count = m_fs_info->getBlockCount();
        part_info.block_size = m_fs_info->getBlockSize();
        part_info.data_unit_name = m_fs_info->getDataUnitName();
        part_info.device_size = m_fs_info->getDeviceSize();
        part_info.first_block = m_fs_info->getFirstBlock();
        part_info.last_block = m_fs_info->getLastBlock();
        part_info.file_system_name = m_fs_info->typeToName(m_fs_info->getFsType());
        part_info.first_inode = m_fs_info->getFirstINum();
        part_info.byte_offset = m_fs_info->getOffset();
        part_info.bool_has_fs = true;
        part_info.tsk_fs_info = m_fs_info;
        part_info.tsk_part_info = part;
        part_info.start_byte_address = part->getStart() * tsk_vs_info->getBlockSize();

        create_file_list(m_fs_info, part_info.fs_db);
        part_info.fs_db_ptr = &(part_info.fs_db);
        list_tsk_part_info.append(part_info);

    }
}

void fs_parser::create_file_list(TskFsInfo *fs_info, QSqlDatabase &db)
{
    progress_counter = 0;
    all_file_details.clear();

    fs_info->dirWalk(fs_info->getRootINum(), TSK_FS_DIR_WALK_FLAG_RECURSE, fs_function_walk, (void *)&all_file_details);

    QString connection_naam = QString(Q_FUNC_INFO) + QDateTime::currentDateTime().toString() + QString::number(partition_counter++);
    QSqlDatabase::removeDatabase(connection_naam);
    db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    db.setDatabaseName(":memory:");
    if(!db.open())
    {
        recon_static_functions::app_debug("Opening result DB - -----FAILED---- - create_file_list", Q_FUNC_INFO);
        recon_static_functions::app_debug("Error:" + db.lastError().text(), Q_FUNC_INFO);
        return ;
    }


    QSqlQuery query_create(db);

    QString command_image_info = "create table files (INT INTEGER PRIMARY KEY, filename varchar(300), inode_no integer, is_dir integer, file_parent_dir varchar(300), size integer, m_access_time integer, m_modified_time integer, m_change_time integer )";

    query_create.exec(command_image_info);


    db.transaction();
    QSqlQuery query(db);

    query.prepare("insert into files (filename, inode_no, file_parent_dir, size, m_access_time, m_modified_time,m_change_time, is_dir ) values (?,?,?,?, ?,?,?,?)");

    for(int i = 0 ; i < all_file_details.size(); i++)
    {
        progress_counter++;

        if((fs_info->getFsType() == TSK_FS_TYPE_FAT12 || fs_info->getFsType() == TSK_FS_TYPE_FAT16 || fs_info->getFsType() == TSK_FS_TYPE_FAT32)
                && all_file_details.at(i).file_name.startsWith("$OrphanFiles"))
            continue;

        if(progress_counter % 200 == 0)
            QCoreApplication::processEvents();

        query.addBindValue(all_file_details.at(i).file_name);
        query.addBindValue(all_file_details.at(i).inode);
        query.addBindValue(all_file_details.at(i).file_path);
        query.addBindValue(all_file_details.at(i).size);
        query.addBindValue(all_file_details.at(i).access_date);
        query.addBindValue(all_file_details.at(i).modify_date);
        query.addBindValue(all_file_details.at(i).change_date);
        query.addBindValue(all_file_details.at(i).is_dir);

        query.exec();

        query.clear();
        query.prepare("insert into files (filename, inode_no, file_parent_dir, size, m_access_time, m_modified_time,m_change_time, is_dir ) values (?,?,?,?, ?,?,?,?)");
    }

    db.commit();
    // db.close();
}


TSK_WALK_RET_ENUM fs_function_walk(TskFsFile *a_fs_file, const char *a_path, void *a_ptr)
{

    file_detail f1;
    f1.file_name = QString(a_fs_file->getName()->getName());
    f1.file_path =  "/" + QString(a_path);
    f1.size = QString::number(a_fs_file->getMeta()->getSize());
    f1.inode = QString::number(a_fs_file->getMeta()->getAddr());
    f1.access_date = QString::number(a_fs_file->getMeta()->getATime());
    f1.change_date = QString::number(a_fs_file->getMeta()->getCrTime());
    f1.modify_date = QString::number(a_fs_file->getMeta()->getMTime());
    f1.is_dir = "0";

    if(a_fs_file->getMeta()->getType() == TSK_FS_META_TYPE_DIR)
        f1.is_dir = "1";

    // TSK_FS_META_TYPE_DIR

    // TSK_FS_META_TYPE_ENUM
    if(a_fs_file->getMeta()->getType() == TSK_FS_META_TYPE_DIR)
    {
        f1.is_dir ="1";
    }

    if(((QList<file_detail> *)a_ptr)->size() % 50 == 0)
        QCoreApplication::processEvents();

    ((QList<file_detail> *)a_ptr)->append(f1);


    return TSK_WALK_CONT;
}
