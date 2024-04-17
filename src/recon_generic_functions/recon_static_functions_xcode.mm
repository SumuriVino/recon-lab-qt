#include <QtCore>
#include "recon_static_functions.h"

#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#import <Appkit/AppKit.h>
#import <Cocoa/Cocoa.h>

QString recon_static_functions::convert_cfstring_to_string_xcode_mm(CFStringRef str, QString caller_func)
{
    NSString *nsstr = (NSString *)str;
    QString jstr = QString::fromNSString(nsstr);
    return jstr;
}

QString recon_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(void *ptr_nsstring, void *ptr_nsdictionary, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSString *nsitem;
    nsitem = static_cast<NSString *>(ptr_nsstring);

    NSDictionary *m_plist_dict;
    m_plist_dict = static_cast<NSDictionary *>(ptr_nsdictionary);

    NSObject* my_object;
    my_object = [m_plist_dict objectForKey:nsitem];

    if(![my_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        return QString("");
    }

    NSString *item_name = [m_plist_dict objectForKey:nsitem];
    QString str_nsstringitem = "";

    if(item_name != nil && [my_object isKindOfClass:[NSString class]])
    {
        str_nsstringitem = QString::fromNSString(item_name);
        str_nsstringitem = str_nsstringitem.trimmed();
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    [pool drain];

    return str_nsstringitem;
}

QString recon_static_functions::extract_os_version_full_xcode_mm_macOS(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QString version = extract_os_version_extended_xcode_mm_macOS(source_path, Q_FUNC_INFO);

    version = version.trimmed();

    if(version.size() > 5)
        version.chop(2);


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return version;
}

QString recon_static_functions::extract_os_version_extended_xcode_mm_macOS(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QString version;

    QString source_file1 = source_path + "System/Library/CoreServices/SystemVersion.plist";
    QString source_file2 = source_path + "Library/Preferences/com.apple.SoftwareUpdate.plist";
    if(QFileInfo(source_file1).exists())
    {
        // ProductVersion = 10.13.6
        version = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(source_file1,enum_Global_Plist_File_Key_Type_String, "ProductVersion", Q_FUNC_INFO).trimmed();

    }
    if(version.isEmpty())
    {
        if(QFileInfo(source_file2).exists())
        {
            // LastAttemptSystemVersion = 10.15.6 (19A536g)
            version = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(source_file2,enum_Global_Plist_File_Key_Type_String, "LastAttemptSystemVersion", Q_FUNC_INFO).trimmed();
            QString str1 = " (";
            if(version.contains(str1))
            {
                version.remove(version.indexOf(str1), version.size());
            }

        }
    }



    version = version.trimmed();


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return version;
}

QString recon_static_functions::extract_time_zone_macOS(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " - start",Q_FUNC_INFO);

    //input  : / OR /Volumes/Catalina
    //output : Asia/Kolkata

    QString timezonename = extract_time_zone_macOS_by_file(source_path, Q_FUNC_INFO);

    if(timezonename.isEmpty())
    {
        timezonename = extract_time_zone_macOS_by_command(source_path,Q_FUNC_INFO);
    }

    if(timezonename.isEmpty() && source_path.trimmed() == "/")
    {
        timezonename = extract_time_zone_macOS_by_api_current_machine(Q_FUNC_INFO);
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end Time ZoneName " + timezonename,Q_FUNC_INFO);

    return timezonename;
}

QString recon_static_functions::extract_time_zone_macOS_by_file(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    //input  : / OR /Volumes/Catalina
    //output : Asia/Kolkata

    QString timezonename;
    source_path = source_path.trimmed();
    if(source_path.isEmpty())
    {
        return timezonename;
    }

    QString source_file = source_path + "Library/Preferences/.GlobalPreferences.plist";
    QFileInfo info(source_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug("file not found" + source_file, Q_FUNC_INFO);
        return timezonename;
    }

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSObject* my_object;

    NSString *target_file = [NSString stringWithUTF8String: source_file.toUtf8().data()];
    my_object = [NSString stringWithUTF8String: source_file.toUtf8().data()];
    if(target_file != nil && [my_object isKindOfClass:[NSString class]])
    {
        NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
        my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];
        if(m_plist != nil &&  [my_object isKindOfClass:[NSDictionary class]])
        {
            NSString *timezone_select_city = @"com.apple.preferences.timezone.selected_city";
            NSDictionary *timezone_select_dict = [m_plist objectForKey:timezone_select_city];
            my_object =  [m_plist objectForKey:timezone_select_city];
            if(timezone_select_dict != nil &&  [my_object isKindOfClass:[NSDictionary class]])
            {
                NSString *timezone_city_ns = @"TimeZoneName";

                NSString *item_name = [timezone_select_dict objectForKey:timezone_city_ns];

                if(item_name != nil && [my_object isKindOfClass:[NSString class]])
                {
                    timezonename = [item_name UTF8String];

                }
            }

        }

        //=========================

        if(timezonename.trimmed().isEmpty() || timezonename == "(null)")
        {
            NSString *info_name = @"com.apple.TimeZonePref.Last_Selected_City";
            NSArray *info_value = [m_plist objectForKey:info_name];
            my_object = [m_plist objectForKey:info_name];
            if(info_value != nil && [my_object isKindOfClass:[NSArray class]])
            {
                for(NSUInteger k = 0 ; k < [info_value count] ; k++)
                {
                    my_object = [info_value objectAtIndex:k];
                    if(![my_object isKindOfClass:[NSString class]])
                        continue;

                    NSString *item = [info_value objectAtIndex:k];
                    QString item_str = [item UTF8String];

                    if(k == 3)
                    {
                        timezonename = item_str;
                        continue;
                    }
                }
            }
        }

        //=========================

    }
    [pool drain];

    if(!timezonename.trimmed().isEmpty() && timezonename != "(null)")
        return timezonename;


    recon_static_functions::app_debug("TimeZoneName not found by objective c, now trying by line parsing " + source_file , Q_FUNC_INFO);

    // if TimeZoneName not found by objective c
    QFile file(source_file);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        recon_static_functions::app_debug("File Opend ---FAILED--- " + source_file, Q_FUNC_INFO);
        recon_static_functions::app_debug("error " + file.errorString() + source_file, Q_FUNC_INFO);
        return timezonename;
    }

    while(!file.atEnd())
    {
        QString line = file.readLine();

        if(line.contains("<key>TimeZoneName</key>"))
        {
            QString str = file.readLine();
            str = str.trimmed();
            str.remove("<string>").remove("</string>");
            if(!str.trimmed().isEmpty())
                timezonename = str.trimmed();
        }
    }

    //=========================

    if(timezonename.trimmed().isEmpty() || timezonename == "(null)")
    {
        file.seek(0);
        int count = 0;

        while(!file.atEnd())
        {
            QString line = file.readLine();
            line = line.trimmed();

            if(line.contains("<key>com.apple.TimeZonePref.Last_Selected_City</key>"))
            {
                line =file.readLine(); // <array>
                while(!file.atEnd() && !line.startsWith("</array>"))
                {
                    line = file.readLine().trimmed();
                    line.remove("<string>").remove("</string>");
                    line = line.trimmed();

                    if(count == 3)
                    {
                        timezonename = line;
                    }
                    count++;
                }

                break;
            }
        }
    } // if macOS Sierra



    //=========================

    file.close();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " End " , Q_FUNC_INFO);

    return timezonename;
}


