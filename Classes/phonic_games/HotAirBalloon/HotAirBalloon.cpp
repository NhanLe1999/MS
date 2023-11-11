#include "HSAudioEngine.h"
#include "HotAirBalloon.h"

#include "JsonDump.h"
#include "MJHelper.h"

//gaf path

//sounds path
#define HAB_SOUND_INTRO_SYLLABLE "sounds/hotairballoon/hab_question1_old.mp3"
#define HAB_SOUND_INTRO_WORD "sounds/hotairballoon/hab_question2.mp3"
#define HAB_SOUND_QUESTION "sounds/hotairballoon/hab_question%d.mp3"
#define HAB_SOUND_BGM "sounds/hotairballoon/hab_bgm.mp3"
#define HAB_SOUND_TAP "sounds/hotairballoon/hab_tap.mp3"
#define HAB_SOUND_WATERFALL_AMB "sounds/hotairballoon/hab_waterfall amb.mp3"
#define HAB_SOUND_WRONG "sounds/hotairballoon/hab_wrong.mp3"
#define HAB_SOUND_CORRECT "sounds/hotairballoon/hab_correct.mp3"


#define HAB_BALLOON_START_POS cocos2d::Vec2(Director::getInstance()->getWinSize().width/2 - 362,275)


HotAirBalloon * HotAirBalloon::createGame(std::string json_file, bool isAutoPlay) {
	auto p = new HotAirBalloon();
	if (p->init()) {
        p->autorelease();
		p->didLoadFromCSB(json_file, isAutoPlay);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void HotAirBalloon::didLoadFromCSB(std::string json_file, bool isAutoPlay) {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	number_turnplay = document.FindMember("game_mode")->value.FindMember("number_turnplay")->value.GetInt();
	is_syllable_game = document.FindMember("game_mode")->value.FindMember("is_syllable")->value.GetBool();
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	number_turnplay = min(number_turnplay, (int)data_game.size());
	autoPlay = isAutoPlay;

	PHONIC_MANAGER->cacheGAF("hab_cloud","gaf/hotairballoon/hab_cloud/hab_cloud.gaf");
	PHONIC_MANAGER->cacheGAF("hab_rainbow","gaf/hotairballoon/hab_rainbow/hab_rainbow.gaf");
	PHONIC_MANAGER->cacheGAF("hab_lightning_r1","gaf/hotairballoon/hab_lightning_r1/hab_lightning_r1.gaf");
	PHONIC_MANAGER->cacheGAF("hab_lightning_r2","gaf/hotairballoon/hab_lightning_r2/hab_lightning_r2.gaf");
	PHONIC_MANAGER->cacheGAF("hab_lightning_r3","gaf/hotairballoon/hab_lightning_r3/hab_lightning_r3.gaf");

	this->loadFrames();


}

void HotAirBalloon::generateData(rapidjson::Value value_data) {

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	data_game.clear();
	for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
		auto letter = value_data[i].FindMember("text")->value.GetString();
		auto pImage = math::resource::ResHelper::getImage(images, letter);

		HAB_INFO tmp;
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


void HotAirBalloon::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	playBackgroundMusic(HAB_SOUND_BGM);
	AUDIO_ENGINE->playSound(HAB_SOUND_WATERFALL_AMB, false, [=] {
		AUDIO_ENGINE->playSound(HAB_SOUND_WATERFALL_AMB, true, nullptr, 0.5f);
	});
	if (autoPlay) {
		this->runAction(Sequence::create(DelayTime::create(4.0), CallFunc::create([=] {
			this->onAutoPlay();
		}), NULL));
	}
	showIntroGame();
}

void HotAirBalloon::loadFrames() {
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	current_index = 0;

	root_layout = cocos2d::ui::Layout::create();
	root_layout->setContentSize(screen_size);
	root_layout->setTouchEnabled(true);
	addChild(root_layout);

	// gui
	auto background = cocos2d::ui::ImageView::create("games/hotairballoon/hab_bg.jpg");
	background->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	background->setPosition(Vec2(screen_size / 2));
	root_layout->addChild(background);

	mountain_layout = cocos2d::ui::Layout::create();
	mountain_layout->setContentSize(screen_size);
	root_layout->addChild(mountain_layout, 10);

	auto hab_moutainB = cocos2d::ui::ImageView::create("games/hotairballoon/hab_mountainB.png");
	hab_moutainB->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	hab_moutainB->setPosition(Vec2(screen_size.width / 2 - 200, -300));
	mountain_layout->addChild(hab_moutainB, 1);

	auto hab_moutainF = cocos2d::ui::ImageView::create("games/hotairballoon/hab_mountainF.png");
	hab_moutainF->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	hab_moutainF->setPosition(Vec2(screen_size.width / 2 + 200, -300));
	mountain_layout->addChild(hab_moutainF, 2);

	gaf_waterfall = GAFWrapper::createGAF("gaf/hotairballoon/hab_waterfall/hab_waterfall.gaf");
	gaf_waterfall->setPosition(Vec2(125, screen_size.height / 2));
	gaf_waterfall->play(true);
	root_layout->addChild(gaf_waterfall, 19);

	cloud_layout = cocos2d::ui::Layout::create();
	cloud_layout->setContentSize(screen_size);
	cloud_layout->setTouchEnabled(false);
	cloud_layout->setPosition(Vec2(0, -500));
	root_layout->addChild(cloud_layout, 20);

	auto hab_cloudB = cocos2d::ui::ImageView::create("games/hotairballoon/hab_CloudB.png");
	hab_cloudB->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	hab_cloudB->setPosition(Vec2(screen_size.width / 2, 0));
	cloud_layout->addChild(hab_cloudB, 1);

	auto hab_cloudF = cocos2d::ui::ImageView::create("games/hotairballoon/hab_CloudF.png");
	hab_cloudF->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	hab_cloudF->setPosition(Vec2(screen_size.width / 2, 0));
	cloud_layout->addChild(hab_cloudF, 2);

	gaf_balloon = GAFWrapper::createGAF("gaf/hotairballoon/hab_balloon/hab_balloon.gaf");
	gaf_balloon->setVisible(true);
	gaf_balloon->setPosition(Vec2(-200, -200));
	root_layout->addChild(gaf_balloon, 11);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setBackGroundColor(cocos2d::Color3B::GRAY);
	touch_layout->setBackGroundColorOpacity(200);
	touch_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	touch_layout->setVisible(false);
    touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 500);

	text_lb = cocos2d::ui::Text::create(" ", font_name, font_sz);
	text_lb->setTextColor(Color4B::BLACK);
	text_lb->enableOutline(Color4B::WHITE, 5);
	text_lb->setOpacity(0);
	text_lb->removeFromParentAndCleanup(false);
	text_lb->setPosition(Vec2(screen_size / 2));
	this->addChild(text_lb, 50);

	gaf_effect = GAFWrapper::createGAF("gaf/hotairballoon/hab_effect/hab_effect.gaf");
	root_layout->addChild(gaf_effect, 100);
	gaf_effect->setVisible(false);

	gaf_effectlight1 = GAFWrapper::createGAF("gaf/hotairballoon/hab_lightningeffect1/hab_lightningeffect1.gaf");
	root_layout->addChild(gaf_effectlight1, 100);
	gaf_effectlight1->setVisible(false);

	gaf_effectlight2 = GAFWrapper::createGAF("gaf/hotairballoon/hab_lightningeffect2/hab_lightningeffect2.gaf");
	root_layout->addChild(gaf_effectlight2, 100);
	gaf_effectlight2->setVisible(false);

	speaker = cocos2d::ui::Button::create("games/hotairballoon/loa icon.png");
	speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	speaker->setPosition(Vec2(50, screen_size.height - 50));
	root_layout->addChild(speaker, 200);
	speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(HotAirBalloon::onClickedSpeaker, this));
}

