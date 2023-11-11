#include "TextNode_VM.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"

USING_NS_CC;

WizardCauldron_VM::TextNode_VM * WizardCauldron_VM::TextNode_VM::create(gaf::GAFAsset * asset)
{
	auto result = TextNode_VM::create();
    if(result)
        result->m_gafAsset = asset;
    else
        CC_SAFE_DELETE(result);
	return result;
}

void WizardCauldron_VM::TextNode_VM::setText(mj::GameObject text)
{
    std::string font_name =PHONIC_MANAGER->getGameFontName();
    if (text.text == "/" || text.text == "`" || text.text == "~" || text.text == "?" || text.text == "."|| text.text == "´"){
        font_name = ("fonts/vmonkey_punctuation_custom_font.ttf");
    }
    m_label->removeFromParent();
    m_label = cocos2d::Label::create(text.text, font_name, 80);
    m_label->setTextColor(Color4B::BLACK);
	m_text = text;
    addChild(m_label);
    
}

bool WizardCauldron_VM::TextNode_VM::init()
{
	if (!Node::init())
		return false;

	initColor();

	m_label = cocos2d::Label::create("", CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF", 80);
	m_label->setTextColor(Color4B::BLACK);
	addChild(m_label);

	return true;
}

void WizardCauldron_VM::TextNode_VM::onEnter()
{
	Node::onEnter();

	auto mainAction = CallFunc::create([=]() {
		auto baseScale = getScale();
		auto scaleUp = ScaleTo::create(0.3f, baseScale * 1.1);
		auto scaleDown = ScaleTo::create(0.3f, baseScale);
		runAction(RepeatForever::create(Sequence::createWithTwoActions(scaleUp, scaleDown)));
	});
	auto delay = DelayTime::create((std::rand() % 5) * 0.6f / 5);
	runAction(Sequence::createWithTwoActions(delay, mainAction));

	// setup touch move letter
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(TextNode_VM::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TextNode_VM::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TextNode_VM::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

cocos2d::Color4B WizardCauldron_VM::TextNode_VM::getColor()
{
	return m_color;
}

mj::GameObject WizardCauldron_VM::TextNode_VM::getString()
{
	return m_text;
}

bool WizardCauldron_VM::TextNode_VM::onTouchBegan(Touch * touch, Event * unusedEvent)
{
	if (!m_bg)
		return false;

	if (!m_touchEnabled)
		return false;

	auto localLocation = m_bg->convertToNodeSpace(touch->getLocation());
	if (Rect(0, 0, m_bg->getContentSize().width, m_bg->getContentSize().height).containsPoint(localLocation)) {
		m_effect->setVisible(true);
		m_lastTouchLocation = touch->getLocation();
		if (m_touchStartCallback)
			m_touchStartCallback(this);
		return true;
	}
	return false;
}

void WizardCauldron_VM::TextNode_VM::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	auto delta = touch->getLocation() - m_lastTouchLocation;
	auto pos = getPosition() + delta;
	setPosition(pos);
	m_lastTouchLocation = touch->getLocation();
}

void WizardCauldron_VM::TextNode_VM::onTouchEnded(Touch * touch, Event * unusedEvent)
{
	auto callback = m_touchReleaseCallback;
	//m_touchReleaseCallback = nullptr;
	bool cbResult = false;
	m_effect->setVisible(false);
	if (callback)
		cbResult = callback(this);
	if (!cbResult)
		moveToBasePosition();
}

void WizardCauldron_VM::TextNode_VM::setTouchReleaseCallback(std::function<bool(TextNode_VM*)> callback)
{
	m_touchReleaseCallback = callback;
}

void WizardCauldron_VM::TextNode_VM::setTouchStartCallback(std::function<void(TextNode_VM*)> callback)
{
	m_touchStartCallback = callback;
}

void WizardCauldron_VM::TextNode_VM::setEffectEnabled(bool isEnabled)
{
	m_effect->setVisible(isEnabled);
}

void WizardCauldron_VM::TextNode_VM::setBasePosition(cocos2d::Vec2 position)
{
	setPosition(position);
	m_basePosition = position;
}

cocos2d::Vec2 WizardCauldron_VM::TextNode_VM::getBasePosition()
{
	return m_basePosition;
}

void WizardCauldron_VM::TextNode_VM::moveToBasePosition()
{
	m_touchEnabled = false;
	auto path = (getPosition() - m_basePosition);
	auto distance = path.getLength();
	auto move = EaseBackOut::create(MoveTo::create(distance / 2500, m_basePosition));
	//    CCLOG("%f %f", m_basePosition);
	auto enableTouch = CallFunc::create([=]() {
		m_touchEnabled = true;
		setPosition(m_basePosition);
	});
	runAction(Sequence::createWithTwoActions(move, enableTouch));
}

WizardCauldron_VM::TextNode_VM::TextNode_VM() :
	m_label(nullptr),
	m_color(0, 0, 0, 255),
	m_bg(nullptr),
	m_touchReleaseCallback(nullptr),
	m_basePosition(0, 0),
	m_touchEnabled(true),
	m_touchStartCallback(nullptr),
	m_effect(nullptr),
	m_gafAsset(nullptr)
{
	m_colorMap.insert(std::make_pair<int, cocos2d::Color4B>(4, Color4B(0xf2, 0x66, 0xd7, 255)));
	m_colorMap.insert(std::make_pair<int, cocos2d::Color4B>(5, Color4B(0xf4, 0x2b, 0x0b, 255)));
	m_colorMap.insert(std::make_pair<int, cocos2d::Color4B>(6, Color4B(0xe2, 0xe2, 0x05, 255)));
	m_colorMap.insert(std::make_pair<int, cocos2d::Color4B>(7, Color4B(0x3f, 0x61, 0xef, 255)));
	m_colorMap.insert(std::make_pair<int, cocos2d::Color4B>(8, Color4B(0xf9, 0x59, 0x00, 255)));
}

void WizardCauldron_VM::TextNode_VM::initColor(int index)
{
	if (index < 4 || index > 8) {
		// generate random color
		index = (std::rand() % 5) + 4;
	}

	if (!m_gafAsset)
		m_effect = GAFWrapper::createGAF("gaf/wizardcauldron/effect.gaf");
	else
		m_effect = GAFWrapper::createGAF(m_gafAsset);
	m_effect->play(true);
	m_effect->setScale(1.5f);
	m_effect->setVisible(false);
	addChild(m_effect);

	auto bg = Sprite::create(std::string("games/wizardcauldron/") + cocos2d::StringUtils::toString(index) + ".png");
	addChild(bg);
	m_bg = bg;
	auto it = m_colorMap.find(index);
	if (it != m_colorMap.end())
		m_color = it->second;
}
