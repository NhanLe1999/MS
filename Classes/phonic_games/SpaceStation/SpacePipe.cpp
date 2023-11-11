#include "SpacePipe.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"

USING_NS_CC;

bool SpacePipe::init()
{
	if (!Node::init())
		return false;
	m_sprite = Sprite::create("games/spacestation/4.png");

	addChild(m_sprite);

    m_label = Label::create("", "fonts/KGWHATTHETEACHERWANTS.TTF", 40);
	m_label->setTextColor(Color4B::BLACK);
	m_label->enableOutline(Color4B::WHITE, 2);
    m_label->setName("text");
	addChild(m_label);
	return true;
}

void SpacePipe::onEnter()
{
	Node::onEnter();

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SpacePipe::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SpacePipe::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SpacePipe::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
}

void SpacePipe::onExit()
{
	Node::onExit();
}

void SpacePipe::setText(mj::GameObject text)
{
	m_label->setString(text.text);
	m_text = text;
}

mj::GameObject SpacePipe::getText()
{
	return m_text;
}

void SpacePipe::attach(cocos2d::Vec2 position)
{
	if (m_attached)
		return;
	m_attached = true;
	stopAllActions();
	Vector<FiniteTimeAction*> actions;
	actions.pushBack(Spawn::createWithTwoActions(
		MoveTo::create(0.25f, position),
		RotateTo::create(0.25f, 0)
	));
	actions.pushBack(CallFunc::create([=]() {
		m_sprite->setTexture("games/spacestation/4.png");
		m_label->setTextColor(Color4B::BLACK);
		m_label->enableOutline(Color4B::WHITE, 2);
		auto effect = GAFWrapper::createGAF("gaf/spacestation/drop.gaf");
		addChild(effect);
		effect->play(false, [=](gaf::GAFObject* obj) {
			effect->removeFromParent();
		});
		AUDIO_ENGINE->play("sounds/spacestation/correct.mp3");
	}));
	actions.pushBack(ScaleTo::create(0.1f, 1.1f));
	actions.pushBack(ScaleTo::create(0.1f, 1));
	runAction(Sequence::create(actions));
}

void SpacePipe::detach()
{
	auto pos = getPosition();

    float y = random(220, 280);
    pos.y = rand()%2 ? y : -y;
    
//    pos.y += ((rand() % 2) ? random (200, 250) : (-250));
	pos.x += ((rand() % 200) - 100);
	detach(pos);
}

void SpacePipe::detach(cocos2d::Vec2 pos)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
	m_attached = false;
	m_sprite->stopAllActions();
	m_label->disableEffect(LabelEffect::OUTLINE);
	m_label->setTextColor(Color4B::WHITE);
	auto path = pos - getPosition();
	runAction(RotateTo::create(1, rand() % 40 - 20));
	auto move = EaseOut::create(MoveTo::create(1, pos), 1.3f);
	auto changeTexture = CallFunc::create([=]() {
		m_sprite->setTexture("games/spacestation/7.png");
	});
    runAction(Sequence::create(changeTexture, move, CallFunc::create([=]{
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
    }),NULL));
	AUDIO_ENGINE->play("sounds/spacestation/disconnect.mp3");
}

void SpacePipe::hideText()
{
	m_label->setVisible(false);
}

void SpacePipe::revealText()
{
	m_label->setOpacity(0);
	m_label->setVisible(true);
	m_label->runAction(FadeIn::create(0.3f));
}

void SpacePipe::setReleaseCallback(std::function<void(SpacePipe*)> callback)
{
	m_releaseCallback = callback;
}

bool SpacePipe::onTouchBegan(Touch * touch, Event * unusedEvent)
{
    if(!m_label->isVisible()) return false;
    
	auto localPoint = m_sprite->convertToNodeSpace(touch->getLocation());
	if (!Rect(0, 0, m_sprite->getContentSize().width, m_sprite->getContentSize().height).containsPoint(localPoint)) {
		return false;
	}

	AUDIO_ENGINE->play("sounds/spacestation/interact.mp3");
    AUDIO_ENGINE->play(m_text.audio_path);
	if (m_attached) {
		//detach();
		return false;
	}
	m_lastTouchPosition = touch->getLocation();
	setLocalZOrder(3);
	return true;
}

void SpacePipe::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto location = touch->getLocation();
	location.x = std::min(std::max(100.0f, location.x), visibleSize.width - 150);
	location.y = std::min(std::max(100.0f, location.y), visibleSize.height - 100);
	setPosition(getPosition() + location - m_lastTouchPosition);
	auto pos = getPosition();
	CCLOG("%f %f", pos.x, pos.y);
	m_lastTouchPosition = location;
}

void SpacePipe::onTouchEnded(Touch * touch, Event * unusedEvent)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    float x = rand() % 50 - 25;
    float y = rand() % 50 - 25;
    
    if(touch->getLocation().y > visibleSize.height/2){
        y = -y;
    }
    
    if(touch->getLocation().x > visibleSize.width / 2){
        x = -x;
    }
    
	runAction(EaseOut::create(MoveBy::create(0.75f, cocos2d::Vec2(x, y)),1.3f));
	setLocalZOrder(2);
	if (m_releaseCallback)
		m_releaseCallback(this);
}

SpacePipe::SpacePipe() :
	m_attached(true),
	m_sprite(nullptr),
	m_label(nullptr),
	m_releaseCallback(nullptr)
{
	srand(time(NULL));
}
