//
//  CompareWeightTwoObjectsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef CompareWeightTwoObjectsQuiz_h
#define CompareWeightTwoObjectsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CompareWeightTwoObjectsQuiz : public Math_Quiz
{
public:
    static CompareWeightTwoObjectsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    cocos2d::ui::ImageView* balance = nullptr;
    cocos2d::ui::ImageView* balance_scales = nullptr;
    cocos2d::ui::ImageView* balance_hand = nullptr;

    //Game config
    bool drag_object;
    bool isBig = false;
    
    cocos2d::Size object_size;
    std::string background_key;
    std::string fit_object;

    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;

    
    //callback Func
    void onTouchObject (Node* node);
    void onMoveObjectEnded(Node* node, cocos2d::Vec2 point);
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    int last_index;
    std::vector <Math_Answer*> answer_tested;
    void autoTestingRun(float dt);
};
#endif /* CompareWeightTwoObjectsQuiz_h */
