#include "Mole.h"
#include "HSAudioEngine.h"

using namespace WhackAMole;

Mole * Mole::createMole(const std::string &gaf_name)
{
	auto wrapper = new Mole();
	if (wrapper && wrapper->init(gaf_name)) {
		wrapper->autorelease();
	}
	else {
		CC_SAFE_DELETE(wrapper);
		wrapper = nullptr;
	}

	return wrapper;
}

bool WhackAMole::Mole::isUp()
{
	return m_isUp;
}

void WhackAMole::Mole::rise()
{
	if (isUp())
		return;
	m_isUp = true;
	m_hitTime = 0;

	// move up action
	{
		auto moveUpAction = cocos2d::MoveTo::create(0.5f, m_baseLabelPos + m_labelOffset);
		auto fadeIn = cocos2d::FadeIn::create(0.5f);
		auto delay = cocos2d::DelayTime::create(0.3f);
		m_label->stopAllActions();
		m_label->runAction(cocos2d::Sequence::createWithTwoActions(
			delay,
			cocos2d::Spawn::createWithTwoActions(moveUpAction, fadeIn)
		));
	}

	AUDIO_ENGINE->play("sounds/whackamole2/up.mp3");
	playSequence("up", false, [=](gaf::GAFObject* obj, std::string) {
		obj->playSequence("idle", true);
	});
	scheduleOnce([=](float) {
		m_isUp = false;
		// move down action
		{
			auto delay = cocos2d::DelayTime::create(0.3f);
			auto moveDownAction = cocos2d::MoveTo::create(0.5f, m_baseLabelPos);
			auto fadeOut = cocos2d::FadeOut::create(0.5f);
			m_label->stopAllActions();
			m_label->runAction(cocos2d::Sequence::createWithTwoActions(
				delay,
				cocos2d::Spawn::createWithTwoActions(moveDownAction, fadeOut)
			));
		}
		AUDIO_ENGINE->play(std::string("sounds/whackamole2/laugh_") + cocos2d::StringUtils::toString(std::rand() % 2 + 1) + ".mp3");
		if (m_onHitListener)
			m_onHitListener->onMoleHide();
		playSequence("down", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
			playSequence("ground", true);
		});
	}, 3.0f, "down_schedule");
}

void WhackAMole::Mole::onEnter()
{
	GAFWrapper::onEnter();
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Mole::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void WhackAMole::Mole::onExit()
{
	GAFWrapper::onExit();
}

void WhackAMole::Mole::setOnHitListener(HitMoleListener* listener)
{
	m_onHitListener = listener;
}

bool WhackAMole::Mole::init(const std::string & gaf_name)
{
	GAFWrapper::init(gaf_name);
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	m_label = cocos2d::Label::create("", font_name, 80.0f);
	m_label->setTextColor(Color4B::BLACK);
	m_label->setPosition(m_baseLabelPos);
	m_label->enableOutline(Color4B::WHITE, 2);
	m_label->setOpacity(0);
	addChild(m_label);
	return true;
}

bool WhackAMole::Mole::onTouchBegan(Touch * touch, Event * event)
{
	if ((!isUp()) || m_invulnerable)
		return false;
	auto pos = touch->getLocation();
	pos = convertToNodeSpace(pos);
	Rect rect(210, 175, getContentSize().width - 480, getContentSize().height - 450);
	/*auto layer = cocos2d::LayerColor::create(Color4B::BLACK, rect.size.width, rect.size.height);
	layer->setPosition(cocos2d::Vec2(rect.getMinX(), rect.getMinY()));
	layer->setAnchorPoint(cocos2d::Vec2::ZERO);
	addChild(layer);*/
	if (rect.containsPoint(pos)) {
		unschedule("down_schedule");
		hit();
		return true;
	}
	return false;
}

void WhackAMole::Mole::update(float dt)
{
	GAFWrapper::update(dt);
	m_hitTime += dt;
}

void WhackAMole::Mole::setText(mj::GameObject str)
{
	m_label->setString(str.text);
	m_go = str;
}

void WhackAMole::Mole::reset()
{
	unschedule("down_schedule");
	playSequence("ground", true);
	m_isUp = false;
	m_invulnerable = false;
	m_label->stopAllActions();
	m_label->setPosition(m_baseLabelPos);
	m_label->setOpacity(0);
}

WhackAMole::Mole::Mole()
	:m_onHitListener(nullptr),
	m_isUp(false),
	m_label(nullptr),
	m_baseLabelPos(300, 200),
	m_labelOffset(0, 140),
	m_hitTime(0),
	m_invulnerable(false)
{
}

void WhackAMole::Mole::hit()
{
	playSequence("hit", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		playSequence("ground", true);
	});

	m_invulnerable = true;
	//fly up text
	{
		auto moveUp = cocos2d::MoveTo::create(0.35f, m_baseLabelPos + m_labelOffset * 2);
		auto delay = cocos2d::DelayTime::create(0.25f);
		auto fade = cocos2d::FadeOut::create(0.15f);
		auto moveBack = cocos2d::CallFunc::create([=]() {
			m_label->setPosition(m_baseLabelPos);
			m_isUp = false;
			m_invulnerable = false;
		});
		m_label->stopAllActions();
		m_label->runAction(moveUp);
		m_label->runAction(cocos2d::Sequence::create(
			delay,
			fade,
			moveBack,
			nullptr
		));
	}

	AUDIO_ENGINE->play(m_go.audio_path);
	AUDIO_ENGINE->play("sounds/whackamole2/hit.mp3");

	if (m_onHitListener)
		m_onHitListener->onMoleHit(m_hitTime,this);
}

void WhackAMole::HitMoleListener::onMoleHide()
{
}

void WhackAMole::Mole::setDown(bool down){
    m_isDown = down;
}

void WhackAMole::Mole::hide(){
    m_isUp = false;
    // move down action
    {
        auto delay = cocos2d::DelayTime::create(0.3f);
        auto moveDownAction = cocos2d::MoveTo::create(0.5f, m_baseLabelPos);
        auto fadeOut = cocos2d::FadeOut::create(0.5f);
        m_label->stopAllActions();
        m_label->runAction(cocos2d::Sequence::createWithTwoActions(delay, cocos2d::Spawn::createWithTwoActions(moveDownAction, fadeOut)));
    }
    AUDIO_ENGINE->playEffect(std::string("soundswhackamole2/laugh_") + StringUtils::toString(std::rand() % 2 + 1) + ".mp3");
    if (m_onHitListener)
        m_onHitListener->onMoleHide();
    
    if(!m_isDown){
        playSequence("down", false, [=](gaf::GAFObject * obj, const std::string & callback_name) {
            playSequence("ground", true);
        });
    }
}
