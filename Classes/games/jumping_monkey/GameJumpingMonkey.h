
#ifndef MJGameJumpingMonkey_hpp
#define MJGameJumpingMonkey_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameJumpingMonkey_Item.h"
#include "GameJumpingMonkey_Food.h"
#include "GAFWrapper.h"
#include "MJBaseActivity.h"

class MJGameJumpingMonkey : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameJumpingMonkey_ItemDelegate, public MJGameJumpingMonkey_FoodDelegate  {
public:
    CREATE_FUNC(MJGameJumpingMonkey);
	static MJGameJumpingMonkey * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
    //bool game_play_enable = true;
    //void pauseGame();
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
    void onStartGame();
    void onFinishGame();
    void setInputConfigGame();
    void onSpeakerClicked(cocos2d::Ref* sender);
    void inActiveMode();
    void doAutoPlay();
    
    struct GameDataSet {
        std::vector<MJGameJumpingMonkey_Item::ItemInfo> list_objects;
        std::vector<MJGameJumpingMonkey_Item::ItemInfo> list_distracting;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
    };
    GameDataSet game_dataset;
    int object_index = 0;
    GAFWrapper *monkey = nullptr;
    std::vector<MJGameJumpingMonkey_Food *> list_food;
    void onClickFood(cocos2d::Ref *sender, bool is_answer) override;
    bool is_monkey_run_wrong = false, is_monkey_run_wright = true;
    cocos2d::ui::Layout *layout_content = nullptr;
    void monkeyFallGround();
    cocos2d::ui::ImageView *board_object = nullptr;
    void showBoardObject();
    void showItemAnswer(int type);
    void onClickItem(cocos2d::Ref *sender, int type) override;
    bool is_answer_enable = false;
    int audio_mc_id = 0;
    int audio_mc_id2 = 0;
    int max_turn_jumpingmonkey = 3;
    int max_item_jumpingmonkey = 4;
    std::vector<int> list_food_true;
    int _foodCorrectPossition ;
    MJGameJumpingMonkey_Food* _foodCorrect ;
    MJGameJumpingMonkey_Item* _itemCorrect;
    cocos2d::ui::Button* _speaker ;
    std::string speaker_audio_path;

    //--------------------
    EventListenerTouchOneByOne* listener;
    bool _paused = false;
    float countdown;
    int _foodIsShowing = true;
    int _questionType;
    bool _isSpeaking = true;
    enum _typeOfQuestion {
        Food = 0,
        Text = 1,
        Image = 2,
    };
};

CREATE_READER(MJGameJumpingMonkey);

#endif
