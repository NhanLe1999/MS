#include "CarOnHighwayGame.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "HSAudioEngine.h"
#include "Shake.h"

CarOnHighwayGame* CarOnHighwayGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = CarOnHighwayGame::create();
	if (json_file.length() > 5)
		result->m_configPath = json_file;
	return result;
}

bool CarOnHighwayGame::init()
{
	if (!MJMainGBase::init())
		return false;

	initBackground();
	initCar();

	return true;
}

CarOnHighwayGame::CarOnHighwayGame() :
	m_configPath("json/caronhighway/config.json"),
	m_bgNode(nullptr),
	m_sideObjectNode(nullptr),
	m_gameNode(nullptr),
	m_car(nullptr),
	m_clickable(false),
	m_laneIndex(1)
{
	srand(time(NULL));
}

void CarOnHighwayGame::initBackground()
{
	auto offsetWrap = cocos2d::Node::create();
	offsetWrap->setPosition(0, 100);
	addChild(offsetWrap);

	auto bgNode = cocos2d::Node::create();
	offsetWrap->addChild(bgNode);
	m_bgNode = bgNode;

	auto sideObjectNode = cocos2d::Node::create();
	bgNode->addChild(sideObjectNode, 1);
	m_sideObjectNode = sideObjectNode;

	auto gameNode = cocos2d::Node::create();
	offsetWrap->addChild(gameNode);
	m_gameNode = gameNode;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create("games/caronhighway/0.png");
	bg->setPosition(visibleSize / 2);
	bgNode->addChild(bg);
	if (bg->getContentSize().width < visibleSize.width + 180)
	{
		bg->setScaleX((visibleSize.width + 180) / bg->getContentSize().width);
	}

	bg = Sprite::create("games/caronhighway/2.png");
	bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	bg->setScale(1.15f);
	bg->setPosition(visibleSize / 2);
	bgNode->addChild(bg);
	if (bg->getContentSize().width < visibleSize.width + 180)
	{
		bg->setScaleX((visibleSize.width + 180) / bg->getContentSize().width);
	}

	bg = Sprite::create("games/caronhighway/3.png");
	bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	bg->setPosition(visibleSize / 2);
	bgNode->addChild(bg);
	if (bg->getContentSize().width < visibleSize.width + 180)
	{
		bg->setScaleX((visibleSize.width + 180) / bg->getContentSize().width);
	}

	bg = Sprite::create("games/caronhighway/4.png");
	bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
	bg->setPosition(cocos2d::Vec2(0, 10) + visibleSize / 2);
	bgNode->addChild(bg);

	if (bg->getContentSize().width < visibleSize.width + 180)
	{
		bg->setScaleX((visibleSize.width + 180) / bg->getContentSize().width);
	}

	auto road = GAFWrapper::createGAF("gaf/caronhighway/road.gaf");
	road->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
	road->setPosition(cocos2d::Vec2(0, 32) + visibleSize / 2);
	road->play(true);
	bgNode->addChild(road);

	auto leftSpeed = GAFWrapper::createGAF("gaf/caronhighway/speed.gaf");
	leftSpeed->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT);
	leftSpeed->setPosition(cocos2d::Vec2(visibleSize.width / 2 + 42, -122));
	leftSpeed->play(true);
	bgNode->addChild(leftSpeed);

	auto rightSpeed = GAFWrapper::createGAF("gaf/caronhighway/speed.gaf");
	rightSpeed->setFlippedX(true);
	rightSpeed->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT);
	rightSpeed->setPosition(cocos2d::Vec2(visibleSize.width / 2 - 43, -122));
	rightSpeed->play(true);
	bgNode->addChild(rightSpeed);

	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);
	_speaker->addClickEventListener([=](Ref*) {
		stopGuiding();
		if (!m_clickable)
			return;
		std::vector<std::string> tapChain;
		auto path_tap = "sounds/caronhighway/tap_on.mp3";
		if (CONFIG_MANAGER->isGameVMonkey()) {
			path_tap = (hoc_am_hoc_van == "hoc_am") ? "sounds/caronhighway/tap_on_vi_am.mp3" : "sounds/caronhighway/tap_on_vi_van.mp3";
		}
		tapChain.push_back(path_tap);
		tapChain.push_back(m_correctAnswer.audio_path);
		AUDIO_ENGINE->playChain(tapChain);
		});
	auto backgound_game = Sprite::create("games/caronhighway/highway racer.png");
	backgound_game->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	backgound_game->setPosition(Vec2(visibleSize.width/2, visibleSize.height/1.5));
	backgound_game->setName("highway_racer.png");
	this->addChild(backgound_game, 19000000000);
}

