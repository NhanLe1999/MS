/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
#import "AppController.h"
#import "AppDelegate.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "RootViewController.h"
#include "NativeBridge.h"
#include "MJPay.h"
#include "CleverTapManager.h"

#import <FBSDKCoreKit/FBSDKCoreKit.h>

#import <GoogleSignIn/GoogleSignIn.h>

#import <UserNotifications/UserNotifications.h>
#include <FirebaseCrashlytics/FirebaseCrashlytics.h>
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <ReplayKit/ReplayKit.h>
#endif
#import "MJStory-Swift.h"
#include "CameraHelper.h"

#include "StoryLanguageManager.h"
//#include "LimitedLengthTextField.h"
#include "APOTPPopup.h"
#include "APPopupBase.h"
#include "MJhelper.h"
#if IS_CLEVER_TAP_ENABLE

#import <CleverTap.h>
#import <CleverTapInstanceConfig.h>
#import <CleverTap+Inbox.h>
#import <Foundation/Foundation.h>

#endif

#include <ifaddrs.h>
#include <arpa/inet.h>

#import <UserNotifications/UserNotifications.h>
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>

#define SYSTEM_VERSION_LESS_THAN(v) ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)

@interface AppController()<UIApplicationDelegate, UNUserNotificationCenterDelegate>
@end

@implementation AppController
@synthesize window;
#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

