#include "WizardCauldronGame_VM.h"
#include "TextNode_VM.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "Shake.h"
#include "ui/CocosGUI.h"
#include "HSAudioEngine.h"
#include "BackgroundNode.h"
#include "cocos2d.h"


using namespace WizardCauldron_VM;

WizardCauldronGame_VM * WizardCauldronGame_VM::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = WizardCauldronGame_VM::create();
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

bool WizardCauldronGame_VM::init()
{
	if (!MJMainGBase::init())
		return false;
	cacheTexture();
	initBackground();
	return true;
}

void WizardCauldronGame_VM::onEnter()
{
	MJMainGBase::onEnter();
	setScale(1.01f);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto move = DelayTime::create(visibleSize.width / 400);
	auto bgSprite = dynamic_cast<BackgroundNode*>(m_bgSprite);
	if (bgSprite)
		bgSprite->setMovingSpeed(-400);
	initCauldron();
	auto startGameAction = CallFunc::create([=]() {
		if (bgSprite)
			bgSprite->setMovingSpeed(0);
		//startGame();
	});
	m_bgNode->runAction(Sequence::createWithTwoActions(move, startGameAction));
    
    getGameData();
    
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/music_low.mp3", true);
	AUDIO_ENGINE->playSound(CONFIG_MANAGER->isGameVMonkey()?question_vi:"sounds/wizardcauldron/intro.mp3", false, [this] {
		startGame();
	});
}

void WizardCauldronGame_VM::onExit()
{
	MJMainGBase::onExit();
//    AUDIO_ENGINE->stopAllAudio();
}

bool WizardCauldronGame_VM::onReleaseTextNode_VM(TextNode_VM *node)
{
	bool result = false;
	AUDIO_ENGINE->stopSound("sounds/wizardcauldron/drag_on_air.mp3");

	if (m_letterIndex >= m_correctLetters.size())
		result = false;
	else if (m_correctLetters.at(m_letterIndex).text == node->getString().text) {
		result = true;
	}

	auto cauldronOffset = cocos2d::Vec2(0, 130);
	auto destinationPos = m_cauldron->getPosition() + cauldronOffset;
	auto path = node->getPosition() - destinationPos;

	result &= (path.getLength() < 400 - 175);

	if (result) {
		m_consecutiveWrong = 0;
		//AUDIO_ENGINE->playSound("sounds/wizardcauldron/correct.mp3", false);
		std::vector<std::string> sounds;
		AUDIO_ENGINE->playSound("sounds/wizardcauldron/correct_2.mp3");

		m_letterIndex++;
		auto winCondition = checkWinCondition();
		raiseClickBlockLayer();
		auto duration = path.getLength() / 800;
		auto move = MoveTo::create(duration, destinationPos);
		auto baseScale = node->getScale();
		auto scale = ScaleTo::create(duration, baseScale * 0.4f);
		auto spawn = Spawn::createWithTwoActions(move, scale);
		auto disappear = CallFunc::create([=]() {
			m_WordFrame_VM->reveal();
			playShowTextScript(node->getString(), node->getColor(), [=]() {
				if (!winCondition)
					m_cauldron->playSequence("idle", true);
				else {
					AUDIO_ENGINE->playSound("sounds/wizardcauldron/end_win.mp3", false);
					auto scaleDown = ScaleTo::create(1, 0);
					m_thinkingNode->stopAllActions();
					m_thinkingNode->runAction(scaleDown);

					auto delay = DelayTime::create(1.3f);
					auto playSequence = CallFunc::create([=]() {
						m_cauldron->playSequence("drop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
							m_cauldron->playSequence("win", true);
						});
					});
					m_cauldron->runAction(Sequence::createWithTwoActions(delay, playSequence));
				}
				if (m_guidePlayed)
					destroyClickBlockLayer();
			});
			auto shakeAction = Shake::create(1, 5, 5);
			auto delayAction = DelayTime::create(1.5f);
			auto string = node->getString();
			auto checkAction = CallFunc::create([=]() {
				if (winCondition)
					playEndingScript();
			});
			m_bgNode->runAction(Sequence::create(shakeAction, delayAction, checkAction, nullptr));
			m_cauldron->playSequence("drop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				if (!winCondition)
					m_wizard->playSequence("idle", true);
				m_cauldron->playSequence("idle", true);
			});
			node->removeFromParent();
		});

		node->runAction(Sequence::createWithTwoActions(spawn, disappear));
		m_wizard->playSequence("drop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			if (winCondition) {
				m_wizard->playSequence("drop", true);
				return;
			}
			m_wizard->playSequence("finish_spell", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
				m_wizard->playSequence("idle", true);
			});
		});
		return true;
	}
	else {
		m_consecutiveWrong++;
		if (m_consecutiveWrong >= 4) {
			//m_thinkingNode->runAction(Shake::create(1.5f, 10, 10));
			auto children = m_gameNode->getChildren();
			for (auto it = children.begin(); it != children.end(); it++) {
				auto textNode = dynamic_cast<TextNode_VM*>(*it);
				if (!textNode)
					continue;
				if (textNode->getString().text != m_correctLetters.at(m_letterIndex).text)
					continue;
				textNode->setEffectEnabled(true);
				break;
			}
			m_consecutiveWrong = 0;
		}
		AUDIO_ENGINE->playSound("sounds/wizardcauldron/wrong.mp3", false);
		m_wizard->playSequence("finish_spell", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			m_wizard->playSequence("idle", true);
		});
		node->setLocalZOrder(1);
		return false;
	}
	return false;
}