void CarOnHighwayGame::initCar()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto car = GAFWrapper::createGAF("gaf/caronhighway/car.gaf");
	car->setScale(0.85f);
	car->playSequence("idle_front", true);
	car->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2 - 180));
	m_gameNode->addChild(car, 1);
	m_car = car;

	schedule([=](float) {
		createSideObject();
		}, 0.5f, "side_object");
}

void CarOnHighwayGame::createSideObject()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 leftPos(visibleSize.width / 2 - 50, visibleSize.height / 2 + 10);
	cocos2d::Vec2 rightPos(visibleSize.width / 2 + 50, visibleSize.height / 2 + 10);
	auto duration = 1.7f;

	auto random = (rand() % 5) + 9;
	auto sideObject = Sprite::create(std::string("games/caronhighway/") + cocos2d::StringUtils::toString(random) + ".png");
	sideObject->setScale(0);
	m_sideObjectNode->addChild(sideObject);
	ActionInterval* spawn = nullptr;
	if (rand() % 2) {
		sideObject->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT);
		sideObject->setPosition(leftPos);
		spawn = Spawn::createWithTwoActions(MoveTo::create(duration, cocos2d::Vec2(-200, visibleSize.height / 2 + 10)),
			ScaleTo::create(duration, 1));
	}
	else {
		sideObject->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
		sideObject->setPosition(rightPos);
		spawn = Spawn::createWithTwoActions(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width + 200, visibleSize.height / 2 + 10)),
			ScaleTo::create(duration, 1));
	}

	spawn = EaseIn::create(spawn, 1.5f);
	sideObject->runAction(Sequence::createWithTwoActions(spawn, CallFunc::create([=]() {
		sideObject->removeFromParent();
		})));

	// move object to bottom
	auto& children = m_sideObjectNode->getChildren();
	for (auto it = children.begin(); it != children.end(); it++) {
		if (*it == sideObject) {
			children.erase(it);
			children.insert(0, sideObject);
			break;
		}
	}
}

