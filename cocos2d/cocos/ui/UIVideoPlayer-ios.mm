/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "ui/UIVideoPlayer.h"

// No Available on tvOS
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS && !defined(CC_TARGET_OS_TVOS)

using namespace cocos2d::experimental::ui;
//-------------------------------------------------------------------------------------

#include "platform/ios/CCEAGLView-ios.h"
#import <AVKit/AVPlayerViewController.h>
#import <CoreMedia/CMTime.h>
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

@interface UIVideoViewWrapperIos : NSObject

typedef NS_ENUM(NSInteger, PlayerbackState) {
    PlayerbackStateUnknown = 0,
    PlayerbackStatePaused,
    PlayerbackStopped,
    PlayerbackStatePlaying,
    PlayerbackStateCompleted
};

@property (strong, nonatomic)  AVPlayerViewController * playerController;
@property (readwrite, nonatomic) std::unordered_map<int,std::pair<float,float>> *_tracks;
@property (readwrite, nonatomic) int32_t _timeScale;
@property (readwrite, nonatomic) AVMutableCompositionTrack *_compositionVideoTrack;

- (void) setFrame:(int) left :(int) top :(int) width :(int) height;
- (void) setURL:(int) videoSource :(std::string&) videoUrl;
- (void) setURLs:(std::vector<std::pair<int,std::string>>&) videoUrls;
- (void) setMuteAudio:(BOOL) isMuteAudio;
- (void) setAVMutableCompositionTrack:(AVMutableCompositionTrack*) compositionAudioTrack;
- (AVMutableCompositionTrack*) getAVMutableCompositionTrack;
- (void) play;
- (void) pause;
- (void) resume;
- (void) stop;
- (void) seekTo:(float) sec;
- (void) seekTo:(float) sec :(CMTime)toleranceBefore :(CMTime)toleranceAfter;
- (float)currentTime;
- (float)duration;
- (std::unordered_map<int,std::pair<float,float>>) getInfoTracks;
- (void) setRotate:(float) degree;
- (void) setCornerRadius:(float) radius;
- (void) setVisible:(BOOL) visible;
- (void) setKeepRatioEnabled:(BOOL) enabled;
- (void) setFullScreenEnabled:(BOOL) enabled;
- (BOOL) isFullScreenEnabled;
- (void) showPlaybackControls:(BOOL) value;
- (void) setRepeatEnabled:(BOOL)enabled;
- (void) setUserInteractionEnabled:(BOOL)userInteractionEnabled;
//- (void) setLoopEnabled:(BOOL) isLoop;
//- (void) setCloseButtonEnabled:(BOOL) is_enabled withType:(int) type andTile:(float) tile;


-(id) init:(void*) videoPlayer;
-(void) videoFinished:(NSNotification*) notification;

@end

@implementation UIVideoViewWrapperIos
{
    int _left;
    int _top;
    int _width;
    int _height;
    BOOL _keepRatioEnabled;
    BOOL _repeatEnabled;
    BOOL _showPlaybackControls;
    BOOL _userInteractionEnabled;
    PlayerbackState _state;
    VideoPlayer* _videoPlayer;
}

-(id)init:(void*)videoPlayer
{
    if (self = [super init]) {
        self.playerController = [[AVPlayerViewController new] autorelease];

        [self setRepeatEnabled:FALSE];
        [self showPlaybackControls:FALSE];
        [self setUserInteractionEnabled:FALSE];
        [self setKeepRatioEnabled:FALSE];
        
        _videoPlayer = (VideoPlayer*)videoPlayer;
        _state = PlayerbackStateUnknown;
        
        self._tracks = new std::unordered_map<int,std::pair<float,float>>();
        
        self.playerController.view.backgroundColor = [UIColor whiteColor];
    }

    return self;
}

-(void) dealloc
{
    [self clean];
    [self.playerController release];
    
    if(self._tracks) {
    
        delete self._tracks;
        
        self._tracks = nullptr;
    }
    
    [super dealloc];
    
}

-(void) clean
{
    [self stop];
    [self removePlayerEventListener];
    [self.playerController.view removeFromSuperview];
}

-(void) setFrame:(int)left :(int)top :(int)width :(int)height
{
    _left = left;
    _width = width;
    _top = top;
    _height = height;
    [self.playerController.view setFrame:CGRectMake(left, top, width, height)];
}