void WizardCauldronGame_VM::getGameData()
{
	using namespace rapidjson;
	auto jsonString = FileUtils::getInstance()->getStringFromFile(m_configPath);
	if (!jsonString.length())
		return;

 	Document resultDocument;
 	if (resultDocument.Parse(jsonString.c_str()).HasParseError())
 		return;

	//clear old data
	m_correctLetters.clear();
	m_answers.clear();
    co_dau = false;
    pos_dau = 0;
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && resultDocument.HasMember("audio_intro_vi")){
       auto audio_intro = resultDocument["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
    
	auto games = resultDocument["games"].GetArray();
	auto singleGameObj = games[m_gameIndex].GetObject();
	m_gameIndex = (m_gameIndex) % games.Size();
    
	number_turnplay = games.Size();
    std::vector<std::string> correctLetters;
    std::vector<std::string> correctAudios;
    
    auto correctString = singleGameObj["correctWord"].GetString();
    {
        auto pImage = math::resource::ResHelper::getImage(m_images, correctString);
        if (pImage) {
            m_correctString.text = pImage->getText("name_1");
            m_correctString.image_path = pImage->path;
            m_correctString.audio_path = pImage->getAudios("name_1").front();
        }
        else {
            m_correctString.text = correctString;
            m_correctString.image_path = std::string("games/wizardcauldron/words/") + correctString + ".png";
            m_correctString.audio_path = std::string("sounds/wizardcauldron/letters/") + correctString + "/n_" + correctString + ".mp3";
        }

        if (m_correctString.audio_path.find(".mp3") == std::string::npos) {
            m_correctString.audio_path = std::string("sounds/wizardcauldron/letters/") + m_correctString.text + "/n_" + m_correctString.text + ".mp3";
        }
        if (pImage->getComparisionValue("position_p") != -1){
            co_dau = true;
            pos_dau = pImage->getComparisionValue("position_p");
        };
        correctLetters = pImage->getTextPhonic("name_p");
        correctAudios = pImage->getAudioPhonic("name_p");
        if (co_dau){
            m_text_dau.text = correctLetters.back();
            correctLetters.pop_back();
            m_text_dau.audio_path = correctAudios.back();
            correctAudios.pop_back();
            
            m_text_khong_dau.text = correctLetters.back();
            correctLetters.pop_back();
            m_text_khong_dau.audio_path = correctAudios.back();
            correctAudios.pop_back();
            
            correctLetters.push_back(m_text_dau.text);
            correctAudios.push_back(m_text_dau.audio_path);
        }
        
    }

	for (rapidjson::SizeType i = 0; i < correctLetters.size(); i++) {
        mj::GameObject go;
        go.text = correctLetters[i];
        go.audio_path = correctAudios[i];
        go.image_path = "";
		m_correctLetters.push_back(go);
        m_answers.push_back(go);
	}
    

	auto answers = singleGameObj["answers"].GetArray();
	for (rapidjson::SizeType i = 0; i < answers.Size(); i++) {
		auto answer = answers[i].GetString();
		auto pImage = math::resource::ResHelper::getImage(m_images, answer);
		mj::GameObject go;
		if (pImage) {
			go.text = pImage->getText("name_1");
			go.image_path = pImage->path;
			go.audio_path = pImage->getAudios("name_1").front();
		}
		else {
			go.text = answer;
			go.image_path = std::string("games/wizardcauldron/words/") + answer + ".png";
			go.audio_path = std::string("sounds/wizardcauldron/letters/") + m_correctString.text + "/" + answer + ".mp3";
		}
		m_answers.push_back(go);
	}
    
	/*if (m_correctSprite)
		m_correctSprite->removeFromParent();*/
	//m_correctImage = singleGameObj["correctImage"].GetString();
	/*m_correctSprite = cocos2d::Sprite::create(std::string("games/wizardcauldron/words/") + m_correctImage);
	m_correctSprite->setVisible(false);
	m_gameNode->addChild(m_correctSprite, 10);*/
}

void WizardCauldronGame_VM::raiseClickBlockLayer()
{
	if (m_clickBlockLayer)
		return;

	m_clickBlockLayer = cocos2d::Node::create();
	m_gameNode->addChild(m_clickBlockLayer, 10);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_clickBlockLayer);
}

