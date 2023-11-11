//
//  ChooseCorrectObjectsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef ChooseCorrectObjectsQuiz_h
#define ChooseCorrectObjectsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class ChooseCorrectObjectsQuiz : public Math_Quiz
{
public:
    static ChooseCorrectObjectsQuiz* createQuiz();
    
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
    bool basic_type;
    math::comparision::Key compare_key;
    bool size_request;
    bool had_temp;
    bool show_temp;
    
    std::string icon_filter;
    std::string icon_same = "";

    bool move_align;
    bool align;
    
    cocos2d::Size object_size;
    std::string fit_object;
    int number_object;
    int number_kind;
    std::string show_icon = "icon";
    
    std::vector<std::string> question_req;
    bool same_req = false;
    int number_same;
    bool ordinal_req = false;
    int ordinal_number;
    bool different_req = false;
    bool isBigger;
    
    //display
    std::string display_type;
    int max_in_line;
    
    int move_align_target;
    
    //background image
    math::resource::Image*img_bg = nullptr;
    std::vector<std::string> color_list;

    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;

    std::string dif_type;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    cocos2d::ui::ImageView* line = nullptr;
    int move_aligned=0;
    
    std::vector<Math_Object*> answer_list;
    Math_Object* temp_object=NULL;
    Math_Object* selected_object=NULL;
    
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectBegan(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    
    bool checkObject(Math_Object* object);
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    void autoTestingRun(float dt);
};


#endif /* ChooseCorrectObjectsQuiz_h */
