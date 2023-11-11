//
//  MJPlatformConfig-Apple.cpp
//  M
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#include "MJPlatformConfig-Apple.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "Valet/Valet.h"
#include "StoryConfigManager.h"
#include "MJDefault.h"
#import "StoryLanguageManager.h"
#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>
#import <sys/utsname.h>
#import <StoreKit/StoreKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>
#import <FBSDKShareKit/FBSDKShareDialog.h>
#include "MJHelper.h"

#define GROUD_IDENTIFIER @"com.earlystart"
#define UUID_KEYCHAIN @"monkey_junior_uuid"

@interface MailHelper : NSObject<MFMailComposeViewControllerDelegate> {
}
+ (id) sharedHelper;
- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(nullable NSError *)error;
@end

@implementation MailHelper
+ (id)sharedHelper {
    static MailHelper * shared_helper = nil;
    @synchronized (self) {
        if (shared_helper == nil) {
            shared_helper = [[self alloc] init];
        }

        return shared_helper;
    }
}

- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(nullable NSError *)error {
    [controller dismissViewControllerAnimated:YES completion:nil];
}
@end

@interface StoreViewController : SKStoreProductViewController {
}
- (BOOL)shouldAutorotate;
@end

@implementation StoreViewController
- (BOOL)shouldAutorotate {
    return NO;
}
@end

@interface StoreHelper : NSObject<SKStoreProductViewControllerDelegate> {
}
+ (id) sharedHelper;
- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController;
@end

@implementation StoreHelper
+ (id)sharedHelper {
    static StoreHelper * shared_helper = nil;
    @synchronized (self) {
        if (shared_helper == nil) {
            shared_helper = [[self alloc] init];
        }
        
        return shared_helper;
    }
}

- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController {
    [viewController dismissViewControllerAnimated:YES completion:nil];
}

@end


@interface ImageHelper : NSObject<SKStoreProductViewControllerDelegate> {
}
+ (id) ImageHelper;
- (void)thisImage:(UIImage *)image hasBeenSavedInPhotoAlbumWithError:(NSError *)error usingContextInfo:(void*)ctxInfo;

@end

@implementation ImageHelper
+ (id)ImageHelper {
    static ImageHelper * img_helper = nil;
    @synchronized (self) {
        if (img_helper == nil) {
            img_helper = [[self alloc] init];
        }
        
        return img_helper;
    }
}
- (void)thisImage:(UIImage *)image hasBeenSavedInPhotoAlbumWithError:(NSError *)error usingContextInfo:(void*)ctxInfo
{
    if (error) {
        // Do anything needed to handle the error or display it to the user
        
    } else {
        // .... do anything you want here to handle
        // .... when the image has been saved in the photo album
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("save picture");
    }
}

@end

namespace mj {
    void PlatformConfigApple::clearUserDefaults()
    {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSString *bundleIdentifier = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleIdentifierKey];
        NSMutableDictionary *dict = [[defaults persistentDomainForName:bundleIdentifier] mutableCopy];

        for (NSString *key in [dict allKeys])
        {
           id value = dict[key];
    
           if([key  containsString: @"WizRocket"])
           {
               //[dict removeObjectForKey: key];
               NSLog(@"clearUserDefaults|We dont delete the CleverTap Key: %@ - value: %@", key, value);
           }
           else
           {
               //[dict removeObjectForKey: key];
               NSLog(@"clearUserDefaults|delete key:%@ - value: %@", key, value);

               [[NSUserDefaults standardUserDefaults] removeObjectForKey:key];
           }
           //NSLog(@"key: %@ - value: %@", key, value);
        }
        
