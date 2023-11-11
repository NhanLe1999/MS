//
//  SumaryWordProblemsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef SumaryWordProblemsQuiz_h
#define SumaryWordProblemsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "ClockBase.h"

class SumaryWordProblemsQuiz : public Math_Quiz
{
public:
    static SumaryWordProblemsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;

    bool is_onething = true;
    bool is_addition = true;
    bool is_time_ask = false;
    int multiple = 1;
    
    enum DragDropType {
        DT_result=0,
        DT_operator,
        DT_all_number
    };
    DragDropType dragdrop_type;
    cocos2d::Size object_size;
    
    std::vector<math::resource::Image*> img_number;
    std::vector<math::resource::Image*> img_person;
    std::vector<math::resource::Image*> img_thing;
    std::vector<math::resource::Image*> img_unit;
    std::vector<math::resource::Image*> img_time;
    std::vector<math::resource::Image*> img_tobe;

    std::vector<math::resource::Image*> img_answer;

    //callback Func
    void onAnswerDone(Node* node);
    void onTouchObjectBegan(Node* node);
    void onTouchObjectEnded(Node* node, cocos2d::Vec2 position);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);    

};
#endif /* SumaryWordProblemsQuiz_h */
