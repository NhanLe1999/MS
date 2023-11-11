#include "MSRecord.h"
#include "HSAudioEngine.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"
#include "MMQRScan.h"
#include "cocostudiomacro2.h"
#include "network/HttpClient.h"
#include "MJRequest.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include <iostream>
#include "MJHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
USING_NS_CC;
#include "HSAudioEngine.h"
#import <AVFoundation/AVFoundation.h>
//#import <FirebaseDynamicLinks/FirebaseDynamicLinks.h>
@interface UIRecordIos : NSObject <AVAudioRecorderDelegate, AVAudioPlayerDelegate>
- (void) createRecord:(std::string&) savingPath :(BOOL) isAVAudioSessionChange :(BOOL) isMinSetting;
- (void) startRecord;
- (void) showRemainPermissionPoup;
- (void) stopRecord;
- (void) checkBlow;
- (void) playRecord;
- (void) stopPlay;
- (id) init;
- (void) deleteRecord;
- (bool) getPermissionStatus;
- (bool) isRecording;
- (string) getURL;
@property (readonly) NSString *pathAudio;
@property (readwrite) BOOL isAVAudioSessionChange;
@property (readwrite) AVAudioSessionCategoryOptions recordOption;
@end

@implementation UIRecordIos
{
    AVAudioRecorder *recorder;
    AVAudioPlayer *player;
    double lowPassResults;
}

-(id)init
{
    if (self = [super init]) {
        player = nullptr;
        recorder = nullptr;
    }
    
    return self;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
        if (buttonIndex == [alertView cancelButtonIndex])
        {
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
        }
        else
        {
            [alertView dismissWithClickedButtonIndex:0 animated:YES];
        }
}
#endif
 AVAudioSessionCategory currentAVAudioSessionCategory ;
 AVAudioSessionCategoryOptions currentCategoryOptions;

-(void) createRecord:(std::string&) savingPath :(BOOL) isAVAudioSessionChange :(BOOL) isMinSetting {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    
    self.isAVAudioSessionChange = isAVAudioSessionChange;
    AVAudioSession *session = [AVAudioSession sharedInstance];
    currentAVAudioSessionCategory =[session category];
    currentCategoryOptions = [session categoryOptions];
    
    if( self.isAVAudioSessionChange){
        if (@available(iOS 16, *)) {
            
        [session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:nil];
        [session setActive:YES error:nil];
                
        }
        else
        {
            [session setCategory:AVAudioSessionCategoryRecord withOptions:_recordOption error:nil];
            [session setActive:YES error:nil];
        }
    }

    auto isAccept = [session recordPermission];
    if (isAccept != AVAudioSessionRecordPermissionGranted) {
        [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
            
            if (granted) {
                MJDEFAULT->setStringForKey("key_turn_record", "YES");
            }
            else {
                MJDEFAULT->setStringForKey("key_turn_record", "NO");
            }
        }];
    }
    NSMutableDictionary* recordSetting = [[NSMutableDictionary alloc] init];
    
    if(!isMinSetting) {
        recordSetting[AVFormatIDKey] = @(kAudioFormatLinearPCM);
        recordSetting[AVSampleRateKey] = @44100.0f;
        recordSetting[AVNumberOfChannelsKey] = @1;
        recordSetting[AVEncoderAudioQualityKey] = @(AVAudioQualityMax);
        recordSetting[AVLinearPCMBitDepthKey] = @16;
        recordSetting[AVLinearPCMIsBigEndianKey] = [NSNumber numberWithBool:NO];
    }
    else
    {
        recordSetting[AVFormatIDKey] = @(kAudioFormatLinearPCM);
        recordSetting[AVSampleRateKey] = @8000.0f;
        recordSetting[AVNumberOfChannelsKey] = @1;
        recordSetting[AVEncoderAudioQualityKey] = @(AVAudioQualityLow);
        recordSetting[AVLinearPCMBitDepthKey] = @16;
        recordSetting[AVLinearPCMIsBigEndianKey] = [NSNumber numberWithBool:NO];
    }

    auto fullPath = FileUtils::getInstance()->getWritablePath() + "/" + savingPath;
    
    NSString *fullFilePath= [[NSString alloc] initWithCString:fullPath.c_str() encoding:NSUTF8StringEncoding];

    _pathAudio = fullFilePath;
    
    NSLog(@"fullFilePath = %@", fullFilePath);
    NSURL *inUrl = [NSURL fileURLWithPath:fullFilePath];
    
    NSError *error = NULL;
    recorder = [[ AVAudioRecorder alloc] initWithURL:inUrl settings:recordSetting error:&error];
    [recorder setMeteringEnabled:YES];
    [recorder setDelegate:self];
    [recorder prepareToRecord];
