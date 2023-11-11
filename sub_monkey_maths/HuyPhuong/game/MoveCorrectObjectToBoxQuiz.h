//
//  MoveCorrectObjectToBoxQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef MoveCorrectObjectToBoxQuiz_h
#define MoveCorrectObjectToBoxQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


class MoveCorrectObjectToBoxQuiz : public Math_Quiz
{
public:
    static MoveCorrectObjectToBoxQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    math::comparision::Key compare_key;
    bool had_temp;
    bool show_temp;
    
    std::string icon_filter;
    std::string icon_same = "";
    
    cocos2d::Size object_size;
    std::string fit_object;
    int number_object;
    int number_kind;
    std::string show_icon = "icon";
    
    std::vector<std::string> question_req;
    bool ordinal_req = false;
    int ordinal_number;
    bool different_req = false;
    bool isBigger;
    
    //display
    std::string display_type;
    cocos2d::Size max_object_size;
    
    //background image
    math::resource::Image*img_bg;
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;

    std::string dif_type;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    cocos2d::DrawNode* drawer = nullptr;
    int move_aligned=0;
    
    Math_Object* temp_object=nullptr;
    Math_Object* selected_object=nullptr;
    
    //callback Func
    void onTouchedObject(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    void autoTestingRun(float dt);
    
};


#endif /* MoveCorrectObjectToBoxQuiz_h */
