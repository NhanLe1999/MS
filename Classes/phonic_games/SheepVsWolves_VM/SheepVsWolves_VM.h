#ifndef MJ_GAME_SHEEP_VS_WOLVES_VM_H_
#define MJ_GAME_SHEEP_VS_WOLVES_VM_H_

#include "MJMainGBase.h"
#include "../external/json/document.h"
#include "../external/json/rapidjson.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "SheepVsWolves_VMInfoModel.h"
#include "SheepVsWolves_VM_Board.h"
#include "SheepVsWolves_VM_Item.h"
#include "MJHelper.h"
USING_NS_CC;
using namespace ui;

enum class SVWGameState_VM {
	NONE,
	GAMENAME,
	INTRO,
	START,
	MOVING,
	PLAYING_LEFT,
	PLAYING_RIGHT,
	SYNC_TEXT,
	ENDED
};
class SheepVsWolves_VM : public MJMainGBase ,public SheepVsWolves_VM_ItemDelegate ,public SheepVsWolves_VM_BoardDelegate{
public:
	CREATE_FUNC(SheepVsWolves_VM);
	static SheepVsWolves_VM * createGame(std::string json_path, bool is_auto_play);
	void generateData(rapidjson::Value& value);
//    void generateData(rapidjson::Value& value, bool is_remote);
protected:
private:
	void didLoadFromCSB(std::string json_path, bool is_auto_play);
	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void update(float dt) override;
	void showGameName();
	void showIntroGame();
	void startGame();
	void onNextSide();
	void onChangeSide(bool left_side);
	void onTransition();
	
	void onNextGame();
	void onFinishGame() ;
	void onRefresh();
	void showGuideGame(float dt);
	void hideGuideGame();
	void onTransitionSyncText(SheepVsWolvesVM::GAME_INFO info, cocos2d::Vec2 pos,std::function<void()> call_back = nullptr);
	void readSyncText(SheepVsWolvesVM::GAME_INFO info,cocos2d::Vec2 pos, std::function<void()> call_back = nullptr);
	void initSheepItems();
	void onShowSheepFromLeftSide(std::function<void()> call_back = nullptr);
	void onShowSheepFromRightSide(std::function<void()> call_back= nullptr);
	void onClickSheepVsWolves_VMDelegate(cocos2d::Ref * sender) override;
	void onClickedBoardDelegate(cocos2d::Ref  *sender) override;
	void showFarm(float dt,std::function<void()> call_back = nullptr);
	void hideFarm(float dt, std::function<void()> call_back = nullptr);
	void setFencesSide(bool flipped, bool transition = true);
	void randomSheepSound();

	std::vector<cocos2d::Vec2> getFrameLocations(cocos2d::Vec2 mid_point,cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding = Vec2::ZERO,cocos2d::Vec2 skew = Vec2::ZERO);

	std::vector<std::string> splitString(const std::string& s, char delimiter);
	
	
	std::vector<SheepVsWolves_VM_Item *> m_sheep_items;
	std::vector<SheepVsWolves_VM_Item *> m_sheep_left_items;
	std::vector<SheepVsWolves_VM_Item *> m_sheep_right_items;
	std::vector<SheepVsWolvesVM::GAME_INFO> m_game_info;
	cocos2d::ui::Layout * m_layout_root{ nullptr };
	cocos2d::ui::Layout * m_layout_synctext{ nullptr };
	cocos2d::ui::Layout * m_layout_sheeps{ nullptr };
	cocos2d::ui::ImageView * m_img_bg{ nullptr };
	cocos2d::ui::ImageView * m_img_loa{ nullptr };
	cocos2d::ui::ImageView * m_img_bg_left{ nullptr };
	cocos2d::ui::ImageView * m_img_bg_right{ nullptr };
    cocos2d::ui::Text * lbl_dau{ nullptr };
	int m_sheep_missing_left;
	int m_sheep_missing_right;

	int game_data_index{ 0 };
	SVWGameState_VM m_game_state;
	SheepVsWolves_VM_Board * m_board{ nullptr };
	GAFWrapper * m_gaf_fences{ nullptr };
	GAFWrapper * m_gaf_warehouse_front{ nullptr };
	GAFWrapper * m_gaf_warehouse_back{ nullptr };
	GAFWrapper * m_gaf_warehouse_door{ nullptr };
	std::string font_name = "fonts/danhvan.ttf";

    
	bool m_fences_flipped{ false };
	// auto play
	bool m_is_auto_play;
	void autoPlay(float dt);
    std::string question_vi;
    bool checkRomaji(char c);
};

#endif // MJ_GAME_SHEEP_VS_WOLVES_H_
