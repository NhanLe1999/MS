//
//  BA_ColoredBrickGraphQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 10/9/17.
//

#ifndef BA_ColoredBrickGraphQuiz_hpp
#define BA_ColoredBrickGraphQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"


class BA_ColoredBrickGraphQuiz : public Math_Quiz{
public:
    static BA_ColoredBrickGraphQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    //Game config
    int number_object_kind;
    int number_object;
    int number_right;
    
    int obj_id =0;
    int count ;
    std::string compare_key;
    
    std::vector<math::resource::Image*> img_quantity;
    std::vector<std::string> key ;
    
    
    cocos2d::Size object_size;
    int number_object_max, number_object_min;
    
    std::vector<std::string> list_groupname;
    
    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    int group_cap;
    
    cocos2d::Size group_size;
    
    bool display_mode;
    
    //lists size
    int number_kind;
    
   // int autoCountList =0;
   // math::comparision::Key sort_key;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<std::pair<int, int>> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    std::vector<std::string> answer_list;
    std::vector<int> answers_list;
    
    std::vector<std::vector<Math_Object*>> object_list;
    
    std::vector<std::pair<int, std::vector<int>>> answer_vector;
    std::vector<std::pair<int, std::vector<Math_Answer*>>> answer_btn_vector;
    //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    // std::vector<math::common::QuestionData*> list_question;
    
    std::vector<int> index_question;
    
    //callback Func
    void onTouchedAnswer(cocos2d::Node* node);
    
    
    //rieng cua game nay
    std::vector<Math_Answer*> answers;
    bool clickable = true;
    std::string right_color;
    std::vector<std::pair<math::resource::Image*,  int>> img_number_list;
    
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};

#endif /* BA_ColoredBrickGraphQuiz_hpp */
