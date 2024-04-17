#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include <QtCore>
#include <tsk/libtsk.h>
#include <QSqlDatabase>
#include "fs_viewer_tsk.h"

#define SUCCESS 0
#define FAILURE -1

#define BIG_ENDIAN_HEX 1
#define LITTLE_ENDIAN_HEX -1

#define ROOT_DIRECTORY "$ROOT DIRECTORY"

struct struct_hex_viewer_tag
{
    qint64 start_address;
    qint64 end_address;
    QString color_hex;
    QString tag_name_str;
    QString description;
};

struct struct_hex_viewer_markers
{
    qint64 marker_address;
    QString marker_description;
    QString marker_partition;
    QString marker_item;
};

enum enum_offset_jump_type{
    enum_offset_jump_from_start,
    enum_offset_jump_from_current,
    enum_offset_jump_from_back
};



struct file_detail
{
    QString file_name;
    QString file_path;
    QString inode;
    QString size;
    QString access_date;
    QString modify_date;
    QString change_date;
    QString created_date;
    QString is_dir;
};

struct partition_detail {
    qint64 start_byte_address;
    qint64 block_count;
    qint64 block_size;
    QString data_unit_name;
    qint64 device_size;
    qint64 first_block;
    qint64 last_block;
    QString file_system_name;
    qint64 first_inode;
    qint64 byte_offset;
    TskFsInfo *tsk_fs_info;
    bool bool_has_fs;
    const TskVsPartInfo *tsk_part_info;
    QSqlDatabase fs_db;
    QSqlDatabase *fs_db_ptr;
    fs_viewer_tsk *fs_viewer;
    int sector_size;
    int sector_per_cluster;
    qint64 root_directory_address;
    qint64 fat1_address;
    qint64 fat2_address;
    qint64 mft_address;
    int fat_sector_count;
};



enum fs_list_columns
{
    enum_fs_INT= 0,
    enum_fs_inode,
    enum_fs_filename,
    enum_fs_size,
    enum_fs_access,
    enum_fs_modify,
    enum_fs_change,
    enum_fs_dir
};


enum enum_data_interpreter_rows {
    enum_data_interpreter_8_bit_binary,
    enum_data_interpreter_8_bit_signed,
    enum_data_interpreter_8_bit_unsigned,
    enum_data_interpreter_16_bit_signed,
    enum_data_interpreter_16_bit_unsigned,
   // enum_data_interpreter_24_bit_signed,
    enum_data_interpreter_32_bit_signed,
    enum_data_interpreter_32_bit_unsigned,
    enum_data_interpreter_64_bit_signed,
    enum_data_interpreter_64_bit_unsigned,
    enum_data_interpreter_DOS_time,
    enum_data_interpreter_DOS_date,
    enum_data_interpreter_Unix_timestamp,
    enum_data_interpreter_Windows_timestamp,
   // enum_data_interpreter_Mac_timestamp,
    enum_data_interpreter_count
};

struct mft_entry {

    bool ok;
    QByteArray signature;
    QByteArray offset_to_fixup_array;
    QByteArray entries_in_fix_array;
    QByteArray logfile_sequence_number;
    QByteArray sequence_count;
    QByteArray hard_link_count;
    QByteArray offset_to_first_attribute;
    QByteArray allocation_status;
    QByteArray logical_size;
    QByteArray physical_size;
    QByteArray file_refrence_to_base_record;
    QByteArray next_attribute_identification;

};

enum enum_mft_entry {
    enum_mft_entry_signature,
    enum_mft_entry_offset_to_fixup_array,
    enum_mft_entry_entries_in_fix_array,
    enum_mft_entry_logfile_sequence_number,
    enum_mft_entry_sequence_count,
    enum_mft_entry_hard_link_count,
    enum_mft_entry_offset_to_first_attribute,
    enum_mft_entry_allocation_status,
    enum_mft_entry_logical_size,
    enum_mft_entry_physical_size,
    enum_mft_entry_file_refrence_to_base_record,
    enum_mft_entry_next_attribute_identification
};

struct common_structure_for_data_table {

    QString offset_hex;
    QString offset_dec;
    QString length;
    QString value;
    QString value_hex;
    QString description;
};

enum enum_ntfs_mft_attributes {
    enum_ntfs_mft_standard_attribute, // 0x10
    enum_ntfs_mft_file_name_attribute, // 0x30
    enum_ntfs_mft_data_attribute, // 0x80
    enum_ntfs_mft_security_descriptor_attribute, // 0x50
    enum_ntfs_mft_unknown_attribute,
    enum_ntfs_mft_end_attribute
};

struct ntfs_attribute_and_offset {
    int offset;
    int attribute_type;
    int size;
};

struct struct_device_info {
    QString device_name;
    qint64 device_size;
};
#endif // COMMON_HEADER_H