-(void) setFullScreenEnabled:(BOOL) enabled
{
    // AVPlayerViewController doesn't provide API to enable fullscreen. But you can toggle
    // fullsreen by the playback controllers.
}

-(BOOL) isFullScreenEnabled
{
    return false;
}


-(void) showPlaybackControls:(BOOL)value
{
    _showPlaybackControls = value;
    self.playerController.showsPlaybackControls = value;
}

-(void) setRepeatEnabled:(BOOL)enabled
{
    _repeatEnabled = enabled;
    if (self.playerController.player) {
        if (_repeatEnabled)
            self.playerController.player.actionAtItemEnd = AVPlayerActionAtItemEndNone;
        else
            self.playerController.player.actionAtItemEnd = AVPlayerActionAtItemEndPause;
    }
}

-(void) setUserInteractionEnabled:(BOOL)userInteractionEnabled
{
    _userInteractionEnabled = userInteractionEnabled;
    self.playerController.view.userInteractionEnabled = _userInteractionEnabled;
}
- (void) setMuteAudio:(BOOL) isMuteAudio
{
    if (@available(iOS 13.0, *)) {
        if(isMuteAudio)
            [self getAVMutableCompositionTrack].enabled = YES;
        else
            [self getAVMutableCompositionTrack].enabled = NO;
    } else {
        // Fallback on earlier versions
    }
}
- (void) setAVMutableCompositionTrack:(AVMutableCompositionTrack*) compositionAudioTrack
{
    self._compositionVideoTrack = compositionAudioTrack;
}
- (AVMutableCompositionTrack*) getAVMutableCompositionTrack{
    return self._compositionVideoTrack;
}


- (void) setURLs:(std::vector<std::pair<int,std::string>>&) videoUrls {
    
    AVMutableComposition * movie = [AVMutableComposition composition];
    AVMutableCompositionTrack *compositionVideoTrack = [movie addMutableTrackWithMediaType:AVMediaTypeVideo preferredTrackID:kCMPersistentTrackID_Invalid];
    AVMutableCompositionTrack *compositionAudioTrack = [movie addMutableTrackWithMediaType:AVMediaTypeAudio preferredTrackID:kCMPersistentTrackID_Invalid];
    [self setAVMutableCompositionTrack:compositionAudioTrack];
    CMTime offset = kCMTimeZero;
        
    self._tracks->clear();
    
    self._timeScale = 1000;
    
    for (auto trackName:videoUrls)
    {
        auto path =   cocos2d::FileUtils::getInstance()->fullPathForFilename(trackName.second);
        
        CCLOG("UIViewPlayer setURLs url:%s",path.c_str());
        
        if(path.empty()){
            continue;
        }
        
        NSString *nsStringfullPath= [[NSString alloc] initWithCString:path.c_str() encoding:NSUTF8StringEncoding];
        
        NSURL *assetURL = [NSURL fileURLWithPath:nsStringfullPath];
        
        AVURLAsset *asset = [AVURLAsset assetWithURL:assetURL];
        
        AVAssetTrack *assetVideoTrack = [asset tracksWithMediaType:AVMediaTypeVideo].firstObject;
        
        AVAssetTrack *assetAudioTrack = [asset tracksWithMediaType:AVMediaTypeAudio].firstObject;
        
        CMTime assetDuration = asset.duration;

        CMTime videoTime = assetVideoTrack.timeRange.duration;
        
        CMTime audioTime = assetAudioTrack.timeRange.duration;

        float vTime  =CMTimeGetSeconds(videoTime);
        
        float aTime  =CMTimeGetSeconds(audioTime);

        float assDuration  = CMTimeGetSeconds(assetDuration);

        float minDuration = std::min(aTime,vTime);
        
        assetDuration = CMTimeMakeWithSeconds(vTime,videoTime.timescale);
        
        self._timeScale = videoTime.timescale;
        
        NSLog(@"UIViewPlayer assetDuration duration : %f , videoTime : %f, audioTime : %f ,minDuration %f videoTime.timescale:%d", assDuration,vTime, aTime,minDuration,videoTime.timescale);
        
        CMTimeRange timeRange = CMTimeRangeMake(kCMTimeZero, assetDuration);
        NSError * error = nil;
        
        if (![compositionVideoTrack insertTimeRange: timeRange ofTrack: assetVideoTrack atTime: offset error: &error]) {
            NSLog(@"UIViewPlayer Error adding video track - %@", error);
        }
        
        if (![compositionAudioTrack insertTimeRange: timeRange ofTrack: assetAudioTrack atTime: offset error: &error]) {
            NSLog(@"UIViewPlayer Error adding audio track - %@", error);
        }
        
        auto addOffset = CMTimeAdd(offset , assetDuration);
  
        auto pair =  std::make_pair(CMTimeGetSeconds(offset),CMTimeGetSeconds(addOffset));
        
        auto mapPair = make_pair(trackName.first,pair);
        
        self._tracks->insert(mapPair);
        
        offset = addOffset;
    }
    
    AVPlayerItem* playerItem = [[AVPlayerItem alloc] initWithAsset:movie];
    
    self.playerController.player = [[AVPlayer alloc] initWithPlayerItem:playerItem];

    [self setRepeatEnabled:_repeatEnabled];
    [self setKeepRatioEnabled:_keepRatioEnabled];
    [self setUserInteractionEnabled:_userInteractionEnabled];
    [self showPlaybackControls:_showPlaybackControls];
    //self.playerController.view.transform = CGAffineTransform(rotationAngle: .pi/2.0);
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    auto eaglview = (CCEAGLView *) view->getEAGLView();
    [eaglview addSubview:self.playerController.view];
    [self registerPlayerEventListener];
}

