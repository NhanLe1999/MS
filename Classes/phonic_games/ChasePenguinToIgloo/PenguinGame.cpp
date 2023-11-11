#include "PenguinGame.h"
#include "BigFoot.h"
#include "Penguin.h"
#include "StoryConfigManager.h"
#include "AudioEngine.h"
#include "HSAudioEngine.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "ui/CocosGUI.h"
#include <ctime>

#define BIGFOOT_TRIGGER_RANGE 170.0f
#define IGLOO_SCALE 0.7f
#define GUIDES_PLAYED_KEY "CHASE_PENGUIN_GUIDES_PLAYED_KEY"

PenguinGame * PenguinGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = PenguinGame::create();
	if (json_file.length() > 5)
		result->m_configPath = json_file;
	return result;
}

bool PenguinGame::init()
{
	std::srand(std::time(0));
	if (!MJMainGBase::init())
		return false;
	if (!initBackground())
		return false;
	if (!initBigFoot())
		return false;
	//if (!initIgloos())
	//	return false;
	if (!initPenguin())
		return false;
	if (!initEndingNode())
		return false;
	return true;
}

void PenguinGame::onEnter()
{
	MJMainGBase::onEnter();
	scheduleUpdate();
	startGame();
}

void PenguinGame::onExit()
{
	MJMainGBase::onExit();
	unscheduleUpdate();
}

void PenguinGame::update(float delta)
{
	MJMainGBase::update(delta);
	if (m_gameState == PLAYING || m_gameState == PenguinGameState::SCRIPT_GUIDING) {
		checkBigFootContact();
	}	
}

void PenguinGame::startGame()
{
	initIgloos();
	m_touchEnabled = false;
	setGameState(WAITING);
	bool needGuide = true; /*default true*/
	float delayTime = 0;
    if (CONFIG_MANAGER->isGameVMonkey()) needGuide = false;
	auto guidePlayed = MJDefault::getInstance()->getIntegerForKey(GUIDES_PLAYED_KEY, 0);
	if (guidePlayed > 3 || m_scriptGuided) {
		//setGameState(PLAYING);
		needGuide = false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (level_index == 0) {
		if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/chasepenguintoigloo/Penguin_Patrol.mp3")) {
			delayTime = 3;
			AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/Penguin_Patrol.mp3", false, ([=] {
				releaseAllPenguins();
				AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/ohno.mp3", false);
				}));
			//show name game
			if (cocos2d::FileUtils::getInstance()->isFileExist("games/chasepenguintoigloo/Penguin_Patrol.png")) {
				auto gameNameImage = ImageView::create("games/chasepenguintoigloo/Penguin_Patrol.png");
				gameNameImage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				gameNameImage->setScale(2);
				gameNameImage->setOpacity(0);
				this->addChild(gameNameImage);
				gameNameImage->runAction(Sequence::create(FadeIn::create(0.25), DelayTime::create(1.5), RemoveSelf::create(), nullptr));
			}			
		}
		else {
			AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/ohno.mp3", false);
			delayTime = 1.5;
			releaseAllPenguins();
		}
	}

	m_bigFoot->setPosition(cocos2d::Vec2(-m_bigFoot->getContentSize().width, visibleSize.height * 0.6));
	m_bigFoot->setDestinationPoint(m_bigFoot->getPosition());
	
	scheduleOnce([=](float) {
		m_bigFoot->setDestinationPoint(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height*0.6), [=]() {
			setGameState(SCRIPT_GUIDING);
			showGuideGame([=] {
				scheduleAudioAndActiveGuiding();	// schedule 10s
			});

		});
	}, delayTime, "big_foot_initial");
	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		(*it)->setVisible(false);
	}
	schedule([=](float) {
		idleAction();
	}, 5.0f, "idle_action");
	if (level_index != 0) {
		releaseAllPenguins();
	}	
}

void PenguinGame::movePenguinToRandomPosition(Penguin* penguin)
{
	if (!penguin)
		return;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	penguin->setVisible(true);
	penguin->setDestinationPoint(cocos2d::Vec2(
		std::rand() % ((int)(visibleSize.width * 0.8)) + visibleSize.width * 0.1,
		std::rand() % ((int)(visibleSize.height * 0.45)) + visibleSize.height * 0.05
	));
	penguin->setWithinBigFoot(false);
}

