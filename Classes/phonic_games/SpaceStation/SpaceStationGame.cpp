#include "SpaceStationGame.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "Shake.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
#include "APProfileManager.h"
#include "TextLeaf.h"

#define font_name "fonts/KGWHATTHETEACHERWANTS.TTF"
#define font_sz 40
#define image_game "games/spacestation/Re-fueling spacecraft.png"

SpaceStationGame* SpaceStationGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = SpaceStationGame::create();
	if (json_file.length() > 5) {
		result->m_configPath = json_file;
	}
	return result;
}

bool SpaceStationGame::init()
{
	if (!MJMainGBase::init())
		return false;
	initBackground();
	initObjects();
	return true;
}

void SpaceStationGame::onEnter()
{
	MJMainGBase::onEnter();
	scheduleUpdate();
	playBackgroundMusic("sounds/spacestation/bgm.mp3");
	AUDIO_ENGINE->playSound("sounds/spacestation/Re-fueling_Spacecraft.mp3", false, [=]() {
		if (re_Fueling_Spacecraft)
		{
			re_Fueling_Spacecraft->runAction(FadeOut::create(1.0f));
		}
		this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=] {
			AUDIO_ENGINE->playSound("sounds/spacestation/intro.mp3");
		}), NULL));
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			startGame();
		}), NULL));
		});
}

void SpaceStationGame::onExit()
{
	stopGuideGame();
	MJMainGBase::onExit();
	unscheduleUpdate();
}

void SpaceStationGame::update(float dt)
{
	MJMainGBase::update(dt);
	auto pos = m_bgNode->getPosition();
	auto scale = m_bgNode->getScale();
	//    m_planetNode->setPosition(pos.x * 0.1f, pos.y);
	m_planetNode->setScale((scale - 1) * 0.8 + 1);
	m_starNode->setPosition(pos.x * 0.5f, pos.y);
	m_starNode->setScale((scale - 1) * 0.95 + 1);
	m_gameNode->setPosition(pos);
}

SpaceStationGame::SpaceStationGame() :
	m_configPath("json/spacestation/config.json"),
	m_bgNode(nullptr),
	m_planetNode(nullptr),
	m_gameNode(nullptr),
	m_starNode(nullptr),
	m_astronaut(nullptr),
	m_ship(nullptr),
	m_leftLight(nullptr),
	m_smoke(nullptr),
	m_rightLight(nullptr),
	m_numberOfDetaches(0),
	m_clickable(false),
	m_energy(nullptr)
{
}

void SpaceStationGame::initBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_bgNode = cocos2d::Node::create();
	m_bgNode->setPosition(0, visibleSize.height / 2);
	addChild(m_bgNode);

	initStars();

	m_planetNode = cocos2d::Node::create();
	m_planetNode->setPosition(m_bgNode->getPosition());
	addChild(m_planetNode);

	m_gameNode = cocos2d::Node::create();
	m_gameNode->setPosition(0, -visibleSize.height / 2);
	addChild(m_gameNode);

	for (auto i = 0; i < 3; i++) {
		auto dark = Sprite::create("games/spacestation/3.png");
		dark->setPosition(dark->getContentSize().width * (i * 0.99f + 0.5f), 0);
		m_bgNode->addChild(dark);
	}

	auto planet = Sprite::create("games/spacestation/2.png");
	planet->setPosition(planet->getContentSize().width / 2, 0);
	m_planetNode->addChild(planet);

	m_bgNode->setOpacity(200);

	// setup touch move camera
 //   if(isEnableMovingScreen()){
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SpaceStationGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SpaceStationGame::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SpaceStationGame::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_bgNode);
	//    }

		// click block layer
	auto clickBlock = cocos2d::Node::create();
	addChild(clickBlock, 5);
	auto blockListener = cocos2d::EventListenerTouchOneByOne::create();
	blockListener->setSwallowTouches(true);
	blockListener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
		return !m_clickable;
	};
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(blockListener, clickBlock);

	m_speakable = false;
	speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	speaker->setPosition(Vec2(70, visibleSize.height - 60));
	speaker->addClickEventListener([=](cocos2d::Ref* sender) {
		stopGuideGame();
		guideGame();
		if (m_speakable)
			AUDIO_ENGINE->playSound(m_sentence.audio_path);
		});
	this->addChild(speaker);

	//backgound game
	re_Fueling_Spacecraft = cocos2d::ui::ImageView::create();

	if (re_Fueling_Spacecraft)
	{
		re_Fueling_Spacecraft->loadTexture(image_game);

		re_Fueling_Spacecraft->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		re_Fueling_Spacecraft->setPosition(visibleSize / 2);
		re_Fueling_Spacecraft->setName("re_Fueling_Spacecraft");
		this->addChild(re_Fueling_Spacecraft, 19000000000);
	}
}

