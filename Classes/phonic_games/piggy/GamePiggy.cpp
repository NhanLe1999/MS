//
//  GamePiggy.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 12/2/17.
//

#include "GamePiggy.h"
#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"

USING_NS_CC;

#define VELOCITY_X          350.0f
#define TIME_LETTER_FALL    3.0f    //second
#define BASKET_W            300.0f
#define BASKET_H            20.0f
#define GROUND_HEIGHT       229.0f

#define SE_CROWN_CHEERING   "sounds/piggy/crowd_cheering.mp3"
#define SE_END_MUSIC        "sounds/piggy/end_music.mp3"
#define SE_FALLING          "sounds/piggy/falling.mp3"
#define SE_HOORAY           "sounds/piggy/Hooray.mp3"
#define SE_JUMP             "sounds/piggy/jump.mp3"
#define SE_MOVE_THE_PIG     "sounds/piggy/move_the_pig.mp3"
#define SE_MOVE_THE_PIG_W   "sounds/piggy/move_the_pig_word.mp3"
#define SE_MOVE_THE_PIG_VI_AM   "sounds/piggy/move_the_pig_vi_am.mp3"
#define SE_MOVE_THE_PIG_VI_VAN  "sounds/piggy/move_the_pig_vi_van.mp3"
#define SE_BG               "sounds/piggy/music_bg.mp3"
#define SE_PIG              "sounds/piggy/pig_sound.mp3"
#define SE_POP_CORRECT      "sounds/piggy/pop_correct.mp3"
#define SE_WALK             "sounds/piggy/Walk.mp3"
#define SE_WRONG            "sounds/piggy/wrong.mp3"
#define SE_GAME_NAME        "sounds/piggy/gameName.mp3"

GamePiggy::GamePiggy() :
	_arrows(nullptr),
	_isTouchOnPig(false),
	_chanceOfCreateWrongLetter(40),
	_comboCollectCorrect(0),
	_tickCountUserNotPlay(.0f),
	_level(1),
	_isPause(true),
	_lbExpectedLetter(nullptr),
	_zOrder(-1),
	_numCollectCorrect(0),
	_numCollectWrong(0),
	_playSoundMove(nullptr),
	_basket(nullptr),
	_state(kIdle),
	_pig(nullptr),
	_goodJobSprite(nullptr),
	_listener(nullptr)
{

}

GamePiggy* GamePiggy::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = GamePiggy::create();
	if (result)
	{
		if (json_file.length() > 5)
		{
			result->loadGame();
		}
	}
	else
	{
		CC_SAFE_DELETE(result);
	}
	return result;
}

GamePiggy::~GamePiggy() {
	_vSprite.clear();

	_vLabel.clear();
	_playSoundMove->release();
}

void GamePiggy::getDataConfig()
{
	auto jsonData = FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document doc;
	doc.Parse(jsonData.c_str());

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");


	if (JSON_SAFE_CHECK(doc, "type_play", String))
	{
		_typePlay = doc["type_play"].GetString();
	}


	is_word = false; // phan biet choi word hay choi sound
	for (auto img : images) {
		if (img->getComparisionValue("role") == 1) is_word = true;
	}

	_level = 5;
	if (JSON_SAFE_CHECK(doc, "level", Int))
		_level = doc["level"].GetInt();


	if (JSON_SAFE_CHECK(doc, "correct", Array))
	{
		const rapidjson::Value& a = doc["correct"];
		for (rapidjson::SizeType i = 0; i < a.Size(); i++) {
			auto image_id_1 = a[i].GetString();
			auto image_full = math::resource::ResHelper::getImage(images, image_id_1);

			_correctImagesPaths.push_back(image_full->path);

			_lettersCorrect.push_back(image_full->getText("name_1"));
			_audioLetters.insert({ image_full->getText("name_1"), image_full->getAudios("name_1").front() });
			//            _lettersCorrect.push_back( a[i].GetString() );
		}
	}
	else {
		_lettersCorrect = { "a","b","c" };
	}

	if (JSON_SAFE_CHECK(doc, "wrong", Array))
	{
		const rapidjson::Value& a = doc["wrong"];
		for (rapidjson::SizeType i = 0; i < a.Size(); i++) {
			auto image_id_1 = a[i].GetString();
			auto image_full = math::resource::ResHelper::getImage(images, image_id_1);

			_wrongImagesPaths.push_back(image_full->path);

			_lettersWrong.push_back(image_full->getText("name_1"));

			_audioLetters.insert({ image_full->getText("name_1"), image_full->getAudios("name_1").front() });

		}
	}
	else {
		_lettersWrong = { "d","e","f","g","h","i","k","l","m","n","o","p","q",
			"r","s","t","u","v","w","x","y","z" };
	}
	if (JSON_SAFE_CHECK(doc, "option_vm_phonic", String))
		hoc_am_hoc_van = doc["option_vm_phonic"].GetString();
}

