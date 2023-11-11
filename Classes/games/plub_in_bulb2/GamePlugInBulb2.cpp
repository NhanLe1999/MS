
#include "GamePlugInBulb2.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
INITIALIZE_READER(GamePlugInBulb2);
#define speed_opacity 15

std::once_flag GamePlugInBulb2_reader;
#define CSB_NAME "csb/game/plug_in_bulb2/GamePlugInBulb2.csb"

#define POS1 Vec2(35,68)
#define POS2 Vec2(35,420)

GamePlugInBulb2 * GamePlugInBulb2::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GamePlugInBulb2_reader, [] {
        REGISTER_CSB_READER(GamePlugInBulb2);
        REGISTER_CSB_READER(GamePlugInBulb2_Item);
    });
    auto p = static_cast<GamePlugInBulb2 *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}


#include "MJDefault.h"
#define key_count_guide_plug_bulbs_2 "key_count_guide_plug_bulbs_2"
void GamePlugInBulb2::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_plug_bulbs_2, 0);
    this->scheduleOnce([count](float) {
        MJDEFAULT->setIntegerForKey(key_count_guide_plug_bulbs_2, count + 1);
    }, 20, "delay_update_count_guide_plug_bulbs_2");
    
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/game_sample.plist");
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
}

void GamePlugInBulb2::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

	
	game_dataset.number_level = value_data["content_info"]["number_level"].GetInt();
	auto list_object = value_data["content_info"]["data"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
		PlugData pdata;
		auto object_value = list_object[i].GetObject();
		pdata.pType = object_value["type"].GetInt();
		for (int j = 0; j < object_value["items"].Size(); j++){
			GamePlugInBulb2_Item::ItemInfo word_info;
			word_info.text = object_value["items"][j]["w"].GetString();
			word_info.img_path = object_value["items"][j]["img_path"].GetString();
			word_info.audio_path = object_value["items"][j]["audio_path"].GetString();
			word_info.description_path = object_value["items"][j]["audio_description"].GetString();
			word_info.id_answer = object_value["items"][j]["id_answer"].GetInt();
			pdata.items.push_back(word_info);
		}

		if ((GAME_PLUG_IN_BULB2_ITEM_TYPE)pdata.pType == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IMAGE_AND_TEXT || (GAME_PLUG_IN_BULB2_ITEM_TYPE)pdata.pType == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_TEXT_TO_TEXT){
			for (int j = 0; j < object_value["item2"].Size(); j++){
				GamePlugInBulb2_Item::ItemInfo word_info;
				word_info.text = object_value["item2"][j]["w"].GetString();
				word_info.img_path = object_value["item2"][j]["img_path"].GetString();
				word_info.audio_path = object_value["item2"][j]["audio_path"].GetString();
				word_info.description_path = object_value["item2"][j]["audio_description"].GetString();
				word_info.id_answer = object_value["item2"][j]["id_answer"].GetInt();
				pdata.item2.push_back(word_info);
			}
		}
		game_dataset.list_objects.push_back(pdata);
    }
	
}

void GamePlugInBulb2::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	game_dataset.number_level = value_data["number_level"].GetInt();
	auto img_intro = math::resource::ResHelper::getImage(images, value_data["intro"].GetString());
	game_dataset.audio_intro = img_intro->getAudio();
	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		PlugData pdata;
		auto object_value = list_object[i].GetObject();
		pdata.pType = object_value["type"].GetInt();
		for (int j = 0; j < object_value["items"].Size(); j++){
			GamePlugInBulb2_Item::ItemInfo word_info;
			word_info.id_answer = object_value["items"][j]["id_answer"].GetInt();

			auto text_true = math::resource::ResHelper::getImage(images, object_value["items"][j]["data1"].GetString());
			auto text_false = math::resource::ResHelper::getImage(images, object_value["items"][j]["data2"].GetString());

			word_info.text = text_true->getText();
			word_info.img_path = text_true->path;
			word_info.audio_path = text_true->getAudio();
			word_info.description_path = text_false->getAudio();
			
			pdata.items.push_back(word_info);
		}

		if ((GAME_PLUG_IN_BULB2_ITEM_TYPE)pdata.pType == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IMAGE_AND_TEXT || (GAME_PLUG_IN_BULB2_ITEM_TYPE)pdata.pType == GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_TEXT_TO_TEXT){
			for (int j = 0; j < object_value["item2"].Size(); j++){
				GamePlugInBulb2_Item::ItemInfo word_info;
				word_info.id_answer = object_value["item2"][j]["id_answer"].GetInt();

				auto text_true = math::resource::ResHelper::getImage(images, object_value["item2"][j]["data1"].GetString());

				word_info.text = text_true->getText();
				word_info.img_path = text_true->path;
				word_info.audio_path = text_true->getAudios().front();
				pdata.item2.push_back(word_info);
			}
		}
		game_dataset.list_objects.push_back(pdata);
	}
}

