#include "DanhGia.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
INITIALIZE_READER(DanhGia);

std::once_flag DanhGia_reader;
#define CSB_NAME "csb/game/DanhGia/DanhGia.csb"
#define CSB_NAME_VM "csb/game/DanhGia/DanhGia_VM.csb"
#define _game_path "json/DanhGia/"
#define GAME_TUTORIAL "sounds/DanhGia/answer.mp3"
#define BUTTON_ANSWER_CORRECT "hdr/games/DanhGia/True.png"
#define BUTTON_ANSWER_WRONG "hdr/games/DanhGia/False.png"
#define BUTTON_ANSWER "hdr/games/DanhGia/yellow.png"

DanhGia * DanhGia::createGame() {
	std::call_once(DanhGia_reader, [] {
		REGISTER_CSB_READER(DanhGia);
	});
	auto p = static_cast<DanhGia *>(cocos2d::CSLoader::createNodeWithVisibleSize(CONFIG_MANAGER->isVietnameseStories()?CSB_NAME_VM:CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void DanhGia::didLoadFromCSB() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	so_luot_choi = document.FindMember("number_play")->value.GetInt();
    is_speak_answer = false;
    if (document.HasMember("is_read_answer")){
        is_speak_answer = document.FindMember("is_read_answer")->value.GetBool();
    }
	rapidjson::Value& json_data = document.FindMember("data")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();
	//    autoPlay = isAutoPlay;*/
	this->loadFrames();
}

void DanhGia::generateData(rapidjson::Value value_data) {
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
		DataGame data;
        if (value_data[i].HasMember("intro_question")){
            std::string img_intro_id = value_data[i].FindMember("intro_question")->value.GetString();
            data.ques.intro_question = math::resource::ResHelper::getImage(images, img_intro_id)->getAudios("name_1",lang).front();
        }
		data.ques.audio = image->getAudios("name_1",lang).front();
		data.ques.image_path = image->path;
		data.ques.text = image->getQuestionVMonkey("name_1", lang);
		auto image_smile = math::resource::ResHelper::getImage(images, value_data[i].FindMember("smile")->value.GetString());
		data.ques.text_smile = image_smile->getText("name_1",lang);
		data.ques.audio_smile= image_smile->getAudios("name_1", lang).front();
		data.ques.path_smile = image_smile->path;
		auto image_sad = math::resource::ResHelper::getImage(images, value_data[i].FindMember("sad")->value.GetString());
		data.ques.text_sad = image_sad->getText("name_1", lang);
		data.ques.audio_sad = image_sad->getAudios("name_1", lang).front();
		data.ques.path_sad = image_sad->path;
		rapidjson::Value value_answers = value_data[i].FindMember("image_question")->value.GetArray();
		for (rapidjson::SizeType j = 0; j < value_answers.Size(); j++) {
			ImageQuestion iq;
			auto img = math::resource::ResHelper::getImage(images, value_answers[j].FindMember("image")->value.GetString());
			iq.is_smile = value_answers[j].FindMember("is_smile")->value.GetBool();
			iq.image_path = img->path;
			iq.audio = img->getAudios("name_1", lang).front();
			data.images_question.push_back(iq);
		}
		data_game.push_back(data);
	}
}

void DanhGia::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

	}), NULL));

	
}

