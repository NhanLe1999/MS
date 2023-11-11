//
//  ChooseCorrectClockQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef ChooseCorrectClockQuiz_h
#define ChooseCorrectClockQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"
#include "DigitalClock.h"

class ChooseCorrectClockQuiz : public Math_Quiz
{
public:
    static ChooseCorrectClockQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum GameType {
        GT_hour_hand_point = 0,
        GT_minute_hand_point,
        GT_compare_time,
        GT_compare_clock,
        GT_different_time
    };
    
    enum TimeType {
        TT_time_right = 0,
        TT_time_half,
        TT_random
    };
    
    enum ShowTimeType {
        ST_time = 0,
        ST_text,
        ST_semi_text
    };
    
    GameType game_type = GT_hour_hand_point;
    TimeType time_type = TT_time_right;
    ShowTimeType show_time = ST_time;

    int number_clock = 2;
    bool is_analog = true;
    bool same_style = true;
    int clock_size = 200;
    bool analog_decimal = true;
    bool mode_24 = true;
    
    int cmp_hour;
    int cmp_minute;
    
    std::vector<std::pair<int, int>> time_list;

    std::vector<math::resource::Image*> img_list;
    std::vector<math::ClockBase*> clock_list;
    math::ClockBase* clock_cmp;
    
    //callback Func
    void onTouchObject(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
};
#endif /* ChooseCorrectClockQuiz_h */
