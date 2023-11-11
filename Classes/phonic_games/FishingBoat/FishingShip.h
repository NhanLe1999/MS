
#ifndef FishingShip_HPP
#define FishingShip_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "FishingShip_Item.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class FishingShip : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public FishingShip_ItemDelegate {
public:
    CREATE_FUNC(FishingShip);
    static FishingShip * createGame(std::string json_file, bool isAutoPlay);
    static FishingShip * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
    void generateData(rapidjson::Value value_data);
private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
    
    struct AUDIO_PHONIC_BA{
        std::string audio;
        std::string s_audio;
        std::vector<std::string> p_audio;

    };
    struct PHONIC_INFO_BA {
        std::string text_full;
        std::vector<std::string> text_phonic;
        AUDIO_PHONIC_BA audio_phonic;
        std::string audio_full;
        std::string image_path;
        std::string video_path;
        std::vector<int> highlights;
        cocos2d::Color3B normal_color, highlight_color;
    };
    
    std::vector<PHONIC_INFO_BA> _list_phonic_info_ba;
    
    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;
    
    std::vector<std::vector<int>> list_index;
    
    void loadFrames();
    void onEnter() override;
    void onExitTransitionDidStart() override;
//    void onRefresh();
    void onClickedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type) override;
    
    void update(float dt = 1.0f ) override;
    void onAutoPlay();
    void autoPlayRun(float dt);
    void onFinishGame() ;
    
//    void showIntroGame();
    void showTapGuideGame( std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void inActiveMode();
    void onNextStage();
    
    void nextGame();
    void showGame();
    
    FishingShip_Item* createFish();
    void moveFish(Node* fish, float fish_speed, bool down = false );
    void removeFish(Node* fish);
    void returnFish(Node* fish, float pos_y, bool down = false);
  
    void scheduleFish(float time);
    
    void hideHint();
    void phaoHoa(int shadow_index);
    
    void readPhonicText(Text* text);
    void readSlowText(Text* text);
    void readNormalText(Text* text);

    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_size = 150;
    std::string type_game;
    
    int shadow = 0;
    int fish_count;
    int wrong_count;
    int max_game = 4; //configurable
    int count_game = 0;
    
    int wrong = 3;
    
    int index = 0;
    int img_audio_id = -1;
    float f_speed ;
    float f_speeds [4] = { 0.0055f, 0.0045f, 0.0035f, 0.0025f };
    float f_times [4] = { 1.6f, 1.4f, 1.2f, 0.9f };
    int f_i = 2 ;
    
    int timer =0;
    
     bool _is_guide_showed = false;
    //layout
    Layout* water_layout;
    Layout* ship_layout;
    Layout* root_layout;
    Layout* board_layout;
    
    GAFWrapper* water_boom;
    float water_pos_y;

    //vector
    std::vector<ImageView*> shadows;
    std::vector<mj::PHONIC_INFO> w_phonic;
    std::vector<mj::PHONIC_INFO> w_phonic_copy;
    
    //autoPlay
    int testing_step = 1;
    std::vector<cocos2d::ui::Layout*> monkeys;
    bool autoPlay = false;

	Button* speaker;
    ImageView* _imagePhonic;
    bool _flag = true;
    bool _flag2 = true;

    void stopInActiveMode();
    void itemOverLay();
    void removeItemOverLay();
    void onClickedSpeaker(cocos2d::Ref* sender);
    void stopAudioSpeaker();
    void stopAudioImage();
};

CREATE_READER(FishingShip);

#endif
