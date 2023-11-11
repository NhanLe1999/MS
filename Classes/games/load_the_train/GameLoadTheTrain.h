
#ifndef GAME_LOAD_THE_TRAIN_HPP
#define GAME_LOAD_THE_TRAIN_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GameLoadTheTrain_Item.h"
#include "GameLoadTheTrain_ToaTau.h"
#include "BaseState.h"
#include "MJBaseActivity.h"

USING_NS_CC;
using namespace ui;

class GameLoadTheTrain : public ms::BaseActivity, public BaseState {
public:
    CREATE_FUNC(GameLoadTheTrain);
	static GameLoadTheTrain * createGame(std::vector<math::resource::Image*> images);
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
	void onLoadLevel();
    void setInputConfigGame();

	//can modify
	void onItemTouch(Ref* sender, Widget::TouchEventType evt, GameLoadTheTrain_Item* aItem);
	void showItem();
	void playItem(int curItem);
	void moveTheTrain(bool isMove);
	void updateLoadTheTrain(float delta);
	void moveHand();

	struct ToaData
	{
		std::string key;
		std::vector<std::string> listDistracting;
	};
	struct TrainData
	{
		std::string audio_path;
		std::string img_path;
		std::vector<ToaData> toas;
		std::vector<math::resource::Sync_Text> listTextHight;
	};

    struct GameDataSet {
        std::unordered_map<std::string,std::string> textAndAudioPathList;//("ba","sound/xxx/ba.mp3")
		std::vector<TrainData> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
		int max_leng;
    };
    GameDataSet game_dataset;
	std::vector<GameLoadTheTrain_ToaTau*> toas;
	std::vector<GameLoadTheTrain_Item*> items;
	std::vector<std::vector<std::string>> datas;

	GAFWrapper *train = nullptr;
	Sprite* hand = nullptr;
	float timeNoTouch = 0;
	bool onHandMove = false;
	int timeWrong = 0;

	int loa_id = 0;
	int audio_id = 0;
	int curText = 0;
	int curWord = 0;
	int curZorder = 1;
	float velocity = 0;

	float timeToPause = 0;
    bool is_show_pause_game = false;
    std::string question_vi = "";

	float timerCount = 0.0f;
	int currentGameSentenceAudioID = -1;
	float currentGameSentenceDuration = 0.0f;
};

CREATE_READER(GameLoadTheTrain);

#endif
