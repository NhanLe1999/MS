#include "DinosaurChasingGame.h"
#include "BackgroundNode.h"
#include "HSAudioEngine.h"
#include "Shake.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "StoryConfigManager.h"
#include "MJHelper.h"
using namespace Dinosaur;

#define CLOSE_TO_OBSTACLE_THRESHOLD 500
#define END_DISTANCE_THRESHOLD 500.0f
#define OBSTACLE_DISTANCE 1800
#define GROUND_SPEED 650

DinosaurChasingGame * DinosaurChasingGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = DinosaurChasingGame::create();
	if (json_file.length() > 5)
		result->m_configPath = json_file;
	return result;
}

bool DinosaurChasingGame::init()
{
	if (!MJMainGBase::init())
		return false;
	initBackground();
	initGoat();
	initDinosaur();
	return true;
}

void DinosaurChasingGame::onEnter()
{
	MJMainGBase::onEnter();
	createObstacle();
//    AUDIO_ENGINE->play("sounds/dinosaurchasing/bg_music.mp3", true);
    MJMainGBase::playBackgroundMusic("sounds/dinosaurchasing/bg_music.mp3", 0.5);
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
		jumpGoat();
		return true;
	};
	listener->onTouchMoved = listener->onTouchBegan;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_gameNode);

	setGameState(STARTING);
}

void DinosaurChasingGame::onExit()
{
	MJMainGBase::onExit();
}

void DinosaurChasingGame::update(float dt)
{
    MJMainGBase::update(dt);
	//dt *= m_timeScale;
	if (m_obstacleNode) {
		auto pos = m_obstacleNode->getPosition();
		pos.x -= ((m_groundNode ?
			m_groundNode->getMovingSpeed() : 0) * dt);
		if (pos.x < -m_obstacleNode->getContentSize().width) {
			if (m_consecutiveCorrect < max_tap)
				pos.x += m_nextObstacleDistance;
			else
				pos.x += (m_nextObstacleDistance * 0.8f);
			m_balloonNode->setVisible(true);

			if (m_lastAnswerResult)
				getNextLetterOrEndGame();
		}
		m_obstacleNode->setPosition(pos);


		// close to obstacle
		if (m_touchObstacleEnabled == false && m_obstacleNode->isVisible() &&
			m_goat->getPosition().x + CLOSE_TO_OBSTACLE_THRESHOLD > pos.x
			&& pos.x > m_goat->getPosition().x) {
			comeCloseToObstacle();
			return;
		}

		// touch the obstacle
		if (m_touchObstacleEnabled && m_obstacleNode->isVisible() &&
			m_goat->getPosition().x > pos.x) {
			onTouchObstacle();
		}
		//if (!m_need_guide && (m_obstacleNode->getPosition().x - m_goat->getPosition().x < CLOSE_TO_OBSTACLE_THRESHOLD/2)) {
		//	playGuidingScript();
		//}
	}
}

void DinosaurChasingGame::setGameState(Dinosaur::GameState state)
{
	CCLOG("aaaa");
	if (state == m_gameState)
		return;
	m_gameState = state;
	switch (state)
	{
	case Dinosaur::STARTING:
		playStartingScript();
		break;
	case Dinosaur::PLAYING:
		m_obstacleNode->setPositionX(m_nextObstacleDistance);
		getNextLetterOrEndGame();
		scheduleUpdate();
		break;
	case Dinosaur::ENDING:
		playEndingScript();
		break;
	default:
		break;
	}
	m_obstacleNode->setVisible(state == PLAYING);
	if (state != ENDING) {
		m_endNodeLayer1->setVisible(false);
		m_endNodeLayer2->setVisible(false);
		m_dinosaur->setLocalZOrder(0);
	}
	else {
		m_dinosaur->setLocalZOrder(3);
	}
}

