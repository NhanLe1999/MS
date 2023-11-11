//
//  ImmortalCircle.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 9/18/18.
//

#include "ImmortalCircle.h"
#define CSB_NAME "csb/game/ImmortalCircle/gacma.csb"

USING_NS_CC;
INITIALIZE_READER(ImmortalCircle);

std::once_flag ImmortalCircle_reader;
ImmortalCircle* ImmortalCircle::createGame(std::string json_file, bool isAutoPlay){
    
    std::call_once(ImmortalCircle_reader, []{
        REGISTER_CSB_READER(ImmortalCircle);
    });
    
    auto p = static_cast<ImmortalCircle*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

ImmortalCircle* ImmortalCircle::createGame(){
    
    std::call_once(ImmortalCircle_reader, []{
        REGISTER_CSB_READER(ImmortalCircle);
    });
    
    auto p = static_cast<ImmortalCircle*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB();
        return p;
    }
    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void ImmortalCircle::didLoadFromCSB(std::string json_file, bool isAutoPlay){
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    mj::GameObject go;
    go.text = "china";
    go.audio_path = "sounds/immortalCircle/this.mp3";
    go.image_path = "";
    
    _word_list.push_back(go);
    
    screen_size = Director::getInstance()->getVisibleSize();
    loadFrame();
}

void ImmortalCircle::gameConfig(){
    _word = _word_list.back();
    if(_word_list.size() > 1){
        _word_list.pop_back();
    }
    
    not_attack_start_pos = {
        Vec2(-screen_size.width/2, screen_size.height*0.1),
        Vec2(screen_size.width*1.5, screen_size.height*0.1)
    };
    
    not_attack_end_pos = {
        circle->convertToNodeSpace( Vec2(screen_size.width*2.5, screen_size.height*0.1)),
        circle->convertToNodeSpace( Vec2(-screen_size.width*2.5, screen_size.height*0.1))
    };
    
    attack_start_pos = {
//        Vec2(-screen_size.width/2, screen_size.height*0.55),
//        Vec2(screen_size.width*1.5, screen_size.height*0.55),
        Vec2(-screen_size.width/2, -screen_size.height*0.3),
        Vec2(screen_size.width*1.5, -screen_size.height*0.3),
        Vec2(-screen_size.width*0.2, -screen_size.height*0.3),
        Vec2(screen_size.width*0.8, -screen_size.height*0.3)
    };
    
    attack_end_pos = {
//        Vec2(front_solider->getPositionX() - front_solider->getContentSize().width*0.8, front_solider->getContentSize().height*0.25),
//        Vec2(front_solider->getPositionX() + front_solider->getContentSize().width*0.8, front_solider->getContentSize().height*0.25),
        Vec2(front_solider->getPositionX() - front_solider->getContentSize().width*0.2, front_solider->getContentSize().height*0.2),
        Vec2(front_solider->getPositionX(), front_solider->getContentSize().height*0.0),
        Vec2(front_solider->getPositionX() + front_solider->getContentSize().width*0.2, front_solider->getContentSize().height*0.2),
        Vec2(front_solider->getPositionX(), front_solider->getContentSize().height*0.0)
    };

}

void ImmortalCircle::didLoadFromCSB(){
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("words")->value;
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    this->generateData(json_data.GetArray(), images);
    
    max_game = document.FindMember("nubmer_game")->value.GetInt();
    max_tap = document.FindMember("number_tap")->value.GetInt();
    
    if(max_game > _word_list.size()){
        max_game = (int) _word_list.size();
    }

    screen_size = Director::getInstance()->getVisibleSize();

    speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
    speaker->setTouchEnabled(false);
    speaker->addClickEventListener([=](cocos2d::Ref* sender) 
        {
            AUDIO_ENGINE->playSound("sounds/immortalCircle/Tap on the shark to get the word.mp3");
            this->unschedule("sound_guide");
            showGuideGame();
        });
    this->addChild(speaker);
    this->loadFrame();
}

void ImmortalCircle::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){

    //get answers
    for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
        rapidjson::Value& json_answer=value_data[i].FindMember("word")->value;
 
        mj::GameObject go;
        auto letter = json_answer.FindMember("text")->value.GetString();
        auto pImage = math::resource::ResHelper::getImage(images, letter);
        go.text = pImage->getText();
        go.audio_path = pImage->getAudios().front();
        go.image_path = pImage->path;
        _word_list.push_back(go);
    }
}

