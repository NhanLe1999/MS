//
//  CompletePicture.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "CompletePicture.h"

#define CSB_NAME "csb/game/CompletePicture/CompletePicture.csb"
#define bg_music "sounds/CompletePicture/bg_music.mp3"

#define SOUND_CORRECT "sounds/CompletePicture/correct.mp3"
#define SOUND_WRONG "sounds/CompletePicture/wrong_%d.mp3"
#define SOUND_TOUCH "sounds/CompletePicture/touch_%d.mp3"
#define SOUND_DROP "sounds/CompletePicture/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/CompletePicture/moveback_%d.mp3"

USING_NS_CC;
INITIALIZE_READER(CompletePicture);

std::once_flag CompletePicture_reader;

CompletePicture* CompletePicture::createGame(){
    
    std::call_once(CompletePicture_reader, []{
        REGISTER_CSB_READER(CompletePicture);
    });
    
    auto p = static_cast<CompletePicture*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        
        p->didLoadFromCSB();
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void CompletePicture::didLoadFromCSB(){
    
    this->generateData();
    screen_size = Director::getInstance()->getVisibleSize();
    //    autoPlay = isAutoPlay;
    this->loadFrame();
}

void CompletePicture::generateData(){
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
		qo.text = qImage->getQuestionVMonkey("name_1", lang);
        qo.audio_path = qImage->getAudios("name_1", lang).front();
        qo.image_path = qImage->path;
    }
    
    if(JSON_SAFE_CHECK(document, "is_free", Bool)){
        is_free = document.FindMember("is_free")->value.GetBool();
    }
    
    if(JSON_SAFE_CHECK(document, "img_zoom", Bool)){
        img_zoom = document.FindMember("img_zoom")->value.GetBool();
    }
    
    if (JSON_SAFE_CHECK(document, "number_obj", Int)) {
        number_obj = document.FindMember("number_obj")->value.GetInt();
    }
    
    if(document.HasMember("answer_audio")){
        auto answers = document.FindMember("answer_audio")->value.GetArray();
        for (int i = 0; i < answers.Size(); i++) {
            auto a_id = answers[i]["answer"].GetString();
            auto aImage = math::resource::ResHelper::getImage(images, a_id);
            answer_audios.push_back(aImage->getAudios("name_1", lang).front());
        }
    }
}

