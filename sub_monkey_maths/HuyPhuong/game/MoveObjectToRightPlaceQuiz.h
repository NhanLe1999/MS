//
//  MoveObjectToRightPlaceQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef MoveObjectToRightPlaceQuiz_h
#define MoveObjectToRightPlaceQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class MoveObjectToRightPlaceQuiz : public Math_Quiz
{
public:
    static MoveObjectToRightPlaceQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //callback Func
    void onMovedObject(Node* node, cocos2d::Vec2 position);
    void onMoveDone(Node* node);

    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
    //att
    enum QuestionType {
        QT_Left_Right = 0,
        QT_Top_Bottom,
        QT_Above_Below,
        QT_NextTo
    };
    QuestionType question_type;
    int fix_ask_pos = 0;
    int number_object = 3;
    int index_correct = 3;
    int index_pivot=0;
    
    cocos2d::Size object_size;
    cocos2d::Size group_size;
    
    std::vector<math::resource::Image*> img_list;
};
#endif /* MoveObjectToRightPlaceQuiz_h */
