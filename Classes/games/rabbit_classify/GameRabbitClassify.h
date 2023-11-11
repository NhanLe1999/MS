
#ifndef MJGameSample_hpp
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "GameRabbitClassify_Light.h"
#include "GameRabbitClassify_Item.h"


#include "MJBaseActivity.h"

enum BUNNY_STATE
{
	BS_NONE = -1,
	BS_IDLE,
	BS_WIN,
	BS_LOSE
};

enum GAME_RABBIT_CLASSIFY_STATE
{
	GRCS_NONE = -1,
	GRCS_PAUSE_TIME,
	GRCS_PLAYING_TIME,
	GRCS_END_TIME
};

class GameRabbitClassify : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameSample_ItemDelegate  {
public:
    CREATE_FUNC(GameRabbitClassify);
	static GameRabbitClassify * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();
    
    void showGuide();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

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
	void updateRabbitClassify(float dt);
	void updatePosItemForStart();
	cocos2d::Vec2 getMinPos(bool isDragFail = false);
	void updatePosAItem(GameRabbitClassify_Item* aItem);
	void onItemTouch(Ref* aItem, Widget::TouchEventType evt);
	void placeTrueItem(GameRabbitClassify_Item* aItem, Layout* boxContain);
	
	struct Type_Info
	{
		int iType;
		float duration;
		std::string text;
		std::string audio;
	};

    struct GameDataSet {
        std::vector<GameRabbitClassify_Item::ItemInfo> list_objects;
		std::vector<std::vector<Type_Info>> list_types;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
		std::vector<std::string> audio_mc_path;
        bool read_box_name;
    };
    GameDataSet game_dataset;
	GAFWrapper *bunny = nullptr;
	GAFWrapper* _bling = nullptr;
	void playBunny(BUNNY_STATE bs);

	cocos2d::Vector<GameRabbitClassify_Item*> list_items;
	cocos2d::Vector<GameRabbitClassify_Light*> list_lights;
	cocos2d::Vector<Layout*> list_boxs;
	GameRabbitClassify_Item* _item;
	std::string _reminderAudio = "";

	
	cocos2d::Vec2 touchLocation = cocos2d::Vec2(0, 0);
	int numLight = 0;
	int curLevel = 0;
	int curZ = 10;
	float posLightOff = 0;
	float posLightOn = 0;
    int object_index = 0;
	float topPanelPosY = 0;
	int timeToPause = 0;
	float time_pause = 0;
	GAME_RABBIT_CLASSIFY_STATE game_state = GRCS_NONE;
    
    bool _isGuideShowing = false;
	bool _isSpeaking = false;
    void hideTapGuideGame();
	void showGuideGame();
    int q_id = -1;
	int _countGuide = 0;
};

CREATE_READER(GameRabbitClassify);

#endif
