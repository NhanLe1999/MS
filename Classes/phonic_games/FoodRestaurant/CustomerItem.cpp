#include "CustomerItem.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
#define GAF_DOG "gaf/foodrestaurant/dog/dog.gaf"
#define GAF_CAT "gaf/foodrestaurant/cat/cat.gaf"
#define GAF_SHEEP "gaf/foodrestaurant/sheep/sheep.gaf"
#define BUBBLE_TALK "games/foodrestaurant/talk.png"

CustomerItem * CustomerItem::createItem(FoodRestaurantInfo info, int customer_id)
{
	CustomerItem * obj = new CustomerItem();
	if (obj->init()) {
		obj->autorelease();
		obj->didLoadFromCSB(info, customer_id);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void CustomerItem::didLoadFromCSB(FoodRestaurantInfo info, int customer_id)
{
	this->_food_restaurant_info = info;
	this->customer_id = customer_id;

	if (customer_id > 3 || customer_id < 1) {
		this->customer_id = cocos2d::random(1, 3);
	}
	this->loadFrames();
}

void CustomerItem::loadFrames()
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	cocos2d::Size root_size = cocos2d::Size(200, 380);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(root_size);
	this->setBackGroundColor(cocos2d::Color3B::BLUE);
	this->setBackGroundColorType(BackGroundColorType::SOLID);
	this->setOpacity(0);
	root_layout = cocos2d::ui::Layout::create();
	root_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	root_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	root_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	root_layout->setOpacity(0);
	root_layout->setContentSize(root_size);
	this->addChild(root_layout, 10);

	switch (customer_id)
	{
	case 1: {
		gaf_customer = GAFWrapper::createGAF(GAF_CAT);
		break;
	}
	case 2: {
		gaf_customer = GAFWrapper::createGAF(GAF_DOG);
		break;
	}
	case 3: {
		gaf_customer = GAFWrapper::createGAF(GAF_SHEEP);
		break;
	}
	}
	gaf_customer->setPosition(root_size);
	gaf_customer->playSequence("idle", true);
	root_layout->addChild(gaf_customer, 100);

	cocos2d::Size order_size = cocos2d::Size(230, 200);
	order_layout = cocos2d::ui::Layout::create();
	order_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	order_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	order_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	order_layout->setOpacity(0);
	order_layout->setContentSize(order_size);
	order_layout->setPosition(Vec2(root_size.width * 1.15, 1.5 * root_size.height - 50));
	root_layout->addChild(order_layout, 200);

	img_talk = cocos2d::ui::ImageView::create(BUBBLE_TALK);
	img_talk->setOpacity(0);
	img_talk->setPosition(order_size / 2);
	img_talk->setScaleX(-1);
	order_layout->addChild(img_talk, 100);

	lbl_order_info = cocos2d::ui::Text::create(_food_restaurant_info.text, font_name, 80);
	lbl_order_info->setColor(Color3B::BLACK);
	lbl_order_info->setPosition(Vec2(order_size.width / 2, 0.6 * order_size.height));
	lbl_order_info->setOpacity(0);
	order_layout->addChild(lbl_order_info, 200);

	//if (lbl_order_info->getContentSize().width > img_talk->getContentSize().width) {
	//	lbl_order_info->setFontSize(60);
	//}
}
void CustomerItem::onChangeCustomerState(CUSTOMER_STATE state)
{
	if (this->m_state == state) return;
	this->m_state = state;
	if (m_delegate) {
		m_delegate->onChangeCustomerItemStateDelegate(state);
	}
}
void CustomerItem::showOrder(std::function<void()> callback)
{
	order_layout->stopAllActions();
	onChangeCustomerState(CUSTOMER_STATE::SHOW_ORDER);
	img_talk->setOpacity(255);
	lbl_order_info->setOpacity(0);
	order_layout->setScale(1);
	img_talk->setScale(0);
	img_talk->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	img_talk->setPositionY(0);
	img_talk->runAction(ScaleTo::create(0.25f, -1, 1));

	//if (this->isFlippedX()) {
	//	order_layout->setFlippedX(true);
	//	order_layout->setPosition(Vec2(root_layout->getContentSize().width * 1.15, 1.5 * root_layout->getContentSize().height - 50));
	//}

	lbl_order_info->setFontSize(font_size);
	lbl_order_info->runAction(Sequence::create(DelayTime::create(0.25f), FadeIn::create(0.25f), CallFunc::create([=] {
		order_layout->runAction(Repeat::create(Sequence::create(MoveBy::create(0.1f, Vec2(-2, 0))
			, MoveBy::create(0.2, Vec2(4, 0)), MoveBy::create(0.1f, Vec2(-2, 0)), NULL), 100000000));
		if (callback) {
			callback();
		}
	}), NULL));
}

void CustomerItem::hideOrder(std::function<void()> callback)
{
	order_layout->stopAllActions();
	onChangeCustomerState(CUSTOMER_STATE::HIDE_ORDER);
	order_layout->runAction(Sequence::create(ScaleTo::create(0.25f, 0), CallFunc::create([=] {
		if (callback) {
			callback();
		}
	}), NULL));
}

void CustomerItem::playGafSequence(std::string sequenceName, bool loop, std::function<void()> call_back)
{
	if (sequenceName == "idle") {
		onChangeCustomerState(CUSTOMER_STATE::IDLE);
	}
	else if (sequenceName == "win") {
		onChangeCustomerState(CUSTOMER_STATE::WIN);
	}
	else if (sequenceName == "lose") {
		onChangeCustomerState(CUSTOMER_STATE::LOSE);
	}
	else if (sequenceName == "walk") {
		onChangeCustomerState(CUSTOMER_STATE::WALKING);
	}
	gaf_customer->playSequence(sequenceName, loop, [=](gaf::GAFObject * obj, const std::string & callback_name) {
		if (call_back != nullptr) call_back();
	});
}

void CustomerItem::setInfo(FoodRestaurantInfo  info)
{
	this->_food_restaurant_info = info;
	lbl_order_info->setString(info.text);
}

void CustomerItem::setDelegate(CustomerItemDelegate * delegate)
{
	this->m_delegate = delegate;
}

void CustomerItem::setInfoNumberRepeat(int re)
{
	_food_restaurant_info.number_repeat = re;
}

void CustomerItem::setCustomerFlipped(bool flipped)
{
	gaf_customer->setFlippedX(true);
}

bool CustomerItem::isCustomerFlipped()
{
	return gaf_customer->isFlippedX();
}

FoodRestaurantInfo CustomerItem::getInfo() const
{
	return _food_restaurant_info;
}
