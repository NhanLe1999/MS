//
//	MPBubbleBoom.h
//	Created by mintzy on 04/23/18
//
// 
#include "MPBubbleBoom.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"

INITIALIZE_READER(MPBubbleBoom);

#define speed_opacity 15

std::once_flag MPBubbleBoom_reader;

#define CSB_NAME "csb/game/MPBubbleBoom/MPBubbleBoom.csb"
#define GAF_SEAWEED_LEFT "gaf/mpbubbleboom/mp_bubbleboom_seaweedleft/bubbleboom_seaweedleft.gaf"
#define GAF_SEAWEED_RIGHT "gaf/mpbubbleboom/mp_bubbleboom_seaweedright/bubbleboom_seaweedright.gaf"
#define GAF_SPONGE "gaf/mpbubbleboom/mp_bubbleboom_sponge/bubbleboom_sponge.gaf"
#define GAF_CRAB   "gaf/mpbubbleboom/mp_bubbleboom_crab/bubbleboom_crab.gaf"
#define GAF_INTRO   "gaf/mpbubbleboom/mp_bubbleboom_intro/Bubbleboom_intro.gaf"

#define BUBBLE_SPEED 150.0f
#define BUBBLE_PUSH_TIME 0.75f
MPBubbleBoom * MPBubbleBoom::createGame(std::string json_file)
{
	std::call_once(MPBubbleBoom_reader, [] {
		REGISTER_CSB_READER(MPBubbleBoom);
		REGISTER_CSB_READER(MPBubbleBoom_Item);
	});
	auto p = static_cast<MPBubbleBoom *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

MPBubbleBoom * MPBubbleBoom::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::call_once(MPBubbleBoom_reader, [] {
		REGISTER_CSB_READER(MPBubbleBoom);
		REGISTER_CSB_READER(MPBubbleBoom_Item);
	});
	auto p = static_cast<MPBubbleBoom *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file,images);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

MPBubbleBoom * MPBubbleBoom::createGame(mj::GAME_PHONIC_INFO game_phonic_info, int max_bubble_push)
{
	std::call_once(MPBubbleBoom_reader, [] {
		REGISTER_CSB_READER(MPBubbleBoom);
		REGISTER_CSB_READER(MPBubbleBoom_Item);
	});
	auto p = static_cast<MPBubbleBoom *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(game_phonic_info, max_bubble_push);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;;
}

void MPBubbleBoom::generateData(rapidjson::Value value_data)
{
	rapidjson::Value& game_config_data = value_data.FindMember("game_config")->value;
	this->max_bubble_push = game_config_data.FindMember("max_bubble_push")->value.GetInt();
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	_game_phonic_info.list_phonic.clear();
	_game_phonic_info.list_distracting_phonic.clear();
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		mj::PHONIC_INFO info;
		if (item.HasMember("text_phonic")) {
			info.text_phonic = item.FindMember("text_phonic")->value.GetString();
		}
		if (item.HasMember("audio_phonic")) {
			info.audio_phonic = item.FindMember("audio_phonic")->value.GetString();
		}
		if (item.HasMember("text_full")) {
			info.text_full = item.FindMember("text_full")->value.GetString();
		}
		if (item.HasMember("audio_full")) {
			info.audio_full = item.FindMember("audio_full")->value.GetString();
		}
		if (item.HasMember("image_path")) {
			info.image_path = item.FindMember("image_path")->value.GetString();
		}
		_game_phonic_info.list_phonic.push_back(info);
	}
}

