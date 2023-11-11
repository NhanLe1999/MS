#include "TextLeaf.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
TextLeaf * TextLeaf::create(const std::string & filename, const std::string & text)
{
	TextLeaf *sprite = new (std::nothrow) TextLeaf();
	if (sprite && sprite->initWithText(filename, text));
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool TextLeaf::initWithText(const std::string & filename, const std::string & text)
{
	if (!initWithFile(filename))
		return false;
    std::string font_name = "fonts/LeapingFrogFont.ttf";
    if (CONFIG_MANAGER->isGameVMonkey()){
       font_name = PHONIC_MANAGER->getGameFontName();
    }
	_label = Label::create(text, font_name/*"fonts/KGWHATTHETEACHERWANTS.TTF"*/, 200.0f);
	_label->setTextColor(Color4B::WHITE);

	_label->setPosition(getContentSize() / 2);
	addChild(_label);
	m_text = text;
	return true;
}

void TextLeaf::onEnter()
{
	Sprite::onEnter();
	auto listener = cocos2d::EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(TextLeaf::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TextLeaf::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TextLeaf::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool TextLeaf::onTouchBegan(Touch * touch, Event * event)
{
	if (m_disableClick)
		return false;
	auto localLocation = convertToNodeSpace(touch->getLocation());
	Rect boundRect(0, 0, getContentSize().width, getContentSize().height);
	if (!boundRect.containsPoint(localLocation))
		return false;
	m_lastLocation = touch->getLocation();
	setLocalZOrder(1);
	if (m_touchBeganCallback) {
		m_touchBeganCallback(this);
	}
	return true;
}

void TextLeaf::onTouchMoved(Touch * touch, Event * event)
{
    //setPosition(convertToNodeSpace(touch->getLocation()));
    auto pos = this->getParent()->convertToNodeSpace(touch->getLocation());
	setPosition(pos);
	m_lastLocation = touch->getLocation();
}

void TextLeaf::onTouchEnded(Touch * touch, Event * event)
{
	setLocalZOrder(0);
	auto callback = m_touchEndedCallback;
	//m_touchEndedCallback = nullptr;
	if (callback)
		callback(this);
}

void TextLeaf::setTouchEndedCallback(std::function<void(TextLeaf*)> callback)
{
	m_touchEndedCallback = callback;
}

void TextLeaf::setTouchBeganCallback(std::function<void(TextLeaf*)> callback)
{
	m_touchBeganCallback = callback;
}

void TextLeaf::setBasePosition(cocos2d::Vec2 pos)
{
	m_basePosition = pos;
}

void TextLeaf::disableClick()
{
	m_disableClick = true;
}

cocos2d::Vec2 TextLeaf::getBasePosition()
{
	return m_basePosition;
}

std::string TextLeaf::getText()
{
	return m_text;
}

cocos2d::Label* TextLeaf::getLabel()
{
	return _label;
}
void TextLeaf::setLabel(Label * label)
{
	if(label)
		_label = label;
}
TextLeaf::TextLeaf() :
	m_lastLocation(0, 0),
	m_touchEndedCallback(nullptr),
	m_touchBeganCallback(nullptr),
	m_text(""),
	m_basePosition(0,0),
	m_disableClick(false)
{
}
