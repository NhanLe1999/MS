#include "WhackAMoleGame.h"
#include "GAFWrapper.h"
#include "Mole.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "Shake.h"

#define MOLE_UP_INTERVAL 1.0f
#define MAX_MOLE_UP_INTERVAL 2.5f
#define MIN_MOLE_UP_INTERVAL 0.8f
#define MOLE_UP_TIME_STEP 0.15f

using namespace WhackAMole;

WhackAMoleGame * WhackAMoleGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = WhackAMoleGame::create();
    if(result)
    {
        if (json_file.length() > 5)
            result->m_configPath = json_file;
    }
    else
    {
        CC_SAFE_DELETE(result);
    }

	return result;
}

bool WhackAMoleGame::init()
{
	if (!MJMainGBase::init())
		return false;
	if (!initBackground())
		return false;

	return true;
}

void WhackAMoleGame::onEnter()
{
	MJMainGBase::onEnter();
    getGameData();
		initMoles();
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
		//return handleTouch(touch, event);
		return true;
	};
	listener->onTouchMoved = listener->onTouchBegan;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	playBackgroundMusic("sounds/whackamole2/music.mp3", true);

	_speaker = mj::helper::addSpeaker(this);
	_speaker->addClickEventListener(CC_CALLBACK_1(WhackAMoleGame::onSpeakerClicked, this));

}

void WhackAMoleGame::onExit()
{
	MJMainGBase::onExit();
	unscheduleUpdate();
}

void WhackAMoleGame::update(float delta)
{
	//MJMainGBase::update(delta);
	m_nextMoleUpRemaining -= delta;
	if (m_nextMoleUpRemaining < 0) {
		m_nextMoleUpRemaining = m_moleUpInterval;
		if (!m_moles.size())
			return;
		std::vector<Mole*> downMoles;
		for (auto it = m_moles.begin(); it != m_moles.end(); it++) {
			if (!(*it)->isUp())
				downMoles.push_back(*it);
		}
		if (m_hitTimes <2) {
			if (downMoles.size() > downMoles.size()-1) {
				downMoles[std::rand() % downMoles.size()]->rise();
			}
		}
		else {
			if (downMoles.size() > downMoles.size()-4) {
				downMoles[std::rand() % downMoles.size()]->rise();
			}
		}
		// must > 0
	
	}
}

void WhackAMoleGame::onMoleHit(float time, void* mole)
{
	hideHand();
	_guideShowed = true;

	// shake action
	auto shake = Shake::create(0.3, 30, 30);
	this->stopAllActions();
	this->runAction(shake);
	for (auto it = m_moles.begin(); it != m_moles.end(); it++) {
		auto m = (*it);
		m->setLocalZOrder(std::max(1, m->getLocalZOrder() - 1));
		if (m == (Mole*)mole)
			m->setLocalZOrder(m_moles.size());
	}
	if (time > m_moleUpInterval)
		m_moleUpInterval = std::min(time + MOLE_UP_TIME_STEP, MAX_MOLE_UP_INTERVAL);
	else
		m_moleUpInterval = std::max(time - MOLE_UP_TIME_STEP, MIN_MOLE_UP_INTERVAL);

	if (--m_hitTimes) {
		inActiveMode();
//        this->showTapGuideGame(Vec2(screen_size.width/2, screen_size.height*0.6));
    }
	else {
		this->unscheduleUpdate();
		this->unscheduleAllSelectors();
        for(auto m: m_moles){
            if(m != mole){
                if(m->isUp()){
                    m->hide();
                    m->setDown(true);
                }
            }
        }
        if(m_max_game < letters.size()){
            m_hitTimes = m_count_hit;
            scheduleOnce([=](float){
                startGame();
            }, 1, "start_game");
        }else{
            scheduleOnce([=](float){
                AUDIO_ENGINE->stopAllAudio();
                if (CONFIG_MANAGER->isGameVMonkey()){
                    onFinishGameVMonkey();
                }else{
                    onFinishGame();
                }
            },2.5, "finish_game");
        }
	}
}

void WhackAMoleGame::onMoleHide()
{
	m_moleUpInterval = std::min(m_moleUpInterval + MOLE_UP_TIME_STEP, MAX_MOLE_UP_INTERVAL);
    int id = random(1, 2);
    AUDIO_ENGINE->play("sounds/whackamole2/down_"+ cocos2d::StringUtils::toString(id)+".mp3");
}