void MPBubbleBoom::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	rapidjson::Value& game_config_data = value_data.FindMember("game_config")->value;
	this->max_bubble_push = game_config_data.FindMember("max_bubble_push")->value.GetInt();
	
	if (game_config_data.HasMember("game_type")) {
		std:string type = game_config_data.FindMember("game_type")->value.GetString();
		if (type == "text") play_text = true;
		else if (type == "image") play_text = false;
		else if (type == "imageAndText") play_text_image = true;
	}

	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		mj::PHONIC_INFO info;
		auto image_id = item.FindMember("image_id")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images, image_id);
		info.image_path = image->path;
		info.text_full = image->getText("name_1");
		info.audio_full = image->getAudios("name_1").front();
		info.text_phonic = image->getText("name_n");
		info.audio_phonic = image->getAudios("name_n").front();
		_game_phonic_info.list_phonic.push_back(info);
	}
	for (int i = 0; i < _game_phonic_info.list_phonic.size(); ++i) {
		bool insert = true;
		auto info = _game_phonic_info.list_phonic[i];
		if (info.text_phonic.empty()) {
			continue;
		}
		for (int j = 0; j < _list_phonic.size(); ++j) {
			if (info.text_phonic == _list_phonic[j].text_phonic) {
				insert = false;
				break;
			}
		}
		if (insert) {
			_list_phonic.push_back(info);
		}
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback MPBubbleBoom::onLocateClickCallback(const std::string & callback_name)
{
	if (callback_name == "onRefresh") {
		return CC_CALLBACK_1(MPBubbleBoom::onRefresh, this);
	}
	return nullptr;
}

void MPBubbleBoom::didLoadFromCSB(std::string json_file)
{	
	if (json_file.size() < 5) {
		json_file = "json/mpbubbleboom/config.json";
	};
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	
	this->generateData(document.GetObject());
	mj::helper::shuffle(_game_phonic_info.list_phonic);
	
}

void MPBubbleBoom::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
{
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document value_data;
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

	this->generateData(value_data.GetObject(),images);
	mj::helper::shuffle(_game_phonic_info.list_phonic);
}

void MPBubbleBoom::didLoadFromCSB(mj::GAME_PHONIC_INFO game_phonic_info, int max_bubble_push)
{
	_game_phonic_info = game_phonic_info;
	this->max_bubble_push = max_bubble_push;
	mj::helper::shuffle(_game_phonic_info.list_phonic);
	mj::helper::shuffle(_game_phonic_info.list_distracting_phonic);	
}

void MPBubbleBoom::loadFrames()
{
	screen_size = Director::getInstance()->getWinSize();
	// Component
	layout_root = cocos2d::utils::findChild<ui::Layout* >(this, "layout_root");
	layout_root->setTouchEnabled(true);
	layout_root->addClickEventListener([=](Ref*) {
		CCLOG("touched layout_root");
		scheduleSoundGuiding();
		});

	layout_bg = cocos2d::utils::findChild<ui::Layout* >(this, "layout_bg");
	layout_bg->setTouchEnabled(true);
	layout_bg->addClickEventListener([=](Ref*) {
		CCLOG("clicked layout_bg");
		scheduleSoundGuiding();
		});
	layout_sand = cocos2d::utils::findChild<ui::Layout* >(this, "layout_sand");
	layout_sand->setTouchEnabled(true);
	layout_sand->addClickEventListener([=](Ref*) {
		CCLOG("clicked layout_sand");
		scheduleSoundGuiding();
		});

	//GAF
	gaf_seaweed_left = GAFWrapper::createGAF(GAF_SEAWEED_LEFT);
	gaf_seaweed_left->setPosition(cocos2d::Vec2(0.2 * screen_size.width / 2, 0.3 * screen_size.height));
	gaf_seaweed_left->play(true);
	layout_bg->addChild(gaf_seaweed_left, 1);

	gaf_seaweed_right = GAFWrapper::createGAF(GAF_SEAWEED_RIGHT);
	gaf_seaweed_right->setPosition(Vec2(0.8 * screen_size.width, 0.32 * screen_size.height));
	gaf_seaweed_right->play(true);
	layout_bg->addChild(gaf_seaweed_right, 1);

	gaf_sponge = GAFWrapper::createGAF(GAF_SPONGE);
	layout_sand->addChild(gaf_sponge, 1);
	gaf_sponge->setPosition(Vec2(screen_size.width / 2, 0.3 * screen_size.height));
	gaf_sponge->playSequence("idle", true);

	gaf_crab = GAFWrapper::createGAF(GAF_CRAB);
	layout_sand->addChild(gaf_crab, 1);
	gaf_crab->setPosition(Vec2(0.75 * screen_size.width, 0.16 * screen_size.height));
	gaf_crab->play(true);

	// Genarate location
	_locations.push_back(Vec2(0.5 * screen_size.width, 0.35 * screen_size.height));
	_locations.push_back(Vec2(0.75 * screen_size.width, 0.35 * screen_size.height));
	_locations.push_back(Vec2(0.25 * screen_size.width, 0.35 * screen_size.height));
	_locations.push_back(Vec2(0.625 * screen_size.width, 0.6 * screen_size.height));
	_locations.push_back(Vec2(0.375 * screen_size.width, 0.6 * screen_size.height));

	min_bubble = 1;
	max_bubble = 1;
	combo_count = 0;
	bubble_speed = BUBBLE_SPEED;
	bubble_push_time = BUBBLE_PUSH_TIME;
	time_now = 0;
	last_time = 0;
	push_locked = false;

	PHONIC_MANAGER->cacheGAF("mp_bubbleboom_finbb_1", "gaf/mpbubbleboom/mp_bubbleboom_finbb_1/bubbleboom_finbb_1.gaf");
	PHONIC_MANAGER->cacheGAF("mp_bubbleboom_finbb_2", "gaf/mpbubbleboom/mp_bubbleboom_finbb_2/bubbleboom_finbb_2.gaf");
}

void MPBubbleBoom::showIntroGame()
{
	auto touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 69696969);
	GAFWrapper* gaf_intro = GAFWrapper::createGAF(GAF_INTRO);
	gaf_intro->setPosition(Vec2(screen_size / 2));
	gaf_intro->setVisible(!(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()));
	layout_sand->addChild(gaf_intro, 1000);
	
	AUDIO_ENGINE->play("sounds/mpbubbleboom/mpbubbleboom_intro.mp3");		
	if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/mpbubbleboom/Bubble_Boom_new.mp3")) {
		this->runAction(Sequence::create(DelayTime::create(1), 
			CallFunc::create([=] {
				AUDIO_ENGINE->playSound("sounds/mpbubbleboom/Bubble_Boom_new.mp3", false, ([=] {
					if (cocos2d::experimental::AudioEngine::getState(soundId) != cocos2d::experimental::AudioEngine::AudioState::PLAYING)
					{
						soundId = AUDIO_ENGINE->playSound("sounds/mpbubbleboom/mpbubbleboom_tap_the_bubbles.mp3",
							false,
							([=] { scheduleSoundGuiding(); }), 1.0f);
					}
					}));
				}), 
			nullptr ));			
	}
	else {
		if (cocos2d::experimental::AudioEngine::getState(soundId) != cocos2d::experimental::AudioEngine::AudioState::PLAYING)
		{
			soundId = AUDIO_ENGINE->playSound("sounds/mpbubbleboom/mpbubbleboom_tap_the_bubbles.mp3",
				false,
				([=] { scheduleSoundGuiding(); }), 1.0f);
		}
	}

	gaf_sponge->playSequence("action");
	gaf_intro->play(false, [=](gaf::GAFObject*obj) {
		gaf_intro->removeFromParent();
		
		if (is_auto_play) {
			autoPlay();
		}
		else {
			touch_layout->removeFromParent();
			//showTapGuideGame();
		}
		
	});

	
}