void GamePiggy::loadGame()
{
	auto overlay = ui::Layout::create();
	overlay->setContentSize(Director::getInstance()->getVisibleSize());
	overlay->setTouchEnabled(true);
	overlay->setEnabled(true);
	Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX);
	_sizeScreen = Director::getInstance()->getWinSize();
	getDataConfig();

	MJMainGBase::playBackgroundMusic(SE_BG);
	createActionSoundMove();

	if (CONFIG_MANAGER->isGameVMonkey()) {
		font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
	}
	else {
		font_name = "fonts/monkeystories_ultra.ttf";
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	_center = origin + Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto bg = ui::ImageView::create("games/piggy/bg.jpg");
	if (bg->getContentSize().width < visibleSize.width)
	{
		bg->setScaleX(visibleSize.width / bg->getContentSize().width);
	}
	bg->setPosition(_center);
	addChild(bg);

	//create PIG
	auto asset = gaf::GAFAsset::create("gaf/piggy/Piggy/Piggy.gaf");
	_pig = asset->createObjectAndRun(false);
	_pig->setPosition(-150, _center.y - 155);
	_pig->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_pig->playSequence("idle", true);
	addChild(_pig, 1);

	//create BASKET
	_basket = Sprite::create("games/piggy/basket.png");
	_basket->setPosition(_pig->getContentSize().width / 2,
		_pig->getContentSize().height - 108);
	_pig->addChild(_basket);

	_posLeft = Vec2(origin.x, _pig->getPositionY());
	_posRight = Vec2(origin.x + visibleSize.width, _pig->getPositionY());

	//create Good Job
	asset = gaf::GAFAsset::create("gaf/piggy/goodjob/goodjob.gaf");
	_goodJobSprite = asset->createObjectAndRun(true);
	_goodJobSprite->setPosition(_center.x, origin.y + visibleSize.height - 100);
	_goodJobSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_goodJobSprite->setVisible(false);
	addChild(_goodJobSprite, 1);
	_textLearn = _lettersCorrect.at(0);

	_lbExpectedImage = _correctImagesPaths.at(0);

	_lbExpectedImage = _correctImagesPaths.at(m_letter_index);



	_imgSprite = cocos2d::Sprite::create(_lbExpectedImage);
	_imgSprite->setPosition(_PositionLetter);
	_imgSprite->setVisible(true);
	_imgSprite->setOpacity(0);
	_imgSprite->setTag(1);
	addChild(_imgSprite);


	_lbExpectedLetter = Letter::createWithTTF(_textLearn, font_name, 60);
	

	_PositionLetter = origin + Vec2(visibleSize.width / 2, visibleSize.height - 180);
	_lbExpectedLetter->setPosition(_PositionLetter);
	_lbExpectedLetter->setTag(1);
	_lbExpectedLetter->setTextColor(Color4B::BLACK);
	_lbExpectedLetter->enableOutline(Color4B::WHITE, 2);
	_lbExpectedLetter->setOpacity(0);

	addChild(_lbExpectedLetter);

	if (_typePlay == "image")
	{
		_imgSprite->setPosition(_PositionLetter);
		_imgSprite->setVisible(true);
		_lbExpectedLetter->setVisible(false);
	}
	else {
		_imgSprite->setVisible(false);
		_lbExpectedLetter->setVisible(true);
	}

	_arrows = Node::create();
	addChild(_arrows, 99);
	_arrows->setCascadeOpacityEnabled(true);
	_arrows->setOpacity(0);
	auto rightArrow = Sprite::create("games/piggy/right.png");
	rightArrow->setPosition(_center + Vec2(240, -140));
	_arrows->addChild(rightArrow);

	auto leftArrow = Sprite::create("games/piggy/right.png");
	leftArrow->setPosition(_center + Vec2(-240, -140));
	leftArrow->setFlippedX(true);
	_arrows->addChild(leftArrow);

	this->animateMoveToCenter([=] {
		this->runAction(Sequence::create(
			DelayTime::create(0.5f),
			CallFunc::create([=] {
				overlay->removeFromParent(); 
				}),
			NULL
		));
		});
	_isPlaySound = true;
	addSpeaker();
	if (_lbExpectedLetter)
	{
		resetSizeLetter(_lbExpectedLetter);
	}
}


bool GamePiggy::init() {
	if (!MJMainGBase::init()) {
		return false;
	}

	return true;
}

void GamePiggy::onEnter()
{
	MJMainGBase::onEnter();
}

void GamePiggy::onExit()
{
	AUDIO_ENGINE->stopBackgroundMusic();
    MJMainGBase::onExit();
    unscheduleUpdate();
	
}

void GamePiggy::onClickBack() {
	MJMainGBase::onCloseGame(ms::delegate::Activity::EndReason::INTERRUPTED);
}