void CarOnHighwayGame::getGameData()
{
	auto jsonString = FileUtils::getInstance()->getStringFromFile(m_configPath);
	if (!jsonString.length())
		return;

	using namespace rapidjson;

	// 	std::string schemaPath = "json/caronhighway/schema.json";
	// 	std::string schemaJson = FileUtils::getInstance()->getStringFromFile(schemaPath);
	// 	if (!schemaJson.length())
	// 		return;


	// 	Document sd;
	// 	if (sd.Parse(schemaJson.c_str()).HasParseError())
	// 		return;

	/*	SchemaDocument schema(sd);*/

	Document resultDocument;
	if (resultDocument.Parse(jsonString.c_str()).HasParseError())
		return;

	// 	SchemaValidator sv(schema);
	// 	if (!resultDocument.Accept(sv)) {
	// 		StringBuffer sb;
	// 		sv.GetInvalidSchemaPointer().StringifyUriFragment(sb);
	// 		CCLOG("Invalid schema: %s\n", sb.GetString());
	// 		CCLOG("Invalid keyword: %s\n", sv.GetInvalidSchemaKeyword());
	// 		sb.Clear();
	// 		sv.GetInvalidDocumentPointer().StringifyUriFragment(sb);
	// 		CCLOG("Invalid document: %s\n", sb.GetString());
	// 		return;
	// 	}

	m_questions.clear();
	if (resultDocument.HasMember("option_vm_phonic") && resultDocument["option_vm_phonic"].IsString()) {
		hoc_am_hoc_van = resultDocument["option_vm_phonic"].GetString();
	}
	auto games = resultDocument["games"].GetArray();
	for (rapidjson::SizeType i = 0; i < games.Size(); i++) {
		CarOnHighwayQuestion question;
		auto gameObject = games[i].GetObject();
		auto answers = gameObject["answers"].GetArray();
		auto correctAnswer = gameObject["correctAnswer"].GetString();
		{
			auto pImage = math::resource::ResHelper::getImage(m_images, correctAnswer);
			mj::GameObject go;
			if (pImage) {
				go.text = pImage->getText("name_1");
				go.audio_path = pImage->getAudios("name_1").front();
			}
			else {
				go.text = correctAnswer;
				go.audio_path = std::string("sounds/caronhighway/words/") + correctAnswer + ".mp3";
			}
			question.correctAnswer = go;
		}

		for (rapidjson::SizeType j = 0; j < answers.Size(); j++) {
			auto answer = answers[j].GetString();
			mj::GameObject go;
			auto pImage = math::resource::ResHelper::getImage(m_images, answer);
			if (pImage) {
				go.text = pImage->getText("name_1");
				go.audio_path = pImage->getAudios("name_1").front();
			}
			else {
				go.text = answer;
				go.audio_path = std::string("sounds/caronhighway/words/") + answer + ".mp3";
			}
			question.answers.push_back(go);
		}
		m_questions.push_back(question);
	}
}

void CarOnHighwayGame::startGame()
{
	getGameData();
	playStartingScript();
}

void CarOnHighwayGame::playStartingScript()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto duration = 1.8f;
	m_bgNode->setContentSize(visibleSize);
	m_bgNode->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_bgNode->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
	m_bgNode->setScale(1.3f);

	m_car->playSequence("idle_front", true);
	m_car->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2 - 180));
	m_car->setScale(1.8f);
	m_car->setVisible(true);
	auto baseCarPos = m_car->getPosition();
	m_car->setPosition(baseCarPos - cocos2d::Vec2(0, 300));
	m_laneIndex = 1;

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(CallFunc::create([=]() {
		m_car->runAction(MoveTo::create(duration, baseCarPos));
		m_car->runAction(ScaleTo::create(duration, 1));
		this->runAction(Sequence::create(CallFunc::create([=] {
			AUDIO_ENGINE->playSound("sounds/caronhighway/engine.mp3");
			}), DelayTime::create(1.0f), CallFunc::create([=] {
				AUDIO_ENGINE->playSound("sounds/caronhighway/car_on_highway.mp3",false, [=]() {
					this->removeChildByName("highway_racer.png");
					});
				}), NULL));
		}));
	actions.pushBack(Spawn::createWithTwoActions(
		ScaleTo::create(duration, 1),
		MoveTo::create(duration, visibleSize / 2)
	));
	actions.pushBack(CallFunc::create([=]() {
		m_bgNode->setContentSize(cocos2d::Size::ZERO);
		m_bgNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		m_bgNode->setPosition(cocos2d::Vec2::ZERO);
		}));
	actions.pushBack(DelayTime::create(1.5f));
	actions.pushBack(CallFunc::create([=]() {
		showNextQuestion();
		}));
	m_bgNode->runAction(Sequence::create(actions));
	AUDIO_ENGINE->playSound("sounds/caronhighway/carloop.mp3", true);
}

