//
//  MonsterTeeth.h
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/10/18.
//

#ifndef MonsterTeeth_hpp
#define MonsterTeeth_hpp

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
#include "MonsterTeeth_Item.h"


USING_NS_CC;
using namespace ui;

class MonsterTeeth : public MJMainGBase {
public:
    CREATE_FUNC(MonsterTeeth);
    static MonsterTeeth* createGame(std::string json_file, bool isAutoPlay);

private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);

    void loadFrame();
    void onEnter() override;
    void onExit() override;
    void onTouchedItem(Ref* sender, ui::Widget::TouchEventType type);
    void showGame();
    void onNextStage();
    void onNextGame();
    void onFinishGame();
    void update(float dt) override;

    void gameConfig();
    void playEffect(Node* holder, Vec2 pos, int color = -1);

    void showGuideGame(std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void inActiveMode(float dt);
    void activeMode();
    bool isPhone();

    //game data
    int _gamemode;
    std::vector<mj::PHONIC_WORD> words;
    mj::PHONIC_WORD word;

    //game config
    Vec2 _brush_pos, _water_pos, _table_pos, _circle_pos, _khay_pos;
    float _tool_scale = 1, _tool_rotate = 0, _table_rotate = 1, _monster_scale = 1;

    cocos2d::Size screen_size;
    bool _isAutoPlay;
    int count_game = 0;
    Layout* root_layout;
    ImageView* _bg;
    ImageView* _mouth;
    ImageView* _brush;
    ImageView* _water;
    Vec2 origin_pos[2];
    int tool_index = -1;

    Vec2 last_pos;
    Vec2 current_pos;
    cocos2d::Size bg_size;

    int delta_opacity = 60;
    int number_clean = 0;

    int count = 0;

    std::string tool_sound;
    bool tool_sound_isOn = false;
    int tool_sound_id = -1;

    std::vector<MonsterTeeth_Item*> tooth_item;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 150;

    GAFWrapper* tool_gaf;

    std::function<void()> _onNextStageCallback = nullptr;
};
CREATE_READER(MonsterTeeth);
#endif /* MonsterTeeth_hpp */
