#include "plist_viewer.h"

#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>


// Declaration
@interface xcode_plist_viewer_class : NSObject
{}
@property QString _plist_view_dbtable_name;

-(QString)get_plist_nsstring_values:(NSString *) nsitem Anddict:(NSDictionary *) m_plist_dict;
-(QString)get_nsnumber_values:(NSString *) nsitem Anddict:(NSDictionary *) m_plist_dict;
-(QByteArray)get_nsdata_values:(NSString *) nsitem Anddict:(NSDictionary *) m_plist_dict;
-(QString)get_nsdate_values:(NSString *) nsitem Anddict:(NSDictionary *) m_plist_dict;

-(void)insert_xcodedata_in_db:(QSqlQuery) insert_query andvalue:(QString) value_str andkey:(QString) key_str andtype:(QString) type_str andkeypath:(QString) keypath_str;
-(void)sub_xcode_extract_plistdata_nsitems:(NSString*) nsitem anddict:(NSDictionary*) m_plist_dict andarr:(NSArray*) nsarr andbool:(bool) bool_is_array andquery:(QSqlQuery) insert_query;

-(void)xcode_extract_plistdata:(QString) source_file anddbname:(QString) plist_view_db_table_name andquery:(QSqlQuery) insert_query;

@end

static QString parent_path_key;

//Definition
@implementation xcode_plist_viewer_class

@synthesize _plist_view_dbtable_name = plist_view_dbtable_name;

-(QString )get_plist_nsstring_values:(NSString *) nsitem Anddict: (NSDictionary *) m_plist_dict
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* my_object;

    my_object = [m_plist_dict objectForKey:nsitem];

    if(![my_object isKindOfClass:[NSString class]])
        return QString("");

    NSString *item_name = [m_plist_dict objectForKey:nsitem];
    QString str_nsstringitem = "";

    if(item_name != nil && [my_object isKindOfClass:[NSString class]])
    {
        str_nsstringitem = QString::fromNSString(item_name);
        str_nsstringitem = str_nsstringitem.trimmed();
    }
    else
    {
        QString str = QString::fromNSString(nsitem);
        recon_static_functions::app_debug("-----FAILED----- to get value: " + str, Q_FUNC_INFO);
    }

    [pool drain];
    return str_nsstringitem;
}

-(QString )get_nsnumber_values:(NSString *) nsitem Anddict: (NSDictionary *) m_plist_dict
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* my_object;

    my_object = [m_plist_dict objectForKey:nsitem];

    if(![my_object isKindOfClass:[NSNumber class]])
        return QString("");

    NSNumber *item_name = [m_plist_dict objectForKey:nsitem];
    QString str_nsnumberitem = "";

    if(item_name != nil && [my_object isKindOfClass:[NSNumber class]])
    {
        qint64 value = [item_name doubleValue];

        NSString *strClass = NSStringFromClass([my_object class]);
        QString classtype = QString::fromNSString(strClass);
        classtype = classtype.trimmed();
        if(classtype == "__NSCFBoolean")
        {
            switch (value) {
            case 0:

                str_nsnumberitem = "NO";
                break;
            case 1:
                str_nsnumberitem = "YES";
                break;
            default:
                break;
            }
        }

        if(classtype == "__NSCFNumber")
        {
            str_nsnumberitem = QString::number(value);
        }

        str_nsnumberitem = str_nsnumberitem.trimmed();
    }
    else
    {
        QString str = QString::fromNSString(nsitem);
        recon_static_functions::app_debug("-----FAILED----- to get value: " + str, Q_FUNC_INFO);
    }

    [pool drain];
    return str_nsnumberitem;
}

-(QByteArray )get_nsdata_values:(NSString *) nsitem Anddict: (NSDictionary *) m_plist_dict
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* my_object;

    my_object = [m_plist_dict objectForKey:nsitem];

    if(![my_object isKindOfClass:[NSData class]])
        return QByteArray("");

    NSData *item_name = [m_plist_dict objectForKey:nsitem];

    QByteArray data_read = "";

    if(item_name != nil && [my_object isKindOfClass:[NSData class]])
    {
        QByteArray fileBytes = QByteArray::fromNSData(item_name);
        data_read = fileBytes.toHex();
    }else
    {
        QString str = QString::fromNSString(nsitem) ;
        recon_static_functions::app_debug("-----FAILED----- to get value: " + str, Q_FUNC_INFO);
    }

    [pool drain];
    return data_read;
}

