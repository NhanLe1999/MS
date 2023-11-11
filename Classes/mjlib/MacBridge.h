//
//  MacBridge.h
//  MY VIRTUAL PETS
//
//  Created by HoiSung on 9/27/14.
//  Copyright (c) 2014 nhocitpro. All rights reserved.
//

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#import <Foundation/Foundation.h>
#define MAC_BRIDGE [MacBridge shareIntance]
@interface MacBridge : NSObject
{
    id keyMonitorDown, keyMonitorUp;
    bool isKeyboard;
    int screenKeyboardIndex;
}
@property (strong) NSString *pathFileVideo;
@property (assign) int id_CallVideo;
@property (assign) bool isAutoLoop;
+ (MacBridge *)shareIntance;
-(void) testMacBridge;
- (NSString *)getSystemInfo;
- (void) setWindowWantsLayer;
-(void) startEventKeyBoardWithID:(int) index;
- (bool) osXLessThan10_9;

- (NSString *)getSystemUUID;
-(NSString *) machineModel;
- (NSString *)versionOSXString;
-(float) getRamMemory;
@end

#endif
