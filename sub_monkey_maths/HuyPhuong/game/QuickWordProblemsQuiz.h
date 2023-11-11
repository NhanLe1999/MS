//
//  QuickWordProblemsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef QuickWordProblemsQuiz_h
#define QuickWordProblemsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "ClockBase.h"

class QuickWordProblemsQuiz : public Math_Quiz
{
public:
    static QuickWordProblemsQuiz* createQuiz();
    
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
    
    enum AnswerType {
        AT_number_only=0,
        AT_sentence_only,
        AT_number_and_text
    };
    AnswerType answer_type;
    
    std::vector<math::resource::Image*> img_number;
    std::vector<math::resource::Image*> img_person;
    std::vector<math::resource::Image*> img_thing;
    std::vector<math::resource::Image*> img_unit;
    std::vector<math::resource::Image*> img_time;
    std::vector<math::resource::Image*> img_tobe;

    std::vector<math::resource::Image*> img_answer;

    //callback Func
    void onAnswered(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);    

};
#endif /* QuickWordProblemsQuiz_h */