void MPBubbleBoom::onRefresh(cocos2d::Ref * sender)
{

}

void MPBubbleBoom::onEnter()
{
	MJMainGBase::onEnter();
	this->loadFrames();
	
	playBackgroundMusic("sounds/mpbubbleboom/mpbubbleboom_bgm.mp3");
	//this->scheduleOnce([=](float) {
	//	playBackgroundMusic("sounds/mpbubbleboom/mpbubbleboom_bgm.mp3");
	//	
	//}, 0.1f, "playBackgroundMusic");
	bubble_push_count = 0;


	MPBubbleBoom_Pool::getInstance()->releaseAllItem();
	// prepare bubble 
	for (auto info : _game_phonic_info.list_phonic) {
        auto bubble_item = MPBubbleBoom_Item::createItem(info);
        MPBubbleBoom_Pool::getInstance()->releaseItem(bubble_item);
		
	}
	float delay_intro = 6.0f;
	this->schedule([&](float dt) {
		int n = cocos2d::random(min_bubble, max_bubble);
		int count = 0;
		for (auto child : layout_root->getChildren()) {
			auto item = static_cast<MPBubbleBoom_Item *>(child);
			if (!item->isBoomed() && item->getPositionY() < screen_size.height) ++count;
		}
		if (count == 0 && bubble_push_count < max_bubble_push) {
			pushBubble(n);	
		}
	}, 0.1f, kRepeatForever, delay_intro, "pushBubble");
	this->schedule(CC_CALLBACK_1(MPBubbleBoom::smallBubble, this),
		3.0f, kRepeatForever, 0.0f, "pushSmallBubble");
	this->schedule([=](float dt) {
		time_now += 0.1;
	},0.1f, kRepeatForever, 0.0f, "time");
	showIntroGame();
	
}

