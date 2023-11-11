/*
*
* MexicanCactus.cpp
* Created by minhtzy on 3/10/18
*/
#include "MexicanCactus.h"

#define game_name_image "games/mexicancactus/cactus.png"
#define game_name_sound "sounds/mexicancactus/cactus_jumble.mp3"

MexicanCactus::MexicanCactus()
	: item_index{ 0 }
	, m_configpath{ "" }
	, root_layout{ nullptr }
{
}

MexicanCactus * MexicanCactus::createGame(std::string json_file)
{
	auto game = new MexicanCactus();
	if (game && game->init()) {
		game->autorelease();
		game->didLoadFromCSB(json_file);
	}
	else {
		CC_SAFE_DELETE(game);
		game = nullptr;
	}
	return game;
}

void MexicanCactus::onEnter()
{
	MJMainGBase::onEnter();
	generateData();
	loadFrames();
	showGamename();
	playBackgroundMusic("sounds/mexicancactuses/sfx/cactus_bgm.mp3");
}

void MexicanCactus::onExitTransitionDidStart()
{
	MJMainGBase::onExitTransitionDidStart();
}

void MexicanCactus::onExit()
{
	this->unschedule("in_active_mode");
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
	AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onExit();
}

void MexicanCactus::onFinishGame()
{
	MJMainGBase::onFinishGame();
}

void MexicanCactus::didLoadFromCSB(std::string json_file)
{
	m_configpath = json_file;
}

void MexicanCactus::generateData()
{
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(m_configpath);
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

	if (value_data.HasMember("number_turnplay") && value_data["number_turnplay"].IsInt()) {
		number_turnplay = value_data["number_turnplay"].GetInt();
	}

	if (value_data.HasMember("list_item") && value_data["list_item"].IsArray()) {
		auto list_item_data = value_data["list_item"].GetArray();
		for (int i = 0; i < (int)list_item_data.Size(); i++) {
			auto item_data = list_item_data[i].GetObject();

			MEXICAN_CACTUS_INFO item_info;			
			auto letter = item_data["text"].GetString();
			auto pImage = math::resource::ResHelper::getImage(m_images, letter);
			item_info.text = pImage->getText("name_1");
			item_info.normal_audio = pImage->getAudios("name_1").front();
			item_info.slow_audio = pImage->getAudios("name_n").front();

			//TODO: audio and text phonic
			auto phonics = pImage->getTextPhonic("name_p");
			auto phonic_audios = pImage->getAudioPhonic("name_p");

			for (int j = 0; j < (int)phonics.size(); j++) {
				MEXICAN_CACTUS_CHARACTER_INFO character_info;
				character_info.text =phonics[j];
				character_info.audio = phonic_audios[j];
				item_info.list_character.push_back(character_info);
			}
			genCharacterIndex(item_info);

			list_item_info.push_back(item_info);
		}
	}
}

void MexicanCactus::loadFrames()
{
	this->setContentSize(screen_size);

	root_layout = cocos2d::ui::Layout::create();
	root_layout->setContentSize(screen_size);
	root_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	root_layout->setBackGroundColorOpacity(255);
	root_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	root_layout->setTouchEnabled(true);
	this->addChild(root_layout);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	touch_layout->setBackGroundColor(cocos2d::Color3B::BLACK);
	touch_layout->setBackGroundColorOpacity(0);
	touch_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	this->addChild(touch_layout, 1000);

	if (0 == cocos2d::random(0, 1)) {
		auto background = cocos2d::ui::ImageView::create("games/mexicancactus/bg.png");
		background->setPosition(Vec2(screen_size.width / 2, 450));
		root_layout->addChild(background, 1, "bg");
	}
	else {
		auto background = cocos2d::ui::ImageView::create("games/mexicancactus/bg1.png");
		background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		background->setPosition(Vec2(screen_size.width / 2, 0));
		root_layout->addChild(background, 1, "bg");
	}


	auto rocks = cocos2d::ui::ImageView::create("games/mexicancactus/rocks.png");
	rocks->setPosition(Vec2(screen_size.width / 2, 175));
	root_layout->addChild(rocks, 2, "rocks");

	text_lb = cocos2d::ui::Text::create(" ", font_name, 150);
	text_lb->enableOutline(cocos2d::Color4B::BLACK, 3);
	this->addChild(text_lb, 1001);

	text_hint = cocos2d::ui::Text::create(" ", font_name, 150);
	text_hint->enableOutline(cocos2d::Color4B::BLACK, 3);
	text_hint->setOpacity(0);
	text_hint->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.8));
	this->addChild(text_hint, 1001);

	speaker = cocos2d::ui::Button::create("games/mexicancactus/loa icon.png");
	speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	speaker->setPosition(Vec2(50, screen_size.height - 50));
	root_layout->addChild(speaker, 100);
	speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(MexicanCactus::onClickedSpeaker, this));

	gaf_confetti = GAFWrapper::createGAF("gaf/mexicancactus/confetti/confetti.gaf");
	gaf_confetti->setPosition(Vec2(screen_size.width / 2 - 50, screen_size.height / 2 + 150));
	root_layout->addChild(gaf_confetti, 1002);

	disableTouch();
}

