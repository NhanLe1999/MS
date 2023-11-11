
#include "FishingShip.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
INITIALIZE_READER(FishingShip);
#define speed_opacity 15

std::once_flag FishingShip_reader;
#define CSB_NAME "csb/game/FishingShip/FishingShip.csb"
#define _game_path "json/fishingboat/"

//sounds
#define boat_show "sounds/fishingship/boat_show_up.mp3"
#define fish_show "sounds/fishingship/fish_Show_up.mp3"
#define drag_fish "sounds/fishingship/drag_fish.mp3"
#define complete "sounds/fishingship/complete.mp3"
#define drop_fish "sounds/fishingship/fall.mp3"
#define drop_water "sounds/fishingship/water_splash2.mp3"
#define drop "sounds/fishingship/drop.mp3"
#define bg_music "sounds/fishingship/music_fishing.mp3"
#define right "sounds/fishingship/voice.mp3"
#define right2 "sounds/fishingship/Yay.mp3"
#define water_splash "sounds/fishingship/water_splash.mp3"
#define sound_pop "sounds/fishingship/huzz_pop.mp3"
#define question "sounds/fishingship/question.mp3"
#define make_the_word "sounds/fishingship/Make the word.mp3"

#define shadow_path "games/fishingship/fish_shadow.png"

#define audio_path "sample_game/fishingboat/"

FishingShip * FishingShip::createGame(std::string json_file, bool isAutoPlay) {
	std::call_once(FishingShip_reader, [] {
        REGISTER_CSB_READER(FishingShip);
        REGISTER_CSB_READER(FishingShip_Item);
    });
    auto p = static_cast<FishingShip *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(json_file, isAutoPlay);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void FishingShip::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;

    this->generateData(json_data.GetArray());
   
    if(max_game > _list_phonic_info_ba.size()){
        max_game = (int) _list_phonic_info_ba.size();
    }
    auto count = MJDEFAULT->getIntegerForKey("key_count_guide_fishing", 0);
    _is_guide_showed = false; //!(count < 3);
    screen_size = Director::getInstance()->getVisibleSize();
    autoPlay = isAutoPlay;
    this->loadFrames();
   
}

void FishingShip::generateData(rapidjson::Value value_data){
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    for (int i = 0 ; i < value_data.Size(); i++) {
        rapidjson::Value& json_item = value_data[i].FindMember("item")->value;
        
        auto letter = json_item.FindMember("text_full")->value.GetString();
        auto pImage = math::resource::ResHelper::getImage(images, letter);
        
        PHONIC_INFO_BA phonic_info;
        phonic_info.text_full = pImage->getText("name_1");
        phonic_info.text_phonic = pImage->getTextPhonic("name_p");
        phonic_info.audio_full = pImage->getAudios("name_1").front();
        phonic_info.audio_phonic.s_audio = pImage->getAudios("name_n").front();
        phonic_info.audio_phonic.p_audio = pImage->getAudioPhonic("name_p");
        phonic_info.image_path = pImage->path;
        _list_phonic_info_ba.push_back(phonic_info);
    }
}


FishingShip * FishingShip::createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay, std::string game_type) {

    return nullptr;
}

void FishingShip::didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay) {
}

void FishingShip::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->setTouchEnabled(true);
    
    playBackgroundMusic(bg_music);
    this->runAction(Sequence::create(DelayTime::create(5.0f),CallFunc::create([=]{
    }), NULL));
    
    if(autoPlay){
        this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=]{
            this->onAutoPlay();
        }), NULL));
        
    }
}