void WizardCauldronGame_VM::destroyClickBlockLayer()
{
	if (m_clickBlockLayer)
		m_clickBlockLayer->removeFromParent();
	m_clickBlockLayer = nullptr;
}

void WizardCauldronGame_VM::playShowTextScript(mj::GameObject text, Color4B textColor, std::function<void()> callback)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto textLabel = cocos2d::ui::Text::create(text.text,CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getGameFontName(): "fonts/arial.ttf", 200);
    if (text.text == "?"){
        textLabel->setFontName("fonts/vmonkey_punctuation_custom_font.ttf");
    }
	textLabel->setTextColor(textColor);
	textLabel->enableOutline(Color4B::WHITE, 3);
	textLabel->setPosition(visibleSize / 2);
	m_gameNode->addChild(textLabel, 10);
	textLabel->setScale(0);
	Vector<FiniteTimeAction*> actions;
	actions.pushBack(EaseOut::create(ScaleTo::create(1.1f, 2.5f), 3));
	scheduleOnce([=](float) {
		AUDIO_ENGINE->playSound(text.audio_path);
	}, 0.5f, "delay_text");
	actions.pushBack(FadeOut::create(0.2f));
	actions.pushBack(CallFunc::create([=]() {
		textLabel->removeFromParent();
		if (callback)
			callback();
	}));
	textLabel->runAction(Sequence::create(actions));
}

bool WizardCauldronGame_VM::checkWinCondition()
{
	return m_letterIndex >= m_correctLetters.size();
}

