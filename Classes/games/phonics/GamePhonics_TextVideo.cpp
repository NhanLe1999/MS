
#include "GamePhonics_TextVideo.h"
#include "HSAudioEngine.h"
#include "NativeBridge.h"
#include "MJDefault.h"
INITIALIZE_READER(MJGamePhonics_TextVideo);
#include "platform/CCPlatformConfig.h"
USING_NS_CC;
#define CSB_NAME "csb/game/phonics/GamePhonics_TextVideo.csb"
#define opacity_hide 120
#define record_img_path "games/phonics/phonics_btrecord.png"
#define play_img_path "games/phonics/phonics_btplay.png"
#define stop_img_path "games/phonics/phonics_btstop.png"
#define next_img_path "games/phonics/phonics_btnext.png"
#define arrow_img_path "games/phonics/phonics_btnext_arrow.png"
#include "AskPermissionPopup.h"
#include "MJDefinitions.h"
#include "AskPermissionPopup.h"

MJGamePhonics_TextVideo * MJGamePhonics_TextVideo::createGame(ObjectInfo _object) {
    auto p = static_cast<MJGamePhonics_TextVideo *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(_object);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGamePhonics_TextVideo::didLoadFromCSB(ObjectInfo _object) {
    
    _logLayer = ui::ImageView::create("bg.jpg");
    _logLayer->setVisible(false);
    _logLayer->setEnabled(true);
    _logLayer->setAnchorPoint(Vec2(0.5, 0.5));
    _logLayer->setContentSize(Director::getInstance()->getRunningScene()->getBoundingBox().size);
    _logLayer->setPosition(Director::getInstance()->getRunningScene()->getBoundingBox().size / 2);
    _logLayer->setTouchEnabled(true);
    
    _logLayer->setOpacity(0.0f);
    
    _logLayer->addClickEventListener([=] (Ref * sender) {
        CCLOG("Nhanh nenennenenennene");
    });
    
    Director::getInstance()->getRunningScene()->addChild(_logLayer, INT_MAX);
    
    _object_info = _object;
    cocos2d::utils::findChild(this, "bg_layout")->setCascadeOpacityEnabled(false);
    text_highlight = Label::createWithTTF(_object.text, "fonts/monkeystories_ultra.ttf", 120);
    text_highlight->setColor(Color3B::BLACK);
    auto text_layout = cocos2d::utils::findChild(this, "text_layout");
    text_layout->addChild(text_highlight);
    text_highlight->setPosition(Vec2(text_layout->getContentSize().width/2, text_layout->getContentSize().height/2+20));
    text_highlight->setScale(0);
    text_highlight->setVisible(false);
    scale_value = 1.0f;
    if (text_highlight->getContentSize().width > text_layout->getContentSize().width - 40) {
        scale_value = (text_layout->getContentSize().width - 40)*1.0/text_highlight->getContentSize().width;
    }
    
    television_gaf = GAFWrapper::createGAF("gaf/phonics/phonic_on_off/phonic_on_off.gaf");
    text_layout->addChild(television_gaf, 10000);
    television_gaf->setPosition(text_layout->getContentSize()/2);
    television_gaf->setVisible(false);
    
    btplay = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "play_layout");
    btrecord = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "record_layout");
    btnext = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "next_layout");

    btplay->setCascadeOpacityEnabled(true);
    btrecord->setCascadeOpacityEnabled(true);
    btnext->setCascadeOpacityEnabled(true);
    
    auto img_record = cocos2d::ui::ImageView::create(record_img_path);
    btrecord->addChild(img_record);
    img_record->setPosition(btrecord->getContentSize()/2);
    img_record->setName("img_record");
    
    auto img_play = cocos2d::ui::ImageView::create(play_img_path);
    btplay->addChild(img_play);
    img_play->setPosition(btrecord->getContentSize()/2);
    
    auto next_img = cocos2d::ui::ImageView::create(next_img_path);
    btnext->addChild(next_img);
    next_img->setPosition(btnext->getContentSize()/2);
    auto arrow = cocos2d::ui::ImageView::create(arrow_img_path);
    next_img->addChild(arrow);
    next_img->setCascadeOpacityEnabled(true);
    arrow->setPosition(Vec2(next_img->getContentSize().width*0.8, next_img->getContentSize().height/2));
    auto text_next = cocos2d::ui::Text::create("Next", "fonts/monkeystories_ultra.ttf", 30);
    next_img->addChild(text_next);
    text_next->setPosition(Vec2((arrow->getPositionX() - arrow->getContentSize().width/2)/2, next_img->getContentSize().height/2));
    
    this->runLayoutDisplay(btplay, false);
    this->runLayoutDisplay(btrecord, false);
    this->runLayoutDisplay(btnext, false);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_app_active_again, [this](EventCustom *) {
        this->playVideo();
    }), this);
}

