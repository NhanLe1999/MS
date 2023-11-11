
#include "GameJumpingMonkey.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
//#include "listener.h"
#include "MJHelper.h"
#define SOUND_CORRECT_ANSWER "sounds/correct.mp3"

INITIALIZE_READER(MJGameJumpingMonkey);

USING_NS_CC;
std::once_flag jumping_monkey_reader;
#define CSB_NAME "csb/game/jumping_monkey/GameJumpingMonkey.csb"

MJGameJumpingMonkey * MJGameJumpingMonkey::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(jumping_monkey_reader, [] {
        REGISTER_CSB_READER(MJGameJumpingMonkey);
        REGISTER_CSB_READER(MJGameJumpingMonkey_Item);
        REGISTER_CSB_READER(MJGameJumpingMonkey_Food);
    });
    auto p = static_cast<MJGameJumpingMonkey *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameJumpingMonkey::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_waterspray.plist");
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
    
	//generateData(value_data.GetObject());
	readConfig(value_data.GetObject(),images);
    
    auto bg = cocos2d::utils::findChild(this, "bg");
    if ((bg->getContentSize().width-150) < Director::getInstance()->getWinSize().width){
        bg->setScale(Director::getInstance()->getWinSize().width*1.0/(bg->getContentSize().width-150));
        bg->setPositionX(bg->getPositionX()-50);
    }
    if (layout_content = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "layout_content")) {
        layout_content->setSwallowTouches(false);
        listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        listener->onTouchBegan = [](Touch* touch, cocos2d::Event* event) {
            return true;
        };

        listener->onTouchMoved = [](Touch* touch, cocos2d::Event* event) {
        };

        listener->onTouchEnded = [=](Touch* touch, cocos2d::Event* event) {
            unschedule("inActiveMode");
            this->inActiveMode();
        };
        listener->onTouchCancelled = [=](cocos2d::Touch*, cocos2d::Event*) {
        };
    }
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layout_content);
}

void MJGameJumpingMonkey::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

    auto content_info = value_data["content_info"].GetObject();
    auto list_object = content_info["list_items"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameJumpingMonkey_Item::ItemInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        object_info.img_path = object_value["img_path"].GetString();
        
        game_dataset.list_objects.push_back(object_info);
    }
    
    auto list_distracting = content_info["list_distracting"].GetArray();
    for (int i = 0; i < (int) list_distracting.Size(); i++) {
        auto distracting = list_distracting[i].GetObject();
        MJGameJumpingMonkey_Item::ItemInfo distracting_info;
        distracting_info.text = distracting["text"].GetString();
        distracting_info.img_path = distracting["img_path"].GetString();
        game_dataset.list_distracting.push_back(distracting_info);
    }

//    mj::helper::shuffle(game_dataset.list_objects);
    
}

void MJGameJumpingMonkey::onEnter(){
    ms::BaseActivity::onEnter();
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/jumping_monkey/jumpingmonkey_bg.mp3");
    }
//    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
//    touch_listener->onTouchBegan = CC_CALLBACK_2(MJGameJumpingMonkey::onTouchBegan, this);
//    touch_listener->onTouchEnded = CC_CALLBACK_2(MJGameJumpingMonkey::onTouchEnded, this);
//    touch_listener->onTouchMoved = CC_CALLBACK_2(MJGameJumpingMonkey::onTouchMoved, this);
//    touch_listener->onTouchCancelled = CC_CALLBACK_2(MJGameJumpingMonkey::onTouchEnded, this);
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    loadFrames();
	auto imgGameName = cocos2d::ui::ImageView::create("games/jumping_monkey/logo.png");
	this->addChild(imgGameName, 10000);
	imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
	
	auto audio_path = std::string("sounds/jumping_monkey/gameName.mp3");
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
        AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_huhet.mp3");
		imgGameName->runAction(Sequence::create(FadeOut::create(0.5f)
			, CallFunc::create([=](){
			onStartGame();
		}), RemoveSelf::create(true), nullptr));
       // onStartGame();
    }), NULL));
	});
    
}
void MJGameJumpingMonkey::loadFrames() {
    setInputConfigGame();
    layout_content = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "layout_content");
    monkey = GAFWrapper::createGAF("gaf/jumping_monkey/jumping_monkey/jumping_monkey.gaf");
    layout_content->addChild(monkey, 2);
    monkey->setPosition(Vec2(layout_content->getContentSize().width*0.85, layout_content->getContentSize().height*0.35));
    monkey->playSequence("idle", true);

    _speaker = mj::helper::addSpeaker(this);
    _speaker->addClickEventListener(CC_CALLBACK_1(MJGameJumpingMonkey::onSpeakerClicked, this));

    board_object = cocos2d::ui::ImageView::create("games/jumping_monkey/jumpingmonkey_board.png");
    this->addChild(board_object, 10);
    board_object->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height*1.625));
    
}

