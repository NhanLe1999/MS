/*
*
* MexicanCactus_Item.cpp
* Created by minhtzy on 3/10/18
*/
#include "MexicanCactus_Item.h"
#include "PhonicConfig.h"
#include "HSAudioEngine.h"

MexicanCactus_Item * MexicanCactus_Item::createCactus(MexicanCactus_ItemInfo info, int cactus_id)
{
	auto cactus = new MexicanCactus_Item();
	if (cactus && cactus->init()) {
		cactus->autorelease();
		cactus->didLoadFromCSB(info, cactus_id);
	}
	else {
		CC_SAFE_DELETE(cactus);
		cactus = nullptr;
	}
	return cactus;
}
MexicanCactus_Item::MexicanCactus_Item()
	: _delegate{ nullptr }
	, m_gaf_cactus{ nullptr }
	, m_lbl_text{ nullptr }
	, cactus_size{ cocos2d::Size(220,300) }
{
}

void MexicanCactus_Item::didLoadFromCSB(MexicanCactus_ItemInfo info, int cactus_id)
{
	this->setContentSize(cactus_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	//this->setCascadeOpacityEnabled(true);
	//this->setBackGroundColor(cocos2d::Color3B::BLUE);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColorType(BackGroundColorType::SOLID);

	m_info = info;

	m_gaf_cactus = GAFWrapper::createGAF(StringUtils::format("gaf/mexicancactus/mexc_cactus%01d/mexc_cactus%01d.gaf", cactus_id, cactus_id));
	m_gaf_cactus->setPosition(Vec2(110, 240));
	m_gaf_cactus->setCascadeOpacityEnabled(true);
	m_gaf_cactus->setOpacity(0);
	m_gaf_cactus->play(false);
	m_gaf_cactus->reset();
	this->addChild(m_gaf_cactus, 1, "gaf_cactus");

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(cactus_size);
	this->addChild(touch_layout, 100000);
	touch_layout->addTouchEventListener(CC_CALLBACK_2(MexicanCactus_Item::onTouchCactusListener, this));

	m_lbl_text = cocos2d::ui::Text::create(m_info.text, font_name, 100);
	m_lbl_text->setPosition(cocos2d::Vec2(cactus_size.width / 2, cactus_size.height * 0.5));
	m_lbl_text->enableOutline(cocos2d::Color4B::BLACK, 3);
	m_lbl_text->setOpacity(0);
	this->addChild(m_lbl_text, 10);
	this->setOpacity(0);
}

void MexicanCactus_Item::onTouchCactusListener(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		start_pos = this->getPosition();
		if (_delegate) {
			_delegate->onMovedCactusBegan(this);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
	{
		auto move_pos = touch_layout->getTouchMovePosition();
		this->setPosition(move_pos);
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		if (_delegate) {
			_delegate->onMovedCactusEnded(this);
		}
		break;
	default:
		break;
	}
}

void MexicanCactus_Item::setCactusTouchable(bool touchable)
{
	touch_layout->setTouchEnabled(touchable);
}

void MexicanCactus_Item::backToStart(float move_time)
{
	this->runAction(Sequence::create(cocos2d::MoveTo::create(move_time, start_pos), CallFunc::create([=] {
	}), NULL));
}

cocos2d::ui::Text * MexicanCactus_Item::getLblText()
{
	return m_lbl_text;
}

void MexicanCactus_Item::setDelegate(MexicanCactus_ItemDelegate * delegate)
{
	_delegate = delegate;
}

MexicanCactus_Item::MexicanCactus_ItemInfo MexicanCactus_Item::getCactusInfo()
{
	return m_info;
}

void MexicanCactus_Item::setCactusInfo(MexicanCactus_ItemInfo info)
{
	this->m_info = info;
	m_lbl_text->setString(m_info.text);
}

void MexicanCactus_Item::showCactus(float dt)
{
	this->scheduleOnce([=](float dt_) {
		m_gaf_cactus->reset();
		this->runAction(cocos2d::Sequence::create(FadeIn::create(0.25f), NULL));
		m_lbl_text->runAction(cocos2d::Sequence::create(DelayTime::create(0.25f),FadeIn::create(0.25f),NULL));
		m_gaf_cactus->runAction(FadeIn::create(0.25f));
	}, dt, "show cactus");
}

void MexicanCactus_Item::hideCactus(float dt, int type)
{
	this->scheduleOnce([=](float dt_) {
		m_lbl_text->setOpacity(0);
		auto screen_size = cocos2d::Director::getInstance()->getWinSize();
		this->runAction(Sequence::create(FadeOut::create(0.5f),RemoveSelf::create(), NULL));
	}, dt, "hide cactus");
}



void MexicanCactus_Item::onCorrectCactus(float dt)
{
	this->scheduleOnce([=](float dt_) {
		m_lbl_text->setOpacity(0);
		m_gaf_cactus->play(false);
	}, dt, "correct cactus");
}

void MexicanCactus_Item::resetCactus()
{
	m_gaf_cactus->reset();
}

