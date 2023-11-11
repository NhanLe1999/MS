#ifndef WordWheel_HPP
#define WordWheel_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "WordWheel_Item.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class WordWheel : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public WordWheel_ItemDelegate {
public:
    CREATE_FUNC(WordWheel);
    static WordWheel * createGame(std::string json_file, bool isAutoPlay);
    static WordWheel * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    std::string question_vi;
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
    
    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;
    std::vector<mj::PHONIC_INFO> _list_phonic_info;
   
    struct WWGAME{
        std::string type;
		mj::PHONIC_INFO correct_word;
        std::vector<mj::PHONIC_INFO> game;
    };
    std::vector<WWGAME> data;
    
    void loadFrames();
    void onEnter() override;
    void onExitTransitionDidStart() override;
//    void onRefresh();
    void onTouchedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type ) override;
    
    void onAutoPlay();
    void autoPlayRun(float dt);
    void onFinishGame();
    bool checkDrop(WordWheel_Item* item);   
    void onNextStage();
    
    void nextGame();
    void showGame();
    void showHint();
    
    void showTapGuideGame(float delay_time = 6.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
//    void hideTapGuideGame();
    
    void inActiveMode(float delay_time = 7.0f);
    
    void enabledAllItems(bool _lock);
    void onTvTouched(Ref* sender, ui::Widget::TouchEventType type);
    std::string font_name = "fonts/Linotte Regular.ttf";
    std::string type_game;
    
    std::vector<Vec2> calculate_pos(int number_point, int r);
    float moveTime(WordWheel_Item* item, WordWheel_Item* item_clone);
    float calculateAngle(Vec2 pos, Vec2 center);
    int number_bubble = 5;  //config so luong dap an
    int max_game = 4;       //confic so lan choi
    int count_game = 0;
    int count_wrong = 0;
//    Vec2 pos_guide;
    
    ui::Layout* left_panel;
    ui::Layout* root_panel;
    Layout* item_panel;
    Node* energyBar;
    
    GAFWrapper* character;
    GAFWrapper* holder;
    GAFWrapper* bolt;
    
    std::vector<WordWheel_Item*> items;
    WordWheel_Item* right_item;
    string _text;
    
    std::vector<Vec2> pos_list;
    std::vector<float> angles;
    int bankinh = 250;          // khoang cach giua tam va dap an
    int angle;
    Vec2 holder_pos;
    std::string txt_full;
    std::string txt_phonic;
    
    WordWheel_Item* item_cloned;
    Vec2 start_pos;

    mj::PHONIC_INFO phonic;
    
    //autoPlay
    int testing_step = 1;
    bool autoPlay = false;
    

	// speaker
	cocos2d::ui::Button *speaker;
	std::string speaker_audio_path;
	void onSpeakerClicked(cocos2d::Ref * sender);

    void learningReminder(float delay_time);
    void stopLearningReminder();
    void stopAudioSpeaker();
    void itemOverLay();
    void removeItemOverLay();
    bool flag = true;
    float _audio = -1;
    float _audioSpeaker = -1;
    float _audioGuideGame = -1;
};

CREATE_READER(WordWheel);

#endif
