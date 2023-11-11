#include "Ball.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"
#define GAF_FIREWORK "gaf/ClownOnBike/phao_giay/phao_giay.gaf"
#define GAF_EFFEC "gaf/ClownOnBike/effec/effec.gaf"
#define GAF_HAND_TAP "gaf/ClownOnBike/hand_tap/hand_tap.gaf"

#define SOUND_BALL_FADEIN "sounds/ClownOnBike/cob_balloon_fadein.mp3"
#define SOUND_BALL_POP "sounds/ClownOnBike/cob_balloon_pop.mp3"

#define AUDIO_WRONG "sounds/ClownOnBike/wrong.mp3"
#define AUDIO_OOPS "sounds/ClownOnBike/oops.mp3"
#define AUDIO_TRY_AGAIN "sounds/ClownOnBike/try_again.mp3"
#define AUDIO_TRY_AGAIN_VI "sounds/ClownOnBike/try_again_vi.mp3"
Ball* Ball::Init(string gaf, bool left, float width, string word)
{
	auto ball = new Ball();
	ball->init(gaf);
	ball->Left = left;
	ball->playSequence("idle", true);
	ball->setAnchorPoint(Vec2(0.5, 0.5));
	ball->boomed = false;
	ball->touchable = true;

	//set text
	string font_name;
	//font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
	//font_name = PHONIC_MANAGER->getGameFontName(GAME_TYPE_SIGHTWORD);
	font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getGameFontName(GAME_TYPE_SIGHTWORD);
    }
	//not disable outLine after Boom
	ball->textField = cocos2d::ui::Text::create(word, font_name , 100);
	ball->textField->setTextColor(Color4B::BLACK);
	ball->textField->setVisible(true);
	ball->addChild(ball->textField);
	ball->textField->enableOutline(Color4B::WHITE, 4);
	ball->textField->setPosition(Vec2(ball->getContentSize().width*0.5, ball->getContentSize().height*0.60));
	ball->textField->setLocalZOrder(100);
	ball->word = word;

	//set effec
	ball->firework = GAFWrapper::createGAF(GAF_FIREWORK);
	ball->effec = GAFWrapper::createGAF(GAF_EFFEC);
	ball->addChild(ball->effec);
	ball->addChild(ball->firework);
	ball->effec->setPosition(ball->textField->getPosition());
	ball->firework->setPosition(ball->textField->getPosition());
	ball->firework->setVisible(false);
	ball->effec->setVisible(false);

	//set hand tap
	ball->hand = GAFWrapper::createGAF(GAF_HAND_TAP);
	ball->hand->setVisible(false);
	ball->addChild(ball->hand);
	ball->hand->setPosition(ball->textField->getPosition());
	ball->hand->setScale(2.0);
	ball->hand->playSequence("tap", true);
	ball->hand->setLocalZOrder(1000);

	//setup Touch Handling
	ball->layout = Layout::create();
	//ball->layout->setBackGroundColor(Color3B::BLUE);
	//ball->layout->setBackGroundColorType(BackGroundColorType::SOLID);
	//ball->layout->setBackGroundColorOpacity(255);
	ball->layout->setSize(ball->getContentSize()*0.4);
	ball->layout->setVisible(true);
	ball->layout->setTouchEnabled(true);
	ball->layout->setAnchorPoint(Vec2(0.5, 0.5));
	ball->layout->setPosition(Vec2(ball->getContentSize().width*0.5, ball->getContentSize().height*0.6));
	ball->layout->setLocalZOrder(1000);
	ball->addChild(ball->layout);
	ball->layout->addClickEventListener([=] (Ref* ref){
		ball->addClick();
	});


	if (left) ball->setPosition(Vec2(width*0.15, -250.0));
	else ball->setPosition(Vec2(width*0.85, -250));
	ball->phrase2 = false;
    ball->autorelease();
	return ball;
}

void Ball::setDelegate(BallDelegate * delegate1)
{
	this->delegate = delegate1;
}

void Ball::setAudio(string wordAudio)
{
	this->wordAudio = wordAudio;
}

void Ball::addClick()
{
	if (!this->touchable) return;
	if (delegate) delegate->BallClick(this);
}

