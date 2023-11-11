
#ifndef MJGameWaterSpray_hpp
#define MJGameWaterSpray_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameWaterSpray_Item.h"
#include "GAFWrapper.h"
#include "MJBaseActivity.h"

class MJGameWaterSpray : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameWaterSpray_ItemDelegate  {
public:
    CREATE_FUNC(MJGameWaterSpray);
	static MJGameWaterSpray * createGame(std::vector<math::resource::Image*> images);
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
    
    struct GameDataSet {
        std::vector<MJGameWaterSpray_Item::ItemInfo> list_objects;
        std::vector<MJGameWaterSpray_Item::ItemInfo> list_distracting;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
    };
    GameDataSet game_dataset;
    int object_index = 0;
    std::vector<MJGameWaterSpray_Item*> listAnswerObject;
    Layout *layout_content;
    std::vector<cocos2d::ui::ImageView *> list_vetban;
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    Vec2 pointTouch;
    std::vector<int> list_count_touch;
    void updateVetBan(float dt);
    int check_check = 255;
    GAFWrapper *phun_nuoc = nullptr, *head_monkey = nullptr, *finish_gaf = nullptr;
    Layout *phun_nuoc_layout, *tay_layout;
    float getAnglePhunNuoc();
    float getScalePhunNuoc();
    float witdh_phunnuoc = 0;
    int count_xoavetban_done = 0;
    int index_type_voinuoc = 3;
    cocos2d::ui::ImageView *img_object = nullptr;
    cocos2d::ui::ImageView* _imgFrames = nullptr;
    cocos2d::ui::Text *text_object = nullptr;
    cocos2d::ui::Button* _speaker;
    void showItemQuestion();
    void showItemAnswer(int type);
    void onClickItem(cocos2d::Ref *sender, int type) override;
    bool is_answer_enable = false;
    int sound_water_id = -1;
    bool is_begin = false, is_start_game = false;
    void cleanVetBanDone();
    int max_turn_water = 3;
    int max_item_water = 4;
    int _currentAudio = -1;
    int _tapOnAudio = -1;
    bool _isStain;
    void inActiveMode();
    void unscheduleInActiveMode();
};

CREATE_READER(MJGameWaterSpray);

#endif