void GamePlugInBulb2::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();
}

void GamePlugInBulb2::setInputConfigGame(){
//    auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
	answer = game_dataset.list_objects.size();
	for (int i = 0; i < game_dataset.list_objects.size(); i++){
		mj::helper::shuffle(game_dataset.list_objects.at(i).items);
	}
}

void GamePlugInBulb2::loadFrames() {	
	//load anim con khi o day

	auto bg = cocos2d::utils::findChild<ImageView*>(this, "bg");
	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	}

	onPreGame();
}

void GamePlugInBulb2::onPreGame()
{
	auto voice_id1 = cocos2d::experimental::AudioEngine::play2d("sounds/plug_in_bulb2/plug_in_bulbs.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(voice_id1, [=](int au_id, std::string path){

		auto gameName = cocos2d::utils::findChild<ImageView*>(this, "imgGameName");
		gameName->runAction(Sequence::create(FadeOut::create(1.0f)
			, CallFunc::create([=](){
			onLoadLevel();
			onStartGame();
		}), nullptr));
	});
}

void GamePlugInBulb2::onLoadLevel()
{
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
	this->addChild(_speaker);
	_speaker->addClickEventListener([=](Ref* sender) {
		audioSpeaker();
	});
	_speaker->setTouchEnabled(false);

	this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=] {
		if (flag) {
			AUDIO_ENGINE->playSound(game_dataset.audio_intro, false, [=] {
				if (_flag && _flagAnswer) {
					inActiveMode(start, stop);
				}
				else
				{
					learningReminder();
				}
				_speaker->setTouchEnabled(true);
				flag = false;
			});
		}
	}), NULL));
	connect_true = 0;
	auto leftPanel = cocos2d::utils::findChild<Layout*>(this, "leftPanel");
	leftPanel->removeAllChildren();
	left_items.clear();
	for (int i = 0; i < game_dataset.list_objects.at(curText).items.size(); i++){
		GamePlugInBulb2_Item* aItem = GamePlugInBulb2_Item::createItem(game_dataset.list_objects.at(curText).items.at(i), GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_OUT, (GAME_PLUG_IN_BULB2_ITEM_TYPE)game_dataset.list_objects.at(curText).pType);
		aItem->setScale(0.85);
		if (i == 0)
			aItem->setPosition(POS1);
		else
			aItem->setPosition(POS2);
		
		aItem->plug->addTouchEventListener([=](cocos2d::Ref * sender, ui::Widget::TouchEventType event_type){
             this->unschedule("inActive_Mode");
			auto obj = static_cast<ui::Widget *>(sender);
			switch (event_type) {
			case ui::Widget::TouchEventType::BEGAN:{
				stopAudioSpeaker();
				if (_flag && _flagAnswer) {
					stopInActiveMode();
					inActiveMode(start, stop);
				}
				else
				{
					stopLearningReminder();
					learningReminder();
				}
				AUDIO_ENGINE->playEffect("sounds/plug_in_bulbs/bulb_drag.mp3");
				aItem->setLocalZOrder(++curZorder);
				aItem->getParent()->setLocalZOrder(curZorder);
				break;
			}
			case ui::Widget::TouchEventType::MOVED:{
				aItem->plug->setPosition(aItem->convertToNodeSpace(obj->getTouchMovePosition()));
				break;
			}
			case ui::Widget::TouchEventType::ENDED:
			case ui::Widget::TouchEventType::CANCELED:{
				for (auto f : right_items) {
					auto bounding_box = f->getBoundingBox();
					bounding_box.origin = f->getParent()->convertToWorldSpace(bounding_box.origin);
                    auto delta = bounding_box.origin.x - Director::getInstance()->getVisibleSize().width / 2;
                    bounding_box.origin.x -= delta;
                    bounding_box.size.width += delta;

					if (bounding_box.containsPoint(obj->getTouchEndPosition())) if (f->_item_info.id_answer == aItem->_item_info.id_answer) {
						itemOverLay();
						this->unschedule("show_guide_plug_bulbs_2");
                        Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");                                                                  
						connect_true++;
						int connect_t = connect_true;
						int cur_t = curText;
						aItem->connectToRight(f, [=](){
							removeItemOverLay();
							this->flag = false;
							if (!game_play_enable){
								if (connect_t == 1) {
									this->pauseGame();
									return;
								}
							}
							if (connect_t == game_dataset.list_objects.at(cur_t).items.size()) {
								itemOverLay();
								onFinishLevel();
								this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
									_speaker->setTouchEnabled(true);
								}), NULL));
                            }else{
                                this->unschedule("inActive_Mode");
                            }
						});
						return;
					}
				}

				aItem->movePlugBack();
				break;
			}
			default:
				break;
			}
		});
		leftPanel->addChild(aItem);
		left_items.push_back(aItem);
	}
	
	if (game_dataset.list_objects.at(curText).item2.size()>0){
		mj::helper::shuffle(game_dataset.list_objects.at(curText).item2);
	}
	else
		mj::helper::shuffle(game_dataset.list_objects.at(curText).items);
	auto rightPanel = cocos2d::utils::findChild<Layout*>(this, "rightPanel");
	rightPanel->removeAllChildren();
	right_items.clear();
	for (int i = 0; i < game_dataset.list_objects.at(curText).items.size(); i++){
		GamePlugInBulb2_Item* aItem = nullptr;
		if (game_dataset.list_objects.at(curText).item2.size()>i){
			aItem = GamePlugInBulb2_Item::createItem(game_dataset.list_objects.at(curText).item2.at(i), GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IN, (GAME_PLUG_IN_BULB2_ITEM_TYPE)game_dataset.list_objects.at(curText).pType);
			aItem->setScale(0.85);
		}
		else{
			aItem = GamePlugInBulb2_Item::createItem(game_dataset.list_objects.at(curText).items.at(i), GAME_PLUG_IN_BULB2_ITEM_TYPE::GPIB_IT_IN, (GAME_PLUG_IN_BULB2_ITEM_TYPE)game_dataset.list_objects.at(curText).pType);
			aItem->setScale(0.85);
		}
		
		if (i == 0)
			aItem->setPosition(POS1);
		else
			aItem->setPosition(POS2);

		rightPanel->addChild(aItem);
		right_items.push_back(aItem);
	}
}