#endif
    
}


- (bool) getPermissionStatus{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AVAudioSession *session = [AVAudioSession sharedInstance];
    return [session recordPermission] == AVAudioSessionRecordPermissionGranted;
#endif
    return false;
}

- (bool) isRecording {
 
    return recorder != nil ? [recorder isRecording] : false;
}

- (void) startRecord{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
        if (granted) {
            MJDEFAULT->setStringForKey("key_turn_record", "YES");
        }
        else {
            MJDEFAULT->setStringForKey("key_turn_record", "NO");
        }
    }];
    
    if(MJDEFAULT->getStringForKey("key_turn_record") == "NO")
    {
        [self showRemainPermissionPoup];
    }
#endif

    [recorder record];
}

- (void) showRemainPermissionPoup {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AVCaptureSession * _session;
    AVCaptureDevice * _device;
    AVCaptureDeviceInput * _input;
    AVCaptureMetadataOutput * _output;
    AVCaptureVideoPreviewLayer * _previewLayer;
    std::string _detectionString;
    self = [super init];
    _session = [AVCaptureSession new];
    
    _device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeAudio];
    NSError * error = nil;
    _input = [AVCaptureDeviceInput deviceInputWithDevice:_device error:&error];
    if (_input) {
        [_session addInput:_input];
    } else {
        NSLog(@"Error : %@", error);
    }
    
    NSString *mediaType = AVMediaTypeAudio;
    
    AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:mediaType];
    NSString *alertText = [[NSString alloc] initWithUTF8String:LANGUAGE_MANAGER->getDisplayTextByKey("key.setting.audio.ios").c_str()];
    
    NSString *alertCancelButtonText = [[NSString alloc] initWithUTF8String:LANGUAGE_MANAGER->getDisplayTextByKey("key.close.setting.ios").c_str()];
    NSString *alertGoButtonText = [[NSString alloc] initWithUTF8String:LANGUAGE_MANAGER->getDisplayTextByKey("key.go.setting.ios").c_str()];
    
    UIAlertView *alert = [[UIAlertView alloc]
                          initWithTitle:@"Error"
                          message:alertText
                          delegate:self
                          cancelButtonTitle:alertGoButtonText
                          otherButtonTitles:alertCancelButtonText, nil
    ];
    alert.tag = 3491832;
    [alert show];
#endif
}