void MexicanCactus::showIntroGame()
{
}

void MexicanCactus::showGamename()
{

	if (FileUtils::getInstance()->isFileExist(game_name_image)) {
		_gameNameImage = cocos2d::Sprite::create(game_name_image);
		if (_gameNameImage)
		{
			_gameNameImage->setPosition(screen_size / 2);
			_gameNameImage->setScale(0.8);
			this->addChild(_gameNameImage);
			_gameNameImage->setOpacity(0);

			this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
				_gameNameImage->runAction(FadeIn::create(1.0f));
				}), DelayTime::create(0.25f), CallFunc::create([=] {
					AUDIO_ENGINE->playSound(game_name_sound, false, [=] {
						this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
							_gameNameImage->runAction(FadeOut::create(1.0f));
							}), NULL));
						});
					}), NULL));
		}
	}

	this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=] {
		startGame();
		}), NULL));
}

void MexicanCactus::startGame()
{
	wrong_count = 0;
	createCactus();
}

void MexicanCactus::onMovedCactusBegan(cocos2d::Ref * sender)
{
	stopInActiveMode();
	skipAudioSpeaker();
	auto cactus = static_cast<MexicanCactus_Item *> (sender);
	cactus->setScale(1.25f);
	AUDIO_ENGINE->playSound(cactus->getCactusInfo().audio);
	AUDIO_ENGINE->playSound("sounds/mexicancactus/mc_interact.mp3");
	cactus->setLocalZOrder(15);
}


void MexicanCactus::onMovedCactusEnded(cocos2d::Ref * sender)
{
	speaker->setTouchEnabled(true);
	AUDIO_ENGINE->playSound("sounds/mexicancactus/mc_release.mp3");
	auto cactus = static_cast<MexicanCactus_Item *> (sender);
	cactus->setScale(1.0f);
	cactus->setLocalZOrder(10);

	MexicanCactus_Item * d_cactus = nullptr;

	float max_distance = screen_size.width + screen_size.height;
	auto cactus_boundingbox = cactus->getBoundingBox();
	for (auto item : list_cactus_item) {
		if (item != cactus && item->getBoundingBox().intersectsRect(cactus_boundingbox)) {
			CCLOG("SCRAMBLE");
			auto distance = abs(map_index_items.find(cactus)->second - map_index_items.find(item)->second);//cactus->getPosition().distance(item->getPosition());
			if (distance < max_distance) {
				d_cactus = item;
				max_distance = distance;
			}
		}
	}
	if (d_cactus == nullptr) {
		inActiveMode();
		auto it = map_index_items.find(cactus);
		cactus->stopAllActions();
		cactus->runAction(cocos2d::MoveTo::create(0.25f, cactus_locations[it->second]));
	}
	else {
		onScrambleCactus(cactus, d_cactus);
		_flag = false;
	}
}

void MexicanCactus::onScrambleCactus(MexicanCactus_Item * s_cactus, MexicanCactus_Item * d_cactus)
{
	disableTouch();
	int s_index = map_index_items.find(s_cactus)->second;
	int d_index = map_index_items.find(d_cactus)->second;
	if (s_index > d_index) {
		for (auto item : list_cactus_item) {
			auto it = map_index_items.find(item);
			if (it != map_index_items.end() && it->second >= d_index && it->second < s_index) {
				it->second += 1;
			}
		}
	}
	else {
		for (auto item : list_cactus_item) {
			auto it = map_index_items.find(item);
			if (it != map_index_items.end() && it->second > s_index && it->second <= d_index) {
				it->second -= 1;
			}
		}
	}
	map_index_items.find(s_cactus)->second = d_index;
	float dt = 0.25f;
	for (auto item : list_cactus_item) {
		auto it = map_index_items.find(item);
		if (it != map_index_items.end()) {
			item->stopAllActions();
			item->runAction(Sequence::create(cocos2d::MoveTo::create(dt, cactus_locations[it->second]), CallFunc::create([=] {
			}), NULL));
		}
	}
	this->scheduleOnce([=](float t) {
		checkWinGame();
	}, dt, "check win game");
}