void DanhGia::loadFrames() {
	auto bg = utils::findChild<ImageView*>(this, "bg");
	current_question = 0;
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	
	star_smile_csb = utils::findChild<Layout*>(this, "Panel_smile");
	star_sad_csb = utils::findChild<Layout*>(this, "Panel_sad");
	star_smile_csb->setVisible(false);
	star_sad_csb->setVisible(false);
	btn_radio = utils::findChild<Layout*>(this, "sound_button");
	btn_radio->setVisible(false);

	btn_sound = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	btn_sound->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(btn_sound);

	//label TRUE
	std::string texttrue = data_game[current_question].ques.text_smile;
	textTrue = Label::createWithTTF(texttrue,"fonts/KGWHATTHETEACHERWANTS.TTF",30);
	textTrue->setPosition(Vec2(screen_size.width / 3, screen_size.height / 5));
	textTrue->setAnchorPoint(Vec2(0.5, 0.5));
	textTrue->setVisible(false);
	textTrue->setColor(Color3B::BLACK);
	addChild(textTrue, 10000);

	//label FALSE
	std::string textfalse = data_game[current_question].ques.text_sad;
	textFalse = Label::createWithTTF(textfalse, "fonts/KGWHATTHETEACHERWANTS.TTF", 30);
	textFalse->setPosition(Vec2(screen_size.width * 2 / 3, screen_size.height / 5));
	textFalse->setAnchorPoint(Vec2(0.5, 0.5));
	textFalse->setVisible(false);
	textFalse->setColor(Color3B::BLACK);
	addChild(textFalse,10000);

	

	//button answer correct
	btn_true_blue = ImageView::create(BUTTON_ANSWER_CORRECT);
	btn_true_blue->setPosition(Vec2(screen_size.width / 3, screen_size.height / 5));
	btn_true_blue->setAnchorPoint(Vec2(0.5, 0.5));
	btn_true_blue->ignoreContentAdaptWithSize(false);
	btn_true_blue->setContentSize(screen_size / 10);
	float btn_true_blue_scalex = 1;
	while (btn_true_blue->getBoundingBox().size.width < textTrue->getBoundingBox().size.width + 40) {
		btn_true_blue_scalex = btn_true_blue_scalex + 0.001;
		btn_true_blue->setScaleX(btn_true_blue_scalex);
	}
	
	btn_true_blue->setVisible(false);
	addChild(btn_true_blue);

	btn_false_blue = ImageView::create(BUTTON_ANSWER_CORRECT);
	btn_false_blue->setPosition(Vec2(screen_size.width * 2 / 3, screen_size.height / 5));
	btn_false_blue->setAnchorPoint(Vec2(0.5, 0.5));
	btn_false_blue->ignoreContentAdaptWithSize(false);
	btn_false_blue->setContentSize(screen_size / 10);
	float btn_false_blue_scalex = 1;
	while (btn_false_blue->getBoundingBox().size.width < textFalse->getBoundingBox().size.width + 40) {
		btn_false_blue_scalex = btn_false_blue_scalex + 0.001;
		btn_false_blue->setScaleX(btn_false_blue_scalex);
	}
	
	btn_false_blue->setVisible(false);
	addChild(btn_false_blue);
	

	
	// button answer wrong
	btn_false_red = ImageView::create(BUTTON_ANSWER_WRONG);
	btn_false_red->setPosition(Vec2(screen_size.width * 2 / 3, screen_size.height / 5));
	btn_false_red->setAnchorPoint(Vec2(0.5, 0.5));
	btn_false_red->ignoreContentAdaptWithSize(false);
	btn_false_red->setContentSize(screen_size / 11);
	btn_false_red->setVisible(false);
	float btn_false_red_scalex = 1;
	while (btn_false_red->getBoundingBox().size.width < textTrue->getBoundingBox().size.width + 40) {
		btn_false_red_scalex = btn_false_red_scalex + 0.001;
		btn_false_red->setScaleX(btn_false_red_scalex);
	}
	btn_false_red->setScaleX(btn_false_red_scalex - 0.02);
	addChild(btn_false_red);

	btn_true_red = ImageView::create(BUTTON_ANSWER_WRONG);
	btn_true_red->setPosition(Vec2(screen_size.width / 3, screen_size.height / 5));
	btn_true_red->setAnchorPoint(Vec2(0.5, 0.5));
	btn_true_red->ignoreContentAdaptWithSize(false);
	btn_true_red->setContentSize(screen_size / 10);
	btn_true_red->setVisible(false);
	float btn_true_red_scalex = 1;
	while (btn_true_red->getBoundingBox().size.width < textTrue->getBoundingBox().size.width + 40) {
		btn_true_red_scalex = btn_true_red_scalex + 0.001;
		btn_true_red->setScaleX(btn_true_red_scalex);
	}
	addChild(btn_true_red);
	
	//show button true and false
	btn_select_true = ImageView::create(BUTTON_ANSWER);
	btn_select_true->setVisible(false);
	btn_select_true->setPosition(Vec2(screen_size.width / 3, screen_size.height / 5));
	btn_select_true->setAnchorPoint(Vec2(0.5, 0.5));
	btn_select_true->ignoreContentAdaptWithSize(false);
	btn_select_true->setContentSize(screen_size / 10);
	float textTruescalex = 1;
	while (btn_select_true->getBoundingBox().size.width < textTrue->getBoundingBox().size.width + 40) {
		textTruescalex = textTruescalex + 0.001;
		btn_select_true->setScaleX(textTruescalex);
	}
	
	addChild(btn_select_true);

	btn_select_false = ImageView::create(BUTTON_ANSWER);
	btn_select_false->setVisible(false);
	btn_select_false->setPosition(Vec2(screen_size.width * 2 / 3, screen_size.height / 5));
	btn_select_false->setAnchorPoint(Vec2(0.5, 0.5));
	btn_select_false->ignoreContentAdaptWithSize(false);
	btn_select_false->setContentSize(screen_size / 10);
	float textFalsescalsex = 1;
	while (btn_select_false->getBoundingBox().size.width < textFalse->getBoundingBox().size.width + 40) {
		textFalsescalsex = textFalsescalsex + 0.001;
		btn_select_false->setScaleX(textFalsescalsex);
	}
	addChild(btn_select_false);

	panel_image = utils::findChild<Layout*>(this, "panel_image_question");
	this->showQuestion();
	
}

