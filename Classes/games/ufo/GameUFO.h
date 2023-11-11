
#ifndef GAME_UFO_HPP
#define GAME_UFO_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GameUFO_Item.h"
#include "BaseState.h"
#include "MJBaseActivity.h"

enum UFO_GAF_STATE
{
	UFO_GAF_STATE_NONE = -1,
	UFO_GAF_STATE_INTRO,
	UFO_GAF_STATE_PHASE1,
	UFO_GAF_STATE_PHASE2,
	UFO_GAF_STATE_PHASE3,
	UFO_GAF_STATE_PHASE4,
	UFO_GAF_STATE_PHASE5,
	UFO_GAF_STATE_IDLE,
	UFO_GAF_STATE_WIN,
	UFO_GAF_STATE_END,
	UFO_GAF_STATE_SHOT_LAZER
};

class GameUFO : public ms::BaseActivity, public BaseState {
public:
    CREATE_FUNC(GameUFO);
	static GameUFO * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();

    void onRefresh(cocos2d::Ref * sender);
	void showTutorial(float dt);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
	void onPreGame();
    void onStartGame();
    void onFinishGame();
	void onFinishLevel();
    void setInputConfigGame();

	//can modify
	void playUfo(UFO_GAF_STATE ufo_state);
	void updateTimeNoTap(float dt);

    struct GameDataSet {
		std::vector<GameUFO_Item::ItemInfo> list_objects;
		std::vector<std::string> list_distractings;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
    };
    GameDataSet game_dataset;

	GAFWrapper *ufo = nullptr;
	std::vector<GameUFO_Item::ItemInfo> list_words;

	std::vector<GameUFO_Item*> listItems;
	float timeNoTap = 0;
	UFO_GAF_STATE ufo_state;
	int curText = 0;
	float timeToPause = 0;
	int phase = 1;
	int audio_id = 0;
	bool flag;
	cocos2d::ui::Button* _speaker;
};

CREATE_READER(GameUFO);

#endif
