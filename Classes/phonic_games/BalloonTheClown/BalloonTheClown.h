//
//  BalloonTheClown.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/24/18.
//

#ifndef BalloonTheClown_hpp
#define BalloonTheClown_hpp

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "MJHelper.h"
#include "GameData.h"
#include "Math_Func.h"
#include <stdio.h>
#include "QuizGuideCenter.h"
USING_NS_CC;
using namespace ui;

class BalloonTheClown : public MJMainGBase {
public:
    CREATE_FUNC(BalloonTheClown);
    static BalloonTheClown * createGame(std::string json_file, bool isAutoPlay);
    
private:
    void didLoadFromCSB(std::string json_file, bool isAutoPlay);
    void loadFrame();
    void onEnter() override ;
    void onExit() override;
    void onStartGame();
    void onTransition();
    void onNextGame();
    void onNextStage();
    void onFinishGame() ;

    void showGuideGame(std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void showIntroGame();
    
    void onClickedAnswer (Ref* sender);
    void onClickedSpeaker(Ref* sender);

    void learningReminder();
    std::string _learningReminderScheduleName = "Learning Reminder";
    int _learingReminderCountDown = 0;
    math::SequenceAudio* _seq = new math::SequenceAudio();
    void stopSequenceAudio(math::SequenceAudio* param_seq);
    cocos2d::Sequence* _testSequence = nullptr;
    cocos2d::Sequence* _testSequenceForAudioButton = nullptr;
    bool _touchAbleSpeaker = false;
    
    std::vector<std::vector<int>> list_index;
    void playPhonicText(Text* text);
    void playSlowText(Text* text);
    void playNormalText(Text* text);

    void throwBall(bool correct, bool clown_effect = true);
    void clownEffect(bool correct);
    void gameConfig();
    
    cocos2d::Size screen_size;
    bool _isAutoPlay;
    int count_game = 0;
    int max_game = 1; //configurable
    
    struct DataInfo{
        //main obj
        std::string text_khong_dau;
        std::string audio_khong_dau;
        std::string text_dau;
        std::string audio_dau;
        bool co_dau = false;
        int pos_dau = 0;
        std::string text;
        std::string audio;
        std::string s_audio;
        std::vector<std::string> p_audio;
        std::vector<std::string> phonics;

        //missing position
        std::vector<int> position;

        //right obj
        mj::GameObject right_answer;
        std::string right_sub_text;
        //list answer
        std::vector<mj::GameObject> answers;
    };
    std::vector<DataInfo> words;
    DataInfo word;
    
    //audio id
    int bg_id = -1;
    int q_id = -1;
    int p_id = -1;
    int w_id = -1;

    

    Layout* root_layout;
    ImageView* _bg;
    std::vector<ImageView*> answers;
    Layout* fullText;
    GAFWrapper* cat;
    GAFWrapper* goal;
    GAFWrapper* clown;
    bool checkRomaji(char c);
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 120;
    
};
CREATE_READER(BalloonTheClown);

#endif /* BalloonTheClown_hpp */