void PenguinGame::setGameState(PenguinGameState state)
{
	if (state == m_gameState)
		return;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	switch (state)
	{
	case WAITING:
//        if(MJDEFAULT->getBoolForKey(key_background_music)){
//            AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/bg.mp3", true);
//        }
		break;
	case GUIDING:
		break;
	case PLAYING:
//            inActiveMode();
		break;
	case ENDED:
            if (CONFIG_MANAGER->isVietnameseStories()){
                AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/Gioi qua.mp3",false);
            }
//            hideTapGuideGame();
		break;
	default:
		break;
	}
	m_gameState = state;
	//m_gameNode->setVisible(state != ENDED);
	m_endingNode->setVisible(state == ENDED);
	for (int i = 0; i < m_igloos.size(); i++) {
		m_labels[i]->setVisible(state == PLAYING || state == SCRIPT_GUIDING);
	}
}

void PenguinGame::releaseAllPenguins()
{
	int counter = 0;
	auto offset = cocos2d::Vec2(-10, -90);
	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		auto penguin = *it;
		auto igloo = m_igloos[std::rand() % m_igloos.size()];
		penguin->setPosition(igloo->getPosition() + offset);
		penguin->setDestinationPoint(penguin->getPosition());
		scheduleOnce([=](float) {
			penguin->setVisible(true);
			zoomIgloo(igloo);
			movePenguinToRandomPosition(penguin);
		}, 0.2*(++counter), std::string("abc") + cocos2d::StringUtils::toString(counter));

		scheduleOnce([=](float) {
			// z-order penguin
			std::sort(m_penguins.begin(), m_penguins.end(), [](Penguin* first, Penguin* second) -> bool {
				return first->getDestinationPoint().y > second->getDestinationPoint().y;
			});

			auto c = 1;
			for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
				if ((*it)->isAttached())
					continue;
				(*it)->setLocalZOrder(c++);
			}
		}, 0.2 * counter, "setZOrder");
	}
}

bool PenguinGame::checkWinCondition()
{
	//if (m_gameState != PLAYING)
	//	return false;

	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		auto penguin = *it;
		if (penguin->isVisible())
			return false;
	}
	return true;
}

void PenguinGame::endGame()
{
	setGameState(WAITING);
	m_touchEnabled = false;
	this->unschedule("guiding");
	AUDIO_ENGINE->stopAllSound();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto destinationPos = m_bigFoot->getPosition();
	destinationPos.x = visibleSize.width + m_bigFoot->getContentSize().width;
	AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/end_music.mp3", false);
	AUDIO_ENGINE->stopSound("sounds/chasepenguintoigloo/bg.mp3");
	scheduleOnce([=](float) {
		setGameState(ENDED);
		m_bigFoot->setDesiredSpeed(ANIMAL_MOVE_SPEED);
		AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/bigfoot_hooray.mp3", false);
		auto moveUp = cocos2d::MoveTo::create(0.2f, visibleSize / 2);
		auto delayAction1 = cocos2d::DelayTime::create(2.0f);
		auto moveDown = cocos2d::MoveBy::create(0.2f, cocos2d::Vec2(0, -visibleSize.height * 0.9));
		auto newGameAction = cocos2d::CallFunc::create([=]() {
			//startGame();
            if(randomIndex == max_game){
                MJMainGBase::onCloseGame();
            }else{
                randomIndex++;
				level_index++;
                this->startGame();
            }
			
		});

		//m_endingBigFoot->runAction(cocos2d::Sequence::create(moveUp, delayAction1, moveDown, newGameAction, nullptr));
		this->runAction(Sequence::create(delayAction1, ([=] {newGameAction; }), nullptr));
		m_bigFoot->setDesiredSpeed(ANIMAL_MOVE_SPEED);
	}, 1.23f, "end_schedule");
	m_bigFoot->setDesiredSpeed(2500);
	m_bigFoot->setDestinationPoint(destinationPos);
}

