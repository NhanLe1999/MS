//
//  BA_ReGroupQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/2/17.
//

#ifndef BA_ReGroupQuiz_hpp
#define BA_ReGroupQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"

class BA_ReGroupQuiz : public Math_Quiz{
public:
    static BA_ReGroupQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    int number_group;
    int number_object;
    int number_slot;
    
    
    //size
    cocos2d::Size object_size;
    cocos2d::Size slot_size;

    //object config
    int number_max, number_min;
    int obj_id=0;
    
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_obj_list;
    //danh sach so luong obj cho moi group
    std::vector<math::resource::Image*> number_list;
    math::resource::Image* drag_bg = nullptr;
    //danh sach size cho tung doi tuong
   // std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(bool correct);
    
    //rieng cua game nay
    std::vector<Math_Object*> _dragers;
    std::vector<Math_Slot*> drag_slots;
    std::vector<Math_Object*> expr_base;
    std::vector<Math_Object*> expr_oprt;
    std::vector<int> number ;
    std::vector<int> number_reverse;
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    //debugging mode
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
    
};
#endif /* BA_ReGroupQuiz_hpp */
