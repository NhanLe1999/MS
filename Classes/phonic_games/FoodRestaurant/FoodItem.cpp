#include "FoodItem.h"

#define PLATE_IMAGE   "games/foodrestaurant/plate.png"
#define SAUSAGE_IMAGE "games/foodrestaurant/sausage.png"
#define MEAT_IMAGE    "games/foodrestaurant/meat.png"
#define HOTDOG_IMAGE  "games/foodrestaurant/hotdog.png"
#define LID_IMAGE "hdr/games/foodrestaurant/lid.png"

FoodItem * FoodItem::createItem(FoodRestaurantInfo food_restaurant_info, int food_type)
{
	
	FoodItem * item = new FoodItem();
	if (item->init()) {
		item->autorelease();
		item->didLoadFromCSB(food_restaurant_info, food_type);
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}
void FoodItem::didLoadFromCSB(FoodRestaurantInfo food_restaurant_info, int food_type)
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
	this->_food_restaurant_info = food_restaurant_info;
	this->game_type = game_type;
	this->food_type = food_type;
	if (food_type < 1 || food_type > 3) {
		this->food_type = cocos2d::random(1, 3);
	}
	this->loadFrames();

	// add Touch Event Listener
	touch_layout->setTouchEnabled(true);
	touch_layout->addTouchEventListener(CC_CALLBACK_2(FoodItem::onTouchFoodItem, this));
	this->onChangeFoodItemState(FOOD_ITEM_STATE::SHOWED);
}

void FoodItem::onTouchFoodItem(cocos2d::Ref * sender, Widget::TouchEventType type)
{
	if (type == TouchEventType::BEGAN) {
		if (_delegate) {
			_delegate->onTouchBeganFoodItemDelegate(this);
		}
	}
	else if(type == TouchEventType::MOVED) {
		CCLOG("on Touch Move");
		onChangeFoodItemState(FOOD_ITEM_STATE::MOVING);
		auto pos = touch_layout->getTouchMovePosition();
		this->setPosition(pos);
	}
	else {
		if (_delegate) {
			_delegate->onMoveEndedFoodItemDelegate(this);
		}
	}
}

void FoodItem::loadFrames()
{
	cocos2d::Size root_size = cocos2d::Size(248, 150);
	this->setContentSize(root_size);

	root_layout = cocos2d::ui::Layout::create();
	root_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	root_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	root_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	root_layout->setOpacity(0);
	root_layout->setContentSize(root_size);
	this->addChild(root_layout, 10);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	touch_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
	touch_layout->setBackGroundColorType(BackGroundColorType::SOLID);
	touch_layout->setOpacity(0);
	touch_layout->setContentSize(root_size);
	this->addChild(touch_layout, 1000);

	plate_image = cocos2d::ui::ImageView::create(PLATE_IMAGE);
	plate_image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	plate_image->setPosition(root_size / 2);
	root_layout->addChild(plate_image, 1);


	
	/*switch (food_type)
	{
	case 1: {
		food_image->loadTexture(SAUSAGE_IMAGE);
		break;
	}
	case 2: {
		food_image->loadTexture(HOTDOG_IMAGE);
		break;
	}
	case 3: {
		food_image->loadTexture(MEAT_IMAGE);
		break;
	}*/
	food_image = cocos2d::ui::ImageView::create(LID_IMAGE);
	food_image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	food_image->ignoreContentAdaptWithSize(false);
	food_image->setContentSize(root_size*1.2);
	food_image->setPosition(Vec2(root_size.width / 2,root_size.height * 0.575)+ Vec2(0,40));
	root_layout->addChild(food_image, 2);

	lbl_text = cocos2d::ui::Text::create("",font_name,60);
	lbl_text->setString(_food_restaurant_info.text);
	lbl_text->setPosition(Vec2(root_size.width / 2, root_size.height * 0.575));
	lbl_text->enableOutline(Color4B::BLACK, 2);
	root_layout->addChild(lbl_text,3);
	
	
}

void FoodItem::setFoodItemDelegate(FoodItemDelegate * delegate)
{
	_delegate = delegate;
}

void FoodItem::onChangeFoodItemState(FOOD_ITEM_STATE state)
{
	if (food_state == state) return;
	food_state = state;
	if (_delegate) {
		_delegate->onChangeFoodItemStateDelegate(state);
	}
}

FOOD_ITEM_STATE FoodItem::getState() const
{
	return food_state;
}

FoodRestaurantInfo FoodItem::getInfo() const
{
	return _food_restaurant_info;
}


void FoodItem::cleanPlate()
{
	food_image->setOpacity(0);
	lbl_text->setOpacity(0);
	touch_layout->setTouchEnabled(false);
}