void PenguinGame::idleAction()
{
	if (m_gameState != PLAYING)
		return;

	std::vector<Penguin*> activePenguins;
	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		if ((*it)->isVisible() && (*it)->isAttached() == false)
			activePenguins.push_back(*it);
	}

	if (activePenguins.size() == 0)
		return;
	auto penguin = activePenguins.at(std::rand() % activePenguins.size());
	movePenguinToRandomPosition(penguin);
}

PenguinGame::PenguinGame() :
	m_gameNode(nullptr),
	m_endingNode(nullptr),
	m_touchEnabled(false),
    m_gameState(PenguinGameState::NONE),
	m_endingBigFoot(nullptr),
	m_correctAnswer(-1),
	m_scriptGuided(false),
	m_configPath("json/chasepenguintoigloo/config.json")
{
}

bool PenguinGame::initBackground()
{
	// image background
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bgSprite = ImageView::create("games/chasepenguintoigloo/bg.jpg");
	bgSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	bgSprite->addClickEventListener([=](Ref*) {
		CCLOG("cliked background");
		scheduleAudioAndActiveGuiding();
		});
	if (!bgSprite)
		return false;

	float scaleX = bgSprite->getScaleX();
	while (bgSprite->getBoundingBox().size.width < visibleSize.width) {
		scaleX = scaleX + 0.01;
		bgSprite->setScaleX(scaleX);
	}
	float scaleY = bgSprite->getScaleY();
	while (bgSprite->getBoundingBox().size.height < visibleSize.height) {
		scaleY = scaleY + 0.01;
		bgSprite->setScaleY(scaleY);
	}

	bgSprite->setPosition(Vec2(visibleSize.width, visibleSize.height));
	this->addChild(bgSprite);

	// audio background
	AUDIO_ENGINE->playBackgroundMusic("sounds/chasepenguintoigloo/bg.mp3");

	m_gameNode = cocos2d::Node::create();
	this->addChild(m_gameNode);

	m_endingNode = cocos2d::Node::create();
	m_endingNode->setVisible(false);
	this->addChild(m_endingNode);

	// guide btn 
	_guideBtn = ui::Button::create("games/chasepenguintoigloo/2.png");
	_guideBtn->setPosition(cocos2d::Vec2(_guideBtn->getContentSize().width / 2 + 20, visibleSize.height - _guideBtn->getContentSize().height / 2 - 20));
	m_gameNode->addChild(_guideBtn);
	_guideBtn->addClickEventListener([=](Ref*) {
		if (m_touchEnabled) {
			AUDIO_ENGINE->stopAllSound();
			AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/guide.mp3", false, ([=] {
				AUDIO_ENGINE->playSound(m_correctLetter.audio_path, false, ([=] {
					scheduleAudioAndActiveGuiding();
					}));
				}));
		}
	});

	return true;
}

