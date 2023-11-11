#include "ClownOnBike.h"
#include "MJHelper.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#include <string.h>

INITIALIZE_READER(ClownOnBike);

std::once_flag ClownOnBike_reader;
#define CSB_NAME "csb/game/ClownOnBike/ClownBike.csb"

#define GAF_ROPE "gaf/ClownOnBike/rope2/rope2.gaf"
#define GAF_CLOWN "gaf/ClownOnBike/chu_he/chu_he.gaf"
#define GAF_BALL1 "gaf/ClownOnBike/bong_tim/bong_tim.gaf"
#define GAF_BALL2 "gaf/ClownOnBike/bong_vang/bong_vang.gaf"
#define GAF_BALL3 "gaf/ClownOnBike/bong_xanh/bong_xanh.gaf"
#define GAF_HAND_TAP "gaf/ClownOnBike/hand_tap/hand_tap.gaf"

#define SOUND_BG1 "sounds/ClownOnBike/cob_bgm1.mp3"
#define SOUND_BG2 "sounds/ClownOnBike/cob_bgm2.mp3"
#define SOUND_BALL_FADEIN "sounds/ClownOnBike/cob_balloon_fadein.mp3"
#define SOUND_BALL_POP "sounds/ClownOnBike/cob_balloon_pop.mp3"
#define SOUND_CLOWN_BALANCE "sounds/ClownOnBike/cob_clown_balanced.mp3"
#define SOUND_CLOWN_FALL "sounds/ClownOnBike/cob_clown_fall.mp3"
#define SOUND_CLOWN_FRIGHTENED "sounds/ClownOnBike/cob_clown_frightened.mp3"
#define SOUNE_CLOWN_UNICYCLE "sounds/ClownOnBike/cob_unicycle.mp3"

#define AUDIO_TAP_TO_BALANCE "sounds/ClownOnBike/tap_on_the_bubble_to_keep_the_clown_balance.mp3"
#define AUDIO_TAP_TO_BALANCE_VI "sounds/ClownOnBike/tap_on_the_bubble_to_keep_the_clown_balance_vi.mp3"
#define AUDIO_TAP_ON "sounds/ClownOnBike/Which_one_is2.mp3"
#define AUDIO_TAP_TO_HELP "sounds/ClownOnBike/tap_the_bubble_to_help_him.mp3"
#define AUDIO_THE_CLOWN_FALLING "sounds/ClownOnBike/the_clown_is_falling_down.mp3"
#define AUDIO_COME_ON "sounds/ClownOnBike/come_on.mp3"
#define AUDIO_WHICH_ONE "sounds/ClownOnBike/Which_one_is2.mp3"
#define AUDIO_WHICH_ONE_VI_AM "sounds/ClownOnBike/Which_one_is2_vi_am.mp3"
#define AUDIO_WHICH_ONE_VI_VAN "sounds/ClownOnBike/Which_one_is2_vi_van.mp3"
#define AUDIO_GAME_NAME "sounds/ClownOnBike/Rocking Clown.mp3"
#define BG_GAME "games/ClownOnBike/RockingClown.png"
#define BG "games/ClownOnBike/2.png"
#define TAP_ON "sounds/ClownOnBike/which_one_is.mp3"