// cocos2d application instance
- (void)didResolveDeepLink:(AppsFlyerDeepLinkResult *)result {//for appsflyer onelink only
    NSLog(@"this function will do nothing");
    switch (result.status) {
        case AFSDKDeepLinkResultStatusNotFound:
            NSLog(@"[AFSDK] Deep link not found");
            break;
        case AFSDKDeepLinkResultStatusFound:
            NSLog(@"[AFSDK] Deep link found");
            break;
        
        case AFSDKDeepLinkResultStatusFailure:
            NSLog(@"[AFSDK] Deep link found");
            break;
        
        default:
            break;
    }
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSTimeInterval delayInSeconds = 1.0;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        if ([[UIApplication sharedApplication] respondsToSelector:@selector(supportsAlternateIcons)] &&
            [[UIApplication sharedApplication] supportsAlternateIcons])
        {
            NSMutableString *selectorString = [[NSMutableString alloc] initWithCapacity:40];
            [selectorString appendString:@"_setAlternate"];
            [selectorString appendString:@"IconName:"];
            [selectorString appendString:@"completionHandler:"];

            SEL selector = NSSelectorFromString(selectorString);
            IMP imp = [[UIApplication sharedApplication] methodForSelector:selector];
            
            void (*func)(id, SEL, id, id) = (void (*)(id, SEL, id, id)) imp;
            if (func)
            {
                func([UIApplication sharedApplication], selector, @"AppIconDf", ^(NSError * _Nullable error) {
                    int cc = 0;
                });
            }
        }
    });
 
    [FIRApp configure];
    
    [FIRMessaging messaging].delegate = self;
    
    [GIDSignIn sharedInstance].clientID = [FIRApp defaultApp].options.clientID;
    
    [[FBSDKApplicationDelegate sharedInstance] application:application
                             didFinishLaunchingWithOptions:launchOptions];
    
    CleverTapManager::GetInstance()->initFeatureFlags();
    CleverTapManager::GetInstance()->autoIntegrate();
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Use RootViewController manage CCEAGLView
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter]
       addObserver:self selector:@selector(orientationChanged:)
       name:UIDeviceOrientationDidChangeNotification
       object:[UIDevice currentDevice]];
    
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        [window addSubview: _viewController.view];
    }
    else
    {
        [window setRootViewController:_viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    CleverTapManager::GetInstance()->registerDelegates();
    
    [GIDSignIn sharedInstance].presentingViewController = _viewController;
    [[AppsFlyerLib shared] setDeepLinkDelegate:(id)self];
    //AppsFlyerX::setDidResolveDeepLink(didResolveDeepLink);
    app->run();
    //[self setNotificationForAppsflyer];// chua tim duoc cach su dung vi la ung dung cho trẻ em
//    CGRect someRect = CGRectMake(0.0, 0.0, 100.0, 30.0);
//    UITextField* text = [[UITextField alloc] initWithFrame:someRect];
//    UIView* view = [[UIView alloc] initWithFrame:someRect];
//
//    [view addSubview:text];
    
    return true;
}
bool  isOrientationLock = NO;
- (void) orientationChanged:(NSNotification *)note
{
  //UIInterfaceOrientation orientationBar = [UIApplication sharedApplication].statusBarOrientation;
   UIDevice * device = note.object;
    UIDeviceOrientation orientation = device.orientation;
    if(orientation == UIDeviceOrientationPortrait)
    {
        isOrientationLock = YES;
    }
    else
    {
        isOrientationLock = NO;
    }
}
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
#if !DISABLE_APPSFLYER
    [[AppsFlyerLib shared] registerUninstall:deviceToken];
#endif
    NSString *strtoken=[NSString stringWithFormat:@"%@",deviceToken];
    strtoken=[strtoken stringByReplacingOccurrencesOfString:@"<" withString:@""];
    strtoken=[strtoken stringByReplacingOccurrencesOfString:@">" withString:@""];
    strtoken=[strtoken stringByReplacingOccurrencesOfString:@" " withString:@""];
    NSLog(@"fuck fuck");
    std::string bar = std::string([strtoken UTF8String]);
    printf("Token = %s", bar.c_str());
    [[NSUserDefaults standardUserDefaults] setValue:strtoken forKey:@"device_token_push"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

-(void) showAppInbox
{
#if IS_CLEVER_TAP_ENABLE
    CleverTapInboxStyleConfig *style = [[CleverTapInboxStyleConfig alloc] init];
    CleverTapInboxViewController *inboxController = [[CleverTap sharedInstance] newInboxViewControllerWithConfig:style];
    
    if (inboxController)
    {
        UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:inboxController];
        [_viewController presentViewController:navigationController animated:YES completion:nil];
    }
#endif
}

-(void) initAppInbox
{
#if IS_CLEVER_TAP_ENABLE
    [[CleverTap sharedInstance] initializeInboxWithCallback:^(BOOL success)
     {
        int messageCount = (int)[[CleverTap sharedInstance] getInboxMessageCount];
        int unreadCount = (int)[[CleverTap sharedInstance] getInboxMessageUnreadCount];
        NSLog(@"Inbox Message: %d/%d", messageCount, unreadCount);
    }];
    NSArray<CleverTapInboxMessage *> *all =  [[CleverTap sharedInstance] getUnreadInboxMessages];
    
    if([all count] > 0)
    {
        for( CleverTapInboxMessage * cc in all)
        {
            NSDictionary *json = cc.json;
            NSDictionary *customData = cc.customData;
        }
    }

    //NSMutableArray *all =  [[CleverTap sharedInstance] getUnreadInboxMessages];
    
#endif
}

//getTextFill

-(void) deleteTextFiled
{
    [_myTextField setHidden:true];
    [_myTextField setEnabled:false];
}

-(void) setBackGroundColorForTextFiled:(BOOL)isColor
{
    if(isColor)
    {
        _myTextField.backgroundColor = [UIColor colorWithRed:102.0 / 255.0 green:102.0 / 255.0 blue:102.0/ 255.0 alpha:1.0];
    }
    else{
        _myTextField.backgroundColor = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
    }
}

-(NSString *) getTextFiled
{
    NSString * text =_myTextField.text;
    //std::string value = std::string([text UTF8String]);
    return text;
}

- (NSString *)getIPAddress {

    NSString *address = @"error";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"]) {
                    // Get NSString from C String
                    address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];

                }

            }

            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return address;

}