void WhackAMoleGame::startGame()
{
	
    mj::GameObject letter;
    letter = letters.at(m_max_game);
    m_letter = letter;
    m_max_game++;
    
	scheduleOnce([=](float) {
		std::vector<std::string> guide;
		if (_audioIntro!="") {
			guide.push_back(_audioIntro);
		}
		else {
			if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
			{
				guide.push_back("sounds/whackamole2/instruction_VN.mp3");
			}
			else {
				guide.push_back("sounds/whackamole2/guide.mp3");
			}
		}
        
		
		guide.push_back(m_letter.audio_path);
		AUDIO_ENGINE->playChain(guide);
	}, 0.25, "guide");
	if (m_moles.size()) {
		for (auto it = m_moles.begin(); it != m_moles.end(); it++) {
			(*it)->reset();
			(*it)->setText(m_letter);
		}
	}
	//m_moleUpInterval = MOLE_UP_INTERVAL;
	m_nextMoleUpRemaining = m_moleUpInterval;
	this->schedule(schedule_selector(WhackAMoleGame::update), m_moleUpInterval);
//     this->showTapGuideGame(Vec2(screen_size.width/2, screen_size.height*0.6));
    inActiveMode();
}

WhackAMoleGame::WhackAMoleGame() :
	m_configPath("json/whackamole2/config.json"),
	m_gameNode(nullptr),
	m_endingNode(nullptr),
	m_nextMoleUpRemaining(MOLE_UP_INTERVAL),
	m_moleUpInterval(MOLE_UP_INTERVAL),
	m_hitTimes(0),
	m_preventReinit(false)
{
}

bool WhackAMoleGame::initBackground()
{
	auto bg = cocos2d::Sprite::create("games/whackamole2/1.jpg");
	auto bg2 = cocos2d::Sprite::create("games/whackamole2/1.jpg");
	if (!(bg && bg2))
		return false;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	bg->setScale(std::max(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height) * 1.1f);
	bg2->setScale(std::max(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height) * 1.1f);
	bg->setPosition(visibleSize / 2);
	bg2->setPosition(visibleSize / 2 - cocos2d::Size(0, visibleSize.height * 1.1f));

	addChild(bg);
	addChild(bg2);

	m_gameNode = cocos2d::Node::create();
	m_gameNode->setScale(1.05f);
	m_gameNode->setAnchorPoint(cocos2d::Vec2(0.5, 0));
	m_gameNode->setPosition(cocos2d::Vec2(visibleSize.width / 2 + 50.0f, 0));
	addChild(m_gameNode);

	m_endingNode = cocos2d::Node::create();
	addChild(m_endingNode);

	// bg animation
	{
		auto moveAction = cocos2d::MoveBy::create(1, cocos2d::Vec2(0, visibleSize.height*1.1f));
		bg->runAction(moveAction);
		moveAction = cocos2d::MoveBy::create(1, cocos2d::Vec2(0, visibleSize.height*1.1f));
		bg2->runAction(moveAction);
	}

	//initIntro();
	return true;
}

bool WhackAMoleGame::initMoles()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto centerPoint = cocos2d::Vec2(0, visibleSize.height / 2);// visibleSize / 2;
	cocos2d::Size offset(350, 400);
	cocos2d::Size basePos(centerPoint.x - offset.width, centerPoint.y - offset.height / 2);
	if (!m_moles.size()) {
		for (int i = 0; i < 6; i++) {
			auto mole = Mole::createMole("gaf/whackamole2/mole.gaf");
			if (!mole)
				return false;
			mole->setPosition(cocos2d::Vec2(basePos.width + offset.width * (i % 3), basePos.height + offset.height * (i / 3)));
			mole->playSequence("ground", true);
			mole->setOnHitListener(this);
			m_gameNode->addChild(mole);
			m_moles.push_back(mole);
		}

		// 2 middle moles
		for (int i = 0; i < 2; i++) {
			auto mole = Mole::createMole("gaf/whackamole2/mole.gaf");
			if (!mole)
				return false;
			mole->setPosition(cocos2d::Vec2(centerPoint.x + offset.width * (i - 0.5f), centerPoint.y + offset.height * (i / 3)));
			mole->playSequence("ground", true);
			mole->setOnHitListener(this);
			m_gameNode->addChild(mole);
			m_moles.push_back(mole);
		}
	}

	auto visibleSize2 = Director::getInstance()->getVisibleSize();

	for (auto it = m_moles.begin(); it != m_moles.end(); it++) {
		(*it)->setText(m_letter);
		if (m_preventReinit)
			continue;
		auto pos = (*it)->getPosition();
		pos.y -= visibleSize2.height;
		(*it)->setPosition(pos);
		auto move = MoveBy::create(1, cocos2d::Vec2(0, visibleSize2.height));
		(*it)->runAction(move);
	}
	//intro
	auto gameName = ui::ImageView::create("games/whackamole2/Whack-a-mole.png");
	//gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gameName->setPosition(cocos2d::Vec2(centerPoint.x , centerPoint.y + offset.height*(1/3)));
	gameName->setOpacity(0);
	m_gameNode->addChild(gameName);
	AUDIO_ENGINE->play("sounds/whackamole2/whack_a_mole.mp3");
	gameName->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(1.5), RemoveSelf::create(true), CallFunc::create([=]() {
		startGame();
		}), NULL));
	
	m_preventReinit = true;

	return true;
}