ClownOnBike * ClownOnBike::createGame(std::string json_file)
{
	std::call_once(ClownOnBike_reader, [] {
		REGISTER_CSB_READER(ClownOnBike);
	});

	auto p = static_cast<ClownOnBike *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void ClownOnBike::didLoadFromCSB(std::string json_file){
	this->question = new Question();

	string str = FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(str.c_str());

	auto json_data = document.FindMember("games")->value.GetArray();
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

    for (rapidjson::SizeType i = 0; i < json_data.Size(); i++)
	{
		Question* question = new Question();
        question->rep = json_data[i].FindMember("rep")->value.GetInt();
		auto img_id = json_data[i].FindMember("right_answer")->value.GetString();
        auto pImage = math::resource::ResHelper::getImage(images, img_id);

        question->word = pImage->getText("name_1");
        question->audio = pImage->getAudios("name_1").front();
		rapidjson::Value& json_question = json_data[i].FindMember("wrong_answers")->value;
		for (rapidjson::SizeType i = 0; i < json_question.Size(); i++)
		{
            pImage = math::resource::ResHelper::getImage(images, json_question[i].GetString());
            question->question.push_back({pImage->getText("name_1"),pImage->getAudios("name_1").front()});
		}
        question->rep2 = json_data[i].FindMember("rep2")->value.GetInt();
        question->hoc_am_hoc_van = "";
        if (json_data[i].HasMember("option_vm_phonic") && json_data[i].FindMember("option_vm_phonic")->value.GetString()){
            question->hoc_am_hoc_van = json_data[i].FindMember("option_vm_phonic")->value.GetString();
        }
		this->questions.push_back(question);
	}
}

void ClownOnBike::onEnter()
{
	MJMainGBase::onEnter();

	this->setContentSize(screen_size);
	auto root = cocos2d::utils::findChild(this, "root");
	root->setAnchorPoint(Vec2(0, 0));
	root->setPosition(Vec2(0, 0));

	this->currentLevel = -1;

	auto cbg = cocos2d::utils::findChild(this, "bg");
	cbg->setPosition(cbg->getPositionX() - 50, cbg->getPositionY());
	this->bgPosX = cbg->getPositionX();
	this->bgPosY = cbg->getPositionY();

	//init rope
	this->rope = GAFWrapper::createGAF(GAF_ROPE);
	root->addChild(rope);
	rope->setAnchorPoint(Vec2(0, 0));
	rope->setPosition(Vec2(-200,-250));
	rope->play(true);

	//init clown
	this->clown = GAFWrapper::createGAF(GAF_CLOWN);
	root->addChild(clown);
	clown->setAnchorPoint(Vec2(0.5, 0.15));
	clown->setPosition(Vec2(530,250));

	//init handTap
	this->handTap = GAFWrapper::createGAF(GAF_HAND_TAP);
	root->addChild(handTap);
	handTap->setPosition(clown->getPosition());
	handTap->setVisible(false);
	handTap->setZOrder(1000);
	handTap->playSequence("tap", true);

	//init ball nodes
	this->leftBallNode = cocos2d::utils::findChild(this, "left_node");
	this->rightBallNode = cocos2d::utils::findChild(this, "right_node");
	leftBallNode->setLocalZOrder(100);
	rightBallNode->setLocalZOrder(100);
	for (int i = 0; i < 3; i++)
	{
		Ball * ball1;
		Ball * ball2;
		switch (i)
		{
		case 0:
			ball1 = Ball::Init(GAF_BALL1, true,screen_size.width, "");
			ball2 = Ball::Init(GAF_BALL1, false,screen_size.width, "");
			break;
		case 1:
			ball1 = Ball::Init(GAF_BALL2, true, screen_size.width, "");
			ball2 = Ball::Init(GAF_BALL2, false, screen_size.width, "");
			break;
		case 2:
			ball1 = Ball::Init(GAF_BALL3, true, screen_size.width, "");
			ball2 = Ball::Init(GAF_BALL3, false, screen_size.width, "");
			break;
		}
		ball1->setDelegate(this);
		ball2->setDelegate(this);
		leftBalls.push_back(ball1);
		rightBalls.push_back(ball2);
		root->addChild(ball1);
		root->addChild(ball2);
	}

	//init balls for phrase 2
	this->ball1 = Ball::Init(GAF_BALL1, left, screen_size.width, "");
	this->ball2 = Ball::Init(GAF_BALL2, left, screen_size.width, "");
	this->ball3 = Ball::Init(GAF_BALL3, left, screen_size.width, "");
	this->ball1->color = 1;
	this->ball2->color = 2;
	this->ball3->color = 3;
	this->ball1->setDelegate(this);
	this->ball2->setDelegate(this);
	this->ball3->setDelegate(this);
	this->ball1->setScale(1.1);
	this->ball2->setScale(1.1);
	this->ball3->setScale(1.1);
	root->addChild(ball1);
	root->addChild(ball2);
	root->addChild(ball3);
	ball1->phrase2 = true;
	ball2->phrase2 = true;
	ball3->phrase2 = true;

	//init firework
	auto firework1 = cocos2d::utils::findChild(this, "firework1");
	auto firework2 = cocos2d::utils::findChild(this, "firework2");
	firework1->setContentSize(Size(screen_size.width + 10,firework1->getContentSize().height));
	firework2->setContentSize(Size(screen_size.width + 10,firework2->getContentSize().height));

	//game name
	/*auto gameName = cocos2d::ui::Text::create("Clown On Bike",PHONIC_MANAGER->getGameFontName(), 150);
	root->addChild(gameName);
	gameName->setPosition(screen_size/2);
	gameName->setAnchorPoint(Vec2(0.5, 0.5));
	gameName->runAction(Sequence::create(FadeOut::create(0.0),FadeIn::create(1.5),FadeOut::create(1.5),NULL));*/

	this->currentLevel++;
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (FileUtils::getInstance()->isFileExist(BG)) {
		backgound = Sprite::create(BG);
		backgound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		backgound->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		backgound->setScale(2.0);
		backgound->setName("bg");
		this->addChild(backgound);
	}
	if (FileUtils::getInstance()->isFileExist(BG_GAME)) {
		backgound_game = Sprite::create(BG_GAME);
		backgound_game->setPosition(Vec2(visibleSize.width / 2, 1500));
		backgound_game->setName("bg_game");
		backgound_game->setScale(2);
		this->addChild(backgound_game, INT_MAX - 1);
		backgound_game->runAction(MoveTo::create(2.0f, Vec2(visibleSize.width / 2, visibleSize.height / 2 + 540)));
	}
	
	this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
		if (backgound_game) {
			backgound_game->runAction(FadeOut::create(1.5));
		}
		}), DelayTime::create(2.5f), CallFunc::create([=] {
			if (backgound) {
				backgound->removeFromParent();
			}
			triggerReady();
			}), NULL));

	if (FileUtils::getInstance()->isFileExist(AUDIO_GAME_NAME)) {
		this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
			AUDIO_ENGINE->playEffect(AUDIO_GAME_NAME);
			}), NULL));
	}
}