void MPBubbleBoom::onExitTransitionDidStart()
{
	PHONIC_MANAGER->clearCacheGaf();
	MPBubbleBoom_Pool::getInstance()->releaseAllItem();
	MJMainGBase::onExitTransitionDidStart();
}

void MPBubbleBoom::onFinishGame()
{
	//hideTapGuideGame();
	this->unschedule("scheduleSoundGuiding");
	this->unscheduleAllCallbacks();
	MJMainGBase::onFinishGame();
}

void MPBubbleBoom::onClickedBubbleBoomDelegate(cocos2d::Ref * sender)
{
	AUDIO_ENGINE->stopAllSound();
	scheduleSoundGuiding();
	CCLOG("Clicked bubble");
	auto bubble_item = static_cast<MPBubbleBoom_Item *>(sender);
	bubble_item->setEnableClick(false);
	//AUDIO_ENGINE->play(bubble_item->_phonic_info.audio_phonic);
	if (bubble_item->isStarBubble()) {
		this->runAction(Sequence::create(CallFunc::create([=] {
			bubble_item->onBubbleFinishAction("action", false);
		}), DelayTime::create(0.75f), CallFunc::create([=] {
			bubble_item->onBubbleBoomed();
		}), DelayTime::create(1.25f), CallFunc::create([=] {
			bubble_item->removeFromParent();
			showFinishBubble();
		}),NULL));
		
	}
	else {
		bubble_item->onBubbleBoomed([=] {
			if (bubble_item->isPhonicBubble()) {
                MPBubbleBoom_Pool::getInstance()->releaseItem(bubble_item);
				bubble_item->removeFromParentAndCleanup(false);
			}
			else {
				bubble_item->runAction(cocos2d::Sequence::createWithTwoActions(MoveBy::create(0.5f, cocos2d::Vec2(0, screen_size.width)), CallFunc::create([=] {
                    MPBubbleBoom_Pool::getInstance()->releaseItem(bubble_item);
					bubble_item->removeFromParentAndCleanup(false);
				})));
			}

		});
		min_bubble = 2;
		max_bubble = 3;
		if (bubble_push_count > 5) max_bubble = 5;
	}
	//showTapGuideGame();
}

void MPBubbleBoom::onChangeBubbleBoomStateDelegate(cocos2d::Ref * sender, int state)
{
	auto bubble_item = static_cast<MPBubbleBoom_Item *>(sender);
	if (state == MPBubbleBoom_Item::BOOMED) {
		++bubble_push_count;
		if (bubble_push_count == max_bubble_push) showFinishBubble();
		AUDIO_ENGINE->play("sounds/mpbubbleboom/mpbubbleboom_boom.mp3", false);
		if (bubble_item->isPhonicBubble()) {
			AUDIO_ENGINE->play(bubble_item->_phonic_info.audio_phonic);
		}
		else {
			AUDIO_ENGINE->play(bubble_item->_phonic_info.audio_full);
		}	
		int count = 0;
		for (auto child : layout_root->getChildren()) {
			auto item = static_cast<MPBubbleBoom_Item *>(child);
			if (!item->isBoomed()) ++count;
		}
		if (count < 4 && bubble_push_count < max_bubble_push) {
			int n = cocos2d::random(min(min_bubble,2),min( 5 - count,max_bubble));
			pushBubble(n);
			
		}
		if (time_now - last_time > 1.5f) {
			combo_count = 0;
		}
		else {
			combo_count = (combo_count > 5 ) ? 5 : combo_count + 1;
		}
		last_time = time_now;	
	}


}

