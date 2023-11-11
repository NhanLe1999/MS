#ifndef MJ_GAME_SAP_XEP_H__
#define MJ_GAME_SAP_XEP_H__ 

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "GameSapXep_Item.h"
#include "GameSapXep_Slot.h"


using namespace ui;
USING_NS_CC;

enum class GameSapXepState {
	GUIDE,
	SCRIPT_PLAY,
	WAITTING,
	START,
	PLAYING,
	ENDED
};
class GameSapXep : public MJMainGBase, public GameSapXep_SlotDelegate, public GameSapXep_ItemDelegate
{
public:
	CREATE_FUNC(GameSapXep);
	static GameSapXep * createGame(std::string json_file = "config.json");
	static GameSapXep * createGame(std::string json_file,std::vector<math::resource::Image *> images);
private:
	struct GameSapXepInfo {
		mj::PHONIC_INFO question;
		std::vector<mj::PHONIC_INFO> answer;
	};
	void onClickedSlotDelegate(cocos2d::Ref * sender) override;
	virtual void onClickedItem(cocos2d::Ref * sender) override;
	virtual void onMoveBeginItem(cocos2d::Ref * sender) override;
	virtual void onMoveEndedItem(cocos2d::Ref * sender,bool is_item_moved) override;
	
	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image *> images);
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image *> images);

	void loadFrames();
	void showIntroGame();
	void startGame();
	void onFinishGameVMonkey();
	void onRefreshGame();
	void update(float dt) override;
	void onEnter() override;
	void onNextGame();

	void onExitTransitionDidStart() override;
	void onExit() override;

	void setTouchLock(bool locked);
	void onClickedSpeaker(Ref * sender);

	void initQuestion();
	void showItemsAndSlots();
	void inActiveMode();
	void onPair(GameSapXep_Item * item, GameSapXep_Slot * slot);

	void zoomTo(Ref * sender,Vec2 position,float scale = 1.0f,float duration = 0.25f,std::function<void()> callback = nullptr);
	void playEndingScript();
	void playSequence(int index = 0,std::function<void()> callback = nullptr);

	ActionInterval* getAction(std::vector<std::pair<float, Vec2>> listRotateAndPosition, float time);
	void showSlots();
	void shufflingItem();
	int m_game_index;
	int num_paired;
	GameSapXepState m_game_state;
	GameSapXepInfo m_current_data;
	std::vector<GameSapXepInfo> m_game_data;
	std::vector<GameSapXep_Slot *> m_slots;
	std::vector<GameSapXep_Item *> m_items;
	std::vector<Vec2> listPosSlots;
	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_locked;
	cocos2d::ui::Text * m_lbl_question;
	cocos2d::ui::Button * m_btn_loa;
	cocos2d::ui::ImageView * m_bg_question;
	cocos2d::ui::ImageView* buttonPlay;
	cocos2d::ui::ImageView* buttonBGNext;
	cocos2d::ui::ImageView* buttonNext;

	void showInformationGame();

	void showTutorial(float dt);

	void showGuide(Point start, Point stop);

	void stopGuiding();

	void buttonPlayClick();

	void buttonNextClick();

	std::string font_name = "fonts/Montserrat-Medium.ttf";

	std::string sound_item;
	std::string sound_question;

	// auto play 
	void autoPlay();
};

CREATE_READER(GameSapXep);

#endif // !MJ_GAME_SAP_XEP_H__