       // [[NSUserDefaults standardUserDefaults] removePersistentDomainForName:[[NSBundle mainBundle] bundleIdentifier]];
    }
 
    std::string PlatformConfigApple::getUserDefaults() {
        
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSString *bundleIdentifier = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleIdentifierKey];
        NSMutableDictionary *dict = [[defaults persistentDomainForName:bundleIdentifier] mutableCopy];
        for (NSString *key in [dict allKeys]) {
            id value = dict[key];
            NSLog(@"key: %@ - value: %@", key, value);
        }
        
        for (NSString *key in [dict allKeys]) {
            id value = dict[key];
            if([key  containsString: @"com.facebook"] || [key  containsString: @"WebKit"] || [key  containsString: @"GID"] || [key  containsString: @"com.google.firebase"]){
                [dict removeObjectForKey: key];

            }
            NSLog(@"key: %@ - value: %@", key, value);
        }
        
        NSError * err;
        NSData * jsonData = [NSJSONSerialization  dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&err];
        NSString * myString = [[NSString alloc] initWithData:jsonData   encoding:NSUTF8StringEncoding];
        
        return [myString UTF8String];
    }
    
    void PlatformConfigApple::setUserDefaults(std::string info){
        NSError *jsonError;
        NSString *jsonString = [NSString stringWithCString:info.c_str()
                                                  encoding:[NSString defaultCStringEncoding]];
        NSData *data = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
        NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data
                                                             options:NSJSONReadingMutableContainers
                                                               error:&jsonError];
        
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        
        for (NSString *key in json) {
            id value = json[key];
            [defaults setObject: value forKey:key] ;
            [defaults synchronize];
        }
    }
    void PlatformConfigApple::rotateKeyboard(bool isPortrait) {
        if (isPortrait)
            [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
        else
            [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
    }

    /** get unique device id
     *  @return a std::string contains MAC address, identifier for vendor and uuid saved in keychain with delimiter "|"
     */
    std::string PlatformConfigApple::getPlatformDeviceID() {
        auto fake_id = MJDEFAULT->getStringForKey("fake_id", "");
        if (!fake_id.empty()) {
            return fake_id;
        }

        std::string macaddress = "";
        std::string uuid;
        std::string device_id = [[[[UIDevice currentDevice] identifierForVendor] UUIDString] UTF8String];

        VALValet * otherValet = [[VALValet alloc] initWithIdentifier:@"com.nhungdao.edu" accessibility:VALAccessibilityWhenUnlocked];
        if ([otherValet containsObjectForKey:@"EduKeyChain"]) {
            NSLog(@"Old keychain: %@", [otherValet stringForKey:@"EduKeyChain"]);
        }

        VALValet *mySharedValet = [[VALValet alloc] initWithSharedAccessGroupIdentifier:GROUD_IDENTIFIER accessibility:VALAccessibilityWhenUnlocked];

        if ([mySharedValet containsObjectForKey:UUID_KEYCHAIN]) {
            uuid = [[mySharedValet stringForKey:UUID_KEYCHAIN] UTF8String];
        } else {
            uuid = [[[NSUUID UUID] UUIDString] UTF8String];
            [mySharedValet setString:[NSString stringWithUTF8String:uuid.c_str()] forKey:UUID_KEYCHAIN];
        }

        return macaddress + "|" + device_id + "|" + uuid;
    }

    std::string PlatformConfigApple::getOS() {
        return "ios";
    }

    std::string PlatformConfigApple::getAppVersion() {
        //NSString *build = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey];
        NSString * version = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"];
        return [version UTF8String];
    }
    
    std::string PlatformConfigApple::getBuildNumber() {
        NSString *build = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey];
        return [build UTF8String];
    }

    std::string PlatformConfigApple::getReceipt() {
        NSURL * receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
        NSData * receipt = [NSData dataWithContentsOfURL:receiptURL];
        if (!receipt) {
            //không có receipt, người dùng không trong subscription
//            cocos2d::MessageBox("Bạn chưa đăng ký gói thuê bao nào", "Monkey Junior");
            return "";
        }

        return std::string([[receipt base64EncodedStringWithOptions:0] UTF8String]);
    }

    bool PlatformConfigApple::phoneSupport() {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
        if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
            [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
        }
        
        std::string phone_number =LANGUAGE_MANAGER->getDisplayTextByKey("key.support.info.phonenumber");

        NSString *phone = [NSString stringWithCString:phone_number.c_str()
        encoding:[NSString defaultCStringEncoding]];
        NSString * phone_uri = [@"tel:" stringByAppendingString:phone];
        NSURL * url = [NSURL URLWithString:phone_uri];
        if (![[UIApplication sharedApplication] canOpenURL:url]) {
            return false;
        }
        [[UIApplication sharedApplication] openURL:url];
        return true;

//        [[UIApplication sharedApplication] openURL:url options:nil completionHandler:nil];
    }

    bool PlatformConfigApple::mailSupport()
    {
        std::string content = LANGUAGE_MANAGER->getDisplayTextByKey("SupportView_Mail") ;
        content += getDeviceInfo();
        
        std::string subject = LANGUAGE_MANAGER->getDisplayTextByKey("SupportView_Mail_title");
        subject += getAppName() + " v" + getAppVersion();
        
        content = mj::helper::urlencode(content);
        subject = mj::helper::urlencode(subject);
        
        auto mail_uri = StringUtils::format("mailto:%s?subject=%s&body=%s", LANGUAGE_MANAGER->getDisplayTextByKey("key.support.info.email").c_str(), subject.c_str(), content.c_str());
        cocos2d::Application::getInstance()->openURL(mail_uri);
        return true;
    }

    std::string PlatformConfigApple::getDeviceInfo() {
        std::stringstream ss;
        ss << "App Version: " << getAppVersion() << std::endl;
        ss << "OS: " << [[[UIDevice currentDevice] systemName] UTF8String] << std::endl;
        ss << "Model: " << getDeviceName() << std::endl;
        ss << "System Version: " << [[[UIDevice currentDevice] systemVersion] UTF8String] << std::endl;
        ss << "Device Identifier: " << getDeviceId();

        return ss.str();
    }

    std::string PlatformConfigApple::getDeviceInfoShort() {
        std::stringstream ss;
        ss << getAppVersion() << "|"
           << [[[UIDevice currentDevice] systemName] UTF8String] << "|"
           << getDeviceName() << "|"
           << [[[UIDevice currentDevice] systemVersion] UTF8String] << "|"
           << getDeviceId();

        return ss.str();
    }

    const std::unordered_map<std::string, std::pair<std::string, int>> IOS_MODEL = {
        {"AppleTV2,1", {"Apple TV 2", 0}},
        {"AppleTV3,1", {"Apple TV 3", 0}},
        {"AppleTV3,2", {"Apple TV 3", 0}},
        {"AppleTV5,3", {"Apple TV 4", 0}},
        {"Watch1,1", {"Apple Watch 1", 0}},
        {"Watch1,2", {"Apple Watch 1", 0}},
        {"Watch2,6", {"Apple Watch Series 1", 0}},
        {"Watch2,7", {"Apple Watch Series 1", 0}},
        {"Watch2,3", {"Apple Watch Series 2", 0}},
        {"Watch2,4", {"Apple Watch Series 2", 0}},
        {"AudioAccessory1,1", {"HomePod", 0}},
        {"iPad1,1", {"iPad", 2010}},
        {"iPad2,1", {"iPad 2", 2011}},
        {"iPad2,2", {"iPad 2", 2011}},
        {"iPad2,3", {"iPad 2", 2011}},
        {"iPad2,4", {"iPad 2", 2011}},
        {"iPad3,1", {"iPad 3", 2012}},
        {"iPad3,2", {"iPad 3", 2012}},
        {"iPad3,3", {"iPad 3", 2012}},
        {"iPad3,4", {"iPad 4", 2012}},
        {"iPad3,5", {"iPad 4", 2012}},
        {"iPad3,6", {"iPad 4", 2012}},
        {"iPad4,1", {"iPad Air", 2013}},
        {"iPad4,2", {"iPad Air", 2013}},
        {"iPad4,3", {"iPad Air", 2013}},
        {"iPad5,3", {"iPad Air 2", 2014}},
        {"iPad5,4", {"iPad Air 2", 2014}},
        {"iPad6,7", {"iPad Pro (12.9)", 2015}},
        {"iPad6,8", {"iPad Pro (12.9)", 2015}},
        {"iPad6,3", {"iPad Pro (9.7)", 2016}},
        {"iPad6,4", {"iPad Pro (9.7)", 2016}},
        {"iPad6,11",{"iPad 2017", 2017}},
        {"iPad6,12",{"iPad 2017", 2017}},
        {"iPad7,1", {"iPad Pro (12.9) 2017", 2017}},
        {"iPad7,2", {"iPad Pro (12.9) 2017", 2017}},
        {"iPad7,3", {"iPad Pro (10.5)", 2017}},
        {"iPad7,4", {"iPad Pro (10.5)", 2017}},
        {"iPad7,5", {"iPad 2018", 2018}},
        {"iPad7,6", {"iPad 2018", 2018}},
        {"iPad2,5", {"iPad Mini", 2011}},
        {"iPad2,6", {"iPad Mini", 2011}},
        {"iPad2,7", {"iPad Mini", 2011}},
        {"iPad4,4", {"iPad Mini 2", 2013}},
        {"iPad4,5", {"iPad Mini 2", 2013}},
        {"iPad4,6", {"iPad Mini 2", 2013}},
        {"iPad4,7", {"iPad Mini 3", 2013}},
        {"iPad4,8", {"iPad Mini 3", 2013}},
        {"iPad4,9", {"iPad Mini 3", 2013}},
        {"iPad5,1", {"iPad Mini 4", 2014}},
        {"iPad5,2", {"iPad Mini 4", 2014}},
        {"iPhone1,1", {"iPhone", 2007}},
        {"iPhone1,2", {"iPhone 3G", 2008}},
        {"iPhone2,1", {"iPhone 3GS", 2009}},
        {"iPhone3,1", {"iPhone 4", 2010}},
        {"iPhone3,2", {"iPhone 4", 2010}},
        {"iPhone3,3", {"iPhone 4", 2010}},
        {"iPhone4,1", {"iPhone 4S", 2011}},
        {"iPhone5,1", {"iPhone 5", 2012}},
        {"iPhone5,2", {"iPhone 5", 2012}},
        {"iPhone5,3", {"iPhone 5C", 2013}},
        {"iPhone5,4", {"iPhone 5C", 2013}},
        {"iPhone6,1", {"iPhone 5S", 2013}},
        {"iPhone6,2", {"iPhone 5S", 2013}},
        {"iPhone7,2", {"iPhone 6", 2014}},
        {"iPhone7,1", {"iPhone 6 Plus", 2014}},
        {"iPhone8,1", {"iPhone 6S", 2015}},
        {"iPhone8,2", {"iPhone 6S Plus", 2015}},
        {"iPhone8,4", {"iPhone SE", 2016}},
        {"iPhone9,1", {"iPhone 7", 2016}},
        {"iPhone9,3", {"iPhone 7", 2016}},
        {"iPhone9,2", {"iPhone 7 Plus", 2016}},
        {"iPhone9,4", {"iPhone 7 Plus", 2016}},
        {"iPhone10,1", {"iPhone 8", 2017}},
        {"iPhone10,4", {"iPhone 8", 2017}},
        {"iPhone10,2", {"iPhone 8 Plus", 2017}},
        {"iPhone10,5", {"iPhone 8 Plus", 2017}},
        {"iPhone10,3", {"iPhone X", 2017}},
        {"iPhone10,6", {"iPhone X", 2017}},
        {"iPhone11,8", {"iPhone XR", 2018}},
        {"iPhone11,2", {"iPhone XS", 2018}},
        {"iPhone11,6", {"iPhone XS Max", 2018}},
        {"iPhone11,4", {"iPhone XS Max", 2018}},
        {"iPod1,1", {"iPod Touch", 2006}},
        {"iPod2,1", {"iPod Touch 2", 2007}},
        {"iPod3,1", {"iPod Touch 3", 2008}},
        {"iPod4,1", {"iPod Touch 4", 2009}},
        {"iPod5,1", {"iPod Touch 5", 2011}},
        {"iPod7,1", {"iPod Touch 6", 2014}},
    };
    
    int PlatformConfigApple::getDeviceYearClass() {
        struct utsname systemInfo;
        uname(&systemInfo);
        auto model = std::string(systemInfo.machine);
        if (IOS_MODEL.count(model) > 0) {
            return IOS_MODEL.at(model).second;
        }
        
        return 2018;
    }
    
    std::string PlatformConfigApple::getDeviceName() {
        struct utsname systemInfo;
        uname(&systemInfo);
        auto model = std::string(systemInfo.machine);
        if (IOS_MODEL.count(model) > 0) {
            return IOS_MODEL.at(model).first;
        }

        return model;
    }
    
    double PlatformConfigApple::getNotchHeight() {
        auto device_name = getDeviceName();
        if (
            device_name == "iPhone X"
            || device_name == "iPhone XS"
            || device_name == "iPhone XR"
            || device_name == "iPhone XS Max"
            || device_name == "iPhone12,1"
            || device_name == "iPhone12,3"
            || device_name == "iPhone12,5"
            || device_name == "iPhone13,2"
            || device_name == "iPhone13,3"
            || device_name == "iPhone13,4"
            || device_name == "iPhone14,2"
            || device_name == "iPhone14,3"
            || device_name == "iPhone14,4"
            || device_name == "iPhone14,5"
            || device_name == "iPhone14,6"
            
            ) {
            return 60;
        }
        
        return 0;
    }

    void PlatformConfigApple::forceDismissKeyboard() {
        [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
    }
    
    int PlatformConfigApple::getDefaultLanguageCode() {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
        
        for (NSString * lang in languages) {
            char code[3]={0};
            NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:lang];
            NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
            [languageCode getCString:code maxLength:3 encoding:NSASCIIStringEncoding];
            code[2]='\0';
            
            auto l = std::string(code);
            if (l == "vi") {
                return 4;
            }
        }
        
        return 1;
    }
    
	void PlatformConfigApple::showVMonkeyPage() {
        StoreViewController * storeView = [StoreViewController new];
        [storeView setDelegate:[StoreHelper sharedHelper]];
        [storeView loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier: @"1388216940"} completionBlock:nil];
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:storeView animated:YES completion:nil];
    }
	
    void PlatformConfigApple::showLearnToReadStorePage() {
        StoreViewController * storeView = [StoreViewController new];
        [storeView setDelegate:[StoreHelper sharedHelper]];
        [storeView loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier: @"930331514"} completionBlock:nil];
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:storeView animated:YES completion:nil];
    }
    void PlatformConfigApple::showStoriesStorePage() {
        StoreViewController * storeView = [StoreViewController new];
        [storeView setDelegate:[StoreHelper sharedHelper]];
        [storeView loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier: @"1236664909"} completionBlock:nil];
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:storeView animated:YES completion:nil];
    }
    void PlatformConfigApple::showMathStorePage() {
        StoreViewController * storeView = [StoreViewController new];
        [storeView setDelegate:[StoreHelper sharedHelper]];
        [storeView loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier: @"1298508851"} completionBlock:nil];
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:storeView animated:YES completion:nil];
    }
    
    bool PlatformConfigApple::canOpenLearnToRead() {
        return [[UIApplication sharedApplication] canOpenURL: [NSURL URLWithString:@"monkeyltr://"]];
    }
    
    void PlatformConfigApple::openLearnToRead() {
        [[UIApplication sharedApplication] openURL: [NSURL URLWithString:@"monkeyltr://"]];
    }
        
    std::string PlatformConfigApple::getPackageName() {
        return [[[NSBundle mainBundle] bundleIdentifier] UTF8String];
    }

    std::string PlatformConfigApple::getAppName() {
        return [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"] UTF8String];
    }
    
    void PlatformConfigApple::shareViral(std::string url){

        bool isInstalled = [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"fb://"]];
        if (isInstalled) {
            FBSDKShareLinkContent *content = [[FBSDKShareLinkContent alloc] init];
            const char * viral_url = url.c_str();
            content.contentURL =[NSURL URLWithString: [NSString stringWithUTF8String:viral_url]];
            content.quote = @"Con mình đang học tiếng Anh trên Monkey Stories và mình thấy rất hữu ích. Cả nhà có thể cho con trải nghiệm miễn phí, tại đây nhé: ";
            [FBSDKShareDialog showFromViewController: [[[UIApplication sharedApplication] keyWindow] rootViewController]
                                         withContent:content
                                            delegate:nil];
        } else {
            auto content = "Con mình đang học tiếng Anh trên Monkey Stories và mình thấy rất hữu ích. Cả nhà có thể cho con trải nghiệm miễn phí, tại đây nhé: ";
            url = StringUtils::format("https://www.facebook.com/dialog/share?app_id=520137134992261&display=popup&href=%s&quote=%s", mj::helper::urlencode(url).c_str(), mj::helper::urlencode(content).c_str());
            cocos2d::Application::getInstance()->openURL(url);
        }

    }
    
    void PlatformConfigApple::shareScreenShot(std::string file_url){
        bool isInstalled = [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"fb://"]];
        if (isInstalled) {
            const char * screenshot_url = file_url.c_str();
            UIImage *image = [UIImage imageNamed: [NSString stringWithUTF8String:screenshot_url]];
            FBSDKSharePhoto *photo = [[FBSDKSharePhoto alloc] init];
            photo.image = image;
            photo.userGenerated = YES;
            FBSDKSharePhotoContent *content = [[FBSDKSharePhotoContent alloc] init];
            content.photos = @[photo];
            
            [FBSDKShareDialog showFromViewController: [[[UIApplication sharedApplication] keyWindow] rootViewController]
                                         withContent:content
                                            delegate:nil];
        } else {
//            file_url = StringUtils::format("https://www.facebook.com/dialog/share?app_id=520137134992261&display=popup&href=%s&quote=%s", mj::helper::urlencode(url).c_str());
//            cocos2d::Application::getInstance()->openURL(url);
        }
    }
    
    void PlatformConfigApple::saveScreenShot(std::string file_url, std::string file_name){
        
        if ([PHPhotoLibrary authorizationStatus] != PHAuthorizationStatusAuthorized) {
            [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
                if (status == PHAuthorizationStatusAuthorized) {
                    // Access has been granted.
                    dispatch_async(dispatch_get_main_queue(), ^{
                        const char * screenshot_url = file_url.c_str();
                        UIImage *image = [UIImage imageNamed: [NSString stringWithUTF8String:screenshot_url]];
                        
                        UIImageWriteToSavedPhotosAlbum(image, [ImageHelper ImageHelper], @selector( thisImage:hasBeenSavedInPhotoAlbumWithError:usingContextInfo:) , nil);
                    });
                } else {
                    // Access has been denied.
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("deny save picture");
                }
            }];
        }else{
            dispatch_async(dispatch_get_main_queue(), ^{
                const char * screenshot_url = file_url.c_str();
                UIImage *image = [UIImage imageNamed: [NSString stringWithUTF8String:screenshot_url]];
                
                UIImageWriteToSavedPhotosAlbum(image, [ImageHelper ImageHelper], @selector( thisImage:hasBeenSavedInPhotoAlbumWithError:usingContextInfo:) , nil);
                });
            }
        }
}



#endif //CC_TARGET_PLATFORM
