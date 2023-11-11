#include "CatchingThiefGame.h"
#include "BackgroundNode.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "HSAudioEngine.h"

#define BACKGROUND_MOVE_SPEED 500

CatchingThiefGame * CatchingThiefGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = CatchingThiefGame::create();
	if (json_file.length() > 5)
		result->m_configPath = json_file;
	return result;
}

bool CatchingThiefGame::init()
{
	if (!MJMainGBase::init())
		return false;
	initBackground();
	initCharacter();
	return true;
}

void CatchingThiefGame::onEnter()
{
	MJMainGBase::onEnter();
	startGame();
}

void CatchingThiefGame::onExit()
{
    MJMainGBase::hideTapGuideGame();
    MJMainGBase::onExit();
}

CatchingThiefGame::CatchingThiefGame() :
	m_configPath("json/catchingthief/config.json"),
	m_backgroundNode(nullptr),
	m_police(nullptr),
	m_thief(nullptr),
	m_gameNode(nullptr),
	m_wordFrame(nullptr),
	m_letterIndex(0),
	m_clickable(false)
{
}

void CatchingThiefGame::initBackground()
{
	auto bgNode = BackgroundNode::createBackground("games/catchingthief/3_1.png", "games/catchingthief/3_2.png", BACKGROUND_MOVE_SPEED);
	addChild(bgNode);
	m_backgroundNode = bgNode;
	

	auto gameNode = cocos2d::Node::create();
	addChild(gameNode);
	m_gameNode = gameNode;

	// prevent click
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event* event) {
		CCLOG("touch began");
		if (m_correctAnswer != nullptr)
		{
			scheduleGuidingGame(*m_correctAnswer);
		}	
		return true;
	};

	// guide btn 
	auto guideBtn = ui::Button::create("games/catchingthief/speaker.png");
	auto visibleSize = Director::getInstance()->getVisibleSize();
	guideBtn->setPosition(cocos2d::Vec2(guideBtn->getContentSize().width / 2 + 20, visibleSize.height - guideBtn->getContentSize().height / 2));
	m_gameNode->addChild(guideBtn);
	guideBtn->addClickEventListener([=](Ref*) {
		if (m_letterIndex < 1 || m_letterIndex > m_correctLetters.size())
			return;
		if (!m_clickable)
			return;
		AUDIO_ENGINE->playSound(m_correctLetters[m_letterIndex - 1].audio_path);
		// schedule next guiding
		this->unschedule("show_delay");
		this->unschedule("sound_guide");
		scheduleGuidingGame(m_correctLetters[m_letterIndex - 1]);
	});

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void CatchingThiefGame::initCharacter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto police = GAFWrapper::createGAF("gaf/catchingthief/police.gaf");
	police->playSequence("run", true);
	police->setPosition(cocos2d::Vec2(visibleSize.width / 2, 250));
	m_gameNode->addChild(police);
	m_police = police;

	auto thief = GAFWrapper::createGAF("gaf/catchingthief/thief.gaf");
	thief->playSequence("run", true);
	thief->setPosition(cocos2d::Vec2(visibleSize.width + 300, 250));
	m_gameNode->addChild(thief);
	m_thief = thief;
}

void CatchingThiefGame::startGame()
{
	m_letterIndex = 0;
	getGameData();
	initWordFrame();
	playStartingScript();
	AUDIO_ENGINE->playBackgroundMusic("sounds/catchingthief/music.mp3", 0.8f);
	this->runAction(
		Sequence::create(
			DelayTime::create(1),
			CallFunc::create([this] 
				{
					auto nameGame = ui::ImageView::create("games/catchingthief/catch_the_thief_2.png");
					auto visibleSize = Director::getInstance()->getVisibleSize();
					nameGame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.8));
					nameGame->setName("nameGame");
					nameGame->setOpacity(0);
					this->addChild(nameGame);
					nameGame->runAction(FadeIn::create(1));
					if (FileUtils::getInstance()->isFileExist("sounds/catchingthief/intro.mp3"))
						AUDIO_ENGINE->playSound("sounds/catchingthief/intro.mp3");
				}),
			DelayTime::create(3),
			CallFunc::create([this] {
					this->removeChildByName("nameGame");
					if (FileUtils::getInstance()->isFileExist("sounds/catchingthief/introduction.mp3"))
						AUDIO_ENGINE->playSound("sounds/catchingthief/introduction.mp3");				
				}),
			nullptr
		)
	);
}

