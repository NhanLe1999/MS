//
//  CaterpillaRacing.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/16/18.
//

#include "CaterpillaRacing.h"
#include "StoryConfigManager.h"
USING_NS_CC;
INITIALIZE_READER(CaterpillaRacing);
#define speed_opacity 10

std::once_flag CaterpillaRacing_reader;
#define CSB_NAME "csb/game/CaterpillaRacing/CaterpillaRacing.csb"
#define _game_path "json/CaterpillaRacing/"
#define _audio_path "sample_game/sounds/"

#define MAX_MOVE_SPEED 450
#define MIN_MOVE_SPEED 250

CaterpillaRacing* CaterpillaRacing::createGame(std::string json_file, bool isAutoPlay){
    std::call_once(CaterpillaRacing_reader, []{
        REGISTER_CSB_READER(CaterpillaRacing);
        REGISTER_CSB_READER(CaterpillaRacing_Item);
    });
    
    auto p = static_cast<CaterpillaRacing*>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if(p){
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void CaterpillaRacing::didLoadFromCSB(std::string json_file, bool isAutoPlay){
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    number_tap = document.FindMember("number_tap")->value.GetInt();
    rapidjson::Value& data = document.FindMember("data")->value;
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    for (int i = 0; i < data.Size(); i++) {
        auto json_item = data[i].FindMember("images")->value.GetArray();
        for(int j = 0; j < json_item.Size(); j++){
            CaterpillaData::Game game;

            auto letter = json_item[j].FindMember("item")->value.FindMember("img_path")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
			bool is_image = false;
			if (json_item[j].FindMember("item")->value.HasMember("is_image")) {
                is_image = json_item[j].FindMember("item")->value.FindMember("is_image")->value.GetBool();
			}
           
            game.text = pImage->getText("name_n");
            game.audio_path = pImage->getAudios("name_n").front();
            
            if(is_image){
                std::pair<std::string, std::string> image;
                image.first = pImage->path;
                image.second = pImage->getAudios("name_1").front();
                game.images.push_back(image);
            }
            _games.push_back(game);
        }
    }
    
    screen_size = Director::getInstance()->getVisibleSize();
    _isAutoPlay = isAutoPlay;
    
    loadFrame();
}

//void CaterpillaRacing::didLoadFromCSB(std::string json_file, bool isAutoPlay){
//    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
//    rapidjson::Document document;
//    document.Parse<0>(str.c_str());
//    
//    number_tap = document.FindMember("number_tap")->value.GetInt();
//    rapidjson::Value& data = document.FindMember("data")->value;
//
//    for (int i = 0; i < data.Size(); i++) {
//        CaterpillaData::Game game;
//        game.text = data[i].FindMember("text")->value.GetString() ;
//        game.audio_path = data[i].FindMember("audio")->value.GetString();
//        rapidjson::Value& images = data[i].FindMember("images")->value;
//        for(int j = 0; j < images.Size(); j++){
//            std::pair<std::string, std::string> image;
//            image.first = images[j].FindMember("item")->value.FindMember("img_path")->value.GetString();
//            image.second = images[j].FindMember("item")->value.FindMember("img_audio")->value.GetString();
//            game.images.push_back(image);
//        }
//        _games.push_back(game);
//    }
//
//    screen_size = Director::getInstance()->getVisibleSize();
//    _isAutoPlay = isAutoPlay;
//
//    loadFrame();
//}

void CaterpillaRacing::loadFrame(){
    
    root_layout = utils::findChild<Layout*>(this, "root_layout");
   
    start_bg = ImageView::create("games/caterpillaracing/start.png");
    start_bg->setAnchorPoint(Point(0,0.5));
    root_layout->addChild(start_bg,2);
    start_bg->setPosition(Vec2(0, screen_size.height/2));
    start_bg->setName("start");
   
    _bg = Layout::create();
    root_layout->addChild(_bg,1);
    _bg->setPosition(Vec2(start_bg->getContentSize().width - 9, 0));
    
    //character panel config
    snail_panel = Layout::create();
    root_layout->addChild(snail_panel,2);
    snail_panel->setPosition(Vec2(-screen_size.width*0.2, screen_size.height*0.33));
    
    ant_panel = Layout::create();
    root_layout->addChild(ant_panel,2);
    ant_panel->setPosition(Vec2( -screen_size.width*0.2, screen_size.height*0.22));
    
    sau_panel = Layout::create();
    root_layout->addChild(sau_panel,2);
    sau_panel->setPosition(Vec2( -screen_size.width*0.2, screen_size.height*0.15));
    
    //oc sen
    snail = GAFWrapper::createGAF("gaf/caterpillaracing/Snail/Oc sen bo.gaf");
    snail_panel->addChild(snail);
    snail->setAnchorPoint(Point(1,0));
    snail->setPosition(snail_panel->getContentSize()/2);
    snail->setName("gaf");
    
    //kien
    ant = GAFWrapper::createGAF("gaf/caterpillaracing/Ant/Kien chay(1).gaf");
    ant_panel->addChild(ant);
    ant->setAnchorPoint(Point(0.8,0));
    ant->setPosition(ant_panel->getContentSize()/2);
    ant->setName("gaf");

    //sau
    sau = GAFWrapper::createGAF("gaf/caterpillaracing/Head/Dau sau bo 1.gaf");
    sau_panel->addChild(sau);
    sau->setAnchorPoint(Point(0.5,0.2));
    sau->setName("gaf");
    
    //road
    auto road_1 = ImageView::create("games/caterpillaracing/road.png");
    _bg->addChild(road_1,100000);
    road_1->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    road_1->setPosition(Vec2(0, screen_size.height*0.5));
    road_1->setName("road");
    
    auto road_2 = ImageView::create("games/caterpillaracing/road.png");
    _bg->addChild(road_2, 100000 - 1);
    road_2->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    road_2->setPosition(Vec2(road_1->getContentSize().width, screen_size.height*0.5));
    
    auto road_3 = ImageView::create("games/caterpillaracing/road.png");
    _bg->addChild(road_3, 100000 - 2);
    road_3->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    road_3->setPosition(Vec2(road_1->getContentSize().width*2, screen_size.height*0.5));
    
    gameConfig();
    createBubble(start_bg,0);
    for(auto c: start_bg->getChildren()){
        c->setScale(0);
    }
    
    createBubble(road_1);
    
    for(auto c: road_1->getChildren()){
        c->setScale(0);
    }
//    createBubble(road_2);
//    createBubble(road_3);
    setGameState(0);
    
}

//game config
void CaterpillaRacing::gameConfig(){
    game = _games[0];
    tap = 0;
    move_speed = 300;
    positions.clear();
    
    auto road_width = utils::findChild(_bg, "road")->getContentSize().width;
    auto number_bubble = road_width/300 - 1;
    for(int i = 0; i < number_bubble; i++ ){
        auto space = 320;
        if(i == 0){
            space = 280;
        }
        positions.push_back(Vec2(space*(i+0.5), screen_size.height*(random(0.65, 0.75))));
    }
}

void CaterpillaRacing::setGameState(int game_state){
    if(game_state == _game_state) return;
    switch (game_state) {
        case 0:
            CCLOG("intro");
            showIntroGame();
            break;
        case 1:
            CCLOG("playing");
            //move bg
            startGame();
            break;
       
        case 2:
            CCLOG("ending");
            onNextStage();
            break;
        case 3:
            CCLOG("transitioning");
            onNextGame();
            break;
        default:
            break;
    }
}

void CaterpillaRacing::startGame(){
    inactiveMode();
    start_bg->runAction(Sequence::create(MoveTo::create(start_bg->getContentSize().width/move_speed, Vec2(-start_bg->getContentSize().width, start_bg->getPositionY())), CallFunc::create([=]{
        start_bg->removeFromParentAndCleanup(true);
        this->schedule([=](float){
            _bg->stopAllActions();
            _bg->runAction(MoveBy::create(1, Vec2(-move_speed, 0)));
            timer++;
            if (timer >= 6) {
                if (move_speed > MIN_MOVE_SPEED) {
                    move_speed -= 50;
                }
            }
        }, 0.5f, "move_bg");
    }) ,NULL));
  
    _bg->runAction(RepeatForever::create(MoveBy::create(1, Vec2(-move_speed, 0))));
    
    run_id = AUDIO_ENGINE->play("sounds/caterpillaracing/run.mp3", true);
    ant_panel->runAction(Sequence::create(CallFunc::create([=]{
        ant->playSequence("Kien_bat_dau_chay", false, [=](gaf::GAFObject* obj, std::string path){
            ant->playSequence("Kien_chay_mat_vui", true);
        });
    }), MoveBy::create(2, Vec2(200, 0)), CallFunc::create([=]{
        this->schedule([=](float){
            ant_panel->runAction(MoveBy::create(0.5f, Vec2((ant_panel->getPositionX() > sau->getPositionX())? -10: 40 ,0)));
            snail_panel->runAction(MoveBy::create(0.5f, Vec2((snail_panel->getPositionX() > sau->getPositionX())? -15: 5 ,0)));
        }, 1.0f, "update pos");
    }) ,NULL));
   
    sau->playSequence("Sau_chay", true);
    this->schedule(schedule_selector(CaterpillaRacing::bg_update));
}

void CaterpillaRacing::createBubble(Node* holder, float scale){
    for(int i = 0; i < positions.size(); i++){
        auto delta = random(10, 15);
        auto j = random(0,(int) _games.size()-1);
        int showImage = 0;
        if (_games[j].images.size() > 0) {
            if (_games[j].images.front().first != "") {
                showImage = 1;
            }
        }
        auto item = CaterpillaRacing_Item::createItem(_games[j], showImage);
        item->setAnchorPoint(Point(0.5, 0.5));
        holder->addChild(item, 10);
        item->setPosition(positions[i]);
        item->setDelegate(this);
        item->setScale(scale);
        item->bubbleEffect(delta, i);
        item->setEnabled(true);
        item->setName("item_" + cocos2d::StringUtils::toString(i));
    }
}


void CaterpillaRacing::bg_update(float dt){
    auto bg_children = _bg->getChildren();
    for (auto c : bg_children) {
        auto bb = c->getBoundingBox();
        bb.origin = _bg->convertToWorldSpace(bb.origin);
        if (bb.getMinX() > screen_size.width && bb.getMinX() < screen_size.width + 100 && c->getChildrenCount() == 0)
        {
            createBubble(c);
        }
        if (bb.getMaxX() < 0) {
            if(tap < number_tap){
                c->setPositionX(c->getPositionX() + c->getContentSize().width*3 );
				c->setLocalZOrder(c->getLocalZOrder() - 3);
                c->removeAllChildren();
                //createBubble(c);
            }else{
                finish_pos =Vec2 ( c->getPositionX() + c->getContentSize().width*2 -50, c->getPositionY());
                this->unschedule(schedule_selector(CaterpillaRacing::bg_update));
                this->unschedule("update pos");
                setGameState(2);
            }
        }
    }
}

void CaterpillaRacing::showIntroGame()
{   //"games/caterpillaracing/Caterpillar race.png"
    auto img = ImageView::create("games/caterpillaracing/Caterpillar race.png");
    img->setPosition(start_bg->getContentSize()/2+Size(-150,150) );
    // text->setColor(Color3B(cocos2d::Color4B.r, cocos2d::Color4B.g, cocos2d::Color4B.b));
    img->setName("text");
    img->setScale(1.3f);
    img->setOpacity(0);
    start_bg->addChild(img);
    img->runAction(Sequence::create(DelayTime::create(0.5f), FadeIn::create(0.75f), DelayTime::create(0.25f), CallFunc::create([=]() {
        AUDIO_ENGINE->play("sounds/caterpillaracing/Caterpillar race.mp3", false, 1.0, [=]() {

            ImageView* start_flag = ImageView::create(CONFIG_MANAGER->isGameVMonkey() ? "games/caterpillaracing/start_flag_vi.png" : "games/caterpillaracing/start_flag.png");

            root_layout->addChild(start_flag);
            start_flag->setPosition(Vec2(screen_size.width * 0.5, screen_size.height * 0.6));
            start_flag->setScale(0);
            start_flag->setLocalZOrder(100);

            int run_id = AUDIO_ENGINE->play("sounds/caterpillaracing/run.mp3", true);
            snail_panel->runAction(MoveTo::create(1.8f, Vec2(screen_size.width * 0.18, screen_size.height * 0.33)));
            snail->playSequence("oc_sen_bo", true, [=](gaf::GAFObject* obj, const std::string string) {
                AUDIO_ENGINE->stopEffect(run_id);
                });

            ant_panel->runAction(MoveTo::create(1.5f, Vec2(screen_size.width * 0.18, screen_size.height * 0.22)));
            ant->playSequence("kien_chay_vao", false, [=](gaf::GAFObject* obj, string path) {
                ant->playSequence("kien_chuan_bi", true);
                });

            sau_panel->runAction(MoveTo::create(1.6f, Vec2(screen_size.width * 0.18, screen_size.height * 0.15)));
            sau->playSequence("Sau_chay", false, [=](gaf::GAFObject* obj, string path) {
                sau->playSequence("Sau_chuan_bi", true);
                });

            //show guide, sound guide
            int intro_id = AUDIO_ENGINE->play(CONFIG_MANAGER->isGameVMonkey() ? "sounds/caterpillaracing/intro_vi.mp3" : "sounds/caterpillaracing/intro.mp3");
            img->runAction(FadeOut::create(0.2f));
            //show flag
            cocos2d::experimental::AudioEngine::setFinishCallback(intro_id, [=](int id, std::string string) {
                start_flag->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.5f, 2)), CallFunc::create([=] {
                    int audioReadyGo = AUDIO_ENGINE->play(CONFIG_MANAGER->isGameVMonkey() ? "sounds/caterpillaracing/ready_go_vi.mp3" : "sounds/caterpillaracing/ready_go.mp3");
                    cocos2d::experimental::AudioEngine::setFinishCallback(audioReadyGo, [=](int id, std::string string) {
                        this->setGameState(1);
                        // show bubble in start background
                        for (int i = 0; i < start_bg->getChildren().size(); i++) {
                            Layout* item = utils::findChild<Layout*>(start_bg, "item_" + cocos2d::StringUtils::toString(i));
                            if (item) item->runAction(ScaleTo::create(0.2 * (i + 1), 1));
                        }

                        // show bubble in next background
                        auto bg_children = _bg->getChildren();
                        for (auto road : bg_children) {
                            for (int i = 0; i < road->getChildren().size(); i++) {
                                Layout* item = utils::findChild<Layout*>(road, "item_" + cocos2d::StringUtils::toString(i));
                                if (item) item->runAction(ScaleTo::create(0.2 * (i + 1), 1));
                            }
                        }
                    });
                }), DelayTime::create(0.5f), ScaleTo::create(0.5f, 4.5f), CallFunc::create([=] {
                    AUDIO_ENGINE->play("sounds/caterpillaracing/start_run.mp3");
                    start_flag->setVisible(false);
                    start_flag->removeFromParent();
                }), NULL));
            });
                        
            });
     }), NULL));
   

   
}

