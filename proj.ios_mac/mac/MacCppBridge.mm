#include "MacCppBridge.h"
//#import "MacBridge.h"
//#include "GetPrimaryMACAddress.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#import "Reachability.h"
//#import "MJInApp.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace std;
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
    //return MacOSInfo::MacOSInfo::hsmacAddress();
    return "";
}

std::string MacCppBridge::getOSXSystemInfo(){
//    NSString *foo = [MAC_BRIDGE getSystemInfo];
//    std::string systemInfo = std::string([foo UTF8String]);
//    return systemInfo;
    return "";
}

void MacCppBridge::playVideo(std::string fileName, int idCallVideo, bool autoLoop){
    NSString *pathVideo = [NSString stringWithCString:fileName.c_str()
                                                encoding:[NSString defaultCStringEncoding]];
    //[MAC_BRIDGE playVideo:pathVideo andIdCallVideo:idCallVideo withLoop:autoLoop];
}

void MacCppBridge::setWindowWantsLayer(){
    //[MAC_BRIDGE setWindowWantsLayer];
}

cocos2d::Size MacCppBridge::getScreenSize(){
//    int width = [[NSScreen mainScreen] frame].size.width;
//    int height = [[NSScreen mainScreen] frame].size.height;
//    NSLog(@"HSLog - ScreenSize = %d x %d", width, height);
//    return cocos2d::Size(width, height);
}

void MacCppBridge::removeVideoMAC(){
    //[MAC_BRIDGE removeVideoMac];
}

void MacCppBridge::startEventKeyBoardWithID(int index){
    //[MAC_BRIDGE startEventKeyBoardWithID:index];
}

void MacCppBridge::removeUserDefault(){
    NSString *appDomain = [[NSBundle mainBundle] bundleIdentifier];
    [[NSUserDefaults standardUserDefaults] removePersistentDomainForName:appDomain];
}

void MacCppBridge::createContainerDataPath(){
//    NSString* bundleID = [[NSBundle mainBundle] bundleIdentifier];
//
//    NSFileManager* fileManager = [[NSFileManager alloc] init];
//    NSArray* urlPaths = [fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
//    NSURL *userPath = [urlPaths objectAtIndex:0];
//    NSURL* appDirectory = [userPath URLByAppendingPathComponent:@"MonkeyJunior" isDirectory:YES];
//    NSString *abc = [NSString stringWithFormat:@"%@", appDirectory];
//    NSLog(@"path %@", abc);
//    //TODO: handle the error
//    if (![fileManager fileExistsAtPath:[appDirectory path]]) {
//        [fileManager createDirectoryAtURL:appDirectory withIntermediateDirectories:NO attributes:nil error:nil];
//        NSLog(@"path %@", appDirectory);
//    }
//    NSLog(@"path 2 %@", appDirectory);
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
    NSFileManager* fileManager = [[NSFileManager alloc] init];
    NSArray* urlPaths = [fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    NSURL *userPath = [urlPaths objectAtIndex:0];
    NSURL* appDirectory = [userPath URLByAppendingPathComponent:@"MonkeyJunior" isDirectory:YES];
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
}

void MacCppBridge::loadAllItemPackageIAP(){
    //[MJInAppInstant loadAllPackage];
}

void MacCppBridge::buyPackgeViaIAP(std::string indexPackageStr){
//    indexPackageStr = std::string("mac.").append(indexPackageStr);
//    MJInAppInstant.namePackage = [NSString stringWithCString:indexPackageStr.c_str() encoding:[NSString defaultCStringEncoding]];
//    
//    [MJInAppInstant startBuyItemWithType];
//        //[cWindow setLevel: NSStatusWindowLevel]
//    
//    
//    cocos2d::Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
//        [[NSRunningApplication currentApplication] hide];
//    }), nil));
//    
//    
//    //displayIndicator(true);
////    NSAlert *alert = [NSAlert alertWithMessageText:@"Alert" defaultButton:@"Ok" alternateButton:@"Cancel" otherButton:nil informativeTextWithFormat:@"Alert pop up displayed"];
////    [alert runModal];
}

void MacCppBridge::restorePackageIAP(){
//    [MJInAppInstant restorePackageIAP];
//    cocos2d::Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
//        [[NSRunningApplication currentApplication] hide];
//    }), nil));
}

void MacCppBridge::sendMailSupport(std::string subjectStr, std::string contentStr){
//    NSString *toAddress, *subject, *bodyText;
//    toAddress = @"support@monkeyjunior.com";
//    subjectStr.append("_");
//    subjectStr.append(contentStr);
//    subject = [NSString stringWithCString:subjectStr.c_str()
//                                                          encoding:[NSString defaultCStringEncoding]];
//    subject = [subject stringByReplacingOccurrencesOfString:@"|" withString:@"_"];
//    
//    bodyText = [NSString stringWithCString:contentStr.c_str()
//                                  encoding:[NSString defaultCStringEncoding]];
//    bodyText = [bodyText stringByReplacingOccurrencesOfString:@"|" withString:@"_"];
//
//    
//    NSString *mailtoAddress = [[NSString stringWithFormat:@"mailto:%@?Subject=%@&body=%@",toAddress,subject,bodyText] stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
//    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:mailtoAddress]];
//    NSLog(@"Mailto:%@",mailtoAddress);
}

void MacCppBridge::displayIndicator(bool isDisplay){
//    if (!isDisplay) {
//        [[NSRunningApplication currentApplication] unhide];
//    }
}


bool MacCppBridge::osXlessthan10_9(){
    //return [MAC_BRIDGE osXLessThan10_9];
}

int MacCppBridge::getIDCallVideo(){
    //return MAC_BRIDGE.id_CallVideo;
}