void ClownOnBike::onExit(){
    //AUDIO_ENGINE->stopAllEffects();
    for(Question* question : questions)
    {
        CC_SAFE_DELETE(question);
    }
    MJMainGBase::onExit();
}

void ClownOnBike::triggerReady()
{
	screen_size = Director::getInstance()->getVisibleSize();
	this->timeCount = -4.0f;
	this->point = 0;
	this->gameState = GameState::Ready;
	this->clownState = ClownState::Idle;
	rope->setPosition(Vec2(-200, -250));
	//get data
	if (this->currentLevel == this->questions.size()) {
        AUDIO_ENGINE->stopAllAudio();
		MJMainGBase::onFinishGame();
		return;
	}
	this->question = this->questions.at(this->currentLevel);

	//Init Ball Position
	for (int i = 0; i < 3; i++)
	{
		leftBalls[i]->stopAllActions();
		rightBalls[i]->stopAllActions();
	}
	this->topLeftBall = 0;
	this->topRightBall = 0;
	leftBalls[0]->setPosition(Vec2(leftBalls[0]->getPositionX(), -250));
	rightBalls[0]->setPosition(Vec2(rightBalls[0]->getPositionX(), -250));
	for (int i = 1; i < 3; i++)
	{
		putBallToLast(i, true);
		putBallToLast(i, false);
	}

	//play sound back ground
	AUDIO_ENGINE->stopAllSound();
    playBackgroundMusic(SOUND_BG1);
//    AUDIO_ENGINE->playSound(SOUND_BG1, true, nullptr, 1.0);
	AUDIO_ENGINE->playSound(SOUNE_CLOWN_UNICYCLE, true, nullptr, 1.0);

	//zoom out background
	auto bg = cocos2d::utils::findChild(this, "bg");
	auto bg2 = cocos2d::utils::findChild(this, "bg2");
	auto hihi = bg->getChildByName("hihi");
	auto firework1 = cocos2d::utils::findChild(this, "firework1");
	auto firework2 = cocos2d::utils::findChild(this, "firework2");

	firework1->stopAllActions();
	firework2->stopAllActions();

	bg->stopAllActions();
	bg2->stopAllActions();
	bg->setPosition(Vec2(this->bgPosX, this->bgPosY));
	bg2->setPosition(Vec2(this->bgPosX+bg->getContentSize().width, this->bgPosY));

	clown->stopAllActions();
	clown->setRotation(0);
	clown->setPosition(Vec2(530, 250));
	clown->playSequence("idle", true);

	hihi->setVisible(true);

	//create unlimited firework
	firework1->runAction(cocos2d::RepeatForever::create(cocos2d::MoveBy::create(6.0, Vec2(0, -screen_size.height))));
	firework2->runAction(cocos2d::RepeatForever::create(cocos2d::MoveBy::create(6.0, Vec2(0, -screen_size.height))));
	firework2->setPosition(Vec2(firework2->getPositionX(), firework1->getPositionY() + firework1->getContentSize().height - 40.0));

	bg->runAction(Sequence::create(cocos2d::ScaleTo::create(0.0, 2.0), cocos2d::ScaleTo::create(3.0, 1.0), NULL));
	bg->runAction(Sequence::create(cocos2d::MoveBy::create(0.0, cocos2d::Vec2(0, -screen_size.height/3)),
		CallFunc::create([=] {
			this->scheduleOnce([=](float dt) {
				AUDIO_ENGINE->playSound(AUDIO_TAP_TO_BALANCE, false, nullptr, 1.0);
			}, 2, "play in");
		}),
		cocos2d::MoveBy::create(3.0, cocos2d::Vec2(0,screen_size.height/3)) ,

		Sequence::create(cocos2d::MoveBy::create(1.5, cocos2d::Vec2(-screen_size.width/4, 0.0)),
			CallFunc::create([=] {
					hihi->setVisible(false);
					bg2->setPosition(Vec2(bg->getPositionX() + bg->getContentSize().width - 40.0, bg2->getPositionY()));

					auto repeat4ever = cocos2d::RepeatForever::create(cocos2d::MoveBy::create(6.0, cocos2d::Vec2(-screen_size.width, 0.0)));
					bg->runAction(repeat4ever);

					auto repeat4ever2 = cocos2d::RepeatForever::create(cocos2d::MoveBy::create(6.0, cocos2d::Vec2(-screen_size.width, 0.0)));
					bg2->runAction(repeat4ever2);

					//create unlimited ball nodes
					for (int i = 0; i < 3; i++)
					{
						this->leftBalls[i]->runAction(RepeatForever::create(MoveBy::create(2.0, Vec2(0, screen_size.height))));
						this->rightBalls[i]->runAction(RepeatForever::create(MoveBy::create(2.0, Vec2(0, screen_size.height))));
					}
					this->leftSpeed = 0;
					this->rightSpeed = 0;

					this->gameState = GameState::Playing;
					this->triggerPlay();
				//}, 1);
			}),NULL), NULL));
}

