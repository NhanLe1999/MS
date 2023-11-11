//
//  GearsForCar.h
//


#include "GearsForCar.h"
#include "JsonDump.h"
#include "MJHelper.h"
#define num_egg 3
#define game_name_image "games/GearsForCar/ready_set_go.png"
#define game_name_sound "sounds/GearsForCar/ready_set_go.mp3"

using namespace cocos2d;
GearsForCar * GearsForCar::createGame(std::string json_file) {
    
    GearsForCar * obj = new GearsForCar();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(json_file);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void GearsForCar::didLoadFromCSB(std::string json_file){
    auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile(json_file);
        buffer_string = JsonDump::getInstance()->MapToString(value_map);
    }
    
    value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
    if (value_data.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
        return;
    }
    // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    value_data.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    generateData(value_data.GetObject(), images);
}

void GearsForCar::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images){
    for(int i=0; i< value_data["gear"].Size(); i++){
        int number_gear = value_data["gear"][i].GetInt();
        if(number_gear > 3)
        {
            game_info.gears_per_level.push_back(number_gear / 2);
            number_gear -= number_gear / 2;
        }
        game_info.gears_per_level.push_back(number_gear);
    }
    
	if (value_data.HasMember("question")) {
		is_show_question = value_data["question"].GetBool();
	}
	else {
		is_show_question = false;
	}
	if (value_data.HasMember("showTextItem")) {
		is_show_text_item = value_data["showTextItem"].GetBool();
	}
	else {
		is_show_text_item = false;
	}
    auto data = value_data["data"].GetArray();
    for (int i=0; i< data.Size(); i++){
        GEARS_INFO gi;
        gi.type = data[i]["kind"].GetString();
        
        auto game = data[i]["game"].GetArray();
        for (int j = 0; j < game.Size(); j++) {
            auto letter = game[j].FindMember("full_text")->value.GetString();
            auto pImage = math::resource::ResHelper::getImage(images, letter);
            
			GearsForCar_Item::GEAR_ITEM_WITH_TEXT g_item;
			g_item.isTrueAns = false;
			if(game[j].HasMember("is_correct")) 
				g_item.isTrueAns = game[j].FindMember("is_correct")->value.GetBool();
            g_item.audio_path = pImage->getAudios("name_1").front();
            g_item.image_path = pImage->path;
			g_item.text = pImage->getText("name_1");
            gi.items.push_back(g_item);
            
            if(g_item.isTrueAns){
                gi.word_audio = pImage->getAudios("name_n").front();
				gi.word_text = pImage->getText("name_n");
            }
        }
        game_info.gears_info.push_back(gi);
    }
    
    mj::helper::shuffle(game_info.gears_info);
    for(int i =0; i< game_info.gears_info.size(); i++){
        mj::helper::shuffle(game_info.gears_info[i].items);
    }
}

void GearsForCar::onEnter(){
    MJMainGBase::onEnter();
    this->setContentSize(screen_size);
    this->loadFrames();
}

