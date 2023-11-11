#include "TimTuCungVanLoai.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
INITIALIZE_READER(TimTuCungVanLoai);

std::once_flag TimTuCungVanLoai_reader;
#define CSB_NAME "csb/game/TimTuCungVanLoai/TimTuCungVanLoai.csb"
#define _game_path "json/TimTuCungVanLoai/"
#define AUDIO_WRONG "sounds/TimTuCungVanLoai/sw_wrong.mp3"
#define AUDIO_BG "sounds/TimTuCungVanLoai/light_background.mp3"

TimTuCungVanLoai* TimTuCungVanLoai::createGame() {
	std::call_once(TimTuCungVanLoai_reader, [] {
		REGISTER_CSB_READER(TimTuCungVanLoai);
		REGISTER_CSB_READER(TimTuCungVanLoai_Item);
		});
	auto p = static_cast<TimTuCungVanLoai*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void TimTuCungVanLoai::didLoadFromCSB() {
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

void TimTuCungVanLoai::generateData(rapidjson::Value value_data) {
	so_luot_choi = value_data.Size();
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
		rapidjson::Value& json_cau_hoi = value_data[i].FindMember("cau_hoi")->value;
		std::string img_id = json_cau_hoi.FindMember("text_question")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		DataGame data;
		data.ques.dau_cau = "!";
		if (json_cau_hoi.HasMember("dau_cau")) {
			data.ques.dau_cau = json_cau_hoi.FindMember("dau_cau")->value.GetString();
		}
		data.ques.audio_question = image->getAudios("name_1", lang).front();
		data.ques.text_question = image->getQuestionVMonkey("name_1", lang);
		std::string img_id_question_phonic = json_cau_hoi.FindMember("text_phonic")->value.GetString();
		image = math::resource::ResHelper::getImage(images, img_id_question_phonic);
		data.ques.audio_phonic = image->getAudios("name_1", lang).front();
		data.ques.text_phonic = image->getText("name_1", lang);
		data.ques.is_phonic = json_cau_hoi.FindMember("is_phonic")->value.GetBool();
		rapidjson::Value value_answers = value_data[i].FindMember("dap_an")->value.GetArray();
		for (rapidjson::SizeType j = 0; j < value_answers.Size(); j++) {
			TimTuCungVanLoai_Item::Answer ans;
			auto img = math::resource::ResHelper::getImage(images, value_answers[j].FindMember("text_answer")->value.GetString());
			if (value_answers[j].HasMember("isTrue")) {
				ans.is_true = value_answers[j].FindMember("isTrue")->value.GetBool();
			}
			else {
				ans.is_true = false;
			}
			ans.audio = img->getAudios("name_1", lang).front();
			ans.text = img->getText("name_1", lang);
			ans.phonics = img->getTextPhonic("name_p", lang);
			ans.p_audio = img->getAudioPhonic("name_p", lang);
			ans.co_dau = (img->getComparisionValue("position_p") == -1) ? false : true;
			ans.pos_dau = img->getComparisionValue("position_p");
			data.answers.push_back(ans);
		}
		data_game.push_back(data);
	}
}

void TimTuCungVanLoai::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

		}), NULL));


}

void TimTuCungVanLoai::loadFrames() {
	auto bg = utils::findChild<ImageView*>(this, "bg");
	current_question = 0;
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	auto question = utils::findChild<Layout*>(this, "question");
	btn_sound = utils::findChild<Layout*>(this, "sound_button");
	btn_sound->setOpacity(0);
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	_speaker->setTouchEnabled(false);
	this->addChild(_speaker);
	this->showQuestion();
	if (FileUtils::getInstance()->isFileExist(AUDIO_BG)) {
		AUDIO_ENGINE->playSound(AUDIO_BG, true);
	}
}

void TimTuCungVanLoai::showQuestion() {
	//gen answer
	answers_item.clear();
	std::vector<std::string> gaf_path;
	if (data_game[current_question].answers.size() <= 4) {
		gaf_path.push_back("gaf/TimTuCungVanLoai/Blue/Blue.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Yellow/Yellow.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Green/Green.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Pink/Pink.gaf");
	}
	else {
		gaf_path.push_back("gaf/TimTuCungVanLoai/Blue/Blue.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Yellow/Yellow.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Blue/Blue.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Green/Green.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Pink/Pink.gaf");
		gaf_path.push_back("gaf/TimTuCungVanLoai/Green/Green.gaf");
	}
	mj::helper::shuffle(data_game[current_question].answers);
	for (int i = 0; i < data_game[current_question].answers.size(); i++) {
		TimTuCungVanLoai_Item* item = TimTuCungVanLoai_Item::createItem(data_game[current_question].answers[i], gaf_path[i], true);
		item->setDelegate(this);
		answers_item.push_back(item);
		item->setAnchorPoint(Vec2(0.5, 0.5));

	}


	if (answers_item.size() != 4) {
		math::func::setPositionList(answers_item, 3, Rect(0, 0.05 * 768, 1024, 0.6 * 768));
	}
	else {
		math::func::setPositionList(answers_item, 2, Rect(0, 0.05 * 768, 1024, 0.6 * 768));
	}
	for (auto item : answers_item) {
		root_layout->addChild(item, 10);
		item->showItem();
	}
	text_question = utils::findChild<Text*>(this, "text_question");
	font_name = PHONIC_MANAGER->getGameFontName();
	text_question->setFontName(font_name);
	text_question->setFontSize(35);
	text_question->setText(data_game[current_question].ques.text_question + " " + data_game[current_question].ques.text_phonic + data_game[current_question].ques.dau_cau);
	for (auto a : answers_item) {
		a->setTouchEnable(false);
	}
	this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
		text_question->setTextColor(Color4B::RED);
		AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio_question);
		audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_question, false, [=]() {
			audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_phonic, false, [=]() {
				guideGame();
				text_question->setTextColor(Color4B::BLACK);
				for (auto a : answers_item) {
					a->setTouchEnable(true);
				}
				_speaker->setTouchEnabled(true);
				});
			});
		}), NULL));
	_speaker->addClickEventListener([=](cocos2d::Ref* sender) {
		text_question->setTextColor(Color4B::RED);
		stopGuideGame();
		AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio_question);
		_speaker->setTouchEnabled(false);
		_speaker->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
		audio_id_sound = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_question, false, [=]() {
			audio_id_sound = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_phonic, false, [=]() {
				text_question->setTextColor(Color4B::BLACK);
				_speaker->setTouchEnabled(true);
				guideGame();
				});

			});
		});
}
void TimTuCungVanLoai::nextGame() {
	data_game.erase(data_game.begin());
	if (data_game.size() > 0) {
		showQuestion();
	}
	else {
		onFinishGameVMonkey();
	}
}
void TimTuCungVanLoai::onExitTransitionDidStart() {
	AUDIO_ENGINE->stopAllSound();
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}





