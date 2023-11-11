//
//  FootballKick.h
//

#ifndef FootballKick_h
#define FootballKick_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "PhonicInfoModel.h"
using namespace cocos2d;
class FootballKick : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(FootballKick);
    static FootballKick *createGame(std::string json_file);
    
private:
    void didLoadFromCSB(std::string json_file);
    void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void loadFrames();
    void showIntroGame();
    cocos2d::Vec2 pos_guide;
    void onSpeakerClicked(cocos2d::Ref *sender);
    cocos2d::ui::Button *speaker;
    std::string speaker_audio_path;
    void showImage();
    GAFWrapper *ball_gaf, *bg_gaf, *fox_gaf, *goal_gaf, *score_gaf, *gamename_gaf, *shooteffect_gaf;
    std::vector<cocos2d::ui::ImageView *> list_ball, list_score;
    void addBall();
    void showBall();
    void hideBall();
    cocos2d::ui::Layout *ball_layout;
    void onBall(cocos2d::Ref *sender);
    void inactiveMode();
    void unscheduleInactiveMode();
    struct FOOTBALLKICK_INFO {
        std::string text;
        std::string audio_path_n;
        std::string audio_path_p;
        std::vector<std::pair<std::string,std::string>> list_distracting;
    };
    
    std::vector<FOOTBALLKICK_INFO> list_item_info;
    FOOTBALLKICK_INFO current_item;
    std::string text_current;
    int index_item = 0;
    void runFox();
    void runFoxRepeat(float duration, int type, float pos_right, float pos_left);
    void runScore(int type_goal);
    std::vector<GAFWrapper *> list_score_gaf;
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onNextTurn();
    void disableTouchBall();
    void enableTouchBall();
    int audio_id_p = -1;
    int index_correct = -1;
    cocos2d::ui::ImageView *shoot_effect;
    std::string question_vi;
    int _audioTapOn = -1;
    int _audioInactiveMode1 = -1;
    int _audioInactiveMode2 = -1;
    
};
CREATE_READER(FootballKick);
#endif /* MJ_FootballKick_h */