void CarOnHighwayGame::playEndingScript()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vector<FiniteTimeAction*> actions;
	// init gaf
	auto gaf_carsmoke = GAFWrapper::createGAF("gaf/caronhighway/car smoke/car smoke.gaf");
	//auto gaf_carsmoke = GAFWrapper::createGAF("gaf/caronhighway/car smoke/car smoke.gaf");
	m_gameNode->addChild(gaf_carsmoke, 2);
	gaf_carsmoke->setVisible(false);

	auto smoke_transititon = GAFWrapper::createGAF("gaf/caronhighway/smoketransition/smoketransition.gaf");
	//auto smoke_transititon = GAFWrapper::createGAF("gaf/caronhighway/smoketransition/smoketransition.gaf");
	smoke_transititon->setPosition(screen_size / 2);
	this->addChild(smoke_transititon, 100);

	//	auto ending_confetti = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["caronhighway_ending_confetti"]);
	//	//auto ending_confetti = GAFWrapper::createGAF("gaf/caronhighway/ending_confetti/ending_confetti.gaf");
	//	ending_confetti->setPosition(screen_size / 2);
	//	this->addChild(ending_confetti, 100);
	//
	//	auto ending_fireworks = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["caronhighway_ending_fireworks"]);
	//	//auto ending_fireworks = GAFWrapper::createGAF("gaf/caronhighway/ending_fireworks/ending_fireworks.gaf");
	//	ending_fireworks->setPosition(screen_size / 2);
	//	ending_fireworks->setVisible(false);
	//	this->addChild(ending_fireworks, 100);
	//
	//	auto ending_text = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache["caronhighway_ending_text"]);
	//	//auto ending_text = GAFWrapper::createGAF("gaf/caronhighway/ending_text/ending_text.gaf");
	//	ending_text->setPosition(Vec2(screen_size.width / 2 - 250, screen_size.height / 2 + 450));
	//	this->addChild(ending_text, 100);
		// clear cache gaf
		//PHONIC_MANAGER->clearCacheGaf();
		//1 reset position
	actions.pushBack(CallFunc::create([=]() {
		m_bgNode->setContentSize(visibleSize);
		m_bgNode->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		m_bgNode->setPosition(m_bgNode->getPosition() + visibleSize / 2);
		}));
	// 2 move car
	actions.pushBack(CallFunc::create([=]() {
		m_car->runAction(ScaleTo::create(1, 0.85f));
		switch (m_laneIndex)
		{
		case 0:
			m_car->runAction(MoveBy::create(1, cocos2d::Vec2(50, 60)));
			break;
		case 1:
			m_car->runAction(MoveBy::create(1, cocos2d::Vec2(0, 50)));
			break;
		case 2:
			m_car->runAction(MoveBy::create(1, cocos2d::Vec2(-50, 60)));
			break;
		}
		}));
	// 3 zoom in
	actions.pushBack(ScaleTo::create(1, 1.4f));
	// 4 play nitro effect
	actions.pushBack(CallFunc::create([=]() {
		switch (m_laneIndex)
		{
		case 0:
			m_car->playSequence("nitro_left", true);
			break;
		case 1:
			m_car->playSequence("nitro_front", true);
			break;
		case 2:
			m_car->playSequence("nitro_right", true);
			break;
		}
		}));
	// 5 delay
	//actions.pushBack(DelayTime::create(0.025f));
	// 6 play smoke effect
	actions.pushBack(CallFunc::create([=]() {
		m_car->runAction(ScaleTo::create(0.5f, 0.1f));
		gaf_carsmoke->setPosition(m_car->getPosition() + Vec2(0, -150));
		gaf_carsmoke->setVisible(true);
		switch (m_laneIndex)
		{
		case 0:
			gaf_carsmoke->playSequence("smoke_left");
			m_car->runAction(MoveBy::create(1, cocos2d::Vec2(170, -160)));
			break;
		case 1:
			gaf_carsmoke->playSequence("smoke_center");
			m_car->runAction(MoveBy::create(1, cocos2d::Vec2(0, -40)));
			break;
		case 2:
			gaf_carsmoke->playSequence("smoke_right");
			m_car->runAction(MoveBy::create(1, cocos2d::Vec2(-170, -160)));
			break;
		}
		AUDIO_ENGINE->playSound("sounds/caronhighway/engine.mp3");
		}));

	// play
	m_bgNode->stopAllActions();
	m_bgNode->runAction(Sequence::create(Sequence::create(actions),
		Spawn::createWithTwoActions(
			// 7.1  play smoke transition and ending
			Sequence::createWithTwoActions(
				DelayTime::create(0),
				CallFunc::create([=]() {
					smoke_transititon->play(false, [=](gaf::GAFObject* obj) {
						onFinishGame();
						});
					})),
			// 7.2 move back ground
						MoveBy::create(1, cocos2d::Vec2(0, -300))
						),
		CallFunc::create([=]() {
						m_car->setVisible(false);
						AUDIO_ENGINE->stopSound("sounds/caronhighway/loop_car.mp3");
			}),
						NULL));
}

