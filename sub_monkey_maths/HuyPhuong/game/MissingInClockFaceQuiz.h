//
//  MissingInClockFaceQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef MissingInClockFaceQuiz_h
#define MissingInClockFaceQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"

class MissingInClockFaceQuiz : public Math_Quiz
{
public:
    static MissingInClockFaceQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    int clock_size;
    bool decimal_number;
    int missing_number = 1;

    std::vector<math::resource::Image*> img_list;
    math::AnalogClock* clock = nullptr;
    
    //callback Func
    void onAnswered(Node* node);
    
    int time_count;
    int hour;
    int minute;
    void clockRun(float dt);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
};
#endif /* MissingInClockFaceQuiz_h */
