//
//  MJPlatformConfig-Mac.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/10/17.
//
//

#include "MJPlatformConfig-Mac.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MacCppBridge.h"
#import <AppKit/AppKit.h>
#include "WebService.h"

@interface MacFullScreenHelper : NSObject {
}
+ (id) sharedHelper;

@end

@implementation MacFullScreenHelper
+ (id)sharedHelper {
    static MacFullScreenHelper * shared_helper = nil;
    @synchronized (self) {
        if (shared_helper == nil) {
            shared_helper = [[self alloc] init];
        }
        
        return shared_helper;
    }
}

- (void)windowDidBecomeFullscreen:(NSNotification *) notification {
    CCLOG("fullscreen mode");
    cocos2d::Director::getInstance()->getOpenGLView()->setFrameSize(1300, 768);
    [[[NSApplication sharedApplication] keyWindow] setAspectRatio:NSMakeSize(1300, 768 + 22)];
}

@end


void mj::PlatformConfigMac::setFullscreen() {
    [[NSApplication sharedApplication] setPresentationOptions:NSApplicationPresentationFullScreen];
    [[[NSApplication sharedApplication] keyWindow] setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    [[[NSApplication sharedApplication] keyWindow] setMinSize:NSMakeSize(800, 600)];
    
    auto menu_bar_height = [[[NSApplication sharedApplication] mainMenu] menuBarHeight];
    
////    [[[NSScreen mainScreen] visibleFrame] size];
//
//    [[[NSApplication sharedApplication] keyWindow] setAspectRatio:[[[NSScreen mainScreen] visibleFrame] size]];
    
    [[NSNotificationCenter defaultCenter] addObserver:[MacFullScreenHelper sharedHelper]
                                             selector:@selector(windowDidBecomeFullscreen:)
                                                 name:NSWindowDidEnterFullScreenNotification object:nil];
    
}

void mj::PlatformConfigMac::clearUserDefaults() {
    [[NSUserDefaults standardUserDefaults] removePersistentDomainForName:[[NSBundle mainBundle] bundleIdentifier]];
    CCLOG("clear user default");
    NSLog(@"%@", [[[NSUserDefaults standardUserDefaults] persistentDomainForName:[[NSBundle mainBundle] bundleIdentifier]] allKeys]);
}

std::string mj::PlatformConfigMac::getUserDefaults() {
 
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *bundleIdentifier = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleIdentifierKey];
    NSDictionary *dict = [defaults persistentDomainForName:bundleIdentifier];
    
    NSError * err;
    NSData * jsonData = [NSJSONSerialization  dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&err];
    NSString * myString = [[NSString alloc] initWithData:jsonData   encoding:NSUTF8StringEncoding];

    for (NSString *key in dict) {
        id value = dict[key];
        NSLog(@"key: %@ - value: %@", key, value);
    }
//    NSLog(@"%@", jsonData);
    
    return [myString UTF8String];
}