void ImmortalCircle::loadFrame(){
    
    this->setIgnoreAnchorPointForPosition(true);
    this->setAnchorPoint(Point(0.5, 0.9));
    
    root_layout = utils::findChild<Layout*>(this, "root_panel");
   
    circle = utils::findChild(this, "circle");
    base_flag = utils::findChild(this, "base_flag");
    auto ocean = utils::findChild(this, "ocean");
    
    back_solider = GAFWrapper::createGAF("gaf/immortalCircle/chien_si_hai_quan_phia_sau/chien_si_hai_quan_phia_sau.gaf");
    circle->addChild(back_solider);
    back_solider->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM );
    back_solider->setPosition(Vec2(circle->getContentSize().width*0.5, circle->getContentSize().height*0.50));
    back_solider->playSequence("idle", true);
    back_solider->setTouchEnabled(true);
    back_solider->addClickEventListener([=](cocos2d::Ref* sender)
        {
            showGuideGame();
        });
    
    auto vn_flag = GAFWrapper::createGAF("gaf/immortalCircle/flag/flag.gaf");
    circle->addChild(vn_flag, 10);
    vn_flag->setAnchorPoint(Point(0.15,0.12));
    vn_flag->setPosition(base_flag->getPosition());
    vn_flag->play(true);
    vn_flag->setTouchEnabled(true);
    vn_flag->addClickEventListener([=](cocos2d::Ref* sender)
        {
            showGuideGame();
        });
    
    front_solider = GAFWrapper::createGAF("gaf/immortalCircle/chien_si_hai_quan_phia_truoc/chien_si_hai_quan_phia_truoc.gaf");
    circle->addChild(front_solider, 100);
    front_solider->setAnchorPoint(Point::ANCHOR_MIDDLE);
    front_solider->setPosition(Vec2(circle->getContentSize().width*0.505, circle->getContentSize().height*0.55));
    front_solider->playSequence("idle", true);
    front_solider->setTouchEnabled(true);
    front_solider->addClickEventListener([=](cocos2d::Ref* sender)
        {
            showGuideGame();
        });
    
    auto wave_asset = gaf::GAFAsset::create("gaf/immortalCircle/song_wave/song_wave.gaf");
    auto ocean_sz = ocean->getContentSize();
    for(int i = 0; i < 3; i++){
        auto wave = GAFWrapper::createGAF(wave_asset);
        wave->setTouchEnabled(true);
        wave->addClickEventListener([=](cocos2d::Ref* sender)
            {
                showGuideGame();
            });
        ocean->addChild(wave);
        wave->setZOrder(10);
        wave->setPosition(Vec2(ocean_sz.width* 0.1 * random(4, 6), ocean_sz.height* 0.1 * (8.5- (i*2.5))));
        wave->play(true);
    }
    //ocean->setTouchEnabled(true);
    //ocean->addClickEventListener([=](cocos2d::Ref* sender)
    //    {
    //        m_contacted = true;
    //        showGuideGame(1);
    //    });
    
//    seal_asset = gaf::GAFAsset::create("gaf/immortalCircle/ca_no/ca_no.gaf");
    
    auto game_name = utils::findChild(this, "game_name");
    game_name->setPosition(screen_size/2);
    AUDIO_ENGINE->playSound("sounds/immortalCircle/Immortal Circle.mp3", false, [=]{
        this->runAction(Sequence::create(DelayTime::create(0.5), Spawn::create(ScaleTo::create(1.5, 1.8), CallFunc::create([=]{
            game_name->runAction(FadeOut::create(1.0));
        }), NULL), CallFunc::create([=]{
            AUDIO_ENGINE->playSound("sounds/immortalCircle/intro.mp3", false, ([=] {
                speaker->setTouchEnabled(true);
                }));
        }), DelayTime::create(1.5), CallFunc::create([=]{
             onGameStart();
        }), NULL));
    });
}