void DanhGia::showQuestion() {
	//test
	this->unschedule("gameTutorial");
	AUDIO_ENGINE->stopEffect(audio_id_question);
	//gen question
	disableTouch();	
	auto text_question = utils::findChild<Text*>(this, "Text_question");
	text_question->setFontName(font_name);
	text_question->setFontSize(50);
	text_question->setText(data_game[current_question].ques.text);
	
	//gen answer
	/*auto image_smile = utils::findChild<ImageView*>(this, "Image_smile");
	image_smile->loadTexture(data_game[current_question].ques.path_smile);
	auto text_smile = utils::findChild<Text*>(this, "Text_smile");
	
	text_smile->setText(data_game[current_question].ques.text_smile);
    if (data_game[current_question].ques.text_smile.size() > 21){
        text_smile->setFontSize(25);
    }*/
	/*auto image_sad = utils::findChild<ImageView*>(this, "Image_sad");
	image_sad->loadTexture(data_game[current_question].ques.path_sad);
	auto text_sad = utils::findChild<Text*>(this, "Text_sad");
	text_sad->setText(data_game[current_question].ques.text_sad);
    if (data_game[current_question].ques.text_sad.size() > 21){
        text_sad->setFontSize(25);
    }*/
	// render
	auto panel_size = panel_image->getContentSize();
	cocos2d::Sprite * cutzone = cocos2d::Sprite::create("games/DanhGia/mask.png");
	auto item_size = cutzone->getContentSize();
	model = ui::ImageView::create(data_game[current_question].images_question.front().image_path);
	model->setScale(1.6);
	float modelScale = model->getScaleX();
	while (model->getBoundingBox().size.width < panel_image->getBoundingBox().size.width - 20) {
		modelScale = modelScale + 0.01;
		model->setScale(modelScale);
	}
	panel_image->addChild(model, INT_MAX - 1);
	cutzone->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	model->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	cutzone->setPosition(item_size / 2);
	model->setPosition(panel_size / 2);
	auto size_model = model->getContentSize();
	cocos2d::RenderTexture * render = cocos2d::RenderTexture::create(item_size.width, item_size.height);
	render->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	render->beginWithClear(1, 1, 1, 1);
	cutzone->setBlendFunc(cocos2d::BlendFunc{ GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA });
	cutzone->visit();
	render->end();
	render->getSprite()->setPosition(Vec2(panel_size.width / 2, panel_size.height / 2 + 5));
	render->getSprite()->setScaleX(0.95*(panel_image->getContentSize().width / item_size.width));
	render->getSprite()->setScaleY(0.9*(panel_image->getContentSize().height / item_size.height));
	render->setName("render");
	panel_image->addChild(render);
	//
	panel_image->setScale(0);
	panel_image->runAction(EaseIn::create(ScaleTo::create(0.5, 1.f), 1.5));
	AUDIO_ENGINE->playEffect("sounds/DanhGia/turn.mp3");
    if (data_game[current_question].ques.intro_question.empty()){
        startGame();
    }else{
        AUDIO_ENGINE->play(data_game[current_question].ques.intro_question, false, 1.0f,[=]() {
            this->scheduleOnce([=](float dt) {
                startGame();
            }, 0.5f, cocos2d::StringUtils::format("start"));
        });
    }
}
void DanhGia::startGame() {
	this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(GAME_TUTORIAL, false, [=]() {
			audio_id_question = AUDIO_ENGINE->play(data_game[current_question].ques.audio, false, 1.0f, [=]() {
				this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
					btn_select_true->setVisible(true);
					textTrue->setVisible(true);
					btn_sound->setTouchEnabled(false);
					AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_smile, false, ([=] {
						AUDIO_ENGINE->playSound("sounds/DanhGia/or.mp3", false, ([=] {
							btn_select_false->setVisible(true);
							textFalse->setVisible(true);
							AUDIO_ENGINE->playSound(data_game[current_question].ques.audio_sad, false, [=]() {
								this->unschedule("gameTutorial");
								gameTutorial();
								enableTouch();
								model->setTouchEnabled(true);
								}, 1.0f);
							}));
						}));
					}), NULL));
				pictureClick();
				buttonTrueClick();
				buttonFalseClick();
				radio();
				});
			}, 1.0f);
		}), NULL));
	
    
}
void DanhGia::nextGame() {
    if (cocos2d::FileUtils::getInstance()->isFileExist(BUTTON_ANSWER)) {
            btn_select_false->loadTexture(BUTTON_ANSWER);
            btn_select_true->loadTexture(BUTTON_ANSWER);
        }
	data_game[current_question].images_question.erase(data_game[current_question].images_question.begin());
	if (data_game[current_question].images_question.size() > 0) {
		panel_image->removeChildByName("render");
		auto panel_size = panel_image->getContentSize();
		cocos2d::Sprite * cutzone = cocos2d::Sprite::create("games/DanhGia/mask.png");
		auto item_size = cutzone->getContentSize();
		model = ImageView::create(data_game[current_question].images_question.front().image_path);
		model->setScale(0.6);
		float modelScale = model->getScaleX();
		while (model->getBoundingBox().size.width < panel_image->getBoundingBox().size.width - 20) {
			modelScale = modelScale + 0.01;
			model->setScale(modelScale);
		}
		panel_image->addChild(model);
		btn_select_false->setVisible(true);
		btn_select_true->setVisible(true);
		model->setVisible(true);
		cutzone->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		model->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		cutzone->setPosition(item_size / 2);
		model->setPosition(panel_size / 2);
		auto size_model = model->getContentSize();
		cocos2d::RenderTexture * render = cocos2d::RenderTexture::create(item_size.width, item_size.height);
		render->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

		render->beginWithClear(1, 1, 1, 1);
		cutzone->setBlendFunc(cocos2d::BlendFunc{ GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA });
		cutzone->visit();
		render->end();
		render->getSprite()->setPosition(Vec2(panel_size.width/2,panel_size.height/2+5));
		render->getSprite()->setScaleX(0.95*(panel_image->getContentSize().width / item_size.width));
		render->getSprite()->setScaleY(0.9*(panel_image->getContentSize().height / item_size.height));
		render->setName("render");
		panel_image->addChild(render);
		panel_image->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.5, 1.f), 1.5), CallFunc::create([=]() {
			enableTouch();
		}), NULL));
		AUDIO_ENGINE->playEffect("sounds/DanhGia/turn.mp3");
	}
	else {
		current_question++;
		if (current_question < data_game.size()) {
			showQuestion();
		}
		else {
			this->scheduleOnce([=](float dt) {
				onFinishGame();
			}, 1.f, cocos2d::StringUtils::format("endgame"));
		}
	}
}

