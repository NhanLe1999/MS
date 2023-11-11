//
//  CaterpillaRacing.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/16/18.
//

#ifndef CaterpillaRacing_hpp
#define CaterpillaRacing_hpp


#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "CaterpillaRacing_Item.h"
//#include "BackgroundNode.h"
#include "MJHelper.h"

USING_NS_CC;
using namespace ui;

class CaterpillaRacing : public MJMainGBase, public CaterpillaRacing_ItemDelegate {
public:
    CREATE_FUNC(CaterpillaRacing);
//    static CaterpillaRacing * createGame(std::string json_file, std::vector<math::resource::Image*> images, bool isAutoPlay);
    static CaterpillaRacing * createGame(std::string json_file, bool isAutoPlay);
    
private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
//     void didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images, bool isAutoPlay);
    void loadFrame();
    void onEnter() override ;
    void onClickedDelegate (Ref* sender) override;
    void startGame();
    void onNextStage();
    void onNextGame();
    void onFinishGame() override ;
    
    void bg_update(float dt);
    void setGameState(int game_state );
    void showIntroGame();
    void inactiveMode();

    void createBubble(Node* holder, float showImage = 1);
    void gameConfig();
    void showGuideGame(std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    int number_tap = 10;
    std::vector<CaterpillaData::Game> _games ;
    CaterpillaData::Game game;
    
    int _game_state = -1;
    
    int timer = 0;
    
    cocos2d::Size screen_size;
    bool _isAutoPlay;
    int count_game = 0;
    Layout* root_layout;
    Layout* _bg;
    
    ImageView* start_bg;
    ImageView* finish_bg;
    Vec2 finish_pos;
    
    Layout* snail_panel;
    Layout* ant_panel;
    Layout* sau_panel;
    
    GAFWrapper* snail;
    GAFWrapper* ant;
    GAFWrapper* sau;
    
    std::vector<Vec2> positions;
    
    int tap = 0;
    float move_speed = 300;
    float time = 0;
    int run_id;
    
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 300;
    int _audioInactiveMode = -1;
};
CREATE_READER(CaterpillaRacing);

#endif /* CaterpillaRacing_hpp */
