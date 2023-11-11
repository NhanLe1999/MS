/*
*
* VMRythmMatching_Item.h
* Created by minhtzy on 26/12/18
*/

#ifndef VM_GAME_RYTHM_MATCHING_ITEM_H__
#define VM_GAME_RYTHM_MATCHING_ITEM_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocostudiomacro2.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;


class VMRythmMatching_ItemDelegate {
public:
	virtual void onTouchItemBegan(cocos2d::Ref * sender) {};
	virtual void onTouchItemMoved(cocos2d::Ref * sender) {};
	virtual void onTouchItemEnded(cocos2d::Ref * sender) {};
};

class VMRythmMatching_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {

public:
	struct RM_ITEM_INFO {
		std::string text;
		std::string audio;
		std::string image;
		int pair;
	};
	enum RM_ITEM_BG_TYPE {
		RMI_BG_NONE = 0,
		RMI_BG_DASH,
		RMI_BG_YELLOW,
		RMI_BG_WHITE
	};

	enum RM_ITEM_TOUCHPERMISSION {
		RMI_DENY,
		RMI_CLICK,
		RMI_TOUCH
	};

	enum RM_ITEM_TYPE
	{
		RMI_IMAGE = 1,
		RMI_TEXT,
		RMI_IMAGE_TEXT
	};
	CREATE_FUNC(VMRythmMatching_Item);
	static VMRythmMatching_Item * createItem(RM_ITEM_INFO info,RM_ITEM_TYPE item_type,RM_ITEM_BG_TYPE bg_type = RM_ITEM_BG_TYPE::RMI_BG_NONE, RM_ITEM_TOUCHPERMISSION touch = RM_ITEM_TOUCHPERMISSION::RMI_DENY);
	void setType(RM_ITEM_TYPE item_type);
	void setBackgroundType(RM_ITEM_BG_TYPE bg_type);
	void changePosition();
	void setTouchPermission(RM_ITEM_TOUCHPERMISSION permission);
	void setInfo(RM_ITEM_INFO info);
	RM_ITEM_INFO getInfo();
	void backToStart(float duration = 0.15f);
	void onItemCorrect();
	void onItemWrong();
	void setDelegate(VMRythmMatching_ItemDelegate * delegate);
	void onShow(float delay);
	void onHide(float delay);
protected:

	VMRythmMatching_Item();
	void didLoadFromCSB(RM_ITEM_INFO info, RM_ITEM_TYPE item_type, RM_ITEM_BG_TYPE bg_type, RM_ITEM_TOUCHPERMISSION touch);
	cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callback_name) override;
	void onTouchedItem(cocos2d::Ref * sender,cocos2d::ui::Widget::TouchEventType type);
	void onTouchBegan(cocos2d::Ref * sender);
	void onTouchMoved(cocos2d::Ref * sender);
	void onTouchEnded(cocos2d::Ref * sender);
	cocos2d::ui::Layout * _touch_layout;
	bool _clickable;
	bool _dragable;
	cocos2d::Vec2 _start_pos;

	VMRythmMatching_ItemDelegate * _delegate;

private:
	cocos2d::Size m_size;
	RM_ITEM_INFO m_info;
	RM_ITEM_BG_TYPE m_bg_type;
	RM_ITEM_TYPE m_item_type;
	RM_ITEM_TOUCHPERMISSION m_touch;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
};
CREATE_READER(VMRythmMatching_Item);
#endif // !VM_GAME_RYTHM_MATCHING_ITEM_H__
