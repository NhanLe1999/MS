//
//  MSPlaylistHelper.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/21/18.
//

#include "MSPlaylistHelper.h"
#include "cocos2d.h"
#include "MJHelper.h"

#import <MediaPlayer/MediaPlayer.h>
#import "STKAudioPlayer.h"
#import <AVFoundation/AVFoundation.h>

#include "StoryDataModel.h"
#include "MSAudiobookPlayer.h"

#include "MSDownloader.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
@interface PlaylistImp : NSObject<STKDataSourceDelegate, STKAudioPlayerDelegate> {
    BOOL _isAudioSessionInterrupted; // kiểm tra xem audio session có bị interrupt không
}
+ (id) sharedInstance;

@property (strong, nonatomic) STKAudioPlayer * player;
@property (readwrite, nonatomic) BOOL isAudioSessionInterrupted;

-(void) queue:(NSString *)file_path withAudiobookID:(int) audiobook_id;
-(void) pause;
-(void) resume;
-(void) stop;
-(double) getCurrentProgress;
-(int) getCurrentItemId;
-(std::vector<int>) getNext5ItemsId;
-(STKAudioPlayerState) getState;
-(void) seek:(double) timestamp;
-(int) pendingQueueItemCount;

-(void) dataSourceDataAvailable:(STKDataSource*)dataSource;
-(void) dataSourceErrorOccured:(STKDataSource*)dataSource;
-(void) dataSourceEof:(STKDataSource*)dataSource;
-(void) dataSource:(STKDataSource*)dataSource didReadStreamMetadata:(NSDictionary*)metadata;

/// Raised when an item has started playing
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didStartPlayingQueueItemId:(NSObject*)queueItemId;
/// Raised when an item has finished buffering (may or may not be the currently playing item)
/// This event may be raised multiple times for the same item if seek is invoked on the player
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didFinishBufferingSourceWithQueueItemId:(NSObject*)queueItemId;
/// Raised when the state of the player has changed
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer stateChanged:(STKAudioPlayerState)state previousState:(STKAudioPlayerState)previousState;
/// Raised when an item has finished playing
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didFinishPlayingQueueItemId:(NSObject*)queueItemId withReason:(STKAudioPlayerStopReason)stopReason andProgress:(double)progress andDuration:(double)duration;
/// Raised when an unexpected and possibly unrecoverable error has occured (usually best to recreate the STKAudioPlauyer)
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer unexpectedError:(STKAudioPlayerErrorCode)errorCode;

/// Optionally implemented to get logging information from the STKAudioPlayer (used internally for debugging)
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer logInfo:(NSString*)line;
/// Raised when items queued items are cleared (usually because of a call to play, setDataSource or stop)
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didCancelQueuedItems:(NSArray*)queuedItems;
/// Raised when datasource read stream metadata
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didReadStreamMetadata:(NSDictionary*)dictionary;

@end

@implementation PlaylistImp
@synthesize isAudioSessionInterrupted = _isAudioSessionInterrupted;

+ (id)sharedInstance {
    static PlaylistImp * shared_helper = nil;
    @synchronized (self) {
        if (shared_helper == nil) {
            shared_helper = [[self alloc] init];
            [shared_helper setPlayer: [STKAudioPlayer new]];
            [[shared_helper player] setDelegate: shared_helper];
            
            [[NSNotificationCenter defaultCenter] addObserver:shared_helper
                                                     selector:@selector(handleAudioSessionInterruption:)
                                                         name:AVAudioSessionInterruptionNotification
                                                       object:[AVAudioSession sharedInstance]];
            shared_helper.isAudioSessionInterrupted = NO;
        }
        
        return shared_helper;
    }
}

