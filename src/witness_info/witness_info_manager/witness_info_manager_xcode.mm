#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#include "witness_info_manager.h"

QList<struct_GLOBAL_snapshots_info> witness_info_manager::get_apfs_snapshots_QList_for_source_from_snapshots_plist(QString source_count_name)
{
    source_count_name = source_count_name.trimmed();
    QList<struct_GLOBAL_snapshots_info> apfs_snapshots_list;

    if(source_count_name.isEmpty())
        return apfs_snapshots_list;

    QString dest_plist_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "snapshots.plist";
    if(!QFileInfo(dest_plist_path).exists())
        return apfs_snapshots_list;

    NSAutoreleasePool* pool  = [[NSAutoreleasePool alloc] init];
    NSArray *data_read_array = [[NSMutableArray alloc]initWithContentsOfFile:dest_plist_path.toNSString()];

    if(data_read_array == nil)
    {
        [pool drain];
        return apfs_snapshots_list;
    }

    for(id data_dict in data_read_array)
    {
        NSString *source_count_str= data_dict[QString(MACRO_APFS_Snapshot_Plist_Key_Source_Count_Name).toNSString()];
        if(source_count_name == QString::fromNSString(source_count_str))
        {
            struct_GLOBAL_snapshots_info snapshots_info_obj;

            snapshots_info_obj.snapshot_actual_name = QString::fromNSString(data_dict[QString(MACRO_APFS_Snapshot_Plist_Key_Snapshot_Name).toNSString()]);
            snapshots_info_obj.fs_run_status = QString::fromNSString(data_dict[QString(MACRO_APFS_Snapshot_Plist_Key_FS_Run_Status).toNSString()]);
            snapshots_info_obj.source_count_name = QString::fromNSString(source_count_str);

            apfs_snapshots_list.append(snapshots_info_obj);
        }
    }

    [pool drain];

    return apfs_snapshots_list;
}