QString recon_static_functions::extract_time_zone_macOS_by_command(QString path_str, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start " , Q_FUNC_INFO);

    //input  : / OR /Volumes/Catalina
    //output : Asia/Kolkata

    QString timezone_str;

    path_str = path_str.trimmed();
    if(path_str.isEmpty())
        return timezone_str;

    if(!path_str.endsWith("/"))
        path_str.append("/");

    QString timezone_file = path_str + QString("etc/localtime");

    //==========Get Timezone Name=Start==========//
    //command : ls -la ~received_path/etc/localtime
    //output  : /etc/localtime -> /var/db/timezone/zoneinfo/Asia/Kolkata
    //Timezone Name : Asia/Kolkata

    QString args_str = QString("ls -la ") + timezone_file;

    recon_helper_process recon_helper_process_obj;

    QString tz_name_out_str = recon_helper_process_obj.run_command_by_shell(args_str , Q_FUNC_INFO);

    QString tz_name_str;
    QString zoneinfo_str = QString("zoneinfo/");
    if(tz_name_out_str.contains(zoneinfo_str))
    {
        QStringList tmp_list = tz_name_out_str.split(zoneinfo_str);
        if(tmp_list.size() > 1)
            tz_name_str = tmp_list.at(1);
    }

    timezone_str = tz_name_str;

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " End " , Q_FUNC_INFO);
    return timezone_str;
}

QString recon_static_functions::extract_time_zone_xcode_mm_iOS(QString path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " -START" + path,Q_FUNC_INFO);

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject *my_object;

    NSString *target_file = path.toNSString();
    my_object = path.toNSString();

    if(target_file == nil || ![my_object isKindOfClass:[NSString class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " source file open ------FAILED------",Q_FUNC_INFO);
        [pool drain];
        return "";
    }


    NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
    my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];
    if(m_plist == nil || ![my_object isKindOfClass:[NSDictionary class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + "  mplist nil ------FAILED------",Q_FUNC_INFO);
        [pool drain];
        return "";
    }

    NSString *timezopne_pattern = @"timezone";
    my_object = [m_plist objectForKey:timezopne_pattern];
    NSString *timezone_nsstring = [m_plist objectForKey:timezopne_pattern];
    QString timezone_qstring;
    if(timezone_nsstring != nil && [my_object isKindOfClass:[NSString class]])
    {
        timezone_qstring = QString::fromNSString(timezone_nsstring);
        timezone_qstring = timezone_qstring.trimmed();

        QStringList temp_list = timezone_qstring.split(",");
        timezone_qstring = temp_list.at(0);
    }

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " -END",Q_FUNC_INFO);

    return timezone_qstring;
}

QString recon_static_functions::extract_model_number_xcode_mm_macOS(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " - Starts",Q_FUNC_INFO);

    QString modl_no;

    QString source_file1 = source_path + "Library/Preferences/SystemConfiguration/preferences.plist";
    if(QFileInfo(source_file1).exists())
    {
        // Model = Macmini8,1
        modl_no = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(source_file1,enum_Global_Plist_File_Key_Type_String, "Model", Q_FUNC_INFO).trimmed();
    }
    else
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- file does not found " + source_file1,Q_FUNC_INFO);
    }

    modl_no = modl_no.trimmed();

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return modl_no;
}

QString recon_static_functions::extract_time_zone_macOS_by_api_current_machine(QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    //output : Asia/Kolkata

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSObject* my_object;

    NSTimeZone* localTimeZone = [NSTimeZone localTimeZone];

    NSString* local_name = [localTimeZone name];
    my_object = [localTimeZone name];
    QString local_name_qstring;
    if(local_name != nil && [my_object isKindOfClass:[NSString class]])
    {
        local_name_qstring = QString::fromNSString(local_name);
    }

    [pool drain];

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return local_name_qstring.trimmed();
}