void TimTuCungVanLoai::showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path) {
	auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
	if (guide_layout_check) return;
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		auto hand = GAFWrapper::createGAF(gaf_guide_path);
		guide_layout->addChild(hand, 1900000000);
		hand->setPosition(pos1);
		hand->play(false, [=](gaf::GAFObject* obj) {
			hand->runAction(Sequence::create(MoveTo::create(0.6f, pos2),
				CallFunc::create([=]() {
					hand->play(false, [=](gaf::GAFObject* obj) {
						hand->removeFromParent();
						});
					}), NULL));
			});
		}, delay_time, 100, delay_time, "animated_guide");
}

void TimTuCungVanLoai::hideTapGuideGame() {
	Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
}

void TimTuCungVanLoai::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame();
		}, 7, 100, delay_time, "animated_guide");

}

void TimTuCungVanLoai::onFinishGameVMonkey() {
	MJMainGBase::onFinishGameVMonkey();
}
void TimTuCungVanLoai::onClickedBox(cocos2d::Ref* sender)
{
	text_question->setTextColor(Color4B::BLACK);
	auto obj = static_cast<TimTuCungVanLoai_Item*>(sender);
	AUDIO_ENGINE->stopEffect(audio_id_sound);
	if (obj->isTrue()) {
		stopGuideGame();
		_speaker->setTouchEnabled(false);
		obj->setTouchEnable(false);
		for (auto a : answers_item) {
			a->setTouchEnable(false);
		}
		obj->effectCorrect(data_game[current_question].ques.text_phonic, data_game[current_question].ques.is_phonic);
		this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=] {
			obj->text->setTextColor(Color4B::RED);
			}), DelayTime::create(1.0f), CallFunc::create([=] {
				obj->text->setTextColor(Color4B::WHITE);
				}), NULL));
		obj->setName("ok");
		//obj->setVisible(false);
		bool kt = true;
		for (auto a : answers_item) {
			if (a->getName() != "ok" && a->isTrue()) {
				kt = false;
			}
		}
		if (kt) {
			this->scheduleOnce([=](float dt) {
				for (auto a : answers_item) {
					a->setTouchEnable(true);
				}
				current_question++;
				for (auto i : answers_item) i->removeFromParent();
				if (current_question >= so_luot_choi) {
					onFinishGameVMonkey();
				}
				else {
					showQuestion();
				}
				}, 2.5f, cocos2d::StringUtils::format("next"));
		}
	}
	else {
		stopGuideGame();
		obj->setTouchEnable(false);
		for (auto a : answers_item) {
			a->setTouchEnable(false);
		}
		if (FileUtils::getInstance()->isFileExist(AUDIO_WRONG)) {
			AUDIO_ENGINE->playSound(AUDIO_WRONG, false, [=] {
				_speaker->setTouchEnabled(true);
				for (auto a : answers_item) {
					a->setTouchEnable(true);
				}
				guideGame();
			});
		}
		obj->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(-20, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), CallFunc::create([=] {
			obj->setTouchEnable(true);
			}), NULL));
	}
	//AUDIO_ENGINE->playSound(obj->getInfo().audio);
	//SoundManager::getInstance()->playSound(obj->getInfo().audio_phonic);
	//hideGuideGame();
}

void TimTuCungVanLoai::guideGame() {
	this->schedule([=](float) {
		AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio_question);
		audio_guide_game = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_question, false, [=]() {
			audio_guide_game = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_phonic);
			});
		}, 12.5, 100, 10, "guide_game");
}

void TimTuCungVanLoai::stopGuideGame()
{
	this->unschedule("guide_game");
	AUDIO_ENGINE->stopEffect(audio_guide_game);
}

ui::Widget::ccWidgetClickCallback TimTuCungVanLoai::onLocateClickCallback(const std::string& callback_name) {
	return nullptr;
}
