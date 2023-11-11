#include "HSAudioEngine.h"
#include "SweetLand.h"

#include "JsonDump.h"
#include "MJHelper.h"

//sounds path
#define sl_path_sound_question "sounds/sweetland/how_many_sounds_are_there_in_the_word.mp3"
#define sl_path_sound_question1 "sounds/sweetland/sl_question1.mp3"
#define sl_path_sound_bgm "sounds/sweetland/sl_bgm.mp3"
#define sl_path_sound_interact "sounds/sweetland/sl_interact.mp3"
#define sl_path_sound_whistle_in "sounds/sweetland/sl_whistle_in.mp3"
#define sl_path_sound_whistle_out "sounds/sweetland/sl_whistle_out.mp3"
#define sl_path_sound_correct "sounds/sweetland/sl_correct.mp3"
#define sl_path_sound_press "sounds/sweetland/sl_press.mp3"
#define sl_path_sound_wrong "sounds/sweetland/sl_wrong.mp3"
#define sl_path_sound_footstep "sounds/sweetland/sl_footstep.mp3"
#define sl_path_sound_candy_drop "sounds/sweetland/sl_candydrop%d.mp3"
#define sl_path_sound_Nat_nice "sounds/sweetland/Nat_voice_nice.mp3"
#define sl_path_sound_Nat_oops "sounds/sweetland/Nat_voice_oops.mp3"
#define sl_path_sound_Nat_yay "sounds/sweetland/Nat_voice_yay.mp3"
#define speakr_audio "sounds/sweetland/How many sounds are there in the word....mp3"
#define bg_game "games/sweetland/sweetland.png"
#define music_game_game "sounds/sweetland/Sweet_land.mp3"

SweetLand * SweetLand::createGame(std::string json_file, bool isAutoPlay) {
	auto p = new SweetLand();
	if (p->init()) {
        p->autorelease();
		p->didLoadFromCSB(json_file, isAutoPlay);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void SweetLand::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	so_luot_choi = document.FindMember("game_mode")->value.FindMember("number_turnplay")->value.GetInt();
	is_syllable = false;
	if(document.FindMember("game_mode")->value.HasMember("is_syllable") )
		is_syllable = document.FindMember("game_mode")->value.FindMember("is_syllable")->value.GetBool();
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();
	autoPlay = isAutoPlay;

	this->loadFrames();
}

void SweetLand::generateData(rapidjson::Value value_data) {

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	data_game.clear();
	for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
		auto letter = value_data[i].FindMember("text")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);

		word tmp;
		tmp.text = pImage->getText("name_1");
		tmp.result = value_data[i].FindMember("number_syllable")->value.GetInt();
		tmp.audio_path = pImage->getAudios("name_1").front();
		tmp.audio_slow_path = pImage->getAudios("name_n").front();
		tmp.phonics = pImage->getTextPhonic("name_p");
		tmp.phonic_audios = pImage->getAudioPhonic("name_p");
		data_game.push_back(tmp);
	}
	mj::helper::shuffle(data_game);
}


void SweetLand::didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay) {
	screen_size = Director::getInstance()->getVisibleSize();
	autoPlay = isAutoPlay;
	this->loadFrames();
}

void SweetLand::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	playBackgroundMusic(sl_path_sound_bgm);
	if (autoPlay) {
		this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=] {
			this->onAutoPlay();
		}), NULL));

	}
}

