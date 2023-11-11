#include "DeliveryFood_Hand.h"

DeliveryFood_Hand * DeliveryFood_Hand::createHand(mj::PHONIC_INFO info, int hand_id)
{
	auto * obj = new DeliveryFood_Hand();
	if (obj && obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(info, hand_id);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

mj::PHONIC_INFO DeliveryFood_Hand::getInfo() const
{
	return m_hand_info;
}

void DeliveryFood_Hand::setInfo(const mj::PHONIC_INFO info)
{
	this->m_hand_info = info;
}

void DeliveryFood_Hand::idle()
{
	m_layout_root->stopAllActions();
	m_layout_root->setPosition(cocos2d::Vec2::ZERO);
	m_layout_root->runAction(cocos2d::Repeat::create(Sequence::create(MoveBy::create(0.5f, Vec2(0, 20)), MoveBy::create(0.5f, Vec2(0, -20)), NULL), 10000000));
}

void DeliveryFood_Hand::showHand(float dt, std::function<void()> call_back)
{
	m_layout_root->stopAllActions();
	m_layout_root->setPositionY(-50);
	m_layout_root->runAction(cocos2d::Sequence::create(
		Spawn::create(FadeIn::create(dt), MoveBy::create(dt, Vec2(0, 50)), NULL), CallFunc::create([=] {
		this->idle();
		if (call_back) {
			call_back();
		}
	}), NULL));
}

void DeliveryFood_Hand::hideHand(float dt, std::function<void()> call_back)
{
	m_layout_root->stopAllActions();
	this->runAction(cocos2d::Sequence::create(
		Spawn::create(FadeOut::create(dt), MoveBy::create(dt, Vec2(0, -50)), NULL), CallFunc::create([=] {
		this->removeFromParent();
		if (call_back) {
			call_back();
		}
	}), NULL));
}

void DeliveryFood_Hand::servedFood(cocos2d::Vec2 pos)
{
	m_layout_root->stopAllActions();
	m_layout_root->setPosition(cocos2d::Vec2::ZERO);
	auto move_pos = this->convertToNodeSpace(pos);
	m_img_hand->setVisible(false);
	m_lbl_info->setVisible(false);
	m_img_hand_put->setVisible(true);

	m_layout_food->setVisible(true);
	m_layout_food->runAction(Sequence::create(
		Spawn::create(cocos2d::MoveTo::create(0.25f, move_pos), ScaleTo::create(0.25f, 0), NULL),
		CallFunc::create([=] {
		hideHand();
	}), NULL));
}

void DeliveryFood_Hand::backToStart()
{
	m_layout_touch->setTouchEnabled(false);
	this->runAction(Sequence::create(cocos2d::EaseQuadraticActionIn::create(cocos2d::MoveTo::create(0.25f, _start_pos)), CallFunc::create([=] {
		m_layout_touch->setTouchEnabled(true);
		m_layout_root->setVisible(true);
		idle();
		m_layout_food->setVisible(false);
	}), NULL));
}

int DeliveryFood_Hand::getHandId()
{
	return m_hand_id;
}

void DeliveryFood_Hand::setTouchHandleType(DeliveryFoodHandTouchType type)
{
	switch (type)
	{
	case DeliveryFoodHandTouchType::NONE:
		break;
	case DeliveryFoodHandTouchType::CLICK:
		m_layout_touch->setTouchEnabled(true);
		m_layout_touch->addClickEventListener([=](cocos2d::Ref * sender) {
			if (_delegate) {
				_delegate->onClickedHand(this);
			}
		});
		break;
	case DeliveryFoodHandTouchType::TOUCH:
		m_layout_touch->setTouchEnabled(true);
		m_layout_touch->addTouchEventListener([=](cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type) {
			cocos2d::Vec2 move_pos;
			switch (type)

			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				CCLOG("onTouchBegan");
				m_is_moved = false;
				_start_pos = this->getPosition();
				if (_delegate) {
					_delegate->onMoveBeginHand(this);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				m_is_moved = true;
				if (_delegate) {
					m_layout_root->setVisible(false);
					m_layout_food->setVisible(true);
					move_pos = m_layout_touch->getTouchMovePosition();
					this->setPosition(move_pos);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				if (_delegate) {
					_delegate->onMoveEndedHand(this);
				}
				break;
			default:
				break;
			}
		});
		break;
	default:
		break;
	}
}

void DeliveryFood_Hand::setTouchEnable(bool enable)
{
	m_layout_touch->setTouchEnabled(enable);
}

void DeliveryFood_Hand::setVisibleLayout(bool layout_hand, bool layout_food)
{
	m_layout_root->setVisible(layout_hand);
	m_layout_food->setVisible(layout_food);
}


void DeliveryFood_Hand::didLoadFromCSB(mj::PHONIC_INFO info, int hand_id)
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }

	m_hand_info = info;
	auto hand_size = cocos2d::Size(425, 325);

	this->setContentSize(hand_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	//this->setBackGroundColorType(BackGroundColorType::SOLID);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColor(cocos2d::Color3B::BLUE);

	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(hand_size);
	this->addChild(m_layout_root);

	m_layout_touch = cocos2d::ui::Layout::create();
	m_layout_touch->setContentSize(hand_size);
	this->addChild(m_layout_touch, 10000);

	if (hand_id < 1 || hand_id > 4) {
		hand_id = cocos2d::random(1, 4);
	}
	m_hand_id = hand_id;
	std::string hand_path = cocos2d::StringUtils::format("games/deliveryfood/hand/%d.png", hand_id);
	m_img_hand = cocos2d::ui::ImageView::create(hand_path);
	m_img_hand->setPosition((hand_size / 2) + Size(0,-20));
	m_layout_root->addChild(m_img_hand, 1);

	m_img_hand_put = cocos2d::ui::ImageView::create("games/deliveryfood/hand/5.png");
	m_img_hand_put->setPosition(hand_size / 2);
	m_img_hand_put->setVisible(false);
	m_layout_root->addChild(m_img_hand_put, 1);

	m_lbl_info = cocos2d::ui::Text::create(m_hand_info.text_phonic, font_name, 80);
	m_lbl_info->setPosition(cocos2d::Vec2(hand_size.width / 2, 0.6 * hand_size.height));
	m_lbl_info->setTextColor(cocos2d::Color4B::BLACK);
	m_lbl_info->enableOutline(cocos2d::Color4B::WHITE,2);
	m_layout_root->addChild(m_lbl_info, 100);

	m_layout_food = cocos2d::ui::Layout::create();
	m_layout_food->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_layout_food->setContentSize(hand_size);
	m_layout_food->setVisible(false);
	m_layout_food->setPosition(Vec2(hand_size.width / 2, 0.5*hand_size.height));
	this->addChild(m_layout_food, 1000);

	m_img_food_served = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("games/deliveryfood/hand/%d.png", 5 + hand_id));
	m_img_food_served->setPosition(hand_size / 2);
	m_layout_food->addChild(m_img_food_served, 1);

	m_lbl_food = cocos2d::ui::Text::create(m_hand_info.text_phonic, font_name, 80);
	m_lbl_food->setPosition(hand_size / 2);
	m_lbl_food->setTextColor(cocos2d::Color4B::BLACK);
	m_lbl_food->enableOutline(cocos2d::Color4B::WHITE, 2);
	m_layout_food->addChild(m_lbl_food, 100);

}
