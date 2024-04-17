#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#include "recon_helper_singular.h"



qint64 recon_helper_singular::convert_nsdate_to_utc_numeric(NSDate *ns_date_field , NSObject *my_object, QString caller_func)
{

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);

    ///Input
    ///   -----26-Nov-2018 at 2:38:34 PM
    ///OutPut
    ///    ----1543243114


    qint64 epch_time = 0;
    if(ns_date_field == nil || ![my_object isKindOfClass:[NSDate class]])
        return epch_time;

    NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
    [dateFormatter setDateFormat:@"dd-MMM-yyyy,HH:mm:ss a"];
    NSString *datetime_NSString = [dateFormatter stringFromDate:ns_date_field];
    QString datetime_str = QString::fromNSString(datetime_NSString);

    QString format = "dd-MMM-yyyy,HH:mm:ss a";
    QDateTime valid = QDateTime::fromString(datetime_str, format).toTimeSpec(Qt::UTC);
    valid.setTimeSpec(Qt::UTC);
    epch_time = (valid.toMSecsSinceEpoch())/1000;


    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -end " , Q_FUNC_INFO);

    return epch_time;
}

void recon_helper_singular::pub_remove_info_from_snapshots_plist_for_source(QString source_count_name)
{
    source_count_name = source_count_name.trimmed();


    if(source_count_name.isEmpty())
        return;

    QString dest_plist_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Conf_Path_Location_Sources_In_Result_QString).toString() + "snapshots.plist";


    if(!QFileInfo(dest_plist_path).exists())
        return;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

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

        if(source_count_str == nil)
            continue;

        if(source_count_name == QString::fromNSString(source_count_str))
            continue;

        [data_write_array addObject:data_dict];
    }


    [data_write_array writeToFile:dest_plist_path.toNSString() atomically:YES];

    [data_write_array release];

    [pool drain];
}



