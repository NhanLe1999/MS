//
//  Huy_AnswerNumberLine.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef Huy_AnswerNumberLine_h
#define Huy_AnswerNumberLine_h

#include <stdio.h>
#include "Math_Quiz.h"

class Huy_AnswerNumberLine : public Math_Quiz
{
public:
    static Huy_AnswerNumberLine* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum GameType {
        come_next=0,
        come_before,
        between
    };
    GameType game_type;
    
    int number_object;
    int number_question;
    int number_start;
    bool number_line = true;
    cocos2d::Size object_size;
    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> all_images;
    std::vector<math::resource::Image*> img_answer;
    
    //callback Func
    void onAnswered(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
};
#endif /* Huy_AnswerNumberLine_h */

