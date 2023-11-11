//
//  CountRowAndDragQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/19/17.
//
//

#ifndef CountRowAndDragQuiz_h
#define CountRowAndDragQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CountRowAndDragQuiz : public Math_Quiz
{
public:
    static CountRowAndDragQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    bool counting;
    bool count_sequence;
    int number_group;
    std::string slot_position;
    bool align_group;
    
    //object config
    cocos2d::Size object_size;
    bool object_value;
    bool dragger_value;
    std::string object_value_position;
    int number_object;
    int number_kind;
    
    //group config
    std::string object_align;
    std::string group_type;
    cocos2d::Size group_size;
    bool group_arrange;
    int group_column;
    int group_row;
    
    //drager config
    int number_drager;
    cocos2d::Size drager_size;
    math::comparision::Key key_compare=math::comparision::number;
    std::vector<math::resource::Image*> img_drager;
    std::vector<int> drag_values;

    
    //icon config
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;

    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    int number_counting=0;
    int count_index=0;
    
    //Danh sach cac doi tuong keo tha.
    std::vector<Math_Object*> _dragers;
    Math_Group* _group_counting = nullptr;
    std::vector<int> _index_counting;
    
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
};

#endif /* CountRowAndDragQuiz_h */