void SpaceStationGame::initStars()
{
	m_starNode = cocos2d::Node::create();
	m_starNode->setPosition(m_bgNode->getPosition());
	addChild(m_starNode);

	for (auto i = 0; i < 3; i++) {
		auto star = Sprite::create("games/spacestation/1.png");
		star->setPosition(star->getContentSize().width * (i + 0.5), 0);
		m_starNode->addChild(star);

		star->runAction(RepeatForever::create(Sequence::createWithTwoActions(
			FadeTo::create(0.5f, 100),
			FadeTo::create(0.5f, 200)
		)));
	}
}

void SpaceStationGame::initObjects()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto station = GAFWrapper::createGAF("gaf/spacestation/station.gaf");
	station->play(true);
	m_gameNode->addChild(station);

	auto energy = GAFWrapper::createGAF("gaf/spacestation/energy.gaf");
	energy->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
	energy->play(true);
	energy->setPosition(cocos2d::Vec2(20, -6));
	energy->setVisible(false);
	m_gameNode->addChild(energy);
	m_energy = energy;

	auto upperLayer = Sprite::create("games/spacestation/upper_layer.png");
	upperLayer->setPosition(40, -3);
	m_gameNode->addChild(upperLayer, INT_MAX - 1);

	auto astronaut = GAFWrapper::createGAF("gaf/spacestation/astronaut.gaf");
	astronaut->play(true);
	astronaut->setPosition(cocos2d::Vec2(235, -300));
	m_gameNode->addChild(astronaut);
	m_astronaut = astronaut;

	auto light = Sprite::create("games/spacestation/6.png");
	light->setPosition(131, -154);
	light->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		FadeTo::create(0.25f, 10),
		FadeTo::create(0.25f, 255)
	)));
	m_gameNode->addChild(light);

	light = Sprite::create("games/spacestation/6.png");
	light->setFlippedY(true);
	light->setPosition(131, 160);
	light->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		FadeTo::create(0.25f, 10),
		FadeTo::create(0.25f, 255)
	)));
	m_gameNode->addChild(light);

	auto ship = GAFWrapper::createGAF("gaf/spacestation/ship.gaf");
	ship->setPosition(cocos2d::Vec2(visibleSize.width, -24));
	m_gameNode->addChild(ship);
	m_ship = ship;

	auto leftLight = Sprite::create("games/spacestation/5.png");
	leftLight->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
	leftLight->setPosition(77, -13);
	leftLight->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		FadeTo::create(1, 10),
		FadeTo::create(1, 255)
	)));
	m_gameNode->addChild(leftLight);
	m_leftLight = leftLight;

	auto rightLight = Sprite::create("games/spacestation/5.png");
	rightLight->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_RIGHT);
	rightLight->setFlippedX(true);
	m_gameNode->addChild(rightLight);
	rightLight->setPosition(1658, -8);
	rightLight->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		FadeTo::create(1, 10),
		FadeTo::create(1, 255)
	)));
	m_rightLight = rightLight;

	auto smoke = GAFWrapper::createGAF("gaf/spacestation/smoke.gaf");
	smoke->play(true);
	smoke->setPosition(cocos2d::Vec2(164, 16));
	smoke->setVisible(false);
	m_smoke = smoke;
	m_gameNode->addChild(smoke, 2);

	// 	auto target = energy;
	// 	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	// 	listener->setSwallowTouches(true);
	// 	listener->onTouchBegan = CC_CALLBACK_2(SpaceStationGame::onTouchBegan, this);
	// 	listener->onTouchMoved = [=](Touch* touch, Event* unused) {
	// 		target->setPosition(target->getPosition() + touch->getLocation() - m_lastTouchLocation);
	// 		m_lastTouchLocation = touch->getLocation();
	// 		CCLOG("%f %f", target->getPosition().x, target->getPosition().y);
	// 	};
	// 	listener->onTouchEnded = CC_CALLBACK_2(SpaceStationGame::onTouchEnded, this);
	// 
	// 	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
	// 
	// 	auto kblistener = EventListenerKeyboard::create();
	// 	kblistener->onKeyPressed = [=](EventKeyboard::KeyCode kc, Event*) {
	// 		switch (kc)
	// 		{
	// 		case EventKeyboard::KeyCode::KEY_UP_ARROW:
	// 			target->setPosition(target->getPosition() + cocos2d::Vec2(0, 1));
	// 			break;
	// 		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	// 			target->setPosition(target->getPosition() + cocos2d::Vec2(0, -1));
	// 			break;
	// 		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	// 			target->setPosition(target->getPosition() + cocos2d::Vec2(-1, 0));
	// 			break;
	// 		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	// 			target->setPosition(target->getPosition() + cocos2d::Vec2(1, 0));
	// 			break;
	// 		default:
	// 			break;
	// 		}
	// 		CCLOG("%f %f", target->getPosition().x, target->getPosition().y);
	// 	};
	// 	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(kblistener, target);
}