void MexicanCactus::showCactus() {
	CCLOG("Debug : Show Cactus");
	disableTouch();
	std::vector<std::string> chain;
	if(item_index == 0)
		chain.push_back(RandomHelper::random_int(0,1)?"sounds/mexicancactus/intro1.mp3": "sounds/mexicancactus/intro2.mp3");
	chain.push_back(current_item_info.normal_audio);
	AUDIO_ENGINE->playChain(chain, [=] {
		enableTouch();
		inActiveMode();
	});

	float dt = 0.25f;
	for (auto it = map_index_items.begin(); it != map_index_items.end(); ++it) {
		it->first->showCactus(dt);
	};
}

void MexicanCactus::createCactus()
{
	current_item_info = list_item_info[item_index];
	list_cactus_item.clear();
	int index = 0;

	// create cactus
	for (auto info : current_item_info.list_character) {
		MexicanCactus_Item::MexicanCactus_ItemInfo c_info;
		c_info.text = info.text;
		c_info.audio = info.audio;
		for (auto index : info.list_available_index) {
			c_info.correct_index.push_back(index);
		}
		MexicanCactus_Item * cactus = MexicanCactus_Item::createCactus(c_info, ++index);
		cactus->setDelegate(this);
		cactus->setCactusTouchable(true);
		root_layout->addChild(cactus, 10);
		list_cactus_item.push_back(cactus);
		_listCactusItemClone.push_back(cactus);
	}

	// random pos
	do {
		map_index_items.clear();
		mj::helper::shuffle(list_cactus_item);
		for (int i = 0; i < list_cactus_item.size(); ++i) {
			map_index_items.insert({ list_cactus_item[i],i });
		}
	} while (checkCorrectIndex());

	// set pos
	float remain_space = screen_size.width - list_cactus_item[0]->getContentSize().width * list_cactus_item.size();
	float padding = remain_space / list_cactus_item.size();
	if (padding > 100) {
		padding = 100;
	}
	if (padding < 0) {
		padding = 0;
	}
	Size cactus_size = list_cactus_item[0]->getContentSize();
	Vec2 middle_pos = Vec2(screen_size.width / 2, root_layout->getChildByName("rocks")->getPositionY() + cactus_size.height / 2);
	cactus_locations = getFrameLocations(list_cactus_item.size(), middle_pos, cactus_size, padding);

	for (auto it = map_index_items.begin(); it != map_index_items.end(); ++it) {
		it->first->setPosition(cactus_locations[it->second]);
	};

	text_hint->stopAllActions();
	text_hint->setString(current_item_info.text);
	text_hint->setOpacity(0);

	showCactus();
}

void MexicanCactus::changeInfoCactus()
{
	current_item_info = list_item_info[item_index];

	wrong_count = 0;
	text_hint->stopAllActions();
	text_hint->setString(current_item_info.text);
	text_hint->setOpacity(0);

	std::vector<MexicanCactus_Item::MexicanCactus_ItemInfo> list_info;
	for (auto info : current_item_info.list_character) {
		MexicanCactus_Item::MexicanCactus_ItemInfo c_info;
		c_info.text = info.text;
		c_info.audio = info.audio;
		for (auto index : info.list_available_index) {
			c_info.correct_index.push_back(index);
		}
		list_info.push_back(c_info);
	}
	do {
		mj::helper::shuffle(list_cactus_item);
		for (int i = 0; i < list_cactus_item.size(); ++i) {
			list_cactus_item[i]->setCactusInfo(list_info[i]);
		}
	} while (checkCorrectIndex());
}

void MexicanCactus::checkWinGame()
{
	CCLOG("Debug: Check win game");
	if (checkCorrectIndex()) {
		onMergeWords();
	}
	else {
		++wrong_count;
		inActiveMode();
		showHint();
		enableTouch();
	}
}

