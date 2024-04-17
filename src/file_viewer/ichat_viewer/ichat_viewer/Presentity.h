
#import <Foundation/Foundation.h>

@interface Presentity : NSObject <NSCoding /* Decoding only */>

@property (readonly, copy) NSString *accountName;
@property (readonly, copy) NSString *serviceName;

@end