void WizardCauldronGame_VM::playEndingScript()
{
	stopAmbientSounds();
	m_cauldron->playSequence("win", true);
	m_wizard->playSequence("win", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		m_wizard->playSequence("win", true);
		m_cauldron->playSequence("drop");
	});

	raiseClickBlockLayer();

	auto marginTop = 350;
	auto marginSide = 200;
	auto marginBottom = 100;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto endingNode = cocos2d::Node::create();
	auto endingTextNode = cocos2d::Node::create();

	endingNode->setPosition(visibleSize / 2);
	m_gameNode->addChild(endingNode, 11);
	m_gameNode->addChild(endingTextNode, 12);

	auto totalWidth = 0;

	// move letters to new layer
	auto allChilden = m_WordFrame_VM->getLetterNodes();
	auto highestHeight = 0;

	endingTextNode->setPosition(visibleSize.width / 2 + 22, visibleSize.height - marginTop / 2 - 40);
	for (auto it = allChilden.begin(); it != allChilden.end(); it++) {
		auto label = *it;
		auto worldPos = label->getParent()->convertToWorldSpace(label->getPosition());
		label->retain();
		label->removeFromParentAndCleanup(false);
		label->setPosition(endingTextNode->convertToNodeSpace(worldPos));
		endingTextNode->addChild(label);
		label->release();
		if (label->getContentSize().height > highestHeight)
			highestHeight = label->getContentSize().height;
		totalWidth += label->getContentSize().width;
	}
    
	m_WordFrame_VM->clearLetterNodes();

	// reposition letters
	{
		auto basePos = cocos2d::Vec2((-totalWidth) / 2, 0);
		Vec2 offset = Vec2(0,0);
        std::vector <Vec2> new_pos;
		for (auto it = allChilden.begin(); it != allChilden.end(); it++) {
			auto label = *it;
			auto pos = basePos;
			pos += offset;
            new_pos.push_back(pos);
			auto move = MoveTo::create(0.8f, pos);
            if (co_dau && *it == allChilden[allChilden.size() - 1]){
                
            }else{
                label->runAction(move);
            }
            offset += Vec2(label->getContentSize().width,0);
		}
        if (co_dau){
            auto dau = allChilden[allChilden.size() - 1];
            dau->setAnchorPoint(Vec2(0.5,0.5));
            auto text_pos_dau = allChilden[pos_dau];
            auto pos = new_pos[pos_dau] + text_pos_dau->getContentSize()/2;
            std::vector<std::string> nguyen_am = {"a","e","y","u","o"};
            int yy = 10 ;
            for (auto am : nguyen_am){
                if (text_pos_dau->getString() == am){
                    yy = -5;
                    break;
                }
            }
            if (m_text_dau.text == "."){
                yy = - text_pos_dau->getContentSize().height*0.5 - 14;
            }
            int xx = 0;
            if (m_text_dau.text == "?") xx = 5;
            //if (m_text_dau.text == "`") xx = -5;
            pos = Vec2(pos.x+xx, pos.y + text_pos_dau->getContentSize().height/2+yy);
            dau->runAction(MoveTo::create(0.8f, pos));
        }
		endingTextNode->runAction(ScaleTo::create(0.8f, 1.8f));
	}

	// show images
	{
		auto blackLayer = LayerColor::create(Color4B::BLACK);
		blackLayer->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		blackLayer->setPosition(-visibleSize.width / 2, -visibleSize.height / 2);
		blackLayer->setOpacity(0);
		endingNode->addChild(blackLayer);

		auto whiteRect = ui::Scale9Sprite::create();
		whiteRect->setCapInsets(Rect(20, 20, 1, 1));
		whiteRect->setPreferredSize(cocos2d::Size(visibleSize.width - 2 * marginSide, visibleSize.height - marginTop - marginBottom));
		whiteRect->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		whiteRect->setPosition(endingNode->convertToNodeSpace(m_cauldron->getPosition()));
		whiteRect->setScale(0);
		endingNode->addChild(whiteRect);

		auto correctImage = Sprite::create(m_correctString.image_path);
		correctImage->setPosition(whiteRect->getContentSize() / 2);
		correctImage->setScale(whiteRect->getContentSize().height / correctImage->getContentSize().height);
		whiteRect->addChild(correctImage);
		Vector<FiniteTimeAction*> actions;

		AUDIO_ENGINE->playSound("sounds/wizardcauldron/picture_show_up.mp3", false);
		AUDIO_ENGINE->stopSound("sounds/wizardcauldron/music_low.mp3");

		blackLayer->runAction(FadeTo::create(0.8f,255));
		whiteRect->runAction(ScaleTo::create(0.8f, 1));
		actions.pushBack(JumpTo::create(0.75f, cocos2d::Vec2(0,-100), visibleSize.height * 0.5, 1));
		cocos2d::Vec2 jumpBounce(0, 120);
		for (int i = 0; i < 3; i++)
		{
			actions.pushBack(MoveBy::create(0.1f, jumpBounce));
			actions.pushBack(MoveBy::create(0.1f, -jumpBounce));
			jumpBounce.y -= 40;
		}
		actions.pushBack(DelayTime::create(0.5f));
		actions.pushBack(CallFunc::create([=]() {
            
			playSynctext(allChilden, [=]() {
                
                if(m_gameIndex >= number_turnplay)
                {
                    return;
                }
                
                if(endingNode)
                {
                    endingNode->removeFromParent();
                }
                
                if(endingTextNode)
                {
                    endingTextNode->removeFromParent();
                }

				nextGame();
			});
		}));
		whiteRect->runAction(Sequence::create(actions));
	}
}

void WizardCauldronGame_VM::initThinkingNode()
{
	auto thinkingSprite = Sprite::create("games/wizardcauldron/9.png");
	cocos2d::Vec2 offset(80, 50);
	thinkingSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
	thinkingSprite->setPosition(m_wizard->getPosition() + offset);
	thinkingSprite->setScale(0);

	auto scaleAction = ScaleTo::create(0.8f, 1);
	auto mainAction = CallFunc::create([=]() {
		auto move1 = ScaleTo::create(1, 1.05f);
		auto move2 = ScaleTo::create(1, 1);
		thinkingSprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(move1, move2)));
	});

	thinkingSprite->runAction(Sequence::createWithTwoActions(scaleAction, mainAction));
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/huzz_pop.mp3", false);
	m_thinkingNode = thinkingSprite;
	m_gameNode->addChild(thinkingSprite);

	auto thinkingImage = ui::Button::create(m_correctString.image_path);
	thinkingImage->setZoomScale(0);
	auto widthScale = 190 / thinkingImage->getContentSize().width;
	auto heightScale = 142 / thinkingImage->getContentSize().height;
	auto scale = widthScale < heightScale ? widthScale : heightScale;
	thinkingImage->setScale(scale);
	thinkingImage->setPosition(cocos2d::Vec2(144, 129));
	thinkingImage->addClickEventListener([=](Ref*) {
        playGuide();
	});
	m_thinkingNode->addChild(thinkingImage);
}