#include "AudioEngine.h"
- (void)handleAudioSessionInterruption:(NSNotification*)notification {
    NSNumber *interruptionType = [[notification userInfo] objectForKey:AVAudioSessionInterruptionTypeKey];
    NSNumber *interruptionOption = [[notification userInfo] objectForKey:AVAudioSessionInterruptionOptionKey];
    
    switch (interruptionType.unsignedIntegerValue) {
        case AVAudioSessionInterruptionTypeBegan:{
            ms::playlist::ABPlayer::getInstance().stop();
            NSError * error = nil;
            [[AVAudioSession sharedInstance] setActive:YES error:&error];
            if (error) {
                NSLog(@"%@", error.description);
            }
            // • Audio has stopped, already inactive
            // • Change state of UI, etc., to reflect non-playing state
            _isAudioSessionInterrupted = YES;
        } break;
        case AVAudioSessionInterruptionTypeEnded:{
            // • Make session active
            // • Update user interface
            // • AVAudioSessionInterruptionOptionShouldResume option
            if (interruptionOption.unsignedIntegerValue == AVAudioSessionInterruptionOptionShouldResume) {
                // Here you should continue playback.
                [_player resume];
            }
            _isAudioSessionInterrupted = NO;
        } break;
        default:
            break;
    }
}

- (void) queue:(NSString *)file_path withAudiobookID:(int)audiobook_id {
    STKDataSource * data_source = [STKAudioPlayer dataSourceFromURL:[NSURL URLWithString:file_path]];
//    [data_source setRecordToFileUrl:[NSURL fileURLWithPath:[NSString stringWithUTF8String:(cocos2d::FileUtils::getInstance()->getWritablePath() + cocos2d::StringUtils::toString(audiobook_id) + ".mp3").c_str()]]];
    [data_source setDelegate:self];
    [_player queueDataSource:data_source withQueueItemId:@(audiobook_id)];
}

- (void) pause {
    [_player pause];
}

- (void) resume {
    [_player resume];
}

- (void) stop {
    [_player setDelegate:nil];
    [_player stop];
    [_player release];
    _player = [STKAudioPlayer new];
    [_player setDelegate:[PlaylistImp sharedInstance]];
}

- (double)getCurrentProgress {
    return [_player progress];
}

- (STKAudioPlayerState)getState {
    return [_player state];
}

- (int)pendingQueueItemCount {
    return (int)[_player pendingQueueCount];
}

- (void)seek:(double)timestamp {
    [_player seekToTime:timestamp];
}

- (int)getCurrentItemId {
    return [(NSNumber *)[_player currentlyPlayingQueueItemId] intValue];
}

- (std::vector<int>)getNext5ItemsId {
    std::vector<int> next_ids;
    next_ids.push_back([self getCurrentItemId]);
    NSArray * pending = [_player pendingQueue];
    for (NSNumber * i in [pending reverseObjectEnumerator]) {
        next_ids.push_back([i intValue]);
        if (next_ids.size() == 5) {
            break;
        }
    }
    
    return next_ids;
}

- (void)dataSourceDataAvailable:(STKDataSource *)dataSource {
    NSLog(@"datasource %@", dataSource);
}

- (void)dataSourceErrorOccured:(STKDataSource *)dataSource {
    
}

- (void)dataSourceEof:(STKDataSource *)dataSource {
    
}

- (void)dataSource:(STKDataSource *)dataSource didReadStreamMetadata:(NSDictionary *)metadata {
    NSLog(@"metadata %@", metadata);
}

/// Raised when an item has started playing
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didStartPlayingQueueItemId:(NSObject*)queueItemId {
    NSLog(@"start play %@", queueItemId);
    
    auto info = StoryDataModel::getInstance()->getAudiobook([(NSNumber *)queueItemId intValue]);
    
    auto image_name = mj::helper::getFilenameFromURL(info.thumbnail_url);
    auto cached_image = FileUtils::getInstance()->getWritablePath() + "imgcache/" + image_name;
    
    ms::playlist::setNowPlayingInfo(info, 0);
    ms::playlist::ABPlayer::getInstance().didStartPlayingQueueItemId([(NSNumber *)queueItemId intValue]);
}
/// Raised when an item has finished buffering (may or may not be the currently playing item)
/// This event may be raised multiple times for the same item if seek is invoked on the player
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didFinishBufferingSourceWithQueueItemId:(NSObject*)queueItemId {
    NSLog(@"finish buffer %@", queueItemId);
}
/// Raised when the state of the player has changed
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer stateChanged:(STKAudioPlayerState)state previousState:(STKAudioPlayerState)previousState {
}