void CatchingThiefGame::initWordFrame()
{
	// 	if (m_wordFrame)
	// 		m_wordFrame->removeFromParent();
	// 
	// 	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 	auto wordFrame = CatchingThief::WordFrame::createWithLetters(m_correctLetters);
	// 	wordFrame->setPosition(300, visibleSize.height - 150);
	// 	addChild(wordFrame);
	// 	m_wordFrame = wordFrame;
}

void CatchingThiefGame::getGameData()
{
	auto jsonString = FileUtils::getInstance()->getStringFromFile(m_configPath);
	if (!jsonString.length())
		return;
	using namespace rapidjson;

// 	std::string schemaPath = "json/catchingthief/schema.json";
// 	std::string schemaJson = FileUtils::getInstance()->getStringFromFile(schemaPath);
// 	if (!schemaJson.length())
// 		return;
// 	
// 	Document sd;
// 	if (sd.Parse(schemaJson.c_str()).HasParseError())
// 		return;
// 
// 	SchemaDocument schema(sd);

	Document value_data;
	if (value_data.Parse(jsonString.c_str()).HasParseError())
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
	m_correctLetters.clear();
    
    
    if (value_data.HasMember("game_mode") && value_data["game_mode"].IsInt()) {
        number_turnplay = value_data["game_mode"].GetInt();
    }
    if (value_data.HasMember("option_vm_phonic") && value_data["option_vm_phonic"].IsString()) {
        hoc_am_hoc_van = value_data["option_vm_phonic"].GetString();
    }
	is_word = false;
    if (value_data.HasMember("data") && value_data["data"].IsArray()) {
        auto list_item = value_data["data"].GetArray();
        for (int i = 0; i < (int) list_item.Size(); i++) {
           std::vector<mj::GameObject> answers;
            for (int j = 0; j < 3; j++) {
                mj::GameObject item_info;
                auto letter = list_item[i][j].FindMember("full_text")->value.GetString();
                auto pImage = math::resource::ResHelper::getImage(m_images, letter);
				if (pImage->getComparisionValue("role") == 1) is_word = true;
                item_info.text = pImage->getText("name_1");
                item_info.audio_path = pImage->getAudios("name_1").front();
                item_info.image_path = pImage->path;
                
                auto correct = list_item[i][j].HasMember("is_correct") ? list_item[i][j].FindMember("is_correct")->value.GetBool() : false;
                if(correct){
//                    m_correctWord = item_info;
                    m_correctLetters.push_back(item_info);
                   
                }
                answers.push_back (item_info);
            }
            m_questions.push_back(answers);
        }
    }

//    auto games = resultDocument["games"].GetArray();
//    auto singleGameObj = games[0].GetObject();
//
//    auto correctWord = singleGameObj["correctWord"].GetString();
//    {
//        auto pImage = math::resource::ResHelper::getImage(m_images, correctWord);
//        mj::GameObject go;
//        if (pImage) {
//            go.text = pImage->getText();
//            go.audio_path = pImage->getAudio();
//            go.image_path = pImage->path;
//        }
//        else {
//            go.text = correctWord;
//            go.audio_path = std::string("sounds/catchingthief/letters/") + correctWord + "/n_" + correctWord + ".mp3";
//            go.image_path = std::string("games/catchingthief/words/") + correctWord + ".png";
//        }
//        m_correctWord = go;
//    }
//
//    auto correctLetters = singleGameObj["correctLetters"].GetArray();
//    for (rapidjson::SizeType i = 0; i < correctLetters.Size(); i++) {
//        auto letter = correctLetters[i].GetString();
//        auto pImage = math::resource::ResHelper::getImage(m_images, letter);
//        mj::GameObject go;
//        if (pImage) {
//            go.text = pImage->getText();
//            go.audio_path = pImage->getAudio();
//            go.image_path = pImage->path;
//        }
//        else {
//            go.text = letter;
//            go.audio_path = std::string("sounds/catchingthief/letters/") + m_correctWord.text + "/" + letter + ".mp3";
//        }
//        m_correctLetters.push_back(go);
//    }
//
//    auto questionArray = singleGameObj["promptAnswers"].GetArray();
//    for (rapidjson::SizeType i = 0; i < questionArray.Size(); i++) {
//        CatchingThief::Question question;
//        auto answerArray = questionArray[i].GetArray();
//        for (rapidjson::SizeType j = 0; j < answerArray.Size(); j++) {
//            auto questionStr = answerArray[j].GetString();
//            auto pImage = math::resource::ResHelper::getImage(m_images, questionStr);
//            mj::GameObject go;
//            if (pImage) {
//                go.audio_path = pImage->getAudio();
//                go.text = pImage->getText();
//                go.image_path = pImage->path;
//            }
//            else {
//                go.text = questionStr;
//                go.audio_path = std::string("sounds/catchingthief/letters/") + m_correctWord.text + "/" + questionStr + ".mp3";
//            }
//            question.push_back(go);
//        }
//        m_questions.push_back(question);
//    }
}