void Ball::Warning()
{
	this->runAction(Sequence::create(ScaleTo::create(0.25, 1.2), ScaleTo::create(0.25,1.0), ScaleTo::create(0.25, 1.2), ScaleTo::create(0.25, 1.0) , NULL));
}

void Ball::Boom()
{
	//disable touch for 2.5s
	AUDIO_ENGINE->stopSound(AUDIO_TRY_AGAIN);
	this->runAction(Sequence::create(CallFunc::create([=] {
		this->touchable = false;
	}), DelayTime::create(2.5), CallFunc::create([=] {
		this->touchable = true;
	}), NULL));

	this->boomed = true;

	//play sound "the"
	
	AUDIO_ENGINE->stopSound(this->wordAudio);
	AUDIO_ENGINE->stopSound(SOUND_BALL_POP);
	AUDIO_ENGINE->playSound(SOUND_BALL_POP, false, nullptr, 1.0);
	AUDIO_ENGINE->playSound(this->wordAudio, false, nullptr, 1.0);

	//effect
	this->playSequence("broke", false);

	this->firework->setVisible(true);
	this->effec->setVisible(true);

	this->firework->runAction(Sequence::create(DelayTime::create(2.0), FadeOut::create(0.5) , NULL));

	this->firework->play(false);
	this->effec->play(false);
	this->textField->runAction(Sequence::create(ScaleTo::create(1.0, 1.5), FadeOut::create(0.5), ScaleTo::create(0.0, 1.0),NULL));
}

void Ball::InitPosition()
{
	/*this->textField->setTextColor(Color4B::BLACK);
	this->textField->enableOutline(Color4B::WHITE, 4);*/
	this->textField->stopAllActions();
	this->textField->setScale(1.0);
	this->textField->setOpacity(255);
	this->playSequence("idle", true);
}

void Ball::setText(string word)
{
	this->textField->setString(word);
	this->setColor(cocos2d::Color3B::WHITE);
	this->textField->setTextColor(Color4B::BLACK);
	this->textField->enableOutline(Color4B::WHITE, 3);
	auto size = this->layout->getContentSize() * 0.8;
	auto label_size = this->textField->getContentSize();
	float fSize = this->textField->getFontSize();
	if (flag) {
		if (label_size.width > size.width * 0.8) {
			fSize = fSize / 2;
			this->textField->setFontSize(fSize);
			label_size = this->textField->getContentSize();
			flag = false;
		}
	}
	
}

void Ball::FlyAway()
{
	this->runAction(Sequence::create(MoveTo::create(0.75, Vec2(this->getPositionX(), this->getPositionY() * 2.5)), CallFunc::create([=] {
		this->setPosition(Vec2(this->getPositionX(), -250));
	}), NULL));
}

void Ball::Wrong()
{
	//play Sound wrong
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1,3);

	int ran = dis(gen);

	AUDIO_ENGINE->stopSound(AUDIO_WRONG);
	AUDIO_ENGINE->stopSound(AUDIO_OOPS);
	AUDIO_ENGINE->stopSound(AUDIO_TRY_AGAIN);
	AUDIO_ENGINE->playSound(AUDIO_TRY_AGAIN);

	this->playSequence("wrong", false);

	float miniAction = 0.15;

	auto left = CallFunc::create([=] {
		this->runAction(MoveBy::create(miniAction, Vec2(30, 0)));
	});

	auto right = CallFunc::create([=] {
		this->runAction(MoveBy::create(miniAction, Vec2(-50, 0)));
	});

	auto left1 = CallFunc::create([=] {
		this->runAction(MoveBy::create(miniAction, Vec2(30, 0)));
	});

	auto right1 = CallFunc::create([=] {
		this->runAction(MoveBy::create(miniAction, Vec2(-10, 0)));
	});

	this->runAction(Sequence::create(CallFunc::create([=] {this->touchable = false; }),
		left, DelayTime::create(miniAction), right, DelayTime::create(miniAction),left1, DelayTime::create(miniAction),right1, 
		CallFunc::create([=] {
		this->touchable = true;
		this->playSequence("idle", true); }), NULL));
}

void Ball::HandTap()
{
	this->hand->runAction(Sequence::create(DelayTime::create(2.5),CallFunc::create([=] {
		this->hand->setVisible(true);
	}), DelayTime::create(2.0), CallFunc::create([=] {
		this->hand->setVisible(false);
	}), NULL));
}
