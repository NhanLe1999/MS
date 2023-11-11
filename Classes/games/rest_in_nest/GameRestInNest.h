
#ifndef GAME_REST_IN_NEST_HPP
#define GAME_REST_IN_NEST_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GameRestInNest_Item.h"
#include "BaseState.h"
#include "MJBaseActivity.h"
#include "StoryConfigManager.h"
USING_NS_CC;
using namespace ui;

enum REST_IN_NEST_GAME_STATE
{
	RIN_GS_NONE = -1,
	RIN_GS_PLAYING,
	RIN_GS_PAUSE,
	RIN_GS_PLAY_VIDEO,
	RIN_GS_PRE_GAME,
	RIN_GS_PRE_FINISH,
	RIN_GS_FINISH
};

class GameRestInNest : public ms::BaseActivity, public BaseState  {
public:
    CREATE_FUNC(GameRestInNest);
	static GameRestInNest * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
	void onPreGame();
    void onStartGame();
    void onFinishGame();
	void onFinishLevel();
    void setInputConfigGame();

	//can modify
	void updateRestInNest(float delta);
	void clickTrueItem(GameRestInNest_Item* aItem, ImageView* aImg);
	float getAngelObjectPlayer(Vec2 pos_objectplayer, Vec2 pos_to);
	void showTextPreGame(GameRestInNest_Item::ItemInfo a_info);
	void playKien(GAFWrapper* kien);
    void setCallbackKienRunAction();

	struct MainTextData
	{
		std::string text;
		std::string audio_path;
	};

    struct GameDataSet {
		std::vector<MainTextData> list_main_texts;
		std::vector<std::string> list_texts;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
        std::string hoc_am_hoc_van;
    };
    GameDataSet game_dataset;

	GAFWrapper *bird = nullptr;
	std::vector<GAFWrapper*> kiens;
	std::vector<GameRestInNest_Item::ItemInfo> item_infos;
	std::vector<GameRestInNest_Item*> rest_items;
	Vector<ImageView*> bgs;
	REST_IN_NEST_GAME_STATE game_state_rin = REST_IN_NEST_GAME_STATE::RIN_GS_NONE;

	Vec2 posBird;
	int curText = 0;
	int curTime = 0;
	int totalTime = 0;
	int item_per_level = 5;
    std::function<void(GAFWrapper* kien)> _callBackKienRunAction;

	int audio_id = 0;

    void inActiveMode();
};

CREATE_READER(GameRestInNest);

#endif
