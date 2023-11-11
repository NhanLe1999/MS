/*
*
* VMRythmMatching.h
* Created by minhtzy on 26/12/18
*/

#ifndef VM_GAME_RYTHM_MATCHING_H1__
#define VM_GAME_RYTHM_MATCHING_H1__

#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "VMRythmMatching_Item.h"
#include "cocostudiomacro2.h"
#include "VMRythmMatching_Group.h"
USING_NS_CC;
using namespace ui;


class VMRythmMatching : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public VMRythmMatching_ItemDelegate {

public:
	CREATE_FUNC(VMRythmMatching);
	static VMRythmMatching * createGame(std::string json_file);
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void onFinishGame() ;
	Vec2 helpChildPlayEasier(Point p1, Point p2);

protected:
	VMRythmMatching();

	struct RYTHM_GAME_INFO {
		std::vector<std::pair<VMRythmMatching_Item::RM_ITEM_INFO, VMRythmMatching_Item::RM_ITEM_INFO>> pair_items;
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
	void onPairing(VMRythmMatching_Group * group, VMRythmMatching_Item * item);

	void showGuideGame();
	void hideGuideGame();
	void showHand();

private:
	enum RM_GAME_PLAY {
		RM_IMAGE_IMAGE = 1,
		RM_IMAGE_TEXT,
		RM_TEXT_IMAGE,
		RM_TEXT_TEXT,
		RM_BOTH
	};
	std::string m_instruction;
	std::string m_configPath;
	RM_GAME_PLAY m_play_type;
	cocos2d::ui::Layout * root_layout;
	cocos2d::ui::Layout * touch_layout;
	GAFWrapper * gaf_house1;
	GAFWrapper * gaf_house2;

	std::vector<VMRythmMatching_Item *> rm_drag_items ;
	std::vector<VMRythmMatching_Group *> rm_groups ;
	std::vector<Vec2> getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding = 0);

	std::vector<RYTHM_GAME_INFO> list_game_info;
	RYTHM_GAME_INFO current_game_info;

	std::string font_name = "fonts/Montserrat-Medium.ttf";
	cocos2d::ui::Button * speaker;
	void onClickedSpeaker(cocos2d::Ref * sender);
	bool _isSpeaking = false;
	bool _guideshowed = false;
	int _countWrongClick = 0;
	int _audioGuiding = -1;
    bool _isShowHand = true;
};
CREATE_READER(VMRythmMatching);
#endif // !VM_GAME_RYTHM_MATCHING_H1__