void MJGameJumpingMonkey::setInputConfigGame(){
    list_food_true = {1, 4, 7, 10};
    mj::helper::shuffle(list_food_true);
    auto size_objects = (int) game_dataset.list_objects.size();
    max_turn_jumpingmonkey = min(size_objects, 4);
    max_item_jumpingmonkey = 4;
//    mj::helper::shuffle(game_dataset.list_objects);
//    game_dataset.list_objects.erase(game_dataset.list_objects.begin() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
}

void MJGameJumpingMonkey::onStartGame(){
    _questionType = _typeOfQuestion::Food;
    list_food.clear();
    std::vector<int> list_food_type = {2, 3, 5, 6, 8, 9};
    speaker_audio_path = game_dataset.list_objects[object_index].audio_path;
    mj::helper::shuffle(list_food_type);
    list_food_type.erase(list_food_type.begin()+2, list_food_type.begin() + (int)list_food_type.size()-1);
    list_food_type.insert(list_food_type.begin(), list_food_true[object_index]);
    _foodIsShowing = true; 
    std::vector<int> list_pos = {0, 1, 2, 3};
    mj::helper::shuffle(list_pos);
    AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_show1.mp3");
    for (int i = 0; i < 4; i ++) {
        auto food = MJGameJumpingMonkey_Food::createGame(list_food_type[i]);
        layout_content->addChild(food, 1);
        food->setPosition(Vec2(layout_content->getContentSize().width *0.1+ 200*list_pos[i], layout_content->getContentSize().height *1.1));
        food->runAction(Sequence::create(DelayTime::create(0.1f*i), EaseBackOut::create(MoveTo::create(0.5f, Vec2(layout_content->getContentSize().width *0.1+ 200*list_pos[i], layout_content->getContentSize().height *0.62))), CallFuncN::create([=](cocos2d::Node* sender){
            if (list_food_type[i] == 1 || list_food_type[i] == 4 || list_food_type[i] == 7 || list_food_type[i] == 10) {
                _foodCorrectPossition = list_food_type[i];
                _foodCorrect = food;
            }
            if (i == 3) {
                if (!game_play_enable && object_index > 0) {
                    this->pauseGame();
                    return;
				}
                if (object_index == 0) {
                    audio_mc_id = AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_mc1.mp3");
                    _isSpeaking = true;
                    cocos2d::experimental::AudioEngine::setFinishCallback(audio_mc_id, [=](int au_id, std::string path) {
                        _isSpeaking = false;
                        this->inActiveMode();
                    });
                }
                else
                {
                    _isSpeaking = false;
                    this->inActiveMode();
                }
				//else
					//audio_mc_id = AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_mc3.mp3");
            }
        }), NULL));
    
        food->setDelegate(this);
        list_food.push_back(food);
    }
    list_food[0]->setLocalZOrder(3);
    
    
    board_object->removeAllChildren();
    auto img_object = cocos2d::ui::ImageView::create(game_dataset.list_objects[object_index].img_path);
    board_object->addChild(img_object);
    img_object->setPosition(Vec2(board_object->getContentSize().width/2, 322));
	if (img_object->getContentSize().height>300){
		img_object->setScale(300 / img_object->getContentSize().height);
	}
    
    auto text_object = cocos2d::ui::Text::create(game_dataset.list_objects[object_index].text, "fonts/monkeystories_ultra.ttf", 50);
    board_object->addChild(text_object);
    text_object->setPosition(Vec2(board_object->getContentSize().width/2, 120));
    text_object->setTextColor(Color4B::BLACK);
    text_object->setName("text_object");
    doAutoPlay();
}