void SpaceStationGame::getGameData()
{
	m_images = math::resource::ResHelper::getImages("icons.json");

	auto jsonString = FileUtils::getInstance()->getStringFromFile(m_configPath);
	if (!jsonString.length())
		return;

	using namespace rapidjson;
	// 	std::string schemaPath = "json/spacestation/schema.json";
	// 	std::string schemaJson = FileUtils::getInstance()->getStringFromFile(schemaPath);
	// 	if (!schemaJson.length())
	// 		return;
	// 
	// 	Document sd;
	// 	if (sd.Parse(schemaJson.c_str()).HasParseError())
	// 		return;
	// 
	// 	SchemaDocument schema(sd);

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

	auto games = resultDocument["games"].GetArray();
	if (!games.Size())
		return;

	m_words.clear();
	m_detachIndex.clear();
	//m_wordsClone.clear();

	if (m_game_index >= games.Size()) {
		speaker->setTouchEnabled(false);
		MJMainGBase::onFinishGame();
		return;
	}

	auto singleGame = games[m_game_index++].GetObject();
	auto words = singleGame["words"].GetArray();

	auto sentence_Img = math::resource::ResHelper::getImage(m_images, singleGame["sentence"].GetString());
	if (sentence_Img) {
		m_sentence.image_path = sentence_Img->path;
		m_sentence.text = sentence_Img->getText("name_1");
		m_sentence.audio_path = sentence_Img->getAudios("name_1").front();
	}
	//m_audioPath = singleGame["audio"].GetString();

	for (rapidjson::SizeType i = 0; i < words.Size(); i++) {
		auto singleWord = words[i].GetObject();
		// 		WordSync ws;
		// 		ws.word = singleWord["word"].GetString();
		// 		ws.spawnTime = singleWord["time"].GetFloat();
		// 		auto answerObjects = singleWord["answers"].GetArray();
		// 		for (rapidjson::SizeType j = 0; j < answerObjects.Size(); j++) {
		// 			ws.answers.push_back(answerObjects[j].GetString());
		// 		}

		if (singleWord.HasMember("isDetach")) {
			auto is_detach = singleWord["isDetach"].GetBool();
			if (is_detach) {
				m_detachIndex.push_back(i);
			}
		}
		m_numberOfDetaches = m_detachIndex.size();

		auto wordString = singleWord["word"].GetString();
		auto pImage = math::resource::ResHelper::getImage(m_images, wordString);
		mj::GameObject go;
		if (pImage) {
			go.text = pImage->getText("name_1");
			go.audio_path = pImage->getAudios("name_1").front();
		}
		else {
			go.text = wordString;
			go.audio_path = std::string("sounds/spacestation/words/") + wordString + ".mp3";
		}
		m_words.push_back(go);
	}

	sentence.text_normal = m_sentence;
	sentence.text_slow = m_sentence;
	sentence.text_part = m_words;
	sentence.text_part_slow = m_words;

	initPipes();
	playStartingScript();
}

void SpaceStationGame::startGame()
{
	getGameData();
}

void SpaceStationGame::initPipes()
{
	for (auto it = m_pipes.begin(); m_pipes.end() != it;) {
		(*it)->removeFromParent();
		it = m_pipes.erase(it);
	}
	for (auto it = m_pipeSlots.begin(); m_pipeSlots.end() != it;) {
		(*it)->removeFromParent();
		it = m_pipeSlots.erase(it);
	}
	for (auto i = 0; i < m_words.size(); i++) {
		auto pipe = SpacePipe::create();
		pipe->setPosition(190 + (i - ((int)m_words.size())) * 190, 0);
		CCLOG("%f %i", pipe->getPosition().x, m_words.size());
		pipe->setText(m_words[i]);
		pipe->hideText();
		m_gameNode->addChild(pipe);
		m_pipes.push_back(pipe);
	}

	m_energy->setScaleX((m_words.size() + 1) * 190.0f / 1350);

	auto pos = m_ship->getPosition();
	pos.x = 230 + m_words.size() * 190;
	pos.y = -24;
	m_ship->setPosition(pos);
}