void CaterpillaRacing::onEnter(){
    MJMainGBase::onEnter();
//    AUDIO_ENGINE->playMusic("sounds/caterpillaracing/music.mp3");
    this->setContentSize(screen_size);
    this->unscheduleUpdate();
//    this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=]{
//        this->showGuideGame();
//    }), NULL));
}

void CaterpillaRacing::onClickedDelegate(cocos2d::Ref *sender){
    CaterpillaRacing_Item* item = (CaterpillaRacing_Item*)sender;
    CCLOG("item click");
    tap++;
    if (_audioInactiveMode != -1) {
        AUDIO_ENGINE->stopEffect(_audioInactiveMode);
    }
    root_layout->unschedule("inactivemode");

    inactiveMode();
    //update move_speed
    if (timer <= 2 ){
        if(move_speed < MAX_MOVE_SPEED){
            move_speed += 50;
        }
    }
    else if (timer < 5) {
        if (move_speed < MAX_MOVE_SPEED) {
            move_speed += 30;
        }
    }
    timer = 0;
    
    std::string item_audio = item->getAudioPath();
    int pop_id = AUDIO_ENGINE->play("sounds/caterpillaracing/Click.mp3");
    cocos2d::experimental::AudioEngine::setFinishCallback(pop_id, [=](int id, string path){
         AUDIO_ENGINE->play(item_audio);
    });
  
    auto img = utils::findChild(item, "child");
    auto pos = item->convertToWorldSpace(img->getPosition());
    img->retain();
    img->removeFromParent();
    item->removeFromParent();
    root_layout->addChild(img, 9);
    img->setPosition(pos);
    img->release();
    
    GAFWrapper* star = GAFWrapper::createGAF("gaf/Star/cloudcatcher_effectunder.gaf");
    star->setAnchorPoint(Point(0.4, 0.5));
    star->setScale(1.3);
    root_layout->addChild(star, 8);
    star->setPosition(pos);
    star->playSequence("hit", false, [=](gaf::GAFObject* obj, string path){
        star->removeFromParent();
    });
    
    img->runAction(Sequence::create(ScaleBy::create(0.2f, 1.2), DelayTime::create(0.5), Spawn::create(ScaleTo::create(0.5f, 0.2), MoveTo::create(0.5f, sau_panel->convertToWorldSpace(Vec2(sau->getBoundingBox().getMaxX(), sau->getPositionY()))), CallFunc::create([=]{
        
        cocos2d::experimental::AudioEngine::setVolume(run_id, 0.2);
        int eat_id = AUDIO_ENGINE->play("sounds/caterpillaracing/eat.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(eat_id, [=](int id, string path){
            cocos2d::experimental::AudioEngine::setVolume(run_id, 1);
        });
        
        sau->playSequence("Sau_an", false, [=](gaf::GAFObject* obj, std::string path){
            sau->playSequence("Sau_chay", true);
        });
        ant->playSequence("Kien_chay_mat_meu", false, [=](gaf::GAFObject* obj, std::string path){
            ant->playSequence("Kien_chay_mat_vui", true);
        });
    }), NULL), CallFunc::create([=]{
        img->removeFromParent();
        if(tap > 15) {
            sau_panel->runAction(MoveBy::create(0.2, Vec2(-30,0)));
        }
        GAFWrapper* duoi = GAFWrapper::createGAF("gaf/caterpillaracing/Tail/Duoi sau bo.gaf");
        sau->addChild(duoi, -tap);
        duoi->setAnchorPoint(Point(0.5, 0.5));
        duoi->setPosition(Vec2(sau->convertToNodeSpace(sau_panel->getPosition()).x-sau->getContentSize().width*0.55, sau->convertToNodeSpace(sau_panel->getPosition()).y + 12));
        sau->runAction(MoveBy::create(0.1f, Vec2(40 ,0)));
        duoi->playSequence("Duoi_moc", false, [=](gaf::GAFObject* obj, std::string path){
            duoi->playSequence("Duoi_chay", true);
        });
        
    }), NULL));
}

