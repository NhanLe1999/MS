//
//  MacBridge.m
//  MY VIRTUAL PETS
//
//  Created by HoiSung on 9/27/14.
//  Copyright (c) 2014 nhocitpro. All rights reserved.
//

#import "MacBridge.h"
#import <Cocoa/Cocoa.h>

#import <IOKit/IOKitLib.h>
#import <sys/sysctl.h>
#include <sys/types.h>

#include "HSLib.h"
@interface MacBridge(){
}

@end
static MacBridge *shareIntance;
@implementation MacBridge
@synthesize id_CallVideo, pathFileVideo, isAutoLoop;
+ (MacBridge *)shareIntance{
    if (!shareIntance) {
        shareIntance = [[[self class] alloc] init];
        
    }
    return shareIntance;
}

- (id)init{
    if (self = [super init]) {
        keyMonitorDown = 0;
        keyMonitorUp = 0;
        isKeyboard = NO;
        screenKeyboardIndex = 0;
        id_CallVideo = 0;
        isAutoLoop = NO;
    }
    return self;
}

-(void) testMacBridge{
    //[[[NSApplication sharedApplication] dockTile] setBadgeLabel:@"2234"];
    

}
- (bool) osXLessThan10_9{
    NSString *versionOSXString = [[NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"] objectForKey:@"ProductVersion"];
    NSArray *arr = [versionOSXString componentsSeparatedByString:@"."];
    int a2 = (int)[[arr objectAtIndex:1] integerValue];
    if (a2 < 10) {
        return true;
    }
    return false;
}

- (NSString *)getSystemInfo {
    NSString *versionOSXString = [[NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"] objectForKey:@"ProductVersion"];
    
    NSProcessInfo* pinfo = [NSProcessInfo processInfo];
    float ram = (float)([pinfo physicalMemory]*1.0/(1024*1024*1024));
    
    NSString *systemInfo = [NSString stringWithFormat:@"ModelID:%@|OsX_Version:%@|Ram:%0.1f", [self machineModel], versionOSXString, ram];
    return systemInfo;
}

- (NSString *)versionOSXString{
    return [[NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"] objectForKey:@"ProductVersion"];
}

-(float) getRamMemory{
    NSProcessInfo* pinfo = [NSProcessInfo processInfo];
    return (float)([pinfo physicalMemory]*1.0/(1024*1024*1024));
}

- (NSString *)getSystemUUID {
    io_service_t platformExpert = IOServiceGetMatchingService(kIOMasterPortDefault,IOServiceMatching("IOPlatformExpertDevice"));
    if (!platformExpert)
        return nil;
    
    CFTypeRef serialNumberAsCFString = IORegistryEntryCreateCFProperty(platformExpert,CFSTR(kIOPlatformUUIDKey),kCFAllocatorDefault, 0);
    IOObjectRelease(platformExpert);
    if (!serialNumberAsCFString)
        return nil;
    
    return (__bridge NSString *)(serialNumberAsCFString);
}
-(NSString *) machineModel
{
    size_t len = 0;
    sysctlbyname("hw.model", NULL, &len, NULL, 0);
    
    if (len)
    {
        char *model = (char *)malloc(len*sizeof(char));
        sysctlbyname("hw.model", model, &len, NULL, 0);
        NSString *model_ns = [NSString stringWithUTF8String:model];
        free(model);
        return model_ns;//model name macbook
    }
    
    return @"N/A"; //incase model name can't be read
}

- (void) setWindowWantsLayer {
    if (![self osXLessThan10_9]) {
        NSWindow *window = [[NSApplication sharedApplication] keyWindow];
        [window.contentView setWantsLayer:YES];
        CGRect frame = [[NSScreen mainScreen] visibleFrame];
        frame.origin.x = frame.origin.x+5;
        frame.origin.y = [[NSScreen mainScreen] visibleFrame].size.height;
        [window setFrameOrigin:frame.origin];
        cocos2d::Director::getInstance()->getRunningScene()->schedule([=](float){
            [self setReturnWindowPosition];
        }, 0.05f, "delaySetWindowWantsLayer");
    }
//
//    NSString *userPath = NSHomeDirectory();
//    auto strRet = std::string([userPath UTF8String]);
//    CCLOG("HoiSung = %s", strRet.c_str());
}

-(void) setReturnWindowPosition{
    NSWindow *window = [[NSApplication sharedApplication] keyWindow];
     CGRect frame = [[NSScreen mainScreen] visibleFrame];
    [window setFrameOrigin:frame.origin];
    window = nil;
}
#pragma mark - KEY BOARD
#include "MJDefinitions.h"
-(void) startEventKeyBoardWithID:(int) index{
    if (keyMonitorDown) {
        [NSEvent removeMonitor:keyMonitorDown];
        keyMonitorDown = nil;
        
        [NSEvent removeMonitor:keyMonitorUp];
        keyMonitorUp = nil;
    }
    screenKeyboardIndex = index;
    keyMonitorDown = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyDownMask handler:^(NSEvent *event) {
        if (event.type == NSKeyDown) {
            NSLog(@"DOWN");
        }
        
        if (isKeyboard) return event;
        isKeyboard = YES;
        unichar character = [[event characters] characterAtIndex:0];
        if (screenKeyboardIndex == 5) {
            if (character == NSLeftArrowFunctionKey)
            {
                CCLOG("VK_LEFT NULLLLLLLLL");
                XHSLib->keyboard_index = 2;
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.keyboard_page");
                
            }
            if (character == NSRightArrowFunctionKey)
            {
                CCLOG("VK_RIGHT NULLLLLLLLL");
                XHSLib->keyboard_index = 1;
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.keyboard_page");

            
            }
            if (character == NSUpArrowFunctionKey || character == NSDownArrowFunctionKey)
            {
                XHSLib->keyboard_index = 3;
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.keyboard_page");
                CCLOG("VK_DOWN NULLLLLLLLL");
            }
        }
        else if(screenKeyboardIndex == 6){
            if (event.keyCode == 53)
            {
                CCLOG("ESC");
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_even_quit_app_with_esc_keyboard);
            }
        }
        
        return event;
    }];
    
    keyMonitorUp = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyUpMask handler:^(NSEvent *event) {
        if (event.type == NSKeyUp) {
            NSLog(@"UP");
            isKeyboard = false;
        }
        return event;
    }];

}
@end
