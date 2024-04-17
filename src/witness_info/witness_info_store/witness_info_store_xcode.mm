#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#include "witness_info_store.h"

void witness_info_store::pub_update_key_with_new_value_in_snapshots_plist(QString snapshot_name, QString source_count_name, QString key_str, QString value_str)
{

    snapshot_name = snapshot_name.trimmed();
    source_count_name = source_count_name.trimmed();
    key_str = key_str.trimmed();

    if(snapshot_name.isEmpty() || source_count_name.isEmpty() || key_str.isEmpty())
        return;

    QString dest_plist_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "snapshots.plist";

    if(!QFileInfo(dest_plist_path).exists())
        return;

    NSAutoreleasePool* pool  = [[NSAutoreleasePool alloc] init];
    NSArray *data_read_array = [[NSMutableArray alloc]initWithContentsOfFile:dest_plist_path.toNSString()];

    if(data_read_array == nil)
    {
        [pool drain];
        return;
    }

    NSMutableArray *data_write_array = [[NSMutableArray alloc] init];
    for(id data_dict in data_read_array)
    {
        NSString *source_count_str  = data_dict[QString(MACRO_APFS_Snapshot_Plist_Key_Source_Count_Name).toNSString()];
        NSString *snapshot_name_str = data_dict[QString(MACRO_APFS_Snapshot_Plist_Key_Snapshot_Name).toNSString()];

        if(source_count_str == nil || snapshot_name_str == nil)
            continue;

        if(source_count_name == QString::fromNSString(source_count_str) && snapshot_name == QString::fromNSString(snapshot_name_str))
        {
            [data_dict setValue:value_str.toNSString() forKey:key_str.toNSString()];
        }

        [data_write_array addObject:data_dict];
    }

    [data_write_array writeToFile:dest_plist_path.toNSString() atomically:YES];

    [data_write_array release];

    [pool drain];
}

void witness_info_store::pub_update_apfs_snapshots_info_in_plist_file()
{
    recon_static_functions::app_debug(" -Start", Q_FUNC_INFO);

    QStringList saved_snapshots_list = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Snapshots_APFS_QStringList).toStringList();
    if(saved_snapshots_list.isEmpty())
        return;


    QString source_count_name_str  = global_narad_muni_class_obj->get_field(MACRO_NARAD_Source_Count_Name_QString).toString();
    QString destination_plist_file = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "snapshots.plist";

    bool bool_plist_exists = QFileInfo(destination_plist_file).exists();

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSMutableArray *array = nil;
    if(bool_plist_exists)
        array = [[NSMutableArray alloc] initWithContentsOfFile:destination_plist_file.toNSString()];
    else
        array = [[NSMutableArray alloc] init];

    if(array == nil)
    {
        [pool drain];
        return;
    }

    for(int count = 0; count < saved_snapshots_list.size(); count++)
    {
        QString snapshot_name_str = saved_snapshots_list.at(count).trimmed();

        if(snapshot_name_str.isEmpty())
            continue;

        NSString *source_count_name = source_count_name_str.toNSString();
        NSString *snapshot_name  = snapshot_name_str.toNSString();
        NSString *fs_run_status  = QString("0").toNSString();

        NSMutableDictionary *sources_dict = [[NSMutableDictionary alloc] init];

        [sources_dict setObject:fs_run_status     forKey:QString(MACRO_APFS_Snapshot_Plist_Key_FS_Run_Status).toNSString()];
        [sources_dict setObject:source_count_name forKey:QString(MACRO_APFS_Snapshot_Plist_Key_Source_Count_Name).toNSString()];
        [sources_dict setObject:snapshot_name     forKey:QString(MACRO_APFS_Snapshot_Plist_Key_Snapshot_Name).toNSString()];

        [array addObject:sources_dict];
    }

    [array writeToFile:destination_plist_file.toNSString() atomically:YES];

    [array release];
    [pool drain];

    recon_static_functions::app_debug(" -End", Q_FUNC_INFO);

}
