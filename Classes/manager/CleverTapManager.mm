#include "CleverTapManager.h"
#include "MJDefinitions.h"
#include "MJDefault.h"
#include "MSHome.h"
#include "NativeBridge.h"
#if IS_CLEVER_TAP_ENABLE

#import <CleverTap.h>
#import "AppController.h"
#import <CleverTap+ProductConfig.h>
#import <CleverTapSyncDelegate.h>
#import <CleverTap+FeatureFlags.h>

//https://developer.clevertap.com/docs/ios#section-user-profiles
//https://blog.csdn.net/yejiajun945/article/details/49124133 -> convert types ( std::string, NSData , NSString ... ).
std::function<void()> g_callback = nullptr;
	

@interface CleverTapPushNotificationDelegateImplementation : NSObject<CleverTapPushNotificationDelegate>

- (void)pushNotificationTappedWithCustomExtras:(NSDictionary *)customExtras;

@end

@implementation CleverTapPushNotificationDelegateImplementation

- (void)pushNotificationTappedWithCustomExtras:(NSDictionary *)customExtras;
{
    if (customExtras != nil && [customExtras objectForKey:@"deeplink"] != nil)
    {
        NSString *url = [[customExtras objectForKey:@"deeplink"] stringValue];
      
        AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    
        [app applicationHandleOpenURL:url];
    }
}

@end
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


@interface CleverTapURLListenerDelegateImplementation : NSObject<CleverTapURLDelegate>

- (BOOL)shouldHandleCleverTapURL:(NSURL *_Nullable )url forChannel:(CleverTapChannel)channel;

@end

@implementation CleverTapURLListenerDelegateImplementation

- (BOOL)shouldHandleCleverTapURL:(NSURL *_Nullable )url forChannel:(CleverTapChannel)channel;
{
    if (url != nil)
    {
        AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    
        [app applicationHandleOpenURL:url.absoluteString];
    }
}

@end


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@interface CleverTapInAppNotificationDelegateImplementation : NSObject<CleverTapInAppNotificationDelegate>

- (void)inAppNotificationButtonTappedWithCustomExtras:(NSDictionary *)customExtras;

//- (void)inAppNotificationDismissedWithExtras:(NSDictionary *)extras andActionExtras:(NSDictionary *)actionExtras;

@end

@implementation CleverTapInAppNotificationDelegateImplementation

- (void)inAppNotificationButtonTappedWithCustomExtras:(NSDictionary *)customExtras
{
    if (customExtras != nil && [customExtras objectForKey:@"deeplink"] != nil)
    {
        NSString *url = [[customExtras objectForKey:@"deeplink"] stringValue];
      
        AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    
        [app applicationHandleOpenURL:url];
    }
}

//- (void)inAppNotificationDismissedWithExtras:(NSDictionary *)extras andActionExtras:(NSDictionary *)actionExtras {
//
//    if (actionExtras != nil && [actionExtras objectForKey:@"deeplink"] != nil)
//    {
//        NSString *url = [[actionExtras objectForKey:@"deeplink"] stringValue];
//
//        AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
//
//        [app applicationHandleOpenURL:url];
//    }
//}

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@interface CleverTapDelegate : NSObject<CleverTapSyncDelegate,CleverTapProductConfigDelegate,CleverTapFeatureFlagsDelegate>

- (void)profileDidInitialize:(NSString*)CleverTapID;

- (void)profileDidInitialize:(NSString*)CleverTapID forAccountId:(NSString*)accountId;

- (void)ctProductConfigFetched;
- (void)ctProductConfigActivated;
- (void)ctProductConfigInitialized;
- (void)ctFeatureFlagsUpdated;

@property (nonatomic, retain) NSDictionary* m_data;

@end

@implementation CleverTapDelegate

- (void)profileDidInitialize:(NSString*)CleverTapID
{
    NSLog(@"pushIOSProfile| called profileDidInitialize :(NSString*)CleverTapID");
    
}