void FishingShip::loadFrames(){
	root_layout = utils::findChild<Layout*>(this, "root_layout");

	speaker = cocos2d::ui::Button::create("games/fishingship/loa icon.png");
	speaker->setAnchorPoint(Vec2(0.5, 0.5));
	speaker->setPosition(Vec2(50, screen_size.height - 50));
	root_layout->addChild(speaker, 200);
	//speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(FishingShip::onClickedSpeaker, this));
	speaker->setTouchEnabled(false);

    root_layout = utils::findChild<Layout*>(this, "root_layout");
    auto tree_1 = utils::findChild(root_layout, "cay_1");
    GAFWrapper* tree_gaf_1 = GAFWrapper::createGAF("gaf/fishingship/cay/cay.gaf");
    tree_1->addChild(tree_gaf_1);
    tree_gaf_1->setAnchorPoint(Point(-0.2,0));
    tree_gaf_1->setScale(0.8);
    tree_gaf_1->play(true);
    
    auto tree_2 = utils::findChild(root_layout, "cay_2");
    GAFWrapper* tree_gaf_2 = GAFWrapper::createGAF("gaf/fishingship/cay/cay.gaf");
    tree_2->addChild(tree_gaf_2);
    tree_gaf_2->setAnchorPoint(Point(1,0));
    tree_gaf_2->setFlippedX(true);
    tree_gaf_2->setScale(0.5);
    tree_gaf_2->play(true);
    
    mj::helper::shuffle(_list_phonic_info_ba);

    showGame();
    
}