void DinosaurChasingGame::createObstacle()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto obstacle = Sprite::create("games/dinosaurchasing/1.png");
	obstacle->setPosition(visibleSize.width / 2, 170);
	m_gameNode->addChild(obstacle);
	m_obstacleNode = obstacle;

	m_balloonNode = GAFWrapper::createGAF("gaf/dinosaurchasing/bubble.gaf");
	m_balloonNode->play(true);
	m_balloonNode->setScale(1.2);
	m_balloonNode->setPosition(cocos2d::Vec2(m_obstacleNode->getContentSize().width / 2 + 10, 250));
	m_obstacleNode->addChild(m_balloonNode);

	//m_label = cocos2d::Label::create("", "fonts/KGWHATTHETEACHERWANTS.TTF", 80.0f);
	//m_label->setTextColor(Color4B::BLACK);
	//m_label->enableOutline(Color4B::WHITE, 2);
	//m_label->setPosition(Vec2(m_balloonNode->getPosition() + Vec2(-20, 25)));
	//m_obstacleNode->addChild(m_label);

	m_endNodeLayer1 = Sprite::create("games/dinosaurchasing/cave.png");
	auto pos = cocos2d::Vec2(visibleSize.width * 2, 320);
	m_endNodeLayer1->setPosition(pos);
	m_endNodeLayer1->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
	m_endNodeLayer1->setVisible(false);
	m_gameNode->addChild(m_endNodeLayer1);

	m_endNodeLayer2 = Sprite::create("games/dinosaurchasing/cave_top_layer.png");
	m_endNodeLayer2->setPosition(pos);
	m_endNodeLayer2->setAnchorPoint(m_endNodeLayer1->getAnchorPoint());
	m_endNodeLayer2->setVisible(false);
	m_gameNode->addChild(m_endNodeLayer2, 2);

	boom_effect = GAFWrapper::createGAF("gaf/dinosaurchasing/boom.gaf");
	boom_effect->setVisible(false);
	m_obstacleNode->addChild(boom_effect, 3);

	correct_effect = GAFWrapper::createGAF("gaf/dinosaurchasing/effect.gaf");
	correct_effect->setVisible(false);
	m_obstacleNode->addChild(correct_effect, 3);
}

DinosaurChasingGame::DinosaurChasingGame()
	:m_configPath("json/dinosaurchasing/config.json"),
	m_bgRootNode(nullptr),
	m_goat(nullptr),
	m_gameNode(nullptr),
	m_groundNode(nullptr),
	m_obstacleNode(nullptr),
	m_dinosaur(nullptr),
	m_gameState(Dinosaur::GameState::NONE),
	m_balloonNode(nullptr),
	m_timeScale(1),
	m_touchObstacleEnabled(false),
	m_isGoatOnAir(false),
	m_endNodeLayer1(nullptr),
	m_endNodeLayer2(nullptr),
	m_label(nullptr),
	m_nextObstacleDistance(OBSTACLE_DISTANCE),
	m_isGoatFallen(false),
	m_obstacleSprite(nullptr),
	m_lastAnswerResult(false),
	m_consecutiveCorrect(0),
	m_need_guide(true)
{
	/*m_lettes.push_back("a_e");
	m_lettes.push_back("ee");
	m_lettes.push_back("f");*/
}