-(QString )get_nsdate_values:(NSString *) nsitem Anddict: (NSDictionary *) m_plist_dict
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* my_object;

    my_object = [m_plist_dict objectForKey:nsitem];

    if(![my_object isKindOfClass:[NSDate class]])
        return QString("");

    NSDate *item_name = [m_plist_dict objectForKey:nsitem];

    QString str_date = "";

    if(item_name != nil && [my_object isKindOfClass:[NSDate class]])
    {
//        NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
//        [dateFormatter setDateFormat:@"dd-MMM-yyyy,HH:mm:ss a"];
//        NSString *dateString = [dateFormatter stringFromDate:item_name];
//        str_date = QString::fromNSString(dateString);

        qint64 utc_epch =  global_recon_helper_singular_class_obj->convert_nsdate_to_utc_numeric(item_name, my_object, Q_FUNC_INFO);
        str_date = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_utc_timezone(QString::number(utc_epch), Q_FUNC_INFO);

    }
    else
    {
        QString str = QString::fromNSString(nsitem);
        recon_static_functions::app_debug("-----FAILED----- to get value: " + str, Q_FUNC_INFO);
    }

    [pool drain];
    return str_date;
}

-(void)sub_xcode_extract_plistdata_nsitems:(NSString*) nsitem anddict:(NSDictionary*) m_plist_dict andarr:(NSArray*) nsarr andbool:(bool) bool_is_array andquery:(QSqlQuery) insert_query
{
    recon_static_functions::app_debug("Starts " , Q_FUNC_INFO);
    ///this function calling recursively

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* my_object = nil;
    QString str_keyname = "";

    my_object = [m_plist_dict objectForKey:nsitem];
    if(nsitem != nil)
    {
        str_keyname = QString::fromNSString(nsitem);
        parent_path_key.append(str_keyname + "/");
    }

    self = [super init];

    ///check if class type is number
    if([my_object isKindOfClass:[NSNumber class]])
    {
        QString num = [self get_nsnumber_values:nsitem Anddict:m_plist_dict];
        NSString *strClass = NSStringFromClass([my_object class]);
        QString classtype = QString::fromNSString(strClass);
        classtype = classtype.trimmed();
        QString type = ((classtype == "__NSCFBoolean") ? "Boolean" : "Number");

        if(parent_path_key.endsWith("/"))
            parent_path_key.chop(1);

        [self insert_xcodedata_in_db:insert_query andvalue:num andkey:str_keyname andtype:type andkeypath:parent_path_key];
        parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
    }

    ///check if class type is string
    if([my_object isKindOfClass:[NSString class]])
    {
        QString str_name = [self get_plist_nsstring_values:nsitem Anddict:m_plist_dict];
        if(parent_path_key.endsWith("/"))
            parent_path_key.chop(1);

        [self insert_xcodedata_in_db:insert_query andvalue:str_name andkey:str_keyname andtype:"String" andkeypath:parent_path_key];
        parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
    }

    ///check if class is data
    if([my_object isKindOfClass:[NSData class]])
    {
        QByteArray value_name = [self get_nsdata_values:nsitem Anddict:m_plist_dict];

        if(parent_path_key.endsWith("/"))
            parent_path_key.chop(1);

        [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:str_keyname andtype:"Data" andkeypath:parent_path_key];
        parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
    }

    ///check if class is date
    if([my_object isKindOfClass:[NSDate class]])
    {
        QString value_name = [self get_nsdate_values:nsitem Anddict:m_plist_dict];

        if(parent_path_key.endsWith("/"))
            parent_path_key.chop(1);

        [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:str_keyname andtype:"Date" andkeypath:parent_path_key];
        parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
    }

    ///check if class type is array,, again array can contain any type of class like string, dictionary, number or array etc...so we have to make these checkes inside the array
    if([my_object isKindOfClass:[NSArray class]])
    {
        NSArray *item_name = [m_plist_dict objectForKey:nsitem];
        if(item_name != nil && [my_object isKindOfClass:[NSArray class]])
        {
            int itemscount = [item_name count];
            QString item_keyname = parent_path_key;
            if(item_keyname.endsWith("/"))
                item_keyname.chop(1);

            item_keyname = item_keyname.remove(0,item_keyname.lastIndexOf("/") + 1);
            if([item_name count] == 0)
            {
                QString value_name = QString::number(itemscount);
                [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:item_keyname andtype:"Array" andkeypath:parent_path_key];
                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);

                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());

            }
            else
            {
                int itemscount = [item_name count];
                QString value_name = QString::number(itemscount);
                [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:item_keyname andtype:"Array" andkeypath:parent_path_key];
            }


            int count = 0;
            for(NSUInteger j = 0 ; j < [item_name count]; j++)
            {
                count++;
                if(count % 20 == 0)
                    QCoreApplication::processEvents();

                parent_path_key.append("item" + QString::number(j) + "/");
                my_object = [item_name objectAtIndex:j];

                ///check if class is array
                if([my_object isKindOfClass:[NSArray class]])
                {
                    NSArray *item_arr = [nsarr objectAtIndex:j];
                    QString tmp_key_path = parent_path_key;
                    [self sub_xcode_extract_plistdata_nsitems:nil anddict:nil andarr:item_arr andbool:true andquery:insert_query];
                    parent_path_key = tmp_key_path;
                }

                ///check if class is number
                if([my_object isKindOfClass:[NSNumber class]])
                {
                    NSNumber *num = [item_name objectAtIndex:j];
                    float numb = [num doubleValue];

                    QString value_name = "", type = "";

                    NSString *strClass = NSStringFromClass([my_object class]);
                    QString classtype = QString::fromNSString(strClass);
                    classtype = classtype.trimmed();
                    if(classtype == "__NSCFBoolean")
                    {
                        int value = numb;
                        type = "Boolean";

                        switch (value)
                        {
                        case 0:
                            value_name = "NO";
                            break;

                        case 1:
                            value_name = "YES";
                            break;

                        default:
                            break;
                        }
                    }

                    if(classtype == "__NSCFNumber")
                    {
                        type = "Number";
                        value_name = QString::number(numb);
                    }

                    if(parent_path_key.endsWith("/"))
                        parent_path_key.chop(1);

                    [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:type andkeypath:parent_path_key];
                    parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                }

                ///check if class is string
                if([my_object isKindOfClass:[NSString class]])
                {
                    NSString *str_name = [item_name objectAtIndex:j];

                    QString ss = QString::fromNSString(str_name);

                    if(parent_path_key.endsWith("/"))
                        parent_path_key.chop(1);

                    [self insert_xcodedata_in_db:insert_query andvalue:ss andkey:QString("item " + QString::number(j)) andtype:"String" andkeypath:parent_path_key];
                    parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                }

                ///check if class is dictionary
                if([my_object isKindOfClass:[NSDictionary class]])
                {
                    NSDictionary *dict_value = [item_name objectAtIndex:j];
                    my_object = [item_name objectAtIndex:j];
                    if(dict_value != nil && [my_object isKindOfClass:[NSDictionary class]])
                    {
                        if([dict_value count] == 0)
                        {
                            QString value_name = QString::number(0);
                            [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:"Dictionary" andkeypath:parent_path_key];

                            if(parent_path_key.endsWith("/"))
                                parent_path_key.chop(1);

                            parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                        }
                        else
                        {
                            int itemscount = [dict_value count];
                            QString value_name = QString::number(itemscount);
                            [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:"Dictionary" andkeypath:parent_path_key];
                        }

                        for(id key in dict_value)
                        {
                            QString tmp_key_path = parent_path_key;
                            [self sub_xcode_extract_plistdata_nsitems:key anddict:dict_value andarr:nil andbool:false andquery:insert_query];
                            parent_path_key = tmp_key_path;
                        }

                        if(parent_path_key.endsWith("/"))
                            parent_path_key.chop(1);

                        parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                    }
                }

                ///check if class is data
                if([my_object isKindOfClass:[NSData class]])
                {
                    NSData *data_value = [item_name objectAtIndex:j];
                    QByteArray data_read = QByteArray::fromNSData(data_value);
                    data_read = data_read.toHex();

                    if(parent_path_key.endsWith("/"))
                        parent_path_key.chop(1);

                    [self insert_xcodedata_in_db:insert_query andvalue:data_read andkey:QString("item " + QString::number(j)) andtype:"Data" andkeypath:parent_path_key];
                    parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                }

                ///check if class is date
                if([my_object isKindOfClass:[NSDate class]])
                {
                    NSDate *date_value = [item_name objectAtIndex:j];
                    QString str_date = "";

//                    NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
//                    [dateFormatter setDateFormat:@"dd-MMM-yyyy,HH:mm:ss a"];
//                    NSString *dateString = [dateFormatter stringFromDate:date_value];
//                    str_date = QString::fromNSString(dateString);

                    qint64 utc_epch =  global_recon_helper_singular_class_obj->convert_nsdate_to_utc_numeric(date_value, my_object, Q_FUNC_INFO);
                    str_date = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_utc_timezone(QString::number(utc_epch), Q_FUNC_INFO);

                    if(parent_path_key.endsWith("/"))
                        parent_path_key.chop(1);

                    [self insert_xcodedata_in_db:insert_query andvalue:str_date andkey:QString("item " + QString::number(j)) andtype:"Date" andkeypath:parent_path_key];
                    parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                }

                ///Remove last appended child from the parent path
                if(j == [item_name count] - 1)
                {
                    if(parent_path_key.endsWith("/"))
                        parent_path_key.chop(1);
                    parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                }
            }
        }
    }

    ///there can be nested array, just like array class can have children of array type, so we will use this approach to handle this concept.
    if(bool_is_array && (nsarr != nil))
    {
        int itemscount = [nsarr count];

        QString item_keyname = parent_path_key;
        if(item_keyname.endsWith("/"))
            item_keyname.chop(1);

        item_keyname = item_keyname.remove(0,item_keyname.lastIndexOf("/") + 1);

        if([nsarr count] == 0)
        {
            QString value_name = QString::number(itemscount);
            [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:item_keyname andtype:"Array" andkeypath:parent_path_key];

            if(parent_path_key.endsWith("/"))
                parent_path_key.chop(1);

            parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
        }
        else
        {
            int itemscount = [nsarr count];
            QString value_name = QString::number(itemscount);
            [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:item_keyname andtype:"Array" andkeypath:parent_path_key];
        }

        int count = 0;
        for(NSUInteger j = 0 ; j < [nsarr count]; j++)
        {
            count++;
            if(count % 20 == 0)
                QCoreApplication::processEvents();
            parent_path_key.append("item" + QString::number(j) + "/");
            my_object = [nsarr objectAtIndex:j];

            ///check if class is array
            if([my_object isKindOfClass:[NSArray class]])
            {
                NSArray *item_arr = [nsarr objectAtIndex:j];
                [self sub_xcode_extract_plistdata_nsitems:nil anddict:nil andarr:item_arr andbool:true andquery:insert_query];
            }

            ///check if class is number
            if([my_object isKindOfClass:[NSNumber class]])
            {
                NSNumber *num = [nsarr objectAtIndex:j];
                QString value_name = "", type = "";
                float numb = [num doubleValue];

                NSString *strClass = NSStringFromClass([my_object class]);
                QString classtype = QString::fromNSString(strClass);
                classtype = classtype.trimmed();
                if(classtype == "__NSCFBoolean")
                {
                    int value = numb;
                    type = "Boolean";

                    switch (value) {

                    case 0:
                        value_name = "NO";
                        break;
                    case 1:
                        value_name = "YES";
                        break;

                    default:
                        break;
                    }
                }

                if(classtype == "__NSCFNumber")
                {
                    type = "Number";
                    value_name = QString::number(numb);
                }

                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);

                [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:type andkeypath:parent_path_key];
                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
            }

            ///check if class is string
            if([my_object isKindOfClass:[NSString class]])
            {
                NSString *str_name = [nsarr objectAtIndex:j];
                QString value_name = QString::fromNSString(str_name);

                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);

                [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:"String" andkeypath:parent_path_key];
                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
            }

            ///check if class is dictionary
            if([my_object isKindOfClass:[NSDictionary class]])
            {
                NSDictionary *dict_value = [nsarr objectAtIndex:j];
                my_object = [nsarr objectAtIndex:j];
                if(dict_value != nil && [my_object isKindOfClass:[NSDictionary class]])
                {
                    if([dict_value count] == 0)
                    {
                        QString value_name = QString::number(0);
                        [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:"Dictionary" andkeypath:parent_path_key];

                        parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());

                        if(parent_path_key.endsWith("/"))
                            parent_path_key.chop(1);
                    }
                    else
                    {
                        int itemscount = [dict_value count];
                        QString value_name = QString::number(itemscount);
                        [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:QString("item " + QString::number(j)) andtype:"Dictionary" andkeypath:parent_path_key];
                    }

                    for(id key in dict_value)
                    {
                        QString tmp_key_path = parent_path_key;
                        [self sub_xcode_extract_plistdata_nsitems:key anddict:dict_value andarr:nil andbool:false andquery:insert_query];
                        parent_path_key = tmp_key_path;
                    }

                    if(parent_path_key.endsWith("/"))
                        parent_path_key.chop(1);

                    parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                }
            }

            ///check if class is data
            if([my_object isKindOfClass:[NSData class]])
            {
                NSData *data_value = [nsarr objectAtIndex:j];

                QByteArray data_read = QByteArray::fromNSData(data_value);
                data_read = data_read.toHex();
                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);

                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
                [self insert_xcodedata_in_db:insert_query andvalue:data_read andkey:QString("item " + QString::number(j)) andtype:"Data" andkeypath:parent_path_key];
                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
            }

            ///check if class is date
            if([my_object isKindOfClass:[NSDate class]])
            {
                NSDate *date_value = [nsarr objectAtIndex:j];
                QString str_date = "";

//                NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
//                [dateFormatter setDateFormat:@"dd-MMM-yyyy,HH:mm:ss a"];
//                NSString *dateString = [dateFormatter stringFromDate:date_value];
//                str_date = QString::fromNSString(dateString);

                qint64 utc_epch =  global_recon_helper_singular_class_obj->convert_nsdate_to_utc_numeric(date_value, my_object, Q_FUNC_INFO);
                str_date = global_recon_helper_singular_class_obj->convert_numeric_to_readable_in_utc_timezone(QString::number(utc_epch), Q_FUNC_INFO);

                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);

                [self insert_xcodedata_in_db:insert_query andvalue:str_date andkey:QString("item " + QString::number(j)) andtype:"Date" andkeypath:parent_path_key];
                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
            }

            ///Remove last appended child from the parent path
            if(j == [nsarr count] - 1)
            {
                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);

                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
            }
        }
    }

    ///check if class is dictionary
    if([my_object isKindOfClass:[NSDictionary class]])
    {
        NSDictionary *dict_value = [m_plist_dict objectForKey:nsitem];
        my_object = [m_plist_dict objectForKey:nsitem];
        if(dict_value != nil && [my_object isKindOfClass:[NSDictionary class]])
        {
            if([dict_value count] == 0)
            {
                QString value_name = QString::number(0);
                [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:str_keyname andtype:"Dictionary" andkeypath:parent_path_key];
                parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());

                if(parent_path_key.endsWith("/"))
                    parent_path_key.chop(1);
            }
            else
            {
                int itemscount = [dict_value count];
                QString value_name = QString::number(itemscount);
                [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:str_keyname andtype:"Dictionary" andkeypath:parent_path_key];
            }

            int count = 0;
            for(id key in dict_value)
            {
                count++;
                if(count % 20 == 0)
                    QCoreApplication::processEvents();

                QString tmp_key_path = parent_path_key;
                [self sub_xcode_extract_plistdata_nsitems:key anddict:dict_value andarr:nil andbool:false andquery:insert_query];
                parent_path_key = tmp_key_path;
            }

            if(parent_path_key.endsWith("/"))
                parent_path_key.chop(1);

            parent_path_key.remove(parent_path_key.lastIndexOf("/") + 1 , parent_path_key.size());
        }
    }

    [pool drain];
    recon_static_functions::app_debug("Ends " , Q_FUNC_INFO);
}

