#include "FrogSentenceGame.h"
#include "TextLeaf.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "HSAudioEngine.h"
#include "SentenceGroup.h"
#include "Shake.h"
#include "APProfileManager.h"
#define SLOT_SNAP_DISTANCE 250

using namespace FrogSentence;

FrogSentenceGame * FrogSentenceGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = FrogSentenceGame::create();
	if (json_file.length() > 5)
		result->m_configPath = json_file;
	return result;
}

bool FrogSentenceGame::init()
{
	if (!MJMainGBase::init())
		return false;
	initBackground();
	initFrog();
	initSelectors();
	//getGameData();
	initLeafSlot();
	//showNextWordOrEndGame();
	return true;
}

void FrogSentenceGame::onEnter()
{
	MJMainGBase::onEnter();
	scheduleUpdate();

	AUDIO_ENGINE->playBackgroundMusic("sounds/frogsentence/bg_music.mp3", 0.8f);
	
	//AUDIO_ENGINE->playBackgroundMusic("sounds/frogsentence/bg_music.mp3",0.8f);

    if (CONFIG_MANAGER->isVietnameseStories()) {
        getAudioVMonkey();
    }
    
	playIntroGame();

	auto listener = cocos2d::EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(FrogSentenceGame::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(FrogSentenceGame::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_bgRootNode);
}

void FrogSentenceGame::onExit()
{
	unscheduleUpdate();
	MJMainGBase::onExit();
}

void FrogSentenceGame::update(float dt) {
    
	MJMainGBase::update(dt);
    
	if (m_bgSprite2) {
		
        if (m_bgSprite2->getBoundingBox().getMaxX() < 0) {
			m_bgSprite2->setPositionX(m_bgSprite1->getPositionX() + m_bgSprite1->getContentSize().width * m_bgSprite1->getScale());
			CCLOG("bg sprite");
		}
	}

	if (m_bgSprite1) {
		if (m_bgSprite1->getBoundingBox().getMaxX() < 0) {
			m_bgSprite1->setPositionX(m_bgSprite->getPositionX() + m_bgSprite->getContentSize().width * m_bgSprite->getScale());
			CCLOG("bg sprite");
		}
	}
	if (m_bgSprite) {
		if (m_bgSprite->getBoundingBox().getMaxX() < 0) {
			m_bgSprite->setPositionX(m_bgSprite2->getPositionX() + m_bgSprite2->getContentSize().width * m_bgSprite2->getScale());
			CCLOG("bg sprite 1");
		}
	}
}

bool FrogSentenceGame::onTouchBegan(Touch * touch, Event * unusedEvent)
{
	m_bgStartLocation = m_bgRootNode->getPosition();
	m_lastTouchLocation = touch->getLocation();
	return true;
}

void FrogSentenceGame::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	if (m_gameState != PLAYING)
		return;
    
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto location = touch->getLocation();
	auto posDelta = location - m_lastTouchLocation;
	posDelta.y = 0;

	auto predictPos = m_bgRootNode->getPosition() + posDelta;
	if (predictPos.x > 0)
		predictPos.x = 0;
	auto rightEdge = visibleSize.width - m_rightTree->getPosition().x;
	if (predictPos.x < rightEdge)
		predictPos.x = rightEdge;
	m_bgRootNode->setPosition(predictPos);
	m_lastTouchLocation = location;
}

void FrogSentenceGame::onTouchEnded(Touch * touch, Event * unusedEvent)
{
	if (m_gameState == PLAYING)
		m_bgRootNode->setPosition(m_bgStartLocation);
}

FrogSentenceGame::FrogSentenceGame() :
	m_configPath("json/frogsentence/config.json"),
	m_bgRootNode(nullptr),
	m_frog(nullptr),
	m_selectFrame(nullptr),
	m_rightTree(nullptr),
	m_leftTree(nullptr),
    m_gameState(GameState::NONE),
	m_slot(nullptr),
	m_bgSprite(nullptr),
	m_bgSprite1(nullptr),
	m_bgSprite2(nullptr),
	m_lastTouchLocation(0, 0),
	m_guideNode(nullptr),
	m_sentenceGroup(nullptr),
	m_firstTimePlay(true),
	_finishActivity(false),
	_soundAvaiable(true)
{
}