void recon_helper_singular::fill_apple_metadata_from_api(QString filepath, QStringList keys_list , struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj, QString caller_func)
{
    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " -Starts " , Q_FUNC_INFO);


    extended_attr_parsed_data_obj.extended_attrs_keys_list.clear();
    extended_attr_parsed_data_obj.extended_attrs_values_list.clear();

    //=============================Extended Attributes=========================================///
    MDItemRef item = MDItemCreate(NULL, (CFStringRef)filepath.toNSString());
    if(item == NULL)
    {
        set_blank_apple_metadata(keys_list, extended_attr_parsed_data_obj);
        return ;
    }

    for(int count = 0; count < keys_list.size(); count++)
    {
        QString key_str = keys_list.at(count);
        id type_check_obj = (id)CFBridgingRelease(MDItemCopyAttribute(item, (__bridge CFStringRef)key_str.toNSString()));

        QString value_str;

        /// value types check
        if([type_check_obj isKindOfClass:[NSString class]])
        {
            value_str = QString::fromNSString(type_check_obj);
        }
        else if([type_check_obj isKindOfClass:[NSNumber class]])
        {
            double value = [type_check_obj doubleValue];
            value_str = QString::number(value);
        }
        else if([type_check_obj isKindOfClass:[NSArray class]])
        {
            for(id data_type_obj in type_check_obj)
            {
                /// value types check for an array
                if([data_type_obj isKindOfClass:[NSString class]])
                {
                    QString tmp_data_str = QString::fromNSString(data_type_obj);

                    value_str.append(tmp_data_str);
                    value_str.append(QString(";"));
                }
                else if([data_type_obj isKindOfClass:[NSNumber class]])
                {
                    double tmp_data_num = [data_type_obj doubleValue];

                    value_str.append(QString::number(tmp_data_num));
                    value_str.append(QString(";"));
                }
                else if([data_type_obj isKindOfClass:[NSDate class]])
                {
                    NSTimeInterval ti = [data_type_obj timeIntervalSince1970];

                    if(ti != 0)
                    {
                        NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
                        data_type_obj = [NSString stringWithFormat:@"%f", ti];

                        if(intervalString != nil && [data_type_obj isKindOfClass:[NSString class]])
                        {
                            QString date_interval = QString::fromNSString(intervalString);

                            if(date_interval.contains("."))
                            {
                                QStringList temp = date_interval.split(".");

                                if(temp.size() > 0)
                                    date_interval = temp.at(0);
                            }


                            // date_interval = convert_epoch_to_custom(date_interval , Q_FUNC_INFO);


                            value_str.append(date_interval);
                            value_str.append(QString(";"));
                        }

                    }
                }

            }

            if(value_str.endsWith(QString(";")))
                value_str.chop(1);

        }
        else if([type_check_obj isKindOfClass:[NSDate class]])
        {
            NSTimeInterval ti = [type_check_obj timeIntervalSince1970];

            if(ti != 0)
            {
                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
                type_check_obj = [NSString stringWithFormat:@"%f", ti];

                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
                {
                    QString date_interval = QString::fromNSString(intervalString);

                    if(date_interval.contains("."))
                    {
                        QStringList temp = date_interval.split(".");

                        if(temp.size() > 0)
                            date_interval = temp.at(0);
                    }

                    // value_str = convert_epoch_to_custom(date_interval , Q_FUNC_INFO);
                    value_str = date_interval;
                }

            }
        }

        extended_attr_parsed_data_obj.extended_attrs_keys_list   << key_str;
        extended_attr_parsed_data_obj.extended_attrs_values_list << value_str;


        //        if(key_str == QString("kMDItemFSCreationDate"))
        //        {
        //            extended_attr_parsed_data_obj.kMDItemFSCreationDate_Created =  value_str;
        //        }
        //        if(key_str == QString("kMDItemFSContentChangeDate"))
        //        {
        //            extended_attr_parsed_data_obj.kMDItemFSContentChangeDate_Modified =  value_str;
        //        }
        //        if(key_str == QString("kMDItemLastUsedDate"))
        //        {
        //            extended_attr_parsed_data_obj.kMDItemLastUsedDate_Last_Opened_Date =  value_str;
        //        }
        //        if(key_str == QString("kMDItemDateAdded"))
        //        {
        //            extended_attr_parsed_data_obj.kMDItemDateAdded_Date_Added =  value_str;
        //        }

    }

    CFRelease(item);

    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Ends" , Q_FUNC_INFO);
    return ;


    //=============================Extended Attributes=========================================///


















    //===========Previous code

    //    NSString *target_file = filepath.toNSString();

    //    MDItemRef fileMetadata=MDItemCreate(NULL,(CFStringRef)target_file);

    //    if(fileMetadata == nil)
    //        return extended_attr_parsed_data_obj;

    //    NSDictionary *metadataDictionary = (__bridge NSDictionary*)MDItemCopyAttributes (fileMetadata,
    //                                                                                     (CFArrayRef)[NSArray arrayWithObjects:
    //            (id)kMDItemTitle,
    //            (id)kMDItemKind,
    //            (id)kMDItemFSSize,
    //            (id)kMDItemFSName,
    //            (id)kMDItemFSCreationDate,
    //            (id)kMDItemFSContentChangeDate,
    //            (id)kMDItemDurationSeconds,
    //            (id)kMDItemDisplayName,
    //            (id)kMDItemDateAdded,
    //            (id)kMDItemContentModificationDate,
    //            (id)kMDItemContentCreationDate,
    //            (id)kMDItemComment,
    //            (id)kMDItemAlbum,
    //            (id)kMDItemAudioBitRate,
    //            (id)kMDItemLastUsedDate,
    //            (id)kMDItemLatitude,
    //            (id)kMDItemLongitude,
    //            (id)kMDItemWhereFroms,
    //            nil]);



    //    /*
    //     * Do not remove this line , this is to create a file of all
    //     * the items to see the type of the value for parsing purpose.
    //     */
    //    //    // Write dictionary
    //    //[metadataDictionary writeToFile:@"/Users/moksha/tmp/extended_attr.plist" atomically:YES];




    //    if(metadataDictionary != NULL)
    //    {

    //        NSObject* type_check_obj;


    //        ///kMDItemTitle
    //        NSString* kMDItemTitle_key = @"kMDItemTitle";
    //        NSString* kMDItemTitle_value = [metadataDictionary objectForKey:kMDItemTitle_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemTitle_key];
    //        if(kMDItemTitle_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    //        {
    //            extended_attr_parsed_data_obj.kMDItemTitle_Title = [kMDItemTitle_value UTF8String];
    //        }

    //        /// kMDItemAudioBitRate
    //        double    kMDItemAudioBitRate = 0;
    //        NSString* kMDItemAudioBitRate_key = @"kMDItemAudioBitRate";
    //        NSNumber* kMDItemAudioBitRate_value = [metadataDictionary objectForKey:kMDItemAudioBitRate_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemAudioBitRate_key];
    //        if(kMDItemAudioBitRate_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    //        {
    //            kMDItemAudioBitRate = [kMDItemAudioBitRate_value doubleValue];

    //            extended_attr_parsed_data_obj.kMDItemAudioBitRate_Audio_Bit_Rate = QString::number(kMDItemAudioBitRate);
    //        }


    //        /// kMDItemComment
    //        NSString* kMDItemComment_key = @"kMDItemComment";
    //        NSString* kMDItemComment_value = [metadataDictionary objectForKey:kMDItemComment_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemComment_key];
    //        if(kMDItemComment_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    //        {
    //            extended_attr_parsed_data_obj.kMDItemComment_Comment = [kMDItemComment_value UTF8String];
    //        }

    //        /// kMDItemContentCreationDate
    //        NSString *kMDItemContentCreationDate_field = @"kMDItemContentCreationDate";
    //        NSDate* kMDItemContentCreationDate_value = [metadataDictionary objectForKey:kMDItemContentCreationDate_field];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemContentCreationDate_field];
    //        QString kMDItemContentCreationDate = "";
    //        if(kMDItemContentCreationDate_value != nil && [type_check_obj isKindOfClass:[NSDate class]])
    //        {

    //            NSTimeInterval ti = [kMDItemContentCreationDate_value timeIntervalSince1970];

    //            if(ti != 0)
    //            {
    //                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
    //                type_check_obj = [NSString stringWithFormat:@"%f", ti];

    //                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
    //                {
    //                    QString date_interval = [intervalString UTF8String];

    //                    if(date_interval.contains("."))
    //                    {
    //                        QStringList temp = date_interval.split(".");

    //                        if(temp.size() > 0)
    //                            date_interval = temp.at(0);
    //                    }

    //                    kMDItemContentCreationDate = date_interval;
    //                    kMDItemContentCreationDate = kMDItemContentCreationDate.trimmed();


    //                }

    //            }

    //        }



    //        extended_attr_parsed_data_obj.kMDItemContentCreationDate_Created_Date = convert_epoch_to_custom_for_display(kMDItemContentCreationDate , Q_FUNC_INFO);



    //        /// kMDItemContentModificationDate
    //        NSString *kMDItemContentModificationDate_field = @"kMDItemContentModificationDate";
    //        NSDate* kMDItemContentModificationDate_value = [metadataDictionary objectForKey:kMDItemContentModificationDate_field];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemContentModificationDate_field];
    //        QString kMDItemContentModificationDate = "";
    //        if(kMDItemContentModificationDate_value != nil && [type_check_obj isKindOfClass:[NSDate class]])
    //        {

    //            NSTimeInterval ti = [kMDItemContentModificationDate_value timeIntervalSince1970];

    //            if(ti != 0)
    //            {
    //                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
    //                type_check_obj = [NSString stringWithFormat:@"%f", ti];

    //                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
    //                {
    //                    QString date_interval = [intervalString UTF8String];

    //                    if(date_interval.contains("."))
    //                    {
    //                        QStringList temp = date_interval.split(".");

    //                        if(temp.size() > 0)
    //                            date_interval = temp.at(0);
    //                    }

    //                    kMDItemContentModificationDate = date_interval;
    //                    kMDItemContentModificationDate = kMDItemContentModificationDate.trimmed();


    //                }

    //            }

    //        }

    //        extended_attr_parsed_data_obj.kMDItemContentModificationDate_Last_Modified_Date = convert_epoch_to_custom_for_display(kMDItemContentModificationDate , Q_FUNC_INFO);



    //        /// kMDItemDateAdded
    //        NSString *kMDItemDateAdded_field = @"kMDItemDateAdded";
    //        NSDate* kMDItemDateAdded_value = [metadataDictionary objectForKey:kMDItemDateAdded_field];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemDateAdded_field];
    //        QString kMDItemDateAdded = "";
    //        if(kMDItemDateAdded_value != nil && [type_check_obj isKindOfClass:[NSDate class]])
    //        {

    //            NSTimeInterval ti = [kMDItemDateAdded_value timeIntervalSince1970];

    //            if(ti != 0)
    //            {
    //                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
    //                type_check_obj = [NSString stringWithFormat:@"%f", ti];

    //                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
    //                {
    //                    QString date_interval = [intervalString UTF8String];

    //                    if(date_interval.contains("."))
    //                    {
    //                        QStringList temp = date_interval.split(".");

    //                        if(temp.size() > 0)
    //                            date_interval = temp.at(0);
    //                    }

    //                    kMDItemDateAdded = date_interval;
    //                    kMDItemDateAdded = kMDItemDateAdded.trimmed();


    //                }

    //            }

    //        }

    //        extended_attr_parsed_data_obj.kMDItemDateAdded_Date_Added = convert_epoch_to_custom_for_display(kMDItemDateAdded , Q_FUNC_INFO);


    //        /// kMDItemDisplayName
    //        NSString* kMDItemDisplayName_key = @"kMDItemDisplayName";
    //        NSString* kMDItemDisplayName_value = [metadataDictionary objectForKey:kMDItemDisplayName_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemDisplayName_key];
    //        if(kMDItemDisplayName_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    //        {
    //            extended_attr_parsed_data_obj.kMDItemDisplayName_Name = [kMDItemDisplayName_value UTF8String];
    //        }



    //        /// kMDItemDurationSeconds
    //        double    kMDItemDurationSeconds = 0;
    //        NSString* kMDItemDurationSeconds_key = @"kMDItemDurationSeconds";
    //        NSNumber* kMDItemDurationSeconds_value = [metadataDictionary objectForKey:kMDItemDurationSeconds_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemDurationSeconds_key];
    //        if(kMDItemDurationSeconds_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    //        {
    //            kMDItemDurationSeconds = [kMDItemDurationSeconds_value doubleValue];

    //            extended_attr_parsed_data_obj.kMDItemDurationSeconds_Duration = QString::number(kMDItemDurationSeconds);

    //        }


    //        /// kMDItemFSContentChangeDate

    //        NSString *kMDItemFSContentChangeDate_field = @"kMDItemFSContentChangeDate";
    //        NSDate* kMDItemFSContentChangeDate_value = [metadataDictionary objectForKey:kMDItemFSContentChangeDate_field];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemFSContentChangeDate_field];
    //        QString kMDItemFSContentChangeDate = "";
    //        if(kMDItemFSContentChangeDate_value != nil && [type_check_obj isKindOfClass:[NSDate class]])
    //        {

    //            NSTimeInterval ti = [kMDItemFSContentChangeDate_value timeIntervalSince1970];

    //            if(ti != 0)
    //            {
    //                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
    //                type_check_obj = [NSString stringWithFormat:@"%f", ti];

    //                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
    //                {
    //                    QString date_interval = [intervalString UTF8String];

    //                    if(date_interval.contains("."))
    //                    {
    //                        QStringList temp = date_interval.split(".");

    //                        if(temp.size() > 0)
    //                            date_interval = temp.at(0);
    //                    }

    //                    kMDItemFSContentChangeDate = date_interval;
    //                    kMDItemFSContentChangeDate = kMDItemFSContentChangeDate.trimmed();


    //                }

    //            }

    //        }

    //        extended_attr_parsed_data_obj.kMDItemFSContentChangeDate_Modified = convert_epoch_to_custom_for_display(kMDItemFSContentChangeDate , Q_FUNC_INFO);



    //        /// kMDItemFSCreationDate
    //        NSString *kMDItemFSCreationDate_field = @"kMDItemFSCreationDate";
    //        NSDate* kMDItemFSCreationDate_value = [metadataDictionary objectForKey:kMDItemFSCreationDate_field];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemFSCreationDate_field];
    //        QString kMDItemFSCreationDate = "";
    //        if(kMDItemFSCreationDate_value != nil && [type_check_obj isKindOfClass:[NSDate class]])
    //        {

    //            NSTimeInterval ti = [kMDItemFSCreationDate_value timeIntervalSince1970];

    //            if(ti != 0)
    //            {
    //                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
    //                type_check_obj = [NSString stringWithFormat:@"%f", ti];

    //                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
    //                {
    //                    QString date_interval = [intervalString UTF8String];

    //                    if(date_interval.contains("."))
    //                    {
    //                        QStringList temp = date_interval.split(".");

    //                        if(temp.size() > 0)
    //                            date_interval = temp.at(0);
    //                    }

    //                    kMDItemFSCreationDate = date_interval;
    //                    kMDItemFSCreationDate = kMDItemFSCreationDate.trimmed();


    //                }

    //            }

    //        }

    //        extended_attr_parsed_data_obj.kMDItemFSCreationDate_Created = convert_epoch_to_custom_for_display(kMDItemFSCreationDate , Q_FUNC_INFO);



    //        /// kMDItemFSName
    //        NSString* kMDItemFSName_key = @"kMDItemFSName";
    //        NSString* kMDItemFSName_value = [metadataDictionary objectForKey:kMDItemFSName_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemFSName_key];
    //        if(kMDItemFSName_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    //        {
    //            extended_attr_parsed_data_obj.kMDItemFSName_File_System_File_Name = [kMDItemFSName_value UTF8String];
    //        }



    //        /// kMDItemFSSize
    //        double    kMDItemFSSize = 0;
    //        NSString* kMDItemFSSize_key = @"kMDItemFSSize";
    //        NSNumber* kMDItemFSSize_value = [metadataDictionary objectForKey:kMDItemFSSize_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemFSSize_key];
    //        if(kMDItemFSSize_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    //        {
    //            kMDItemFSSize = [kMDItemFSSize_value doubleValue];
    //            extended_attr_parsed_data_obj.kMDItemFSSize_File_Size = QString::number(kMDItemFSSize);
    //        }

    //        /// kMDItemKind
    //        NSString* kMDItemKind_key = @"kMDItemKind";
    //        NSString* kMDItemKind_value = [metadataDictionary objectForKey:kMDItemKind_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemKind_key];
    //        if(kMDItemKind_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    //        {
    //            extended_attr_parsed_data_obj.kMDItemKind_Kind = [kMDItemKind_value UTF8String];
    //        }

    //        ///kMDItemAlbum
    //        NSString* kMDItemAlbum_key = @"kMDItemAlbum";
    //        NSString* kMDItemAlbum_value = [metadataDictionary objectForKey:kMDItemAlbum_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemAlbum_key];
    //        if(kMDItemAlbum_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    //        {
    //            extended_attr_parsed_data_obj.kMDItemAlbum_Album = [kMDItemAlbum_value UTF8String];
    //        }

    //        /// kMDItemLastUsedDate
    //        NSString *kMDItemLastUsedDate_field = @"kMDItemLastUsedDate";
    //        NSDate* kMDItemLastUsedDate_value = [metadataDictionary objectForKey:kMDItemLastUsedDate_field];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemLastUsedDate_field];
    //        QString kMDItemLastUsedDate = "";
    //        if(kMDItemLastUsedDate_value != nil && [type_check_obj isKindOfClass:[NSDate class]])
    //        {

    //            NSTimeInterval ti = [kMDItemLastUsedDate_value timeIntervalSince1970];

    //            if(ti != 0)
    //            {
    //                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
    //                type_check_obj = [NSString stringWithFormat:@"%f", ti];

    //                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
    //                {
    //                    QString date_interval = [intervalString UTF8String];

    //                    if(date_interval.contains("."))
    //                    {
    //                        QStringList temp = date_interval.split(".");

    //                        if(temp.size() > 0)
    //                            date_interval = temp.at(0);
    //                    }

    //                    kMDItemLastUsedDate = date_interval;
    //                    kMDItemLastUsedDate = kMDItemLastUsedDate.trimmed();


    //                }

    //            }

    //        }

    //        extended_attr_parsed_data_obj.kMDItemLastUsedDate_Last_Opened_Date = convert_epoch_to_custom_for_display(kMDItemLastUsedDate , Q_FUNC_INFO);




    //        /// kMDItemLatitude
    //        double    kMDItemLatitude = 0;
    //        NSString* kMDItemLatitude_key = @"kMDItemLatitude";
    //        NSNumber* kMDItemLatitude_value = [metadataDictionary objectForKey:kMDItemLatitude_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemLatitude_key];
    //        if(kMDItemLatitude_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    //        {
    //            kMDItemLatitude = [kMDItemLatitude_value doubleValue];
    //            extended_attr_parsed_data_obj.kMDItemLatitude_Latitude = QString::number(kMDItemLatitude);

    //        }


    //        /// kMDItemLongitude
    //        double    kMDItemLongitude = 0;
    //        NSString* kMDItemLongitude_key = @"kMDItemLongitude";
    //        NSNumber* kMDItemLongitude_value = [metadataDictionary objectForKey:kMDItemLongitude_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemLongitude_key];
    //        if(kMDItemLongitude_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    //        {
    //            kMDItemLongitude = [kMDItemLongitude_value doubleValue];
    //            extended_attr_parsed_data_obj.kMDItemLongitude_Longitude = QString::number(kMDItemLongitude);
    //        }


    //        /// kMDItemWhereFroms
    //        QString wherefrom_str;
    //        NSString* kMDItemWhereFroms_key = @"kMDItemWhereFroms";
    //        NSArray * kMDItemWhereFroms_value = [metadataDictionary objectForKey:kMDItemWhereFroms_key];
    //        type_check_obj = [metadataDictionary objectForKey:kMDItemWhereFroms_key];
    //        if(kMDItemWhereFroms_value != nil || [type_check_obj isKindOfClass:[NSArray class]])
    //        {
    //            for(NSString *data_str in kMDItemWhereFroms_value)
    //            {
    //                wherefrom_str.append([data_str UTF8String]);
    //            }

    //            extended_attr_parsed_data_obj.kMDItemWhereFroms_Where_From = wherefrom_str.trimmed();
    //        }

    //    }
    //    else
    //        recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Dictionary Empty" , Q_FUNC_INFO);



    //    recon_static_functions::debug_intensive(recon_static_functions::prepare_callerfun(caller_func) + " - Ends" , Q_FUNC_INFO);

    //    return extended_attr_parsed_data_obj;

}