-(void)insert_xcodedata_in_db:(QSqlQuery) insert_query andvalue:(QString) value_str andkey:(QString) key_str andtype:(QString) type_str andkeypath:(QString) keypath_str
{
    recon_static_functions::app_debug("Starts ", Q_FUNC_INFO);

    keypath_str = keypath_str.trimmed();

    if(keypath_str.endsWith("/"))
        keypath_str.chop(1);

    int count = 0;
    unsigned int hCount = 0;
    for(QString::const_iterator iter_const(keypath_str.begin()); iter_const != keypath_str.end(); ++iter_const)
    {
        count++;
        if(count % 20 == 0)
            QCoreApplication::processEvents();
        if(*iter_const == '/')
            ++hCount;
    }

    QString parentpath = keypath_str;
    if(parentpath == "Root")///remove node from keypath and make it parent node path
    {
        parentpath = "";
    }
    else
    {
        parentpath = parentpath.remove(parentpath.lastIndexOf("/"),parentpath.size());
    }

    QString getvalue = "";
    if(type_str == QString("Dictionary") || type_str == QString("Array"))
    {
        getvalue = QString("(" + value_str + " items)");
    }
    else
    {
        getvalue = value_str;
    }

    insert_query.clear();
    insert_query.prepare("insert into '" + plist_view_dbtable_name +"'(value,key,type, keypath,parentpath, parent,bookmark,id , recon_tag_value,notes) values(?,?,?,?,?,?,?,?,?,?)");
    insert_query.addBindValue(getvalue);
    insert_query.addBindValue(key_str);
    insert_query.addBindValue(type_str);
    insert_query.addBindValue(keypath_str);//complete node path
    insert_query.addBindValue(parentpath);//complete parent path
    insert_query.addBindValue(type_str == QString("Dictionary") || type_str == QString("Array") ? QString::number(0) : QString::number(1));//0 - means parent //1 means child
    insert_query.addBindValue("0");
    insert_query.addBindValue(QString::number(hCount));
    insert_query.addBindValue("");
    insert_query.addBindValue("");

    if(!insert_query.exec())
    {
        recon_static_functions::app_debug("insert_query execution ---FAILED---and keypath is " + keypath_str + "  Type is " + type_str, Q_FUNC_INFO);
        recon_static_functions::app_debug(insert_query.lastError().text() , Q_FUNC_INFO);
        recon_static_functions::app_debug(insert_query.executedQuery(), Q_FUNC_INFO);
        return;
    }

    recon_static_functions::app_debug("Ends ", Q_FUNC_INFO);
}

