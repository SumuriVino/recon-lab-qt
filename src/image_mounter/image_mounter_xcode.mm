#include "image_mounter.h"
#import <Foundation/Foundation.h>
#include "recon_generic_functions/recon_static_functions.h"
#include <QtCore>

QStringList image_mounter::get_encrypted_apfs_volume_disk_node()
{
    //    "diskutil apfs list -plist"

    QStringList list_identifiers;

    QStringList list_arg;
    list_arg << MACRO_FILE_SYSTEM_TYPE_APFS << "list" << "-plist";
    QString out_put = run_command_with_argument("diskutil",list_arg);
    NSDictionary *m_plist = [NSPropertyListSerialization propertyListWithData:[out_put.toNSString() dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions format:NULL error:NULL];

    NSString *info_name = @"Containers";
    NSArray  *all_disk_array    = [m_plist objectForKey:info_name];
    NSObject *my_object = [m_plist objectForKey:info_name];
    if(all_disk_array != nil && [my_object isKindOfClass:[NSArray class]])
    {
        for(NSUInteger i = 0 ; i < [all_disk_array count] ; i++)
        {
            my_object = [all_disk_array objectAtIndex:i];
            if(![my_object isKindOfClass:[NSDictionary class]])
                continue;

            //- For inside item 0 Dictionary.
            NSDictionary *item_dictionary = [all_disk_array objectAtIndex:i];
            NSString *volume_str = @"Volumes";
            NSArray *array_for_volumes = [item_dictionary objectForKey:volume_str];

            for(id key in array_for_volumes)
            {

                NSDictionary *key_dictionary = key ;

                NSString *encryption_str = @"Encryption";
                NSNumber  *encryption_value_no   = [key_dictionary objectForKey:encryption_str];
                qint64 value = [encryption_value_no doubleValue];

                if(value != 1)
                    continue;



                NSString *identifier_str = @"DeviceIdentifier";
                NSString  *identifier_value_str   = [key_dictionary objectForKey:identifier_str];

                list_identifiers << QString::fromNSString(identifier_value_str);

            }
        }
    }



    return list_identifiers;

}

QString image_mounter::get_volume_name_using_diskutil_info(QString device_node)
{

    /// 1:                APFS Volume Forenso_1013              46.3 GB    disk5s1

    /// Input: disk5s1
    /// Return:  Forenso_1013

    //---------------------------------------------//



    QStringList list_arg;
    list_arg << "info" <<  "-plist" << device_node;
    QString out_put = run_command_with_argument("diskutil",list_arg);
    NSDictionary *m_plist = [NSPropertyListSerialization propertyListWithData:[out_put.toNSString() dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions format:NULL error:NULL];


    NSString *info_name = @"VolumeName";

    NSString  *str_volume_name    = [m_plist objectForKey:info_name];

    if(str_volume_name == nil)
        return QString("");
    else
        return QString::fromNSString(str_volume_name);


}


