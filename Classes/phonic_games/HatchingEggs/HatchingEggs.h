
#ifndef HatchingEggs_HPP
#define HatchingEggs_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "HatchingEggs_Item.h"
#include "PhonicInfoModel.h"

USING_NS_CC;
using namespace ui;

class HatchingEggs : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public HatchingEggs_ItemDelegate {
public:
    CREATE_FUNC(HatchingEggs);
    static HatchingEggs * createGame(std::string json_file, bool isAutoPlay);
    static HatchingEggs * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
    
    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;
    std::vector< std::vector<mj::PHONIC_INFO>> _list_phonic_info;
    std::vector<mj::PHONIC_INFO> _sublist_phonic_info;
    std::vector<int> q_id;
    
    
    void loadFrames();
    void onEnter() override;
    void onExitTransitionDidStart() override;
//    void onRefresh();
    void onClickedDelegate(cocos2d::Ref *sender) override;
    
    void onAutoPlay();
    void autoPlayRun(float dt);
    void onFinishGame();
    void inActiveMode();
    void onNextStage();
    void onBoardClicked(Ref* sender);
    void nextGame();
    void showGame();
    void stopAudioAnswer();
    void stopInActiveMode();
    
    std::string font_name = "fonts/Linotte Regular.otf";
    std::string type_game;
    
    int number_egg = 4;
    int number_chick; //co the config so luong tu moi man choi
    int count_game = 0;
   
    ui::Text* board_text;

    ImageView* _img = nullptr;

    ui::Layout* chicken_root;
    
    std::vector<HatchingEggs_Item*> chickens;
    std::vector<GAFWrapper*> eggs;
    string _text;
    std::string right_audio;
    std::string right_phonic;
    Vec2 start_pos;
    //autoPlay
    int testing_step = 1;
    std::vector<cocos2d::ui::Layout*> monkeys;
    bool autoPlay = false;
    std::string question_vi;

    Button* _speaker;
    bool _flag = true;
    Layout* _board;
    float _audio = -1;
    struct LevelData {
        string ansImage;
        string ansAudio;
    };

    vector<LevelData> _totalLevels;

    int _countImg = 0;

    bool _configImg = false;

    float _audioImg = -1;

    Sprite* _backgoundGame = nullptr;

    Sprite* _backgound = nullptr;
};

CREATE_READER(HatchingEggs);

#endif