void HotAirBalloon::showGamename()
{
	auto game_name = cocos2d::ui::Text::create("Hot Air Balloon", font_name, 100);
	game_name->enableOutline(Color4B::BLACK, 3);
	game_name->setPosition(screen_size / 2);
	root_layout->addChild(game_name, 100);

	game_name->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(0.5f), FadeOut::create(0.25f), CallFunc::create([=] {
		showIntroGame();
	}), NULL));
}


void HotAirBalloon::showIntroGame()
{
	if (is_syllable_game) {
		AUDIO_ENGINE->playSound(HAB_SOUND_INTRO_SYLLABLE);
	}
	else {
		AUDIO_ENGINE->playSound(HAB_SOUND_INTRO_WORD);
	}
	gaf_balloon->playSequence("fade_in", false);
	mountain_layout->setScale(1.5f);
	runBalloonTo(Vec2(screen_size.width / 2, 600), 3.0f, [=] {
		idleBalloon(100.0f);
		gaf_waterfall->runAction(MoveTo::create(1.5f, Vec2(screen_size.width / 2 - 930.5f, screen_size.height / 2)));
		gaf_balloon->runAction(
			Sequence::create(DelayTime::create(1.5f), 
			CallFunc::create([=] {
				gaf_balloon->setLocalZOrder(30);
				mountain_layout->runAction(ScaleTo::create(1.0f,1.0f));
				cloud_layout->runAction(MoveBy::create(1.0f, Vec2(0, 250)));
				runBalloonTo(HAB_BALLOON_START_POS, 1.0f, [=] {
					idleBalloon(100000.0f);
				});
				gaf_balloon->runAction(ScaleTo::create(1.0f, 0.5f));
				showGame();
		}), NULL));
	});

}
void HotAirBalloon::showGame() {
	count_fail = 0;
	current_data = data_game[current_index];
	std::vector<std::string> chain;
	chain.push_back(cocos2d::StringUtils::format(HAB_SOUND_QUESTION,3));
	chain.push_back(current_data.audio_path);
	AUDIO_ENGINE->playChain(chain, [=] {

	});
	this->showQuestion();
}

