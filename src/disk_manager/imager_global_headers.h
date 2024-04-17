#ifndef GLOBAL_HEADERS_H
#define GLOBAL_HEADERS_H

#include <QtCore>


#define MACRO_Generic_Value_KB_int 1024
#define MACRO_Generic_Value_MB_int 1024*1024
#define MACRO_Generic_Value_GB_int 1024*1024*1024
#define MACRO_Generic_Value_TB_int 1024*1024*1024*1024

//#define MACRO_Generic_Value_KB_int 1000
//#define MACRO_Generic_Value_MB_int 1000*1000
//#define MACRO_Generic_Value_GB_int 1000*1000*1000
//#define MACRO_Generic_Value_TB_int 1000*1000*1000*1000


#define MACRO_Imaging_Type_Physical "Physical"
#define MACRO_Imaging_Type_Logical  "Logical"


#define MACRO_ImageFormat_DD_RAW "DD"
#define MACRO_ImageFormat_DMG_RAW "DMG"
#define MACRO_ImageFormat_EWF_E01 "E01"
#define MACRO_ImageFormat_EWF_Ex01 "Ex01"
#define MACRO_ImageFormat_EWF_SMART "SMART"
#define MACRO_ImageFormat_Logical_Sparse "Logical (Sparse Image)"
#define MACRO_ImageFormat_Logical_Folder "Logical (Folder)"
#define MACRO_ImageFormat_Logical_Tar "Logical (Tar)"
#define MACRO_ImageFormat_Logical_DMG_RW "Logical (DMG-RW)"

#define NONE "none"

#define WRITE_WITH_TRUNCATE 0
#define WRITE_WITH_APPEND 1

#define FAILURE -1
#define SUCCESS 0

#define NOMOUNT "NOMOUNT"

#define FINAL_DEPLOY

#ifdef FINAL_DEPLOY
#define NETWORK_SHARE_PARENT_DIR "/private/var/tmp/NET"
#else
#define NETWORK_SHARE_PARENT_DIR "/Users/manoj/tmp/mnt"
#endif

//#define MOUNT_POINT_PARENT_DIR "/private/var/tmp/mnt_dir"
#define MOUNT_POINT_PARENT_DIR "/tmp/mnt_dir"
#define MACRO_Disk_Manager_Temp_Mount_DirName "/private/var/tmp/disk_manager/"

#define MOUNT_POINT_FOR_SNAPSHOT "/private/var/tmp/snapshots"

#define HASH_MD5    "MD5"
#define HASH_SHA1   "SHA1"

#define disk_manager_backround_color_Red           "Red"         // Read Write Mounted
#define disk_manager_backround_color_Green         "Green"       // Read Only Mounted
#define disk_manager_backround_color_Gray          "Gray"        // Full Disk (Disk0, Disk1)
#define disk_manager_backround_color_Yellow        "Yellow"      // derived disks
#define disk_manager_backround_color_Light_Brown   "Light Brown" // Apfs Normal or derived both
#define disk_manager_backround_color_Orange        "Orange"      // Apple Corestorage
#define disk_manager_backround_color_Olive         "Olive"       // File Vaulted Decrypted

#define mount_mode_ReadOnly  "Read Only"
#define mount_mode_ReadWrite "Read Write"

#define color_Red           "#CD0000"
#define color_Green         "#66D066"
#define color_Olive         "#808000"

#define Size_2G_bytes  2147483648
#define Size_1G_bytes  1073741824

#define Macro_Target_Disk_Mode_Model_Media_Name    "Target Disk Mode"
#define Macro_Timer_Counter_10_Seconds_int         10
#define Macro_Timer_Counter_1_Minutes_Seconds_int  60
#define Macro_Timer_Counter_5_Minutes_Seconds_int  300
#define Macro_Timer_Counter_10_Minutes_Seconds_int 600

struct struct_disk_info
{

    QString device_identifier;
    QString model_MediaName;
    qint64 total_size_qint64_TotalSize;
    QString total_size_human_readable;
    QString type_Content;
    QString name_VolumeName;
    QString filesystem_FilesystemType;
    QString derived_from;
    QString mode;
    QString mount_point_MountPoint;
    bool is_internal_Internal;
    qint64 free_space_qint64_FreeSpace;
    bool is_mounted;

    QString disk_uuid;
    bool    bool_is_encrypt;
    bool    bool_is_locked;

    bool is_whole_disk;
    QString parent_whole_disk;

    ///=======APFS-Physical Device==========///
    QString apfs_physical_container_uuid;
    QString apfs_physical_device_id;
    QString apfs_physical_device_uuid;
    bool    apfs_bool_is_physical_fusion;

    ///=======APFS-Derived Volume==========///
    QString apfs_volume_id;
    QString apfs_volume_uuid;
    QString apfs_volume_label;

    bool    apfs_bool_is_encrypt;
    bool    apfs_bool_is_filevault;
    bool    apfs_bool_is_locked;

    QStringList apfs_volume_roles_list;


};

struct plugins_data
{
    QString plugin_name;
    QStringList file_list;
};

struct network_share_info
{
    QString address;
    QString mount_path;
    QString share_name;
    QString user_name;
    QString user_password;
};

struct struct_hashes{
  QString md5;
  QString sha1;
};

struct struct_imager_hashes_with_details
{
    QString md5;
    QString sha1;
    QString hash_string;
};


struct apfs_imager_snapshot_mount_paths
{
    QString snapshot_name;
    QString snapshot_mount_path;

    QString snapshot_sqlite_path;
    QString snapshot_diff_sqlite_path;

    QString snapshot_output_dir;
};

#endif // GLOBAL_HEADERS_H