void CaterpillaRacing::onNextStage(){
    
    auto block_touch = Layout::create();
    block_touch->setContentSize(Director::getInstance()->getWinSize());
    block_touch->setTouchEnabled(true);
    this->addChild(block_touch, 1000);
    
    finish_pos = _bg->convertToWorldSpace(finish_pos);
    //create goal
    finish_bg = ImageView::create("games/caterpillaracing/goal.png");
    finish_bg->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    root_layout->addChild(finish_bg, 1);
    finish_bg->setPosition(finish_pos);
    
    createBubble(finish_bg);
    
    // create road finnish
    auto road_finnish = ImageView::create("games/caterpillaracing/road.png");
    root_layout->addChild(road_finnish,1);
    road_finnish->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    road_finnish->setPosition(Vec2(finish_pos.x + finish_bg->getContentSize().width, finish_pos.y + 2));
    
    //create turtle
    GAFWrapper* turtle = GAFWrapper::createGAF("gaf/caterpillaracing/Rua nhay/Rua nhay.gaf");
    finish_bg->addChild(turtle);
    turtle->setPosition(Vec2(finish_bg->getContentSize().width*0.6, finish_bg->getContentSize().height*0.5));
    turtle->play(true);
    
    road_finnish->runAction(MoveBy::create(finish_bg->getPositionX()/move_speed,Vec2(-3-finish_pos.x,0)));
    
    finish_bg->runAction(Sequence::create(MoveTo::create(finish_bg->getPositionX()/move_speed, Vec2(-2, screen_size.height*0.5)), CallFunc::create([=]{
         this->unschedule("move_bg");
        //sau_panel->runAction(Sequence::create(CallFunc::create([=]{
        //    //AUDIO_ENGINE->play("sounds/caterpillaracing/yah.mp3");
        //    sau->pauseAnimation();
        //    for(auto c: sau->getChildren()){
        //        c->pause();
        //    }
        //}), NULL));
        
        ant_panel->runAction(Sequence::create(MoveTo::create(1.0f, Vec2(screen_size.width*0.65, ant_panel->getPositionY())), CallFunc::create([=]{
            ant->pauseAnimation();
        }), NULL));
        
        snail_panel->runAction(Sequence::create(MoveTo::create(1.5f, Vec2(screen_size.width*0.6, snail_panel->getPositionY())), CallFunc::create([=]{
            snail->pauseAnimation();
        }), NULL));
        
        //create finish flag
        ImageView* goal_flag = ImageView::create(CONFIG_MANAGER->isGameVMonkey() ? "games/caterpillaracing/finish_flag_vi.png":"games/caterpillaracing/finish_flag.png");
        root_layout->addChild(goal_flag, 999999);
        goal_flag->setPosition(screen_size/2);
        goal_flag->setScale(0);
        
        AUDIO_ENGINE->stopAllAudio();
        AUDIO_ENGINE->play("sounds/caterpillaracing/winner.mp3", false, 1, [=]() {
            goal_flag->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.5f, 1.2)), DelayTime::create(0.3f), ScaleTo::create(0.8, 4), CallFunc::create([=] {
                goal_flag->removeFromParent();
                _games.clear();
                this->setGameState(3);
                }), NULL));
            });
  
    }), NULL) );
}