void HotAirBalloon::showQuestion() {
	// create text

	text_lb->setString(current_data.text);
	text_lb->setOpacity(0);
	// create item
	current_result = current_data.result;
	vector<int> tmp = {current_result - 3,current_result - 2, current_result + 2,current_result - 1,current_result + 1 };
	std::vector<int>::iterator it = std::remove_if(tmp.begin(), tmp.end(), [](int a)->bool {
		return a <= 0;
	});
	tmp.erase(it, tmp.end());
	while (tmp.size() <= 3) {
		tmp.push_back(current_result + tmp.size() + 1);
	}
	mj::helper::shuffle(tmp);
	tmp.resize(3);
	tmp.push_back(current_result);
	mj::helper::shuffle(tmp);

	//auto start_locations = getFrameLocations(Vec2(screen_size.width + 150, screen_size.height), Size(240, 160), 4, 1, Vec2(0, 100), Vec2(-50, 0));
	//auto locations = getFrameLocations(Vec2(screen_size.width / 2 + 250, screen_size.height / 2), Size(240, 160), 4, 1, Vec2(0, 20), Vec2(-50, 0));	
	
	auto start_locations = getArcLocations(4, HAB_BALLOON_START_POS + Vec2(0,-175), 54, screen_size.width + 240 - HAB_BALLOON_START_POS.x);
	auto locations = getArcLocations(4, HAB_BALLOON_START_POS + Vec2(0,-175), 54, screen_size.width/2 + 362 - HAB_BALLOON_START_POS.x);

	items.clear();
	float duration = 1.0f;
	for (int i = 0; i < 4; i++) {
		auto item = HotAirBalloon_Item::createItem(tmp[i], i);
		item->setPosition(start_locations[i]);
		item->runAction(MoveTo::create(duration, locations[i]));
		root_layout->addChild(item, 50);
		item->setDelegate(this);
		item->setTouchOnOff(true);
		item->setUserData(&locations[i]);
		items.push_back(item);
		item->onShow(duration);
	}
	this->scheduleOnce([=](float dt) {
		enableTouch();
		if(current_index == 0) 
			inactiveMode(10.0f);
	}, duration, "start game");

}


void HotAirBalloon::onExitTransitionDidStart() {
	PHONIC_MANAGER->clearCacheGaf();
	MJMainGBase::onExitTransitionDidStart();
}


