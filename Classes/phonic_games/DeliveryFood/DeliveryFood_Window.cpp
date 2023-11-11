#include "DeliveryFood_Window.h"

#define GAF_WINDOW "gaf/deliveryfood/window/window.gaf"
#define GAF_FOX "gaf/deliveryfood/fox/fox.gaf" 
#define GAF_CAT "gaf/deliveryfood/cat/cat.gaf" 
#define GAF_DOG "gaf/deliveryfood/dog/dog.gaf" 
#define GAF_SHEEP "gaf/deliveryfood/sheep/sheep.gaf" 
#define GAF_COW "gaf/deliveryfood/cow/cow.gaf"
#define GAF_GOAT "gaf/deliveryfood/goat/goat.gaf"
#define GAF_ANGRY_FX "gaf/deliveryfood/angry_fx/angry_fx.gaf"

#define SOUND_OPEN "sounds/deliveryfood/df_open.mp3"
#define SOUND_CLOSE "sounds/deliveryfood/df_close.mp3"
#define SOUND_GIVEFOOD "sounds/deliveryfood/df_givefood.mp3"
#define SOUND_IDLE "sounds/deliveryfood/df_idle.mp3"

DeliveryFood_Window * DeliveryFood_Window::createWindow(mj::PHONIC_INFO info, int character_id)
{
	auto * obj = new DeliveryFood_Window();
	if (obj && obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(info, character_id);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

cocos2d::ui::Text *  DeliveryFood_Window::getLableText()
{
	return m_lbl_info;
}

void DeliveryFood_Window::setDelegate(DeliveryFood_WindowDelegate * delegate)
{
	this->_delegate = delegate;
}

void DeliveryFood_Window::idleWindow(std::function<void()> call_back)
{
	m_window_state = DeliveryFood_WindowState::IDLE;
	m_gaf_window->playSequence("idle", false, [=](gaf::GAFObject * obj, const std::string &sequenceName) {
		if (call_back) {
			call_back();
		}
	});
	m_lbl_info->setVisible(true);
}

void DeliveryFood_Window::showWindow(float dt,std::function<void()> call_back)
{
	m_layout_root->scheduleOnce([=](float dt) {
		m_layout_touch->setTouchEnabled(false);
		m_window_state = DeliveryFood_WindowState::SHOWING;
		m_gaf_window->playSequence("idle", false);
		m_lbl_info->setVisible(true);
		m_lbl_info->runAction(cocos2d::Sequence::create(cocos2d::FadeIn::create(1.0f), cocos2d::CallFunc::create([=] {
			m_layout_touch->setTouchEnabled(true);
			if (call_back) {
				call_back();
			}
		}), NULL));
	}, dt, "show_window");
	
}

void DeliveryFood_Window::openWindow(std::function<void()> call_back)
{
	m_window_state = DeliveryFood_WindowState::OPEN;
	AUDIO_ENGINE->playSound(SOUND_OPEN);
	m_gaf_character->playSequence("idle");
	m_gaf_window->playSequence("open", false, [=](gaf::GAFObject * obj, const std::string &sequenceName) {
		m_gaf_character->setLocalZOrder(1001);
		if (call_back) {
			call_back();
		}
	});
	m_lbl_info->setVisible(false);
}

void DeliveryFood_Window::closeWindow(std::function<void()> call_back)
{
	m_window_state = DeliveryFood_WindowState::CLOSE;
	AUDIO_ENGINE->playSound(SOUND_CLOSE);
	m_gaf_character->playSequence("idle");
	m_gaf_character->setLocalZOrder(10);
	m_gaf_window->playSequenceWithPause("close", false, [=](gaf::GAFObject * obj, const std::string &sequenceName) {
		//m_gaf_window->playSequence("idle");
		m_lbl_info->setVisible(true);
		if (call_back) {
			call_back();
		}
	});
}

void DeliveryFood_Window::offerFood()
{
	m_layout_root->stopAllActions();
	//m_layout_touch->setTouchEnabled(false);
	if (m_window_state != DeliveryFood_WindowState::OPEN) {
		openWindow();
	}
	unschedule("close_offer");
	m_layout_offer->setVisible(true);
	m_layout_offer->setOpacity(0);
	AUDIO_ENGINE->playSound(SOUND_IDLE);
	m_gaf_character->playSequence("idle", false, [=](gaf::GAFObject * obj, const std::string &sqN) {
		//	closeWindow();
	});
	m_layout_offer->runAction(cocos2d::Sequence::create(cocos2d::FadeIn::create(0.25f), cocos2d::DelayTime::create(44.0f / 28), cocos2d::FadeOut::create(0.25f), NULL));
	scheduleOnce([=](float dt) {
		closeWindow();
	}, 64.0f / 28, "close_offer");


}

void DeliveryFood_Window::playWin(int food_id, std::function<void()> call_back)
{
	unschedule("close_offer");
	/**
		@param id:
			1: xucxich
			2: chicken
			3: soup
			4: shrimp
	*/
	m_layout_offer->setVisible(false);
	m_window_state = DeliveryFood_WindowState::WIN;
	m_layout_touch->setTouchEnabled(false);
	openWindow();
	std::string win_sequence = "idle";
	switch (food_id)
	{
	case 1:
		win_sequence = "win_xucxich";
		break;
	case 2:
		win_sequence = "win_chicken";
		break;
	case 3:
		win_sequence = "win_soup";
		break;
	case 4:
		win_sequence = "win_shrimp";
		break;
	default:
		break;
	}
	m_gaf_character->playSequence(win_sequence, false, [=](gaf::GAFObject * obj, const std::string &squenceName) {
		m_layout_touch->setTouchEnabled(true);
		if (call_back) {
			call_back();
		}
		
		closeWindow();
	});
}

void DeliveryFood_Window::playLose(std::function<void()> call_back)
{
	unschedule("close_offer");
	m_layout_offer->setVisible(false);
	m_window_state = DeliveryFood_WindowState::LOSE;
	m_layout_touch->setTouchEnabled(false);
	openWindow();
	m_gaf_angry_fx->setVisible(true);
	m_gaf_angry_fx->play(false, [=](gaf::GAFObject * obj) {
		m_gaf_angry_fx->setVisible(false);
	});
	m_gaf_character->playSequence("lose", false, [=](gaf::GAFObject * obj, const std::string &sequenceName) {
		m_layout_touch->setTouchEnabled(true);
		if (call_back) {
			call_back();
		}
		closeWindow();
	});
}

mj::PHONIC_INFO DeliveryFood_Window::getInfo() const
{
	return m_window_info;
}

void DeliveryFood_Window::setInfo(mj::PHONIC_INFO info)
{
	m_window_info = info;
	m_lbl_info->setString(info.text_full);
	m_lbl_offer->setString(info.text_full);
    CCLOG("label %s",info.text_full.c_str());
}

DeliveryFood_WindowState DeliveryFood_Window::getWindowState() const
{
	return m_window_state;
}

void DeliveryFood_Window::onClickedWindow(cocos2d::Ref * sender)
{
	if (_delegate) {
		_delegate->onClickedWindowDelegate(this);
	}
}

void DeliveryFood_Window::didLoadFromCSB(mj::PHONIC_INFO info, int character_id)
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	m_window_info = info;
	cocos2d::Size window_size = cocos2d::Size(223, 172);
	m_window_state = DeliveryFood_WindowState::NONE;

	this->setContentSize(window_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	/*this->setBackGroundColorType(BackGroundColorType::SOLID);
	this->setBackGroundColorOpacity(100);
	this->setBackGroundColor(cocos2d::Color3B::BLACK);*/

	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(window_size);
	this->addChild(m_layout_root);

	m_layout_touch = cocos2d::ui::Layout::create();
	m_layout_touch->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_layout_touch->setPosition(window_size / 2);
	m_layout_touch->setContentSize(cocos2d::Size(window_size.width + 15, window_size.height + 40));
	m_layout_touch->setTouchEnabled(true);
	m_layout_touch->addClickEventListener(CC_CALLBACK_1(DeliveryFood_Window::onClickedWindow, this));
	this->addChild(m_layout_touch);

	//m_layout_touch->setBackGroundColorType(BackGroundColorType::SOLID);
	//m_layout_touch->setBackGroundColorOpacity(100);
	//m_layout_touch->setBackGroundColor(cocos2d::Color3B::BLACK);

	m_lbl_info = cocos2d::ui::Text::create(info.text_full, font_name, 52);
    CCLOG("label %s",info.text_full.c_str());
	m_lbl_info->setTextColor(cocos2d::Color4B::BLACK);
	m_lbl_info->setPosition(window_size / 2);
	m_lbl_info->setOpacity(0);
	m_lbl_info->enableOutline(cocos2d::Color4B::WHITE, 2);
	m_layout_root->addChild(m_lbl_info, 1000);

	m_gaf_window = GAFWrapper::createGAF(GAF_WINDOW);
	m_gaf_window->setPosition(cocos2d::Vec2(window_size.width / 2, 135));
	m_layout_root->addChild(m_gaf_window, 100);
	if (character_id < 1 || character_id > 6) {
		character_id = cocos2d::random(1, 6);
	}
	switch (character_id)
	{
	case 1:
		m_gaf_character = GAFWrapper::createGAF(GAF_FOX);
		break;
	case 2:
		m_gaf_character = GAFWrapper::createGAF(GAF_CAT);
		break;
	case 3:
		m_gaf_character = GAFWrapper::createGAF(GAF_DOG);
		break;
	case 4:
		m_gaf_character = GAFWrapper::createGAF(GAF_SHEEP);
		break;
	case 5:
		m_gaf_character = GAFWrapper::createGAF(GAF_GOAT);
		break;
	default:
		m_gaf_character = GAFWrapper::createGAF(GAF_COW);
		break;
	}
	m_gaf_character->playSequence("idle");
	m_gaf_character->setPosition(cocos2d::Vec2(window_size.width / 2, 0.485 * window_size.height));
	m_layout_root->addChild(m_gaf_character, 10);

	m_gaf_angry_fx = GAFWrapper::createGAF(GAF_ANGRY_FX);
	m_gaf_angry_fx->setVisible(false);
	m_gaf_angry_fx->setPosition(cocos2d::Vec2(window_size.width / 2, window_size.height));
	m_layout_root->addChild(m_gaf_angry_fx, 1000);

	auto offer_size = cocos2d::Size(143, 120);

	m_layout_offer = cocos2d::ui::Layout::create();
	m_layout_offer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_layout_offer->setContentSize(offer_size);
	m_layout_offer->setVisible(false);
	m_layout_offer->setPosition(cocos2d::Vec2(window_size.width, window_size.height));
	m_layout_offer->setCascadeOpacityEnabled(true);
	this->addChild(m_layout_offer, 100);

	m_img_offer = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("games/deliveryfood/4.png"));
	m_img_offer->setPosition(offer_size / 2);
	m_layout_offer->addChild(m_img_offer, 1);

	m_lbl_offer = cocos2d::ui::Text::create(m_window_info.text_full, font_name, 40);
    CCLOG("label %s",m_window_info.text_full.c_str());
	m_lbl_offer->setPosition(offer_size / 2);
	m_lbl_offer->setTextColor(cocos2d::Color4B::BLACK);
	m_lbl_offer->enableOutline(cocos2d::Color4B::WHITE, 2);
	m_layout_offer->addChild(m_lbl_offer, 100);
}