- (void)profileDidInitialize:(NSString*)CleverTapID forAccountId:(NSString*)accountId
{
    if([_m_data count] <= 0)
    {
        NSLog(@"pushIOSProfile| profileDidInitialize _m_data Dictionary is empty");
        
        return;
    }
    
    NSLog(@"pushIOSProfile| called profileDidInitialize function");

    if ([_m_data objectForKey:@"latitude"] != nil &&  [_m_data objectForKey:@"longitude"] != nil)
    {
        float latitude = [[_m_data objectForKey:@"latitude"] floatValue];
           
        float longitude = [[_m_data objectForKey:@"longitude"] floatValue];
        
        CLLocationCoordinate2D location  = CLLocationCoordinate2DMake(latitude,longitude);

        [[CleverTap sharedInstance] setLocation:location];
        
        [CleverTap getLocationWithSuccess:^(CLLocationCoordinate2D location) {
             //do something with location here, optionally set on CleverTap for use in segmentation etc
            [CleverTap setLocation:location];
        }
        andError:^(NSString *error) {
            NSLog(@"CleverTapLocation Error is %@", error);
        }];
    }
    
    [[CleverTap sharedInstance] profilePush:_m_data];
    
    if (g_callback)
    {	
        NSLog(@"pushIOSProfile| called profileDidInitialize -> called g_callback accountId:%@",accountId);

        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
        {
            g_callback();
        });
    }
}

- (void)ctProductConfigFetched
{
    NSLog(@"Product Config ctProductConfigFetched");

}

- (void)ctProductConfigActivated
{
    NSLog(@"Product Config ctProductConfigActivated");
    
}

- (void)ctProductConfigInitialized
{
    NSLog(@"Product Config ctProductConfigInitialized");

}

- (void)ctFeatureFlagsUpdated
{
    NSLog(@"ctFeatureFlagsUpdated");
    
    if (!NativeBridge::networkAvaiable())
    {
        return;
    }
    
    BOOL isMonkeyTalkingShow =[[[CleverTap sharedInstance] featureFlags] get:@"showmonkeytalking" withDefaultValue:NO];
    
    MJDEFAULT->setBoolForKey(SHOW_MONKEY_TALKING_KEY, isMonkeyTalkingShow);
}

@end

static CleverTapDelegate * delegate = nullptr;

static CleverTapInAppNotificationDelegateImplementation * notificationDelegate = nullptr;

static CleverTapURLListenerDelegateImplementation * urlDelegate = nullptr;

static CleverTapPushNotificationDelegateImplementation * pushNotificationDelegate = nullptr;