void SpaceStationGame::playStartingScript()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_leftLight->setScaleX(0);
	m_rightLight->setScaleX(0);
	m_ship->setScale(0.75f);
	m_ship->setRotation(0);
	m_bgNode->setPosition(0, visibleSize.height / 2);
	m_bgNode->setScale(1.3f);
	m_gameNode->setScale(1.3f);
	m_bgNode->runAction(ScaleTo::create(1.5, 1));
	m_gameNode->runAction(ScaleTo::create(1.5, 1));
	m_astronaut->setPosition(400, -600);
	m_smoke->setVisible(false);
	m_energy->setVisible(false);
	index_sentence = 0;

	//create image and sentence
	cocos2d::ui::ImageView* img = cocos2d::ui::ImageView::create(m_sentence.image_path);
	img->setContentSize(Size(600, 400));
	img->ignoreContentAdaptWithSize(false);
	m_bgNode->addChild(img, 1000);
	img->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.8));
	img->setGlobalZOrder(100);

	text_panel = cocos2d::ui::Layout::create();

	float start_x = 0;
	for (int i = 0; i < m_words.size(); i++) {
		cocos2d::ui::Text* text = cocos2d::ui::Text::create(m_words[i].text, font_name, font_sz);
		text->setAnchorPoint(Point(0, 0));
		text_panel->addChild(text);
		text->setPosition(Vec2(start_x, 0));
		text->enableOutline(Color4B::BLACK, 2);
		text->setGlobalZOrder(100);
		text->setTag(i);
		start_x += text->getContentSize().width + 10;
	}

	text_panel->setContentSize(Size(start_x, 100));
	text_panel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	m_bgNode->addChild(text_panel);
	text_panel->setPosition(Vec2(visibleSize.width / 2, -visibleSize.height * 0.7));

	Vector<FiniteTimeAction*> actions;
	if (m_game_index == 1)
		actions.pushBack(DelayTime::create(2.0f)); // delay intro
	actions.pushBack(DelayTime::create(1.5f));
	actions.pushBack(CallFunc::create([=]() {
		m_leftLight->runAction(ScaleTo::create(0.2f, 1, 1));
		}));
	actions.pushBack(DelayTime::create(0.15f));
	actions.pushBack(CallFunc::create([=]() {
		for (auto it = m_pipes.begin(); it != m_pipes.end(); it++) {
			(*it)->runAction(EaseOut::create(MoveBy::create(1.6f, cocos2d::Vec2(190 * m_pipes.size(), 0)), 1.3f));
		}
		AUDIO_ENGINE->playSound("sounds/spacestation/fadein.mp3");
		}));
	//actions.pushBack(DelayTime::create(0.5f));
	actions.pushBack(CallFunc::create([=]() {
		m_ship->runAction(Sequence::createWithTwoActions(
			DelayTime::create(1),
			EaseOut::create(ScaleTo::create(0.4f, 1), 1.3f)
		));
		}));

	if (isEnableMovingScreen()) {
		actions.pushBack(EaseOut::create(MoveBy::create(2, cocos2d::Vec2(-m_ship->getPosition().x + visibleSize.width, 0)), 1.3f));
	}
	actions.pushBack(DelayTime::create(0.5f));

	actions.pushBack(CallFunc::create([=]() {
		m_rightLight->runAction(ScaleTo::create(0.2f, 1, 1));
		}));
	actions.pushBack(DelayTime::create(1));
	if (isEnableMovingScreen()) {
		actions.pushBack(EaseOut::create(MoveBy::create(1, cocos2d::Vec2(m_ship->getPosition().x - visibleSize.width, 0)), 1.3f));
	}

	actions.pushBack(CallFunc::create([=] {
		img->runAction(EaseBackOut::create(MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height * 0.15))));
		text_panel->runAction(EaseBackOut::create(MoveTo::create(0.5, Vec2(visibleSize.width / 2, -visibleSize.height * 0.25))));
		}));
	actions.pushBack(CallFunc::create([=] {
		readSentence([=] {
			Vector<FiniteTimeAction*> actions;
			auto delay_time = 1.0f;
			actions.pushBack(DelayTime::create(delay_time));
			actions.pushBack(CallFunc::create([=] {
				img->runAction(EaseBackIn::create(MoveTo::create(1.0, Vec2(visibleSize.width / 2, visibleSize.height * 1.2))));
				text_panel->runAction(EaseBackIn::create(MoveTo::create(0.8, Vec2(visibleSize.width / 2, -visibleSize.height * 0.7))));

				}));
			actions.pushBack(CallFunc::create([=]() {
				detachAndReveal([=]() {
					m_speakable = true;
					if (isEnableMovingScreen())
						m_clickable = true;
					});
				}));

			actions.pushBack(DelayTime::create(m_numberOfDetaches));
			if (isEnableMovingScreen()) {
				actions.pushBack(EaseOut::create(MoveBy::create(1, cocos2d::Vec2(m_ship->getPosition().x - visibleSize.width, 0)), 1.3f));
			}

			actions.pushBack(CallFunc::create([=]() {
				m_astronaut->runAction(EaseOut::create(MoveTo::create(1, cocos2d::Vec2(235, -300)), 1.3f));
				AUDIO_ENGINE->playSound("sounds/spacestation/astronaut.mp3");
				}));
			actions.pushBack(DelayTime::create(1));
			actions.pushBack(CallFunc::create([]() {
				AUDIO_ENGINE->playSound("sounds/spacestation/weld.mp3", true);
				}));
			actions.pushBack(CallFunc::create([=]() {
				this->guideGame();
				}));
			if (flag) {
				actions.pushBack(CallFunc::create([=]() {
					this->inActiveMode();
					}));
			}
			m_bgNode->runAction(Sequence::create(actions));
			});
		}));

	//   auto delay_time = 3;
	//   actions.pushBack(DelayTime::create(delay_time));
	//   actions.pushBack(CallFunc::create([=]{
	//       img->runAction(EaseBackIn::create(MoveTo::create(1.0, Vec2(visibleSize.width/2, visibleSize.height*1.2))));
	//       text_panel->runAction(EaseBackIn::create(MoveTo::create(0.8, Vec2(visibleSize.width/2, -visibleSize.height*0.7))));
	//       
	//   }));
	   //actions.pushBack(CallFunc::create([=]() {
	   //	detachAndReveal([=]() {
	   //		m_clickable = true;
	   //	});
	   //}));
	//   
	   //actions.pushBack(DelayTime::create(m_numberOfDetaches));
	//   if(isEnableMovingScreen()){
	//       actions.pushBack(EaseOut::create(MoveBy::create(1, cocos2d::Vec2(m_ship->getPosition().x - visibleSize.width, 0)), 1.3f));
	//   }
	//   
	   //actions.pushBack(CallFunc::create([=]() {
	   //	m_astronaut->runAction(EaseOut::create(MoveTo::create(1, cocos2d::Vec2(235, -300)), 1.3f));
	   //	AUDIO_ENGINE->playSound("sounds/spacestation/astronaut.mp3");
	   //}));
	   //actions.pushBack(DelayTime::create(1));
	   //actions.pushBack(CallFunc::create([]() {
	   //	AUDIO_ENGINE->playSound("sounds/spacestation/weld.mp3",true);
	   //}));

	m_bgNode->runAction(Sequence::create(actions));
}

