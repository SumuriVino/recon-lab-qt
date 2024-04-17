#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#include "recon_helper_standard.h"


QStringList recon_helper_standard::extract_macos_current_login_apple_ids(QString path, QStringList users_list)
{
    QStringList config_ids;

    QString cmd1 = "select ZUSERNAME from ZACCOUNT where ZACCOUNTDESCRIPTION = ?";

    for(int ul = 0; ul < users_list.size(); ul++)
    {
        QString user_name = users_list.at(ul);
        QString dir_path = path + QString("Users/") + user_name + QString("/Library/Accounts");

        QStringList file_list = QDir(dir_path).entryList();

        for(int fl = 0 ; fl < file_list.size() ; fl++)
        {
            QString acc_file_sqlite = file_list.at(fl).trimmed();
            if(acc_file_sqlite.startsWith("Accounts") && acc_file_sqlite.endsWith(".sqlite"))
            {
                QString account_db_path = path + QString("Users/") + user_name + QString("/Library/Accounts/") + acc_file_sqlite;

                if(!QFileInfo(account_db_path).exists())
                    continue;

                QString adhoc_path = get_adhoc_path_for_database_using_QFile(account_db_path);

                config_ids << get_string_from_db_with_addbindvalues_by_dbpath(cmd1,QStringList("iCloud"),0, adhoc_path, Q_FUNC_INFO);
            }
        }
    }

    config_ids.removeAll("");
    config_ids.removeDuplicates();
    //=================================//


    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    for(int zz = 0; zz < users_list.size(); zz++)
    {
        QString plist_path = path + QString("Users/") + users_list.at(zz) + QString("/Library/Preferences/MobileMeAccounts.plist");
        if(!QFileInfo(plist_path).exists())
        {
            continue;
        }

        NSString *target_file = plist_path.toNSString();
        NSObject* my_object =  plist_path.toNSString();
        if(target_file == nil || ![my_object isKindOfClass:[NSString class]])
        {
            recon_static_functions::app_debug("target_file open ---------FAILED----------" + plist_path, Q_FUNC_INFO);
            continue;
        }


        NSDictionary *m_plist = [NSDictionary dictionaryWithContentsOfFile:target_file];
        my_object = [NSDictionary dictionaryWithContentsOfFile:target_file];
        if(m_plist == nil)
        {
            recon_static_functions::app_debug("m_plist Plist NIL ---------FAILED----------" + plist_path, Q_FUNC_INFO);
            continue;
        }

        if(![my_object isKindOfClass:[NSDictionary class]])
        {
            recon_static_functions::app_debug("m_plist Root not dictionary ---------FAILED----------" + plist_path, Q_FUNC_INFO);
            continue;
        }


        NSString* accounts = @"Accounts";
        NSArray* accounts_array = [m_plist objectForKey:accounts];
        my_object = [m_plist objectForKey:accounts];
        if(accounts_array == nil || ![my_object isKindOfClass:[NSArray class]])
        {
            recon_static_functions::app_debug("accounts_array  ---------FAILED----------" + plist_path,Q_FUNC_INFO);
            continue;
        }


        for (NSUInteger i = 0; i < [accounts_array count]; i++)
        {

            NSDictionary* account_item_dict = [accounts_array objectAtIndex:i];
            my_object = [accounts_array objectAtIndex:i];
            if(account_item_dict == nil || ![my_object isKindOfClass:[NSDictionary class]])
            {
                recon_static_functions::app_debug("account_item_dict is nil,continue from here" , Q_FUNC_INFO);
                continue;
            }

            NSString* ns_isloggedin = @"LoggedIn";
            my_object = [account_item_dict objectForKey:ns_isloggedin];

            QString is_logged_in_str;
            if([my_object isKindOfClass:[NSNumber class]])
            {
                NSNumber *isloggedin_val = [account_item_dict objectForKey:ns_isloggedin] ;
                if(isloggedin_val != nil)
                {
                    int tempint = [isloggedin_val intValue];
                    is_logged_in_str = (tempint == 1) ? QString("YES") : QString("NO") ;
                }
            }

            if(is_logged_in_str != "YES")
                continue;


            NSString* ns_AccountID = @"AccountID";
            NSString* ns_AccountID_val = [account_item_dict objectForKey:ns_AccountID];
            my_object = [account_item_dict objectForKey:ns_AccountID];

            QString accountid_str;
            if(ns_AccountID_val != nil && [my_object isKindOfClass:[NSString class]])
            {
                accountid_str = QString::fromNSString(ns_AccountID_val);
                accountid_str = accountid_str.trimmed();

                config_ids << accountid_str;
                break;
            }


        }

    }

    [pool drain];

    config_ids.removeAll("");
    config_ids.removeDuplicates();

    return config_ids;
}


