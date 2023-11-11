//
//  MatchingTimeQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef MatchingTimeQuiz_h
#define MatchingTimeQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "ClockBase.h"

class MatchingTimeQuiz : public Math_Quiz
{
public:
    static MatchingTimeQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum GameType {
        GT_Name_picture = 0,
        GT_Time_Clock,
        GT_Clock_Clock,
        GT_Picture_Picture
    };
    GameType game_type;
    
    enum ShowType {
        ST_Text=0,
        ST_Semi_Text,
        ST_Number
    };
    ShowType show_type;
    
    int number_pair;
    
    bool time_24 = true;
    bool is_same_clock = false;
    bool is_analog = true;
    bool is_time_right = true;
    bool is_time_game = true;

    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::ClockBase*> clock_list;
    std::vector<math::ClockBase*> clock_drag_list;

    //callback Func
    void onTouchedObject(Node* node);
    void onMovedObject(Node* node, cocos2d::Vec2 position);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);    

};
#endif /* MatchingTimeQuiz_h */
