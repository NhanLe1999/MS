#include "Seapearl_VM_Item.h"


Seapearl_VM_Item * Seapearl_VM_Item::createItem(CHARACTER_INFO item_info)
{
	auto item = new Seapearl_VM_Item();
	if (item) {
		item->autorelease();
		item->didLoadFromCSB(item_info);
	}
	else
	{
		CC_SAFE_DELETE(item);
		item = nullptr;
	}
	return item;
}

void Seapearl_VM_Item::setDelegate(Seapearl_VM_ItemDelegate * delegate)
{
	this->_delegate = delegate;
}


void Seapearl_VM_Item::showFish()
{
	gaf_fish->playSequence("enlarge");
	lbl_info->runAction(cocos2d::FadeIn::create(13.0f / 32.0f));
}

void Seapearl_VM_Item::hideFish()
{
}

void Seapearl_VM_Item::enableTouch()
{
	m_touch_layout->setTouchEnabled(true);
}

void Seapearl_VM_Item::disableTouch()
{
	m_touch_layout->setTouchEnabled(false);
}

cocos2d::ui::Text * Seapearl_VM_Item::getLblInfo()
{
	return lbl_info;
}

void Seapearl_VM_Item::setSwimArea(cocos2d::Rect rect)
{
	swim_area = rect;
}

void Seapearl_VM_Item::setFlipped(bool flippedX)
{
	gaf_fish->setFlippedX(flippedX);
	if (flippedX) {
		gaf_fish->setPosition(cocos2d::Vec2(220, 140));
	}
	else {
		gaf_fish->setPosition(cocos2d::Vec2(30, 140));
	}
}

void Seapearl_VM_Item::swim()
{
	//this->stopAllActions();
	auto x = cocos2d::random(swim_area.getMinX(), swim_area.getMaxX());
	auto y = cocos2d::random(swim_area.getMinY(), swim_area.getMaxY());
	auto pos_goal = Vec2(x, y);

	auto duration = 64 * 1.0f / 1024 * pos_goal.distance(this->getWorldPosition());
	this->runAction(Sequence::create(EaseSineInOut::create(MoveTo::create(duration, pos_goal)), CallFunc::create([=] {
		this->swim();
	}), NULL));
}


GAFWrapper * Seapearl_VM_Item::getGAFFish()
{
	return gaf_fish;
}


void Seapearl_VM_Item::didLoadFromCSB(CHARACTER_INFO item_info)
{
	screen_size = Director::getInstance()->getWinSize();
	this->item_info = item_info;
	auto fish_size = cocos2d::Size(250, 240);
	this->setContentSize(fish_size);
	this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	//this->setBackGroundColor(cocos2d::Color3B::BLUE);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColorType(BackGroundColorType::SOLID);

	gaf_fish = GAFWrapper::createGAF("gaf/Seapearl_VM/sp_fish/sp_fish.gaf");
	gaf_fish->setPosition(cocos2d::Vec2(30, 140));
	gaf_fish->playSequence("swim_slow");

	this->addChild(gaf_fish);
    font_name = "fonts/danhvan.ttf";
    if (item_info.character == "/" || item_info.character == "`" || item_info.character == "~" || item_info.character == "?" || item_info.character == "." || item_info.character == "´"){
        if (item_info.character == "´"){
            item_info.character = "/";
        }
        font_name = "fonts/vmonkey_punctuation_custom_font.ttf";
    }
	lbl_info = cocos2d::ui::Text::create(item_info.character, font_name, 100);
    if (item_info.character == "/") lbl_info->setString("´");
	lbl_info->setPosition(fish_size / 2);
	lbl_info->setTextColor(cocos2d::Color4B::BLACK);
	lbl_info->enableOutline(cocos2d::Color4B::WHITE, 3);
	this->addChild(lbl_info);

	m_touch_layout = cocos2d::ui::Layout::create();
	m_touch_layout->setContentSize(fish_size);
	this->addChild(m_touch_layout, 10);

	m_touch_layout->addClickEventListener([=](cocos2d::Ref * sender) {
		if (_delegate) {
			_delegate->onItemClicked(this);
		}
	});

	max_speed = 800.0f / 60 * screen_size.width / 1024;
	max_force = max_speed/30;

}

void Seapearl_VM_Item::act(Seapearl_VM_Item * target, std::function<void()> call_back)
{
	this->schedule([=](float dt) {
		Vec2 location = this->getWorldPosition();
		auto target_loc = target->getWorldPosition();
		max_speed = 800.0f *dt * screen_size.width / 1024;
		max_force = max_speed * dt * 2;
		seek(target_loc);
		updateFish();
		if (location.distance(target_loc) < 200) {
			this->unschedule("update fish");
			if (call_back) {
				call_back();
			}
		}
	}, "update fish");
}


void Seapearl_VM_Item::updateFish()
{
	velocity.add(acceleration);
	if (velocity.length() > max_speed) {
		velocity.scale(max_speed / velocity.length());
	}
	Vec2 location = this->getWorldPosition();
	location.add(velocity);
	//acceleration = Vec2::ZERO;
	//auto m_rotationSpeed = 2 * M_PI / 60;
	//auto m_destinationAngle = velocity.getAngle();
	//auto m_baseAngle = this->getRotation() + (float)(gaf_fish->isFlippedX() ? M_PI : 0);
	//if (m_destinationAngle > m_baseAngle) {
	//	m_baseAngle += m_rotationSpeed;
	//	m_baseAngle = std::min(m_baseAngle, m_destinationAngle);
	//}
	//else {
	//	m_baseAngle -= m_rotationSpeed;
	//	m_baseAngle = std::max(m_baseAngle, m_destinationAngle);
	//}
	auto angle = this->getRotation() + (int)CC_RADIANS_TO_DEGREES(velocity.getAngle()) - (gaf_fish->isFlippedX() ? 270 : 120);// +this->getRotation();

	//angle %= 20;
	angle /= 60;
	CCLOG("angle %f", angle);
	this->setPosition(location);
	//this->setRotation(angle);
	this->runAction(RotateBy::create(0.0f, angle));
}

void Seapearl_VM_Item::seek(Vec2 target)
{
	Vec2 location = this->getWorldPosition();
	Vec2 desired = target - location;
	desired.normalize();
	desired *= max_speed;
	Vec2 steer = desired - velocity;
	if (steer.length() > max_force) {
		steer.scale(max_force / steer.length());
	}
	acceleration.add(steer);
}
