
#ifndef MJGameShark22_hpp
#define MJGameShark22_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameShark2_Fish.h"
#include "GAFWrapper.h"
#include "MJBaseActivity.h"

class MJGameShark2 : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameShark2_FishDelegate  {
public:
    CREATE_FUNC(MJGameShark2);
	static MJGameShark2 * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
    //bool game_play_enable = true;
    //void pauseGame();
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    static MJGameShark2* getInstance();
    static MJGameShark2* _mjShark2;
    Button* _speaker;
    void radio();
    void stopInActiveMode();
    void inActiveMode();
    void stopRadio();
    float audio_id = -1;
    float audio_id_text = -1;
    float _radio = -1;
    float _radio2 = -1;
    Image* root_layout;
    Size screen_size;
    std::vector<Layout*> stars;
    int total_count = 0;
    GAFWrapper* gaf_fish = nullptr;
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
	void onLoadLevel();
	void onFinishLevel();
    void onStartGame();
    void onFinishGame();
    void setInputConfigGame();
	void updateCallFish(float dt);
    
    struct SharkDataSet {
        std::vector<MJGameShark2_Fish::FishInfo> list_objects;
		std::vector<std::pair<std::string,std::string>> list_texts;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
        std::string hoc_am_hoc_van;
    };
    SharkDataSet game_dataset;
    int object_index = 0, count_fish_tap = 0;
	std::vector<MJGameShark2_Fish*> sharks;
	Vector<MJGameShark2_Fish*> list_fishs;
	MJGameShark2_Fish* finishFish;
	std::vector<std::vector<Vec2>> listPosMove_;
	std::vector<float> time_call_fishs;
	std::vector<int> list_text;
    
    void onFish(cocos2d::Ref *sender) override;
    void onSharkRun(cocos2d::Ref *sender) override;
	void resetText(cocos2d::Ref *sender) override;
    bool isPlay = false;
	int max_turn = 3;
	int max_item = 3;
	int max_fish_on_screen = 5;
	int zOrderShark = 100;
};

CREATE_READER(MJGameShark2);

#endif