void GamePlugInBulb2::onStartGame(){
	auto leftPanel = cocos2d::utils::findChild<Layout*>(this, "leftPanel");
	auto rightPanel = cocos2d::utils::findChild<Layout*>(this, "rightPanel");

	leftPanel->runAction(EaseIn::create(MoveTo::create(1.0f, Vec2(0, 0)), 1));
	rightPanel->runAction(Sequence::create(
		EaseIn::create(MoveTo::create(1.0f, Vec2(Director::getInstance()->getVisibleSize().width, 0)), 1)
		, CallFunc::create([=](){
		game_state = GAME_BASE_STATE::GBS_PLAYING;
        
        this->unschedule("show_guide_plug_bulbs_2");
        Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");   
            start, stop;
            auto l = left_items.at(random(0, (int)left_items.size() - 1));
            for (auto r : right_items) {
                if (l->_item_info.id_answer == r->_item_info.id_answer) {
                    auto start_bb = l->getBoundingBox();
                    start = Point(start_bb.getMidX(), start_bb.getMidY());
                    start = l->getParent()->convertToWorldSpace(start) + Point(215, -50);
                    
                    auto stop_bb = r->getBoundingBox();
                    stop = Point(stop_bb.getMidX(), stop_bb.getMidY());
                    stop = r->getParent()->convertToWorldSpace(stop) + Point(-215, -50);
                    
                    break;
                }
            }
			if (!flag) {
				if (_flag && _flagAnswer) {
					stopInActiveMode();
					inActiveMode(start, stop);
				}
				else
				{
					stopLearningReminder();
					learningReminder();
				}
			}
	}),nullptr));
}

