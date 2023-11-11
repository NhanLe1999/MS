//
//  TellTheStory.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "TellTheStory.h"

#define CSB_NAME "csb/game/TellTheStory/TellTheStory.csb"
#define bg_music "sounds/TellTheStory/bg_music.mp3"

#define opacity_hide 120

USING_NS_CC;
INITIALIZE_READER(TellTheStory);

std::once_flag TellTheStory_reader;

TellTheStory* TellTheStory::createGame(){
    
    std::call_once(TellTheStory_reader, []{
        REGISTER_CSB_READER(TellTheStory);
    });
    
    auto p = static_cast<TellTheStory*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        
        p->didLoadFromCSB();
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void TellTheStory::didLoadFromCSB(){
    
    this->generateData();
    
    screen_size = Director::getInstance()->getVisibleSize();
    //    autoPlay = isAutoPlay;
    this->loadFrame();
    
}

void TellTheStory::generateData(){
    
//    // get data
    string json_file = "config.json";
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
//    rapidjson::Value& value_data = document.FindMember("data")->value;

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    timer = document.FindMember("time")->value.GetInt();
    
    auto letter = document.FindMember("character")->value.GetString();
    auto pImage = math::resource::ResHelper::getImage(images, letter);
    character = pImage->path;

    auto value_data = document.FindMember("img_list")->value.GetArray();
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        auto letter = value_data[i].GetString();
        auto pImage = math::resource::ResHelper::getImage(images, letter);
        imgs.push_back(pImage->path);
    }
    
}

#include "Math_Quiz.h"
void TellTheStory::loadFrame(){

    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);
    
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    auto img_panel = utils::findChild<Layout*>(this, "img_panel");
    
    auto character_img = utils::findChild<ImageView*>(this, "icon");
    character_img->loadTexture(character);
    character_img->ignoreContentAdaptWithSize(false);
    
    std::vector<ImageView*> images = {};
    auto size = Size(300, 300);
    if(imgs.size() > 3){
        size = Size(200, 200);
    }

    for (int i = 0; i < imgs.size(); i++) {
        auto img_holder = ImageView::create("games/tellTheStory/holder.png");
        img_holder->setContentSize(size);
        img_holder->ignoreContentAdaptWithSize(false);
        
        auto img = ImageView::create(imgs[i]);
        img->setPosition(img_holder->getContentSize()/2);
        img->setContentSize(img_holder->getContentSize()*0.9);
        img->ignoreContentAdaptWithSize(false);
        
        img_holder->addChild(img);
        img_panel->addChild(img_holder);
        images.push_back(img_holder);
    }
    
    if (images.size() != 4) {
        math::func::setPositionList(images, 3, Rect(0.15, 0., 0.7, 0.5));
    }
    else {
        math::func::setPositionList(images, 2, Rect(0.15, 0., 0.7, 0.55));
    }
    
    btrecord = cocos2d::utils::findChild<Button *>(this, "btn_record");
    btrecord->setPositionX(screen_size.width/2);
    btstop = cocos2d::utils::findChild<Button *>(this, "btn_stop");
    btstop->setPositionX(screen_size.width/2);
    btstop->setTouchEnabled(false);
    btplay = cocos2d::utils::findChild<Button *>(this, "btn_play");
    btplay->setOpacity(0);
    btplay->setTouchEnabled(false);
    btshare = cocos2d::utils::findChild<Button *>(this, "btn_share");
    btshare->setOpacity(0);
    btshare->setTouchEnabled(false);
    
    btnext = cocos2d::utils::findChild<Button *>(this, "btn_next");
    btnext->setOpacity(0);
    btnext->setTouchEnabled(false);
    
    btstopPlay = cocos2d::utils::findChild<Button *>(this, "btn_stop_play");
    btstopPlay->setOpacity(0);
    btstopPlay->setTouchEnabled(false);
}

void TellTheStory::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->play("sounds/TellTheStory/bgm.mp3", true);
//    AUDIO_ENGINE->play("sounds/TellTheStory/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void TellTheStory::onFinishGameVMonkey(){
//    AUDIO_ENGINE->stopAllEffects();
    MJMainGBase::onFinishGameVMonkey();
}

void TellTheStory::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void TellTheStory::onRecord(Ref* sender){
    CCLOG("on record");
    _is_recording = true;
    startRecord();
    setGameState(ON_RECORD);
}

void TellTheStory::onStop(cocos2d::Ref *sender){
    _is_recording = false;
    _record->stopRecord();
    
    setGameState(ON_STOP);
}

void TellTheStory::onRepeat(Ref* sender){
     CCLOG("on repeat");
    _is_recording = false;
    _record->startPlay(false);
    setGameState(ON_REPEAT);
}

void TellTheStory::onStopPlay(cocos2d::Ref *sender){
    _is_recording = false;
    _record->stopPlay();
    
    setGameState(ON_STOP_PLAY);
}

#include "MJPlatformConfig.h"
void TellTheStory::onShare(Ref* sender){
     CCLOG("on share");
    setGameState(ON_SHARE);
//    MJ_PLATFORM_CONFIG.shareScreenShot();
}

void TellTheStory::onNext(Ref* sender){
    this->scheduleOnce([=](float dt){
        onFinishGameVMonkey();
    }, 0.5, "finish_game");
}

void TellTheStory::onClickSpeaker(Ref* sender){
     CCLOG("on speaker");
}

