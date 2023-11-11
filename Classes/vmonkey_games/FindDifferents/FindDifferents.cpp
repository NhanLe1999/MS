//
//  FindDifferents.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "FindDifferents.h"

#define CSB_NAME "csb/game/FindDifferents/findDiff.csb"
#define bg_music "sounds/FindDifferents/bg_music.mp3"

USING_NS_CC;
INITIALIZE_READER(FindDifferents);

std::once_flag FindDifferents_reader;

FindDifferents* FindDifferents::createGame(){
    
    std::call_once(FindDifferents_reader, []{
        REGISTER_CSB_READER(FindDifferents);
    });
    
    auto p = static_cast<FindDifferents*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        
        p->didLoadFromCSB();
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void FindDifferents::didLoadFromCSB(){
    
    this->generateData();
    
    screen_size = Director::getInstance()->getVisibleSize();
    //    autoPlay = isAutoPlay;
    this->loadFrame();
}

void FindDifferents::generateData(){

    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

//    // get data
    string json_file = "config.json";
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    if (JSON_SAFE_CHECK(document, "question", String)){
        auto q_id = document.FindMember("question")->value.GetString();
        auto qImage = math::resource::ResHelper::getImage(images, q_id);
        
		question.text = qImage->getQuestionVMonkey("name_1", lang);
        question.audio_path = qImage->getAudios("name_1",  lang).front();
        question.image_path = qImage->path;
    }
    
    if(JSON_SAFE_CHECK(document, "number_obj", Int)){
        number_diff = document.FindMember("number_obj")->value.GetInt();
    }
}

void FindDifferents::loadFrame(){

    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);
    
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    root_layout->setContentSize(Size(1024, 768));
    
    auto root_width = root_layout->getContentSize().width;
    auto root_height = root_layout->getContentSize().height;
    utils::findChild<Layout*>(root_layout, "Panel_0")->setPosition(Vec2(root_width * 0.26, root_height * 0.4));
    utils::findChild<Layout*>(root_layout, "Panel_1")->setPosition(Vec2(root_width * 0.74, root_height * 0.4));
    utils::findChild<Sprite*>(root_layout, "question")->setPositionX(root_width * 0.5);
    
    auto question_text = utils::findChild<Text*>(root_layout, "Text_1");
    question_text->setContentSize(Size(utils::findChild<Sprite*>(root_layout, "question")->getContentSize()*0.9));
    question_text->ignoreContentAdaptWithSize(false);
    question_text->setString(question.text);
    question_text->setFontName(font_name);
    question_text->setFontSize(font_sz);
    
    utils::findChild<ImageView*>(utils::findChild<Button*>(this, "speaker"), "Image_1")->setName("loa");
    
    utils::findChild<ImageView*>(this, "Image_1")->addClickEventListener(CC_CALLBACK_0( FindDifferents::onTouchPicture, this));
    utils::findChild<ImageView*>(this, "Image_1")->setTouchEnabled(true);
     utils::findChild<ImageView*>(this, "Image_2")->addClickEventListener(CC_CALLBACK_0( FindDifferents::onTouchPicture, this));
     utils::findChild<ImageView*>(this, "Image_2")->setTouchEnabled(true);
    
    q_id = AUDIO_ENGINE->playSound(question.audio_path);
    for(auto c: root_layout->getChildren()){
        CCLOG("pos: %f, %f", c->getPositionX(), c->getPositionY());
        auto pos = c->getPosition();
        
        c->setPosition(root_layout->convertToNodeSpace(root_layout->convertToWorldSpace(pos)));
    }
}

void FindDifferents::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->play("sounds/FindDifferents/bgm.mp3", true);
//    AUDIO_ENGINE->play("sounds/FindDifferents/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void FindDifferents::onFinishGameVMonkey(){
    
    auto layout_block_touch = Layout::create();
    layout_block_touch->setContentSize(Director::getInstance()->getWinSize());
    layout_block_touch->setTouchEnabled(true);
    layout_block_touch->addClickEventListener([=](Ref* sender){
        CCLOG("touch blocked");
    });
    this->addChild(layout_block_touch);

    AUDIO_ENGINE->stopEffect(q_id);
    MJMainGBase::onFinishGameVMonkey();
}

void FindDifferents::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void FindDifferents::onClickHolder(Ref* sender){
    CCLOG("let's go");
    AUDIO_ENGINE->play("sounds/findDiff/correct.mp3");
    count++;
    auto ring = (ImageView*)sender;
    ring->setOpacity(255);
    playEffect(ring);
    ImageView* ring_2 = nullptr;
     CCLOG("ring name: %s", ring->getName().c_str());
    if(ring->getParent()->getName() == "Image_1" ){
        ring_2 = utils::findChild<ImageView*>(utils::findChild<ImageView*>(this, "Image_2"), ring->getName());
    }else{
       
         ring_2 = utils::findChild<ImageView*>(utils::findChild<ImageView*>(this, "Image_1"), ring->getName());
    }
    ring_2->setOpacity(255);
    playEffect(ring_2);
    
    ring->setTouchEnabled(false);
    ring_2->setTouchEnabled(false);
    
    //check finish game
    if(count == number_diff){
        this->scheduleOnce([=](float dt){
            onFinishGameVMonkey();
        }, 0.5, "finish_game");
    }
}

void FindDifferents::playEffect(ImageView* ring){
    auto effect = GAFWrapper::createGAF("gaf/effect/effec.gaf");
    ring->getParent()->addChild(effect);
    effect->setPosition(ring->getPosition());
    effect->setScale(0.5);
    effect->play(false);
    
}

void FindDifferents::onClickSpeaker(Ref* sender){
//    Button* loa = (Button*) sender;
//    loa->setTouchEnabled(false);
//    q_id = AUDIO_ENGINE->play(question.audio_path, false, 1.0f, [=]{
//        loa->setTouchEnabled(true);
//    });
    AUDIO_ENGINE->stopSound(question.audio_path);
    q_id = AUDIO_ENGINE->playSound(question.audio_path);
}

void FindDifferents::onTouchPicture(){
    AUDIO_ENGINE->play("sounds/findDiff/touch_2.mp3");
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback FindDifferents::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(FindDifferents, onClickHolder),
        CLICK_MAP(FindDifferents, onClickSpeaker)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


