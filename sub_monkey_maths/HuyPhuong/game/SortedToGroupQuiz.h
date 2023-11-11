//
//  SortedToGroupQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#ifndef SortedToGroupQuiz_h
#define SortedToGroupQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class SortedToGroupQuiz : public Math_Quiz
{
public:
    static SortedToGroupQuiz* createQuiz();
    
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
    std::string sort_by;
    std::string sorted;
    std::string align_group_object;
    std::string remain_object;
    std::string position_name;
    bool dinamic_match;
    bool only_correct_move;
    
    cocos2d::Size object_size;
    std::vector<std::string> list_groupname;

    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    cocos2d::Size group_size;
    
    int number_object;
    int number_kind;
    
    int group_cap;
    float min_size;
    
    math::comparision::Key sort_key;
    std::vector<int> list_number_drag; //Danh cho game keo tha so luong
    
    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    
    Math_Object* selected_object=nullptr;    
    
    void onSortedObjects();
    void sortedToGroup (Math_Object* object, Math_Group* group);
    
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

#endif /* SortedToGroupQuiz_h */