void GamePiggy::gameUpdate(float dt) {
	_tickCountUserNotPlay += dt;
	if (_tickCountUserNotPlay >= 10.0f) {
		playAudioGetTheSound(.0f);
	}

	//check if letters intersect with pig's basket
	auto p = _pig->convertToWorldSpace(_basket->getPosition());

	if (_typePlay == "image" || _typePlay == "imageAndText")
	{
		auto it = _vSprite.begin();
		while (it != _vSprite.end())
		{
			if (isBasketIntersectSprite(*it))
			{
				if ((*it)->getTag() == 1)
				{
					_numCollectCorrect++;
					_comboCollectCorrect++;
					_numCollectWrong = 0;
					if (_comboCollectCorrect == 3)
						_chanceOfCreateWrongLetter = 40;

					AUDIO_ENGINE->playEffect(SE_POP_CORRECT);
					std::string soundPath;
					auto audio_it = _audioLetters.find(_lbExpectedLetter->getString());
					if (audio_it != _audioLetters.end()) {
						soundPath = audio_it->second;
					}
					else {
						soundPath = "sample/piggy/" + _lbExpectedLetter->getString() + ".mp3";
					}

					AUDIO_ENGINE->play(soundPath.c_str());

					auto cSize = (*it)->getContentSize();
					float offset = 0.2f * cSize.width;
					auto _rightPoint = Vec2(cSize.width - offset, cSize.height / 2 - 60);
					auto _leftPoint = Vec2(offset, cSize.height / 2 - 60);

					float deltaX = .0f;
					if ((*it)->getPositionX() <= _pig->getPositionX()) {
						if (_pig->getPositionX() - (*it)->convertToWorldSpace(_leftPoint).x > 90) {
							deltaX = _pig->getPositionX() - (*it)->convertToWorldSpace(_leftPoint).x - 90;
							deltaX += random(.0f, 15.0f);
						}
					}
					else {
						if ((*it)->convertToWorldSpace(_rightPoint).x - _pig->getPositionX() > 90) {
							deltaX = -((*it)->convertToWorldSpace(_rightPoint).x - _pig->getPositionX() - 90);
							deltaX -= random(.0f, 15.0f);
						}
					}

					auto posOfBasket = Vec2(_pig->getContentSize().width / 2,
						_pig->getContentSize().height - 40);
					auto oldWorPos = (*it)->convertToWorldSpaceAR(Vec2(0, 0));
					auto newLocPos = _pig->convertToNodeSpaceAR(oldWorPos);

					(*it)->removeFromParentAndCleanup(false);
					(*it)->setPosition(Vec2(newLocPos.x + posOfBasket.x, posOfBasket.y));
					_pig->addChild((*it), _zOrder--);

					(*it)->stopAllActions();
					(*it)->runAction(JumpBy::create(.3f, Vec2(deltaX, 0), 30, 1));

					checkGameEnd();

				}
				else {
					_comboCollectCorrect = 0;
					_numCollectWrong++;
					if (_numCollectWrong >= 4)
						_chanceOfCreateWrongLetter = 15;
					AUDIO_ENGINE->playEffect(SE_WRONG);
					
					(*it)->stopAllActions();
					int direction = rand() % 2 == 0 ? 1 : -1;
					(*it)->runAction(Sequence::create(JumpBy::create(0.8f, Vec2(250 * direction, -400), 280, 1),
						RemoveSelf::create(), NULL));
				}
				it = _vSprite.erase(it);
			}
			else ++it;

		}
	}
		auto it = _vLabel.begin();
		while (it != _vLabel.end()) {
			if (isBasketIntersectLetter(*it)) {

			//TODO: check if letter is correct
			if ((*it)->getString().compare(_lbExpectedLetter->getString()) == 0) {
				_numCollectCorrect++;
				_comboCollectCorrect++;
				_numCollectWrong = 0;
				if (_comboCollectCorrect == 3)
					_chanceOfCreateWrongLetter = 40;
				AUDIO_ENGINE->playEffect(SE_POP_CORRECT);
				std::string soundPath;
				auto audio_it = _audioLetters.find(_lbExpectedLetter->getString());
				if (audio_it != _audioLetters.end()) {
					soundPath = audio_it->second;
				}
				else {
					soundPath = "sample/piggy/" + _lbExpectedLetter->getString() + ".mp3";
				}

				AUDIO_ENGINE->play(soundPath.c_str());

				(*it)->addToBasket(_pig, _zOrder--);
				checkGameEnd();

			}
			else {
				_comboCollectCorrect = 0;
				_numCollectWrong++;
				if (_numCollectWrong >= 4)
					_chanceOfCreateWrongLetter = 15;
				AUDIO_ENGINE->playEffect(SE_WRONG);
				(*it)->jumpOutBasket();
			}

			it = _vLabel.erase(it);
		}
		else ++it;
	}

}


void GamePiggy::registerTouchListener() {
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(GamePiggy::onTouchesBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(GamePiggy::onTouchMove, this);
	_listener->onTouchEnded = CC_CALLBACK_2(GamePiggy::onTouchEnded, this);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto layer_touch = cocos2d::Layer::create();
	layer_touch->setTouchEnabled(true);
	layer_touch->setContentSize(visibleSize);
	addChild(layer_touch);
	layer_touch->setName("piggy_layer_touch");
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, layer_touch);

}