-(void) setURL:(int)videoSource :(std::string &)videoUrl
{
    [self clean];

    if (videoSource == 1)
        self.playerController.player = [[[AVPlayer alloc] initWithURL:[NSURL URLWithString:@(videoUrl.c_str())]] autorelease];
    else
        self.playerController.player = [[[AVPlayer alloc] initWithURL:[NSURL fileURLWithPath:@(videoUrl.c_str())]] autorelease];

    [self setRepeatEnabled:_repeatEnabled];
    [self setKeepRatioEnabled:_keepRatioEnabled];
    [self setUserInteractionEnabled:_userInteractionEnabled];
    [self showPlaybackControls:_showPlaybackControls];

    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    auto eaglview = (CCEAGLView *) view->getEAGLView();
    [eaglview addSubview:self.playerController.view];
    [self registerPlayerEventListener];
}

-(void) videoFinished:(NSNotification *)notification
{
    if(_videoPlayer != nullptr) {
        _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::COMPLETED);
        _state = PlayerbackStateCompleted;

        if (_repeatEnabled) {
            [self seekTo:0];
            [self play];
        }
    }
}


-(void) seekTo:(float)sec
{
    if(!self._tracks->empty()) {
        
        if (self.playerController.player) {
            [self.playerController.player seekToTime:CMTimeMakeWithSeconds(sec, self._timeScale)];
        }
        
        return;
    }
    
    if (self.playerController.player)
        [self.playerController.player seekToTime:CMTimeMake(sec, 1)];
    
}

- (void) seekTo:(float)sec :(CMTime)toleranceBefore :(CMTime)toleranceAfter
{
    if(!self._tracks->empty()) {
        
        if (self.playerController.player) {
            [self.playerController.player seekToTime:CMTimeMakeWithSeconds(sec, self._timeScale) toleranceBefore:toleranceBefore toleranceAfter:toleranceAfter];
        }
        
        return;
    }
    
    if (self.playerController.player)
        [self.playerController.player seekToTime:CMTimeMake(sec, 1) toleranceBefore:toleranceBefore toleranceAfter:toleranceAfter];
    
}

- (float)currentTime {
    if (self.playerController.player)
        return CMTimeGetSeconds([self.playerController.player currentTime]);

    return -1;
}

- (float)duration {
    
    if (self.playerController.player) {
        
        CMTime vv = self.playerController.player.currentItem.asset.duration;
        
        return CMTimeGetSeconds(vv);
    }


    return -1;
    
}

-(std::unordered_map<int,std::pair<float, float>>)getInfoTracks
{
    return *(self._tracks);
}

- (void) setRotate:(float)degree
{
    if (self.playerController.player) {
      
        [self.playerController.view.layer setAffineTransform:CGAffineTransformMakeRotation(M_PI * degree/180.0)];
    }
}

