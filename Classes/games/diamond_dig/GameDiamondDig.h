
#ifndef GAME_DIAMOND_DIG_HPP
#define GAME_DIAMOND_DIG_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GameDiamondDig_Item.h"
#include "BaseState.h"

USING_NS_CC;
using namespace ui;

class GameDiamondDig : public ms::BaseActivity, public BaseState {
public:
    CREATE_FUNC(GameDiamondDig);
	static GameDiamondDig * createGame(std::vector<math::resource::Image*> images);
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
	void callTextPreGame(int index = 0);
	void digTrueItem(GameDiamondDig_Item* sender);
    void inActiveMode();
	void showGuiding();

	struct DiamondData
	{
		std::string img_path;
        std::string audio_path;
		std::vector<GameDiamondDig_Item::ItemInfo> word_infos;
	};

    struct GameDataSet {
		std::vector<DiamondData> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
    };
    GameDataSet game_dataset;

	Node* nodeStone;

	GAFWrapper* stone = nullptr;
	GAFWrapper *riu = nullptr;
	Vector<GameDiamondDig_Item*> word_items;

	
	int curText = 0;
	int curZorder = 0;
	float timeToPause = 0;
    int audio_question_id = 0;
    bool _is_guide_showed = false;
	bool _speakable= true;
	int _audioReminder;
};

CREATE_READER(GameDiamondDig);

#endif