void CaterpillaRacing::onNextGame(){
    AUDIO_ENGINE->stopAllAudio();
    
    AUDIO_ENGINE->play("sounds/caterpillaracing/ending.mp3");
    //create transitions
    auto trans_layout = cocos2d::ui::Layout::create();
    trans_layout->setContentSize(screen_size);
    trans_layout->setTouchEnabled(true);
    this->addChild(trans_layout, 999999);
    trans_layout->setBackGroundColor(Color3B::BLACK);
    trans_layout->setBackGroundImage("games/caterpillaracing/goal.png");
    trans_layout->setAnchorPoint(Point(0.2, 0));
    
    GAFWrapper* win = GAFWrapper::createGAF("gaf/caterpillaracing/Ending/Ending.gaf");
    trans_layout->addChild(win,10);
    win->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    win->setPosition(Vec2(trans_layout->getContentSize().width*0.2, trans_layout->getContentSize().height*0.6));
    trans_layout->setScale(1.5);

    sau_panel->runAction(Sequence::create(CallFunc::create([=]{
          //AUDIO_ENGINE->play("sounds/caterpillaracing/yah.mp3");
          sau->pauseAnimation();
          for(auto c: sau->getChildren()){
              c->pause();
          }
      }), NULL));

    //sau->retain();
    //sau->removeFromParentAndCleanup(false);
    //trans_layout->addChild(sau, 10);
    //sau->setAnchorPoint(Point(0.6, 0));
    //sau->setPosition(Vec2(trans_layout->getContentSize().width * 0.5, trans_layout->getContentSize().height * 0.2));
    //trans_layout->setScale(1.5);

    //GAFWrapper* duoi = GAFWrapper::createGAF("gaf/caterpillaracing/Tail/Duoi sau bo.gaf");
    //sau->addChild(duoi, -tap);
    //duoi->setAnchorPoint(Point(0.5, 0.5));
    //duoi->setPosition(Vec2(sau->convertToNodeSpace(sau_panel->getPosition()).x - sau->getContentSize().width * 0.55, sau->convertToNodeSpace(sau_panel->getPosition()).y + 12));
    //sau->runAction(MoveBy::create(0.1f, Vec2(40, 0)));
    //duoi->playSequence("Duoi_moc", false, [=](gaf::GAFObject* obj, std::string path) {
    //    duoi->playSequence("Duoi_chay", true);
    //    });

    Vec2 pos = sau->getPosition();
   
    this->schedule([=](float){
        int deltaX = random(-200, 350);
        int deltaY = random(100, 300);
        GAFWrapper* phaohoa= GAFWrapper::createGAF("gaf/caterpillaracing/phaoHoaNo_standalone/phaoHoaNo_standalone.gaf");
        trans_layout->addChild(phaohoa, 2);
        phaohoa->setPosition(Vec2(pos.x + deltaX, pos.y + deltaY));
//        phaohoa->setPosition(pos);
        phaohoa->setScale(1.5);
        phaohoa->play(false, [=](gaf::GAFObject* obj){
            phaohoa->removeFromParent();
        });
    }, 0.5f, 4, 0.0, "shooting firework");
   
    AUDIO_ENGINE->play("sounds/caterpillaracing/firework.mp3");
    win->play(true);
    
    if(_games.size() == 0){
        this->onFinishGame();
    }else {
        this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([=]{
            trans_layout->removeFromParent();
            root_layout->cocos2d::Node::removeAllChildren();
            loadFrame();
        }), NULL));
        
        if(!_isAutoPlay){
            //            inActiveMode();
        }
    }
}

void CaterpillaRacing::onFinishGame(){
    root_layout->unschedule("inactivemode");
    this->runAction(Sequence::create(DelayTime::create(4), CallFunc::create([=]{
        AUDIO_ENGINE->stopAllAudio();
		MJMainGBase::onCloseGame();
    }), NULL));
}

void CaterpillaRacing::inactiveMode(){
    root_layout->scheduleOnce([=](float) {
        if (_audioInactiveMode != -1) {
            AUDIO_ENGINE->stopEffect(_audioInactiveMode);
        }
        _audioInactiveMode = AUDIO_ENGINE->play("sounds/caterpillaracing/tapon.mp3", false, 1.0f, [=]() {
            inactiveMode();
            });
    }, 10.0f, "inactivemode");
}