void ImmortalCircle::onGameStart(){
    
    count_tap = 0;
    this->runAction(Sequence::create(DelayTime::create(4), CallFunc::create([=] {
        showGuideGame();
        }),
        nullptr));
    gameConfig();
    if(count_game > 0){
        delay_time = 1.5;
        AUDIO_ENGINE->play("sounds/immortalCircle/gacma_shark_swim.mp3", true);
        this->schedule([=](float){
            createShark();
            delay_time = 3;
            timer++;
        }, delay_time, 1000, 0, "create shark");
    }else{
        this->runAction(Sequence::create(EaseOut::create(ScaleTo::create(2.0, 1), 3), CallFunc::create([=]{
            AUDIO_ENGINE->play("sounds/immortalCircle/gacma_shark_swim.mp3", true);
            this->schedule([=](float){
                createShark();
                timer++;
            }, delay_time, 1000, 0, "create shark");
        }), NULL));
    }
}

void ImmortalCircle::createShark(){
    
    auto a = random(0,7);
    Vec2 shark_pos, shark_goal;
    
    if(a == 0 || a == 1){
         shark_pos = Vec2(not_attack_start_pos[a].x, not_attack_start_pos[a].y * random(2, 4));
         shark_goal = Vec2(not_attack_end_pos[a].x, not_attack_end_pos[a].y * random(2, 3));
    }else{
//        if(a == 2 || a == 3){
//            shark_pos = attack_start_pos[a-2];
//            shark_goal = attack_end_pos[a-2];
//        }else{
            shark_pos = attack_start_pos[random(0,3)];
            shark_goal = attack_end_pos[random(0,3)];
//        }
    }
    
    CCLOG("a: %i", a);
    if(this->getBoundingBox().containsPoint(shark_pos)){
        CCLOG("shark pos: %f, %f", shark_pos.x, shark_pos.y);
    }
    
    auto shark_asset = gaf::GAFAsset::create("gaf/immortalCircle/ca_map/ca_map.gaf");
    auto shark = GAFWrapper::createGAF(shark_asset) ;
//     auto shark = GAFWrapper::createGAF("gaf/immortalCircle/ca_map/ca_map.gaf") ;
    shark->setZOrder(100);
    addChild(shark);
    shark->setPosition(shark_pos);
    shark->playSequence("swim", true);
    shark->setAnchorPoint(Point(0.3, 0.7));
    shark->setTouchEnabled(true);
    shark->addClickEventListener(CC_CALLBACK_1(ImmortalCircle::onClicked, this));
    shark->setTag(0);
    shark->setName("shark");
    
    Text* text = Text::create(_word.text, font_name, font_sz);
    text->enableOutline(Color4B::BLACK, 2);
    text->setTextColor(Color4B::WHITE);
    text->setTextVerticalAlignment(TextVAlignment::CENTER);
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    shark->addChild(text, 9);
    text->setPosition(Vec2(shark->getContentSize().width*0.5, shark->getContentSize().height*0.75));
    text->setName("text");
    
    if(shark_pos.x > screen_size.width/2){
        shark->setFlippedX(true);
        text->setFlippedX(true);
    }
    
    auto time = random(3, 5);
    auto shark_positive = Vec2(circle->convertToWorldSpace(shark_goal).x, abs( circle->convertToWorldSpace(shark_goal).y));
//    shark_pos.getDistance(shark_goal) / vantoc;
    shark->runAction(Sequence::create(/*EaseOut::create(*/MoveTo::create(time, shark_positive)/*circle->convertToWorldSpace(shark_goal)), 4)*/ , CallFunc::create([=]{
        CCLOG("shark end pos: %f, %f", circle->convertToWorldSpace(shark_goal).x, circle->convertToWorldSpace(shark_goal).y );
        if(a != 0 && a != 1){
            AUDIO_ENGINE->play("sounds/immortalCircle/gacma_hit.mp3", false, 1.0, [=]{
                int rand = random(1, 9);
                string path = StringUtils::format( "sounds/immortalCircle/gacma_yell%d.mp3", rand) ;
                AUDIO_ENGINE->play(path);
            });
            shark->setTouchEnabled(false);
            front_solider->playSequence("hit", false, [=](gaf::GAFObject* obj, string path ){
                front_solider->playSequence("idle", true);
            });
            back_solider->playSequence("hit", false, [=](gaf::GAFObject* obj, string path ){
                back_solider->playSequence("idle", true);
            });
            
            GAFWrapper* boom = GAFWrapper::createGAF("gaf/immortalCircle/hieu_ung_no/hieu_ung_no.gaf");
            shark->addChild(boom, 99);
            boom->setPosition(Vec2(shark->getContentSize().width*0.7, shark->getContentSize().height*0.7));
            boom->play(false, [=](gaf::GAFObject* obj){
                boom->removeFromParent();
            });
            text->runAction(FadeOut::create(0.5));
            AUDIO_ENGINE->play("sounds/immortalCircle/gacma_shark_die.mp3");
            shark->playSequence("lan", false, [=](gaf::GAFObject* obj, string path){
                shark->removeFromParent();
            });
        }else{
             shark->removeFromParent();
        }
    }), NULL));
}