void MPBubbleBoom::pushBubble(int num_bubble)
{
	if (push_locked) return;
	push_locked = true;
	// Genarate Bubble
	AUDIO_ENGINE->play("sounds/mpbubbleboom/mpbubbleboom_sponge.mp3");
	gaf_sponge->playSequence("action", false, [=](gaf::GAFObject * object, const std::string & callbackName) {
		gaf_sponge->playSequence("idle");
	});
	auto bubbles = getListBubble(num_bubble);
	
	bubble_push_time = BUBBLE_PUSH_TIME - combo_count * 0.05;
	bubble_speed = BUBBLE_SPEED + 10 * combo_count;

	this->runAction(Sequence::create(DelayTime::create(bubble_push_time + (num_bubble - 1) * (0.2 - combo_count * 0.03)),
		CallFunc::create([=] {
		push_locked = false;
	}), NULL));
	

	for (int i = 0; i < bubbles.size(); ++i) {
		float delay_move_time = (num_bubble - 1 - i ) * (0.3 - combo_count * 0.035);
		layout_root->addChild(bubbles[i], 1000);
		bubbles[i]->setUsed(true);		
		bubbles[i]->setVisible(true);
		float life_time = cocos2d::random(4.0f, 6.0f);
		Vec2 loc;
		int index;
		bool find_loc = false;
		for (int i = 0; i < _locations.size(); ++i) {
			if (!region_used[i]) {
				loc = _locations[i];
				index = i;
				find_loc = true;
				break;
			}
		}
		if (!find_loc) {
			index = cocos2d::random(0, (int)_locations.size() - 1);
			loc = _locations[index];
		}
		region_used[index] = true;
		scheduleOnce([=](float dt) {
			region_used[index] = false;
		}, delay_move_time + bubble_push_time + 150/BUBBLE_SPEED, cocos2d::StringUtils::format("region %d", bubbles[i]));
		bubbles[i]->setScale(0.0f);
		Vec2 start_pos;
		if (index == 1) {
			start_pos = cocos2d::Vec2(screen_size.width / 2 + 70 , 0.15 * screen_size.height);
		}
		else if(index == 2){
			start_pos = cocos2d::Vec2(screen_size.width / 2 - 50, 0.15 * screen_size.height);
		}
		else {
			start_pos = cocos2d::Vec2(screen_size.width / 2 + 20, 0.2 * screen_size.height);
		}
		bubbles[i]->setPosition(start_pos);
		bubbles[i]->runAction(Sequence::create(
			DelayTime::create(delay_move_time),
			Spawn::createWithTwoActions(MoveTo::create(bubble_push_time, loc), ScaleTo::create(bubble_push_time, 1)),
			MoveBy::create(screen_size.height / bubble_speed, cocos2d::Vec2(0, screen_size.height)),
			CallFunc::create([=] {
            MPBubbleBoom_Pool::getInstance()->releaseItem(bubbles[i]);
			bubbles[i]->removeFromParentAndCleanup(false);
		}), NULL));

	}
	
}

void MPBubbleBoom::smallBubble(float dt)
{
	int num_bubble = cocos2d::random(3, 7);
	for (int i = 0; i < num_bubble; ++i)
	{
		std::string img_type = (cocos2d::random(0, 1) == 1) ? "bubblestar" : "bubble";
		int img_index = cocos2d::random(1, 5);
		ui::ImageView * small_bubble = ui::ImageView::create();
		small_bubble->loadTexture(cocos2d::StringUtils::format("games/mpbubbleboom/mp_bubbleboom_%s%d.png", img_type.c_str(), img_index));
		int size = cocos2d::random(2, 5) * 10;
		small_bubble->setScale9Enabled(true);
		auto render_size = small_bubble->getVirtualRendererSize();
		small_bubble->setCapInsets(Rect(1, 1, render_size.width - 2, render_size.height - 2));

		small_bubble->setContentSize(Size(size, size));
		auto pos = Vec2(cocos2d::random(0.1,0.9) * screen_size.width, 0);
		small_bubble->setPosition(pos);
		layout_bg->addChild(small_bubble, 2);
		float time_life = cocos2d::random(2.0f, 4.0f);
		float speed = 150 + cocos2d::random(-75, 75);
		small_bubble->runAction(Sequence::create(
			Spawn::createWithTwoActions(MoveBy::create(time_life, Vec2(0, speed * time_life)),ScaleTo::create(time_life,0.3f)),
			CallFunc::create([=] {
			small_bubble->removeFromParent();
		}), NULL));

	}
}

void MPBubbleBoom::showTapGuideGame(float delay_time, std::string gaf_guide_path)
{
	auto guide_layout_check = this->getChildByName("guide_layout");
	if (guide_layout_check) return;
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		if (layout_root->getChildrenCount() > 0) {
			auto children = layout_root->getChildren();
			for (auto child : children) {
				if (child->getPositionY() < screen_size.height * 0.7) {
					static_cast<MPBubbleBoom_Item *>(child)->showTapGuideGame(gaf_guide_path);
					break;
				}
			}
		}


	}, 5, 100, delay_time, "animated_guide");
}

void MPBubbleBoom::hideTapGuideGame()
{
	Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
}