void SpaceStationGame::playEndingScript()
{
	flag = false;
	this->unschedule("inActive_Mode");
	stopGuideGame();
	this->hideTapGuideGame();
	m_clickable = false;
	m_speakable = false;
	AUDIO_ENGINE->stopSound(m_sentence.audio_path);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vector<FiniteTimeAction*> actions;
	auto old_pos = m_planetNode->getPosition();

	actions.pushBack(DelayTime::create(1.5f));
	actions.pushBack(CallFunc::create([=]() {
		m_astronaut->runAction(EaseIn::create(MoveTo::create(1.5f, cocos2d::Vec2(400, -1200)), 1.3f));
		AUDIO_ENGINE->playSound("sounds/spacestation/sfx2_opt.mp3");
		AUDIO_ENGINE->stopSound("sounds/spacestation/weld.mp3");
		}));
	if (isEnableMovingScreen()) {
		actions.pushBack(EaseInOut::create(MoveTo::create(0.8f, cocos2d::Vec2(0, visibleSize.height / 2)), 1.3f));
	}
	actions.pushBack(CallFunc::create([=]() {
		m_smoke->setVisible(true);
		m_energy->setVisible(true);
		}));
	actions.pushBack(CallFunc::create([=]() {
		AUDIO_ENGINE->playSound("sounds/spacestation/powerup.mp3");
		m_gameNode->runAction(Shake::create(2.5f, 20, 20));
		}));
	actions.pushBack(Shake::create(2.5f, 20, 20));
	actions.pushBack(DelayTime::create(1));

	if (isEnableMovingScreen()) {
		actions.pushBack(EaseOut::create(MoveTo::create(2.5f, cocos2d::Vec2(-m_ship->getPosition().x + visibleSize.width, visibleSize.height / 2)), 1.3f));
	}
	actions.pushBack(CallFunc::create([=] {
		if (m_sentence.audio_path == "") {
			index_sentence = 0;
			readSentence();
		}
		else {
			AUDIO_ENGINE->playSound(m_sentence.audio_path, false, [=]() {
				for (auto it = m_pipes.begin(); it != m_pipes.end(); it++) {
					(*it)->runAction(MoveBy::create(3.5f, cocos2d::Vec2(-((int)m_pipes.size() + 1) * 190, 0)));
				}
				});
		}
		}));
	actions.pushBack(DelayTime::create(1.0f));
	actions.pushBack(CallFunc::create([]() {
		AUDIO_ENGINE->playSound("sounds/spacestation/sfx1_opt.mp3");
		}));
	actions.pushBack(DelayTime::create(0.5f));
	actions.pushBack(CallFunc::create([=]() {
		m_energy->setVisible(false);
		m_rightLight->runAction(ScaleTo::create(0.25f, 0, 1));
		}));
	actions.pushBack(DelayTime::create(4.25f));
	actions.pushBack(CallFunc::create([=]() {
		auto pos = m_ship->getParent()->convertToWorldSpace(m_ship->getPosition());
		m_ship->retain();
		m_ship->removeFromParentAndCleanup(false);
		addChild(m_ship, 5);
		m_ship->release();
		m_ship->setPosition(pos);
		auto posx = m_ship->getPosition().x;
		auto gameScale = 0.4f;
		m_gameNode->runAction(ScaleTo::create(1, gameScale));
		m_ship->runAction(MoveTo::create(0.7f, visibleSize / 2));
		m_ship->runAction(ScaleTo::create(0.7f, gameScale));
		AUDIO_ENGINE->playSound("sounds/spacestation/ship.mp3");
		}));
	actions.pushBack(ScaleTo::create(0.7, 0.95f));
	actions.pushBack(CallFunc::create([=]() {
		m_ship->runAction(RotateTo::create(0.8f, 90));
		}));
	actions.pushBack(DelayTime::create(0.8f));
	actions.pushBack(CallFunc::create([=]() {
		m_ship->playSequence("fly", true);
		m_ship->runAction(Sequence::createWithTwoActions(
			EaseOut::create(MoveBy::create(0.8f, cocos2d::Vec2(-50, 0)), 1.6f),
			EaseIn::create(MoveBy::create(1.5f, cocos2d::Vec2(2500, 0)), 1.6f)
		));

		m_planetNode->runAction(EaseInOut::create(MoveBy::create(2.0f, cocos2d::Vec2(-2000, 0)), 1.6f));
		}));

	actions.pushBack(DelayTime::create(1.0f));
	actions.pushBack(EaseIn::create(MoveBy::create(1.5f, cocos2d::Vec2(-1600, 0)), 1.3f));
	actions.pushBack(DelayTime::create(2.5));
	actions.pushBack(CallFunc::create([=]() {
		m_ship->retain();
		m_ship->removeFromParentAndCleanup(false);
		m_gameNode->addChild(m_ship);
		m_ship->release();
		m_ship->setLocalZOrder(0);

		m_planetNode->setPosition(old_pos);
		startGame();
		//MJMainGBase::onFinishGame();
		}));
	m_bgNode->runAction(Sequence::create(actions));
}