void CompletePicture::loadFrame(){

    AUDIO_ENGINE->playEffect(bg_music, true, 0.3);
    
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    if (utils::findChild<Node*>(root_layout, "mattroi")) is_mattroi_act = true;
    bg_img = utils::findChild<ImageView*>(this, "img_bg");
    if(bg_img){
        bg_img->setTouchEnabled(true);
        bg_img->addClickEventListener(CC_CALLBACK_0(CompletePicture::onTouchPicture, this));
    }
    auto bg_ques = utils::findChild<ImageView*>(root_layout, "question");
    if (bg_ques != NULL){
        if (bg_ques->getChildByName<ImageView*>("Image_1_0") != NULL){
            bg_ques->getChildByName<ImageView*>("Image_1_0")->setVisible(false);
        }
    }
    auto question = utils::findChild<Text*>(root_layout, "Text_1");
    question->setScale(1.0);
    question->setContentSize(Size(utils::findChild(root_layout, "question")->getContentSize()*0.9));
    question->ignoreContentAdaptWithSize(false);
    
    if (bg_ques){
        question->setContentSize(Size(bg_ques->getContentSize().width*0.9f, bg_ques->getContentSize().height*0.85f));
        question->ignoreContentAdaptWithSize(false);
        question->setPosition(bg_ques->getContentSize()/2);
    }
   
    if (is_free) {
        question->setTextColor(Color4B::WHITE);
        question->enableOutline(Color4B::BLACK);
    }
    question->setString(qo.text);
    question->setFontName(font_name);
    question->setFontSize(38);
    if (!is_free) {
        question->setFontSize(35);
    }
    while (question->getVirtualRendererSize().height > utils::findChild(root_layout, "question")->getContentSize().height * 0.8) {
        font_sz = question->getFontSize();
        question->setFontSize(font_sz - 2);
        CCLOG("Scale down font: %d", font_sz);
    }
    next_btn = utils::findChild<Button*>(this, "btn_next");
    next_btn->setOpacity(0);
    next_btn->setTouchEnabled(false);
    share_btn = utils::findChild<Button*>(this, "btn_share");
    share_btn->setOpacity(0);
    share_btn->setTouchEnabled(false);
    
    if(!is_free){
        for(int i = 0; i < number_obj; i++){
            auto name = StringUtils::format("Slot_%d", i);
            auto slot = utils::findChild<ImageView*>(this, name);
            auto old_color = slot->getColor();
            _colors[name] = old_color;
            slot->setTag(slot->getOpacity());
            if (slot->getOpacity() != 0) {
                slot->runAction(TintTo::create(0, Color3B::BLACK));
                slot->setOpacity(255);
            }
            slot->setTouchEnabled(false);
            
            auto obj = utils::findChild<ImageView*>(this, StringUtils::format("Image_%d", i));
            obj->setTouchEnabled(true);
            obj->setTag(i);
        }
    }
    else{
        for(int i = 0; i < number_obj; i++){
            auto name = StringUtils::format("Image_%d", i);
            auto obj = utils::findChild<ImageView*>(this, name);
            obj->setTouchEnabled(false);
        }
    }
    
   audio_id_question = AUDIO_ENGINE->playSound(qo.audio_path);
    this->scheduleOnce([=](float){
        for(int i = 0; i < number_obj; i++){
            auto name = StringUtils::format("Image_%d", i);
            auto obj = utils::findChild<ImageView*>(this, name);
            obj->setTouchEnabled(true);
        }
    }, 2.0f, "enableTouch");
    
    
    
}

void CompletePicture::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->play("sounds/CompletePicture/bgm.mp3", true);
//    AUDIO_ENGINE->play("sounds/CompletePicture/intro.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void CompletePicture::onFinishGameVMonkey(){
//    AUDIO_ENGINE->stopAllEffects();
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onFinishGameVMonkey();
}