void CatchingThiefGame::playStartingScript()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_police->setPosition(cocos2d::Vec2(-200, 250));
	m_thief->setPosition(cocos2d::Vec2(visibleSize.width + 300, 250));
	Vector<FiniteTimeAction*> actions;
	actions.pushBack(DelayTime::create(1.5f));
	actions.pushBack(CallFunc::create([=]() {
		AUDIO_ENGINE->playSound("sounds/catchingthief/whistle.mp3", false);
		AUDIO_ENGINE->playSound("sounds/catchingthief/run.mp3", true);
	}));
	actions.pushBack(CallFunc::create([=]() {
		auto pos = m_police->getPosition();
		pos.x = visibleSize.width / 2;
		m_police->runAction(MoveTo::create(1.5f, pos));
	}));
	actions.pushBack(DelayTime::create(1.8f));
	actions.pushBack(CallFunc::create([=]() {
		auto pos = m_police->getPosition();
		pos.x = visibleSize.width / 2 - 350;
		m_police->runAction(MoveTo::create(1.2f, pos));
		pos.x = visibleSize.width - 250;
		m_thief->runAction(MoveTo::create(1.2f, pos));
	}));
	actions.pushBack(CallFunc::create([=]() {
		// actually start
		m_question_path = "sounds/catchingthief/Tap on the sound that you hear.mp3";
		if (is_word) m_question_path = "sounds/catchingthief/Tap on the word that you hear.mp3";
        if (CONFIG_MANAGER->isGameVMonkey()){
            m_question_path = hoc_am_hoc_van=="hoc_am"?"sounds/catchingthief/question_vi_am.mp3":"sounds/catchingthief/question_vi_van.mp3";
        }
		this->runAction(
			Sequence::create(
				DelayTime::create(6.5),
				CallFunc::create([this] {
						AUDIO_ENGINE->playSound(m_question_path, false, [=]() {
						showNextQuestion();
						});
					}),
				nullptr
			)
		);
	}));
	runAction(Sequence::create(actions));
}

void CatchingThiefGame::showNextQuestion()
{
	if (checkWinCondition()) {
		playEndingScript();
		return;
	}
	this->unschedule("sound_guide");
	showQuestion(m_questions.at(m_letterIndex), m_correctLetters[m_letterIndex]);

	m_letterIndex++;
}