void WizardCauldronGame_VM::startGame()
{
	//m_gameNode->removeAllChildren();
	auto childen = m_gameNode->getChildren();
	for (auto it = childen.begin(); it != childen.end(); it++) {
		auto child = *it;
		if (child == m_wizard || child == m_cauldron)
			continue;
		child->removeFromParent();
	}
	m_clickBlockLayer = nullptr;
	m_correctSprite = nullptr;
	m_answersNode = nullptr;
	m_thinkingNode = nullptr;
	m_letterIndex = 0;
	//initCauldron();
    
    if(CONFIG_MANAGER->isGameVMonkey())
    {
        
        initStones();
        initWordFrame_VM();
        initThinkingNode();
        playAmbientSounds();
        
        raiseClickBlockLayer();
        this->scheduleOnce([=](float){
            AUDIO_ENGINE->play(m_correctString.audio_path, false, 1.0, [=]{
                if (!m_guidePlayed) {
                    playGuideScript();
                    return;
                }
                destroyClickBlockLayer();
            });
        }, 1.5f, "correct_word");
        
        
    }
    else
    {
        AUDIO_ENGINE->playSound("sounds/wizardcauldron/guide.mp3", false, [=] {
            initStones();
            initWordFrame_VM();
            initThinkingNode();
            playAmbientSounds();
            //        std::vector<std::string> chain;
            //        //chain.push_back("sounds/wizardcauldron/guide.mp3");
            //        chain.push_back(m_correctString.audio_path);
            raiseClickBlockLayer();
            this->scheduleOnce([=](float){
                AUDIO_ENGINE->play(m_correctString.audio_path, false, 1.0, [=]{
                    if (!m_guidePlayed) {
                        playGuideScript();
                        return;
                    }
                    destroyClickBlockLayer();
                });
            }, 1.5f, "correct_word");
            
            //        AUDIO_ENGINE->playChain(chain, [=]() {
            //
            //        });
        });
    }
	
	m_cauldron->playSequence("idle", true);
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/music_low.mp3", true);
}

void WizardCauldronGame_VM::nextGame()
{
	if (++m_gameIndex == number_turnplay) {
		MJMainGBase::onFinishGame();
	}
	else {
        getGameData();
		startGame();
	}
}

void WizardCauldronGame_VM::playAmbientSounds()
{
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/night_ambient.mp3", true);
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/fire_ambient.mp3", true, nullptr, 0.5f);
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/boiling_water.mp3", true, nullptr, 0.5f);
}

void WizardCauldronGame_VM::stopAmbientSounds()
{
	AUDIO_ENGINE->stopSound("sounds/wizardcauldron/night_ambient.mp3");
	AUDIO_ENGINE->stopSound("sounds/wizardcauldron/fire_ambient.mp3");
	AUDIO_ENGINE->stopSound("sounds/wizardcauldron/boiling_water.mp3");
}

void WizardCauldronGame_VM::playGuide()
{
	static bool guidePlaying = false;
	if (guidePlaying)
		return;
	guidePlaying = true;
	std::vector<std::string> guideSounds;

	guideSounds.push_back(m_correctString.audio_path);
	for (auto it = m_correctLetters.begin(); it != m_correctLetters.end(); it++) {
		guideSounds.push_back(it->audio_path);
	}
	AUDIO_ENGINE->playChain(guideSounds, [=]() {
		guidePlaying = false;
	});
}