void DanhGia::onExitTransitionDidStart() {
	AUDIO_ENGINE->stopAllSound();
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void DanhGia::showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path) {
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
		hand->play(false, [=](gaf::GAFObject *obj) {
			hand->runAction(Sequence::create(MoveTo::create(0.6f, pos2),
				CallFunc::create([=]() {
				hand->play(false, [=](gaf::GAFObject *obj) {
					hand->removeFromParent();
				});
			}), NULL));
		});
	}, delay_time, 100, delay_time, "animated_guide");
}

void DanhGia::hideTapGuideGame() {
	Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
}

void DanhGia::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame();
	}, 7, 100, delay_time, "animated_guide");
}

void DanhGia::onFinishGame() {
    MJMainGBase::onFinishGame();
}

ui::Widget::ccWidgetClickCallback DanhGia::onLocateClickCallback(const std::string &callback_name) {
	return nullptr;
}

void DanhGia::enableTouch() {
	btn_select_false->setTouchEnabled(true);
	btn_select_true->setTouchEnabled(true);
	btn_sound->setTouchEnabled(true);
}
void DanhGia::disableTouch() {
	btn_select_false->setTouchEnabled(false);
	btn_select_true->setTouchEnabled(false);
	btn_sound->setTouchEnabled(false);
}