void GamePiggy::checkGameEnd() {
	this->unschedule("delay_suggest");
	if (_numCollectCorrect == (m_letter_index + 1) * _level) {
		++m_letter_index;
		if (m_letter_index >= _lettersCorrect.size() || m_letter_index > 3) {
			hideAllLetters();
			unschedule(CC_SCHEDULE_SELECTOR(GamePiggy::gameUpdate));
			hideAllSprites();
			unschedule(CC_SCHEDULE_SELECTOR(GamePiggy::addFallingRandom));

			this->getEventDispatcher()->removeEventListenersForTarget(this);
			endGane();
		}
		else {
			this->unschedule("delay_setPosition_letter");
			if (_typePlay == "image" && _imgSprite)
			{
				_imgSprite->stopAllActions();
				_imgSprite->removeFromParent();
			}
			if (_lbExpectedLetter)
			{
				_lbExpectedLetter->stopAllActions();
				_lbExpectedLetter->removeFromParent();
			}
			_lbExpectedLetter = Letter::createWithTTF(_textLearn, font_name, 60);
			
			_lbExpectedLetter->setString(_lettersCorrect.at(m_letter_index));
			_lbExpectedImage = _correctImagesPaths.at(m_letter_index);
			_lbExpectedLetter->setTag(1);
			this->addChild(_lbExpectedLetter);
			_textLearn = _lettersCorrect.at(m_letter_index);

			if (_typePlay == "image")
			{
				_imgSprite = cocos2d::Sprite::create(_lbExpectedImage);
				_imgSprite->setPosition(_PositionLetter);
				_imgSprite->setVisible(false);
				_imgSprite->setTag(1);
				this->addChild(_imgSprite);
			}
			else {
				_lbExpectedLetter->setPosition(_PositionLetter);
				_lbExpectedLetter->setScale(1.0);
				_lbExpectedLetter->setVisible(false);
				resetSizeLetter(_lbExpectedLetter);
			}
			this->runAction(Sequence::create(
				DelayTime::create(1.5f),
				CallFunc::create([this] {
					if (_typePlay == "image")
					{
						_imgSprite->setVisible(true);
					}
					else {
						_lbExpectedLetter->setVisible(true);
					}
					}),
				NULL
			));
			
			hideAllLetters();
			hideAllSprites();
			playAudioGetTheSound(2.0f);
			
			
		}
	}
}

void GamePiggy::endGane() {
	auto screen_size = Director::getInstance()->getVisibleSize();
	Layout* overlay = Layout::create();
	overlay->setContentSize(Size(screen_size.width, screen_size.height));
	overlay->setAnchorPoint(Vec2(0.5, 0.5));
	overlay->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
	overlay->setName("overlay");
	this->addChild(overlay, INT_MAX - 1);
	overlay->setTouchEnabled(true);
	overlay->addClickEventListener([=](cocos2d::Ref* sender) {
		CCLOG("ok");
		});

	//pig move to right side
	AUDIO_ENGINE->playEffect(SE_END_MUSIC);
	if (_typePlay == "image")
	{
		_imgSprite->runAction(FadeOut::create(0.5));
	}
	else {
		_lbExpectedLetter->runAction(FadeOut::create(0.5));
	}
	
	_state = kWin;
	animateMoveOut();

}

void GamePiggy::createActionSoundMove() {
	_playSoundMove = RepeatForever::create(Sequence::create(
		CallFunc::create([=] {
			AUDIO_ENGINE->play(SE_WALK);
			}),
		DelayTime::create(1.0f), 
		NULL));
	_playSoundMove->retain();
}

bool GamePiggy::isBasketIntersectLetter(Letter *letter) {
	auto p = _pig->convertToWorldSpace(_basket->getPosition());
	auto rect = Rect(p.x - BASKET_W / 2, p.y - BASKET_H / 2, BASKET_W, BASKET_H);
	if (letter->isLetterIntersectRect(rect))
		return true;
	else
		return false;
}

void GamePiggy::addFallingLetter(float dt) {
	std::string nextLetter = _lbExpectedLetter->getString();
	if (random(0, 100) <= _chanceOfCreateWrongLetter) {
		nextLetter.clear();
		nextLetter.append(_lettersWrong.at(random(0, (int)_lettersWrong.size() - 1)));
	}
	auto font_sz = 60;

	auto lb = Letter::createWithTTF(nextLetter, font_name, font_sz);
	auto cc = lb->getBoundingBox().size;
	auto cc1 = _pig->getBoundingBox().size;
	while (lb->getBoundingBox().size.width > _pig->getBoundingBox().size.width - 80)
	{
		lb->setScale(lb->getScale() - 0.05);
	}
	auto cc2 = lb->getBoundingBox().size;

	auto label_size = lb->getContentSize();
	lb->setPosition(random(_posLeft.x + label_size.width, _posRight.x - label_size.width), 768.0f);

	auto f = CallFunc::create([this, lb] {
		_vLabel.eraseObject(lb);
		});
	lb->runAction(Sequence::create(MoveBy::create(TIME_LETTER_FALL, Vec2(0, -800)),
		f,
		RemoveSelf::create(), NULL));

	addChild(lb);
	_vLabel.pushBack(lb);

	AUDIO_ENGINE->playSound(SE_FALLING, false, nullptr, 0.7);
}