void CatchingThiefGame::showQuestion(CatchingThief::Question& answer, mj::GameObject & correctAnswer)
{
	m_correctAnswer = &correctAnswer;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto counter = 0;
	auto basePos = cocos2d::Vec2(visibleSize.width / 4 - 50, visibleSize.height + 120);
	auto offset = cocos2d::Vec2(visibleSize.width / 4 + 50, 0);

	std::vector<float> delayOffset = { 0.7f,1,1.3f };
	std::random_shuffle(delayOffset.begin(), delayOffset.end());

	m_gameNode->runAction(Sequence::createWithTwoActions(
		DelayTime::create(2),
		CallFunc::create([=]() {
		m_clickable = true;
	})
	));

	this->runAction(Sequence::create(
		DelayTime::create(1),
		CallFunc::create([=]() {
			scheduleOnce([=](float) {
				AUDIO_ENGINE->playSound("sounds/catchingthief/Tap on.mp3", false, [=]() {
					AUDIO_ENGINE->playSound(correctAnswer.audio_path, false);
					});
				}, 1, "show_delay");
			}
		),
		DelayTime::create(3),
		CallFunc::create([=]() {
				scheduleGuidingGame(*m_correctAnswer);
			}
		),
		nullptr
	));

    mj::helper::shuffle(answer);
	static Vec2 pos_true;
	for (auto it = answer.begin(); it != answer.end(); it++) {
		auto btn = cocos2d::ui::Button::create("games/catchingthief/button.png");
		btn->setPosition(basePos + offset * counter);
		btn->setZoomScale(0);
        std::string font_name = CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getInstance()->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF";
		auto label = Label::create(it->text, font_name,70);
		label->setTextColor(Color4B::BLACK);
		label->setPosition(btn->getContentSize() / 2);
		btn->addChild(label);
		btn->setUserObject(label);
		btn->setUserData(&(*it));
        btn->setName(it->audio_path);
		auto pos = btn->getPosition();
		pos.y = visibleSize.height - 120;
		btn->runAction(Sequence::createWithTwoActions(
			DelayTime::create(delayOffset[counter]),
			EaseBackOut::create(MoveTo::create(0.8f, pos))
		));
		if ((*it) == correctAnswer) {
			pos_true = pos;
			inActiveMode(pos,12);
		}
		m_gameNode->addChild(btn, 1);

		btn->addClickEventListener([=](Ref* ref) {
			
			if (!m_clickable)
				return;
			m_clickable = false;
			if ((*it) == correctAnswer) {
				unschedule("active_mode");
				onCorrectAnswer(btn);
			}				
			else {
				unschedule("active_mode");
				//inActiveMode(pos_true,10);
				onWrongAnswer(btn);
			}
				
		});
		m_answerBtn.push_back(btn);
		counter++;
	}
}

void CatchingThiefGame::clearAnswerBtns()
{
	for (auto it = m_answerBtn.begin(); it != m_answerBtn.end();) {
		(*it)->removeFromParent();
		it = m_answerBtn.erase(it);
	}
}

void CatchingThiefGame::onWrongAnswer(cocos2d::ui::Button * btn)
{
	//// schedule next guiding
	this->unschedule("sound_guide");
	this->unschedule("show_delay");
	if (m_letterIndex > 0)
	{
		scheduleGuidingGame(m_correctLetters[m_letterIndex - 1]);
	}

	cocos2d::Vector<FiniteTimeAction*> actions;
	actions.pushBack(CallFunc::create([=]() {
		btn->loadTextureNormal("games/catchingthief/button_wrong.png");
	}));
	auto pos = btn->getPosition();
	for (int i = 0; i < 2; i++) {
		actions.pushBack(MoveTo::create(0.15f, pos - cocos2d::Vec2(0, 10)));
		actions.pushBack(MoveTo::create(0.15f, pos + cocos2d::Vec2(0, 10)));
	}
	actions.pushBack(MoveTo::create(0.15f, pos));
	actions.pushBack(CallFunc::create([=]() {
		btn->loadTextureNormal("games/catchingthief/button.png");
	}));
	actions.pushBack(DelayTime::create(0.2));
	actions.pushBack(CallFunc::create([=]() {
	}));
	btn->runAction(Sequence::create(actions));
	playWrongScript();
	AUDIO_ENGINE->playSound("sounds/catchingthief/wrong.mp3", false);
    if (CONFIG_MANAGER->isVietnameseStories()) AUDIO_ENGINE->playSound(btn->getName(), false);
}