void DinosaurChasingGame::initBackground()
{
	_speaker = mj::helper::addSpeaker(this);
	_speaker->addClickEventListener(CC_CALLBACK_1(DinosaurChasingGame::onSpeakerClicked, this));

	auto scaleWrapper = cocos2d::Node::create();
	scaleWrapper->setScale(1.01f);
	addChild(scaleWrapper);

	m_bgRootNode = cocos2d::Node::create();
	scaleWrapper->addChild(m_bgRootNode);

	m_gameNode = cocos2d::Node::create();
	m_bgRootNode->addChild(m_gameNode, 1);

	auto bgNode = BackgroundNode::createBackground("games/dinosaurchasing/bg1_1.png",
		"games/dinosaurchasing/bg1_2.png", 150);
	m_bgRootNode->addChild(bgNode);
	m_bgNodes.push_back(bgNode);

	bgNode = BackgroundNode::createBackground("games/dinosaurchasing/bg2_1.png",
		"games/dinosaurchasing/bg2_2.png", 200, 1.3f);
	m_bgRootNode->addChild(bgNode);
	m_bgNodes.push_back(bgNode);
	bgNode->setBackgroundAnchor(BOTTOM);

	bgNode = BackgroundNode::createBackground("games/dinosaurchasing/bg3_1.png",
		"games/dinosaurchasing/bg3_2.png", 250, 1.3f);
	m_bgRootNode->addChild(bgNode);
	m_bgNodes.push_back(bgNode);
	bgNode->setBackgroundAnchor(BOTTOM);

	bgNode = BackgroundNode::createBackground("games/dinosaurchasing/bg4_1.png",
		"games/dinosaurchasing/bg4_2.png", 350, 1.5f);
	m_bgRootNode->addChild(bgNode);
	m_bgNodes.push_back(bgNode);
	bgNode->setBackgroundAnchor(BOTTOM);

	bgNode = BackgroundNode::createBackground("games/dinosaurchasing/bg5_1.png",
		"games/dinosaurchasing/bg5_2.png", GROUND_SPEED, 1.0f);
	bgNode->setPosition(cocos2d::Vec2(0, -100));
	m_bgRootNode->addChild(bgNode);
	m_bgNodes.push_back(bgNode);
	m_groundNode = bgNode;
	bgNode->setBackgroundAnchor(BOTTOM);
}

void DinosaurChasingGame::initGoat()
{
	auto goat = Goat::createGAF("gaf/dinosaurchasing/goat.gaf");
	goat->setPosition(cocos2d::Vec2(400, 210));
	goat->playSequence("run", true);
	m_gameNode->addChild(goat, 1);
	m_goat = goat;
}

void DinosaurChasingGame::initDinosaur()
{
	auto dinosaur = GAFWrapper::createGAF("gaf/dinosaurchasing/dino.gaf");
	dinosaur->setPosition(cocos2d::Vec2(-300, 380));
	dinosaur->playSequence("run", true);
	m_gameNode->addChild(dinosaur);
	m_dinosaur = dinosaur;
}

void DinosaurChasingGame::playGuidingScript()
{

	m_need_guide = (m_need_guide && !number_tap);
	if (m_need_guide) {
		setTimeScale(0.25f);
		showTapGuideGame(m_goat->getPosition(), 0, 0.1f);
		//auto duration = CLOSE_TO_OBSTACLE_THRESHOLD / m_groundNode->getMovingSpeed();
		//auto scaleUp = cocos2d::ScaleTo::create(duration, 1.25f);
		//m_bgRootNode->runAction(scaleUp);
	}

}