-(void) setCornerRadius:(float)radius
{
    if (self.playerController.player) {
      
        self.playerController.view.layer.cornerRadius  = radius;
        self.playerController.view.layer.masksToBounds = true;
    }
}


-(void) setVisible:(BOOL)visible
{
    [self.playerController.view setHidden:!visible];
}

-(void) setKeepRatioEnabled:(BOOL)enabled
{
    _keepRatioEnabled = enabled;
    if (_keepRatioEnabled)
        self.playerController.videoGravity = AVLayerVideoGravityResizeAspect;
    else
        self.playerController.videoGravity = AVLayerVideoGravityResizeAspectFill;
}

-(void) play
{
    if (self.playerController.player && _state != PlayerbackStatePlaying) {
        [self.playerController.player play];
        _state = PlayerbackStatePlaying;
        _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::PLAYING);
    }
}

-(void) pause
{
    if (self.playerController.player && _state == PlayerbackStatePlaying) {
        [self.playerController.player pause];
        _state = PlayerbackStatePaused;
        _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::PAUSED);
    }
}

-(void) resume
{
    CCLOG("VideoPlayer resume objet-c start");

    if (self.playerController.player && _state == PlayerbackStatePaused) {
        
        CCLOG("VideoPlayer resume objet-c play dc roi");

        [self play];
    }
    CCLOG("VideoPlayer resume objet-c end");

}

-(void) stop
{
    // AVPlayer doesn't have stop, so just pause it, and seek time to 0.
    if (self.playerController.player && _state != PlayerbackStopped) {
        [self seekTo:0];
        [self.playerController.player pause];
        _state = PlayerbackStopped;
        _videoPlayer->onPlayEvent((int)VideoPlayer::EventType::STOPPED);
    }
}

-(void) registerPlayerEventListener
{
    if (self.playerController.player)
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(videoFinished:)
                                                 name:AVPlayerItemDidPlayToEndTimeNotification
                                                 object:self.playerController.player.currentItem];
}

-(void) removePlayerEventListener
{
    if (self.playerController.player)
        [[NSNotificationCenter defaultCenter] removeObserver:self
                                              name:AVPlayerItemDidPlayToEndTimeNotification
                                              object:self.playerController.player.currentItem];
}

@end
//------------------------------------------------------------------------------------------------------------

VideoPlayer::VideoPlayer()
: _isPlaying(false)
, _fullScreenDirty(false)
, _fullScreenEnabled(false)
, _keepAspectRatioEnabled(false)
, _videoPlayerIndex(-1)
, _eventCallback(nullptr)
, _isLooping(false)
, _isUserInputEnabled(true)
, _styleType(StyleType::DEFAULT)
{
    _videoView = [[UIVideoViewWrapperIos alloc] init:this];

#if CC_VIDEOPLAYER_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif
}

VideoPlayer::~VideoPlayer()
{
    if(_videoView)
    {
        [((UIVideoViewWrapperIos*)_videoView) dealloc];
    }
}

void VideoPlayer::setFileName(const std::string& fileName)
{
    _videoURL = FileUtils::getInstance()->fullPathForFilename(fileName);
    _videoSource = VideoPlayer::Source::FILENAME;
    [((UIVideoViewWrapperIos*)_videoView) setURL:(int)_videoSource :_videoURL];
}

void VideoPlayer::setURL(const std::string& videoUrl)
{
    _videoURL = videoUrl;
    _videoSource = VideoPlayer::Source::URL;
    [((UIVideoViewWrapperIos*)_videoView) setURL:(int)_videoSource :_videoURL];
}

void VideoPlayer::setURLs(const std::vector<std::pair<int,std::string>>& videoURLs)
{
    _videoURLs = videoURLs;
    _videoSource = VideoPlayer::Source::URLs;
    [((UIVideoViewWrapperIos*)_videoView) setURLs:_videoURLs];
}
void VideoPlayer::setMuteAudio(bool isMuteAudio)
{
    [((UIVideoViewWrapperIos*)_videoView) setMuteAudio:isMuteAudio];
}
void VideoPlayer::setLooping(bool looping)
{
    _isLooping = looping;
    [((UIVideoViewWrapperIos*)_videoView) setRepeatEnabled:_isLooping];
}