void ClownOnBike::triggerPlay()
{

		string word = this->question->word;
		string wordAudio = this->question->audio;
		for (int i = 0; i < 3; i++)
		{
			this->leftBalls.at(i)->setText(word);
			this->rightBalls.at(i)->setText(word);
			this->leftBalls.at(i)->setAudio(wordAudio);
			this->rightBalls.at(i)->setAudio(wordAudio);
		}
//		ball1->setAudio(wordAudio);
//		ball2->setAudio(wordAudio);
//		ball3->setAudio(wordAudio);

		this->clickable = true;
		this->scheduleUpdate();


}

void ClownOnBike::update(float dt)
{
	this->timeCount += dt;
	if (this->timeCount == 56000)
		this->timeCount = 5600;
	screen_size = Director::getInstance()->getVisibleSize();

	//clown unbalance every 200 time units
	if (this->gameState == GameState::Playing && this->timeCount > 400.0f / 60)
	{
		timeCount = 0;
		this->changeClownState();
	}

	//create unlimited background
	auto bg = cocos2d::utils::findChild(this, "bg");
	auto bg2 = cocos2d::utils::findChild(this, "bg2");
	if (bg->getPositionX() + bg->getContentSize().width + 50.0 < 0)
	{
		bg->setPosition(Vec2(bg2->getPositionX() + bg2->getContentSize().width, bg->getPositionY()));
	}
	if (bg2->getPositionX() + bg2->getContentSize().width + 50.0 < 0)
	{
		bg2->setPosition(Vec2(bg->getPositionX() + bg->getContentSize().width, bg2->getPositionY()));
	}

	//create unlimited firework
	auto firework1 = cocos2d::utils::findChild(this, "firework1");
	auto firework2 = cocos2d::utils::findChild(this, "firework2");
	if (firework1->getPositionY() + firework1->getContentSize().height + 10.0 < 0)
	{
		firework1->setPositionY(firework2->getPositionY() + firework2->getContentSize().height - 40.0);
	}
	if (firework2->getPositionY() + firework2->getContentSize().height + 10.0 < 0)
	{
		firework2->setPositionY(firework1->getPositionY() + firework1->getContentSize().height - 40.0);
	}

	//create unlimited balls
	if (this->gameState == GameState::Playing)
	{
		if (this->leftBalls[topLeftBall]->getPositionY() - 50.0 >= screen_size.height)
		{
			//if (!leftBalls[topLeftBall]->boomed) this->changeClownState();
			putBallToLast(this->topLeftBall, true);
			this->topLeftBall++;
			if (this->topLeftBall == 3) this->topLeftBall = 0;
		}
		if (this->rightBalls[topRightBall]->getPositionY() + this->rightBallNode->getPositionY() - 50.0 >= screen_size.height)
		{
			//if (!rightBalls[topRightBall]->boomed) this->changeClownState();
			putBallToLast(this->topRightBall, false);
			this->topRightBall++;
			if (this->topRightBall == 3) this->topRightBall = 0;
		}
	}

//	//transistion ending
//	if (this->gameState == GameState::Ending)
//		if (this->timeCount == 350)
//			this->phrase2();
}