void MJGamePhonics_TextVideo::showText(){
    text_highlight->setVisible(true);
    text_highlight->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5f, scale_value)), CallFunc::create([=]{
        //text_highlight->runAction(Sequence::create(ScaleTo::create(0.1f, scale_value*1.2f), ScaleTo::create(0.1f, scale_value), NULL));
    }), NULL));
    auto audio_path = _object_info.audio_path;
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        if (delegate) {
            AUDIO_ENGINE->playEffect("sounds/phonics/phonics_showtext.mp3");
            delegate->onFinishAction(SHOW_TEXT_DONE);
        }
    });
}

void MJGamePhonics_TextVideo::hideText(){
    text_highlight->runAction(FadeOut::create(1.0f));
}

void MJGamePhonics_TextVideo::hightlightText(){
//    float duration = _object_info.list_highlight[0].time_start/1000.0f;
//    this->scheduleOnce([=](float dt){
//        runHighLightText();
//    }, duration, StringUtils::format("start_highlight"));
}

void MJGamePhonics_TextVideo::runHighLightText(){
//    this->unscheduleAllCallbacks();
//    if((_object_info.list_highlight.size()) == sync_index) {
//        text_highlight->setColor(Color3B::BLACK);
//        sync_index = 0;
//        _object_info.list_highlight.clear();
//        if (delegate) {
//            delegate->onFinishAction(HIGH_LIGHT_DONE);
//        }
//        return;
//    }
//    resetTextColor();
//
//    auto _highlight_info = _object_info.list_highlight[sync_index++];
//    for(int i = _highlight_info.index_start; i<= _highlight_info.index_end; i++) {
//        auto letter = text_highlight->getLetter(i);
//        if(letter != NULL) {
//            letter->setColor(Color3B::RED);
//        }
//    }
//    float duration = (_highlight_info.time_end - _highlight_info.time_start)/1000.0f;
//    this->runAction(Sequence::create(DelayTime::create(duration), CallFunc::create([=]{
//        runHighLightText();
//    }), NULL));
}

void MJGamePhonics_TextVideo::resetTextColor(){
    for (int i = 0; i < NativeBridge::utf8LengthOfString(text_highlight->getString()); i++) {
        auto letter = text_highlight->getLetter(i);
        if(letter != NULL) {
            letter->setColor(Color3B::BLACK);
        }
    }
}

void MJGamePhonics_TextVideo::onEnter(){
    cocos2d::ui::Layout::onEnter();
    addVideoWithPath(_object_info.video_path);
}

void MJGamePhonics_TextVideo::resetTextVideoData(ObjectInfo _object){
    _is_recording = false;
    _object_info = _object;
    text_highlight->setScale(0);
    text_highlight->setVisible(false);
    text_highlight->setOpacity(255);
    text_highlight->setString(_object_info.text);
    addVideoWithPath(_object_info.video_path);
    scale_value = 1.0f;
    auto text_layout = cocos2d::utils::findChild(this, "text_layout");
    if (text_highlight->getContentSize().width > text_layout->getContentSize().width - 40) {
        scale_value = (text_layout->getContentSize().width - 40)*1.0/text_highlight->getContentSize().width;
    }
    CCLOG("fuck off - %s", _object_info.text.c_str());
}

void MJGamePhonics_TextVideo::setDelegate(MJGamePhonics_TextVideoDelegate * delegate){
    this->delegate = delegate;
}

void MJGamePhonics_TextVideo::addVideoWithPath(std::string video_path){
    auto video_layout = cocos2d::utils::findChild(this, "text_layout");
    auto sceneSize = Director::getInstance()->getWinSize();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    videoPlayer = VideoPlayer::create();
//    videoPlayer->setFullScreenEnabled(true);
    videoPlayer->setPosition(video_layout->getContentSize()/2);
    videoPlayer->setContentSize(Size(631, 352));
    video_layout->addChild(videoPlayer);
    videoPlayer->setFileName(video_path.c_str());
    videoPlayer->addEventListener(CC_CALLBACK_2(MJGamePhonics_TextVideo::videoEventCallback, this));
    videoPlayer->setVisible(false);
#endif
    
}
 
void MJGamePhonics_TextVideo::playVideo(){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    if (!videoPlayer) {
        return;
    }
    videoPlayer->setVisible(true);
#endif
    television_gaf->setVisible(true);
    AUDIO_ENGINE->playEffect("sounds/phonics/phonics_ontv.mp3");
    text_highlight->setVisible(false);
    television_gaf->playSequence("on", false, [=] (gaf::GAFObject *obj, std::string name_sequence){

        //hightlightText();
    });
    television_gaf->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=]{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        CCLOG("play video");
//        videoPlayer->setLoopEnabled(true);
        if (videoPlayer)
            videoPlayer->play();
        
#endif
    }), NULL));
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

