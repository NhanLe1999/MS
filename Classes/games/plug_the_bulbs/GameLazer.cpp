//
//  GameLazer.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//

#include "GameLazer.h"
#include "GameLazerFrameTop.h"
#include "GameLazerFrameBottom.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameLazer);

USING_NS_CC;

void MJGameLazer::initReader() {
}

MJGameLazer::~MJGameLazer()
{
	MJDEFAULT->deleteValueForKey("show_hand_suggetion");
	MJDEFAULT->deleteValueForKey("key_play_audio_gameLazer");
}

std::once_flag plug_in_bulbs_reader;


mj::GameDataSet<mj::LazerGameDataSet> MJGameLazer::generateData(rapidjson::Value value_data, std::string resource) {
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

mj::GameDataSet<mj::LazerGameDataSet> MJGameLazer::generateData(cocos2d::ValueMap value_map, std::string resource) {
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

mj::GameDataSet<mj::LazerGameDataSet> MJGameLazer::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	mj::GameDataSet<mj::LazerGameDataSet> data;

	rapidjson::Value arrObjects;

	int gameId;
	std::string pathAudioIntro = "";

	if (JSON_SAFE_CHECK(value_data, "text_info", Array))
	{
		arrObjects = value_data["text_info"].GetArray();
	}

	if (JSON_SAFE_CHECK(value_data, "game_id", Int))
	{
		gameId = value_data["game_id"].GetInt();
	}

	if (JSON_SAFE_CHECK(value_data, "audio_intro", String))
	{
		auto key_audio = value_data["audio_intro"].GetString();
		auto img = math::resource::ResHelper::getImage(images, key_audio);
		pathAudioIntro = img->getAudios().front();
	}

	// Game image - image

	std::vector<mj::GameObject> list_keywords;
	for (int i = 0; i < (int)arrObjects.Size(); i++) {
		auto img_nature = math::resource::ResHelper::getImage(images, arrObjects[i].GetString());
		mj::GameObject game_object = {
			img_nature->getText(),
			img_nature->path,
			img_nature->getAudio()
		};
		list_keywords.push_back(game_object);
	}
	mj::helper::shuffle(list_keywords);

		while (!list_keywords.empty()) {
			mj::LazerGameDataSet set;
			if (gameId == mj::GameType::PLUG_IN_BULBS_TI) {
				set.type = mj::LazerGameType::LAZER_IMAGE_TEXT;
			}
			if (gameId == mj::GameType::PLUG_IN_BULBS_TT) {
				set.type = mj::LazerGameType::LAZER_TEXT_TEXT;
			}
		if (gameId == 0) {
			set.type = mj::LazerGameType::LAZER_IMAGE_IMAGE;
		}

			set.audioIntroGame = pathAudioIntro;

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

MJGameLazer * MJGameLazer::createGame(mj::GameDataSet<mj::LazerGameDataSet> data) {
	std::call_once(plug_in_bulbs_reader, [] {
		REGISTER_CSB_READER(MJGameLazer);
		REGISTER_CSB_READER(MJGameLazerFrameTop);
		REGISTER_CSB_READER(MJGameLazerFrameBottom);
	});
	auto p = static_cast<MJGameLazer *>(CSLoader::createNodeWithVisibleSize("csb/game/lazer/game_lazer.csb"));
	if (p) {
		p->didLoadFromCSB(data);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

ui::Widget::ccWidgetClickCallback MJGameLazer::onLocateClickCallback(const std::string &callback_name) {
	if (callback_name == "onRefresh") {
		return CC_CALLBACK_1(MJGameLazer::onRefresh, this);
	}

	return nullptr;
}

void MJGameLazer::onRefresh(cocos2d::Ref *sender) {
}

#include "MJDefault.h"
#define key_count_guide_plug_bulbs_1 "key_count_guide_plug_bulbs_1"

void MJGameLazer::didLoadFromCSB(mj::GameDataSet<mj::LazerGameDataSet> data) {
	_data = data;
	ui::Helper::doLayout(this);

	MJDEFAULT->setBoolForKey("key_play_audio_gameLazer", true);

	auto count = MJDEFAULT->getIntegerForKey(key_count_guide_plug_bulbs_1, 0);
	_is_guide_showed = false;
	this->scheduleOnce([=](float) {
		MJDEFAULT->setIntegerForKey(key_count_guide_plug_bulbs_1, count + 1);
	}, 20, "delay_update_count_guid_plug_bulbs_1");
    setCallbackCickSpeak();
    setCallbackPlayAudio();
}

void MJGameLazer::onEnter() {
	MJGameBase::onEnter();
	_callbackAddSpeakAndShowGuide = [this]() {
		handSuggetionGame();
		addSpeaker();
	};

	MJDEFAULT->setStringForKey("key.stop.audio.intro", "No");
    
	scheduleOnce([=](float dt) {
		/*timeline->play("logo", false);
		timeline->setLastFrameCallFunc([=]() {*/
			if (this->loadQuestion()) {
				this->doAppear();
			}
		//});
		//        this->runAction(timeline);
			if (_pathAudioIntro == "" || _pathAudioIntro == "null")
			{
				if (_data.info.instructions.size() == 0) {
					_pathAudioIntro = mj::INSTRUCTION_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
				}
				else {
					_pathAudioIntro = _data.info.instructions[0].audio_path;
				}
			}
			MJGameInstructionOverlay::playInstructionAfterDelay(/*3*/_time_delay, mj::GameType::PLUG_IN_BULBS_TI, _pathAudioIntro, "", nullptr, [this]() {
					if (_callbackAddSpeakAndShowGuide)
					{
						_callbackAddSpeakAndShowGuide();
					}
				});
	}, _time_delay + 1.f, "delay_onenter");
}

void MJGameLazer::onExit()
{
	MJDEFAULT->setStringForKey("key.stop.audio.intro", "Yes");
    this->unschedule("show_guide_bulbs_1");
	_callbackAddSpeakAndShowGuide = nullptr;
    _callbackCickSpeak = nullptr;
    _callbackPlayAudio = nullptr;
	MJGameBase::onExit();
}

bool MJGameLazer::loadQuestion() {
	if (_data.sets.empty()) {
		return false;
	}

	_current_set = _data.sets.front();
	_pathAudioIntro = _current_set.audioIntroGame;
	_bottom_frames.clear();
	_top_frames.clear();
	_total_connected = 0;

	mj::helper::shuffle(_current_set.top);
	mj::helper::shuffle(_current_set.bottom);

	auto draw = DrawNode::create(15.f);
	draw->setName("draw");
	draw->setLocalZOrder(5);
	this->getChildByName("background")->addChild(draw);

	auto top_layout = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "top_frames"));
	top_layout->removeAllChildren();

	const int margin = 35;
	bool isTextLength = false;
	for (int i = 0; i < _current_set.top.size(); i++)
	{
		if (_current_set.top[i].text.length() > 7)
		{
			isTextLength = true;
			break;
		}
	}

	for (int i = 0; i < _current_set.bottom.size(); i++)
	{
		if (_current_set.bottom[i].text.length() > 7)
		{
			isTextLength = true;
			break;
		}
	}
	auto width = 0;
	for (auto f : _current_set.top) {
		auto frame = MJGameLazerFrameTop::createFrame(f, _current_set.type == mj::LazerGameType::LAZER_IMAGE_IMAGE || _current_set.type == mj::LazerGameType::LAZER_IMAGE_TEXT, isTextLength);
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
		auto frame = MJGameLazerFrameBottom::createFrame(f, _current_set.type == mj::LazerGameType::LAZER_IMAGE_IMAGE || _current_set.type == mj::LazerGameType::LAZER_TEXT_IMAGE, isTextLength);
		frame->setName(f.text);
        frame->setAudio(f.audio_path);
		frame->setPosition(Point(width, 0));
		bottom_layout->addChild(frame);

		width += frame->getContentSize().width + margin;

		_bottom_frames.push_back(frame);
	}

	width -= margin;
	bottom_layout->setContentSize(Size(width, bottom_layout->getContentSize().height));
	_topFramesCopy = _top_frames;

	

	ui::Helper::doLayout(this);
	return true;
}

void MJGameLazer::doAppear() {
	auto timeline = CSLoader::createTimeline("csb/game/lazer/game_lazer.csb");
	timeline->play("appear", false);
	this->runAction(timeline);
}

void MJGameLazer::onPlugMoving(MJGameLazerFrameTop * frame, Point position) {
}

void MJGameLazer::onPlugMoved(MJGameLazerFrameTop * frame, Point position) {
	this->unschedule("show_guide_bulbs_1");
	this->unschedule("delay_show_guide_bulbs_1");
	if (auto hand = utils::findChild(this, "hand_suggetion"))
	{
		hand->removeFromParent();
	}
	handSuggetionGame();
	for (auto f : _bottom_frames) {
        
		auto bounding_box = f->getBoundingBox();
		bounding_box.size.height += 100;
		bounding_box.origin = f->getParent()->convertToWorldSpace(bounding_box.origin);

		if (bounding_box.containsPoint(position)) if (f->getName() == frame->getName()) {
			_is_guide_showed = false;            
			cocos2d::experimental::AudioEngine::play2d(f->getAudio());

			_total_connected++;
			frame->connectToBottom(f);
			if (!game_play_enable) {
				if (_total_connected >= 2) {
					this->pauseGame();
					return;
				}
			}
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

void MJGameLazer::doDisappear() {
	_data.sets.pop();
	auto timeline = CSLoader::createTimeline("csb/game/lazer/game_lazer.csb");
	timeline->play("disappear", false);
	this->runAction(timeline);
	timeline->setLastFrameCallFunc([=]() {
		if (this->loadQuestion()) {
			this->doAppear();
		}
		else {
			this->onGameOver();
			this->unschedule("show_guide_bulbs_1");
		}
	});
}

void MJGameLazer::showGuide(float delay) {
	
	this->schedule([this](float) {
		runActionHand();
	}, 8, 100, 8.5f, "show_guide_bulbs_1");

}

void MJGameLazer::setCallbackCickSpeak()
{
    _callbackCickSpeak = [this] () {
        if (auto hand = utils::findChild(this, "hand_suggetion"))
        {
            hand->removeFromParent();
        }
        this->unschedule("show_guide_bulbs_1");
        if (_isPlayAudioSpeak)
        {
            if (!_isPlayAudioSugget)
            {
                AUDIO_ENGINE->stopSound(_pathAudioIntro);
                _isPlayAudioSugget = true;
            }
            _isPlayAudioSpeak = false;
            AUDIO_ENGINE->playSound(_pathAudioIntro, false, [=] {
                    if(_callbackPlayAudio)
                    {
                        _callbackPlayAudio();
                    }
                });
        }
    };
}

void MJGameLazer::setCallbackPlayAudio()
{
    _callbackPlayAudio = [this] () {
        showGuide(8.5f);
        _isPlayAudioSpeak = true;
    };
}

void MJGameLazer::addSpeaker()
{
	auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	auto screenSize = cocos2d::Director::getInstance()->getWinSize();
	this->addChild(speaker, 10000000);
	speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	speaker->setPosition(Vec2(20, Director::getInstance()->getWinSize().height - 10));
	speaker->addClickEventListener([=](Ref* sender) {
		if(_callbackCickSpeak)
        {
            _callbackCickSpeak();
        }
		
		});
}

void MJGameLazer::handSuggetionGame()
{
	//if (!_is_guide_showed) {
		_is_guide_showed = true;
		if (auto hand = utils::findChild(this, "hand_suggetion"))
		{
			hand->removeFromParent();
		}
		this->unschedule("show_guide_bulbs_1");
		this->unschedule("delay_show_guide_bulbs_1");
		this->scheduleOnce([=](float) {
			if (_topFramesCopy.empty())
				return;
			Point start, stop;
			int index = random(0, (int)_topFramesCopy.size() - 1);
			if (index < _topFramesCopy.size())
			{
				auto t = _topFramesCopy.at(index);
				_topFramesCopy.erase(_topFramesCopy.begin() + index);

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
				postStartHand = start;
				postEndHand = stop;
				this->showGuide(8.5);
			}
			}, 1.0f, "delay_show_guide_bulbs_1");
	//}
}

void MJGameLazer::runActionHand()
{
	auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
	this->addChild(hand, INT_MAX - 1);
	hand->setPosition(postStartHand - Point(0, 25));
	hand->setName("hand_suggetion");
	hand->runAction(Sequence::create(

		Spawn::createWithTwoActions(
			CallFunc::create([=] {
				this->unschedule("show_guide_bulbs_1");
				if (_isPlayAudioSugget)
				{
					_isPlayAudioSugget = false;
					AUDIO_ENGINE->playSound(_pathAudioIntro, false, [=] {
						_isPlayAudioSugget = true;
						showGuide(8.5);
						});
				}
				}),
			MoveTo::create(2, postEndHand))
		,DelayTime::create(0.5),
				RemoveSelf::create(),
				nullptr));
}

int MJGameLazer::randomTopFrame()
{
	if (_valueSizeObject.empty())
	{
		return 0;
	}
	int index = random(0, int(_valueSizeObject.size() - 1));

	int value = _valueSizeObject[index];
	_valueSizeObject.erase(_valueSizeObject.begin() + index);

	return value;
}

void MJGameLazer::stopSoundAudioSuggetion()
{
	AUDIO_ENGINE->stopSound(_pathAudioIntro);
	_isPlayAudioSugget = true;
	_isPlayAudioSpeak = true;
}
