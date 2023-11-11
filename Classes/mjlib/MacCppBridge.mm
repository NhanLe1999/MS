#include "MacCppBridge.h"
#import "MacBridge.h"
#include "GetPrimaryMACAddress.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#import "Reachability.h"
#import "MJInApp.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#import <Cocoa/Cocoa.h>
#import <IOKit/IOKitLib.h>
#import <sys/sysctl.h>
#include <sys/types.h>
#include "StoryConfigManager.h"
//#import <Fabric/Fabric.h>
//#import <Crashlytics/Crashlytics.h>

using namespace std;
using namespace cocos2d;
USING_NS_CC;
static MacCppBridge *shareInstant = NULL;
MacCppBridge* MacCppBridge::getInstance(){
    if (shareInstant == NULL) {
        shareInstant = new MacCppBridge();
    }
    return shareInstant;
}
MacCppBridge::MacCppBridge(){
    
}

void MacCppBridge::onMacCppBridgeTest(){
    //[MAC_BRIDGE testMacBridge];
}

std::string MacCppBridge::getMACAddress(){
    return MacOSInfo::MacOSInfo::hsmacAddress();
}

std::string MacCppBridge::getOSXSystemInfo(){
//    NSString *foo = [MAC_BRIDGE getSystemInfo];
//    std::string systemInfo = std::string([foo UTF8String]);
//   return systemInfo;
    return "";
}

void MacCppBridge::playVideo(std::string fileName, int idCallVideo, bool autoLoop){
    //NSString *pathVideo = [NSString stringWithCString:fileName.c_str()
    //                                            encoding:[NSString defaultCStringEncoding]];
    //[MAC_BRIDGE playVideo:pathVideo andIdCallVideo:idCallVideo withLoop:autoLoop];
}

void MacCppBridge::setWindowWantsLayer(){
    //[MAC_BRIDGE setWindowWantsLayer];
}

cocos2d::Size MacCppBridge::getScreenSize(){
    int width = [[NSScreen mainScreen] frame].size.width;
    int height = [[NSScreen mainScreen] frame].size.height;
    NSLog(@"HSLog - ScreenSize = %d x %d", width, height);
    return cocos2d::Size(width, height);
}

void MacCppBridge::removeVideoMAC(){
    //[MAC_BRIDGE removeVideoMac];
}

void MacCppBridge::startEventKeyBoardWithID(int index){
    [MAC_BRIDGE startEventKeyBoardWithID:index];
}

void MacCppBridge::removeUserDefault(){
    NSString *appDomain = [[NSBundle mainBundle] bundleIdentifier];
    [[NSUserDefaults standardUserDefaults] removePersistentDomainForName:appDomain];
}