void VideoPlayer::setUserInputEnabled(bool enableInput)
{
    _isUserInputEnabled = enableInput;
    [((UIVideoViewWrapperIos*)_videoView) setUserInteractionEnabled:enableInput];
}

void VideoPlayer::setStyle(StyleType style)
{
    _styleType = style;

    switch (style) {
        case StyleType::DEFAULT:
            [((UIVideoViewWrapperIos*)_videoView) showPlaybackControls:TRUE];
            break;

        case StyleType::NONE:
            [((UIVideoViewWrapperIos*)_videoView) showPlaybackControls:FALSE];
            break;
    }
}

void VideoPlayer::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    cocos2d::ui::Widget::draw(renderer,transform,flags);

    if (flags & FLAGS_TRANSFORM_DIRTY)
    {
        auto directorInstance = Director::getInstance();
        auto glView = directorInstance->getOpenGLView();
        auto frameSize = glView->getFrameSize();
        auto scaleFactor = [static_cast<CCEAGLView *>(glView->getEAGLView()) contentScaleFactor];

        auto winSize = directorInstance->getWinSize();

        auto leftBottom = convertToWorldSpace(Vec2::ZERO);
        auto rightTop = convertToWorldSpace(Vec2(_contentSize.width,_contentSize.height));

        auto uiLeft = (frameSize.width / 2 + (leftBottom.x - winSize.width / 2 ) * glView->getScaleX()) / scaleFactor;
        auto uiTop = (frameSize.height /2 - (rightTop.y - winSize.height / 2) * glView->getScaleY()) / scaleFactor;

        [((UIVideoViewWrapperIos*)_videoView) setFrame :uiLeft :uiTop
                                                          :(rightTop.x - leftBottom.x) * glView->getScaleX() / scaleFactor
                                                          :( (rightTop.y - leftBottom.y) * glView->getScaleY()/scaleFactor)];
    }

#if CC_VIDEOPLAYER_DEBUG_DRAW
    _debugDrawNode->clear();
    auto size = getContentSize();
    Point vertices[4]=
    {
        Point::ZERO,
        Point(size.width, 0),
        Point(size.width, size.height),
        Point(0, size.height)
    };
    _debugDrawNode->drawPoly(vertices, 4, true, Color4F(1.0, 1.0, 1.0, 1.0));
#endif
}

bool VideoPlayer::isFullScreenEnabled()const
{
    return [((UIVideoViewWrapperIos*)_videoView) isFullScreenEnabled];
}

void VideoPlayer::setFullScreenEnabled(bool enabled)
{
    [((UIVideoViewWrapperIos*)_videoView) setFullScreenEnabled:enabled];
}

void VideoPlayer::setKeepAspectRatioEnabled(bool enable)
{
    if (_keepAspectRatioEnabled != enable)
    {
        _keepAspectRatioEnabled = enable;
        [((UIVideoViewWrapperIos*)_videoView) setKeepRatioEnabled:enable];
    }
}

void VideoPlayer::play(bool is_loop)
{
    if (! _videoURL.empty())
    {
        [((UIVideoViewWrapperIos*)_videoView) play];
    }
    setLooping(is_loop);
}

void VideoPlayer::play()
{
    if((_videoURL.empty()&&_videoSource != VideoPlayer::Source::URLs) || ( _videoURLs.empty() && _videoSource == VideoPlayer::Source::URLs)) {
        CCLOG("Cannot play!");
        return;
    }
    
    [((UIVideoViewWrapperIos*)_videoView) play];
}

void VideoPlayer::pause()
{
    if((_videoURL.empty()&&_videoSource != VideoPlayer::Source::URLs) || ( _videoURLs.empty() && _videoSource == VideoPlayer::Source::URLs)) {
        CCLOG("Cannot pause!");
        return;
    }
    
   [((UIVideoViewWrapperIos*)_videoView) pause];
}

void VideoPlayer::resume()
{
    
    CCLOG("VideoPlayer resume!");
    
    if((_videoURL.empty()&&_videoSource != VideoPlayer::Source::URLs) || ( _videoURLs.empty() && _videoSource == VideoPlayer::Source::URLs)) {
        CCLOG("VideoPlayer Cannot resume!");
        return;
    }
    
    [((UIVideoViewWrapperIos*)_videoView) resume];
}