void GamePiggy::hideAllLetters() {
	auto childs = _pig->getChildren();
	for (auto & child : childs) {
		if (Letter * l = dynamic_cast<Letter*>(child)) {
			l->runAction(FadeOut::create(.8f));
		}
	}

	for (auto & lb : _vLabel) {
		lb->removeFromParent();
	}
}

void GamePiggy::playAudioGetTheSound(float dt) {
	//hideAllLetters();
	_countHideSuggetGame++;
	animateIdle();
	_tickCountUserNotPlay = .0f;
	_isPause = true;

	auto command = CallFunc::create([=] {
	unschedule(CC_SCHEDULE_SELECTOR(GamePiggy::addFallingRandom));


	this->getEventDispatcher()->pauseEventListenersForTarget(this);

		if (!CONFIG_MANAGER->isGameVMonkey()) {
			auto jsonData = FileUtils::getInstance()->getStringFromFile("config.json");
			rapidjson::Document doc;
			doc.Parse(jsonData.c_str());
			if (JSON_SAFE_CHECK(doc, "custom_intro_audio", String))
			{
				auto intro_icon_id = doc["custom_intro_audio"].GetString();

				if (strcmp(intro_icon_id, "null")) {
					std::vector<math::resource::Image*> icons = math::resource::ResHelper::getImages("icons.json");

					auto image_full = math::resource::ResHelper::getImage(icons, intro_icon_id);

					_audioIntro = image_full->getAudios().front();

				}
				else
				{
					_audioIntro = is_word ? SE_MOVE_THE_PIG_W : SE_MOVE_THE_PIG;
				}
			}
			else
			{
				_audioIntro = is_word ? SE_MOVE_THE_PIG_W : SE_MOVE_THE_PIG;
			}
			if (_index == 0)
			{
				AUDIO_ENGINE->play(_audioIntro);
			}
			_index++;
		}
		else {
			hoc_am_hoc_van == "hoc_am" ? AUDIO_ENGINE->play(SE_MOVE_THE_PIG_VI_AM) : AUDIO_ENGINE->play(SE_MOVE_THE_PIG_VI_VAN);
		}
		});

    auto delay_time = CONFIG_MANAGER->isGameVMonkey()?4.0f:2.5f;

	auto pronounce = CallFunc::create([=] {
		std::string soundPath;
		auto audio_it = _audioLetters.find(_lbExpectedLetter->getString());
		if (audio_it != _audioLetters.end()) {
			soundPath = audio_it->second;
		}
		else {
			soundPath = "sample/piggy/" + _lbExpectedLetter->getString() + ".mp3";
		}
		AUDIO_ENGINE->play(soundPath.c_str());

		if (_typePlay == "image")
		{
			_imgSprite->setOpacity(255);
		}
		else {
			_lbExpectedLetter->setOpacity(255);
		}

		_arrows->setOpacity(255);
		if (_countHideSuggetGame == 1)
		{
			this->unschedule("delay_suggest");
			handSuggetionGame();
		}
	});

	auto resume = CallFunc::create([=] {
		this->getEventDispatcher()->resumeEventListenersForTarget(this);
		if (!isScheduled(CC_SCHEDULE_SELECTOR(GamePiggy::gameUpdate)))
			schedule(CC_SCHEDULE_SELECTOR(GamePiggy::gameUpdate));
        this->registerTouchListener();
		});

	if (_typePlay == "image")
	{
		_imgSprite->runAction(Sequence::create(command, DelayTime::create(delay_time), pronounce, DelayTime::create(0.75f), resume, CallFunc::create([=] {

			}), NULL));
	}
	else {
		_lbExpectedLetter->runAction(Sequence::create(command, DelayTime::create(delay_time), pronounce, DelayTime::create(0.75f), resume, CallFunc::create([=] {

			}), NULL));
	}

	
}

bool GamePiggy::onTouchesBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	if (auto handGaf = utils::findChild(this, "name_sugget_game"))
	{
		handGaf->removeFromParent();
	}
	_pig->stopAllActions();
	this->unschedule("delay_suggest");
	_isSuggetionGame = false;
	_tickCountUserNotPlay = .0f;

	if (_isPause) {
		_lbExpectedLetter->stopAllActions();
		_textLearn = _lbExpectedLetter->getString();
		_lbExpectedLetter->setVisible(true);

		if (_typePlay == "image")
		{
			_imgSprite->runAction(Sequence::create(
				CallFunc::create([=] {
					AUDIO_ENGINE->stopAllAudio();
					}),
				DelayTime::create(0.5),

						CallFunc::create([=] {
						runActionLetter();
							}),
						CallFunc::create([=] {
								std::string soundPath;
								auto audio_it = _audioLetters.find(_lbExpectedLetter->getString());
								if (audio_it != _audioLetters.end()) {
									soundPath = audio_it->second;
								}
								else {
									soundPath = "sample/piggy/" + _lbExpectedLetter->getString() + ".mp3";
								}
								AUDIO_ENGINE->play(soundPath.c_str());
							}),
								NULL
								));
		}
		else {
			_lbExpectedLetter->runAction(Sequence::create(
				CallFunc::create([=] {
					AUDIO_ENGINE->stopAllAudio();
					}),
				DelayTime::create(0.5),

						CallFunc::create([=] {
						runActionLetter();
							}),
						CallFunc::create([=] {
								std::string soundPath;
								auto audio_it = _audioLetters.find(_lbExpectedLetter->getString());
								if (audio_it != _audioLetters.end()) {
									soundPath = audio_it->second;
								}
								else {
									soundPath = "sample/piggy/" + _lbExpectedLetter->getString() + ".mp3";
								}
								AUDIO_ENGINE->play(soundPath.c_str());
							}),
								NULL
								));
		}

		_arrows->runAction(Sequence::create(FadeOut::create(.5f), Hide::create(), NULL));

		schedule(CC_SCHEDULE_SELECTOR(GamePiggy::addFallingRandom), 1.3f);

		_isPause = false;
	}
	if (_pig->getPosition().distance(touch->getLocation()) <= 130)
	{
		stopAction(_playSoundMove);
		runAction(_playSoundMove);
		_isTouchOnPig = true;
		_state = kRunLeft;
		_pig->playSequence("run", true);
	}
	else {
		//move left
		if (touch->getLocation().x < _pig->getPositionX()) {
			animateMoveLeft(touch->getLocation().x);
		}
		else {
			animateMoveRight(touch->getLocation().x);
		}
	}


	return true;
}