void CleverTapManager::pushIOSProfile(std::string i_json,PUSH_TYPE i_type, std::function<void()> i_callback)
{
    if(i_json.empty())
    {
        return;
    }
    
    if(i_callback)
    {
        NSLog(@"pushIOSProfile| g_callback !=nullptr");

        g_callback = i_callback;
    }
    else
    {
        NSLog(@"pushIOSProfile| g_callback ==nullptr");
        g_callback = nullptr;
              
    }
  
    NSError *jsonError;

    NSData *objectData = [[NSData alloc] initWithBytes:i_json.data() length:i_json.length()];
    
    NSDictionary *profileData = [NSJSONSerialization JSONObjectWithData:objectData
       options:NSJSONReadingMutableContainers
         error:&jsonError];
   
    if(profileData == nil || [profileData count] <= 0)
    {
      NSLog(@"pushIOSProfile| profileData Dictionary is empty");
      
      return;
    }
    
    NSString *name=[profileData objectForKey:@"Name"];
    
    if([name length] == 0)
    {
        [profileData removeObjectForKey:@"Name"];
    }
    
    NSString *parent_name=[profileData objectForKey:@"parent_name"];
    
    if([parent_name length] == 0)
    {
        [profileData removeObjectForKey:@"parent_name"];
    }
    
    NSString *device_name= [profileData objectForKey:@"device_name"];
    
    if([device_name length] == 0)
    {
        [profileData removeObjectForKey:@"device_name"];
    }
    
    NSString *parent_id= [profileData objectForKey:@"parent_id"];

    if([parent_id length] == 0)
    {
        [profileData removeObjectForKey:@"parent_id"];
    }
    
    NSString *type  = [profileData objectForKey:@"type"];
    
    if([type length] != 0)
    {
        if([type isEqualToString:@"child"])
        {
            [profileData removeObjectForKey:@"number_of_children"];
        }
    }
    
    if(i_type==PUSH_TYPE::UPDATE)
    {
        NSLog(@"pushIOSProfile| Dictionary i_type==PUSH_TYPE::UPDATE");

        [[CleverTap sharedInstance] profilePush:profileData];

        if ([profileData objectForKey:@"latitude"] != nil &&  [profileData objectForKey:@"longitude"] != nil)
        {
            float latitude = [[profileData objectForKey:@"latitude"] floatValue];
               
            float longitude = [[profileData objectForKey:@"longitude"] floatValue];
            
            CLLocationCoordinate2D location  = CLLocationCoordinate2DMake(latitude,longitude);

            [[CleverTap sharedInstance] setLocation:location];
            
            [CleverTap getLocationWithSuccess:^(CLLocationCoordinate2D location) {
                       //do something with location here, optionally set on CleverTap for use in segmentation etc
                      [CleverTap setLocation:location];
                  }
                  andError:^(NSString *error) {
                      NSLog(@"CleverTapLocation Error is %@", error);
                  }];
        }
    }
    else
    {
        
        NSLog(@"pushIOSProfile|  called onUserLogin |  i_type==PUSH_TYPE::LOGIN");

        NSDictionary *removalProfileData = [NSJSONSerialization JSONObjectWithData:objectData
              options:NSJSONReadingMutableContainers
                error:&jsonError];
           

        if(removalProfileData == nil || [removalProfileData count] <= 0)
        {
           NSLog(@"pushIOSProfile| removalProfileData Dictionary is empty");
           
           return;
        }
        
        NSString *name = [removalProfileData objectForKey:@"Name"];
        
        if([name length] == 0)
        {
            [removalProfileData removeObjectForKey:@"Name"];
        }
        
        NSString *parent_name=[removalProfileData objectForKey:@"parent_name"];
        
        if([parent_name length] == 0)
        {
            [removalProfileData removeObjectForKey:@"parent_name"];
        }
        
        NSString *device_name= [removalProfileData objectForKey:@"device_name"];
        
        if([device_name length] == 0)
        {
            [removalProfileData removeObjectForKey:@"device_name"];
        }
        
        NSString *parent_id= [removalProfileData objectForKey:@"parent_id"];

        if([parent_id length] == 0)
        {
            [removalProfileData removeObjectForKey:@"parent_id"];
        }

        NSString *type  = [removalProfileData objectForKey:@"type"];
        
        if([type length] != 0)
        {
            if([type isEqualToString:@"child"])
            {
              [removalProfileData removeObjectForKey:@"number_of_children"];
            }
        }
        
        if(!delegate)
        {
           delegate = [CleverTapDelegate new];
        }


        delegate.m_data = profileData;

        [removalProfileData removeObjectForKey:@"Email"];
        //[removalProfileData removeObjectForKey:@"Phone"];
        
        registerDelegates();
        
        [[CleverTap sharedInstance] setSyncDelegate:delegate];

        [[CleverTap sharedInstance] onUserLogin:removalProfileData];

    }

}
std::string CleverTapManager::getCTIdentity(){

    NSString *ctId =  [[CleverTap sharedInstance] profileGetCleverTapAttributionIdentifier];
    std::string result = std::string([ctId UTF8String]);
    return result;
}

void CleverTapManager::autoIntegrate()
{
#if (!DISABLE_CLEVERTAP)
    [CleverTap autoIntegrate];
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    [CleverTap setDebugLevel:CleverTapLogDebug];
#endif
    
#endif
}
void CleverTapManager::pushIOSEvent(std::string i_eventName,std::string i_eventProperties)
{
    if(i_eventName.empty())
    {
        return;
    }
  
    NSString *eventName = [[NSString alloc] initWithCString:i_eventName.c_str() encoding:NSUTF8StringEncoding];

    if(i_eventProperties.empty())
    {
        [[CleverTap sharedInstance] recordEvent:eventName];
    }
    else
    {
        NSError *jsonError;

        NSData *propertiesData = [[NSData alloc] initWithBytes:i_eventProperties.data() length:i_eventProperties.length()];
      
        NSDictionary *properties = [NSJSONSerialization JSONObjectWithData:propertiesData
      options:NSJSONReadingMutableContainers
        error:&jsonError];
        
        if([properties count] <= 0)
        {
            NSLog(@"pushIOSProfile| profileData Dictionary is empty");
            
            return;
        }
        
        std::vector<std::string> convertToDateEventNames =
        {
            "time_purchased",
            "time_activated",
            "time_expired",
        };
        
        for(auto eventName : convertToDateEventNames)
        {
            NSString *time_purchased = [properties objectForKey:[NSString stringWithUTF8String:eventName.c_str()]];
            
            if(time_purchased != nil)
            {
                double myDouble = [time_purchased doubleValue];
                
                if(myDouble !=0)
                {
                    NSDate *date = [NSDate dateWithTimeIntervalSince1970:myDouble];
                    
                    [properties setValue:date forKey:[NSString stringWithUTF8String:eventName.c_str()]];
                }
            }
        }
        
        [[CleverTap sharedInstance] recordEvent:eventName withProps:properties];
    }
}
std::string CleverTapManager::getCleverTabIdentityIOS(){

    NSString *ctId =  [[CleverTap sharedInstance] profileGetCleverTapAttributionIdentifier];

    std::string result = std::string([ctId UTF8String]);

    return result;

}
void CleverTapManager::showAppInboxIOS()
{
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app showAppInbox];
}

