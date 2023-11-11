//
//  BA_HowManyKindQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 10/8/17.
//

#ifndef BA_HowManyKindQuiz_hpp
#define BA_HowManyKindQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"


class BA_HowManyKindQuiz : public Math_Quiz{
public:
    static BA_HowManyKindQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;
    
private:
    bool init() override;
    
    //Game config
    int number_object_kind;
    int number_object;
    int number_right;
    
    int obj_id =0;
    int count ;
    std::string compare_key;
    std::string display_type;
    int max_in_line;
    int total_object;
    
    std::vector<math::resource::Image*> kind_list;
    std::vector<std::pair<int, std::vector<math::resource::Image*>>> answer_vector;
    
    cocos2d::Size object_size;
    int number_object_max, number_object_min;
    
    
    //lists size
    int number_kind;
    
    int autoCountList =0;
    math::comparision::Key sort_key;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    std::vector<int> answer_list;
    
    std::vector<std::vector<Math_Object*>> object_list;
    std::vector<std::pair<int, std::vector <int>>> answers_lists;
    //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    
    //callback Func
    void onTouchedAnswer(cocos2d::Node* node);
    bool clickable = true;
    
    //rieng cua game nay
    std::vector<Math_Answer*> answers;
    std::vector<Math_Object*> passed_obj;
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};
#endif /* BA_HowManyKindQuiz_hpp */