void MexicanCactus::onMergeWords()
{
	CCLOG("Debug: Merge Word");
	text_hint->setOpacity(0);
	initText();
	auto index_cmp = [=](MexicanCactus_Item * a, MexicanCactus_Item * b) {
		return map_index_items.find(a)->second < map_index_items.find(b)->second;
	};
	std::sort(list_cactus_item.begin(), list_cactus_item.end(), index_cmp);

	onMergeWord(0, [=] {
		this->scheduleOnce([=](float dt_) {
			playSyncText(0.5f);
		}, 0.75f, "play sync text");
	});
}

void MexicanCactus::onMergeWord(int index, std::function<void()> callback)
{
	if (index < list_cactus_item.size()) {
		auto phonic_info = current_item_info.list_character[index];
		auto locations = getFrameLocations(phonic_info.text.size(), list_cactus_item[index]->getLblText()->getWorldPosition(), cocos2d::Size(40, 60));
		int ind = 0;

		AUDIO_ENGINE->playSound(StringUtils::format("sounds/mexicancactus/mc_star_correct%d.mp3", (index + 1) % 4));

		auto gaf_star_color = GAFWrapper::createGAF("gaf/mexicancactus/stars colors/stars colors.gaf");
		gaf_star_color->setPosition(list_cactus_item[index]->getLblText()->getWorldPosition());
		this->addChild(gaf_star_color, 5);
		gaf_star_color->play(false, [=](gaf::GAFObject * obj) {
			gaf_star_color->removeFromParent();
		});

		this->scheduleOnce([=](float dt) {
			onMergeWord(index + 1, callback);
		}, 0.15f, cocos2d::StringUtils::format("merge word %d", index));
		for (int j = 0; j < phonic_info.text.size(); ++j) {
			if (phonic_info.text[j] != '_') {
				list_cactus_item[index]->getLblText()->setOpacity(0);
				auto text = cocos2d::ui::Text::create(phonic_info.text.substr(j, j + 1), font_name, 100);
				text->enableOutline(cocos2d::Color4B::BLACK, 3);
				text->setPosition(root_layout->convertToWorldSpace(locations[j]));
				root_layout->addChild(text, 20);
				auto letter = text_lb->getLetter(phonic_info.list_index[ind]);
				Vec2 move_location = text_lb->getPosition();
				if (letter) {
					move_location = text_lb->convertToWorldSpace(letter->getPosition());
				}
				text->runAction(Sequence::create(
					Spawn::create(ScaleTo::create(0.5f, 2.0f), MoveTo::create(0.5f, move_location), NULL),
					CallFunc::create([=] {
					if (letter) {
						letter->setVisible(true);
						letter->setOpacity(255);
					}
				}),
					RemoveSelf::create(), NULL));
				++ind;
			}
		}
	}
	else {
		if (callback) {
			callback();
		}
	}
}

void MexicanCactus::playSyncText(float delay_time)
{
	CCLOG("Debug: Play Sync Text ");
	disableTouch(0);
	this->scheduleOnce([=](float dt_) {
		float dt = 0.0f;

		for (int i = 0; i < current_item_info.list_character.size(); ++i) {
			this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
				AUDIO_ENGINE->playSound(current_item_info.list_character[i].audio);
				for (int index : current_item_info.list_character[i].list_index) {
					text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
						text_lb->getLetter(index)->setColor(Color3B::RED);
					}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
						text_lb->getLetter(index)->setColor(Color3B::WHITE);
					}), NULL));
				}
			}), NULL));
			dt += 0.65f;
		}
		this->scheduleOnce([=](float) {
			playNormalText();
		}, dt + 0.1f, "play sync text finnished");
	}, delay_time, "delay synctext");
}

void MexicanCactus::playSlowText()
{
	CCLOG("Debug: Play Slow Text");
	auto audio_path = std::string(current_item_info.slow_audio);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		this->setColorForText(Color3B::RED);
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		if (duration <= 0) {
			duration = 1.5f;
		}
		auto duration0 = duration * 0.3;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 20));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -20));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playNormalText();
		}), NULL));
	}), NULL));
}