void MPBubbleBoom::scheduleSoundGuiding()
{
	CCLOG("schedule guiding");
	this->unschedule("scheduleSoundGuiding");
	this->schedule(([=](float dt) {
		auto sound_id = AUDIO_ENGINE->playSound(CONFIG_MANAGER->isGameVMonkey() ? "sounds/mpbubbleboom/mpbubbleboom_tap_the_bubbles_vi.mp3" : "sounds/mpbubbleboom/tap_the_bubbles.mp3",
			false,([=] {
				scheduleSoundGuiding();
				}));
		}), 10.0f, 1000, 10.0f, "scheduleSoundGuiding");
}

void MPBubbleBoom::showFinishBubble()
{
	this->schedule([=](float dt) {
		int count = 0;
		for (auto child : layout_root->getChildren()) {
			auto item = static_cast<MPBubbleBoom_Item *>(child);
			if (!item->isBoomed() && item->getPositionY() < screen_size.height) ++count;
		}
		// Push Bubble Finish
		if (count == 0) {	
			unschedule("pushBubbleFinish");
			if (_list_phonic.empty()) {
				onFinishGame();
				return;
			}
			auto info = _list_phonic.back();
			_list_phonic.pop_back();
			++bubble_push_count;
			MPBubbleBoom_Item * item = MPBubbleBoom_Item::createItem(info,true);

			if (play_text_image) {
				item->setBubbleConfig(true, true, true, true); // defaul
			}
			else {
				if (play_text) {
					// only show text
					item->setBubbleConfig(false, true, true);
				}
				else {
					// only show image
					item->setBubbleConfig(false, false, true);
				}

			}			
			layout_root->addChild(item, 1001);
			item->setScale(0);
			item->setDelegate(this);
			item->setEnableClick(false);
			item->setPosition(Vec2(screen_size.width / 2 + 70, 0.15 * screen_size.height));

			item->runAction(
				Sequence::create(DelayTime::create(1.0f),
				Spawn::createWithTwoActions(EaseCubicActionOut::create(  MoveTo::create(BUBBLE_PUSH_TIME, Vec2(screen_size.width / 2, screen_size.height * 0.6))),
											ScaleTo::create(BUBBLE_PUSH_TIME, 1.5f)),
				CallFunc::create([=] {
					item->setEnableClick(true);
					item->onBubbleFinishAction("idle",true);
			}),	NULL));
		}
	}, 0.2f, 1000, 0.0f, "pushBubbleFinish");
}

std::vector<MPBubbleBoom_Item*> MPBubbleBoom::getListBubble(int num_bubble)
{
	std::vector<MPBubbleBoom_Item*> list_bubble;
	list_bubble.clear();
	int num_icon = num_bubble * 0.6;
	while (list_bubble.size() < num_bubble) {

		MPBubbleBoom_Item * bubble_item = MPBubbleBoom_Pool::getInstance()->getItem();

		if (bubble_item == nullptr) {
			int index = cocos2d::random(0, (int)_game_phonic_info.list_phonic.size() - 1);
			auto bubble_info = _game_phonic_info.list_phonic[index];
			bubble_item = MPBubbleBoom_Item::createItem(bubble_info);
			CCLOG("Create bubble %s", bubble_info.audio_phonic.c_str());
		}
		if (list_bubble.size() < num_icon && bubble_push_count >= 5) {
			if (play_text_image) {
				bubble_item->setBubbleConfig(false, false, false);
			}
			else {
				if (play_text) {
					bubble_item->setBubbleConfig(false, play_text, false);
				}
				else bubble_item->setBubbleConfig(false, false, false);
			}
		}
		else {
			bool upper_case = cocos2d::random(0, 1) == 1;
			if (play_text_image) {
				bubble_item->setBubbleConfig(true, true, upper_case);
			}
			else {
				if (play_text) {
					bubble_item->setBubbleConfig(false, play_text, upper_case);
				}
				else bubble_item->setBubbleConfig(false, false, upper_case);
			}
		}
		bubble_item->setVisible(false);
		bubble_item->setDelegate(this);
		bubble_item->setUsed(true);
		list_bubble.push_back(bubble_item);
	}
	mj::helper::shuffle(list_bubble);
	return list_bubble;

}



void MPBubbleBoom::autoPlay()
{
	this->schedule([=](float dt) {
		for (auto child : layout_root->getChildren()) {
			auto item = static_cast<MPBubbleBoom_Item *>(child);
			if (!item->isBoomed() && item->getPositionY() < screen_size.height) {
				item->onAutoClick();
				break;
			}
			
		}
	}, 1.0f, kRepeatForever, 2.0f, "autoPlay");
}
