#ifndef MJ_GAME_SHEEP_VS_WOLVES_H_
#define MJ_GAME_SHEEP_VS_WOLVES_H_

#include "MJMainGBase.h"
#include "../external/json/document.h"
#include "../external/json/rapidjson.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "SheepVsWolvesInfoModel.h"
#include "SheepVsWolves_Board.h"
#include "SheepVsWolves_Item.h"
#include "MJHelper.h"
USING_NS_CC;
using namespace ui;

enum class SVWGameState {
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
class SheepVsWolves : public MJMainGBase ,public SheepVsWolves_ItemDelegate ,public SheepVsWolves_BoardDelegate{
public:
	CREATE_FUNC(SheepVsWolves);
	static SheepVsWolves * createGame(std::string json_path, bool is_auto_play);
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
	void onTransitionSyncText(sheepvswolves::GAME_INFO info, cocos2d::Vec2 pos,std::function<void()> call_back = nullptr);
	void readSyncText(sheepvswolves::GAME_INFO info,cocos2d::Vec2 pos, std::function<void()> call_back = nullptr);
	void initSheepItems();
	void onShowSheepFromLeftSide(std::function<void()> call_back = nullptr);
	void onShowSheepFromRightSide(std::function<void()> call_back= nullptr);
	void onClickSheepVsWolvesDelegate(cocos2d::Ref * sender) override;
	void onClickedBoardDelegate(cocos2d::Ref  *sender) override;
	void showFarm(float dt,std::function<void()> call_back = nullptr);
	void hideFarm(float dt, std::function<void()> call_back = nullptr);
	void setFencesSide(bool flipped, bool transition = true);
	void randomSheepSound();
	void learningReminder();
	void stopLearningReminder();
	void itemOverLay();
	void removeItemOverLay();

	std::vector<cocos2d::Vec2> getFrameLocations(cocos2d::Vec2 mid_point,cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding = Vec2::ZERO,cocos2d::Vec2 skew = Vec2::ZERO);

	std::vector<std::string> splitString(const std::string& s, char delimiter);
	
	
	std::vector<SheepVsWolves_Item *> m_sheep_items;
	std::vector<SheepVsWolves_Item *> m_sheep_left_items;
	std::vector<SheepVsWolves_Item *> m_sheep_right_items;
	std::vector<sheepvswolves::GAME_INFO> m_game_info;
	cocos2d::ui::Layout * m_layout_root{ nullptr };
	cocos2d::ui::Layout * m_layout_synctext{ nullptr };
	cocos2d::ui::Layout * m_layout_sheeps{ nullptr };
	cocos2d::ui::ImageView * m_img_bg{ nullptr };
	cocos2d::ui::ImageView * m_img_loa{ nullptr };
	cocos2d::ui::ImageView * m_img_bg_left{ nullptr };
	cocos2d::ui::ImageView * m_img_bg_right{ nullptr };
	int m_sheep_missing_left;
	int m_sheep_missing_right;

	int game_data_index{ 0 };
	SVWGameState m_game_state;
	SheepVsWolves_Board * m_board{ nullptr };
	GAFWrapper * m_gaf_fences{ nullptr };
	GAFWrapper * m_gaf_warehouse_front{ nullptr };
	GAFWrapper * m_gaf_warehouse_back{ nullptr };
	GAFWrapper * m_gaf_warehouse_door{ nullptr };
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";

	bool m_fences_flipped{ false };
	// auto play
	bool m_is_auto_play;
	void autoPlay(float dt);

	Button* _speaker;
	bool _flag = true;
	SheepVsWolves_Board* _board;
	float _audioSheep = -1;
	bool _scaleImg = true;
	float _sheepMove = 125;
	float _sheepChangeDistance = 0;
	float _audioSheepWrong = -1;
};

#endif // MJ_GAME_SHEEP_VS_WOLVES_H_