struct_global_macOS_global_preferences recon_static_functions::extract_global_preferences_xcode_mm_macOS(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " - Starts",Q_FUNC_INFO);

    struct_global_macOS_global_preferences m1;
    m1.city_name = "";
    m1.country   = "";
    m1.latitude  = "";
    m1.longitude = "";

    QString source_file = source_path +  "Library/Preferences/.GlobalPreferences.plist";
    QFileInfo info(source_file);
    if(!info.exists())
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + "  ---FAILED--- file not found" + source_file,Q_FUNC_INFO);
        return m1;
    }
    QString latitude,longitude,country_name,city_name;


    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSObject* my_object;

    NSString *target_file = source_file.toNSString();
    my_object = source_file.toNSString();

    if(target_file != nil && [my_object isKindOfClass:[NSString class]])
    {
        NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
        my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];
        if(m_plist != nil &&  [my_object isKindOfClass:[NSDictionary class]])
        {

            NSString *timezone_select_city = @"com.apple.preferences.timezone.selected_city";
            NSDictionary *timezone_select_dict = [m_plist objectForKey:timezone_select_city];
            my_object = [m_plist objectForKey:timezone_select_city];
            if(timezone_select_dict != nil && [my_object isKindOfClass:[NSDictionary class]])
            {

                NSString *Latitude_ns = @"Latitude";
                NSNumber *Latitude_ns_value = [timezone_select_dict objectForKey:Latitude_ns];
                NSString *latt_number_string = [Latitude_ns_value stringValue];
                my_object = [Latitude_ns_value stringValue];
                if(latt_number_string != nil && [my_object isKindOfClass:[NSString class]])
                {
                    latitude = QString::fromNSString(latt_number_string);
                    latitude = latitude.trimmed();
                }

                NSString *Longitude_ns = @"Longitude";
                NSNumber *Longitude_ns_value = [timezone_select_dict objectForKey:Longitude_ns];
                NSString *long_number_string = [Longitude_ns_value stringValue];
                my_object = [Longitude_ns_value stringValue];
                if(long_number_string != nil && [my_object isKindOfClass:[NSString class]])
                {
                    longitude = QString::fromNSString(long_number_string);
                    longitude = longitude.trimmed();
                }

                NSString *country_ns = @"CountryCode";
                country_name = get_nsstring_values_from_nsdictionary_xcode_mm(country_ns,timezone_select_dict,Q_FUNC_INFO);

                NSString *name_ns = @"Name";
                city_name = get_nsstring_values_from_nsdictionary_xcode_mm(name_ns,timezone_select_dict,Q_FUNC_INFO);

                if(city_name != "(null)" && !city_name.trimmed().isEmpty())
                    m1.city_name = city_name;

                if(country_name != "(null)" && !country_name.trimmed().isEmpty())
                    m1.country = country_name;

                if(latitude != "(null)" && !latitude.trimmed().isEmpty())
                    m1.latitude = latitude;

                if(longitude != "(null)"  && !longitude.trimmed().isEmpty())
                    m1.longitude = longitude;

            }


            //=======================================  for macOS Sierra ========================================//
            if((m1.city_name.trimmed().isEmpty()) || (m1.country.trimmed().isEmpty()) || (m1.latitude.trimmed().isEmpty()) || (m1.longitude.trimmed().isEmpty()))
            {
                latitude.clear();longitude.clear();country_name.clear();city_name.clear();

                NSString *info_name = @"com.apple.TimeZonePref.Last_Selected_City";
                NSArray *info_value = [m_plist objectForKey:info_name];
                my_object = [m_plist objectForKey:info_name];
                if(info_value != nil && [my_object isKindOfClass:[NSArray class]])
                {
                    for(NSUInteger k = 0 ; k < [info_value count] ; k++)
                    {
                        my_object = [info_value objectAtIndex:k];
                        if(![my_object isKindOfClass:[NSString class]])
                            continue;

                        NSString *item = [info_value objectAtIndex:k];
                        QString item_str = QString::fromNSString(item);

                        if(k == 0)
                        {
                            latitude = item_str;
                            continue;
                        }
                        if(k == 1)
                        {
                            longitude = item_str;
                            continue;
                        }
                        if(k == 5)
                        {
                            city_name = item_str;
                            continue;
                        }
                        if(k == 6)
                        {
                            country_name = item_str;
                            continue;
                        }
                    }

                    if(city_name != "(null)" && !city_name.trimmed().isEmpty())
                        m1.city_name = city_name;

                    if(country_name != "(null)" && !country_name.trimmed().isEmpty())
                        m1.country = country_name;

                    if(latitude != "(null)" && !latitude.trimmed().isEmpty())
                        m1.latitude = latitude;

                    if(longitude != "(null)"  && !longitude.trimmed().isEmpty())
                        m1.longitude = longitude;

                }
            }

            //=======================================================================



        }
    }
    [pool drain];


    if((!m1.city_name.trimmed().isEmpty()) && (!m1.country.trimmed().isEmpty()) && (!m1.latitude.trimmed().isEmpty()) && (!m1.longitude.trimmed().isEmpty()))
        return m1;



    recon_static_functions::app_debug(prepare_callerfun(caller_func) + " Global preferences not found by objective c, now trying by line parsing " + source_file,Q_FUNC_INFO);

    QString dest_path = source_file;
    dest_path = global_variable_private_var_tmp_Dir_Path_Case_QString;
    QString file_tmp = recon_static_functions::get_available_filename("pref.xml",dest_path, Q_FUNC_INFO);
    dest_path = dest_path + file_tmp;
    dest_path = dest_path.trimmed();


    QStringList args;
    args << "-convert" << "xml1" << source_file << "-o" << dest_path;
    QProcess prc;
    prc.start("plutil",args);

    recon_static_functions::debug_conditional(prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    while(1)
    {
        QCoreApplication::processEvents();
        if(prc.state() == QProcess::NotRunning)
        {
            break;
        }
    }
    recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);



    // if Global preferences not found by objective c
    QFile file(dest_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " File Opend ---FAILED--- " + source_file,Q_FUNC_INFO);
        recon_static_functions::app_debug("file error " + file.errorString(),Q_FUNC_INFO);
        return m1;
    }


    latitude.clear();longitude.clear();country_name.clear();city_name.clear();
    while(!file.atEnd())
    {
        QString line = file.readLine();
        if(line.contains("<key>Latitude</key>"))
        {
            QString str = file.readLine();
            str = str.trimmed();
            str.remove("<real>").remove("</real>");
            latitude = str.trimmed();
        }
        if(line.contains("<key>Longitude</key>"))
        {
            QString str = file.readLine();
            str = str.trimmed();
            str.remove("<real>").remove("</real>");
            longitude = str.trimmed();
        }
        if(line.contains("<key>Name</key>"))
        {
            QString str = file.readLine();
            str = str.trimmed();
            str.remove("<string>").remove("</string>");
            city_name = str.trimmed();
        }
        if(line.contains("<key>CountryCode</key>"))
        {
            QString str = file.readLine();
            str = str.trimmed();
            str.remove("<string>").remove("</string>");
            country_name = str.trimmed();
        }
    }

    if(!city_name.trimmed().isEmpty())
        m1.city_name = city_name;
    if(!country_name.trimmed().isEmpty())
        m1.country = country_name;
    if(!longitude.trimmed().isEmpty())
        m1.longitude = longitude;
    if(!latitude.trimmed().isEmpty())
        m1.latitude = latitude;


    if((!m1.city_name.trimmed().isEmpty()) && (!m1.country.trimmed().isEmpty()) && (!m1.latitude.trimmed().isEmpty()) && (!m1.longitude.trimmed().isEmpty()))
        return m1;

    // ================== For macOS Sierra ===============
    if((m1.city_name.trimmed().isEmpty()) || (m1.country.trimmed().isEmpty()) || (m1.latitude.trimmed().isEmpty()) || (m1.longitude.trimmed().isEmpty()))
    {
        latitude.clear();longitude.clear();country_name.clear();city_name.clear();

        file.seek(0);
        int count = 0;

        while(!file.atEnd())
        {
            QString line = file.readLine();
            line = line.trimmed();

            if(line.contains("<key>com.apple.TimeZonePref.Last_Selected_City</key>"))
            {
                line =file.readLine(); // <array>
                while(!file.atEnd() && !line.startsWith("</array>"))
                {
                    line = file.readLine().trimmed();
                    line.remove("<string>").remove("</string>");
                    line = line.trimmed();

                    if(count == 0)
                    {
                        latitude = line;
                    }
                    if(count == 1)
                    {
                        longitude = line;
                    }
                    if(count == 5)
                    {
                        city_name = line;
                    }
                    if(count == 6)
                    {
                        country_name = line;
                    }

                    count++;
                }

                break;
            }
        }
    } // if macOS Sierra


    if(!city_name.trimmed().isEmpty())
        m1.city_name = city_name;
    if(!country_name.trimmed().isEmpty())
        m1.country = country_name;
    if(!longitude.trimmed().isEmpty())
        m1.longitude = longitude;
    if(!latitude.trimmed().isEmpty())
        m1.latitude = latitude;

    file.close();


    recon_static_functions::remove_safely_blank_dir_OR_file(dest_path,Q_FUNC_INFO);
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end ",Q_FUNC_INFO);

    return m1;
}

