//
//  MJHelper-Apple.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#include "MJHelper-Apple.h"
#include "MJDefault.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "AppController.h"
#import "MsBackUpDownloadIos.h"
#import "MsBackUpDownloadIosThumb.h"
#import "PageMenuControl.h"

#import <Speech/Speech.h>
#import "MJStory-Swift.h"

static MJHelperSwitf *s_obj  = nullptr;
static MJHelperSwitf* swiftInstance = [[MJHelperSwitf alloc] init];

namespace mj {
    namespace helper {
    
        void stopSpeechToText(std::function<void(std::string)> updateCallback) {
            
            if(!s_obj)  {
                s_obj = [[MJHelperSwitf alloc] init];
            }
            
            [s_obj stopRecording:^(NSString * _Nullable text) {
                
                if(text) {
                    
                    std::string stdString([text UTF8String]);
                    
                    if(updateCallback) {
                        updateCallback(stdString);
                    }
                }
            }];

        }
    
        void startSpeechToText(std::function<void(std::string)> updateCallback) {
            
            if(!s_obj)  {
                s_obj = [[MJHelperSwitf alloc] init];
            }
            
            [s_obj setupSpeech];
            
            [s_obj startRecording:^(NSString * _Nullable text) {
                
                if(text) {
                    
                    std::string stdString([text UTF8String]);
                    
                    if(updateCallback) {
                        updateCallback(stdString);
                    }
                }
            }];

        }
        void showATTrackingNotification(){
            if(!s_obj)  {
                s_obj = [[MJHelperSwitf alloc] init];
            }
            [s_obj ShowATTrackingNotification];
        }
        void setOrientationPortrait(bool isPortrait) {
            if (isPortrait) {
                [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
            }
        }

        void shareStory() {
            if (@available(iOS 14, *)) {
                sharing("http://monkeystories.net", "");
            }
            else
            {
                UIActivityViewController * share_vc = [[UIActivityViewController alloc] initWithActivityItems:@[[NSURL URLWithString:@"http://monkeystories.net"]] applicationActivities:nil];
                [[share_vc popoverPresentationController] setSourceView:[[[[UIApplication sharedApplication] keyWindow] rootViewController] view]];
                [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:share_vc animated:YES completion:nil];
            }
        }
    
        void sharing(std::string msg,std::string imagePath)
        {
            NSString *imgPath = [NSString stringWithCString:imagePath.c_str()
                                               encoding:[NSString defaultCStringEncoding]];
            NSString *msgNs = [NSString stringWithCString:msg.c_str()
                                               encoding:[NSString defaultCStringEncoding]];
            [swiftInstance showIntentWithMessage:msgNs imgPath:imgPath onShareDoneCallback:^(BOOL isSuccess) {
                if(auto scene = utils::findChild<PageMenuControl*>(Director::getInstance()->getRunningScene(), "parent_report")){
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                        scene->onShareFacebookCallback(isSuccess);
                    });
                }
            }];
        }
        
        void logLanguages() {
            NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
            NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
            
            for (NSString * lang in languages) {
                NSLog(@"Language: %@", lang);
            }
        }
        
        void openDeviceAppSettings_ios() {
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
        }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        void changeIconAPP()
        {
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
        }
    
        std::string getTextFiled()
        {
            AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
            NSString * text =[app getTextFiled];
            std::string value = std::string([text UTF8String]);
            return value;
        }
        
        void AddTextFiled()
        {
            AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
           [app AddTextFiled];
        }
    
        void connectUrl()
        {
            AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
           [app connectUrl];
        }
    
        void  setBackGroundColorForTextFiled(bool isColor)
        {
            AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
           [app setBackGroundColorForTextFiled:isColor];
        }
    
        void pushEventLostConnecttionToClevertab(std::string link, std::string errorType, std::string type, std::string errorcode, std::string countRetry, std::string ipAndspeed)
        {
            NSString * idlink = [[NSString alloc] initWithUTF8String:link.c_str()];
            NSString * iderrorType = [[NSString alloc] initWithUTF8String:errorType.c_str()];
            NSString * idtype = [[NSString alloc] initWithUTF8String:type.c_str()];
            NSString * iderrorcode = [[NSString alloc] initWithUTF8String:errorcode.c_str()];
            NSString * idcountRetry = [[NSString alloc] initWithUTF8String:countRetry.c_str()];
            NSString * idipAndspeed = [[NSString alloc] initWithUTF8String:ipAndspeed.c_str()];
            MsBackUpDownloadIos * obj = [[MsBackUpDownloadIos alloc] init];
           [obj pushEventLostConnectionToClevertab:idlink andString:iderrorType andString:idtype andString:iderrorcode andString:idcountRetry andString:idipAndspeed];
            
        }

        void deleteTextFiled()
        {
            AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
           [app deleteTextFiled];
        }
    
        float getDiskSpaceIOS()
        {
            AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
            return [app getFreeDiskspace];
        }
    
        void beginDownloadIos(std::string url, std::string pathFile, bool isDownloadThumb)
        {
                
            NSString * linkUrl = [[NSString alloc] initWithUTF8String:url.c_str()];
            NSString * filePath = [[NSString alloc] initWithUTF8String:pathFile.c_str()];
            NSURL * urlDownload = [[NSURL alloc] initWithString:linkUrl];
            
            if(isDownloadThumb)
            {
                MsBackUpDownloadIosThumb * obj = [[MsBackUpDownloadIosThumb alloc] init];

                [obj setInfoDownload:urlDownload andString:filePath];
            }else{
                MsBackUpDownloadIos * obj = [[MsBackUpDownloadIos alloc] init];

                [obj setInfoDownload:urlDownload andString:filePath];
            }
        }
    
        void stopConnection(bool isDownloadThumb)
        {
            if(isDownloadThumb)
            {
                MsBackUpDownloadIosThumb * obj = [[MsBackUpDownloadIosThumb alloc] init];
                [obj stopConnection];
            }else{
                MsBackUpDownloadIos * obj = [[MsBackUpDownloadIos alloc] init];
                [obj stopConnection];
            }
        }
    
        std::string getTimeDate()
        {
            auto time_local = (int)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto current = MJDEFAULT->getIntegerForKey(key_time_curent, time_local);
            
            NSDate *date = [NSDate dateWithTimeIntervalSince1970:current];
            NSDateFormatter *dateformatter=[[NSDateFormatter alloc]init];
            [dateformatter setDateFormat:@"dd-MM-yyyy"];
            NSString *dateString=[dateformatter stringFromDate:date];
            
            const char *command = [dateString UTF8String];
            std::string str(command);
            
            
            return command;
        }
#endif
    }
}

#endif
