//
//  MoveNumberObjectToGroupQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#ifndef MoveNumberObjectToGroupQuiz_h
#define MoveNumberObjectToGroupQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class MoveNumberObjectToGroupQuiz : public Math_Quiz
{
public:
    static MoveNumberObjectToGroupQuiz* createQuiz();
    
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
    int total_number;
    bool copy_object;
    bool check_move;
    bool drag_allinone;
    bool copying = true;
    bool use_first = false;
    
    int number_kind;
    cocos2d::Size object_size;
    bool names_enable;
    std::string group_name_type;
    std::vector<math::resource::Image*> list_groupname;

    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    cocos2d::Size group_size;
    
    int number_object;
    int group_cap;
    
    std::vector<int> list_number_drag;
    std::vector<int> number_list;
    
    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> group_name_img;

    //danh sach so luong cho moi loai obj
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
   
    Math_Object* selected_object=NULL;

    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    void audioEnding(float dt);
    
    bool clickable=true;
    int index_group = 0;
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    void autoTestingRun(float dt);
};

#endif /* MoveNumberObjectToGroupQuiz_h */