/// Raised when an item has finished playing
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didFinishPlayingQueueItemId:(NSObject*)queueItemId withReason:(STKAudioPlayerStopReason)stopReason andProgress:(double)progress andDuration:(double)duration {
    NSLog(@"finish play %@", queueItemId);
    ms::playlist::ABPlayer::getInstance().didFinishPlayingQueueItemId([(NSNumber *)queueItemId intValue]);
}
/// Raised when an unexpected and possibly unrecoverable error has occured (usually best to recreate the STKAudioPlauyer)
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer unexpectedError:(STKAudioPlayerErrorCode)errorCode {
    NSLog(@"playlist error: %ld", (long)errorCode);
    
}
/// Optionally implemented to get logging information from the STKAudioPlayer (used internally for debugging)
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer logInfo:(NSString*)line {
    NSLog(@"%@", line);
}
/// Raised when items queued items are cleared (usually because of a call to play, setDataSource or stop)
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didCancelQueuedItems:(NSArray*)queuedItems {
    NSLog(@"did cancel item");
    ms::playlist::ABPlayer::getInstance().didCancelQueuedItems();
}

/// Raised when datasource read stream metadata
-(void) audioPlayer:(STKAudioPlayer*)audioPlayer didReadStreamMetadata:(NSDictionary*)dictionary {
    NSLog(@"%@", dictionary);
}

- (void)audioPlayer:(nonnull STKAudioPlayer *)audioPlayer didFinishBufferingSourceWithQueueItemId:(nonnull NSObject *)queueItemId fullyProcessed:(BOOL)fullyProcessed {
    if (fullyProcessed) {
        NSLog(@"Recorded %@", queueItemId);
    } else {
        NSLog(@"Corrupted %@", queueItemId);
    }
}

@end
#endif

namespace ms { namespace playlist {

    static id commandCenter_playCommand_id = 0;
    static id commandCenter_pauseCommand_id = 0;
    static id commandCenter_nextTrackCommand_id = 0;
    static id commandCenter_previousTrackCommand_id = 0;
    static id commandCenter_changePlaybackPositionCommand_id = 0;
    static bool remote_command_initialized = false;
    
    void play(std::vector<std::string> queue) {
    }
    
    void setupRemoteCommand() {
        // hieunt: nếu là tài khoản trial thì không cho hiện player ở màn hình LOCK của iPhone
        if (!CONFIG_MANAGER->isAppActive()) {
            remote_command_initialized = false;
            
            MPRemoteCommandCenter * commandCenter = [MPRemoteCommandCenter sharedCommandCenter];
            
            if (commandCenter_playCommand_id) {
                [commandCenter.playCommand removeTarget:commandCenter_playCommand_id];
                commandCenter_playCommand_id = 0;
            }
            
            if (commandCenter_pauseCommand_id) {
                [commandCenter.pauseCommand removeTarget:commandCenter_pauseCommand_id];
                commandCenter_pauseCommand_id = 0;
            }
            
            if (commandCenter_nextTrackCommand_id) {
                [commandCenter.nextTrackCommand removeTarget:commandCenter_nextTrackCommand_id];
                [commandCenter.nextTrackCommand setEnabled:NO];
                commandCenter_nextTrackCommand_id = 0;
            }
            
            if (commandCenter_previousTrackCommand_id) {
                [commandCenter.previousTrackCommand removeTarget:commandCenter_previousTrackCommand_id];
                [commandCenter.previousTrackCommand setEnabled:NO];
                commandCenter_previousTrackCommand_id = 0;
            }
            
            if (commandCenter_changePlaybackPositionCommand_id) {
                [commandCenter.changePlaybackPositionCommand removeTarget:commandCenter_changePlaybackPositionCommand_id];
                [commandCenter.changePlaybackPositionCommand setEnabled:NO];
                commandCenter_changePlaybackPositionCommand_id = 0;
            }
            
            return;
        }
        
        if (remote_command_initialized) {
            return;
        }
        
        remote_command_initialized = true;
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        MPRemoteCommandCenter * commandCenter = [MPRemoteCommandCenter sharedCommandCenter];
        
        commandCenter_playCommand_id = [commandCenter.playCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent * _Nonnull event) {
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.remote.command.play");
            [[AVAudioSession sharedInstance] setActive:YES error:nil];
            return MPRemoteCommandHandlerStatusSuccess;
        }];
        