void FishingShip::showGame(){
    //reset
    shadows.clear();
    w_phonic.clear();
    list_index.clear();
    shadow = 0;
    fish_count = 0;
    wrong_count = 0;
    
    if(timer < 2400.0f/60 && timer != 0){
        //tang toc
        if(f_i < 3){
            f_i++;
        }else{
            f_i = 3;
        }
        
        timer = 0;
    }else{
        //giam toc
        if(f_i != 0 && count_game != 0){
            f_i--;
        }
        timer = 0;
    }
     f_speed = f_speeds[f_i];
    
    //layout
    ship_layout = Layout::create();
    root_layout->addChild(ship_layout);
    ship_layout->setAnchorPoint(Point(0.5, 0.5));
    ship_layout->setPosition(Vec2(screen_size.width + ship_layout->getContentSize().width + 400, screen_size.height*0.5));
    
    //dropzone
    GAFWrapper* box = GAFWrapper::createGAF("gaf/fishingship/drop_zone/drop_zone.gaf");
    ship_layout->addChild(box,3);
    box->setAnchorPoint(Point(0.405, 0.55));
    box->setPosition(Point(0.405, 0.55));
    box->setName("dropzone");
    box->setVisible(false);
    
    //create ship
    GAFWrapper* ship_gaf = GAFWrapper::createGAF("gaf/fishingship/boat/boat.gaf");
    ship_gaf->playSequence("idle",true);
    ship_gaf->setAnchorPoint(Point(0.3,0.4));
    ship_layout->addChild(ship_gaf);
    ship_gaf->setName("fish_boat");
    
    auto box_layout = Layout::create();
    ship_layout->addChild(box_layout);
    box_layout->setAnchorPoint(Point(0.7, 0.5));
    box_layout->setPosition(ship_layout->getContentSize()/2);
    box_layout->setFlippedX(true);
    box_layout->setContentSize(Size(800, 300));
    box_layout->setName("box_layout");
    
    
    //create list phonic
    std::string phonic = _list_phonic_info_ba[count_game].text_full;
    std::vector<std::string> phonic_list;
    for (auto s:  _list_phonic_info_ba[count_game].text_phonic) {
        phonic_list.push_back(s);
    }
    auto phonic_audio = _list_phonic_info_ba[count_game].audio_phonic.p_audio;
//    std::string audio_fpath = audio_path + phonic+"/";
    for(int i = 0; i < phonic_list.size(); i++){
        mj::PHONIC_INFO letter;
        letter.text_full = phonic_list[i];
        letter.text_phonic = phonic_list[i];
        std::string audio = phonic_audio[i];
        letter.audio_full = audio;
        letter.audio_phonic = audio;
        letter.image_path = "";
        letter.highlights = {0};
        letter.normal_color = Color3B::BLACK;
        letter.highlight_color = Color3B::RED;
        w_phonic.push_back(letter);
    }
    
    //create shadow
    auto number_shadow = (int) w_phonic.size();
    Vec2 start_pos = Vec2(((ship_layout->getContentSize().width-(number_shadow*150))/4), -30);
	if (number_shadow >= 6) start_pos.x -= 30;
    for(int i = 0; i< number_shadow; i++){
        ImageView* shadow = ImageView::create(shadow_path);
        ship_layout->addChild(shadow);
        shadow->setPosition(Vec2(start_pos.x + 120*i, start_pos.y));
        shadow->setOpacity(0);
        shadow->setScale(0.5);
        shadows.push_back(shadow);
    }
    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
        ship_layout->runAction(EaseBackOut::create(MoveTo::create(3.0f, root_layout->getContentSize() / 2)));
        }), NULL));
    AUDIO_ENGINE->play(boat_show);
    
    //create board and image
    board_layout = Layout::create();
    board_layout->setAnchorPoint(Point(1,0));
    root_layout->addChild(board_layout);
    board_layout->setBackGroundImage("games/fishingship/1.png");
    board_layout->setPosition(Vec2(root_layout->getContentSize().width*0.5, root_layout->getContentSize().height*0.7));
    board_layout->setContentSize(Size(250, 200));
    board_layout->setScale(0);
    
    _imagePhonic = ImageView::create();
    _imagePhonic->loadTexture(_list_phonic_info_ba[count_game].image_path);
    board_layout->addChild(_imagePhonic);
    _imagePhonic->setScale(150.0f / _imagePhonic->getContentSize().height);
    _imagePhonic->setPosition(Vec2(110,100));
    _imagePhonic->setOpacity(0);
    _imagePhonic->setName("_imagePhonic");
    _imagePhonic->setTouchEnabled(false);
    _imagePhonic->addClickEventListener([=](cocos2d::Ref* sender) {
        stopInActiveMode();
        stopAudioSpeaker();
        _flag = true;
        _imagePhonic->setTouchEnabled(false);
        this->runAction(Sequence::create(CallFunc::create([=] {
            img_audio_id = AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_full);
            }), DelayTime::create(1.0f), CallFunc::create([=] {
                if (_flag) {
                    AUDIO_ENGINE->playChain(_list_phonic_info_ba[count_game].audio_phonic.p_audio, [=]() {
                        _imagePhonic->setTouchEnabled(true);
                        inActiveMode();
                        });
                }
                }), NULL));
        });
    
    board_layout->runAction(Sequence::create(DelayTime::create(3.0f), ScaleTo::create(0.5f, 1), CallFunc::create([=]{
        AUDIO_ENGINE->play(fish_show);
          board_layout->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(1.0f, 1.05f),ScaleTo::create(1.0f, 1.0f))));
    }),NULL));
  
    
    _imagePhonic->runAction(Sequence::create(DelayTime::create(1.5), FadeIn::create(0.2f), NULL));
    
    for (ImageView* shadow : shadows) {
        shadow->runAction(Sequence::create(DelayTime::create(2), FadeTo::create(0.5f, 200), NULL));
    }
    
    //water splash
    water_boom = GAFWrapper::createGAF("gaf/fishingship/splash/splash.gaf");
    water_layout = utils::findChild<Layout*>(root_layout, "water");
    water_layout->setLocalZOrder(1);
    water_layout->addChild(water_boom);
    water_boom->setPositionY(water_layout->getContentSize().height);
    water_boom->setName("splash");
    

    //create fish
    w_phonic_copy = w_phonic;
    mj::helper::shuffle(w_phonic_copy);
    auto time = f_times[f_i];
    scheduleFish(time);
    
    if (_flag2) {
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(question);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
            AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_full, false, [=] {
                speaker->setTouchEnabled(true);
                _imagePhonic->setTouchEnabled(true);
                inActiveMode();
                });
            });
    }
    else
    {
        this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
            AUDIO_ENGINE->playSound(make_the_word, false, [=] {
                AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_full, false, [=] {
                    removeItemOverLay();
                    });
                speaker->setTouchEnabled(true);
                _imagePhonic->setTouchEnabled(true);
                inActiveMode();
                });
            }), NULL));
    }
    
    this->scheduleUpdate();
}

void FishingShip::update(float delay_time){
    timer += delay_time;
}

void FishingShip::onExitTransitionDidStart(){
	MJMainGBase::onExitTransitionDidStart();
}

void FishingShip::scheduleFish(float time){
    this->schedule([=](float){
        createFish();
    }, time , 1000, 2.0f, "create_fish");
}

