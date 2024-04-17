
#import <Foundation/Foundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>
#include <QtCore>


QStringList usb_parser_module()
{


    QStringList usb_ser;

    CFMutableDictionaryRef matchingDict;
    io_iterator_t iter;
    kern_return_t kr;
    io_service_t device;
    io_name_t deviceName;
    CFStringRef deviceNameAsCFString;
    //kern_return_t kr;
    
    /* set up a matching dictionary for the class */
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (matchingDict == NULL)
    {
        return usb_ser; // fail
    }
    
    /* Now we have a dictionary, get an iterator.*/
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
    if (kr != KERN_SUCCESS)
    {
        return usb_ser; // fail
    }
    
    /* iterate */
    while ((device = IOIteratorNext(iter)))
    {
        
        CFTypeRef  serial = IORegistryEntryCreateCFProperty(device, CFSTR("USB Serial Number"), 0, 0);
        CFTypeRef  vendorid = IORegistryEntryCreateCFProperty(device, CFSTR("idVendor"), 0, 0);
        CFTypeRef  productid = IORegistryEntryCreateCFProperty(device, CFSTR("idProduct"), 0, 0);
        CFTypeRef  bcddevice = IORegistryEntryCreateCFProperty(device, CFSTR("bcdDevice"), 0, 0);
        
        kr = IORegistryEntryGetName(device, deviceName);
        if (KERN_SUCCESS != kr) {
            deviceName[0] = '\0';
        }

        if(serial == NULL)
        {
            continue;
        }
        


        if (CFGetTypeID(serial) == CFStringGetTypeID()) {
            CFStringRef sr2 = CFCopyDescription (serial);
            const char *psz = CFStringGetCStringPtr(sr2, kCFStringEncodingMacRoman);
            QString mstr = psz;
            mstr.remove("{"); mstr.remove("}");
            QStringList mlist = mstr.split("=");

            if(mlist.size() > 1)
                mstr = mlist.at(1);


            usb_ser << mstr.remove("\"").trimmed();



        } else {
            // panic!
        }


        IOObjectRelease(device);
    }
    
    return usb_ser;
    
}
