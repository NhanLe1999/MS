//edited by Cuong Cao Nguyen 4/19/2022

#include "ChoBoPhanTimTongThe.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
INITIALIZE_READER(ChoBoPhanTimTongThe);

std::once_flag ChoBoPhanTimTongThe_reader;
#define CSB_NAME "csb/game/ChoBoPhanTimTongThe/ChoBoPhanTimTongThe.csb"
#define _game_path "json/ChoBoPhanTimTongThe/"
#define BG_GAME "games/ChoBoPhanTimTongThe/Hints-about-what.png"
#define BG "games/ChoBoPhanTimTongThe/bg.png"
#define AUDIO_INTRO "sounds/ChoBoPhanTimTongThe/Hints about What.mp3"

ChoBoPhanTimTongThe * ChoBoPhanTimTongThe::createGame() {
	std::call_once(ChoBoPhanTimTongThe_reader, [] {
		REGISTER_CSB_READER(ChoBoPhanTimTongThe);
		REGISTER_CSB_READER(ChoBoPhanTimTongThe_Item);
	});
	auto p = static_cast<ChoBoPhanTimTongThe *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void ChoBoPhanTimTongThe::didLoadFromCSB() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	//so_luot_choi = document.FindMember("game_mode")->value.GetInt();
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();
	//    autoPlay = isAutoPlay;*/
	this->loadFrames();
}

void ChoBoPhanTimTongThe::generateData(rapidjson::Value value_data) {
	data_game.clear();
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	auto lang = math::lang::english_us;
	if (document_icons.IsArray()) {
		if (JSON_PARSE.getString("en", document_icons[0], "language") == "vi") {
			lang = math::lang::vietnamese;
		}
	}
	for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
		rapidjson::Value& ques_data = value_data[i].FindMember("cau_hoi")->value;
		rapidjson::Value img_ques_data = ques_data.FindMember("images")->value.GetArray();
		std::string img_id = ques_data.FindMember("question")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		DataGame data;
		data.ques.audio = image->getAudios("name_1",lang).front();
		data.ques.text = image->getQuestionVMonkey("name_1", lang);
		for (int j = 0; j < img_ques_data.Size(); j++) {
			auto img = math::resource::ResHelper::getImage(images, img_ques_data[j].GetString());
			bo_phan x;
			x.image_path = img->path;
			x.audio = img->getAudios("name_1",lang).front();
			data.ques.images_question.push_back(x);
		}
		rapidjson::Value value_answers = value_data[i].FindMember("dap_an")->value.GetArray();
		for (rapidjson::SizeType j = 0; j < value_answers.Size(); j++) {
			Answer ans;
			auto img = math::resource::ResHelper::getImage(images, value_answers[j].FindMember("image")->value.GetString());
			if (value_answers[j].FindMember("isTrue")->value.IsBool()) {
				ans.is_true = value_answers[j].FindMember("isTrue")->value.GetBool();
			}
			else
			{
				ans.is_true = false;
			}
			ans.image_path = img->path;
			ans.audio = img->getAudios("name_1",lang).front();
			data.answers.push_back(ans);
		}
		data_game.push_back(data);
	}
}

void ChoBoPhanTimTongThe::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

	}), NULL));

	
}

void ChoBoPhanTimTongThe::loadFrames() {
	auto bg = utils::findChild<ImageView*>(this, "bg");
	current_question = 0;
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	auto question = utils::findChild<Layout*>(this, "question");
	btn_sound = utils::findChild<Layout*>(this, "sound_button");
	btn_sound->setOpacity(0);
	btn_sound->setTouchEnabled(false);
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	_speaker->setTouchEnabled(true);
	lock_sound_button = true;
	this->addChild(_speaker);
	_speaker->addClickEventListener([=](cocos2d::Ref * sender) {
		AUDIO_ENGINE->stopEffect(_audio2);
        if(!lock_sound_button)
        {
			stopGuideGame();
			for (auto item : imgs_ques) {
				item->setTouchEnabled(true);
			}
			_speaker->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
			AUDIO_ENGINE->stopEffect(_audio);
			audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=] { guideGame(); });
        }
    });
	so_luot_choi = data_game.size();

	auto visibleSize = Director::getInstance()->getVisibleSize();

	if (FileUtils::getInstance()->isFileExist(BG)) {
		_backgound = Sprite::create(BG);
		_backgound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_backgound->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		_backgound->setName("bg");
		this->addChild(_backgound);
	}

	if (FileUtils::getInstance()->isFileExist(BG_GAME)) {
		_backgoundGame = Sprite::create(BG_GAME);
		_backgoundGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_backgoundGame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		_backgoundGame->setName("bg_game");
		this->addChild(_backgoundGame);
	}
	
	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playEffect(AUDIO_INTRO);
		}), NULL));

	this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
		if (_backgoundGame) {
			_backgoundGame->runAction(FadeOut::create(1.0f));
		}
		}), DelayTime::create(1.5f), CallFunc::create([=] {
			if (_backgound) {
				_backgound->removeFromParent();
			}
			this->showQuestion();
			}), NULL));
}