void HotAirBalloon::onClickedDelegate(cocos2d::Ref *sender) {
	unInactiveMode();
	AUDIO_ENGINE->playEffect(HAB_SOUND_TAP);
	HotAirBalloon_Item* item_choosed = static_cast<HotAirBalloon_Item*>(sender);

	bool correct = (current_result == item_choosed->getNumber());
	disableTouch();
	item_choosed->setLocalZOrder(51);
	item_choosed->playEffect(correct);

	playBalloonEffect(correct);
	if (correct) {

		for (auto item : items) {
//            item->playEffect(current_result == item->getNumber());
            item->playEffect(correct, current_result == item->getNumber());
		}
		scheduleOnce([=](float dt) {
			AUDIO_ENGINE->playSound(HAB_SOUND_CORRECT);
		}, 1.0f, "play sound correct");
		//this->scheduleOnce([=](float dt) {
		//	
		//}, 52.0f / 32, "correct ");

		showTextEndQuestion();
	}
	else {
//        for (auto item : items) {
//            item->setTouchEnabled(false);
//        }
		++count_fail;
		attackBalloon(item_choosed, [=] {
			AUDIO_ENGINE->playEffect(HAB_SOUND_WRONG);
			runBalloonTo(HAB_BALLOON_START_POS, 0.25f, [=] {
				float interval = 0.3f;
				auto shake = Sequence::create(
					RotateTo::create(interval / 4, -2),
					RotateTo::create(interval / 2, 2),
					RotateTo::create(interval / 4, 0), NULL);

				gaf_balloon->runAction(Sequence::create(Repeat::create(shake, 3), CallFunc::create([=] {
					idleBalloon(100000.0f);
					enableTouch();
				}), NULL));

			});
			item_choosed->setLocalZOrder(50);
			if (count_fail == 2) {
				showHint(1.5f);
			}
		});

	}



}


std::vector<Vec2> HotAirBalloon::getArcLocations(int number, cocos2d::Vec2 central_point, float  arcInDegrees, float radius)
{
	std::vector<Vec2> locations;
	float arc_seg = arcInDegrees / (number - 1);
	for (int i = 0; i < number; ++i) {
		float x = central_point.x + std::cos(CC_DEGREES_TO_RADIANS(i * arc_seg)) * radius;
		float y = central_point.y + std::sin(CC_DEGREES_TO_RADIANS(i * arc_seg)) * radius;
		locations.push_back(Vec2(x, y));
	}
	return locations;
}

std::vector<cocos2d::Vec2> HotAirBalloon::getFrameLocations(cocos2d::Vec2 mid_point, cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding, cocos2d::Vec2 skew)
{
	std::vector<cocos2d::Vec2> locations;
	int col = max_col;
	int row = total_element / max_col;
	if (row * col < total_element) ++row;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(mid_point.x - (col - 1) * (element_size.width + padding.x) * 0.5f - (row - 1) * skew.x / 2, mid_point.y - (row - 1) * (element_size.height + padding.y) / 2 - (col - 1) * skew.y / 2);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			cocos2d::Vec2 pos(start_pos.x + j * (element_size.width + padding.x) + i * skew.x, start_pos.y + i * (element_size.height + padding.y) + j * skew.y);
			locations.push_back(pos);
		}
	}
	return locations;
}

void HotAirBalloon::showTextEndQuestion() {

	auto start_pos = HAB_BALLOON_START_POS + Vec2(text_lb->getContentSize().width / 2 + 150, -175);
	auto text_pos = Vec2(screen_size.width / 2, 600);
	// back layout

	//back_layout->setVisible(true);
	text_lb->stopAllActions();
	text_lb->setPosition(start_pos);
	text_lb->setOpacity(255);
	text_lb->setScale(0, 1.0f);
	text_lb->runAction(cocos2d::Sequence::create(
		ScaleTo::create(0.5f, 1.0f),
		DelayTime::create(0.15f),
		JumpTo::create(0.5f, text_pos, 100, 1),
		DelayTime::create(1.0f),
		CallFunc::create([=] {
		playSyncText();
	}), NULL));
}

void HotAirBalloon::runBalloonTo(cocos2d::Vec2 pos, float duration, std::function<void()> call_back)
{
	gaf_balloon->stopAllActions();
	gaf_balloon->runAction(Sequence::create(
		MoveTo::create(duration, Vec2(pos)),
		CallFunc::create([=] {
		if (call_back) {
			call_back();
		}
	}), NULL));
}

