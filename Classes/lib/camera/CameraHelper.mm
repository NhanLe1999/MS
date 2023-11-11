//
//  CameraHelper.m
//  MJStory
//
//  Created by Thien Phuoc on 09/03/2022.
//

#include "CameraHelper.h"
#include "audio/include/AudioEngine.h"
#include "AISpeakingGame/popups/MSAISpeakingSharePopup.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
#import <Foundation/Foundation.h>
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <ReplayKit/ReplayKit.h>
#endif
#import "MJStory-Swift.h"

#import "AppController.h"
#include "platform/ios/CCEAGLView-ios.h"
#include "MJHelper.h"
//#include <ffmpegkit/FFmpegKit.h>
//#include <ffmpegkit/FFmpegKitConfig.h>
//#include <ffmpegkit/FFprobeKit.h>
#import <AVFoundation/AVFoundation.h>

static CameraHelperSwift *s_obj  = nullptr;

void CameraHelper::addViewToCamera(cocos2d::Vec2 postion,cocos2d::Size size)
{
    AVAudioSession *session = [AVAudioSession sharedInstance];

    auto currentSession = [session category];

    if(currentSession != AVAudioSessionCategoryPlayAndRecord)
    {
        NSError *error = NULL;

        BOOL success =   [session setCategory:AVAudioSessionCategoryPlayAndRecord
                                         mode:AVAudioSessionModeDefault
                                      options:AVAudioSessionCategoryOptionMixWithOthers
                                        error:&error];

        [session setActive:true error:nil];

        if (error)
        {
            auto errorCodeInternal  = (int)error.code;
            auto errorString        = [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding];
        }
    }

   auto wriablePath = FileUtils::getInstance()->getWritablePath();

   CCLOG("AI Speaking:%s" , wriablePath.c_str());

   AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];

   UIWindow* windows = [app getUIWindow];
    
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
   auto convertedRect = convertCocosPositionToIOSScreenPos(cocos2d::Rect(postion,size));
    
   [s_obj addCameraToViewWithX:convertedRect.origin.x y:convertedRect.origin.y width:convertedRect.size.width heigh:convertedRect.size.height parent:windows];
}