void CleverTapManager::initAppInboxIOS()
{
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app initAppInbox];
}

std::vector<std::string> CleverTapManager::getValueDataAppInboxIos()
{
    std::vector<std::string> value;
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    NSMutableArray *array = [app getValueData];
//    for (NSString *jsonString : array) {
//        std::string valueJson = std::string([jsonString UTF8String]);
//        value.push_back(valueJson);
//    }
    return value;
}

void CleverTapManager::pushInboxNotificationViewedEvent(std::string messageId, int type)
{
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    NSString * idMessage = [[NSString alloc] initWithUTF8String:messageId.c_str()];
    [app pushInboxNotificationViewedEvent:type andString:idMessage];
}
void CleverTapManager::registerDelegates() {
    
    if(!urlDelegate)
    {
        urlDelegate = [CleverTapURLListenerDelegateImplementation new];
    }
    
    if(!pushNotificationDelegate)
    {
        pushNotificationDelegate = [CleverTapPushNotificationDelegateImplementation new];
    }
        
    [[CleverTap sharedInstance] setUrlDelegate:urlDelegate];
    [[CleverTap sharedInstance] setInAppNotificationDelegate:notificationDelegate];
    [[CleverTap sharedInstance] setPushNotificationDelegate:pushNotificationDelegate];

}

void CleverTapManager::initProductConfig() {

    if(!delegate)
    {
       delegate = [CleverTapDelegate new];
    }
    
    [CleverTap sharedInstance].productConfig.delegate = delegate;
    [[[CleverTap sharedInstance] productConfig] reset];
    [[[CleverTap sharedInstance] productConfig] setMinimumFetchInterval:60];
}

void CleverTapManager::fechProductConfig() {
  
    [[[CleverTap sharedInstance] productConfig] fetchAndActivate];
}

bool  CleverTapManager::getProductConfigBoolValue(std::string key) {
    
    if(key.empty()) {
        return false;
    }
    
    NSString *nsKey = [[NSString alloc] initWithCString:key.c_str() encoding:NSUTF8StringEncoding];

    return [[[[CleverTap sharedInstance] productConfig] get:nsKey] boolValue];;
}

void CleverTapManager::initFeatureFlags() {
    
    if(!delegate)
    {
       delegate = [CleverTapDelegate new];
    }

    [CleverTap sharedInstance].featureFlags.delegate = delegate;
}

bool CleverTapManager::getFeatureFlagsBoolValue(std::string key,bool defaultValue) {
    
    NSString *nsKey = [[NSString alloc] initWithCString:key.c_str() encoding:NSUTF8StringEncoding];

    BOOL featureFlag = [[[CleverTap sharedInstance] featureFlags] get:nsKey withDefaultValue:defaultValue];
    
    return featureFlag;
}
void CleverTapManager::loginDummyProfile(){
    
    std::string i_json = DUMMY_PROFILE;
    
    NSData *objectData = [[NSData alloc] initWithBytes:i_json.data() length:i_json.length()];
    NSError *jsonError;
    NSDictionary *profileData = [NSJSONSerialization JSONObjectWithData:objectData
       options:NSJSONReadingMutableContainers
         error:&jsonError];
    [[CleverTap sharedInstance] profilePush:profileData];
}

#endif