bool PenguinGame::initIgloos()
{
	// remove old igloos
	for (auto it = m_labels.begin(); it != m_labels.end();) {
		(*it)->removeFromParent();
		it = m_labels.erase(it);
	}

	for (auto it = m_igloos.begin(); it != m_igloos.end();) {
		(*it)->removeFromParent();
		it = m_igloos.erase(it);
	}

	using namespace rapidjson;
	// parse data
	auto fullPath = FileUtils::getInstance()->fullPathForFilename(m_configPath);
	std::string result = FileUtils::getInstance()->getStringFromFile(fullPath);
	if (!result.length())
		return false;

	// schema
// 	auto schemaPath = FileUtils::getInstance()->fullPathForFilename("json/chasepenguintoigloo/schema.json");
// 	std::string schemaJson = FileUtils::getInstance()->getStringFromFile(schemaPath);
// 	
// 	Document sd;
// 	if (sd.Parse(schemaJson.c_str()).HasParseError())
// 		return false;
// 	SchemaDocument schema(sd);

	// data
	Document resultDocument;
	if (resultDocument.Parse(result.c_str()).HasParseError())
		return false;

// 	SchemaValidator sv(schema);
// 	if (!resultDocument.Accept(sv))
// 		return false;

	cocos2d::Size baseOffset(0, 250);
	cocos2d::Size horizontalOffset(400, 0);
	auto visibleSize = Director::getInstance()->getVisibleSize();
//    auto randomIndex = cocos2d::random() % resultDocument["games"].GetArray().Size();
    max_game = resultDocument["games"].GetArray().Size()-1;
	auto gameObject = resultDocument["games"][randomIndex].GetObject();
	cocos2d::Size basePos = baseOffset - (horizontalOffset * (gameObject["allLetters"].Size() - 1) / 2);
    if (gameObject.HasMember("option_vm_phonic")){
        hoc_am_hoc_van = gameObject["option_vm_phonic"].GetString();
    }
	auto correctString = gameObject["correctAnswer"].GetString();
	mj::GameObject go;
	auto pImage = math::resource::ResHelper::getImage(m_images,correctString);
	if (pImage) {
		go.text = pImage->getText("name_1");
		go.image_path = pImage->path;
		go.audio_path = pImage->getAudios("name_1").front();
	}
	else {
		go.text = correctString;
		go.audio_path = std::string("sounds/chasepenguintoigloo/letters/") + correctString + ".mp3";
	}
	m_correctLetter = go;
    
	// create igloos & letter
	for (rapidjson::SizeType i = 0; i < gameObject["allLetters"].Size(); i++) {
		auto igloo = cocos2d::Sprite::create("games/chasepenguintoigloo/1.png");		
		if (!igloo)
			return false;

		igloo->setPosition(visibleSize / 2 + basePos + horizontalOffset * i);
		igloo->setScale(IGLOO_SCALE);
		m_gameNode->addChild(igloo, -15);
        std::string font_name = CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getInstance()->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF";
		auto label = cocos2d::Label::create("",font_name , 80.0f);
		correctString = gameObject["allLetters"][i].GetString();
		pImage = math::resource::ResHelper::getImage(m_images, correctString);
		label->setString(pImage ? pImage->getText("name_1") : correctString);
		label->enableOutline(Color4B::WHITE, 2);
        if (CONFIG_MANAGER->isGameVMonkey()) igloo->setName(pImage ? pImage->getAudios("name_1").front():"");
		if (label->getString() == go.text)
			m_correctAnswer = i;
		label->setTextColor(Color4B::BLACK);
		cocos2d::Vec2 offset(-20, 60);
		label->setPosition(igloo->getPosition() + offset);
		m_gameNode->addChild(label);

		m_igloos.push_back(igloo);
		m_labels.push_back(label);
	}

	// set position for penguins
	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		auto penguin = *it;
		penguin->setPosition(m_igloos[std::rand() % m_igloos.size()]->getPosition());
	}

	return true;
}

bool PenguinGame::initBigFoot()
{
	auto bigFoot = BigFoot::createGAF("gaf/chasepenguintoigloo/Big_foot.gaf");
	if (!bigFoot)
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	bigFoot->setPosition(visibleSize / 2);
	bigFoot->setDesiredSpeed(ANIMAL_MOVE_SPEED);
	bigFoot->setScale(1.3f);
	bigFoot->setDestinationPoint(bigFoot->getPosition());
	m_gameNode->addChild(bigFoot, 1);
	m_bigFoot = bigFoot;

	// setup touch guiding big foot
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
		return handleTouch(touch, event);
	};
	listener->onTouchMoved = listener->onTouchBegan;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_gameNode);
	return true;
}

bool PenguinGame::initPenguin()
{
	for (int i = 0; i < 10; i++) {
		auto penguin = Penguin::createGAF("gaf/chasepenguintoigloo/Penguin.gaf");
		if (!penguin)
			return false;
		//penguin->setPosition(m_igloos[std::rand() % m_igloos.size()]->getPosition());
		penguin->setVisible(false);
		penguin->setScale(1.3f);
		m_gameNode->addChild(penguin, 2);
		m_penguins.push_back(penguin);
		penguin->setDesiredSpeed(ANIMAL_MOVE_SPEED * 1.5f);
	}
	return true;
}