struct_global_ios_uid_backup_detail recon_static_functions::extract_backup_details_iOS_uid(QString path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start ",Q_FUNC_INFO);


    struct_global_ios_uid_backup_detail mobj;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject *my_object;

    NSString *target_file = path.toNSString();
    my_object = path.toNSString();

    if(target_file == nil || ![my_object isKindOfClass:[NSString class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " source file open ------FAILED------",Q_FUNC_INFO);
        [pool drain];
        return mobj;
    }


    NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
    my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];
    if(m_plist == nil || ![my_object isKindOfClass:[NSDictionary class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " mplist nil ------FAILED------",Q_FUNC_INFO);
        [pool drain];
        return mobj;
    }


    NSString *build_version = @"Build Version";
    mobj.build_version = get_nsstring_values_from_nsdictionary_xcode_mm(build_version,m_plist,Q_FUNC_INFO);

    NSString *display_name = @"Display Name";
    mobj.display_name = get_nsstring_values_from_nsdictionary_xcode_mm(display_name,m_plist,Q_FUNC_INFO);

    NSString *guid = @"GUID";
    mobj.guid = get_nsstring_values_from_nsdictionary_xcode_mm(guid,m_plist,Q_FUNC_INFO);

    NSString *product_name = @"Product Name";
    mobj.product_name = get_nsstring_values_from_nsdictionary_xcode_mm(product_name,m_plist,Q_FUNC_INFO);

    NSString *target_type = @"Target Type";
    mobj.target_type = get_nsstring_values_from_nsdictionary_xcode_mm(target_type,m_plist,Q_FUNC_INFO);

    NSString *iccid = @"ICCID";
    mobj.sim_no_iccid_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(iccid,m_plist,Q_FUNC_INFO);

    NSString *iccid_2 = @"ICCID 2";
    mobj.sim_no_iccid_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(iccid_2,m_plist,Q_FUNC_INFO);

    mobj.sim_no_iccid_QStringList.removeDuplicates();
    mobj.sim_no_iccid_QStringList.removeAll("");

    NSString *device_name = @"Device Name";
    mobj.device_name = get_nsstring_values_from_nsdictionary_xcode_mm(device_name,m_plist,Q_FUNC_INFO);

    NSString *imei = @"IMEI";
    mobj.imei_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(imei,m_plist,Q_FUNC_INFO);

    NSString *imei_2 = @"IMEI 2";
    mobj.imei_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(imei_2,m_plist,Q_FUNC_INFO);

    mobj.imei_QStringList.removeDuplicates();
    mobj.imei_QStringList.removeAll("");

    NSString *last_backup_date = @"Last Backup Date";
    NSDate *last_date = [m_plist objectForKey:last_backup_date];
    my_object = [m_plist objectForKey:last_backup_date];

    if(last_date != nil && [my_object isKindOfClass:[NSDate class]])
    {
        NSTimeInterval ti = [last_date timeIntervalSince1970];

        //  if(ti != nil)
        {
            NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
            QString date_interval = QString::fromNSString(intervalString);
            QStringList temp = date_interval.split(".");
            QString last_backup_date_qstring = temp.at(0);

            mobj.last_backup_date_numeric_qint64 = last_backup_date_qstring.trimmed().toLongLong();

        }
    }

    NSString *meid = @"MEID";
    mobj.meid = get_nsstring_values_from_nsdictionary_xcode_mm(meid,m_plist,Q_FUNC_INFO);


    NSString *phone_number = @"Phone Number";
    mobj.phone_no_msisdn_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(phone_number,m_plist, Q_FUNC_INFO);

    NSString *phone_number_2 = @"Phone Number 2";
    mobj.phone_no_msisdn_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(phone_number_2,m_plist, Q_FUNC_INFO);

    mobj.phone_no_msisdn_QStringList.removeDuplicates();
    mobj.phone_no_msisdn_QStringList.removeAll("");


    NSString *product_type = @"Product Type";
    mobj.product_type = get_nsstring_values_from_nsdictionary_xcode_mm(product_type,m_plist,Q_FUNC_INFO);

    NSString *product_version = @"Product Version";
    mobj.product_version = get_nsstring_values_from_nsdictionary_xcode_mm(product_version,m_plist,Q_FUNC_INFO);


    NSString *serial_number = @"Serial Number";
    mobj.serial_number = get_nsstring_values_from_nsdictionary_xcode_mm(serial_number,m_plist,Q_FUNC_INFO);

    NSString *target_identifier = @"Target Identifier";
    mobj.target_identifier = get_nsstring_values_from_nsdictionary_xcode_mm(target_identifier,m_plist,Q_FUNC_INFO);

    NSString *unique_identifier = @"Unique Identifier";
    mobj.unique_identifier = get_nsstring_values_from_nsdictionary_xcode_mm(unique_identifier,m_plist,Q_FUNC_INFO);


    NSString *itune_version = @"iTunes Version";
    mobj.itune_version = get_nsstring_values_from_nsdictionary_xcode_mm(itune_version,m_plist,Q_FUNC_INFO);


    QFileInfo info1(path);
    mobj.backup_size_byte_numeric_QString = mining_size(info1.absolutePath(),Q_FUNC_INFO);



    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end",Q_FUNC_INFO);

    [pool drain];

    return mobj;
}

QString recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(QString plist_file_path, int m_datatype, QString key_name, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start ",Q_FUNC_INFO);

    QString value;

    if(plist_file_path.isEmpty())
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- empty file path " + plist_file_path,Q_FUNC_INFO);
        return value;
    }

    if(key_name.isEmpty())
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- BLANK key, file path -> " + plist_file_path,Q_FUNC_INFO);
        return value;
    }


    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSObject* my_object;
    NSString *target_file = plist_file_path.toNSString();
    my_object = plist_file_path.toNSString();
    if(target_file == nil || ![my_object isKindOfClass:[NSString class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- stage 1, file path -> " + plist_file_path,Q_FUNC_INFO);
        [pool drain];
        return value;
    }

    NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
    my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];

    if(m_plist == nil || ![my_object isKindOfClass:[NSDictionary class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- stage 2, file path -> " + plist_file_path,Q_FUNC_INFO);
        [pool drain];
        return value;
    }

    if(m_datatype == enum_Global_Plist_File_Key_Type_Boolean)
    {
        NSString *boolean_key = key_name.toNSString();
        NSNumber* boolean_value = [m_plist objectForKey:boolean_key];
        my_object = [m_plist objectForKey:boolean_key];

        qint64 value_number = 0;
        if(boolean_value != nil && [my_object isKindOfClass:[NSNumber class]])
        {
            value_number = [boolean_value doubleValue];
        }

        value = QString::number(value_number);
    }
    else if(m_datatype == enum_Global_Plist_File_Key_Type_Data)
    {

    }
    else if(m_datatype == enum_Global_Plist_File_Key_Type_Date)
    {

    }
    else if(m_datatype == enum_Global_Plist_File_Key_Type_String)
    {
        NSString *string_key =  key_name.toNSString();
        NSString* string_value = [m_plist objectForKey:string_key];
        my_object = [m_plist objectForKey:string_key];

        if(string_value != nil && [my_object isKindOfClass:[NSString class]])
        {
            value = QString::fromNSString(string_value);
        }
    }
    else if(m_datatype == enum_Global_Plist_File_Key_Type_Number)
    {
        NSString *number_key = key_name.toNSString();
        NSNumber* number_value = [m_plist objectForKey:number_key];
        my_object = [m_plist objectForKey:number_key];

        qint64 value_number = 0;
        if(number_value != nil && [my_object isKindOfClass:[NSNumber class]])
        {
            value_number = [number_value doubleValue];
        }

        value = QString::number(value_number);
    }


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " End ",Q_FUNC_INFO);

    [pool drain];
    return value;
}

QString recon_static_functions::get_value_of_key_from_diskutil_apfs_list_plist_volume(QString plist_file_path, QString volumn_identifier, QString key_name, int key_datatype, QString caller_func)
{
    //This function returns only Volumes related info

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " Start ",Q_FUNC_INFO);

    QString value;

    if(plist_file_path.isEmpty())
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- empty file path " + plist_file_path,Q_FUNC_INFO);
        return value;
    }

    if(volumn_identifier.isEmpty())
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- Volumn Identyfier, file path -> " + plist_file_path,Q_FUNC_INFO);
        return value;
    }

    if(key_name.isEmpty())
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- BLANK key, file path -> " + plist_file_path,Q_FUNC_INFO);
        return value;
    }


    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSObject* my_object;
    NSString *target_file = plist_file_path.toNSString();
    my_object = plist_file_path.toNSString();
    if(target_file == nil || ![my_object isKindOfClass:[NSString class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- stage 3, file path -> " + plist_file_path,Q_FUNC_INFO);
        [pool drain];
        return value;
    }

    NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
    my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];

    if(m_plist == nil || ![my_object isKindOfClass:[NSDictionary class]])
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- stage 4, file path -> " + plist_file_path,Q_FUNC_INFO);
        [pool drain];
        return value;
    }


    NSString *all_disks_label = @"Containers";
    NSArray *all_disks_arr    = [m_plist objectForKey:all_disks_label];
    my_object = [m_plist objectForKey:all_disks_label];

    if(all_disks_arr == nil || ![my_object isKindOfClass:[NSArray class]])
    {
        return value;
    }


    for(NSDictionary *disk_dict in all_disks_arr)
    {
        // Volumes
        NSString *volumes_label = @"Volumes";
        NSArray  *all_Volumes_array  = [disk_dict objectForKey:volumes_label];

        for(NSDictionary *volume_dict in all_Volumes_array)
        {
            // DeviceIdentifier
            NSString *device_identifier_label = @"DeviceIdentifier";
            NSString *device_identifier_value = [volume_dict objectForKey:device_identifier_label];
            QString  apfs_volume_id_str = QString::fromNSString(device_identifier_value);


            if(volumn_identifier == apfs_volume_id_str && !apfs_volume_id_str.trimmed().isEmpty())
            {
                if(key_datatype == enum_Global_Plist_File_Key_Type_String)
                {
                    NSString *string_key_label = key_name.toNSString();
                    NSString *string_key_value = [volume_dict objectForKey:string_key_label];
                    if(string_key_value != nil && [my_object isKindOfClass:[NSString class]])
                    {
                        value = QString::fromNSString(string_key_value);
                    }
                }
                else if(key_datatype == enum_Global_Plist_File_Key_Type_Boolean)
                {
                    NSString *nsnumber_key_label = key_name.toNSString();
                    NSNumber *nsnumber_key_value = [volume_dict objectForKey:nsnumber_key_label];
                    if(nsnumber_key_value != nil && [my_object isKindOfClass:[NSString class]])
                    {
                        value = QString(MACRO_FALSE_VALUE_READABLE_QString);
                        if([nsnumber_key_value doubleValue] == 1)
                            value = QString(MACRO_TRUE_VALUE_READABLE_QString);
                    }
                }
                else if(key_datatype == enum_Global_Plist_File_Key_Type_Array)
                {
                    NSString *nsarray_key_label = key_name.toNSString();
                    NSArray  *nsarray_key_arr = [volume_dict objectForKey:nsarray_key_label];
                    if(nsarray_key_arr != nil && [my_object isKindOfClass:[NSArray class]])
                    {
                        for(NSString *roles_str in nsarray_key_arr)
                            value = QString::fromNSString(roles_str);
                    }
                }
            }
        }
    }

    [pool drain];

    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " End ",Q_FUNC_INFO);
    return value;
}