        commandCenter_pauseCommand_id = [commandCenter.pauseCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent * _Nonnull event) {
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.remote.command.pause");
            [[AVAudioSession sharedInstance] setActive:NO error:nil];
            return MPRemoteCommandHandlerStatusSuccess;
        }];
        
        commandCenter_nextTrackCommand_id = [commandCenter.nextTrackCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent * _Nonnull event) {
            ABPlayer::getInstance().next();
            return MPRemoteCommandHandlerStatusSuccess;
        }];
        [commandCenter.nextTrackCommand setEnabled:YES];
        
        commandCenter_previousTrackCommand_id = [commandCenter.previousTrackCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent * _Nonnull event) {
            ABPlayer::getInstance().back();
            return MPRemoteCommandHandlerStatusSuccess;
        }];
        [commandCenter.previousTrackCommand setEnabled:YES];
        
        commandCenter_changePlaybackPositionCommand_id = [commandCenter.changePlaybackPositionCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent * _Nonnull event) {
            auto change_position_event = (MPChangePlaybackPositionCommandEvent *) event;
            auto time_interval = new double;
            * time_interval = [change_position_event positionTime];
            cocos2d::EventCustom e("event.remote.command.seek");
            e.setUserData(time_interval);
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
            ms::playlist::ABPlayer::getInstance().seekToTime(*time_interval);
            
            NSMutableDictionary * now_playing_info = [[[MPNowPlayingInfoCenter defaultCenter] nowPlayingInfo] mutableCopy];
            now_playing_info[MPNowPlayingInfoPropertyElapsedPlaybackTime] = [NSNumber numberWithDouble:[change_position_event positionTime]];
            [[MPNowPlayingInfoCenter defaultCenter] setNowPlayingInfo:now_playing_info];
            
            delete time_interval;
            return MPRemoteCommandHandlerStatusSuccess;
        }];
        [commandCenter.changePlaybackPositionCommand setEnabled:YES];
#endif
    }
    
    void setNowPlayingInfo(mj::model::AudiobookInfo info, double elapsed_time) {
        // hieunt: nếu là tài khoản trial thì không cho hiện player ở màn hình LOCK của iPhone
        if (!CONFIG_MANAGER->isAppActive()) {
            return;
        }
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        auto full_cover_path = cocos2d::FileUtils::getInstance()->fullPathForFilename(FileUtils::getInstance()->getWritablePath() + "imgcache/" + mj::helper::getFilenameFromURL(info.thumbnail_url));
        UIImage * cover = [UIImage imageWithContentsOfFile:[NSString stringWithUTF8String:full_cover_path.c_str()]];
        MPMediaItemArtwork * artwork = nil;
        if (cover != nil)
            artwork = [[MPMediaItemArtwork alloc] initWithImage: cover];
        
        MPNowPlayingInfoCenter * center = [MPNowPlayingInfoCenter defaultCenter];

        NSMutableDictionary * nowPlayingInfo = [@{
                                                  MPMediaItemPropertyTitle: [NSString stringWithUTF8String:info.name.c_str()],
                                                  MPMediaItemPropertyArtist: [NSString stringWithUTF8String:info.extra.c_str()],
                                                  MPMediaItemPropertyPlaybackDuration: [NSNumber numberWithFloat:info.duration / 1000.f],
                                                  MPNowPlayingInfoPropertyElapsedPlaybackTime: [NSNumber numberWithFloat:elapsed_time],
                                                  } mutableCopy];
        
        if (artwork != nil) {
            nowPlayingInfo[MPMediaItemPropertyArtwork] = artwork;
        }
        
        [center setNowPlayingInfo: nowPlayingInfo];
#endif
    }
    
    void resetNowPlayingInfo() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//        [[MPNowPlayingInfoCenter defaultCenter] setNowPlayingInfo:nil];
