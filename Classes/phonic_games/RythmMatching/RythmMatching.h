/*
*
* RythmMatching.h
* Created by minhtzy on 26/12/18
*/

#ifndef MJ_GAME_RYTHM_MATCHING_H__
#define MJ_GAME_RYTHM_MATCHING_H__

#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "RythmMatching_Item.h"
#include "cocostudiomacro2.h"
#include "RythmMatching_Group.h"
USING_NS_CC;
using namespace ui;


class RythmMatching : public MJMainGBase , public cocostudio::WidgetCallBackHandlerProtocol, public RythmMatching_ItemDelegate {

public:
	CREATE_FUNC(RythmMatching);
	static RythmMatching * createGame(std::string json_file);
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void onFinishGame() ;
	
protected:
	RythmMatching();

	struct RYTHM_GAME_INFO {
		std::vector<std::pair<RythmMatching_Item::RM_ITEM_INFO, RythmMatching_Item::RM_ITEM_INFO>> pair_items;
	};
	
	void didLoadFromCSB(std::string json_file);
	void generateData();
	void loadFrames();

	void showIntroGame();
	void showGamename();
	void prepareGame();
	void startGame();
	void onNextGame();

	void showGroups();
	void showDragItems();

	int wrong_count;
	int game_index;
	int remain_item;

	void enableTouch();
	void disableTouch(int bg_opacity = 0);

	void onTouchItemBegan(cocos2d::Ref * sender) override;
	void onTouchItemMoved(cocos2d::Ref * sender) override;
	void onTouchItemEnded(cocos2d::Ref * sender) override;
	void onPairing(RythmMatching_Group * group, RythmMatching_Item * item);

	void showGuideGame(float delay_time);
	void hideGuideGame();
private:

	std::string m_configPath;
	cocos2d::ui::Layout * root_layout;
	cocos2d::ui::Layout * touch_layout;
	GAFWrapper * gaf_house1;
	GAFWrapper * gaf_house2;

	std::vector<RythmMatching_Item *> rm_drag_items ;
	std::vector<RythmMatching_Group *> rm_groups ;
	std::vector<Vec2> getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding = 0);

	std::vector<RYTHM_GAME_INFO> list_game_info;
	RYTHM_GAME_INFO current_game_info;

	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    bool m_touchable;
	cocos2d::ui::Button * speaker;
	void onClickedSpeaker(cocos2d::Ref * sender);
};
CREATE_READER(RythmMatching);
#endif // !MJ_GAME_RYTHM_MATCHING_H__