-(void) AddTextFiled
{
    float x = self.viewController.view.frame.size.width / 2 - 100;
    float y = self.viewController.view.frame.size.height / 2 - 25;
    
    _myTextField = [[UITextField alloc] initWithFrame:CGRectMake(x, y, 200, 50)];

    _myTextField.borderStyle = UITextBorderStyleRoundedRect;
    _myTextField.textColor = UIColor.blackColor;
    
    //_myTextField.alpha = 0.01;

    if (@available(iOS 12.0, *)) {
          _myTextField.textContentType =UITextContentTypeOneTimeCode;
    } else {
        // Fallback on earlier versions
    }
    _myTextField.placeholder = @"Enter OTP";
    _myTextField.returnKeyType = UIReturnKeyDone;
    _myTextField.keyboardType = UIKeyboardTypeNumberPad;
    _myTextField.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    _myTextField.textAlignment = NSTextAlignmentCenter;
    
    [_myTextField addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
    self.viewController.view.backgroundColor = [UIColor blackColor];
    [self.viewController.view addSubview:_myTextField];
}


- (void)textFieldDidChange:(UITextField *)textField {
    NSLog(@"text changed: %@", textField.text);
    
    NSString * text =_myTextField.text;
    std::string value = std::string([text UTF8String]);
    
    BOOL isTextNumber = mj::helper::isValidNumber(value);
    if(!isTextNumber)
    {
        textField.text = _textFiledValue;
    }
    
    if(textField.text.length <= 4)
    {
        _textFiledValue = textField.text;
        if(auto POTPPopup = dynamic_cast<APOTPPopup*>(cocos2d::utils::findChild(cocos2d:: Director::getInstance()->getRunningScene(), "APOTPPopup")))
        {
            POTPPopup->setEnableButtonOk();
        }
    }else{
        if(textField.text.length > 4)
        {
            textField.text = _textFiledValue;
        }
    }
    
}

-(BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSString * searchStr = [textField.text stringByReplacingCharactersInRange:range withString:string];

    NSLog(@"%@",searchStr);
    return YES;
}

-(NSMutableArray *) getValueData
{
    NSMutableArray * value;
#if IS_CLEVER_TAP_ENABLE
    [[CleverTap sharedInstance] initializeInboxWithCallback:^(BOOL success)
     {
        int messageCount = (int)[[CleverTap sharedInstance] getInboxMessageCount];
        int unreadCount = (int)[[CleverTap sharedInstance] getInboxMessageUnreadCount];
        NSLog(@"Inbox Message: %d/%d", messageCount, unreadCount);
    }];
    NSArray<CleverTapInboxMessage *> *all =  [[CleverTap sharedInstance] getUnreadInboxMessages];
        
    if([all count] > 0)
    {
        for( CleverTapInboxMessage * cc in all)
        {
            NSDictionary *json = cc.json;
            
            //NSString *valueOb = [NSString stringWithFormat:@"%@", json];
            
            
            
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:json options:NSJSONWritingPrettyPrinted error:&error];
                NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
//                std::string valueJson = std::string([jsonString UTF8String]);
//                value.push_back(valueJson);
                [value addObject:jsonString];

                NSLog(@"%@", jsonString);
            
        }
    }

    //NSMutableArray *all =  [[CleverTap sharedInstance] getUnreadInboxMessages];
    
#endif
    return value;
}

