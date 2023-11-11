//
//  Thanh_ReadClockAndWriteTime.hpp
//  MJQuiz
//
//  Created by Daniel on 12/12/17.
//
//

#ifndef Thanh_ReadClockAndWriteTime_hpp
#define Thanh_ReadClockAndWriteTime_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"

class Thanh_ReadClockAndWriteTime : public Math_Quiz
{
public:
    enum GameMode{
        minute,
        hour
    }; 

    static Thanh_ReadClockAndWriteTime* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;


private:
    bool init() override;
 
    // ---------------
    // clock config
    int _radius;
    math::resource::Image* _background = nullptr;
    math::AnalogClock* _analogClock = nullptr;
    int _hour, _minute;
	bool _grade_1_or_lower;
	bool number_text;

    // ---------------
    // answer config
    int _number_answer; 

    // ----------------
    // Question config
    

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> _icons;

    Math_Object* _object_be_touch = nullptr;

    //callback Func
    void onAnswerDone(cocos2d::Node* node);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    void autoTestingRun(float dt);

    // button
    Math_Answer* btn_done;
    std::vector<Math_Answer*> _list_answer;
    Math_Answer* _correct_answer = nullptr;
    std::vector<std::pair<int, int> > _value_answers;

    // in game
    bool _disable; // disable touch
};
#endif /* Thanh_ReadClockAndWriteTime_hpp */