void ClownOnBike::phrase2()
{
	guideGame();
	this->generateQuestion();
	ball1->runAction(MoveTo::create(0.75, Vec2(ball1->getPositionX(), screen_size.height*0.5)));
	ball2->runAction(MoveTo::create(0.75, Vec2(ball2->getPositionX(), screen_size.height*0.5)));
	ball3->runAction(MoveTo::create(0.75, Vec2(ball3->getPositionX(), screen_size.height*0.5)));

	//play Sound "tap on 'the'. Which on is 'the'"
	AUDIO_ENGINE->playSound(SOUND_BALL_FADEIN, false);
	vector<string> sounds;
	sounds.clear();
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 2);
	int ran = dis(gen);
    if (CONFIG_MANAGER->isGameVMonkey()){
        if (this->question->hoc_am_hoc_van == "hoc_am"){
            sounds.push_back(AUDIO_WHICH_ONE_VI_AM);
        }else{
            sounds.push_back(AUDIO_WHICH_ONE_VI_VAN);
        }
    }else{
        if (ran == 1) sounds.push_back(AUDIO_TAP_ON);
        else sounds.push_back(AUDIO_WHICH_ONE);
    }
	sounds.push_back(this->question->audio);
	AUDIO_ENGINE->playChain(sounds, nullptr, 1.0, 0);
}

void ClownOnBike::changeClownState()
{
	changeBallSpeed(true, -screen_size.height / 5);
	changeBallSpeed(false, -screen_size.height / 5);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1,2);

	int ran;

	switch (this->clownState)
	{
	case ClownState::Idle:
		ran = dis(gen);
		if (ran == 1) this->clownState = ClownState::Right;
		else this->clownState = ClownState::Right;
		break;
	case ClownState::Left:
		this->clownState = ClownState::LeftIdle;
		break;
	case ClownState::LeftIdle:
		this->clownState = ClownState::LeftFall;
		break;
	case ClownState::LeftFall:
		this->clownState = ClownState::FallToLeft;
	case ClownState::Right:
		this->clownState = ClownState::RightIdle;
		break;
	case ClownState::RightIdle:
		this->clownState = ClownState::RightFall;
		break;
	case ClownState::RightFall:
		this->clownState = ClownState::FallToRight;
	}
	this->presentClown();
}