QString recon_static_functions::extract_build_version_xcode_mm_macOS(QString source_path, QString caller_func)
{
    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " start " , Q_FUNC_INFO);

    QString bld_version;

    QString source_file1 = source_path + "System/Library/CoreServices/SystemVersion.plist";
    QString source_file2 = source_path + "Library/Preferences/com.apple.SoftwareUpdate.plist";
    if(QFileInfo(source_file1).exists())
    {
        // ProductBuildVersion = 19A536g
        bld_version = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(source_file1,enum_Global_Plist_File_Key_Type_String, "ProductBuildVersion", Q_FUNC_INFO).trimmed();
    }
    else if(QFileInfo(source_file2).exists())
    {
        // LastAttemptSystemVersion = 10.15.6 (19A536g)

        QString str1 = recon_static_functions::get_value_of_key_from_diskutil_info_plist_file(source_file2,enum_Global_Plist_File_Key_Type_String, "LastAttemptSystemVersion", Q_FUNC_INFO).trimmed();

        QString str_chk = " (";
        if(str1.contains(str_chk))
        {
            str1.remove(0, str1.indexOf(str_chk));
            str1.remove("(").remove(")");
            bld_version = str1;
        }
    }
    else
    {
        recon_static_functions::app_debug(prepare_callerfun(caller_func) + " ---FAILED--- any of the file does not found ",Q_FUNC_INFO);
    }



    bld_version = bld_version.trimmed();


    recon_static_functions::debug_intensive(prepare_callerfun(caller_func) + " end " , Q_FUNC_INFO);

    return bld_version;
}


