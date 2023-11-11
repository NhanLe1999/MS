#include "GameSapXep_Slot.h"

#define CSB_NAME "csb/game/vm_sap_xep/GameSapXep_Slot.csb"


GameSapXep_Slot * GameSapXep_Slot::createSlot(int order, std::string custom_background)
{
	auto * obj = new GameSapXep_Slot();
	if (obj && obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(order, custom_background);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void GameSapXep_Slot::setDelegate(GameSapXep_SlotDelegate * delegate)
{
	this->_delegate = delegate;
}



void GameSapXep_Slot::onShow(float dt,std::function<void()> call_back)
{
	this->setOpacity(0);
	
		m_layout_touch->setTouchEnabled(false);
		m_lbl_order->setVisible(true);
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),cocos2d::FadeIn::create(0.25f), cocos2d::CallFunc::create([=] {
			m_layout_touch->setTouchEnabled(true);
			m_slot_state = GameSapXep_SlotState::SHOWED;
			if (call_back) {
				call_back();
			}
		}), NULL));
	
}

void GameSapXep_Slot::onHide(float dt,std::function<void()> call_back)
{
	m_layout_touch->setTouchEnabled(false);
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::FadeOut::create(0.25f), cocos2d::CallFunc::create([=] {
		m_slot_state = GameSapXep_SlotState::HIDED;
		if (call_back) {
			call_back();
		}
	}), NULL));
}

void GameSapXep_Slot::setLinker(GameSapXep_Item * linker)
{
	m_linker = linker;
}

GameSapXep_Item * GameSapXep_Slot::getLinker()
{
	return m_linker;
}

void GameSapXep_Slot::removeLinker()
{
	m_linker = nullptr;
}


void GameSapXep_Slot::onClickedSlot(cocos2d::Ref * sender)
{
	if (_delegate) {
		_delegate->onClickedSlotDelegate(this);
	}
}

void GameSapXep_Slot::didLoadFromCSB(int order, std::string custom_background)
{
	cocos2d::Size slot_size = cocos2d::Size(240, 240);
	this->setContentSize(slot_size);
	m_slot_state = GameSapXep_SlotState::NONE;
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
	m_layout_touch->addClickEventListener(CC_CALLBACK_1(GameSapXep_Slot::onClickedSlot, this));
	this->addChild(m_layout_touch);

	m_lbl_order = cocos2d::ui::Text::create();
	m_lbl_order->setFontSize(58);
	m_lbl_order->setFontName(font_name);
	m_lbl_order->setString(cocos2d::StringUtils::format("%d", order));
	m_lbl_order->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_lbl_order->setPosition(cocos2d::Vec2(slot_size.width / 2, slot_size.height - 5));
	m_lbl_order->setTextColor(cocos2d::Color4B(214, 95, 0,255));
	this->addChild(m_lbl_order);
	if (custom_background != "" && cocos2d::FileUtils::getInstance()->isFileExist(custom_background)) {
		m_background = cocos2d::ui::ImageView::create(custom_background);
	}
	else {
		m_background = cocos2d::ui::ImageView::create("games/vm_sap_xep/Layer-42-copy-2.png");
	}
	m_background->setPosition(slot_size / 2);
	this->addChild(m_background);
}