void CarOnHighwayGame::showQuestion(std::vector<mj::GameObject> answers, mj::GameObject correctAnswer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto pos = visibleSize.width / 4 - 80;
	auto offset = visibleSize.width / 4 + 80;
	m_correctAnswer = correctAnswer;

	mj::helper::shuffle(answers);

	for (auto i = 0; i < answers.size(); i++) {
		auto btn = ui::Button::create("games/caronhighway/button.png");
		btn->setScale9Enabled(true);
		btn->setCapInsets(Rect(21, 21, 250 - 42, 90 - 42));
		btn->setContentSize(cocos2d::Size(250, 90));
		btn->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		btn->setPosition(cocos2d::Vec2(pos, 40));
		btn->setZoomScale(0);
		auto scale = btn->getScale();

		// appearing btn animation
		btn->setScale(0);
		auto delay = DelayTime::create(i * 0.15f);
		auto zoomAppear = ScaleTo::create(0.3f, scale);
		btn->runAction(Sequence::create(delay, zoomAppear, nullptr));

		m_gameNode->addChild(btn, 2);
		pos += offset;
		std::string font_name = CONFIG_MANAGER->isGameVMonkey() ? PHONIC_MANAGER->getInstance()->getGameFontName() : "fonts/KGWHATTHETEACHERWANTS.TTF";
		auto label = Label::create(answers[i].text, font_name, 50.0f);
		label->setPosition(btn->getContentSize() / 2);
		label->setTextColor(Color4B(0xA0, 0x2A, 0x1E, 0xFF));
		btn->addChild(label);

		if (answers[i].text == correctAnswer.text) {
			right_pos = label->getParent()->convertToWorldSpace(label->getPosition());
			//            right_pos = btn->getPosition();
		}
		btn->addClickEventListener([=](Ref*) {
			GAFWrapper* effect = nullptr;
			if (!m_clickable)
				return;
            m_clickable = false;
			this->hideTapGuideGame();
			if (answers[i].text != correctAnswer.text) {
				//
				btn->loadTextureNormal("games/caronhighway/button_wrong.png");
				this->runAction(Sequence::createWithTwoActions(
					Shake::create(0.8f, 5, 5),
					CallFunc::create([=]() {
						btn->loadTextureNormal("games/caronhighway/button.png");
						btn->runAction(ScaleTo::create(0.2f, 1));
						if (!CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->playSound(m_correctAnswer.audio_path);
						this->showTapGuideGame(right_pos);
						this->runAction(Sequence::create(DelayTime::create(8.0f), CallFunc::create([=] {
							AUDIO_ENGINE->playSound(m_correctAnswer.audio_path);
							}), NULL));
						})
				));
				AUDIO_ENGINE->playSound("sounds/caronhighway/wrong.mp3");
				if (CONFIG_MANAGER->isVietnameseStories()) {
					AUDIO_ENGINE->playSound(answers[i].audio_path);
				}
			}
			else {
				//
                if(m_questions.size() > 0)
                {
                    m_questions.erase(m_questions.begin());
                }
				
				effect = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at("effect"));
				effect->play(false);
				effect->setPosition(btn->getPosition());
				m_gameNode->addChild(effect, 2);
				AUDIO_ENGINE->playSound("sounds/caronhighway/correct.mp3");
			}

			btn->stopAllActions();
			auto scaleUp = EaseBounceOut::create(ScaleTo::create(0.3f, 1.2f * scale));
			auto scaleDown = ScaleTo::create(0.3f, scale);
			auto clearAction = CallFunc::create([=]() {
				if (effect)
					effect->removeFromParent();
				if (answers[i] == correctAnswer) {
					this->unschedule("audio_path");
					onCorrectAnswer();
					auto labelPos = label->getPosition();
					labelPos = label->getParent()->convertToWorldSpace(labelPos);
					label->retain();
					label->removeFromParentAndCleanup(false);
					label->setPosition(labelPos);
					label->setTextColor(Color4B::WHITE);
					label->disableEffect(LabelEffect::OUTLINE);
					label->enableOutline(Color4B::BLACK, 3);
					addChild(label);
					label->release();
					Vector<FiniteTimeAction*> actions;
					actions.pushBack(CallFunc::create([=]() {
						label->runAction(EaseOut::create(ScaleTo::create(1, 2.5f), 1.4f));
						}));
					//                    actions.pushBack(DelayTime::create(0.35f));
					actions.pushBack(CallFunc::create([=]() {
						AUDIO_ENGINE->playSound(correctAnswer.audio_path);
						}));
					actions.pushBack(DelayTime::create(0.5f));
					actions.pushBack(CallFunc::create([=]() {
						label->removeFromParent();
						}));
					label->runAction(Sequence::create(actions));
					removeAllAnswerBtns();
				}
				else {

					onWrongAnswer();
				}
				});
			btn->runAction(Sequence::create(scaleUp, scaleDown, DelayTime::create(0.2f), clearAction, nullptr));

			m_clickable = false;
			});
		m_answerBtns.push_back(btn);
	}

	std::vector<std::string> tapChain;
	auto path_tap = "sounds/caronhighway/tap_on.mp3";
	if (CONFIG_MANAGER->isGameVMonkey()) {
		path_tap = (hoc_am_hoc_van == "hoc_am") ? "sounds/caronhighway/tap_on_vi_am.mp3" : "sounds/caronhighway/tap_on_vi_van.mp3";
	}
	if (count_lv == 1) {
		tapChain.push_back(path_tap);
	}
	tapChain.push_back(correctAnswer.audio_path);
	this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playChain(tapChain, [=]() {
			m_clickable = true;
			this->showTapGuideGame(right_pos);
			});
		}), NULL));
	this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
		_speaker->setTouchEnabled(true);
		}), NULL));
}