void DinosaurChasingGame::playStartingScript()
{
	number_tap = 0;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	getDataFromConfig();
	m_lettes = m_games[0];

	setTimeScale(1);
	auto pos = m_dinosaur->getPosition();
	pos.x = -800;
	m_dinosaur->setPosition(pos); // push the dinosaur behind the scene

	pos = m_goat->getPosition();
	pos.x = -500;
	pos.y = 210;
	m_goat->setPosition(pos); // push the goat behind the scene

	// dinosaur animations
	{
		pos = m_dinosaur->getPosition();
		pos.x = visibleSize.width - 700;
		auto move1 = cocos2d::MoveTo::create(2.0f, pos);

		pos.x -= m_dinosaur->getContentSize().width / 8;
		auto move2 = cocos2d::MoveTo::create(0.4f, pos);
		pos.x -= m_dinosaur->getContentSize().width / 8;
		auto move3 = cocos2d::MoveTo::create(1.2f, pos);

		auto move4 = cocos2d::DelayTime::create(0.3f);
		pos.x -= m_dinosaur->getContentSize().width / 6;
		auto move5 = cocos2d::MoveTo::create(0.5f, pos);

		auto move6 = cocos2d::DelayTime::create(0.5f);
		pos.x = -m_dinosaur->getContentSize().width / 2;
		auto move7 = cocos2d::MoveTo::create(0.8f, pos);

		auto offSound = cocos2d::CallFunc::create([=]() {
			AUDIO_ENGINE->stopSound("sounds/dinosaurchasing/dino_footstep.mp3");
			m_bgRootNode->stopAllActions();
		});

		m_dinosaur->runAction(Sequence::create(move1, move2, move3, move4, move5, move6, move7, offSound, nullptr));
		m_dinosaur->playSequence("run", true);
	}

	// goat animations
	{
		pos = m_goat->getPosition();
		pos.x = visibleSize.width - m_goat->getContentSize().width / 2;
		auto move1 = cocos2d::MoveTo::create(2.0f, pos);

		pos.x -= (m_goat->getContentSize().width / 2);
		auto move2 = cocos2d::MoveTo::create(0.8f, pos);

		pos.x += (m_goat->getContentSize().width / 2);
		auto move3 = cocos2d::MoveTo::create(0.8f, pos);

		pos.x = visibleSize.width / 2;
		auto move4 = cocos2d::MoveTo::create(0.8f, pos);
		auto move5 = cocos2d::DelayTime::create(0.5f);

		pos.x = visibleSize.width / 4;
		auto move6 = cocos2d::MoveTo::create(0.8f, pos);

		auto changeState = cocos2d::CallFunc::create([=]() {
			auto pos = m_obstacleNode->getPosition();
			pos.x = visibleSize.width * 2 - m_obstacleNode->getContentSize().width;
			m_obstacleNode->setPosition(pos);
			AUDIO_ENGINE->playSound(CONFIG_MANAGER->isGameVMonkey()?"sounds/dinosaurchasing/guide_vi.mp3":"sounds/dinosaurchasing/guide.mp3", false, [=]() {
				inActiveMode();
				setGameState(PLAYING);
				auto pos = m_obstacleNode->getPosition();
				pos.x = visibleSize.width + 30;
				m_obstacleNode->setPosition(pos);
			});
		});

		m_goat->runAction(Sequence::create(move1, move2, move3, changeState, move4, move5, move6, nullptr));
	}

	// sound
	{
		AUDIO_ENGINE->playSound("sounds/dinosaurchasing/dino_footstep.mp3", true);
		auto shake = Shake::create(0.3f, 5, 5);
		m_bgRootNode->runAction(cocos2d::RepeatForever::create(shake));
		scheduleOnce([=](float) {
			AUDIO_ENGINE->playSound("sounds/dinosaurchasing/roar.mp3", false);
		}, 1.2f, "roar");

		AUDIO_ENGINE->playSound("sounds/dinosaurchasing/goat_run.mp3", true);
	}
}