- (void) stopRecord{
    
    if(recorder != nil && [recorder isRecording]) {
        [recorder stop];
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AVAudioSession *session = [AVAudioSession sharedInstance];
    if( self.isAVAudioSessionChange){
        if (@available(iOS 16, *)) {
            AVAudioSession *session = [AVAudioSession sharedInstance];
            [session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:nil];
            [session setActive:YES error:nil];
        }
        else
        [session setCategory:AVAudioSessionCategoryPlayback error:nil];
    }
    
#endif
}

- (void) checkBlow {
    
    if(recorder != nil){
        
      if([recorder isRecording]) {
            
            [recorder updateMeters];

            CCLOG("[abc2]Dis be da level I'm hearin' you in dat mic %f ",[recorder peakPowerForChannel:0]);

            const double ALPHA = 0.05;
                double peakPowerForChannel = pow(10, (0.05 * [recorder peakPowerForChannel:0]));
             lowPassResults = ALPHA * peakPowerForChannel + (1.0 - ALPHA) * lowPassResults;
            
            if (lowPassResults > 0.95) {
                
                NSLog(@"Mic blow detected");
                
                //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ai_speaking_ev_animated");
            }
            else{
                //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ai_speaking_ev_idle");
            }
                    
            if ([recorder averagePowerForChannel:0] > -25) {
                
                int animationID = 1;
                
                if([recorder averagePowerForChannel:0] > -5) {
                    
                    animationID = 2;
                }
                
                if([recorder averagePowerForChannel:0] > -2) {
                 
                    animationID = 3;
                }
                
                NSLog(@"[abc1]Mic blow detected pC:%f aID:%d",[recorder peakPowerForChannel:0],animationID);

                
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ai_speaking_ev_animate_record_button",new int(animationID));
            }
            else
            {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ai_speaking_ev_idle_record_button");
            }
        }
    }

}

- (void) playRecord{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    if( self.isAVAudioSessionChange){
        AVAudioSession *session = [AVAudioSession sharedInstance];
        [session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:nil];
        [session setActive:YES error:nil];
    }
    player = [[AVAudioPlayer alloc] initWithContentsOfURL:recorder.url error:nil];
    [player setVolume:1.0];
    [player play];
#endif
}

- (void) stopPlay{
//    player = [[AVAudioPlayer alloc] initWithContentsOfURL:recorder.url error:nil];
    [player stop];
}


- (std::string) getBase64Data2 {
    NSArray* documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* fullFilePath = [[documentPaths objectAtIndex:0] stringByAppendingPathComponent: @"audiomj.wav"];
    
    NSData *audioData = [NSData dataWithContentsOfFile:fullFilePath];
    NSString * base64 = [audioData base64EncodedStringWithOptions:0];
    return [base64 UTF8String];
}


- (std::string) getBase64Data {
    NSArray* documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* fullFilePath = [[documentPaths objectAtIndex:0] stringByAppendingPathComponent: @"gamemonkeyphonics.m4a"];
    
    NSData *audioData = [NSData dataWithContentsOfFile:fullFilePath];
    NSString * base64 = [audioData base64EncodedStringWithOptions:0];
    return [base64 UTF8String];
}

- (void) deleteRecord{
    recorder = nullptr;
    player = nullptr;
}

- (string) getURL {
    return std::string([_pathAudio UTF8String]);
}

@end

UIRecordIos *_record = nullptr;

MSRecord * MSRecord::createRecordWithDuration(float audioDuration,std::string path,bool isAVAudioSessionChange,bool isLowSettingEnable,MSRecord::Type type) {
    auto p_food = new MSRecord();
    if (p_food ) {
        p_food->_type = type;
        p_food->_isAVAudioLowSettingEnable = isLowSettingEnable;
        p_food->_isisAVAudioSessionChange = isAVAudioSessionChange;
        p_food->_phrase = path;
        p_food->init(audioDuration,false,true);
    } else {
        delete p_food;
        p_food = nullptr;
    }
    return p_food;
}

MSRecord * MSRecord::createRecord(float timer, std::string phrase,bool isShowNativeRemainPopup) {
    auto p_food = new MSRecord();
    if (p_food ) {
        p_food->_phrase = phrase;
        p_food->init(timer,isShowNativeRemainPopup);
        p_food->autorelease();
    } else {
        delete p_food;
        p_food = nullptr;
    }
    return p_food;
}

bool MSRecord::init(float timer,bool isShowNativeRemainPopup,bool isStartRecord) {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    };
    
    is_show_remainding_permission_popup = isShowNativeRemainPopup;
    is_record_stop = false;
    time_record = 0.0f;
    _record_duration = timer;
    
    _record = [[UIRecordIos alloc] init];
    [_record createRecord:_phrase:_isisAVAudioSessionChange:_isAVAudioLowSettingEnable];
    if(![_record getPermissionStatus]){
        [_record stopRecord];
        
        if(is_show_remainding_permission_popup) {
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
            showRemainPermissionPopup();
#endif
        }
        return false;
    }
    
    if(isStartRecord){
        
        startRecord();
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
            if (granted) {
                MJDEFAULT->setStringForKey("key_turn_record", "YES");
            }
            else {
                MJDEFAULT->setStringForKey("key_turn_record", "NO");
            }
        }];
#endif
    return true;
}

void MSRecord::startRecord(){
    
    if(!_record) {
        _record = [[UIRecordIos alloc] init];
    }
    
    switch (_type) {
        case MSRecord::Type::AVAudioSessionCategoryOptionMixWithOthers:
            [_record setRecordOption:AVAudioSessionCategoryOptionMixWithOthers];
            break;
        case MSRecord::Type::AVAudioSessionCategoryOptionDuckOthers:
            [_record setRecordOption:AVAudioSessionCategoryOptionDuckOthers];
            break;
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
        case MSRecord::Type::AVAudioSessionCategoryOptionAllowBluetooth:
            [_record setRecordOption:AVAudioSessionCategoryOptionAllowBluetooth];
            break;
        case MSRecord::Type::AVAudioSessionCategoryOptionDefaultToSpeaker:
            [_record setRecordOption:AVAudioSessionCategoryOptionDefaultToSpeaker];
            break;
        case MSRecord::Type::AVAudioSessionCategoryOptionInterruptSpokenAudioAndMixWithOthers:
            [_record setRecordOption:AVAudioSessionCategoryOptionInterruptSpokenAudioAndMixWithOthers];
            break;
        case MSRecord::Type::AVAudioSessionCategoryOptionAllowBluetoothA2DP:
            [_record setRecordOption:AVAudioSessionCategoryOptionAllowBluetoothA2DP];
            break;
        case MSRecord::Type::AVAudioSessionCategoryOptionAllowAirPlay:
            [_record setRecordOption:AVAudioSessionCategoryOptionAllowAirPlay];
            break;
        default:
            [_record setRecordOption:AVAudioSessionCategoryOptionDefaultToSpeaker];
            break;
#endif

    }
    
    [_record createRecord:_phrase:_isisAVAudioSessionChange:_isAVAudioLowSettingEnable];
    is_record_stop = false;
    this->schedule(schedule_selector(MSRecord::updateCountTimeRecord));
    [_record startRecord];
    this->runAction(Sequence::create(DelayTime::create(_record_duration), CallFunc::create([=]{
        if (!is_record_stop) {
            this->unschedule(schedule_selector(MSRecord::updateCountTimeRecord));
            is_record_stop = true;
            [_record stopRecord];
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                if (delegate) {
                    delegate->onFinishRecord(RECORD_DONE, [_record getURL]);

                }
            });
        }
    }), nullptr));
}