FishingShip_Item* FishingShip::createFish(){
    
    int height = cocos2d::random(3, 4);
    Vec2 pos = Vec2(screen_size.width * 1.5, water_layout->getContentSize().height * 0.18 * height);
    
    //create text
    if(index >= (int) w_phonic.size()){
        index = 0;
        mj::helper::shuffle(w_phonic_copy);
    }
    
    auto fish_item = FishingShip_Item::createItem(w_phonic_copy[index]);
    fish_item->setAnchorPoint(Point(0.5, 0.5));
    water_layout->addChild(fish_item,2);
    fish_item->setScale(1.3);
    fish_item->setPosition(pos);
    fish_item->setDelegate(this);
    fish_item->setName("item_" + cocos2d::StringUtils::toString(fish_count));
    
    //create gaf fish
    int random = cocos2d::random(0, 2);
    std::string name;
    if (random == 0){
        name = "green_fish";
    }else if (random == 1){
        name = "orange_fish";
    }else{
        name = "pink_fish";
    }
    std::string gaf_path = "gaf/fishingship/" + name + "/" + name + ".gaf";

    auto fish = GAFWrapper::createGAF(gaf_path);
    water_layout->addChild(fish, -1);
    fish->setPosition(pos);
    if(w_phonic_copy[index].text_phonic.size() >1 ){
        fish->setScale(1.5);
    }else{
         fish->setScale(1.3);
    }
    fish->setName("gaf_" + fish_item->getName());
    fish->playSequence("swim", true);

    //create layout to save position
    auto layer_pos = Layout::create();
    water_layout->addChild(layer_pos, -2);
    layer_pos->setPosition(pos);
    layer_pos->setName("pos_" + fish_item->getName());
    
    //moving fish
    moveFish(fish, f_speed);
    moveFish(fish_item, f_speed);
    moveFish(layer_pos, f_speed);
    fish_count++;
    index++;
    
    return fish_item;
}

void FishingShip::moveFish(Node* fish, float fish_speed, bool down){
    //moving fish
    auto time = fish->getPositionX() * fish_speed;
    auto pos_y = fish->getPositionY();
    float rotate = 0;
    if(down){
        pos_y = -pos_y;
        rotate = -15;
    }
    auto movingFish = MoveTo::create(time, Vec2(0, pos_y));
    auto spawnAction = Spawn::create(RotateTo::create(time, rotate, rotate), movingFish, NULL);
    auto fishSequence = RepeatForever::create(Sequence::create( spawnAction, CallFunc::create([=]{
        removeFish(fish);
    }), NULL ));
    
    fish->runAction(fishSequence);
}

void FishingShip::removeFish(Node* fish){
        fish->stopAllActions();
        fish->removeFromParent();
}

void FishingShip::stopAudioImage() {
    _imagePhonic->setTouchEnabled(true);
    AUDIO_ENGINE->stopEffect(img_audio_id);
    for (std::string path : _list_phonic_info_ba[count_game].audio_phonic.p_audio) {
        AUDIO_ENGINE->stopSound(path);
    }
}

