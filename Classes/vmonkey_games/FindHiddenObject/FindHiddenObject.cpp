//
//  FindHiddenObject.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "FindHiddenObject.h"

#define CSB_NAME "csb/game/FindHiddenObject/FindHiddenObject.csb"
#define bg_music "sounds/findHiddenObj/bg_music.mp3"

USING_NS_CC;
INITIALIZE_READER(FindHiddenObject);

std::once_flag FindHiddenObject_reader;

FindHiddenObject* FindHiddenObject::createGame(){
    
    std::call_once(FindHiddenObject_reader, []{
        REGISTER_CSB_READER(FindHiddenObject);
    });
    
    auto p = static_cast<FindHiddenObject*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
//        generateData();
        p->didLoadFromCSB();
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void FindHiddenObject::didLoadFromCSB(){
    
    this->generateData();
    
    screen_size = Director::getInstance()->getVisibleSize();
    //    autoPlay = isAutoPlay;
    this->loadFrame();
}

void FindHiddenObject::generateData(){
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
        question.audio_path = qImage->getAudios("name_1", lang).front();
        question.image_path = qImage->path;
    }
    if (JSON_SAFE_CHECK(document, "is_portrait", Bool)){
        is_portrait = document.FindMember("is_portrait")->value.GetBool();
    }
    
    if(JSON_SAFE_CHECK(document, "number_obj", Int)){
        number_obj= document.FindMember("number_obj")->value.GetInt();
    }
}

void FindHiddenObject::loadFrame(){
    
    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);

    if(is_portrait){
         root_layout = utils::findChild<Layout*>(this, "root_layout_portrait");
        utils::findChild<Layout*>(this, "root_layout_landscape")->setVisible(false);
    }else{
        root_layout = utils::findChild<Layout*>(this, "root_layout_landscape");
        utils::findChild<Layout*>(this, "root_layout_portrait")->setVisible(false);
    }
   
    auto question_text = utils::findChild<Text*>(root_layout, "Text_1");
    question_text->setContentSize(Size(utils::findChild<ImageView*>(root_layout, "question")->getContentSize()*0.9));
    question_text->ignoreContentAdaptWithSize(false);
    question_text->setString(question.text);
    question_text->setFontName(font_name);
    question_text->setFontSize(font_sz);
    
    q_id = AUDIO_ENGINE->play(question.audio_path, false, 1.0f, [=]{
        utils::findChild<ImageView*>(this, "speaker")->setTouchEnabled(true);
    });
}

void FindHiddenObject::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->play("sounds/FindHiddenObject/bgm.mp3", true);
//    AUDIO_ENGINE->play("sounds/FindHiddenObject/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void FindHiddenObject::onFinishGameVMonkey(){
//    AUDIO_ENGINE->stopAllEffects();
    AUDIO_ENGINE->stopEffect(q_id);
    MJMainGBase::onFinishGameVMonkey();
}

void FindHiddenObject::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopEffect(q_id);
//    AUDIO_ENGINE->stopAllEffects();
}

void FindHiddenObject::onClick(Ref* sender){
    auto obj = (ImageView*)sender;
    if (obj->getName() == "Image_1_0") return;
    CCLOG("let's go");
    AUDIO_ENGINE->play("sounds/findHiddenObj/correct.mp3");
    
    auto side_panel = utils::findChild<ImageView*>(root_layout, "side_panel");
    auto board =utils::findChild<ImageView*>(root_layout, "board");
    
    
    obj->setLocalZOrder(INT_MAX-1);
    obj->setTouchEnabled(false);
    
    auto slot = utils::findChild<ImageView*>(side_panel, obj->getName());
    auto scale_x = slot->getContentSize().width/ obj->getContentSize().width;
    auto scale_y = slot->getContentSize().height/ obj->getContentSize().height;
    if(abs(scale_x - scale_y) > 0.1){
        //xem khuôn có phải bị scale bé so vs hình mẫu hay kích thước ảnh khác nhau
        scale_x = 1;
        scale_y = 1;
    }
    auto pos = board->convertToNodeSpace(side_panel->convertToWorldSpace(slot->getPosition())) ;
    
    obj->runAction(Sequence::create(Spawn::createWithTwoActions(MoveTo::create(0.3, Vec2(board->getContentSize()/2)), ScaleTo::create(0.3, 2)), DelayTime::create(0.3), Spawn::createWithTwoActions(MoveTo::create(0.3, pos), ScaleTo::create(0.3, scale_x, scale_y)), CallFunc::create([=]{
        count++;
        //check finish game
        if(count == number_obj){
            this->scheduleOnce([=](float dt){
                onFinishGameVMonkey();
            }, 0.5, "finish_game");
        }
    }) ,NULL));
}

void FindHiddenObject::onClickSpeaker(Ref* sender){
    AUDIO_ENGINE->stopEffect(q_id);
    q_id = AUDIO_ENGINE->play(question.audio_path);
    
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback FindHiddenObject::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(FindHiddenObject, onClick),
        CLICK_MAP(FindHiddenObject, onClickSpeaker)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