void DanhGia::gameTutorial() {
	schedule([=](float dt) {
		btn_sound->setTouchEnabled(false);
		model->setTouchEnabled(false);
		AUDIO_ENGINE->playSound(GAME_TUTORIAL, false, [=]() {
			btn_sound->setTouchEnabled(true);
			model->setTouchEnabled(true);
			});
		}, 12.0f, CC_REPEAT_FOREVER, 10.0f, "gameTutorial");
	
}

void DanhGia::pictureClick() {
	model->addClickEventListener([=](cocos2d::Ref* sender) {
		AUDIO_ENGINE->stopAllSound();
		this->unschedule("gameTutorial");
		model->setTouchEnabled(false);
		btn_sound->setTouchEnabled(false);
		audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=]() {
			model->setTouchEnabled(true);
			btn_sound->setTouchEnabled(true);
			gameTutorial();
			});
		});
}

void DanhGia::buttonTrueClick() {
	btn_select_true->addClickEventListener([=](cocos2d::Ref* sender) {
		AUDIO_ENGINE->stopAllSound();
		this->unschedule("gameTutorial");
		//btn_smile->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
		if (data_game[current_question].images_question.front().is_smile) {
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
			disableTouch();
			btn_true_blue->setVisible(true);
			btn_select_true->setVisible(false);
			AUDIO_ENGINE->playEffect("sounds/DanhGia/correct.mp3");
			
			btn_sound->setTouchEnabled(true);
			panel_image->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.15f)), CallFunc::create([=]() {
				auto audio_path = is_speak_answer ? data_game[current_question].ques.audio_smile : data_game[current_question].images_question.front().audio;
				AUDIO_ENGINE->playSound(audio_path, false, [=]() {
					panel_image->runAction(Sequence::create(CallFunc::create([=] {
						AUDIO_ENGINE->playSound("sounds/DanhGia/Correct!.mp3");
						}), DelayTime::create(1.0f),ScaleTo::create(0.3, 0.f), CallFunc::create([=]() {
							btn_true_blue->setVisible(false);
							btn_select_false->setVisible(false);
							textTrue->setVisible(false);
							textFalse->setVisible(false);
						model->setVisible(false);
						nextGame();
						}), NULL));
					});
				}), NULL));
			btn_select_true->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {

				}), NULL));
		}
		else {
			//btn_smile->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
			btn_sound->setTouchEnabled(true);
			model->setTouchEnabled(true);
			btn_select_true->setTouchEnabled(false);
			btn_select_false->setTouchEnabled(false);
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
			AUDIO_ENGINE->playSound("sounds/DanhGia/wrong.mp3", false, [=]() {
				gameTutorial();
				}, 1.0f);

			btn_select_true->runAction(Sequence::create(CallFunc::create([=] {
				btn_true_red->setVisible(true);
				btn_select_true->setVisible(false);
				btn_select_true->setVisible(false);
				}), DelayTime::create(1.0f), CallFunc::create([=] {
					btn_true_red->setVisible(false);
					btn_select_true->setVisible(true);
					btn_select_true->setTouchEnabled(true);
					btn_select_false->setTouchEnabled(true);
					}), NULL));

			
			btn_sound->setTouchEnabled(true);
			panel_image->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(-20, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), NULL));
			
		}
		});
}