void VideoPlayer::stop()
{
    if((_videoURL.empty()&&_videoSource != VideoPlayer::Source::URLs) || ( _videoURLs.empty() && _videoSource == VideoPlayer::Source::URLs)) {
        CCLOG("Cannot stop!");
        return;
    }
    
    [((UIVideoViewWrapperIos*)_videoView) stop];
}

void VideoPlayer::seekTo(float sec)
{
   [((UIVideoViewWrapperIos*)_videoView) seekTo:sec];
}

void VideoPlayer::seekWithoutDelay(float sec)
{
   [((UIVideoViewWrapperIos*)_videoView) seekTo:sec:kCMTimeZero:kCMTimeZero];
}

void VideoPlayer::setLoopEnabled(bool is_loop) {
//    [((UIVideoViewWrapperIos*)_videoView) setLoopEnabled: is_loop];
}

void VideoPlayer::setCloseButtonEnabled(bool is_enabled, int type, float tile) {
//    [((UIVideoViewWrapperIos *) _videoView) setCloseButtonEnabled:is_enabled withType:type andTile:tile];
}

bool VideoPlayer::isPlaying() const
{
    return _isPlaying;
}

bool VideoPlayer::isLooping() const
{
    return _isLooping;
}

bool VideoPlayer::isUserInputEnabled() const
{
    return _isUserInputEnabled;
}

void VideoPlayer::setVisible(bool visible)
{
    cocos2d::ui::Widget::setVisible(visible);

    if (!visible)
    {
        [((UIVideoViewWrapperIos*)_videoView) setVisible:NO];
    }
    else if(isRunning())
    {
        [((UIVideoViewWrapperIos*)_videoView) setVisible:YES];
    }
}

void VideoPlayer::onEnter()
{
    Widget::onEnter();
    if (isVisible())
    {
        [((UIVideoViewWrapperIos*)_videoView) setVisible: YES];
    }
}

void VideoPlayer::onExit()
{
    Widget::onExit();
    [((UIVideoViewWrapperIos*)_videoView) setVisible: NO];
}

void VideoPlayer::addEventListener(const VideoPlayer::ccVideoPlayerCallback& callback)
{
    _eventCallback = callback;
}

float VideoPlayer::getCurrentTime() const {
    
   return [((UIVideoViewWrapperIos *)_videoView) currentTime];
}

float VideoPlayer::getDuration() const {
    
   return [((UIVideoViewWrapperIos *)_videoView) duration];
}

void VideoPlayer::setRotate(float degree) {
            
   return [((UIVideoViewWrapperIos *)_videoView) setRotate:degree];
}

void VideoPlayer::setCornerRadius(float radius) {
            
   return [((UIVideoViewWrapperIos *)_videoView) setCornerRadius:radius];
}

std::unordered_map<int, std::pair<float,float>> VideoPlayer::getInfoTracks() {
            
        return [((UIVideoViewWrapperIos *)_videoView) getInfoTracks];
}

void VideoPlayer::onPlayEvent(int event)
{
    if (event == (int)VideoPlayer::EventType::PLAYING) {
        _isPlaying = true;
    } else {
        _isPlaying = false;
    }

    if (_eventCallback)
    {
        _eventCallback(this, (VideoPlayer::EventType)event);
    }
}

cocos2d::ui::Widget* VideoPlayer::createCloneInstance()
{
    return VideoPlayer::create();
}

void VideoPlayer::copySpecialProperties(Widget *widget)
{
    VideoPlayer* videoPlayer = dynamic_cast<VideoPlayer*>(widget);
    if (videoPlayer)
    {
        _isPlaying = videoPlayer->_isPlaying;
        _isLooping = videoPlayer->_isLooping;
        _isUserInputEnabled = videoPlayer->_isUserInputEnabled;
        _styleType = videoPlayer->_styleType;
        _fullScreenEnabled = videoPlayer->_fullScreenEnabled;
        _fullScreenDirty = videoPlayer->_fullScreenDirty;
        _videoURL = videoPlayer->_videoURL;
        _keepAspectRatioEnabled = videoPlayer->_keepAspectRatioEnabled;
        _videoSource = videoPlayer->_videoSource;
        _videoPlayerIndex = videoPlayer->_videoPlayerIndex;
        _eventCallback = videoPlayer->_eventCallback;
        _videoView = videoPlayer->_videoView;
    }
}

#endif
