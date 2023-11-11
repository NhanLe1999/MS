//
//  MJ_Seapearl_VM_Item.h
//

#ifndef Seapearl_VM_Item_h
#define Seapearl_VM_Item_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"


USING_NS_CC;

class Seapearl_VM_ItemDelegate {
public:
	virtual void onItemClicked(cocos2d::Ref *sender) {};
};

class Seapearl_VM_Item : public cocos2d::ui::Layout
{
public:

	struct CHARACTER_INFO {
		std::string character;
		std::string audio_path;
	};

	CREATE_FUNC(Seapearl_VM_Item);
	static Seapearl_VM_Item *createItem(CHARACTER_INFO item_info);
	void setDelegate(Seapearl_VM_ItemDelegate *delegate);
	CHARACTER_INFO item_info;
	void showFish();
	void hideFish();
	void enableTouch();
	void disableTouch();
	GAFWrapper * getGAFFish();
	cocos2d::ui::Text * getLblInfo();
	void setSwimArea(cocos2d::Rect rect);
	void setFlipped(bool flippedX);
	void swim();

	void act(Seapearl_VM_Item * target,std::function<void()> call_back = nullptr);

private:

	float max_speed;
	float max_force; 
	Vec2 acceleration;
	Vec2 velocity;
	void updateFish();
	void seek(Vec2 target);
	Size screen_size;

	void didLoadFromCSB(CHARACTER_INFO item_info);
	Seapearl_VM_ItemDelegate *_delegate = nullptr;
	GAFWrapper * gaf_fish;
	cocos2d::ui::Text * lbl_info;
	Rect swim_area;
	std::string font_name = "fonts/vmonkey_punctuation_custom_font.ttf";

	cocos2d::ui::Layout * m_touch_layout;
};
CREATE_READER(Seapearl_VM_Item);
#endif /* MJ_Seapearl_VM_Item_h */
