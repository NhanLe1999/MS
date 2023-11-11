//
//  GameNoi.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//	Cloned by MinhTzy on 29/05/2019

#include "GameNoi.h"
#include "GameNoiFrameTop.h"
#include "GameNoiFrameBottom.h"
#include "GameInstructionOverlay.h"

INITIALIZE_READER(GameNoi);

USING_NS_CC;

#define CSB_NAME  "csb/game/vm_game_noi/game_noi.csb"
void GameNoi::initReader() {
}

std::once_flag game_noi_reader;


mj::GameDataSet<mj::LazerGameDataSet> GameNoi::generateData(rapidjson::Value value_data, std::string resource) {
	auto images = math::resource::ResHelper::getImages("icons.json");

	mj::GameDataSet<mj::LazerGameDataSet> data;

	auto game_id = value_data["game_id"].GetInt();

	std::vector<mj::GameObject> list_keywords;

	auto arrObjects = value_data["text_info"].GetArray();
	for (int i = 0; i < (int)arrObjects.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			resource + img_nature->path,
			resource + img_nature->getAudio()
		};
		list_keywords.push_back(game_object);
	}


	mj::helper::shuffle(list_keywords);

	while (!list_keywords.empty()) {
		mj::LazerGameDataSet set;
		if (game_id == mj::GameType::PLUG_IN_BULBS_TI) {
			set.type = mj::LazerGameType::LAZER_IMAGE_TEXT;
		}
		if (game_id == mj::GameType::PLUG_IN_BULBS_TT) {
			set.type = mj::LazerGameType::LAZER_TEXT_TEXT;
		}

		for (auto i = 0; i < std::min((int)list_keywords.size(), 4); i++) {
			set.top.push_back(list_keywords[i]);
		}

		set.bottom = set.top;

		list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.top.size());
		data.sets.push(set);
		break;
	}


	return data;
}

mj::GameDataSet<mj::LazerGameDataSet> GameNoi::generateData(cocos2d::ValueMap value_map, std::string resource) {
	mj::GameDataSet<mj::LazerGameDataSet> data;

	auto gamedict = value_map["game"].asValueMap();

	auto game_id = gamedict["id"].asInt();


	std::vector<mj::GameObject> list_keywords;

	auto arrObjects = value_map["text_info"].asValueVector();
	for (auto it : arrObjects) {
		auto obj = it.asValueMap();
		mj::GameObject game_object = {
			obj["text"].asString(),
			resource + obj["img_path"].asString(),
			resource + obj["audio_path"].asString()
		};
		list_keywords.push_back(game_object);
		//        set.top.push_back(game_object);
	}

	mj::helper::shuffle(list_keywords);

	while (!list_keywords.empty()) {
		mj::LazerGameDataSet set;
		if (game_id == mj::GameType::PLUG_IN_BULBS_TI) {
			set.type = mj::LazerGameType::LAZER_IMAGE_TEXT;
		}
		if (game_id == mj::GameType::PLUG_IN_BULBS_TT) {
			set.type = mj::LazerGameType::LAZER_TEXT_TEXT;
		}

		for (auto i = 0; i < std::min((int)list_keywords.size(), 4); i++) {
			set.top.push_back(list_keywords[i]);
		}

		set.bottom = set.top;

		list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.top.size());
		data.sets.push(set);
		break;
	}

	//    if (set.top.size() > 4) {
	//        //trim to 4
	//        mj::helper::shuffle(set.top);
	//        set.top.erase(set.top.begin() + 4, set.top.end());
	//    }
	//
	//    set.bottom = set.top;

	return data;
}

mj::GameDataSet<mj::LazerGameDataSet> GameNoi::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{

	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

	mj::GameDataSet<mj::LazerGameDataSet> data;

	auto game_id = value_data["game_id"].GetInt();

	auto audio_intro = value_data["audio_intro"].GetArray();
	for (int i = 0; i < audio_intro.Size(); ++i) {
		auto img_intro = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
		mj::GameObject game_object = {
			img_intro->getQuestionVMonkey("name_1",lang),
			img_intro->path,
			img_intro->getAudios("name_1",lang).front()
		};
		data.info.instructions.push_back(game_object);
	}


	auto arrObjects = value_data["text_info"].GetArray();

	// Game image - image
	if (game_id == 0) {
		mj::LazerGameDataSet set;
		set.type = mj::LazerGameType::LAZER_IMAGE_IMAGE;

		for (int i = 0; i <(int)arrObjects.Size(); i++) {
			auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
			int pair = img_nature->getComparisionValue("pair");
			int role = img_nature->getComparisionValue("role");
			mj::GameObject game_object = {
				StringUtils::toString(img_nature->getComparisionValue("pair")),
				img_nature->path,
				img_nature->getAudios("name_1",lang).front()
			};
			if (role == 0) {
				set.top.push_back(game_object);
			}
			else {
				set.bottom.push_back(game_object);
			}
		}
		data.sets.push(set);
		mj::helper::shuffle(set.top);
		mj::helper::shuffle(set.bottom);
	}
	else {
		std::vector<mj::GameObject> list_keywords;
		for (int i = 0; i < (int)arrObjects.Size(); i++) {
			auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
			mj::GameObject game_object = {
				img_nature->getText("name_1",lang),
				img_nature->path,
				img_nature->getAudios("name_1",lang).front()
			};
			list_keywords.push_back(game_object);
		}
		mj::helper::shuffle(list_keywords);

		while (!list_keywords.empty()) {
			mj::LazerGameDataSet set;
			if (game_id == mj::GameType::PLUG_IN_BULBS_TI) {
				set.type = mj::LazerGameType::LAZER_IMAGE_TEXT;
			}
			if (game_id == mj::GameType::PLUG_IN_BULBS_TT) {
				set.type = mj::LazerGameType::LAZER_TEXT_TEXT;
			}

			for (auto i = 0; i < std::min((int)list_keywords.size(), 4); i++) {
				set.top.push_back(list_keywords[i]);
			}

			set.bottom = set.top;

			list_keywords.erase(list_keywords.begin(), list_keywords.begin() + set.top.size());
			data.sets.push(set);
			break;
		}
	}
	return data;
}

