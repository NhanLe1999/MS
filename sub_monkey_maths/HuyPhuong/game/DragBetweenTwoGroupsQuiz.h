//
//  DragBetweenTwoGroupsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/24/17.
//
//

#ifndef DragBetweenTwoGroupsQuiz_h
#define DragBetweenTwoGroupsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class DragBetweenTwoGroupsQuiz : public Math_Quiz
{
public:
    static DragBetweenTwoGroupsQuiz* createQuiz();
    
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
    bool reorder;
    bool align;
    std::string request;
    std::string relation;
    int quantity;
    
    enum DragType {
        make_equal,
        make_more,
        make_less,
        make_at_least,
        make_equal_less,
        by_quantity,
        compare
    };
    DragType drag_type;
    
    cocos2d::Size object_size;
    std::vector<std::string> list_groupname;
    std::vector<math::resource::Image*> group_name_img;
    std::vector<int> array_range;
    bool object_arrange;
    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    cocos2d::Size group_size;
    
    math::comparision::Key compare_key = math::comparision::exactly;
    
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
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    void autoTestingRun(float dt);
};

#endif /* DragBetweenTwoGroupsQuiz_h */