void GearsForCar::loadFrames(){
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    
    auto root_layout = cocos2d::ui::Layout::create();
    root_layout->setContentSize(screen_size);
    root_layout->setTouchEnabled(true);
    this->addChild(root_layout);
    
    touchLayout = cocos2d::ui::Layout::create();
    touchLayout->setContentSize(screen_size);
    touchLayout->setTouchEnabled(true);
    this->addChild(touchLayout,1000);
    
    auto background = cocos2d::Sprite::create("games/GearsForCar/bg.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(screen_size/2);
    if(background->getContentSize().width<screen_size.width){
        background->setScale(screen_size.width/background->getContentSize().width);
    }
    this->addChild(background);
    
    item_layout = cocos2d::ui::Layout::create();
    item_layout->setTouchEnabled(true);
    item_layout->setPositionX(screen_size.width/2);
    item_layout->setPositionY(screen_size.height - 150);
    item_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(item_layout,100);
    
    bg_Gear = ImageView::create("games/GearsForCar/bg_slot.png");
    this->addChild(bg_Gear,50);
    bg_Gear->setPositionX(screen_size.width/2);
    bg_Gear->setPositionY(screen_size.height/2 - 150);
    bg_Gear->setVisible(false);
    for (int i=0; i< 3; i++){
        auto gear = GearsForCar_Gear::createItem();
        bg_Gear->addChild(gear);
        gear->setPositionY(bg_Gear->getContentSize().height/2);
        gear->setPositionX((i-1)*bg_Gear->getContentSize().width/3.5f + bg_Gear->getContentSize().width/2);
        gears.push_back(gear);
        gear->setCallBack([=](){
        });
    }
    
    carLayout = cocos2d::ui::Layout::create();
    carLayout->setContentSize(bg_Gear->getContentSize()*2);
    this->addChild(carLayout,51);
    carLayout->setPosition(bg_Gear->getPosition());
    carLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    car = GAFWrapper::createGAF("gaf/GearsForCar/car/car.gaf");
    this->addChild(car,10);
    car->playSequence("idle", true);
    car->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    car->setPositionX(screen_size.width/2-700);
    car->setPositionY(screen_size.height/2);
    car->setVisible(false);
    
    btn_Loa = cocos2d::ui::Button::create("games/GearsForCar/loa.png");
    this->addChild(btn_Loa, 1000);
    btn_Loa->setPosition(Vec2(40, screen_size.height - 40));
    btn_Loa->addClickEventListener([=](Ref* sender){
        stopGuideGame();
        btn_Loa->setTouchEnabled(false);
        AUDIO_ENGINE->stopSound(getIntroPath(game_info.gears_info[0].type));
        AUDIO_ENGINE->playSound(getIntroPath(game_info.gears_info[0].type), false, [=] {
            AUDIO_ENGINE->playSound(game_info.gears_info[0].word_audio);
            btn_Loa->setTouchEnabled(true);
            showGuideGame();
        });
    });
    btn_Loa->setVisible(false);
    
    if (FileUtils::getInstance()->isFileExist(game_name_image)) {
        _gameNameImage = cocos2d::Sprite::create(game_name_image);
        if (_gameNameImage)
        {
            _gameNameImage->setPosition(screen_size / 2);
            this->addChild(_gameNameImage);
            _gameNameImage->setOpacity(0);
            
            this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                _gameNameImage->setOpacity(255);
            }), DelayTime::create(0.25f), CallFunc::create([=] {
                AUDIO_ENGINE->playSound(game_name_sound, false, [=] {
                    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
                        if (_gameNameImage) {
                            _gameNameImage->removeFromParent();
                        }
                    }), NULL));
                });
            }), DelayTime::create(3.5), CallFunc::create([=] {
                startGame();
            }), NULL));
        }
    }
    else
    {
        startGame();
    }
}

void GearsForCar::showIntroGame(){
    
}

void GearsForCar::onExitTransitionDidStar(){
    MJMainGBase::onExitTransitionDidStart();
}

std::string GearsForCar::getIntroPath(std::string type){
    if(type.compare("start")==0){
        return "sounds/GearsForCar/intro_start.mp3";
    }
    else if (type.compare("contain")==0){
        return "sounds/GearsForCar/intro_contain.mp3";
    }
    else if(type.compare("end")==0){
        return "sounds/GearsForCar/intro_end.mp3";
    }
    return "";
}
std::string GearsForCar::getTextQuestion(std::string type) {
	if (type.compare("start") == 0) {
		return "Starts with ";
	}
	else if (type.compare("contain") == 0) {
		return "Contains ";
	}
	else if (type.compare("end") == 0) {
		return "Ends with ";
	}
	return "";
}

void GearsForCar::playCar(CAR_STATE state){
    switch (state) {
        case CS_IDLE:{
            car->playSequence("idle", true);
            break;
        }
        case CS_DROP:{
            car->playSequence("drop", false, [=](gaf::GAFObject *obj, const std::string seq_name){
                car->playSequence("idle", true);
                //showItem();
            });
            break;
        }
        case CS_RUN_IN:{
            car->setPositionX(-700);
            car->setVisible(true);
            car->playSequence("idle", true);
            Vec2 pos = car->getPosition();
            pos.x = screen_size.width/2-700;
            AUDIO_ENGINE->play("sounds/GearsForCar/car_begin.mp3");
            car->runAction(Sequence::create(EaseIn::create(MoveTo::create(0.5f, pos),1.0f),
                                            CallFunc::create([=](){
                showItem();
                bg_Gear->setVisible(true);
                car->playSequence("idle", true);
            }), NULL));
            break;
        }
        case CS_RUN_OUT:{
            car->playSequence("end", false, [=](gaf::GAFObject *obj, const std::string seq_name){
                //showItem();
                cur_level++;
                if(cur_level>=game_info.gears_per_level.size()){
                    AUDIO_ENGINE->stopAllAudio();
                    onFinishGame();
                }
                else{
                    startLevel();
                }
            });
        }
        default:
            break;
    }
}

void GearsForCar::startGame(){
    touchLayout->setTouchEnabled(true);
    cur_level=0;
    curZ=0;
	this->playBackgroundMusic("sounds/GearsForCar/music_bg.mp3");
	startLevel();
}

