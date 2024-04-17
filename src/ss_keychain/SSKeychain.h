

#import <Foundation/Foundation.h>
#import <Security/Security.h>

/** Error codes that can be returned in NSError objects. */
typedef enum {
	SSKeychainErrorNone = noErr,
	
	SSKeychainErrorBadArguments = -1001,
	
	SSKeychainErrorNoPassword = -1002,
	
	SSKeychainErrorInvalidParameter = errSecParam,
	
	SSKeychainErrorFailedToAllocated = errSecAllocate,
	
	SSKeychainErrorNotAvailable = errSecNotAvailable,
	
	SSKeychainErrorAuthorizationFailed = errSecAuthFailed,
	
	SSKeychainErrorDuplicatedItem = errSecDuplicateItem,
	
	SSKeychainErrorNotFound = errSecItemNotFound,
	
	SSKeychainErrorInteractionNotAllowed = errSecInteractionNotAllowed,
	
	SSKeychainErrorFailedToDecode = errSecDecode
} SSKeychainErrorCode;

extern NSString *const kSSKeychainErrorDomain;

extern NSString *const kSSKeychainAccountKey;

extern NSString *const kSSKeychainCreatedAtKey;

extern NSString *const kSSKeychainClassKey;

extern NSString *const kSSKeychainDescriptionKey;

extern NSString *const kSSKeychainLabelKey;

 
extern NSString *const kSSKeychainLastModifiedKey;

extern NSString *const kSSKeychainWhereKey;

@interface Recon_Keychain : NSObject

///-----------------------
/// @name Getting Accounts
///-----------------------

+ (NSArray *)allAccounts;

+ (NSArray *)allAccounts:(NSError **)error;

+ (NSArray *)accountsForService:(NSString *)serviceName;

+ (NSArray *)accountsForService:(NSString *)serviceName error:(NSError **)error;


///------------------------
/// @name Getting Passwords
///------------------------

+ (NSString *)passwordForService:(NSString *)serviceName account:(NSString *)account;

+ (NSString *)passwordForService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

+ (NSData *)passwordDataForService:(NSString *)serviceName account:(NSString *)account;

+ (NSData *)passwordDataForService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;



+ (BOOL)deletePasswordForService:(NSString *)serviceName account:(NSString *)account;

+ (BOOL)deletePasswordForService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;


+ (BOOL)setPassword:(NSString *)password forService:(NSString *)serviceName account:(NSString *)account;

+ (BOOL)setPassword:(NSString *)password forService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;

+ (BOOL)setPasswordData:(NSData *)password forService:(NSString *)serviceName account:(NSString *)account;

+ (BOOL)setPasswordData:(NSData *)password forService:(NSString *)serviceName account:(NSString *)account error:(NSError **)error;


///--------------------
/// @name Configuration
///--------------------

#if __IPHONE_4_0 && TARGET_OS_IPHONE
/**
 Returns the accessibility type for all future passwords saved to the Keychain.
 
 @return Returns the accessibility type.
 
 The return value will be `NULL` or one of the "Keychain Item Accessibility Constants" used for determining when a
 keychain item should be readable.
 
 @see accessibilityType
 */
+ (CFTypeRef)accessibilityType;

/**
 Sets the accessibility type for all future passwords saved to the Keychain.
 
 @param accessibilityType One of the "Keychain Item Accessibility Constants" used for determining when a keychain item
 should be readable.
 
 If the value is `NULL` (the default), the Keychain default will be used.
 
 @see accessibilityType
 */
+ (void)setAccessibilityType:(CFTypeRef)accessibilityType;
#endif

@end
