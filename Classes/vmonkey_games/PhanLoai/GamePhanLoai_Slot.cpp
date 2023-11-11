#include "GamePhanLoai_Slot.h"

#define CSB_NAME "csb/game/vm_sap_xep/GamePhanLoai_Slot.csb"


GamePhanLoai_Slot * GamePhanLoai_Slot::createSlot(mj::PHONIC_INFO info, std::string custom_background)
{
	auto * obj = new GamePhanLoai_Slot();
	if (obj && obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(info, custom_background);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void GamePhanLoai_Slot::setDelegate(GamePhanLoai_SlotDelegate * delegate)
{
	this->_delegate = delegate;
}



void GamePhanLoai_Slot::onShow(float dt,std::function<void()> call_back)
{
	this->setOpacity(0);
	
		m_layout_touch->setTouchEnabled(false);
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt),cocos2d::FadeIn::create(0.25f), cocos2d::CallFunc::create([=] {
			m_layout_touch->setTouchEnabled(true);
			m_slot_state = GamePhanLoai_SlotState::SHOWED;
			if (call_back) {
				call_back();
			}
		}), NULL));
	
}

void GamePhanLoai_Slot::onHide(float dt,std::function<void()> call_back)
{
	m_layout_touch->setTouchEnabled(false);
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::FadeOut::create(0.25f), cocos2d::CallFunc::create([=] {
		m_slot_state = GamePhanLoai_SlotState::HIDED;
		if (call_back) {
			call_back();
		}
	}), NULL));
}

void GamePhanLoai_Slot::setCompareValue(int value)
{
	m_compare_value = value;
}

void GamePhanLoai_Slot::setSlotSize(cocos2d::Size size)
{
	this->setContentSize(size);
	m_background->setContentSize(size);
	m_background->setPosition(size / 2);

	m_layout_touch->setContentSize(size);
	m_layout_touch->setPosition(size / 2);

	if (m_character) {
		float scale = 1;
		auto char_size = m_character->getVirtualRendererSize();
		if (char_size.height > 150) {
			scale = 150/char_size.height;
		}
		if (char_size.width * scale > 130) {
			scale *= 130 / (char_size.width * scale);
		}
		m_character->setScale(scale);
		m_character->setPosition(cocos2d::Vec2(20, size.height / 2));
	}

	if (m_custom_background) {
		cocos2d::Size fix_size = cocos2d::Size::ZERO;
		cocos2d::Vec2 fix_pos = cocos2d::Vec2::ZERO;
		if (_slot_mode & SHOW_BACKGROUND) {
			if (size.width < 400) {
				fix_size = cocos2d::Size(16, 23);
				fix_pos = cocos2d::Vec2(-2.5, 2);
			}
			else {
				fix_size = cocos2d::Size(32, 25);
				fix_pos = cocos2d::Vec2(-2.5, 1);
			}
		}

		m_custom_background->setContentSize(size - fix_size);
		m_custom_background->setPosition(cocos2d::Vec2((size) /2) + fix_pos);
	}

	if (size.height < 250) {
		m_title->setScale(0.67f);
		m_bg_title->setScale(0.67f);
	}
	m_title->setPosition(cocos2d::Vec2(size.width / 2, size.height - 5));
	m_bg_title->setPosition(cocos2d::Vec2(size.width / 2, size.height - 5));
	

}