void mj::PlatformConfigMac::setUserDefaults(std::string info){
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

std::string mj::PlatformConfigMac::getPlatformDeviceID(){
    auto fake_id = MJDEFAULT->getStringForKey("fake_id", "");
    if (!fake_id.empty()) {
        return fake_id;
    }
    auto mac_address = MAC_CPP_BRIDGE->getMACAddress();
    auto uuid = MAC_CPP_BRIDGE->getSystemUUID();
    return cocos2d::StringUtils::format("%s|%s|null", mac_address.c_str(), uuid.c_str());
}

std::string mj::PlatformConfigMac::getDeviceInfo(){
    std::stringstream ss;
    ss << "App Version: " << WebService::getInstance()->sub_version << std::endl;
    ss << "OS: mac" << std::endl;
    ss << "Model: " << MAC_CPP_BRIDGE->getMacModelName() << std::endl;
    ss << "System Version: " << MAC_CPP_BRIDGE->getOSVersion() << std::endl;
    ss << "Device Identifier: " << getDeviceId()<< std::endl;;
    ss << "Ram: " << cocos2d::StringUtils::format("%0.1f", MAC_CPP_BRIDGE->getRamMemory());
    return ss.str();
}
std::string mj::PlatformConfigMac::getDeviceInfoShort(){
    std::stringstream ss;
    ss << getAppVersion() << "|"
    << MAC_CPP_BRIDGE->getMacModelName() << "|"
    << MAC_CPP_BRIDGE->getOSVersion() << "|"
    << cocos2d::StringUtils::format("%0.1f", MAC_CPP_BRIDGE->getRamMemory()) << "|"
    << getDeviceId();
    return ss.str();
}

std::string mj::PlatformConfigMac::getAppVersion() {
    NSString * version = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"];
    return [version UTF8String];
}

#include "StoryLanguageManager.h"
bool mj::PlatformConfigMac::phoneSupport() {
    cocos2d::Application::getInstance()->openURL("tel:1900636052");
    return true;
}
bool mj::PlatformConfigMac::mailSupport() {
    auto device_info = "Device_info_";
    if (MJDEFAULT->getIntegerForKey(key_lang_display) == 4) {
        device_info = "Thong_tin_thiet_bi_";
    }
    
    std::string content = "Chào bạn, để được hỗ trợ một cách tốt nhất, bạn vui lòng cung cấp cho Monkey Care các thông tin sau: \n"
    "1. Họ và tên:\n"
    "2. SĐT:\n"
    "3. Email: (chỉ áp dụng đối với Khách hàng gửi mail từ địa chỉ @icloud.com)\n"
    "4. Yêu cầu cần hỗ trợ:\n"
    "Monkey Care sẽ kiểm tra các thông tin bạn cung cấp và phản hồi trong thời gian sớm nhất.\n"
    "Trân trọng.\n\n"
    "--------------------\n";
    content += getDeviceInfo();
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == 1) {
        content = "Please provide us with the following information:\n"
        "Your name:\n"
        "Your phone number:\n"
        "Your Email:\n"
        "What do you need help with:\n"
        "We will respond to your request as soon as possible.\n\n"
        "--------------------\n"
        + getDeviceInfo();
    }
    
    auto subject = LANGUAGE_MANAGER->getDisplayLangId() == 1 ? "Need help with " : "Trợ giúp về ";
    
    MAC_CPP_BRIDGE->sendMailSupport(subject + getAppName() + " v" + getAppVersion(), content);
    return true;
}

void mj::PlatformConfigMac::showVMonkeyPage() {
    Application::getInstance()->openURL("https://vmonkey.vn/");
}

void mj::PlatformConfigMac::showLearnToReadStorePage() {
    Application::getInstance()->openURL("https://monkeyjunior.vn");
}

void mj::PlatformConfigMac::showStoriesStorePage() {
    Application::getInstance()->openURL("https://monkeystories.vn");
}

void mj::PlatformConfigMac::showMathStorePage() {
    Application::getInstance()->openURL("https://monkeymath.vn");
}

std::string mj::PlatformConfigMac::getPackageName() {
    return [[[NSBundle mainBundle] bundleIdentifier] UTF8String];
}

std::string mj::PlatformConfigMac::getAppName() {
    return [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"] UTF8String];
}

bool mj::PlatformConfigMac::canOpenLearnToRead() {
    NSString *path = [[NSWorkspace sharedWorkspace] fullPathForApplication:@"Monkey Junior"];
    BOOL isAppInstalled = (nil != path);
    return isAppInstalled;
}

void mj::PlatformConfigMac::openLearnToRead() {
    NSString *path = [[NSWorkspace sharedWorkspace] fullPathForApplication:@"Monkey Junior"];
    [[NSWorkspace sharedWorkspace] launchApplication:path];
}

void mj::PlatformConfigMac::shareViral(std::string url){
    [[NSPasteboard generalPasteboard] clearContents];
    [[NSPasteboard generalPasteboard] setString:[NSString stringWithUTF8String:url.c_str()] forType:NSStringPboardType];
}

void mj::PlatformConfigMac::saveScreenShot(std::string url, std::string file_name){
    
    dispatch_async(dispatch_get_main_queue(), ^{
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("save picture");
    });
    //move ảnh ra folder dễ tìm hơn
//    NSFileManager *manager = [NSFileManager defaultManager];
//    NSError *error;
//    url = "file://" + url;
//    NSURL *oldURL = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
//    auto desktopURL = "/Downloads/" + file_name;
//    NSURL *newURL = [NSURL fileURLWithPath:[NSHomeDirectory()stringByAppendingPathComponent:[NSString stringWithUTF8String:desktopURL.c_str()]]];
//    if([manager moveItemAtURL:oldURL toURL:newURL error:&error]){
//        NSLog(@"Move Success");
//
//    }else{
//        NSLog(@"Move error: %@", error);
//    }
}
#endif