void SweetLand::loadFrames() {
	screen_size = cocos2d::Director::getInstance()->getWinSize();

	_gafPlace = 100;

	current_index = 0;
	root_layout = cocos2d::ui::Layout::create();
	root_layout->setContentSize(screen_size);
	root_layout->setTouchEnabled(true);
	addChild(root_layout);

	auto background = cocos2d::ui::ImageView::create("games/sweetland/sl_bg.jpg");
	background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	background->setPosition(Vec2(screen_size.width / 2, 0));
	root_layout->addChild(background);

	gaf_Nat = GAFWrapper::createGAF("gaf/sweetland/sl_Nat/sl_Nat.gaf");
	gaf_Nat->setVisible(true);
	gaf_Nat->setPosition(Vec2(screen_size.width / 2, 0) + Vec2(155, 310));
	root_layout->addChild(gaf_Nat, 100);

	back_layout = cocos2d::ui::Layout::create();
	back_layout->setContentSize(screen_size);
	back_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	back_layout->setBackGroundColorOpacity(255);
	back_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	back_layout->setOpacity(0);
	root_layout->addChild(back_layout, 1);

	text_lb = cocos2d::ui::Text::create(" ", font_name, font_sz);
	text_lb->setTextColor(Color4B::BLACK);
	text_lb->enableOutline(Color4B::WHITE, 6);
	text_lb->setOpacity(0);
	text_lb->removeFromParentAndCleanup(false);
	text_lb->setPosition(Vec2(screen_size / 2));
	this->addChild(text_lb, 501);

	m_speaker = cocos2d::ui::Button::create("games/sweetland/loa icon.png");
	m_speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_speaker->setPosition(Vec2(50, screen_size.height - 50));
	root_layout->addChild(m_speaker, 200);
	m_speaker->setTouchEnabled(false);
	m_speaker->addClickEventListener(CC_CALLBACK_1(SweetLand::onClickedSpeaker, this));

	auto visibleSize = Director::getInstance()->getVisibleSize();

	if (FileUtils::getInstance()->isFileExist(bg_game)) {
		_backgoundGame = Sprite::create(bg_game);
		_backgoundGame->setPosition(Vec2(visibleSize / 2));
		_backgoundGame->setName("bg_game");
		_backgoundGame->setScale(2.5);
		this->addChild(_backgoundGame, INT_MAX - 1);
	}

	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(music_game_game);
		}), NULL));

	this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
		if (_backgoundGame) {
			_backgoundGame->runAction(FadeOut::create(1.0f));
		}
		}), DelayTime::create(1.5f), CallFunc::create([=] {
			this->showIntroGame();
			}), NULL));
}

void SweetLand::showGame() {

	AUDIO_ENGINE->playSound(sl_path_sound_bgm);

	count_fail = 0;
	current_data = data_game[current_index];

	if (current_index == 0) {
		std::string sound_question = is_syllable ? sl_path_sound_question1 : sl_path_sound_question;
		AUDIO_ENGINE->playSound(sound_question, false, [=] {
			AUDIO_ENGINE->playSound(current_data.audio_path, false, [=] {
				this->showQuestion();
				gafLeapOrRun();
				inActiveMode();
				showHint();
				m_speaker->setTouchEnabled(true);
				});
		});
	}
	else {
		AUDIO_ENGINE->playSound(current_data.audio_path, false, [=] {
			this->showQuestion();
			inActiveMode();
			gafLeapOrRun();
			_flag = true;
			m_speaker->setTouchEnabled(true);
			showHint();
			});
	}

}

void SweetLand::gafRun(cocos2d::Vec2 pos) {
	gaf_Nat->playSequence("run_pre", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
		gaf_Nat->playSequence("run", true);
		});

	bool isFlipped = (pos.x > gaf_Nat->getPositionX() + (gaf_Nat->isFlippedX() ? -200 : 0));

	if (gaf_Nat->isFlippedX() != isFlipped) {
		gaf_Nat->setFlippedX(isFlipped);
		float fixed = isFlipped ? 200 : -200;
		gaf_Nat->setPositionX(gaf_Nat->getPositionX() + fixed);
	}

	auto scale_factor = 1024.0f / screen_size.width;

	float distance = Vec2(gaf_Nat->getPositionX(), gaf_Nat->getPositionY()).distance(pos);

	auto time = distance * scale_factor / 300;

	auto actions = Sequence::create(MoveTo::create(time, Vec2(pos.x, 310)), CallFunc::create([=] {
		gaf_Nat->playSequence("run_stop", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
			});
		}), NULL);
	actions->setTag(999999);
	gaf_Nat->runAction(actions);
}

void SweetLand::gafLeap() {
	gaf_Nat->playSequence("cheer_pre", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
		gaf_Nat->playSequence("cheer", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
			gaf_Nat->playSequence("cheer", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
				gaf_Nat->playSequence("cheer_post", false);
				});
			});
		});
}

