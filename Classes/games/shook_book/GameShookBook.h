
#ifndef GAME_SHOOK_BOOK_HPP
#define GAME_SHOOK_BOOK_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "BaseState.h"
#include "MJBaseActivity.h"

USING_NS_CC;
using namespace ui;

class GameShookBook : public ms::BaseActivity, public BaseState {
public:
    CREATE_FUNC(GameShookBook);
	static GameShookBook * createGame(std::vector<math::resource::Image*> images);
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
	void hideDistractingItem(int i);
    void hideTapGuideGame();
	void showCorrectTextResult(int &level);
	void creatLabelItem(int index);
	struct ItemInfo {
		std::string text;
		std::string audio_path;
		std::string img_path;
	};
	struct LevelData
	{
		std::string intro;
		std::string audio_quest;
		ItemInfo true_ans;
		ItemInfo false_ans;
	};
    struct GameDataSet {
		std::vector<LevelData> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
		int repeat_duration;
    };
    GameDataSet game_dataset;

	GAFWrapper *monkey = nullptr;

	std::vector<ItemInfo> item_infos;
	std::vector<ImageView*> img_items;
	
	int curText = 0;
	bool isReadingSound = false;
	int curZorder = 0;
	float timeToPause = 0;
    void inActiveMode();
};

CREATE_READER(GameShookBook);

#endif