void MexicanCactus::playNormalText()
{
	CCLOG("Debug: Play Normal Text");
	auto audio_id = AUDIO_ENGINE->play(current_item_info.normal_audio);
	this->setColorForText(Color3B::RED);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		if (duration <= 0) {
			duration = 0.5f;
		}
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), CallFunc::create([=] {
			//this->onNextWord();
			
			for (auto c : list_cactus_item) {
				c->onCorrectCactus(0.10f);
			}
			gaf_confetti->play(false, [=](gaf::GAFObject * obj) {
				gaf_confetti->reset();
			});
			AUDIO_ENGINE->playSound(StringUtils::format("sounds/mexicancactus/mc_correct%d.mp3", item_index % 2 + 1));

			this->scheduleOnce([=](float dt_) {
				onNextWord();
			}, 12.10f, "on next word");
		}), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), NULL));  //DelayTime::create(12.10f),
	}), NULL));
}

void MexicanCactus::onNextWord()
{
	CCLOG("Debug: Next Word");
	item_index++;
	disableTouch();
	if (item_index >= (int)list_item_info.size()) {
		this->onNextGame();
	}
	else {
		changeInfoCactus();
		showCactus();
	}
}

void MexicanCactus::onNextGame()
{
	CCLOG("Debug: Next Game");
	float dt = 0.15f;

	this->scheduleOnce([=](float dt_) {
		onFinishGame();
	}, dt, "end game");
}

void MexicanCactus::showHint()
{
	if (wrong_count > 2) {
		text_hint->stopAllActions();
		if (wrong_count == 3 || wrong_count == 4) {
			std::vector<std::string> chain;
			chain.push_back("sounds/mexicancactus/make the word.mp3");
			chain.push_back(current_item_info.normal_audio);
			AUDIO_ENGINE->playChain(chain);
		}
		else if (wrong_count == 5) {
			text_hint->runAction(Sequence::create(FadeIn::create(0.25f), DelayTime::create(1.5f), FadeOut::create(0.25f), NULL));
			AUDIO_ENGINE->playSound(current_item_info.slow_audio);
		}
		else {
			text_hint->runAction(FadeIn::create(0.25f));
		}
	}
}

inline void MexicanCactus::setColorForText(cocos2d::Color3B color_text)
{
	for (int i = 0; i < (int)text_lb->getString().size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}

void MexicanCactus::enableTouch()
{
	touch_layout->setVisible(false);
}

void MexicanCactus::disableTouch(int bg_opacity)
{
	touch_layout->setVisible(true);
	touch_layout->setBackGroundColorOpacity(bg_opacity);
}

void MexicanCactus::initText()
{
	text_lb->setString(current_item_info.text);
	text_lb->setScale(1.0f);
	text_lb->setPosition(Vec2(screen_size.width / 2, 100));

	for (int i = 0; i < text_lb->getStringLength(); ++i) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setOpacity(0);
		}
	}
}

void MexicanCactus::genCharacterIndex(MexicanCactus::MEXICAN_CACTUS_INFO & info)
{
	// Gen character index	
	int start_index = 0;
	std::queue<int> underline_index;
	for (int i = 0; i < info.list_character.size(); ++i) {
		auto phonic_data = info.list_character[i];
		for (int j = 0; j < phonic_data.text.size(); ++j) {
			if (phonic_data.text[j] == '_') {
				//underline_index
				underline_index.push(start_index + j);
			}
			else {
				if (underline_index.empty() || underline_index.front() > start_index) {
					info.list_character[i].list_index.push_back(start_index + j);
				}
				else {
					info.list_character[i].list_index.push_back(underline_index.front());
					underline_index.pop();
				}
			}
		}
		start_index += phonic_data.text.size();
	}
	for (int i = 0; i < info.list_character.size(); ++i) {
		for (int j = 0; j < info.list_character.size(); ++j) {
			if (info.list_character[i].text == info.list_character[j].text) {
				info.list_character[i].list_available_index.push_back(j);
			}
		}
	}
}

std::vector<Vec2> MexicanCactus::getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding)
{
	std::vector<Vec2> locations;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(center.x - (number - 1) * (size.width + padding) / 2, center.y);
	for (int i = 0; i < number; ++i) {
		Vec2 pos(start_pos.x + i * (size.width + padding), start_pos.y);
		locations.push_back(pos);
	}
	return locations;
}

bool MexicanCactus::checkCorrectIndex()
{
	bool resVal = true;
	for (auto cactus : list_cactus_item) {
		auto it = map_index_items.find(cactus);
		if (it != map_index_items.end()) {
			int d_index = it->second;
			bool found = false;
			for (auto index : cactus->getCactusInfo().correct_index) {
				if (index == d_index) {
					found = true;
					break;
				}
			}
			if (!found) {
				resVal = false;
				break;
			}
		}
	}
	return resVal;
}