void CompletePicture::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void CompletePicture::onTouchObject(Ref* sender, TouchEventType type ){
    auto obj = (ImageView*)sender;
    if (isDone) return;
    auto name_obj = obj->getName();
    bool check_mattroi = true;
//    CCLOG("parent name: %s", obj->getParent()->getName().c_str());
    if (is_mattroi_act){
        if ((have_eye != "") && (name_obj == "Image_2" || name_obj == "Image_3" || name_obj == "Image_5") && (name_obj != have_eye)){
            check_mattroi = false;
        }
        if ((have_mouth != "") && (name_obj == "Image_0" || name_obj == "Image_1" || name_obj == "Image_4") && (name_obj != have_mouth)){
            check_mattroi = false;
        }
    }
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
//             CCLOG("let's begin");
			AUDIO_ENGINE->play(StringUtils::format(SOUND_TOUCH, 1));
            pos = obj->getPosition();
            if(is_free){
                 obj->setPosition(obj->getParent()->convertToNodeSpace(obj->getTouchBeganPosition()) );
            }else{
                 obj->setPosition(root_layout->convertToNodeSpace(obj->getTouchBeganPosition()));
            }
           
            obj->setLocalZOrder(99);
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
//             CCLOG("let's move");
            if(is_free){
                obj->setPosition(obj->getParent()->convertToNodeSpace(obj->getTouchMovePosition()) );
            }else{
               obj->setPosition(root_layout->convertToNodeSpace(obj->getTouchMovePosition()));
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
//            CCLOG("let's end");
            
            if (is_free) {
                auto interSize = bg_img->getBoundingBox().unionWithRect(obj->getBoundingBox()).size;
                auto bgSize = bg_img->getBoundingBox().size;
                if (bg_img->getBoundingBox().containsPoint(obj->getParent()->convertToNodeSpace(obj->getTouchEndPosition())) &&
                    interSize.width == bgSize.width && interSize.height == bgSize.height && check_mattroi) {
                    AUDIO_ENGINE->play(StringUtils::format(SOUND_DROP, cocos2d::random(1, 3)));
                    obj->setPosition(obj->getParent()->convertToNodeSpace(obj->getTouchEndPosition()));
                    if (is_mattroi_act){
                        if ((name_obj == "Image_2" || name_obj == "Image_3" || name_obj == "Image_5")){
                            have_eye = name_obj;
                        }
                        if ((name_obj == "Image_0" || name_obj == "Image_1" || name_obj == "Image_4")){
                            have_mouth = name_obj;
                        }
                    }
                    if(checkAllInboxObject()){
                        showDoneButton();
                    };
                }else{
                    AUDIO_ENGINE->playSound(StringUtils::format(SOUND_MOVEBACK, cocos2d::random(1, 3)));
                    obj->setPosition(pos);
                }
            }else {
                CCLOG("tag %i:", obj->getTag());
                
                auto slot = utils::findChild<ImageView*>(root_layout, StringUtils::format("Slot_%d", obj->getTag()));
                if(!slot){
                    //return old pos
                    obj->setPosition(pos);
                    AUDIO_ENGINE->playSound(StringUtils::format(SOUND_WRONG, cocos2d::random(1, 3)));
                    break;
                }
                
                Node* box = nullptr;
                if(utils::findChild(root_layout, StringUtils::format("%s_panel", slot->getName().c_str()))){
                    box = utils::findChild(root_layout, StringUtils::format("%s_panel", slot->getName().c_str()));
                }else{
                    box = slot;
                }
                if (box->getBoundingBox().containsPoint(root_layout->convertToNodeSpace(obj->getTouchEndPosition()))) {
                    count++;
                    AUDIO_ENGINE->play("sounds/CompletePicture/answer_correct.mp3");
                    
                    if(answer_audios.size()){
                        AUDIO_ENGINE->stopEffect(audio_id_question);
                        AUDIO_ENGINE->playSound(answer_audios[obj->getTag()]);
                    }
                    
                    obj->setPosition(slot->getPosition());
                    slot->setColor(_colors[StringUtils::format("Slot_%d", obj->getTag())]);
                    if(slot->getTag() != 0){
                        slot->setOpacity(slot->getTag());
                    }else{
                        slot->setOpacity(255);
                    }
                    obj->setVisible(false);
                    obj->setTouchEnabled(false);
                    
                    //check finish game
                    if(count == number_obj){
                        auto time_delay = 1.0f;
                        if(img_zoom){
                            auto full_img = utils::findChild<ImageView*>(this, "full_img");
                            time_delay = 2.0f;
                            full_img->runAction(Sequence::create( Spawn::createWithTwoActions( FadeIn::create(0.2), CallFunc::create([=]{
                                utils::findChild<ImageView*>(this, "img_bg")->runAction(FadeOut::create(0.2));
                                for (int i = 0 ; i< number_obj; i++) {
                                    auto slot = utils::findChild<ImageView*>(root_layout, StringUtils::format("Slot_%d",i));
                                    slot->runAction(FadeOut::create(0.2));
                                }
                            })), Spawn::create( ScaleTo::create(0.5, 2), MoveTo::create(0.5, utils::findChild(this, "board")->getPosition()), CallFunc::create([=]{
                                
                            }), NULL), NULL));
                        }
                        
                        if(answer_audios.size()){
                            time_delay = 2.0f;
                        }
                        
                        AUDIO_ENGINE->play(SOUND_CORRECT);
                        this->scheduleOnce([=](float dt){
                            onFinishGameVMonkey();
                        }, time_delay, "finish_game");
                    }
                    
                }else {
                    //return old pos
                    obj->setPosition(pos);
                    AUDIO_ENGINE->playSound(StringUtils::format(SOUND_WRONG, cocos2d::random(1, 3)));
                }
            }
            
             obj->setLocalZOrder(10);
            break;
            
        default:
            break;
    }
}