void FishingShip::onClickedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type){

    stopInActiveMode();
    stopAudioImage();
    stopAudioSpeaker();
    _flag = false;

    this->hideTapGuideGame();
    this->unschedule("animated_guide");
    AUDIO_ENGINE->stopEffect(img_audio_id);
    
    Layout* fish = (Layout*) sender;
    
    auto fish_p = (FishingShip_Item *)fish->getParent();
	fish_p->setHint(false);
    
    auto gaf_name = "gaf_" + fish_p->getName();
    GAFWrapper* fish_gaf = (GAFWrapper*) water_layout->getChildByName(gaf_name);
    
    
	auto fish_data = fish_p->getInfo();
	string fish_text = fish_data.text_full;
   
    auto layer_pos = water_layout->getChildByName("pos_" + fish_p->getName());
    
    int i_index = shadow;
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
//            CCLOG("began");
            utils::findChild<GAFWrapper*>(ship_layout, "dropzone")->setVisible(true);
            utils::findChild<GAFWrapper*>(ship_layout, "dropzone")->play(true);
            
            fish_p->stopAllActions();
            fish_gaf->stopAllActions();
            
            AUDIO_ENGINE->play(drag_fish);
            AUDIO_ENGINE->play(fish_data.audio_full);
            break;
        
        case cocos2d::ui::Widget::TouchEventType::MOVED:
//            CCLOG("moved");
            //move fish and text
            fish_p->setPosition(water_layout->convertToNodeSpace (((Layout*)sender)->getTouchMovePosition()));
            fish_gaf->setPosition(fish_p->getPosition());

            //water effect
            if (fish_p->getPosition().y >= water_layout->getContentSize().height && !fish_p->getSplash()) {
                fish_p->setScale(1.6);
                fish_gaf->setScale(1.6);
                fish_p->setSplash(true);
            }
            break;
            
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
//            CCLOG("log canceled");
            auto box = utils::findChild(ship_layout, "box_layout");
            auto pos = ship_layout->convertToWorldSpace(box->getBoundingBox().origin);
            auto bb = Rect(pos.x, pos.y, box->getBoundingBox().size.width - 300, box->getBoundingBox().size.height);
            if(!w_phonic.empty()&& i_index>=0 && i_index < w_phonic.size() && fish_text == w_phonic[i_index].text_full && bb.containsPoint(water_layout->convertToWorldSpace( fish_p->getPosition()) )){
//            if( fish_text == w_phonic[i_index].text_full && utils::findChild(ship_layout, "box_layout")->getBoundingBox().containsPoint(ship_layout->convertToNodeSpace(fish_p->getPosition())) ){
                //dan fish vao dropzone
                itemOverLay();
                _flag2 = false;
                fish_gaf->setLocalZOrder(0);
                fish_gaf->retain();
                fish_gaf->removeFromParentAndCleanup(false);
                ship_layout->addChild(fish_gaf);
                fish_gaf->setPosition(shadows[i_index]->getPosition());
                fish_gaf->runAction(Sequence::create(ScaleTo::create(0.1, 0.8),ScaleTo::create(0.1, 1), NULL));
                fish_gaf->playSequence("stop", true);
                fish_gaf->release();

                fish->retain();
                fish->removeFromParentAndCleanup(false);
                fish->setScale(1.6);
                ship_layout->addChild(fish);
                fish->setPosition(shadows[i_index]->getPosition());
                fish->runAction(Sequence::create(ScaleTo::create(0.1, 0.8),ScaleTo::create(0.1, 1), NULL));
                fish->setTouchEnabled(false); // cần phải disable touch ở đây, không dễ bị lỗi
                fish->release();

                shadow++;
                phaoHoa(i_index);
                utils::findChild<Text*>(fish, "text_"+ w_phonic[i_index].text_full)->disableEffect();
                
                this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                    utils::findChild<GAFWrapper*>(ship_layout, "fish_boat")->playSequence("win", false);
                    AUDIO_ENGINE->playSound(right2, false, [=] {
                        inActiveMode();
                        removeItemOverLay();
                        });
                }), DelayTime::create(0.5f), CallFunc::create([=]{
                }), DelayTime::create(0.5), CallFunc::create([=]{
                    utils::findChild<GAFWrapper*>(ship_layout, "fish_boat")->playSequence("idle", true);
                }), NULL));
            
            
                if(shadow == (int) w_phonic.size()){
                    stopInActiveMode();
                    itemOverLay();
                    speaker->setTouchEnabled(false);
                    _imagePhonic->setTouchEnabled(false);
                    AUDIO_ENGINE->play(complete);
                    this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
                        onNextStage();
                    }), NULL));
                }
                
                //hide hint
                wrong_count = 0;
                hideHint();

            }else{
                inActiveMode();
                this->runAction(Sequence::create(CallFunc::create([=]{
                    utils::findChild<GAFWrapper*>(ship_layout, "fish_boat")->playSequence("wrong", false);
                }), DelayTime::create(1.0f), CallFunc::create([=]{
                    utils::findChild<GAFWrapper*>(ship_layout, "fish_boat")->playSequence("idle", true);
                }), NULL));
                

                if(fish_p->getPosition().y > water_layout->getContentSize().height){
                    //tra ca xuong nuoc
                    float pos_y = water_layout->getContentSize().height * 0.15 * random(3,4);
                    fish_p->setSplash(false);
                    returnFish(fish_p, pos_y, true);
                    returnFish(fish_gaf, pos_y, true);
                    
                }else{
                    if(layer_pos){
                        fish_p->runAction(Sequence::create(MoveTo::create(0.2f, layer_pos->getPosition()), CallFunc::create([=]{
                            this->moveFish(fish_p, f_speed);
                        }) , NULL));
                        
                        fish_gaf->runAction(Sequence::create(MoveTo::create(0.2f, layer_pos->getPosition()), CallFunc::create([=]{
                            this->moveFish(fish_gaf, f_speed);
                        }) , NULL));
                    }else{
                        float pos_y = fish_p->getPositionY();
                        returnFish(fish_p, pos_y, true);
                        returnFish(fish_gaf, pos_y, true);
                    }
                }
                wrong_count++;
            }
            //hide dropzone
            utils::findChild<GAFWrapper*>(ship_layout, "dropzone")->setVisible(false);
            utils::findChild<GAFWrapper*>(ship_layout, "dropzone")->pauseAnimation();
            
            break;
    }
}