GameNoi * GameNoi::createGame(mj::GameDataSet<mj::LazerGameDataSet> data) {
	std::call_once(game_noi_reader, [] {
		REGISTER_CSB_READER(GameNoi);
		REGISTER_CSB_READER(GameNoiFrameTop);
		REGISTER_CSB_READER(GameNoiFrameBottom);
	});
	auto p = static_cast<GameNoi *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(data);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

GameNoi * GameNoi::createGame(std::string configPath)
{
	std::call_once(game_noi_reader, [] {
		REGISTER_CSB_READER(GameNoi);
		REGISTER_CSB_READER(GameNoiFrameTop);
		REGISTER_CSB_READER(GameNoiFrameBottom);
	});
	auto p = static_cast<GameNoi *>(CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(configPath);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

ui::Widget::ccWidgetClickCallback GameNoi::onLocateClickCallback(const std::string &callback_name) {
	if (callback_name == "onRefresh") {
		return CC_CALLBACK_1(GameNoi::onRefresh, this);
	}

	return nullptr;
}

void GameNoi::onRefresh(cocos2d::Ref *sender) {
}

#include "MJDefault.h"
#define key_count_guide_game_noi "key_count_guide_game_noi"

void GameNoi::didLoadFromCSB(std::string configPath)
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(configPath);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	auto data = generateData(document.GetObject(), images);

	didLoadFromCSB(data);
}

void GameNoi::didLoadFromCSB(mj::GameDataSet<mj::LazerGameDataSet> data) {
	_data = data;
	ui::Helper::doLayout(this);

	auto count = MJDEFAULT->getIntegerForKey(key_count_guide_game_noi, 0);
	_is_guide_showed = false ;//!(count < 3);
	this->scheduleOnce([=](float) {
		MJDEFAULT->setIntegerForKey(key_count_guide_game_noi, count + 1);
	}, 20, "delay_update_count_guide_game_noi");
}

void GameNoi::onEnter() {
	MJMainGBase::onEnter();
	loadFrames();

	if (this->loadQuestion()) {
		this->doAppear();
	}

//    MJGameInstructionOverlay::playInstructionAfterDelay(/*3*/0.5f, mj::GameType::PLUG_IN_BULBS_TI, sound_question);

    q_id = AUDIO_ENGINE->playSound(sound_question);
}

bool GameNoi::loadQuestion() {
	if (_data.sets.empty()) {
		return false;
	}
	
	_current_set = _data.sets.front();
	_bottom_frames.clear();
	_top_frames.clear();
	_total_connected = 0;

	if (!_data.info.instructions.empty()) {
		lbl_question->setString(_data.info.instructions.front().text);
		sound_question = _data.info.instructions.front().audio_path;
	}
    
	auto question_size = Size(600,120);
	auto label_size = lbl_question->getContentSize();
    while (label_size.height > question_size.height) {
        font_sz = lbl_question->getFontSize();
        lbl_question->setFontSize(font_sz - 2);
		label_size = lbl_question->getContentSize();
        CCLOG("Scale down font: %d", font_sz);
    }

	mj::helper::shuffle(_current_set.top);
	mj::helper::shuffle(_current_set.bottom);

	auto draw = DrawNode::create(15.f);
	draw->setName("draw");
	draw->setLocalZOrder(5);
	this->getChildByName("background")->addChild(draw);

	auto top_layout = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "top_frames"));
	top_layout->removeAllChildren();

	const int margin = 35;
	auto width = 0;
	for (auto f : _current_set.top) {
		auto frame = GameNoiFrameTop::createFrame(f, _current_set.type == mj::LazerGameType::LAZER_IMAGE_IMAGE || _current_set.type == mj::LazerGameType::LAZER_IMAGE_TEXT);
		frame->setName(f.text);
		frame->setDelegate(this);
		frame->setPosition(Point(width, 0));
		top_layout->addChild(frame);
		width += frame->getContentSize().width + margin;
		_top_frames.push_back(frame);
	}

	width -= margin;
	top_layout->setContentSize(Size(width, top_layout->getContentSize().height));

	auto bottom_layout = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "bottom_frames"));
	bottom_layout->removeAllChildren();

	width = 0;
	for (auto f : _current_set.bottom) {
		auto frame = GameNoiFrameBottom::createFrame(f, _current_set.type == mj::LazerGameType::LAZER_IMAGE_IMAGE || _current_set.type == mj::LazerGameType::LAZER_TEXT_IMAGE);
		frame->setName(f.text);
		frame->setPosition(Point(width, 0));
		bottom_layout->addChild(frame);

		width += frame->getContentSize().width + margin;

		_bottom_frames.push_back(frame);
	}

	width -= margin;
	bottom_layout->setContentSize(Size(width, bottom_layout->getContentSize().height));

	ui::Helper::doLayout(this);
	return true;
}