void CompletePicture::onTouchPicture(){
    AUDIO_ENGINE->playSound(StringUtils::format(SOUND_TOUCH, 2));
}

void CompletePicture::showDoneButton(){
    // add object and background to board layout to capture
    if(is_free)
    {
        auto board = utils::findChild(this, "board");
        auto img_bg = utils::findChild(this, "img_bg");
        auto img_pos = img_bg->getParent()->convertToWorldSpace(img_bg->getPosition());
      
        std::vector<ImageView*> captureNodes;
        this->enumerateChildren("//Image_[[:digit:]]", [&](Node* node)->bool{
            auto obj = dynamic_cast<ImageView*>(node);
            if (bg_img->getBoundingBox().containsPoint(/*obj->getParent()->convertToNodeSpace(obj->getTouchEndPosition())*/obj->getPosition())) {
                    captureNodes.push_back(obj);
            }else{
                obj->setTouchEnabled(false);
            }
            return false;
        });
        img_bg->retain();
        img_bg->removeFromParentAndCleanup(false);
        board->addChild(img_bg);
        img_bg->setPosition(board->convertToNodeSpace(img_pos));
        img_bg->release();
        
        for(auto obj : captureNodes) {
            auto pos = obj->getParent()->convertToWorldSpace(obj->getPosition());
            obj->retain();
            obj->removeFromParentAndCleanup(false);
            board->addChild(obj);
            obj->setPosition(board->convertToNodeSpace(pos));
            obj->release();
        }
        
    }
    if (share_btn->getChildByName<Text*>("Text_1")){
        share_btn->getChildByName<Text*>("Text_1")->setString("Lưu ảnh");
    }
    share_btn->runAction(Sequence::createWithTwoActions(FadeIn::create(0.2f), CallFunc::create([=]{
        share_btn->setTouchEnabled(true);
    })) );
    
    next_btn->runAction(Sequence::createWithTwoActions(FadeIn::create(0.2f), CallFunc::create([=]{
        next_btn->setTouchEnabled(true);
    })) );
   
}

bool CompletePicture::checkAllInboxObject(){
    int count = 0;
    
    this->enumerateChildren("//Image_[[:digit:]]", [&count,this](Node* node)->bool{
        auto obj = reinterpret_cast<ImageView*>(node);
        auto posObj = obj->getParent()->convertToNodeSpace(obj->getPosition());
        auto bg_bd = bg_img->getBoundingBox();
        if (bg_img->getBoundingBox().containsPoint(obj->getPosition())){
            count = count + 1;
        }
        return false;
    });
    return count >= number_obj;
}

void CompletePicture::onClickSpeaker(Ref* sender){
//    Button* loa = (Button*) sender;
//    loa->setTouchEnabled(false);
//    AUDIO_ENGINE->play(qo.audio_path, false, 1.0f, [=]{
//        loa->setTouchEnabled(true);
//    });
    //AUDIO_ENGINE->stopEffect(audio_id_question);
    AUDIO_ENGINE->stopSound(qo.audio_path);
    audio_id_question = AUDIO_ENGINE->playSound(qo.audio_path);
}

void CompletePicture::onComplete(std::function<void()> callback){
    AUDIO_ENGINE->play(SOUND_CORRECT);
    for (int i = 0; i < number_obj; i++) {
        utils::findChild(this, StringUtils::format("Slot_%d", i))->runAction(FadeOut::create(0.5));
        utils::findChild<ImageView*>(this, StringUtils::format("Image_%d", i))->setTouchEnabled(false);
    }
    if(callback){
        this->scheduleOnce([=](float){
            callback();
        }, 0.8f ,"onComplete_Callback");
    }
   
}