bool PenguinGame::initEndingNode()
{
	auto layer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 128));
	//m_endingNode->addChild(layer);
    
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto endBigFoot = GAFWrapper::createGAF("gaf/chasepenguintoigloo/Big_foot.gaf");
	if (!endBigFoot)
		return false;
	endBigFoot->setPosition(cocos2d::Vec2(visibleSize.width / 2, -visibleSize.height * 0.4));
	endBigFoot->setScale(3.5f);
	//m_endingNode->addChild(endBigFoot);
	endBigFoot->playSequence("idle", true);
	m_endingBigFoot = endBigFoot;

	//auto welldone = GAFWrapper::createGAF("gaf/chasepenguintoigloo/welldone.gaf");
	std::string label_endgame = CONFIG_MANAGER->isGameVMonkey()?"":"Well done!";
	auto welldone = ui::Text::create(label_endgame,"fonts/Linotte Bold.ttf",120);
	cocos2d::Size welldoneOffset(0, 300);
	welldone->setPosition(visibleSize / 2 + welldoneOffset);
	//m_endingNode->addChild(welldone,1,"lbl_welldone");
	//welldone->play(true);

	//auto welldone1 = GAFWrapper::createGAF("gaf/chasepenguintoigloo/welldone.gaf");
	//cocos2d::Size welldoneOffset(0, 300);
	//welldone1->setPosition(visibleSize / 2 + welldoneOffset);
	//m_endingNode->addChild(welldone1);
	//welldone1->play(true);

	auto upDown = Sequence::create(
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, 16))),
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, -16))),
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, -16))),
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, 16))), NULL);

	auto downUp = Sequence::create(
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, -16))),
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, 16))),
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, 16))),
		EaseQuadraticActionOut::create(MoveBy::create(6.0f / 24, Vec2(0, -16))), NULL);

	for (int i = 0; i < cocos2d::StringUtils::getCharacterCountInUTF8String(label_endgame); ++i) {
		auto letter = welldone->getLetter(i);
		if (letter) {
			auto index = i < 4 ? i : i - 5;
			if (index >= 0) {
				if (index % 2 == 0) {
					letter->runAction(Repeat::create(upDown->clone(), 1000000000));
				}
				else {
					letter->runAction(Repeat::create(downUp->clone(), 1000000000));
				}
			}
			
		}
	}
    

	return true;
}

void PenguinGame::showGuideGame(std::function<void()> callback)
{
	static bool guidePlaying = false;
	if (guidePlaying)
		return;
	guidePlaying = true;
	std::string guide;
    if (!CONFIG_MANAGER->isGameVMonkey()){
		guide = "sounds/chasepenguintoigloo/guide.mp3";
    }else{
        if (hoc_am_hoc_van == "hoc_am"){
			guide = "sounds/chasepenguintoigloo/guide_vn_am.mp3";
        }else{
            if (hoc_am_hoc_van == "hoc_van"){
				guide = "sounds/chasepenguintoigloo/guide_vn_van.mp3";
            }else{
				guide = "sounds/chasepenguintoigloo/guide_vn.mp3";
            }
        }
        
    }

	//chains.push_back(m_correctLetter.audio_path);
	AUDIO_ENGINE->playSound(guide, false, ([=] {
		AUDIO_ENGINE->playSound(m_correctLetter.audio_path, false, ([=] {
			guidePlaying = false;
			m_touchEnabled = true;
			}));
		if (callback)
			callback();
		}));

}

void PenguinGame::checkBigFootContact()
{
	// idle
	if (m_bigFoot->getPosition() == m_bigFoot->getDestinationPosition())
		return;

	// check contact with penguins
	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		auto penguin = *it;
		if (penguin->isAttached() || penguin->isVisible() == false)
			continue;
		auto path = penguin->getPosition() - m_bigFoot->getPosition();
		if (path.length() < BIGFOOT_TRIGGER_RANGE) {
			penguin->setWithinBigFoot(true);
			m_bigFoot->attach(penguin);			
		}
	}

	// check contact with igloos
	for (auto i = 0; i < m_igloos.size(); i++) {
		auto path = m_igloos[i]->getPosition() - m_bigFoot->getPosition();
		if (path.length() < BIGFOOT_TRIGGER_RANGE) {
			reachIgloo(i);
			return;
		}
	}
}