void GameNoi::doAppear() {
	auto timeline = CSLoader::createTimeline(CSB_NAME);
	timeline->play("appear", false);
	this->runAction(timeline);

	if (!_is_guide_showed) {
		_is_guide_showed = true;
		Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
		this->unschedule("show_guide_bulbs_1");
		this->unschedule("delay_show_guide_bulbs_1");
		this->scheduleOnce([=](float) {
			Point start, stop;
			auto t = _top_frames.at(random(0, (int)_top_frames.size() - 1));
			for (auto b : _bottom_frames) {
				if (b->getName() == t->getName()) {
					auto start_bb = t->getBoundingBox();
					start = Point(start_bb.getMidX(), start_bb.getMidY());
					start = t->getParent()->convertToWorldSpace(start) - Point(0, 125);

					auto stop_bb = b->getBoundingBox();
					stop = Point(stop_bb.getMidX(), stop_bb.getMidY());
					stop = b->getParent()->convertToWorldSpace(stop) + Point(0, 125);
					break;
				}
			}

			this->schedule([this, start, stop](float) {
				this->showGuide(start, stop);
			}, 5, 100, 10, "show_guide_bulbs_1");
		}, 2, "delay_show_guide_bulbs_1");
	}
}

void GameNoi::onPlugBeginning(GameNoiFrameTop * frame) {
    AUDIO_ENGINE->stopEffect(q_id);
}

void GameNoi::onPlugMoving(GameNoiFrameTop * frame, Point position) {
}

void GameNoi::onPlugMoved(GameNoiFrameTop * frame, Point position) {
	for (auto f : _bottom_frames) {
		auto bounding_box = f->getBoundingBox();
		bounding_box.size.height += 100;
		bounding_box.origin = f->getParent()->convertToWorldSpace(bounding_box.origin);

		if (bounding_box.containsPoint(position)) if (f->getName() == frame->getName()) {
			_is_guide_showed = true;
			Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
			this->unschedule("show_guide_bulbs_1");
			this->unschedule("delay_show_guide_bulbs_1");

			_total_connected++;
			frame->connectToBottom(f);
			/*if (!game_play_enable) {
				if (_total_connected >= 2) {
					this->pauseGame();
					return;
				}
			}*/
			if (_total_connected == _current_set.top.size()) {
				this->scheduleOnce([=](float dt) {
					this->doDisappear();
				}, 2, "next_sets");
			}
			return;
		}
	}

	frame->movePlugBack();
}

void GameNoi::doDisappear() {
	_data.sets.pop();
	auto timeline = CSLoader::createTimeline(CSB_NAME);
	timeline->play("disappear", false);
	this->runAction(timeline);
	timeline->setLastFrameCallFunc([=]() {
		if (this->loadQuestion()) {
			this->doAppear();
		}
		else {
			MJMainGBase::onFinishGameVMonkey();
		}
	});
}

void GameNoi::loadFrames()
{
	lbl_question = utils::findChild<ui::Text*>(this, "lbl_question");
	btn_speaker = utils::findChild<ui::Button*>(this, "btn_speaker");
	auto bg_ques = utils::findChild<ui::ImageView*>(this, "bg_ques");
	btn_speaker->addClickEventListener(CC_CALLBACK_1(GameNoi::onClickedSpeaker, this));
	bg_ques->addClickEventListener(CC_CALLBACK_1(GameNoi::onClickedSpeaker, this));
    lbl_question->setTextAreaSize(Size(600,0));
    lbl_question->ignoreContentAdaptWithSize(true);

	lbl_question->setFontName("fonts/Montserrat-Medium.ttf");
	
}

void GameNoi::showGuide(Point start, Point stop) {
	Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");

	auto overlay = ui::Layout::create();
	overlay->setContentSize(Director::getInstance()->getVisibleSize());
	overlay->setName("guide_overlay");
	Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);

	auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
	overlay->addChild(hand, INT_MAX - 1);
	hand->setPosition(start - Point(0, 25));

	hand->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(2, stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));
}

void GameNoi::onClickedSpeaker(cocos2d::Ref * sender)
{
    AUDIO_ENGINE->stopSound(sound_question);
	q_id = AUDIO_ENGINE->playSound(sound_question);
}
