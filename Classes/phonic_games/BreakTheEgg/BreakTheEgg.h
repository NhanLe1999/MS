//
//  BreakTheEgg.h
//  Created by HoiSung on 02/02/18.
//
//

#ifndef MJ_BreakTheEgg_h
#define MJ_BreakTheEgg_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "BreakTheEgg_EggItem.h"
#include "PhonicInfoModel.h"
#include "BreakTheEgg_Board.h"
using namespace cocos2d;
class BreakTheEgg : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public BreakTheEgg_EggItemDelegate, public BreakTheEgg_BoardDelegate
{
public:
    CREATE_FUNC(BreakTheEgg);
    static BreakTheEgg *createBreakTheEgg(std::string json_file, std::string game_type = GAME_TYPE_PHONIC);
    static BreakTheEgg *createBreakTheEgg(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play, std::string game_type = GAME_TYPE_PHONIC);
    
private:
    void didLoadFromCSB(std::string json_file);
    void didLoadFromCSB(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play);
    std::vector<mj::PHONIC_INFO> _list_phonic_info;
    bool is_auto_play;
    void generateData();
    void showInformationGame();
    void showTutorial();
    void onEnter() override;
    void onExitTransitionDidStart() override;
    cocos2d::Vec2 touch_pos;
    cocos2d::Size screen_size;
    void loadFrame();
    int egg_state = -1;
	std::string m_json_config;
    Button* _speaker;
    BreakTheEgg_EggItem *current_egg = nullptr;
    void onClickedEggDelegate(cocos2d::Ref *sender) override;
    void onChangeEggStateDelegate(int state) override;
    std::vector<BreakTheEgg_EggItem *> list_egg;
    int local_zorder = 2;
    void onBoardStartHide() override;
    int count_phonic_done = 0;
    void onFinishGame() ;
    BreakTheEgg_Board *board;
    void radio();
    void showIntroGame();
    cocos2d::Vec2 pos_guide;
    std::vector<math::resource::Image*> m_images;
};
CREATE_READER(BreakTheEgg);
#endif /* MJ_BreakTheEgg_h */
