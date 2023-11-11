#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>
#define XAppDelegate ((AppController*)[[UIApplication sharedApplication] delegate])
#import "MJStory-Swift.h"
//#include "CleverTapManager.h"
#import <FirebaseMessaging/FirebaseMessaging.h>
#import <Firebase/Firebase.h>
#if !DISABLE_APPSFLYER
#import "AppsFlyerLib.h"
#endif
@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate
, FIRMessagingDelegate, AppsFlyerDeepLinkDelegate
//#if IS_CLEVER_TAP_ENABLE
//,CleverTapInboxViewControllerDelegate
//#endif
> {
//    UIWindow *window;
    int countFBLogin;
}
        
@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, retain) UIWindow * window;
@property(nonatomic, readonly) UITextField *myTextField;
@property(nonatomic, readonly) NSString * textFiledValue;

//- (void) onGoogleSignIn;
//- (void) onGoogleSignOut;
//- (void) onFBSignIn;
//- (void) onFBSignOut;
-(void) setCrashlyticsWithKey:(NSString*) key andValue:(NSString*) value;
-(void) showAppInbox;
-(void) initAppInbox;
-(void) applicationHandleOpenURL:(NSString *)url;
-(NSMutableArray *) getValueData;
//
-(NSString *) getTextFiled;
-(void) AddTextFiled;
-(void) connectUrl;
-(void) deleteTextFiled;
-(void) setBackGroundColorForTextFiled:(BOOL)isColor;
-(float)getFreeDiskspace;
//
-(void) setNotificationForAppinbox;
-(BOOL) setNotificationForAppsflyer;
-(void)sendLaunch:(UIApplication *)application;
-(void) pushInboxNotificationViewedEvent:(int)type andString:(NSString*) idMessage;
-(void) pushInboxNotificationViewedEvent1:(int)type andString:(NSString*) idMessage andString:(NSString*) nhan;
-(UIWindow*) getUIWindow;
@end

#endif