void CameraHelper::saveVideoToFile(std::vector<std::string> videoPaths,std::string cmd, std::string outputPath,float video01Duration, std::function<void(bool, std::string)> callback)
{
    std::string videoInputPathsStr = "";
    
    // commbine inputs path to a string
    for (int i=0; i< videoPaths.size() ; i++) {
        
        std::string  path = videoPaths.at(i);
        
        path = FileUtils::getInstance()->fullPathForFilename(path);
        
        if(path.empty())
        {
            CCLOG("<Error> AI SPeaking ! Save video failed , Empty file pathches !");
            callback(false, outputPath);
            return;
        }
        videoPaths.at(i) = path;
        videoInputPathsStr+= " -i " + path + " ";
    }
    
    outputPath = FileUtils::getInstance()->getWritablePath() + outputPath;
    
    auto bg1920x1080ForMerge = FileUtils::getInstance()->fullPathForFilename("game/main.jpg");
    
    if(bg1920x1080ForMerge.empty()) {
        
        CCLOG("<Error> AI SPeaking ! Save video failed , Empty file pathches !");
        callback(false, outputPath);
        return;
    }

    videoInputPathsStr += " -i " + bg1920x1080ForMerge;

    std::string ffmpegCMD = StringUtils::format(" %s %s %s",videoInputPathsStr.c_str(),cmd.c_str(),outputPath.c_str());
    
//    std::string ffmpegCMD = StringUtils::format("-i %s -i %s -i %s -i %s -i %s -i %s -filter_complex \"[0]scale=722:-1[q1];[5][q1]overlay=159:112:format=auto[q1bg];[1]transpose=1[fg];[fg]scale=722:-1[rvaa];[q1bg][rvaa]overlay=1035:112:format=auto[q1rv1bg];[2:v]setpts=PTS+%f/TB[ls];[ls]scale=722:-1[vid1];[q1rv1bg][vid1]overlay=159:112:format=auto[a1];[3]setpts=PTS+%f/TB[rv02];[rv02]transpose=1[rv02tr];[rv02tr]scale=722:-1[rv02s];[a1][rv02s]overlay=1035:112:format=auto[b1];[4]scale=-1:435[image];[b1][image]overlay=666:584:format=auto;[0:a]volume=1.0[a];[3:a]adelay=%d|%d[c];[a][c]amix=inputs=2:dropout_transition=0\" -b:v 128k -y %s",video01Path.c_str(),video02Path.c_str(),video03Path.c_str(),video04Path.c_str(),imagePath.c_str(),bg1920x1080ForMerge.c_str(),video01Duration,video01Duration,video01DurationMiliseconds,video01DurationMiliseconds,outputPath.c_str());
    
    CCLOG("ffmpegCMD: %s",ffmpegCMD.c_str());
    
//    NSString *ffmpegCommand = [[NSString alloc] initWithCString:ffmpegCMD.c_str() encoding:NSUTF8StringEncoding];
//
//    [FFmpegKit executeAsync:ffmpegCommand withCompleteCallback:^(FFmpegSession* session) {
//
//        SessionState state = [session getState];
//
//        ReturnCode* returnCode = [session getReturnCode];
//
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//        NSLog(@"FFmpeg process exited with state %@ and rc %@.%@", [FFmpegKitConfig sessionStateToString:state], returnCode,(static_cast<void>([session getFailStackTrace]), @"\n"));
//#endif
//          if (state == SessionStateFailed || !returnCode.isValueSuccess) {
//              if(callback) {
//                  callback(false, outputPath);
//              }
//          }
//          else
//          {
//              if(callback) {
//                  callback(true, outputPath);
//              }
//          }
//
//      } withLogCallback:^(Log *log) {
//
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//          if(log != nil) {
//
//              NSLog(@"FFmpeg log: %@", [log getMessage]);
//          }
//#endif
//      } withStatisticsCallback:^(Statistics* statistics){
//
//          if(statistics) {
//
//              NSLog(@"FFmpeg statistics log:getTime %d", [statistics getTime]);
//
//              NSLog(@"FFmpeg statistics log:getSize %ld", [statistics getSize]);
//
//              NSLog(@"FFmpeg statistics log:getVideoFps %f", [statistics getVideoFps]);
//
//              NSLog(@"FFmpeg statistics log:getVideoQuality %f", [statistics getVideoQuality]);
//
//              NSLog(@"FFmpeg statistics log:getSessionId %ld", [statistics getSessionId]);
//
//          }
//
//      }];
    
    return;
}

void CameraHelper::enableBorder(int borderSize,cocos2d::Color4B color) {
    
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    [s_obj enableBorderWithWidth:borderSize r:color.r  g:color.g b:color.b a:color.a];

}

void CameraHelper::removeCamera(std::function<void()> callback) {

    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    [s_obj removeCamera:^{
        
        if(callback) {
            callback();
        }
        
    }];
    
}

void CameraHelper::moveFrontCameraToPos(cocos2d::Vec2 postion,cocos2d::Size size)
{
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    auto convertedRect = convertCocosPositionToIOSScreenPos(cocos2d::Rect(postion,size));

   [s_obj moveFrontCameraToPosWithX:convertedRect.origin.x y:convertedRect.origin.y];
}

void CameraHelper::authorization(bool firstCheckPermisison){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    [s_obj authorization];
    if(!getAuthorizationStatus() && !firstCheckPermisison)
    {
        leadUserToGrantPermissionSetting();
    }
}
bool CameraHelper::getAuthorizationStatus(){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    return [s_obj getAuthorizationStatus];
}

int CameraHelper::getOrientation(){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    return [s_obj getOrientation];
}

bool CameraHelper::getSupportIOS15(){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    return [s_obj getOSVersion15Support];
}
bool CameraHelper::getVideoOrientationLockStatus(){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    return [s_obj getVideoOrientationLockStatus];
}
void CameraHelper::setVideoOrientationLockStatus(bool status){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
   // return [s_obj setVideoOrientationLockStatusWithIsOrientationLock:status];
}
void CameraHelper::leadUserToGrantPermissionSetting(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setCategory:AVAudioSessionCategoryPlayback error:nil];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
#endif
}
void CameraHelper::startCapture(bool isHighlightEnable,bool isSoundEnable)
{
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    //[s_obj startCaptureWithIsHighlightEnable:isHighlightEnable isSoundEnable:isSoundEnable];
    
}

