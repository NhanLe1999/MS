//
//  EverydayEventsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef EverydayEventsQuiz_h
#define EverydayEventsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "ClockBase.h"

class EverydayEventsQuiz : public Math_Quiz
{
public:
    static EverydayEventsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum ShowStyle {
        ST_fully_name=0,
        ST_short_name,
        ST_show_number
    };
    ShowStyle show_style;
    
    enum GameType {
        GT_am_pm=0,
        GT_time_range
    };
    GameType game_type;
    
    enum TimeType {
        TT_time_right=0,
        TT_time_half
    };
    TimeType time_type;
    
    std::vector<math::resource::Image*> all_images;
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_time;

    std::vector<math::ClockBase*> clock_list;

    //callback Func
    void onAnswered(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);    

};
#endif /* EverydayEventsQuiz_h */
