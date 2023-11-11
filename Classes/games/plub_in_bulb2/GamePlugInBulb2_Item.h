
#ifndef GAME_PLUG_IN_BULB2_ITEM_HPP
#define GAME_PLUG_IN_BULB2_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"


USING_NS_CC;
using namespace ui;

enum GAME_PLUG_IN_BULB2_ITEM_TYPE
{
	GPIB_IT_IN = 0,	//right
	GPIB_IT_OUT,	//left
	GPIB_IT_IMAGE_AND_TEXT = 10,
	GPIB_IT_TEXT_TO_IMAGE,
	GPIB_IT_IMAGE_TO_TEXT,
	GPIB_IT_TEXT_TO_TEXT
};

class GamePlugInBulb2_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GamePlugInBulb2_Item);

    struct ItemInfo {
		std::string img_path;
        std::string text;
        std::string audio_path;
		std::string description_path;
		int id_answer;
    };
    
    static GamePlugInBulb2_Item * createItem(ItemInfo iten_info, GAME_PLUG_IN_BULB2_ITEM_TYPE type1, GAME_PLUG_IN_BULB2_ITEM_TYPE type2);
	ItemInfo _item_info;
	ImageView* plug = nullptr;
	ImageView* dot = nullptr;
	cocos2d::DrawNode * _rope = nullptr;
	Vec2 posPlug;
	cocos2d::Node * end1 = nullptr;
	cocos2d::Node * end2 = nullptr;
	cocos2d::Node * control_point = nullptr;

	GAME_PLUG_IN_BULB2_ITEM_TYPE _type1;
	GAME_PLUG_IN_BULB2_ITEM_TYPE _type2;
    float font_sz = 60;

	void onTouchPlug(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);
	void updateRope(float dt);
	void movePlugBack();
	void turnBulbOn();
	void connectToRight(GamePlugInBulb2_Item * right, const std::function<void()> afunc = nullptr);
	Layout* panelItem;
	bool flag = true;
	Layout* overlay;
private:
	void didLoadFromCSB(ItemInfo iten_info, GAME_PLUG_IN_BULB2_ITEM_TYPE type1, GAME_PLUG_IN_BULB2_ITEM_TYPE type2);
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GamePlugInBulb2_Item);

#endif