void SweetLand::gafLeapOrRun() {
	this->schedule([=](float) {
		auto action = Sequence::create(CallFunc::create([=] {
			gafLeap();
			}), DelayTime::create(7.0f), CallFunc::create([=] {
				if (_flag) {
					gafRun(Vec2(_gafPlace, 310));
					if (_gafPlace == 100) {
						_gafPlace = screen_size.width - 100;
					}
					else
					{
						_gafPlace = 100;
					}
				}
				}), NULL);
		this->runAction(action);
		}, 17, 100, 7, "gaf_leap_or_run");
}

void SweetLand::stopGafLeapOrRun() {
	this->unschedule("gaf_leap_or_run");
	gaf_Nat->playSequence("run_stop", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
		});
	gaf_Nat->stopActionByTag(999999);
	_flag = false;
}

void SweetLand::showQuestion() {
	// create text
	text_lb->setString(current_data.text);
	text_lb->setOpacity(0);
	// create item
	current_result = current_data.result;
	vector<int>tmp = { current_result - 2, current_result - 1,current_result + 1, current_result + 2 };
	std::vector<int>::iterator it = std::remove_if(tmp.begin(), tmp.end(), [](int a)->bool {
		return a <= 0;
	});
	tmp.erase(it, tmp.end());
	if (tmp.size() == 2) {
		tmp.push_back(current_result + 3);
	}
	tmp.resize(3);
	tmp.push_back(current_result);
	mj::helper::shuffle(tmp);
	items.clear();

	auto locations = getFrameLocations(tmp.size(), Vec2(screen_size.width / 2, 100), Size(180, 150), 20);
	for (int i = 0; i < 4; i++) {
		auto item = SweetLand_Item::createItem(tmp[i], i);
		item->setPosition(locations[i]);
		root_layout->addChild(item, 100);
		item->setDelegate(this);
		item->setTouchOnOff(true);
		items.push_back(item);
		item->onShow();
	}
}

void SweetLand::onExitTransitionDidStart() {
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void SweetLand::onClickedDelegate(cocos2d::Ref *sender) {
	m_speaker->setTouchEnabled(false);
	stopGafLeapOrRun();
	stopAudioGuideGameAndSpeaker();
	this->unschedule("animated_guide");
	AUDIO_ENGINE->playSound(sl_path_sound_interact);
	SweetLand_Item* item_choosed = static_cast<SweetLand_Item*>(sender);

	bool correct = (current_result == item_choosed->getNumber());

	for (auto i : items) {
		i->setTouchOnOff(false);
	}

	runNatTo(item_choosed->getPosition(), 600.0f, [=] {
		item_choosed->scheduleOnce([=](float dt) {
			item_choosed->playEffect(correct);
			if (correct) {
				AUDIO_ENGINE->playSound(sl_path_sound_correct);
			}
			else {
				AUDIO_ENGINE->playSound(sl_path_sound_wrong);
			}
		}, 6.0f / 32, "delay item pressed");
		AUDIO_ENGINE->playSound(sl_path_sound_press);
		gaf_Nat->playSequence("press", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			playNatEffect(correct);
			if (correct) {
				addSweetToLand();
				showTextEndQuestion();
				m_speaker->setTouchEnabled(false);
			}
			else {
				m_speaker->setTouchEnabled(true);
				gafLeapOrRun();
				this->unschedule("animated_guide");
				inActiveMode();
				_flag = true;
				for (auto i : items) {
					i->setTouchOnOff(true);
				}
			}
		});
	});
}

std::vector<Vec2> SweetLand::getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding)
{
	std::vector<Vec2> locations;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(center.x - (number - 1) * (size.width + padding) / 2, center.y);
	for (int i = 0; i < number; ++i) {
		Vec2 pos(start_pos.x + i * (size.width + padding), start_pos.y);
		locations.push_back(pos);
	}
	return locations;
}