void ChoBoPhanTimTongThe::showQuestion() {
	//gen answer

	auto panel_bp = utils::findChild<Layout*>(this, "Panel_bophan");

	answers_item.clear();
	imgs_ques.clear();
	for (int i = 0; i < data_game[current_question].answers.size(); i++) {
		ChoBoPhanTimTongThe_Item* item = ChoBoPhanTimTongThe_Item::createItem(data_game[current_question].answers[i].image_path, data_game[current_question].answers[i].audio, data_game[current_question].answers[i].is_true);
		item->setDelegate(this);
		answers_item.push_back(item);
		//item->setScale(0.75);
		item->setAnchorPoint(Vec2(0.5, 0.5));

	}
	mj::helper::shuffle(answers_item);
	math::func::setPositionList(answers_item, answers_item.size(), Rect(0, 0.05*768, 1024, 0.5*768));
	for (auto item : answers_item) {
		root_layout->addChild(item, 10);
		item->showItem();
		item->setTouchEnable(false);
		item->setOpacity(0);
	}

	auto text_question = utils::findChild<Text*>(this, "text_question");
	text_question->setFontName(font_name);
	text_question->setFontSize(35);
	text_question->setText(data_game[current_question].ques.text);
	//sad
	
	for (int i = 0; i < data_game[current_question].ques.images_question.size(); i++) {
		ImageView* img = ImageView::create(data_game[current_question].ques.images_question[i].image_path);
		img->setScale(150 / img->getContentSize().height);
		root_layout->addChild(img, 10);
		img->setTouchEnabled(false);
		img->setOpacity(0);
		img->addClickEventListener([=](cocos2d::Ref * sender) {
			stopGuideGame();
			AUDIO_ENGINE->stopEffect(_audio2);
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.images_question[i].audio);
			for (auto item : imgs_ques) {
				item->setTouchEnabled(false);
			}
			_audio = AUDIO_ENGINE->playSound(data_game[current_question].ques.images_question[i].audio, false, [=] {
				for (auto item : imgs_ques) {
					item->setTouchEnabled(true);
					guideGame();
				}
				});
		});
		//set touch
		imgs_ques.push_back(img);
	}

	math::func::setPositionList(imgs_ques, imgs_ques.size(), panel_bp->getBoundingBox());

    lock_sound_button = true;
	audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=]() {
		imgFadeIn(0, imgs_ques);
	});
}

void ChoBoPhanTimTongThe::imgFadeIn(int i, vector<ImageView*> imgs_ques) {
	float timeDelay = 1.5f;
	if (i >= imgs_ques.size()) {
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			for (auto item : imgs_ques) {
				item->setTouchEnabled(true);
			}
			for (auto item : answers_item) {
				item->runAction(Sequence::create(FadeIn::create(1.0F), DelayTime::create(1.0f), lock_sound_button = false, NULL));
				item->setTouchEnable(true);
				guideGame();
			}
			}), NULL));
		return;
	}
	else
	{
		if (i == 0) { timeDelay = 0.25f; }
		imgs_ques[i]->runAction(Sequence::create(DelayTime::create(timeDelay), CallFunc::create([=] {
			imgs_ques[i]->runAction(FadeIn::create(1.0f));
			this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=] {
				AUDIO_ENGINE->play(data_game[current_question].ques.images_question[i].audio);
				}), NULL));
			imgFadeIn(i + 1, imgs_ques);
			}), NULL));
	}
}

void ChoBoPhanTimTongThe::nextGame() {
	data_game.erase(data_game.begin());
	if (data_game.size() > 0) {
		showQuestion();
	}
	else {
		onFinishGameVMonkey();
	}
}
void ChoBoPhanTimTongThe::onExitTransitionDidStart() {
	AUDIO_ENGINE->stopAllSound();
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void ChoBoPhanTimTongThe::guideGame() {
	this->schedule([=](float) {
		AUDIO_ENGINE->playSound(data_game[current_question].ques.audio);
	}, 12, 100, 10, "in_active_mode");
}

void ChoBoPhanTimTongThe::stopGuideGame() {
	unschedule("in_active_mode");
	AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
}

void ChoBoPhanTimTongThe::onFinishGameVMonkey() {
	MJMainGBase::onFinishGameVMonkey();
}
void ChoBoPhanTimTongThe::onClickedBox(cocos2d::Ref * sender)
{
	stopGuideGame();
	AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
	AUDIO_ENGINE->stopEffect(_audio);
	lock_sound_button = false;
	AUDIO_ENGINE->stopEffect(audio_id_question);
	for (auto item : imgs_ques) {
		item->setTouchEnabled(true);
	}
	auto obj = static_cast<ChoBoPhanTimTongThe_Item *>(sender);
	if (obj->isTrue()) {
		for (auto item : imgs_ques) {
			item->setTouchEnabled(false);
		}
		lock_sound_button = true;
		for (auto item : answers_item) {
			item->setTouchEnable(false);
		}
		obj->setTouchEnable(false);
		AUDIO_ENGINE->playEffect("sounds/ChoBoPhanTimTongThe/correct.mp3");
		obj->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0.25f, 1.)), ScaleTo::create(0.25f, 0.75f), CallFunc::create([=] {
			obj->playAudio();
		}), NULL));
		this->scheduleOnce([=](float dt) {
			current_question++;
			for (auto i : answers_item) i->removeFromParent();
			for (auto i : imgs_ques) i->removeFromParent();
			if (current_question >= so_luot_choi) {
				onFinishGameVMonkey();
			}
			else {
				showQuestion();
			}
		}, 2.0f, cocos2d::StringUtils::format("next"));
	}
	else {
		guideGame();
		obj->setTouchEnable(false);
		_audio2 = AUDIO_ENGINE->playEffect("sounds/ChoBoPhanTimTongThe/wrong.mp3");
		obj->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(-20, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), CallFunc::create([=] {
			obj->setTouchEnable(true);
		}), NULL));
	}
}

ui::Widget::ccWidgetClickCallback ChoBoPhanTimTongThe::onLocateClickCallback(const std::string &callback_name) {
	return nullptr;
}
