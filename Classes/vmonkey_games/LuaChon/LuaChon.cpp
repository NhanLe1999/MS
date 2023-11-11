#include "LuaChon.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
#include "AudioEngine.h"
#include "AudioHelper.h"
#include "SimpleAudioEngine.h"
INITIALIZE_READER(LuaChon);

std::once_flag LuaChon_reader;
#define CSB_NAME "csb/game/LuaChon/LuaChon.csb"
#define _game_path "json/LuaChon/"
//sounds path
#define path_bg_music "sounds/LuaChon/music.mp3"
#define path_command "sounds/LuaChon/Choose the correct answer.mp3"
LuaChon* LuaChon::createGame() {
	std::call_once(LuaChon_reader, [] {
		REGISTER_CSB_READER(LuaChon);
		REGISTER_CSB_READER(LuaChon_Item);
		});
	auto p = static_cast<LuaChon*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void LuaChon::didLoadFromCSB() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	so_luot_choi = document.FindMember("game_mode")->value.GetInt();
	path_background = "none";
	if (document.HasMember("background")) {
		path_background = document.FindMember("background")->value.GetString();
	}
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();

	//    atoPlay = isAutoPlay;*/
	this->loadFrames();
}

void LuaChon::generateData(rapidjson::Value value_data) {
	data_game.clear();
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	if (path_background != "none") path_background = math::resource::ResHelper::getImage(images, path_background)->path;
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

	for (rapidjson::SizeType i = 0; i < so_luot_choi; i++) {
		std::string img_id = value_data[i].FindMember("question")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		DataGame data;
		data.ques.audio = image->getAudios("name_1", lang).front();
		data.ques.image_path = image->path;
		data.ques.text = image->getQuestionVMonkey("name_1", lang);
		data.ques.visible_image = false;
		if (value_data[i].HasMember("visible_image_question")) data.ques.visible_image = value_data[i].FindMember("visible_image_question")->value.GetBool();
		rapidjson::Value value_answers = value_data[i].FindMember("answer")->value.GetArray();
		for (rapidjson::SizeType j = 0; j < value_answers.Size(); j++) {
			Answer ans;
			auto img = math::resource::ResHelper::getImage(images, value_answers[j].FindMember("full_text")->value.GetString());
			if (value_answers[j].HasMember("is_correct")) {
				ans.is_true = value_answers[j].FindMember("is_correct")->value.GetBool();
			}
			else {
				ans.is_true = false;
			}
			ans.image_path = img->path;
			ans.audio = img->getAudios("name_1", lang).front();
			data.answers.push_back(ans);
		}
		data_game.push_back(data);
	}
}

void LuaChon::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

		}), NULL));


}

void LuaChon::loadFrames() {
	auto bg = utils::findChild<ImageView*>(this, "bg");
	if (path_background != "none") {
		bg->loadTexture(path_background);
	}
	current_question = 0;
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	auto question = utils::findChild<ui::ImageView*>(this, "question");
	question->setVisible(false);
	btn_sound = utils::findChild<Layout*>(this, "sound_button");
	btn_sound->setVisible(false);

	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);

	image_character = utils::findChild<ImageView*>(this, "Image_question");
	image_character->setVisible(false);
	image_character->setTouchEnabled(false);

	if (auto textQuestion = utils::findChild<ui::Text*>(this, "text_question"))
	{
		textQuestion->setString("");
	}

	AUDIO_ENGINE->play(path_command, false, 1.0f, [=] {
		image_character->setVisible(true);
		question->setVisible(true);
		this->showQuestion();
		});
}

void LuaChon::showQuestion() {
	//gen answer
	AUDIO_ENGINE->play(path_bg_music, true, 0.25f);

	auto mid_layout = utils::findChild<Layout*>(this, "Panel_mid");
	answers_item.clear();
	for (int i = 0; i < data_game[current_question].answers.size(); i++) {
		LuaChon_Item* item = LuaChon_Item::createItem(data_game[current_question].answers[i].image_path, data_game[current_question].answers[i].audio, data_game[current_question].answers[i].is_true);
		item->setDelegate(this);
		item->setTag(i);
		answers_item.push_back(item);
		item->setAnchorPoint(Vec2(0.5, 0.5));
		mid_layout->addChild(item, 10);
		item->setScale(0);
		item->setTouchEnable(false);
	}
	mj::helper::shuffle(answers_item);

	if (answers_item.size() != 4) {
		math::func::setPositionList(answers_item, 3, Rect(0, 0.05 * 768, 1024, 0.65 * 768));
	}
	else {
		math::func::setPositionList(answers_item, 2, Rect(0, 0.05 * 768, 1024, 0.65 * 768));
	}
	float dt = 0;
	for (auto item : answers_item) {
		item->definePosRot();
		this->scheduleOnce([=](float dt) {
			item->showItem();
			}, dt += 0.5f, cocos2d::StringUtils::format("shown%f", dt));
	}
	auto text_question = utils::findChild<Text*>(this, "text_question");
	text_question->setFontName(font_name);
	text_question->setFontSize(35);
	text_question->setText(data_game[current_question].ques.text);

	image_character->loadTexture(data_game[current_question].ques.image_path);
	if (!data_game[current_question].ques.visible_image) image_character->setVisible(false);
	else {
		image_character->setVisible(true);
	}
	_speaker->setTouchEnabled(false);
	audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=]() {
		_speaker->setTouchEnabled(true);
		for (auto a : answers_item) {
			a->setTouchEnable(true);
		}
		showTutorial();
		_speaker->addClickEventListener([=](cocos2d::Ref* sender) {
			_speaker->setTouchEnabled(false);
			stopGuiding();
			_speaker->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
			audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=]() {
				_speaker->setTouchEnabled(true);
				showTutorial();
				});
			});
		});
}