void GamePhanLoai_Slot::setSlotEnabled(int size_type, uint8_t mode)
{
	_slot_mode = mode;
	cocos2d::Size size;
	switch (size_type) {
	case 1: 
		size = cocos2d::Size(624, 350);
		break;
	case 2:
		size = cocos2d::Size(624, 282);
		break;
	case 3:
		size = cocos2d::Size(624, 188);
		break;
	case 4:
		size = cocos2d::Size(/*328*/318, 282);
		break;
	default:
		size = cocos2d::Size(624, 282);
	}
	

	if (size_type == 4) {
		m_background->loadTexture("games/vm_phan_loai/Layer 13.png");
	}
	else {
		m_background->loadTexture("games/vm_phan_loai/Layer 1.png");
	}

	setSlotSize(size);

	if (mode & SHOW_TEXT) {
		m_bg_title->setVisible(true);
		m_title->setVisible(true);
	}
	else {
		m_bg_title->setVisible(false);
		m_title->setVisible(false);
	}
	if (m_character) {
		if (mode & SHOW_ICON) {
			m_character->setVisible(true);
		}
		else {
			m_character->setVisible(false);
		}
	}

	if (m_background) {
		if (mode & SHOW_BACKGROUND) {
			m_background->setVisible(true);
		}
		else {
			m_background->setVisible(false);
		}
	}
	if (m_custom_background) {
		if (mode & SHOW_CUSTOM_BG) {
			m_custom_background->setVisible(true);
		}
		else {
			m_custom_background->setVisible(false);
		}
	}

	
}

int GamePhanLoai_Slot::getCompareValue()
{
	return m_compare_value;
}


void GamePhanLoai_Slot::onClickedSlot(cocos2d::Ref * sender)
{
	if (_delegate) {
		_delegate->onClickedSlotDelegate(this);
	}
}

void GamePhanLoai_Slot::didLoadFromCSB(mj::PHONIC_INFO info, std::string custom_background)
{
	this->_info = info;
	cocos2d::Size slot_size = cocos2d::Size(624, 282);
	this->setContentSize(slot_size);
	m_slot_state = GamePhanLoai_SlotState::NONE;
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
	m_layout_touch->addClickEventListener(CC_CALLBACK_1(GamePhanLoai_Slot::onClickedSlot, this));
	this->addChild(m_layout_touch);

	m_background = cocos2d::ui::ImageView::create("games/vm_phan_loai/Layer 13.png");
	m_background->setPosition(slot_size / 2);
	m_background->setScale9Enabled(true);
	auto render_size = m_background->getVirtualRendererSize();
	cocos2d::Rect cap = cocos2d::Rect(40, 40, render_size.width - 80, render_size.height - 80);
	m_background->setCapInsets(cap);
	this->addChild(m_background);

	if (custom_background != "" && cocos2d::FileUtils::getInstance()->isFileExist(custom_background)) {
		m_custom_background = cocos2d::ui::ImageView::create(custom_background);
		m_custom_background->setPosition(slot_size / 2);
		m_custom_background->setScale9Enabled(true);
		auto render_size = m_custom_background->getVirtualRendererSize();
		cocos2d::Rect cap = cocos2d::Rect(1, 1, render_size.width - 2, render_size.height - 2);
		m_custom_background->setCapInsets(cap);
		this->addChild(m_custom_background);
	}

	m_bg_title = cocos2d::ui::ImageView::create("games/vm_phan_loai/Vector Smart Object3.png");
	m_bg_title->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_bg_title->setPosition(cocos2d::Vec2(slot_size.width / 2, slot_size.height));
	m_bg_title->setScale9Enabled(true);
	auto t_render_size = m_bg_title->getVirtualRendererSize();
	cocos2d::Rect t_cap = cocos2d::Rect(15, 5, render_size.width - 30, render_size.height - 10);
	m_bg_title->setCapInsets(cap);
	this->addChild(m_bg_title, 1,"bg_title");

	m_character = cocos2d::ui::ImageView::create(info.image_path);
	m_character->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_character->setPosition(cocos2d::Vec2(0, slot_size.height / 2));
	this->addChild(m_character, 2);

	m_title = cocos2d::ui::Text::create();
	m_title->setFontSize(42);
	m_title->setFontName(font_name);
	m_title->setString(info.text_full);
	m_title->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_title->setPosition(cocos2d::Vec2(slot_size.width / 2, slot_size.height));
	this->addChild(m_title,2);

	fitValueTextSize();

}

void GamePhanLoai_Slot::fitValueTextSize()
{
	auto size = m_bg_title->getContentSize() ;
	float label_width = m_title->getContentSize().width;
	
	if (label_width >= size.width) {
		m_bg_title->setContentSize(cocos2d::Size(label_width + 20, size.height));
	}
}

