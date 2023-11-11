#include "TextNode.h"
#include "WizardCauldronGame.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "Shake.h"
#include "ui/CocosGUI.h"
#include "HSAudioEngine.h"
#include "BackgroundNode.h"
#include "cocos2d.h"
#include "MJMainGBase.h"

USING_NS_CC;

WizardCauldron::TextNode * WizardCauldron::TextNode::create(gaf::GAFAsset * asset, bool hideText)
{
	auto result = TextNode::create();
	if (result)
	{
		result->m_gafAsset = asset;
		result->_hideText = hideText;
	}
    else
        CC_SAFE_DELETE(result);
	return result;
}

void WizardCauldron::TextNode::setText(mj::GameObject text)
{
	if (int index = m_bg->getTag())
	{
		if (index == 7 || index == 5)
		{
			m_label->setTextColor(Color4B::WHITE);
		}
	}
	m_label->setString(text.text);

	if (_hideText)
	{
		m_label->setScale(0.75);
	}

	m_text = text;
}

bool WizardCauldron::TextNode::init()
{
	if (!Node::init())
		return false;

	initColor();

	if (_hideText)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		for (int i = 4; i <= 8; i++)
		{
			auto bg = Sprite::create(std::string("games/wizardcauldron/") + cocos2d::StringUtils::toString(i) + ".png");
			if (bg->getContentSize().width < _minSizeWidth)
			{
				_minSizeHeight = bg->getContentSize().height;
				_minSizeWidth = bg->getContentSize().width;
			}

		}
		
	}
	m_label = cocos2d::Label::create("", "fonts/KGWHATTHETEACHERWANTS.TTF", _hideText ? 50 : 80);

	m_label->setTextColor(Color4B::BLACK);
	addChild(m_label);

	return true;
}

void WizardCauldron::TextNode::onEnter()
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
	listener->onTouchBegan = CC_CALLBACK_2(TextNode::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TextNode::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TextNode::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

cocos2d::Color4B WizardCauldron::TextNode::getColor()
{
	return m_color;
}

mj::GameObject WizardCauldron::TextNode::getString()
{
	return m_text;
}

bool WizardCauldron::TextNode::onTouchBegan(Touch * touch, Event * unusedEvent)
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

void WizardCauldron::TextNode::onTouchMoved(Touch * touch, Event * unusedEvent)
{
	auto delta = touch->getLocation() - m_lastTouchLocation;
	auto pos = getPosition() + delta;
	setPosition(pos);
	m_lastTouchLocation = touch->getLocation();
}

void WizardCauldron::TextNode::onTouchEnded(Touch * touch, Event * unusedEvent)
{
	auto callback = m_touchReleaseCallback;
	bool cbResult = false;
	m_effect->setVisible(false);
	if (callback)
		cbResult = callback(this);
	if (!cbResult)
		moveToBasePosition();
}

void WizardCauldron::TextNode::setTouchReleaseCallback(std::function<bool(TextNode*)> callback)
{
	m_touchReleaseCallback = callback;
}

void WizardCauldron::TextNode::setTouchStartCallback(std::function<void(TextNode*)> callback)
{
	m_touchStartCallback = callback;
}

void WizardCauldron::TextNode::setEffectEnabled(bool isEnabled)
{
	m_effect->setVisible(isEnabled);
}

void WizardCauldron::TextNode::setBasePosition(cocos2d::Vec2 position)
{
	setPosition(position);
	m_basePosition = position;
}

cocos2d::Vec2 WizardCauldron::TextNode::getBasePosition()
{
	return m_basePosition;
}

void WizardCauldron::TextNode::moveToBasePosition()
{
	m_touchEnabled = false;
	auto path = (getPosition() - m_basePosition);
	auto distance = path.getLength();
	auto move = EaseBackOut::create(MoveTo::create(distance / 500, m_basePosition));
	auto enableTouch = CallFunc::create([=]() {
		m_touchEnabled = true;
		setPosition(m_basePosition);
	});
	runAction(Sequence::createWithTwoActions(move, enableTouch));
}

WizardCauldron::TextNode::TextNode() :
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

void WizardCauldron::TextNode::initColor(int index)
{
	auto visbisSize = Director::getInstance()->getVisibleSize();
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
	bg->setTag(index);
	addChild(bg);
	m_bg = bg;
	auto it = m_colorMap.find(index);
	if (it != m_colorMap.end())
		m_color = it->second;
}