void FishingShip::phaoHoa(int shadow_index){
    GAFWrapper* firework = GAFWrapper::createGAF("gaf/effect/effec.gaf");
    ship_layout->addChild(firework);
    firework->setPosition(shadows[shadow_index]->getPosition());
    firework->play(false, [=](gaf::GAFObject* obj){
        firework->removeFromParent();
    });
}

void FishingShip::hideHint(){
    for(auto item: water_layout->getChildren()){
        if(utils::findChild(item, "fish") && ((FishingShip_Item*) item)->getHint()){
            ((FishingShip_Item*) item)->setHint(false);
            if(auto text = utils::findChild<Text*>(item, "text_" + w_phonic[shadow-1].text_full)){
                text->disableEffect();
            }
            item->stopActionsByFlags(0x050809);
            utils::findChild(water_layout, "gaf_" + item->getName())->setLocalZOrder(-1);
            utils::findChild(water_layout, "gaf_" + item->getName())->stopActionsByFlags(0x120589);
        }
    }
}

void FishingShip::returnFish(cocos2d::Node *fish, float pos_y, bool down){
    fish->stopAllActions();
    
    float current_pos_y = fish->getPositionY();
    float time = std::abs(current_pos_y - pos_y)/800;
    float speed = f_speed;
    float delta = 150;

    if(down){
        speed = 0.0015;
        delta = 0;
    }
    
    fish->runAction(Sequence::create(Spawn::create(EaseBackOut::create(MoveTo::create(time, Vec2(fish->getPositionX(), pos_y))), ScaleTo::create(0.2, 1.4),CallFunc::create([=]{
        if(current_pos_y >= water_layout->getContentSize().height){
            AUDIO_ENGINE->play(drop_fish);
            water_boom->setPositionX(fish->getPosition().x);
            water_boom->play(false);
            this->runAction(Sequence::create(DelayTime::create(time*0.2), CallFunc::create([=]{
                AUDIO_ENGINE->play(drop_water);
            }), NULL));
        }
       
    }), NULL), CallFunc::create([=]{
        for(auto child: water_layout->getChildren()){
            
            if(utils::findChild(child, "fish")  && child->getPosition().getDistance(fish->getPosition()) < 100 && child->getPosition() != fish->getPosition()){
                    fish->runAction(EaseBounceOut::create(MoveTo::create(0.1f, Vec2(fish->getPosition().x +((fish->getPositionX() < child->getPositionX())? -delta : delta), pos_y))));
            }
           
        }
        moveFish(fish, speed, down);
    }), NULL));
}