void SpaceStationGame::readSentence(std::function<void()> callback) {
	if (m_words.size() == index_sentence) {
		if (callback)
			callback();
		return;
	}

	//int p_id = AUDIO_ENGINE->play(m_words[index_sentence].audio_path);
	//auto text_array = text_panel->getChildren();
	auto text = (cocos2d::ui::Text*)text_panel->getChildByTag(index_sentence);// text_array.at(index_sentence);
	if (text) text->setTextColor(Color4B::RED);
	AUDIO_ENGINE->playSound(m_words[index_sentence].audio_path, false, [=]() {
		if (text) text->setTextColor(Color4B::WHITE);
		index_sentence++;
		this->readSentence(callback);
		});
}

void SpaceStationGame::detachAndReveal(std::function<void()> callback)
{
	std::vector<SpacePipe*> clonePipes(m_pipes);
	//    std::random_shuffle(clonePipes.begin(), clonePipes.end());
	std::vector<mj::GameObject> cloneWords;
	for (auto i = 0; i < m_numberOfDetaches; i++) {
		cloneWords.push_back(clonePipes[m_detachIndex[i]]->getText());
	}
	mj::helper::shuffle(cloneWords);
	/*	cocos2d::Vec2 pospool[] = {
			cocos2d::Vec2(220,260),
			cocos2d::Vec2(486,266),
			cocos2d::Vec2(742,193),
			cocos2d::Vec2(950,279),
			cocos2d::Vec2(1136,174),
			cocos2d::Vec2(1251,-169),
			cocos2d::Vec2(1007,-305),
			cocos2d::Vec2(563,-170),
			cocos2d::Vec2(351,-272)
		}*/;

		std::vector<cocos2d::Vec2> pospool;
		for (int i = 0; i < m_pipes.size() - 1; ++i) {
			auto top_x = 220 + i * 240 + cocos2d::random(-10, 10);
			auto top_y = cocos2d::random(170, 300);

			auto bot_x = 220 + i * 240 + cocos2d::random(-10, 10);
			auto bot_y = cocos2d::random(-300, -170);
			if (top_x > 110 + m_pipes.size() * 200) break;
			pospool.push_back(Vec2(top_x, top_y));
			pospool.push_back(Vec2(bot_x, bot_y));
		}

		std::vector<cocos2d::Vec2> randomPool(pospool);// , pospool + sizeof(pospool) / sizeof(cocos2d::Vec2));
		mj::helper::shuffle(randomPool);
		std::vector<cocos2d::Vec2> detachPos;
		for (auto i = 0; i < m_numberOfDetaches; i++) {
			auto pos = cocos2d::Vec2::ZERO;
			auto it = randomPool.begin();
			do
			{
				if (it == randomPool.end()) break;
				pos = *it;
				auto path = pos - clonePipes[m_detachIndex[i]]->getPosition();
				if (path.length() > 400) {
					it++;
					continue;
				}
				detachPos.push_back(*it);
				randomPool.erase(it);
				break;
			} while (true);
		}

		while (detachPos.size() < m_numberOfDetaches)
		{
			int index = cocos2d::random(0, (int)randomPool.size() - 1);
			detachPos.push_back(*(randomPool.begin() + index));
			randomPool.erase(randomPool.begin() + index);
		}
		for (auto i = 0; i < m_numberOfDetaches; i++) {
			clonePipes[m_detachIndex[i]]->runAction(Sequence::createWithTwoActions(
				DelayTime::create(i),
				CallFunc::create([=]() {
					auto slot = SpacePipeSlot::create();
					slot->setPosition(clonePipes[m_detachIndex[i]]->getPosition());
					slot->setWord(clonePipes[m_detachIndex[i]]->getText());
					m_gameNode->addChild(slot);
					m_pipeSlots.push_back(slot);

					clonePipes[m_detachIndex[i]]->detach(detachPos[i]);

					clonePipes[m_detachIndex[i]]->setLocalZOrder(2);
					clonePipes[m_detachIndex[i]]->setText(cloneWords[i]);
					clonePipes[m_detachIndex[i]]->setReleaseCallback(CC_CALLBACK_1(SpaceStationGame::onDropPipe, this));
					})
			));
			auto width = (m_ship->getPositionX() - Director::getInstance()->getVisibleSize().width) / m_numberOfDetaches;
			if (isEnableMovingScreen()) {
				m_bgNode->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5 * i), EaseOut::create(MoveBy::create(0.5, cocos2d::Vec2(-width, 0)), 1.3f)));
			}
		}
		runAction(Sequence::createWithTwoActions(
			DelayTime::create(m_numberOfDetaches),
			CallFunc::create([=]() {
				for (auto it = m_pipes.begin(); it != m_pipes.end(); it++) {
					(*it)->revealText();
				}
				if (callback)
					callback();
				})
		));
}

