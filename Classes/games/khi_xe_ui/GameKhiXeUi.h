
#ifndef GAME_KHI_XE_UI_HPP
#define GAME_KHI_XE_UI_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GameKhiXeUi_Item.h"
#include "BaseState.h"
#include "StoryConfigManager.h"
#include "Item.h"
#include "MJMainGBase.h"

enum MONKEY_STATE
{
	MS_NONE = -1,
	MS_IDLE,
	MS_RUN,
	MS_DESTROY,
	MS_WRONG
};

class GameKhiXeUi : public ms::BaseActivity, public BaseState{
public:
    CREATE_FUNC(GameKhiXeUi);
	static GameKhiXeUi * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
	void onFinishGame();
	void onPreGame();
    void onStartGame();
	void onFinishLevel();
	void onLoadLevel();
    void setInputConfigGame();

	//can modify
	void updateKhiXeUi(float dt);
	void playMonkey(MONKEY_STATE ms, int posDestroy = 0 ,const std::function<void()> aFunc = nullptr);
	void callAText();
	void crashATrueItem();
	void PlayChooseItem(Item* chooseItem = NULL, std::string audio_path = "");
	void showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop = 100, float delay_time = 0.5f, float dt = 4.5f, std::string gaf_guide_path = "gaf/guidegame/hand_tap/hand_tap.gaf", cocos2d::ui::Layout* parentLayout = NULL);
	void onItemClicked(Ref* sender);
	void doAutoPlay();

	struct Type_Info
	{
		int iType;
		float duration;
		std::string text;
	};

    struct GameDataSet {
        std::vector<GameKhiXeUi_Item::ItemInfo> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
        std::string hoc_am_hoc_van;
		int maxNumItem;
    };
    GameDataSet game_dataset;

	struct PlayData
	{
		float timeFromCall = 0;
		int timeWrong = 0;
	};

	bool is_phonic_lesson;

	PlayData game_playData;

	GAFWrapper *monkey = nullptr;
	GAFWrapper *house_gaf = nullptr;
	cocos2d::ui::ImageView* imgTextBuilding = nullptr;
	std::vector<GameKhiXeUi_Item::ItemInfo> text_call;
	GameKhiXeUi_Item* house_item = nullptr;
	std::vector<GameKhiXeUi_Item*> _listItem;
	
	int curText = 0;
	int curAudio = 0;
	cocos2d::Vec2 posMonkeyStop = cocos2d::Vec2(0,0);

	bool _speakable = false;
	bool _click_wrongItem = false;
	cocos2d::ui::Button* _speakerBtn;
};

CREATE_READER(GameKhiXeUi);

#endif