void MSRecord::startPlay(bool _is_play_once){
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
    [_record playRecord];
    CCLOG("time_record = %f", time_record);
    this->runAction(Sequence::create(DelayTime::create(time_record), CallFunc::create([=]{
        if(_is_play_once){
            [_record stopPlay];
            [_record deleteRecord];
            [_record release];
        }
        
    }), DelayTime::create(0.25f), CallFunc::create([=]{
        [_record stopPlay];
        if (delegate) {
            delegate->onFinishRecord(PLAY_DONE,[_record getURL]);
            if (@available(iOS 16, *)) {
                AVAudioSession *session = [AVAudioSession sharedInstance];
                [session setCategory:currentAVAudioSessionCategory withOptions:currentCategoryOptions error:nil];
                [session setActive:YES error:nil];
            }
        }
        if(_is_play_once){
            this->removeFromParent();
        }
    }),nullptr));
#endif
}

void MSRecord::stopPlay(){
     [_record stopPlay];
    if (delegate) {
        delegate->onFinishRecord(PLAY_DONE, "");
    }
}

void MSRecord::stopRecord() {
    
    if (is_record_stop) {
        return;
    }
    this->unschedule(schedule_selector(MSRecord::updateCountTimeRecord));
    is_record_stop = true;
    if(_record) {
        [_record stopRecord];
    }
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        if (delegate) {
            delegate->onFinishRecord(RECORD_DONE, [_record getURL]);
        }
    });
}

void MSRecord::checkBlow(){
    if (!is_record_stop) {
     [_record checkBlow];
    }
}

bool MSRecord::isRecording(){
    return _record != nullptr ? [_record isRecording]:false;
}
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
void MSRecord::showRemainPermissionPopup(){
    if(MJDEFAULT->getStringForKey("key_turn_record","NO") == "NO")
    {
       // [_record showRemainPermissionPoup];
    }
}
#endif

void MSRecord::updateCountTimeRecord(float dt){
    time_record += dt;
}

void MSRecord::setDelegate(MSRecordDelegate *_delegate){
    delegate = _delegate;
}
MSRecord::PERMISSION_STATUS MSRecord::getPermissionStatus(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AVAudioSession *session = [AVAudioSession sharedInstance];
    switch ([session recordPermission]) {
        case AVAudioSessionRecordPermissionGranted:
            return GRANTED;
            break;
        case AVAudioSessionRecordPermissionDenied:
            return DENIED;
            break;
        case AVAudioSessionRecordPermissionUndetermined:
            return UNDETERMINED;
            break;
        default:
            return DENIED;
            break;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return DENIED;
#endif
}
void MSRecord::askPermission(std::function<void()> accepted,std::function<void()> denied){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(MSRecord::getPermissionStatus() == MSRecord::DENIED){
        MSRecord::leadUserToGrantPermissionSetting();
        return;
    }
    AVAudioSession *session = [AVAudioSession sharedInstance];
    
    [session requestRecordPermission:^(BOOL granted) {
        if (granted) {
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([accepted] {
                
                if(accepted) {
                    accepted();
                }
                
            });
        }
        else {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([denied] {
                
                if(denied) {
                    denied();
                }
                
            });
        }
    }];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return;
#endif
}
void MSRecord::leadUserToGrantPermissionSetting(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AVAudioSession *session = [AVAudioSession sharedInstance];
    
    //if(_isisAVAudioSessionChange){
        [session setCategory:AVAudioSessionCategoryPlayback error:nil];
    //}
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
#endif
}
#endif
