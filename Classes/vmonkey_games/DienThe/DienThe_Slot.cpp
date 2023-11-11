#include "DienThe_Slot.h"

#define CSB_NAME "csb/game/vm_sap_xep/DienThe_Slot.csb"


DienThe_Slot * DienThe_Slot::createSlot(int order, std::string custom_background)
{
	auto * obj = new DienThe_Slot();
	if (obj && obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(order, custom_background);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void DienThe_Slot::setDelegate(DienThe_SlotDelegate * delegate)
{
	this->_delegate = delegate;
}



void DienThe_Slot::onShow(float dt,std::function<void()> call_back)
{
	this->setOpacity(0);
		m_layout_touch->setTouchEnabled(false);
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),cocos2d::FadeIn::create(0.25f), cocos2d::CallFunc::create([=] {
			m_layout_touch->setTouchEnabled(true);
			m_slot_state = DienThe_SlotState::SHOWED;
			if (call_back) {
				call_back();
			}
		}), NULL));
	
}

void DienThe_Slot::onHide(float dt,std::function<void()> call_back)
{
	m_layout_touch->setTouchEnabled(false);
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::FadeOut::create(0.25f), cocos2d::CallFunc::create([=] {
		m_slot_state = DienThe_SlotState::HIDED;
		if (call_back) {
			call_back();
		}
	}), NULL));
}

void DienThe_Slot::setLinker(DienThe_Item * linker)
{
	m_linker = linker;
}

DienThe_Item * DienThe_Slot::getLinker()
{
	return m_linker;
}

void DienThe_Slot::removeLinker()
{
	m_linker = nullptr;
}


void DienThe_Slot::onClickedSlot(cocos2d::Ref * sender)
{
	if (_delegate) {
		_delegate->onClickedSlotDelegate(this);
	}
}

void DienThe_Slot::didLoadFromCSB(int order, std::string custom_background)
{
	cocos2d::Size slot_size = cocos2d::Size(135, 135);
	this->setContentSize(slot_size);
	m_slot_state = DienThe_SlotState::NONE;
	this->setCascadeOpacityEnabled(true);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	//this->setBackGroundColorType(BackGroundColorType::SOLID);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColor(cocos2d::Color3B::GREEN);

	m_layout_touch = cocos2d::ui::Layout::create();
	m_layout_touch->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_layout_touch->setPosition(slot_size / 2);
	m_layout_touch->setContentSize(slot_size);
	m_layout_touch->setTouchEnabled(true);
	m_layout_touch->addClickEventListener(CC_CALLBACK_1(DienThe_Slot::onClickedSlot, this));
	this->addChild(m_layout_touch);

	if (custom_background != "" && cocos2d::FileUtils::getInstance()->isFileExist(custom_background)) {
		m_background = cocos2d::ui::ImageView::create(custom_background);
	}
	else {
		m_background = cocos2d::ui::ImageView::create("games/DienThe/Vector-Smart-Object.png");
	}
	m_background->setPosition(slot_size / 2);
	this->addChild(m_background);
}