-(void)xcode_extract_plistdata:(QString) source_file anddbname:(QString) plist_view_db_table_name andquery:(QSqlQuery) insert_query
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    plist_view_dbtable_name = plist_view_db_table_name;
    self = [super init];

    NSObject* my_object;
    NSString *target_file = source_file.toNSString();
    my_object = source_file.toNSString();
    if(target_file == nil || ![my_object isKindOfClass:[NSString class]])
    {
        recon_static_functions::app_debug("target_file file open --------FAILED------" , Q_FUNC_INFO);
        recon_static_functions::app_debug("end with failure" , Q_FUNC_INFO);
        [pool drain];
        return;
    }

    NSDictionary *m_plist_dict = [NSDictionary dictionaryWithContentsOfFile:target_file];
    my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];
    if(m_plist_dict != nil || [my_object isKindOfClass:[NSDictionary class]])
    {
        if([m_plist_dict count] == 0)
        {
            QString value_name = QString::number(0);
            [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:"Root" andtype:"Dictionary" andkeypath:"Root/"];
        }
        else
        {
            int itemscount = [m_plist_dict count];
            QString value_name = QString::number(itemscount);
            [self insert_xcodedata_in_db:insert_query andvalue:value_name andkey:"Root" andtype:"Dictionary" andkeypath:"Root/"];
        }

        int count = 0;
        for(id key in m_plist_dict)
        {
            count++;
            if(count % 20 == 0)
                QCoreApplication::processEvents();
            parent_path_key.clear();
            parent_path_key.append("Root/");

            [self sub_xcode_extract_plistdata_nsitems:key anddict:m_plist_dict andarr:nil andbool:false andquery:insert_query];
        }
    }

    NSArray *m_plist_array = [NSArray arrayWithContentsOfFile:target_file];
    my_object = [NSArray arrayWithContentsOfFile:target_file];
    if(m_plist_array != nil || [my_object isKindOfClass:[NSArray class]])
    {
        parent_path_key.clear();
        parent_path_key.append("Root/");
        [self sub_xcode_extract_plistdata_nsitems:nil anddict:nil andarr:m_plist_array andbool:true andquery:insert_query];
    }

    [pool drain];
    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
}

@end

int plist_viewer::xcode_extract_plist(QString source_file)
{
    recon_static_functions::app_debug("Start " , Q_FUNC_INFO);

    QSqlDatabase destination_db;
    QString connection_naam = QString(Q_FUNC_INFO);
    QSqlDatabase::removeDatabase(connection_naam);
    destination_db = QSqlDatabase::addDatabase("QSQLITE", connection_naam);
    destination_db.setDatabaseName(destination_plist_result_db_path);
    if(!destination_db.open())
    {
        recon_static_functions::app_debug(" db open ----FAILED-----" , Q_FUNC_INFO);
        recon_static_functions::app_debug(destination_db.lastError().text(), Q_FUNC_INFO);
        return 1;
    }

    destination_db.transaction();
    QSqlQuery insert_query(destination_db);
    xcode_plist_viewer_class *xcode_plist_viewer_obj = [[xcode_plist_viewer_class alloc] init];
    [xcode_plist_viewer_obj xcode_extract_plistdata:source_file anddbname:plist_view_db_table_name andquery:insert_query];

    destination_db.commit();

    destination_db.close();
    [xcode_plist_viewer_obj release];
    recon_static_functions::app_debug("End " , Q_FUNC_INFO);
    return 1;
}