void GamePiggy::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event) {
	_tickCountUserNotPlay = .0f;

	if (_isTouchOnPig && _state != kWin) {
		if (touch->getDelta().x < 0)
			_pig->setFlippedX(true);
		else
			_pig->setFlippedX(false);
		auto pos = _pig->getParent()->convertToNodeSpace(touch->getLocation());
		pos = checkPositionPig(pos);
        _pig->setPositionX(pos.x);
	}
}

void GamePiggy::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
	this->unschedule("handSuggetionGame");
	handSuggetionGame();
	animateIdle();
	_tickCountUserNotPlay = .0f;
	_isTouchOnPig = false;
}

void GamePiggy::animateMoveLeft(const float & posX, float time) {
	if (_state != kRunLeft && _state != kWin) {
		//play sound move
		stopAction(_playSoundMove);
		runAction(_playSoundMove);

		_pig->setFlippedX(true);
		_pig->playSequence("run", true);

		Vec2 des(posX, GROUND_HEIGHT);
		auto distance = des.distance(_pig->getPosition());
		float timeMove = distance / VELOCITY_X;

		if (time != -1)
		{
			timeMove = time;
		}

		if (timeMove > 0) {
			des = checkPositionPig(des);
			auto actionMove = MoveTo::create(timeMove, des);
			actionMove->setTag(kRunLeft);
			_pig->stopActionByTag(kRunLeft);
			_pig->stopActionByTag(kRunRight);
			_pig->runAction(actionMove);
		}

		_state = kRunLeft;
	}
}

void GamePiggy::animateMoveRight(const float & posX, float time) {
	if (_state != kRunRight && _state != kWin) {
		stopAction(_playSoundMove);
		runAction(_playSoundMove);

		_pig->setFlippedX(false);
		_pig->playSequence("run", true);
		 
		Vec2 des(posX, GROUND_HEIGHT);
		auto distance = des.distance(_pig->getPosition());
		float timeMove = distance / VELOCITY_X;

		if (time != -1)
		{
			timeMove = time;
		}

		if (timeMove > 0) {
			des = checkPositionPig(des);
			auto actionMove = MoveTo::create(timeMove, des);
			actionMove->setTag(kRunRight);
			_pig->stopActionByTag(kRunRight);
			_pig->stopActionByTag(kRunLeft);
			_pig->runAction(actionMove);
		}

		_state = kRunRight;
	}
}

void GamePiggy::animateMoveToCenter(std::function<void()> callback) {
	playAudioGetTheSound(2.0f);
	if (_state != kRunRight) {
		stopAction(_playSoundMove);
		runAction(_playSoundMove);

		_pig->setFlippedX(false);
		_pig->playSequence("run", true);

		auto center = Vec2(_center.x, _pig->getPositionY());
		float timeMove = 3.0f;
		auto moveDone = CallFunc::create([=] {
			AUDIO_ENGINE->playEffect(SE_PIG);
			this->animateIdle();
			this->registerTouchListener();
			if (callback)
			{
				callback();
			}
			});
		auto actionMove = MoveTo::create(timeMove, center);
		actionMove->setTag(kRunRight);
		_pig->stopActionByTag(kRunRight);
		_pig->runAction(Sequence::create(actionMove, moveDone, NULL));

		_state = kRunRight;
	}
}

void GamePiggy::animateMoveOut() {
	//play sound move
	stopAction(_playSoundMove);
	runAction(_playSoundMove);

	_pig->setFlippedX(false);
	_pig->playSequence("run", true);

	auto des = Vec2(_center.x * 2 + 400, _pig->getPositionY());
	auto distance = des.distance(_pig->getPosition());
	float timeMove = distance / 400.0f;
	if (timeMove > 0) {
		auto moveDone = CallFunc::create([this] {
            AUDIO_ENGINE->stopAllAudio();
			AUDIO_ENGINE->playEffect(SE_HOORAY);
			AUDIO_ENGINE->playEffect(SE_CROWN_CHEERING);
			//this->animateIdle();
			this->animateWin();
			stopAction(_playSoundMove);
		});
		auto actionMove = MoveTo::create(timeMove, des);
		actionMove->setTag(kRunRight);
		_pig->stopActionByTag(kRunRight);
		_pig->runAction(Sequence::create(actionMove, moveDone, NULL));
	}

}