void WizardCauldronGame_VM::playSynctext(std::vector<Label*> nodes, std::function<void()> callback)
{
	auto count = 0;
    Label* dau = NULL;
    auto nodes_co_dau = nodes;
    if (co_dau) {
        dau = nodes.back();
        nodes.pop_back();
    }
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
				AUDIO_ENGINE->playSound(audioIt->audio_path, false);
			}));
			
			auto scale = node->getScale();
			actions.pushBack(CallFunc::create([=]() {
				node->setTextColor(Color4B::RED);
			}));
			actions.pushBack(ScaleTo::create(0.2f, scale, 1.2f*scale));
			actions.pushBack(ScaleTo::create(0.2f, scale, scale));
			actions.pushBack(CallFunc::create([=]() {
				node->setTextColor(Color4B::WHITE);
			}));
			node->runAction(Sequence::create(actions));

			node = dynamic_cast<cocos2d::Label*>(node->getUserObject());
		}
		audioIt++;
		count++;
	}

	scheduleOnce([=](float) {
        if (co_dau){
            AUDIO_ENGINE->playSound(m_text_khong_dau.audio_path, false);
            for (auto it = nodes.begin(); it != nodes.end(); it++) {
                auto node = *it;
                // skip tail node
                if (node->getUserData())
                    continue;
                while (node) {
                    Vector<FiniteTimeAction*> actions;
                    auto scale = node->getScale();
                    actions.pushBack(CallFunc::create([=]() {
                        node->setTextColor(Color4B::RED);
                    }));
                    actions.pushBack(ScaleTo::create(0.2f, scale, 1.2f*scale));
                    actions.pushBack(ScaleTo::create(0.2f, scale, scale));
                    actions.pushBack(CallFunc::create([=]() {
                        node->setTextColor(Color4B::WHITE);
                    }));
                    node->runAction(Sequence::create(actions));
                    node = dynamic_cast<cocos2d::Label*>(node->getUserObject());
                }
            }
            scheduleOnce([=](float) {
                auto node = dau;
                // skip tail node
                AUDIO_ENGINE->playSound(m_text_dau.audio_path, false);
                while (node) {
                    Vector<FiniteTimeAction*> actions;
                    auto scale = node->getScale();
                    actions.pushBack(CallFunc::create([=]() {
                        node->setTextColor(Color4B::RED);
                    }));
                    actions.pushBack(ScaleTo::create(0.2f, scale, 1.2f*scale));
                    actions.pushBack(ScaleTo::create(0.2f, scale, scale));
                    actions.pushBack(CallFunc::create([=]() {
                        node->setTextColor(Color4B::WHITE);
                        AUDIO_ENGINE->playSound(m_correctString.audio_path, false, [=]() {
                            if (callback)
                                callback();
                        });
                        for (auto it = nodes_co_dau.begin(); it != nodes_co_dau.end(); it++) {
                            auto scale = (*it)->getScaleX();
                            auto node = *it;
                            auto delay = DelayTime::create(2);
                            auto recolor = CallFunc::create([=]() {
                                node->setTextColor(Color4B::WHITE);
                            });
                            (*it)->runAction(Sequence::create(delay, recolor, nullptr));
                            (*it)->setTextColor(Color4B::RED);
                        }
                        auto scale = (*nodes.begin())->getParent()->getScale();
                        auto scale1 = EaseBounceOut::create(ScaleTo::create(1, 1.2f*scale));
                        auto scale2 = EaseBounceOut::create(ScaleTo::create(1, scale));
                        (*nodes.begin())->getParent()->runAction(Sequence::createWithTwoActions(scale1, scale2));
                    }));
                    node->runAction(Sequence::create(actions));
                    node = dynamic_cast<cocos2d::Label*>(node->getUserObject());
                }
            }, 0.7, "asfvac");
        }
        else{
            AUDIO_ENGINE->playSound(m_correctString.audio_path, false, [=]() {
                if (callback)
                    callback();
            });
            for (auto it = nodes.begin(); it != nodes.end(); it++) {
                auto scale = (*it)->getScaleX();
                auto node = *it;
                auto delay = DelayTime::create(2);
                auto recolor = CallFunc::create([=]() {
                    node->setTextColor(Color4B::WHITE);
                });
                (*it)->runAction(Sequence::create(delay, recolor, nullptr));
                (*it)->setTextColor(Color4B::RED);
            }
            auto scale = (*nodes.begin())->getParent()->getScale();
            auto scale1 = EaseBounceOut::create(ScaleTo::create(1, 1.2f*scale));
            auto scale2 = EaseBounceOut::create(ScaleTo::create(1, scale));
            (*nodes.begin())->getParent()->runAction(Sequence::createWithTwoActions(scale1, scale2));
        }
		
	}, count * 0.7f, "asd");
}