-(void) setNotificationForAppinbox
{
    if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
        auto valueNotification = delegate->getValueNotificationAppinbox();
        if(valueNotification.empty())
        return;
        
        auto notification = valueNotification[0];
        auto title = notification.first;
        std::string message = notification.second.asString();
        NSString *valueTitle = [[NSString alloc] initWithUTF8String:title.c_str()];
        NSString *valueMessage = [[NSString alloc] initWithUTF8String:message.c_str()];
        UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
        content.title = [NSString localizedUserNotificationStringForKey:valueTitle arguments:nil];
        content.body = [NSString localizedUserNotificationStringForKey:valueMessage
                                                             arguments:nil];
        content.sound = [UNNotificationSound defaultSound];

        /// 4. update application icon badge number
        content.badge = @([[UIApplication sharedApplication] applicationIconBadgeNumber] + 1);
        // Deliver the notification in five seconds.
        UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger
                                                      triggerWithTimeInterval:300.f repeats:NO];
        UNNotificationRequest *request = [UNNotificationRequest requestWithIdentifier:valueTitle
                                                                              content:content trigger:trigger];
        /// 3. schedule localNotification
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        NSLog(@"nhan thanh cong succeeded!");
        [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
            if (!error) {
                NSLog(@"add NotificationRequest succeeded!");
            }
        }];
    }
}
- (void)sendLaunch:(UIApplication *)application {
    if (@available(iOS 14, *)) {
        // voi ios 14 tro len, sẽ xu ly cho khac
    }
    else
    {
        //[[AppsFlyerLib shared] start];
    }
    //
}
-(BOOL) setNotificationForAppsflyer
{
    [AppsFlyerLib shared].appsFlyerDevKey = @"HEX6DVgSUPuyhSY6Z3PDRh";
    [AppsFlyerLib shared].appleAppID = @"1236664909";
       /* Uncomment the following line to see AppsFlyer debug logs */
       // [AppsFlyerLib shared].isDebug = true;
    // SceneDelegate support
    [[NSNotificationCenter defaultCenter] addObserver:self
     selector:@selector(sendLaunch:)
     name:UIApplicationDidBecomeActiveNotification
     object:nil];
    if (@available(iOS 10, *)) {
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        center.delegate = self;
        [center requestAuthorizationWithOptions:(UNAuthorizationOptionSound | UNAuthorizationOptionAlert | UNAuthorizationOptionBadge) completionHandler:^(BOOL granted, NSError * _Nullable error) {
        }];
    }

    else {
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes: UIUserNotificationTypeAlert | UIUserNotificationTypeSound | UIUserNotificationTypeBadge categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    }

    [[UIApplication sharedApplication] registerForRemoteNotifications];
    //[[AppsFlyerLib shared] waitForATTUserAuthorizationWithTimeoutInterval:60];
    [[AppsFlyerLib shared] startWithCompletionHandler:^(NSDictionary<NSString *,id> *dictionary, NSError *error) {
            if (error) {
                NSLog(@"%@", error);
                return;
            }
            if (dictionary) {
                NSLog(@"%@", dictionary);
                //mj::helper::showATTrackingNotification();
                return;
            }
        }];
    mj::helper::showATTrackingNotification();
    return YES;
}
-(void)pushInboxNotificationViewedEvent:(int)type andString:(NSString*) idMessage
{
    if(type == 0)
    {
        [[CleverTap sharedInstance] recordInboxNotificationViewedEventForID:(NSString * _Nonnull)idMessage];
    }else{
        [[CleverTap sharedInstance] recordInboxNotificationClickedEventForID:(NSString * _Nonnull)idMessage];
    }
}

-(UIWindow*) getUIWindow
{
    return window;
}


-(void) setCrashlyticsWithKey:(NSString *)key andValue:(NSString *)value{
//    [[FIRCrashlytics crashlytics] setObjectValue:value forKey:key];
    [[FIRCrashlytics crashlytics] setCustomValue:value forKey:key]; // phiên bản crashlytics mới dùng hàm này, nếu lỗi thì đổi lại dùng hàm ở trên
}
- (void)applicationWillResignActive:(UIApplication *)application {
    
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app setNotificationForAppinbox];
    
    if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
        delegate->applicationWillResignActive();
    }
    
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app setNotificationForAppinbox];
    if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
        delegate->applicationDidBecomeActive();
    }
    
    [FBSDKAppEvents activateApp];
//
//    // Do the following if you use Mobile App Engagement Ads to get the deferred
//    // app link after your app is installed.
    [FBSDKAppLinkUtility fetchDeferredAppLink:^(NSURL *url, NSError *error) {
        if (error) {
            NSLog(@"Received error while fetching deferred app link %@", error);
        }
        if (url) {
            [[UIApplication sharedApplication] openURL:url];
        }
    }];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app setNotificationForAppinbox];
    if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
        delegate->applicationWillTerminate();
    }
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {

}

