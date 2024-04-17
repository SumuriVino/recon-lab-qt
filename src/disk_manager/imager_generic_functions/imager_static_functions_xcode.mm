#include "imager_static_functions.h"

#import <Foundation/Foundation.h>
#import <Foundation/NSAutoreleasePool.h>

#import <Appkit/AppKit.h>
#import <Cocoa/Cocoa.h>

QString imager_static_functions::convert_cfstring_to_string_xcode_mm(CFStringRef str, QString caller_func)
{
    NSString *nsstr = (NSString *)str;
    QString jstr = QString::fromNSString(nsstr);
    return jstr;
}

QString imager_static_functions::get_nsstring_values_from_nsdictionary_xcode_mm(void *ptr_nsstring, void *ptr_nsdictionary, QString caller_func)
{

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


    [pool drain];

    return str_nsstringitem;
}

qint64 imager_static_functions::get_nsnumber_values_qint64(void *ptr_nsstring, void *ptr_nsdictionary, QString caller_func)
{
    qint64 nsnumberitem_value = 0;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSString *nsitem;
    nsitem = static_cast<NSString *>(ptr_nsstring);

    NSDictionary *m_plist;
    m_plist = static_cast<NSDictionary *>(ptr_nsdictionary);

    NSObject* my_object;

    my_object = [m_plist objectForKey:nsitem];

    if(![my_object isKindOfClass:[NSNumber class]])
    {
        [pool drain];
        return nsnumberitem_value;
    }

    NSNumber *item_name = [m_plist objectForKey:nsitem];

    if(item_name != nil && [my_object isKindOfClass:[NSNumber class]])
    {
        nsnumberitem_value = [item_name doubleValue];
    }


    [pool drain];

    return nsnumberitem_value;

}