void CatchingThiefGame::onCorrectAnswer(cocos2d::ui::Button * btn)
{
	this->unschedule("show_delay");
	this->unschedule("sound_guide");

	btn->setVisible(false);
	auto effect = GAFWrapper::createGAF("gaf/catchingthief/effect.gaf");
	effect->setPosition(btn->getPosition());
	m_gameNode->addChild(effect);
	effect->play(false, [=](gaf::GAFObject* obj) {
		effect->removeFromParent();
		clearAnswerBtns();
	});
	auto label = dynamic_cast<Label*>(btn->getUserObject());
	if (label) {
		auto pos = label->getParent()->convertToWorldSpace(label->getPosition());
		label->removeFromParentAndCleanup(false);
		m_gameNode->addChild(label, 5);
		label->setPosition(pos);
		Vector<FiniteTimeAction*> actions;
		actions.pushBack(CallFunc::create([=]() {
			label->runAction(EaseOut::create(ScaleTo::create(0.7f, 2.5f), 1.4f));
		}));
		actions.pushBack(DelayTime::create(0.35f));
		actions.pushBack(CallFunc::create([=]() {
			mj::GameObject *pGo = (mj::GameObject*)btn->getUserData();
			if (pGo)
				AUDIO_ENGINE->playSound(pGo->audio_path);
			label->removeFromParent();
		}));
		label->runAction(Sequence::create(actions));
	}

	//m_wordFrame->reveal();
	playCorrectScript();
	AUDIO_ENGINE->playSound("sounds/catchingthief/correct.mp3", false);
}

void CatchingThiefGame::playWrongScript()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto trash = GAFWrapper::createGAF("gaf/catchingthief/trash.gaf");
	trash->setAnchorPoint(cocos2d::Vec2(0.2f, 0));
	cocos2d::Vec2 pos(visibleSize.width + 200, 70);
	trash->setPosition(pos);
	m_gameNode->addChild(trash);
	pos.x = -200;
	trash->runAction(Sequence::createWithTwoActions(
		MoveTo::create((visibleSize.width + 400) / BACKGROUND_MOVE_SPEED, pos),
		CallFunc::create([=]() {
		m_clickable = true;
		trash->removeFromParent();
	})
	));

	auto reachThiefDuration = (trash->getPosition().x - m_thief->getPosition().x) / BACKGROUND_MOVE_SPEED;
	jumpCharacter(reachThiefDuration, m_thief);

	auto reachPoliceDuration = (trash->getPosition().x - m_police->getPosition().x) / BACKGROUND_MOVE_SPEED;
	hitTrash(reachPoliceDuration - 0.2f, trash);
}

void CatchingThiefGame::playCorrectScript()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto trash = GAFWrapper::createGAF("gaf/catchingthief/trash.gaf");
	trash->setAnchorPoint(cocos2d::Vec2::ZERO);
	cocos2d::Vec2 pos(visibleSize.width + 200, 70);
	trash->setPosition(pos);
	m_gameNode->addChild(trash);
	pos.x = -200;
	trash->runAction(MoveTo::create((visibleSize.width + 400) / BACKGROUND_MOVE_SPEED, pos));

	auto reachThiefDuration = (trash->getPosition().x - m_thief->getPosition().x) / BACKGROUND_MOVE_SPEED;
	jumpCharacter(reachThiefDuration, m_thief);

	auto reachPoliceDuration = (trash->getPosition().x - m_police->getPosition().x) / BACKGROUND_MOVE_SPEED;
	jumpCharacter(reachPoliceDuration + 0.1f, m_police);

	m_gameNode->runAction(Sequence::createWithTwoActions(
		DelayTime::create(reachPoliceDuration + 0.5f),
		CallFunc::create([=]() {
		showNextQuestion();
	})
	));
}