void CarOnHighwayGame::showNextQuestion()
{
	count_lv++;
	if (!m_questions.size()) {
		playEndingScript();
		return;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto alert = Sprite::create("games/caronhighway/5.png");
	alert->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 135);
	m_gameNode->addChild(alert);

	AUDIO_ENGINE->playSound("sounds/caronhighway/warning_pop.mp3", false);

	Vector<FiniteTimeAction*> actions;
	for (auto i = 0; i < 5; i++) {
		actions.pushBack(FadeOut::create(0.15f));
		actions.pushBack(FadeIn::create(0.15f));
	}

	actions.pushBack(DelayTime::create(3.5f));
	actions.pushBack(CallFunc::create([=]() {
		auto question = m_questions.at(0);
		showQuestion(question.answers, question.correctAnswer);
		}));
	actions.pushBack(FadeOut::create(0.4f));
	actions.pushBack(CallFunc::create([=]() {
		alert->removeFromParent();
		}));
	alert->runAction(Sequence::create(actions));
}

void CarOnHighwayGame::onCorrectAnswer()
{
	_speaker->setTouchEnabled(false);



	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto obstacle = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at("break"));
	auto duration = 1.6f;
	obstacle->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	obstacle->setScale(0.05f);
	switch (m_laneIndex)
	{
	case 0:
		obstacle->setPosition(cocos2d::Vec2(-20, 0) + visibleSize / 2);
		obstacle->runAction(EaseIn::create(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width / 2 - 550, -400)), 1.5f));
		break;
	case 1:
		obstacle->setPosition(visibleSize / 2);
		obstacle->runAction(EaseIn::create(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width / 2, -400)), 1.5f));
		break;
	case 2:
		obstacle->setPosition(cocos2d::Vec2(20, 0) + visibleSize / 2);
		obstacle->runAction(EaseIn::create(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width / 2 + 550, -400)), 1.5f));
		break;
	default:
		break;
	}
	obstacle->runAction(Sequence::create(EaseIn::create(ScaleTo::create(duration, 0.9f), 1.5f), CallFunc::create([=]() {
		m_clickable = true;
		obstacle->removeFromParent();
		showNextQuestion();
		}), nullptr));
	m_bgNode->addChild(obstacle);

	// car animation
	Vector<FiniteTimeAction*> actions;
	int moveDirection = 0;
	int afterLaneIndex = 0;
	if (m_laneIndex == 0) {
		moveDirection = 1;
		afterLaneIndex = 1;
	}
	else if (m_laneIndex == 1) {
		moveDirection = rand() % 2;
		afterLaneIndex = moveDirection * 2;
	}
	else if (m_laneIndex == 2) {
		moveDirection = 0;
		afterLaneIndex = 1;
	}
	actions.pushBack(DelayTime::create(0.25f));
	actions.pushBack(CallFunc::create([=]() {
		if (moveDirection)
			m_bgNode->runAction(MoveBy::create(0.7f, cocos2d::Vec2(-85, 0)));
		else
			m_bgNode->runAction(MoveBy::create(0.7f, cocos2d::Vec2(85, 0)));

		AUDIO_ENGINE->playSound("sounds/caronhighway/avoid.mp3", false, nullptr, 0.3f);
		//        if (rand() % 2)
		//            AUDIO_ENGINE->playSound("sounds/caronhighway/yeah.mp3", false, nullptr, 0.5f);
		}));
	if (moveDirection)
		actions.pushBack(MoveBy::create(0.5f, cocos2d::Vec2(165, 0)));
	else
		actions.pushBack(MoveBy::create(0.5f, cocos2d::Vec2(-165, 0)));
	actions.pushBack(CallFunc::create([=]() {
		std::string sequence;
		switch (afterLaneIndex)
		{
		case 0:
			sequence = "idle_left";
			break;
		case 1:
			sequence = "idle_front";
			break;
		case 2:
			sequence = "idle_right";
			break;
		}
		m_car->playSequence(sequence, true);
		m_laneIndex = afterLaneIndex;
		}));
	m_car->runAction(Sequence::create(actions));
}

