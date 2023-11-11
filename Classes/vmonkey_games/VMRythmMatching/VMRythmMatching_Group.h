/*
*
* VMRythmMatching_Group.h
* Created by minhtzy on 26/12/18
*/

#ifndef VM_GAME_RYTHM_MATCHING_GROUP_H__
#define VM_GAME_RYTHM_MATCHING_GROUP_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocostudiomacro2.h"
#include "VMRythmMatching_Item.h"

USING_NS_CC;
using namespace ui;


class VMRythmMatching_GroupDelegate {
public:
	virtual void onTouchItemBegan(cocos2d::Ref * sender) {};
	virtual void onTouchItemEnded(cocos2d::Ref * sender) {};
};

class VMRythmMatching_Group : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {

public:

	CREATE_FUNC(VMRythmMatching_Group);
	static VMRythmMatching_Group * createGroup(VMRythmMatching_Item * item1, VMRythmMatching_Item * item2);
	
	void onShow(float delay = 0.25f);
	void onShowDash(float delay = 0.25f);
	void onHide(float delay = 0.5f);
	void onPairItem();
	void onDetected(cocos2d::Vec2 point);
	bool isDetected(cocos2d::Vec2 point);
	bool isPaired();
	VMRythmMatching_Item * getItem1();
	VMRythmMatching_Item * getItem2();

protected:
	VMRythmMatching_Group();
	void didLoadFromCSB(VMRythmMatching_Item * item1, VMRythmMatching_Item * item2);
	VMRythmMatching_Item * _item1;
	VMRythmMatching_Item * _item2;
	bool is_paired;
private:
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
};
CREATE_READER(VMRythmMatching_Group);
#endif // !VM_GAME_RYTHM_MATCHING_GROUP_H__