void MJGamePhonics_TextVideo::videoEventCallback(Ref* sender, VideoPlayer::EventType eventType)
{
    switch (eventType) {
        case VideoPlayer::EventType::PLAYING:
            CCLOG("PLAYING");
            break;
        case VideoPlayer::EventType::PAUSED:
            CCLOG("PAUSED");
            break;
        case VideoPlayer::EventType::STOPPED:
            CCLOG("STOPPED");
            break;
        case VideoPlayer::EventType::COMPLETED:
            CCLOG("COMPLETED");
            this->scheduleOnce([=](float dt){
                //videoCompleted();
                AUDIO_ENGINE->playEffect("sounds/phonics/phonics_offtv.mp3");
                text_highlight->setVisible(true);
                television_gaf->playSequence("off", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                    if (delegate) {
                        delegate->onFinishAction(VIDEO_DONE);
                    }
                    television_gaf->setVisible(false);
                });
                videoPlayer->removeFromParent();
                videoPlayer = nullptr;
            }, 0.05f, "video_complete_aaaaa");
            break;
        default:
            break;
    }
}
#endif

void MJGamePhonics_TextVideo::startRecord(){
    _logLayer->setVisible(true);
    auto text_layout = cocos2d::utils::findChild(this, "text_layout");
    if (_record) {
        text_layout->removeChildByName("_record");
        _record = nullptr;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(_recordCopy)
    {
        _recordCopy->removeFromParent();
    }
#endif
    _record = MSRecord::createRecord(5.0f, _object_info.text);//1.5*_object_info.audio_duration
    if(!_record) {
        return;
    }
    text_layout->addChild(_record);
    _record->setDelegate(this);
    _record->setPosition(text_layout->getContentSize()/2);
    _record->setName("_record");
    
    this->runLayoutDisplay(btrecord, false);
    this->runLayoutDisplay(btnext, false);
    btrecord->getChildByName<cocos2d::ui::ImageView *>("img_record")->loadTexture(stop_img_path);
    /*this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]{
        this->runLayoutDisplay(btrecord, true);
    }), NULL));*/
}

void MJGamePhonics_TextVideo::onFinishRecord(int finish_type,std::string url){
    
    _logLayer->setVisible(false);
    
    if (finish_type == RECORD_DONE){
        btrecord->stopAllActions();
        this->runLayoutDisplay(btplay, true);
        mj::helper::showTap(btplay->getParent()->convertToWorldSpace(btplay->getPosition()));
        this->runLayoutDisplay(btrecord, false);
        btrecord->getChildByName<cocos2d::ui::ImageView *>("img_record")->loadTexture(record_img_path);
        
    }else {//PlayDone
        mj::helper::showTap(btnext->getParent()->convertToWorldSpace(btnext->getPosition()));
        this->runLayoutDisplay(btnext, true);
        btrecord->setTouchEnabled(true);
        btrecord->runAction(FadeIn::create(0.2));
        btrecord->getChildByName<cocos2d::ui::ImageView *>("img_record")->loadTexture(record_img_path);
    }
    
    if (delegate) {
        delegate->onFinishAction(finish_type);
    }
}

void MJGamePhonics_TextVideo::deleteLayerLock()
{
    _logLayer->removeFromParent();
}

void MJGamePhonics_TextVideo::removeVideo() {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
	if (videoPlayer){
		videoPlayer->removeFromParent();
	}
    if (_record && _is_recording)
        _record->stopRecord();
#endif
    this->removeChildByName("_record");
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGamePhonics_TextVideo::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onPlay") {
        return CC_CALLBACK_1(MJGamePhonics_TextVideo::onPlay, this);
    }
    if (callback_name == "onRecord") {
        return CC_CALLBACK_1(MJGamePhonics_TextVideo::onRecord, this);
    }
    
    if (callback_name == "onNext") {
        return CC_CALLBACK_1(MJGamePhonics_TextVideo::onNext, this);
    }
    
    return nullptr;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void MJGamePhonics_TextVideo::onRecord(cocos2d::Ref *sender){
    bool checkPermissions = cocos2d::JniHelper::callStaticBooleanMethod("org.cocos2dx.cpp.AppActivity", "checkPermissionsForAudio");
    if(checkPermissions)
    {
        if (!_is_recording) {
            _is_recording = true;
            if (delegate){
                delegate->onRecordClicked();
            }
            startRecord();
        }else {//stop record
            _is_recording = false;
            _record->stopRecord();
            this->runLayoutDisplay(btplay, false);
            this->runLayoutDisplay(btrecord, false);
            this->runLayoutDisplay(btnext, false);
            btrecord->getChildByName<cocos2d::ui::ImageView *>("img_record")->loadTexture(record_img_path);
        }
    } else{
        mj::helper::hideTap();
        auto screen_size = Director::getInstance()->getVisibleSize();
        micLayout = Layout::create(); // k cho nhan vao mic khi hien popup
        micLayout->setContentSize(Size(400, 400));
        micLayout->setAnchorPoint(Vec2(0.5, 0.5));
        micLayout->setPosition(Vec2(screen_size.width / 2, screen_size.height));
        micLayout->setName("mic_layout");
        this->addChild(micLayout, INT_MAX - 1);
        micLayout->setTouchEnabled(true);
        AskPermissionPopup::showPopup(AskPermissionPopup::PERMISSION_KIND::RECORD, CC_CALLBACK_0(MJGamePhonics_TextVideo::acceptPermissionCallback, this), CC_CALLBACK_0(MJGamePhonics_TextVideo::denyPermissioCallback, this), this, true, nullptr, [=] {
            if (micLayout) {
                micLayout->removeFromParent();
            }
        });
        return;
    }
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

void MJGamePhonics_TextVideo::onRecord(cocos2d::Ref* sender) {

}

#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 )
void MJGamePhonics_TextVideo::onRecord(cocos2d::Ref *sender){
    _recordCopy = MSRecord::createRecord(1.0f, _object_info.text,true);
    if(MJDEFAULT->getStringForKey("key_turn_record") == "YES")
    {
        if (!_is_recording) {
            _is_recording = true;
            if (delegate){
                delegate->onRecordClicked();
            }
            startRecord();
        }else {//stop record
            _is_recording = false;
            _record->stopRecord();
            this->runLayoutDisplay(btplay, false);
            this->runLayoutDisplay(btrecord, false);
            this->runLayoutDisplay(btnext, false);
            btrecord->getChildByName<cocos2d::ui::ImageView *>("img_record")->loadTexture(record_img_path);
        }
    }
    else{
        mj::helper::hideTap();
        auto screen_size = Director::getInstance()->getVisibleSize();
        micLayout = Layout::create(); // k cho nhan vao mic khi hien popup
        micLayout->setContentSize(Size(400, 400));
        micLayout->setAnchorPoint(Vec2(0.5, 0.5));
        micLayout->setPosition(Vec2(screen_size.width / 2, screen_size.height));
        micLayout->setName("mic_layout");
        this->addChild(micLayout, INT_MAX - 1);
        micLayout->setTouchEnabled(true);
        AskPermissionPopup::showPopup(AskPermissionPopup::PERMISSION_KIND::RECORD, CC_CALLBACK_0(MJGamePhonics_TextVideo::acceptPermissionCallback, this), CC_CALLBACK_0(MJGamePhonics_TextVideo::denyPermissioCallback, this), this, true, nullptr, [=] {
            if (micLayout) {
                micLayout->removeFromParent();
            }
        });
        return;
    }
}
#endif



void MJGamePhonics_TextVideo::onPlay(cocos2d::Ref *sender){
    mj::helper::hideTap();
    if (delegate){
        delegate->onPlayClicked();
    }
    _is_recording = false;
    CCLOG("hoisunglog - onPlay1");
    this->runLayoutDisplay(btplay, false);
    this->runLayoutDisplay(btrecord, false);
    _record->startPlay(false);
    this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=]{
        this->removeChildByName("_record");
    }), NULL));
}