void FrogSentenceGame::initBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_bgRootNode = cocos2d::Node::create();
	m_bgRootNode->setPosition(0, visibleSize.height * 0.6f);
	addChild(m_bgRootNode);

	auto bg = Sprite::create("games/frogsentence/bg.jpg");
	bg->setScale(visibleSize.height / bg->getContentSize().height * 1.5f);
	bg->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
	bg->setPosition(0, visibleSize.height * 0.15f);
	m_bgRootNode->addChild(bg);
	m_bgSprite = bg;

	auto bg1 = Sprite::create("games/frogsentence/bg.jpg");
	bg1->setScale(visibleSize.height / bg1->getContentSize().height * 1.5f);
	bg1->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
	bg1->setPosition(bg->getBoundingBox().getMaxX(), visibleSize.height * 0.15f);
	m_bgRootNode->addChild(bg1);
	m_bgSprite1 = bg1;

	auto bg2 = Sprite::create("games/frogsentence/bg.jpg");
	bg2->setScale(visibleSize.height / bg2->getContentSize().height * 1.5f);
	bg2->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
	bg2->setPosition(bg1->getBoundingBox().getMaxX(), visibleSize.height * 0.15f);
	m_bgRootNode->addChild(bg2);
	m_bgSprite2 = bg2;

	auto leftTree = cocos2d::Sprite::create("games/frogsentence/tree.png");
	leftTree->setFlippedX(true);
	leftTree->setPosition(cocos2d::Vec2(-150, -250));
	leftTree->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	//leftTree->setScale(0.8f);
	m_bgRootNode->addChild(leftTree);
	m_leftTree = leftTree;

	auto rightTree = cocos2d::Sprite::create("games/frogsentence/tree.png");
	auto pos = leftTree->getPosition();
	pos.x += 900;// number of leaves
	pos.y -= 20;
	rightTree->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	rightTree->setPosition(pos);
	m_bgRootNode->addChild(rightTree);
	m_rightTree = rightTree;

	// guide btn 
	guideBtn = ui::Button::create("games/frogsentence/loa.png");
	guideBtn->setPosition(cocos2d::Vec2(guideBtn->getContentSize().width / 2 + 20, visibleSize.height - guideBtn->getContentSize().height / 2 - 20));
	addChild(guideBtn);
	guideBtn->addClickEventListener([=](Ref*) {
		if (m_gameState == PLAYING)
			showGuideGame();
	});
}

void FrogSentenceGame::initFrog()
{
	m_frog = GAFWrapper::createGAF("gaf/frogsentence/frog.gaf");
	m_frog->setPosition(cocos2d::Vec2(150, 0));
	m_frog->playSequence("idle", true);
	m_frog->setVisible(false);
	m_bgRootNode->addChild(m_frog, 1);
}

void FrogSentenceGame::initLeafSlot()
{
	auto basePos = m_leftTree->getPosition() + cocos2d::Vec2(580, 120);//cocos2d::Vec2(400, -150);
	if (!m_slot) {
		m_slot = Sprite::create("games/frogsentence/3.png");
		m_bgRootNode->addChild(m_slot);
	}
	m_slot->setScale(0.27f);
	m_slot->setPosition(basePos);
	auto rightTreePos = m_leftTree->getPosition() + cocos2d::Vec2(950, 0);
	auto number_leaf = m_words.size() == 0 ? 6 : m_words.size();
	rightTreePos.x += (200 * number_leaf);
	m_rightTree->setPosition(rightTreePos);
	/*auto offset = cocos2d::Vec2(200, 0);
	for (auto i = 0; i < 6; i++) {
		auto leaf = Sprite::create("games/frogsentence/3.png");
		leaf->setScale(0.27f);
		leaf->setPosition(basePos + offset * i);
		m_bgRootNode->addChild(leaf);
	}*/
}

void FrogSentenceGame::initSelectors()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_selectFrame = ui::Scale9Sprite::create(CCRectMake(35, 35, 1, 1), "games/frogsentence/1.png");
	m_selectFrame->setScale9Enabled(true);
	m_selectFrame->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	m_selectFrame->setPreferredSize(cocos2d::Size(600, 140));
	m_selectFrame->setPosition(visibleSize.width / 2, -140 * 1.3f);
	m_selectFrame->setScale(1.3f);

	addChild(m_selectFrame);
	std::vector<std::string> strs;
	strs.push_back("the");
	strs.push_back("quick");
	strs.push_back("brown");
}

