#ifndef MJ_GAME_O_CHU_H__
#define MJ_GAME_O_CHU_H__ 

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "Math_Object.h"
#include "Math_Answer.h"
#include "Math_Slot.h"

using namespace ui;
USING_NS_CC;

enum class GameOChuState {
	GUIDE,
	SCRIPT_PLAY,
	WAITTING,
	START,
	PLAYING,
	ENDED
};
class GameOChu : public MJMainGBase
{
public:
	CREATE_FUNC(GameOChu);
	static GameOChu * createGame(std::string json_file = "config.json");
	GameOChu * createGame(std::string json_file, std::vector<math::resource::Image*> images);
private:

	struct GameOChuInfo {
		mj::PHONIC_INFO question;
		std::vector<mj::PHONIC_INFO> sync_text;
        std::vector<bool> is_drags;
		std::vector<mj::PHONIC_INFO> distracting;
		mj::PHONIC_INFO answer;
	};

	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image *> images);
	void didLoadFromCSB(std::string json_file);
	void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images);

	//callback Func
	void onMoveBegan(cocos2d::Node * node);
	void onMoveEnded(cocos2d::Node * node, cocos2d::Vec2 pos);
	bool isPointInnerSlot(Math_Slot * slot, cocos2d::Vec2 point, int padding);

	int count_wrong;
	void onShowGuideGame(float dt);

	void disableControlQuiz(float time = 0);
	void enableControlQuiz();

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
	void onPair(Math_Object * item, Math_Slot * slot);

	void zoomTo(Ref * sender, Vec2 position, float scale = 1.0f, float duration = 0.25f, std::function<void()> callback = nullptr);
	void playEndingScript();

	int m_game_index;
	int num_paired;
	GameOChuState m_game_state;
	std::vector<GameOChuInfo> m_game_data;
	GameOChuInfo m_current_data;
	std::vector<Math_Slot *> m_slots;
	std::vector<Math_Object *> m_items;
    std::vector<Math_Object *> m_items_no_drag;

	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_locked;
	cocos2d::ui::Text * m_lbl_question;
	cocos2d::ui::Button * m_btn_loa;
	cocos2d::ui::ImageView * m_bg_question;

	std::string font_name = "fonts/Montserrat-Medium.ttf";
    int q_id = -1;


	// auto play 
	void autoPlay();
};

CREATE_READER(GameOChu);

#endif // !MJ_GAME_O_CHU_H__