void DinosaurChasingGame::playEndingScript()
{
    this->unschedule("active_mode");
	m_obstacleNode->setVisible(false);

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	scheduleOnce([=](float) {
		m_endNodeLayer1->setVisible(true);
		m_endNodeLayer2->setVisible(true);

		auto delayDuration = (m_nextObstacleDistance - 10) / m_groundNode->getMovingSpeed() / m_timeScale;

		// lair animation
		{
			auto pos = m_endNodeLayer1->getPosition();
			pos.x = visibleSize.width + m_nextObstacleDistance;
			m_endNodeLayer1->setPosition(pos);
			m_endNodeLayer2->setPosition(pos);

			pos.x = visibleSize.width + 10;
			auto move = cocos2d::MoveTo::create(delayDuration, pos);
			auto moveClone = move->clone();
			m_endNodeLayer1->runAction(move);
			m_endNodeLayer2->runAction(moveClone);
		}

		// goat animation
		{
			auto move1 = cocos2d::DelayTime::create(delayDuration);

			auto pos = m_goat->getPosition();
			pos.x += visibleSize.width;
			pos.y = 210;
			auto duration = visibleSize.width / m_groundNode->getMovingSpeed();
			auto func1 = cocos2d::CallFunc::create([=]() {
				setTimeScale(0);
			});

			auto move2 = cocos2d::MoveTo::create(duration, pos);
			auto func2 = cocos2d::CallFunc::create([=]() {
				AUDIO_ENGINE->stopSound("sounds/dinosaurchasing/goat_run.mp3");
			});

			//m_goat->stopAllActions();
			m_goat->runAction(Sequence::create(move1, func1, move2, func2, nullptr));
		}

		// dinosaur animation
		{
			auto move1 = cocos2d::DelayTime::create(delayDuration);
			auto pos = m_dinosaur->getPosition();
			pos.x = visibleSize.width - 900;
			auto func1 = cocos2d::CallFunc::create([=]() {
				scheduleOnce([=](float) {
					AUDIO_ENGINE->playSound("sounds/dinosaurchasing/roar.mp3", false);
				}, 1.1f, "roar_delay");
				auto shake = Shake::create(0.3f, 5, 5);
				m_bgRootNode->runAction(cocos2d::RepeatForever::create(shake));
				AUDIO_ENGINE->playSound("sounds/dinosaurchasing/dino_footstep.mp3", true);
			});
			auto move2 = cocos2d::MoveTo::create(2.0f, pos);
			auto func2 = cocos2d::CallFunc::create([=]() {
				m_dinosaur->playSequence("stop", false);
				m_dinosaur->setLocalZOrder(3);
				m_bgRootNode->stopAllActions();
				AUDIO_ENGINE->stopSound("sounds/dinosaurchasing/dino_footstep.mp3");
				AUDIO_ENGINE->playSound("sounds/dinosaurchasing/roar.mp3", false);
				scheduleOnce([=](float) {
					m_games.erase(m_games.begin());
					if (m_games.size()) {
						setGameState(STARTING);
					}
					else {
                        AUDIO_ENGINE->stopAllAudio();
						this->onFinishGame();
					}

				}, 2.5, "restart_game");
			});
			//m_dinosaur->stopAllActions();
			m_dinosaur->runAction(Sequence::create(move1, func1, move2, func2, nullptr));
		}
	}, 0.1f, "ending_script_delay");
}

void DinosaurChasingGame::comeCloseToObstacle()
{
	//setTimeScale(0.5f);
	//auto duration = CLOSE_TO_OBSTACLE_THRESHOLD / m_groundNode->getMovingSpeed();
	m_touchObstacleEnabled = true;
	//auto scaleUp = cocos2d::ScaleTo::create(duration, 1.5f);
	//m_bgRootNode->runAction(scaleUp);
	if (m_need_guide) {
		m_need_guide = false;
		playGuidingScript();
	}
}