void CameraHelper::endCapture(std::string fileName,std::function<void(bool)> callback)
{
    CCLOG("NextLevel endCapture s [object-c]: %s", fileName.c_str());

    std::string fullPath = cocos2d::FileUtils::getInstance()->getWritablePath() + fileName;
    

    auto enCaptureFun = [callback](std::string fullPath ){
        
        CCLOG("NextLevel endCapture enCaptureFun [object-c]: %s", fullPath.c_str());

        if(!s_obj)  {
            s_obj = [[CameraHelperSwift alloc] init];
        }
        
        NSString *nsStringfullPath= [[NSString alloc] initWithCString:fullPath.c_str() encoding:NSUTF8StringEncoding];
     
        [s_obj endCaptureWithFileName:nsStringfullPath :^(BOOL isSuccess){
            
            if(callback) {
                
                callback(isSuccess);
            }

        }];
        
    };
    
    if(FileUtils::getInstance()->isFileExist(fullPath)) {
        
        CCLOG("NextLevel endCapture isFileExist removeFile call [object-c]: %s", fileName.c_str());

        FileUtils::getInstance()->removeFile(fullPath,[=](bool isSuccess){
            
            CCLOG("NextLevel endCapture isFileExist removeFile success [object-c]: %s", fileName.c_str());

            enCaptureFun(fullPath);
          
        });
    }
    else
    {
        enCaptureFun(fullPath);
    }
}

void CameraHelper::setVisible(bool isVisible, std::function<void()> callback){

    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    [s_obj setVisibleWithIsVisible:isVisible];
}

void CameraHelper::clean(std::function<void()> callback) {
    
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    [s_obj resetCapture];
    
    [s_obj removeCamera:^{
        
        if(callback) {
            callback();
        }
        
    }];
  
    BOOL success = [[AVAudioSession sharedInstance]
                    setCategory: AVAudioSessionCategoryPlayback
                    error: nil];
    
    [[AVAudioSession sharedInstance] setActive:true error:nil];
    
}

void CameraHelper::stop(std::function<void()> callback){

    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    [s_obj stopCapture];
}

cocos2d::Rect CameraHelper::convertCocosPositionToIOSScreenPos(cocos2d::Rect rect) {
    
    cocos2d::Vec2 backButtonLeftBottom = cocos2d::Vec2(rect.origin.x,rect.origin.y);
    cocos2d::Vec2 backButtonRightTop = cocos2d::Vec2(rect.origin.x+rect.size.width,rect.origin.y+rect.size.height);

    auto director       = cocos2d::Director::getInstance();
    auto glView         = director->getOpenGLView();
    auto frameSize      = glView->getFrameSize();
    auto scaleFactor    = [static_cast<CCEAGLView *>(glView->getEAGLView()) contentScaleFactor];

    auto winSize = director->getWinSize();

    auto backButtonPosX = (frameSize.width / 2 + (backButtonLeftBottom.x - winSize.width / 2) * glView->getScaleX()) / scaleFactor;
    auto backButtonPosY = (frameSize.height / 2 - (backButtonRightTop.y - winSize.height / 2) * glView->getScaleY()) / scaleFactor;
    auto backButtonWidth = (backButtonRightTop.x - backButtonLeftBottom.x) * glView->getScaleX() / scaleFactor;
    auto backButtonHeight = (backButtonRightTop.y - backButtonLeftBottom.y) * glView->getScaleY() / scaleFactor;
  
    return cocos2d::Rect(backButtonPosX,backButtonPosY,backButtonWidth,backButtonHeight);
}

void CameraHelper::shareVideoIOSWithUrl(std::string videoPath, std::function<void()> callback) {
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    UIWindow* windows = [app getUIWindow];
    NSString *videoPathSwift = [[NSString alloc] initWithCString:videoPath.c_str() encoding:NSUTF8StringEncoding];
    [s_obj shareVideoIOSWithUrlWithVideoPath:videoPathSwift parent:windows completion:^{
        callback();
    }];
}