void WizardCauldronGame_VM::playGuideScript()
{
	m_guidePlayed = true;
	//destroyClickBlockLayer();
	auto hand = GAFWrapper::createGAF("gaf/guidegame/hand_tap/hand_tap.gaf");
	m_gameNode->addChild(hand, 99);

	// select the correct stone
	auto children = m_gameNode->getChildren();
	WizardCauldron_VM::TextNode_VM* textNode = nullptr;
	for (auto it = children.begin(); it != children.end(); it++) {
		auto node = dynamic_cast<WizardCauldron_VM::TextNode_VM*>(*it);
		if (!node)
			continue;
		if (node->getString().text != (*m_correctLetters.begin()).text)
			continue;
		textNode = node;
	}
	if (!textNode)
		return;

	hand->setPosition(textNode->getPosition());

	Vector<FiniteTimeAction*> actions;
	auto pos = textNode->getPosition();
	actions.pushBack(DelayTime::create(1));
	actions.pushBack(CallFunc::create([=]() {
		textNode->runAction(MoveTo::create(1, m_cauldron->getPosition()));
	}));
	actions.pushBack(MoveTo::create(1, m_cauldron->getPosition()));
	actions.pushBack(CallFunc::create([=]() {
		hand->playSequence("swipe", true);
		//onReleaseTextNode(textNode);
	}));
	actions.pushBack(DelayTime::create(0.5f));
	actions.pushBack(CallFunc::create([=]() {
		hand->removeFromParent();
		textNode->moveToBasePosition();
		destroyClickBlockLayer();
	}));

	hand->playSequence("tap", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		hand->playSequence("drag", true);
		hand->runAction(Sequence::create(actions));
		//onTouchTextNode(textNode);
	});
}

void WizardCauldronGame_VM::initBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bgNode = cocos2d::Node::create();
	addChild(bgNode);
	m_bgNode = bgNode;

	auto gameNode = cocos2d::Node::create();
	m_bgNode->addChild(gameNode, 1);
	m_gameNode = gameNode;

	/*auto bgSprite = Sprite::create("games/wizardcauldron/bg.png");
	bgSprite->setScale(visibleSize.height / bgSprite->getContentSize().height * 1.4f);
	auto bgWidth = bgSprite->getContentSize().width * bgSprite->getScale();
	bgSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
	auto totalWidth = bgWidth;
	bgNode->addChild(bgSprite);
	auto offsetPerBg = bgWidth - 5;
	auto offset = offsetPerBg;
	while (totalWidth < visibleSize.width * 2) {
		auto cloneSprite = Sprite::create("games/wizardcauldron/bg.png");
		cloneSprite->setScale(bgSprite->getScale());
		cloneSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		cloneSprite->setPosition(cocos2d::Vec2(offset, 0));
		bgNode->addChild(cloneSprite);
		totalWidth += offsetPerBg;
		offset += offsetPerBg;
	}*/
	auto bgSprite = BackgroundNode::createBackground("games/wizardcauldron/bg_1.png", "games/wizardcauldron/bg_2.png", 0, 0.9f);
	bgSprite->setPosition(0, 85);
	bgNode->addChild(bgSprite);
	m_bgSprite = bgSprite;

	//fog 
	auto fog = Sprite::create("games/wizardcauldron/shadow_front_layer.png");
	fog->setScale(visibleSize.width / fog->getContentSize().width * 1.2f, visibleSize.height / fog->getContentSize().height* 1.2f);
	fog->setPosition(visibleSize / 2);
	//addChild(fog);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch*, Event*) -> bool {
		return true;
	};
    
    // speaker
    auto speaker = cocos2d::ui::Button::create();
    speaker->loadTextures("mjstory/new_ui/speaker.png", "mjstory/new_ui/speaker.png");
    speaker->addClickEventListener([=](cocos2d::Ref * sender)
    {
        playGuide();
    });
    speaker->setPosition(Vec2(80, visibleSize.height - 70));
    this->addChild(speaker, 10000);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bgNode);
}

void WizardCauldronGame_VM::initCauldron()
{
	auto cauldron = GAFWrapper::createGAF("gaf/wizardcauldron/cauldron.gaf");
	cauldron->playSequence("idle", true);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto offset = cocos2d::Vec2(-160, -40);
	cauldron->setPosition(offset + visibleSize / 2);
	m_gameNode->addChild(cauldron);
	m_cauldron = cauldron;

	auto wizard = GAFWrapper::createGAF("gaf/wizardcauldron/wizard.gaf");
	wizard->playSequence("idle", true);
	offset.x = 300;
	offset.y = 105;
	wizard->setPosition(offset + cauldron->getPosition());
	m_gameNode->addChild(wizard);
	m_wizard = wizard;

	m_gameNode->setPosition(-visibleSize.width, 0);
	m_gameNode->runAction(MoveTo::create(visibleSize.width / 400, cocos2d::Vec2::ZERO));
}