void CatchingThiefGame::playEndingScript()
{
	auto endingNode = cocos2d::Node::create();
	addChild(endingNode);

	auto whiteLayer = LayerColor::create(Color4B::WHITE);
	endingNode->addChild(whiteLayer);

	whiteLayer->setOpacity(0);
	//whiteLayer->runAction(FadeIn::create(1));

	auto catchDuration = (m_thief->getPosition().x - m_police->getPosition().x - 160) / BACKGROUND_MOVE_SPEED;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_police->runAction(Sequence::createWithTwoActions(
		MoveTo::create(catchDuration, m_thief->getPosition() - cocos2d::Vec2(160, 0)),
		CallFunc::create([=]() {
		auto boom = GAFWrapper::createGAF("gaf/catchingthief/boom.gaf");
		boom->setPosition(m_thief->getPosition());
		m_gameNode->addChild(boom);
		AUDIO_ENGINE->stopSound("sounds/catchingthief/music.mp3");
		AUDIO_ENGINE->stopSound("sounds/catchingthief/run.mp3");
		
		//if (FileUtils::getInstance()->isFileExist("sounds/catchingthief/congratulation.mp3"))
		//	AUDIO_ENGINE->play("sounds/catchingthief/congratulation.mp3", false, 0.8f);
		AUDIO_ENGINE->play("sounds/catchingthief/crash.mp3", false,0.8f);
		
		if (FileUtils::getInstance()->isFileExist("sounds/catchingthief/scream.mp3"))
			AUDIO_ENGINE->play("sounds/catchingthief/scream.mp3", false, 0.8f);	
		boom->play(false, [=](gaf::GAFObject* obj) {
			boom->removeFromParent();
		});

        // fade out
		Vector<FiniteTimeAction*> showJailActions;
		showJailActions.pushBack(FadeIn::create(0.5f));
		showJailActions.pushBack(CallFunc::create([=]() {
			// init jail 
			auto jail = Sprite::create("games/catchingthief/2.png");
			jail->setAnchorPoint(cocos2d::Vec2(0.2, 0.5f));
			jail->setPosition(150, visibleSize.height / 2);
            float scale = 0.9f;
            if(screen_size.width > jail->getContentSize().width * 0.9f){
                scale = screen_size.width / ((jail->getContentSize().width - 50) * 0.9);
            }
			jail->setScale(scale);
			endingNode->addChild(jail, -1);
			jail->runAction(Sequence::createWithTwoActions(
				DelayTime::create(1),
				ScaleTo::create(1.2f, scale + 0.3)
			));
			AUDIO_ENGINE->play("sounds/catchingthief/prison.mp3",false,2.0f);
		}));
		showJailActions.pushBack(FadeOut::create(0.8f));
		showJailActions.pushBack(DelayTime::create(3.5f));
		showJailActions.pushBack(CallFunc::create([=]() {
//            endingNode->removeFromParent();
//            startGame();
            MJMainGBase::onFinishGame();
		}));
		whiteLayer->runAction(Sequence::create(showJailActions));
	})
	));
}

bool CatchingThiefGame::checkWinCondition()
{
	return m_letterIndex >= m_correctLetters.size();
}

void CatchingThiefGame::scheduleGuidingGame(mj::GameObject& correctAnswer)
{
	CCLOG("contacted");
	this->unschedule("sound_guide");
	this->schedule([=](float) 
		{
			{
				int sound_id = 0;
				AUDIO_ENGINE->playSound("sounds/catchingthief/Tap on.mp3", false, [=]() {
					AUDIO_ENGINE->playSound(correctAnswer.audio_path, false, ([=] {
						scheduleGuidingGame(*m_correctAnswer);
						}));
					});				
			}
		}, 10, 100, 10, "sound_guide");
}