void PenguinGame::playGuideScript()
{
	CCLOG("guiding hand");
	auto guidePlayed = MJDefault::getInstance()->getIntegerForKey(GUIDES_PLAYED_KEY, 0);
	setGameState(SCRIPT_GUIDING);
	m_scriptGuided = true;

	// play guide script 
	// tim chim gan nhat
	Penguin* penguin = nullptr;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float distanceBetweenPenguin_bigFoot = visibleSize.width;

	int numPenguinUnVisible = 0;
	for (auto peng : m_penguins) {
		if (peng->isVisible() == false)
		{
			numPenguinUnVisible++;
			continue;
		}
		Point peng_pos = peng->getPosition();
		auto distan = peng_pos.getDistance(m_bigFoot->getPosition());
		if (distanceBetweenPenguin_bigFoot > distan) {
			distanceBetweenPenguin_bigFoot = distan;
			penguin = peng;
		}
	}
	if (numPenguinUnVisible >= 1) setGameState(SCRIPT_GUIDING);

	// set Anchor for bigFoot
	Point new_posPenguin;
	if (penguin->getPositionX() < m_bigFoot->getPositionX()) {
		new_posPenguin = Point(
			penguin->getPositionX() + penguin->getBoundingBox().size.width,
			penguin->getPositionY() + penguin->getBoundingBox().size.height
		);
	}
	else {
		new_posPenguin = Point(
			penguin->getPositionX() - penguin->getBoundingBox().size.width,
			penguin->getPositionY() + penguin->getBoundingBox().size.height
		);
	}
	showHandGuideTap(new_posPenguin, [=]() {
		m_bigFoot->setDestinationPoint(new_posPenguin, [=]() {
			// go to the igloo
			auto igloo = m_igloos[m_correctAnswer];
			showHandGuideTap(igloo->getPosition(), [=]() {
				m_bigFoot->setDestinationPoint(igloo->getPosition(), [=]() {
					scheduleOnce([=](float) {
						m_bigFoot->setDestinationPoint(igloo->getPosition() - igloo->getBoundingBox().size / 3.5, ([=] {													
							for (auto pPengui : m_penguins) {
								if (pPengui->isWithinBigFoot()) {
									movePenguinToRandomPosition(pPengui);
								}
							}
							//delay + end guide
							scheduleOnce([=](float) {
								MJDefault::getInstance()->setIntegerForKey(GUIDES_PLAYED_KEY, guidePlayed + 1);
								setGameState(PenguinGameState::PLAYING);
								m_bigFoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
								}, 3.0f, "end_guide_delay");
							}));
						}, 1.0f, "before_end_delay");
										
					});
				});
			});
		//end
		});
}

void PenguinGame::showHandGuideTap(cocos2d::Vec2 pos, std::function<void()> callback)
{
	auto hand = GAFWrapper::createGAF("gaf/guidegame/hand_tap/hand_tap.gaf");
	hand->setPosition(m_bigFoot->getPosition());
	m_gameNode->addChild(hand, 99);

	auto path = hand->getPosition() - pos;
	auto length = path.length();
	auto delay = length / 600.0f;

	hand->playSequence("tap", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		if (callback)
			callback();
		hand->playSequence("swipe", true);
		auto moveAction = cocos2d::MoveTo::create(delay, pos);
		hand->runAction(moveAction);

		scheduleOnce([=](float) {
			hand->removeFromParent();
		}, delay + 0.2f, "safe_remove");
	});
}

