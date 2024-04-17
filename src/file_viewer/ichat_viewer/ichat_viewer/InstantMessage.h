

#import <Foundation/Foundation.h>
#import "Presentity.h"

@interface InstantMessage : NSObject <NSCoding /* Decoding only */>

@property (readonly, copy) NSDate *date;
@property (readonly, copy) NSString *message;
@property (readonly, copy) NSMutableArray *files;
@property () BOOL isMultiParty;
@property () BOOL isRead;
@property () NSSet *participantIds;
@property () NSString *chatId;
@property (readonly, copy) Presentity *sender;
@property (readonly, copy) Presentity *subject;
- (NSDictionary *) get_message_detail;
@end
