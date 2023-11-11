//
//  MJ_GearsForCar_Item.h
//

#ifndef GearsForCar_Item_h
#define GearsForCar_Item_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"

class GearsForCar_Item : public cocos2d::ui::Layout
{
public:
	struct GEAR_ITEM_WITH_TEXT {
		bool isTrueAns;
		std::string text;
		std::string image_path;
		std::string audio_path;
	};
    CREATE_FUNC(GearsForCar_Item);
    static GearsForCar_Item *createItem(GEAR_ITEM_WITH_TEXT item_info);
    
    GEAR_ITEM_WITH_TEXT _item_info;
    bool itemOnMove =false;
    cocos2d::Vec2 itemPos =cocos2d::Vec2::ZERO;
    void onClicked();
	void onFinishClick();
    void setClickedCallback(const std::function<void()> afunc);
    
    void setStart();
    void showItem();
	void showText();
private:
    void didLoadFromCSB(GEAR_ITEM_WITH_TEXT item_info);
    void onEnter();
    
    cocos2d::ui::ImageView *bgGear;
    cocos2d::ui::ImageView *item;
	cocos2d::ui::Text* text_item;
    std::function<void()> clickedFinish =nullptr;
};
#endif /* MJ_GearsForCar_Item_h */
