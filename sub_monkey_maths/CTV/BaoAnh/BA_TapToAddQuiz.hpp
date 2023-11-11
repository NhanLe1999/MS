//
//  BA_TapToAdd.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/18/17.
//
//

#ifndef BA_TapToAddQuiz_hpp
#define BA_TapToAddQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"

class BA_TapToAddQuiz : public Math_Quiz{
public:
    static BA_TapToAddQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    int number_object;
    int total_object;
    int standard_object;
    
    int obj_id =0;
    
    bool game_mode;
    cocos2d::Size object_size;
    
    
    int number_object_max, number_object_min;//value_object_max, value_object_min;
//    int range_max, range_min;
    
    //lists size
    int number_kind;
    std::string display_type;
    int max_in_line;
    // int autoCountList =0;
    
    std::vector<std::vector<Math_Object*>> object_list;
    std::vector<Math_Object*> extra_object;
    //  std::vector<Math_Object*> standard_objects;
    std::vector<std::pair<int, std::vector <int>>> answers_lists;
    //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    
    //    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
    void onAnswerDone();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
    void onTouchedObject(Node* node);
    
    //rieng cua game nay
    Math_Object* line = nullptr;

    void setLinePos(bool go);
    cocos2d::Vec2 line_pos;
    void setObjectPos(std::vector<Math_Object*> object_list, float pos_y, float t);
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};


#endif /* BA_TapToAdd_hpp */