void WhackAMoleGame::initIntro(std::function<void()> finishCallback)
{

}

void WhackAMoleGame::getGameData()
{
	auto fullPath = FileUtils::getInstance()->fullPathForFilename("json/whackamole2/schema.json");
	std::string schemaJson = FileUtils::getInstance()->getStringFromFile(fullPath);

	using namespace rapidjson;

	auto configPath = FileUtils::getInstance()->fullPathForFilename(m_configPath);
	auto data = FileUtils::getInstance()->getStringFromFile(configPath);
	if (!data.length())
		return;

	Document dataDocument;
	if (dataDocument.Parse(data.c_str()).HasParseError()) {
		//
		return;
	}
	//get audio after config on v2
	if (JSON_SAFE_CHECK(dataDocument, "guiding_audio", String))
	{
		auto keyID = dataDocument["guiding_audio"].GetString();
		auto img = math::resource::ResHelper::getImage(m_images, keyID);
		if (img) {
			_audioGuiding = img->getAudios().front();
		}
	}
	if (JSON_SAFE_CHECK(dataDocument, "intro_audio", String))
	{
		auto keyID = dataDocument["intro_audio"].GetString();
		auto img = math::resource::ResHelper::getImage(m_images, keyID);
		if (img) {
			_audioIntro = img->getAudios().front();
		}
	}

	for (rapidjson::SizeType i = 0; i < dataDocument["games"].Size(); i++) {
		auto letterStr = dataDocument["games"][i].GetString();
		auto pImage = math::resource::ResHelper::getImage(m_images, letterStr);
		mj::GameObject go;
		if (pImage) {
			go.text = pImage->getText("name_1");
			go.audio_path = pImage->getAudios("name_1").front();
			go.image_path = pImage->path;
		}
		else {
			go.text = letterStr;
			go.audio_path = std::string("sounds/whackamole2/") + letterStr + ".mp3";
			go.image_path = "";
		}
		letters.push_back(go);
	}
    
    m_count_hit = dataDocument["hitTimes"].GetInt();
    m_hitTimes = m_count_hit;
}

void WhackAMoleGame::inActiveMode(){
    this->schedule([=](float){
		if (_audioGuiding != "") {
			if (_audioInactiveMode != -1) {
				cocos2d::experimental::AudioEngine::stop(_audioInactiveMode);
			}
			_audioInactiveMode =cocos2d::experimental::AudioEngine::play2d(_audioGuiding);
			
		}
		else {
			if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
			{
				_audioGuiding = "sounds/whackamole2/instruction_VN.mp3";
			}
			else {
				_audioGuiding = "sounds/whackamole2/guide.mp3";
			}
			cocos2d::experimental::AudioEngine::play2d(_audioGuiding);
		}
		showHand();
    }, 10, 10, 0, "inActive_Mode");
    
}

void WhackAMoleGame::showHand()
{
	if (!_guideShowed) {
		for (auto m : m_moles) {
			if (m->isUp()) {
				auto hand = GAFWrapper::createGAF("gaf/guidegame/hand2/hand2.gaf");
				this->addChild(hand, 1900000000);
				hand->setPosition(m->getParent()->convertToWorldSpace(m->getPosition()));
				hand->play(false, [=](gaf::GAFObject* obj) {
					hand->removeFromParent();
					});
				hand->setName("hand_guide");
				break;
			}
		}
	}
}

void WhackAMoleGame::hideHand()
{
	unschedule("inActive_Mode");
	if (_audioSpeaker) {
		cocos2d::experimental::AudioEngine::stop(_audioSpeaker);
	}
	if (_audioInactiveMode) {
		cocos2d::experimental::AudioEngine::stop(_audioInactiveMode);
	}
}

void WhackAMoleGame::onSpeakerClicked(cocos2d::Ref* sender) {
	hideHand();
	if (m_letter.audio_path != "") {
		if (_audioSpeaker != -1) {
			cocos2d::experimental::AudioEngine::stop(_audioSpeaker);
		}
		_audioSpeaker = cocos2d::experimental::AudioEngine::play2d(m_letter.audio_path);
		cocos2d::experimental::AudioEngine::setFinishCallback(_audioSpeaker, [=](int, std::string) {
			inActiveMode();
			});
	}
}