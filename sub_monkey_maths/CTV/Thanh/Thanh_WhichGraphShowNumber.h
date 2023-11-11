//
//  Thanh_WhichGraphShowNumber.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_WhichGraphShowNumber_h
#define Thanh_WhichGraphShowNumber_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_WhichGraphShowNumber : public Math_Quiz
{
public:
    static Thanh_WhichGraphShowNumber* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    void onTouchedGroup(cocos2d::Node* node);
private:
    bool init() override;

    // ---------------
    // Thanh_WhichGraphShowNumber config
    int number_type_object;
    bool touch_off;

    // ---------------
    // group
    math::group::GroupType type_group;
    cocos2d::Size element_size;
    cocos2d::Size group_size;
    math::resource::Image *background = nullptr;

    // ----------------
    // Question config
    

    //danh sach image cho moi loai obj
    std::vector<std::vector<std::pair<math::resource::Image*, int> > > icons_group;
    std::vector<std::pair<math::resource::Image*, int> > icons_result;
    
    // danh sach cac object, group
    std::vector<Math_Group*> groups;

    // result
    Math_Group* result = nullptr;

    //callback Func
    void onAnswerDone(bool correct);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    // button
    Math_Answer* btn_done = nullptr;
};

#endif /* Thanh_WhichGraphShowNumber_h */