void DanhGia::buttonFalseClick() {
	btn_select_false->addClickEventListener([=](cocos2d::Ref* sender) {
		AUDIO_ENGINE->stopAllSound();
		this->unschedule("gameTutorial");
		if (!data_game[current_question].images_question.front().is_smile) {
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
			disableTouch();
			btn_false_blue->setVisible(false);
			btn_select_false->setVisible(true);		
			AUDIO_ENGINE->playEffect("sounds/DanhGia/correct.mp3");			
			btn_sound->setTouchEnabled(true);
			panel_image->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.15f)), CallFunc::create([=]() {
				btn_select_false->loadTexture(BUTTON_ANSWER_CORRECT);
				auto audio_path = is_speak_answer ? data_game[current_question].ques.audio_sad : data_game[current_question].images_question.front().audio;
				AUDIO_ENGINE->playSound(audio_path, false, [=]() {					
					panel_image->runAction(Sequence::create(CallFunc::create([=] {						
						AUDIO_ENGINE->playSound("sounds/DanhGia/Correct!.mp3");
						}), DelayTime::create(1.0f),ScaleTo::create(0.3, 0.f), CallFunc::create([=]() {
							btn_false_blue->setVisible(false);
							btn_select_true->setVisible(false);
							btn_select_true->setVisible(false);
							textFalse->setVisible(false);
							textTrue->setVisible(false);
							btn_select_false->setVisible(false);
						model->setVisible(false);
						nextGame();
						}), NULL));
					});
				}), NULL));
			btn_select_false->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
				
				}), NULL));
		}
		else {
			//btn_sad->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
			btn_sound->setTouchEnabled(true);
			model->setTouchEnabled(true);
			btn_select_false->setTouchEnabled(false);
			btn_select_true->setTouchEnabled(false);
			AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
			AUDIO_ENGINE->playSound("sounds/DanhGia/wrong.mp3", false, [=]() {
				gameTutorial();
				}, 1.0f);
			panel_image->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(-20, 0))), EaseBackOut::create(MoveBy::create(0.1f, Vec2(10, 0))), NULL));

			btn_select_false->runAction(Sequence::create(CallFunc::create([=] {
				//btn_false_red->setVisible(true);
				//btn_select_false->setVisible(false);
				if (cocos2d::FileUtils::getInstance()->isFileExist(BUTTON_ANSWER_WRONG)) {
					btn_select_false->loadTexture(BUTTON_ANSWER_WRONG);
				}
				}), DelayTime::create(1.0f), CallFunc::create([=] {
					//btn_false_red->setVisible(false);
					//btn_select_false->setVisible(true);
					btn_select_false->setTouchEnabled(true);
					if (cocos2d::FileUtils::getInstance()->isFileExist(BUTTON_ANSWER)) {
						btn_select_false->loadTexture(BUTTON_ANSWER);
					}
					btn_select_true->setTouchEnabled(true);
					}), NULL));
		}
		});
}

void DanhGia::radio() {
	btn_sound->addClickEventListener([=](cocos2d::Ref* sender) {
		this->unschedule("gameTutorial");
		btn_sound->setTouchEnabled(false);
		model->setTouchEnabled(false);
		btn_select_true->setTouchEnabled(false);
		btn_select_false->setTouchEnabled(false);
		btn_false_red->setTouchEnabled(false);
		btn_true_red->setTouchEnabled(false);
		btn_sound->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.2, 1.3f)), ScaleTo::create(0.2f, 1.f), NULL));
		audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=]() {
			btn_sound->setTouchEnabled(true);
			btn_select_true->setTouchEnabled(true);
			btn_select_false->setTouchEnabled(true);
			model->setTouchEnabled(true);
			gameTutorial();
			});
		});
}