void MJGamePhonics_TextVideo::onNext(cocos2d::Ref *sender){
    mj::helper::hideTap();
    this->runLayoutDisplay(btnext, false);
    this->runLayoutDisplay(btrecord, false);
    if (delegate){
        delegate->onNextTurnClicked();
    }
}

void MJGamePhonics_TextVideo::runLayoutDisplay(cocos2d::ui::Layout *layout, bool isShow){
    layout->stopAllActions();
    if (isShow) {
        AUDIO_ENGINE->playEffect("sounds/ui/tray_exit.mp3");
        layout->setTouchEnabled(true);
        auto duration = 0.25f;
        layout->runAction(FadeIn::create(0.2));
        layout->runAction(Repeat::create(Sequence::create(ScaleTo::create(duration, 0.9f), ScaleTo::create(duration, 1.0f), NULL), 10000));
    }else{
        layout->setScale(1.0f);
        layout->setTouchEnabled(false);
        layout->runAction(FadeTo::create(0.2f, opacity_hide));
    }
}

void MJGamePhonics_TextVideo::onExit() {
    
    CCLOG("onExit-------------------");
    
    if (_record && _is_recording)
        _record->stopRecord();
    ui::Layout::onExit();
}

void MJGamePhonics_TextVideo::acceptPermissionCallback() {
    CCLOG("acceptPermissionCallback-------------------");
}
void MJGamePhonics_TextVideo::denyPermissioCallback() {
    CCLOG("denyPermissioCallback-----------");
}