void FishingShip::onNextStage(){
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    
    //create transitions
    auto trans_layout = cocos2d::ui::Layout::create();
    trans_layout->setContentSize(screen_size);
    trans_layout->setTouchEnabled(true);
    this->addChild(trans_layout, 190);
    trans_layout->setBackGroundColor(Color3B::BLACK);
    trans_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    trans_layout->setOpacity(150);
    
    //image
    auto image = utils::findChild(board_layout, "_imagePhonic");
    auto pos = image->getParent()->convertToWorldSpace(image->getPosition());
    image->retain();
    image->removeFromParentAndCleanup(false);
    trans_layout->addChild(image);
    image->setPosition(pos);
    image->release();
    
    //create full text
    Text* text = Text::create();
    text->setString(_list_phonic_info_ba[count_game].text_full);
    text->setFontName(font_name);
    text->setFontSize(font_size);
    trans_layout->addChild(text);
    text->setPosition(Vec2(screen_size.width/2, -screen_size.height*0.2 ));

    for(Node* child : water_layout->getChildren()){
        child->setVisible(false);
    }
    
    this->runAction(Sequence::create(/*DelayTime::create(0.5f),*/ CallFunc::create([=]{
        board_layout->stopAllActions();
        board_layout->runAction(Sequence::create(ScaleTo::create(0.5f, 0), CallFunc::create([=]{
            board_layout->removeFromParent();
        }), NULL));
        AUDIO_ENGINE->play(sound_pop);
		float scale = image->getScale() * 2.5f;
        image->runAction(Spawn::create(MoveTo::create(0.8f, Vec2(screen_size.width/2, screen_size.height*0.6)), ScaleTo::create(1.0f,scale), NULL));
        text->runAction(EaseBackOut::create(MoveTo::create(1.2f, Vec2(screen_size.width/2, screen_size.height*0.2))));
         ship_layout->runAction(MoveTo::create(3.0f, Vec2(-screen_size.width, ship_layout->getPositionY() )));
    }), DelayTime::create(1.5f), CallFunc::create([=]{
        //synctext
        readPhonicText(text);
    }), DelayTime::create(w_phonic.size()+0.5), CallFunc::create([=]{
        //read slow text
        readSlowText(text);
    }), DelayTime::create(2.5), CallFunc::create([=]{
        //read normal text
        readNormalText(text);
    }), DelayTime::create(1.0), CallFunc::create([=]{
        image->runAction(EaseBackIn::create(MoveTo::create(1.5f, Vec2(screen_size.width/2, screen_size.height*1.5))));
        text->runAction(EaseBackIn::create(MoveTo::create(1.5f, Vec2(screen_size.width/2, -screen_size.height))));
    }), DelayTime::create(2.0f), CallFunc::create([=]{
        trans_layout->runAction(FadeOut::create(0.5f));
    }), DelayTime::create(2.0f), CallFunc::create([=]{
        trans_layout->removeFromParent();
        //image->release();
        nextGame();
    }), NULL));
    
}

void FishingShip::readPhonicText(Text* text){
	int start_index = 0;
	std::queue<int> underline_index;

	for (int i = 0; i < w_phonic.size(); ++i) {
		std::vector<int> indexes;
		auto phonic_data = w_phonic[i].text_phonic;
		for (int j = 0; j < phonic_data.size(); ++j) {
			if (phonic_data[j] == '_') {
				//underline_index
				underline_index.push(start_index + j);
			}
			else {
				if (underline_index.empty() || underline_index.front() > start_index) {
					indexes.push_back(start_index + j);
				}
				else {
					indexes.push_back(underline_index.front());
					underline_index.pop();
				}
			}
		}
		start_index += phonic_data.size();
		list_index.push_back(indexes);
	}
	for (int i = 0; i < list_index.size(); i++) {
		this->runAction(Sequence::create(DelayTime::create(1.0f *(i + 1)), CallFunc::create([=] {
			AUDIO_ENGINE->play(_list_phonic_info_ba[count_game].audio_phonic.p_audio[i]);
			for (int j = 0; j < list_index[i].size(); j++) {
				int index = list_index[i][j];
				text->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					text->getLetter(index)->setColor(Color3B(250, 54, 208));
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					text->getLetter(index)->setColor(Color3B(255, 255, 255));
				}), NULL));
			}
		}), NULL));
	}
}

void FishingShip::readSlowText(cocos2d::ui::Text *text){
    AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_phonic.s_audio);
    text->runAction(EaseBounceOut::create(Sequence::create(ScaleTo::create(1, 1.2), ScaleTo::create(1, 1), NULL)));
}

void FishingShip::readNormalText(cocos2d::ui::Text *text){
    text->setColor(Color3B(250, 54, 208));
    AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_full);
    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
        text->setColor(Color3B(255, 255, 255));
        }), NULL));
    text->runAction(EaseBounceOut::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1), NULL)));
}

void FishingShip::onFinishGame(){
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onFinishGame();
}

void FishingShip::nextGame(){
    count_game++;
    if(count_game == max_game){
        this->onFinishGame();
    }else {
        showGame();
        if(!autoPlay){
        }
    }
}

