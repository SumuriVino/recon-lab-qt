#include "online_maps_dialog.h"

#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>
#include <Coreservices/CoreServices.h>
#include <Foundation/NSAutoreleasePool.h>

void online_maps_dialog::pub_show_apple_maps(QString link_str)
{
    NSString *urlString = link_str.toNSString();
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:urlString]];
}
