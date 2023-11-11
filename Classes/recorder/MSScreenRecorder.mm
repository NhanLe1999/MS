//
//  MSScreenRecorder.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/4/18.
//

#include "MSScreenRecorder.h"
#import <CoreGraphics/CoreGraphics.h>
#include "StoryAlert.h"
#include "AudioEngine.h"
#include "StoryLanguageManager.h"

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#import <ReplayKit/ReplayKit.h>
@interface ScreenRecorderHelper : NSObject<RPPreviewViewControllerDelegate> {
}
+ (id) sharedSCHelper;
- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController;
@end

@implementation ScreenRecorderHelper
+ (id)sharedSCHelper {
    static ScreenRecorderHelper * shared_schelper = nil;
    @synchronized (self) {
        if (shared_schelper == nil) {
            shared_schelper = [[self alloc] init];
        }

        return shared_schelper;
    }
}

- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController {
    [previewController dismissViewControllerAnimated:YES completion:^() {
        experimental::AudioEngine::end();
        experimental::AudioEngine::lazyInit();
        cocos2d::Director::getInstance()->getRunningScene()->removeChildByName("overlay_recorder");
    }];
}
@end

namespace ms { namespace recorder {

    void startRecord(std::function<void()> callback) {
        if ([[RPScreenRecorder sharedRecorder] isAvailable]) {
//            enableMicro();
            @try {
                [[RPScreenRecorder sharedRecorder] setMicrophoneEnabled: YES];

                [[RPScreenRecorder sharedRecorder] startRecordingWithHandler:^(NSError * _Nullable error) {
                    CCLOG("vào rồi vào rồi");
                    if (error) {
                        //auto msg = std::string([error.localizedDescription UTF8String]);
                        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
			    auto msg =LANGUAGE_MANAGER->getDisplayTextByKey("reject.record.audio");
                            StoryAlert::showAlert(msg, "OK", "OK");
                        });
                        return;
                    }

                    //                [[AVAudioSession sharedInstance] overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
                    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:nil];
                    if (callback)
                        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([callback] {
                            callback();
                        });
                }];


//            [[RPScreenRecorder sharedRecorder] startRecordingWithMicrophoneEnabled:YES handler:^(NSError * _Nullable error) {
//                if (error) {
//                    auto msg = std::string([error.localizedDescription UTF8String]);
//                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
//                        StoryAlert::showAlert(msg, "OK", "OK");
//                    });
//                    return;
//                }
//
////                [[AVAudioSession sharedInstance] overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
//                [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:nil];
//                if (callback)
//                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([callback] {
//                        callback();
//                    });
//            }];
            }
            @catch (NSException * e) {
                StoryAlert::showAlert("There was an error.\nPlease try again later.", "OK", "OK");
            }
            @finally {

            }
        } else {
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                StoryAlert::showAlert("There was an error.\nPlease try again later.", "OK", "OK");
            });
        }
    }

    void stopRecord() {
        [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
        [[RPScreenRecorder sharedRecorder] stopRecordingWithHandler:^(RPPreviewViewController * _Nullable previewViewController, NSError * _Nullable error) {
            if (previewViewController) {
                [previewViewController setModalInPopover:YES];
                [previewViewController setPreviewControllerDelegate:[ScreenRecorderHelper sharedSCHelper]];

                [previewViewController popoverPresentationController].sourceView = [[[[UIApplication sharedApplication] keyWindow] rootViewController] view];
                [[previewViewController popoverPresentationController] setPermittedArrowDirections:0];

                [[[UIApplication sharedApplication] keyWindow] rootViewController].title = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
                CGRect r = [[[[[UIApplication sharedApplication] keyWindow] rootViewController] view] bounds];
                [previewViewController popoverPresentationController].sourceRect = CGRectMake(r.size.width / 2, r.size.height / 2, 0, 0);
                [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:previewViewController animated:YES completion:nil];

                auto overlay = cocos2d::ui::Layout::create();
                overlay->setBackGroundColor(Color3B::BLACK);
                overlay->setTouchEnabled(true);
                overlay->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                overlay->setOpacity(0xdd);
                overlay->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
                overlay->setName("overlay_recorder");
                cocos2d::Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
            } else {
                if (error) {
                    auto msg = std::string([error.localizedDescription UTF8String]);
                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
                        StoryAlert::showAlert(msg, "OK", "OK");
                    });
                    return;
                } else {
                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                        StoryAlert::showAlert("There was an error.\nPlease try again later.", "OK", "OK");
                    });
                }
            }
        }];
    }

    void cancelRecord() {
        if ([[RPScreenRecorder sharedRecorder] isRecording]) {
            [[RPScreenRecorder sharedRecorder] stopRecordingWithHandler:^(RPPreviewViewController * _Nullable previewViewController, NSError * _Nullable error) {
                [[RPScreenRecorder sharedRecorder] discardRecordingWithHandler:^{

                }];
            }];
        }
    }

    bool isRecording() {
        return [[RPScreenRecorder sharedRecorder] isRecording];
    }

    bool isSupported() {
        if (@available(iOS 10.0, *)) {
            if ([[RPScreenRecorder sharedRecorder] isAvailable])
                return true;
        }
        return false;
    }

    void enableMicro() {
//        [[RPScreenRecorder sharedRecorder] setMicrophoneEnabled:YES];
    }

    void disableMicro() {
//        [[RPScreenRecorder sharedRecorder] setMicrophoneEnabled:NO];
    }
}}

#elif TARGET_OS_MAC
namespace ms { namespace recorder {
    void startRecord(std::function<void()> callback) {}
    void stopRecord() {}
    void cancelRecord() {}
    bool isRecording() { return false; }

    bool isSupported() { return false; }

    void enableMicro() {}
    void disableMicro() {}
}}
#endif