void FishingShip::showTapGuideGame(std::string gaf_guide_path){
    auto guide_layout_check = this->getChildByName("guide_layout");
    if (guide_layout_check) return;
    auto guide_layout = cocos2d::ui::Layout::create();
        if(utils::findChild(water_layout, "text_"+ w_phonic[shadow].text_full)){
            auto item = utils::findChild(water_layout, "text_"+ w_phonic[shadow].text_full)->getParent()->getParent();
            CCLOG("item pos: %f, %f", item->getPositionX(), item->getPositionY());
            
            auto hand = GAFWrapper::createGAF(gaf_guide_path);
            auto it_pos = item->getPosition().y;
        
            guide_layout->setContentSize(item->getContentSize());
            item->addChild(guide_layout,1900);
            guide_layout->setPosition(Vec2(0,0));
            guide_layout->setName("guide_layout");
            
            guide_layout->addChild(hand, 1900000, "guide_gaf");
            hand->setScale(0.5);
            hand->setPosition(Vec2(guide_layout->getContentSize().width*0.3, guide_layout->getContentSize().height*0.3));
        
            //guide for item
            ((Layout*)item)->setEnabled(false);
            ((Layout*)item)->setTouchEnabled(false);
            item->stopAllActions();
            utils::findChild(water_layout, "gaf_" + item->getName())->stopAllActions();
            item->runAction(Sequence::create(MoveTo::create(0.8f, water_layout->convertToNodeSpace(shadows[shadow]->getParent()->getPosition())),DelayTime::create(0.3f), CallFunc::create([=]{
                hand->removeFromParent();
                ((Layout*)item)->setEnabled(true);
                
                this->returnFish(item, it_pos, true);
            }) ,NULL));
            
            //guide for gaf
            utils::findChild(water_layout, "gaf_" + item->getName())->runAction(Sequence::create(MoveTo::create(0.8f, water_layout->convertToNodeSpace(shadows[shadow]->getParent()->getPosition())),DelayTime::create(0.3f), CallFunc::create([=]{
                this->returnFish(utils::findChild(water_layout, "gaf_" + item->getName()), it_pos, true);
            }) ,NULL));
        }
}

void FishingShip::inActiveMode(){
    this->schedule([=](float){
        AUDIO_ENGINE->playSound(make_the_word, false, [=] {
            AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_full);
            });
        if (_flag2) {
            this->showTapGuideGame();
        }
    }, 13, 100, 10, "animated_guide");
}

void FishingShip::stopInActiveMode() {
    this->unschedule("animated_guide");
    hideTapGuideGame();
    AUDIO_ENGINE->stopSound(make_the_word);
    AUDIO_ENGINE->stopSound(_list_phonic_info_ba[count_game].audio_full);
}

void FishingShip::onClickedSpeaker(cocos2d::Ref* sender) {
    stopInActiveMode();
    stopAudioImage();
	speaker->setTouchEnabled(false);
    _flag = false;
    AUDIO_ENGINE->stopSound(_list_phonic_info_ba[count_game].audio_full);
    AUDIO_ENGINE->playSound(make_the_word, false, [=] {
        AUDIO_ENGINE->playSound(_list_phonic_info_ba[count_game].audio_full);
        speaker->setTouchEnabled(true);
        inActiveMode();
        });
}

void FishingShip::stopAudioSpeaker() {
    AUDIO_ENGINE->stopSound(make_the_word);
    AUDIO_ENGINE->stopSound(_list_phonic_info_ba[count_game].audio_full);
    speaker->setTouchEnabled(true);
}

#define auto_testing_delay 1.5f
void FishingShip::onAutoPlay()
{
    this->scheduleOnce([=](float dt) {
        this->autoPlayRun(1.5f);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void FishingShip::autoPlayRun(float dt)
{
    this->scheduleOnce([=](float dt) {
        this->onAutoPlay();
    }, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}

void FishingShip::itemOverLay() {
    auto screen_size = Director::getInstance()->getVisibleSize();
    Layout* overlay = Layout::create();
    overlay->setContentSize(Size(screen_size.width, screen_size.height));
    overlay->setAnchorPoint(Vec2(0.5, 0.5));
    overlay->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
    overlay->setName("overlay");
    this->addChild(overlay, INT_MAX - 1);
    overlay->setTouchEnabled(true);
    overlay->addClickEventListener([=](cocos2d::Ref* sender) {
        CCLOG("ok");
        });
}

void FishingShip::removeItemOverLay() {
    if (cocos2d::utils::findChild(this, "overlay")) {
        this->removeChildByName("overlay");
    }
}