//
//  ChooseObjectAtPositionQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef ChooseObjectAtPositionQuiz_h
#define ChooseObjectAtPositionQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class ChooseObjectAtPositionQuiz : public Math_Quiz
{
public:
    static ChooseObjectAtPositionQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //callback Func
    void onTouchObject(Node* node);
    void onTouchObjectColor(Node* node);
    
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
    bool coloring = false;
    
    bool random_icon = true;
    int number_object;
    int fix_ask_pos = 0;
    
    cocos2d::Size object_size;
    cocos2d::Size group_size;
    
    std::string color_selected = "white";
    std::string color_correct = "";

    std::vector<std::string> color_list;
    
    std::vector<Math_Object*> next_to_selected;
    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_color;

};
#endif /* ChooseObjectAtPositionQuiz_h */
