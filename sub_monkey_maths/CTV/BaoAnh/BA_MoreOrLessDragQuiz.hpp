//
//  BA_MoreOrLessDragQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/14/17.
//
//

#ifndef BA_MoreOrLessDragQuiz_hpp
#define BA_MoreOrLessDragQuiz_hpp

#include <stdio.h>

#include "Math_Quiz.h"


class BA_MoreOrLessDragQuiz : public Math_Quiz{
public:
    static BA_MoreOrLessDragQuiz* createQuiz();
    
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
    
    std::vector<std::string> list_groupname;
    
    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    
    cocos2d::Size object_size;
    cocos2d::Size group_size;
    cocos2d::Size slot_size;
    int value_range;
    
    //lists size
    int number_object;
    int number_kind;
    int number_drager;
    int number_slot;

    //object random numbers
    int number_max, number_min;
    
    //drager values
    int value_min;
    int value_max;
    std::string display_value;
    
    //---------------------------
    std::vector <int> slot_index;
    std::vector <int> drag_values;
    
    int group_cap;
    
    int counting = 1;
    bool display_number;
    std::string type ;
    //int random_img ;

    
    //danh sach doi tuong keo tha
    std::vector<Math_Object*> _results;

    std::vector<Math_Object*> _dragers;
    std::vector<Node*> _slot_object;
    std::vector<int> number_objects;
  
   // std::vector<Math_Object*> _group_list;
    std::vector<int> answer_list;
    
    std::vector<int> value_list;
    
    //
    std::vector<int> list_number_drag; //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    math::resource::Image* drag_bg_img = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(bool correct);
    
    
    //rieng cua game nay
    int result = 0, result2;
    std::vector<cocos2d::Color4B> color_list;
    cocos2d::Color4B result_color;
    Math_Object* example_obj = nullptr;
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    //debugging mode
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
    
};

#endif /* BA_MoreOrLessDragQuiz_hpp */