void recon_helper_singular::set_blank_apple_metadata(QStringList keys_list ,struct_global_apple_metadata_parsed_data &extended_attr_parsed_data_obj)
{
    extended_attr_parsed_data_obj.extended_attrs_keys_list.clear();
    extended_attr_parsed_data_obj.extended_attrs_values_list.clear();

    for(int count = 0; count < keys_list.size(); count++)
    {
        QString key_str = keys_list.at(count);

        extended_attr_parsed_data_obj.extended_attrs_keys_list   << key_str;
        extended_attr_parsed_data_obj.extended_attrs_values_list << QString("");
    }
}


QStringList recon_helper_singular::get_apple_metadata_from_api(QString m_file_path)
{

    QStringList m_list;
    //=============================Extended Attributes=========================================///


    NSString *path = m_file_path.toNSString();
    MDItemRef item = MDItemCreate(NULL, (CFStringRef)path);

    if(item == NULL)
    {
        return m_list;
    }


    CFArrayRef attributeNames = MDItemCopyAttributeNames(item);

    if(attributeNames == NULL)
    {
        return m_list;
    }


    NSArray *m_attributes = (__bridge NSArray*)attributeNames;




    for(NSUInteger count = 0; count < [m_attributes count]; count++)
    {
        QString field_str;
        NSString* field_item = [m_attributes objectAtIndex:count];
        if([field_item isKindOfClass:[NSString class]])
        {
            field_str = QString::fromNSString(field_item);
        }

        CFStringRef key_ref = (__bridge CFStringRef)field_str.toNSString();

        id type_check_obj = (id)CFBridgingRelease(MDItemCopyAttribute(item, key_ref));

        QString value_str;

        /// value types check

        if([type_check_obj isKindOfClass:[NSString class]])
        {
            value_str = QString::fromNSString(type_check_obj);
        }
        else if([type_check_obj isKindOfClass:[NSNumber class]])
        {
            double value = [type_check_obj doubleValue];
            value_str = QString::number(value);
        }
        else if([type_check_obj isKindOfClass:[NSArray class]])
        {
            for(id data_type_obj in type_check_obj)
            {
                /// value types check for an array

                if([data_type_obj isKindOfClass:[NSString class]])
                {
                    QString tmp_data_str = QString::fromNSString(data_type_obj);

                    value_str.append(tmp_data_str);
                    value_str.append("\n");
                }
                else if([data_type_obj isKindOfClass:[NSNumber class]])
                {
                    double tmp_data_num = [data_type_obj doubleValue];

                    value_str.append(QString::number(tmp_data_num));
                    value_str.append("\n");
                }
                else if([data_type_obj isKindOfClass:[NSDate class]])
                {
                    NSTimeInterval ti = [data_type_obj timeIntervalSince1970];

                    if(ti != 0)
                    {
                        NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
                        data_type_obj = [NSString stringWithFormat:@"%f", ti];

                        if(intervalString != nil && [data_type_obj isKindOfClass:[NSString class]])
                        {
                            QString date_interval = QString::fromNSString(intervalString);

                            if(date_interval.contains("."))
                            {
                                QStringList temp = date_interval.split(".");

                                if(temp.size() > 0)
                                    date_interval = temp.at(0);
                            }


                            value_str.append(convert_numeric_to_readable_in_selected_timezone(date_interval, Q_FUNC_INFO));
                            value_str.append("\n");
                        }

                    }
                }

            }

            value_str = value_str.trimmed();

        }
        else if([type_check_obj isKindOfClass:[NSDate class]])
        {
            NSTimeInterval ti = [type_check_obj timeIntervalSince1970];

            if(ti != 0)
            {
                NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
                type_check_obj = [NSString stringWithFormat:@"%f", ti];

                if(intervalString != nil && [type_check_obj isKindOfClass:[NSString class]])
                {
                    QString date_interval = QString::fromNSString(intervalString);

                    if(date_interval.contains("."))
                    {
                        QStringList temp = date_interval.split(".");

                        if(temp.size() > 0)
                            date_interval = temp.at(0);
                    }

                    value_str = convert_numeric_to_readable_in_selected_timezone(date_interval, Q_FUNC_INFO);
                }

            }
        }


        if(!value_str.isEmpty())
        {
            m_list << field_str + MACRO_RECON_Splitter_1_multiple + value_str;
        }

    }


    path = NULL;
    m_attributes = NULL;
    CFRelease(attributeNames);


    CFRelease(item);


    return m_list;


    //    QStringList list;
    //    CFTypeRef attrValue = NULL;
    //    QString value_str ="";

    //    CFStringRef filePath = NULL;
    //    MDItemRef item = NULL;

    //    filePath = (__bridge CFStringRef)filepath.toNSString();
    //    item = MDItemCreate(kCFAllocatorDefault, filePath);

    //    if(item == NULL)
    //    {
    //        return list;
    //    }

    //    NSObject* my_object;
    //    NSArray *my_attributes = (NSArray*)CFBridgingRelease(MDItemCopyAttributeNames(item));
    //    my_object = (NSArray*)CFBridgingRelease(MDItemCopyAttributeNames(item));

    //    if(my_attributes == nil || ![my_object isKindOfClass:[NSArray class]])
    //    {
    //        return list;
    //    }

    //    for(NSUInteger i = 0 ; i < [my_attributes count]; ++i)
    //    {
    //        QString field_str;
    //        NSString* field_item = [my_attributes objectAtIndex:i];
    //        my_object = [my_attributes objectAtIndex:i];
    //        if(field_item != nil && [my_object isKindOfClass:[NSString class]])
    //        {
    //            field_str = QString::fromNSString(field_item);
    //        }

    //        CFStringRef name = (CFStringRef)field_item;
    //        CFStringRef  description = MDSchemaCopyDisplayDescriptionForAttribute (name);

    //        QString desc_str;
    //        NSString *ns_description = (NSString*)description;
    //        my_object = (NSString*)description;
    //        if(ns_description != nil && [my_object isKindOfClass:[NSString class]])
    //        {
    //            desc_str =  QString::fromNSString(ns_description);
    //        }

    //        attrValue = MDItemCopyAttribute(item, name);
    //        if(attrValue != NULL)
    //        {
    //            if (CFGetTypeID(attrValue) == CFStringGetTypeID())
    //            {
    //                CFStringRef m_string = CFCopyDescription (attrValue);

    //                CFIndex length = CFStringGetLength(m_string);
    //                CFIndex maxSize =
    //                        CFStringGetMaximumSizeForEncoding(length,
    //                                                          kCFStringEncodingUTF8);
    //                char *buffer = (char *)malloc(maxSize);
    //                if (CFStringGetCString(m_string, buffer, maxSize,
    //                                       kCFStringEncodingUTF8))
    //                {

    //                }

    //                QString m_display_name = QString::fromLocal8Bit(buffer);
    //                QString display_value = "";

    //                QStringList m_list = m_display_name.split("{contents = ");
    //                if(m_list.size() > 1)
    //                {
    //                    display_value = m_list.at(1);
    //                    display_value.remove("}");
    //                    display_value.remove("\"");
    //                    display_value =  display_value.trimmed();
    //                }

    //                value_str = display_value;
    //            }
    //            else if(CFGetTypeID(attrValue) == CFArrayGetTypeID())
    //            {
    //                CFStringRef m_string1 = CFCopyDescription (attrValue);
    //                const char *m_C_string1 = CFStringGetCStringPtr(m_string1, kCFStringEncodingMacRoman);

    //                QString temp_wherefrom = QString::fromLocal8Bit(m_C_string1);
    //                QString m_wherefrom = temp_wherefrom.trimmed();

    //                m_wherefrom.remove("\"");
    //                m_wherefrom.remove("(\n");
    //                m_wherefrom.remove("\n)");
    //                m_wherefrom.replace(",\n", "\n");
    //                m_wherefrom.replace("<", "(");
    //                m_wherefrom.replace(">", ")");
    //                m_wherefrom = m_wherefrom.trimmed();
    //                m_wherefrom = convert_string_to_unicode(m_wherefrom, Q_FUNC_INFO);
    //                value_str = m_wherefrom;

    //            }
    //            else if(CFGetTypeID(attrValue) == CFNumberGetTypeID())
    //            {
    //                CFStringRef m_CF_fs_size = CFCopyDescription (attrValue);
    //                const char *m_C_fs_size = CFStringGetCStringPtr(m_CF_fs_size, kCFStringEncodingMacRoman);
    //                QString fs_value = "";
    //                QString m_fs_size = m_C_fs_size;
    //                QStringList list_1 = m_fs_size.split(", ");
    //                m_fs_size = list_1.at(0);
    //                QStringList temp_list = m_fs_size.split("value = ");

    //                if(temp_list.size() > 1)
    //                {
    //                    fs_value = temp_list.at(1);
    //                    fs_value = fs_value.trimmed();
    //                }
    //                value_str = fs_value;
    //            }
    //            else if(CFGetTypeID(attrValue) == CFDateGetTypeID())
    //            {
    //                CFStringRef m_CF_last_used_date = CFCopyDescription (attrValue);
    //                const char *m_C_last_used_date = CFStringGetCStringPtr(m_CF_last_used_date, kCFStringEncodingMacRoman);

    //                QString m_last_used_date = m_C_last_used_date;
    //                value_str = m_last_used_date;
    //            }
    //            else if(CFGetTypeID(attrValue) == CFBooleanGetTypeID())
    //            {
    //                CFStringRef m_string = CFCopyDescription (attrValue);
    //                const char *m_C_string = CFStringGetCStringPtr(m_string, kCFStringEncodingMacRoman);

    //                QString m_display_name = m_C_string;
    //                QStringList m_list = m_display_name.split("value = ");

    //                QString display_value;
    //                if(m_list.size() > 1)
    //                {
    //                    display_value = m_list.at(1);
    //                    display_value.remove("}");
    //                    display_value.remove("\"");
    //                    display_value =  display_value.trimmed();
    //                }
    //                value_str = display_value;
    //            }
    //        }
    //        else
    //            value_str = "";

    //        list << field_str + MACRO_RECON_Splitter + value_str;
    //    }

    //    CFRelease(item);

    //    return list;

}
