#ifndef MJ_GAME_PHAN_LOAI_H__
#define MJ_GAME_PHAN_LOAI_H__ 

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "GamePhanLoai_Item.h"
#include "GamePhanLoai_Slot.h"
#include "ParseJSONHelper.hpp"

using namespace ui;
USING_NS_CC;

enum class GamePhanLoaiState {
	GUIDE,
	SCRIPT_PLAY,
	WAITTING,
	START,
	PLAYING,
	ENDED
};
class GamePhanLoai : public MJMainGBase, public GamePhanLoai_SlotDelegate, public GamePhanLoai_ItemDelegate
{
public:
	CREATE_FUNC(GamePhanLoai);
	static GamePhanLoai * createGame();
	static GamePhanLoai * createGame(std::string json_file,std::vector<math::resource::Image *> images);
private:
	struct Type_Info
	{
		std::vector<int> iType;
		float duration;
		std::string text;
		std::string audio;
		std::string image_path;
		std::string background;
	};

	struct GamePhanLoaiInfo {
		std::vector<Type_Info> list_objects;
		std::vector<std::vector<Type_Info>> list_types;
		std::string audio_title;
		std::string audio_intro;
		int game_id;
		std::string title;
		std::string intro;
		std::vector<Type_Info> questions;
	};

	void onClickedSlotDelegate(cocos2d::Ref * sender) override;
	virtual void onClickedItem(cocos2d::Ref * sender) override;
	virtual void onMoveBeginItem(cocos2d::Ref * sender) override;
	virtual void onMoveEndedItem(cocos2d::Ref * sender,bool is_item_moved) override;
	
	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image *> images);
	void didLoadFromCSB();
	void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image *> images);

	void loadFrames();
	void showIntroGame();
	void scheduleSoundGuiding();
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
	void onPair(GamePhanLoai_Item * item, GamePhanLoai_Slot * slot);

	void zoomTo(Ref * sender,Vec2 position,float scale = 1.0f,float duration = 0.25f,std::function<void()> callback = nullptr);
	void playEndingScript();
	
	uint8_t slot_mode;
	int total_item;
	int m_game_index;
	int num_paired = 0;
	GamePhanLoaiState m_game_state;
	GamePhanLoaiInfo m_game_data;
	std::vector<GamePhanLoai_Slot *> m_slots;
	std::vector<GamePhanLoai_Item *> m_items;
	std::map<GamePhanLoai_Slot *, std::vector<Vec2> > m_slot_positions;
	std::map<int, std::string> m_slot_trace;
	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_locked;
	cocos2d::ui::Text * m_lbl_question;
	cocos2d::ui::Button * m_btn_loa;
	cocos2d::ui::ImageView * m_bg_question;

	std::string font_name = "fonts/Montserrat-Medium.ttf";
	std::string sound_question;
	std::string sound_item;
	std::string sound_slot;
    int main_audio_id = -1;
    bool is_ngang = false;
	// auto play 
	void autoPlay();
};

CREATE_READER(GamePhanLoai);

#endif // !MJ_GAME_PHAN_LOAI_H__