void HotAirBalloon::idleBalloon(float idle_time, float idle_interval)
{
	// move up down
	//int times = idle_time / idle_interval;
	//auto amp = 25;
	//auto idle_action = Sequence::create(MoveBy::create(idle_interval, Vec2(0, amp)), MoveBy::create(idle_interval, Vec2(0, -amp)), NULL);
	//gaf_balloon->stopAllActions();
	//gaf_balloon->runAction(Repeat::create(idle_action, times));

	// move by circle
	int times = idle_time / idle_interval;
	int seg = 25;
	float duration = idle_interval / seg;
	auto balloon_pos = gaf_balloon->getPosition() + Vec2(-5,0);
	auto locs = getArcLocations(seg, balloon_pos, 360.0f,5);
	locs.pop_back();
	Vector<FiniteTimeAction *> idle;
	for (int i = locs.size() - 1; i >= 0; --i) {
		auto action = MoveTo::create(duration, locs[i]);
		idle.pushBack(action);
	}
	gaf_balloon->stopAllActions();
	gaf_balloon->runAction(Repeat::create(Sequence::create(idle), times));
}

void HotAirBalloon::attackBalloon(HotAirBalloon_Item * item, std::function<void()> call_back)
{
	Vec2 mid_point = gaf_balloon->getPosition() + ( item->getPosition() - gaf_balloon->getPosition()  ) / 2;
	float padding = cocos2d::random(10, 20) * 10;
	auto locations = getFrameLocations(mid_point, Size(240, 160), 9, 1, Vec2(0, padding), Vec2(-50, 0));
	int offset = 0;
	for (int i = 0; i < items.size(); ++i) {
		if (item == items[i]) {
			offset = 4 - i;
		}
	}

	runBalloonTo(mid_point + Vec2(-100, -50), 0.5f, [=] {

		gaf_effect->setVisible(true);
		gaf_effect->setPosition(locations[4]);
		gaf_effect->play(false, [=](gaf::GAFObject * obj) {
			gaf_effect->setVisible(false);
		});

		gaf_effectlight1->setVisible(true);
		gaf_effectlight1->setPosition(locations[4] + Vec2(-100, 0));
		gaf_effectlight1->play(false, [=](gaf::GAFObject * obj) {
			gaf_effectlight1->setVisible(false);
		});

		gaf_effectlight2->setVisible(true);
		gaf_effectlight2->setPosition(locations[4] + Vec2(-150, -200));
		gaf_effectlight2->play(false, [=](gaf::GAFObject * obj) {
			gaf_effectlight2->setVisible(false);
		});

		if (call_back) {
			call_back();
		}
	});
	for (int i = 0; i < 4; i++) {
		auto item = items[i];
		auto start_pos = item->getPosition();
		item->runAction(Sequence::create(MoveTo::create(0.5f, locations[i + offset] + Vec2(120, 0)), MoveTo::create(0.3f, start_pos), NULL));
	}
}

void HotAirBalloon::transitionQuestion(std::function<void()> call_back)
{
	HotAirBalloon_Item * correct_item;
	int offset;
	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->getNumber() == current_result) {
			offset = 4 - i;
			correct_item = items[i];
			break;
		}
	}
	Vec2 mid_point = getPointParallel(correct_item->getPosition(), gaf_balloon->getPosition() + Vec2(0, 150), -300);
	float padding = cocos2d::random(30, 38) * 10;
	auto locations = getFrameLocations(mid_point, Size(240, 160), 9, 1, Vec2(0, padding), Vec2(-50, 0));

	for (int i = 0; i < 4; i++) {
		auto item = items[i];
		auto start_pos = item->getPosition();
		item->runAction(Sequence::create(MoveTo::create(1.5f, locations[i + offset]), RemoveSelf::create(), NULL));
	}
	scheduleOnce([=](float dt) {
		if (call_back) {
			call_back();
		}
	}, 1.5f, "transition balloon callback");

	mountain_layout->runAction(MoveBy::create(1.5f, Vec2(-25, -25)));
	cloud_layout->runAction(MoveBy::create(1.5f, Vec2(-10, -10)));
}

inline Vec2 HotAirBalloon::getPointParallel(Vec2 A, Vec2 B, float xC)
{
	float k = (B.x - A.x) / (xC - A.x);
	float yC = (B.y - A.y) / k + A.y;
	return Vec2(xC, yC);
}