void recon_static_functions::open_file_with_specified_app(QString file_path, QString application_path , QString caller_fun)
{
    // NSWorkspace.shared.openFile("/Myfiles/README", withApplication: "TextEdit")

    if(file_path.trimmed().isEmpty() || application_path.trimmed().isEmpty())
        return;

    [[NSWorkspace sharedWorkspace] openFile:file_path.toNSString() withApplication:application_path.toNSString()];
}

void recon_static_functions::get_graykey_product_type_and_country_code_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString plist_path = ios_backup_path + "private/var/mobile/Library/Preferences/com.apple.assistant.plist";

    if(!QFileInfo(plist_path).exists())
    {
        recon_static_functions::app_debug("File does not exist ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* m_object;

    NSString* target_plist_file = plist_path.toNSString();
    m_object = plist_path.toNSString();
    if(target_plist_file == nil || ![m_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        recon_static_functions::app_debug("target_plist_file ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSDictionary* root_dict = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    m_object = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    if(root_dict == nil || ![m_object isKindOfClass:[NSDictionary class]])
    {
        [pool drain];
        recon_static_functions::app_debug("root_dict nill ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSString* manifest_v2 = @"PHS Asset Manifest V2";
    NSDictionary* manifest_v2_obj = [root_dict objectForKey:manifest_v2];
    m_object = [root_dict objectForKey:manifest_v2];
    if( manifest_v2_obj != nil && [m_object isKindOfClass:[NSDictionary class]])
    {
        for(id item in  manifest_v2_obj)
        {
            NSString* iphone_verstion_dict = item;
            NSDictionary *iphone_verstion_dict_obj = [ manifest_v2_obj objectForKey:iphone_verstion_dict];
            m_object = [ manifest_v2_obj objectForKey:iphone_verstion_dict];
            if(iphone_verstion_dict_obj != nil && [m_object isKindOfClass:[NSDictionary class]])
            {
                NSString* product_type = @"productType";
                mobj.product_type = get_nsstring_values_from_nsdictionary_xcode_mm(product_type,iphone_verstion_dict_obj,Q_FUNC_INFO);
                break;
            }
        }
    }

    NSString* cntry_cd = @"Country Code";
    mobj.country_code = get_nsstring_values_from_nsdictionary_xcode_mm(cntry_cd,root_dict,Q_FUNC_INFO);

    [pool drain];

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);
}

void recon_static_functions::get_graykey_imei_phone_and_iccid_no_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString plist_path = ios_backup_path + "private/var/wireless/Library/Preferences/com.apple.commcenter.device_specific_nobackup.plist";

    if(!QFileInfo(plist_path).exists())
    {
        recon_static_functions::app_debug("File does not exist ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* m_object;

    NSString *target_plist_file = plist_path.toNSString();
    m_object = plist_path.toNSString();

    if(target_plist_file == nil || ![m_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        recon_static_functions::app_debug("File Open ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return ;
    }

    NSDictionary *root_dict = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    m_object = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    if(root_dict == nil || ![m_object isKindOfClass:[NSDictionary class]])
    {
        [pool drain];
        recon_static_functions::app_debug(" root dict ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return ;
    }

    NSString* imeis = @"imeis";
    NSArray *imeis_obj = [root_dict objectForKey:imeis];
    m_object = [root_dict objectForKey:imeis];
    if(imeis_obj != nil && [m_object isKindOfClass:[NSArray class]])
    {
        for(id item in imeis_obj)
        {

            NSString* second = @"second";
            NSString* second_obj = [item objectForKey:second];
            m_object = [item objectForKey:second];
            if(second_obj != nil && [m_object isKindOfClass:[NSString class]])
            {
                QString imei_number = QString::fromNSString(second_obj);

                if(!imei_number.isEmpty())
                {
                    mobj.imei_number_QStringList << imei_number;
                }
            }
        }
    }

    NSString* reported_phone_number = @"ReportedPhoneNumber";
    mobj.phone_number_mssid_QStringList << get_nsstring_values_from_nsdictionary_xcode_mm(reported_phone_number,root_dict,Q_FUNC_INFO);

    NSString* subscriber_account_ids = @"subscriber_account_ids";
    NSArray *isubscriber_account_ids_obj = [root_dict objectForKey:subscriber_account_ids];
    m_object = [root_dict objectForKey:subscriber_account_ids];
    if(isubscriber_account_ids_obj != nil && [m_object isKindOfClass:[NSArray class]])
    {
        for(id item in isubscriber_account_ids_obj)
        {
            NSString* iccid_second = @"second";
            NSString* iccid_second_obj = [item objectForKey:iccid_second];
            m_object = [item objectForKey:iccid_second];
            if(iccid_second_obj != nil && [m_object isKindOfClass:[NSString class]])
            {
                QString iccid = QString::fromNSString(iccid_second_obj);
                if(!iccid.isEmpty())
                {
                    mobj.iccid_number_QStringList << iccid;
                }
            }
        }
    }

    mobj.iccid_number_QStringList.removeDuplicates();
    mobj.phone_number_mssid_QStringList.removeDuplicates();
    mobj.imei_number_QStringList.removeDuplicates();

    mobj.iccid_number_QStringList.removeAll("");
    mobj.phone_number_mssid_QStringList.removeAll("");
    mobj.imei_number_QStringList.removeAll("");

    [pool drain];

    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);
}

void recon_static_functions::get_graykey_device_name_and_timezone_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString plist_path = ios_backup_path + "private/var/root/Library/Lockdown/data_ark.plist";

    if(!QFileInfo(plist_path).exists())
    {
        recon_static_functions::app_debug("File does not exist ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* m_object;

    NSString *target_plist_file = plist_path.toNSString();
    m_object = plist_path.toNSString();
    if(target_plist_file == nil || ![m_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        recon_static_functions::app_debug("target_plist_file ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSDictionary *root_dict = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    m_object = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    if(root_dict == nil || ![m_object isKindOfClass:[NSDictionary class]])
    {
        [pool drain];
        recon_static_functions::app_debug("root dict ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSString* device_name = @"-DeviceName";
    mobj.device_name = get_nsstring_values_from_nsdictionary_xcode_mm(device_name,root_dict,Q_FUNC_INFO);
    mobj.display_name = mobj.device_name;

    NSString* time_zone = @"-TimeZone";
    mobj.time_zone = get_nsstring_values_from_nsdictionary_xcode_mm(time_zone,root_dict,Q_FUNC_INFO);

    [pool drain];
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);
}

void recon_static_functions::get_graykey_serial_no_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString plist_path = ios_backup_path + "private/var/db/SoC/lts.ecpu.plist";

    if(!QFileInfo(plist_path).exists())
    {
        recon_static_functions::app_debug("File does not exist ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* m_object;

    NSString *target_plist_file = plist_path.toNSString();
    m_object = plist_path.toNSString();
    if(target_plist_file == nil || ![m_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        recon_static_functions::app_debug("target_plist_file ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return ;
    }

    NSDictionary *root_dict = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    m_object = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    if(root_dict == nil || ![m_object isKindOfClass:[NSDictionary class]])
    {
        [pool drain];
        recon_static_functions::app_debug("root dict ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return ;
    }

    NSString* serial = @"Serial";
    mobj.serial_number = get_nsstring_values_from_nsdictionary_xcode_mm(serial,root_dict,Q_FUNC_INFO);

    [pool drain];
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);

}

void recon_static_functions::get_graykey_device_guid_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString plist_path = ios_backup_path + "private/var/mobile/Library/Preferences/com.apple.appstored.plist";

    if(!QFileInfo(plist_path).exists())
    {
        recon_static_functions::app_debug("File does not exist ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* m_object;

    NSString *target_plist_file = plist_path.toNSString();
    m_object = plist_path.toNSString();
    if(target_plist_file == nil || ![m_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        recon_static_functions::app_debug("target_plist_file ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return ;
    }

    NSDictionary *root_dict = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    m_object = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    if(root_dict == nil || ![m_object isKindOfClass:[NSDictionary class]])
    {
        [pool drain];
        recon_static_functions::app_debug("root dict ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return ;
    }

    NSString* arcade_device_guid = @"ArcadeDeviceGUID";
    mobj.device_guid = get_nsstring_values_from_nsdictionary_xcode_mm(arcade_device_guid,root_dict,Q_FUNC_INFO);

    [pool drain];
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);
}

void recon_static_functions::get_graykey_product_vrsn_build_model_plist(QString ios_backup_path, struct_global_ios_uniform_backup_detail &mobj, QString caller_func)
{
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " start  ",Q_FUNC_INFO);

    QString plist_path = ios_backup_path + "/private/var/installd/Library/MobileInstallation/LastBuildInfo.plist";
    if(!QFileInfo(plist_path).exists())
    {
        recon_static_functions::app_debug("File does not exist ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSObject* plist_object;

    NSString* target_plist_file = plist_path.toNSString();
    plist_object = plist_path.toNSString();
    if(target_plist_file == nil || ![plist_object isKindOfClass:[NSString class]])
    {
        [pool drain];
        recon_static_functions::app_debug("target_plist_file ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSDictionary *info_dictionary_version = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    plist_object = [NSDictionary dictionaryWithContentsOfFile:target_plist_file];
    if(info_dictionary_version == nil || ![plist_object isKindOfClass:[NSDictionary class]])
    {
        [pool drain];
        recon_static_functions::app_debug("info_dictionary_version ---FAILED--- " + plist_path, Q_FUNC_INFO);
        return;
    }

    NSString* product_version_str = @"ProductVersion";
    mobj.product_version = get_nsstring_values_from_nsdictionary_xcode_mm(product_version_str,info_dictionary_version,Q_FUNC_INFO);


    NSString* product_build_version_str = @"ProductBuildVersion";
    mobj.product_build_version = get_nsstring_values_from_nsdictionary_xcode_mm(product_build_version_str,info_dictionary_version,Q_FUNC_INFO);


    NSString* product_name_str = @"ProductName";
    mobj.product_name = get_nsstring_values_from_nsdictionary_xcode_mm(product_name_str,info_dictionary_version,Q_FUNC_INFO);


    NSString* product_model_str = @"FullVersionString";
    mobj.product_model = get_nsstring_values_from_nsdictionary_xcode_mm(product_model_str,info_dictionary_version,Q_FUNC_INFO);


    [pool drain];
    recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + " end  ",Q_FUNC_INFO);
}

void recon_static_functions:: clear_variables_ios_uniform_backup_structure(struct_global_ios_uniform_backup_detail &mobj)
{
    mobj.product_version.clear();
    mobj.product_build_version.clear();
    mobj.product_name.clear();
    mobj.product_model.clear();
    mobj.product_type.clear();
    mobj.country_code.clear();
    mobj.locale_language.clear();
    mobj.imei_number_QStringList.clear();
    mobj.phone_number_mssid_QStringList.clear();
    mobj.iccid_number_QStringList.clear();
    mobj.device_name.clear();
    mobj.display_name.clear();
    mobj.time_zone.clear();
    mobj.serial_number.clear();
    mobj.device_guid.clear();
}