void SweetLand::showTextEndQuestion() {

	auto start_pos = Vec2(screen_size.width / 2, screen_size.height + 100);
	auto text_pos = Vec2(screen_size.width / 2, 600);

	text_lb->stopAllActions();
	text_lb->setPosition(text_pos);
	text_lb->setOpacity(255);
	text_lb->setScale(0, 1.0f);
	text_lb->runAction(cocos2d::Sequence::create(
		ScaleTo::create(0.5f, 1.0f, 1.0f),
		DelayTime::create(0.75f),
		CallFunc::create([=] {
		playSyncText();
	}), NULL));
}

void SweetLand::runNatTo(cocos2d::Vec2 pos, float speed, std::function<void()> call_back)
{
	AUDIO_ENGINE->playSound(sl_path_sound_footstep, true);
	gaf_Nat->playSequence("run_pre", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
		gaf_Nat->playSequence("run", true);
	});

	bool isFlipped = (pos.x > gaf_Nat->getPositionX() + (gaf_Nat->isFlippedX() ? -200 : 0));
	if (gaf_Nat->isFlippedX() != isFlipped) {
		gaf_Nat->setFlippedX(isFlipped);
		float fixed = isFlipped ? 200 : -200;
		gaf_Nat->setPositionX(gaf_Nat->getPositionX() + fixed);
	}
	auto scale_factor = 1024.0f / screen_size.width;
	float distance = Vec2(gaf_Nat->getPositionX(), gaf_Nat->getPositionY()).distance(pos);
	if (distance < 250.0f) {
		if (call_back) {
			AUDIO_ENGINE->stopSound(sl_path_sound_footstep);
			call_back();
		}
		return;
	}
	auto time = distance * scale_factor / speed;
	gaf_Nat->runAction(Sequence::create(MoveTo::create(time, Vec2(pos.x, 310)), CallFunc::create([=] {
		gaf_Nat->playSequence("run_stop", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			if (call_back) {
				AUDIO_ENGINE->stopSound(sl_path_sound_footstep);
				call_back();
			}
		});
	}), NULL));
}

void SweetLand::playNatEffect(bool correct)
{
	if (correct) {
		if (count_fail == 0) {
			AUDIO_ENGINE->playSound(sl_path_sound_Nat_yay);
		}
		else {
			AUDIO_ENGINE->playSound(sl_path_sound_Nat_nice);
		}
		gaf_Nat->playSequence("cheer_pre", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			gaf_Nat->playSequence("cheer", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
				gaf_Nat->playSequence("cheer", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
					gaf_Nat->playSequence("cheer_post", false);
				});
			});
		});
	}
	else {
		AUDIO_ENGINE->playSound(sl_path_sound_Nat_oops);
		gaf_Nat->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
		});
	}
}

std::vector<cocos2d::Vec2> SweetLand::randomPositionsInRect(cocos2d::Size item_size, cocos2d::Rect box_rect, int number_item)
{
	std::vector<cocos2d::Vec2> list;

	float delta = 1.4;
	int count_rand = 0;

	for (int i = 0; i < number_item; i++) {
		if (count_rand >= 10) {
			count_rand = 0;
			delta -= 0.1;
		}
		cocos2d::Vec2 point;
		bool avail = true;
		int count = 0;
		do {
			count++;
			if (count > 50) {
				break;
			}

			avail = true;
			int rand_x = cocos2d::random(0, (int)(box_rect.size.width - item_size.width)) + (int)item_size.width*0.6;
			int rand_y = cocos2d::random(0, (int)(box_rect.size.height - item_size.height)) + (int)item_size.height*0.6;
			point = cocos2d::Vec2(rand_x + box_rect.origin.x, rand_y + box_rect.origin.y);

			for (cocos2d::Vec2 _point : list) {
				if (point.distance(_point) < item_size.width*delta) {
					avail = false;
					break;
				}
			}
		} while (!avail);

		if (count > 50) {
			i = -1;
			list.clear();
			count_rand++;
			continue;
		}
		else {
			list.push_back(point);
		}
	}
	return list;
}