void MexicanCactus::onClickedSpeaker(cocos2d::Ref * sender)
{
	skipAudioSpeaker();

	stopInActiveMode();

	std::vector<std::string> chain_sound;
	AUDIO_ENGINE->stopSound(current_item_info.normal_audio);
	chain_sound.push_back(current_item_info.normal_audio);

	std::vector<std::string> chain_sound2;
	for (auto character : current_item_info.list_character) {
		chain_sound2.push_back(character.audio);
		AUDIO_ENGINE->stopSound(character.audio);
	}
	AUDIO_ENGINE->playChain(chain_sound, [=] {
		_flagSpeaker = true;
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			if (_flagSpeaker) {
				AUDIO_ENGINE->playChain(chain_sound2, [=] {
					inActiveMode();
					});
			}
			}), NULL));
		});
}

void MexicanCactus::skipAudioSpeaker()
{
	AUDIO_ENGINE->stopSound(current_item_info.normal_audio);
	for (auto character : current_item_info.list_character) {
		AUDIO_ENGINE->stopSound(character.audio);
	}
	_flagSpeaker = false;
}

void MexicanCactus::onClickedSkip(cocos2d::Ref * sender)
{
	CCLOG(__FUNCTION__);
	btnSkip->setVisible(false);
	for (auto c : list_cactus_item) {
		c->resetCactus();
	}
	gaf_confetti->reset();
	AUDIO_ENGINE->stopSound("sounds/mexicancactus/mc_correct1.mp3");
	AUDIO_ENGINE->stopSound("sounds/mexicancactus/mc_correct2.mp3");
	text_lb->stopAllActions();
	text_lb->runAction(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))));
	this->unschedule("on next word");
	this->unschedule("hide skip");
	onNextWord();
}

void MexicanCactus::inActiveMode(){

	this->schedule([=](float) {
		if (_flag) {
			disableTouch();
			auto hand = GAFWrapper::createGAF("gaf/guidegame/hand_tap/hand_tap.gaf");
			hand->setName("hand");
			this->addChild(hand, 1900000000);

			if (list_cactus_item.size() > 2)
			{
				if (_listCactusItemClone[0] == list_cactus_item[0])
				{
					_startPos = list_cactus_item[list_cactus_item.size() - 1]->getPosition();
					_endPos = list_cactus_item[1]->getPosition();
				}
				else
				{
					for (auto item : list_cactus_item)
					{
						if (item == _listCactusItemClone[0])
						{
							_startPos = item->getPosition();
							_endPos = list_cactus_item[0]->getPosition();
						}
					}
				}
			}
			else
			{
				_startPos = list_cactus_item[list_cactus_item.size() - 1]->getPosition();
				_endPos = list_cactus_item[0]->getPosition();
			}
			
			hand->setPosition(_startPos);
			hand->play(false, [=](gaf::GAFObject* obj) {
				hand->runAction(Sequence::create(MoveTo::create(0.6f, _endPos),
					CallFunc::create([=]() {
						hand->play(false, [=](gaf::GAFObject* obj) {
							if (hand) {
								hand->removeFromParent();
							}
							});
						}), NULL));
				});
		}

		std::vector<std::string> chain_sound;
		chain_sound.push_back(current_item_info.normal_audio);
		
		std::vector<std::string> chain_sound2;
		for (auto character : current_item_info.list_character) {
			chain_sound2.push_back(character.audio);
		}
		AUDIO_ENGINE->playChain(chain_sound, [=] {
			this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
				if (_flagInActiveMove) {
					AUDIO_ENGINE->playChain(chain_sound2, [=] {
						enableTouch();
						});
				}
				}), NULL));
			});
		}, 12.5, 100, 10, "in_active_mode");
}

void MexicanCactus::stopInActiveMode() {
	_flagInActiveMove = false;
	this->runAction(Sequence::create(DelayTime::create(1.05f), CallFunc::create([=] {
		_flagInActiveMove = true;
		}), NULL));
	this->unschedule("in_active_mode");
	std::vector<std::string> chain_sound;
	chain_sound.push_back(current_item_info.normal_audio);
	AUDIO_ENGINE->stopSound(current_item_info.normal_audio);
	for (auto character : current_item_info.list_character) {
		chain_sound.push_back(character.audio);
		AUDIO_ENGINE->stopSound(character.audio);
	}
	if (utils::findChild(this, "hand")) {
		this->removeChildByName("hand");
	}
}