void HotAirBalloon::playBalloonEffect(bool correct)
{
	if (correct) {
		gaf_balloon->playSequence("correct", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			gaf_balloon->playSequence("idle", true);
		});
	}
	else {
		gaf_balloon->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			gaf_balloon->playSequence("idle", true);
		});
	}
}

void HotAirBalloon::onNextStage() {
	this->unscheduleAllCallbacks();

}

void HotAirBalloon::onFinishGame() {


	AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onFinishGame(/*MJMainGBase::CHARACTER::DAN*/);


}

void HotAirBalloon::nextGame() {
	this->unscheduleAllCallbacks();
	current_index++;
	if (current_index > number_turnplay - 1) {
		transitionQuestion([=] {
			this->onFinishGame();
		});
	}
	else {
		transitionQuestion();
		//text_lb->runAction(ScaleTo::create(0.25f, 0, 1.0f));
		this->scheduleOnce([=](float dt) {
			this->showGame();
		}, 0.5f, "show next game");

	}
}



void HotAirBalloon::inactiveMode(float delay_time) {
	
	if (items.size() > 0) {
		for (auto item : items) {
			if (current_result == item->getNumber()) {
				showTapGuideGame(item->getPosition(), delay_time);
				break;
			}
		}
	}
}

void HotAirBalloon::unInactiveMode()
{
	hideTapGuideGame();
}

void HotAirBalloon::onClickedSpeaker(cocos2d::Ref* sender) {
	AUDIO_ENGINE->playSound(current_data.audio_path);
}

void HotAirBalloon::playSyncText()
{
	_char_filled.clear();
	_char_filled.resize(current_data.text.size(), false);
	float dt = 0;
	for (int i = 0; i < current_data.phonics.size(); ++i) {
		this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->play(current_data.phonic_audios[i]);
		}), NULL));
		auto phonic = current_data.phonics[i];
		for (auto u = 0; u < (int)phonic.size(); ++u) {
			for (auto i = 0; i < (int)_char_filled.size(); ++i) if (!_char_filled[i] && current_data.text[i] == phonic[u]) {
				_char_filled[i] = true;
				auto char_img = text_lb->getLetter(i);
				if (char_img) {
					char_img->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
						char_img->setColor(Color3B::RED);
					}), ScaleTo::create(0.25f, 1, 1.2f), ScaleTo::create(0.25f, 1, 1), CallFunc::create([=] {
						char_img->setColor(Color3B::WHITE);
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

void HotAirBalloon::playSlowText()
{
	auto audio_path = std::string(data_game[current_index].audio_slow_path);
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

void HotAirBalloon::playNormalText()
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
		}, (duration + duration0 + 1.0f), "onResetRound");
	}), NULL));
}

inline void HotAirBalloon::setColorForText(cocos2d::Color3B color_text)
{
	for (int i = 0; i < (int)text_lb->getString().size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}

void HotAirBalloon::showHint(float duration)
{
	auto text_pos = HAB_BALLOON_START_POS + Vec2(text_lb->getContentSize().width / 2 + 150, -175);
	text_lb->setPosition(text_pos);
	text_lb->setOpacity(255);
	text_lb->setScale(0.0f, 1.0f);
	text_lb->setVisible(true);
	AUDIO_ENGINE->playChain(current_data.phonic_audios);
	text_lb->runAction(Sequence::create(ScaleTo::create(0.25f, 1.0f, 1.0f), CallFunc::create([=] {
		
	}), DelayTime::create(duration), ScaleTo::create(0.25f, 0, 1.0f), NULL));
}

void HotAirBalloon::enableTouch()
{
	touch_layout->setVisible(false);
}

void HotAirBalloon::disableTouch(int opacity)
{
	touch_layout->setVisible(true);
	touch_layout->setOpacity(10);
}

#define auto_testing_delay 1.5f
void HotAirBalloon::onAutoPlay()
{
	this->scheduleOnce([=](float dt) {
		this->autoPlayRun(1.5f);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void HotAirBalloon::autoPlayRun(float dt)
{
	this->scheduleOnce([=](float dt) {
		this->onAutoPlay();
	}, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}


