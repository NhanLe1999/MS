
#ifndef MJGameShark_hpp
#define MJGameShark_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameShark_Fish.h"
#include "GAFWrapper.h"
#include "MJBaseActivity.h"

class MJGameShark : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameShark_FishDelegate  {
public:
    CREATE_FUNC(MJGameShark);
	static MJGameShark * createGame(std::vector<math::resource::Image*> images);
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
    
    struct SharkDataSet {
        std::vector<MJGameShark_Fish::FishInfo> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
    };
    SharkDataSet game_dataset;
    int object_index = 0, count_fish_tap = 0;
    MJGameShark_Fish *shark;
	Vector<MJGameShark_Fish*> list_fishs;
    
    void onFish(cocos2d::Ref *sender) override;
    void onSharkRun(cocos2d::Ref *sender) override;
    bool isPlay = false;
	int max_turn = 3;
	int max_item = 3;
	int max_fish_on_screen = 5;
};

CREATE_READER(MJGameShark);

#endif