void SpaceStationGame::onDropPipe(SpacePipe* pipe)
{
	stopGuideGame();
	guideGame();
	// check overlap
	if (ap::ProfileManager::getInstance().getInformationOfProfile().age < _age_check) {
		auto slot = m_pipeSlots[i];
		if (slot->getWord() == pipe->getText()) {
			for (auto it = m_pipeSlots.begin(); it != m_pipeSlots.end();) {
				auto slot = *it;
				pipe->getPosition();
				auto move = MoveTo::create(0.3f, slot->getPosition());
				pipe->runAction(Sequence::create(move, nullptr));
				pipe->attach(slot->getPosition());
				pipe->setLocalZOrder(1);
				slot->removeFromParent();

				this->unschedule("inActive_Mode");
				it = m_pipeSlots.erase(it);

				if (!m_pipeSlots.size()) {
					//ending
					this->hideTapGuideGame();
					playEndingScript();
					return;
				}
				return;
			}
		}
	}
	else
	{
		for (auto it = m_pipeSlots.begin(); it != m_pipeSlots.end();) {
			auto slot = *it;
			auto path = pipe->getPosition() - slot->getPosition();
			if (path.length() > 150.0f) {
				it++;
				continue;
			}

			if (slot->getWord() == pipe->getText()) {
				//correct
				this->unschedule("inActive_Mode");
				pipe->attach(slot->getPosition());
				pipe->setLocalZOrder(1);
				slot->removeFromParent();
				it = m_pipeSlots.erase(it);

				if (!m_pipeSlots.size()) {
					//ending
					this->hideTapGuideGame();
					playEndingScript();
					return;
				}
				return;
			}
			else {
				this->unschedule("inActive_Mode");
				inActiveMode();
				auto wrongPipe = Sprite::create("games/spacestation/4_2.png");
				//wrongPipe->setPosition(wrongPipe->getContentSize() / 2);
				pipe->addChild(wrongPipe);

				//fade
				Vector<FiniteTimeAction*> actions;
				actions.pushBack(DelayTime::create(0.1f));
				actions.pushBack(FadeOut::create(1.2f));
				actions.pushBack(CallFunc::create([wrongPipe]() {
					wrongPipe->removeFromParent();
					}));
				wrongPipe->runAction(Sequence::create(actions));

				pipe->detach();
				AUDIO_ENGINE->playSound("sounds/spacestation/wrong.mp3");
				it++;

				return;
			}
		}
	}
	if (std::abs(pipe->getPosition().y) < 150) {
		pipe->detach();
	}
}