void ClownOnBike::putBallToLast(int i, bool left)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(7,15);

	int ran;

	int prep = i - 1;
	if (prep == -1) prep = 2;
	{
		ran = dis(gen)*50;
		if (left)
		{
			leftBalls.at(i)->setPosition(Vec2(leftBalls.at(i)->getPositionX(), leftBalls.at(prep)->getPositionY() - ran));
			leftBalls.at(i)->setLocalZOrder(leftBalls.at(prep)->getLocalZOrder()+1);
			leftBalls.at(i)->boomed = false;
			leftBalls.at(i)->InitPosition();
			leftBalls.at(i)->touchable = true;
		}
		else
		{
			rightBalls.at(i)->setPosition(Vec2(rightBalls.at(i)->getPositionX(), rightBalls.at(prep)->getPositionY() - ran));
			rightBalls.at(i)->setLocalZOrder(rightBalls.at(prep)->getLocalZOrder()+1);
			rightBalls.at(i)->boomed = false;
			rightBalls.at(i)->InitPosition();
			rightBalls.at(i)->touchable = true;
		}
	}

}

void ClownOnBike::presentClown()
{
	switch (this->clownState)
	{
	case ClownState::Idle:
		flag = true;
		this->clown->runAction(cocos2d::RotateTo::create(0.5, 0.0));
		this->clown->playSequence("idle", true);
		break;
	case ClownState::Left:
		this->warning();
		this->clown->runAction(cocos2d::RotateTo::create(0.5, -10.0));
		this->clown->playSequence("idle", true);
		break;
	case ClownState::LeftIdle:
		this->clown->runAction(cocos2d::RotateTo::create(0.5, -30.0));
		this->clown->playSequence("idle_left", true);
		break;
	case ClownState::LeftFall:
		this->warning();
		this->clown->runAction(cocos2d::RotateTo::create(0.5, -45.0));
		this->clown->playSequence("fall_left", true);
		AUDIO_ENGINE->playSound(SOUND_CLOWN_FRIGHTENED,false);
		break;
	case ClownState::Right:
		if (flag) {
			flag = false;
			this->warning();
		}
		this->clown->runAction(cocos2d::RotateTo::create(0.5, 10.0));
		this->clown->playSequence("idle", true);
		break;
	case ClownState::RightIdle:
		this->clown->runAction(cocos2d::RotateTo::create(0.5, 30.0));
		this->clown->playSequence("idle_right", true);
		break;
	case ClownState::RightFall:
		this->warning();
		this->clown->runAction(cocos2d::RotateTo::create(0.5, 45.0));
		this->clown->playSequence("fall_right", true);
		AUDIO_ENGINE->playSound(SOUND_CLOWN_FRIGHTENED, false);
		break;
	case ClownState::FallToLeft:
		this->clown->runAction(cocos2d::RotateTo::create(1.5, -180.0));
		this->clown->runAction(cocos2d::MoveTo::create(1.5, Vec2(this->clown->getPositionX(), -250)));
		this->clown->playSequence("fall", true);
		AUDIO_ENGINE->playSound(SOUND_CLOWN_FALL, false);
		this->triggerLose();
		break;
	case ClownState::FallToRight:
		flag = true;
		this->clown->runAction(cocos2d::RotateTo::create(1.5, 180.0));
		this->clown->runAction(cocos2d::MoveTo::create(1.5, Vec2(this->clown->getPositionX(), -250)));
		this->clown->playSequence("fall", true);
		AUDIO_ENGINE->playSound(SOUND_CLOWN_FALL, false);
		this->triggerLose();
		break;
	}
}