void WizardCauldronGame_VM::initStones()
{
	srand(time(NULL));
	std::random_shuffle(m_answers.begin(), m_answers.end());
	auto offset = Director::getInstance()->getVisibleSize().width / (m_answers.size() + 1);
	auto posX = 0;

	std::vector<TextNode_VM*> allStones;

	for (auto i = 0; i < m_answers.size(); i++) {
		auto textNode = TextNode_VM::create(m_effectAsset);
		auto basePos = cocos2d::Vec2(posX += offset, i % 2 ? 200 : 120);
		textNode->setBasePosition(basePos);
		textNode->setScale(0.7f);
		textNode->setText(m_answers[i]);
		textNode->setTouchReleaseCallback([=](TextNode_VM* node)->bool {
			return onReleaseTextNode_VM(node);
		});
		textNode->setTouchStartCallback([=](TextNode_VM* node) {
			onTouchTextNode_VM(node);
		});

		textNode->setPosition(cocos2d::Vec2(basePos.x, -200));
		allStones.push_back(textNode);

		m_gameNode->addChild(textNode);
	}

	std::random_shuffle(allStones.begin(), allStones.end());

	int count = 0;
	for (auto it = allStones.begin(); it != allStones.end(); it++) {
		auto delay = DelayTime::create(count * 0.15f);
		auto basePos = (*it)->getBasePosition();
		auto moveAction = EaseBackOut::create(cocos2d::MoveTo::create((basePos.y - (*it)->getPosition().y) / 300, basePos));
		(*it)->runAction(Sequence::createWithTwoActions(delay, moveAction));
		count++;
	}

	m_wizard->playSequence("drop", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		m_wizard->playSequence("finish_spell", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			m_wizard->playSequence("idle", true);
		});
	});
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/spell_saram.mp3", false, nullptr, 0.5);
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/diamond_show_up.mp3", false, nullptr, 0.5);
}

void WizardCauldronGame_VM::initWordFrame_VM()
{
	auto WordFrame_VM = WordFrame_VM::createWithLetters(m_correctLetters,co_dau,pos_dau);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	WordFrame_VM->setPosition(visibleSize.width / 2, visibleSize.height - 80);
	m_WordFrame_VM = WordFrame_VM;
	m_gameNode->addChild(WordFrame_VM);
}

void WizardCauldronGame_VM::onTouchTextNode_VM(WizardCauldron_VM::TextNode_VM *node)
{
	srand(time(NULL));
	m_wizard->playSequence("drag", true);
	auto randomSound = rand() % 2;
	if (randomSound)
		AUDIO_ENGINE->playSound("sounds/wizardcauldron/ahaa.mp3", false);
	else
		AUDIO_ENGINE->playSound("sounds/wizardcauldron/scream_huzzzee.mp3", false);
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/choose_drag.mp3", false);
	AUDIO_ENGINE->playSound("sounds/wizardcauldron/drag_on_air.mp3", true);
	node->setLocalZOrder(2);
}

WizardCauldronGame_VM::WizardCauldronGame_VM() :
	m_configPath("json/wizardcauldron/config.json"),
	m_bgNode(nullptr),
	m_gameNode(nullptr),
	m_cauldron(nullptr),
	m_clickBlockLayer(nullptr),
	m_wizard(nullptr),
	m_correctSprite(nullptr),
	m_answersNode(nullptr),
	m_WordFrame_VM(nullptr),
	m_bgSprite(nullptr),
	m_thinkingNode(nullptr),
	m_consecutiveWrong(0),
	m_guidePlayed(false),
	m_gameIndex(0),
	m_letterIndex(0)
{

}

void WizardCauldronGame_VM::cacheTexture()
{
	auto director = Director::getInstance();
	auto textureCache = director->getTextureCache();
	textureCache->addImage("games/wizardcauldron/bg.png");
	textureCache->addImage("games/wizardcauldron/end_bg.png");
	textureCache->addImage("games/wizardcauldron/shadow_front_layer.png");
	textureCache->addImage("games/wizardcauldron/thinking/shrimp.png");
	textureCache->addImage("games/wizardcauldron/words/shrimp.png");
	for (auto i = 3; i <= 10; i++) {
		textureCache->addImage(std::string("games/wizardcauldron/" + cocos2d::StringUtils::toString(i) + ".png"));
	}
	auto m_effectAsset = gaf::GAFAsset::create("gaf/wizardcauldron/effect.gaf");
}