void CatchingThiefGame::jumpCharacter(float catchDuration, GAFWrapper* character)
{
	Vector<FiniteTimeAction*> characterActions;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	characterActions.pushBack(DelayTime::create(catchDuration - 0.25f));
	characterActions.pushBack(CallFunc::create([=]() {
		character->playSequence("jump", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			character->playSequence("air", true);
		});
	}));
	auto fallAction = Sequence::createWithTwoActions(
		DelayTime::create(0.25f),
		CallFunc::create([=]() {
		character->playSequence("fall", true);
		if (character == m_thief) {
			AUDIO_ENGINE->playSound("sounds/catchingthief/jump2.mp3", false);
		}
		else if (character == m_police) {
			AUDIO_ENGINE->playSound("sounds/catchingthief/jump.mp3", false);
		}
	})
	);
	characterActions.pushBack(
		Spawn::createWithTwoActions(JumpTo::create(0.5f, character->getPosition(), 200, 1),
			fallAction
		));
	characterActions.pushBack(CallFunc::create([=]() {
		character->playSequence("run", true);
	}));
	if (character == m_police) {
		auto pos = m_police->getPosition();
        auto remain_answer = m_correctLetters.size() - m_letterIndex;
        if(remain_answer <= 0) remain_answer = 1;
        auto delta = (m_thief->getPositionX() - m_police->getPositionX()) / remain_answer;
		pos.x = std::min(visibleSize.width - 300, pos.x + std::min(delta,100.0f));
		characterActions.pushBack(MoveTo::create(0.6f, pos));
	}
    
	character->runAction(Sequence::create(characterActions));
}

void CatchingThiefGame::hitTrash(float hitDuration, cocos2d::Node* trash)
{
	Vector<FiniteTimeAction*> actions;
	auto trashGaf = dynamic_cast<GAFWrapper*>(trash);
	actions.pushBack(DelayTime::create(hitDuration));
	actions.pushBack(CallFunc::create([=]() {
		auto boom = GAFWrapper::createGAF("gaf/catchingthief/boom.gaf");
		boom->setPosition(m_police->getPosition());
		m_gameNode->addChild(boom);
		if (trashGaf) {
			trashGaf->playSequence("crash", false);
		}
		boom->play(true, [=](gaf::GAFObject* obj) {
			boom->removeFromParent();
		});
		AUDIO_ENGINE->playSound("sounds/catchingthief/crash.mp3", false);
	}));
	auto pos = m_police->getPosition();
	pos.x = std::max(120.0f, pos.x - 100);
	actions.pushBack(MoveTo::create((m_police->getPosition().x - pos.x) / BACKGROUND_MOVE_SPEED, pos));
	m_police->runAction(Sequence::create(actions));
}