void TellTheStory::setGameState(TellTheStory::GAME_STATE state){
    switch (state) {
        case ON_NONE:
            btplay->runAction(FadeIn::create(0.2f));
            btshare->runAction(FadeIn::create(0.2f));
            btnext->runAction(FadeIn::create(0.2f));
            btrecord->runAction(FadeIn::create(0.2f));
            btstop->runAction(FadeIn::create(0.2f));
            btstop->setVisible(false);
            
            btplay->setTouchEnabled(true);
            btshare->setTouchEnabled(true);
            btnext->setTouchEnabled(true);
            btrecord->setTouchEnabled(true);
            btstop->setTouchEnabled(false);
            break;
        case ON_RECORD:
            btplay->runAction(FadeOut::create(0.2f));
            btplay->setTouchEnabled(false);
            btshare->runAction(FadeOut::create(0.2f));
//            btshare->runAction(FadeTo::create(0.2f, opacity_hide));
            btshare->setTouchEnabled(false);
            btnext->runAction(FadeOut::create(0.2f));
            btnext->setTouchEnabled(false);
            btstopPlay->setOpacity(0);
            
            
            btrecord->setTouchEnabled(false);
            btrecord->runAction(FadeOut::create(0.2f));
            btstop->setVisible(true);
            btstop->setOpacity(0);
            btstop->runAction(FadeTo::create(0.2f, opacity_hide));
            btstop->setTouchEnabled(false);
            btstop->runAction(Sequence::create(DelayTime::create(2.0f), FadeIn::create(0.2f), CallFunc::create([=]{
                btstop->setTouchEnabled(true);
            }), NULL));
            
            utils::findChild<ImageView*>(btstop, "micro")->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeTo::create(0.3f, opacity_hide), FadeIn::create(0.3f)) ));
            
            break;
        case ON_REPEAT:
            btrecord->runAction(FadeTo::create(0.2f, opacity_hide));
            btrecord->setTouchEnabled(false);
            btshare->runAction(FadeTo::create(0.2f, opacity_hide));
            btshare->setTouchEnabled(false);
            btnext->runAction(FadeTo::create(0.2f, opacity_hide));
            btnext->setTouchEnabled(false);
            
            btplay->setTouchEnabled(false);
            btplay->runAction(FadeOut::create(0.2f));
            btstopPlay->setVisible(true);
            btstopPlay->setOpacity(0);
            btstopPlay->runAction(FadeTo::create(0.2f, opacity_hide));
            btstopPlay->setTouchEnabled(false);
            btstopPlay->runAction(Sequence::create(DelayTime::create(2.0f), FadeIn::create(0.2f), CallFunc::create([=]{
                btstopPlay->setTouchEnabled(true);
            }), NULL));
            
            utils::findChild<ImageView*>(btstopPlay, "loa")->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeTo::create(0.3f, opacity_hide), FadeIn::create(0.3f)) ));
            
            break;
        case ON_STOP:
            utils::findChild<ImageView*>(btstop, "micro")->stopAllActions();
            btstop->setTouchEnabled(false);
            btstop->runAction(FadeOut::create(0.2f));
            btrecord->setVisible(true);
            btrecord->setOpacity(0);
            btrecord->runAction(FadeTo::create(0.2f, opacity_hide));
            btrecord->setTouchEnabled(false);
            btrecord->runAction(Sequence::create(FadeIn::create(0.2f), MoveTo::create(0.5f, Vec2(root_layout->getContentSize().width*0.125, btrecord->getPositionY())), CallFunc::create([=]{
                btrecord->setTouchEnabled(true);
            }), NULL));
            
            btplay->runAction(FadeIn::create(0.2f));
            btshare->runAction(FadeIn::create(0.2f));
            btnext->runAction(FadeIn::create(0.2f));
            
            btnext->setTouchEnabled(true);
            btshare->setTouchEnabled(true);
            btplay->setTouchEnabled(true);
            break;
            
        case ON_STOP_PLAY:
            utils::findChild<ImageView*>(btstopPlay, "loa")->stopAllActions();
            btstopPlay->setTouchEnabled(false);
            btstopPlay->runAction(FadeOut::create(0.2f));
            btplay->setVisible(true);
            btplay->setOpacity(0);
            btplay->runAction(FadeTo::create(0.2f, opacity_hide));
            btplay->setTouchEnabled(false);
            btplay->runAction(Sequence::create(FadeIn::create(0.2f), CallFunc::create([=]{
                btplay->setTouchEnabled(true);
            }), NULL));
            
            btrecord->runAction(FadeIn::create(0.2f));
            btrecord->setTouchEnabled(true);
            btshare->runAction(FadeIn::create(0.2f));
            btshare->setTouchEnabled(true);
            btnext->runAction(FadeIn::create(0.2f));
            btnext->setTouchEnabled(true);
            break;
            
        case ON_SHARE:
            setGameState(ON_NONE);
            break;
            
        default:
            break;
    }
}

void TellTheStory::startRecord(){
    if (_record) {
        this->removeChildByName("_record");
        _record = nullptr;
    }
    _record = MSRecord::createRecord(timer);//1.5*_object_info.audio_duration
    this->addChild(_record);
    _record->setDelegate(this);
    _record->setName("_record");
   
}

void TellTheStory::onFinishRecord(int finish_type,std::string url){
    setGameState(ON_NONE);
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback TellTheStory::onLocateClickCallback(const std::string &callback_name) {
  
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(TellTheStory, onRecord),
        CLICK_MAP(TellTheStory, onStop),
        CLICK_MAP(TellTheStory, onRepeat),
        CLICK_MAP(TellTheStory, onStopPlay),
        CLICK_MAP(TellTheStory, onShare),
        CLICK_MAP(TellTheStory, onNext),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


