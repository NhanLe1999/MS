
#ifndef GAME_XEP_CHU_HPP
#define GAME_XEP_CHU_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "Gamexepchu_Item.h"
#include "BaseState.h"
#include "MJBaseActivity.h"

class GameXepChu : public ms::BaseActivity, public BaseState  {
public:
    CREATE_FUNC(GameXepChu);
	static GameXepChu * createGame(std::vector<math::resource::Image*> images);
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
	void monkeyJump();
	void softItem();
	void wordFlyToSky(GameXepChu_Item* aItem);
	void onItemTouch(Ref* aItem, Widget::TouchEventType evt);
	void placeTrueItem(GameXepChu_Item* aItem, GameXepChu_Item* trueItem, float timeDelay);
	void checkFinsh();

	struct XepChuData
	{
		std::string audio_path;
		int numItemFly;
		std::vector<GameXepChu_Item::ItemInfo> word_infos;
		std::vector<math::resource::Sync_Text> _listTextHight;
	};

    struct GameDataSet {
        std::vector<XepChuData> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		int number_level;
    };
    GameDataSet game_dataset;

	GAFWrapper *monkey = nullptr;
	Vector<GameXepChu_Item*> word_items;
	Vector<GameXepChu_Item*> _wordItemsCopy = {};
	Vector<GameXepChu_Item*> flying_items;

	std::vector<float> posY;
	std::vector<Vec2> posFlyingItems;
	std::vector<Color4B> color4bs;
	std::vector<float> _listTimeDelay = {};
	
	int audio_id = 0;
	int curText = 0;
	int curZorder = 1;

	float timeToPause = 0;
    
    void showGuide(cocos2d::Point, cocos2d::Point);
	void addSpeaker();
	void handSuggetionGame(float delayTime = 8.5f);
	void runActionSuggetion(Point start, Point stop, float timeDelay = 8.5f);
    
    bool _is_guide_showed;
	Vector<FiniteTimeAction*> _actions;
	std::string _pathAudioIntro = "";
	bool _isPlayTextItem = true;
	bool _isPlayAudio = true;
	bool _isRunActionHand = true;
	int _age = 0;
};

CREATE_READER(GameXepChu);

#endif