void DinosaurChasingGame::onTouchObstacle()
{
	m_touchObstacleEnabled = false;
	setTimeScale(1);
	auto duration = CLOSE_TO_OBSTACLE_THRESHOLD / m_groundNode->getMovingSpeed();

	auto scaleDown = cocos2d::ScaleTo::create(duration, 1);
	m_bgRootNode->stopAllActions();
	m_bgRootNode->runAction(scaleDown);
	if (m_isGoatOnAir) {
		m_nextObstacleDistance = OBSTACLE_DISTANCE;
		m_consecutiveCorrect++;

		m_balloonNode->setVisible(false);

		auto pos = m_gameNode->convertToNodeSpace(m_label->getParent()->convertToWorldSpace(m_label->getPosition()));
		m_label->retain();
		m_label->removeFromParentAndCleanup(false);
		m_label->setPosition(pos);
		m_gameNode->addChild(m_label);
		m_label->setVisible(true);
        m_label->release();
		m_label->runAction(Sequence::create(
			Spawn::createWithTwoActions(ScaleTo::create(0.25f, 2.0f), MoveTo::create(0.25f, Vec2(screen_size.width / 2, screen_size.height * 0.75))),
			DelayTime::create(0.5f), FadeOut::create(0.25f), CallFunc::create([=] {
		}), NULL));

		if (m_obstacleSprite) {
			m_obstacleSprite->setVisible(false);
		}
		auto letter = (mj::GameObject*)m_label->getUserData();
		AUDIO_ENGINE->playSound(letter->audio_path);
		AUDIO_ENGINE->playSound("/sounds/dinosaurchasing/correct.mp3", false);
		number_tap++;
		//        AUDIO_ENGINE->playSound(std::string("/sounds/dinosaurchasing/letters/") + m_label->getString() + ".mp3", false);
		correct_effect->setPosition(m_balloonNode->getPosition());
		correct_effect->setVisible(true);
		correct_effect->play(false, [=](gaf::GAFObject* obj) {
			correct_effect->setVisible(false);
		});
	}
	else {

		// move obstacle away
		m_nextObstacleDistance = OBSTACLE_DISTANCE * 2;
		m_consecutiveCorrect = 0;

		// play wrong animation
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

		AUDIO_ENGINE->playSound("sounds/dinosaurchasing/wrong.mp3", false);
		boom_effect->setPosition(cocos2d::Vec2(m_obstacleNode->getContentSize().width / 2, 0));
		boom_effect->setVisible(true);
		boom_effect->play(false, [=](gaf::GAFObject* obj) {
			boom_effect->setVisible(false);
		});

		// goat animation
		{
			m_isGoatFallen = true;
			auto pos = m_goat->getPosition();
			auto baseX = pos.x;
			pos.x = visibleSize.width * 3 / 4;
			auto move1 = cocos2d::MoveTo::create(2.0f, pos);

			pos.x = visibleSize.width / 2;
			auto move2 = cocos2d::MoveTo::create(0.6f, pos);
			auto move3 = cocos2d::DelayTime::create(0.5f);

			pos.x = baseX;
			auto move4 = cocos2d::MoveTo::create(0.5f, pos);
			auto move5 = CallFunc::create([=]() {
				m_isGoatFallen = false;
			});
			m_goat->runAction(Sequence::create(move1, move2, move3, move4, move5, nullptr));
		}

		// dinosaur animation
		{
			auto pos = m_dinosaur->getPosition();
			auto baseX = pos.x;

			auto move1 = cocos2d::DelayTime::create(0.05f);
			auto sound1 = cocos2d::CallFunc::create([=]() {
				auto shake = Shake::create(0.3f, 5, 5);
				m_bgRootNode->runAction(cocos2d::RepeatForever::create(shake));
				AUDIO_ENGINE->playSound("sounds/dinosaurchasing/dino_footstep.mp3", true);
				AUDIO_ENGINE->playSound("sounds/dinosaurchasing/roar.mp3", false);
			});
			pos.x = visibleSize.width / 4;
			auto move2 = EaseOut::create(cocos2d::MoveTo::create(1.5f, pos), 2.5f);

			pos.x = baseX;
			auto move3 = cocos2d::MoveTo::create(2.4f, pos);
			auto sound2 = cocos2d::CallFunc::create([=]() {
				m_bgRootNode->stopAllActions();
				AUDIO_ENGINE->stopSound("sounds/dinosaurchasing/dino_footstep.mp3");
			});

			m_dinosaur->runAction(Sequence::create(move1, sound1, move2, move3, sound2, nullptr));
		}

		m_goat->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			m_goat->playSequence("run", true);
		});
		//AUDIO_ENGINE->playSound("sounds/dinosaurchasing/ohno.mp3", false);
	}
	m_lastAnswerResult = m_isGoatOnAir;
}

void DinosaurChasingGame::setTimeScale(float timescale)
{
	m_timeScale = timescale;
	for (auto it = m_bgNodes.begin(); it != m_bgNodes.end(); it++) {
		auto bg = (*it);
		bg->setSpeedScale(timescale);
	}
	m_goat->setTimeScale(timescale);
}