void MJGameJumpingMonkey::onFinishGame(){
	monkey->setVisible(false);
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf", true);
	}), NULL));
}


void MJGameJumpingMonkey::onClickFood(cocos2d::Ref *sender, bool is_answer){
    
    _isSpeaking = false;

    AUDIO_ENGINE->stopEffect(audio_mc_id2);
    unschedule("inActiveMode");

    if (is_answer) {
        AUDIO_ENGINE->stopAllEffects();
        AUDIO_ENGINE->playEffect(SOUND_CORRECT_ANSWER);
        for (int i = 0; i < (int)list_food.size(); i++) {
            list_food[i]->isRunAni = true;
        }
        monkey->playSequence("walk", true);
        auto duration = 4.0f/1024;
        auto pos_goal = Vec2(list_food[0]->getPositionX()+80, monkey->getPositionY());
        auto audio_walk = AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_walk.mp3");
        duration = duration*monkey->getPosition().distance(pos_goal);
        monkey->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=]{
            AUDIO_ENGINE->stopEffect(audio_walk);
            monkey->playSequence("jump", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
                AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_pull.mp3");
                layout_content->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 768)), CallFunc::create([=]{
                    monkey->setPosition(Vec2(layout_content->getContentSize().width*0.85, layout_content->getContentSize().height));
                    showBoardObject();
                    
                }), NULL));
            });
        }), DelayTime::create(0.2f), CallFunc::create([=]{
            AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_jump.mp3");
        }), NULL));
    }else {
        if (is_monkey_run_wrong) return;
//        is_monkey_run_wrong = true;
		//AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_wrong.mp3");
		AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_mc4.mp3");
        monkey->playSequence("wrong", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            is_monkey_run_wrong = false;
            monkey->playSequence("idle", true);
        });
    }
}
void MJGameJumpingMonkey::showBoardObject(){
    
    _foodIsShowing = false;
    _questionType = _typeOfQuestion::Food;
    AUDIO_ENGINE->stopEffect(audio_mc_id2);
    unschedule("inActiveMode");

    for (int i = 0; i < (int) list_food.size(); i++) {
        list_food[i]->setVisible(false);
    }
    if (!board_object) return;
    AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_show1.mp3");
    auto width_value = 0.5;
    board_object->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Vec2(this->getContentSize().width*width_value, this->getContentSize().height*0.625))), DelayTime::create(1.0f), CallFunc::create([=]{
        auto text = board_object->getChildByName<cocos2d::ui::Text *>("text_object");
        text->setTextColor(Color4B::RED);
        text->runAction(ScaleTo::create(0.2f, 1.3f));
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            text->setTextColor(Color4B::BLACK);
            text->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f), DelayTime::create(1.5f), CallFunc::create([=]{
                AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_show2.mp3");
                board_object->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.5f, Vec2(this->getContentSize().width*width_value, this->getContentSize().height*1.625))),CallFunc::create([=]{
                }), DelayTime::create(0.5f), CallFunc::create([=]{
                    monkeyFallGround();
                }), NULL));
            }), NULL));
        });
    }),NULL));
}