void GamePiggy::animateIdle() {
	if (_state != kIdle && _state != kWin) {
		//stop sound move
		stopAction(_playSoundMove);

		_pig->playSequence("idle", true);
		_pig->stopActionByTag(kRunLeft);
		_pig->stopActionByTag(kRunRight);
		_state = kIdle;
	}
}

void GamePiggy::animateWin() {
	_pig->playSequence("win", true);
	_state = kWin;

	auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(darkLayer);

	_goodJobSprite->setPositionY(_goodJobSprite->getPositionY() + 300);
	_goodJobSprite->setVisible(!CONFIG_MANAGER->isGameVMonkey());
	_goodJobSprite->runAction(EaseInOut::create(MoveBy::create(.5f, Vec2(0, -300)), 3.0f));

	for (auto child : _pig->getChildren()) {
		if (Letter* lt = dynamic_cast<Letter*>(child))
			lt->setVisible(false);
	}
	_pig->setScale(2.3f);
	_pig->setPosition(_center.x, -300);
	auto moveIn = EaseInOut::create(MoveBy::create(.5f, Vec2(0, 600)), 3.0f);
	auto moveOut = EaseInOut::create(MoveBy::create(.5f, Vec2(0, -600)), 3.0f);
	auto playSound = CallFunc::create([] {
		AUDIO_ENGINE->playEffect(SE_JUMP);
	});
	_pig->runAction(Sequence::create(moveIn, DelayTime::create(2.0f), playSound, moveOut, CallFunc::create([=] {
		MJMainGBase::onCloseGame();
	}), NULL));
}


bool GamePiggy::isBasketIntersectSprite(Sprite* sprite) {
	auto p = _pig->convertToWorldSpace(_basket->getPosition());
	auto rect = Rect(p.x - BASKET_W / 2, p.y - BASKET_H / 2, BASKET_W, BASKET_H);

	if (sprite->getBoundingBox().intersectsRect(rect))
		return true;
	else
		return false;
}

void GamePiggy::addFallingRandom(float dt) {

	if (_typePlay == "image")
	{
		GamePiggy::addFallingSprite(dt);
	}
	else if (_typePlay == "imageAndText")
	{
		(random<int>(0, 1)) ? GamePiggy::addFallingSprite(dt) : GamePiggy::addFallingLetter(dt);
	}
	else
	{
		GamePiggy::addFallingLetter(dt);
	}
}

void GamePiggy::addFallingSprite(float dt) {
	Sprite* nextSprite = Sprite::create(_lbExpectedImage);
	nextSprite->setTag(1);
	if (random(0, 100) <= _chanceOfCreateWrongLetter) {
		if ((int)_wrongImagesPaths.size())
		{
			int index = random(0, (int)_wrongImagesPaths.size() - 1);
			nextSprite = Sprite::create(_wrongImagesPaths.at(index));
			nextSprite->setTag(0);
		}
	}

	auto nextSpriteSize = nextSprite->getContentSize();
	nextSprite->setPosition(random(_posLeft.x + nextSpriteSize.width, _posRight.x - nextSpriteSize.width), 768.0f);

	auto f = CallFunc::create([this, nextSprite] {
		_vSprite.eraseObject(nextSprite);
		});
	nextSprite->runAction(Sequence::create(MoveBy::create(TIME_LETTER_FALL, Vec2(0, -800)),
		f,
		RemoveSelf::create(), NULL));
	addChild(nextSprite);
	_vSprite.pushBack(nextSprite);

	AUDIO_ENGINE->playSound(SE_FALLING, false, nullptr, 0.7);
}

void GamePiggy::hideAllSprites() {
	auto childs = _pig->getChildren();
	for (auto& child : childs) {
		if (Sprite* l = dynamic_cast<Sprite*>(child)) {
			l->runAction(FadeOut::create(.8f));
		}
	}

	for (auto& lb : _vSprite) {	
		lb->removeFromParent();
	}
}

void GamePiggy::handSuggetionGame()
{
	this->schedule([this](float) {
		if (_isRunSuggetionGame)
		{
			_isSuggetionGame = true;
			runActionPiggySuggetGame();
		}
		else
		{
			_isRunSuggetionGame = true;
		}
		}, 8, 100, 5, "delay_suggest");
}