void GearsForCar::startLevel(){
	
    playCar(GearsForCar::CAR_STATE::CS_RUN_IN);
    //showItem();
    for(int i = 0; i< gears.size(); i++){
        if (i<game_info.gears_per_level[cur_level]){
            gears.at(gears.size()-1-i)->setSlot(true);
        }
        else{
            gears.at(gears.size()-1-i)->setSlot(false);
        }
    }
}

void GearsForCar::showItem(){
	// show text question
	if (is_show_question) {
		this->removeChildByName("text_question");
		string str_question = getTextQuestion(game_info.gears_info[0].type) + game_info.gears_info[0].word_text + '!';
		Text* text_question = Text::create(str_question, font_name, 60);
		text_question->setTextColor(Color4B::WHITE);
		text_question->enableOutline(Color4B::BLACK, 5);
		text_question->setPosition(Vec2(screen_size.width / 2, screen_size.height*0.04));
		text_question->setName("text_question");
		this->addChild(text_question);
	}
	
	////
    cocos2d::Size totalSize = cocos2d::Size::ZERO;
    item_layout->removeAllChildren();
    btn_Loa->setVisible(true);
    btn_Loa->setTouchEnabled(false);
    items.clear();
    position_list.clear();
    
    for (int i=0; i< game_info.gears_info[0].items.size(); i++){
        auto item = GearsForCar_Item::createItem(game_info.gears_info[0].items[i]);
        items.push_back(item);
        item->setTouchEnabled(true);
        item->addTouchEventListener([=](Ref* aRef, Widget::TouchEventType evt){
            stopGuideGame();
            btn_Loa->setTouchEnabled(true);
            if (evt == Widget::TouchEventType::BEGAN){
                //phat tieng
                itemOverLay();
                item->itemPos = position_list[item->getTag()];
                item->setLocalZOrder(++curZ);
				item->onClicked();
            }
            else if (evt == Widget::TouchEventType::MOVED){
                if (item->getTouchBeganPosition().distance(item->getTouchMovePosition())>7){
                    item->itemOnMove = true;
                }
                if (item->itemOnMove){
                    item->setPosition(item_layout->convertToNodeSpace(item->getTouchMovePosition()));
                }
            }
            else if (evt == Widget::TouchEventType::ENDED || evt == Widget::TouchEventType::CANCELED){
                touchLayout->setTouchEnabled(true);
                if (item->itemOnMove){
                    if(carLayout->getBoundingBox().containsPoint(item->getTouchEndPosition())
                       && item->_item_info.isTrueAns){
                        // tra loi dung
                        _flag = false;
                        right_item = nullptr; // lock show guide
                        guide = 1;
                        btn_Loa->setTouchEnabled(false);
                        item->setVisible(false);
                        
                        bool isComplete =false;
                        for(int n=0; n < game_info.gears_per_level[cur_level]; n++){
                            if (gears.at(n)->isEmpty){
                                gears.at(n)->setCorrect();
                                if (n== game_info.gears_per_level[cur_level] - 1) {
                                    isComplete=true;
                                }
                                break;
                            }
                        }
                        std::string audio_picture = game_info.gears_info[0].items[i].audio_path;
                        game_info.gears_info.erase(game_info.gears_info.begin());
                        playCar(CAR_STATE::CS_DROP);
                        this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                            AUDIO_ENGINE->stopSound(audio_picture);
                            AUDIO_ENGINE->playSound(audio_picture, false, [=] {
                                if (isComplete) {
                                    bg_Gear->setVisible(false);
                                    playCar(CAR_STATE::CS_RUN_OUT);
                                    AUDIO_ENGINE->play("sounds/GearsForCar/win.mp3");
                                }
                                else {
                                    showItem();
                                }
                                });
                            }), NULL));
                        
                        for(int n=0; n< items.size(); n++){
                            items.at(n)->runAction(FadeOut::create(0.2f));
                            items.at(n)->setTouchEnabled(false);
                        }
                    }
                    else{
                        showGuideGame();
                        this->runAction(Sequence::create(DelayTime::create(0.75f), CallFunc::create([=] {
                            removeItemOverLay();
                            }), NULL));
                        AUDIO_ENGINE->play("sounds/GearsForCar/wrong.mp3");
                        item->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.2f, 1.0f), EaseBackOut::create(MoveTo::create(0.2f, item->itemPos))),
                            CallFunc::create([=](){
                            touchLayout->setTouchEnabled(false);
                        }), NULL));
                    }
                }
                else{
					item->onFinishClick();
                    showGuideGame();
                    this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
                        removeItemOverLay();
                        }), NULL));
                }
				touchLayout->setTouchEnabled(false);
                item->itemOnMove=false;
            }
        });
        item->setClickedCallback([=](){
            touchLayout->setTouchEnabled(false);
			CCLOG("CLICKED");
            if(item->itemOnMove) return ;
			if (is_show_text_item) {
				item->showText();
			}
            
            if(guide == 0 && cur_level==0) {
                for(int n=0; n< items.size(); n++){
                    if (items.at(i)->_item_info.isTrueAns){
                        right_item = items.at(i);
                        break;
                    }
                }
            }
        });
        item_layout->addChild(item);
        item->setStart();
        item->runAction(Sequence::create(DelayTime::create(0.3f*i)
                                         ,CallFunc::create([=](){
            item->showItem();
        }), NULL));
        
        
        totalSize.width += item->getContentSize().width;
        totalSize.height = item->getContentSize().height;
    }
    auto margin = (screen_size.width - totalSize.width)/(items.size()+1);
    if(margin > 100) margin = 100;
    
    totalSize.width += (item_layout->getChildren().size() - 1) * margin;
    
    item_layout->setContentSize(totalSize);
    
    auto start_x = items[0]->getContentSize().width/2;
    
    int i = 0;
    for(auto item: item_layout->getChildren()){
        item->setPosition(Vec2(start_x + (item->getContentSize().width + margin)* i, item_layout->getContentSize().height/2));
        position_list.push_back(item->getPosition());
        item->setTag(i);
        i++;
    }
    
    Size finalSize;
    finalSize.width = position_list.back().x + start_x + margin;
    finalSize.height = item_layout->getContentSize().height;
    item_layout->setContentSize(finalSize);
    
	right_item = nullptr;

	auto word_audio_path = game_info.gears_info[0].word_audio;

    auto intro_id = cocos2d::experimental::AudioEngine::play2d(getIntroPath(game_info.gears_info[0].type));
    cocos2d::experimental::AudioEngine::setFinishCallback(intro_id, [=](int au_id, std::string path){
        auto word_id = cocos2d::experimental::AudioEngine::play2d(word_audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(word_id, [=](int au_id, std::string path){
            btn_Loa->setTouchEnabled(true);
            touchLayout->setTouchEnabled(false);
            
            for(int i=0; i< items.size(); i++){
                if (items.at(i)->_item_info.isTrueAns){
                    right_item = items.at(i);
                    stopGuideGame();
                    showGuideGame();
                    if(guide == 0){
                        break;
                    }
                    else{
                        removeItemOverLay();
                        break;
                        
                    }
                }
            }
        });
    });
}

