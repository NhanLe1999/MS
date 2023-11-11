/*
*
* RythmMatching_Group.h
* Created by minhtzy on 26/12/18
*/

#ifndef MJ_GAME_RYTHM_MATCHING_GROUP_H__
#define MJ_GAME_RYTHM_MATCHING_GROUP_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocostudiomacro2.h"
#include "RythmMatching_Item.h"

USING_NS_CC;
using namespace ui;


class RythmMatching_GroupDelegate {
public:
	virtual void onTouchItemBegan(cocos2d::Ref * sender) {};
	virtual void onTouchItemEnded(cocos2d::Ref * sender) {};
};

class RythmMatching_Group : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {

public:

	CREATE_FUNC(RythmMatching_Group);
	static RythmMatching_Group * createGroup(RythmMatching_Item * item1, RythmMatching_Item * item2);
	
	void onShow(float delay = 0.25f);
	void onShowDash(float delay = 0.25f);
	void onHide(float delay = 0.5f);
	void onPairItem();
	void onDetected(cocos2d::Vec2 point);
	bool isDetected(cocos2d::Vec2 point);
	bool isPaired();
	RythmMatching_Item * getItem1();
	RythmMatching_Item * getItem2();

protected:
	RythmMatching_Group();
	void didLoadFromCSB(RythmMatching_Item * item1, RythmMatching_Item * item2);
	RythmMatching_Item * _item1;
	RythmMatching_Item * _item2;
	bool is_paired;
private:
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
};
CREATE_READER(RythmMatching_Group);
#endif // !MJ_GAME_RYTHM_MATCHING_GROUP_H__