std::vector<std::string> CameraHelper::joinMultiVideo(std::vector<std::string> videoPaths, std::string outputPath, std::function<void(bool,std::string)> callback)
{
    //ffmpeg -i "concat:input1.mp4|input2.mp4|input3.mp4" -c copy output.mp4
    std::string videoInputPathsStr = "";
    for(int i = 0; i< videoPaths.size(); i++)
    {
        if(i<videoPaths.size()-1)
            videoInputPathsStr += videoPaths.at(i) + "|";
        else if(i == videoPaths.size()-1)
            videoInputPathsStr += videoPaths.at(i);
    }
    outputPath = FileUtils::getInstance()->getWritablePath() + outputPath;
    
    std::string ffmpegCMD = StringUtils::format("ffmpeg -i \"concat:%s\" -codec copy %s",videoInputPathsStr.c_str(),outputPath.c_str());

    CCLOG("ffmpegCMD: %s",ffmpegCMD.c_str());
    
//    NSString *ffmpegCommand = [[NSString alloc] initWithCString:ffmpegCMD.c_str() encoding:NSUTF8StringEncoding];
//
//    [FFmpegKit executeAsync:ffmpegCommand withCompleteCallback:^(FFmpegSession* session) {
//
//        SessionState state = [session getState];
//
//        ReturnCode* returnCode = [session getReturnCode];
//
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//        NSLog(@"FFmpeg process exited with state %@ and rc %@.%@", [FFmpegKitConfig sessionStateToString:state], returnCode,(static_cast<void>([session getFailStackTrace]), @"\n"));
//#endif
//          if (state == SessionStateFailed || !returnCode.isValueSuccess) {
//              if(callback) {
//                  callback(false, outputPath);
//              }
//          }
//          else
//          {
//              if(callback) {
//                  callback(true, outputPath);
//              }
//          }
//
//      } withLogCallback:^(Log *log) {
//
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//          if(log != nil) {
//
//              NSLog(@"FFmpeg log: %@", [log getMessage]);
//          }
//#endif
//      } withStatisticsCallback:^(Statistics* statistics){
//
//          if(statistics) {
//
//              NSLog(@"FFmpeg statistics log:getTime %d", [statistics getTime]);
//
//              NSLog(@"FFmpeg statistics log:getSize %ld", [statistics getSize]);
//
//              NSLog(@"FFmpeg statistics log:getVideoFps %f", [statistics getVideoFps]);
//
//              NSLog(@"FFmpeg statistics log:getVideoQuality %f", [statistics getVideoQuality]);
//
//              NSLog(@"FFmpeg statistics log:getSessionId %ld", [statistics getSessionId]);
//
//          }
//
//      }];
}

void CameraHelper::enableAudioWhenRecordCamera(bool isEnable) {
    
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    [s_obj enableAudioWhenRecordCameraWithIsSoundEnable:isEnable];
}

void CameraHelper::startVideoRecorderIOS() {
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    
    [s_obj startVideoCapture];
}

void CameraHelper::stopVideoRecorderIOS(std::function<void(void)> callback, ms::delegate::AISpeakingSharePopupDelegate *aISpeakingSharePopupDelegate) {
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
    UIWindow* windows = [app getUIWindow];
    [s_obj stopVideoCaptureWithoutOutputUrlWithParent: windows completion:^(RPPreviewViewController *previewViewController){
        if(previewViewController)
        {
            if(auto mSAISpeakingSharePopup = MSAISpeakingSharePopup::showPopup(aISpeakingSharePopupDelegate, nullptr)){
                mSAISpeakingSharePopup->setShareCallback([=](){
                    if(!s_obj)  {
                        s_obj = [[CameraHelperSwift alloc] init];
                    }
                    AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
                    UIWindow* windows = [app getUIWindow];
                    
                    [s_obj shareVideoIOSWithOutUrlWithParent: windows completion:^{
                        callback();
                    }];
                });
                mSAISpeakingSharePopup->setFinishCallback([=](){
                    
                });
                mSAISpeakingSharePopup->setOnExitCallback([=](){
                    
                });
            }
        }
    }];

}

void CameraHelper::requestNotifyPermision(){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    [s_obj authorizationNotifyStatus];

}

bool CameraHelper::checkNotifyPermision(){
    if(!s_obj)  {
        s_obj = [[CameraHelperSwift alloc] init];
    }
    return [s_obj getAuthorizationNotifyStatus];

}

void CameraHelper::setIconApp()
{
    std::string nameIcon = "AppIcon";
    NSString *iconName = [[NSString alloc] initWithUTF8String:nameIcon.c_str()];
                //anti apple private method call analyse
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
                        func([UIApplication sharedApplication], selector, iconName, ^(NSError * _Nullable error) {
                        });
                    }
                }
}

#endif
