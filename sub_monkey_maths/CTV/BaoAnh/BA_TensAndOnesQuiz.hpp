//
//  BA_TensAndOnesQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/2/17.
//

#ifndef BA_TensAndOnesQuiz_hpp
#define BA_TensAndOnesQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"

class BA_TensAndOnesQuiz : public Math_Quiz{
public:
    static BA_TensAndOnesQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    int total_object;
   // int number_object;
    bool answer_checked;
    
    //group config
 //   std::string outer_group_type;
    int column;
    int row;
    
    std::vector<Math_Object*> base_list;
    std::vector<Math_Object*> oprt_list;
    std::vector<Math_Object*> _dragers;
    std::vector<Math_Object*> expression;
    std::vector<Math_Object*> blocks_object;
    std::vector<Math_Object*> ones_object;
    
    //size
    cocos2d::Size object_img_size;
    cocos2d::Size object_size;

    
    //object config
    int number_max, number_min;
    int obj_id=0;
   // std::vector<int> drag_value;
    std::vector<int> number_drag;
    
    math::resource::Image* img_bg;
    math::resource::Image* drag_img;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_obj_list;
    
    std::vector<math::resource::Image*> number_list;
    //danh sach so luong obj cho moi group
   // std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
//    void onTouchedObject(cocos2d::Node* node, cocos2d::Vec2 point);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(bool correct);
    
    //rieng cua game nay
    Math_Object* result;
    std::vector<Math_Slot*> drag_slots;
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    //debugging mode
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch;
    void autoTestingRun(float dt);
    
};
#endif /* BA_TensAndOnesQuiz_hpp */