void ClownOnBike::Balance(bool toIdle)
{
	if (toIdle)
	{
		this->clownState = ClownState::Idle;
		presentClown();
		AUDIO_ENGINE->playSound(SOUND_CLOWN_BALANCE, false);
		return;
	}
	switch (this->clownState)
	{
	case ClownState::LeftFall:
		this->clownState = ClownState::LeftIdle;
		break;
	case ClownState::LeftIdle:
		this->clownState = ClownState::Left;
		break;
	case ClownState::Left:
		this->clownState = ClownState::Idle;
		break;
	case ClownState::RightFall:
		this->clownState = ClownState::RightIdle;
		break;
	case ClownState::RightIdle:
		this->clownState = ClownState::Right;
		break;
	case ClownState::Right:
		this->clownState = ClownState::Idle;
		break;
	}
	presentClown();
}

//called in presentClown
void ClownOnBike::warning()
{
	//play Sound "Come on! the Clown is falling down..."
    if (!CONFIG_MANAGER->isGameVMonkey())
	AUDIO_ENGINE->playSound(AUDIO_COME_ON, false, nullptr, 1.0);

	//ball warning
	for (int i = 0; i < 3; i++)
	{
		leftBalls[i]->Warning();
		rightBalls[i]->Warning();
	}
}

void ClownOnBike::changeBallSpeed(bool left, int DSpeed)
{
	if (left)
	{
		if (DSpeed > 0)
		{
			if (this->leftSpeed == 10) return;
			else this->leftSpeed++;
		}
		else
		{
			if (this->leftSpeed == 0) return;
			else this->leftSpeed--;
		}
	}
	else
	{
		if (DSpeed > 0)
		{
			if (this->rightSpeed == 10) return;
			else this->rightSpeed++;
		}
		else
		{
			if (this->rightSpeed == 0) return;
			else this->rightSpeed--;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		this->leftBalls[i]->runAction(RepeatForever::create(MoveBy::create(2.0, Vec2(0, DSpeed))));
		this->rightBalls[i]->runAction(RepeatForever::create(MoveBy::create(2.0, Vec2(0, DSpeed))));
	}
}

void ClownOnBike::triggerLose()
{
	this->gameState = GameState::Lose;

	this->unscheduleUpdate();
	this->transition();
	//lose ->shut down game
}

void ClownOnBike::triggerEnd()
{
	screen_size = Director::getInstance()->getVisibleSize();
	this->point = 0;
	auto bg = cocos2d::utils::findChild(this, "bg");
	auto bg2 = cocos2d::utils::findChild(this, "bg2");

	//set up background
	/*this->clown->setOpacity(0);
	this->rope->setOpacity(0);
	bg->stopAllActions();
	bg2->stopAllActions();*/
	this->Balance(true);
	this->clown->runAction(MoveBy::create(3.0, Vec2(-screen_size.width,0)));
	this->rope->runAction(MoveBy::create(3.0, Vec2(-screen_size.width, 0)));
	bg->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([=] {
		bg->stopAllActions();
	}), NULL));
	bg2->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([=] {
		bg2->stopAllActions();
	}), NULL));

	//Init Ball Position
	for (int i = 0; i < 3; i++)
	{
		leftBalls[i]->stopAllActions();
		rightBalls[i]->stopAllActions();
	}
	this->topLeftBall = 0;
	this->topRightBall = 0;
	leftBalls[0]->setPosition(Vec2(leftBalls[0]->getPositionX(), -250));
	rightBalls[0]->setPosition(Vec2(rightBalls[0]->getPositionX(), -250));
	for (int i = 1; i < 3; i++)
	{
		putBallToLast(i, true);
		putBallToLast(i, false);
	}

	//play background sound
	AUDIO_ENGINE->stopAllAudio();
    AUDIO_ENGINE->playSound(SOUND_BG2, true);

	this->timeCount = 250;

    //transistion ending
    this->scheduleOnce([=](float)
    {
        this->phrase2();
    },1.5f,"transition_ending");

}

void ClownOnBike::transition()
{
	this->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([=] {
		this->triggerReady();
	}), NULL));
}

