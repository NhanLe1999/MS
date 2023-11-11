//
//  ChooseCorrectGroupQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/25/17.
//
//

#ifndef ChooseCorrectGroupQuiz_h
#define ChooseCorrectGroupQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class ChooseCorrectGroupQuiz : public Math_Quiz
{
public:
    static ChooseCorrectGroupQuiz* createQuiz();
    
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
    int number_group;
    bool reorder;
    bool align;
    std::string request;
    std::string relation;
    std::string game_type;
    int quantity;
    
    std::string fit_object;
    math::comparision::Key compare_key;
    
    cocos2d::Size object_size;
    std::vector<std::string> list_groupname;
    int kind_in_group;
    std::string sort_type;
    bool object_arrange;
    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    cocos2d::Size group_size;
    
    int number_object;
    int group_cap;
    
    Math_Object* selected_object = nullptr;
    
    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;

    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    //callback Func
    void onTouchedGroup(Node* node);
    
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    void autoTestingRun(float dt);
};

#endif /* ChooseCorrectGroupQuiz_h */