void FrogSentenceGame::showWords(std::vector<mj::GameObject> strs)
{
	if (strs.size() != 3)
		return;
	auto pos = m_selectFrame->getPosition();
	pos.y = -140 * 1.3f;
	auto move1 = cocos2d::MoveTo::create(0.4f, pos);
	auto move2 = cocos2d::CallFunc::create([=]() {
		m_selectFrame->removeAllChildren();
		auto basePos = cocos2d::Vec2(100, m_selectFrame->getContentSize().height / 2);
		auto offset = cocos2d::Vec2(200, 0);
		auto counter = 0;
		for (auto it = strs.begin(); it != strs.end(); it++) {
			auto leafSprite = TextLeaf::create("games/frogsentence/2.png", it->text);
			auto leafSpriteBlink = TextLeaf::create("games/frogsentence/3.png", "");
			if (it->text.size() > 6)
			{
				float scaleText = (float)(6.0f / (float)it->text.size());
				leafSprite->getLabel()->setScale(scaleText);
			}
			//leafSprite->getLabel()->setColor(cocos2d::Color3B::RED);
			leafSprite->setScale(0.27f);
			leafSpriteBlink->setScale(0.27f);

			leafSprite->setPosition(basePos + counter * offset);
			leafSpriteBlink->setPosition(basePos + counter * offset);
			leafSpriteBlink->setZOrder(leafSprite->getZOrder() - 1);
			leafSprite->setBasePosition(leafSprite->getPosition());

			leafSprite->setTouchEndedCallback([=](TextLeaf* leaf) {
				onDropLeaf(leaf);
			});
			leafSprite->setTouchBeganCallback([=](TextLeaf* leaf) {
				hideTapGuideGame();
				//this->unschedule("showDragGuideGame");
			});
			m_selectFrame->addChild(leafSprite);
			m_selectFrame->addChild(leafSpriteBlink);

			counter++;
		}
		AUDIO_ENGINE->play("sounds/frogsentence/show_word.mp3", false);
	});
	pos.y = 15;
	auto move3 = cocos2d::MoveTo::create(0.4f, pos);
	m_selectFrame->runAction(Sequence::create(move1, move2, move3, nullptr));
}

void FrogSentenceGame::showNextWordOrEndGame()
{
	if (!m_words.size()) {
		//end game
		setGameState(ENDING);
		return;
	}

	WordSync ws = (*m_words.begin());
	m_wordsClone.push_back(ws);
	m_words.erase(m_words.begin());

	m_correctWord = ws.word;
	showWords(ws.answers);
}

