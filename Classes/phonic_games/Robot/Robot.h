
#ifndef Robot_HPP
#define Robot_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "Robot_Item.h"
#include "PhonicInfoModel.h"
#include "GameData.h"
#include "StoryConfigManager.h"
USING_NS_CC;
using namespace ui;

class Robot : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public Robot_ItemDelegate {
public:
    CREATE_FUNC(Robot);
    static Robot * createGame(std::string json_file, bool isAutoPlay);
    
//    static Robot * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);

    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    void onExit() override;
private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
//    void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images, bool isAutoPlay);
//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
   
    std::vector<Robot_Item::Phonic> phonics;
    std::vector<std::vector<Robot_Item::Phonic>> data;
    
    void loadFrames();
    void onEnter() override;
    void onExitTransitionDidStart() override;
//    void onRefresh();
    void onSpeakerClicked(Ref* sender);
    void onClickedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type, int item_pair, std::string audio_path) override;
    
//    void update(float dt = 1.0f ) override;
    void onAutoPlay();
    void autoPlayRun(float dt);
    void onFinishGame() ;
    
//    void showIntroGame();
    void showTapGuideGame(float delay_time = 1.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void hideTapGuideGame();
    void inActiveMode(float delay_time);
    void onNextStage();
    
    void nextGame();
    void showGame();

    
    void right_drag(Robot_Item* item);
    void wrong_drag(Robot_Item* item, Vec2 old_pos, bool inbox);
    
    void showHint();
    void hideHint();
    
//    void getPhonicList(std::string json_file = "phonic.json");
//    std::vector<std::string> _phonic_list;
//    void wordToPhonic(std::string word);
    int getItemFontSize();
    
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_size = 80;
    std::string type_game;
    
    
    int game_mode = 0;
    int wrong_count =0;
    int right_count =0;
    int max_game = 5; //configurable
    int count_game = 0;
    int q_id = 0; //question id
    
    int wrong = 3;
    int right_ans =0;
    std::string right_audio;
	int right_pair; // dung de xet cac am giong nhau
    int q_audio_id = -1;

    Layout* box;
    Layout* root_layout;
    Layout* item_layout;
    Button* loa;
    
    GAFWrapper* robot;
    GAFWrapper* robot_right;
    GAFWrapper* robot_win;
    //vector
    std::vector<Robot_Item*> items;
    Vec2 old_pos ;

    int testing_step = 1;
    std::vector<cocos2d::ui::Layout*> monkeys;
    bool autoPlay = false;
	bool is_word; // phan biet choi word hay sound
    std::string hoc_am_hoc_van = "";

    int _intro = 0;
    bool _flag = true;
    bool _flag2 = true;
    float _age_check = 3.5;
    void stopInActiveMode();
    void stopAudioSpeaker();
    void itemOverLay();
    void removeItemOverLay();
};

CREATE_READER(Robot);

#endif
