//
//  ImmortalCircle.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 9/18/18.
//

#ifndef ImmortalCircle_hpp
#define ImmortalCircle_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "GameData.h"
#include "HSAudioEngine.h"
//#include "ImmortalCircle_Item.h"
//#include "../external/gaf/Library/Sources/GAFAsset.h"
//#include "Math_Libs.h"

USING_NS_CC;
using namespace ui;

class ImmortalCircle : public MJMainGBase {
public:
    CREATE_FUNC(ImmortalCircle);
    static ImmortalCircle * createGame(std::string json_file, bool isAutoPlay = false);
    static ImmortalCircle * createGame();
    
private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
    void didLoadFromCSB();
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images = {} );
    void onEnter() override;
    void gameConfig();
    
    void loadFrame();
    void onGameStart();
    void onFinishGame() override;
    void onExitTransitionDidStart() override;
    
    void createShark();
    void showGuideGame();
    void onClicked(Ref* sender);
    
    std::vector<Vec2> attack_start_pos;
    std::vector<Vec2> attack_end_pos;
    std::vector<Vec2> not_attack_start_pos;
    std::vector<Vec2> not_attack_end_pos;
    
    std::vector<mj::GameObject> _word_list;
    
    Layout* root_layout;
    Node* base_flag;
    GAFWrapper* back_solider;
    GAFWrapper* front_solider;
    Button* speaker;
    Node* circle ;
//    gaf::GAFAsset* shark_asset;
//    gaf::GAFAsset* seal_asset;
    
    mj::GameObject _word;
    std::vector<Vec2> goal_list;
    
    int count_tap = 0;
    int max_tap = 10;
    
    int count_game = 0;
    int max_game = 3;
    
    float delay_time = 3;
    float timer = 0;

    Size screen_size;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 70;
    bool autoPlay = false;
};
CREATE_READER(ImmortalCircle);

#endif /* ImmortalCircle_hpp */