void DinosaurChasingGame::jumpGoat()
{
	if (m_isGoatOnAir || m_isGoatFallen || m_gameState != PLAYING)
		return;
	hideTapGuideGame();
	m_need_guide = false;
	setTimeScale(1.0f);
	m_isGoatOnAir = true;
	CCLOG("obstacle pos : %f | goat pos %f", m_obstacleNode->getPosition().x, m_goat->getPosition().x);
	m_goat->jump(150, [=]() {m_isGoatOnAir = false; });

	if (m_obstacleNode->isVisible() && m_obstacleNode->getPosition().x > m_goat->getPosition().x
		&& m_obstacleNode->getPosition().x - m_goat->getPosition().x < CLOSE_TO_OBSTACLE_THRESHOLD) {
		this->unschedule("active_mode");
		this->inActiveMode();
		//setTimeScale(1f);
		auto duration = CLOSE_TO_OBSTACLE_THRESHOLD / m_groundNode->getMovingSpeed();
		auto scaleUp = cocos2d::ScaleTo::create(duration, 1.5f);
		//m_bgRootNode->runAction(scaleUp);
	}

	/*static cocos2d::Vec2 jumpOffset(0, 150);
	auto move1 = cocos2d::MoveBy::create(0.8f, jumpOffset);
	auto move2 = cocos2d::MoveBy::create(0.375, -jumpOffset);
	m_goat->runAction(Sequence::create(move1, move2, nullptr));
	AUDIO_ENGINE->stopSound("sounds/dinosaurchasing/goat_run.mp3");
	m_goat->playSequence("jump", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		m_goat->playSequence("fall", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			m_isGoatOnAir = false;
			m_goat->playSequence("run", true);
			AUDIO_ENGINE->playSound("sounds/dinosaurchasing/goat_run.mp3", true);
		});
	});*/
}

void DinosaurChasingGame::getNextLetterOrEndGame()
{
	if (m_gameState != PLAYING)
		return;

	//    if (!m_lettes.size()) {
	//        setGameState(ENDING);
	//        return;
	//    }

	if (number_tap == max_tap) {
		setGameState(ENDING);
		return;
	}

	//    auto letter = m_lettes.begin();

	if (m_obstacleSprite) {
		m_obstacleSprite->removeFromParent();
		m_obstacleSprite = nullptr;
	}
	if (m_label) {
		m_label->removeFromParent();
		m_label = nullptr;
	}

	if (number_tap % m_lettes.size() == 0) {
		mj::helper::shuffle(m_lettes);
	}

	mj::GameObject * letter;
		auto index = number_tap % (int)m_lettes.size();
		letter = &m_lettes[index];

	std::string text_bubble = letter->text;
	bool upper_case = number_tap % 2;
	if (upper_case && text_bubble.size() == 1) {
		text_bubble = mj::helper::to_upper(text_bubble);
		//std::transform(text_bubble.begin(), text_bubble.end(), text_bubble.begin(), ::toupper);
	}
	m_label = cocos2d::Label::create(text_bubble, PHONIC_MANAGER->getGameFontName()/*"fonts/KGWHATTHETEACHERWANTS.TTF"*/, 80.0f);
	m_label->setTextColor(Color4B::BLACK);
	m_label->enableOutline(Color4B::WHITE, 2);
	m_label->setPosition(Vec2(m_balloonNode->getPosition() + Vec2(-20, 25)));
	m_obstacleNode->addChild(m_label);
	m_label->setUserData(letter);
	//m_label->setString(letter.text);
	//m_label->setTag(index);
	// in case of image
	{
		auto sprite = Sprite::create(letter->image_path);
		if (sprite) { // image
			m_obstacleSprite = sprite;
			sprite->setScale(std::min(150 / sprite->getContentSize().width, 150 / sprite->getContentSize().height));
			sprite->setPosition(m_balloonNode->getPosition() + Vec2(-20, 25));
			m_obstacleNode->addChild(sprite);
			m_label->setVisible(false);
		}
		else { // label
			m_label->setVisible(true);
		}
	}

	//    m_lettes.erase(letter);
}