void MJGameJumpingMonkey::monkeyFallGround(){
    layout_content->setPosition(this->getContentSize()/2);
    monkey->runAction(Sequence::create(MoveTo::create(0.75f, Vec2(layout_content->getContentSize().width*0.85, layout_content->getContentSize().height*0.35)), CallFunc::create([=]{
        monkey->playSequence("fallground", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            monkey->playSequence("idle", true);
            this->showItemAnswer(MJGameJumpingMonkey_Item::IMG_TYPE);
        });
    }), NULL));
    monkey->playSequence("fall", true);
    AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_fall.mp3");
    
}

void MJGameJumpingMonkey::showItemAnswer(int type){
    //type ==1 la text , 2 la image
    _questionType = (type == MJGameJumpingMonkey_Item::TEXT_TYPE ? _typeOfQuestion::Text : _typeOfQuestion::Image);
    AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_show1.mp3");

    std::vector<MJGameJumpingMonkey_Item::ItemInfo> list_answer;
    auto list_distracting = game_dataset.list_distracting;
    for (int i = 0; i < (int) game_dataset.list_objects.size(); i++) {
        if (i == object_index) continue;
        list_distracting.push_back(game_dataset.list_objects[i]);
    }
    mj::helper::shuffle(list_distracting);
    
    for (int i = 0; i < max_item_jumpingmonkey-1; i++) {
        list_answer.push_back(list_distracting[i]);
        CCLOG("i = %d | %s", i, list_distracting[i].text.c_str());
    }
    list_answer.push_back(game_dataset.list_objects[object_index]);
    mj::helper::shuffle(list_answer);
    
    for (int i = 0; i < list_answer.size(); i++) {
        CCLOG("i = %d | %s", i, list_answer[i].text.c_str());
    }

    for (int i = 0; i < max_item_jumpingmonkey; i++) {
        
        auto item = MJGameJumpingMonkey_Item::createGame(list_answer[i], type);
        if (item->_item_info.text == game_dataset.list_objects[object_index].text) {
            _itemCorrect = item;
        }
        auto pos_item = Vec2(this->getContentSize().width*0.4 + 175*(i%2*2-1), this->getContentSize().height*0.5 + 150*(i/2*2-1));
        item->setPosition(Vec2(pos_item.x, 850));
        this->addChild(item);
        item->setDelegate(this);
        item->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, pos_item)), CallFunc::create([=]{
            item->is_click_enable = true;
            if (i == max_item_jumpingmonkey -1) {
                monkey->playSequence("talk", true);
				auto audio_path = StringUtils::format("sounds/jumping_monkey/jumpingmonkey_mc2%d.mp3", random(1, 4));
                _isSpeaking = true;
                audio_mc_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_mc_id, [=](int au_id, std::string path){
                    _isSpeaking = false;
                    this->inActiveMode();
                    monkey->playSequence("idle", true);
                    auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
                    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                    });
                });
            }is_answer_enable = true;
        }), NULL));
        item->setName(StringUtils::format("item_%d", i));
    }
    doAutoPlay();
}

void MJGameJumpingMonkey::onClickItem(cocos2d::Ref *sender, int type){
    _isSpeaking = false;
    if (!is_answer_enable) return;
    AUDIO_ENGINE->stopEffect(audio_mc_id);

    unschedule("inActiveMode");
    AUDIO_ENGINE->stopEffect(audio_mc_id2);

    auto item = static_cast<MJGameJumpingMonkey_Item *>(sender);
    if (item->_item_info.text == game_dataset.list_objects[object_index].text) {
        AUDIO_ENGINE->stopAllEffects();
        item->onRightAnswer();
        is_answer_enable = false;


        monkey->playSequence("idle", true);
    }else{
        item->onWrongAnswer();
		AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_wrong.mp3");
        monkey->playSequence("wrong", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            monkey->playSequence("idle", true);
        });
    }
    
    if (!is_answer_enable) {
        auto    correct_audio = cocos2d::experimental::AudioEngine::play2d(SOUND_CORRECT_ANSWER);
        cocos2d::experimental::AudioEngine::setFinishCallback(correct_audio, [=](int, std::string) {
            auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {
                    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                        //  AUDIO_ENGINE->playEffect("sounds/jumping_monkey/jumpingmonkey_show2.mp3");
                        for (int i = 0; i < max_item_jumpingmonkey; i++) {
                            auto item = this->getChildByName(StringUtils::format("item_%d", i));
                            if (item) {
                                item->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(1.0f, Vec2(0, 1000))), CallFunc::create([=] {
                                    item->removeFromParent();
                                    }), NULL));
                            }
                        }

                        if (type == MJGameJumpingMonkey_Item::TEXT_TYPE) {
                            object_index++;
                            if (object_index >= max_turn_jumpingmonkey) {//game finish
                                this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
                                    this->onFinishGame();
                                    }), NULL));

                            }
                            else {//new turn
                                this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
                                    this->onStartGame();
                                    _isSpeaking = true;
                                    }), NULL));
                            }

                        }
                        else {
                            this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
                                this->showItemAnswer(MJGameJumpingMonkey_Item::TEXT_TYPE);
                                is_answer_enable = true;
                                }), NULL));

                        }
                        }), NULL));
                    });
            });
        /*cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
            });*/
        
    }
}