void CatchingThiefGame::playSynctext(std::vector<Label*> nodes, std::function<void()> callback)
{
	auto count = 0;
	auto audioIt = m_correctLetters.begin();
	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		auto node = *it;

		// skip tail node
		if (node->getUserData())
			continue;
		while (node) {
			Vector<FiniteTimeAction*> actions;
			actions.pushBack(DelayTime::create(count * 0.7f));
			actions.pushBack(CallFunc::create([=]() {
// 				auto path = prefix + node->getString();
// 				cocos2d::Label* tail = dynamic_cast<cocos2d::Label*>(node->getUserObject());
// 				while (tail) {
// 					auto offset = (uintptr_t)(tail->getUserData());
// 					for (auto i = 0; i < offset; i++)
// 						path += ("_" + tail->getString());
// 					tail = dynamic_cast<cocos2d::Label*>(tail->getUserObject());
// 				}
// 				path += ".mp3";
				AUDIO_ENGINE->playSound(audioIt->audio_path, false);
			}));
			auto scale = node->getScale();
			actions.pushBack(CallFunc::create([=]() {
				node->setTextColor(Color4B::BLUE);
			}));
			actions.pushBack(ScaleTo::create(0.2f, scale, 1.2f*scale));
			actions.pushBack(ScaleTo::create(0.2f, scale, scale));
			actions.pushBack(CallFunc::create([=]() {
				node->setTextColor(Color4B(0xff, 0x33, 0, 255));
			}));
			node->runAction(Sequence::create(actions));

			node = dynamic_cast<cocos2d::Label*>(node->getUserObject());
		}
		audioIt++;
		count++;
	}
	scheduleOnce([=](float) {
		AUDIO_ENGINE->playSound(m_correctWord.audio_path, false, [=]() {
			AUDIO_ENGINE->playSound(m_correctWord.audio_path, false, [=]() {
				if (callback)
					callback();
			});
			for (auto it = nodes.begin(); it != nodes.end(); it++) {
				auto node = *it;
				/*auto scale1 = ScaleTo::create(0.2f, 1.2f*scale);
				auto scale2 = ScaleTo::create(0.2f, scale);*/
				auto delay = DelayTime::create(0.4f);
				auto recolor = CallFunc::create([=]() {
					node->setTextColor(Color4B(0xff, 0x33, 0, 255));
				});
				(*it)->runAction(Sequence::create(delay, recolor, nullptr));
				(*it)->setTextColor(Color4B::BLUE);
			}
			auto scale = (*nodes.begin())->getParent()->getScale();
			auto scale1 = ScaleTo::create(0.2f, 1.2f*scale);
			auto scale2 = ScaleTo::create(0.2f, scale);
			(*nodes.begin())->getParent()->runAction(Sequence::createWithTwoActions(scale1, scale2));
		});
		for (auto it = nodes.begin(); it != nodes.end(); it++) {
			auto scale = (*it)->getScaleX();
			auto node = *it;
			/*auto scale1 = ScaleTo::create(0.2f, 1.2f*scale);
			auto scale2 = ScaleTo::create(0.2f, scale);*/
			auto delay = DelayTime::create(2);
			auto recolor = CallFunc::create([=]() {
				node->setTextColor(Color4B(0xff, 0x33, 0, 255));
			});
			(*it)->runAction(Sequence::create(delay, recolor, nullptr));
			(*it)->setTextColor(Color4B::BLUE);
		}
		auto scale = (*nodes.begin())->getParent()->getScale();
		auto scale1 = EaseBounceOut::create(ScaleTo::create(1, 1.2f*scale));
		auto scale2 = EaseBounceOut::create(ScaleTo::create(1, scale));
		(*nodes.begin())->getParent()->runAction(Sequence::createWithTwoActions(scale1, scale2));
	}, count * 0.7f, "asd");
}
void CatchingThiefGame::showTapGuideGame(Vec2 position, float delay_time, std::string gaf_guide_path) {
	//    auto guide_layout_check = this->getChildByName("guide_layout");
	//    if (guide_layout_check) return;
	//    auto guide_layout = cocos2d::ui::Layout::create();
	//    guide_layout->setContentSize(screen_size);
	//    Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	//    guide_layout->setName("guide_layout");

	//    guide_layout->schedule([=](float) {
	auto hand = GAFWrapper::createGAF(gaf_guide_path);
	this->addChild(hand, INT_MAX - 1);
	hand->setPosition(position);
	hand->play(true);
	hand->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
		hand->removeFromParent();
	}), NULL));

	//    }, 5, 100, delay_time, "animated_guide");
}

void  CatchingThiefGame::inActiveMode(Vec2 position, float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame(position);
        if (CONFIG_MANAGER->isGameVMonkey()){
            AUDIO_ENGINE->playSound(m_correctLetters[m_letterIndex - 1].audio_path);
        }else{
            //AUDIO_ENGINE->playSound("sounds/catchingthief/Tap on.mp3", false, [=]() {
            //   AUDIO_ENGINE->playSound(m_correctLetters[m_letterIndex - 1].audio_path);
            //});
        }
	}, 7, 100, delay_time, "active_mode");
}