void DinosaurChasingGame::getDataFromConfig()
{
	m_lettes.clear();
	if (m_configPath.length()) {
		using namespace rapidjson;
		// parse data
		auto fullPath = FileUtils::getInstance()->fullPathForFilename(m_configPath);
		std::string result = FileUtils::getInstance()->getStringFromFile(fullPath);
		if (!result.length())
			return;

		// schema
// 		auto schemaPath = FileUtils::getInstance()->fullPathForFilename("json/dinosaurchasing/schema.json");
// 		std::string schemaJson = FileUtils::getInstance()->getStringFromFile(schemaPath);
// 		
// 		Document sd;
// 		if (sd.Parse(schemaJson.c_str()).HasParseError())
// 			return;
// 		SchemaDocument schema(sd);

		// data
		Document resultDocument;
		if (resultDocument.Parse(result.c_str()).HasParseError())
			return;

		// 		SchemaValidator sv(schema);
		// 		if (!resultDocument.Accept(sv))
		// 			return;

		//        auto letterArray = resultDocument["letters"].GetArray();
		//        for (rapidjson::SizeType i = 0; i < letterArray.Size(); i++) {
		//            std::string letter = letterArray[i].GetString();
		//            auto pImage = math::resource::ResHelper::getImage(m_images, letter);
		//            mj::GameObject go;
		//            if (pImage) {
		//                go.text = pImage->getText();
		//                go.image_path = pImage->path;
		//                go.audio_path = pImage->getAudio();
		//            }
		//            else {
		//                go.text = letter;
		//                go.image_path = std::string("games/dinosaurchasing/letters/") + letter + ".png";
		//                go.audio_path = std::string("sounds/dinosaurchasing/letters/") + letter + ".mp3";
		//            }
		//            m_lettes.push_back(go);
		//        }
		max_tap = resultDocument["number_tap"].GetInt();
		auto data = resultDocument["data"].GetArray();

		for (int j = 0; j < data.Size(); j++) {
			std::vector<mj::GameObject> go_list;
			auto img_list = data[j]["images"].GetArray();
			for (int i = 0; i < img_list.Size(); i++) {
				std::string letter = img_list[i]["item"]["img_path"].GetString();
				auto pImage = math::resource::ResHelper::getImage(m_images, letter);
				mj::GameObject go;

				if (pImage) {
					if (img_list[i]["item"]["is_text"].GetBool()) {
						go.image_path = pImage->path;
					}
					else {
						go.image_path = "";
					}
					go.text = pImage->getText("name_1");
					go.audio_path = pImage->getAudios("name_1").front();

					if (!pImage->getText("name_n").empty()) {
						mj::GameObject p;
						p.text = pImage->getText("name_n");
						p.audio_path = pImage->getAudios("name_n").front();
						m_phonics.push_back(p);
					}
				}
				else {
					go.text = letter;
					go.image_path = std::string("games/dinosaurchasing/letters/") + letter + ".png";
					go.audio_path = std::string("sounds/dinosaurchasing/letters/") + letter + ".mp3";
				}
				go_list.push_back(go);
			}
			m_games.push_back(go_list);
		}

	}
	else {
		// sample
// 		m_lettes.push_back("a_e");
// 		m_lettes.push_back("ee");
// 		m_lettes.push_back("f");
	}
}
void DinosaurChasingGame::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		CCLOG("ACTIVE_MODE");
		AUDIO_ENGINE->playSound(CONFIG_MANAGER->isGameVMonkey()?"sounds/dinosaurchasing/Come on, tap the bubble_vi.mp3":"sounds/dinosaurchasing/Come on, tap the bubble.mp3");
	}, 7, 100, delay_time, "active_mode");
}
void DinosaurChasingGame::onSpeakerClicked(cocos2d::Ref* sender) {
	AUDIO_ENGINE->playSound(CONFIG_MANAGER->isGameVMonkey() ? "sounds/dinosaurchasing/Come on, tap the bubble_vi.mp3" : "sounds/dinosaurchasing/Come on, tap the bubble.mp3");
}