#endif
    }
    
    ABPlayer::ABPlayer() {
        [PlaylistImp sharedInstance];
    }
    
    void ABPlayer::init() {
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    bool ABPlayer::isAudioSessionInterrupted() {
        return [[PlaylistImp sharedInstance] isAudioSessionInterrupted] == YES ? true : false;
    }
#endif
    
    int ABPlayer::getCurrentPlayingId() {
        return [[PlaylistImp sharedInstance] getCurrentItemId];
    }
    
    void ABPlayer::enqueue(mj::model::AudiobookInfo info) {
        CCLOG("enqueue audiobook: %s", info.name.c_str());
        
        auto url = info.audio_url;
        auto local = cocos2d::FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(url);
        
        NSString * item;
        
        if (cocos2d::FileUtils::getInstance()->isFileExist(local)) {
            item = [[NSURL fileURLWithPath:[NSString stringWithUTF8String:local.c_str()]] absoluteString];
        } else {
            item = [NSString stringWithUTF8String:url.c_str()];
        }
        
        [[PlaylistImp sharedInstance] queue:item withAudiobookID:info.book_id];
        _queue.push_back(info);
    }
    
    void ABPlayer::play() {
//        [[UIApplication sharedApplication] beginReceivingRemoteControlEvents];
    }
    
    void ABPlayer::playFromBookId(int book_id, bool first_time, MSAudiobookPlayer::MSAudiobookPlayerType i_type) {
        
    }
    
    void ABPlayer::resume() {
        [[PlaylistImp sharedInstance] resume];
    }
    
    void ABPlayer::pause() {
        [[PlaylistImp sharedInstance] pause];
    }
    
    void ABPlayer::beginReceivingRemoteEvents() {
        [[UIApplication sharedApplication] beginReceivingRemoteControlEvents];
    }
    
    void ABPlayer::endReceivingRemoteEvents() {
        [[UIApplication sharedApplication] endReceivingRemoteControlEvents];
    }
    
    void ABPlayer::stop(std::function<void()> callback) {
        [[PlaylistImp sharedInstance] stop];
        _queue.clear();
        _stop_callback = callback;
        resetNowPlayingInfo();
    }
    
    void ABPlayer::next() {
        auto index = [[PlaylistImp sharedInstance] getCurrentItemId];
        stop();
        auto it = std::find_if(_original_queue.begin(), _original_queue.end(), [index](mj::model::AudiobookInfo a) {
            return a.book_id == index;
        });
        if (it == _original_queue.end()) {
            endReceivingRemoteEvents();
            return;
        }
        it++;
        if (it == _original_queue.end()) {
            endReceivingRemoteEvents();
            return;
        }
        for (; it != _original_queue.end(); ++it) {
            enqueue(*it);
        }
    }
    
    void ABPlayer::back() {
        auto index = [[PlaylistImp sharedInstance] getCurrentItemId];
        stop();
        auto it = std::find_if(_original_queue.begin(), _original_queue.end(), [index](mj::model::AudiobookInfo a) {
            return a.book_id == index;
        });
        
        if (it == _original_queue.begin()) {
            return;
        }
        
        it--;
        for (; it != _original_queue.end(); ++it) {
            enqueue(*it);
        }
    }
    
    double ABPlayer::getCurrentProgress() {
        return [[PlaylistImp sharedInstance] getCurrentProgress];
    }
    
    void ABPlayer::seekToTime(double timestamp) {
        [[PlaylistImp sharedInstance] seek:timestamp];
    }
    
    ABState ABPlayer::getState() {
        return (ABState) [[PlaylistImp sharedInstance] getState];
    }
    
    bool ABPlayer::isBackgroundState() {
        return [[UIApplication sharedApplication] applicationState] == UIApplicationStateBackground || [[UIApplication sharedApplication] applicationState] == UIApplicationStateInactive;
    }
    
    void ABPlayer::resetPlayer() {
        [[PlaylistImp sharedInstance] stop];
    }
    
    void ABPlayer::replaceSourceAtIndex(int index, std::string source) {
        
    }

    void ABPlayer::clearPlaylist() {
    }
}}