void MacCppBridge::createContainerDataPath(){
//    NSString* bundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSString* app_name = @"MonkeyStories";
    if(CONFIG_MANAGER->isVietnameseStories()){
        app_name = @"VMonkey";
    }

    NSFileManager* fileManager = [[NSFileManager alloc] init];
    NSArray* urlPaths = [fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    NSURL *userPath = [urlPaths objectAtIndex:0];
    NSURL* appDirectory = [userPath URLByAppendingPathComponent:app_name isDirectory:YES];
    NSString *abc = [NSString stringWithFormat:@"%@", appDirectory];
    abc = [abc stringByReplacingOccurrencesOfString:@"%20" withString:@" "];
    NSString *prefix = @"file://";
    NSRange needleRange = NSMakeRange(prefix.length,
                                      abc.length - prefix.length);
    NSString *needle = [abc substringWithRange:needleRange];
    NSLog(@"path %@", needle);
    
    //TODO: handle the error
    if (![fileManager fileExistsAtPath:[appDirectory path]]) {
        [fileManager createDirectoryAtURL:appDirectory withIntermediateDirectories:NO attributes:nil error:nil];
        NSLog(@"path %@", appDirectory);
    }
    NSLog(@"path 2 %@", appDirectory);
 
    FileUtils::getInstance()->setWritablePath(std::string([needle UTF8String]));
}

bool MacCppBridge::createMacPathFileByString(std::string strPath){
    NSFileManager* fileManager = [[NSFileManager alloc] init];
    NSString *newPath = [NSString stringWithCString:strPath.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *pathDirectory = [NSString stringWithFormat:@"%@/%@", @"file://",newPath];
    NSURL* appDirectory = [NSURL URLWithString:pathDirectory];
    if (![fileManager fileExistsAtPath:[appDirectory path]]) {
        [fileManager createDirectoryAtURL:appDirectory withIntermediateDirectories:NO attributes:nil error:nil];
        if (![fileManager fileExistsAtPath:[appDirectory path]]) {
            return false;
        }
    }
    return true;
}

std::string MacCppBridge::getDocumentPath(){
    //Khong dung den nua!
    NSString* app_name = @"MonkeyStories";
   if(CONFIG_MANAGER->isVietnameseStories()){
       app_name = @"VMonkey";
   }
    
    NSFileManager* fileManager = [[NSFileManager alloc] init];
    NSArray* urlPaths = [fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    NSURL *userPath = [urlPaths objectAtIndex:0];
    NSURL* appDirectory = [userPath URLByAppendingPathComponent:app_name isDirectory:YES];
    NSString *documentPath = [NSString stringWithFormat:@"%@", appDirectory];
    documentPath = [documentPath stringByReplacingOccurrencesOfString:@"%20" withString:@" "];
    return std::string([documentPath UTF8String]);
}


bool MacCppBridge::isInternetConnection(){
    Reachability *networkReachability = [Reachability reachabilityForInternetConnection];
    NetworkStatus networkStatus = [networkReachability currentReachabilityStatus];
    if (networkStatus == NotReachable) {
        return false;
    } else {
        return true;
    }
    
    return true;
}

void MacCppBridge::loadAllItemPackageIAP(){
    //[MJInAppInstant loadAllPackage];
}

void MacCppBridge::buyPackgeViaIAP(std::string indexPackageStr){
    /*indexPackageStr = std::string("mac.").append(indexPackageStr);
    MJInAppInstant.namePackage = [NSString stringWithCString:indexPackageStr.c_str() encoding:[NSString defaultCStringEncoding]];  
    
    MJInAppInstant.indexofpackage = [MJInAppInstant getIndexPackage];
    NSLog(@"indexofpackage = %d", MJInAppInstant.indexofpackage);
    if (MJInAppInstant.indexofpackage == -1) {
        Scene *scene = Director::getInstance()->getRunningScene();
        BuyScene* buy_scene = dynamic_cast<BuyScene*>(scene->getChildByTag(300));
        buy_scene->showAlertCantBuyIAP();
        return;
    }

    [MJInAppInstant startBuyItemWithType];*/
        //[cWindow setLevel: NSStatusWindowLevel]
    
    
//    cocos2d::Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
//        [[NSRunningApplication currentApplication] hide];
//    }), nil));
    
    
    //displayIndicator(true);
//    NSAlert *alert = [NSAlert alertWithMessageText:@"Alert" defaultButton:@"Ok" alternateButton:@"Cancel" otherButton:nil informativeTextWithFormat:@"Alert pop up displayed"];
//    [alert runModal];
}

void MacCppBridge::restorePackageIAP(){
    /*[MJInAppInstant restorePackageIAP];
    cocos2d::Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
        [[NSRunningApplication currentApplication] hide];
    }), nil));
     */
}

std::string cchar2hex(char dec) {
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);
    if (0 <= dig1 && dig1 <= 9) dig1 += 48;    //0,48 in ascii
    if (10 <= dig1 && dig1 <= 15) dig1 += 65 - 10; //A,65 in ascii
    if (0 <= dig2 && dig2 <= 9) dig2 += 48;
    if (10 <= dig2 && dig2 <= 15) dig2 += 65 - 10;
    
    std::string r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}

std::string curlencode(const std::string &c) {
    std::string escaped;
    int max = (int) c.length();
    for (auto i = 0; i < max; i++) {
        if (('0' <= c[i] && c[i] <= '9') ||//0-9
            ('A' <= c[i] && c[i] <= 'Z') ||//ABC...XYZ
            ('a' <= c[i] && c[i] <= 'z') || //abc...xyz
            (c[i] == '~' || c[i] == '-' || c[i] == '_' || c[i] == '.')) {
            escaped.append(&c[i], 1);
        } else {
            escaped.append("%");
            escaped.append(cchar2hex(c[i]));//converts char 255 to string "FF"
        }
    }
    return escaped;
}

void MacCppBridge::sendMailSupport(std::string subjectStr, std::string contentStr){
    subjectStr = curlencode(subjectStr);
    contentStr = curlencode(contentStr);
    
    NSString *toAddress, *subject, *bodyText;
    toAddress = @"monkeyxinchao@monkey.edu.vn";
//    subjectStr.append("_");
//    subjectStr.append(contentStr);
    subject = [NSString stringWithCString:subjectStr.c_str()
                                                          encoding:[NSString defaultCStringEncoding]];
    subject = [subject stringByReplacingOccurrencesOfString:@"|" withString:@"_"];
    
    bodyText = [NSString stringWithCString:contentStr.c_str()
                                  encoding:[NSString defaultCStringEncoding]];
    bodyText = [bodyText stringByReplacingOccurrencesOfString:@"|" withString:@"_"];

    
    NSString *mailtoAddress = [[NSString stringWithFormat:@"mailto:%@?Subject=%@&body=%@",toAddress,subject,bodyText] stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:mailtoAddress]];
    NSLog(@"Mailto:%@",mailtoAddress);
}

void MacCppBridge::displayIndicator(bool isDisplay){
    if (!isDisplay) {
        [[NSRunningApplication currentApplication] unhide];
    }
//    if (isDisplay) {
//        cocos2d::ui::Layout *panel = static_cast<ui::Layout *>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName("panelindicatorIAP"));
//        if (!panel) {
//            panel = cocos2d::ui::Layout::create();
//            panel->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//            panel->setBackGroundColor(cocos2d::Color3B::WHITE);
//            panel->setOpacity(230);
//            panel->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
//            panel->setTouchEnabled(false);
//            panel->setName(cocos2d::StringUtils::format("panelindicatorIAP"));
//            cocos2d::Director::getInstance()->getRunningScene()->addChild(panel, 1000000);
//            auto indicator = ui::ImageView::create("install/Indicator.png");
//            indicator->setPosition(Vec2(cocos2d::Director::getInstance()->getVisibleSize().width/2, cocos2d::Director::getInstance()->getVisibleSize().height/2));
//            panel->addChild(indicator);
//            indicator->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));
//        }
//    }else{
//        cocos2d::ui::Layout *panel = static_cast<ui::Layout *>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName("panelindicatorIAP"));
//        if (panel) {
//            panel->removeFromParent();
//        }
//    }
}


bool MacCppBridge::osXlessthan10_9(){
    return [MAC_BRIDGE osXLessThan10_9];
}

std::string MacCppBridge::getSystemUUID(){
    return std::string([[MAC_BRIDGE getSystemUUID] UTF8String]);
}
std::string MacCppBridge::getMacModelName(){
    return std::string([[MAC_BRIDGE machineModel] UTF8String]);
}
std::string MacCppBridge::getOSVersion(){
    return std::string([[MAC_BRIDGE versionOSXString] UTF8String]);
}
float MacCppBridge::getRamMemory(){
    return [MAC_BRIDGE getRamMemory];
}

void MacCppBridge::fabricInit(){
    //[Fabric with:@[[Crashlytics class]]];
}

void MacCppBridge::setCrashlyticsKeyValue(std::string key, std::string value){
//    [[Crashlytics sharedInstance] setObjectValue:[NSString stringWithCString:value.c_str()
//                                                                    encoding:[NSString defaultCStringEncoding]] forKey:[NSString stringWithCString:key.c_str()
//                                                                                                                                          encoding:[NSString defaultCStringEncoding]]];
//    
    //$(VC_IncludePath);$(WindowsSdk_71A_IncludePath);C:\MJStories\Classes\base_include;C:\MJStories\Classes\mjlib;C:\MJStories\Classes\lib;C:\MJStories\Classes\lib\az;C:\MJStories\Classes\utf8;C:\MJStories\Classes\story\homescene;C:\MJStories\Classes\story\popup;\MJStories\Classes\story\homescene\subscriptions;;C:\MJStories\Classes\story;C:\MJStories\Classes\games;C:\MJStories\Classes\games\build_sentence;C:\MJStories\Classes\games\chickens_to_coop;C:\MJStories\Classes\games\jumping_frog;C:\MJStories\Classes\games\land_the_plane;C:\MJStories\Classes\games\make_a_match;C:\MJStories\Classes\games\matching;C:\MJStories\Classes\games\plug_the_bulbs;C:\MJStories\Classes\games\qa;C:\MJStories\Classes\games\sing_birdie;C:\MJStories\Classes\games\word_machine;C:\MJStories\Classes\activity;C:\MJStories\Classes\games\shark;C:\MJStories\Classes\games\phonics;C:\MJStories\Classes\games\typing;
}

