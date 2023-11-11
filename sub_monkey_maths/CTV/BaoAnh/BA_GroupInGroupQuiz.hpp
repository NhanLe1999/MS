//
//  BA_GroupInGroupQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/1/17.
//

#ifndef BA_GroupInGroupQuiz_hpp
#define BA_GroupInGroupQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"


class BA_GroupInGroupQuiz : public Math_Quiz{
public:
    static BA_GroupInGroupQuiz* createQuiz();
    
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
    
    //group config
    std::string outer_group_type;
    int outer_group_column;
    int outer_group_row;
    
    std::string inner_group_type;
    int inner_group_column;
    int inner_group_row;
    
    //size
    cocos2d::Size object_size;
    cocos2d::Size outer_group_size;
    cocos2d::Size inner_group_size;

    //object config
    int number_max, number_min;
    int obj_id=0;

    //lists
    std::vector<std::vector<Math_Group*>> inner_groups;
    
 
    int group_cap;
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_obj_list;
    //danh sach so luong obj cho moi group
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
    void onTouchOnGroup(Node* node);
 
    
    //rieng cua game nay

    //debugging mode
    int testing_step=0;
    //Smart autotesy
    //debugging mode
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
    
};


#endif /* BA_GroupInGroupQuiz_hpp */