void PenguinGame::reachIgloo(int index)
{
	auto correctAnswer = m_correctAnswer;
	if (correctAnswer != index && m_gameState == SCRIPT_GUIDING)
		return;
	auto offset = cocos2d::Vec2(-5, -55);
	for (auto it = m_penguins.begin(); it != m_penguins.end(); it++) {
		auto penguin = *it;
		if (!penguin->isAttached())
			continue;
		if (index == correctAnswer && correctAnswer != -1) {
			AUDIO_ENGINE->stopAllSound();
			_guideBtn->setTouchEnabled(false);
			penguin->setDestinationPoint(m_igloos[index]->getPosition() + offset, [=]() {				
				if (m_gameState == PenguinGameState::PLAYING) {
					_needGuidingByHand = false;
					_enablePenguinIntoIglooWithGuiding = false;								
					zoomIgloo(m_igloos[index]);
					penguin->setVisible(false);
					penguin->setWithinBigFoot(false);
					AUDIO_ENGINE->play("sounds/chasepenguintoigloo/correct.mp3", false);
					AUDIO_ENGINE->play(m_correctLetter.audio_path, false, 1.0f, ([=] {
						this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {_guideBtn->setTouchEnabled(true); }), nullptr));
						}));
					if (m_gameState != SCRIPT_GUIDING && checkWinCondition()) {
						endGame();
					}
				}
				else if (m_gameState == PenguinGameState::SCRIPT_GUIDING) {
					if (_enablePenguinIntoIglooWithGuiding) {
						_enablePenguinIntoIglooWithGuiding = false;
						zoomIgloo(m_igloos[index]);
						penguin->setVisible(false);
						penguin->setWithinBigFoot(false);
						AUDIO_ENGINE->play("sounds/chasepenguintoigloo/correct.mp3", false);
						AUDIO_ENGINE->playEffect(m_correctLetter.audio_path, false);
					}
				}
				
			});
		}
		else {
            if (CONFIG_MANAGER->isVietnameseStories())
            AUDIO_ENGINE->playSound(m_igloos[index]->getName(), false);
			movePenguinToRandomPosition(penguin);
//            inActiveMode();
			AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/wrong.mp3", false);
		}
	}
	m_bigFoot->detachAll();
}

bool PenguinGame::handleTouch(Touch * touch, Event * event)
{
	if (m_touchEnabled) {
		CCLOG("handleTouch");
		AUDIO_ENGINE->stopAllSound();
		this->unschedule("guiding");
		setGameState(PLAYING);
		if (m_gameState != PLAYING)
			return true;
		auto visibleSize = Director::getInstance()->getVisibleSize();
		auto location = touch->getLocation();
		location.y = std::min(location.y, visibleSize.height * 0.8f);
		location.y = std::max(location.y, visibleSize.height * 0.25f);
		this->runAction(Sequence::createWithTwoActions(CallFunc::create([=] {
			m_bigFoot->setDestinationPoint(location);
			}), CallFunc::create([=] {
				//this->inActiveMode();
				scheduleOnce([=](float) {
					CCLOG("after running...");
					scheduleAudioAndActiveGuiding(); //waiting for finished running
					}, 3.0f, "scheduleGuiding_delay");
				})));
	}	
	return true;
}

void PenguinGame::zoomIgloo(cocos2d::Sprite* igloo)
{
	igloo->setScale(IGLOO_SCALE);
	auto scaleAction = cocos2d::ScaleBy::create(0.2f, 1.2f);
	auto returnAction = cocos2d::CallFunc::create([=]() {
		igloo->setScale(IGLOO_SCALE);
	});
	igloo->stopAllActions();
	igloo->runAction(cocos2d::Sequence::createWithTwoActions(scaleAction, returnAction));
}

void PenguinGame::inActiveMode(){
    this->scheduleOnce([=](float){
        for(auto p : m_penguins){
            if (p->isAttached()) {
                this->showTapGuideGame(m_igloos[m_correctAnswer]->getPosition());
                return ;
            }
        }
        auto penguin = m_penguins[std::rand() % m_penguins.size()];
        this->showTapGuideGame(penguin->getPosition());
    }, 10, "inActiveMode");
}

void PenguinGame::scheduleAudioAndActiveGuiding()
{
	CCLOG("start schedule guiding...");
	this->unschedule("guiding");
	schedule([=](float dt)
		{
			if (cocos2d::experimental::AudioEngine::getPlayingAudioCount() == 1)
			{
				if (cocos2d::FileUtils::getInstance()->isFileExist("sounds/chasepenguintoigloo/guide.mp3")) {
					AUDIO_ENGINE->playSound("sounds/chasepenguintoigloo/guide.mp3", false, ([=] {
						AUDIO_ENGINE->playSound(m_correctLetter.audio_path, false, ([=] {
							scheduleAudioAndActiveGuiding();
							}));
						}));
				}
			}
			if (_needGuidingByHand) {
				playGuideScript();			
				scheduleOnce([=](float) {
					scheduleAudioAndActiveGuiding();
					}, 3, "delayNextSchedule");
			}
		}, 11.f, 100, 11.f, "guiding");
}
