#include "CauHoiMo.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
INITIALIZE_READER(CauHoiMo);

std::once_flag CauHoiMo_reader;
#define CSB_NAME "csb/game/cau_hoi_mo/CauHoiMo.csb"
#define _game_path "json/CauHoiMo/"

CauHoiMo* CauHoiMo::createGame()
{
	std::call_once(CauHoiMo_reader, []
		{
			REGISTER_CSB_READER(CauHoiMo);
		});
	auto p = static_cast<CauHoiMo*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void CauHoiMo::didLoadFromCSB()
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");

	if (str.empty())
	{
		CCLOG("CauHoiMo::didLoadFromCSB|str %s", str.c_str());

		return;
	}

	rapidjson::Document document;
	document.Parse<rapidjson::kParseDefaultFlags>(str.c_str());
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();
	//    autoPlay = isAutoPlay;
	this->loadFrames();
}

void CauHoiMo::generateData(rapidjson::Value value_data) {
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
		std::string img_id = value_data[i].FindMember("question")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		Question data;
		data.audio = image->getAudios("name_1", lang).front();
		data.image_path = image->path;
		data.text = image->getQuestionVMonkey("name_1", lang);
		data.bg_path = "";
		if (value_data[i].HasMember("background")) {
			if (value_data[i].FindMember("background")->value.GetString() != "null") {
				auto bgImage = math::resource::ResHelper::getImage(images, value_data[i].FindMember("background")->value.GetString());
				data.bg_path = bgImage->path;
			}
		}
		data_game.push_back(data);

	}
}

void CauHoiMo::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

		}), NULL));


}

void CauHoiMo::loadFrames() {
	//auto bg = utils::findChild<ImageView*>(this, "bg");
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	auto question = utils::findChild<Layout*>(this, "question");
	btn_sound = utils::findChild<Layout*>(this, "sound_button");
	btn_next = utils::findChild<Layout*>(this, "next_button");
	utils::findChild<ImageView*>(this, "character")->setVisible(false);
	pos_character = utils::findChild<ImageView*>(this, "character")->getPosition();
	btn_next->setTouchEnabled(false);
	btn_sound->setTouchEnabled(false);
	this->showQuestion();

	this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=] {
		AUDIO_ENGINE->playEffect("audio/ruJq2JGe9F9tz4AT9Nulc48GaPMuL5hQ.mp3");
		}), NULL));

	this->schedule([=](float) {
		btn_next->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.5, 1.5f)), ScaleTo::create(0.5f, 1.f), NULL));
		}, 5, 100, 60, "endgame");
	guideGame();
}

void CauHoiMo::showQuestion() {
	if (data_game.front().bg_path != "") root_layout->setBackGroundImage(data_game.front().bg_path);
	auto text_question = utils::findChild<Text*>(this, "text_question");
	text_question->setFontName(font_name);
	text_question->setFontSize(35);
	text_question->setText(data_game.front().text);
	Size bgTextSize = text_question->getParent()->getContentSize();
	text_question->setContentSize(Size(bgTextSize.width * 0.9, bgTextSize.height * 0.75));
	//image_character->loadTexture(data_game.front().image_path);
	root_layout->removeChildByName("character");
	auto image_character = ImageView::create(data_game.front().image_path);
	auto size_ic = image_character->getContentSize();
	float rate_scale = std::min(450.0 / size_ic.height, 850.0 / size_ic.width);
	image_character->setScale(rate_scale);
	image_character->setAnchorPoint(Vec2(0.5, 0));
	image_character->setPosition(pos_character);
	image_character->setName("character");
	root_layout->addChild(image_character);
	auto posY = (500 - image_character->getContentSize().height * rate_scale) / 2;
	image_character->setPositionY(posY);
	audio_id_question = AUDIO_ENGINE->playSound(data_game.front().audio, false, [=]() {
		btn_next->setTouchEnabled(true);
		this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=] {
			btn_sound->setTouchEnabled(true);
			image_character->setTouchEnabled(true);
			}), NULL));
		btn_sound->addClickEventListener([=](cocos2d::Ref* sender) {
			btn_sound->setTouchEnabled(false);
			image_character->setTouchEnabled(false);
			btn_sound->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
			audio_id_question = AUDIO_ENGINE->playSound(data_game.front().audio, false, [=]() {
				btn_sound->setTouchEnabled(true);
				image_character->setTouchEnabled(true);
				});
			});
		image_character->addClickEventListener([=](cocos2d::Ref* sender) {
			btn_sound->setTouchEnabled(false);
			image_character->setTouchEnabled(false);
			auto rate_scale = image_character->getScale();
			//image_character->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, rate_scale*1.3)), ScaleTo::create(0.2f, rate_scale), NULL));
			audio_id_question = AUDIO_ENGINE->playSound(data_game.front().audio, false, [=]() {
				btn_sound->setTouchEnabled(true);
				image_character->setTouchEnabled(true);
				});
			});
		btn_next->addClickEventListener([=](cocos2d::Ref* sender) {
			AUDIO_ENGINE->stopEffect(audio_id_question);
			btn_next->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
			btn_next->setTouchEnabled(false);
			btn_sound->setTouchEnabled(false);
			image_character->setTouchEnabled(false);
			nextGame();
			});
		});


}
void CauHoiMo::nextGame() {
	data_game.erase(data_game.begin());
	if (data_game.size() > 0) {
		showQuestion();
	}
	else {
		onFinishGameVMonkey();
	}
}
void CauHoiMo::onExitTransitionDidStart() {
	AUDIO_ENGINE->stopAllSound();
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}





void CauHoiMo::showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path) {
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

void CauHoiMo::hideTapGuideGame() {
	Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
}

void CauHoiMo::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame();
		}, 7, 100, delay_time, "animated_guide");

}

void CauHoiMo::onFinishGameVMonkey() {
	MJMainGBase::onFinishGameVMonkey();
}


ui::Widget::ccWidgetClickCallback CauHoiMo::onLocateClickCallback(const std::string& callback_name) {
	return nullptr;
}

void CauHoiMo::guideGame() {
	this->runAction(Sequence::create(DelayTime::create(12.0f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound("sounds/cau_hoi_mo/Let's_answer_the_question!.mp3");
	}), NULL));
}