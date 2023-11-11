//
//  TimeComeNextQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef TimeComeNextQuiz_h
#define TimeComeNextQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"

class TimeComeNextQuiz : public Math_Quiz
{
public:
    static TimeComeNextQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum TimeType {
        season_of_year=0,
        date_of_week,
        month_of_year
    };
    TimeType time_type;
    
    enum GameType {
        come_next=0,
        come_before,
        between
    };
    GameType game_type;
    
    enum ShowType {
        fully_name=0,
        short_name,
        show_number
    };
    ShowType show_type;
    
    bool only_answer = true;
    int number_object;

    int number_question;
    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> all_images;

    //callback Func
    void onAnswered(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);    

};
#endif /* TimeComeNextQuiz_h */
