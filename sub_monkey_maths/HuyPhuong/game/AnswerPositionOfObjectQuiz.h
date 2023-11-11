//
//  AnswerPositionOfObjectQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef AnswerPositionOfObjectQuiz_h
#define AnswerPositionOfObjectQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class AnswerPositionOfObjectQuiz : public Math_Quiz
{
public:
    static AnswerPositionOfObjectQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //callback Func
    void onAnswered(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
    //att
    enum QuestionType {
        QT_Left_Right = 0,
        QT_Top_Bottom,
        QT_Above_Below,
        QT_NextTo,
        QT_UsePicture
    };
    QuestionType question_type;
    
    int number_object;
    int fix_ask_pos = 0;
    
    cocos2d::Size object_size;
    cocos2d::Size group_size;
    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_answer;

    
};
#endif /* AnswerPositionOfObjectQuiz_h */
