
#ifndef GAME_PLUG_IN_BULB2_HPP
#define GAME_PLUG_IN_BULB2_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GamePlugInBulb2_Item.h"
#include "BaseState.h"
#include "MJBaseActivity.h"

USING_NS_CC;
using namespace ui;

class GamePlugInBulb2 : public ms::BaseActivity, public BaseState  {
public:
    CREATE_FUNC(GamePlugInBulb2);
	static GamePlugInBulb2 * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
	void inActiveMode(Point start, Point stop);
	void stopInActiveMode();
	void showGuide(cocos2d::Point, cocos2d::Point);
	void learningReminder();
	void stopLearningReminder();
	void stopAudioIntro();
	void audioSpeaker();
	void stopAudioSpeaker();
	Point start;
	Point stop;
	bool _flag = true;
	bool _flagAnswer = true;
	int answer = 0;
	int answerCorrect = 0;
	Button* _speaker;
	//bool flag = true;
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
	void onPreGame();
	void onLoadLevel();
    void onStartGame();
    void onFinishGame();
	void onFinishLevel();
    void setInputConfigGame();
	void itemOverLay();
	void removeItemOverLay();

	//can modify

	struct PlugData
	{
		int pType;
		std::vector<GamePlugInBulb2_Item::ItemInfo> items;
		std::vector<GamePlugInBulb2_Item::ItemInfo> item2;
	};

    struct GameDataSet {
		std::vector<PlugData> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
    };
    GameDataSet game_dataset;

	std::vector<GamePlugInBulb2_Item*> left_items;
	std::vector<GamePlugInBulb2_Item*> right_items;
	
	int curText = 0;
	int curZorder = 0;
	float timeToPause = 0;
	int connect_true = 0;
	int audio_id = 0;
	bool flag = true;
};

CREATE_READER(GamePlugInBulb2);

#endif