void LuaChon::nextGame() {
	data_game.erase(data_game.begin());
	if (data_game.size() > 0) {
		showQuestion();
	}
	else {
		onFinishGameVMonkey();
	}
}
void LuaChon::onExitTransitionDidStart() {
	AUDIO_ENGINE->stopAllAudio();
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void LuaChon::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame();
		}, 7, 100, delay_time, "animated_guide");
}

void LuaChon::onFinishGameVMonkey() {
	MJMainGBase::onFinishGameVMonkey();
	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->playSound("sounds/LuaChon/winner.mp3");
}
void LuaChon::onClickedBox(cocos2d::Ref* sender)
{
	AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
	_speaker->setTouchEnabled(false);

	auto obj = static_cast<LuaChon_Item*>(sender);
	int zOrder_old = obj->getZOrder();
	obj->setZOrder(1000);
	if (obj->isTrue()) {
		stopGuiding();
		ImageView* image_true = ImageView::create("games/LuaChon/check.png");
		image_true->setAnchorPoint(Vec2(0.5, 0.5));
		image_true->setPosition((obj->getContentSize() / 2));
		image_true->setScale((obj->getContentSize().height / image_true->getContentSize().height) * 1.15);
		obj->addChild(image_true);
		obj->setTouchEnable(false);
		obj->setName("ok");

		AUDIO_ENGINE->playEffect("sounds/LuaChon/correct.mp3");
		obj->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0.25f, 1.3f)), ScaleTo::create(0.25f, 1.f), CallFunc::create([=] {
			obj->setZOrder(zOrder_old);
			image_true->runAction(Spawn::create(ScaleBy::create(0.25, 0.5), MoveTo::create(0.25, Vec2(70, 50)), NULL));
			}), NULL));
		bool kt_end = true;
		for (auto item : answers_item) {
			if (item->isTrue() && item->getName() != "ok") {
				kt_end = false;
				break;
			}
		}
		if (kt_end) {
			AUDIO_ENGINE->stopEffect(audio_id_question);
			for (auto item : answers_item) {
				item->setTouchEnable(false);
			}
		}
		//        else{
		//            btn_sound->setTouchEnabled(true);
		//        }
		AUDIO_ENGINE->playSound(obj->getAudio(), false, [=]() {
			if (kt_end) {
				this->scheduleOnce([=](float dt) {
					current_question++;
					for (auto i : answers_item) i->removeFromParent();
					if (current_question >= so_luot_choi) {
						onFinishGameVMonkey();
					}
					else {
						showQuestion();
					}
					}, 1.5f, cocos2d::StringUtils::format("next"));
			}
			else {
				_speaker->setTouchEnabled(true);
			}
			});
	}
	else {
		stopGuiding();
		showTutorial();
		obj->setTouchEnable(false);
		_speaker->setTouchEnabled(true);
		AUDIO_ENGINE->playEffect("sounds/LuaChon/wrong.mp3");
		ImageView* image_wrong = ImageView::create("games/LuaChon/cross.png");
		image_wrong->setAnchorPoint(Vec2(0.5, 0.5));
		image_wrong->setPosition((obj->getContentSize() / 2));
		image_wrong->setScale((obj->getContentSize().height / image_wrong->getContentSize().height) * 1.25);
		obj->resetPosRot();
		obj->addChild(image_wrong);
		obj->runAction(Sequence::create(/*Spawn::create(*/MoveBy::create(0.1f, Vec2(0, 15)), RotateBy::create(0.1, -8)/*,NULL)*/,/*Spawn::create(*/MoveBy::create(0.1f, Vec2(0, -15)), RotateBy::create(0.1, 8)/*, NULL)*/, CallFunc::create([=] {
			obj->setTouchEnable(true);
			obj->setZOrder(zOrder_old);
			image_wrong->removeFromParent();
			}), NULL));
	}
	//AUDIO_ENGINE->playSound(obj->getInfo().audio);
	//SoundManager::getInstance()->playSound(obj->getInfo().audio_phonic);
	//hideGuideGame();
}

ui::Widget::ccWidgetClickCallback LuaChon::onLocateClickCallback(const std::string& callback_name) {
	return nullptr;
}

void LuaChon::showTapGuideGame2(Vec2 pos, int loop, float delay_time, std::string gaf_guide_path) {

	this->schedule([=](float) {
		_audio = AUDIO_ENGINE->play(path_command, false);
		}, 12, loop, delay_time, "animated_guide");
}

void LuaChon::showTutorial() {
	auto itemCorrect = utils::findChild<LuaChon_Item*>(this, 0);
	itemCorrect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto posItem = itemCorrect->getBoundingBox();
	auto posStart = Point(posItem.getMidX(), posItem.getMidY());
	showTapGuideGame2(itemCorrect->getParent()->convertToWorldSpace(posStart));
}

void LuaChon::stopGuiding() {
	AUDIO_ENGINE->stopEffect(_audio);
	this->unschedule("animated_guide");
	if (auto hand = utils::findChild(this, "show_hand")) {
		hand->stopAllActions();
		hand->removeFromParent();
	}
}