bool SpaceStationGame::onTouchBegan(Touch* touch, Event* unusedEvent)
{
	if (!m_clickable)
		return false;
	this->hideTapGuideGame();
	this->unschedule("inActive_Mode");
	m_lastTouchLocation = touch->getLocation();
	return true;
}

void SpaceStationGame::onTouchMoved(Touch* touch, Event* unusedEvent)
{
	static auto visibleSize = Director::getInstance()->getVisibleSize();
	auto location = touch->getLocation();
	auto posDelta = location - m_lastTouchLocation;
	posDelta.y = 0;

	// predict camera
	auto predictPos = m_bgNode->getPosition() + posDelta;
	if (predictPos.x > 0)
		predictPos.x = 0;
	auto rightEdge = -m_ship->getPosition().x + visibleSize.width;
	if (predictPos.x < rightEdge)
		predictPos.x = rightEdge;
	m_bgNode->setPosition(predictPos);
	m_lastTouchLocation = location;
}

void SpaceStationGame::onTouchEnded(Touch* touch, Event* unusedEvent)
{
	//this->inActiveMode();
}

bool SpaceStationGame::isEnableMovingScreen() {
	if (m_words.size() > 5 && Director::getInstance()->getWinSize().width < 110 + m_words.size() * 200) return true;
	return false;
}

void SpaceStationGame::inActiveMode() {

	this->schedule([=](float) {
		auto rand = cocos2d::random(0, (int)m_pipeSlots.size() - 1);
		auto slot = m_pipeSlots[rand];

		for (int i = 0; i < m_pipes.size(); i++) {
			auto pipe = m_pipes[i];
			if (slot->getWord() == pipe->getText()) {
				if (ap::ProfileManager::getInstance().getInformationOfProfile().age >= _age_check) {
					this->showDragGuideGame(pipe->getParent()->convertToWorldSpace(pipe->getPosition()), slot->getParent()->convertToWorldSpace(slot->getPosition()), 0);
					AUDIO_ENGINE->playSound("sounds/spacestation/Drag the words to complete the sentence.mp3");
				}
			}
		}
		}, 10, "inActive_Mode");
}

void SpaceStationGame::guideGame() {
	this->schedule([=](float) {
		AUDIO_ENGINE->playSound("sounds/spacestation/Drag the words to complete the sentence.mp3");
	}, 10, 100, 10, "guidegame");
}

void SpaceStationGame::stopGuideGame() {
	this->unschedule("guidegame");
	AUDIO_ENGINE->stopSound("sounds/spacestation/Drag the words to complete the sentence.mp3");
}