void CarOnHighwayGame::onWrongAnswer()
{
	stopGuiding();
	showTapGuideGame(right_pos);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto obstacle = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at("break"));
	auto duration = 0.85f;
	std::string sequence, afterSequence;
	obstacle->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	obstacle->setScale(0.05f);
	switch (m_laneIndex)
	{
	case 0:
		obstacle->setPosition(cocos2d::Vec2(-20, 0) + visibleSize / 2);
		obstacle->runAction(EaseIn::create(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width / 2 - 175, 150)), 1.5f));
		sequence = "crash_left";
		afterSequence = "idle_left";
		break;
	case 1:
		obstacle->setPosition(visibleSize / 2);
		obstacle->runAction(EaseIn::create(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width / 2, 150)), 1.5f));
		sequence = "crash_front";
		afterSequence = "idle_front";
		break;
	case 2:
		obstacle->setPosition(cocos2d::Vec2(20, 0) + visibleSize / 2);
		obstacle->runAction(EaseIn::create(MoveTo::create(duration, cocos2d::Vec2(visibleSize.width / 2 + 175, 150)), 1.5f));
		sequence = "crash_right";
		afterSequence = "idle_right";
		break;
	default:
		break;
	}
	obstacle->runAction(Sequence::create(EaseIn::create(ScaleTo::create(duration, 0.35f), 1.5f), CallFunc::create([=]() {
		m_clickable = true;
		obstacle->playSequence("break", false, [=](gaf::GAFObject* obj, const std::string& callback_name) {
			obstacle->removeFromParent();
			});
		m_car->playSequence(sequence, false, [=](gaf::GAFObject* obj, const std::string& callback_name) {
			m_car->playSequence(afterSequence, true);
			});
		AUDIO_ENGINE->playSound("sounds/caronhighway/crash.mp3");
		}), nullptr));
	m_bgNode->addChild(obstacle);
}