- (void)dealloc {
    [window release];
    [super dealloc];
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    if(isOrientationLock)
        return UIInterfaceOrientationMaskLandscapeRight;
    else
        return UIInterfaceOrientationMaskLandscape;
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err {
    NSLog(@"failed to regiser %@", err);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    NSLog(@"notification options %@", userInfo);
}

-(void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler{
    NSLog(@"User Info : %@",notification.request.content.userInfo);
    completionHandler(UNAuthorizationOptionSound | UNAuthorizationOptionAlert | UNAuthorizationOptionBadge);
}

-(void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void(^)())completionHandler{
    NSLog(@"User Info : %@",response.notification.request.content.userInfo);
    completionHandler();
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    BOOL handledFacebookSignIn = [[FBSDKApplicationDelegate sharedInstance] application:application openURL:url sourceApplication:options[UIApplicationOpenURLOptionsSourceApplicationKey] annotation:options[UIApplicationOpenURLOptionsAnnotationKey] ];
    
    
    BOOL handledGoogleSignIn = [[GIDSignIn sharedInstance] handleURL:url];
                          
    if (handledFacebookSignIn) return handledFacebookSignIn;
    if (handledGoogleSignIn) return handledGoogleSignIn;

}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(nullable NSString *)sourceApplication annotation:(id)annotation {

    if(url != nil ) {
        [self applicationHandleOpenURL:url.absoluteString];
    }

    NSLog(@"Handle URL: %@", [url absoluteString]);

    NSURLComponents * component = [NSURLComponents componentsWithURL:url resolvingAgainstBaseURL:YES];
    for (NSURLQueryItem * item in component.queryItems) {

        if ([item.name isEqualToString:@"mdb"]) {
            int value = [item.value intValue];
            cocos2d::UserDefault::getInstance()->setIntegerForKey("key.debug.epoch", value);
        }
    }
    
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                   openURL:url
                                         sourceApplication:sourceApplication
                                                annotation:annotation];
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {

    if(url != nil ) {
        [self applicationHandleOpenURL:url.absoluteString];
    }
    
    NSLog(@"Handle URL: %@", [url absoluteString]);
    [[[application keyWindow] rootViewController] dismissViewControllerAnimated:YES completion:nil];

    NSURLComponents * component = [NSURLComponents componentsWithURL:url resolvingAgainstBaseURL:YES];
    
    for (NSURLQueryItem * item in component.queryItems) {
       
        if ([item.name isEqualToString:@"mdb"]) {
            int value = [item.value intValue];
            cocos2d::UserDefault::getInstance()->setIntegerForKey("key.debug.epoch", value);
        }
    }

    return YES;
}

- (void)applicationHandleOpenURL:(NSString *)url
{
    if(url != nil) {
        if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
            delegate->applicationHandleOpenURL([url UTF8String]);
        }
    }
}

-(float)getFreeDiskspace {
    uint64_t totalSpace = 0;
    uint64_t totalFreeSpace = 0;
    NSError *error = nil;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSDictionary *dictionary = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[paths lastObject] error: &error];

    if (dictionary) {
        NSNumber *fileSystemSizeInBytes = [dictionary objectForKey: NSFileSystemSize];
        NSNumber *freeFileSystemSizeInBytes = [dictionary objectForKey:NSFileSystemFreeSize];
        totalSpace = [fileSystemSizeInBytes unsignedLongLongValue];
        totalFreeSpace = [freeFileSystemSizeInBytes unsignedLongLongValue];
        NSLog(@"Memory Capacity of %llu MiB with %llu MiB Free memory available.", ((totalSpace/1024ll)/1024ll), ((totalFreeSpace/1024ll)/1024ll));
    } else {
        NSLog(@"Error Obtaining System Memory Info: Domain = %@, Code = %ld", [error domain], (long)[error code]);
    }
    
    float freeSpace =(float) ((totalFreeSpace/1024ll)/1024ll);

    return freeSpace / 1024;
}

@end
#endif