std::vector<int> SweetLand::randomArrayIndexes(int number, int size)
{
	std::vector<int> datas;
	for (int i = 0; i < size; i++) {
		datas.push_back(i);
	}
	mj::helper::shuffle(datas);
	if (number == size) {
		return datas;
	}

	std::vector<int> results;
	for (int i = 0; i < number; i++) {
		results.push_back(datas[i]);
	}
	return results;
}

void SweetLand::onNextStage() {
	this->unscheduleAllCallbacks();

}

void SweetLand::onFinishGame() {
	this->unschedule("animated_guide");
	stopGafLeapOrRun();
	AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onFinishGame(/*CHARACTER::NAT*/);
}

void SweetLand::nextGame() {
	this->unscheduleAllCallbacks();
    current_index++;
	if (current_index > so_luot_choi - 1) {
		this->onFinishGame();
	}
	else {
		for (auto i : items) {
			i->onHide(0);
		}
		//text_lb->runAction(ScaleTo::create(0.25f, 0, 1.0f));
		this->scheduleOnce([=](float dt) {
			this->showGame();
		}, 0.5f, "show next game");

	}
}

void SweetLand::showGamename()
{
	auto game_name = cocos2d::ui::Text::create("Sweet Land", font_name, 100);
	game_name->enableOutline(Color4B::BLACK, 3);
	game_name->setPosition(screen_size / 2);
	root_layout->addChild(game_name, 100);
	game_name->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(0.5f), FadeOut::create(0.25f), CallFunc::create([=] {
		showIntroGame();
	}), NULL));
}

void SweetLand::showIntroGame()
{
	AUDIO_ENGINE->playSound(sl_path_sound_whistle_in);
	gaf_Nat->playSequence("fadein_under", false, [=](gaf::GAFObject* obj, const std::string &sqN) {
		AUDIO_ENGINE->playSound(sl_path_sound_whistle_out);
		AUDIO_ENGINE->playSound(sl_path_sound_footstep, true);
		back_layout->runAction(FadeTo::create(34.0f / 32, 0.3f * 256));
		gaf_Nat->playSequence("fadein_over", false, [=](gaf::GAFObject* obj, const std::string &sqN) {
			AUDIO_ENGINE->stopSound(sl_path_sound_footstep);
			showGame();
		});
	});
}

void SweetLand::inActiveMode() {
    this->schedule([=](float) {
		AUDIO_ENGINE->playSound(speakr_audio, false, [=] {
			AUDIO_ENGINE->playEffect(current_data.audio_path);
			});
	}, 15, 100, 11, "animated_guide");
}

void SweetLand::stopAudioGuideGameAndSpeaker() {
	AUDIO_ENGINE->stopSound(speakr_audio);
	AUDIO_ENGINE->stopSound(current_data.audio_path);
}

void SweetLand::addSweetToLand()
{
	// random number sweet : max 35
	float top_sweet = cocos2d::random(13, 25);
	float bot_sweet = cocos2d::random(5, 10);
	auto top_locations = randomPositionsInRect(Size(60, 40), Rect(0, 130, screen_size.width, 100), top_sweet);
	auto bot_locations = randomPositionsInRect(Size(60, 40), Rect(0, -75, screen_size.width, 100), bot_sweet);

	std::vector<Vec2> locations;

	locations.insert(locations.end(), top_locations.begin(), top_locations.end());
	locations.insert(locations.end(), bot_locations.begin(), bot_locations.end());

	mj::helper::shuffle(locations);
	auto indexes = randomArrayIndexes(locations.size(), 35);
	float dt = 0;
	int index = 0;
	for (auto loc : locations) {
		auto sweet = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("games/sweetland/candy-%02d.png", indexes[index++]));
		sweet->setPosition(Vec2(loc.x, screen_size.height + 200));
		int localZ;
		if (loc.y <= 75) {
			localZ = 101 + (int)(75 - loc.y) / 2;
		}
		else {
			localZ = 99 + (int)(loc.y - 230) / 2;
		}
		root_layout->addChild(sweet, localZ);
		float scale = 0.5f + (75 - loc.y) / 700;
		sweet->setScale(scale);
		auto width = cocos2d::random(-60, 60);
		auto jumps = cocos2d::random(1, 3);
		sweet->runAction(Sequence::create(DelayTime::create(dt), EaseBounceOut::create(MoveTo::create(0.25f, loc)), JumpBy::create(0.12f, Vec2(width, 0), 30, jumps), NULL));
		dt += 0.05f;
	}
	this->scheduleOnce([](float dt) {
		AUDIO_ENGINE->playSound(StringUtils::format(sl_path_sound_candy_drop, cocos2d::random(1, 4)));
	}, 0.25f, "play sound sweetdrop");
}

