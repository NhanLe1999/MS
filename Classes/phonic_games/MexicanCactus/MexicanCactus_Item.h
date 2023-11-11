/*
*
* MexicanCactus_Item.cpp
* Created by minhtzy on 3/10/18
*/

#ifndef MJ_GAME_MEXICAN_CACTUS_ITEM_H__
#define MJ_GAME_MEXICAN_CACTUS_ITEM_H__

#include "GAFWrapper.h"
#include "cocos2d.h"

USING_NS_CC;

class MexicanCactus_ItemDelegate {
public:
	virtual void onMovedCactusBegan(cocos2d::Ref * sender) {};
	//virtual void onCactusMoved(cocos2d::Ref * sender) {};
	virtual void onMovedCactusEnded(cocos2d::Ref * sender) {};
};

class MexicanCactus_Item : public cocos2d::ui::Layout {
public:
	struct MexicanCactus_ItemInfo {
		std::string text;
		std::string audio;
		std::vector<int> correct_index;
	};
	static MexicanCactus_Item * createCactus(MexicanCactus_ItemInfo info, int cactus_id = 1);
	void setDelegate(MexicanCactus_ItemDelegate * delegate);
	MexicanCactus_ItemInfo getCactusInfo();
	void setCactusInfo(MexicanCactus_ItemInfo info);
	void showCactus(float dt = 0.25f);
	void hideCactus(float dt = 0.25f, int type = 1);
	void onCorrectCactus(float dt = 0.25f);
	void resetCactus();
	void setCactusTouchable(bool touchable);
	void backToStart(float move_time = 0.25f);
	cocos2d::ui::Text * getLblText();
protected:
	MexicanCactus_Item();
	void didLoadFromCSB(MexicanCactus_ItemInfo info, int cactus_id);
	MexicanCactus_ItemDelegate * _delegate;
	void onTouchCactusListener(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type);
	cocos2d::Vec2 start_pos;
private:
	cocos2d::ui::Layout * touch_layout;
	MexicanCactus_ItemInfo m_info;
	GAFWrapper * m_gaf_cactus;
	cocos2d::ui::Text * m_lbl_text;
	cocos2d::Size cactus_size;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";


};
#endif // !MJ_GAME_MEXICAN_CACTUS_ITEM_H__