void ImmortalCircle::showGuideGame()
{
    CCLOG("cliked");
    this->unschedule("sound_guide");
    this->schedule([=](float) {
        {
            if (FileUtils::getInstance()->isFileExist("sounds/immortalCircle/Tap on the shark to get the word.mp3"))
                AUDIO_ENGINE->playSound("sounds/immortalCircle/Tap on the shark to get the word.mp3", false, ([=] {
                    showGuideGame();
                    }));
        }
    }, 10, 100, 10, "sound_guide");  
}

void ImmortalCircle::onEnter(){
    MJMainGBase::onEnter();
    AUDIO_ENGINE->play("sounds/immortalCircle/gacma_bgm.mp3", true);
    AUDIO_ENGINE->play("sounds/immortalCircle/gacma_amb_seagull.mp3", true);
    AUDIO_ENGINE->play("sounds/immortalCircle/gacma_amb_wave.mp3", true);
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
}

void ImmortalCircle::onFinishGame(){
    this->unschedule("sound_guide");
    MJMainGBase::onFinishGame();
}

void ImmortalCircle::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
}

void ImmortalCircle::onClicked(cocos2d::Ref *sender){
    CCLOG("shark clicked");
    // schedule sound guiding
    showGuideGame();

    //TODO
    //set toc do
    CCLOG("%f", timer);
    if(timer > 4){
        delay_time++;
    }else{
        if(delay_time > 2.5){
            delay_time -= 0.2;
        }else{
            delay_time = 3;
        }
    }
    timer = 0;
    
    CCLOG("%f", delay_time);
    
    GAFWrapper* shark = (GAFWrapper*)sender;
    shark->setTouchEnabled(false);
    shark->stopAllActions();
    
    AUDIO_ENGINE->play("sounds/immortalCircle/gacma_correct.mp3");
    GAFWrapper* seal = GAFWrapper::createGAF("gaf/immortalCircle/ca_no/ca_no.gaf");
    addChild(seal, INT_MAX -2);
    seal->setAnchorPoint(Point(0.5, 0.4));
    seal->setPositionY(screen_size.height*0.5);
    seal->playSequence("idle", true);
    
    Vec2 seal_pos = Vec2(0, shark->getPositionY());
    Vec2 seal_goal = Vec2(0, shark->getPositionY());
  
    if(shark->getPositionY() < screen_size.height* 0.25){
        shark->setLocalZOrder(INT_MAX - 2);
    }else if (shark->getPositionY() < screen_size.height* 0.5) {
        shark->setLocalZOrder(INT_MAX - 3);
    }else{
        shark->setLocalZOrder(INT_MAX - 4);
    }
    
    seal->setLocalZOrder(shark->getLocalZOrder()+1);
    
    if(shark->getPositionX() >= screen_size.width/2 ){
         if(!shark->isFlippedX()){
             seal_pos.x = shark->getPositionX() - shark->getContentSize().width*0.2 ;
        }else{
            seal_pos.x = shark->getPositionX() - seal->getContentSize().width*0.6 ;
        }
        seal->setPositionX(-screen_size.width/2);
        seal_goal.x = screen_size.width*1.5;
    }else {
        seal->setFlippedX(true);
        if(!shark->isFlippedX()){
            seal_pos.x = shark->getPositionX() + seal->getContentSize().width*0.6;
        }else{
            seal_pos.x = shark->getPositionX() + shark->getContentSize().width*0.2;
        }
        seal->setPositionX(screen_size.width*1.5);
        seal_goal.x = -screen_size.width*0.5;
    }
    
    seal->runAction(Sequence::create(EaseOut::create(MoveTo::create(0.3, seal_pos), 4), CallFunc::create([=]{
        seal->playSequence("fight", false, [=](gaf::GAFObject* obj, string path){
            seal->playSequence("water", true);
            shark->playSequence("hit", true);
            this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]{
                seal->playSequence("stop_fight", false, [=](gaf::GAFObject* obj, string path){
                    seal->playSequence("idle", true);
                    auto text = utils::findChild<Text*>(shark, "text");
                    auto text_pos = shark->convertToWorldSpace(text->getPosition());
                    text->setFlippedX(false);
                    text->retain();
                    text->removeFromParent();
                    this->addChild(text, 99);
                    text->setPosition(text_pos);
                    text->runAction(Spawn::create(MoveBy::create(0.5, Vec2(0, 50)), ScaleTo::create(0.5, 1.5), FadeOut::create(0.5), NULL));
                    AUDIO_ENGINE->play(_word.audio_path);
                    AUDIO_ENGINE->play("sounds/immortalCircle/gacma_shark_die.mp3");
                    shark->playSequence("die", false, [=](gaf::GAFObject* obj, string path){
                        shark->removeFromParent();
                        seal->runAction(Sequence::create(EaseIn::create(MoveTo::create(0.5, seal_goal), 4), CallFunc::create([=]{
                            seal->removeFromParent();
                            
                        }), NULL));
                    });
                });
            }), NULL));
        });
    }), NULL));
    
    count_tap++;
    if(count_tap == max_tap){
        count_game++;
        this->unschedule("create shark");
        this->unschedule("sound_guide");
        this->runAction(Sequence::create(DelayTime::create(3.5), CallFunc::create([=]{
            
            for(auto c: this->getChildren()){
                if( c->getName() == "shark"){
                    ((GAFWrapper*)c)->setTouchEnabled(false);
                }
            }
            
            auto phao_hoa = GAFWrapper::createGAF("gaf/phao_hoa/phao_hoa.gaf");
            this->addChild(phao_hoa, INT_MAX -2);
            phao_hoa->setPosition(screen_size/2);
            std::string str = StringUtils::format("sounds/immortalCircle/khen_%d.mp3", random(1,4));
            AUDIO_ENGINE->play(str);
            AUDIO_ENGINE->play("sounds/end_game/firework.mp3");
            
            phao_hoa->play(false, [=](gaf::GAFObject *obj){
                phao_hoa->removeFromParent();
                if(count_game == max_game){
                    this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]{
                            
                            this->onFinishGame();
                    }), NULL));
                    
                    
                }else{
                    
                    this->onGameStart();
                }
            });
        }) , NULL));
    }
}

