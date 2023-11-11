//
//  CompleteSymbolsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef CompleteSymbolsQuiz_h
#define CompleteSymbolsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CompleteSymbolsQuiz : public Math_Quiz
{
public:
    static CompleteSymbolsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //callback Func
    void onAnswerDone(Node* node);
    void onTouchObject(Node* node);
    void onMovedObject(Node* node, cocos2d::Vec2 position);
    void onMovedObjectToSlot(Node* node, cocos2d::Vec2 position);
    
    void onTouchIcon(Node* node);
    void onMovedIcon(Node* node, cocos2d::Vec2 position);


    bool is_addition = true;
    
    int number_1;
    int number_2;
    int number_total;
    bool copying = false;
    Math_Object* selected_object = nullptr;
    Math_Object* item_object = nullptr;

    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
    //att
    cocos2d::Size number_size;
    cocos2d::Size object_size;
    cocos2d::Size group_size;

    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> img_operator_list;
};
#endif /* CompleteSymbolsQuiz_h */