void CarOnHighwayGame::removeAllAnswerBtns()
{
	for (auto it = m_answerBtns.begin(); it != m_answerBtns.end();) {
		auto btn = *it;
		btn->removeFromParent();
		it = m_answerBtns.erase(it);
	}
}

void CarOnHighwayGame::onEnter()
{
	MJMainGBase::onEnter();

	cacheGAF();
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	playBackgroundMusic("sounds/caronhighway/music.mp3", true);
	this->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create([=] {
		auto path_audio = "sounds/caronhighway/intro.mp3";
		if (CONFIG_MANAGER->isGameVMonkey()) {
			path_audio = (hoc_am_hoc_van == "hoc_am") ? "sounds/caronhighway/intro_vi_am.mp3" : "sounds/caronhighway/intro_vi_van.mp3";
		}
		AUDIO_ENGINE->playSound(path_audio);
		}), NULL));
	startGame();
}

void CarOnHighwayGame::onExit()
{
	MJMainGBase::onExit();
	//this->_eventDispatcher->removeEventListenersForTarget(this);
}

void CarOnHighwayGame::onFinishGame()
{
	hideTapGuideGame();
	AUDIO_ENGINE->stopAllAudio();
	MJMainGBase::onFinishGame();
}

void CarOnHighwayGame::onExitTransitionDidStart()
{
	PHONIC_MANAGER->clearCacheGaf();
	MJMainGBase::onExitTransitionDidStart();
}

void CarOnHighwayGame::showTapGuideGame(Vec2 pos, int loop, float delay_time, std::string gaf_guide_path) {
	this->schedule([=](float) {
		_audio = AUDIO_ENGINE->playSound(m_correctAnswer.audio_path);
		}, 8.0f, "audio_path");
}

void CarOnHighwayGame::stopGuiding() {
	this->unschedule("audio_path");
	AUDIO_ENGINE->stopEffect(_audio);	
	showTapGuideGame(right_pos);
}

void CarOnHighwayGame::cacheGAF()
{
	PHONIC_MANAGER->cacheGAF("break", "gaf/caronhighway/break.gaf");
	//PHONIC_MANAGER->cacheGAF("speed", "gaf/caronhighway/speed.gaf");
	PHONIC_MANAGER->cacheGAF("effect", "gaf/caronhighway/effect.gaf");
	//PHONIC_MANAGER->cacheGAF("road", "gaf/caronhighway/road.gaf");
	//PHONIC_MANAGER->cacheGAF("car", "gaf/caronhighway/car.gaf");

	// cache ending gaf
//	PHONIC_MANAGER->cacheGAF("caronhighway_car_smoke", "gaf/caronhighway/car smoke/car smoke.gaf");
//	PHONIC_MANAGER->cacheGAF("caronhighway_smoketransition", "gaf/caronhighway/smoketransition/smoketransition.gaf");
//	PHONIC_MANAGER->cacheGAF("caronhighway_ending_confetti", "gaf/caronhighway/ending_confetti/ending_confetti.gaf");
//	PHONIC_MANAGER->cacheGAF("caronhighway_ending_fireworks", "gaf/caronhighway/ending_fireworks/ending_fireworks.gaf");
//	PHONIC_MANAGER->cacheGAF("caronhighway_ending_text", "gaf/caronhighway/ending_text/ending_text.gaf");

}