#include "MJPlatformConfig.h"
#include "LTDate.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
void CompletePicture::onShare(Ref* sender){
    auto button = (Button*)sender;
    button->setTouchEnabled(false);
    button->setOpacity(200);
    
    onComplete([=]{
        
        auto board = utils::findChild(this, "board");
        auto board_pos = board->getParent()->convertToWorldSpace(board->getPosition());
//        board->retain();
//        board->removeFromParent();
//        this->addChild(board);
//        board->setPosition(this->convertToNodeSpace(board_pos));
//        board->release();
        
        auto board_size = board->getBoundingBox().size;
        auto ap = board->getAnchorPoint();
        board_pos = board->getPosition();
        auto start_x = root_layout->convertToWorldSpace(board_pos).x - board_size.width/2;
        auto start_y = root_layout->convertToWorldSpace(board_pos).y - board_size.height/2;
        
        RenderTexture* renderTexture = RenderTexture::create(board_size.width, board_size.height, Texture2D::PixelFormat::RGBA8888);
        renderTexture->setAutoDraw(false);
        renderTexture->setKeepMatrix(true);
        renderTexture->setVirtualViewport(board_pos - Vec2(board_size.width * ap.x, board_size.height *ap.y ), Rect(0, 0, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height),Rect(start_x, start_y, Director::getInstance()->getWinSizeInPixels().width,Director::getInstance()->getWinSizeInPixels().height));

        renderTexture->begin();
        board->visit();
        renderTexture->end();
        
        auto name = StringUtils::format("Screenshot_%s.png", LTDate::getCurrentDate().toPrettyFileName().c_str());
        renderTexture->saveToFile(name, Image::Format::PNG);
        auto full_name = FileUtils::getInstance()->getWritablePath() + name;
        
        auto listener = EventListenerCustom::create("save picture", [this](EventCustom* event){
            isDone = true;
            StoryAlert::showAlert("Đã lưu vào ảnh", "OK", "",false,30,this);
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this);
        
        auto deny_listener = EventListenerCustom::create("deny save picture", [=](EventCustom* event){
            StoryAlert::showAlert("Ảnh chưa được lưu. Hãy cấp quyền trong cài đặt để lưu ảnh", "OK", "");
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( deny_listener, this);
        
        this->scheduleOnce([=](float){
            button->setTouchEnabled(true);
            button->setOpacity(255);
        }, 1.0f, "enable_Save");
        
        this->scheduleOnce([=](float){
            //chờ render
            if(FileUtils::getInstance()->isFileExist(full_name)){
                MJ_PLATFORM_CONFIG.saveScreenShot(full_name, name);
            }
        }, 0.01f, "save_Screenshot");
       
    });
}

void CompletePicture::onNext(Ref* sender){
    onComplete([=]{
        auto layout_block_touch = Layout::create();
        layout_block_touch->setContentSize(Director::getInstance()->getWinSize());
        layout_block_touch->setTouchEnabled(true);
        this->addChild(layout_block_touch, INT_MAX-1);
        
        onFinishGameVMonkey();
    });
}
void CompletePicture::onAlertOK(cocos2d::Ref * sender){
    onNext(NULL);
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback CompletePicture::onLocateClickCallback(const std::string &callback_name) {
  
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(CompletePicture, onNext),
        CLICK_MAP(CompletePicture, onShare),
        CLICK_MAP(CompletePicture, onClickSpeaker)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

ui::Widget::ccWidgetTouchCallback CompletePicture::onLocateTouchCallback(const std::string &callback_name){
    if (callback_name == "onTouchObject") {
        return CC_CALLBACK_2(CompletePicture::onTouchObject, this);
    }
    
    return nullptr;
}