void SweetLand::playSound(cocos2d::Ref* sender) {

}

void SweetLand::playSyncText()
{
	_char_filled.clear();
	_char_filled.resize(current_data.text.size(), false);
	float dt = 0;
	for (int i = 0; i < current_data.phonics.size(); ++i) {
		this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->playSound(current_data.phonic_audios[i]);
		}), NULL));
		auto phonic = current_data.phonics[i];
		for (auto u = 0; u < (int)phonic.size(); ++u) {
			for (auto i = 0; i < (int)_char_filled.size(); ++i) if (!_char_filled[i] && current_data.text[i] == phonic[u]) {
				_char_filled[i] = true;
				auto char_img = text_lb->getLetter(i);
				if (char_img) {
					char_img->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
						text_lb->getLetter(i)->setColor(Color3B::RED);
					}), ScaleTo::create(0.25f, 1, 1.2f), ScaleTo::create(0.25f, 1, 1), CallFunc::create([=] {
						text_lb->getLetter(i)->setColor(Color3B::WHITE);
					}), NULL));
				}
				break;
			}
		}
		dt += 0.6;
	}
	dt += 0.25f;
	this->scheduleOnce([=](float delay) {
		playSlowText();
	}, dt, "playSlowText");
}

void SweetLand::playSlowText()
{
	auto audio_path = std::string(current_data.audio_slow_path);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		//this->setColorForText(Color3B::RED);
		this->setColorForText(Color3B::WHITE);
		text_lb->setTextColor(cocos2d::Color4B::RED);
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
			//this->setColorForText(Color3B::WHITE);
			text_lb->setTextColor(cocos2d::Color4B::BLACK);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playNormalText();
		}), NULL));
	}), NULL));
}

void SweetLand::playNormalText()
{
	auto audio_id = AUDIO_ENGINE->play(current_data.audio_path);
	//this->setColorForText(Color3B::RED);
	this->setColorForText(Color3B::WHITE);
	text_lb->setTextColor(cocos2d::Color4B::RED);
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
			//this->setColorForText(Color3B::WHITE);
			text_lb->setTextColor(cocos2d::Color4B::BLACK);
		}), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), NULL));

		this->scheduleOnce([=](float delay) {
			nextGame();
		}, (duration + duration0 + 0.25f), "onResetRound");
	}), NULL));
}

inline void SweetLand::setColorForText(cocos2d::Color3B color_text)
{
	for (int i = 0; i < (int)text_lb->getString().size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}

void SweetLand::showHint()
{
	text_lb->setPosition(Vec2(screen_size.width / 2, 600));
	text_lb->setOpacity(255);
	text_lb->setScale(0.0f, 1.0f);
	text_lb->setVisible(true);
	text_lb->runAction(Sequence::create(ScaleTo::create(0.25f, 1.0f, 1.0f), NULL));
}

void SweetLand::onClickedSpeaker(cocos2d::Ref * sender)
{
	stopAudioGuideGameAndSpeaker();
	AUDIO_ENGINE->playSound(speakr_audio, false, [=] {
		AUDIO_ENGINE->stopSound(current_data.audio_path);
		AUDIO_ENGINE->playSound(current_data.audio_path);
		this->unschedule("animated_guide");
		inActiveMode();
		});
}

#define auto_testing_delay 1.5f
void SweetLand::onAutoPlay()
{
	this->scheduleOnce([=](float dt) {
		this->autoPlayRun(1.5f);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void SweetLand::autoPlayRun(float dt)
{
	this->scheduleOnce([=](float dt) {
		this->onAutoPlay();
	}, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}