void GamePiggy::runActionPiggySuggetGame()
{
	if (auto handGaf = utils::findChild(this, "name_sugget_game"))
	{
		handGaf->removeFromParent();
	}
	this->unschedule("delay_suggest");
	if (_isSuggetionGame)
	{
		auto positionPiggy = _pig->getPosition();

		float xMove = positionPiggy.x;
		this->runAction(Sequence::create(
			CallFunc::create([=] {
				if (_isSuggetionGame)
				{
					std::string pathHand = "mjstory/new_ui/hand.png";
					auto hand = ui::ImageView::create(pathHand);
					hand->setName("name_sugget_game");
					hand->setAnchorPoint(Point(0.5, 0.5));
					hand->setVisible(true);
					hand->setPosition(Vec2(_pig->getBoundingBox().size.width / 2, _pig->getBoundingBox().size.height / 3));
					if (hand)
					{
						_pig->addChild(hand, 1000);
					}
					_arrows->runAction(Sequence::create(FadeOut::create(.5f), Hide::create(), NULL));
					animateMoveLeft(xMove - 100, 1.5f);
					auto postY = _pig->getPositionY() + _pig->getBoundingBox().size.height / 2;
					_positonXLetter = postY - 30;
					auto postX = _lbExpectedLetter->getPositionX();
					runActionLetter();
				}
			}),
			DelayTime::create(1.5f),
			CallFunc::create([=] {
				if (_isSuggetionGame)
				{
					animateIdle();
					animateMoveRight(xMove + 200, 3.0f);
				}
			}),
			DelayTime::create(3.0f),
			CallFunc::create([=] {
				if (_isSuggetionGame)
				{
					animateIdle();
					animateMoveLeft(xMove, 1.5f);
				}
			}),
			DelayTime::create(2.0f),
			CallFunc::create([=] {
				if (auto handGaf = utils::findChild(this, "name_sugget_game"))
				{
					handGaf->removeFromParent();
				}
				animateIdle();
				handSuggetionGame();
			}),
			NULL));
	}
}

void GamePiggy::addSpeaker()
{
	auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	auto screenSize = cocos2d::Director::getInstance()->getWinSize();
	this->addChild(speaker, 10000000);
	speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	speaker->setPosition(Vec2(50, Director::getInstance()->getWinSize().height - 20));
	speaker->addClickEventListener([=](Ref* sender) {
		if (_isPlaySound)
		{
			_isPlaySound = false;
			AUDIO_ENGINE->playSound(_audioIntro, false, [this]() {
				_isPlaySound = true;
				});
		}
	});
}

void GamePiggy::setPositionLetter()
{
	this->scheduleOnce([=](float)
		{
			if (_typePlay == "image")
			{
				if (_textLearn == _lbExpectedLetter->getString())
				{
					_imgSprite->setPosition(Vec2(_pig->getPositionX(), _positonXLetter));
					setPositionLetter();
				}
			}
			else {
				if (_lbExpectedLetter->isVisible() && _textLearn == _lbExpectedLetter->getString())
				{
					_lbExpectedLetter->setPosition(Vec2(_pig->getPositionX(), _positonXLetter));
					setPositionLetter();
				}
			}
			
		}, 0.0f, "delay_setPosition_letter");
}

void GamePiggy::runActionLetter()
{
	if (_typePlay == "image")
	{
		auto f = CallFunc::create([this] {
			_vSprite.eraseObject(_imgSprite);
			});
		_imgSprite->runAction(Sequence::create(MoveBy::create(TIME_LETTER_FALL, Vec2(0, -800)),
			f, NULL));
		_imgSprite->setName("No_delete");
		_vSprite.pushBack(_imgSprite);
	}
	else {
		auto f = CallFunc::create([this] {
			_vLabel.eraseObject(_lbExpectedLetter);
			});
		_lbExpectedLetter->runAction(Sequence::create(MoveBy::create(TIME_LETTER_FALL, Vec2(0, -800)),
			f, NULL));
		_lbExpectedLetter->setName("No_delete");
		_vLabel.pushBack(_lbExpectedLetter);
	}
}

void GamePiggy::runActionNode(cocos2d::Node* node)
{

}

void GamePiggy::redAudioLetter()
{
	auto audio_it = _audioLetters.find(_lbExpectedLetter->getString());
	std::string soundPath = "";
	if (audio_it != _audioLetters.end()) {
		soundPath = audio_it->second;
	}
	else {
		soundPath = "sample/piggy/" + _lbExpectedLetter->getString() + ".mp3";
	}
	AUDIO_ENGINE->play(soundPath.c_str());
}

void GamePiggy::resetSizeLetter(Letter * _lbExpectedLetter)
{
	while (_lbExpectedLetter->getBoundingBox().size.width > _pig->getBoundingBox().size.width - 80)
	{
		_lbExpectedLetter->setScale(_lbExpectedLetter->getScale() - 0.05);
	}
}

cocos2d::Vec2 GamePiggy::checkPositionPig(cocos2d::Vec2 positionPig)
{
	if (positionPig.x + _pig->getBoundingBox().size.width / 2 >= _sizeScreen.x)
	{
		positionPig.x = _sizeScreen.x - _pig->getBoundingBox().size.width / 2;
		return positionPig;
	}
	if (positionPig.x - _pig->getBoundingBox().size.width / 2 <= 0)
	{
		positionPig.x =  _pig->getBoundingBox().size.width / 2;
		return positionPig;
	}
	return positionPig;
}