void FrogSentenceGame::getAudioVMonkey()
{
    auto jsonString = FileUtils::getInstance()->getStringFromFile(m_configPath);
    if (!jsonString.length())
        return;

    using namespace rapidjson;

    Document resultDocument;
    if (resultDocument.Parse(jsonString.c_str()).HasParseError())
        return;
    
    if (CONFIG_MANAGER->isGameVMonkey() && resultDocument.HasMember("audio_intro_vi")){
       auto audio_intro = resultDocument["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(m_images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
}
void FrogSentenceGame::getGameData()
{
	auto jsonString = FileUtils::getInstance()->getStringFromFile(m_configPath);
	if (!jsonString.length())
		return;

	using namespace rapidjson;
	
	Document resultDocument;
	if (resultDocument.Parse(jsonString.c_str()).HasParseError())
		return;
	auto games = resultDocument["games"].GetArray();

	if (!games.Size())
		return;
	max_game = games.Size();

	m_words.clear();
	m_wordsClone.clear();

	auto singleGame = games[count_game].GetObject();
	auto words = singleGame["words"].GetArray();
	auto audio = singleGame["audio"].GetString();
	{
		auto pImage = math::resource::ResHelper::getImage(m_images, audio);
		if (pImage) {
			m_audioPath = pImage->getAudios("name_1").front();
			m_imagePath = pImage->path;
			m_words_SyncText = pImage->getSyncText();
		}
		else {
			m_audioPath = std::string("sounds/frogsentence/") + audio;
			m_imagePath = "";//"games/frogsentence/Dan and Shane can see six eggs.png";
		}

	}


	for (rapidjson::SizeType i = 0; i < words.Size(); i++) {
		auto singleWord = words[i].GetObject();
		WordSync ws;
		auto correctWord = singleWord["word"].GetString();
		auto pImage = math::resource::ResHelper::getImage(m_images, correctWord);
		if (pImage) {
			ws.word.audio_path = pImage->getAudios("name_1").front();
			ws.word.image_path = pImage->path;
			ws.word.text = pImage->getText("name_1");
		}
		else {
			ws.word.text = correctWord;
			ws.word.audio_path = std::string("sounds/frogsentence/words/") + correctWord + ".mp3";
			ws.word.image_path = "";
		}
		auto answerObjects = singleWord["answers"].GetArray();
		for (rapidjson::SizeType j = 0; j < answerObjects.Size(); j++) {
			auto answer = answerObjects[j].GetString();
			pImage = math::resource::ResHelper::getImage(m_images, answer);
			mj::GameObject go;
			if (pImage) {
				go.text = pImage->getText("name_1");
				go.audio_path = pImage->getAudios("name_1").front();
				go.image_path = pImage->path;
			}
			else {
				go.text = answer;
				go.audio_path = std::string("sounds/frogsentence/") + answer + ".mp3";
				go.image_path = "";
			}
			ws.answers.push_back(go);
		}

		m_words.push_back(ws);
	}

	// get spawntime async
    int audio_id = cocos2d::experimental::AudioEngine::play2d(m_audioPath,false,0.0f);
	this->scheduleOnce([=](float) {
		float spawnTime = 0.0f;
		float time = cocos2d::experimental::AudioEngine::getDuration(audio_id) / m_words.size();
		for (int i = 0; i < m_words.size(); ++i) {
			spawnTime = time * i;
			m_words[i].spawnTime = spawnTime;
		}
	},0.1f,"get duration async");
		
}

void FrogSentenceGame::onDropLeaf(TextLeaf * leaf)
{
	m_beginHaveAction = 0;
	auto leafPos = leaf->getParent()->convertToWorldSpace(leaf->getPosition());
	auto slotPos = m_slot->getParent()->convertToWorldSpace(m_slot->getPosition());
	auto path = slotPos - leafPos;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	if ( (ap::ProfileManager::getInstance().getInformationOfProfile().age  <_age_check || path.length() <= SLOT_SNAP_DISTANCE) && leaf->getText() == m_correctWord.text) {
		AUDIO_ENGINE->stopSound(m_audioPath);
		_soundAvaiable = true;
		AUDIO_ENGINE->stopSound(m_audioPath);
		_soundAvaiable = true;
		
		AUDIO_ENGINE->play("sounds/frogsentence/correct.mp3");
		//TODO:
		AUDIO_ENGINE->play(m_wordsClone.back().word.audio_path, false, 1.0f,[=](void) {
			_soundAvaiable = true;
			//AUDIO_ENGINE->resumEffects(_guidAudioId);
			});

		//        AUDIO_ENGINE->play(std::string("sounds/frogsentence/words/") + leaf->getText() + ".mp3");
		leaf->disableClick();
		leaf->retain();
		leaf->removeFromParentAndCleanup(false);
		m_bgRootNode->addChild(leaf);
		leaf->setPosition(m_bgRootNode->convertToNodeSpace(leafPos));
		leaf->release();

		auto move = MoveTo::create(0.1f, m_slot->getPosition());
		auto scale = leaf->getScale();
		auto move2 = ScaleTo::create(0.1f, 0.8f * scale);
		auto move3 = ScaleTo::create(0.1f, scale);
		leaf->stopAllActions();
		leaf->runAction(Sequence::create(move, move2, move3, nullptr));
		m_leaves.push_back(leaf);
		m_slot->setPosition(m_slot->getPosition() + Vec2(200, 0));
		auto slotWorldPos = m_slot->getParent()->convertToWorldSpace(m_slot->getPosition());
		if (slotWorldPos.x + 300 > visibleSize.width) {
			auto moveCam = MoveBy::create(0.5f, -cocos2d::Vec2(slotWorldPos.x + 300 - visibleSize.width, 0));
			m_bgRootNode->stopAllActions();
			m_bgRootNode->runAction(moveCam);
		}
		showNextWordOrEndGame();
	}
	else {
		auto move = MoveTo::create(0.2f, leaf->getBasePosition());
		leaf->stopAllActions();
		Vector<FiniteTimeAction*> actions;
		actions.pushBack(move);
		if (path.length() < SLOT_SNAP_DISTANCE) {
			AUDIO_ENGINE->play("sounds/frogsentence/wrong.mp3");
			if (CONFIG_MANAGER->isVietnameseStories()) {
				for (auto w : m_wordsClone.back().answers) {
					if (w.text == leaf->getText()) {
						AUDIO_ENGINE->play(w.audio_path);
						break;
					}
				}
			}
			actions.pushBack(Shake::create(0.3f, 10, 50));
		}
		leaf->runAction(Sequence::create(actions));
	}
}

void FrogSentenceGame::playIntroGame()
{
	if (m_slot) {
		m_slot->setVisible(false);
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto rightEdge = visibleSize.width - m_rightTree->getPosition().x;
	auto moveCam = MoveTo::create(0.5f + m_words.size() * 0.1f, cocos2d::Vec2(rightEdge, m_bgRootNode->getPosition().y));
	auto moveBack = MoveTo::create(0.5f + m_words.size() * 0.1f, m_bgRootNode->getPosition());
	Vector<FiniteTimeAction*> actions;
	//actions.pushBack(ScaleTo::create(1.0f, 1.5));
	actions.pushBack(DelayTime::create(1.5f));
	//actions.pushBack(ScaleTo::create(0.5f, 1));
	actions.pushBack(moveCam);
	actions.pushBack(DelayTime::create(1.0f));
	actions.pushBack(moveBack);
	Vector<FiniteTimeAction*> actionsPlayAudio;
	
	actionsPlayAudio.pushBack(CallFunc::create([=]() {
		AUDIO_ENGINE->play("sounds/frogsentence/leaping_frog.mp3", false, 1.0f);
			}));

	auto thum = TextLeaf::create("games/frogsentence/Leaping Frog.png", "");
	//leafSpriteBlink->runAction();
	auto mainSize = Director::getInstance()->getVisibleSize();
	Vector<FiniteTimeAction*> actionsShowThum;
	auto initThumesize = thum->getContentSize();
	float scaleheightWidth = (float)initThumesize.width / (float)initThumesize.height;
	initThumesize.width = mainSize.width/2;
	initThumesize.height = initThumesize.width / scaleheightWidth;
	thum->setContentSize(initThumesize);
	thum->setVisible(true);
	this->addChild(thum);
	thum->setPosition(Vec2(mainSize.width / 2, mainSize.height / 2));

	// show thum game image
	//--------------------------//
	int thumeoOpacity = 0;
	for (int i = 0; i <= 15; i++) {

		thumeoOpacity += 20;
		if (thumeoOpacity >= 255)
			thumeoOpacity = 255;
		actionsShowThum.pushBack(CallFunc::create([=]() {
			thum->setOpacity(thumeoOpacity);
			}));
		actionsShowThum.pushBack(DelayTime::create(0.1f));
	}
	actionsShowThum.pushBack(DelayTime::create(0.5f));
	for (int i = 0; i <= 15; i++) {
		thumeoOpacity -= 20;
		if (thumeoOpacity <= 0)
			thumeoOpacity = 0;
		actionsShowThum.pushBack(CallFunc::create([=]() {
			thum->setOpacity(thumeoOpacity);
			}));
		actionsShowThum.pushBack(DelayTime::create(0.1f));
	}
	actionsShowThum.pushBack(CallFunc::create([=]() {
		thum->setVisible(false);
		}));

	thum->runAction(Sequence::create(actionsShowThum));
	//----------------------------------------//

	actionsPlayAudio.pushBack(DelayTime::create(3.0f));
	actionsPlayAudio.pushBack(CallFunc::create([=]() {
		auto audio_path = CONFIG_MANAGER->isVietnameseStories() ? question_vi : "sounds/frogsentence/guide.mp3";
		AUDIO_ENGINE->play(audio_path, false, 1.0f, [=] {
			setGameState(STARTING);
			});

		}));

	m_bgRootNode->runAction(Sequence::create(actionsPlayAudio));

	m_bgRootNode->runAction(Sequence::create(actions));
}

void FrogSentenceGame::playStartingScript()
{
	count_game++;
	m_beginHaveAction = 0;
	_finishActivity = false;

	// frog animation
	do {
		if (!m_firstTimePlay)
			break;
		m_firstTimePlay = false;
		auto basePosition = m_frog->getPosition();
		auto offset = cocos2d::Vec2(-100, 100);
		m_frog->setPosition(basePosition + offset);
		auto move = MoveTo::create(0.3f, basePosition);
		auto move2 = CallFunc::create([=]() {
			m_frog->playSequence("idle", true);
		});
		m_frog->runAction(Sequence::create(move, move2, nullptr));
		m_frog->playSequence("fall", false);

	} while (0);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto guideNode = cocos2d::Node::create();
	guideNode->setPosition(visibleSize / 2);
	addChild(guideNode);
    
	// guide
	auto sentenceGroup = SentenceGroup::create();
	sentenceGroup->setPosition(0, -200);
	guideNode->addChild(sentenceGroup);
	//guideNode->setScale(sqrt((visibleSize.width - 700) / 350));
	guideNode->setVisible(false);
	m_guideNode = guideNode;
	m_sentenceGroup = sentenceGroup;

	//picture
	auto sprite = Sprite::create(m_imagePath);
	sprite->setScale(350.0f / sprite->getContentSize().height);
    sprite->setPositionY(50);
	guideNode->addChild(sprite);

	m_slot->setVisible(false);
	Vector<FiniteTimeAction*> actions;
	actions.pushBack(DelayTime::create(1.5f));
	actions.pushBack(CallFunc::create([=]() {
		initLeafSlot();
	}));

	actions.pushBack(CallFunc::create([=]() {
		// remove old leaves
		for (auto it = m_leaves.begin(); it != m_leaves.end();) {
			(*it)->removeFromParent();
			it = m_leaves.erase(it);
		}

		guideNode->setVisible(true);
		auto baseScale = guideNode->getScale();
		guideNode->setScale(0.8f * baseScale);
		auto scale = ScaleTo::create(0.4f, baseScale);
		guideNode->runAction(scale);
		sentenceGroup->repeatSentence(m_words, m_words.begin(), [=](int, std::string) {
			}, m_words_SyncText, m_audioPath, {}, cocos2d::Color4B::BLACK, cocos2d::Color4B::RED, cocos2d::Color4B::BLACK);
	}));
	actions.pushBack(DelayTime::create(4.5f));
	actions.pushBack(CallFunc::create([=]() {
		guideNode->setVisible(false);
		setGameState(PLAYING);
		}));
	m_bgRootNode->runAction(Sequence::create(actions));


	this->schedule([=](float) {

		m_beginHaveAction += 1;
		showDragGuideGame();
		_countTimer++;
		}, 1, 1000, 1, "runTimeSchedule");
}

void FrogSentenceGame::playEndingScript()
{
	_finishActivity= true;

	// move camera
	{
		auto currentPos = m_bgRootNode->getPosition();
		auto startingPos = currentPos;
		startingPos.x = 0;
		m_bgRootNode->stopAllActions();
		auto move1 = DelayTime::create(1.5f);
		auto move2 = MoveTo::create(0.5f + m_leaves.size() * 0.1f, startingPos);
		auto move3 = DelayTime::create(0.5f);
		auto move4 = MoveTo::create(0.5f + m_leaves.size() * 0.4f, currentPos - cocos2d::Vec2(100, 0));
		m_bgRootNode->runAction(Sequence::create(move1, move2, move3, move4, nullptr));
	}

	// frog jump
	{
		Vector<FiniteTimeAction*> actions;
		actions.pushBack(DelayTime::create(2.8f));
		auto basePos = (*m_leaves.begin())->getPosition();
		basePos.y += 70;
		basePos.x -= 180;
		int i = 0;
		auto guideNode = cocos2d::Node::create();
		auto visibleSize = Director::getInstance()->getVisibleSize();
		guideNode->setPosition(visibleSize / 2);
		addChild(guideNode);

		auto sentenceGroup = SentenceGroup::create();
		sentenceGroup->setPosition(0, -200);
		guideNode->addChild(sentenceGroup);
		//guideNode->setScale(sqrt((visibleSize.width - 700) / 350));
		guideNode->setVisible(false);

		actions.pushBack(CallFunc::create([=]() {
			// remove old leave

			sentenceGroup->repeatSentence(m_wordsClone, m_wordsClone.begin(), [=](int, std::string) {


				}, m_words_SyncText, m_audioPath, m_leaves, cocos2d::Color4B::WHITE, cocos2d::Color4B::RED, cocos2d::Color4B::WHITE);
			}));

		float sumDelay = 0.0;

		for (int i = 0; i < m_words_SyncText.size(); i++)
		{
			sumDelay += (m_words_SyncText[i].e - m_words_SyncText[i].s) / 1000.0f;
		}
		actions.pushBack(DelayTime::create(sumDelay));
		for (auto it = m_leaves.begin(); it != m_leaves.end(); it++) {
			auto leaf = (*it);
			actions.pushBack(CallFunc::create([=]() {
				AUDIO_ENGINE->play("sounds/frogsentence/jump.mp3");
			}));
			basePos.x += 200;
			actions.pushBack(CallFunc::create([=]() {
				m_frog->playSequence("jump", false);
				m_frog->scheduleOnce([=](float) {
					m_frog->playSequence("fall", false);
				}, 0.28f, "fall_schedule");
			}));
			//actions.pushBack(CallFunc::create([=]() {
			//	this->scheduleOnce([=](float) {
			//		AUDIO_ENGINE->play(m_wordsClone[i].word.audio_path);
			//	}, 0.32f, m_wordsClone[i].word.audio_path);
			//}));
			actions.pushBack(JumpTo::create(0.56f, basePos, 100, 1));


			//actions.pushBack(CallFunc::create([=]() {
			//	AUDIO_ENGINE->play(m_wordsClone[i].word.audio_path);
			//}));
			actions.pushBack(CallFunc::create([=]() {
				auto baseScale = leaf->getScale();
				leaf->setTexture("games/frogsentence/2_white.png");
				auto scaleDown = ScaleTo::create(0.1f, baseScale * 0.8f);
				auto scaleUp = ScaleTo::create(0.1f, baseScale);
				auto func = CallFunc::create([=]() {
					leaf->setTexture("games/frogsentence/2.png");
				});
				leaf->runAction(Sequence::create(scaleDown, scaleUp, func, nullptr));
			}));
			i++;
		}
		auto frogPos = m_frog->getPosition();
		frogPos.x += (200 * m_leaves.size() + 300);
		actions.pushBack(CallFunc::create([=]() {
			AUDIO_ENGINE->play("sounds/frogsentence/jump.mp3");
		}));
		actions.pushBack(CallFunc::create([=]() {
			m_frog->playSequence("jump", false);
			m_frog->scheduleOnce([=](float) {
				m_frog->playSequence("fall", false);
			}, 0.28f, "fall_schedule");
		}));
		actions.pushBack(JumpTo::create(0.58f, frogPos, 150, 1));
		actions.pushBack(CallFunc::create([=]() {
			m_frog->playSequence("idle", true);
		}));
		actions.pushBack(DelayTime::create(1.5f));

		// show next session of game
		actions.pushBack(CallFunc::create([=]() {
			auto pos = m_guideNode->getPosition();
			pos.x = Director::getInstance()->getVisibleSize().width - pos.x;
			m_guideNode->setVisible(true);
			m_guideNode->setPosition(pos);

            m_sentenceGroup->repeatSentence(m_words, m_words.begin(), [=](int, std::string) {
                if (count_game == max_game) {
                    setGameState(FINISH);
                }
                else {
                    setGameState(TRANSITIONING);
                }
                m_guideNode->setVisible(false);
            }, m_words_SyncText, m_audioPath);
		}));

		m_frog->runAction(Sequence::create(actions));
	}

	// hide words
	{
		auto pos = m_selectFrame->getPosition();
		pos.y = -140 * 1.3f;
		auto move = MoveTo::create(0.4f, pos);
		m_selectFrame->runAction(move);
	}
}

void FrogSentenceGame::playTransitioningScript()
{
	Vector<FiniteTimeAction*> actions;
	auto visibleSize = Director::getInstance()->getWinSize();
	auto transitionOffset = m_rightTree->getPosition() - m_leftTree->getPosition();

	// frog walk
	{
		m_frog->playSequence("walk", true);
		auto moveAction = MoveBy::create(2.0f, cocos2d::Vec2(m_rightTree->getPosition().x + 300 - m_frog->getPosition().x, 0));
		auto idle = CallFunc::create([=]() {
			m_frog->playSequence("idle", true);
		});
		m_frog->runAction(Sequence::create(moveAction, idle, nullptr));
	}

	// move half of screen
	actions.pushBack(MoveBy::create(1.5f, cocos2d::Vec2(-visibleSize.width / 2 - 100, 0)));
	actions.pushBack(CallFunc::create([=]() {
		m_leftTree->setPosition(m_rightTree->getPosition() + transitionOffset);
		auto temp = m_rightTree;
		m_rightTree = m_leftTree;
		m_leftTree = temp;
	}));
	// move another half
	actions.pushBack(MoveBy::create(1.5f, cocos2d::Vec2(-visibleSize.width / 2 - 100, 0)));

	// translation
	actions.pushBack(CallFunc::create([=]() {
		auto children = m_bgRootNode->getChildren();
		for (auto it = children.begin(); it != children.end(); it++) {
			(*it)->setPosition((*it)->getPosition() - transitionOffset);
		}
		m_bgRootNode->setPosition(m_bgRootNode->getPosition() + transitionOffset);
	}));

	auto rootPos = m_bgRootNode->getPosition();
	auto moveDistance = std::abs(rootPos.x - visibleSize.width + transitionOffset.x);
	rootPos.x = m_leftTree->getPosition().x + 150;
	actions.pushBack(MoveTo::create(moveDistance / visibleSize.width * 3.0f, rootPos));

	actions.pushBack(CallFunc::create([=]() {
		setGameState(STARTING);
	}));

	m_bgRootNode->runAction(Sequence::create(actions));
}

void FrogSentenceGame::setGameState(FrogSentence::GameState state)
{
	if (state == m_gameState)
		return;
	switch (state)
	{
	case FrogSentence::STARTING:
		getGameData();
		playStartingScript();
		break;
	case FrogSentence::PLAYING:
		showNextWordOrEndGame();
		showDragGuideGame();
		break;
	case FrogSentence::ENDING:
		playEndingScript();
		break;
	case FrogSentence::TRANSITIONING:
		playTransitioningScript();
		break;
	case FrogSentence::FINISH:
        AUDIO_ENGINE->stopAllAudio();
		onFinishGame();
		break;
	case FrogSentence::NONE:
		break;
	default:
		break;
	}
	m_gameState = state;
	m_slot->setVisible(m_gameState == FrogSentence::PLAYING);
	m_frog->setVisible(m_gameState != FrogSentence::NONE);
}

void FrogSentenceGame::showGuideGame()
{
	if (!_soundAvaiable)
		return;
	Vector<FiniteTimeAction*> actions;
	_soundAvaiable = false;
	actions.pushBack(CallFunc::create([=]() {
		 AUDIO_ENGINE->playSound(m_audioPath, false, [=]() {
			_soundAvaiable = true;
			});
		}));

	this->runAction(Sequence::create(actions));
}

void FrogSentenceGame::showDragGuideGame(float delay)
{

		if (_finishActivity)
		{
			if (this->isScheduled("runTimeSchedule"))
			{
				AUDIO_ENGINE->stopSound(m_audioPath);
				_soundAvaiable = true;
				this->unschedule("runTimeSchedule");
			}
			m_wordsClone2.clear();
			return;
		}

		else if(_countTimer >1000) // restart guide schedule again  when schedule over duration 
		{
			_countTimer = 0;
			if (this->isScheduled("runTimeSchedule"))
			{
				AUDIO_ENGINE->stopSound(m_audioPath);
				_soundAvaiable = true;
				this->unschedule("runTimeSchedule");
			}

			this->schedule([=](float) {

				m_beginHaveAction += 1;
				showDragGuideGame();
				_countTimer++;
				}, 1, 1000, 1, "runTimeSchedule");
			return;
		}

		if (m_beginHaveAction >= 13) {

			for (WordSync word : m_words)
			{
				m_wordsClone2.push_back(word);
			}
			if (_soundAvaiable) {
				if (!CONFIG_MANAGER->isVietnameseStories() && m_firstTimePlay) {
					_soundAvaiable = false;
					 AUDIO_ENGINE->playSound("sounds/frogsentence/noti_guide.mp3", false, [=](void) {_soundAvaiable = true; });
					return;
				}
			}
			auto slotPos = m_slot->getParent()->convertToWorldSpace(m_slot->getPosition());
			auto basePos = cocos2d::Vec2(100, m_selectFrame->getContentSize().height / 2);
			auto offset = cocos2d::Vec2(200, 0);
			auto leafPos = basePos + offset;

			// blink correct Item guiding----//
			for (auto c : m_selectFrame->getChildren()) {
				auto item = (TextLeaf *)c;
				if (item->getText() == m_correctWord.text) {
					leafPos = item->getParent()->convertToWorldSpace(item->getPosition());
					if (ap::ProfileManager::getInstance().getInformationOfProfile().age < _age_check) {
						item->schedule([=](float) {

							Vector<FiniteTimeAction*> actions;
//							_soundAvaiable = false;
							actions.pushBack(CallFunc::create([=]() {
								item->setVisible(false);
								}));
							actions.pushBack(DelayTime::create(0.2f));

							actions.pushBack(CallFunc::create([=]() {
								item->setVisible(true);
								}));
							item->runAction(Sequence::create(actions));

							}, 0.4f, 5, 0.1f, "blinkCorrectItem");
					}

					break;
				}
			}
			//--------------------------------------//

			if (_soundAvaiable) {

				_soundAvaiable = false;
				 AUDIO_ENGINE->playSound(m_audioPath, false, [=]() {
					_soundAvaiable = true;
					});
			}

			if(ap::ProfileManager::getInstance().getInformationOfProfile().age >=  _age_check)
				MJMainGBase::showDragGuideGame(leafPos, slotPos, 0, 0.1f, 1.0f, "gaf/guidegame/hand_tap/hand_tap.gaf", this);
			m_beginHaveAction = 0;
		}
}