void ClownOnBike::generateQuestion()
{
	//init balls' position
	screen_size = Director::getInstance()->getVisibleSize();
	this->ball1->setPosition(Vec2(screen_size.width*0.15, -250));
	this->ball2->setPosition(Vec2(screen_size.width*0.5, -250));
	this->ball3->setPosition(Vec2(screen_size.width*0.85, -250));

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1,3);

	int ran = dis(gen);
	this->rightAnswer = ran;
	switch (ran)
	{
	case 1:
		ball1->setText(this->question->word);
        ball1->setAudio(this->question->audio);
        ball2->setText(this->question->question.at(0).first);
        ball2->setAudio(this->question->question.at(0).second);
		ball3->setText(this->question->question.at(1).first);
        ball3->setAudio(this->question->question.at(1).second);
		break;
	case 2:
		ball1->setText(this->question->question.at(0).first);
        ball1->setAudio(this->question->question.at(0).second);
		ball2->setText(this->question->word);
        ball2->setAudio(this->question->audio);
		ball3->setText(this->question->question.at(1).first);
        ball3->setAudio(this->question->question.at(1).second);
		break;
	case 3:
		ball1->setText(this->question->question.at(0).first);
        ball1->setAudio(this->question->question.at(0).second);
		ball2->setText(this->question->question.at(1).first);
        ball2->setAudio(this->question->question.at(1).second);
		ball3->setText(this->question->word);
        ball3->setAudio(this->question->audio);
            
	}
}

void ClownOnBike::BallClick(Ref* ref)
{
	AUDIO_ENGINE->stopSound(AUDIO_COME_ON);
	if (!this->clickable) return;
	Ball * ball = static_cast<Ball*>(ref);
	if (this->gameState == GameState::Playing)
	{
		AUDIO_ENGINE->stopSound(AUDIO_TAP_TO_BALANCE);
		if (!ball->boomed)
		//check if ball boomed or not
		{
			ball->Boom();
			changeBallSpeed(ball->Left, screen_size.height / 10);
			this->point++;
			this->timeCount = 0;
			Balance(false);
			if (this->point == this->question->rep)
			{
				this->gameState = GameState::Ending;
				this->triggerEnd();
			}
		}
	}

	if (this->gameState == GameState::Ending && ball->phrase2)
	{
		AUDIO_ENGINE->stopSound(AUDIO_TAP_ON);
		AUDIO_ENGINE->stopSound(AUDIO_WHICH_ONE);
		if (ball->color == this->rightAnswer)
		{
			itemOverLay();
			stopGuideGame();
			guideGame();
			this->point++;
			this->runAction(Sequence::create(DelayTime::create(2.5), CallFunc::create([=] {
				if (this->point == this->question->rep2)
				{
					this->timeCount = 0;
					//MJMainGBase::onFinishGame();
					this->currentLevel++;
					triggerReady();
					stopGuideGame();
				}
			}), NULL));
			ball->Boom();
			ball->runAction(Sequence::create(DelayTime::create(2.5), CallFunc::create([=] {
				ball->InitPosition();
			}), NULL));
			switch (this->rightAnswer)
			{
			case 1:
				ball2->FlyAway(); ball3->FlyAway();
				break;
			case 2:
				ball1->FlyAway(); ball3->FlyAway();
				break;
			case 3:
				ball1->FlyAway(); ball2->FlyAway();
				break;
			}
			this->rightAnswer = -1;
			this->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([=] {
                this->generateQuestion();
				this->timeCount = 300;
            }),DelayTime::create(0.75f),CallFunc::create([=]{
                if (this->point < this->question->rep2 && this->gameState == GameState::Ending)
                    this->phrase2();
				removeItemOverLay();
            }), NULL));
		}
		else {
			ball->Wrong();
			stopGuideGame();
			guideGame();
		}
	}
}

void ClownOnBike::guideGame()
{
	vector<string> sounds;
	sounds.clear();
	sounds.push_back(this->question->audio);
	//play Sound "tap on the bubble to keep the clown balance
	this->schedule([=](float) {
			AUDIO_ENGINE->playChain(sounds);
		}, 10, 100, 10, "show_guide_plug_bulbs_2");
}

void ClownOnBike::stopGuideGame() {
	this->unschedule("show_guide_plug_bulbs_2");
}

void ClownOnBike::itemOverLay() {
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
}

void ClownOnBike::removeItemOverLay() {
	if (cocos2d::utils::findChild(this, "overlay")) {
		this->removeChildByName("overlay");
	}
}
