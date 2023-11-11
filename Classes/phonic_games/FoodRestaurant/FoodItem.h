//
//	FoodItem.h
//	Created by mintzy on 28/06/2018
//
//

#ifndef Food_Item_h
#define Food_Item_h

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "FoodRestaurantModel.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;

enum class FOOD_ITEM_STATE
{
	SHOWED,
	RUNNING,
	MOVING,
	CATCHED,
	HIDE
};

class FoodItemDelegate {
public:
	virtual void onChangeFoodItemStateDelegate(FOOD_ITEM_STATE state) {};
	virtual void onTouchBeganFoodItemDelegate(cocos2d::Ref * sender) {};
	virtual void onMoveEndedFoodItemDelegate(cocos2d::Ref * sender) {};
};
class FoodItem : public cocos2d::ui::Layout {
public:
	CREATE_FUNC(FoodItem);
	static FoodItem * createItem(FoodRestaurantInfo food_restaurant_info, int food_type = 4);
	void setFoodItemDelegate(FoodItemDelegate * delegate);
	void onChangeFoodItemState(FOOD_ITEM_STATE state);
	FOOD_ITEM_STATE getState() const;
	FoodRestaurantInfo getInfo() const;
	
	void cleanPlate();

private:
	void loadFrames();
	void didLoadFromCSB(FoodRestaurantInfo food_restaurant_info, int food_type);
	void onTouchFoodItem(cocos2d::Ref * sender, Widget::TouchEventType type);
	FoodItemDelegate * _delegate;
	std::string game_type;
	int food_type;
	FOOD_ITEM_STATE food_state;
	FoodRestaurantInfo _food_restaurant_info;
	cocos2d::ui::Layout * root_layout;
	cocos2d::ui::Layout * touch_layout;

	cocos2d::ui::ImageView * food_image;
	cocos2d::ui::ImageView * plate_image;
	cocos2d::ui::Text * lbl_text;

	std::string font_name = "fonts/Linotte Regular.ttf";
};

#endif // Food_Item_h