void GamePlugInBulb2::onFinishGame(){
	stopInActiveMode();
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_COMMON);
	}), NULL));
}

void GamePlugInBulb2::onFinishLevel()
{
	stopInActiveMode();
	curText++;
	auto leftPanel = cocos2d::utils::findChild<Layout*>(this, "leftPanel");
	auto rightPanel = cocos2d::utils::findChild<Layout*>(this, "rightPanel");

	leftPanel->runAction(EaseInOut::create(MoveTo::create(1.0f, Vec2(-Director::getInstance()->getVisibleSize().width*0.8f, 0)),2));
	rightPanel->runAction(EaseInOut::create(MoveTo::create(1.0f, Vec2(Director::getInstance()->getVisibleSize().width*1.8f, 0)),2));
	if (curText < game_dataset.number_level){
		this->scheduleOnce([=](float dt){
			_flag = false;
			stopLearningReminder();
            onLoadLevel();
            onStartGame();
			removeItemOverLay();
		}, 1.2f, "start_next_level");
	}
	else{
		//_speaker->setTouchEnabled(false);
		this->scheduleOnce([=](float dt){
			stopLearningReminder();
			onFinishGame();
		}, 0.5f, "finish_game");
		
	}
}

#pragma callback

void GamePlugInBulb2::onRefresh(cocos2d::Ref *sender) {
    /*this->removeFromParent();
    auto phonics = GamePlugInBulb2::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GamePlugInBulb2::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void GamePlugInBulb2::showGuide(Point start, Point stop) {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    
    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setName("guide_overlay");
    Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
    
    auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
	hand->setName("hand_guide_game");
    overlay->addChild(hand, INT_MAX - 1);
    hand->setPosition(start);
    
    hand->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(2, stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));
}

void GamePlugInBulb2::inActiveMode(Point start, Point stop){
	this->schedule([this, start, stop](float) {
		this->showGuide(start, stop);
		AUDIO_ENGINE->stopSound(game_dataset.audio_intro);
		AUDIO_ENGINE->playSound(game_dataset.audio_intro);
		}, 15, 100, 10, "show_guide_plug_bulbs_2");
}

void GamePlugInBulb2::stopInActiveMode() {
	unschedule("show_guide_plug_bulbs_2");
	AUDIO_ENGINE->stopSound(game_dataset.audio_intro);
}

void GamePlugInBulb2::learningReminder() {
	this->schedule([=](float) {
		AUDIO_ENGINE->stopSound(game_dataset.audio_intro);
		AUDIO_ENGINE->playSound(game_dataset.audio_intro);
	}, 15, 100, 10, "learning_reminder");
}

void GamePlugInBulb2::stopLearningReminder() {
	unschedule("learning_reminder");
	AUDIO_ENGINE->stopSound(game_dataset.audio_intro);
}

void GamePlugInBulb2::stopAudioIntro() {
	AUDIO_ENGINE->stopSound(game_dataset.audio_intro);
}

void GamePlugInBulb2::audioSpeaker() {
	_speaker->setTouchEnabled(false);
	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->playSound(game_dataset.audio_intro, false, [=] {
		_speaker->setTouchEnabled(true);
		if (_flag && _flagAnswer) {
			stopInActiveMode();
			inActiveMode(start, stop);
		}
		else
		{
			stopLearningReminder();
			learningReminder();
		}
	});
}

void GamePlugInBulb2::stopAudioSpeaker() {
	_speaker->setTouchEnabled(true);
	AUDIO_ENGINE->stopSound(game_dataset.audio_intro);
	AUDIO_ENGINE->stopAllAudio();
}

void GamePlugInBulb2::itemOverLay() {
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

void GamePlugInBulb2::removeItemOverLay() {
	if (cocos2d::utils::findChild(this, "overlay")) {
		this->removeChildByName("overlay");
	}
}