//void MJGameJumpingMonkey::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(this->getContentSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(this->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(this->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

void MJGameJumpingMonkey::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	auto list_object = value_data["list_item"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGameJumpingMonkey_Item::ItemInfo object_info;
		auto img_nature = math::resource::ResHelper::getImage(images, list_object[i].GetString());

		object_info.text = img_nature->getText();
		object_info.audio_path = img_nature->getAudio();
		object_info.img_path = img_nature->path;

		game_dataset.list_objects.push_back(object_info);
	}

	auto list_distracting = value_data["list_distracting"].GetArray();
	for (int i = 0; i < (int)list_distracting.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, list_distracting[i].GetString());
		MJGameJumpingMonkey_Item::ItemInfo distracting_info;
		distracting_info.text = img_nature->getText();
		distracting_info.img_path = img_nature->path;
		game_dataset.list_distracting.push_back(distracting_info);
	}
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameJumpingMonkey::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameJumpingMonkey::onRefresh, this);
    }
    return nullptr;
}

void MJGameJumpingMonkey::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = MJGameJumpingMonkey::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void MJGameJumpingMonkey::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}
void MJGameJumpingMonkey::onSpeakerClicked(cocos2d::Ref* sender) {
    if (!_isSpeaking) {
        _isSpeaking = true;
        auto speaker = cocos2d::experimental::AudioEngine::play2d(_foodIsShowing ? "sounds/jumping_monkey/jumpingmonkey_mc1.mp3" : speaker_audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(speaker, [=](int, std::string) {
            _isSpeaking = false;
        });
    } 
}

void MJGameJumpingMonkey::inActiveMode() {
  
    this->schedule([=](float dt) {
        
        if (!_isSpeaking) 
        {
            _isSpeaking = true;
            audio_mc_id2 = cocos2d::experimental::AudioEngine::play2d(_foodIsShowing ? "sounds/jumping_monkey/jumpingmonkey_mc1.mp3" : speaker_audio_path);

            cocos2d::experimental::AudioEngine::setFinishCallback(audio_mc_id2, [=](int, std::string) {
                _isSpeaking = false;
            });
        }

    }, _foodIsShowing ? 10.0f :  6.9f, "inActiveMode");

}

void MJGameJumpingMonkey::doAutoPlay()
{
    if (CONFIG_MANAGER->isAutoPlay()) {
        this->scheduleOnce([=](float dt) {

            if (_questionType == _typeOfQuestion::Food) {
                onClickFood(_foodCorrect, true);
            }
            else if (_questionType == _typeOfQuestion::Text) {
                onClickItem(_itemCorrect, MJGameJumpingMonkey_Item::TEXT_TYPE);
            }
            else {
                onClickItem(_itemCorrect, MJGameJumpingMonkey_Item::IMG_TYPE);
            }
        }, 5, "delay_auto_play");

    }
}