void GearsForCar::showGuideGame() {
    this->schedule([=](float) {
        if (_flag) {
            auto hand = GAFWrapper::createGAF("gaf/guidegame/hand_tap/hand_tap.gaf");
            hand->setName("hand_guide_game");
            this->addChild(hand, 1900000000);
            hand->setPosition(item_layout->convertToWorldSpace(right_item->getPosition()));
            hand->play(false, [=](gaf::GAFObject* obj) {
                hand->runAction(Sequence::create(MoveTo::create(1.5f, bg_Gear->getPosition() - Vec2(100, 25)),
                    CallFunc::create([=]() {
                        hand->play(false, [=](gaf::GAFObject* obj) {
                            hand->removeFromParent();
                            });
                        }), NULL));
                });
        }

        AUDIO_ENGINE->stopSound(getIntroPath(game_info.gears_info[0].type));
        AUDIO_ENGINE->playSound(getIntroPath(game_info.gears_info[0].type), false, [=] {
            AUDIO_ENGINE->playSound(game_info.gears_info[0].word_audio);
            });
        }, 15, 100, 11, "animated_guide");
}

void GearsForCar::stopGuideGame()
{
    if (utils::findChild(this, "hand_guide_game")) {
        this->removeChildByName("hand_guide_game");
    }
    this->unschedule("animated_guide");
    AUDIO_ENGINE->stopSound(getIntroPath(game_info.gears_info[0].type));
    AUDIO_ENGINE->stopSound(game_info.gears_info[0].word_audio);
}

void GearsForCar::itemOverLay() {
    auto screen_size = Director::getInstance()->getVisibleSize();
    Layout* overlay = Layout::create();
    overlay->setContentSize(Size(screen_size.width, screen_size.height));
    overlay->setAnchorPoint(Vec2(0.5, 0.5));
    overlay->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
    overlay->setName("overlay");
    if (!cocos2d::utils::findChild(this, "overlay")) {
        this->addChild(overlay, INT_MAX - 1);
        overlay->setTouchEnabled(true);
    }
}

void GearsForCar::removeItemOverLay() {
    if (cocos2d::utils::findChild(this, "overlay")) {
        this->removeChildByName("overlay");
    }
}

void GearsForCar::onExit()
{
    this->unschedule("animated_guide");
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllAudio();
    MJMainGBase::onExit();
}
